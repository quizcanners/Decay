#define RAGANIM_GOTOCOORD     0x00000001
#define RAGANIM_GOTOLIMB      0x00000002
#define RAGANIM_GOTOEXTERNAL  0x00000004
#define RAGANIM_ChangeAngle   0x00000100

#define LIMB_Unused           0x01
#define LIMB_Occupied         0x02
#define LIMB_PreOp            0x04 // Before making it disappear make sure it had one more cycle to loose all connections
#define LIMB_Dismembered      0x08


const int defaultAnimationCenterHeight=12;
const int testPCS=50;
int AnimationAccuracy=6;
int AnimationSmoothingVar=0;
int AssignedTestPcs=0 ;

struct LimbConnectionData{
int connected; 
int animation;  // to type 
float defdist;    // to type 
float lockAngle;  // to type
float lockSize;  // to type
float DisplayAngle; 
};

struct AnyRagLimb{ 
phisicalMe force;
worldcoordinates pos;
LimbConnectionData* LinkData;
int hp;
Uint16 connectedNo;//nextPiece;   // downscale HP and I if need to add something
float dist;  // Can be made dynamic
float angle; 
Uint8 G8piece;  // to type
Uint8 G8type;  // to type
Uint8 flags; 
//bool symetry; // to type  
//bool connection;
//not doing to type will let you dinamically create limbs;
} test[testPCS];
const int what=sizeof(AnyRagLimb);


AnyRagLimb LIMBS[numberOfRagLimbs];


struct usableAnimationFrame{
int distance;
int power;
int h;
float angle;
// more to be added: enforce one or two sided interaction with outside object
// force of animation
};

struct convertedAnimation{
usableAnimationFrame* frame;
int frames;
int limbs;
float animationTime;
float TimePerFrame;
bool loaded;
};

struct limbFrame{
Uint32 flags; 
int x,y,h;
int InternalLimb;
int ExternalLimb;
int power;
float angle;
};

const int maxAnimFrames=64;
const int MaxAnimationLimbs=32;


struct AnimationEditor{
convertedAnimation usableTestingAnimation;
TunableDisplayValue AnimationTime;
TunableDisplayValue frames;
TunableDisplayValue CurrentFrame;
TunableDisplayValue limbs;
TunableDisplayValue curLimb;
TunableDisplayValue AnimatorPower;
TunableDisplayValue AnimatorDensity;
limbFrame FrameLimb[maxAnimFrames][MaxAnimationLimbs]; // 128 - number of frames, 32 - number of limbs;
double playingTime;
double IngameplayingTime;
bool local[MaxAnimationLimbs];
bool AllignBefore, AllignAfter;
bool playing;
bool animatedLastFrame; // To avoid rewriting last frame
} animEd;



struct RagDoll {
LimbConnectionData limbDTA[maxLimbs];
AnyRagLimb limb[maxLimbs];
TunableDisplayValue mass[maxLimbs];
Uint8 LimbCount;
}RagTemplate[DollGroups][DollsInGroup];

phisicalMe mouseforce;
bool testinited=0;
worldcoordinates MousePos;

void LoadRagDoll(int Group, int Doll){

std::stringstream ss;
std::string strx;
ss<<"RagDolls/GR"<<Group<<"D"<<Doll<<".mgv";
ss>>strx;
ifstream file (strx, ios::binary);	
RagDoll* doll = &RagTemplate[Group][Doll];
file.read ((char *)&doll->LimbCount, 1);
file.read((char *)&doll->limb[0], sizeof(AnyRagLimb)*doll->LimbCount);
file.read((char *)&doll->limbDTA[0], sizeof(LimbConnectionData)*doll->LimbCount);
for (int i=0; i<maxLimbs; i++){
	file.read ((char *)&doll->mass[i].val, 2);
	doll->limb[i].LinkData=&RagTemplate[Group][Doll].limbDTA[i];
}
file.close (); 
}

void StoreRagDoll(int Group, int Doll){

std::stringstream ss;
std::string strx;
ss<<"RagDolls/GR"<<Group<<"D"<<Doll<<".mgv";
ss>>strx;
ofstream file (strx, ios::binary);	
RagDoll* doll = &RagTemplate[Group][Doll];
file.write ((char *)&doll->LimbCount, 1);
file.write((char *)&doll->limb[0], sizeof(AnyRagLimb)*doll->LimbCount);
file.write((char *)&doll->limbDTA[0], sizeof(LimbConnectionData)*doll->LimbCount);
for (int i=0; i<maxLimbs; i++)
	file.write ((char *)&doll->mass[i].val, 2);
file.close (); 
}

void LoadAnimation(int no){
std::stringstream ss;
std::string strx;
ss<<"RagDolls/Anim"<<no<<".mgv";
ss>>strx;
ifstream file (strx, ios::binary);	
file.read ((char *)&animEd.AnimationTime.val, 2);
file.read ((char *)&animEd.frames.val, 2);
file.read ((char *)&animEd.CurrentFrame.val, 2);
file.read ((char *)&animEd.limbs.val, 2);
file.read ((char *)&animEd.curLimb.val, 2);
file.read ((char *)&animEd.local[0], sizeof(animEd.local));
file.read ((char *) &animEd.FrameLimb, sizeof(limbFrame)*32*animEd.frames.val);
file.close (); 
}

void StoreAnimation(int no){
std::stringstream ss;
std::string strx;
ss<<"RagDolls/Anim"<<no<<".mgv";
ss>>strx;
ofstream file (strx, ios::binary);	
file.write ((char *)&animEd.AnimationTime.val, 2);
file.write ((char *)&animEd.frames.val, 2);
file.write ((char *)&animEd.CurrentFrame.val, 2);
file.write ((char *)&animEd.limbs.val, 2);
file.write ((char *)&animEd.curLimb.val, 2);
file.write ((char *)&animEd.local[0], sizeof(animEd.local));
file.write((char *) &animEd.FrameLimb, sizeof(limbFrame)*32*animEd.frames.val);
file.close (); 
}

const float DismemberHealth=50;

void dismember(AnyRagLimb* limb){
limb->flags|=LIMB_Dismembered;
limb->hp=DismemberHealth;
limb->force.pr=0.8;
//limb->connectedNo=0;
//limb->force.mass=100;
limb->force.speedx/=128;
limb->force.speedy/=128;
limb->force.speedh/=128;
}

void Animation(convertedAnimation* anim , Uint16* LimbNs, int LimbCount, float Time){

	if (!anim->loaded) return;

	if (Time> anim->animationTime) Time=anim->animationTime-0.1; // to avoid reading unexisting frame
	int frame=anim->frames*Time/anim->animationTime;
	float movepercantage=(GameSpeed*AnimationModifier)/(anim->TimePerFrame-(Time-frame*anim->TimePerFrame)); 
	movepercantage=min(0.5,movepercantage); // must be, for percentage can be>1
	AnyRagLimb* CenterLimb=&LIMBS[LimbNs[0]];
	usableAnimationFrame* frameLimbs=anim->frame+frame*anim->limbs; 
	for (int i=1; i< LimbCount; i++)
		if (LimbNs[i]) {
	AnyRagLimb* limb=&LIMBS[LimbNs[i]];

	if ((limb->LinkData->animation==4) || (limb->LinkData->animation==5)){
		worldcoordinates* tmp=(limb->LinkData->animation==4) ? &walker->legs.lleg : &walker->legs.rleg;
	//memcpy(&limb->pos, &walker->legs.rleg, sizeof(worldcoordinates));
		int xdif=(tmp->tx-limb->pos.tx)*tyleSize*accuracy + (tmp->x-limb->pos.x)-limb->force.speedx;
		int ydif=(tmp->ty-limb->pos.ty)*tyleSize*accuracy + (tmp->y-limb->pos.y)-limb->force.speedy; 
	limb->force.speedx+=xdif;
	limb->force.speedy+=ydif;

	}
	else
		if ((limb->hp>0) && (limb->LinkData->animation) && (limb->LinkData->animation<=anim->limbs)) {
			usableAnimationFrame* animPoint=frameLimbs+(limb->LinkData->animation-1);
			float desiredangle=CenterLimb->angle+animPoint->angle;
			int desiredx=animPoint->distance*cos(desiredangle);
			int desiredy=animPoint->distance*sin(desiredangle);
			int desiredh=animPoint->h+CenterLimb->pos.h;
			int curx=(limb->pos.tx-CenterLimb->pos.tx)*tyleSize*accuracy+(limb->pos.x-CenterLimb->pos.x - CenterLimb->force.speedx);
			int cury=(limb->pos.ty-CenterLimb->pos.ty)*tyleSize*accuracy+(limb->pos.y-CenterLimb->pos.y - CenterLimb->force.speedy);
			int speedx=(desiredx-curx);
			int speedy=(desiredy-cury);
			int speedh=(desiredh-limb->pos.h+GravCalc)*movepercantage;
			speedx=(speedx-limb->force.speedx)*movepercantage;
			speedy=(speedy-limb->force.speedy)*movepercantage;
			JustKatet (speedx/duIn13_8192, speedy/duIn13_8192);
			/*if (fuck>animPoint->power*temptest) {
				fuck=(double)animPoint->power/fuck;
				speedx*=fuck;
				speedy*=fuck;
			}*/
			limb->force.speedx+=speedx;
			limb->force.speedy+=speedy;
			limb->force.speedh+=speedh;
			}
		}
}


void EnforceAnimation(convertedAnimation* anim , AnyRagLimb* CenterLimb, int LimbCount, float Time){

	if (!anim->loaded) return;

	if (Time> anim->animationTime) Time=anim->animationTime-0.1; // to avoid reading unexisting frame
	int frame=anim->frames*Time/anim->animationTime;
	float movepercantage=(GameSpeed*AnimationModifier)/(anim->TimePerFrame-(Time-frame*anim->TimePerFrame)); 
	//movepercantage=min(0.5,movepercantage);
	//if (movepercantage<=0.01)
	//	movepercantage=0.01;
	// calculate this using GameSpeed variable

	AnyRagLimb* limb=CenterLimb+1;
	usableAnimationFrame* frameLimbs=anim->frame+frame*anim->limbs; 
	for (int i=0; i< LimbCount-1; i++){


		if ((limb->LinkData->animation) && (limb->LinkData->animation<=anim->limbs)) {
			usableAnimationFrame* animPoint=frameLimbs+(limb->LinkData->animation-1);
		// Find desired position
			float desiredangle=CenterLimb->angle+animPoint->angle;
			int desiredx=animPoint->distance*cos(desiredangle);
			int desiredy=animPoint->distance*sin(desiredangle);
			int desiredh=animPoint->h+CenterLimb->pos.h;

			int curx=(limb->pos.tx-CenterLimb->pos.tx)*tyleSize*accuracy+(limb->pos.x-CenterLimb->pos.x - CenterLimb->force.speedx);
			int cury=(limb->pos.ty-CenterLimb->pos.ty)*tyleSize*accuracy+(limb->pos.y-CenterLimb->pos.y - CenterLimb->force.speedy);
		int speedx=(desiredx-curx);
		int speedy=(desiredy-cury);
		int speedh=(desiredh-limb->pos.h+GravCalc)*movepercantage;
		// Now find desired speed change
		speedx=(speedx-limb->force.speedx)*movepercantage;
		speedy=(speedy-limb->force.speedy)*movepercantage;
		
		JustKatet (speedx/duIn13_8192, speedy/duIn13_8192);
		if (fuck>animPoint->power) {
			fuck=(double)animPoint->power/fuck;
			speedx*=fuck;
			speedy*=fuck;
		}

		limb->force.speedx+=speedx;
		limb->force.speedy+=speedy;
		limb->force.speedh+=speedh;
		/*	if (limb->connected!=-1){
				AnyRagLimb* con=CenterLimb+limb->connected;
				int masscoof=limb->force.mass/con->force.mass; 
				con->force.speedx-=speedx*masscoof;
				con->force.speedy-=speedy*masscoof;
			}*/

		}
		limb++;
	}



}

void ConvertAnimationToUsable (convertedAnimation* converted){
	if (animEd.limbs.val<2) return; 
	if (converted->loaded){
		delete(converted->frame);
		converted->loaded=0;
	}
	converted->frame = new usableAnimationFrame [(animEd.limbs.val-1)*animEd.frames.val];
	converted->loaded=1;
	converted->animationTime=animEd.AnimationTime.val;
	converted->frames=animEd.frames.val;
	converted->limbs=animEd.limbs.val-1; // zero Limb is not accounted
	converted->TimePerFrame=(double)animEd.AnimationTime.val/(double)animEd.frames.val; 
	usableAnimationFrame* frame=converted->frame;
	
	for (int j=0; j<animEd.frames.val; j++){
		limbFrame* limb=&animEd.FrameLimb[j][1];
		limbFrame* CenterLimb=&animEd.FrameLimb[j][0];
		for (int i=1; i<animEd.limbs.val; i++){
		
			frame->angle=getAngle(limb->x-CenterLimb->x, limb->y-CenterLimb->y);
			frame->distance=JustKatet(limb->x-CenterLimb->x, limb->y-CenterLimb->y)*accuracy/RagDollEditorUpsize;
			frame->power=limb->power;
			frame->h=(limb->h-CenterLimb->h)*accuracy/RagDollEditorUpsize;
			frame++;
			limb++;
		}
	}

}

void LimbToBuff(AnyRagLimb* limb){
	//if (limb->connectedNo){
int litAngle=64*(limb->angle + limb->LinkData->DisplayAngle + (M_PI*2*((limb->angle+limb->LinkData->DisplayAngle)<=-M_PI*2)) + (M_PI*2*((limb->angle+limb->LinkData->DisplayAngle)<0)) - (M_PI*2*((limb->angle+limb->LinkData->DisplayAngle)>=2*M_PI)))/M_PI/2;
MegaVox8bitDrow(palette.Buffer8bit,&mgvDB.mgvarray[limb->G8piece][limb->G8type].Converted 
				, 0, 0, accurate(limb->pos.h), limb->angle+limb->LinkData->DisplayAngle, 
	offx, 
	offy,litAngle,0,0);
//} else{
//QuickDrow8bitVex(palette.Buffer8bit,&limb->pos, &mgvDB.mgvarray[limb->G8piece][limb->G8type].Converted
//				 , 0, 0, accurate(limb->pos.h)+32, 1+(double)accurate(limb->pos.h)/VoxUpResize,
//				 limb->angle+limb->LinkData->DisplayAngle, limb->dist, 0,0);

//}
}

void LimbDrow(AnyRagLimb* limb){
optimizeCoordinates(&limb->pos);

if (!PosOnScreen(&limb->pos, 48)) return;

PosToFlatOffs(&limb->pos);

LimbToBuff(limb);

LightsTo180arrayF(&limb->pos, defAngleFade,0 , 0,deAlphaMask,0);//glob
DrowBufferByHeight(mgvDB.ConvertedPalette[limb->G8piece],  offx, offy, screen,1);
}

void LimbGroupDrow(AnyRagLimb* limb, int x, int y){

//	if ( (abs(x)+24>E8bitBufferRad) || (abs(y)+24>E8bitBufferRad))
//		LimbDrow(limb);
//	else {
int litAngle=64*(limb->angle + limb->LinkData->DisplayAngle + (M_PI*2*((limb->angle+limb->LinkData->DisplayAngle)<=-M_PI*2)) + (M_PI*2*((limb->angle+limb->LinkData->DisplayAngle)<0)) - (M_PI*2*((limb->angle+limb->LinkData->DisplayAngle)>=2*M_PI)))/M_PI/2;
MegaVox8bitDrow(palette.Buffer8bit,&mgvDB.mgvarray[limb->G8piece][limb->G8type].Converted 
				, x, y, accurate(limb->pos.h), limb->angle+limb->LinkData->DisplayAngle, 
	offx, 
	offy,litAngle,0,0);
//	}
}

void LimbCollision(AnyRagLimb* limb, int einma ){

	if (limb->flags & LIMB_Dismembered){
	limb->force.speedx*=limb->force.pr;
		limb->force.speedy*=limb->force.pr;
		limb->force.speedh*=limb->force.pr;
	}

dddcoords accel;
PosToFlatOffs(&limb->pos);
GetWind(&accel, offx, offy, 0);
limb->force.speedx+=accel.x/limb->force.mass;
limb->force.speedy+=accel.y/limb->force.mass;
bool collision=0;

limb->pos.h+=limb->force.speedh;
matr=getmematrix (limb->pos.tx, limb->pos.ty, 
	(int)(limb->pos.x)>>16, 
	(int)(limb->pos.y)>>16,
	(limb->pos.h)>>16);
if (matr){
	if (matr>limbinmatr)
		phisCollision (&limb->pos, &LIMBS[matr-limbinmatr].pos, &limb->force,  &LIMBS[matr-limbinmatr].force, 4, phisConnectionDeSquizeForce/3);
	else
	{
		if (matr>phinmatr)
			phisCollision (&limb->pos, &ph[matr-phinmatr].pos, &limb->force,  &ph[matr-phinmatr].force, 9, phisConnectionDeSquizeForce/3);
		else
			if (matr>einmatr)
			phisCollision (&limb->pos, &e[matr-einmatr].pos, &limb->force,  &e[matr-einmatr].force, 9, phisConnectionDeSquizeForce/3);
		else {
			limb->pos.h+=abs(limb->force.speedh); limb->force.speedh=-limb->force.speedh*limb->force.pr; 
			
			if (!limb->connectedNo){
				//limb->dist+=(float)limb->force.speedx/accuracy;
			}
			
			}

		
	
	}
	
}
//speed=min(accuracy*7, speed);
if (limb->pos.h<0) limb->pos.h=0;


for (AnimationSmoothingVar=0; AnimationSmoothingVar<AnimationAccuracy; AnimationSmoothingVar++){

float OrgnAng=getAngle(limb->force.speedx>>8, limb->force.speedy>>8); 
int speed=((int)(abs(JustKatet (limb->force.speedx>>12, limb->force.speedy>>12)))
		   *(AnimationAccuracy-AnimationSmoothingVar)/AnimationAccuracy)<<12;


	matr=getmematrix (limb->pos.tx, limb->pos.ty, 
	(int)(limb->pos.x+speed*cos(OrgnAng))>>16, 
	(int)(limb->pos.y+speed*sin(OrgnAng))>>16,
	(limb->pos.h)>>16);

if (matr && (matr<einmatr)){

	float displ=0.2;
	float NewAng;
	int n=6;

	while (n>0){

		NewAng=OrgnAng+displ;
		matr=getmematrix (limb->pos.tx, limb->pos.ty, 
			(int)(limb->pos.x+speed*cos(NewAng))>>16, 
			(int)(limb->pos.y+speed*sin(NewAng))>>16,
			limb->pos.h>>16);
		if ((!matr) || (matr<einmatr)) 
			goto brme;

			NewAng=OrgnAng-displ;
		matr=getmematrix (limb->pos.tx, limb->pos.ty, 
			(int)(limb->pos.x+speed*cos(NewAng))>>16, 
			(int)(limb->pos.y+speed*sin(NewAng))>>16,
			limb->pos.h>>16);
		if ((!matr) || (matr<einmatr)) 
			goto brme;


		displ*=2;
		n--;
	}

brme:

//int nspx=speed*cos(NewAng);
//int nspy=speed*sin(NewAng);


limb->force.speedx=(speed*cos(NewAng)+limb->force.speedx*3)/4;
limb->force.speedy=(speed*sin(NewAng)+limb->force.speedy*3)/4;

	limb->pos.x+=speed*cos(NewAng)/AnimationAccuracy;
	limb->pos.y+=speed*sin(NewAng)/AnimationAccuracy;


}else{

	float percent=double(AnimationAccuracy-AnimationSmoothingVar)/AnimationAccuracy;
limb->pos.x+=limb->force.speedx*percent;//speed*cos(OrgnAng);
limb->pos.y+=limb->force.speedy*percent;//speed*sin(OrgnAng);
AnimationSmoothingVar=AnimationAccuracy;

}
}


if(!einma)
	limb->force.speedh-=GravCalc/AnimationAccuracy;





}

void ManageRaggLimb(AnyRagLimb* limb, AnyRagLimb* connectedLimb){


	// !!!!!!!!!!! After creating animation add the full H phisics since doll will be held up by animation

float preAngle;
PosToFlatOffs(&limb->pos);


if ((limb->LinkData->connected!=-1) && (connectedLimb) && (connectedLimb->flags & LIMB_Occupied))
	{
		
		phisLink (&limb->pos, &connectedLimb->pos, &limb->force, &connectedLimb->force, limb->dist, 1000, 1000);

		if (connectedLimb->hp<5)
			{ limb->connectedNo=0; limb->dist=0; return; } 

		if (limb->LinkData->lockSize>0.2){	
limb->angle=getAngleFromPos(&connectedLimb->pos,&limb->pos);

float angDis=GetFloatAngleDistanceNegPos(connectedLimb->angle, limb->angle);

int distx=((limb->pos.tx-connectedLimb->pos.tx)<<20)+((limb->pos.x+limb->force.speedx-connectedLimb->pos.x-connectedLimb->force.speedx)>>6);// 20&6
int disty=((limb->pos.ty-connectedLimb->pos.ty)<<20)+((limb->pos.y+limb->force.speedy-connectedLimb->pos.y-connectedLimb->force.speedy)>>6);

angDis=GetFloatAngleDistanceNegPos(limb->LinkData->lockAngle, angDis);
float absdis=abs(angDis);
if ((absdis)<limb->LinkData->lockSize){
float massc=limb->force.mass/connectedLimb->force.mass;
double temppower= (((angDis>=0)*(limb->LinkData->lockSize-angDis)+(angDis<0)*(-limb->LinkData->lockSize-angDis)));
limb->force.speedx-=(disty*temppower);
limb->force.speedy+=(distx*temppower);
connectedLimb->force.speedx+=(disty*temppower)*massc;
connectedLimb->force.speedy-=(distx*temppower)*massc;
} 
		}

limb->angle=getAngleFromPos(&connectedLimb->pos,&limb->pos);


		}



}

Uint16 SpawnedLimbsNombers[maxLimbs];

int SpawnRagDoll(int Group, int Doll, worldcoordinates* target){

memset(SpawnedLimbsNombers,0,sizeof(SpawnedLimbsNombers));

	RagDoll* doll=&RagTemplate[Group][ Doll];
	doll->limb[0].angle=0; // to make sure voxel is facing left
	//AnyRagLimb LIMBS[numberOfRagLimbs];
	//int limbsInUse=0;
	//firstFreeLimb
//int ZeroLimb=-1;
int limbsToGet=doll->LimbCount;
int limbsGot=0;
int previousLimb=0;
while ((limbsToGet) && (firstFreeLimb<numberOfRagLimbs)){
	AnyRagLimb* limb=&LIMBS[firstFreeLimb];
	if (!(limb->flags & LIMB_Occupied)){
		limb->hp=doll->mass[limbsGot].val*10;
		SpawnedLimbsNombers[limbsGot]=firstFreeLimb;
		memcpy(&limb->pos,target, sizeof(worldcoordinates));
		limb->LinkData=&RagTemplate[Group][ Doll].limbDTA[limbsGot];
		limb->angle=doll->limb[limbsGot].angle; 
		//limb->lockSize=doll->limb[i].lockSize;
		limb->pos.h=doll->limb[limbsGot].pos.h*accuracy/RagDollEditorUpsize;
		limb->G8piece=doll->limb[limbsGot].G8piece;	
		limb->G8type=doll->limb[limbsGot].G8type;
		//limb->animation=doll->limb[i].animation;
		limb->force.mass=doll->mass[limbsGot].val;
		limb->force.speedx=limb->force.speedy=limb->force.speedh=0;
		limb->force.pr=0.6;//fix zero limb connection
		limb->flags=LIMB_Occupied;
		limb->dist=0;
		if ((doll->limb[limbsGot].LinkData->connected!=-1) && (doll->limb[limbsGot].LinkData->connected!=limbsGot))	 {
			AnyRagLimb* connected=&RagTemplate[Group][ Doll].limb[doll->limb[limbsGot].LinkData->connected];
			katet3dD (doll->limb[limbsGot].pos.x-connected->pos.x, doll->limb[limbsGot].pos.y-connected->pos.y,
				doll->limb[limbsGot].pos.h-connected->pos.h);
			limb->LinkData->defdist=limb->dist=fuck/RagDollEditorUpsize;


		
		limb->LinkData->lockAngle=GetFloatAngleDistanceNegPos(connected->angle, doll->limb[limbsGot].angle)+3.14;
	}
	else 
		limb->connectedNo=0;
	//test[i].DisplayAngle=doll->limb[i].DisplayAngle;
//test[0].LinkData->connected=-1;

		/////// END


	    limbsGot++;
		limbsToGet--;
		previousLimb=firstFreeLimb;
	}

firstFreeLimb++;
}
//LIMBS[previousLimb].nextPiece=0; // This will be last limb

limbsInUse=max(limbsInUse,firstFreeLimb);

//now fill all the connections:
for (int i=0; i<limbsGot; i++){
	AnyRagLimb* limb=&LIMBS[SpawnedLimbsNombers[i]];
	if ((limb->LinkData->connected!=-1) && (SpawnedLimbsNombers[doll->limb[i].LinkData->connected]!=SpawnedLimbsNombers[i]))
		limb->connectedNo=SpawnedLimbsNombers[doll->limb[i].LinkData->connected];
}

return limbsGot; 
}

void TestingLimbs(){
if (!testinited) {
		for (int i=0; i<testPCS; i++){
				memcpy(&test[i].pos, &player.host->pos, sizeof(worldcoordinates));
			test[i].force.mass=20;//testPCS*2-i;
			test[i].force.pr=0.6;
			test[i].dist=15;
		}
	mouseforce.mass=2000;
		mouseforce.pr=0.9;
		testinited=1;
		test[0].force.mass=2000;

		animEd.AllignAfter=1;

	}

if (animEd.playing){
	animEd.playingTime+=GameSpeed*AnimationModifier;
	if (animEd.playingTime>animEd.AnimationTime.val) animEd.playingTime=0;
	animEd.CurrentFrame.val=(float)((float)animEd.frames.val*animEd.playingTime)/animEd.AnimationTime.val;
	} else
		animEd.playingTime=(double)animEd.CurrentFrame.val*((double)animEd.AnimationTime.val/(double)animEd.frames.val);


MouseToPos (&MousePos);

phisLink (&test[0].pos, &MousePos, &test[0].force, &mouseforce, 30, 1000, 1000);

PosToFlatOffs(&test[0].pos);
test[0].angle=getAngle(mouse.pos.x-offx, mouse.pos.y-offy);
test[0].pos.h=defaultAnimationCenterHeight*accuracy; 

for (int i=0; i<AssignedTestPcs; i++){
test[i].force.speedx*=test[i].force.pr;
test[i].force.speedy*=test[i].force.pr;
test[i].force.speedh*=test[i].force.pr;
}

if (animEd.playing) EnforceAnimation(&animEd.usableTestingAnimation , &test[0], AssignedTestPcs, animEd.playingTime);
if (AssignedTestPcs)
ManageRaggLimb(&test[0], NULL);

for (int i=1; i<AssignedTestPcs; i++)
	ManageRaggLimb(&test[i], &test[test[i].LinkData->connected]);

for (int i=0; i<AssignedTestPcs; i++)
	LimbCollision(&test[i], 0);

for (int i=0; i<AssignedTestPcs; i++)
	LimbDrow(&test[i]);

mouseforce.speedx=mouseforce.speedy=mouseforce.speedh=0;
}

void LimbBlitting( AnyRagLimb* limb)
{
optimizeCoordinates(&limb->pos);

PosToFlatOffs(&limb->pos);

LimbToBuff(limb);

LightsTo180arrayF(&limb->pos, defAngleFade,0 , 0,deAlphaMask,0);//glob

int dx=accurate(limb->pos.x);
int dy=accurate(limb->pos.y);
int gdx=dx;
int gdy=dy;
int wid=24, hid=24;

if (dx<32) {limb->pos.tx-=1; dx+=tyleSize; gdx+=tyleSize;}
if (dy<32) {limb->pos.ty-=1; dy+=tyleSize; gdy+=tyleSize;}

int totylex=tylextogrid(limb->pos.tx)+1;
int totyley=tyleytogrid(limb->pos.ty)+1;
int ra=0;

int d=(tyleSize-wid);
int z=(tyleSize-hid);

for (int i=0; i<2; i++)
	for (int j=0; j<2; j++){

if (((dx>d) || !i) && ((dy>z) || !j)){
totylex=tylextogrid(limb->pos.tx+i)+1;
totyley=tyleytogrid(limb->pos.ty+j)+1;
BufferToGround(mgvDB.ConvertedPalette[limb->G8piece], gdx-tyleSize*i, gdy-tyleSize*j, storedmp[totylex][totyley]);	
RefreshGround (dx-12-tyleSize*i, dy-12-tyleSize*j, 24, 24, totylex, totyley);
}
}
CleanBuffer();
}

void RagdollDrowing(){

	animEd.IngameplayingTime+=AnimationModifier*GameSpeed;
	if (animEd.IngameplayingTime>=animEd.AnimationTime.val) animEd.IngameplayingTime=0;

	for (int i=1; i<limbsInUse; i++){
		AnyRagLimb* limb=&LIMBS[i];
		if (limb->flags & LIMB_Occupied){
			ManageRaggLimb(limb,limb->connectedNo ? &LIMBS[limb->connectedNo] : NULL );
			if (limb->flags & LIMB_Dismembered){ 
TryClearmatrix (limb->pos.tx, limb->pos.ty, accurate(limb->pos.x), accurate(limb->pos.y), accurate(limb->pos.h), i+limbinmatr);

				LimbCollision(limb, 0);	

				LimbDrow(limb);
				limb->hp--;
				if (limb->hp<=0) {
						if (limb->flags & LIMB_PreOp) {
							
						firstFreeLimb=min(firstFreeLimb,i);
						limb->flags=0;
					} else{
						limb->flags|=LIMB_PreOp;
						LimbBlitting( limb);
						}
				} else 
					TryFillrmatrix(limb->pos.tx, limb->pos.ty, accurate(limb->pos.x), accurate(limb->pos.y), accurate(limb->pos.h), i+limbinmatr);
			
			if (limb->hp>1){
					float alpha=limb->hp;
					alpha/=(float)DismemberHealth;
					DrowMyIndShadowA (&imgBank.DefaultShadow, offx-imgBank.DefaultShadow.w/2+accurate(limb->pos.h)+10, offy-imgBank.DefaultShadow.h/2, alpha);
				}
			}else
DrowMyIndShadow (&imgBank.DefaultShadow, offx-imgBank.DefaultShadow.w/2+accurate(limb->pos.h)+10, offy-imgBank.DefaultShadow.h/2);
				
			}
	}

	while ((limbsInUse>1) && (LIMBS[limbsInUse-1].hp<=0)) limbsInUse--;

}

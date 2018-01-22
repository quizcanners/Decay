void (*addShootEffectPTR) (int x,int y,int tx,int ty) = NULL;

double swingPulse;
double aview;
double adir;
double speed;
const int fpstep=30;
walkingHero* walker;
phisicalMe* force;
worldcoordinates* pos;



// DrowingMethods
void zeroG8Drow (BodyPartDrow* limb)
{
	if (FullyOnScreen(offx, offy, limb->G8->radius*2, limb->G8->radius*2))
{
int boffx=limb->x-offx;
int boffy=limb->y-offy;
NegPosDisplacement2DLimit(&boffx, &boffy, E8bitBufferRad-limb->G8->radius-16);
	//limb->x-offx, limb->y-offy - to drow all to one
QuickDrow8bitVex(palette.Buffer8bit,&epointer->pos, limb->G8, boffx, boffy, limb->h, 1, 0, 0, limb->angle,0);
 // offx, offy to drow all to one buffer
}
}
void oneSuperFast (BodyPartDrow* limb)
{
	SuperFastVoxel(limb->G, limb->x, limb->y, limb->h,  screen, 0 , 0, limb->angle);
}
void twoListItemDrow (BodyPartDrow* limb)
{
	listOfStuff[limb->itemInList].inArmDrow(limb->x, limb->y, limb->angle);
}

void drowLimb(int i)
{
	BodyPartDrow* limb=&LimbsCoords.limb[i];
	switch (limb->drowMethod)
	{
	case 0: zeroG8Drow (limb); break;
	case 1: oneSuperFast (limb); break;
	case 2: twoListItemDrow (limb); break;
	}

}
// Limbs calculations
int AddTorso()
{
	BodyPartDrow* limb=&LimbsCoords.limb[LimbsCoords.count];
	LimbsCoords.count++;
//If player shooting take cool side position
limb->angle=walker->angles.Ah+stepsize*walker->legs.negPosStep*(walker->armraise*100/(200-walker->armraise) +20)/75; // last is to lower the angle tilting while run
//	double torsoangle=walker->angles.Ah+stepsize*walker->legs.negPosStep*(walker->armraise+20)/100;
limb->x=offx;
limb->y=offy;
limb->h=0;
//calculate angle between aim and torso
//double teorVang=aview;
double minangle=abs(aview-limb->angle);
minangle=min(abs(minangle-2*M_PI),minangle);
//if (teorVang<torsoangle) 
//end of that
int torsooff=-(55*(100-walker->armraise)/100)*minangle/M_PI;
rotateOffXOffYh(0, 0,aview ,  &limb->x,  &limb->y,torsooff, -torsooff/2,  &limb->h);
limb->h=40;
//limb->x+=walker->torso.x+accurate(walker->torso.disx);
//limb->y+=walker->torso.y+accurate(walker->torso.disy);

limb->drowMethod=0;
limb->G8=&gayVoxel[3][0]; 
// Tilting sideways:
limb->x+=(walker->legs.negPosStep*cos(limb->angle+0.8))*walker->armraise/500;
limb->y+=(walker->legs.negPosStep*sin(limb->angle+0.8))*walker->armraise/500; // last to lover the side tilting while run
//rotateOffXOffYh(0, 0, aview+M_PI/2,  &limb->x,  &limb->y, gundist, gundist,  &offh); //fpstep
// tilt to compensate acceleration
//walker->torso.disx+=walker->forceAccel.speedx;
//walker->torso.disy+=walker->forceAccel.speedy;

if (flashlight)	
{
int flx=limb->x;
int fly=limb->y;
int flh=limb->h;
//rotateOffXOffYh(0, 0,aview ,  &flx,  &fly,15, 10,  &flh);
//int flx=(player.host->pos.tx-cameratylx)*tyleSize + accurate(player.host->pos.x+player.host->walk.gunholex)-camerax;
//int fly=(player.host->pos.ty-cameratyly)*tyleSize + accurate(player.host->pos.y+player.host->walk.gunholey)-cameray;
DrowFlashLight(3, flx, fly,	mouse.pos.x+player.bag.recoil.offx, mouse.pos.y+player.bag.recoil.offy,&player.host->pos);
	
}
//poffx=offx;
//poffy=offy;
return LimbsCoords.count-1;

}
int AddlimbG8(double angle, int offx, int offy, int h , G8_Particle* particle)
{
BodyPartDrow* limb=&LimbsCoords.limb[LimbsCoords.count];
LimbsCoords.count++;
//If player shooting take cool side position
limb->angle=angle;//walker->angles.Ah+stepsize*walker->legs.negPosStep*(walker->armraise+20)/100;
//	double torsoangle=walker->angles.Ah+stepsize*walker->legs.negPosStep*(walker->armraise+20)/100;
limb->x=offx;
limb->y=offy;
limb->h=0;
//calculate angle between aim and torso
//double teorVang=aview;
//double minangle=abs(aview-limb->angle);
//minangle=min(abs(minangle-2*M_PI),minangle);
//if (teorVang<torsoangle) 
//end of that
//int torsooff=-(55*(100-walker->armraise)/100)*minangle/M_PI;
//rotateOffXOffYh(0, 0,aview ,  &limb->x,  &limb->y,torsooff, -torsooff/2,  &limb->h);
//limb->x+=walker->torso.x+accurate(walker->torso.disx);
//olimb->y+=walker->torso.y+accurate(walker->torso.disy);

limb->drowMethod=0;
limb->G8=particle; 

// tilt to compensate acceleration
//walker->torso.disx+=walker->forceAccel.speedx*tempx;
//walker->torso.disy+=walker->forceAccel.speedy*tempx;

//poffx=offx;
//poffy=offy;
return LimbsCoords.count-1;

}

int AddLegs(double torsoangle, BodyPartDrow* torso)
{
	
/*	BodyPartDrow* lleg=&LimbsCoords.limb[LimbsCoords.count];
		BodyPartDrow* rleg=&LimbsCoords.limb[LimbsCoords.count+1];*/
			BodyPartDrow* lfoot=&LimbsCoords.limb[LimbsCoords.count];
				BodyPartDrow* rfoot=&LimbsCoords.limb[LimbsCoords.count+1];
	LimbsCoords.count+=2;


if ( !walker->jumping)  
{

		if (walker->legs.rightLegUp)
		{//walker->legs.negPosStep+=(speed+(abs(walker->lastStepAngle-walker->angles.Ah)))*GameSpeed;
		if (walker->legs.negPosStep>fpstep)
		{
			walker->legs.negPosStep=fpstep;
			walker->legs.rightLegUp=0;
			walker->lastStepAngle=walker->angles.Ah;
			if (soundEffects) PlayMySound(&sounds.bank.step[(rand() % SoundSteps)]);

		}
		}

		else {//walker->legs.negPosStep-=(speed+(abs(walker->lastStepAngle-walker->angles.Ah)))*GameSpeed;
		if (walker->legs.negPosStep<-fpstep) 
		{
			walker->legs.negPosStep=-fpstep;
			walker->legs.rightLegUp=1;
			walker->lastStepAngle=walker->angles.Ah;
			if (soundEffects) PlayMySound(&sounds.bank.step[(rand() % SoundSteps)]);
		}
		}

		//
} 
else
{
	walker->legs.negPosStep*=0.6;
			if (walker->legs.rightLegUp==1)
				copyPos(pos, &walker->legs.lleg);
			else
				copyPos(pos, &walker->legs.rleg);		
}

float torsoTwist=abs(walker->lastStepAngle-walker->angles.Ah);

if ((speed) || (torsoTwist*5>1))
{
if (walker->legs.rightLegUp)
		walker->legs.negPosStep+=(speed+torsoTwist)*GameSpeed;
else 
	walker->legs.negPosStep-=(speed+torsoTwist)*GameSpeed;


worldcoordinates* upleg;
int legx;
if (!walker->legs.rightLegUp) { upleg=&walker->legs.lleg;  legx=-15;}
else {upleg=&walker->legs.rleg; legx=15;}

int offh=0;
int poffx=offx;
int poffy=offy;
//First find future coordinates of character, then find its leg with its direction, and move to side with angle fo moving direction
rotateOffXOffYh(0, 0, adir+M_PI/2,  &poffx,  &poffy, -20, -20,  &offh);
rotateOffXOffYh(0, 0, walker->angles.Ah-stepsize*walker->legs.negPosStep+0.8,  &poffx,  &poffy, legx, 0,  &offh);
putSameTyle(upleg, pos);
upleg->x+=((pos->x+(poffx-offx)*accuracy)-upleg->x)*GameSpeed*(-abs(walker->legs.negPosStep)+fpstep)/(fpstep*2);
upleg->y+=((pos->y+(poffy-offy)*accuracy)-upleg->y)*GameSpeed*(-abs(walker->legs.negPosStep)+fpstep)/(fpstep*2);
}


lfoot->x= accurate(walker->legs.lleg.x) - actcamx - cameratylx*tyleSize + walker->legs.lleg.tx*tyleSize;
lfoot->y= accurate(walker->legs.lleg.y) - actcamy - cameratyly*tyleSize + walker->legs.lleg.ty*tyleSize;
double footangle;

if (!walker->legs.rightLegUp) 
{
//calculating leg rise
int elevation=(pos->h+feetRiseHeight*accuracy*walker->CoefForMidStepAsTop)/dddstength;
lfoot->x+=((lfoot->x-scrwid)*elevation)>>16;
lfoot->y+=((lfoot->y-scrhid)*elevation)>>16;
//legrise
	footangle=torsoangle-1.8;
}
else 
{
int elevation=pos->h/dddstength;
lfoot->x+=((lfoot->x-scrwid)*elevation)>>16;
lfoot->y+=((lfoot->y-scrhid)*elevation)>>16;
	footangle=walker->angles.Ah-1.8;
}

lfoot->drowMethod=0;
lfoot->G=&G_p[3][2]; 
lfoot->G8=&gayVoxel[3][2];
lfoot->h=-132;
lfoot->angle=footangle-0.8;

// ******************************Another leg

rfoot->x= accurate(walker->legs.rleg.x) - actcamx - cameratylx*tyleSize + walker->legs.rleg.tx*tyleSize;
rfoot->y= accurate(walker->legs.rleg.y) - actcamy - cameratyly*tyleSize + walker->legs.rleg.ty*tyleSize;
//double footangle;

if (walker->legs.rightLegUp) 
{
//calculating leg rise
int elevation=(pos->h+feetRiseHeight*accuracy*walker->CoefForMidStepAsTop)/dddstength;
rfoot->x+=((rfoot->x-scrwid)*elevation)>>16;
rfoot->y+=((rfoot->y-scrhid)*elevation)>>16;
//legrise
	footangle=torsoangle-1.8;
}
else 
{
int elevation=pos->h/dddstength;
rfoot->x+=((rfoot->x-scrwid)*elevation)>>16;
rfoot->y+=((rfoot->y-scrhid)*elevation)>>16;
	footangle=walker->angles.Ah-1.8;
}

rfoot->drowMethod=0;
rfoot->G=&G_p[3][2]; 
rfoot->G8=&gayVoxel[3][2];
rfoot->h=-132;
rfoot->angle=footangle;

return LimbsCoords.count-2;

}



int DrowGun()
{
	BodyPartDrow* limb=&LimbsCoords.limb[LimbsCoords.count];
	LimbsCoords.count++;

int	poffx=offx;
int poffy=offy;



double passivegun;
if (epointer->walk.blade.inHand)
passivegun=aview+(stepsize*walker->legs.negPosStep - 0.8)*walker->armraise/70*walker->blade.swingDirection;//-walker->blade.swingDirection
else
passivegun=aview+(stepsize*walker->legs.negPosStep - 0.8)*walker->armraise/100;//-walker->blade.swingDirection

double teorAngl=walker->gunpoint-2*M_PI;
if (abs(teorAngl-passivegun)<abs(walker->gunpoint-passivegun)) walker->gunpoint=teorAngl;
teorAngl=walker->gunpoint+2*M_PI;
if (abs(teorAngl-passivegun)<abs(walker->gunpoint-passivegun)) walker->gunpoint=teorAngl;

walker->gunpoint+=(passivegun-walker->gunpoint)*GameSpeed/10;
if (walker->forAngleX) walker->gunpoint=getAngle(walker->forAngleX, walker->forAngleY)+0.8; 
int offh=0;
float someAngle=M_PI*walker->armraise/600;
//walker->legs negPosStep
int gundist=min(((-25+walker->legs.negPosStep/10)*(100-walker->armraise)/100),-10);//-17);

double hill=hillGrapth(walker->blade.frame, bladeSwingTime);
	//hill=(walker->blade.frame-hill)/bladeSwingTime;
offh=0;
if (walker->shooting) 
rotateOffXOffYh(0, 0, aview+M_PI/2,  &poffx,  &poffy, gundist, gundist,  &offh);
else
{
double gunangle=(aview*(100-walker->armraise)/100)+(walker->angles.Ah+stepsize*walker->legs.negPosStep)*walker->armraise/100+M_PI/2;
if (walker->blade.activated)
{

	
	gundist+=gundist*swingPulse;
	gunangle=gunangle+(swingSizeConst-walker->blade.combo*0.09)*swingPulse;
	walker->gunpoint+=(bladeRotationSpeed+walker->blade.combo*0.003)*swingPulse*walker->blade.swingDirection;

if (epointer->walk.blade.frame>bladeSwingTime) epointer->walk.blade.activated=0;
	if (epointer->walk.blade.activated)
epointer->walk.blade.frame++;
	else
		epointer->walk.blade.frame=0;

}
offh=0;
rotateOffXOffYh(0, 0,gunangle+hill*1.5 ,  &poffx,  &poffy, gundist, gundist,  &offh);  
}
//poffx+=shake.x;
//poffy+=shake.y;
//int gunx=poffx;
//int guny=poffy;
limb->x=poffx;
limb->y=poffy;

walker->gunholex=(poffx-offx)*accuracy*gunstats[playerW->itemInHand->type].gunHoleDist;
walker->gunholey=(poffy-offy)*accuracy*gunstats[playerW->itemInHand->type].gunHoleDist;

if (walker->blade.activated) 
{
	createTornado(offx+accurate(walker->gunholex), offy+accurate(walker->gunholey), 10000+2000*walker->blade.combo, 4+0.02*walker->blade.combo, -walker->blade.swingDirection);
/*if (RadialDamage (pos->tx, pos->ty, pos->x+walker->gunholex, pos->y+walker->gunholey, pos->h, bladeRadius+0.8*walker->blade.combo, bladeDamage+50*walker->blade.combo, epointer->i))
{
	walker->blade.combo=min(walker->blade.combo+1,MaxBladeCombo);
	//walker->blade.combo=10;
shakeMeBaby(5000*walker->blade.combo);
walker->blade.poseffectsCharge+=bladeDamage;
bladeSwingTime=bladeSwingTimeDef+player.host->walk.blade.combo;
MakeBloodyNoise (offx, offy);
}*/
treeChop (pos,  bladeRadius+0.8*walker->blade.combo, bladeDamage+50*walker->blade.combo);
}

if (walker->forAngleX)
{
createData.xspeed=radiusx(20)*(accuracy/15);
createData.yspeed=radiusy*(accuracy/15);

addShootEffectPTR(pos->x+walker->gunholex+createData.xspeed*5,pos->y+walker->gunholey+createData.yspeed*5,pos->tx,pos->ty);

createData.slowing=0.95;
createData.tx=pos->tx;
createData.ty=pos->ty;
createData.x=pos->x+walker->gunholex;
createData.y=pos->y+walker->gunholey;
createData.hp=1599;
createData.h=max(pos->h+accuracy*8,accuracy*25);
createData.type=1505 + (rand() % 2); 
//createData.xspeed=accuracy * (rand() % 100 - 50)/40;
//createData.yspeed=accuracy * (rand() % 100 - 50)/40;

createData.hspeed=(rand() % accuracy) - accuracy/2;
createData.xaccel=0;
createData.yaccel=0;
createData.gravity=2;//30+rand() % 40;
//createData.gravity/=100;
createData.hp=max(pos->h+accuracy*8,accuracy*25);
createData.MaxLight=rand() % 2;
createData.boolFunction=&belowgroundmuch;
if (rand() % HeightDrowChance)
createData.voidfunction=&HeightDrow;
else
{
	createData.voidfunction=&HeightLight; createData.maskIMG=flameLight[rand() % 4];}
createData.staticPos=0;
addEffect();
}

walker->forAngleX=0;


limb->drowMethod=2;
//limb->G8=&gayVoxel[3][0]; 
limb->itemInList=walker->weaponUsing;
limb->angle=someAngle;
//limb->x=gunx;
//limb->y=guny;

//listOfStuff[walker->weaponUsing].inArmDrow(gunx, guny, someAngle);
return LimbsCoords.count-1;


}


void AnimateWalkingInit()
{
 walker=&epointer->walk; 
 force=&epointer->force;
 pos =&epointer->pos;
LimbsCoords.count=0;

int j=accurate(pos->h);
int k=0;//distance from player to shadow
while (k<matrhig)
{
	if ((getmematrix (pos->tx, pos->ty,accurate(pos->x)+timeofdayx*(k+4)/5, accurate(pos->y)+timeofdayy*(k+4)/5, j-pixInPhis-k*pixInPhis)!=0) || (j-k*pixInPhis<pixInPhis)) break;
	else 
	{
		k++;
	}
}
k*=pixInPhis;


int xx=offx;
int yy=offy;

if (pos->h<0) 
	{
		lightStyle (mergeimage[811], screen, offx-mergeimage[811]->w/2, offy-mergeimage[811]->h/2, 1);
		//lightStyle (image[79], screen, mouse.pos.x-image[79]->w/2, mouse.pos.y-image[79]->h/2, 1);
}
else
{

xx+=timeofdayx*(k+(j % pixInPhis)+4)/5;
yy+=timeofdayy*(k+(j % pixInPhis)+4)/5;

int elevation=((j/pixInPhis)*pixInPhis-k)*accuracy/dddstength;
xx+=((xx-scrwid)*elevation)>>16;
yy+=((yy-scrhid)*elevation)>>16;

}

	/// ********************************************************* Voxel animation simulation
//SDL_Rect qdest;
offx+=accurate(walker->torso.disx);
offy+=accurate(walker->torso.disy);

swingPulse=1.1*pulseGrapth(walker->blade.frame, bladeSwingTime)/bladeSwingTime*walker->blade.activated;
aview=getAngle(epointer->targetOnScreen->x-scrwid, epointer->targetOnScreen->y-scrhid)+0.8+swingPulse*swingBacking;
adir=getAngle(force->speedx, force->speedy)+0.8+swingPulse*1.5;
double ah;
 //30
int offh;
double angx=0;
double angy=0;
walker->CoefForMidStepAsTop=(fpstep-abs(walker->legs.negPosStep));
walker->CoefForMidStepAsTop/=fpstep;
//double tilt=0;
//bool armraise=0;

// TILTING To SIDE WHILE SHOOTING OR RUNING
//angx=(accurate(force->speedx))*0.2;
//angy=+(accurate(force->speedy))*0.2;

if ((walker->runing) && !(walker->shooting ))
{ 


	walker->armholddelay=fpstep*2;
	ah=adir; 
if (walker->forAngleX) ah+=((getAngle(walker->forAngleX, walker->forAngleY)+0.8)-ah)*0.6; 
}
else {ah=aview; 
if (walker->shooting) 
	ah+=0.8;    }

//RISE/lower arms while shooting;
if (walker->shooting) 
	   walker->armraise-=10; if (walker->armraise<0) walker->armraise=0;
	   else
	   {walker->armraise++; if (walker->armraise>100) walker->armraise=100;}

double rottationspeed=0.09;
//Fixing angle
double teorAngl=walker->angles.Ah-2*M_PI;
if (abs(teorAngl-ah)<abs(walker->angles.Ah-ah)) walker->angles.Ah=teorAngl;
teorAngl=walker->angles.Ah+2*M_PI;
if (abs(teorAngl-ah)<abs(walker->angles.Ah-ah)) walker->angles.Ah=teorAngl;
walker->angles.Ah+=((ah-walker->angles.Ah)*rottationspeed);

speed=ipifagor(abs(force->speedx)>>8,abs(force->speedy)>>8);
speed/=256;//+ipifagor(accurate(abs(walker->torso.spdisx)), accurate(abs(walker->torso.spdisy)));
//if (speed==1) speed=0;
//shake.x+=walker->legs.negPosStep*speed*camerashkespeed; // only for player!!!!!!!!!!!!!!!!!!!!!!!!!!!!


//************************************************************drow Torso
 
// Torso drow wil usually be called now;
int torsoInList=AddTorso();
// Tilt While moving and leg placement

walker->torso.disx*=comeBackForce;
walker->torso.disy*=comeBackForce;

walker->torso.disx+=walker->torso.spdisx;
walker->torso.disy+=walker->torso.spdisy;

walker->torso.spdisx=walker->torso.spdisx+walker->forceAccel.speedx-walker->torso.disx/comeBackTime;
walker->torso.spdisy=walker->torso.spdisy+walker->forceAccel.speedy-walker->torso.disy/comeBackTime;

NegPosDisplacement2DLimit(&walker->torso.disx, &walker->torso.disy, accuracy*20);

//********************************************************************************* Now the Fucking Legs
BodyPartDrow *torsoD=&LimbsCoords.limb[torsoInList];
int legsinlimb=AddLegs(torsoD->angle,torsoD);
drowLimb(legsinlimb);
drowLimb(legsinlimb+1);
// Here was legs calculations
//**************************************************************************************** LEGS OVER
CastShadowForDarkenfFunk(image[11],screen,xx-45+accurate(walker->torso.disx),yy-50+accurate(walker->torso.disy), pos->h,0);


//************************************************************************************Drow Gun
// Drow Gun here
int GunInList=DrowGun();
//**********************************************************************Drow upper right arm
offh=0;
int rarmx=LimbsCoords.limb[GunInList].x; int rarmy=LimbsCoords.limb[GunInList].y;
rotateOffXOffYh(0, 0,walker->gunpoint+M_PI/2 ,  &rarmx,  &rarmy, 10, 10,  &offh);
int ruparx=torsoD->x;
int rupary=torsoD->y;
offh=0;
rotateOffXOffYh(0, 0, torsoD->angle,  &ruparx,  &rupary, 14, 14,  &offh); // coordinates off upper part
double rarmangle=getAngle(rarmx-ruparx, rarmy-rupary)-0.8; 
//SuperFastVoxel(&G_p[3][3], ruparx, rupary, 25,  screen, 0 , 0, rarmangle);
drowLimb(AddlimbG8(rarmangle, ruparx, rupary, 25 , &gayVoxel[3][3]));
offh=0;
rotateOffXOffYh(0, 0,rarmangle,  &ruparx,  &rupary, -5, -5,  &offh);
rarmangle=getAngle(ruparx-rarmx, rupary-rarmy); 
//SuperFastVoxel(&G_p[3][4], rarmx, rarmy, 30,  screen, 0 , 0, rarmangle-0.8);
drowLimb(AddlimbG8(rarmangle-0.8,rarmx, rarmy, 30 , &gayVoxel[3][4]));
// ***************************************************************************    Left Arm
offh=0;
rarmx=LimbsCoords.limb[GunInList].x; rarmy=LimbsCoords.limb[GunInList].y;
rotateOffXOffYh(0, 0,walker->gunpoint+M_PI/2 ,  &rarmx,  &rarmy, -2, -2,  &offh);
ruparx=torsoD->x;
rupary=torsoD->y;
offh=0;
rotateOffXOffYh(0, 0, torsoD->angle,  &ruparx,  &rupary, -14, -14,  &offh); // coordinates off upper part
//int test=torsoD->x;
rarmangle=getAngle(rarmx-ruparx, rarmy-rupary)-0.8; 
//SuperFastVoxel(&G_p[3][3], ruparx, rupary, 25,  screen, 0 , 0, rarmangle);
drowLimb(AddlimbG8(rarmangle, ruparx, rupary, 25 , &gayVoxel[3][3]));
offh=0;
rotateOffXOffYh(0, 0,rarmangle,  &ruparx,  &rupary, 5, 5,  &offh); // end part of upper arm part

int ToGunDist=pifagor (ruparx-rarmx, rupary-rarmy);
rarmangle=getAngle(ruparx-rarmx, rupary-rarmy);

//SuperFastVoxel(&G_p[3][4], rarmx, rarmy, 30,  screen, 0 ,0, rarmangle-0.8);
drowLimb(AddlimbG8(rarmangle-0.8,rarmx, rarmy, 30 , &gayVoxel[3][4]));
//listOfStuff[walker->weaponUsing].inArmDrow(gunx, guny, someAngle);
drowLimb(GunInList);
//torso drowing was here
drowLimb(torsoInList);
offh=0;
//rotateOffXOffYh(0, 0,torsoD->angle,  &torsoD->x,  &torsoD->y, 0, 20,  &offh);
//listOfStuff[walker->weaponPassive].passiveDrow(torsoD->x, torsoD->y,torsoD->angle);
LightsTo180arrayF(&epointer->pos, defAngleFade,LimbsCoords.limb[torsoInList].angle , 0,deAlphaMask,0);//glob
DrowBufferByHeight(gayVoxel[3][0].palette,  offx, offy, screen,1);


}


void JustLegs()
{
 walker=&epointer->walk; 
 force=&epointer->force;
 pos =&epointer->pos;

adir=getAngle(force->speedx, force->speedy)+0.8;

walker->CoefForMidStepAsTop=(fpstep-abs(walker->legs.negPosStep));
walker->CoefForMidStepAsTop/=fpstep;

speed=ipifagor(abs(force->speedx)>>8,abs(force->speedy)>>8);
speed/=256;


		if (walker->legs.rightLegUp)
		{
		if (walker->legs.negPosStep>fpstep)
		{
			walker->legs.negPosStep=fpstep;
			walker->legs.rightLegUp=0;
			walker->lastStepAngle=adir;
		
		}
		}

		else {
		if (walker->legs.negPosStep<-fpstep) 
		{
			walker->legs.negPosStep=-fpstep;
			walker->legs.rightLegUp=1;
			walker->lastStepAngle=adir;
		}
		}



float torsoTwist=abs(walker->lastStepAngle-adir);

if ((speed) || (torsoTwist*5>1))
{
if (walker->legs.rightLegUp)
		walker->legs.negPosStep+=(speed+torsoTwist)*GameSpeed;
else 
	walker->legs.negPosStep-=(speed+torsoTwist)*GameSpeed;


worldcoordinates* upleg;
int legx;
if (!walker->legs.rightLegUp) { upleg=&walker->legs.lleg;  legx=-15;}
else {upleg=&walker->legs.rleg; legx=15;}

int offh=0;
int poffx=offx;
int poffy=offy;
//First find future coordinates of character, then find its leg with its direction, and move to side with angle fo moving direction
rotateOffXOffYh(0, 0, adir+M_PI/2,  &poffx,  &poffy, -20, -20,  &offh);
rotateOffXOffYh(0, 0, adir-stepsize*walker->legs.negPosStep,  &poffx,  &poffy, legx, 0,  &offh);
putSameTyle(upleg, pos);
upleg->x+=((pos->x+(poffx-offx)*accuracy)-upleg->x)*GameSpeed*(-abs(walker->legs.negPosStep)+fpstep)/(fpstep*2);
upleg->y+=((pos->y+(poffy-offy)*accuracy)-upleg->y)*GameSpeed*(-abs(walker->legs.negPosStep)+fpstep)/(fpstep*2);
}

}


bool TakeWalkingDamage(worldcoordinates* giverPos, phisicalMe* giveRforce, worldcoordinates* recPos, phisicalMe* recForce, offsetcoords* torso , int* hp)
{
		int av1=abs((recPos->tx-giverPos->tx)*tyleSize+accurate(recPos->x-giverPos->x));
	int bv1=abs((recPos->ty-giverPos->ty)*tyleSize+accurate(recPos->y-giverPos->y));
	int cv=accurate(recPos->h-giverPos->h);
	int damage=ipifagor(ipifagor (av1, bv1),cv);
	if (damage<40)
	{
		int av=accurate(recForce->speedx-giveRforce->speedx);
		int bv=accurate(recForce->speedy-giveRforce->speedy);
		damage=(40-damage)+(pifagor (av, bv));
		torso->spdisx+=giveRforce->speedx*2;
		torso->spdisy+=giveRforce->speedy*2;
		recForce->speedx+=accuracy/(av1+1);
		recForce->speedy+=accuracy/(bv1+1);

		*hp-=damage;//=rand() % damage;
createData.tx=giverPos->tx; createData.ty=giverPos->ty; createData.x=giverPos->x; createData.y=giverPos->y; createData.h=giverPos->h;
createData.type=20; createData.xspeed=giveRforce->speedx; createData.yspeed=giveRforce->speedy;  createData.hspeed=0;
createData.randosity=randy*damage; createData.numberof=bloodness*damage; createData.flooidness=2; createData.slowing=0.98;

addphisicalrandom();

giveRforce->speedx/=2;
giveRforce->speedy/=2;
return 1;
	}
return 0;
}
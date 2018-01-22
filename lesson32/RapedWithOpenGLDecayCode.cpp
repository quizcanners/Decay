#include "DataStructures.h"
#include "mechanicsFunctions.h"
#include "TrustedFunctions.h"
#include "TestingGroundsEngine.h"

#include "HeightDesk.h"
#include "PathFinder.h"
#include "Voxeldesk.h"
#include "Walking_man.h"
#include "missions.h"
#include "SpaceEngine.h"
#include "MegavoxelDesk.h"
#include "TerrainTools.h"


//test[i].LinkData=(edit.CurDollGroup*DollsInGroup + edit.CurDoll)*maxLimbs+i;
//	LimbConnectionData * LCD=&RagTemplate[edit.CurDollGroup][edit.CurDoll].limbDTA[i];
//LimbConnectionData * LCD=&RagTemplate[edit.CurDollGroup][edit.CurDoll].limbDTA[edit.Curlimb]; //	LCD->


//******************************************  INVENTORY

void dropItem(Item * _item, worldcoordinates * pos)
{
	if (_item->count<1) return;
 QO * n = new QO;
 n->qotx=pos->tx;
	n->qoty=pos->ty;
	n->qox=accurate(pos->x);
	n->qoy=accurate(pos->y);
	n->qoh=accurate(pos->h);
	n->qotimer=_item->type;
	n->qot = 305; //2000+...
	n->next=qedit.QOhead;
 // n->holding=_item->type;
  n->charge=_item->count;
  n->foo = PickMe;
  n->DeleteMe=0;
  n->interacting=0;
  n->scriptPtr=0;
  n->scriptText=0;
  qedit.QOhead = n; 
n->age=now;
//nq=n;
	_item->count=0;
	_item->type=0;
}
void SwapItems(Item * _item, Item * _item2)
{
	_item->count+=_item2->count;
	_item2->count=_item->count-_item2->count;
	_item->count-=_item2->count;

	_item->type+=_item2->type;
	_item2->type=_item->type-_item2->type;
	_item->type-=_item2->type;
}
void addToBottomList(QO* groundGoods)
{
	int i=0;
	while ((i<bottomListSize) && (Idisplay.ItemsOnTheGround[i]) && (Idisplay.ItemsOnTheGround[i]!=groundGoods)) i++;
	if (i<bottomListSize)
	if ((!Idisplay.ItemsOnTheGround[i]) || (Idisplay.ItemsOnTheGround[i]==groundGoods))
	{
		Idisplay.ItemsOnTheGround[i]=groundGoods; //need one more check
	groundGoods->interacting=1;
	Idisplay.IOGfading[i]=30;
	}
}
void removeFromBottomList(QO* groundGoods)
{
	int i=0;
	while ((i<bottomListSize) && (Idisplay.ItemsOnTheGround[i]!=groundGoods)) i++;
	groundGoods->interacting=0;
	
	if (Idisplay.IOGfading[i]<0)
	Idisplay.ItemsOnTheGround[i]=0;
	
}

void displayItemsOnTheGround()
{
	for (int i=0; i<bottomListSize; i++)
	if (Idisplay.ItemsOnTheGround[i]) 
	{
		apply_surface( Idisplay.IOGframe[i].x, Idisplay.IOGframe[i].y+100-Idisplay.IOGfading[i], image[80], screen );
		SuperFastVoxel360(&listOfStuff[Idisplay.ItemsOnTheGround[i]->qotimer].InvVoxel, Idisplay.IOGframe[i].x+Idisplay.halfw, Idisplay.IOGframe[i].y+Idisplay.halfh+100-Idisplay.IOGfading[i],0,  screen,Idisplay.objectRotation.Ax, Idisplay.objectRotation.Ay, Idisplay.objectRotation.Ah);
		if (!Idisplay.ItemsOnTheGround[i]->interacting) { Idisplay.IOGfading[i]--; 	if (Idisplay.IOGfading[i]<0) Idisplay.ItemsOnTheGround[i]=0;}
		else { Idisplay.IOGfading[i]+=10; 	if (Idisplay.IOGfading[i]>100) Idisplay.IOGfading[i]=100;}
	}
}
void bagison()
{
return;
Idisplay.objectRotation.Ah+=0.01;

camxaccel=((player.host->pos.tx-cameratylx)*tyleSize+accurate(player.host->pos.x) - camerax-scrwid)/cameraFollowSpeed;
camyaccel=((player.host->pos.ty-cameratyly)*tyleSize+accurate(player.host->pos.y) - cameray-scrhid)/cameraFollowSpeed;

actcamx+=camxaccel; 
camerax=actcamx;
actcamy+=camyaccel; 
cameray=actcamy;
DisplacementStyle (screen, Idisplay.rectInventoryBlock.x + 15, Idisplay.rectInventoryBlock.y + 15, pixInPhis, Idisplay.materials.Glassdisplacement, Idisplay.rectInventoryBlock.x , Idisplay.rectInventoryBlock.y );
	apply_surface( Idisplay.rectInventoryBlock.x, Idisplay.rectInventoryBlock.y, Idisplay.materials.invImge, screen );

for (int i=0; i<8; i++)
	for (int j=0; j<8; j++)
		if (player.bag.pack[i][j].count)
			SuperFastVoxel360(&listOfStuff[player.bag.pack[i][j].type].InvVoxel, Idisplay.rectInventoryBlock.x+25+50*i, Idisplay.rectInventoryBlock.y+25+50*j,0,  screen,0,0,0);

if (player.bag.RightHand.count) 
	SuperFastVoxel360(&listOfStuff[player.bag.RightHand.type].InvVoxel, mouse.pos.x+20, mouse.pos.y+20,90,  screen,Idisplay.objectRotation.Ax, Idisplay.objectRotation.Ay, Idisplay.objectRotation.Ah);
else
	apply_surface( mouse.pos.x, mouse.pos.y-Idisplay.materials.leftMB->h/2, Idisplay.materials.rightMB, screen );

if (player.bag.LeftHand.count) 
	SuperFastVoxel360(&listOfStuff[player.bag.LeftHand.type].InvVoxel, mouse.pos.x-20, mouse.pos.y+20,90,  screen,Idisplay.objectRotation.Ax, Idisplay.objectRotation.Ay, Idisplay.objectRotation.Ah);
else
	apply_surface( mouse.pos.x-Idisplay.materials.rightMB->w, mouse.pos.y-Idisplay.materials.leftMB->h/2, Idisplay.materials.leftMB, screen );

}
void ClickBrowseInventory(int MB)
{
	Item* _item=0;
	bool handfull=0;
if (MB==1) _item=&player.bag.LeftHand;  else
if (MB==3) _item=&player.bag.RightHand;
else return;

handfull=_item->count;

if 	(!(coordsInRectum(&Idisplay.rectInventoryBlock, mouse.pos.x, mouse.pos.y))) //(mouse.pos.y>scrhi-200)
	{
		if (handfull) dropItem(_item, &player.host->pos);
		//if ((MB==3) && (player.bag.RightHand.count)) dropItem(&player.bag.RightHand);

for (int i=0; i<bottomListSize; i++)
	if (Idisplay.ItemsOnTheGround[i]) 
	{
		if (coordsInRectum(&Idisplay.IOGframe[i], mouse.pos.x, mouse.pos.y))
		{
			_item->count=Idisplay.ItemsOnTheGround[i]->charge;
			_item->type=Idisplay.ItemsOnTheGround[i]->qotimer;
			Idisplay.ItemsOnTheGround[i]->DeleteMe=1;
			Idisplay.ItemsOnTheGround[i]=0;
			if (handfull)
			{
	Idisplay.ItemsOnTheGround[i]= qedit.QOhead;
	 qedit.QOhead->interacting=1;
	Idisplay.IOGfading[i]=30;
			}
		}
	}
	}
else
{
	int i=(mouse.pos.x-Idisplay.rectInventoryBlock.x)/50;
	int j=(mouse.pos.y-Idisplay.rectInventoryBlock.y)/50;
		SwapItems(_item, &player.bag.pack[i][j]);

}
player.host->walk.itemInHand=&player.bag.LeftHand;
player.host->walk.weaponUsing=player.host->walk.itemInHand->type;
player.host->walk.weaponPassive=player.bag.RightHand.type;
}
void AddToBag(int type, int count)
{

}

//*************************************************** Lights


//******************************************* Particle Effects N Shit
void QuickVoxelDrow()
{
//dddcoords* accel=GetWindGlobal (offx, offy);
//PEpoint->speedx+=accel->x;
//PEpoint->speedy+=accel->y;
//PEpoint->speedh+=accel->h
//PEpoint->x+=PEpoint->speedx;
//PEpoint->y+=PEpoint->speedy;


	int elevation;//=PEpoint->pos.h/dddstength;
//offx=offx+=((offx-scrwid)*elevation)>>16;
//offy=offy+=((offy-scrhid)*elevation)>>16;
PEpoint->anglex+=PEpoint->rotatex*GameSpeed;
PEpoint->angley+=PEpoint->rotatey*GameSpeed;
PEpoint->angleh+=PEpoint->rotateh*GameSpeed;
elevation=(PEpoint->pos.h>>16);
float res=0.1+0.03*PEpoint->hp;
PEpoint->hp++;
//SuperFastQVoxel(&QG_p[PEpoint->G_c][PEpoint->type], offx, offy,elevation,  screen, PEpoint->anglex, PEpoint->angley,PEpoint->angleh, res,PEpoint->g);//+voxTextureOffset);
createData.decaySurfaceNo=PEpoint->decaySurface;
//QuickDrowVex
//LightsToSingleValueF(&PEpoint->pos,255);
BloodSuperFast(&QG_p[PEpoint->G_c][PEpoint->type], doffx, doffy,elevation,  screen,res,PEpoint->anglex, PEpoint->angley,PEpoint->angleh, PEpoint->g+voxTextureOffset, DecayByOffxOffy, PEpoint->displayTime);//drowBloodByOffxOffy);
//else
//QuickDrowVex(&QG_p[5][PEpoint->type], offx, offy,elevation,  screen,res,PEpoint->anglex, PEpoint->angley,PEpoint->angleh, PEpoint->g+voxTextureOffset, voxNothing);
}
void SuperFastVoxelDrow()
{
dddcoords* accel=GetWindGlobal (offx, offy);
PEpoint->speedx+=(accel->x-PEpoint->speedx)/10;
PEpoint->speedy+=(accel->y-PEpoint->speedy)/10;
//PEpoint->speedh+=accel->h
//PEpoint->x+=PEpoint->speedx;
//PEpoint->y+=PEpoint->speedy;
int elevation=PEpoint->pos.h/dddstength;
offx=offx+=((offx-scrwid)*elevation)>>16;
offy=offy+=((offy-scrhid)*elevation)>>16;
PEpoint->anglex+=PEpoint->rotatex;
PEpoint->angley+=PEpoint->rotatey;
PEpoint->angleh+=PEpoint->rotateh;
//elevation=(PEpoint->pos.h>>13)-256;
//float res=0.1+0.05*PEpoint->hp;
//PEpoint->hp++;
//SuperFastQVoxel(&QG_p[5][PEpoint->type], offx, offy,elevation,  screen, PEpoint->anglex, PEpoint->angley,PEpoint->angleh, PEpoint->g+voxTextureOffset);
SuperFastQVoxel(&QG_p[PEpoint->G_c][PEpoint->type], offx, offy,elevation,  screen,PEpoint->anglex, PEpoint->angley,PEpoint->angleh,1, PEpoint->g);
}
void G8BloodLevelDrow()
{

	if (!QuickDrow8bitLayer(&PEpoint->pos, &gayVoxel[PEpoint->G_c][PEpoint->G_p], 0, 0, 0, 1, 0 , 0 , PEpoint->angleh, 1, &PEpoint->hp))
	PEpoint->hp=-1;
else
DrowBuffer(gayVoxel[PEpoint->G_c][PEpoint->G_p].palette,offx , offy, screen);
}
void G8ParticleVoxelDrow()
{
dddcoords* accel=GetWindGlobal (offx, offy);
PEpoint->pos.h=accuracy;
PEpoint->speedx+=(accel->x-PEpoint->speedx)/10;
PEpoint->speedy+=(accel->y-PEpoint->speedy)/10;
int elevation=PEpoint->pos.h/dddstength;
offx=offx+=((offx-scrwid)*elevation)>>16;
offy=offy+=((offy-scrhid)*elevation)>>16;
PEpoint->anglex+=PEpoint->rotatex;
PEpoint->angley+=PEpoint->rotatey;
PEpoint->angleh+=PEpoint->rotateh;
//SuperFastQVoxel(&QG_p[PEpoint->G_c][PEpoint->type], offx, offy,elevation,  screen,PEpoint->anglex, PEpoint->angley,PEpoint->angleh,1, PEpoint->g);
QuickDrow8bitVex(palette.Buffer8bit,&PEpoint->pos, PEpoint->G8t, 0, 0, accurate(PEpoint->pos.h),
	1+(double)accurate(PEpoint->pos.h)/VoxUpResize, PEpoint->anglex, PEpoint->angley, PEpoint->angleh, 0);
LightsTo180arrayF(&PEpoint->pos, defAngleFade, 0, 0,deAlphaMask,0); 
DrowBufferByHeight(mgvDB.ConvertedPalette[4], offx, offy, screen,1);
}

void DisplacementDrow()
{
int doffx=PEpoint->offx-displacement[PEpoint->type]->w/2;
int doffy=PEpoint->offy-displacement[PEpoint->type]->h/2;
int displ=hillGrapth(PEpoint->hp, 100)*accuracy;
//int elevation=PEpoint->h/dddstength;
int offx2=doffx+(((offx-scrwid)*displ)>>19);
int offy2=doffy+(((offy-scrhid)*displ)>>19);
PEpoint->hp-=10;
SameSurfaceDisplacement (screen, doffx , doffy , displacement[PEpoint->type], offx2, offy2);
}
//settings:
void VoxelBloodBasicSet(worldcoordinates* pos, int dx, int dy)
{
createData.g=(Uint32*)QGpalette[5]->pixels;
createData.tx=pos->tx; createData.ty=pos->ty;
createData.x=pos->x+dx*accuracy; createData.y=pos->y+dy*accuracy; 
createData.h=pos->h;//+accuracy*4;
createData.G_c=5;
createData.staticPos=0;
createData.xaccel=0;
//createData.xspeed=0;
createData.yaccel=0;
//createData.yspeed=0;
createData.hspeed=accuracy*VoxBloodUpDrop;
//createData.gravity=30+rand() % 40;
createData.slowing=VoxBloodSlowing;
createData.decaySurfaceNo=12+(rand() % 2)*2;
createData.gravity=VoxBloodGravity;
createData.displayTime=4; // bigger to stretch h (brush size)
createData.voidfunction=&QuickVoxelDrow;
createData.boolFunction=&belowgroundToomuch;
const double rot=0.002;
createData.anglex=GetRandomRotation(3);
createData.angley=GetRandomRotation(3);
createData.angleh=GetRandomRotation(3);
createData.rotationx=GetRandomRotation(rot);
createData.rotationh=GetRandomRotation(0.001);
createData.rotationy=GetRandomRotation(rot);
createData.hp=1;
createData.blitToTyle=rand() % 2;

}
void G8BloodBasicSet(worldcoordinates* pos)
{
createData.tx=pos->tx; createData.ty=pos->ty;
createData.x=pos->x; createData.y=pos->y; 
createData.h=pos->h+accuracy*20;
createData.G_c=10;
createData.G_p=1;
createData.staticPos=0;
createData.xaccel=0;
createData.yaccel=0;
createData.xspeed=0;
createData.yspeed=0;
createData.hspeed=accuracy/2;
//createData.gravity=30+rand() % 40;
createData.slowing=0.95;
createData.decaySurfaceNo=12+(rand() % 3);
createData.gravity=1;
createData.voidfunction=&G8BloodLevelDrow;
createData.boolFunction=&noHealth;
const double rot=0.006;
createData.anglex=0;
createData.angley=0;
createData.angleh=GetRandomRotation(3);
createData.rotationx=GetRandomRotation(rot);
createData.rotationh=GetRandomRotation(0.001);
createData.rotationy=GetRandomRotation(rot);
createData.hp=0;
//createData.blitToTyle=rand() % 2;

}
bool RadialDamage (int tx, int ty, int x, int y, int h, int rad, int damage, int DNA)
{
h>>=16;
x>>=16;
y>>=16;
h+=2;

bool killedone=0;
for (int ch=-rad; ch<rad; ch+=8)
	for (int cy=-rad; cy<rad; cy+=8)
		for (int cx=-rad; cx<rad; cx+=8)
		{ katet3dD (cx, cy,ch);
		if (fuck<rad)
		{

//int rdam=damage*(rad-fuck)/rad;
int lmatr=getmematrix (tx, ty, x+cx, y+cy,h+ch); 
if (lmatr>1){
if ((lmatr>=einmatr) && (lmatr<phinmatr) && (lmatr!=DNA+einmatr))
{
somebody* epo=&e[lmatr-einmatr];
if (player.host!=epo)
	epo->hp=-damage*(rad-fuck)/rad;
/*
createData.g=(Uint32*)QGpalette[5]->pixels;//(Uint32*)mergeimaged[0]->pixels+(rand() % (1024*1000));//(Uint32*)QGpalette[5]->pixels;

	
		VoxelBloodBasicSet(&epo->pos, 0, 0);
		createData.type=rand() % 8;//14 + (rand() % 2);//
		createData.xspeed=radiusx(accuracy*2);
		createData.yspeed=radiusy;
		addEffect();*/
		

	//epointer->walk.blade.frame-=2;
	//epointer->walk.itemInHand->cooldown+=2;
}else
	if ((lmatr<einmatr) && (w[lmatr].Megavoxel)){
		int wh=(h+ch)/8;//ch/8;
		wally* wall=&w[lmatr];
		// hitting Voxel Wall
		int vx=((tx-wall->pos.tx)*tyleSize+x+cx-accurate(wall->pos.x)+4*8)/8; 
		int vy=((ty-wall->pos.ty)*tyleSize+y+cy-accurate(wall->pos.y)+4*8)/8;

		if ((wh>=0) && (wh<8) && (vx>=0) && (vx<8) && (vy>=0) && (vy<8))	{
			wall->Megavoxel->hp[wh*8*8+vy*8+vx]-=damage*(rad-fuck)/rad;
			int num=wh*8*8+vy*8+vx;
			if (wall->Megavoxel->hp[num]<1) { wall->Megavoxel->NeedUpdate=1;
			
createData.lightPisObj=0;
for (int v=0; v<2; v++){
int type=rand() % 7;
int no=addphisicalrandomold(curphisobj->pos.tx,curphisobj->pos.ty, jhx+cx*accuracy, jhy+cy*accuracy, curphisobj->pos.h+ch*accuracy, 860, 0, 0,
	1 //up
	, 0 , 240+type*60, 20-type, 0 ,0, randy*(20-type), 1,10+(rand() % 10));			

ph[no].G_p=1;
ph[no].BlitOnDeath=1;
ph[no].data=wall->Megavoxel->piece[num]; 
ph[no].g8particle=&mgvDB.mgvarray[11][type].Converted;
ph[no].solidness=0.08;
ph[no].voxelPalette=image[60];

int dlist;
if (wall->Debris!=-1) dlist=wall->Debris;
else dlist=wall->Debris=GetFreeDebris();

if (dlist>=0)
	if (DebrisList[dlist].Max<RelatedParticlesLim){
		DebrisList[dlist].no[DebrisList[dlist].Max]=no;
DebrisList[dlist].Max++;
}



}

			}
		}

	}

}

}
}
return killedone;
}
void addG8particle(worldcoordinates* pos)
{
int type=rand() % 4;
createData.G8t=&mgvDB.mgvarray[12][type].Converted;
createData.tx=pos->tx; createData.ty=pos->ty;
createData.x=pos->x; createData.y=pos->y; 
createData.h=pos->h;
createData.staticPos=0;
createData.xaccel=0;
createData.yaccel=0;
createData.hspeed=0;
createData.slowing=VoxBloodSlowing;
createData.gravity=0;
createData.displayTime=4; // bigger to stretch h (brush size)
createData.voidfunction=&G8ParticleVoxelDrow;
createData.boolFunction=&offScreenDeath;
const double rot=0.006;
createData.anglex=GetRandomRotation(3);
createData.angley=GetRandomRotation(3);
createData.angleh=GetRandomRotation(3);
createData.rotationx=GetRandomRotation(rot);
createData.rotationh=GetRandomRotation(0.001);
createData.rotationy=GetRandomRotation(rot);
createData.hp=1;
createData.blitToTyle=0;

addEffect();
}

void SuperFastBasicSet()
{
createData.xaccel=0;
createData.yaccel=0;
createData.hspeed=0;
//createData.gravity=30+rand() % 40;
createData.slowing=0.95;
createData.G_c=5;
createData.gravity=1;
createData.voidfunction=&SuperFastVoxelDrow;
createData.boolFunction=&timedDeath;
const double rot=0.006;
createData.anglex=GetRandomRotation(3);
createData.angley=GetRandomRotation(3);
createData.angleh=GetRandomRotation(3);
createData.rotationx=GetRandomRotation(rot);
createData.rotationh=GetRandomRotation(0.001);
createData.rotationy=GetRandomRotation(rot);
createData.hp=1;
createData.staticPos=0;
//addEffect();
}
void smokedrop()
{
VoxelBloodBasicSet(&curphisobj->pos, 0, 0);
createData.x=jhx;
createData.y=jhy;
createData.hp=1599;
createData.h=curphisobj->pos.h+accuracy*15;//max(curphisobj->pos.h,accuracy*35);
createData.type=1502 + (rand() % 5); 
createData.xspeed=radiusx(20)*(accuracy/15) - jspx/3;
createData.yspeed=radiusy*(accuracy/15) - jspy/3;
//createData.xspeed=accuracy * (rand() % 100 - 50)/40 - jspx;
//createData.yspeed=accuracy * (rand() % 100 - 50)/40 - jspy;
createData.hspeed=(rand() % accuracy) - accuracy/2;
createData.xaccel=0;
createData.yaccel=0;
createData.MaxLight=rand() % 2;
createData.gravity=0.1;//30+rand() % 40;
//createData.gravity/=100;
createData.boolFunction=&belowgroundmuch;
createData.hp=curphisobj->pos.h;
if (rand() % HeightDrowChance)
createData.voidfunction=&HeightDrow;
else
{
	createData.voidfunction=&HeightLight; createData.maskIMG=flameLight[rand() % 4];}
createData.staticPos=0;
addEffect();
}

void movePE (ParticleEffects* bo, int i)
{
if (hig<0) hig=0;
PEpoint=PEpoint->next;

bo->prev->next=bo->next;
bo->next->prev=bo->prev;

if (hig>matrhig+matrdepth-1) hig=matrhig+matrdepth-1;
	
if (i<hig) bo->skip=true;

PEfirst[hig].next->prev=bo;
bo->prev=&PEfirst[hig];
bo->next=PEfirst[hig].next;
PEfirst[hig].next=bo;

}
void removePE (ParticleEffects* bo, int i)
{
PEpoint->light.active=0;
PEpoint=PEpoint->next;
	
	/*if (bo->prev==NULL)  
	{
		PEfirst[i]=bo->next; 
		if (bo->next!=0)
		bo->next->prev=NULL; 
	}
	else
	{
		bo->prev->next=bo->next;
		if (bo->next!=NULL)
		bo->next->prev=bo->prev;
	}*/
bo->prev->next=bo->next;
bo->next->prev=bo->prev;
		bo->type=0;
	
}
void PEphisics (ParticleEffects* PEone, int lvl)//int lvl)
{
int i;
i=1;
PEpoint=PEone;//PEfirst[lvl].next;

while (PEpoint->next)
{
testCountEfects++;
	//while (PEpoint->skip) {PEpoint->skip=0; 	PEpoint=PEpoint->next;}
if (!PEpoint->skip)
{
int shortx=PEpoint->pos.x>>16;
int shorty=PEpoint->pos.y>>16;


offx=shortx - actcamx - (cameratylx - PEpoint->pos.tx)*tyleSize;
offy=shorty - actcamy - (cameratyly - PEpoint->pos.ty)*tyleSize;

doffx=PEpoint->pos.x-(actcamx<<16);
doffx/=accuracy;
doffx-=(cameratylx - PEpoint->pos.tx)*tyleSize;
doffy=PEpoint->pos.y-(actcamy<<16);
doffy/=accuracy;
doffy-=(cameratyly - PEpoint->pos.ty)*tyleSize;
int elevation=PEpoint->pos.h/dddstength;
doffx+=(((doffx-scrwid)*elevation)/accuracy);
doffy+=(((doffy-scrhid)*elevation)/accuracy);

PEpoint->drowing();

if (!PEpoint->staticPos)
{
PEpoint->speedx+=PEpoint->xaccel*GameSpeed;
PEpoint->pos.x+=PEpoint->speedx*GameSpeed;

PEpoint->speedy+=PEpoint->yaccel*GameSpeed;
PEpoint->pos.y+=PEpoint->speedy*GameSpeed;

PEpoint->pos.h+=PEpoint->speedh*GameSpeed;
PEpoint->speedh-=GravCalc*PEpoint->gravity;
double NewSlowing=1;
NewSlowing-=(1-PEpoint->slowing)*GameSpeed;
PEpoint->speedx*=NewSlowing;
PEpoint->speedy*=NewSlowing;
PEpoint->speedh*=NewSlowing;

}

if (!PEpoint->deathcondition()) 
{
	hig=(int)(PEpoint->pos.h>>16)/pixInPhis+matrdepth;
	if ((hig!=lvl) && (lvl!=666))	
	movePE (PEpoint, lvl);
	else 
		PEpoint=PEpoint->next;
}
else
 {


removePE(PEpoint, lvl);
}
}
else
{
	PEpoint->skip=0; 	PEpoint=PEpoint->next;
}

}
	
}


// presets:

void splatter(worldcoordinates* pos)
{
	createData.type=14+(rand() % 3);
VoxelBloodBasicSet(pos, 0, 0);
createData.xspeed=accuracy/300*(rand() % 20 - 10);
createData.yspeed=accuracy/300*(rand() % 20 - 10);
createData.g=(Uint32*)QGpalette[5]->pixels;
createData.staticPos=0;
addEffect();
	/*
createData.tx=pos->tx; createData.ty=pos->ty; createData.x=pos->x; createData.y=pos->y; createData.h=pos->h;
createData.type=20; createData.xspeed=0; createData.yspeed=0;  createData.hspeed=accuracy;
createData.randosity=randy*4; createData.numberof=100; createData.flooidness=1; createData.slowing=0.98;
createData.lightPisObj=0;
addphisicalrandom();


createData.type=813; 
createData.randosity=randy*2; createData.numberof=100; createData.flooidness=2; createData.slowing=0.98;
createData.lightPisObj=0;
addphisicalrandom();
*/


}

// ******************************** Phisics functions

// ***************** on hit

void addHitEffect(int h)
{
createData.tx=curphisobj->pos.tx;
createData.ty=curphisobj->pos.ty;
createData.x=jhx; 
createData.y=jhy;
createData.h=h;//curphisobj->pos.h;
createData.gravity=0;
createData.slowing=createData.hp=rand() % 15;
createData.boolFunction=&timedDeath;
createData.voidfunction=&gunshoot;
createData.xaccel=0;
createData.yaccel=0;
createData.xspeed=offx-accurate(jspx*20);
createData.yspeed=offy-accurate(jspy*20);
createData.staticPos=1;
createData.displayTime=1;
createData.light.active=1;
Uint32 intensity=(rand() % 38) + 192;
//curphisobj->light.color=intensity/3+(intensity<<8) + (intensity<<16);
createData.light.color=intensity+(intensity<<8) + (intensity<<16);
createData.light.dinamic=1;
createData.light.distanceFading=0.0083;
createData.light.brightness=BulletBrightnes;
addEffect();
}
void burn()
{

	//if (e[1].hp>1) e[1].hp++;
	if ((matr>1) && (matr<einmatr) && (w[matr].type)) 
	{


			if (!w[matr].brick) return;
	if (!w[matr].brickcopy) 
		{
		w[matr].brickcopy=1; 
		w[matr].brick=SDL_ConvertSurface(w[matr].brick, w[matr].brick->format, SDL_SWSURFACE);
		} 
int fx=(curphisobj->pos.tx-w[matr].pos.tx)*tyleSize+accurate(jhx)-w[matr].pos.x;
int fy=(curphisobj->pos.ty-w[matr].pos.ty)*tyleSize+accurate(jhy)-w[matr].pos.y;
g=fuckwalls(w[matr].brick, fx , fy, 0);
if	(!(g>>30)) return;
if (InnerWall[w[matr].type-900])
FuckWusingMaskWithInner(w[matr].brick,InnerWall[w[matr].type-900], wallFmask[1],fx , fy);
else
	if (imageVoxel[w[matr].type])
	{
/*
fx=SuperFastVoxelGetImpactCoords(imageVoxel[w[matr].type], w[matr].brick->w/2, w[matr].brick->h/2, 0, w[matr].brick, w[matr].angles.Ax,w[matr].angles.Ay,w[matr].angles.Ah, 1, fx, fy,w[matr].gutsout);
fy=fx/w[matr].gutsout->w;
FuckVexelGutsOutDoubleImage(w[matr].gutsout, wallFmask[1], fx % w[matr].gutsout->w , fy,0);	*/
	}
	else
FuckWusingMask(w[matr].brick, wallFmask[1],fx , fy);
curphisobj->hp=-1;
ja=ji;
redirect=1;
createData.type=9;
	createData.mask=2;
	addHitEffect(w[matr].pos.h*pixInPhis*accuracy);


//
//gi=(Uint32*)wall[w[matr].type-900]->pixels+wall[w[matr].type-900]->w*wall[w[matr].type-900]->h/2;
	}
	else
		if (matr==1)
		{
			curphisobj->hp=-1;
			ji=ja;
			decayTyles(curphisobj->pos.tx, curphisobj->pos.ty, (int)((int)jhx>>16), (int)((int)jhy>>16),mergeimaged[curphisobj->type],image[60]);
			
		}

}

int basic()
{
	redirect=1;
	curphisobj->light.distanceFading*=curphisobj->hp;
	curphisobj->hp-=onImpactHealthLoss;
	if (curphisobj->hp>0)
	curphisobj->light.distanceFading/=curphisobj->hp;
	return 1;
}
int hitrecohit() // only for guts
{
if (matr>=phinmatr) return 0;


int totylex=tylextogrid(curphisobj->pos.tx)+1;
int totyley=tyleytogrid(curphisobj->pos.ty)+1;


// take acceleration from enemies
int dirx=accuracy*4, diry=0, dirh=0;
rotateVectorI(&dirx, &diry, &dirh, curphisobj->angles.Ah, curphisobj->angles.Ax,curphisobj->angles.Ay);

createData.tx=curphisobj->pos.tx; createData.ty=curphisobj->pos.ty; createData.x=jhx+((rand() % 10)-5)*accuracy; createData.y=jhy+((rand() % 10)-5)*accuracy; 
createData.h=accuracy/2;
createData.type=20; createData.xspeed=(rand() % tyleSize - 500)*bloodness*10+dirx; createData.yspeed=(rand() % tyleSize - 500)*bloodness*10+diry;  createData.hspeed=dirh;
createData.randosity=200; createData.numberof=rand() % (1+bloodness*3*(dirh>0)); createData.flooidness=1; createData.slowing=0.96;

createData.lightPisObj=0;
addphisicalrandom();//curphisobj->pos.tx, curphisobj->pos.ty, curphisobj->pos.x, curphisobj->pos.y, accuracy/2, 20, (rand() % tyleSize - 500)*bloodness*10, (rand() % tyleSize - 500)*bloodness*10,0, 200, rand() % bloodness*3,1, 0.96);

//MakegutsNoise (offx, offy);

if (matr<phinmatr) {curphisobj->hp-=onImpactHealthLoss*GameSpeed; redirect=1;

if (curphisobj->hp<=0){
SDL_Surface* img=DecayMaskForGuts[rand() % DecayGutsMask];
decayTyles(curphisobj->pos.tx, curphisobj->pos.ty, accurate(curphisobj->pos.x), accurate(curphisobj->pos.y), img, mergeimaged[8]);//7+(rand() % 3)
RefreshGround (accurate(curphisobj->pos.x)-img->w/2, accurate(curphisobj->pos.y)-img->h/2, img->w, img->h, totylex, totyley);
}

/*
if ((matr==1) && (curphisobj->hp<0) && (splatPerFrame) && (!flashlight))
{
createData.h=accuracy*27;
createData.type=1502 + (rand() % 5); 
createData.xspeed=0;
createData.yspeed=0;
createData.hspeed=0;//(rand() % accuracy) - accuracy/2;
createData.xaccel=0;
createData.yaccel=0;
createData.hp=curphisobj->pos.h;
createData.gravity=0.5;
//createData.gravity/=100;
createData.MaxLight=rand() % 2;
//30+rand() % 40;
//createData.gravity/=100;
createData.boolFunction=&belowgroundmuch;
createData.voidfunction=&HeightDrowFormergeimaged;
createData.staticPos=0;
addEffect();
}*/


return 1;}
return 0;
}
int hitChangeRotation()
{
//redirect=1;
//curphisobj->hp-=15;

//curphisobj->angles.aSpX=GetRandomRotation(0.05); curphisobj->angles.aSpY=GetRandomRotation(0.05);
/*
/*double ax=curphisobj->angles.aSpX;
double ay=curphisobj->angles.aSpY;

curphisobj->angles.aSpX=(curphisobj->angles.aSpX+accurate(curphisobj->force.speedx))/2;
curphisobj->angles.aSpY=(curphisobj->angles.aSpY-accurate(curphisobj->force.speedy))/2;

curphisobj->force.speedx+=ax*accuracy/2;
curphisobj->force.speedy-=ay*accuracy/2;*/
if ((curphisobj->force.speedx) && (curphisobj->force.speedy))
{
double temprot=curphisobj->force.speedx;
curphisobj->angles.aSpX=((temprot/(accuracy*M_PI*5))+curphisobj->angles.aSpX)/2;
temprot=-curphisobj->force.speedy;
curphisobj->angles.aSpY=((temprot/(accuracy*M_PI*5))+curphisobj->angles.aSpY)/2;

//double xtoy=curphisobj->force.speedx;
//xtoy/=curphisobj->force.speedy;
//if ((xtoy>0.2) && (xtoy<10))
//{
//curphisobj->angles.aSpH=0;
//curphisobj->angles.aSpX=curphisobj->angles.aSpX*curphisobj->force.speedx/curphisobj->force.speedy;
//curphisobj->angles.aSpY=curphisobj->angles.aSpY*curphisobj->force.speedy/curphisobj->force.speedx;
//}
//curphisobj->angles.aSpX=curphisobj->angles.aSpX*((rand() % 10) - 5)/5;
//curphisobj->angles.aSpY=curphisobj->angles.aSpY*((rand() % 10) - 5)/5;

}
createData.tx=curphisobj->pos.tx; createData.ty=curphisobj->pos.ty; createData.x=curphisobj->pos.x; createData.y=curphisobj->pos.y; 
createData.h=accuracy/2;
createData.type=20; createData.xspeed=(rand() % tyleSize - 500)*bloodness*10; createData.yspeed=(rand() % tyleSize - 500)*bloodness*10;  createData.hspeed=0;
createData.randosity=200; createData.numberof=rand() % bloodness*3; createData.flooidness=1; createData.slowing=0.96;


createData.lightPisObj=0;
addphisicalrandom(); 

hitrecohit();
//curphisobj->pos.tx, curphisobj->pos.ty, curphisobj->pos.x, curphisobj->pos.y, accuracy/2, 20, (rand() % tyleSize - 500)*bloodness*10, (rand() % tyleSize - 500)*bloodness*10,0, 200, rand() % bloodness*3,1, 0.96);
return 1;
}
int hitDropFlame()
{
	int sno=addFlameEffect(&curphisobj->pos, 0.6);
	    smoke[sno].pos.x=jhx;
		smoke[sno].pos.y=jhy;
		smoke[sno].pos.h=jhh;
		smoke[sno].anglex=rand() % 400;
		smoke[sno].angley=rand() % 400;
		smoke[sno].hp=20;

redirect=1;
	curphisobj->light.distanceFading*=curphisobj->hp;
	curphisobj->hp-=onImpactHealthLoss;
	if (curphisobj->hp>0)
	curphisobj->light.distanceFading/=curphisobj->hp;

	SDL_Surface* img=DecayMaskForGuts[rand() % DecayGutsMask];
decayTyles(curphisobj->pos.tx, curphisobj->pos.ty, accurate(curphisobj->pos.x), accurate(curphisobj->pos.y), img, image[87]);//7+(rand() % 3)

int totylex=tylextogrid(curphisobj->pos.tx)+1;
int totyley=tyleytogrid(curphisobj->pos.ty)+1;

RefreshGround (accurate(curphisobj->pos.x)-img->w/2, accurate(curphisobj->pos.y)-img->h/2, img->w, img->h, totylex, totyley);

	return 1;
}
int hitDebris()
{
	/*if (! (rand() % 10)){
	int sno=addFlameEffect(&curphisobj->pos);
	    smoke[sno].pos.x=jhx;
		smoke[sno].pos.y=jhy;
		smoke[sno].pos.h=jhh;
		smoke[sno].anglex=rand() % 400;
		smoke[sno].angley=rand() % 400;
		smoke[sno].hp=20;
	}*/
	redirect=1;

	if ((matr>phinmatr) && (matr<limbinmatr)) {
phisCollision (&curphisobj->pos, &ph[matr-phinmatr].pos, &curphisobj->force, &ph[matr-phinmatr].force, 9, phisConnectionDeSquizeForce/3);
//phisicInteraction (&curphisobj->force, &ph[matr-phinmatr].force);
return 1;
	}
	else
		if (matr>limbinmatr){
			AnyRagLimb* limb=&LIMBS[matr-limbinmatr];
			phisCollision (&curphisobj->pos, &limb->pos, &curphisobj->force, &limb->force, 9, phisConnectionDeSquizeForce/3);
		}else
		curphisobj->hp-=onImpactHealthLoss;

curphisobj->hp--;

	
if ( (curphisobj->hp<=0) && (curphisobj->voxelPalette)){
SDL_Surface* img=DecayMaskForGuts[rand() % DecayGutsMask];
decayTyles(curphisobj->pos.tx, curphisobj->pos.ty, accurate(curphisobj->pos.x), accurate(curphisobj->pos.y), img, curphisobj->voxelPalette);


int totylex=tylextogrid(curphisobj->pos.tx)+1;
int totyley=tyleytogrid(curphisobj->pos.ty)+1;

RefreshGround (accurate(curphisobj->pos.x)-img->w/2, accurate(curphisobj->pos.y)-img->h/2, img->w, img->h, totylex, totyley);
}

double temprot=curphisobj->force.speedx;
curphisobj->angles.aSpX=((temprot/(accuracy*2)));
temprot=-curphisobj->force.speedy;
curphisobj->angles.aSpY=((temprot/(accuracy*2)));	

	return 1;
}
void ChangeOrientation()
{
curphisobj->angles.Ah=getAngle(curphisobj->force.speedx, curphisobj->force.speedy);
// curphisobj->angles.Ax=getAngle( curphisobj->force.speedx, curphisobj->force.speedh);

}

int hithurt()
{
	if (curphisobj->power<=0) return 1;
	createRadialWind(offx, offy, 1000*20, 3);
	//if (curphisobj->power<=0) curphisobj->hp=-10;
	if (matr==1)
	{
//createRadialWind(offx, offy, 10000, 3);

	curphisobj->hp-=10;
	curphisobj->power=min(curphisobj->power-1, 1);
	//if ((abs(curphisobj->force.speedx)>accuracy/4) && 	(abs(curphisobj->force.speedy)>accuracy/4))
//if (curphisobj->power>0)
//{
createData.tx=curphisobj->pos.tx; createData.ty=curphisobj->pos.ty; createData.x=jhx; createData.y=jhy; createData.h=jhh;
createData.type=804; createData.xspeed=-jspx; createData.yspeed=-jspy;  createData.hspeed=0; //813 - thick blood , 20 - not, 804 - default
createData.randosity=randy*5; createData.numberof=bloodness/4; createData.flooidness=1; createData.slowing=0.96;
createData.lightIntensity=6;
createData.lightPisObj=1;
addphisicalrandom();
createData.type=(rand() % 5)+3;

// Add Bullet hole decay

decayTyles(curphisobj->pos.tx, curphisobj->pos.ty, accurate(jhx)+(rand() % 8) - 4, accurate(jhy)+(rand() % 8) - 4,shootDecay,image[87]);
shootUpperTyles(curphisobj->pos.tx, curphisobj->pos.ty, accurate(jhx)+(rand() % 8) - 4, accurate(jhy)+(rand() % 8) - 4,shootDecay,image[87]);
int totylex=tylextogrid(curphisobj->pos.tx)+1;
int totyley=tyleytogrid(curphisobj->pos.ty)+1;
RefreshGround (accurate(jhx)-shootDecay->w/2, accurate(jhy)-shootDecay->h/2, shootDecay->w, shootDecay->h, totylex, totyley);
 //seEnd


if (phHorizontal) 
{
	createData.mask=0;
	createData.type=(rand() % 5)+3;
	addHitEffect(curphisobj->pos.h);
	smokedrop();
}
//addHitEffect(curphisobj->pos.h);
//}	a
	return 1;
	}

if (matr>limbinmatr){
	AnyRagLimb * limb=&LIMBS[matr-limbinmatr];
	phisicInteractionOneWay (&curphisobj->force, &limb->force);
	curphisobj->data=matr;
curphisobj->collide=beinginsideph;
	createData.mask=0;
	createData.type=(rand() % 5)+3;
addHitEffect(curphisobj->pos.h);

createData.lightPisObj=0;

limb->hp-=100;
curphisobj->force.speedx/=2;
curphisobj->force.speedy/=2;

if (limb->hp<0){
int type=rand() % 4;
int no=addphisicalrandomold(curphisobj->pos.tx,curphisobj->pos.ty, jhx, jhy, curphisobj->pos.h, 860, 0, 0,
	1 //up
	, 0 , 240+type*60, 20-type, 0 ,0, randy*(15-type), 1,10+(rand() % 10));			

ph[no].G_p=1;
ph[no].BlitOnDeath=1;
ph[no].data=13; 
ph[no].g8particle=&mgvDB.mgvarray[13][type].Converted;
ph[no].solidness=0.08;
ph[no].voxelPalette=0;

createData.lightPisObj=0;
createData.type=14 + (rand() % 2);
VoxelBloodBasicSet(&curphisobj->pos,0,0);
createData.x=jhx;
createData.y=jhy;
createData.xspeed=radiusx(accuracy*3);
createData.yspeed=radiusy;
createData.h=accuracy*8;
createData.hspeed=accuracy;
addEffect();

}
redirect=1;
	return 0;
}
else
if (matr>=phinmatr)
{
int	k=matr-phinmatr;

if (k==curphisobj->i) return 0;


/*
createData.tx=curphisobj->pos.tx; createData.ty=curphisobj->pos.ty; createData.x=jhx; createData.y=jhy; createData.h=jhh;
createData.type=813; createData.xspeed=-jspx; 
createData.yspeed=-jspy;  createData.hspeed=0;
createData.randosity=randy*5; createData.numberof=bloodness; createData.flooidness=0; createData.slowing=0.96;
createData.lightPisObj=1;
addphisicalrandom(); */
phisicInteractionOneWay (&curphisobj->force, &ph[k].force);
ph[k].starthp=0;

curphisobj->data=matr;
//curphisobj->power=max(0,curphisobj->power--);
curphisobj->collide=beinginsideph;
//curphisobj->collide=&beinginside;
redirect=1;
//if ((curphisobj->force.mass<1) || (ph[k].force.mass>curphisobj->force.mass))  return 1;
return 0;
	
}

	else
if (matr>=einmatr) 
{	
//	if (curphisobj->force.mass<1) {curphisobj->hp=0; return 1;}
	int k=matr-einmatr;
	epointer=&e[k];
	//if (!k) return 0;
	if (k==curphisobj->casterDNA) 
		return 0;
	if (epointer->hp<obsorbDamPostDeath) return 0; //|| (epointer->type==0)) return 0;

	damage=curphisobj->power*((abs(curphisobj->force.speedx)+abs(curphisobj->force.speedy)+abs(curphisobj->force.speedh)));
	d=0;
	
	if (!epointer->damaged()) return 0;
	else impactslomo++;
	
	//epointer->clock=-1;
	//d=

	offsets punch=AngleAcceleration(0.1 * (rand() % 3600), accuracy*5);

	
	speedLimitCheckI(&epointer->force.speedx, &epointer->force.speedy, &epointer->force.speedh, accuracy*2);
	

	//curphisobj->force.speedx*=0.8;
	//curphisobj->force.speedy*=0.8;
	//Make ADDDDDDDDDD PHISICAL BLOOD*****************************************************************
	//shakeMeBaby(100050);
	
	createData.mask=0;
	createData.type=(rand() % 5)+3;
	//if (splatPerFrame)
	//addHitEffect(epointer->pos.h);
	//splatPerFrame=0;
	//sound
MakeBloodyNoise (offx, offy);
if (soundEffects) PlayMySound(&sounds.bank.impact[(rand() % 9)]);




CastLightForDarkenfFunk (mergeimage[19], screen, offx-mergeimage[19]->w/2, offy-mergeimage[19]->h/2,1);



epointer->clock=epointer->delay;
	curphisobj->data=matr;
	curphisobj->collide=beinginsideenemyp;
	adrtype=71;
	epointer->curstate=e[k].type+2;
	


//createData.hp=144;
	/*
createData.slowing=0.95;
createData.h=accuracy*32;
createData.type=1502 + (rand() % 5); 
createData.xspeed=0;
createData.yspeed=0;
createData.hspeed=0;//(rand() % accuracy) - accuracy/2;
createData.xaccel=0;
createData.yaccel=0;
createData.hp=epointer->pos.h; // hp used for constant height
createData.gravity=1;
createData.MaxLight=rand() % 2;
createData.boolFunction=&belowgroundmuch;
createData.voidfunction=&HeightDrowFormergeimaged;
createData.staticPos=0;
addEffect();*/



	return 0;
	
}

else 
	// ********************** Now Time for the fucking walls
	//if ((w[matr].type>0) && (matr<wlim))
{
// Damage New voxels
	if (w[matr].Megavoxel) {
		if	(DamageWallInGame(&curphisobj->pos, jhx, jhy, jhh, &w[matr], curphisobj->power)){
int sno=addPseudoAnimatedSmoke(&curphisobj->pos, &curphisobj->force, 0.9, &wallSmokeColor, 30, 1 , smokeMaxLight);
smoke[sno].pos.x=jhx;
smoke[sno].pos.y=jhy;

int type=rand() % 7;
createData.lightPisObj=0;
int no=addphisicalrandomold(curphisobj->pos.tx,curphisobj->pos.ty, jhx, jhy, curphisobj->pos.h, 860, 0, 0,
	1 //up
	, 0 , 20+type*4, 40-type*3, 20 ,0, randy*(8-type), 1,10+(rand() % 50));			
ph[no].G_p=1;
ph[no].BlitOnDeath=1;
ph[no].data=2; 
ph[no].g8particle=&mgvDB.mgvarray[11][type].Converted;
ph[no].voxelPalette=image[60];
	}
addDust(&curphisobj->pos, jhx, jhy, impactDustRand*2, -curphisobj->force.speedx , -curphisobj->force.speedy, 0, 5,5, 15);
createData.mask=0;
	createData.type=(rand() % 5)+3;
	addHitEffect(curphisobj->pos.h);
	smokedrop();
curphisobj->power=max(0,curphisobj->power--);
curphisobj->hp=-1;
adrtype=0; 
ja=ji;
curphisobj->collide=beinsideobjectp;
redirect=1;
//addHitEffect(0);

	return 0;
	}





w[matr]._shake.speedx+=jspx*curphisobj->force.mass/w[matr]._shake.mass/5;
w[matr]._shake.speedy+=jspy*curphisobj->force.mass/w[matr]._shake.mass/5;


// **************************** If its a week wall
	if ((curphisobj->solidness>=w[matr].solid) && (w[matr].brick))	
	{
		d=max(curphisobj->solidness-w[matr].solid,1);
		//if (!w[matr].brick) 
		if (!w[matr].brickcopy) 
		{
			w[matr].brickcopy=1; 
		w[matr].brick=SDL_ConvertSurface(w[matr].brick, w[matr].brick->format, SDL_SWSURFACE);
		} 

// if its a week wall
		if (w[matr].solid<curphisobj->solidness/2) 
		{
			int fx=(curphisobj->pos.tx-w[matr].pos.tx)*tyleSize+accurate(jhx)-w[matr].pos.x;
			int fy=(curphisobj->pos.ty-w[matr].pos.ty)*tyleSize+accurate(jhy)-w[matr].pos.y;
			g=fuckwalls(w[matr].brick, fx , fy , d*(curphisobj->G_c==0));
			if	(g>>24)
			{
				//if ((rand() % 50) - 40>0) burn();
			if (imageVoxel[w[matr].type])
	{

fx=SuperFastVoxelGetImpactCoords(imageVoxel[w[matr].type], w[matr].brick->w/2, w[matr].brick->h/2, 0, w[matr].brick, w[matr].angles.Ax,w[matr].angles.Ay,w[matr].angles.Ah, 1, fx, fy,w[matr].gutsout);
fy=fx/w[matr].gutsout->w;
FuckVexelGutsOutDoubleImage(w[matr].gutsout, wallFmask[1], fx % w[matr].gutsout->w , fy,0);	
	}	else
			{
if (InnerWall[w[matr].type-900])
FuckWusingMaskWithInner(w[matr].brick,InnerWall[w[matr].type-900], imgBank.bulletWallFuck,fx-imgBank.bulletWallFuck->w/2 , fy-imgBank.bulletWallFuck->h/2);
else		
FuckWusingMask(w[matr].brick, imgBank.bulletWallFuck,fx-imgBank.bulletWallFuck->w/2 , fy-imgBank.bulletWallFuck->h/2);
	}
			//	w[matr].pos.hp-=curphisobj->power;
				curwall=&w[matr];
				curwall->decay();
				adrtype=0; 
				if (curphisobj->power>0) {

createData.tx=curphisobj->pos.tx; createData.ty=curphisobj->pos.ty; createData.x=jhx; createData.y=jhy; createData.h=jhh;
createData.type=804; createData.xspeed=-jspx; createData.yspeed=-jspy;  createData.hspeed=0; //813 - thick blood , 20 - not, 804 - default
createData.randosity=randy*5; createData.numberof=bloodness/8; createData.flooidness=1; createData.slowing=0.96;
createData.lightPisObj=1;
addphisicalrandom();
createData.type=(rand() % 5)+3;
createData.mask=0;
if (!imageVoxel[w[matr].type])
addHitEffect(w[matr].pos.h*pixInPhis*accuracy);
if (!(rand() % 10))
{
int sno=addPseudoAnimatedSmoke(&curphisobj->pos, &curphisobj->force, 0.9, &wallSmokeColor, 10, 1 , smokeMaxLight);
smoke[sno].pos.x=jhx;
smoke[sno].pos.y=jhy;
}
addDust(&curphisobj->pos, jhx, jhy, impactDustRand*2, -curphisobj->force.speedx , -curphisobj->force.speedy, 0, 5,5, 15);

curphisobj->power=max(0,curphisobj->power--);
//if (curphisobj->power>3) curphisobj->power--;
//else 
curphisobj->collide=beinsideobjectp;

				//	addphisicalrandom(curphisobj->pos.tx, curphisobj->pos.ty, jhx, jhy, jhh , w[matr].impparticle, -jspx, -jspy, 0, 80, randy*5, bloodness/4 ,1, 0.96);
					//addphisicalrandomold(curphisobj->pos.tx,curphisobj->pos.ty, jhx, jhy, jhh, 807, -jspx, -jspy,0,1, 15, 10,0,g, randy*5, 2,100);
				}
				
				//curphisobj->power=0;
				//curphisobj->hp-=10;	
			}
			//else curphisobj->power=1;
			return 0;}  

		else
			// ******************************** if strong wall
	{
		g=fuckwalls(w[matr].brick, (curphisobj->pos.tx-w[matr].pos.tx)*tyleSize+accurate(jhx)-w[matr].pos.x , (curphisobj->pos.ty-w[matr].pos.ty)*tyleSize+accurate(jhy)-w[matr].pos.y, d);
		//if	(g>1) removeshadow (curphisobj);
		if	(g>>24)
		{


			//w[matr].pos.hp-=curphisobj->power;
			curwall=&w[matr];
		
			//curphisobj->hp-=50/(d);	
			curphisobj->power=max(curphisobj->power-1, 0);
			createData.tx=curphisobj->pos.tx; createData.ty=curphisobj->pos.ty; createData.x=jhx; createData.y=jhy; createData.h=jhh;
createData.type=804; createData.xspeed=-jspx; createData.yspeed=-jspy;  createData.hspeed=0; //813 - thick blood , 20 - not, 804 - default
createData.randosity=randy*5; createData.numberof=bloodness/4; createData.flooidness=1; createData.slowing=0.96;
createData.lightPisObj=1;
addphisicalrandom();
createData.type=(rand() % 5)+3;
createData.mask=0;

addHitEffect(w[matr].pos.h*pixInPhis*accuracy);
	curwall->decay();
			//addphisicalrandom(curphisobj->pos.tx, curphisobj->pos.ty, jhx, jhy, jhh , w[matr].impparticle, -jspx, -jspy, 0, 80, randy*9, 4,1 , 0.97); 
			//addphisicalrandomold(curphisobj->pos.tx,curphisobj->pos.ty, jhx, jhy, jhh, 807, -jspx, -jspy,0,1, 15, 10,0,g, randy*5, 2,100);
			//curphisobj->power=0;
		if (curphisobj->hp<0) ja=ji;
		if (curphisobj->power<1)
		{redirect=1;
		return 1;}
		else
			return 0;
		}
		return 0;
	}
	}
	redirect=1;
}
return 1;
}

int beinsideobject()
{
	adrtype=0;
	//if (ji==ja)
	//curphisobj->hp--;
	if (matr==1) {curphisobj->collide=curphisobj->defaultCollide;  return 1;}
	//if ((matr==0) || (w[matr].type==0)) return 1;
	if ((matr>=einmatr) || (!matr)) {curphisobj->collide=&hithurt; 
curphisobj->force.speedx*=bulletPierceSlowing;
curphisobj->force.speedy*=bulletPierceSlowing;
jspx*=bulletPierceSlowing;
jspy*=bulletPierceSlowing;
	return 0;}
		if (w[matr].brick) {
		if (!w[matr].brickcopy)
		{
			w[matr].brickcopy=1;
			w[matr].brick=SDL_ConvertSurface(w[matr].brick, w[matr].brick->format, SDL_SWSURFACE);
		}
			d=max(curphisobj->solidness-w[matr].solid,1);
g=fuckwalls(w[matr].brick, (curphisobj->pos.tx-w[matr].pos.tx)*tyleSize+accurate(jhx)-w[matr].pos.x , (curphisobj->pos.ty-w[matr].pos.ty)*tyleSize+accurate(jhy)-w[matr].pos.y, 0);

		
		
	
	
			if	(g<0x01000000)
			{
				
				//adrtype=0; 
				//if (curphisobj->power>0) {
				/*
createData.tx=curphisobj->pos.tx; createData.ty=curphisobj->pos.ty; createData.x=jhx; createData.y=jhy; createData.h=jhh;
createData.type=w[matr].impparticle; createData.xspeed=-jspx; 
createData.yspeed=-jspy;  createData.hspeed=0;
createData.hp=200;
createData.randosity=randy*5; createData.numberof=bloodness; createData.flooidness=1; createData.slowing=0.92;

		addphisicalrandom();*/
		//curphisobj->pos.tx, curphisobj->pos.ty, jhx, jhy, jhh , w[matr].impparticle, jspx, jspy,0, randy*5, bloodness/4 ,1, 0.98);


		g=fuckwalls(w[matr].brick, (curphisobj->pos.tx-w[matr].pos.tx)*tyleSize+accurate(jhx-jspx)-w[matr].pos.x , (curphisobj->pos.ty-w[matr].pos.ty)*tyleSize+accurate(jhy-jspy)-w[matr].pos.y, d);
	
		curphisobj->collide=curphisobj->defaultCollide;

			}
			}
			//else curphisobj->power=1;
			return 0;
		//}  
}
int smashIntoPieces ()
{
//slowwing=6;
//curphisobj->hp-=1;
int coef=100;
createDirectionalWind(offx, offy, curphisobj->force.speedx/coef , curphisobj->force.speedy/coef,curphisobj->force.speedh/coef, 2);
addPseudoAnimatedSmoke(&curphisobj->pos, &curphisobj->force, 0.9,&CrambleRocksSmoke, 6, 1, smokeMaxLight);
//if (!createData.returnHolder) return 0;
curphisobj->hp=-1;
int loffx=accurate(curphisobj->pos.x) + (rand() % 100) - 50;
loffx=max(loffx,1);
loffx=min(loffx,tyleSize);
int loffy=accurate(curphisobj->pos.y) + (rand() % 100) - 50;
loffy=max(loffy,1);
loffy=min(loffy,tyleSize-3);
//addFireEffect(&curphisobj->pos, &curphisobj->force, 0.85);
VoxelBloodBasicSet(&curphisobj->pos, ((rand() % 5) - 2) , ((rand() % 5) - 2));
createData.hp=25;
createData.hspeed=curphisobj->force.speedh;//*curphisobj->force.pr;
createData.gravity=0.1;//10+rand() % 20;
//createData.gravity/=1000;
createData.type=curphisobj->G_p;
createData.g=curphisobj->g;
createData.xspeed=curphisobj->force.speedx;
createData.yspeed=curphisobj->force.speedy;
createData.hspeed=curphisobj->force.speedh/10;
createData.h=max(curphisobj->pos.h,accuracy*25);
createData.decaySurfaceNo=5+(rand() % 2);
createData.staticPos=0;
createData.displayTime=1;
createData.G_c=5;
addEffect();
hitDropFlame();
//addEffect();
return 0;
}

int beinginsideenemy()
{
	adrtype=0;
	//if (ji==ja)
	//curphisobj->hp-=5;
if (matr!=curphisobj->data)
{
curphisobj->collide=curphisobj->defaultCollide;
d=1;
//int k=curphisobj->data-einmatr;
//epointer=&e[k];
//damage=curphisobj->force.mass*((abs(curphisobj->force.speedx)+abs(curphisobj->force.speedy)+abs(curphisobj->force.speedh)));
//epointer->damaged();
curphisobj->force.speedx*=bulletPierceSlowing;
curphisobj->force.speedy*=bulletPierceSlowing;
jspx*=bulletPierceSlowing;
jspy*=bulletPierceSlowing;
//make exit blood;
if (matr<einmatr) return 1;
}
	return 0;
}
int beinginsidephis()
{
	adrtype=0;
	if (ji==ja)
	curphisobj->hp--;
if (matr!=curphisobj->data)
{
	curphisobj->collide=curphisobj->defaultCollide;
d=1;
}
return 0;
}
int hitblit()
{
//redirect=1;
//curphisobj->hp-=15;
if (!curphisobj->pos.h>>8) 
{
	//setpixel(storedmp[tylextogrid(curphisobj->pos.tx)+1][tyleytogrid(curphisobj->pos.ty)+1], accurate(curphisobj->pos.x), accurate(curphisobj->pos.y), precalcecvator ,curphisobj->size);
	//setpixel(mainmaps[tylextogrid(curphisobj->pos.tx)+1][tyleytogrid(curphisobj->pos.ty)+1], accurate(curphisobj->pos.x), accurate(curphisobj->pos.y), precalcecvator ,curphisobj->size);
	curphisobj->hp=-10;
	ja=ji;
	//return 0;
}
//if (curphisobj->power<2) { curphisobj->power=0; return 0;}
//else 
	//return (curphisobj->power<tyleSize);	
return 0;
}



//*****aftermove
void signin()
{
	shortx=curphisobj->pos.x>>16; 
	shorty=curphisobj->pos.y>>16; 
	shorth=curphisobj->pos.h>>16; 
TryFillrmatrix (curphisobj->pos.tx, curphisobj->pos.ty, shortx, shorty, shorth, curphisobj->i+phinmatr);
}
void PickOrSignIn()
{
// check the distance
if (curphisobj->pos.tx == player.host->pos.tx)
{
	curphisobj->mortal=true;
	curphisobj->hp=-10;
	AddToBag(curphisobj->type, 1);
}
else
{
	shortx=curphisobj->pos.x>>16; 
	shorty=curphisobj->pos.y>>16; 
	shorth=curphisobj->pos.h>>16; 
fillrmatrix (curphisobj->pos.tx, curphisobj->pos.ty, shortx, shorty, shorth, curphisobj->i+phinmatr);
}
}
void PickOnly()
{
if (curphisobj->pos.tx == player.host->pos.tx)
{
	curphisobj->mortal=true;
	curphisobj->hp=-10;
	AddToBag(curphisobj->type, 1);
}
}
void TurnFromProjectileToPickable()
{
	curphisobj->hp--;
	if (curphisobj->hp<0)
	{
	curphisobj->aftermove=&PickOnly;
	curphisobj->angles.aSpX=0; curphisobj->angles.aSpY=0;
	}
}
void FadeLight()
{
	if ((curphisobj->hp>1) && (curphisobj->hp<10))
	{
	curphisobj->light.distanceFading*=curphisobj->hp;
	curphisobj->hp--;
	curphisobj->light.distanceFading/=curphisobj->hp;
	}
	else curphisobj->hp--;
		// maybe fade blue color too;
}
void CastBoomLight()
{
	if ((curphisobj->hp>1) && (curphisobj->hp<10))
	{
	curphisobj->light.distanceFading*=curphisobj->hp;
	curphisobj->hp--;
	curphisobj->light.distanceFading/=curphisobj->hp;
	}
	else curphisobj->hp--;

	int ttype=11+(rand() % 3)+2*(curphisobj->hp<13);//min(4, (25-curphisobj->hp)/4);//
	//if(!(rand() % 4))
	//if (!rand() % 4)
	CastLightForDarkenfFunkNoVolume (shootAnim[ttype], offx-shootAnim[ttype]->w/2, offy-shootAnim[ttype]->h/2, 1);
}
//*******FinalDrow
void NoFinalDrow()
{
}
void VoxelFinalDrow()
{

jhh-=heimod;
jhh/=dddstength;
offx+=((offx-scrwid)*(int)jhh)>>16;
offy+=((offy-scrhid)*(int)jhh)>>16;

	curphisobj->angles.Ax+=curphisobj->angles.aSpX;
	curphisobj->angles.Ay+=curphisobj->angles.aSpY;
	curphisobj->angles.Ah+=curphisobj->angles.aSpH;
	//DrowGParticle(&G_p[curphisobj->G_p], offx, offy, screen, curphisobj->angles.Ax, curphisobj->angles.Ay);
	if (gayVoxel[curphisobj->G_c][curphisobj->G_p].count)
	{
//QuickDrow8bitVex(palette.Buffer8bit,&curphisobj->pos, &gayVoxel[curphisobj->G_c][curphisobj->G_p], 0, 0, 0, 1+(double)accurate(curphisobj->pos.h)/VoxUpResize, curphisobj->angles.Ax, curphisobj->angles.Ay, curphisobj->angles.Ah, PhisObjectsDefault);
//DrowBuffer(gayVoxel[curphisobj->G_c][curphisobj->G_p].palette, offx, offy, screen);

		if (!redirect)
		{
	QuickDrow8bitVex(palette.Buffer8bit,&curphisobj->pos, &gayVoxel[curphisobj->G_c][curphisobj->G_p]
, 0, 0, accurate(curphisobj->pos.h), 1+(double)accurate(curphisobj->pos.h)/VoxUpResize, curphisobj->angles.Ax, curphisobj->angles.Ay, curphisobj->angles.Ah, 0);

		} else 
			{createData.decaySurfaceNo=13;
		QuickDrow8bitVexWithGutsDecay(&gayVoxel[curphisobj->G_c][curphisobj->G_p], accurate(curphisobj->pos.h), 1+(double)accurate(curphisobj->pos.h)/VoxUpResize, 
	curphisobj->angles.Ax, curphisobj->angles.Ay, curphisobj->angles.Ah, offx, offy);	
		}	
		LightsTo180arrayF(&curphisobj->pos, defAngleFade, 0, 0,deAlphaMask,0); 
	DrowBufferByHeight(gayVoxel[curphisobj->G_c][curphisobj->G_p].palette, offx, offy, screen,1);	
	
	}
	else
	SuperFastVoxel360(&G_p[curphisobj->G_c][curphisobj->G_p], offx, offy,128,  screen,curphisobj->angles.Ax, curphisobj->angles.Ay, curphisobj->angles.Ah);
	//DrowVexNohtpBelly(&G_p[curphisobj->G_c][curphisobj->G_p], offx, offy,0,  screen,curphisobj->angles.Ax, curphisobj->angles.Ay, curphisobj->angles.Ah);
	//DrowVex(&G_p[curphisobj->G_p], offx, offy, screen, curphisobj->angles.Ax, curphisobj->angles.Ay, 0);

}
void MegaVoxelFinalDrow()
{

jhh-=heimod;
jhh/=dddstength;
offx+=((offx-scrwid)*(int)jhh)>>16;
offy+=((offy-scrhid)*(int)jhh)>>16;

	curphisobj->angles.Ax+=curphisobj->angles.aSpX;
	curphisobj->angles.Ay+=curphisobj->angles.aSpY;
	curphisobj->angles.Ah+=curphisobj->angles.aSpH;
	//DrowGParticle(&G_p[curphisobj->G_p], offx, offy, screen, curphisobj->angles.Ax, curphisobj->angles.Ay);
	
	if ((curphisobj->g8particle->count) && (!curphisobj->power))
	{
		QuickDrow8bitVex(palette.Buffer8bit,&curphisobj->pos, curphisobj->g8particle
, 0, 0, accurate(curphisobj->pos.h), 1+(double)accurate(curphisobj->pos.h)/VoxUpResize, curphisobj->angles.Ax, curphisobj->angles.Ay, curphisobj->angles.Ah, PhisObjectsDefault);
			LightsTo180arrayF(&curphisobj->pos, defAngleFade, 0, 0,deAlphaMask,0); 
	DrowBufferByHeight(mgvDB.ConvertedPalette[curphisobj->data], offx, offy, screen,1);
	}
	curphisobj->power=0; // To let function know that this piece is drown with the Megavoxel he is a part of
	
	
	//else
	//SuperFastVoxel360(&G_p[curphisobj->G_c][curphisobj->G_p], offx, offy,128,  screen,curphisobj->angles.Ax, curphisobj->angles.Ay, curphisobj->angles.Ah);
	//DrowVexNohtpBelly(&G_p[curphisobj->G_c][curphisobj->G_p], offx, offy,0,  screen,curphisobj->angles.Ax, curphisobj->angles.Ay, curphisobj->angles.Ah);
	//DrowVex(&G_p[curphisobj->G_p], offx, offy, screen, curphisobj->angles.Ax, curphisobj->angles.Ay, 0);

}
void QuickVoxelFinalDrow()
{
int elevation=jhh;
elevation-=heimod;
elevation/=dddstength;
offx+=((offx-scrwid)*elevation)>>16;
offy+=((offy-scrhid)*elevation)>>16;
curphisobj->angles.Ax+=curphisobj->angles.aSpX;
curphisobj->angles.Ay+=curphisobj->angles.aSpY;
curphisobj->angles.Ah+=curphisobj->angles.aSpH;
if (curphisobj->voxelPaletteCopy){
	LightsToSingleValueF(&curphisobj->pos,255);
	QuickDrowVexPalette(&QG_p[curphisobj->G_c][curphisobj->G_p], offx, offy, curphisobj->pos.h, screen, curphisobj->angles.Ax, curphisobj->angles.Ay, curphisobj->angles.Ah, curphisobj->voxelPalette, 0);
}
else
QuickDrowVexNoBelly(&QG_p[curphisobj->G_c][curphisobj->G_p], offx, offy,  screen, curphisobj->angles.Ax, curphisobj->angles.Ay, curphisobj->angles.Ah, curphisobj->g);
//QuickDrowVex(&QG_p[curphisobj->G_c][curphisobj->G_p], offx, offy,256,  screen,1,curphisobj->angles.Ax, curphisobj->angles.Ay, curphisobj->angles.Ah, curphisobj->g);
//	DrowVexNohtpBelly(&G_p[curphisobj->G_c][curphisobj->G_p], offx, offy,0,  screen,curphisobj->angles.Ax, curphisobj->angles.Ay, curphisobj->angles.Ah);
}
void QuickVoxelFinalDrowProjectile()
{
jhh-=heimod;
jhh/=dddstength;
offx+=((offx-scrwid)*(int)jhh)>>16;
offy+=((offy-scrhid)*(int)jhh)>>16;
curphisobj->angles.Ay+=curphisobj->angles.aSpX;
QuickDrowVexXYH(&QG_p[curphisobj->G_c][curphisobj->G_p], offx, offy,0,  screen,curphisobj->angles.Ax, curphisobj->angles.Ay, curphisobj->angles.Ah, curphisobj->g);
//QuickDrowVex(&QG_p[curphisobj->G_c][curphisobj->G_p], offx, offy,256,  screen,1,curphisobj->angles.Ax, curphisobj->angles.Ay, curphisobj->angles.Ah, curphisobj->g);
//	DrowVexNohtpBelly(&G_p[curphisobj->G_c][curphisobj->G_p], offx, offy,0,  screen,curphisobj->angles.Ax, curphisobj->angles.Ay, curphisobj->angles.Ah);
}

// **************** on fly
int signout()
{
	//slowing=50;
	//shortx=curphisobj->pos.x>>16; 
	//shorty=curphisobj->pos.y>>16; 
	//shorth=curphisobj->pos.h>>16; 
	TryClearmatrix (curphisobj->pos.tx, curphisobj->pos.ty, (int)curphisobj->pos.x>>16, (int)curphisobj->pos.y>>16,(int)curphisobj->pos.h>>16, curphisobj->i+phinmatr);

	if ((curphisobj->starthp<=0) && (curphisobj->finalDrow==&VoxelFinalDrow)){
		curphisobj->hp=-10;
		curphisobj->BlitOnDeath=0;
		for (int i=0; i<3; i++){
		int type=rand() % 4;
		createData.lightPisObj=0;
int no=addphisicalrandomold(curphisobj->pos.tx,curphisobj->pos.ty, jhx, jhy, curphisobj->pos.h, 860, 0, 0,
	1 //up
	, 0 , 240+type*60, 20-type, 0 ,0, randy*(10-type), 1,10+(rand() % 10));			

ph[no].G_p=1;
ph[no].BlitOnDeath=1;
ph[no].data=13;//curphisobj->data; 
ph[no].g8particle=&mgvDB.mgvarray[13][type].Converted;
ph[no].solidness=0.08;
ph[no].voxelPalette=0;//mergeimaged[8];

createData.lightPisObj=0;
createData.type=14 + (rand() % 2);//rand() % 8;//14 + (rand() % 2);
VoxelBloodBasicSet(&curphisobj->pos,0,0);
createData.xspeed=radiusx(accuracy*3);
createData.yspeed=radiusy;
createData.h=accuracy*8;
createData.hspeed=accuracy;
addEffect();



		}
	}

return 0;
}
int slowburn()
{

slowwing=6;
curphisobj->hp-=GameSpeed;
int coef=500;
createDirectionalWind(offx, offy, curphisobj->force.speedx/coef , curphisobj->force.speedy/coef,curphisobj->force.speedh/coef, 2);

return 0;
}

int bloodymess()
{
curphisobj->clock++;
slowwing=7;
//Lets make blood go in certain direction


if (curphisobj->pos.h>accuracy) 
{
int dirx=0, diry=0, dirh=-accuracy*4;
rotateVectorI(&dirx, &diry, &dirh, curphisobj->angles.Ah, curphisobj->angles.Ax,curphisobj->angles.Ay);

createData.tx=curphisobj->pos.tx; createData.ty=curphisobj->pos.ty; createData.x=jhx+((rand() % 10)-5)*accuracy; createData.y=jhy+((rand() % 10)-5)*accuracy; createData.h=jhh;
createData.type=813; 
createData.xspeed=curphisobj->force.speedx+dirx;//+((rand() % accuracy) - accuracy/2); 
createData.yspeed=curphisobj->force.speedy+diry;//+((rand() % accuracy) - accuracy/2);
createData.hspeed=dirh;
createData.randosity=30; createData.numberof=(rand() % 20 - 10)*GameSpeed+1; createData.flooidness=0; createData.slowing=0.99;
createData.lightPisObj=0;
addphisicalrandom();

//curphisobj->pos.tx, curphisobj->pos.ty, curphisobj->pos.x, curphisobj->pos.y, curphisobj->pos.h, 813, curphisobj->force.speedx, curphisobj->force.speedy,0, 30, min(rand() % 11 - 8,1),1, 0.99);
createData.type=20; createData.flooidness=0; createData.numberof=(rand() % 10 - 5)*GameSpeed+1;

addphisicalrandom();
//curphisobj->pos.tx, curphisobj->pos.ty, curphisobj->pos.x, curphisobj->pos.y, curphisobj->pos.h, 20, curphisobj->force.speedx, curphisobj->force.speedy,0, 30, min(rand() % 11 - 8,1),1, 0.99);
}
return 0;
}
int heypixel()
{
	//setpixel(screen, accurate(curphisobj->pos.x)- actcamx - (cameratylx - curphisobj->pos.tx)*tyleSize, accurate(curphisobj->pos.y)- actcamy - (cameratyly - curphisobj->pos.ty)*tyleSize, curphisobj->size);
	slowwing=7;
//	setpixelstyle(screen, accurate(curphisobj->pos.x)- actcamx - (cameratylx - curphisobj->pos.tx)*tyleSize, accurate(curphisobj->pos.y)- actcamy - (cameratyly - curphisobj->pos.ty)*tyleSize,curphisobj->pos.h , curphisobj->size);

	//curphisobj->power++;
	return 0;
}
int nothing()
{return 0;
}
void vnothing()
{
//makeblood (100,  jspx,  jspy, 30);
}
void spread()
{
	if ((curphisobj->force.speedh<0) && (curphisobj->pos.h<accuracy*30)) 
	{smashIntoPieces (); curphisobj->hp=-1;}
curphisobj->hp--;
	createDirectionalWind(offx, offy, curphisobj->force.speedx/100, curphisobj->force.speedy/100, 0, 2);
}
// More effect spawners
int DropSmoke()
{

	if (rand() % 5)
	addPseudoAnimatedSmoke(&curphisobj->pos, &curphisobj->force, 0.9,&ExplosionSmokeColor, 9, 0, smokeMaxLight);
	else 
	{
			addDust(&curphisobj->pos, curphisobj->pos.x, curphisobj->pos.y, CloudSpawnRandy, curphisobj->force.speedx , curphisobj->force.speedy, 0, 30,4, 15);
		int sno=addFlameEffect(&curphisobj->pos, 0.6);
		smoke[sno].anglex=rand() % 400;
		smoke[sno].angley=rand() % 400;
		smoke[sno].hp=20;
	}
	return 1;
}
int DropBigSmoke()
{
addDust(&curphisobj->pos, jhx, jhy, CloudSpawnRandy, curphisobj->force.speedx , curphisobj->force.speedy, 0, 20,10, 70);
addDust(&curphisobj->pos, jhx, jhy, CloudSpawnRandy, curphisobj->force.speedx , curphisobj->force.speedy, 0, 25,10, 0);

//addG8particle(&curphisobj->pos);
	
		createData.lightPisObj=0;
		createData.lightIntensity=0;
/*int type=rand() % 8;
int no=addphisicalrandomold(curphisobj->pos.tx,curphisobj->pos.ty, jhx, jhy, curphisobj->pos.h, 860, 0, 0,
	1 //up
	, 0 , 240+type*60, 20-type, 0 ,0, randy*(20-type), 1,10+(rand() % 10));			

ph[no].G_p=1;
ph[no].BlitOnDeath=1;
ph[no].data=12; 
ph[no].g8particle=&mgvDB.mgvarray[12][type].Converted;
ph[no].solidness=0.2;
*/






	int cp=addPseudoAnimatedSmoke(&curphisobj->pos, &curphisobj->force, 0.5,&ExplosionSmokeColor, 30, 1, smokeMaxLight);
	smoke[cp].pos.x=jhx;
	smoke[cp].pos.y=jhy;
	smoke[cp].pos.h=jhh;
	smoke[cp].drowing=&DrowPseudoWithLightBlic;
	smoke[cp].mask=11+(rand() % 3)+2*(curphisobj->hp<13);
	smoke[cp].rotateh=(rand() % 460) + 40; // brightness of blick
	smoke[cp].rotateh/=100;

	curphisobj->hp-=GameSpeed*4;
	return 1;
}
int DropDebrisSmoke()
{
	//add sign out
	signout();
	addDust(&curphisobj->pos, curphisobj->pos.x, curphisobj->pos.y, CloudSpawnRandy, curphisobj->force.speedx , curphisobj->force.speedy, 0, 4,4, 0);
		
	
	return 1;
}

//**combos:
int bloodyinteractive()
{
	bloodymess();
	signout();
	//hitChangeRotation();
	return 1;
}
int hitBurn()
{
createVortex(offx, offy, 300, 5);
burn();
return hithurt();
}
int hurtAndCramble()
{

createData.returnHolder=hithurt();
if ((createData.returnHolder) || ((curphisobj->pos.h<accuracy*30) && (curphisobj->force.speedh<0)))
{
smashIntoPieces ();

curphisobj->hp=-1;
}
return createData.returnHolder;
}
int FlyGutsParticalize()
{
	curphisobj->hp=-1;
	curphisobj->BlitOnDeath=0;
	int numb=4;//rand() % 4+1;
	getitwo=0;
	createData.lightPisObj=0;
for (true; numb>0; numb--)
{
getione =addphisicalrandomold(curphisobj->pos.tx, curphisobj->pos.ty, curphisobj->pos.x+10, curphisobj->pos.y,curphisobj->pos.h, 831, curphisobj->force.speedx,curphisobj->force.speedy,0,1, 300, 40,0 , 0,200, 1,100);
ph[getione].angles.Ax=curphisobj->angles.Ax;	ph[getione].angles.Ay=curphisobj->angles.Ay;	ph[getione].angles.Ah=curphisobj->angles.Ah;	
ph[getione].angles.aSpX=GetRandomRotation(0.02); 
ph[getione].angles.aSpY=GetRandomRotation(0.02);
ph[getione].G_c=9; ph[getione].G_p=rand() % 4;
ph[getione].finalDrow=&VoxelFinalDrow;
ph[getione].foo=&bloodymess;
ph[getione].aftermove=&vnothing; 
ph[getione].collide=&hitChangeRotation; 
if ((getitwo) && (rand() % 2)) linkelements (getione, getitwo, 15, poweroflinks);
getitwo=getione;
}
createData.g=(Uint32*)QGpalette[5]->pixels;
for (int i=0; i<3; i++)
{
createData.type=14+i;

VoxelBloodBasicSet(&curphisobj->pos, ((rand() % 5) - 2), ((rand() % 5) - 2));
createData.xspeed=-curphisobj->force.speedx/300*(rand() % 20 - 10);
createData.yspeed=-curphisobj->force.speedy/300*(rand() % 20 - 10);
createData.staticPos=0;
addEffect();
}

splatter(&curphisobj->pos);
MakegutsNoise (offx, offy);
/*getione =addphisicalrandomold(curphisobj->pos.tx, curphisobj->pos.ty, curphisobj->pos.x+10, curphisobj->pos.y,curphisobj->pos.h, 831, curphisobj->force.speedx,curphisobj->force.speedy,0,1, 300, 40,0 , 0,100, 1,100);
ph[getione].angles.Ax=curphisobj->angles.Ax;	ph[getione].angles.Ay=curphisobj->angles.Ay;	ph[getione].angles.Ah=curphisobj->angles.Ah;	
ph[getione].G_c=9; ph[getione].G_p=1;
ph[getione].finalDrow=&VoxelFinalDrow;
ph[getione].foo=&bloodyinteractive;
ph[getione].aftermove=&signin; 
ph[getione].collide=&hitChangeRotation; */
return 1;
}
int displayDelay()
{
	//ja<=ji
	//adrtype=0;
	ja=curphisobj->displayDelay*ji/100;
	jhx+=jspx*ja;
	jhy+=jspy*ja;
	jhh+=jsph*ja;
	//if ((curphisobj->displayDelay<ja*100/ji) || (ja==ji)) 
		curphisobj->foo=&slowburn;
		return 0;
}
void SetRandRotation(double power)
{
curphisobj->angles.aSpX=GetRandomRotation(power); 
curphisobj->angles.aSpY=GetRandomRotation(power); 
curphisobj->angles.aSpH=GetRandomRotation(power);
}
void SetRandAngles()
{
curphisobj->angles.Ax=GetRandomDoubleFrom(M_PI*2); 
curphisobj->angles.Ay=GetRandomDoubleFrom(M_PI*2); 
curphisobj->angles.Ah=GetRandomDoubleFrom(M_PI*2);
}

void BooMEffects()
{

CastLightForDarkenfFunk (shootAnim[16], screen, offx-shootAnim[16]->w/2, offy-shootAnim[16]->h/2, 1);//PEpoint->hp/255);

createRadialWind(offx, offy, 900000, 40);

int sno=addFlameEffect(&PEpoint->pos, 0.8);
		smoke[sno].anglex=rand() % 400;
		smoke[sno].angley=rand() % 400;
		smoke[sno].hp=40;	








		
		createData.lightIntensity=0.5;
		int no=0;
		if (PEpoint->hp<192){//(rand() % (int)(256-PEpoint->hp)/50)>0) 	{
			createData.lightPisObj=1;
			no=addphisicalrandomold(PEpoint->pos.tx,PEpoint->pos.ty, PEpoint->pos.x,PEpoint->pos.y, accuracy, 853, 0, 0, 1 , 2 , 128, 70, 20 ,0, randy*16, 1,20+(rand()%40));
			if (no)
			{
			ph[no].foo=&DropSmoke;
			ph[no].aftermove=&FadeLight;
			ph[no].type=17;
			} 
		} 	else {
			createData.lightPisObj=0;
			no=addphisicalrandomold(PEpoint->pos.tx,PEpoint->pos.ty, PEpoint->pos.x,PEpoint->pos.y, accuracy, 853, 0, 0, 1 , 0 ,  36, 70, 20 ,0, randy*24, 1,50);
		if (no)
		{
		
			ph[no].foo=&DropBigSmoke; 
			ph[no].aftermove=&CastBoomLight;
			ph[no].type=17;
			
		}
		}
		
	if (no)
	{
		ph[no].pos.h=PEpoint->pos.h;

		ph[no].force.speedh=accuracy+accuracy*(rand() % 10)/10;
	ph[no].collide=&hitDropFlame;
	ph[no].finalDrow=&NoFinalDrow;

	ph[no].light.color=BoomDroppingsLight;
	ph[no].light.dinamic=1;
	ph[no].light.distanceFading=0.01;
	ph[no].light.angleLight=0;
	ph[no].light.brightness=4;
	AddLight(&ph[no].pos, &ph[no].light);
ph[no].defaultCollide=ph[no].collide;

	}



		//addFireEffect(&ph[no].pos, &ph[no].force, 0.85);
createData.lightPisObj=1;
createData.lightIntensity=1;

for (int h=0; h<6; h++)
		addphisicalrandomold(PEpoint->pos.tx,PEpoint->pos.ty, PEpoint->pos.x,PEpoint->pos.y, PEpoint->pos.h+accuracy*4, 17, 0, 0, 1 , 0 , 128, 70, 20 ,0, randy*65, 1,h*2+5);

	//noOfBullets--;
	//}
PEpoint->pos.h+=(rand() % ExplosionoriginHeightRaise) * accuracy;
PEpoint->pos.x+=((rand() % (ExplosionoriginHeightRaise)) - ExplosionoriginHeightRaise/2) * accuracy;
PEpoint->pos.y+=((rand() % (ExplosionoriginHeightRaise)) - ExplosionoriginHeightRaise/2) * accuracy;
	
	/*
SDL_Surface* bimg=imgBank.ScreenBlicSmoke;
VolumetricLightBlit(screen, bimg, offx-bimg->w/2, offy-bimg->h/2 );	*/
PEpoint->light.distanceFading*=max(PEpoint->hp,0);
PEpoint->hp-=healthLossDrop;
if (PEpoint->hp>0)
PEpoint->light.distanceFading/=PEpoint->hp;
//	}
	
}
void addBoomEffect(worldcoordinates* pos, int type, float distanceFading)
{
createData.tx=pos->tx;
createData.ty=pos->ty;
createData.x=jhx; 
createData.y=jhy;
createData.h=jhh;
createData.hp=255;

createData.displayTime=7;
createData.offx=offx;
createData.offy=offy;
createData.mask=createData.type=type;
createData.h=createData.h;
createData.hspeed=0;
createData.gravity=0;
createData.slowing=createData.hp=255;
createData.boolFunction=&timedDeath;
createData.voidfunction=&BooMEffects;//LightBlickDrow;
createData.xaccel=0;
createData.yaccel=0;
createData.xspeed=0;
createData.yspeed=0;
createData.anglex=0;
createData.angley=0;
createData.angleh=0;
createData.staticPos=1;

createData.hspeed=accuracy*2;

createData.light.active=1;
createData.light.color=BoomLight;
createData.light.dinamic=1;
createData.light.distanceFading=distanceFading;
createData.light.brightness=FireBrightnes;
addEffect();
}
int hitboom()
{
//if (curphisobj->hp>0)
//{
bool FlameIndusingImpact=0;

	if ((matr>=einmatr) && (matr<phinmatr) && (e[matr-einmatr].controled)) return 0;


 //16
Uint32* gi=0;
int z=getmematrix (curphisobj->pos.tx, curphisobj->pos.ty, (int)((int)jhx>>16), (int)((int)jhy>>16),(int)((int)jhh>>16)+pixInPhis);
if ((z<einmatr) && (z>matr)) matr=z;
if ((matr>1) && (matr<einmatr) && (w[matr].type)) 
	{
int fx=(curphisobj->pos.tx-w[matr].pos.tx)*tyleSize+accurate(jhx)-w[matr].pos.x;
int fy=(curphisobj->pos.ty-w[matr].pos.ty)*tyleSize+accurate(jhy)-w[matr].pos.y;

if (w[matr].brick) 
{
	if (!w[matr].brickcopy) 
		{
			w[matr].brickcopy=1; 
		w[matr].brick=SDL_ConvertSurface(w[matr].brick, w[matr].brick->format, SDL_SWSURFACE);
		} 
g=fuckwalls(w[matr].brick, fx , fy, 0);
if	(!(g>>24)) return 0;
if (InnerWall[w[matr].type-900])
FuckWusingMaskWithInner(w[matr].brick,InnerWall[w[matr].type-900], wallFmask[0],fx , fy);
else
FuckWusingMask(w[matr].brick, wallFmask[0],fx , fy);
}
FlameIndusingImpact=1;
if (soundEffects) PlayMySound(&sounds.bank.boom[(rand() % 7)]);

//
//gi=(Uint32*)wall[w[matr].type-900]->pixels+wall[w[matr].type-900]->w*wall[w[matr].type-900]->h/2;
	}
else
if (soundEffects) PlayMySound(&sounds.bank.boomFlat[(rand() % 6)]);
/*int sno=addPseudoAnimatedSmoke(&curphisobj->pos, &curphisobj->force, 0.99, &PostExplosionSmoke, 0, 1, smokeMaxLight);
smoke[sno].staticPos=1;
*/
	curphisobj->hp=-1;
	createData.lightPisObj=1;
	createData.lightIntensity=5;
	
	//addphisicalrandomold(curphisobj->pos.tx,curphisobj->pos.ty, jhx, jhy, curphisobj->pos.h, 17, 0, 0, 1 , 0 , 2, 70, 20 ,0, randy*60, rand() % 5,10);
	//addFireEffect(&ph[d].pos, &ph[d].force, 0.995);
	createData.lightPisObj=0;
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Bullets
/*if (!FlameIndusingImpact)
{
int noOfBullets=((rand() % 3) + 8);
	while (noOfBullets)
	{
		createData.lightPisObj=1;
		int no;
		if ((rand() % 5)>0) 	{
			no=addphisicalrandomold(curphisobj->pos.tx,curphisobj->pos.ty, jhx, jhy, curphisobj->pos.h+accuracy*4, 853, 0, 0, 1 , 0 , 2, 70, 20 ,0, randy*20, 1,5);
			ph[no].foo=&DropSmoke;
			ph[no].type=17;
		} 	else {
			no=addphisicalrandomold(curphisobj->pos.tx,curphisobj->pos.ty, jhx, jhy, curphisobj->pos.h+accuracy*4, 853, 0, 0, 1 , 0 , 2, 70, 20 ,0, randy*40, 1,10);
			ph[no].foo=&DropBigSmoke; 
			ph[no].type=810;
		}
		
	
	ph[no].aftermove=&vnothing;
	ph[no].collide=&hitrecohit;
	ph[no].finalDrow=&NoFinalDrow;

	ph[no].light.color=0x00888822;
	ph[no].light.dinamic=1;
	ph[no].light.distanceFading=0.01;
	AddLight(&ph[no].pos, &ph[no].light);
ph[no].defaultCollide=ph[no].collide;





		//addFireEffect(&ph[no].pos, &ph[no].force, 0.85);
//createData.lightPisObj=0;
for (int h=0; h<6; h++)
{
		addphisicalrandomold(curphisobj->pos.tx,curphisobj->pos.ty, jhx, jhy, curphisobj->pos.h+accuracy*4, 17, 0, 0, 1 , 0 , 2, 70, 20 ,0, randy*40, 1,h*2+5);

}
	noOfBullets--;
	}
}*/
//lightStyle (mergeimage[curphisobj->type+1], screen, offx - mergeimage[curphisobj->type+1]->w/2, offy- mergeimage[curphisobj->type+1]->h/2, 0.99	);
//addphisicalrandomold(epointer->pos.tx, epointer->pos.ty, epointer->pos.x+10, epointer->pos.y,epointer->pos.h+accuracy*11, 835, epointer->force.speedx*(rand() % 20 - 10)/5, epointer->force.speedy*(rand() % 20 - 10)/5,0,1, 300, 40,0 , 0,-epointer->hp, 1,100);

	

createData.tx=curphisobj->pos.tx; createData.ty=curphisobj->pos.ty;
createData.x=jhx-35*accuracy; createData.y=jhy-35*accuracy; createData.h=curphisobj->pos.h;
createData.type=844; 
createData.xspeed=createData.yspeed=createData.hspeed=createData.xaccel=createData.yaccel=0;
/*createData.gravity=-0.001;
createData.hp=144;
createData.boolFunction=&timedDeath;
createData.voidfunction=&boomDrow;
createData.staticPos=0;
addEffect();
*/

createData.x=jhx-100*accuracy; createData.y=jhy-100*accuracy;
// ************************************************************** REMOVED BASIC DROWING OF SOMETHING
/*
createData.type=843;
createData.gravity=-0.03;
createData.hp=1599;
createData.boolFunction=&timedDeath;
createData.voidfunction=&basicDrow;
createData.staticPos=0;
addEffect(); */
int loffx, loffy;
impactslomo+=15;
RadialDamage (curphisobj->pos.tx, curphisobj->pos.ty, jhx, jhy, curphisobj->pos.h, rocketRadius, 2000,-1);//curphisobj->casterDNA);
//	smoke[addPseudoAnimatedSmoke(&curphisobj->pos, &curphisobj->force, 0.1, &EnemyImpactColor, 1, 1, 64 )].staticPos=1;
//FlameIndusingImpact=1;
const int CopySize=30;

PosToGrid(&lgx,&lgy,&lx,&ly, &curphisobj->pos);
lx=min(tyleSize-CopySize*2,max(lx,CopySize));
ly=min(tyleSize-CopySize*2,max(ly,CopySize));
/*
if (matr==1)
for (int u=rand() % 8; u<15; u++)
{
loffx=accurate(jhx) + (rand() % 100) - 50;
loffx=max(loffx,1);
loffx=min(loffx,tyleSize);
loffy=accurate(jhy) + (rand() % 100) - 50;
loffy=max(loffy,1);
loffy=min(loffy,tyleSize-3);
//if (!gi)
gi=(Uint32*)mainmaps[tylextogrid(curphisobj->pos.tx)+1][tyleytogrid(curphisobj->pos.ty)+1]->pixels+loffy*tyleSize+loffx;

int r=addphisicalrandomold(curphisobj->pos.tx,curphisobj->pos.ty, jhx, jhy, curphisobj->pos.h+accuracy*9, 849, 0, 0, 1 , 1 , 300, 70, 20 ,gi, randy*10, 1,600);
ph[r].G_c=5; ph[r].G_p=8+(rand() % 4);
ph[r].pos.x+=ph[r].force.speedx*5;
ph[r].pos.y+=ph[r].force.speedy*5;
ph[r].force.speedh+=accuracy*2;
ph[r].voxelPalette=CopyIntoNewTexture(storedmp[lgx+1][lgy+1], lx+(rand() % (CopySize*2))-CopySize, ly+(rand() % (CopySize*2))-CopySize, CopySize, CopySize);
ph[r].voxelPaletteCopy=1;
}*/


createData.type=102;
ToGroundNoRedrowHeight (curphisobj->pos.tx, curphisobj->pos.ty, (int)((int)jhx>>16), (int)((int)jhy>>16), (int)(curphisobj->pos.h>>16) , 102 );

if  (PosOnScreen(&curphisobj->pos, 100))
{
createData.x=jhx; createData.y=jhy;
createData.h=accuracy*5;
createData.type=7; //image[7]
createData.maskIMG=image[7]; 
createData.xspeed=0;
createData.yspeed=0;
createData.xaccel=0;
createData.yaccel=0;
createData.hspeed=accuracy*1.48;
//createData.gravity=30+rand() % 40;
//case 2: TuneDouble(&tempx, 0.01, "smoke acceleration up", 0.01, 100); break;
//	case 3: TuneDouble(&tempy, 0.01, "smoke gravity", 0.01, 100); break;
createData.MaxLight=8;
createData.gravity=1.4;///=400;
createData.hp=0;//curphisobj->pos.h;
createData.boolFunction=&belowgroundmuch;
createData.voidfunction=&TrueHeightDrow;
createData.staticPos=0;
addEffect();



loffx=accurate(jhx);
loffx=max(loffx,1);
loffx=min(loffx,tyleSize);
loffy=accurate(jhy);
loffy=max(loffy,1);
loffy=min(loffy,tyleSize-3);


createData.hspeed=accuracy;
for (int i=0; i<3; i++){
VoxelBloodBasicSet(&curphisobj->pos, ((rand() % 10) - 5), ((rand() % 10) - 5));
createData.type=14 + (rand() % 2);//rand() % 8;//14 + (rand() % 2);
createData.decaySurfaceNo=5+(rand() % 2);
createData.g=tera.DecayBG[createData.decaySurfaceNo].pixels+loffy*tyleSize+loffx;
	//(Uint32*)mainmaps[tylextogrid(curphisobj->pos.tx)+1][tyleytogrid(curphisobj->pos.ty)+1]->pixels+loffy*tyleSize+loffx;

createData.xspeed=radiusx (accuracy*3);//curphisobj->force.speedx*(rand() % 10 - 5)/50;
createData.yspeed=radiusy;//curphisobj->force.speedy*(rand() % 10 - 5)/50;

createData.staticPos=0;
int ret=addEffect();
smoke[ret].pos.x=jhx;
smoke[ret].pos.y=jhy;
}
//CastLightForDarkenfFunk (image[79], screen, offx-image[79]->w/2, offy-image[79]->h/2,4);

//Displacement (displacement[6], curphisobj->pos.tx, curphisobj->pos.ty, accurate(curphisobj->pos.x)-displacement[6]->w/2, accurate(curphisobj->pos.y)-displacement[6]->h/2, 1 );

createData.displayTime=10;
createData.h=curphisobj->pos.h;
addBoomEffect(&curphisobj->pos, 11, BoomDistanceFade);
//addBlicEffect(offx, offy,11, 0.00073);
shakeMeBaby(120000);

//int doffx=offx-displacement[6]->w/2;
//int doffy=offy-displacement[6]->h/2;
/*createData.offx=offx;
createData.offy=offy;
createData.boolFunction=&timedDeath;
createData.voidfunction=&DisplacementDrow;
createData.hp=100;
createData.type=1;
createData.staticPos=1;
addEffect(); */// displacement
//DisplacementStyle (screen, doffx + 10, doffy + 10, pixInPhis, displacement[6], doffx, doffy);
// release fire:

addDisplacementEffect(&curphisobj->pos);

if (FlameIndusingImpact)
for (int i=0; i<2; i++)
		addFireEffectRandom(&curphisobj->pos, 0.95, fireTrowRandosityOnExplosion );
}
adrtype=6;
ja=ji;
//remove flame effect associated:
//smoke[curphisobj->data].hp=0;
return 1;
}
int SmallHitBoom()
{
//if (curphisobj->hp>0)
//{
bool FlameIndusingImpact=0;

	if ((matr>=einmatr) && (matr<phinmatr) && (e[matr-einmatr].controled)) return 0;

	addDust(&curphisobj->pos, jhx, jhy, CloudSpawnRandy*5, 0 , 0, 0, 15,5, 32);
smokedrop();

if (matr>=phinmatr)
{
int	k=matr-phinmatr;

if (k==curphisobj->i) return 0;

phisicInteractionOneWay (&curphisobj->force, &ph[k].force);
ph[k].starthp=0;
}

 //16
Uint32* gi=0;
int z=getmematrix (curphisobj->pos.tx, curphisobj->pos.ty, (int)((int)jhx>>16), (int)((int)jhy>>16),(int)((int)jhh>>16)+pixInPhis);
if ((z<einmatr) && (z>matr)) matr=z;

	curphisobj->hp=-1;

	createData.lightPisObj=1;
	createData.lightIntensity=5;
	

	createData.lightPisObj=0;



createData.tx=curphisobj->pos.tx; createData.ty=curphisobj->pos.ty;
createData.x=jhx-35*accuracy; createData.y=jhy-35*accuracy; createData.h=curphisobj->pos.h;
createData.type=844; 
createData.xspeed=createData.yspeed=createData.hspeed=createData.xaccel=createData.yaccel=0;
createData.x=jhx-100*accuracy; createData.y=jhy-100*accuracy;

int loffx, loffy;
RadialDamage (curphisobj->pos.tx, curphisobj->pos.ty, jhx, jhy, curphisobj->pos.h, 16, 2000,-1);//curphisobj->casterDNA);

RadialDamage (curphisobj->pos.tx, curphisobj->pos.ty, jhx, jhy, curphisobj->pos.h, 80, 200,-1);

const int CopySize=30;

PosToGrid(&lgx,&lgy,&lx,&ly, &curphisobj->pos);
lx=min(tyleSize-CopySize*2,max(lx,CopySize));
ly=min(tyleSize-CopySize*2,max(ly,CopySize));

if  (PosOnScreen(&curphisobj->pos, 100))
{
createData.x=jhx; createData.y=jhy;
createData.h=accuracy*5;
createData.type=7; //image[7]
createData.maskIMG=image[7]; 
createData.xspeed=0;
createData.yspeed=0;
createData.xaccel=0;
createData.yaccel=0;
createData.hspeed=accuracy*1.48;
createData.MaxLight=8;
createData.gravity=1.4;
createData.hp=0;




createData.type=12;

loffx=accurate(jhx);
loffx=max(loffx,1);
loffx=min(loffx,tyleSize);
loffy=accurate(jhy);
loffy=max(loffy,1);
loffy=min(loffy,tyleSize-3);

createData.displayTime=10;
createData.h=curphisobj->pos.h;

shakeMeBaby(20000);

createRadialWind(offx, offy, 500000, 15);


int cp=addPseudoAnimatedSmoke(&curphisobj->pos, &curphisobj->force, 0.5,&ExplosionSmokeColor, 10, 1, smokeMaxLight);
smoke[cp].pos.x=jhx;
smoke[cp].pos.y=jhy;
	smoke[cp].drowing=&DrowPseudoWithLightBlic;
	smoke[cp].mask=11+(rand() % 3)+2*(curphisobj->hp<13);
	smoke[cp].rotateh=(rand() % 50) + 50; // brightness of blick
	smoke[cp].rotateh/=100;
}
adrtype=6;
ja=ji;
return 1;
}

int phisfuncassigner()
{
	curphisobj->foo=&slowburn;
	curphisobj->aftermove=&vnothing;
	curphisobj->collide=&hitrecohit;
	curphisobj->finalDrow=&NoFinalDrow;
//	curphisobj->angles.aSpH=0;
	
switch (curphisobj->type) 
{
case 17: {curphisobj->foo=&displayDelay; curphisobj->collide=&hithurt;
	Uint32 intensity=(rand() % 28) + 160;
	curphisobj->light.color=intensity/3+(intensity<<8) + (intensity<<16);//0x00888822;
	curphisobj->light.dinamic=1;
	curphisobj->light.distanceFading=0.01;
	curphisobj->light.angleLight=0;
	curphisobj->light.brightness=4;
	AddLight(&curphisobj->pos, &curphisobj->light);
		 } break;
case 18: {curphisobj->foo=&displayDelay; curphisobj->collide=&SmallHitBoom;
	Uint32 intensity=(rand() % 28) + 160;
	curphisobj->light.color=intensity/3+(intensity<<8) + (intensity<<16);//0x00888822;
	curphisobj->light.dinamic=1;
	curphisobj->light.distanceFading=0.01;
	curphisobj->light.angleLight=0;
	curphisobj->light.brightness=3;
	AddLight(&curphisobj->pos, &curphisobj->light);
	curphisobj->type=17;
		 } break;
//******************************************** GUTS particles
case 23:
case 27:

case 35:
//{curphisobj->foo=&bloodymess; curphisobj->collide=&hitChangeRotation;  //curphisobj->aftermove=&signin; 
//curphisobj->G_p=0;  curphisobj->G_c=5; curphisobj->angles.Ax=0; curphisobj->angles.Ay=0; curphisobj->angles.aSpX=GetRandomRotation(0.001); curphisobj->angles.aSpY=GetRandomRotation(0.001); } break;
		{curphisobj->foo=&bloodymess;	 } break;
case 30:
{curphisobj->foo=&bloodymess;	curphisobj->collide=&hitChangeRotation;  //curphisobj->aftermove=&signin; 
curphisobj->finalDrow=&VoxelFinalDrow; curphisobj->G_p=4;  
 } break;

case 860: 
	{
curphisobj->foo=&DropDebrisSmoke;
curphisobj->aftermove=&signin;//&vnothing;//
curphisobj->collide=&hitDebris;
curphisobj->finalDrow=&MegaVoxelFinalDrow;
SetRandRotation(curphisobj->solidness);
SetRandAngles();
//curphisobj->foo=&signout;	curphisobj->aftermove=&signin;
break;
	}

//case 807: {curphisobj->foo=&heypixel; curphisobj->collide=&hitblit;} break;
case 810: 
	{curphisobj->foo=&DropSmoke; curphisobj->collide=&hitboom; 
curphisobj->finalDrow=&VoxelFinalDrow; curphisobj->G_p=3; 
SetRandRotation(0.05); 
curphisobj->light.color=0x00776622;
	curphisobj->light.dinamic=1;
	curphisobj->light.distanceFading=0.0033;
	curphisobj->light.angleLight=0;
	curphisobj->light.brightness=4;
	AddLight(&curphisobj->pos, &curphisobj->light);	
	
	} break;
case 819:
{curphisobj->foo=&bloodymess;	curphisobj->collide=&hitrecohit; } break;
case 822: // Barell:
	{curphisobj->foo=&signout;	curphisobj->collide=&basic; curphisobj->aftermove=&signin; curphisobj->mortal=false; curphisobj->BlitOnDeath=false;} break;
case 828:
	//{curphisobj->foo=&bloodymess;	curphisobj->collide=&hitrecohit; } break;
{curphisobj->foo=&bloodymess;	curphisobj->collide=&hitChangeRotation;   
curphisobj->finalDrow=&VoxelFinalDrow; curphisobj->G_p=2; SetRandRotation(0.05);  } break;
case 831:
{curphisobj->foo=&bloodymess;	curphisobj->collide=&hitrecohit; 
curphisobj->finalDrow=&VoxelFinalDrow; curphisobj->G_p=8;SetRandRotation(0.05); } break;
//MINIGUN
case 834:
	{ curphisobj->finalDrow=&VoxelFinalDrow; curphisobj->G_p=1; curphisobj->G_c=3; 
	SetRandRotation(0.005); 
		curphisobj->foo=&signout;	curphisobj->collide=&hitChangeRotation; curphisobj->aftermove=&PickOrSignIn; curphisobj->mortal=false; curphisobj->BlitOnDeath=false;} break;
//no image voxels
case 835:
	{curphisobj->type=831;  curphisobj->foo=&bloodyinteractive; curphisobj->aftermove=&signin; 	curphisobj->collide=&hitChangeRotation; 
	curphisobj->finalDrow=&VoxelFinalDrow;  SetRandRotation(0.002); 
	curphisobj->G_p=3; curphisobj->G_c=2;
	} break;
case 836:
	{curphisobj->type=831;  curphisobj->foo=&bloodyinteractive; curphisobj->aftermove=&signin; 	curphisobj->collide=&hitChangeRotation; 
	curphisobj->finalDrow=&VoxelFinalDrow; curphisobj->G_p=4; curphisobj->G_c=2;  SetRandRotation(0.005); } break;
case 837:
	{curphisobj->type=831;  curphisobj->foo=&bloodyinteractive; curphisobj->aftermove=&signin; 	curphisobj->collide=&hitChangeRotation; 
	curphisobj->finalDrow=&VoxelFinalDrow; curphisobj->G_p=5; curphisobj->G_c=2; SetRandRotation(0.005); } break;

case 838:
	{curphisobj->type=837; curphisobj->G_p=1; curphisobj->foo=&slowburn;	curphisobj->collide=&hithurt; curphisobj->mortal=true; curphisobj->BlitOnDeath=false;
	curphisobj->finalDrow=&VoxelFinalDrow; SetRandRotation(0.005);  } break;
case 849:  // for any generic voxel
	{
curphisobj->aftermove=spread;
 //curphisobj->G_c=5; curphisobj->G_p=8+(rand() % 4);
 curphisobj->foo=&nothing;	curphisobj->collide=&hurtAndCramble; curphisobj->finalDrow=&QuickVoxelFinalDrow;
SetRandRotation(0.001);   
	}break;
case 852:
	{
		curphisobj->foo=&slowburn; 
		//Generic voxel white (drop 18)
		curphisobj->angles.Ah=getAngle(curphisobj->force.speedx, curphisobj->force.speedy);
		curphisobj->angles.Ax=curphisobj->angles.Ay=0;
		//
		//double adir=getAngle(player.host->force.speedx, player.host->force.speedy)+0.8;
	}
	//curphisobj->G_p=0;  curphisobj->G_c=5;
	
}

	// Collide return 1 to recoshet from ground 0 only if enemy is pierced

curphisobj->defaultCollide=curphisobj->collide;
return 0;
}

//******************************************** Normal phisics
void moveph (part* bo, int i)
{
curphisobj=curphisobj->next;

if (bo->prev==NULL)  
	{
		phfirst[i]=bo->next; 
		if (bo->next)
		bo->next->prev=NULL; 
	}
	else
	{
		bo->prev->next=bo->next;
		if (bo->next)
		bo->next->prev=bo->prev;
	}

	hig=accurate(bo->pos.h)/pixInPhis+matrdepth;
	hig=min(hig,matrhig+matrdepth-1);

	if (hig<0) { hig=0; bo->hp=-10; }
	// hig=matrhig+matrdepth-1;
		if (phfirst[hig]!=NULL) phfirst[hig]->prev=bo;
		if (hig>=i) bo->skip=1;
		bo->prev=NULL;
		bo->next=phfirst[hig];
		
		phfirst[hig]=bo; 

}
void removeph (part* bo, int i)
{
if (curphisobj->voxelPaletteCopy) SDL_FreeSurface(curphisobj->voxelPalette);	
curphisobj->light.active=0;
curphisobj=curphisobj->next;


	if (bo->prev==NULL)  
	{
		phfirst[i]=bo->next; 
		if (bo->next!=0)
		bo->next->prev=NULL; 
	}
	else
	{
		bo->prev->next=bo->next;
		if (bo->next!=NULL)
		bo->next->prev=bo->prev;
	}
		bo->type=0;
	
}
void phisBlitting()
{
normalizeCoordinates(&curphisobj->pos);

int dx=accurate(curphisobj->pos.x);
int dy=accurate(curphisobj->pos.y);
int gdx=dx;
int gdy=dy;
int wid=24, hid=24;

G8_Particle* g8Drow=0;
Uint32* palettePoint=0;
if (curphisobj->g8particle) {
	g8Drow=curphisobj->g8particle; 
	palettePoint=mgvDB.ConvertedPalette[curphisobj->data]; 
} 
else if ((curphisobj->G_p) && (gayVoxel[curphisobj->G_c][curphisobj->G_p].count)) { 
	g8Drow=&gayVoxel[curphisobj->G_c][curphisobj->G_p];
	palettePoint=gayVoxel[curphisobj->G_c][curphisobj->G_p].palette;

} 

if (g8Drow){
QuickDrow8bitVex(palette.Buffer8bit,&curphisobj->pos, g8Drow, 0, 0, 0, 1, curphisobj->angles.Ax, curphisobj->angles.Ay, curphisobj->angles.Ah, 0);
LightsTo180arrayF(&curphisobj->pos, defAngleFade, 0, 0,deAlphaMask,0); 
/*wid=palette.G8BufferRadius*2;
hid=palette.G8BufferRadius*2;
dx-=palette.G8BufferRadius;
dy-=palette.G8BufferRadius;
gdx+=palette.G8BufferRadius;
gdy+=palette.G8BufferRadius;*/
if (dx<32) {curphisobj->pos.tx-=1; dx+=tyleSize; gdx+=tyleSize;}
if (dy<32) {curphisobj->pos.ty-=1; dy+=tyleSize; gdy+=tyleSize;}
} else
if (image[curphisobj->type]){
wid=image[curphisobj->type]->w;
hid=image[curphisobj->type]->h;
dx-=image[curphisobj->type]->w/2;
dy-=image[curphisobj->type]->h/2;
gdx+=image[curphisobj->type]->w/2;
gdy+=image[curphisobj->type]->h/2;
} 

if ((!palettePoint) && (g8Drow))
	return;



//dy+= (1 - dy % 2) * stereo;
int totylex=tylextogrid(curphisobj->pos.tx)+1;
int totyley=tyleytogrid(curphisobj->pos.ty)+1;
int ra=0;


if (curphisobj->G_p)
{
	if (g8Drow)
	{
	if (image[curphisobj->type])
apply_surface( dx, dy, image[curphisobj->type], storedmp[totylex][totyley] ); 
	ra=g8Drow->radius;
//EmptyLight180Array();

//QuickDrow8bitVex(palette.Buffer8bit,&curphisobj->pos, g8Drow, 0, 0, 0, 1, curphisobj->angles.Ax, curphisobj->angles.Ay, curphisobj->angles.Ah, PhisObjectsDefault);
BufferToGround(palettePoint, gdx, gdy, storedmp[totylex][totyley]);
	}
	else
	{
ra=G_p[curphisobj->G_c][curphisobj->G_p].radius;
apply_surface( dx, dy, image[curphisobj->type], storedmp[totylex][totyley] ); 
MergeVexel(&G_p[curphisobj->G_c][curphisobj->G_p], gdx, gdy,0, storedmp[totylex][totyley],curphisobj->angles.Ax, curphisobj->angles.Ay, curphisobj->angles.Ah);
	}
RefreshGround (dx-ra, dy-ra, ra*2, ra*2, totylex, totyley);
}
else
tolendstyle (dx, dy, curphisobj->type, totylex, totyley);


int d=(tyleSize-wid);
int z=(tyleSize-hid);

if (dx>d) 
{

totylex=tylextogrid(curphisobj->pos.tx+1)+1;
totyley=tyleytogrid(curphisobj->pos.ty)+1;
if (curphisobj->G_p)
{
if (image[curphisobj->type])
apply_surface( dx-tyleSize, dy, image[curphisobj->type], storedmp[totylex][totyley] ); 
if (g8Drow)
	{
//EmptyLight180Array();
//QuickDrow8bitVex(palette.Buffer8bit,&curphisobj->pos, g8Drow, 0, 0, 0, 1, curphisobj->angles.Ax, curphisobj->angles.Ay, curphisobj->angles.Ah, PhisObjectsDefault);
BufferToGround(palettePoint, gdx-tyleSize, gdy, storedmp[totylex][totyley]);
	}
	else
MergeVexel(&G_p[curphisobj->G_c][curphisobj->G_p], gdx-tyleSize, gdy,0, storedmp[totylex][totyley],curphisobj->angles.Ax, curphisobj->angles.Ay, curphisobj->angles.Ah);
RefreshGround (dx-ra-tyleSize, dy-ra, ra*2, ra*2, totylex, totyley);
}
else
tolendstyle (dx-tyleSize, dy, curphisobj->type, totylex, totyley);

//if (curphisobj->G_p)
//MergeGParticle(&G_p[curphisobj->G_c][curphisobj->G_p], gdx-tyleSize, gdy, curphisobj->angles.Ax, curphisobj->angles.Ay,tylextogrid(curphisobj->pos.tx+1)+1, tyleytogrid(curphisobj->pos.ty)+1);
//tolendstyle (dx-tyleSize, dy, curphisobj->type, tylextogrid(curphisobj->pos.tx+1)+1, tyleytogrid(curphisobj->pos.ty)+1);
}

if (dy>z) 
{

totylex=tylextogrid(curphisobj->pos.tx)+1;
totyley=tyleytogrid(curphisobj->pos.ty+1)+1;
if (curphisobj->G_p)
{
		if (image[curphisobj->type])
apply_surface( dx, dy-tyleSize, image[curphisobj->type], storedmp[totylex][totyley] ); 
		if (g8Drow)
	{
//EmptyLight180Array();

BufferToGround(palettePoint, gdx, gdy-tyleSize, storedmp[totylex][totyley]);
	}
	else
MergeVexel(&G_p[curphisobj->G_c][curphisobj->G_p], gdx, gdy-tyleSize,0, storedmp[totylex][totyley],curphisobj->angles.Ax, curphisobj->angles.Ay, curphisobj->angles.Ah);
RefreshGround (dx-ra, dy-ra-tyleSize, ra*2, ra*2, totylex, totyley);
}
else
tolendstyle (dx, dy-tyleSize, curphisobj->type, totylex, totyley);

//if (curphisobj->G_p)
//MergeGParticle(&G_p[curphisobj->G_c][curphisobj->G_p], gdx, gdy-tyleSize, curphisobj->angles.Ax, curphisobj->angles.Ay,tylextogrid(curphisobj->pos.tx)+1, tyleytogrid(curphisobj->pos.ty+1)+1);
//tolendstyle (dx, dy-tyleSize, curphisobj->type, tylextogrid(curphisobj->pos.tx)+1, tyleytogrid(curphisobj->pos.ty+1)+1);
}

if ((dy>z) && (dx>d))  
{

totylex=tylextogrid(curphisobj->pos.tx+1)+1;
totyley=tyleytogrid(curphisobj->pos.ty+1)+1;
if (curphisobj->G_p)
{
		if (image[curphisobj->type])
apply_surface( dx-tyleSize, dy-tyleSize, image[curphisobj->type], storedmp[totylex][totyley] ); 
if (g8Drow)
	{
//EmptyLight180Array();
//QuickDrow8bitVex(palette.Buffer8bit,&curphisobj->pos, g8Drow, 0, 0, 0, 1, curphisobj->angles.Ax, curphisobj->angles.Ay, curphisobj->angles.Ah, PhisObjectsDefault);
BufferToGround(palettePoint, gdx-tyleSize, gdy-tyleSize, storedmp[totylex][totyley]);
	}
	else
MergeVexel(&G_p[curphisobj->G_c][curphisobj->G_p], gdx-tyleSize, gdy-tyleSize,0, storedmp[totylex][totyley],curphisobj->angles.Ax, curphisobj->angles.Ay, curphisobj->angles.Ah);
RefreshGround (dx-ra-tyleSize, dy-ra-tyleSize, ra*2, ra*2, totylex, totyley);
}
else
tolendstyle (dx-tyleSize, dy-tyleSize, curphisobj->type, totylex, totyley);

//if (curphisobj->G_p)
//MergeGParticle(&G_p[curphisobj->G_c][curphisobj->G_p], gdx-tyleSize, gdy-tyleSize, curphisobj->angles.Ax, curphisobj->angles.Ay,tylextogrid(curphisobj->pos.tx+1)+1, tyleytogrid(curphisobj->pos.ty+1)+1);
//tolendstyle (dx-tyleSize, dy-tyleSize, curphisobj->type, tylextogrid(curphisobj->pos.tx+1)+1, tyleytogrid(curphisobj->pos.ty+1)+1);
}//CleanBuffer()
if (g8Drow) CleanBuffer();
}
void nphisicsfucktion (part* pOne, int lvl)//int lvl, )
{
float reco;
int i,j;
	curphisobj=pOne;//phfirst[lvl];
	//if (!curphisobj->type) pOne=0;
while (curphisobj)
{
	if (curphisobj->skip) {curphisobj->skip=0; curphisobj=curphisobj->next; continue;}
testCountPhisics++;
offx=accurate(curphisobj->pos.x) - actcamx - (cameratylx - curphisobj->pos.tx)*tyleSize;
offy=accurate(curphisobj->pos.y) - actcamy - (cameratyly - curphisobj->pos.ty)*tyleSize;
int shx=accurate(curphisobj->pos.h*timeofdayx)+offx; //timeofdayx=1, timeofdayy=0.3;
int shy=accurate(curphisobj->pos.h*timeofdayy)+offy;
if (mergeimage[curphisobj->type])
{

CastShadowForDarkenfFunk(mergeimage[curphisobj->type],screen,offx+accurate(curphisobj->pos.h),offy-mergeimage[curphisobj->type]->h/2, curphisobj->pos.h,0);
}else
		if ((!curphisobj->light.active) && (curphisobj->pos.h>accuracy))
		DrowMyIndShadow (&imgBank.DefaultShadow, shx-imgBank.DefaultShadow.w/2, shy-imgBank.DefaultShadow.h/2);




ji=abs(curphisobj->force.speedy);
if (abs(curphisobj->force.speedx)>ji) ji=abs(curphisobj->force.speedx);
ji*=GameSpeed;
if (ji<blurb)
drtype=curphisobj->type+1;
	else 
		drtype=curphisobj->type+2;
if (image[curphisobj->type+1])
ji=accurate((ji*2/image[curphisobj->type+1]->h));
else 
	ji=accurate((ji*2/8));

if (ji<2) ji=2;
 jhx=curphisobj->pos.x;
 jspx=curphisobj->force.speedx*GameSpeed;
 
 offx=accurate(jhx-jspx) - actcamx - (cameratylx - curphisobj->pos.tx)*tyleSize;
 jhy=curphisobj->pos.y;
 jspy=curphisobj->force.speedy*GameSpeed;
 
 offy=accurate(jhy-jspy) - actcamy - (cameratyly - curphisobj->pos.ty)*tyleSize;
 jhh=curphisobj->pos.h;
 jsph=curphisobj->force.speedh*GameSpeed/ji;
 redirect=0;
 ja=1;
curphisobj->foo();

if ((offx<-500) || (offx>scrwi+500) || (offy<-500) || (offy>scrhi+500)) curphisobj->hp=-10;
/*
matr=getmematrix (curphisobj->pos.tx, curphisobj->pos.ty, jhx>>16, jhy>>16,jhh>>16);
if ((matr) && (matr!=curphisobj->i+phinmatr)) {
	if (matr<phinmatr) {curphisobj->pos.x+=jspx; curphisobj->pos.y+=jspy;} 
	if (curphisobj->pos.h<0) curphisobj->pos.h+=accuracy; }
else 
{*/

jspx/=ji;
jspy/=ji;




/*if (ji>2) 
{	
int elevation=jhh-heimod;
elevation/=dddstength;
offx+=((offx-scrwid)*elevation)>>16;
offy+=((offy-scrhid)*elevation)>>16;	
	
drowstyle(offx-image[curphisobj->type+1]->w/2,offy-image[curphisobj->type+1]->h/2,curphisobj->pos.h-jsph-heimod,curphisobj->type+1);

}*/

// ******************************************************** DO NOT DELETE
/*if ((mergeimage[curphisobj->type+2]!=0) && (curphisobj->type>1)) 
	lightStyle (mergeimage[curphisobj->type+2], screen, offx - mergeimage[curphisobj->type+2]->w/2, offy- mergeimage[curphisobj->type+2]->h/2, (float)((float)curphisobj->hp/curphisobj->starthp));
	*/
//int visualdx=shake.cameraDisplacementPerFrame.x

for (true;ja<=ji;ja++)
{
//	if (curphisobj->foo==displayDelay) 
//		ji=ji;
	adrtype=drtype;

jhx+=jspx;
jhy+=jspy;
jhh+=jsph;
if (getmematrix (curphisobj->pos.tx, curphisobj->pos.ty, (int)((int)(jhx)>>16), (int)((int)(jhy)>>16),(int)((int)(jhh)>>16)))
{
	phHorizontal=1;
	jhy-=jspy;
	jhh-=jsph;
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX HIT

if ((matr=getmematrix (curphisobj->pos.tx, curphisobj->pos.ty, (int)((int)jhx>>16), (int)((int)jhy>>16),(int)((int)jhh>>16))) && (curphisobj->collide()))
	{
		reco=rand() % 100 +50;
		reco/=10000;
		jhx-=jspx;
		jspx=-(jspx*(curphisobj->force.pr))*reco;
curphisobj->force.speedx=-(curphisobj->force.speedx*(curphisobj->force.pr))*reco;
redirect=true;
	}
// YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY HIT
jhy+=jspy;
if ((matr=getmematrix (curphisobj->pos.tx, curphisobj->pos.ty, (int)((int)jhx>>16), (int)((int)jhy>>16), (int)((int)jhh>>16))) && (curphisobj->collide()))
{
	reco=rand() % 100 +50;
		reco/=10000;
	jhy-=jspy;
jspy=-(jspy*(curphisobj->force.pr))*reco;

curphisobj->force.speedy=-(curphisobj->force.speedy*(curphisobj->force.pr))*reco;

redirect=true;
}
phHorizontal=0;
//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH HIT

jhh+=jsph;
matr=0;

//matr=getmematrix (curphisobj->pos.tx, curphisobj->pos.ty, (int)(jhx>>16), (int)(jhy>>16), (int)(jhh>>16));
if ((matr=getmematrix (curphisobj->pos.tx, curphisobj->pos.ty, (int)((int)jhx>>16), (int)((int)jhy>>16), (int)((int)jhh>>16))) && (curphisobj->collide()))
{
reco=rand() % 100 +50;
reco/=10000;
jhh-=jsph;
jsph=-(jsph*(curphisobj->force.pr))*reco;
curphisobj->force.speedh=-(curphisobj->force.speedh*(curphisobj->force.pr))*reco;
curphisobj->force.speedy*=curphisobj->slowing;//(curphisobj->force.pr/100);
curphisobj->force.speedx*=curphisobj->slowing;//(curphisobj->force.pr/100);
redirect=true;
}
}


if (adrtype)
{
	
offx=accurate(jhx) - actcamx - (cameratylx - curphisobj->pos.tx)*tyleSize+(shake.cameraDisplacementPerFrame.x*(ji-ja)/ji);
offy=accurate(jhy) - actcamy - (cameratyly - curphisobj->pos.ty)*tyleSize+(shake.cameraDisplacementPerFrame.y*(ji-ja)/ji);

//CastLightForDarkenfFunk 

if ((curphisobj->lightTrack) && (curphisobj->lightIntensity))
{
int elevation=jhh-heimod;
elevation/=dddstength;
offx+=((offx-scrwid)*elevation)>>16;
offy+=((offy-scrhid)*elevation)>>16;
if (shadowimage[curphisobj->type])
CastLightForDarkenfFunkNoVolume (shadowimage[curphisobj->type], offx-shadowimage[curphisobj->type]->w/2, offy-shadowimage[curphisobj->type]->h/2, curphisobj->lightIntensity);
else
CastLightForDarkenfFunkNoVolume (image[adrtype], offx-image[adrtype]->w/2, offy-image[adrtype]->h/2, curphisobj->lightIntensity);
}
//else
//	if (image[adrtype])
//	drowstyle(offx-image[adrtype]->w/2,offy-image[adrtype]->h/2,jhh-heimod,adrtype);
	//else if (!curphisobj->light.active) 
		//sdlanyblit (offx-4, offy-4, imgBank.DefaultPhisicalBlur, screen);

}
}



if (!redirect)
{
curphisobj->pos.x+=curphisobj->force.speedx*GameSpeed;
curphisobj->pos.y+=curphisobj->force.speedy*GameSpeed;
}
else
{
curphisobj->pos.x=jhx;
curphisobj->pos.y=jhy;
}

/*
dddcoords* accel=GetWindGlobal (offx, offy);
PEpoint->speedx+=(accel->x-PEpoint->speedx)/10;
PEpoint->speedy+=(accel->y-PEpoint->speedy)/10;
*/
dddcoords * accel=GetWindGlobal (offx, offy);
float mltpl=GameSpeed;
mltpl/=(curphisobj->force.mass+1);
curphisobj->force.speedx+=(accel->x)*mltpl;
curphisobj->force.speedy+=(accel->y)*mltpl;
//curphisobj->force.speedh+=(accel->h-curphisobj->force.speedh)*mltpl;

//curphisobj->force.speedx*=1-(1-curphisobj->slowing)*GameSpeed;
//curphisobj->force.speedy*=1-(1-curphisobj->slowing)*GameSpeed;
// ***************************** Gravity part
		curphisobj->pos.h=jhh;

	//if (curphisobj->force.speedh>-pixInPhis*accuracy) 
			curphisobj->force.speedh-=GravCalc;
//}


if (adrtype) 
curphisobj->finalDrow();

	if ((curphisobj->hp<0) && (curphisobj->mortal))
	{
		if (curphisobj->BlitOnDeath)
		{
phisBlitting();
//if (tylextogrid(curphisobj->pos.tx)==tyleytogrid(curphisobj->pos.ty))
 //addsoftblit(screen, image[curphisobj->type], image[curphisobj->type], curphisobj->pos.tx, curphisobj->pos.ty, accurate(curphisobj->pos.x)-image[curphisobj->type]->w/2, accurate(curphisobj->pos.y)-image[curphisobj->type]->w/2, curphisobj->pos.h, curphisobj->type,40, 0, 50,0,0);
		}

removeph(curphisobj, lvl);
	}
	else
	{
optimizeCoordinates(&curphisobj->pos);


curphisobj->aftermove(); 
//if (adrtype) 
//curphisobj->finalDrow();
//For afterdrow




//if ((mergeimage[curphisobj->type+2]) && (curphisobj->type>1)) 
//	lightStyle (mergeimage[curphisobj->type+2], screen, offx - mergeimage[curphisobj->type+2]->w/2, offy- mergeimage[curphisobj->type+2]->h/2, (float)((float)curphisobj->hp/curphisobj->starthp));
//end afterdrow

shorth=curphisobj->pos.h>>16;
	if ((shorth/pixInPhis+matrdepth!=lvl) && (curphisobj->type>0))	
	moveph (curphisobj, lvl);
	else 
		curphisobj=curphisobj->next;

	}
	
	}

	}


// ******************************************  DustPhisics
void removedust ( int i)
{
	dust[i].type=0;
	dustArVacancy=min(i,dustArVacancy);
	
  //  if (i==dustArMax) dustArMax--;
	
}
void dustphisics ()
{
dustVacThisTurn=1;
//float NewSlowing=DustSlowing;
int movex;
int movey;
//int camxtyle, camytyle;

dustTurn=!dustTurn;
Uint32 TurnMask=0x1<<(30+dustTurn);

for (int i=1; i<=dustArMax; i++)
	if (dust[i].type)
{
duPoint=&dust[i];

DirectionalLightetImage* dli=&dustMask[duPoint->type-1];
int ja;
int ji=ja=duPoint->prevji;

if (duPoint->newborn) duPoint->newborn=0;
else
if (duPoint->burning<=0)
{
duPoint->prevOffx-=duPoint->prevOffx1;
duPoint->prevOffy-=duPoint->prevOffy1;
while (ji){
	offx=duPoint->prevOffx1+duPoint->prevOffx*(ja-ji)/ja;
	offy=duPoint->prevOffy1+duPoint->prevOffy*(ja-ji)/ja;

Uint32* __restrict dspix=dustScreen + offy*scrwi+offx;
int scrofy=scrwi-dli->w;
for (int j=0; j<dli->h; j++){
	//memset(dspix, 0, dli->w*4);
	for (int jj=0; jj<dli->w; jj++)
		{
			if ((*dspix & 0xFF000000)!=TurnMask)
			*dspix = 0;
			 dspix++;
		}
		dspix+=scrofy;
	}
ji--;
}
}

if ((duPoint->pos.h>0) && (duPoint->hp>0))
{
duPoint->hp-=GameSpeed;
//Find how much paint
shortx=duPoint->pos.x>>16;
offx=shortx;
movex=abs((((offx-scrwid)*(duPoint->force.speedh))>>7)+duPoint->force.speedx);

shorty=duPoint->pos.y>>16;
offy=shorty;
movey=abs((((offy-scrhid)*(duPoint->force.speedh))>>7)+duPoint->force.speedy);
ji = movex>movey ? movex : movey;
ji>>=16;
if (!ji) ji=1;
int jspx=duPoint->force.speedx/ji;
int jspy=duPoint->force.speedy/ji;
int jsph=duPoint->force.speedh/ji;

preoffxs(&duPoint->pos);
SefOffs(&duPoint->pos);

duPoint->prevOffx1=offx;//-dli->w/2;
duPoint->prevOffy1=offy;//-dli->h/2;
ji=min(5,ji);
ja=ji;


if (!FullyOnScreen(offx, offy, 40, 40)) {removedust(i); goto veryend;}


if (duPoint->burning<=0) goto inskip;
else goto inskipblick;
	//dust drowing
while (ji){
SefOffs(&duPoint->pos);

inskip:
duPoint->pos.x+=jspx;
duPoint->pos.h+=jsph;
duPoint->pos.y+=jspy;


//offx-=dli->w/2;
//offy-=dli->h/2;

Uint8 * dustP=dustMask[duPoint->type-1].DnA;
//*((Uint32*)screen->pixels+ offy*scrwi+offx)=0;
Uint32* __restrict dspix=dustScreen + offy*scrwi+offx;
int scrofy=scrwi-dli->w;
int j=dli->w;
	while (j){
		int jj=dli->w;
		while (jj){
			*dspix+= *dustP/ja;
			*dspix|= TurnMask;
			 dustP++; dspix++;
			 jj--;
		}
		dspix+=scrofy; j--;
	}

ji--;
}
goto afterblick;
// dust drowing end

// blick drowing
while (ji){
	SefOffs(&duPoint->pos);
/*
offx=duPoint->pos.x-(actcamx<<16);
offx/=accuracy;
offx-=(cameratylx - duPoint->pos.tx)*tyleSize;
offy=duPoint->pos.y-(actcamy<<16);
offy/=accuracy;
offy-=(cameratyly - duPoint->pos.ty)*tyleSize;
int elevation=duPoint->pos.h/dddstength;
offx=offx+(((offx-scrwid)*elevation)/accuracy);
offy=offy+(((offy-scrhid)*elevation)/accuracy);*/
inskipblick:

duPoint->pos.x+=jspx;
duPoint->pos.h+=jsph;
duPoint->pos.y+=jspy;

//offx-=dli->w/2;
//offy-=dli->h/2;
fillDarkMaskGrid(offx, offy, dli->w, dli->h);
int brtn=minBurnBrightness+duPoint->burning;
int div=ja*32;
*((Uint32*)screen->pixels+(offy)*scrwi+offx)&=0xFFE0E0E0;
Uint8 * dustP=dustMask[duPoint->type-1].DnA;

Uint32* __restrict dspix=darkmask + offy*scrwi+offx;
int scrofy=scrwi-dli->w;
	for (int j=0; j<dli->h; j++){
		int jj=dli->w;
		while (jj){//for (int jj=0; jj<dli->w; jj++){
			*dspix+= (((brtn**dustP)/div)<<16);
			 dustP++; dspix++; jj--;
		}
		dspix+=scrofy;
	}
ji--;
}
duPoint->burning=max(0,duPoint->burning-GameSpeed);
//blic drowing end
afterblick:


duPoint->prevOffx=offx;
duPoint->prevOffy=offy;
duPoint->prevji=ja;

dddcoords accel;
GetWind(&accel, offx, offy, jhh);

duPoint->force.speedx+=(accel.x-duPoint->force.speedx)/duPoint->force.mass;
duPoint->force.speedy+=(accel.y-duPoint->force.speedy)/duPoint->force.mass;
duPoint->force.speedh+=(accel.h-duPoint->force.speedh)/duPoint->force.mass;



duPoint->force.speedh-=GravCalc*dustGravity*duPoint->force.mass;
veryend:
speedLimitCheckINT(&duPoint->force.speedx, &duPoint->force.speedy, &duPoint->force.speedh, 3);

	}
else
	removedust(i);
}

	while ((!dust[dustArMax].type) && (dustArMax>1)) dustArMax--;

}

//******************************************* WindBlowParticles
void DropLeaves(){
createData.g=(Uint32*)leavesQV->pixels;
createData.tx=cameratylx; createData.ty=cameratyly;
createData.x=(camerax+mouse.pos.x)*accuracy; 
createData.y=(cameray+mouse.pos.y)*accuracy; 
createData.h=8*accuracy;
createData.type=18;
createData.xspeed=0;
createData.yspeed=0;
createData.hspeed=0;
SuperFastBasicSet();
createData.boolFunction=&offScreenDeath;
createData.gravity=0;
createData.hp=7000;
createData.staticPos=0;
addEffect();
}
void WblowPhisics ()
{
for (int i=0; i<=WblowMax; i++)
	if (Wblow[i].speed>0)
{

	//int offx=(Wblow[i].pos.tx-cameratylx)*tyleSize+((Wblow[i].pos.x/accuracy)-camerax); 
	//int offy=(Wblow[i].pos.ty-cameratyly)*tyleSize+((Wblow[i].pos.y/accuracy)-cameray);

preoffxs(&Wblow[i].pos);
SefOffs(&Wblow[i].pos);

if (FullyOnScreen(offx, offy, 2, 2))
{

*((Uint32*)screen->pixels+ offy*scrwi+offx)|=0x00707070;

//
dddcoords accel;
GetWind(&accel, offx, offy, jhh);

float OrgnAng=getAngle((Wblow[i].force.speedx+accel.x)/2, (Wblow[i].force.speedy+accel.y)/2); 
int speed=(Wblow[i].passFindSize+8)*accuracy;

matr=getmematrix (Wblow[i].pos.tx, Wblow[i].pos.ty, 
	(int)(Wblow[i].pos.x+speed*cos(OrgnAng))>>16, 
	(int)(Wblow[i].pos.y+speed*sin(OrgnAng))>>16,
	Wblow[i].pos.h>>16);

if (matr){



float displ=0.1;
float NewAng;
int n=6;

while (n>0){
NewAng=OrgnAng+displ;
if (!getmematrix (Wblow[i].pos.tx, Wblow[i].pos.ty, 
	(int)(Wblow[i].pos.x+speed*cos(NewAng))>>16, 
	(int)(Wblow[i].pos.y+speed*sin(NewAng))>>16,
	Wblow[i].pos.h>>16)) 
	break;
else{
NewAng=OrgnAng-displ;
if (!getmematrix (Wblow[i].pos.tx, Wblow[i].pos.ty, 
	(int)(Wblow[i].pos.x+speed*cos(NewAng))>>16, 
	(int)(Wblow[i].pos.y+speed*sin(NewAng))>>16,
	Wblow[i].pos.h>>16)) break;
}

displ*=2;
n--;
}

if (n<3){
//Check for under/overpass
if (!getmematrix (Wblow[i].pos.tx, Wblow[i].pos.ty, 
	(int)(Wblow[i].pos.x+Wblow[i].force.speedx)>>16, 
	(int)(Wblow[i].pos.y+Wblow[i].force.speedy)>>16,
	(Wblow[i].pos.h>>16)-8)) 
	Wblow[i].force.speedh-=accuracy/100;
else
if (!getmematrix (Wblow[i].pos.tx, Wblow[i].pos.ty, 
	(int)(Wblow[i].pos.x+Wblow[i].force.speedx)>>16, 
	(int)(Wblow[i].pos.y+Wblow[i].force.speedy)>>16,
	(Wblow[i].pos.h>>16)+8)) 
	Wblow[i].force.speedh+=accuracy/500;



}

Wblow[i].passFindSize-=(double)Wblow[i].speed/(double)accuracy;

int nspx=Wblow[i].speed*cos(NewAng);
int nspy=Wblow[i].speed*sin(NewAng);

NewAng=OrgnAng+GetAngleDifferance(OrgnAng, NewAng)/2;

putWindBlowValue(offx, offy, nspx, nspy);

Wblow[i].pos.x+=nspx;
Wblow[i].pos.y+=nspy;
Wblow[i].force.speedx=nspx;//nspx/100+Wblow[i].force.speedx*0.99;//
Wblow[i].force.speedy=nspy;//nspy/100+Wblow[i].force.speedy*0.99;//
//(double)Wblow[i].speed;
Wblow[i].passFindSize=max(Wblow[i].passFindSize,0);

//Wblow[i].speed=-10;
}
else
{
	
	Wblow[i].pos.x+=Wblow[i].force.speedx;
    Wblow[i].pos.y+=Wblow[i].force.speedy;
	putWindBlowValue(offx, offy, Wblow[i].force.speedx, Wblow[i].force.speedy);
	Wblow[i].passFindSize+=1;
	Wblow[i].passFindSize=min(Wblow[i].passFindSize,Wblow[i].thick);
}

//Wblow[i].passFindSize*=0.99;

}
else Wblow[i].speed=-1;
Wblow[i].pos.h+=Wblow[i].force.speedh;
Wblow[i].force.speedh*=0.99;
Wblow[i].force.speedh-=accuracy/5000;
Wblow[i].pos.h=max(0,Wblow[i].pos.h);



Wblow[i].speed--;
if (Wblow[i].speed<=0) WblowVacant=min(i,WblowVacant);
}

	while ((Wblow[WblowMax].speed<=0) && (WblowMax>0)) WblowMax--;
}

//******************************************* Bloodphisics
void BloodDrow (int offx, int offy){
const int scrwiM3=scrwi-3;
const int scrhiM3=scrhi-3;
const float mask[] =  {0.98, 0.95, 0.98,
				      0.95, 0.89, 0.95,
				      0.98, 0.95, 0.98};
	if ((offx>=0) && (offx<scrwiM3-3) && (offy>=0) && (offy<scrhiM3-3)){
		Uint32* __restrict scrpix=(Uint32*)screen->pixels+offy*scrwi+offx;
int j=3;
const float*  msk=&mask[0];
while (j){
	int i=3;
	while (i){
register int idst = *scrpix;
*scrpix = (((int)((idst & RedMask)*(*msk))  
	+ (int)(0x660000*(float)(1-*msk)))& RedMask) | //((((idst & RedMask)*15 + 0x770000)/(15+*msk)) & RedMask) |
		  ((int)((idst & GreenMask)*(*msk)) & GreenMask) |
		  (int)((idst & BlueMask)*(*msk)) |
				(AlphaMask & *scrpix);
/*	 ((RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * AH) >>8)))
		| (GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * AH) >>8))) 
		| (BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * AH) >>8))))
		| (AlphaMask & *destpic);*/
scrpix++;
msk++;
	i--;
	}
	scrpix+=scrwiM3;
j--;
}
	
	
	}


}
void moveblood (blood* bo, int i)
{
blpoint=blpoint->next;

bo->prev->next=bo->next;
bo->next->prev=bo->prev;

if (hig>matrhig+matrdepth-1) hig=matrhig+matrdepth-1;
	
bo->prev=&bfirst[hig];
bo->next=bfirst[hig].next;
bfirst[hig].next=bfirst[hig].next->prev=bo;

}
void removeblood (blood* bo, int i)
{
blpoint=blpoint->next;
	
bo->prev->next=bo->next;
bo->next->prev=bo->prev;
bo->type=0;
	
}
void bloodphisics (int lvl)
{
	float NewSlowing=0.98;
	int NewCalc=GravCalc;
//NewSlowing=NewSlowing-(0.02*GameSpeed);
//SDL_Rect qdest, src;
int i;
i=1;
blpoint=bfirst[lvl].next;
int movex;
int movey;
int camxtyle, camytyle;
int elevation;
SDL_Rect qdest;
noBloodVacancyThisTurn=0;

while (blpoint->next)
{
	testCountBlood++;
			//i++;
camxtyle = - actcamx - ((cameratylx - blpoint->pos.tx)<<10);
camytyle = - actcamy - ((cameratyly - blpoint->pos.ty)<<10);

	//Find how much paint
shortx=blpoint->pos.x>>16;
offx=shortx + camxtyle;
movex=abs((((offx-scrwid)*(blpoint->speedh))>>7)+blpoint->speedx);

shorty=blpoint->pos.y>>16;
offy=shorty + camytyle;
movey=abs((((offy-scrhid)*(blpoint->speedh))>>7)+blpoint->speedy);
ji = movex>movey ? movex : movey;

drtype = blpoint->type+1+(ji>blurb);
//ji/=image[drtype]->h;
//drtype = ji<blurb ? blpoint->type+1 : blpoint->type+2;

//drowstyle(offx,offy,blpoint->pos.h,drtype);
ja=ji=ji>>16;

if ((offx>0) && (offy>0) && (offx<scrwi) && (offy<scrhi)) 
*((Uint8*)((int*)darkmask+(offy)*scrwi+offx))>>=1;
//else
//{
//ji=1;
//blpoint->pos.h=-10;
//goto skipper;
//}



if ((ji>1) && (ji<10))
{
jspx=blpoint->speedx/ji;
jspy=blpoint->speedy/ji;
jsph=blpoint->speedh/ji;

//camxtyle = - actcamx - (cameratylx - blpoint->pos.tx)*tyleSize;
//camytyle = - actcamy - (cameratyly - blpoint->pos.ty)*tyleSize;

jhx=blpoint->pos.x + ((camxtyle)<<16);
jhy=blpoint->pos.y + ((camytyle)<<16);
jhh=blpoint->pos.h;


//ja=ji;
while (ja)
{

jhh+=jsph;
elevation=jhh/dddstength;
jhx+=jspx;
shortx=(int)jhx>>16;
qdest.x=shortx+(((shortx-scrwid)*elevation)>>16);//+(shake.cameraDisplacementPerFrame.x*(ja)/ji)*blpoint->lightParticle;
jhy+=jspy;
shorty=(int)jhy>>16;
qdest.y=shorty+(((shorty-scrhid)*elevation)>>16);//+(shake.cameraDisplacementPerFrame.y*(ja)/ji)*blpoint->lightParticle;

//drowstyle(offx,offy,blpoint->pos.h,drtype);
BloodDrow (offx, offy);
//SDL_BlitSurface(image[drtype], NULL, screen, &qdest);
//if (blpoint->lightParticle)
//CastLightForDarkenfFunk (image[drtype], screen, offx-image[drtype]->w/2, offy-image[drtype]->h/2, 10);
ja--;
}
}
//offx=shortx+(((shortx-scrwid)*elevation)>>16);
//offy=shorty+(((shorty-scrhid)*elevation)>>16);

dddcoords accel;
GetWind(&accel, offx, offy, jhh);//wind.mask[offy/8*wind.w+offx/8];
blpoint->pos.x+=(blpoint->speedx+=(accel.x)/10);
blpoint->pos.y+=(blpoint->speedy+=(accel.y)/10);
blpoint->pos.h+=(blpoint->speedh+=(accel.h));




blpoint->speedx*=NewSlowing;
blpoint->speedy*=NewSlowing;
//blpoint->speedh*=NewSlowing;
blpoint->speedh-=NewCalc;

//int sh=(220-(int)(PixOnSurface(offx, offy)>>24));
if ((blpoint->pos.h>0) && (ji<10))
{
hig=(int)(PEpoint->pos.h>>16)/pixInPhis+matrdepth;
	if (hig!=lvl)	
	moveblood (blpoint, lvl);
	else 
		blpoint=blpoint->next;
}
else
 {
	
	 normalizeCoordinates(&blpoint->pos);
	/*  
	if (blpoint->pos.x>unitsIntyle) {blpoint->pos.x-=unitsIntyle;blpoint->pos.tx+=1;}
	if (blpoint->pos.x<0) {blpoint->pos.x+=unitsIntyle; blpoint->pos.tx-=1;}
	if (blpoint->pos.y>unitsIntyle) {blpoint->pos.y-=unitsIntyle; blpoint->pos.ty+=1;}
	if (blpoint->pos.y<0) {blpoint->pos.y+=unitsIntyle; blpoint->pos.ty-=1;} */
	// if (blpoint->type!=904)
	 //{
		
	
	theta = image[blpoint->type]->w;
	theta/=(ji*2);
	fuck=0;
	for (int f=0; f<blpoint->flooidness; f++)
	{
		shortx=(blpoint->pos.x+blpoint->speedx*fuck);
		shortx=shortx>>16;
		shorty=(blpoint->pos.y+blpoint->speedy*fuck);
		shorty=shorty>>16;
int ttx=tylextogrid(blpoint->pos.tx)+1;
int tty=tyleytogrid(blpoint->pos.ty)+1;
//if (blpoint->type!=804)
tolendstyle (shortx, shorty, blpoint->type, ttx, tty);
if (shortx>tyleSize-image[blpoint->type]->w) 
tolendstyle (shortx-tyleSize, shorty, blpoint->type, tylextogrid(blpoint->pos.tx+1)+1, tty);
if (shorty>tyleSize-image[blpoint->type]->h) 
tolendstyle (shortx, shorty-tyleSize, blpoint->type, ttx, tyleytogrid(blpoint->pos.ty+1)+1);
if (shortx<0) 
tolendstyle (shortx+tyleSize, shorty, blpoint->type, tylextogrid(blpoint->pos.tx-1)+1, tty);
if (shorty<0) 
tolendstyle (shortx, shorty+tyleSize, blpoint->type, ttx, tyleytogrid(blpoint->pos.ty-1)+1);

fuck+=theta;
	}
	 //}
	 //else
	 //{

		 // createData.decaySurfaceNo=1;
		// DecayByOffxOffy(shortx, shorty);
	// }
	//skipper:
	removeblood(blpoint, lvl);
}
	

}
	
}

// presets:
void PhisicalVoxelGuts()
{
	createData.lightPisObj=0;
getione =addphisicalrandomold(epointer->pos.tx, epointer->pos.ty, epointer->pos.x+10, epointer->pos.y,epointer->pos.h, 831, epointer->force.speedx, epointer->force.speedy,0,1, 300, 40,0 , 0,600, 1,100);
ph[getione].angles.Ax=epointer->angles.Ax;	ph[getione].angles.Ay=epointer->angles.Ay;	ph[getione].angles.Ah=epointer->angles.Ah;	
ph[getione].angles.aSpX=GetRandomRotation(0.05); 
ph[getione].angles.aSpY=GetRandomRotation(0.05);  

ph[getione].foo=&bloodymess;
ph[getione].aftermove=&vnothing; 
ph[getione].finalDrow=&VoxelFinalDrow;

}



//****************************************** Player Stuff
void SpiritDrow ()
{
// find on which level the shadow will be.
//i=0;
int j=accurate(player.host->pos.h);
int k=0;//distance from player to shadow
while (k<matrhig)
{
	if ((getmematrix (player.host->pos.tx, player.host->pos.ty,accurate(player.host->pos.x)+timeofdayx*(k+4)/5, accurate(player.host->pos.y)+timeofdayy*(k+4)/5, j-pixInPhis-k*pixInPhis)!=0) || (j-k*pixInPhis<pixInPhis)) break;
	else 
	{
		k++;
	}
}
k*=pixInPhis;
//k=0;
int xx=offx= accurate(player.host->pos.x) - actcamx - cameratylx*tyleSize + player.host->pos.tx*tyleSize;
int yy=offy= accurate(player.host->pos.y) - actcamy - cameratyly*tyleSize + player.host->pos.ty*tyleSize;




if (player.host->pos.h<0) 
	{
		lightStyle (mergeimage[811], screen, offx-mergeimage[811]->w/2, offy-mergeimage[811]->h/2, 1);
		lightStyle (image[79], screen, mouse.pos.x-image[79]->w/2, mouse.pos.y-image[79]->h/2, 1);
}
else
{
xx+=timeofdayx*(k+(j % pixInPhis)+4)/5;
yy+=timeofdayy*(k+(j % pixInPhis)+4)/5;
drowstyle (xx-5, yy-5, ((j/pixInPhis)*pixInPhis-k)*accuracy, 11);
}
}
void playerdrow ()
{

TryGetDead();
// find on which level the shadow will be.
//i=0;

//k=0;

	offx= accurate(player.host->pos.x) - actcamx - cameratylx*tyleSize + player.host->pos.tx*tyleSize;

	offy= accurate(player.host->pos.y) - actcamy - cameratyly*tyleSize + player.host->pos.ty*tyleSize;

// creating player wind
//int coef=10;
//createDirectionalWind(offx, offy, player.host->force.speedx/coef , player.host->force.speedy/coef,0, 5);




//player.offx=offx;
//player.offy=offy;
//drowstyle (offx, offy, player.host->pos.h-heimod, 5);

player.host->targetOnScreen=&mouse.pos; 

playerW->jumping=keystates[ SDLK_SPACE ];
playerW->runing=keystates[ SDLK_LSHIFT];
//	player.host->pos.h=player.host->pos.h;
//AnimateWalking(&playerW,offx, offy, &player.host->force, &player.host->pos, mouse.pos.x, mouse.pos.y);
displayItemsOnTheGround();



}
void drowvisiters(int n)
{
// Drow all players:
for (int i=0; i<n; i++)
	if ((inet.serv.clientConected[i]) || (visiter[i].type))
{
int j=accurate(visiter[i].h);
int k=0;//distance from player to shadow
while (k<matrhig)
{
	if ((getmematrix (visiter[i].tx, visiter[i].ty,accurate(visiter[i].x)+timeofdayx*(k+4)/5, accurate(visiter[i].y)+timeofdayy*(k+4)/5, j-pixInPhis-k*pixInPhis)!=0) || (j-k*pixInPhis<pixInPhis)) break;
	else 
	{
		k++;
	}
}
k*=pixInPhis;


int xx=offx=accurate(visiter[i].x) - actcamx - cameratylx*tyleSize + visiter[i].tx*tyleSize;
int yy=offy=accurate(visiter[i].y) - actcamy - cameratyly*tyleSize + visiter[i].ty*tyleSize;

if (visiter[i].h<0) 
	{
		lightStyle (mergeimage[811], screen, offx-mergeimage[811]->w/2, offy-mergeimage[811]->h/2, 1);
		lightStyle (image[79], screen, mouse.pos.x-image[79]->w/2, mouse.pos.y-image[79]->h/2, 1);
}
else
{
xx+=timeofdayx*(k+(j % pixInPhis)+4)/5;
yy+=timeofdayy*(k+(j % pixInPhis)+4)/5;
drowstyle (xx-5, yy-5, ((j/pixInPhis)*pixInPhis-k)*accuracy, 11);
}

drowstyle (offx, offy, visiter[i].h-heimod, 5);

//visiter[i].x+=visiter[i].speedx;
//visiter[i].y+=visiter[i].speedy;
//visiter[i].h+=visiter[i].speedh;
//if (getmematrix (visiter[i].tx, visiter[i].ty, accurate(visiter[i].x), accurate(visiter[i].y),  accurate(visiter[i].h)))
//{
	//visiter[i].h+=accuracy;
//visiter[i].h-=visiter[i].speedh;
//visiter[i].speedh=0;
//}
//visiter[i].speedh-=calc;
//player.host->force.speedh-=calc;


}
}

void autoaim(int shootspeed)
{

	mousetoselectedrad(mouse.pos.x, mouse.pos.y, PosToMouseDistance(&player.host->pos)/autoaimRad);//120);	
	//Use this to determine distance
	
	
if (mouse.selectedinarray>einmatr)
{
	somebody*ep=&e[mouse.selectedinarray-einmatr];
	//ep=&e[mouse.selectedinarray-einmatr];
	//shortx=shortx>>16;
		//		shorty=shorty>>16;
			//	shorth=shorth>>16;
	//shortx=0;
		//		shorty=0;
			//	shorth=0;

	katet3d((player.host->pos.tx-ep->pos.tx)*tyleSize*accuracy + player.host->pos.x - ep->pos.x, (player.host->pos.ty-ep->pos.ty)*tyleSize*accuracy + player.host->pos.y - ep->pos.y, player.host->pos.h - ep->pos.h - ep->height/2*accuracy*pixInPhis );
				fuck/=shootspeed;//gunstats[gun].shootspeed;

				shortx=ep->force.speedx>>16;
				shorty=ep->force.speedy>>16;
				shorth=(ep->force.speedh>>16) * (ep->type!=611) ;
		shorth*=!getmematrixNotMe (ep->pos.tx, ep->pos.ty, accurate(ep->pos.x), accurate(ep->pos.y), accurate(ep->pos.h)-pixInPhis);
				//shorth=e[mouse.selectedinarray-einmatr].speedh>>16;
				shortx*=fuck;
				shorty*=fuck;
				//shorth*=fuck;
				shortx+=ep->pos.x>>16;
				shorty+=ep->pos.y>>16;
				shorth+=(ep->pos.h>>16)+ep->height/2*pixInPhis;
				stx=ep->pos.tx;
				sty=ep->pos.ty;
}
else
{
	shortx=camerax+mouse.pos.x;
		shorty=cameray+mouse.pos.y;
		shorth*=8;
		stx=cameratylx;
		sty=cameratyly;
}
}

void minigun(bool active,Item * arm)
{
if (displ>100) displ=0;
/*
if (mgoh)
{
minigunSMKx=((player.host->off.x+player.host->walk.gunholex/accuracy-RealAnimatedSmoke.w/2)+minigunSMKx)/2;
minigunSMKy=((player.host->off.y+player.host->walk.gunholey/accuracy-RealAnimatedSmoke.h/2)+minigunSMKy)/2;

	RealAnimatedVolume (&player.host->pos, minigunSMKx, minigunSMKy, screen, &RealAnimatedSmoke, BufferAnglesCopy , mgsa, smokeAngleFade, 0, 0, 0x333333, (float)mgoh/50);

}*/
mgsa--;
	if (mgsa<0) mgsa=45;

if (active) epointer->walk.blade.inHand=0;
int intaim=arm->overheating;
	shootingslowing=0;
mgoh=arm->overheating;
	 // multiply by time passed to optimize framerate independancy
	int aimmod;
	if( keystates[ SDLK_LSHIFT ] ) aimmod=1;
	else aimmod=2;

if (active) playerW->shooting=1;

if (!active) 
{	
	arm->overheating-=gunstats[arm->type].aimdown;
if (arm->overheating<1) arm->overheating=1;	

}
else
	if (arm->cooldown<1) 
	{

//int cdd;
if (!(rand() % (3))){
	int cdd=addPseudoAnimatedSmoke(&player.host->pos, &player.host->force, 0.75,&ExplosionSmokeColor, 21, 1, smokeMaxLight);

smoke[cdd].pos.x+=player.host->walk.gunholex;
smoke[cdd].pos.y+=player.host->walk.gunholey;
}

		playerW->gunBarelRotation+=0.3;
		if (arm->overheating>gunstats[arm->type].aimlim/aimmod) arm->overheating=gunstats[arm->type].aimlim/aimmod; 
		shootingslowing=accuracy;
			autoaim(gunstats[arm->type].shootspeed);
int	k=sphere (arm->overheating*2);//radiusx(arm->overheating*2);
	int rx=(player.host->pos.tx-cameratylx)*tyleSize+accurate(player.host->pos.x)-camerax-mouse.pos.x;
	int ry=(player.host->pos.ty-cameratyly)*tyleSize+accurate(player.host->pos.y)-cameray-mouse.pos.y;
	theta=rx*rx+ry*ry;
	int ttheta=sqrt(theta);
	if (ttheta<70) ttheta=70;

	
	//camhoffset-=200;
	createData.lightPisObj=1;
		createData.lightIntensity=minigunLightIntensity;// 0.558
		createData.displayTime=displ;
	d=addphisical(player.host->pos.tx, player.host->pos.ty, player.host->pos.x, player.host->pos.y,player.host->pos.h+accuracy*8, 18, cameratylx, cameratyly, camerax+mouse.pos.x+(k*ttheta/tyleSize), cameray+mouse.pos.y+(radiusy*ttheta/tyleSize),(shorth)* accuracy,25, 10*25,0,10,66,1,100);
	//int displ=rand() % 100;
	ph[d].pos.x+=playerW->gunholex-ph[d].force.speedx*displ/100;
	ph[d].pos.y+=playerW->gunholey-ph[d].force.speedy*displ/100;
	displ+=16;
	 curphisobj=&ph[d];
	curphisobj->foo();


	ph[d].casterDNA=epointer->i;
//	k=rand() % 10+1;
	//ph[d].pos.x+=playerW->gunholex+(ph[d].force.speedx/pixInPhis)*k;
//	ph[d].pos.y+=playerW->gunholey+(ph[d].force.speedy/pixInPhis)*k;

playerW->forAngleX=ph[d].force.speedx;
playerW->forAngleY=ph[d].force.speedy;

//  PlayMySound(&sounds.bank.minigun);
//	Mix_PlayChannel( 1, gunboom[2], 0 );
	shakeMeBaby(100050);
	createTornado(mouse.pos.x, mouse.pos.y, 30000, 5, 1);
	if (soundEffects) PlayMySound(&sounds.bank.chaingun[rand() % rifleSounds]);
arm->cooldown=20-arm->overheating; arm->overheating+=1;
	}
}
void minigunDrow(int gunx, int guny, float pos)
{
	SuperFastVoxel360(&listOfStuff[playerW->weaponUsing].InvVoxel, gunx, guny,70,  screen , pos, playerW->gunBarelRotation, playerW->gunpoint-M_PI/4);
	addDust(&player.host->pos, player.host->pos.x+player.host->walk.gunholex, player.host->pos.y+player.host->walk.gunholey, CloudSpawnRandy*(10+mgoh)/20, 0 , 0, 0, mgoh,8, 20);
	//	PseudoAnimatedDust
	
//PseudoAnimatedTrans(&player.host->pos, player.host->off.x+player.host->walk.gunholex/accuracy-PseudoAnimBig[5]->w/2, 
	//player.host->off.y+player.host->walk.gunholey/accuracy-PseudoAnimBig[5]->h/2,  screen, PseudoAnimBig[5], BufferAnglesCopy , 255+mgoh*8, smokeAngleFade, 0, 0, 0x333333, min(1,float(mgoh)/80));
}

void rifle(bool active, Item * arm)
{
if (displ>100) displ=0;
bool sounded=0;

if (active) epointer->walk.blade.inHand=0;
	int intaim=arm->overheating;
float addx=0,addy=0;
	shootingslowing=0;
	// multiply by time passed to optimize framerate independancy
	int aimmod;
	if( keystates[ SDLK_LSHIFT ] ) aimmod=1;
	else aimmod=2;
if (active) playerW->shooting=1;


//
if (!OpenGLscreen){
player.bag.recoil.offx+=player.bag.recoil.spx+player.bag.recoil.coolx;
player.bag.recoil.offy+=player.bag.recoil.spy+player.bag.recoil.cooly;

player.bag.recoil.coolx*=0.92;
player.bag.recoil.cooly*=0.92;

player.bag.recoil.spx*=0.7;
player.bag.recoil.spy*=0.7;

player.bag.recoil.coolx-=(player.bag.recoil.offx*0.5);
player.bag.recoil.cooly-=(player.bag.recoil.offy*0.5);
}

if (!active) 
{	
	arm->overheating-=gunstats[arm->type].aimdown*GameSpeed;
if (arm->overheating<0.00001) arm->overheating=0.00001;	

}
		else
		{
player.bag.recoil.cooldownPercent=0.01;//gunstats[arm->type].recoilAftershootPercent;
	autoaim(gunstats[arm->type].shootspeed);
double shoots=(int)max(-arm->cooldown/gunstats[arm->type].delay,1);
double baseNS=arm->cooldown;
			shootingslowing=accuracy;
			float allignx=((stx-player.host->prevAim.tx)*tyleSize+shortx-player.host->prevAim.x);
		allignx/=shoots;  //(mouse.pos.x-mouse.prevx)/shoots;
		float alligny=((sty-player.host->prevAim.ty)*tyleSize+shorty-player.host->prevAim.y);//(mouse.pos.y-mouse.prevy)/shoots;
		alligny/=shoots;
		float addx=0,addy=0;
int rx=(player.host->pos.tx-cameratylx)*tyleSize+accurate(player.host->pos.x)-camerax-mouse.pos.x;
int ry=(player.host->pos.ty-cameratyly)*tyleSize+accurate(player.host->pos.y)-cameray-mouse.pos.y;
theta=rx*rx+ry*ry;
	int ttheta=sqrt(theta);
	if (ttheta<70) ttheta=70;
int	k=radiusx(550+arm->overheating);

float coilx=(k*ttheta);
float coily=(radiusy*ttheta);

if (arm->cooldown<0)
{
	int d;
	while (arm->cooldown<0)
	{
	addx+=allignx,addy+=alligny;
	intaim=arm->overheating;

	

		if (arm->overheating>gunstats[arm->type].aimlim/aimmod) arm->overheating=gunstats[arm->type].aimlim/aimmod;

		//sounds.chainno++;
		//sounds.chainno=sounds.chainno % 3;
		//if (sounds.chainno>3) sounds.chainno=0;

	if (!sounded)	{
		int soundNo=rand() % rifleSounds;
		while (soundNo==sounds.info.lastChaingunSound) soundNo=rand() % rifleSounds;
		sounds.info.lastChaingunSound=soundNo;
		if (soundEffects) PlayMySound(&sounds.bank.chaingun[soundNo]);
	sounded=1;
	}

	
	player.bag.recoil.spx+=coilx/tyleSize/50;
	player.bag.recoil.spy+=coily/tyleSize/50;

	createData.lightIntensity=RifleBulletBrightness;
	createData.lightPisObj=1;
	createData.displayTime=displ;
	 d=addphisical(player.host->pos.tx, player.host->pos.ty, player.host->pos.x, player.host->pos.y,player.host->pos.h+accuracy*8, 17, player.host->prevAim.tx, player.host->prevAim.ty, player.host->prevAim.x+addx+player.bag.recoil.offx, player.host->prevAim.y+addy+player.bag.recoil.offy, shorth*accuracy,gunstats[arm->type].shootspeed, 15,0,2,60, 1,50);
if (shoots==1)
{
	//int displ=rand() % 100;
	ph[d].pos.x-=ph[d].force.speedx*displ/100*GameSpeed;
	ph[d].pos.y-=ph[d].force.speedy*displ/100*GameSpeed;
	displ+=7;
}	 



	
ph[d].casterDNA=player.host->i;
	 float k=(arm->cooldown)/baseNS*(shoots>1);
	ph[d].pos.x+=playerW->gunholex+ph[d].force.speedx*k;
	ph[d].pos.y+=playerW->gunholey+ph[d].force.speedy*k;
	ph[d].pos.h+=ph[d].force.speedh*k;
	curphisobj=&ph[d];
	curphisobj->foo();
	arm->cooldown+=gunstats[arm->type].delay; 
	arm->overheating+=gunstats[arm->type].aimup+(arm->overheating/4)/aimmod; 
	
	
//TEST dust
	

	}
playerW->forAngleX=ph[d].force.speedx;
playerW->forAngleY=ph[d].force.speedy;




}
else
{
player.bag.recoil.offx-=player.bag.recoil.offx*player.bag.recoil.cooldownPercent*GameSpeed;
player.bag.recoil.offy-=player.bag.recoil.offy*player.bag.recoil.cooldownPercent*GameSpeed;
}

player.host->prevAim.x=shortx;
player.host->prevAim.tx=stx;
player.host->prevAim.y=shorty;
player.host->prevAim.ty=sty;

}



}
void rifleDrow(int gunx, int guny, float pos)
{
//	DrowLaserLine(LaserThick, player.host->off.x, player.host->off.y, mouse.pos.x+player.bag.recoil.offx, mouse.pos.y+player.bag.recoil.offy,&player.host->pos);
	
SuperFastVoxel360(&listOfStuff[playerW->weaponUsing].InvVoxel, gunx, guny,55,  screen , pos, pos, playerW->gunpoint+M_PI/2);
}
void GuestRifle(bool active, Item * arm, int no)
{
if (displ>100) displ=0;
bool sounded=0;

if (active) epointer->walk.blade.inHand=0;
	int intaim=arm->overheating;
float addx=0,addy=0;
	// multiply by time passed to optimize framerate independancy
if (active) visiter[no].guestW->shooting=1;

if (!OpenGLscreen){
visiter[no].bag.recoil.offx+=visiter[no].bag.recoil.spx+visiter[no].bag.recoil.coolx;
visiter[no].bag.recoil.offy+=visiter[no].bag.recoil.spy+visiter[no].bag.recoil.cooly;

visiter[no].bag.recoil.coolx*=0.92;
visiter[no].bag.recoil.cooly*=0.92;

visiter[no].bag.recoil.spx*=0.7;
visiter[no].bag.recoil.spy*=0.7;

visiter[no].bag.recoil.coolx-=(visiter[no].bag.recoil.offx*0.5);
visiter[no].bag.recoil.cooly-=(visiter[no].bag.recoil.offy*0.5);
}

if (!active) 
{	
	arm->overheating-=gunstats[arm->type].aimdown*GameSpeed;
if (arm->overheating<0.00001) arm->overheating=0.00001;	

}
		else
		{
visiter[no].bag.recoil.cooldownPercent=0.01;//gunstats[arm->type].recoilAftershootPercent;
	//autoaim(gunstats[arm->type].shootspeed);
shortx=visiter[no].host->pos.x/accuracy+visiter[no].host->targetOnScreen->x-scrwid;
shorty=visiter[no].host->pos.y/accuracy+visiter[no].host->targetOnScreen->y-scrhid;
shorth=accuracy*8;
stx=visiter[no].host->pos.tx;
sty=visiter[no].host->pos.ty;
double shoots=(int)max(-arm->cooldown/gunstats[arm->type].delay,1);
double baseNS=arm->cooldown;
float allignx=((stx-visiter[no].host->prevAim.tx)*tyleSize+shortx-visiter[no].host->prevAim.x);
allignx/=shoots;  //(mouse.pos.x-mouse.prevx)/shoots;
float alligny=((sty-visiter[no].host->prevAim.ty)*tyleSize+shorty-visiter[no].host->prevAim.y);//(mouse.pos.y-mouse.prevy)/shoots;
alligny/=shoots;
		float addx=0,addy=0;
int rx=visiter[no].host->targetOnScreen->x-scrwid;
int ry=visiter[no].host->targetOnScreen->y-scrhid;
theta=rx*rx+ry*ry;
	int ttheta=sqrt(theta);
	if (ttheta<70) ttheta=70;
int	k=radiusx(550+arm->overheating);

float coilx=(k*ttheta);
float coily=(radiusy*ttheta);

if (arm->cooldown<0)
{
	int d;
	while (arm->cooldown<0)
	{
	addx+=allignx,addy+=alligny;
	intaim=arm->overheating;

	

		if (arm->overheating>gunstats[arm->type].aimlim) arm->overheating=gunstats[arm->type].aimlim;

		//sounds.chainno++;
		//sounds.chainno=sounds.chainno % 3;
		//if (sounds.chainno>3) sounds.chainno=0;

	if (!sounded)	{
		int soundNo=rand() % rifleSounds;
		while (soundNo==sounds.info.lastChaingunSound) soundNo=rand() % rifleSounds;
		sounds.info.lastChaingunSound=soundNo;
		if (soundEffects) PlayMySound(&sounds.bank.chaingun[soundNo]);
	sounded=1;
	}

	
	visiter[no].bag.recoil.spx+=coilx/tyleSize/500;
	visiter[no].bag.recoil.spy+=coily/tyleSize/500;

	createData.lightIntensity=RifleBulletBrightness;
	createData.lightPisObj=1;
	createData.displayTime=displ;
	 d=addphisical(visiter[no].host->pos.tx, visiter[no].host->pos.ty, visiter[no].host->pos.x, 
		 visiter[no].host->pos.y,visiter[no].host->pos.h+accuracy*8, 17, visiter[no].host->prevAim.tx,
		 visiter[no].host->prevAim.ty, visiter[no].host->prevAim.x+addx+visiter[no].bag.recoil.offx,
		 visiter[no].host->prevAim.y+addy+visiter[no].bag.recoil.offy, shorth*accuracy,gunstats[arm->type].shootspeed,
		 2,0,9,60, 1,50);
if (shoots==1)
{
	//int displ=rand() % 100;
	ph[d].pos.x-=ph[d].force.speedx*displ/100*GameSpeed;
	ph[d].pos.y-=ph[d].force.speedy*displ/100*GameSpeed;
	displ+=7;
}	 



	
ph[d].casterDNA=visiter[no].host->i;
	 float k=(arm->cooldown)/baseNS*(shoots>1);
	 ph[d].pos.x+=visiter[no].guestW->gunholex+ph[d].force.speedx*k;
	ph[d].pos.y+=visiter[no].guestW->gunholey+ph[d].force.speedy*k;
	ph[d].pos.h+=ph[d].force.speedh*k;
	curphisobj=&ph[d];
	curphisobj->foo();
	arm->cooldown+=gunstats[arm->type].delay; 
	arm->overheating+=gunstats[arm->type].aimup+(arm->overheating/4); 
	
	
//TEST dust
	

	}
visiter[no].guestW->forAngleX=ph[d].force.speedx;
visiter[no].guestW->forAngleY=ph[d].force.speedy;




}
else
{
visiter[no].bag.recoil.offx-=visiter[no].bag.recoil.offx*visiter[no].bag.recoil.cooldownPercent*GameSpeed;
visiter[no].bag.recoil.offy-=visiter[no].bag.recoil.offy*visiter[no].bag.recoil.cooldownPercent*GameSpeed;
}

visiter[no].host->prevAim.x=shortx;
visiter[no].host->prevAim.tx=stx;
visiter[no].host->prevAim.y=shorty;
visiter[no].host->prevAim.ty=sty;

}



}

void granade(bool active,Item * arm)
{
if (active) epointer->walk.blade.inHand=0;
	
	 // multiply by time passed to optimize framerate independancy
	int aimmod;
	if( keystates[ SDLK_LSHIFT ] ) aimmod=1;
	else aimmod=2;
if (active) playerW->shooting=1;

if (((arm->cooldown<1) && (active)) || ((arm->overheating>500) && (arm->cooldown<84)))
{	
if (arm->overheating<500) arm->overheating=2000;
else
arm->overheating-=1000;

shakeMeBaby(24050);
autoaim(gunstats[arm->type].shootspeed);
	createData.lightIntensity=4;
createData.lightPisObj=1;
d=addphisical(player.host->pos.tx, player.host->pos.ty, player.host->pos.x, player.host->pos.y,player.host->pos.h+accuracy*8, 810, 
	cameratylx, cameratyly, camerax+mouse.pos.x, cameray+mouse.pos.y, 0,20, 400,0,3,66,1,200);
ph[d].casterDNA=epointer->i;
ph[d].pos.x+=playerW->gunholex;
ph[d].pos.y+=playerW->gunholey;
arm->cooldown=gunstats[arm->type].delay;
	playerW->forAngleX=ph[d].force.speedx;
	playerW->forAngleY=ph[d].force.speedy;

	//ph[d].data=addFireEffect(&ph[d].pos, &ph[d].force, 0.99);
	addPseudoAnimatedSmoke(&ph[d].pos, &ph[d].force, 0.9,&RocketShootColor, 3, 1, smokeMaxLight);
}
}
void granadeDrow(int gunx, int guny, float pos)
{
	SuperFastVoxel360(&listOfStuff[playerW->weaponUsing].InvVoxel, gunx, guny,55,  screen , pos, pos, playerW->gunpoint-M_PI/4);
}

void blade(bool active, Item * arm)
{
	if (active) epointer->walk.blade.inHand=1;
	if (arm->cooldown<0) 
{
player.host->walk.blade.combo=max(player.host->walk.blade.combo--, 0);
		
	if	(active)
{
	player.host->walk.blade.activated=1;
	player.host->walk.blade.frame=0;
//	if (player.host->walk.blade.hitSomebody)
	
//	player.host->walk.blade.hitSomebody=0;
	bladeSwingTime=bladeSwingTimeDef+player.host->walk.blade.combo;
	arm->cooldown=swingCooldown; 
	player.host->walk.blade.swingDirection=-player.host->walk.blade.swingDirection;

}
}
//else if (!active) player.host->walk.blade.combo=0;
		
	


}
void bladeDrowBack(int torsox, int torsoy, float torsoangle)
{
	SuperFastVoxel(&listOfStuff[playerW->weaponPassive].InvVoxel, torsox, torsoy,10,  screen,0, 0, torsoangle);
	//SuperFastVoxel360(&listOfStuff[playerW->weaponUsing].InvVoxel, epointer->off.x,  epointer->off.y,55,  screen , tempx, tempy, playerW->gunpoint+M_PI/2);
}
void bladeDrow(int gunx, int guny, float pos)
{
	
SuperFastVoxel360(&listOfStuff[playerW->weaponUsing].InvVoxel, gunx, guny,55,  screen , 0*epointer->walk.blade.frame, 0*epointer->walk.blade.frame, playerW->gunpoint+M_PI/2);
if (playerW->blade.poseffectsCharge>1)
{
	/*
createData.tx=epointer->pos.tx; createData.ty=epointer->pos.ty; 
createData.x=epointer->pos.x+playerW->gunholex; createData.y=epointer->pos.y+playerW->gunholey; createData.h=epointer->pos.h+accuracy*10;
createData.type=20; createData.xspeed=0; createData.yspeed=0;
createData.hspeed=0;
createData.randosity=randy*3; createData.numberof=rand() % 300-200;
playerW->blade.poseffectsCharge*=0.98;
createData.flooidness=2; createData.slowing=0.98;

addphisicalrandom();*/
}

}

void processHand(Item * Playerarm, bool handactive)
{
		
	if (!Playerarm->count) return;
	Playerarm->cooldown-=GameSpeed;
	if (Playerarm->cooldown<-1) Playerarm->cooldown=-1;
	if ((handactive) && (Playerarm->count)) 
	{
		playerW->weaponUsing=Playerarm->type;
		playerW->itemInHand=Playerarm;
	}

	if (Playerarm->type==4) rifle(handactive, Playerarm);
	if (Playerarm->type==1) granade(handactive, Playerarm);
	if (Playerarm->type==2) minigun(handactive, Playerarm);//, &Playerarm->overheating);
	if (Playerarm->type==3) blade(handactive, Playerarm);// &Playerarm->overheating);
}
void usingToolInHand()
{
playerW->shooting=0;

	if (playerW->blade.activated) 
		return;	
	bool right=(mouse.u8ms & SDL_BUTTON(SDL_BUTTON_RIGHT)) || (MyJoystick.rs);
	processHand(&player.bag.RightHand,right);
	
		processHand(&player.bag.LeftHand, (!right) && (mouse.u8ms & SDL_BUTTON(SDL_BUTTON_LEFT)) || (MyJoystick.rt) );
	
	if (playerW->weaponUsing!=player.bag.RightHand.type) 
		playerW->weaponPassive=player.bag.RightHand.type;
	else
	if (playerW->weaponUsing!=player.bag.LeftHand.type) 
		playerW->weaponPassive=player.bag.LeftHand.type;


}

//******************************************** DEVELOPMENT SUPPORT

void linkedout ()
{
LP * del;
LP * fuckk=linkhead;
LP * lnq = linkhead;

while(lnq) {
//if (nq->qot==2001)
	if  ((ph[lnq->ione].hp <=0) || (ph[lnq->itwo].hp <=0) || (!ph[lnq->ione].type) || (!ph[lnq->itwo].type) || (lnq->broken))
		{del=lnq; if (lnq==linkhead) linkhead=lnq->next; 	lnq = lnq->next; delete del;
	if (lnq==linkhead) fuckk=lnq; else fuckk->next=lnq;}
	else 
	{
		//if (GameSpeed<1.5)
		lnq->broken=phisLink(&ph[lnq->ione].pos, &ph[lnq->itwo].pos, &ph[lnq->ione].force, &ph[lnq->itwo].force, lnq->dist,phisConnectionDeStretchForce, phisConnectionDeSquizeForce);
	/*	//if not deleted
		ixc=(ph[lnq->ione].pos.tx-ph[lnq->itwo].pos.tx)*pixInPhis*accuracy+(ph[lnq->ione].pos.x-ph[lnq->itwo].pos.x+ph[lnq->ione].force.speedx-ph[lnq->itwo].force.speedx)/128;
		ygr=(ph[lnq->ione].pos.ty-ph[lnq->itwo].pos.ty)*pixInPhis*accuracy+(ph[lnq->ione].pos.y-ph[lnq->itwo].pos.y+ph[lnq->ione].force.speedy-ph[lnq->itwo].force.speedy)/128;
		hashh=(ph[lnq->ione].pos.h-ph[lnq->itwo].pos.h+ph[lnq->ione].force.speedh-ph[lnq->itwo].force.speedh)/128;
		tangenssyka=ixc*ixc+ygr*ygr+hashh*hashh;
		tangenssyka=sqrt(tangenssyka);
		if (tangenssyka>0) 
		{
		
		masabla=(ph[lnq->ione].force.mass+ph[lnq->itwo].force.mass);
		fuck=(tangenssyka*128-lnq->dist*accuracy)/tangenssyka;
		calcforcex=fuck*ixc;
		calcforcey=fuck*ygr;
		calcforceh=fuck*hashh;
		if (masabla>0)  
		{
			lqobjects++;
			if (calcforcex!=0)
			{
			//int test=0;
		ph[lnq->ione].force.speedx-=calcforcex*ph[lnq->ione].force.mass/masabla;
		ph[lnq->itwo].force.speedx+=calcforcex*ph[lnq->itwo].force.mass/masabla;
			}
			if (calcforcey!=0)
			{
		ph[lnq->ione].force.speedy-=calcforcey*ph[lnq->ione].force.mass/masabla;
		ph[lnq->itwo].force.speedy+=calcforcey*ph[lnq->itwo].force.mass/masabla;
			}
			if (calcforceh!=0)
			{
				masabla*=2;
		ph[lnq->ione].force.speedh-=calcforceh*ph[lnq->ione].force.mass/masabla;
		ph[lnq->itwo].force.speedh+=calcforceh*ph[lnq->itwo].force.mass/masabla;
			}
		}
		
			*/
		
		//}

		if (fuckk->next==lnq) fuckk=fuckk->next;
		lnq=lnq->next; 	
	}

        }
}

void editoron()
{


	if  (edit.FullScreen.closed){

prodata("qobjects:",qobjects,3);
qobjects=0;
prodata("Framerate:",sumframes,4);
prodata("MVheel controlling:",edit.scrolling,6);
prodata("1 - Ylev:",edit.ylev,7);
prodata("2 - Mlev:",edit.mlev,8);
prodata("3 - Slev:",edit.slev*100,9);
prodata("Folliage:",edit.folliage,10);
mousetoselected(mouse.pos.x, mouse.pos.y);
prodata("Pointing:",mouse.selectedinarray,11);
//qobjects
	}
//if( keystates[ SDLK_t ] ) edit.mlev++; 
//if( keystates[ SDLK_g ] ) edit.mlev--; 

mgvox.windmanQLastIn=0;
mgvox.buttManQLastIn=0;
scrollMax=0;
draggingWindow=0;
edit.MBPressed=0;


if (!edit.FullScreen.closed){
manageWindow(&edit.MegavoxMaker);
manageWindow(&tera.Edit.selectorIntDecay);
manageWindow(&tera.Edit.selectorHDecay);
manageWindow(&edit.DollHouse);
manageWindow(&edit.Puppetier);
if ((!edit.DollHouse.closed) || (!edit.MegavoxMaker.closed)){
manageWindow(&mgvox.pieceSelector);
manageWindow(&mgvox.segmenter);
if (!mgvox.segmenter.closed){
	manageWindow(&mgvox.palette);
	manageWindow(&mgvox.tools);
	manageWindow(&mgvox.segmentLanding);
}
}
}
manageWindow(&edit.FullScreen);

if ( keystates[  SDLK_KP7 ] ) glob-=0.01;
if ( keystates[  SDLK_KP9 ] ) glob+=0.01;
if ( keystates[  SDLK_KP6 ] ) globxh+=0.01;
if ( keystates[  SDLK_KP4 ] ) globxh-=0.01;
if ( keystates[  SDLK_KP8 ] ) globyh+=0.01;
if ( keystates[  SDLK_KP2 ] ) globyh-=0.01;

ShowQuestObject();

if ((qedit.WritingQuestDescription) )
{
	// ***************************** Drow frame for selected object
int offx=qedit.targetQO->qox - actcamx + ((qedit.targetQO->qotx - cameratylx)<<10);
int offy=qedit.targetQO->qoy - actcamy + ((qedit.targetQO->qoty - cameratyly)<<10);
DrowFrame (offx, offy, 40, 40, 0xFFFFFF); 
// ****************************** current mission
prodata("qedit.CurMissionPack:",qedit.CurMissionPack,0);
prodata("qedit.CurMissionPack:",qedit.curMission,1);
if (Inputtext)
	SDL_BlitSurface(Inputtext, NULL, screen, NULL);
}
//if (mergeimaged[edit.editor])
//drowDecayedBG (image[85], screen, mergeimaged[edit.editor], mouse.pos.x, mouse.pos.y);
if (edit.FullScreen.closed)  {
if (!imageVoxel[edit.editor])
//MergeVexel(imageVoxel[edit.editor], mouse.pos.x, mouse.pos.y,edit.mlev,  screen,globxh, globyh, glob);
//MergeVexelUpper(imageVoxel[edit.editor], mouse.pos.x, mouse.pos.y,edit.mlev,  screen,globxh, globyh, glob, screen, mergeimage[edit.editor]);
{
if (heightimage[edit.editor])
{
if (heightimage[edit.editor])
{
	//if ((edit.folliage) && (overlayimage[edit.editor]))
	//BlitNormalAndAlphaInprintScaleFolliage(screen, mergeimage[edit.editor],  screen, overlayimage[edit.editor], heightimage[edit.editor], ((mouse.pos.x-mergeimage[edit.editor]->w/2)),((mouse.pos.y-mergeimage[edit.editor]->h/2)), edit.mlev, edit.slev );
	//else
	//BlitWithAlphaInprintScale(screen, mergeimage[edit.editor], heightimage[edit.editor], ((mouse.pos.x-mergeimage[edit.editor]->w/2)),((mouse.pos.y-mergeimage[edit.editor]->h/2)), edit.mlev, edit.slev);
}


	if (dhimage[edit.editor])
BlitWithDHInprintPreview(screen, image[edit.editor], dhimage[edit.editor], ((mouse.pos.x-image[edit.editor]->w/2)),((mouse.pos.y-image[edit.editor]->h/2)), edit.mlev);


}
else
	if (edit.editor>900)
		if (edit.editor<1100)
	mdrowstyle(((mouse.pos.x-mergeimage[edit.editor]->w/2)/pixInPhis)*pixInPhis,((mouse.pos.y-mergeimage[edit.editor]->h/2)/pixInPhis)*pixInPhis,0,edit.editor);
		else drowstyle(mouse.pos.x-image[edit.editor]->w/2,mouse.pos.y-image[edit.editor]->h/2,0,edit.editor);
	else
		if (edit.editor<800){ if (mergeimage[edit.editor])
		mdrowstyle(mouse.pos.x-mergeimage[edit.editor]->w/2,mouse.pos.y-mergeimage[edit.editor]->h/2,0,edit.editor);
		}
		else
			if (image[edit.editor])
			drowstyle(mouse.pos.x-image[edit.editor]->w/2,mouse.pos.y-image[edit.editor]->h/2,0,edit.editor);
}

	mouse.editorinterference=0;

if (edit.editor<1100)
{
if ((edit.editor>900) && (wallh[edit.editor-900]>-1))
{
d=((edit.ylev/pixInPhis)*pixInPhis)*accuracy;

for (ie=0;ie<(wall[edit.editor-900]->w+7)/pixInPhis;ie++) 
	for (je=0;je<(wall[edit.editor-900]->h+7)/pixInPhis;je++) 
		if (mousetoselectedconsth(((mouse.pos.x-wall[edit.editor-900]->w/2)/pixInPhis+ie)*pixInPhis, ((mouse.pos.y-wall[edit.editor-900]->h/2)/pixInPhis+je)*pixInPhis))
		{
		mouse.editorinterference=1;
		drow (((mouse.pos.x-wall[edit.editor-900]->w/2)/pixInPhis+ie)*pixInPhis,((mouse.pos.y-wall[edit.editor-900]->h/2)/pixInPhis+je)*pixInPhis,image[14],d);
		}
}
}

}





}

void developerdata()
{
/*	if ((camerax>1020) && (actcamx>1020) ) {	cameratylx+=1;	camerax-=tyleSize; actcamx-=tyleSize;  setgrid(1,0);	}
	if ((cameray>1150) && (actcamy>1150) ) {	cameratyly+=1;	cameray-=tyleSize; actcamy-=tyleSize;	 setgrid(0,1);}
	if ((camerax<-150) && (actcamx<-150) ){	cameratylx-=1;	camerax=tyleSize+camerax; actcamx=tyleSize+actcamx;	 setgrid(-1,0);}
	if ((cameray<-150) && (actcamy<-150) ){	cameratyly-=1;	cameray=tyleSize+cameray; actcamy=tyleSize+actcamy;	 setgrid (0,-1); }
	shake.shakerCoolDown=0.006;
	 shake.antishakeCoolUp=0.006;
*/

	if (edit.FullScreen.closed){

for (int i=11; i<20; i++)
hud (i);
hud (22);

float fylev=edit.ylev;
fylev/=pixInPhis;
int iylev=floor(fylev);

d=((edit.ylev/pixInPhis)*pixInPhis)*accuracy;
offx= - actcamx - cameratylx*tyleSize + grid.objectx[tylextogrid(player.host->pos.tx)][tyleytogrid(player.host->pos.ty)]*tyleSize;
offy= - actcamy - cameratyly*tyleSize + grid.objecty[tylextogrid(player.host->pos.tx)][tyleytogrid(player.host->pos.ty)]*tyleSize;
for (int i=0;i<phArSize;i++) 
	for (int j=0;j<phArSize;j++) 
	{
 if (mapmatrix[tylextogrid(player.host->pos.tx)][tyleytogrid(player.host->pos.ty)][i][j][matrdepth+iylev]>0)
	 drowstyle (offx+i*pixInPhis,offy+j*pixInPhis,d,12); 
	}

if (edit.editor<300) {
	if (mergeimage[100+curlay])
	drowmerge(scrwi-hudx[21]-mergeimage[100+curlay]->w/2+hudd[21]->w/2,scrhi-hudy[21]-mergeimage[100+curlay]->h/2+hudd[21]->h/2,100+curlay); 
}
else 
	if (edit.editor<600) drowmerge(scrwi-hudx[21]-mergeimage[300+qlay]->w/2+hudd[21]->w/2,scrhi-hudy[21]-mergeimage[300+qlay]->h/2+hudd[21]->h/2,300+qlay);
	else 
		if (edit.editor<900) drow(scrwi-hudx[21]-image[600+curchar]->w/2+hudd[21]->w/2,scrhi-hudy[21]-image[600+curchar]->h/2+hudd[21]->h/2,image[600+curchar],0);
	else
if (edit.editor<1100)
	{
		int chex=scrwi-hudx[21]-mergeimage[900+wallined]->w/2+hudd[21]->w/2;
		int chey=scrhi-hudy[21]-mergeimage[900+wallined]->h/2+hudd[21]->h/2;
		drowmerge(chex,chey,900+wallined);
		nooffsetdrow(chex, chey,900+wallined);
		//for (i=0;i<=wallh[wallined];i++)
			apply_surface( chex, chey, wall[wallined], screen );
	}
else
{
int chex=scrwi-hudx[21]-underg[curcave][0]->w/2+hudd[21]->w/2;
int chey=scrhi-hudy[21]-underg[curcave][0]->h/2+hudd[21]->h/2;
apply_surface( chex, chey, image[1100+curcave], screen );
}

hud (21);
	}

}
void TuneDouble(double *value, double step, string name, double min, double max)
{
prodata(name,*value,3);
if ( keystates[  SDLK_EQUALS ] ) *value+=step;
if ( keystates[  SDLK_MINUS ] ) *value-=step;

if (min!=-1) if (*value<min) *value=min;
if (max!=-1) if (*value>max) *value=max;
}
void TuneInt(int *value, int step, string name, int min, int max)
{
prodata(name,*value,3);
if ( keystates[  SDLK_EQUALS ] ) *value+=step;
if ( keystates[  SDLK_MINUS ] ) *value-=step;

if (min!=-1) if (*value<min) *value=min;
if (max!=-1) if (*value>max) *value=max;
}
void ReloadSurface(string directory, SDL_Surface * surface, string text)
{
if ( !keystates[  SDLK_EQUALS ] ) 
	prodata(text,0,3);
else
{
	
	SDL_FreeSurface(surface);
	surface=loadany(directory);
	prodata(text,999,3);
}
	//string 

}
void TuningList()
{
	
	if (keystates[  SDLK_KP1 ]) edit.TuningParameter=1;
	if (keystates[  SDLK_KP2 ]) edit.TuningParameter=2;
	if (keystates[  SDLK_KP3 ]) edit.TuningParameter=3;
	if (keystates[  SDLK_KP4 ]) edit.TuningParameter=4;
	if (keystates[  SDLK_KP5 ]) edit.TuningParameter=5;
	if (keystates[  SDLK_KP6 ]) edit.TuningParameter=6;
	if (keystates[  SDLK_KP7 ]) edit.TuningParameter=7;
	if (keystates[  SDLK_KP8 ]) edit.TuningParameter=8;
	if (keystates[  SDLK_KP9 ]) edit.TuningParameter=9;

	switch (edit.TuningParameter)
	{
		
			//case 1: TuneInt(&player.host->walk.blade.combo, 1, "player.host->walk.blade.combo", 0, 5000); break;
			
//case 1: TuneDouble(&bladeRotationSpeed, 0.01, "bladeRotationSpeed:", -10, 10); break;
//case 1: TuneInt(&comeBackTime, 1, "comeBackTime", 2, 5000); break;
	//case 1: TuneInt(&VoxBloodDistanceDiv, 20, "VoxBloodDistanceDiv", 1, 1000000); break;	
//case 1:
	//ReloadSurface("pictures/other/smallSmokeMask.png", mergeimage[1500], "Reload  small cloud.png"); break;
	//TuneInt(&chanceOfSmokyParticleDrop, 1, "chanceOfSmokyParticleDrop", 1, 9999999); break;	
	//TuneInt(&fireFromCenterBlowForce, 10, "fireFromCenterBlowForce", 1, 9999999); break;	
	//TuneInt(&phisLinkConnectionStrength, 1, "phisLinkConnectionStrength", 1, 9999999); break;	
	//G8RotSpeed
	//	case 1: TuneDouble(&G8RotSpeed, 0.0001, "G8RotSpeed", 0.0001, 100); break;
	case 1: TuneDouble(&testy, 0.005, "testy", 0.01, 3); break;
	//case 1: TuneDouble(&sounds.guts.OnImpactGain, 1, "SOUND, impact_track, Gain on impact (255-max vol):", 0, 255); break;
	//case 2: TuneDouble(&gunstats[4].delay, 0.01, "Rifle shoots delay", 0.02, 100); break;
	//case 3: TuneDouble(&gunstats[2].recoilAftershootPercent, 0.01, "RecoilAftershootPercent:", 0.00001, 0.9999); break;
	case 2: TuneDouble(&testx, 0.01, "testx", 0.01, 100); break;
	
		//	case 2: TuneDouble(&UpgoingAngularFadechange, 0.0000001, "UpgoingAngularFadechange", 0.0000001, 100); break;
		//case 2: TuneDouble(&distanceFadingRisePerFrame, 0.00001, "distanceFadingRisePerFrame", 0.00001, 100); break;
	//case 2: TuneDouble(&GameSpeed, 0.01, "GameSpeed", 0.01, 100); break;
		//case 3: TuneDouble(&GameSpeedAdjustmentSpeed, 0.01, "GameSpeedAdjustmentSpeed", 0.01, 1); break;

			case 3: TuneDouble(&VoxBloodUpDrop, 0.01, "VoxBloodUpDrop", 0.01, 10); break;
	//case 3: TuneDouble(&tempy, 0.01, "slomoFade", 0.01, 1); break;
	//case 3: TuneDouble(&impactShake, 0.0000001, "impactShake:", 0.0000001, 10); break;
	
	//case 2: TuneDouble(&sounds.guts.FadeSpeed, 0.005, "SOUND, impact_track fade (%/sec):", 0, 1); break;
	//case 3: TuneDouble(&sounds.softguts.OnImpactGain, 1, "SOUND, splatter_track, Gain on impact (255-max vol):", 0, 255); break;
//	case 4: TuneDouble(&sounds.softguts.FadeSpeed, 0.005, "SOUND, splatter_track fade (%/sec):", 0, 1); break;
		//	case 5: TuneInt(&vbloodness, 1, "vbloodness", 1, 6000); break;
	//case 5: TuneInt(&bladeSwingTime, 1, "bladeSwingTime", 1, 6000); break;
	//	case 5: TuneInt(&bladeRadius, 1, "bladeRadius", 1, 600000); break;
	//case 3: TuneInt(&camerashkespeed, 10, "camerashakespeed:"); break;
	case 4: TuneInt(&temptest, 1, "temptest", 1, 9999999); break;
	//case 5: TuneInt(&sounds.stereoVividness, 1, "Stereo Strength (min - 1000=max vividness)", scrwid, 6000); break;	
	case 6: TuneDouble(&gunstats[4].delay, 0.01, "gunstats[4].delay (rifle)", 0.01, 200); break;
//	healthLossDrop
		case 7: TuneInt(&FireBrightnes, 1, "FireBrightnes", 1, 100); break;	
	//case 7: TuneInt(&wind.maxBend, 1000, "maxBend", 0 , -1); break;	
	case 8: TuneInt(&wind.smoothness, 1, "windSmoothness", 0, -1); break;	
	case 9: TuneInt(&wind.treeBendDelay, 1, "TreeBendDelay", 0, -1); break;	
	}
//camerashkespeed
//
	//prodata("CoolDown:",shake.shakerCoolDown,1);


//if ( keystates[  SDLK_KP7 ] ) shake.shakerCoolDown+=0.001;
//if ( keystates[  SDLK_KP4 ] ) shake.shakerCoolDown-=0.001;

}

void playison ()
{


	


//shake.antishake=0.1;//*(abs(shake.x)+abs(shake.y))/50;
shake.x+=shake.spx;
shake.spx-=shake.x*shake.antishake;
shake.y+=shake.spy;
shake.spy-=shake.y*shake.antishake;
shake.x*=shake.shaker;
shake.y*=shake.shaker;
shake.shaker=max(0,shake.shaker-shake.shakerCoolDown);
shake.antishake=min(1, shake.antishake+shake.antishakeCoolUp);

				//camhoffset=(camhoffset*90)/100;

if (!OpenGLscreen){
camxaccel=(player.host->pos.tx*tyleSize+accurate(player.host->pos.x)+mouse.pos.x - cameratylx*tyleSize - camerax - scrwi )/mousesmoothing;
camyaccel=(player.host->pos.ty*tyleSize+accurate(player.host->pos.y)+mouse.pos.y - cameratyly*tyleSize - cameray - scrhi  )/mousesmoothing;

actcamx+=camxaccel;//+accurate(shake.x); //
camerax=actcamx;
actcamy+=camyaccel;//+accurate(shake.y); 
cameray=actcamy;

}else{
camxaccel=(player.host->pos.tx*tyleSize+accurate(player.host->pos.x) + GLtoCPUx - cameratylx*tyleSize - camerax - scrwi/2)/mouseToCanGLsmoothing;
camyaccel=(player.host->pos.ty*tyleSize+accurate(player.host->pos.y) + GLtoCPUy - cameratyly*tyleSize - cameray - scrhi/2)/mouseToCanGLsmoothing;
}



/*for (int i=0; i<BagSize; i++)
	if (bag.type[i][0])
{
qdest.x=300+i*50;
qdest.y=scrhi-50;
SDL_BlitSurface(image[bag.type[i][0]], NULL, screen, &qdest);
qdest.x=300+i*50;
qdest.y=scrhi-20;
SDL_BlitSurface(bag.description[i][0], NULL, screen, &qdest);
}*/

}
void RepointRenderingOfLight()
{
	if ((player.host->pos.h<0) && (!player.isunderground))
{
	player.isunderground=true;
	lightStyle=&lightenug;
	//drowstyle=&anydrownooffset;
	drowstyle=&anydrow;
//	bloodstyle=&realbloodphisics;
}
else
	if ((player.host->pos.h>=0) && (player.isunderground))
{
	player.isunderground=false;
	if (!stereo)
	{
	lightStyle=&lighten;
	drowstyle=&anydrow;
	sdldrowstyle=&sdlanydrow;
	//bloodstyle=&bloodphisics;
	}
	else
	{
	lightStyle=&lighten;
	drowstyle=&anydrow3d;
	sdldrowstyle=&sdlanydrow3d;
	bloodstyle=&bloodphisics;
	}
}
}

//******************************Enemies functions
void playermove()
{
// ************************************************************************************ Main hero stuff
// Laser pointer

	double locspeed=paccel;
locspeed/=joyStickMaxValue;


if ((play) && (!Idisplay.active)) usingToolInHand();
/*
if( keystates[ SDLK_1 ] ) gun=1;
if( keystates[ SDLK_2 ] ) gun=2;
if( keystates[ SDLK_3 ] ) gun=3;
if( keystates[ SDLK_4 ] ) gun=4;
if( keystates[ SDLK_5 ] ) gun=5;
if( keystates[ SDLK_6 ] ) gun=6;*/

//int tmatr=getmematrix (player.host->pos.tx, player.host->pos.ty, accurate(player.host->pos.x), accurate(player.host->pos.y),  accurate(player.host->pos.h)-pixInPhis);


if( keystates[ SDLK_SPACE ] )
	if ((player.host->pos.h<accuracy*2) || (getmematrixNotMe (player.host->pos.tx, player.host->pos.ty, accurate(player.host->pos.x), accurate(player.host->pos.y),  accurate(player.host->pos.h)-pixInPhis)))	
 { 
	 player.host->force.speedh=accuracy;  
	if (soundEffects) PlayMySound(&sounds.bank.cloth[rand() % 6]);
 }
player.host->force.speedh-=GravCalc;
player.host->pos.h+=player.host->force.speedh*GameSpeed;

if (getmematrixNotMe (player.host->pos.tx, player.host->pos.ty, accurate(player.host->pos.x), accurate(player.host->pos.y),  accurate(player.host->pos.h)))
{
	player.host->pos.h-=player.host->force.speedh*GameSpeed;
	player.host->force.speedh=0;
}


int ground=(256-GetAlphaValue(accurate(player.host->pos.x), accurate(player.host->pos.y), mainmaps[tylextogrid(player.host->pos.tx)+1][tyleytogrid(player.host->pos.ty)+1]))*4;
//prodata("HeightValueReturn",GetAlphaValue(accurate(player.host->pos.x), accurate(player.host->pos.y), mainmaps[tylextogrid(player.host->pos.tx)+1][tyleytogrid(player.host->pos.ty)+1]),7);

//if (player.host->pos.h<0) {player.host->pos.h=0; player.host->force.speedh=-player.host->force.speedh/2;}
//else
//dh=655360-(player.host->pos.h % 655360); 

//player.host->pos.h=max(ground*2565, player.host->pos.h);




int dh=ground*2565-player.host->pos.h;

if ((dh>0) && (getmematrixNotMe (player.host->pos.tx, player.host->pos.ty, accurate(player.host->pos.x), accurate(player.host->pos.y),  accurate(player.host->pos.h)-pixInPhis)) && (accurate(player.host->pos.h)>=0))
{
	player.host->pos.h+=dh;
//	float tempd=655360-dh;
//	tempd/=655360;
	//if (dh>>14)
	//{
	//dh=655360-dh/8;
	//dh=655360;
	//player.host->force.speedx=player.host->force.speedx*tempd;
	//player.host->force.speedy=player.host->force.speedy*tempd;
	//}
}




//prodata("PlayerHight/2565",player.host->pos.h/2560,8);//2560*256 - player high
//prodata("H_Velocity",player.host->force.speedh,9);//2560*256 - player high


if (player.host->pos.h<-matrdepth*accuracy*pixInPhis) player.host->pos.h=-matrdepth*accuracy*pixInPhis;
if (play==1) edit.ylev=accurate(player.host->pos.h)/8*8;

int absspeed=ipifagor(player.host->force.speedx>>4, player.host->force.speedy>>4);
if (absspeed>(topspeed>>4))
{
	double cut=topspeed>>4;
	cut/=absspeed;
	player.host->force.speedx*=cut;
	player.host->force.speedy*=cut;
}

	/*
 if (player.host->force.speedx>topspeed) player.host->force.speedx-=paccel*2;
 if (player.host->force.speedx<-topspeed) player.host->force.speedx+=paccel*2;
 if (player.host->force.speedy>topspeed) player.host->force.speedy-=paccel*2;
 if (player.host->force.speedy<-topspeed) player.host->force.speedy+=paccel*2;
*/
 // PX moving
 player.host->pos.x+=player.host->force.speedx*GameSpeed;
	
		


	float slower=0.9;
if ((getmematrixNotMe (player.host->pos.tx, player.host->pos.ty,accurate(player.host->pos.x), accurate(player.host->pos.y), accurate(player.host->pos.h))))
{
	player.host->pos.x-=player.host->force.speedx*GameSpeed;
	shake.spx+=50;
	if (( keystates[ SDLK_d ] ) || ( keystates[ SDLK_a ] ))
		if ((getmematrixNotMe (player.host->pos.tx, player.host->pos.ty, accurate(player.host->pos.x), accurate(player.host->pos.y),  accurate(player.host->pos.h))) && (!getmematrixNotMe (player.host->pos.tx, player.host->pos.ty, accurate(player.host->pos.x), accurate(player.host->pos.y),  accurate(player.host->pos.h)+pixInPhis))) 
		{
			int dh=655360-(player.host->pos.h % 655360); 
		//	player.host->force.speedx*=(dh/655360);
			player.host->pos.h+=dh; 
			//player.host->force.speedh+=calc*2;
			//player.host->force.speedx*=(mod(player.host->pos.h,655360));
		}
	
}
//else
	if( keystates[ SDLK_d ] ) player.host->force.speedx+=paccel; else 
if( keystates[ SDLK_a ] ) player.host->force.speedx-=paccel; else
	if (MyJoystick.lStickX)
		player.host->force.speedx+=locspeed*MyJoystick.lStickX;
	else
	player.host->force.speedx*=slower;

if (player.host->pos.x>tyleSize*accuracy) {	player.host->pos.x-=tyleSize*accuracy; player.host->pos.tx++;  }
	if (player.host->pos.x<0) {	player.host->pos.x+=tyleSize*accuracy; player.host->pos.tx--;}

	// PY moving

 player.host->pos.y+=player.host->force.speedy*GameSpeed;
 	


if ((getmematrixNotMe (player.host->pos.tx, player.host->pos.ty,accurate(player.host->pos.x), accurate(player.host->pos.y), accurate(player.host->pos.h))))
{
	player.host->pos.y-=player.host->force.speedy*GameSpeed;
 shake.spy+=50;
if (( keystates[ SDLK_w ] ) || ( keystates[ SDLK_s ] ))
	if ((getmematrixNotMe (player.host->pos.tx, player.host->pos.ty, accurate(player.host->pos.x), accurate(player.host->pos.y),  accurate(player.host->pos.h))) && (!getmematrixNotMe (player.host->pos.tx, player.host->pos.ty, accurate(player.host->pos.x), accurate(player.host->pos.y),  accurate(player.host->pos.h)+pixInPhis)))  
	{
		int dh=655360-(player.host->pos.h % 655360); 
	//	player.host->force.speedy*=(dh/655360);
			player.host->pos.h+=dh; 
	}
		//	player.host->force.speedh+=calc*2; 
 
}
//else
	if( keystates[ SDLK_s ] ) player.host->force.speedy+=paccel; else 
if( keystates[ SDLK_w ] ) player.host->force.speedy-=paccel; else
		if (MyJoystick.lStickY)
		player.host->force.speedy+=locspeed*MyJoystick.lStickY;
	else
		player.host->force.speedy*=slower;

if (accurate(player.host->pos.y)>=tyleSize) {	player.host->pos.y-=tyleSize*accuracy; player.host->pos.ty++;	}
	if (accurate(player.host->pos.y)<0) {	player.host->pos.y+=tyleSize*accuracy; player.host->pos.ty--;	  }


if( keystates[ SDLK_LSHIFT ] )
{
	topspeed+=accuracy/100; 
if (topspeed>accuracy*5-shootingslowing) topspeed=accuracy*5-shootingslowing;

}
else
topspeed=accuracy*3-shootingslowing;

if (tera.ZOOMED) topspeed=accuracy*1.5;

player.flashLight.angleLight=ConvertVectorTo360(scrwid-player.host->targetOnScreen->x, scrhid-player.host->targetOnScreen->y);
if (!player.flashLight.angleLight) player.flashLight.angleLight=1;
/*player.flashLight.angleLight=getAngle(player.host->targetOnScreen->x-scrwid, player.host->targetOnScreen->y-scrhid);
player.flashLight.angleLight=player.flashLight.angleLight*180/M_PI;
player.flashLight.angleLight=player.flashLight.angleLight-((int)player.flashLight.angleLight/360)*360;
if (player.flashLight.angleLight<0) player.flashLight.angleLight+=360;*/

}
int waitforclock()
{
	return 0;
}
int basicmovementfunction()

{
	d=0;

	bool sideMovement=0;
	if (player.corporial)
	{
		if (TakeWalkingDamage( &epointer->pos, &epointer->force, &player.host->pos, &player.host->force, &playerW->torso , &player.host->hp))
		{
			epointer->movement=&waitforclock;
			player.host->walk.blade.combo/=2;
		}
		//if (GodMode) player.host->hp=100;
	}
	// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
epointer->pos.x+=epointer->force.speedx*GameSpeed;
if (epointer->pos.x<0) {epointer->pos.x+=tyleSize*accuracy-1; epointer->pos.tx=epointer->pos.tx-1;}

//bool colision=false;

// Slowing Down enemy Below:
matr=getmematrix (epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x), accurate(epointer->pos.y), accurate(epointer->pos.h)-8);
if ((matr>einmatr) && (matr<phinmatr) && (matr-einmatr!=epointer->i))
{
	e[matr-einmatr].clock+=1;
}

if (getmematrix (epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x), accurate(epointer->pos.y), accurate(epointer->pos.h))!=0)
{
if ((!getmematrix (epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x), accurate(epointer->pos.y), accurate(epointer->pos.h)+pixInPhis)) && ( !getmematrix (epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x-epointer->force.speedx), accurate(epointer->pos.y), accurate(epointer->pos.h)+pixInPhis)))
epointer->force.speedh+=GravCalc*2;

epointer->pos.x-=epointer->force.speedx*GameSpeed;
//epointer->force.speedx/=2;
} else {d=1;sideMovement=1;}

if (epointer->pos.x>tyleSize*accuracy-1) {epointer->pos.x-=tyleSize*accuracy+1; epointer->pos.tx=epointer->pos.tx+1;}
// YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY
epointer->pos.y+=epointer->force.speedy*GameSpeed;
if (epointer->pos.y<0) {epointer->pos.y+=tyleSize*accuracy; epointer->pos.ty=epointer->pos.ty-1;}

if (getmematrix (epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x), accurate(epointer->pos.y), accurate(epointer->pos.h))!=0)
	{
if ((getmematrix (epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x), accurate(epointer->pos.y), accurate(epointer->pos.h)+pixInPhis)==0) && ( getmematrix (epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x), accurate(epointer->pos.y-epointer->force.speedy), accurate(epointer->pos.h)+pixInPhis)==0 ))
epointer->force.speedh+=GravCalc*2;
	epointer->pos.y-=epointer->force.speedy*GameSpeed;
	//epointer->force.speedy/=2;
	} else {d=1;sideMovement=1;}
	
if (epointer->pos.y>tyleSize*accuracy-1) {epointer->pos.y-=tyleSize*accuracy+1; epointer->pos.ty=epointer->pos.ty+1;}

	
//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
epointer->pos.h+=epointer->force.speedh*GameSpeed;
bool collision=false;
int gotmtrix;
for (matr=0; matr<epointer->height; matr++)
{
gotmtrix=getmematrix (epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x), accurate(epointer->pos.y), accurate(epointer->pos.h)+matr*pixInPhis);
if ((gotmtrix) && (gotmtrix!=epointer->i-einmatr)) {
	collision=1;
	matr=epointer->height;}
}

if ((collision) || (accurate(epointer->pos.h)>matrhig*pixInPhis))
	{
		
	epointer->pos.h-=epointer->force.speedh*GameSpeed;
	epointer->force.speedh=0;
	}
	else
	{
d=1;

	}

// change array
	
if (!sideMovement) epointer->height=1;
else epointer->height=4;
//


// ************************************************* POST move
epointer->force.speedh-=GravCalc;
double NewSlowing=1;
NewSlowing-=((1-epointer->slowwing)*GameSpeed);
epointer->force.speedx*=NewSlowing;
epointer->force.speedy*=NewSlowing;

if (!d) 
	{
epointer->movement=&waitforclock;
epointer->force.speedx=0;
epointer->force.speedy=0;
}
return 0;

}
int RagDollMovementFunction(){

	
if (!epointer->force.speedx) epointer->force.speedx=1;
double ah=getAngle(epointer->force.speedx, epointer->force.speedy);

//Fixing angle
double teorAngl=epointer->angles.Ah-2*M_PI;
if (abs(teorAngl-ah)<abs(epointer->angles.Ah-ah)) epointer->angles.Ah=teorAngl;
teorAngl=epointer->angles.Ah+2*M_PI;
if (abs(teorAngl-ah)<abs(epointer->angles.Ah-ah)) epointer->angles.Ah=teorAngl;
//playerW->angles.Ah+=((ah-epointer->angles.Ah)/fpstep);

epointer->angles.Ah+=((ah-epointer->angles.Ah)/30)*GameSpeed;

epointer->angles.Ax+=epointer->angles.aSpX*GameSpeed;
epointer->angles.Ay+=epointer->angles.aSpY*GameSpeed;

LIMBS[epointer->SpawnedLimbsNombers[0]].angle=epointer->angles.Ah;

int n=epointer->limbsSpawned;
Uint16* lno=epointer->SpawnedLimbsNombers;
for (int i=0; i<n; i++){
	if (*lno)	{
		AnyRagLimb* limb=&LIMBS[*lno];
		for (int w=0; w<2; w++)
			for (int h=0; h<2; h++)
				for (int tall=-1; tall<1; tall++)
					TryClearmatrix (limb->pos.tx, limb->pos.ty,accurate(limb->pos.x)+(w<<3), 
						accurate(limb->pos.y)+(h<<3), accurate(limb->pos.h)+(tall<<3), epointer->i+einmatr);
		
		optimizeCoordinates(&limb->pos);

		if (limb->connectedNo){
			AnyRagLimb* conlimb=&LIMBS[limb->connectedNo];
			if (conlimb->flags & LIMB_Dismembered) {
				dismember(limb);
			
			}

		}	
	}
	lno++;
}
lno=epointer->SpawnedLimbsNombers;
for (int i=0; i<n; i++) if (*lno) {
	AnyRagLimb* limb=&LIMBS[*lno];
	 LimbCollision(limb,epointer->i+einmatr );
	lno++;
}

basicmovementfunction();
	AnyRagLimb* limb=&LIMBS[epointer->SpawnedLimbsNombers[0]];
memcpy(&limb->pos, &epointer->pos,sizeof(worldcoordinates));
limb->force.speedx=epointer->force.speedx;
limb->force.speedy=epointer->force.speedy;
limb->force.speedh=epointer->force.speedh;

return 1;
}


int bleed()
{
	basicmovementfunction();
	if (epointer->bleeding<0) epointer->movement=epointer->defaultMovement;
epointer->bleeding-=1;

createData.tx=epointer->pos.tx; createData.ty=epointer->pos.ty; createData.x=epointer->pos.x+epointer->bleedx; createData.y=epointer->pos.y+epointer->bleedy; 
createData.h=epointer->pos.h+accuracy*5;
createData.type=813; createData.xspeed=epointer->bleedspx; createData.yspeed=epointer->bleedspy;  createData.hspeed=0;
createData.randosity=260; createData.numberof=rand() % 5; createData.flooidness=2; createData.slowing=0.98;
createData.lightPisObj=0;
addphisicalrandom();//epointer->pos.tx, epointer->pos.ty, epointer->pos.x+12*accuracy, epointer->pos.y+12*accuracy, epointer->pos.h, 813, epointer->force.speedx, epointer->force.speedy,0, 260, min(rand() % 18-4, 0),1, 0.98);

createData.type=20; createData.numberof=rand() % 5; createData.slowing=0.97;

addphisicalrandom();
//epointer->pos.tx, epointer->pos.ty, epointer->pos.x+12*accuracy, epointer->pos.y+12*accuracy, epointer->pos.h, 20, epointer->force.speedx, epointer->force.speedy,0, 260, min(rand() % 22-4, 0),1, 0.97);
epointer->hp-=3;
return 0;
}
int flyMovement()
{
	if (epointer->angles.Ax>2*M_PI) epointer->angles.Ax-=2*M_PI;
if (epointer->angles.Ax<0) epointer->angles.Ax+=2*M_PI;
if (epointer->angles.Ay>2*M_PI) epointer->angles.Ay-=2*M_PI;
if (epointer->angles.Ay<0) epointer->angles.Ay+=2*M_PI;

epointer->angles.aSpX*=0.8;
epointer->angles.aSpY*=0.8;

epointer->angles.Ax*=0.8;
epointer->angles.Ay*=0.8;


	d=0;

	bool sideMovement=0;
if (player.corporial)
	{
		if (TakeWalkingDamage( &epointer->pos, &epointer->force, &player.host->pos, &player.host->force, &playerW->torso , &player.host->hp))
	epointer->movement=&waitforclock;
		//if (GodMode) player.host->hp=100;
	}
	// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
epointer->pos.x+=epointer->force.speedx;
if (epointer->pos.x<0) {epointer->pos.x+=tyleSize*accuracy-1; epointer->pos.tx=epointer->pos.tx-1;}

//bool colision=false;


if (getmematrix (epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x), accurate(epointer->pos.y), accurate(epointer->pos.h)))
{
if ((!getmematrix (epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x), accurate(epointer->pos.y), accurate(epointer->pos.h)+pixInPhis)) && ( !getmematrix (epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x-epointer->force.speedx), accurate(epointer->pos.y), accurate(epointer->pos.h)+pixInPhis)))
epointer->force.speedh+=GravCalc/2;

epointer->pos.x-=epointer->force.speedx;
//epointer->force.speedx/=2;
} else {d=1;sideMovement=1;}

if (epointer->pos.x>tyleSize*accuracy-1) {epointer->pos.x-=tyleSize*accuracy+1; epointer->pos.tx=epointer->pos.tx+1;}
// YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY
epointer->pos.y+=epointer->force.speedy;
if (epointer->pos.y<0) {epointer->pos.y+=tyleSize*accuracy; epointer->pos.ty=epointer->pos.ty-1;}

if (getmematrix (epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x), accurate(epointer->pos.y), accurate(epointer->pos.h))!=0)
	{
if ((getmematrix (epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x), accurate(epointer->pos.y), accurate(epointer->pos.h)+pixInPhis)==0) && ( getmematrix (epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x), accurate(epointer->pos.y-epointer->force.speedy), accurate(epointer->pos.h)+pixInPhis)==0 ))
epointer->force.speedh+=GravCalc/2;
	epointer->pos.y-=epointer->force.speedy;
	//epointer->force.speedy/=2;
	} else {d=1;sideMovement=1;}
	
if (epointer->pos.y>tyleSize*accuracy-1) {epointer->pos.y-=tyleSize*accuracy+1; epointer->pos.ty=epointer->pos.ty+1;}

	
//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
epointer->pos.h+=epointer->force.speedh;
if (epointer->pos.h>accuracy*40) epointer->pos.h=accuracy*40;
bool collision=false;
int gotmtrix;
for (matr=0; matr<epointer->height; matr++)
{
gotmtrix=getmematrix (epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x), accurate(epointer->pos.y), accurate(epointer->pos.h)+matr*pixInPhis);
if ((gotmtrix) && (gotmtrix!=epointer->i-einmatr)) {
	collision=1;
	matr=epointer->height;}
}

if ((collision) || (accurate(epointer->pos.h)>matrhig*pixInPhis))
	{
		
	epointer->pos.h-=epointer->force.speedh;
	epointer->force.speedh=0;
	}
	else
	{
d=1;

	}

// change array
	
if (!sideMovement) epointer->height=1;
else epointer->height=4;
//


// ************************************************* POST move
//epointer->force.speedh-=calc;
epointer->force.speedx*=epointer->slowwing;
epointer->force.speedy*=epointer->slowwing;

if (!d) 
	{
epointer->movement=&waitforclock;
epointer->force.speedx=0;
epointer->force.speedy=0;
}
return 0;

}
int takingbullet()
{
	damage=accurate(damage);
	epointer->hp-=damage;
	SDL_Rect qdest;
epointer->movement=&bleed;
	if ((epointer->off.x>0) && (epointer->off.x<scrwi) && (epointer->off.y>0) && (epointer->off.y<scrhi))
	{

	if (d==0)
makeblood ( epointer->i,  curphisobj->force.speedx, curphisobj->force.speedy, damage);
	else
makebackblood ( epointer->i,  curphisobj->force.speedx, curphisobj->force.speedy, damage);
	if ((epointer->type) && (epointer->hp>0))
	{
	if (epointer->imgcopy==0) 
	{
		epointer->gutsout=SDL_ConvertSurface(image[epointer->type], image[epointer->type]->format, SDL_SWSURFACE);
		epointer->imgcopy=1;
	}
	int iy=min(max(accurate(jhy-epointer->pos.y)+image[epointer->type]->h/2, 0), image[epointer->type]->h-1);
//z=rand() % 2; 
qdest.x = rand() % (epointer->gutsout->w);//+epointer->gutsout->w/4;
qdest.y = iy;//rand() % epointer->gutsout->h;

epointer->bleeding+=3;
epointer->bleedx=(qdest.x-image[epointer->type]->w/2)*accuracy;
epointer->bleedy=(qdest.y-image[epointer->type]->h/2)*accuracy;
epointer->bleedspx=-curphisobj->force.speedx/4;
epointer->bleedspy=-curphisobj->force.speedy/4;

SDL_BlitSurface(mergeimage[curphisobj->type+1], NULL, epointer->gutsout , &qdest); 
gutsup(epointer->gutsout,mergeimage[epointer->type+1], qdest.x, qdest.y, 7);

	}
	}

		return 1;
}
int takingVoxelBullet()
{

epointer->force.speedx+=(curphisobj->force.speedx*(curphisobj->force.mass))/epointer->force.mass;//+punch.x;
epointer->force.speedy+=(curphisobj->force.speedy*(curphisobj->force.mass))/epointer->force.mass;//+punch.y;
epointer->force.speedh+=(curphisobj->force.speedh*(curphisobj->force.mass))/epointer->force.mass;

//bool finalblow=(epointer->hp<0);
damage=accurate(damage);
float voxblood=damage;
voxblood/=abs(max(epointer->hp,200));

	epointer->hp-=damage;


	if (!epointer->voxGut) epointer->voxGut=6+(rand() % 4); 
	else 
	if (epointer->voxGut<10) epointer->voxGut+=4; 
	else 
	if (epointer->voxGut<14) epointer->voxGut+=4; 

	epointer->angles.aSpX+=((float)((rand() % 10) -5) / 200);
epointer->angles.aSpY+=((float)((rand() % 10) -5) / 200);

	SDL_Rect qdest;
	if ((epointer->off.x>0) && (epointer->off.x<scrwi) && (epointer->off.y>0) && (epointer->off.y<scrhi))
	{

createData.lightPisObj=0;

//for (int bld=0; bld<4; bld++ ){
int type=rand() % 4;
int no=addphisicalrandomold(curphisobj->pos.tx,curphisobj->pos.ty, jhx, jhy, curphisobj->pos.h, 860, 0, 0,
	1 //up
	, 0 , 240+type*60, 20-type, 0 ,0, randy*(10-type), 1,10+(rand() % 10));			

ph[no].G_p=1;
ph[no].BlitOnDeath=1;
ph[no].data=13; 
ph[no].g8particle=&mgvDB.mgvarray[13][type].Converted;
ph[no].solidness=0.08;
ph[no].voxelPalette=0;//mergeimaged[8];

//}


	/*	
createData.g=(Uint32*)QGpalette[5]->pixels;//(Uint32*)mergeimaged[0]->pixels+(rand() % (1024*1000));//(Uint32*)QGpalette[5]->pixels;
createData.type=rand() % 8;//14 + (rand() % 2);
VoxelBloodBasicSet(&curphisobj->pos, 0, 0);
createData.x=jhx;
createData.y=jhy;
createData.xspeed=radiusx(accuracy*2);//-(curphisobj->force.speedx/2000)*((rand() % 100)-200);
createData.yspeed=radiusy;//-(curphisobj->force.speedy/2000)*((rand() % 100)-200);
addEffect();
*/
		//epointer->angles.aSpX+=((float)((rand() % 100) -50) *impactShake);
		//epointer->angles.aSpY+=((float)((rand() % 100) -50) *impactShake);

	if (d==0)
makeblood ( epointer->i,  curphisobj->force.speedx, curphisobj->force.speedy, damage);
	else
makebackblood ( epointer->i,  curphisobj->force.speedx, curphisobj->force.speedy, damage);
	if ((epointer->type) && (epointer->hp>0))
	{
	/*	if (!epointer->voxcopy)//(epointer->imgcopy==0) 
	{
		//epointer->gutsout=SDL_ConvertSurface(image[epointer->type], image[epointer->type]->format, SDL_SWSURFACE);
		//epointer->imgcopy=1;
		epointer->voxcopy=1;
		epointer->gutsbool= new bool [5];
	}*/
	int iy=min(max(accurate(jhy-epointer->pos.y)+image[epointer->type]->h/2, 0), image[epointer->type]->h-1);
//z=rand() % 2; 
qdest.x = rand() % 20;//+epointer->gutsout->w/4;
qdest.y = iy;//rand() % epointer->gutsout->h;

epointer->bleeding+=damage;
epointer->bleedx=(qdest.x-image[epointer->type]->w/2)*accuracy;
epointer->bleedy=(qdest.y-image[epointer->type]->h/2)*accuracy;
epointer->bleedspx=-curphisobj->force.speedx/4;
epointer->bleedspy=-curphisobj->force.speedy/4;

//SDL_BlitSurface(mergeimage[curphisobj->type+1], NULL, epointer->gutsout , &qdest); 
//gutsup(epointer->gutsout,mergeimage[epointer->type+1], qdest.x, qdest.y, 15);

	} else 
if (!flashlight){
createData.tx=epointer->pos.tx; createData.ty=epointer->pos.ty;
createData.x=jhx; createData.y=jhy;
createData.h=accuracy*5;
createData.type=7;
createData.maskIMG=imgBank.OnImpactGoreHeight;
createData.xspeed=0;
createData.yspeed=0;
createData.xaccel=0;
createData.yaccel=0;
createData.hspeed=accuracy*1.48;
createData.MaxLight=8;
createData.gravity=1.2;///=400;
createData.hp=0;//curphisobj->pos.h;
createData.boolFunction=&belowgroundmuch;
createData.voidfunction=&TrueHeightDrow;
createData.staticPos=0;
addEffect();}
	
	}

		return 1;
}
int takingRagDollBullet(){

epointer->force.speedx+=(curphisobj->force.speedx*(curphisobj->force.mass))/epointer->force.mass;//+punch.x;
epointer->force.speedy+=(curphisobj->force.speedy*(curphisobj->force.mass))/epointer->force.mass;//+punch.y;
epointer->force.speedh+=(curphisobj->force.speedh*(curphisobj->force.mass))/epointer->force.mass;


damage=accurate(damage);
epointer->hp-=damage;

int no=epointer->limbsSpawned;
Uint16* limbNumbers=epointer->SpawnedLimbsNombers;
int distance=accuracy*tyleSize;
int impacted=0;
	for (int i=1; i< no; i++){
			limbNumbers++;
		if (*limbNumbers) {
			AnyRagLimb* limb=&LIMBS[*limbNumbers];
			int dx=(limb->pos.tx-curphisobj->pos.tx)*tyleSize*accuracy+(limb->pos.x-jhx);
			int dy=(limb->pos.ty-curphisobj->pos.ty)*tyleSize*accuracy+(limb->pos.y-jhy);
			int dh=(limb->pos.h-jhh);
			
			katet3dD (dx/duIn13_8192, dy/duIn13_8192, dh/duIn13_8192);
			if (fuck<distance){
				distance=fuck;
				impacted=i;//*limbNumbers;
			}
		}
	}
		if (impacted){

if (d==0)
makeblood ( epointer->i,  curphisobj->force.speedx, curphisobj->force.speedy, damage);
	else
makebackblood ( epointer->i,  curphisobj->force.speedx, curphisobj->force.speedy, damage);

			AnyRagLimb* limb=&LIMBS[epointer->SpawnedLimbsNombers[impacted]];
			limb->force.speedx+=(curphisobj->force.speedx*(curphisobj->force.mass))/limb->force.mass;//+punch.x;
			limb->force.speedy+=(curphisobj->force.speedy*(curphisobj->force.mass))/limb->force.mass;//+punch.y;
			limb->force.speedh+=(curphisobj->force.speedh*(curphisobj->force.mass))/limb->force.mass;
			//limb->hp-=damage;
			/*if (limb->hp<0){
				createData.lightPisObj=0;
				int type=rand() % 4;
				int no=addphisicalrandomold(curphisobj->pos.tx,curphisobj->pos.ty, jhx, jhy, curphisobj->pos.h, 860, 0, 0,
					1 //up
					, 0 , 240+type*60, 20-type, 0 ,0, randy*(10-type), 1,10+(rand() % 10));			
				ph[no].G_p=1;
				ph[no].BlitOnDeath=1;
				ph[no].data=13; 
				ph[no].g8particle=&mgvDB.mgvarray[13][type].Converted;
				ph[no].solidness=0.08;
				ph[no].voxelPalette=0;
			}*/
		}
		return 1;
}
int takingWingsDamage()
{
bool finalblow=1*epointer->hp>0;
damage=accurate(damage);
float voxblood=damage;
voxblood/=abs(max(epointer->hp,200));

	epointer->hp-=damage;
	
	SDL_Rect qdest;
	if ((epointer->off.x>0) && (epointer->off.x<scrwi) && (epointer->off.y>0) && (epointer->off.y<scrhi))
	{
createData.g=(Uint32*)QGpalette[5]->pixels;//(Uint32*)mergeimaged[0]->pixels+(rand() % (1024*1000));//(Uint32*)QGpalette[5]->pixels;

if ((finalblow) && (epointer->hp<=0)) 
	{ 
//curphisobj->force.speedx=curphisobj->force.speedy=0; 
//rand() % 6;
for (int i=0; i<3; i++)
{
createData.type=14+i;

VoxelBloodBasicSet(&epointer->pos,0 , 0);
createData.xspeed=-curphisobj->force.speedx/300*(rand() % 20 - 10);
createData.yspeed=-curphisobj->force.speedy/300*(rand() % 20 - 10);
addEffect();
}
}
/*else
for (int vb=0; vb<1+voxblood*vbloodness; vb++)
{
createData.type=rand() % 6;//rand() % 6;
createData.xspeed=-curphisobj->force.speedx/VoxBloodDistanceDiv*(rand() % 10);
createData.yspeed=-curphisobj->force.speedy/VoxBloodDistanceDiv*(rand() % 10);
VoxelBloodBasicSet(&curphisobj->pos);
addEffect();
}*/
		epointer->angles.aSpX+=((float)((rand() % 10) -5) / 500);
		epointer->angles.aSpY+=((float)((rand() % 10) -5) / 500);

	if (d==0)
makeblood ( epointer->i,  curphisobj->force.speedx, curphisobj->force.speedy, damage);
	else
makebackblood ( epointer->i,  curphisobj->force.speedx, curphisobj->force.speedy, damage);
	if ((epointer->type!=0) && (epointer->hp>0))
	{
	if (epointer->imgcopy==0) 
	{
		epointer->gutsout=SDL_ConvertSurface(image[epointer->type], image[epointer->type]->format, SDL_SWSURFACE);
		epointer->imgcopy=1;
	}
	int iy=min(max(accurate(jhy-epointer->pos.y)+image[epointer->type]->h/2, 0), image[epointer->type]->h-1);
//z=rand() % 2; 
qdest.x = rand() % (epointer->gutsout->w);//+epointer->gutsout->w/4;
qdest.y = iy;//rand() % epointer->gutsout->h;

epointer->bleeding+=3;
epointer->bleedx=(qdest.x-image[epointer->type]->w/2)*accuracy;
epointer->bleedy=(qdest.y-image[epointer->type]->h/2)*accuracy;
epointer->bleedspx=-curphisobj->force.speedx/4;
epointer->bleedspy=-curphisobj->force.speedy/4;

SDL_BlitSurface(mergeimage[curphisobj->type+1], NULL, epointer->gutsout , &qdest); 
removePixels(epointer->gutsout,mergeimage[epointer->type+1], qdest.x, qdest.y, 3+(rand() % 7));

	}
	}

		return 1;
}
int takingAnimatedBullet()
{
bool finalblow=1*epointer->hp>0;
damage=accurate(damage);
float voxblood=damage;
voxblood/=abs(max(epointer->hp,200));

	epointer->hp-=damage;




	SDL_Rect qdest;
	if ((epointer->off.x>0) && (epointer->off.x<scrwi) && (epointer->off.y>0) && (epointer->off.y<scrhi))
	{
createData.g=(Uint32*)QGpalette[5]->pixels;//(Uint32*)mergeimaged[0]->pixels+(rand() % (1024*1000));//(Uint32*)QGpalette[5]->pixels;


if ((finalblow) && (epointer->hp<=0)) 
	{ 
//curphisobj->force.speedx=curphisobj->force.speedy=0; 
//rand() % 6;
for (int i=0; i<3; i++)
{
createData.type=14+i;

VoxelBloodBasicSet(&curphisobj->pos,0,0);
createData.xspeed=-curphisobj->force.speedx/300*(rand() % 20 - 10);
createData.yspeed=-curphisobj->force.speedy/300*(rand() % 20 - 10);
addEffect();
}
}
else
for (int vb=0; vb<1+voxblood*vbloodness; vb++)
{
createData.type=rand() % 6;//rand() % 6;

VoxelBloodBasicSet(&curphisobj->pos,0,0);
createData.xspeed=-curphisobj->force.speedx/30*(rand() % 10);
createData.yspeed=-curphisobj->force.speedy/30*(rand() % 10);
addEffect();
}
		epointer->angles.aSpX+=((float)((rand() % 10) -5) / 500);
		epointer->angles.aSpY+=((float)((rand() % 10) -5) / 500);

	if (d==0)
makeblood ( epointer->i,  curphisobj->force.speedx, curphisobj->force.speedy, damage);
	else
makebackblood ( epointer->i,  curphisobj->force.speedx, curphisobj->force.speedy, damage);
	if ((epointer->type!=0) && (epointer->hp>0))
	{
	if (epointer->imgcopy==0) 
	{
		epointer->gutsout=SDL_ConvertSurface(image[epointer->type], image[epointer->type]->format, SDL_SWSURFACE);
		epointer->imgcopy=1;
	}
	int iy=min(max(accurate(jhy-epointer->pos.y)+image[epointer->type]->h/2, 0), image[epointer->type]->h-1);
//z=rand() % 2; 
qdest.x = rand() % (epointer->gutsout->w);//+epointer->gutsout->w/4;
qdest.y = iy;//rand() % epointer->gutsout->h;

epointer->bleeding+=3;
epointer->bleedx=(qdest.x-image[epointer->type]->w/2)*accuracy;
epointer->bleedy=(qdest.y-image[epointer->type]->h/2)*accuracy;
epointer->bleedspx=-curphisobj->force.speedx/4;
epointer->bleedspy=-curphisobj->force.speedy/4;

SDL_BlitSurface(mergeimage[curphisobj->type+1], NULL, epointer->gutsout , &qdest); 
gutsup(epointer->gutsout,mergeimage[epointer->type+1], qdest.x, qdest.y, 15);

	}
	}

		return 1;
}
int nottakingbullet()
{
	return 0;
}

int fatmovement()
{
int hs=epointer->size/2;
	

	d=0;

	// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
epointer->pos.x+=epointer->force.speedx;
if (epointer->pos.x<0) {epointer->pos.x+=tyleSize*accuracy-1; epointer->pos.tx=epointer->pos.tx-1;}

bool colision=false;

for (int w=-hs; w<epointer->size-hs; w++)
			for (int h=-hs; h<epointer->size-hs; h++)
if (getmematrix (epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x), accurate(epointer->pos.y), accurate(epointer->pos.h))!=0)
{
if ((!getmematrix (epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x), accurate(epointer->pos.y), accurate(epointer->pos.h)+pixInPhis)) && ( !getmematrix (epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x-epointer->force.speedx), accurate(epointer->pos.y), accurate(epointer->pos.h)+pixInPhis)))
epointer->force.speedh+=GravCalc*2;

epointer->pos.x-=epointer->force.speedx;
//epointer->force.speedx/=2;
} else {d=1;}

if (epointer->pos.x>tyleSize*accuracy-1) {epointer->pos.x-=tyleSize*accuracy+1; epointer->pos.tx=epointer->pos.tx+1;}
// YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY
epointer->pos.y+=epointer->force.speedy;
if (epointer->pos.y<0) {epointer->pos.y+=tyleSize*accuracy; epointer->pos.ty=epointer->pos.ty-1;}

if (getmematrix (epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x), accurate(epointer->pos.y), accurate(epointer->pos.h))!=0)
	{
if ((getmematrix (epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x), accurate(epointer->pos.y), accurate(epointer->pos.h)+pixInPhis)==0) && ( getmematrix (epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x), accurate(epointer->pos.y-epointer->force.speedy), accurate(epointer->pos.h)+pixInPhis)==0 ))
epointer->force.speedh+=GravCalc*2;
	epointer->pos.y-=epointer->force.speedy;
	//epointer->force.speedy/=2;
	} else {d=1;}
	
if (epointer->pos.y>tyleSize*accuracy-1) {epointer->pos.y-=tyleSize*accuracy+1; epointer->pos.ty=epointer->pos.ty+1;}

	
//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
epointer->pos.h+=epointer->force.speedh;
//if (epointer->pos.h<0) {epointer->pos.h=0; epointer->force.speedh=0;}
//else
	if ((getmematrix (epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x), accurate(epointer->pos.y), accurate(epointer->pos.h))!=0) || (accurate(epointer->pos.h)>matrhig*pixInPhis))
	{
		
	epointer->pos.h-=epointer->force.speedh;
	epointer->force.speedh=0;
	}
	else
	{
d=1;

	}

// change array
	

// ************************************************* POST move
epointer->force.speedh-=GravCalc;
epointer->force.speedx*=epointer->slowwing;
epointer->force.speedy*=epointer->slowwing;


if (!d) 
	{
epointer->movement=&waitforclock;

epointer->force.speedx=0;
epointer->force.speedy=0;
}
return 0;

}

int shootsomebodys()
{
	if (epointer->aim>10) {
		epointer->clock=epointer->aim/20;
		return 0;
	}
int	k=getsignedintrix (epointer->pos.tx, epointer->pos.ty, epointer->pos.x,epointer->pos.y, epointer->sight);
	if ((k==0) || (e[k].type==0) || (k ==epointer->i )) {epointer->curstate=0; return 0;}
	//signoutmatrix (e[k].pos.tx, e[k].pos.ty, e[k].pos.x,e[k].pos.y);
	// set target
	//epointer->curstate=k;
	bestdist|=1;
	epointer->targ=&e[k];
	//epointer->targi=k;
	//epointer->curstate=k;
	epointer->shootstodo=(epointer->sight/bestdist)*5;
	epointer->curstate=0;
	epointer->acceleration=bestdist;
	epointer->clock=35;
		createData.lightIntensity=0.1;
	createData.lightPisObj=1;
	d=addphisical(epointer->pos.tx, epointer->pos.ty, epointer->pos.x, epointer->pos.y,epointer->pos.h, 17, e[k].pos.tx, e[k].pos.ty, accurate(e[k].pos.x), accurate(e[k].pos.y),epointer->targ->pos.h, 30, 3,0,3,66, 0.995,200);
	k=rand() % 10+3;
	ph[d].pos.x+=(ph[d].force.speedx/10)*k;
	ph[d].pos.y+=(ph[d].force.speedy/10)*k;

	return 1;
}
int keepshooting()
{
epointer->force.speedx=epointer->force.speedx=0;
	if ((epointer->shootstodo>0) && (epointer->curstate<1) && (epointer->aim<100))
	{
	
int	k=radiusx(epointer->aim);
	creature=epointer->targ;
	//d=addphisical(player.host->pos.tx, player.host->pos.ty, player.host->pos.x, player.host->pos.y,player.host->pos.h, 17, cameratylx, cameratyly, camerax+mouse.pos.x+(k*ttheta/tyleSize), cameray+mouse.pos.y+(radiusy*ttheta/tyleSize),0, 30, aim/3+2,0,3,66);
	katet3d((epointer->pos.tx-creature->pos.tx)*tyleSize*accuracy + epointer->pos.x - creature->pos.x, (epointer->pos.ty-creature->pos.ty)*tyleSize*accuracy +epointer->pos.y - creature->pos.y, epointer->pos.h - creature->pos.h );
	fuck/=30;
		createData.lightIntensity=0.5;
	createData.lightPisObj=1;
	d=addphisical(epointer->pos.tx, epointer->pos.ty, epointer->pos.x, epointer->pos.y,epointer->pos.h, 17, creature->pos.tx, creature->pos.ty, accurate(creature->pos.x+creature->force.speedx*fuck+(k*epointer->acceleration)*100), accurate(creature->pos.y+creature->force.speedy*fuck+(radiusy*epointer->acceleration)*100),epointer->targ->pos.h, 30, 300,0,3,66, 0.995,200);
	k=rand() % 10+3;
	ph[d].pos.x+=(ph[d].force.speedx/10)*k;
	ph[d].pos.y+=(ph[d].force.speedy/10)*k;
	epointer->aim+=58;
	epointer->shootstodo-=1;
	epointer->curstate=1;
	}
	else
		{epointer->aim-=20;
	epointer->curstate-=1;
	if (epointer->aim<0) epointer->aim=0;}
	return 0;
}
int chargeonplayer()
{

	epointer->targetOnScreen=&player.host->off;
if (epointer->curstate==epointer->type+2) 
		{
			
				// addphisicalrandom(epointer->pos.tx, epointer->pos.ty, epointer->pos.x, epointer->pos.y, epointer->pos.h, 20, 0, 0,0, randy*6, bloodness,1, 0.99);
				 epointer->hp=epointer->hp-3;
				 epointer->curstate=epointer->type;
			}

epointer->clock=epointer->delay;
if (!player.corporial) return 0;
	av=(player.host->pos.tx-epointer->pos.tx)*tyleSize+accurate(player.host->pos.x-epointer->pos.x);
	bv=(player.host->pos.ty-epointer->pos.ty)*tyleSize+accurate(player.host->pos.y-epointer->pos.y);
	
	epointer->targ=player.host;
	
	//player.health--;

	if ((av>15)||(bv>15)||(av<-15)||(bv<-15))
			{
				if (!bv) bv=1;
				if (!av) av=1;
			if	(getmematrix (epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x)+pixInPhis*(av/abs(av)), accurate(epointer->pos.y)+pixInPhis*(bv/abs(bv)), accurate(epointer->pos.h)))
			{
				if ((epointer->force.speedx/av)<0) 
					epointer->force.speedx=-epointer->force.speedx;
				if ((epointer->force.speedy/bv)<0) 
					epointer->force.speedy=-epointer->force.speedy;
			}
			epointer->force.speedx+=(av*epointer->acceleration/(abs(av)+abs(bv)));
			epointer->force.speedy+=(bv*epointer->acceleration/(abs(av)+abs(bv)));
			}
	else { 	epointer->force.speedx=0; epointer->force.speedy=0;	}
			return 0;
}
int flyonplayer()
{

//signinmatrix ( epointer->pos.tx, epointer->pos.ty, epointer->pos.x,epointer->pos.y, epointer->i, 1);
if (epointer->curstate==epointer->type+2) 
		{
			
				// addphisicalrandom(epointer->pos.tx, epointer->pos.ty, epointer->pos.x, epointer->pos.y, epointer->pos.h, 20, 0, 0,0, randy*6, bloodness,1, 0.99);
				 epointer->hp=epointer->hp-3;
				 epointer->curstate=epointer->type;
			}

epointer->clock=epointer->delay;
if (!player.corporial) return 0;
	av=(player.host->pos.tx-epointer->pos.tx)*tyleSize+accurate(player.host->pos.x-epointer->pos.x);
	bv=(player.host->pos.ty-epointer->pos.ty)*tyleSize+accurate(player.host->pos.y-epointer->pos.y);
	int cv=accurate(player.host->pos.h-epointer->pos.h);
	
	epointer->targ=player.host;
	
	//player.health--;

	if ((av>15)||(bv>15)||(av<-15)||(bv<-15))
			{
				if (!bv) bv=1;
				if (!av) av=1;
			if	(getmematrix (epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x)+pixInPhis*(av/abs(av)), accurate(epointer->pos.y)+pixInPhis*(bv/abs(bv)), accurate(epointer->pos.h)))
			{
				if ((epointer->force.speedx/av)<0) 
					epointer->force.speedx=-epointer->force.speedx;
				if ((epointer->force.speedy/bv)<0) 
					epointer->force.speedy=-epointer->force.speedy;
			}
			fuck=abs(av)+abs(bv)+abs(cv);
			epointer->force.speedx+=(av*epointer->acceleration/fuck);
			epointer->force.speedy+=(bv*epointer->acceleration/fuck);
			epointer->force.speedh+=(cv*epointer->acceleration/fuck);
			}
	else { 	epointer->force.speedx=0; epointer->force.speedy=0;	}
			return 0;
}

int VoxelDeath()
{


//BufferDrowVex(&G_p[2][0], offx, offy,10,  screen,epointer->angles.Ax, epointer->angles.Ay, epointer->angles.Ah,epointer->gutsout);

	//shakeMeBaby(100050);
//int offh=-10;
//rotateOffXOffYh(epointer->angles.Ax, epointer->angles.Ay, epointer->angles.Ah,  &offx,  &offy, -abs((epointer->clock-50))/3, -abs((epointer->clock-50))/3,  &offh);
	
//for (int i=0; i<4; i++)
//{

createData.lightPisObj=0;
createData.type=14 + (rand() % 2);//rand() % 8;//14 + (rand() % 2);


//createData.type=14+(rand() % 3);//rand() % 6;

VoxelBloodBasicSet(&epointer->pos,0,0);
createData.xspeed=radiusx(accuracy*3);
createData.yspeed=radiusy;
createData.h=accuracy*8;
createData.hspeed=accuracy;
addEffect();
//}

	if (epointer->imgcopy) SDL_FreeSurface(epointer->gutsout);
	epointer->gutsout=0;
//if (bloodness>maxbloodness/3)
			//{
	if  (PosOnScreen(&epointer->pos, 100))
	{
splatter(&epointer->pos);
//gayVoxel[2][epointer->voxGut]
	

getione=addphisicalrandomold(epointer->pos.tx, epointer->pos.ty, epointer->pos.x+10, epointer->pos.y,epointer->pos.h+accuracy*11, 835, epointer->force.speedx*(rand() % 20 - 10)/5, epointer->force.speedy*(rand() % 20 - 10)/5,2,1, 400, 40,0 , 0,GutsRandForce, 1,GibsHealth);

//getione =addphisicalrandomold(epointer->pos.tx, epointer->pos.ty, epointer->pos.x+10, epointer->pos.y,epointer->pos.h+accuracy*11, 835, epointer->force.speedx*(rand() % 20 - 10)/5, epointer->force.speedy*(rand() % 20 - 10)/5,2,1, 30, 40,0 , 0,GutsRandForce, 1,GibsHealth);
ph[getione].angles.Ax=epointer->angles.Ax;	ph[getione].angles.Ay=epointer->angles.Ay;	ph[getione].angles.Ah=epointer->angles.Ah;	
ph[getione].angles.aSpH=0;
ph[getione].type=831; 
ph[getione].foo=&bloodyinteractive;
ph[getione].aftermove=&signin; 
ph[getione].collide=&hitChangeRotation; 
ph[getione].finalDrow=&VoxelFinalDrow;
ph[getione].G_c=2; 
ph[getione].slowing=0.5;
if (!epointer->voxGut)epointer->voxGut=10+(rand() % 8); 
ph[getione].G_p=epointer->voxGut;
	//SetRandRotation(0.005);



getitwo = addphisicalrandomold(epointer->pos.tx, epointer->pos.ty, epointer->pos.x+15, epointer->pos.y+5,epointer->pos.h+accuracy*3, 836, 0, 0, 0,1 ,200, 10,0 , 0,GutsRandForce , 1,GibsHealth);
ph[getitwo].angles.Ax=epointer->angles.Ax;	ph[getitwo].angles.Ay=epointer->angles.Ay;	ph[getitwo].angles.Ah=epointer->angles.Ah;		
ph[getitwo].slowing=0.85;

linkelements (getione, getitwo, 20, poweroflinks);

getione =addphisicalrandomold(epointer->pos.tx, epointer->pos.ty, epointer->pos.x+5, epointer->pos.y, epointer->pos.h+accuracy*3,837, epointer->force.speedx*(rand() % 20 - 10)/5, epointer->force.speedy*(rand() % 20 - 10)/5, 0,1,260, 50, 0 , 0,GutsRandForce, 1,GibsHealth);
ph[getione].angles.Ax=epointer->angles.Ax;	ph[getione].angles.Ay=epointer->angles.Ay;	ph[getione].angles.Ah=epointer->angles.Ah;
	ph[getione].slowing=0.85;

			linkelements (getione, getitwo, 15, poweroflinks);
			getione =addphisicalrandomold(epointer->pos.tx, epointer->pos.ty, epointer->pos.x+5, epointer->pos.y+10,epointer->pos.h+accuracy*3, 35, epointer->force.speedx, epointer->force.speedy, 0,1,300, 20,0 , 0,GutsRandForce, 1,GibsHealth); //right arm
ph[getione].slowing=0.85;

			getitwo =addphisicalrandomold(epointer->pos.tx, epointer->pos.ty, epointer->pos.x+10, epointer->pos.y,epointer->pos.h+accuracy*3, 23, epointer->force.speedx, epointer->force.speedy,0,1, 400, 20,0 , 0,GutsRandForce, 1,GibsHealth);
			linkelements (getione, getitwo, 20, poweroflinks);
			ph[getitwo].slowing=0.85;


}
return 0;
}
int RagDollDeath()
{
int n=epointer->limbsSpawned;
Uint16* lno=epointer->SpawnedLimbsNombers;
for (int i=0; i<n; i++){
	if (*lno)	{
		AnyRagLimb* limb=&LIMBS[*lno];
		for (int w=0; w<2; w++)
			for (int h=0; h<2; h++)
				for (int tall=-1; tall<1; tall++)
					TryClearmatrix (limb->pos.tx, limb->pos.ty,accurate(limb->pos.x)+(w<<3), 
						accurate(limb->pos.y)+(h<<3), accurate(limb->pos.h)+(tall<<3), epointer->i+einmatr);
		dismember(limb);
	}
	lno++;
}

createData.lightPisObj=0;
createData.type=14 + (rand() % 2);
VoxelBloodBasicSet(&epointer->pos,0,0);
createData.xspeed=radiusx(accuracy*3);
createData.yspeed=radiusy;
createData.h=accuracy*8;
createData.hspeed=accuracy;
addEffect();



	if  (PosOnScreen(&epointer->pos, 100))	{
			splatter(&epointer->pos);
		}

return 0;
}
int FlyVoxelDeath()
{


//BufferDrowVex(&G_p[2][0], offx, offy,10,  screen,epointer->angles.Ax, epointer->angles.Ay, epointer->angles.Ah,epointer->gutsout);

	//shakeMeBaby(100050);
//int offh=-10;
//rotateOffXOffYh(epointer->angles.Ax, epointer->angles.Ay, epointer->angles.Ah,  &offx,  &offy, -abs((epointer->clock-50))/3, -abs((epointer->clock-50))/3,  &offh);




	//splatter();


	if (epointer->imgcopy) SDL_FreeSurface(epointer->gutsout);
	epointer->gutsout=0;
//if (bloodness>maxbloodness/3)
			//{
if (PosOnScreen(&epointer->pos, 100))
	{
PhisicalVoxelGuts();
ph[getione].G_c=5; ph[getione].G_p=19;
ph[getione].collide=&FlyGutsParticalize;
PhisicalVoxelGuts();
ph[getione].G_c=9; ph[getione].G_p=5;
ph[getione].collide=&hitChangeRotation;
}
return 0;
}
int basicdeath()
{
if (epointer->imgcopy) SDL_FreeSurface(epointer->gutsout);
	epointer->gutsout=0;

	if (PosOnScreen(&epointer->pos, 100))
	{
splatter(&epointer->pos);
//G8BloodBasicSet(&epointer->pos);
//addEffect();

	
//if (bloodness>maxbloodness/3)
			//{
	createData.lightPisObj=0;
	getione = addphisicalrandomold(epointer->pos.tx, epointer->pos.ty, epointer->pos.x+10, epointer->pos.y,epointer->pos.h+accuracy*11, 828, epointer->force.speedx*(rand() % 20 - 10)/5, epointer->force.speedy*(rand() % 20 - 10)/5,0,1, 300, 40,0 , 0,GutsRandForce, 1,100);
	getitwo = addphisicalrandomold(epointer->pos.tx, epointer->pos.ty, epointer->pos.x+15, epointer->pos.y+5,epointer->pos.h+accuracy*3, 27, 0, 0, 0,1 ,10, 10,0 , 0, GutsRandForce, 1,100);
			linkelements (getione, getitwo, 20, poweroflinks);
			getione = addphisicalrandomold(epointer->pos.tx, epointer->pos.ty, epointer->pos.x+5, epointer->pos.y, epointer->pos.h+accuracy*3,30, epointer->force.speedx*(rand() % 20 - 10)/5, epointer->force.speedy*(rand() % 20 - 10)/5, 0,1,165, 50, 0 , 0,GutsRandForce, 1,100);
			//if (epointer->hp>-50) 
				linkelements (getione, getitwo, 15, poweroflinks);
			getione = addphisicalrandomold(epointer->pos.tx, epointer->pos.ty, epointer->pos.x+5, epointer->pos.y+10,epointer->pos.h+accuracy*3, 35, epointer->force.speedx, epointer->force.speedy, 0,1,10, 20,0 , 0,GutsRandForce, 1,100); //right arm
			//if (epointer->hp>-100) 
				linkelements (getione, getitwo, 10, poweroflinks);
			getitwo = addphisicalrandomold(epointer->pos.tx, epointer->pos.ty, epointer->pos.x+10, epointer->pos.y,epointer->pos.h+accuracy*3, 23, epointer->force.speedx, epointer->force.speedy,0,1, 20, 20,0 , 0,GutsRandForce, 1,100);
			//if (epointer->hp>-150)
				linkelements (getione, getitwo, 5, poweroflinks);
	}			
			//}
			/*else
			{
addphisicalrandom(epointer->pos.tx, epointer->pos.ty, epointer->pos.x+5, epointer->pos.y, epointer->pos.h+accuracy*3, 816, epointer->force.speedx, epointer->force.speedy, 0, 80, randy*7, 1,2);
//addphisicalrandom(epointer->pos.tx, epointer->pos.ty, epointer->pos.x+5, epointer->pos.y, epointer->pos.h+accuracy*3, 27, epointer->force.speedx, epointer->force.speedy, 0, 80, randy*7, 1);
//addphisicalrandom(epointer->pos.tx, epointer->pos.ty, epointer->pos.x+5, epointer->pos.y, epointer->pos.h+accuracy*3, 35, epointer->force.speedx, epointer->force.speedy, 0, 80, randy*7, 1);
//addphisicalrandom(epointer->pos.tx, epointer->pos.ty, epointer->pos.x+5, epointer->pos.y, epointer->pos.h+accuracy*3, 23, epointer->force.speedx, epointer->force.speedy, 0, 80, randy*7, 1);
drowtolend (accurate(epointer->pos.x), accurate(epointer->pos.y), mergeimage[epointer->type], tylextogrid(epointer->pos.tx)+1, tyleytogrid(epointer->pos.ty)+1);
int d=(tyleSize-mergeimage[epointer->type]->w)*accuracy;
int z=(tyleSize-mergeimage[epointer->type]->h)*accuracy;
if (epointer->pos.x>d) 
drowtolend (accurate(epointer->pos.x)-tyleSize, accurate(epointer->pos.y), mergeimage[epointer->type], tylextogrid(epointer->pos.tx+1)+1, tyleytogrid(epointer->pos.ty)+1);
if (epointer->pos.y>z) 
drowtolend (accurate(epointer->pos.x), accurate(epointer->pos.y)-tyleSize, mergeimage[epointer->type], tylextogrid(epointer->pos.tx)+1, tyleytogrid(epointer->pos.ty+1)+1);
if ((epointer->pos.y>z) && (epointer->pos.x>d))  
drowtolend (accurate(epointer->pos.x)-tyleSize, accurate(epointer->pos.y)-tyleSize, mergeimage[epointer->type], tylextogrid(epointer->pos.tx+1)+1, tyleytogrid(epointer->pos.ty+1)+1);*/
			//}
return 0;
}
int fatdeath()
{
//addWallFromDeadFuckersTorso
//shakeMeBaby(200050);
	//createVortex(epointer->off.x, epointer->off.y, 200000, 40);

	if (PosOnScreen(&epointer->pos, 100))
{
	createData.g=(Uint32*)QGpalette[5]->pixels;
for (int vb=0; vb<12; vb++)
{
createData.type=14+vb/4;//rand() % 6;

VoxelBloodBasicSet(&epointer->pos,createData.xspeed/50000,createData.yspeed/50000);
createData.decaySurfaceNo=13;
createData.xspeed=((rand() % 100)-50)*5000;
createData.yspeed=((rand() % 100)-50)*5000;
addEffect();
}

for (int vb=0; vb<10; vb++)
{
createData.type=14 + (rand() % 2);//rand() % 6;

VoxelBloodBasicSet(&epointer->pos,createData.xspeed/20000,createData.yspeed/20000);
createData.decaySurfaceNo=13;
createData.xspeed=((rand() % 100)-50)*5000;
createData.yspeed=((rand() % 100)-50)*5000;
addEffect();
}

	impactslomo+=100;
createRadialWind(epointer->off.x, epointer->off.y, 40000, 40);

int wno=addobjectdata (910, epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x)-image[epointer->type]->w/2, accurate(epointer->pos.y)-image[epointer->type]->w/2, 0, 0, 255, accurate(epointer->pos.h)/pixInPhis, now, 0);
//if (epointer->imgcopy) SDL_FreeSurface(epointer->gutsout);
epointer->imgcopy=0;
//SDL_FreeSurface(w[wno].brick[0]);
//w[wno].height=0;
if (epointer->gutsout){
if (wno) w[wno].brick=epointer->gutsout;
else SDL_FreeSurface(epointer->gutsout);
}
else{
if (wno) w[wno].brick=SDL_ConvertSurface(image[epointer->type], image[epointer->type]->format, SDL_SWSURFACE);
}
epointer->gutsout=0;
//w[wno].=mergeimage[epointer->type];
w[wno].brickcopy=1;
w[wno].randGenerated=1;
	}
	else 
	{

if (epointer->imgcopy) SDL_FreeSurface(epointer->gutsout);
epointer->gutsout=0;
epointer->imgcopy=0;
	}
//w[wno].hp=40;

//for (int z=0;z<=w[wno].h;z++)


	
	return 0;
}

int basicEnemyDrow()
{
	CastShadowForDarkenfFunk(heightimage[epointer->type],screen,offx+shorth*3,offy-image[epointer->type]->h/2,epointer->pos.h,0);
/*int elevation=epointer->pos.h/dddstength;
offx+=((offx-scrwid)*elevation)>>16;
offy+=((offy-scrhid)*elevation)>>16;*/



//DinamicShadowObject (image[epointer->type+1], image[epointer->type], shadowmaps[tylextogrid(epointer->pos.tx)+1][tyleytogrid(epointer->pos.ty)+1],shortx , shorty,  offx,  offy);

int elevation=epointer->pos.h/dddstength;
offx+=((offx-scrwid)*elevation)>>16;
offx-=image[epointer->type]->w/2;
offy+=((offy-scrhid)*elevation)>>16;
offy-=image[epointer->type]->h/2;


	if (epointer->pos.h>524280)
	{
	if (!epointer->imgcopy) 
		IndexedDrowWithShadowErasing (image[epointer->type], offx, offy, epointer->type);
	//drowstyle(offx-image[epointer->type]->w/2,offy-image[epointer->type]->h/2,epointer->pos.h-heimod,epointer->type);
else
	IndexedDrowWithShadowErasing (epointer->gutsout, offx, offy,epointer->type);
	//sdldrowstyle(offx-image[epointer->type]->w/2,offy-image[epointer->type]->h/2,epointer->pos.h-heimod,epointer->gutsout);
	}
	else
	{
if (!epointer->imgcopy)
DinamicShadowObject (image[epointer->type+1], image[epointer->type], &shadUp[tylextogrid(epointer->pos.tx)+1][tyleytogrid(epointer->pos.ty)+1],shortx , shorty,  offx,  offy,0);
else
DinamicShadowObject (image[epointer->type+1], epointer->gutsout, &shadUp[tylextogrid(epointer->pos.tx)+1][tyleytogrid(epointer->pos.ty)+1],shortx , shorty,  offx,  offy,0);
	

if (shorty+image[epointer->type]->h>tyleSize)
if (!epointer->imgcopy)
DinamicShadowObject (image[epointer->type+1], image[epointer->type], &shadUp[tylextogrid(epointer->pos.tx)+1][tyleytogrid(epointer->pos.ty+1)+1],shortx , shorty-tyleSize,  offx,  offy,0);
else
DinamicShadowObject (image[epointer->type+1], epointer->gutsout, &shadUp[tylextogrid(epointer->pos.tx)+1][tyleytogrid(epointer->pos.ty+1)+1],shortx , shorty-tyleSize,  offx,  offy,0);
	

	if (shortx+image[epointer->type]->w>tyleSize)
if (!epointer->imgcopy)
DinamicShadowObject (image[epointer->type+1], image[epointer->type], &shadUp[tylextogrid(epointer->pos.tx+1)+1][tyleytogrid(epointer->pos.ty)+1],shortx-tyleSize , shorty,  offx,  offy,0);
else
DinamicShadowObject (image[epointer->type+1], epointer->gutsout, &shadUp[tylextogrid(epointer->pos.tx+1)+1][tyleytogrid(epointer->pos.ty)+1],shortx-tyleSize , shorty,  offx,  offy,0);
	


if ((shorty+image[epointer->type]->h>tyleSize) && (shortx+image[epointer->type]->w>tyleSize))
if (!epointer->imgcopy)
DinamicShadowObject (image[epointer->type+1], image[epointer->type], &shadUp[tylextogrid(epointer->pos.tx+1)+1][tyleytogrid(epointer->pos.ty+1)+1],shortx-tyleSize , shorty-tyleSize,  offx,  offy,0);
else
DinamicShadowObject (image[epointer->type+1], epointer->gutsout, &shadUp[tylextogrid(epointer->pos.tx+1)+1][tyleytogrid(epointer->pos.ty+1)+1],shortx-tyleSize , shorty-tyleSize,  offx,  offy,0);
	
	}
return 0;
}
int drowZergVoxel()
{
int osx=offx+shorth*3;
int osy=offy-image[epointer->type]->h/2;
/*
if (epointer->anglex>2*M_PI) epointer->anglex-=2*M_PI;
if (epointer->anglex<0) epointer->anglex+=2*M_PI;
if (epointer->angley>2*M_PI) epointer->angley-=2*M_PI;
if (epointer->angley<0) epointer->angley+=2*M_PI;*/



int elevation=epointer->pos.h/dddstength;
offx+=((offx-scrwid)*elevation)>>16;
//offx-=image[epointer->type]->w/2;
offy+=((offy-scrhid)*elevation)>>16;
//offy-=image[epointer->type]->h/2;

if (!epointer->force.speedx) epointer->force.speedx=1;
double ah=getAngle(epointer->force.speedx, epointer->force.speedy);
ah-=0.8;

//Fixing angle
double teorAngl=epointer->angles.Ah-2*M_PI;
if (abs(teorAngl-ah)<abs(epointer->angles.Ah-ah)) epointer->angles.Ah=teorAngl;
teorAngl=epointer->angles.Ah+2*M_PI;
if (abs(teorAngl-ah)<abs(epointer->angles.Ah-ah)) epointer->angles.Ah=teorAngl;
//playerW->angles.Ah+=((ah-epointer->angles.Ah)/fpstep);

epointer->angles.Ah+=((ah-epointer->angles.Ah)/30)*GameSpeed;

epointer->angles.Ax+=epointer->angles.aSpX*GameSpeed;
epointer->angles.Ay+=epointer->angles.aSpY*GameSpeed;


//if (epointer->hp<1300) {epointer->hp-=10;  epointer->clock+=GameSpeed; }

//Head:
/*ah=getAngle( player.offx-offx, player.offy-offy);

if (epointer->gutsout)
BufferDrowVex(&G_p[2][2], offx, offy,40,  screen,epointer->angles.Ax, epointer->angles.Ay, ah,epointer->gutsout);
else
DrowVex(&G_p[2][2], offx, offy,40,  screen,epointer->angles.Ax, epointer->angles.Ay, ah);
*/
int ox=offx;
int oy=offy;
//Legs
int offh=-10;
rotateOffXOffYh(epointer->angles.Ax, epointer->angles.Ay, epointer->angles.Ah,  &offx,  &offy, -abs((epointer->clock-epointer->delay/2))/3, -abs((epointer->clock-epointer->delay/2))/3,  &offh);

if (!PosOnScreen(&epointer->pos, 25)) return 0;

if (flashlight)
{
	if (!GetDarkMaskGrid(offx-16, offy-16, 32, 32)) return 0;
	if (epointer->bleeding){
	G8bitVexToLight(&gayVoxel[11][1], 1,1+(rand() % 100) * 0.005, epointer->angles.Ax+Vox3Deff*(offx-scrwid), epointer->angles.Ay-Vox3Deff*(offy-scrhid), 
		epointer->angles.Ah+ (rand() % 100)*0.01-0.5, offx, offy, epointer->bleeding);
	epointer->bleeding=max(epointer->bleeding-1, 0);
	
}

}
		int radius=gayVoxel[2][1].radius;
		if (FullyOnScreen(offx, offy, gayVoxel[2][1].radius*2, gayVoxel[2][1].radius*2))
		{
		//QuickDrow8bitVexStaticH(&epointer->pos, &gayVoxel[2][1], 0, 0, 32, 1.5, epointer->angles.Ax+Vox3Deff*(offx-scrwid), epointer->angles.Ay-Vox3Deff*(offy-scrhid), epointer->angles.Ah,0);
		
QuickDrow8bitVex(&palette.Buffer8bit[0],&epointer->pos, &gayVoxel[2][1], 0,0, accurate(epointer->pos.h), 1+(double)accurate(epointer->pos.h)/VoxUpResize, epointer->angles.Ax+Vox3Deff*(offx-scrwid), 
				epointer->angles.Ay-Vox3Deff*(offy-scrhid), epointer->angles.Ah,0);			
			//DrowBuffer(gayVoxel[2][1].palette, offx, offy, screen);
		}
		offx=ox;
		offy=oy;
		if (!flashlight) CastShadowForDarkenfFunk(heightimage[epointer->type],screen,osx,osy,epointer->pos.h,0);
		//Torso:
		radius=gayVoxel[2][0].radius;
		if (FullyOnScreen(offx, offy, gayVoxel[2][1].radius*2, gayVoxel[2][1].radius*2))
		{
QuickDrow8bitVex(&palette.Buffer8bit[0],&epointer->pos, &gayVoxel[2][epointer->voxGut], 0,0, accurate(epointer->pos.h)+24, 1+(double)accurate(epointer->pos.h)/VoxUpResize, epointer->angles.Ax+Vox3Deff*(offx-scrwid), 
				epointer->angles.Ay-Vox3Deff*(offy-scrhid), epointer->angles.Ah,0);
		/*	QuickDrow8bitVexStaticH(&epointer->pos, &gayVoxel[2][epointer->voxGut], 0, 0, 64, 
				1.2+(double)accurate(epointer->pos.h)/VoxUpResize, epointer->angles.Ax+Vox3Deff*(offx-scrwid), 
				epointer->angles.Ay-Vox3Deff*(offy-scrhid), epointer->angles.Ah,PhisObjectsDefault);*/
		//QuickDrow8bitVex(&epointer->pos, &gayVoxel[5][0], 0, 20, 10, 1, epointer->angles.Ax+Vox3Deff*(offx-scrwid), epointer->angles.Ay-Vox3Deff*(offy-scrhid), epointer->angles.Ah,PhisObjectsDefault);
		//DrowBuffer
LightsTo180arrayF(&epointer->pos, defAngleFade,epointer->angles.Ah , 0,deAlphaMask,0);
//0x795b00
// Edjust global illumination:

Uint32 mgvcol=0x5a5030;

OnelightR=min(255, (OnelightR*((mgvcol>>16) & 0xFF)>>8));
OnelightG=min(255, (OnelightG*((mgvcol>>8) & 0xFF)>>8));
OnelightB=min(255, (OnelightB*((mgvcol) & 0xFF)>>8));

epointer->light.color=OnelightB | (OnelightG<<8) | (OnelightR<<16);


			if (flashlight) //&& (!epointer->voxGut))
			DrowBufferByLight(gayVoxel[2][0].palette, offx, offy, screen);
			else //DrowBuffer(gayVoxel[2][0].palette, offx, offy, screen);
DrowBufferByHeight(gayVoxel[2][0].palette,  offx, offy, screen,1);
		
		}
//}
//CastShadowForDarkenfFunk(heightimage[epointer->type],screen,offx+shorth*3,offy-image[epointer->type]->h/2);
return 0;
}
int drowFirstRagDoll()
{
	
PosToFlatOffs(&epointer->pos);
int cenx=offx;
int ceny=offy;
int n=epointer->limbsSpawned;
Uint16* lno=epointer->SpawnedLimbsNombers;
bool drow=PosOnScreen(&epointer->pos, 25);

for (int i=0; i<n; i++){
	if (*lno)	{
		AnyRagLimb* limb=&LIMBS[*lno];
		
			
			

		if (drow){
			PosToFlatOffs(&limb->pos);
			LimbGroupDrow(limb, offx-cenx, offy-ceny);	
		}
if (epointer->hp>0) {
		limb->force.speedx*=limb->force.pr;
				limb->force.speedy*=limb->force.pr;
				limb->force.speedh*=limb->force.pr;

		if ((limb->hp>0) && (!(limb->flags & LIMB_Dismembered))) {
			for (int w=0; w<2; w++)
			for (int h=0; h<2; h++)
				for (int tall=-1; tall<1; tall++)
					TryFillrmatrix (limb->pos.tx, limb->pos.ty,accurate(limb->pos.x)+(w<<3), 
						accurate(limb->pos.y)+(h<<3), accurate(limb->pos.h)+(tall<<3), epointer->i+einmatr);
		} else
			{
				if (limb->hp<0) limb->connectedNo=0;
				dismember(limb);
				*lno=0;
			}
}

	}
	lno++;
}
if (drow){
	// For propper quality change angle to 0 and fix MegaVox8bitDrow
	LightsTo180arrayF(&epointer->pos, defAngleFade,0 , 0,deAlphaMask,0);
DrowBufferByHeight(mgvDB.ConvertedPalette[LIMBS[epointer->SpawnedLimbsNombers[0]].G8piece],  cenx, ceny, screen,1);
}

if (epointer->hp>0){
	JustLegs();

	epointer->animationTime+=AnimationModifier*GameSpeed;
	if (epointer->animationTime>=animEd.usableTestingAnimation.animationTime)
		epointer->animationTime=0;

	Animation(&animEd.usableTestingAnimation , epointer->SpawnedLimbsNombers, epointer->limbsSpawned, epointer->animationTime);
	

}

return 0;
}
int drowFlysomebodyVoxel()
{
int osx=offx+shorth*3;
int osy=offy-image[epointer->type]->h/2;


int elevation=epointer->pos.h/dddstength;
offx+=((offx-scrwid)*elevation)>>16;
//offx-=image[epointer->type]->w/2;
offy+=((offy-scrhid)*elevation)>>16;
//offy-=image[epointer->type]->h/2;

if (!epointer->force.speedx) epointer->force.speedx=1;
double targetAngle=getAngle((player.host->pos.tx-epointer->pos.tx)*tyleSize+accurate(player.host->pos.x-epointer->pos.x), (player.host->pos.ty-epointer->pos.ty)*tyleSize+accurate(player.host->pos.y-epointer->pos.y));
double ah=getAngle(epointer->force.speedx, epointer->force.speedy);
//double ah=targetAngle;
ah-=0.8;
//Fixing angle (make into function)
double teorAngl=epointer->angles.Ah-2*M_PI;
if (abs(teorAngl-ah)<abs(epointer->angles.Ah-ah)) epointer->angles.Ah=teorAngl;
teorAngl=epointer->angles.Ah+2*M_PI;
if (abs(teorAngl-ah)<abs(epointer->angles.Ah-ah)) epointer->angles.Ah=teorAngl;
//playerW->angles.Ah+=((ah-epointer->angles.Ah)/fpstep);

epointer->angles.Ah+=((ah-epointer->angles.Ah)/30);

epointer->angles.Ax+=epointer->angles.aSpX;
epointer->angles.Ay+=epointer->angles.aSpY;


//epointer->angles.Ax+=targetAngle-ah;

//Head:
/*ah=getAngle( player.offx-offx, player.offy-offy);

if (epointer->gutsout)
BufferDrowVex(&G_p[2][2], offx, offy,40,  screen,epointer->angles.Ax, epointer->angles.Ay, ah,epointer->gutsout);
else
DrowVex(&G_p[2][2], offx, offy,40,  screen,epointer->angles.Ax, epointer->angles.Ay, ah);
*/
//int ox=offx;
//int oy=offy;
//Legs

//offx=ox;
//offy=oy;
CastShadowForDarkenfFunk(heightimage[epointer->type],screen,osx,osy, epointer->pos.h,0);
//Torso:
double xrot=-(targetAngle-epointer->angles.Ah)/2+epointer->clock*M_PI*2/epointer->delay;
if (epointer->gutsout)
BufferDrowVex(&G_p[6][2], offx, offy,40,  screen,epointer->angles.Ax+xrot, epointer->angles.Ay, epointer->angles.Ah-0.8,epointer->gutsout);
else
SuperFastVoxelHTP(&G_p[6][2], offx, offy,40,  screen,epointer->angles.Ax+xrot, epointer->angles.Ay, epointer->angles.Ah-0.8);//,(Uint32*)epointer->gutsout->pixels);

//	SuperFastQVoxelSimetrical(&QG_p[6][0], offx, offy,10,  screen,epointer->angles.Ax+xrot, epointer->angles.Ay, epointer->angles.Ah-0.8,(Uint32*)epointer->gutsout->pixels);
//BufferDrowVex(&G_p[2][0], offx, offy,10,  screen,epointer->angles.Ax, epointer->angles.Ay, epointer->angles.Ah,epointer->gutsout);
//else
//DrowVex(&G_p[2][0], offx, offy,10,  screen,epointer->angles.Ax, epointer->angles.Ay, epointer->angles.Ah);
SuperFastQVoxelSimetrical(&QG_p[6][0], offx, offy,10,  screen,epointer->angles.Ax+xrot, epointer->angles.Ay, epointer->angles.Ah-0.8, (Uint32*)image[epointer->type]->pixels );
//SuperFastVoxel(&G_p[2][0], offx, offy,10,  screen,epointer->angles.Ax, epointer->angles.Ay, epointer->angles.Ah);


//CastShadowForDarkenfFunk(heightimage[epointer->type],screen,offx+shorth*3,offy-image[epointer->type]->h/2);
return 0;
}
int drowDarkPrince()
{

int elevation=(epointer->pos.h+accuracy*8)/dddstength;
offx+=((offx-scrwid)*elevation)>>16;
offy+=((offy-scrhid)*elevation)>>16;

epointer->walk.forceAccel.speedx=epointer->force.speedx-epointer->walk.forceAccel.speedx;
epointer->walk.forceAccel.speedy=epointer->force.speedy-epointer->walk.forceAccel.speedy;
epointer->walk.forceAccel.speedh=epointer->force.speedh-epointer->walk.forceAccel.speedh;

AnimateWalkingInit();


epointer->walk.forceAccel.speedx=epointer->force.speedx;
epointer->walk.forceAccel.speedy=epointer->force.speedy;
epointer->walk.forceAccel.speedh=epointer->force.speedh;
return 0;
}
//combos
int rotationbasic()
{
if (epointer->angles.Ax>2*M_PI) epointer->angles.Ax-=2*M_PI;
if (epointer->angles.Ax<0) epointer->angles.Ax+=2*M_PI;
if (epointer->angles.Ay>2*M_PI) epointer->angles.Ay-=2*M_PI;
if (epointer->angles.Ay<0) epointer->angles.Ay+=2*M_PI;

epointer->angles.aSpX*=0.8;
epointer->angles.aSpY*=0.8;

epointer->angles.Ax*=0.8;
epointer->angles.Ay*=0.8;
//epointer->angles.aSpX-=((epointer->angles.aSpX)/5);
//epointer->angles.aSpY-=((epointer->angles.aSpY)/5);
//if (abs(epointer->angles.aSpX)<abs(epointer->angles.Ax)) epointer->angles.aSpX-=epointer->angles.Ax/10;
//if (abs(epointer->angles.aSpY)<abs(epointer->angles.Ay)) epointer->angles.aSpY-=epointer->angles.Ay/10;

basicmovementfunction();
return 0;
}
void eDataAssign(int i)
{
	e[i].light.color=0x5a5030;
	e[i].light.dinamic=1;
	e[i].light.distanceFading=0.005;
	e[i].light.angleLight=0;
	e[i].light.brightness=0.4;
	AddLight(&e[i].pos, &e[i].light);


e[i].targetOnScreen=&mouse.pos;
e[i].controled=0;
e[i].walk.legs.lleg.tx=e[i].pos.tx;
e[i].walk.legs.lleg.ty=e[i].pos.ty;
e[i].walk.legs.lleg.x=e[i].pos.x;
e[i].walk.legs.lleg.y=e[i].pos.y;

e[i].walk.legs.rleg.tx=e[i].pos.tx;
e[i].walk.legs.rleg.ty=e[i].pos.ty;
e[i].walk.legs.rleg.x=e[i].pos.x;
e[i].walk.legs.rleg.y=e[i].pos.y;

//e[i].bag.LeftHand.overheating=1;
//e[i].bag.RightHand.overheating=1;

/*for (int bx=0; bx<8; bx++)
				for (int by=0; by<8; by++)
					e[i].bag.pack[bx][by].count=0;
e[i].bag.LeftHand.count=0;
e[i].bag.RightHand.count=0;*/

//e[i].walk.itemInHand=&e[i].bag.RightHand;
e[i].walk.blade.swingDirection=1;

	e[i].acceleration=accuracy*(3+(rand() % 3));
		e[i].aim=1;
		e[i].off.x=-100;
		e[i].off.y=-100;
		e[i].slowwing=0.94+(0.005*(rand() % 5));
		e[i].pos.h=0;
		e[i].force.mass=50+(rand() % 50);
		e[i].imgcopy=false;
		e[i].hp=200+ (rand() % 500);
		e[i].sight=500;
		e[i].force.speedx=0;
		e[i].force.speedy=0;
		e[i].force.speedh=0;
		e[i].clock=0;
		e[i].delay=40 + (rand() % 50);
		e[i].size=2;
		e[i].height=2;

		e[i].angles.Ax=0; e[i].angles.Ay=0; e[i].angles.aSpX=0; e[i].angles.aSpY=0;


	e[i].friendly=0;

	e[i].drow=&basicEnemyDrow;
	e[i].damaged=&takingbullet; 
	e[i].movement=&basicmovementfunction;
	e[i].death=&basicdeath;
	e[i].clockzero=&chargeonplayer;

if (e[i].type==603)
{
	e[i].friendly=1;
e[i].movement=&keepshooting;  e[i].damaged=&nottakingbullet;  e[i].death=&basicdeath; e[i].clockzero=&shootsomebodys; 
	

	}
if (e[i].type==605)
{
	
		  e[i].death=&fatdeath; 
		 	
				e[i].height=5;
				e[i].size=4;
				e[i].hp=5000;
				e[i].force.mass=2000;
				e[i].acceleration=accuracy*2;
				e[i].delay=100;
				e[i].slowwing=0.997;
}
if (e[i].type==607)
{
	e[i].movement=&keepshooting; e[i].clockzero=&shootsomebodys;	
		  e[i].death=&basicdeath;  
		
}

if (e[i].type==609)
{
	e[i].height=3;
	e[i].size=3;
	e[i].force.mass=300;
	e[i].hp=300;//+(rand() % 20)*100;
	e[i].delay=50;//50;
	e[i].slowwing=0.98;//0.996;
e[i].death=&VoxelDeath; e[i].damaged=&takingVoxelBullet;
		e[i].drow=&drowZergVoxel; e[i].acceleration=accuracy*2;  e[i].movement=&rotationbasic;		  
}

if (e[i].type==611)
{
	e[i].force.mass=700;
	e[i].hp=2000;
	e[i].acceleration=accuracy;
	e[i].size=4;
	e[i].delay=200;
	e[i].slowwing=0.997;
	e[i].pos.h=accuracy*pixInPhis*4;
	e[i].death=&FlyVoxelDeath;
	e[i].damaged=&takingWingsDamage;
	e[i].clockzero=&flyonplayer;
		e[i].drow=&drowFlysomebodyVoxel; e[i].acceleration=accuracy*2;  e[i].movement=&flyMovement;
		  	
}

if (e[i].type==613)
{
e[i].height=3;
e[i].size=4;
e[i].death=&VoxelDeath; e[i].damaged=&takingVoxelBullet;
e[i].drow=&drowDarkPrince; e[i].movement=&rotationbasic;
}

if (e[i].type==614)
{
	e[i].height=3;
	e[i].size=3;
	e[i].force.mass=300;
	e[i].hp=800;//1800;//+(rand() % 20)*100;
	e[i].delay=50;//50;
	e[i].slowwing=0.98;//0.996;
	e[i].death=&RagDollDeath; e[i].damaged=&takingRagDollBullet;
	e[i].drow=&drowFirstRagDoll;//&drowZergVoxel;//
	e[i].acceleration=accuracy*2;  e[i].movement=&RagDollMovementFunction;	
	e[i].limbsSpawned=SpawnRagDoll(0,0, &e[i].pos);
	memcpy(e[i].SpawnedLimbsNombers,SpawnedLimbsNombers, 2*e[i].limbsSpawned);
}


e[i].defaultMovement=e[i].movement;
}
int eselect(int i)
{
	if (i<matrhig-1)
{


if ((offx>scrwid+20) && (offy>scrhid+20)) 
{  
if ((getmematrix (epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x), accurate(epointer->pos.y), accurate(epointer->pos.h)+pixInPhis)==0) ||
(getmematrix (epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x)-pixInPhis, accurate(epointer->pos.y)-10, accurate(epointer->pos.h)+pixInPhis)==0)||
(getmematrix (epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x), accurate(epointer->pos.y)-pixInPhis, accurate(epointer->pos.h)+pixInPhis)==0)||
(getmematrix (epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x)-pixInPhis, accurate(epointer->pos.y), accurate(epointer->pos.h)+pixInPhis)==0)) { d=1; return 1;}
}
else
if ((offx>scrwid+20) && (offy<scrhid-20)) 
{  
if ((getmematrix (epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x), accurate(epointer->pos.y), accurate(epointer->pos.h)+pixInPhis)==0) ||
(getmematrix (epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x)-pixInPhis, accurate(epointer->pos.y)+pixInPhis, accurate(epointer->pos.h)+pixInPhis)==0)||
(getmematrix (epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x), accurate(epointer->pos.y)+pixInPhis, accurate(epointer->pos.h)+pixInPhis)==0)||
(getmematrix (epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x)-pixInPhis, accurate(epointer->pos.y), accurate(epointer->pos.h)+pixInPhis)==0)) { d=1; return 1;}
}
else
if ((offx<scrwid-20) && (offy>scrhid+20)) 
{  
if ((getmematrix (epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x), accurate(epointer->pos.y), accurate(epointer->pos.h)+pixInPhis)==0) ||
(getmematrix (epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x)+pixInPhis, accurate(epointer->pos.y)-pixInPhis, accurate(epointer->pos.h)+pixInPhis)==0)||
(getmematrix (epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x), accurate(epointer->pos.y)-pixInPhis, accurate(epointer->pos.h)+pixInPhis)==0)||
(getmematrix (epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x)+pixInPhis, accurate(epointer->pos.y), accurate(epointer->pos.h)+pixInPhis)==0)) { d=1; return 1;}
}
else
if ((offx<scrwid-20) && (offy<scrhid-20)) 
{  
if ((getmematrix (epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x), accurate(epointer->pos.y), accurate(epointer->pos.h)+pixInPhis)==0) ||
(getmematrix (epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x)+pixInPhis, accurate(epointer->pos.y)+pixInPhis, accurate(epointer->pos.h)+pixInPhis)==0)||
(getmematrix (epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x), accurate(epointer->pos.y)+pixInPhis, accurate(epointer->pos.h)+pixInPhis)==0)||
(getmematrix (epointer->pos.tx, epointer->pos.ty, accurate(epointer->pos.x)+pixInPhis, accurate(epointer->pos.y), accurate(epointer->pos.h)+pixInPhis)==0)) { d=1; return 1;}
}
else d=1;

}
else d=1;
return 0;
}
void removesomebody (somebody* mo, int i, int kill)
{
	if (kill==0) epointer=epointer->next;

	
	if (mo->prev==NULL)  
	{
		efirst[i]=mo->next; 
		if (mo->next!=0)
		mo->next->prev=NULL; 
	}
	else
	{
		mo->prev->next=mo->next;
		if (mo->next!=NULL)
		mo->next->prev=mo->prev;
	}
	//mo->type=0;
	if (kill==0)
	{
	int hig=accurate(mo->pos.h)/pixInPhis+matrdepth;
	if (hig>matrhig+matrdepth-1) hig=matrhig+matrdepth-1;
	else
		if (hig<0) { mo->hp=-10; hig=0;}
		if (efirst[hig]!=NULL) efirst[hig]->prev=mo;
		mo->prev=NULL;
		mo->next=efirst[hig];
		
		efirst[hig]=mo; 
	}
	else
	{
		signoutmatrix (mo->pos.tx, mo->pos.ty, mo->pos.x,mo->pos.y);
		mo->type=0;
		mo->light.active=0;
		
	}


}

void enemiesAndAI (int i)
{
	//ENEMIES
	epointer=efirst[i];
	while (epointer)
	{
testCountEnemies++;
if (epointer->type) 
	{
		shortx=epointer->pos.x>>16;
		shorty=epointer->pos.y>>16;
		shorth=epointer->pos.h>>16;
		int hs=epointer->size/2;
		for (int w=-hs; w<epointer->size-hs; w++)
			for (int h=-hs; h<epointer->size-hs; h++)
				for (int tall=0; tall<epointer->height; tall++)
					TryClearmatrix (epointer->pos.tx, epointer->pos.ty, shortx+(w<<3), shorty+(h<<3), shorth+(tall<<3), epointer->i+einmatr);

		if (epointer->hp>-1)
		{	
		epointer->clock-=GameSpeed;

if (!epointer->controled){
if (epointer->clock<0) 
{
	if (epointer->movement==&waitforclock)	epointer->movement=epointer->defaultMovement;
epointer->clockzero();

if ((abs(epointer->pos.tx-player.host->pos.tx)>5) || (abs(epointer->pos.ty-player.host->pos.ty)>5)) epointer->hp=-1;
}
} else
	if (epointer->clock>5000) epointer->hp=-1;

if (!epointer->controled)
epointer->movement();
else
	if (epointer==player.host)
playermove();

shortx=epointer->pos.x>>16;
shorty=epointer->pos.y>>16;
shorth=epointer->pos.h>>16;
epointer->off.x=offx=shortx - actcamx + ((epointer->pos.tx - cameratylx)<<10);
epointer->off.y=offy=shorty - actcamy + ((epointer->pos.ty - cameratyly)<<10);

int coef2=epointer->force.mass+1;
dddcoords * accel=GetWindGlobal (offx, offy);
epointer->force.speedx+=accel->x/coef2;
epointer->force.speedy+=accel->y/coef2;
epointer->force.speedh+=accel->h/coef2;

int coef=10;
createDirectionalWind(offx, offy, epointer->force.speedx/coef , epointer->force.speedy/coef,0, 5);

if (!tera.ZOOMED)
	epointer->drow();
	


shorth=epointer->pos.h>>16;

	for (int w=-hs; w<epointer->size-hs; w++)
			for (int h=-hs; h<epointer->size-hs; h++)
		for (int tall=0; tall<epointer->height; tall++)
			{
	TryFillrmatrix (epointer->pos.tx, epointer->pos.ty, shortx+w*pixInPhis, shorty+h*pixInPhis, shorth+tall*pixInPhis, epointer->i+einmatr);
	
			}
		if ((!epointer->friendly) && (!(epointer->clock <2 ))) signinmatrix ( epointer->pos.tx, epointer->pos.ty, epointer->pos.x,epointer->pos.y, epointer->i, 1);		

	
		} 
		else
		{
			

			shortx=epointer->pos.x>>16;
shorty=epointer->pos.y>>16;
shorth=epointer->pos.h>>16;
epointer->off.x=offx=shortx - actcamx + ((epointer->pos.tx - cameratylx)<<10);
epointer->off.y=offy=shorty - actcamy + ((epointer->pos.ty - cameratyly)<<10);
epointer->drow();
			epointer->death();
			removesomebody (epointer, i,1);
		}



	}

	if ((shorth/pixInPhis!=i-matrdepth) && (epointer->type>0))
	{
		removesomebody (epointer, i,0);
	}
	else 
	epointer=epointer->next;

	}
}

//*********************************************** BackGroundStuff
void softblitting()
{
	int i;
for (i=0; i<SoftBlitCount; i++)
	if (SoftBlit[i].type)
	{
offx= - actcamx + SoftBlit[i].x - (cameratylx - SoftBlit[i].tx)*tyleSize;
offy= - actcamy + SoftBlit[i].y - (cameratyly - SoftBlit[i].ty)*tyleSize;

BlitWithAlphaInprint(screen, SoftBlit[i].source, SoftBlit[i].alpha, offx, offy,SoftBlit[i].border);

SoftBlit[i].border+=SoftBlit[i].speed;

if ((SoftBlit[i].border>SoftBlit[i].borderw) && (!grid.redrowing)) //|| (SoftBlit[i].border<-SoftBlit[i].borderw))
{
grid.redrowing=1;
if (SoftBlit[i].AddObjectIfDone)  addobjectdata (SoftBlit[i].type, SoftBlit[i].tx, SoftBlit[i].ty, SoftBlit[i].x, SoftBlit[i].y, 0, -1, 0, SoftBlit[i].h, 0, 0);


//addTyleAndRedrow(SoftBlit[i].type, SoftBlit[i].tx, SoftBlit[i].ty, SoftBlit[i].x, SoftBlit[i].y);

grid.redrowing=0;
if (SoftBlit[i].FreeIfDone)   SDL_FreeSurface(SoftBlit[i].source);

SoftBlit[i].type=0;
}
	}
}
void waterdrow()
{
	//int i,j;
//offx=0;
//offy=0;
	for (int i=0; i<5; i++)
		for (int j=0; j<5; j++)
			if (grid.water[i+1][j+1])
		{
offx= - actcamx - (cameratylx - grid.objectx[i][j])*tyleSize;
offy= - actcamy - (cameratyly - grid.objecty[i][j])*tyleSize;
	//for (j=0; j<5; j++)
	//for (i=0; i<5; i++)
	//{	
	//offx= - actcamx - (cameratylx - objectx[j][i])*tyleSize;
	//offy= - actcamy - (cameratyly - objecty[j][i])*tyleSize;
	BWAITextureDisplacement(screen, image[62], image[82],&shadUp[i+1][j+1] ,offx,offy, waterEl-5, waterDis);
	
		}//}

}
void ecvatordrow()
{
	// 62-iceberg
	int i,j;
	//Water  parameters
	waterDis+=0.3;
	waterEl+=(waterRise*0.02)-0.01;
	if (waterDis>998) waterDis=0;
	if (waterEl<0) {waterEl=0; waterRise=true;}
	if (waterEl>5) {waterEl=5; waterRise=false;}
	//waterparameters end
//if (openglon==false)
//{
if (!stereo)
for (j=0; j<5; j++)
	for (i=0; i<5; i++)
	{	
	offx= - actcamx - (cameratylx - grid.objectx[j][i])*tyleSize;
	offy= - actcamy - (cameratyly - grid.objecty[j][i])*tyleSize;
	
 	if ((offx<scrwi) && (offy<scrhi) && (offx+tyleSize>0) && (offy+tyleSize>0))	
		if ((grid.iscaved[j+1][i+1]) && (!player.isunderground))
			//drowbg (offx, offy,  j,  i);
			myIndexedMemCopy (offx, offy, mainmaps[j+1][i+1], &grid.LavaIndex[j+1][i+1]);
				//	i=i;
			//myblit (offx, offy, mainmaps[j+1][i+1], &grid.LavaIndex[j+1][i+1]);
			else

			//{
		drowbgmmcpy(offx, offy,  j,  i);

	//Water flow experiment
	//if (i==j==1)
	//BWAITextureDisplacement(screen, image[62], image[81], image[82],shadowmaps[j+1][i+1] ,offx,offy, waterEl, waterDis);
		
	// height image can move too! This will give us wave effect ... you are fucking genius!
	// AND you can make water rize and fall!!! You are so much fucken genius
	//end
		//}
			
	}
else
{
for (j=0; j<5; j++)
	for (i=0; i<5; i++)
	{

		if  (grid.iscaved[j+1][i+1])
	{	
	offx= - actcamx - (cameratylx - grid.objectx[j][i])*tyleSize;
	offy= - actcamy - (cameratyly - grid.objecty[j][i])*tyleSize;
 	if ((offx<scrwi) && (offy<scrhi) && (offx+tyleSize>0) && (offy+tyleSize>0))						
	//drowbg (offx, offy,  j,  i);
			 drowbg3dbase(offx, offy,  j,  i, grid.iscaved[j+1][i+1]);
	}
		else
	{	
	offx= - actcamx - (cameratylx - grid.objectx[j][i])*tyleSize;
	offy= - actcamy - (cameratyly - grid.objecty[j][i])*tyleSize;
 	if ((offx<scrwi) && (offy<scrhi) && (offx+tyleSize>0) && (offy+tyleSize>0))								
			 drowbg3d(offx, offy,  j,  i);
	}

	}
}
//}
/*else
{
for (j=0; j<5; j++)
	for (i=0; i<5; i++)
	{	
	offx= - actcamx - (cameratylx - objectx[j][i])*tyleSize;
	offy= - actcamy - (cameratyly - objecty[j][i])*tyleSize;
 	if ((offx<scrwi) && (offy<scrhi) && (offx+tyleSize>0) && (offy+tyleSize>0))	
		GLDrow(offx,offy,0,selectedsurface);
			//if (grid.iscaved[j+1][i+1])
			//drowbg (offx, offy,  j,  i);
			//else
			//drowbgmmcpy(offx, offy,  j,  i); selectedsurface
			
	}
}*/


CopyBrightStoredOnscreen ();
}
void drowcaves()
{
for (int j=0; j<5; j++)
	for (int i=0; i<5; i++)
		if ((grid.iscaved[j+1][i+1]) && (!player.isunderground))
		{
offx= - actcamx - (cameratylx - grid.objectx[j][i])*tyleSize;
	offy= - actcamy - (cameratyly - grid.objecty[j][i])*tyleSize;
	
 	if ((offx<scrwi) && (offy<scrhi) && (offx+tyleSize>0) && (offy+tyleSize>0))	
			myblit (offx, offy, mainmaps[j+1][i+1], &grid.LavaIndex[j+1][i+1]);
	}
/*
	for (int i=0; i<cavecache; i++)
	if (cave[i].type!=0)
{
	offx= - actcamx + cave[i].x - (cameratylx - cave[i].tx)*tyleSize;
	offy= - actcamy + cave[i].y - (cameratyly - cave[i].ty)*tyleSize;
	if (player.host->pos.h<0) lightStyle (underg[cave[i].type][1], screen, offx, offy, 0.99+(float)(cave[i].elevation*0.01));
	else
		sdldrowstyle(offx-40,offy,cave[i].elevation*accuracy,underg[cave[i].type][0]);
}*/
}

// ****************************************** WALLS DROW
int CreateParticles ()
{
VoxelBloodBasicSet(&curphisobj->pos,0,0);
createData.x=jhx;
createData.y=jhy;
createData.hp=1599;
//createData.hspeed=-curphisobj->force.speedh;//*curphisobj->force.pr;
createData.gravity=1.5;//0+rand() % 20;
//createData.gravity/=50;
createData.h=curphisobj->pos.h;//max(accuracy*30,curphisobj->pos.h);
//ph[r].G_c=5; ph[r].G_p=8+(rand() % 4);
createData.staticPos=0;
createData.G_c=9;
createData.slowing=0.95;
//createData.g=(Uint32*)mainmaps[tylextogrid(curphisobj->pos.tx)+1][tyleytogrid(curphisobj->pos.ty)+1]->pixels+accurate(curphisobj->pos.y)*tyleSize+accurate(curphisobj->pos.x);
int total=rand() % 6-3;
for (int i=total; i>0; i--)
{
createData.type=6+(rand() % 6);
createData.xspeed=radiusx(50)*(accuracy/20) - jspx*3; //accuracy * (rand() % 100 - 50)/20 - jspx*3;
createData.yspeed=radiusy*(accuracy/20) - jspy*3;//accuracy* (rand() % 100 - 50)/20-jspy*3;
createData.hspeed=(rand() % accuracy)+accuracy*2;
createData.decaySurfaceNo=5+(rand() % 2);
createData.g=(Uint32*)mergeimaged[createData.decaySurfaceNo]->pixels + (rand() % 65000);
addEffect();
}
//mergeimaged[1]

return total*(total>0);
}


void movewall (wally* bo, int i)
{
curwall=curwall->next;

if (bo->prev==NULL)  
	{
		wfirst[i]=bo->next; 
		if (bo->next!=0)
		bo->next->prev=NULL; 
	}
	else
	{
		bo->prev->next=bo->next;
		if (bo->next!=NULL)
		bo->next->prev=bo->prev;
	}

	hig=bo->height/pixInPhis+matrdepth;
	if (hig<0) { hig=0; bo->hp=-10; }
	if (hig>matrhig+matrdepth-1) hig=matrhig+matrdepth-1;
		if (wfirst[hig]!=NULL) wfirst[hig]->prev=bo;
		bo->prev=NULL;
		bo->next=wfirst[hig];
		
		wfirst[hig]=bo; 

}
void removewall (wally* bo)
{
if (curwall)
	curwall=curwall->next;
if (bo->Megavoxel)
bo->Megavoxel->InUse=0;
/*
for (int z=0;z<=bo->height;z++)
for (int lj=0; lj<((wall[bo->type-900]->w+7)/pixInPhis);lj++)
for (int ljj=0; ljj<((wall[bo->type-900]->h+7)/pixInPhis);ljj++)
	TryClearmatrix (bo->pos.tx, bo->pos.ty, bo->pos.x+lj*pixInPhis, bo->pos.y+ljj*pixInPhis, (z*pixInPhis+bo->pos.h*pixInPhis), bo->i);
*/
int i=bo->pos.h+matrdepth;
//if (bo->i==activew) activew--; 
	if (bo->prev==NULL)  
	{
		wfirst[i]=bo->next; 
		if (bo->next)
		bo->next->prev=NULL; 
	}
	else
	{
		bo->prev->next=bo->next;
		if (bo->next!=NULL)
		bo->next->prev=bo->prev;
	}
		
	
	/*	if ((bo->brickcopy) && (bo->brick)) 
			SDL_FreeSurface (bo->brick);
		if (bo->gutsout) SDL_FreeSurface (bo->gutsout);*/
		bo->type=0;
		bo->light.active=0;

		


}

// death
void wnothing()
{
}
void DropWood()
{
Item wood;
wood.count=4;
wood.type=1;
//worldcoordinates pos;
curwall->pos.x=(curwall->pos.x+curwall->brick->w/2)*accuracy;
curwall->pos.y=(curwall->pos.x+curwall->brick->h/2)*accuracy;
curwall->pos.h*=accuracy;
dropItem(&wood, &curwall->pos);
curwall->pos.y=(curwall->pos.x-curwall->brick->w/2*accuracy)/accuracy;
curwall->pos.y=(curwall->pos.y-curwall->brick->h/2*accuracy)/accuracy;
curwall->pos.h/=accuracy;
}
void DropGuts()
{

	//createVortex(offx, offy, 300000, 50);
	//createTornado(offx, offy, 60000, 8, 1);
		//
//addphisicalrandomold(curphisobj->pos.tx, curphisobj->pos.ty, curphisobj->pos.x+10, curphisobj->pos.y,curphisobj->pos.h, 831, curphisobj->force.speedx/7, curphisobj->force.speedy/7,0,1, 300, 40,0 , 0,-curphisobj->hp, 1,100);
	
//	createData.type=7;//rand() % 6;
//createData.xspeed=0;
//createData.yspeed=0;
//createData.g=(Uint32*)QGpalette[5]->pixels;

//VoxelBloodBasicSet(&curphisobj->pos,0,0);
//addEffect();

MakegutsNoise (offx, offy);
}
//colision
void ignoreDamage()
{

}
void particleDrop()
{

if (CreateParticles ()) return;			
//smokedrop();
//createData.x=jhx; createData.y=jhy;
//if (rand() % 10<7) return;
		

}
void healthcheck()
{
bool finalBlow=0;
if (curwall->hp>=0)
	finalBlow=1;

	//curwall->hp-=curphisobj->power;

createData.tx=curphisobj->pos.tx; createData.ty=curphisobj->pos.ty; createData.x=jhx; createData.y=jhy; createData.h=jhh;
createData.type=20; createData.xspeed=-jspx/bodythick; createData.yspeed=-jspy/bodythick;  createData.hspeed=0;
createData.randosity=randy*11; createData.numberof=bloodness*2; createData.flooidness=6; createData.slowing=0.97;
createData.lightPisObj=0;
addphisicalrandom();

createData.randosity=randy*5;
createData.numberof=bloodness*2;
createData.type=813;

addphisicalrandom();

/*if (curwall->hp<0)
{*/
int pick=rand() % 5;
switch (pick)
{
case 0: pick=828; break;
case 1: pick=23; break;
case 2: pick=27; break;
case 3: pick=30; break;
case 4: pick=35; break;
}
//pick=831;
	addphisicalrandomold(curphisobj->pos.tx, curphisobj->pos.ty, curphisobj->pos.x+10, curphisobj->pos.y,curphisobj->pos.h, pick, curphisobj->force.speedx/10, curphisobj->force.speedy/10,0,1, 300, 40,0 , 0,-curphisobj->hp, 1,100);

	
			

			

}
void basicImpact()
{
//curwall->hp-=curphisobj->power;
createData.tx=curphisobj->pos.tx; createData.ty=curphisobj->pos.ty; createData.x=jhx; createData.y=jhy; createData.h=jhh;
createData.type=804; createData.xspeed=-jspx/bodythick; createData.yspeed=-jspy/bodythick;  createData.hspeed=0;
createData.randosity=randy*11; createData.numberof=bloodness*2; createData.flooidness=1; createData.slowing=0.97;
createData.lightPisObj=0;
addphisicalrandom();		

}
void ReleaseLavaOnDeath()
{
	curwall->hp-=curphisobj->power;
	if (curwall->hp<0)
	{
	addsoftblit(screen, mergeimage[111], mergeimagem[111],curwall->pos.tx, curwall->pos.ty, curwall->pos.x, curwall->pos.y, 0, 111,20, 255, -0.1,0,1);
	addobjectdata (308, curwall->pos.tx, curwall->pos.ty, curwall->pos.x+154-31, curwall->pos.y+112-27, 0, -1, 100, 0, 0, 0);
	removewall (curwall);
	}
}
// drow

void BlitterWallDrow()
{
	
for (int lj=0; lj<((wall[curwall->type-900]->w+7)/pixInPhis);lj++)
for (int ljj=0; ljj<((wall[curwall->type-900]->h+7)/pixInPhis);ljj++)
	for (int ljjj=0; ljjj<wallh[curwall->type-900];ljjj++)
	TryFillrmatrix (curwall->pos.tx, curwall->pos.ty, curwall->pos.x+lj*pixInPhis, curwall->pos.y+ljj*pixInPhis, ((curwall->pos.h+ljjj)*pixInPhis), curwall->i);


offx=curwall->pos.x - actcamx - (cameratylx - curwall->pos.tx)*tyleSize;
offy=curwall->pos.y - actcamy - (cameratyly - curwall->pos.ty)*tyleSize;
if (curwall->hp>0)
drowalphaborderStyle (curwall->brick, screen , mergeimage[curwall->type],255-curwall->hp, offx, offy,0,32);
curwall->hp-=7*GameSpeed;

/*
createData.tx=curwall->pos.tx; createData.ty=curwall->pos.ty; 
createData.x=curwall->pos.x*accuracy; createData.y=curwall->pos.y*accuracy; createData.h=curwall->pos.h*accuracy;
createData.type=20; createData.xspeed=(rand() % 100 - 50) * 20000; createData.yspeed=(rand() % 100 - 50) * 20000; 
createData.hspeed=(rand() % 50) * 1000;
createData.randosity=randy*9; createData.numberof=10; createData.flooidness=2; createData.slowing=0.98;

addphisicalrandom();*/

}
void basicWallDrow()
{
if (curwall->brick)
{
offx=curwall->pos.x - actcamx - (cameratylx - curwall->pos.tx)*tyleSize;
offy=curwall->pos.y - actcamy - (cameratyly - curwall->pos.ty)*tyleSize;
//+curwall->height
if ((heightimage[curwall->type]) && (curwall->pos.h<1)) 
BlitWithAlphaInprintNoCantactSoftness(screen, curwall->brick, heightimage[curwall->type], offx, offy, 0);
else
	sdldrowstyle (offx, offy, (curwall->pos.h*pixInPhis)*accuracy-heimod, curwall->brick);
}
}
void basicWallDrowRecalVoxel()
{
/*	
int shx=curwall->_shake.speedx;
int shy=curwall->_shake.speedy;
int dist;
dist=ipifagor (shx>>10, shy>>10);
if (dist>wind.maxBend) 
	ShortenRadBend(&curwall->_shake.speedx,&curwall->_shake.speedy , wind.maxBend, dist);
*/
/*
int dist2=ipifagor (shx, shy);
if (dist2>wind.maxBend) 
	ShortenRadBend(&shx,&shy, wind.maxBend, dist2);

int dist3=ipifagor (shx, shy);
*/
	offx=curwall->pos.x - actcamx - (cameratylx - curwall->pos.tx)*tyleSize+accurate(curwall->_shake.offx)/2;
offy=curwall->pos.y - actcamy - (cameratyly - curwall->pos.ty)*tyleSize+accurate(curwall->_shake.offy)/2;

CastShadowForDarkenfFunk(shadowimage[curwall->type],screen,offx+35,offy,curwall->pos.h,0 );

TreeShake();


	curwall->rebendVoxelTimer--;
	if ((curwall->rebendVoxelTimer<0) && (offx>-160) && (offy>-160) && (offx<scrwi+160) && (offy<scrhi+160))
	{
int chance=rand() % 100+accurate(curwall->_shake.offx+curwall->_shake.offy);
if ((chance>90) && ((accurate(curwall->_shake.offx))||(accurate(curwall->_shake.offy))))
{
createData.g=(Uint32*)leavesQV->pixels;
createData.tx=curwall->pos.tx; createData.ty=curwall->pos.ty;
createData.x=(curwall->pos.x+curwall->brick->w/2 + (rand() % 60 - 30))*accuracy+curwall->_shake.offx/2; createData.y=(curwall->pos.y+curwall->brick->h/2+ (rand() % 60 - 30))*accuracy+curwall->_shake.offy/2; 
createData.h=curwall->pos.h*accuracy;
createData.type=18;
createData.xspeed=0;
createData.yspeed=0;
createData.hspeed=0;
SuperFastBasicSet();
createData.boolFunction=&offScreenDeath;
createData.gravity=0;
createData.hp=7000;
createData.staticPos=0;
createData.slowing=1;
addEffect();
}

double Ax=0;
curwall->rebendVoxelTimer=16;
Ax=offx-scrwid+accurate(curwall->_shake.offx)*3;
double Ay=offy-scrhid+accurate(curwall->_shake.offy)*3;
Ax*=-0.001;
Ax+=curwall->angles.Ax;
Ay*=0.001;
Ay+=curwall->angles.Ay;
voxelVallConvertor(imageVoxel[curwall->type], curwall->brick->w/2, curwall->brick->h/2, 0, curwall->brick, Ax,Ay,curwall->angles.Ah, curwall->size, curwall->gutsout);
	}
int elevation=((curwall->pos.h+curwall->height)*pixInPhis)*accuracy-heimod;
	elevation/=dddstength;
offx+=((offx-scrwid)*elevation)>>16;
offy+=((offy-scrhid)*elevation)>>16;
DrowWithShadowErasing (curwall->brick, offx, offy);
//sdldrowstyle (offx, offy, ((curwall->pos.h+curwall->height)*pixInPhis)*accuracy-heimod, curwall->brick);

}

void wallfuncassigner()
{
	curwall->OnDeath=&wnothing;

	if (imageVoxel[curwall->type])
	{ curwall->drowing=&basicWallDrowRecalVoxel; curwall->decay=&basicImpact;  curwall->OnDeath=&DropWood;}
	else
	{curwall->drowing=&basicWallDrow;  curwall->decay=&particleDrop;}


	
	switch (curwall->type)
	{
	case 903:
		{curwall->decay=&particleDrop; break;}
	case 904:
		{curwall->hp=10000; break;}
	case 908:
		{curwall->decay=&ReleaseLavaOnDeath;   break;}
	case 910:
		{curwall->impparticle=20; curwall->hp=219; curwall->drowing=&BlitterWallDrow; 
		curwall->decay=&healthcheck; curwall->OnDeath=DropGuts; break;}
	}
}

void wallsdrow(int i)
{
//for (j=2; j<=activew; j++)
curwall=wfirst[i];
	//part pretype;

while (curwall)
{
if (curwall->type) 
{
	if ((!player.isunderground) && (displacement[curwall->type-900]) && (curwall->drowAbove) && (!tera.ZOOMED))
	{
	// displacement here
		Displacement (displacement[curwall->type-900], curwall->pos.tx, curwall->pos.ty, curwall->pos.x, curwall->pos.y, curwall->pos.h );
	}
	d=i-matrdepth-curwall->pos.h;

	if (!tera.ZOOMED)
	curwall->drowing();
}
if (((curwall->hp<0) || (curwall->RemoveMe)) && (curwall->Debris<0))
{
	if (curwall->hp<0) curwall->OnDeath();
	removewall (curwall);
}
else
curwall=curwall->next;
}
}

// *******************************************Initialization stuff
void setstereo()
{
	int i;
	while (!picturesloaded) SDL_Delay(10);
	//bgprepare

//	onscreeny=onscreeny/2*2-1;
	
//onscreenx-=(conf3d.eyewight/2*portion);


	conf3d.eyewight=150;
	conf3d.tomonitor=400;
	conf3d.bgdepth=30; // multiply by accuracy
	conf3d.bgportion=conf3d.bgdepth;
	conf3d.bgportion/=(conf3d.bgdepth+conf3d.tomonitor);
	precalcecvator=matrhig*pixInPhis*3;
	for (i=0;i<matrhig*pixInPhis*3*2; i++)
	{
portions[i]=conf3d.bgdepth+precalcecvator-i;
portions[i]/=(portions[i]+conf3d.tomonitor);
	}

conf3d.bgprepare=conf3d.bgdepth;
conf3d.bgprepare/=(conf3d.bgdepth+conf3d.tomonitor); 
conf3d.bgprepare=(conf3d.eyewight/2*conf3d.bgprepare)+1;

//portion/=(portion+conf3d.tomonitor);
	stereoscopizeimage(mergeimage[31]);
	stereoscopizeimage(mergeimage[32]);
	//stereoscopizeimage(image[5]);
	stereoscopizeimage(image[6]);
	stereoscopizeimage(image[9]);
	stereoscopizeimage(image[11]);
	//WALLS
	for (i=1; i<totalwalls; i++)
		//for (int k=0; k<=wallh[i];k++)
		if (wall[i])
	stereoscopizeimage(wall[i]);

	//ENEMIES
	for (i=1; i<chars; i++)
		{stereoscopizeimage(image[600+1+(i-1)*2]);
	stereoscopizeimage(image[600+2+(i-1)*2]);
	stereoscopizeimage(mergeimage[600+2+(i-1)*2]);}
	
	//PARTICLES
	for (i=1; i<drops; i++)
	{
	if (image[800+1+(i-1)*3]) stereoscopizeimage(image[800+1+(i-1)*3]);
	if (mergeimage[800+1+(i-1)*3]) 	stereoscopizeimage(mergeimage[800+1+(i-1)*3]);
	if (image[800+2+(i-1)*3]) stereoscopizeimage(image[800+2+(i-1)*3]);
	if (image[800+3+(i-1)*3]) stereoscopizeimage(image[800+3+(i-1)*3]);
	}

	for (i=17; i<44; i++)
	{
		if (image[i]) stereoscopizeimage(image[i]);
		if ((mergeimage[i]) && (i!=31) && (i!=32))   stereoscopizeimage(mergeimage[i]);
	}
	// stereoscopize caves
	for (i=1;i<caves; i++)	
		stereoscopizeimage(underg[i][0]); 
}
int DrowGalaxies ()
{

SDL_Rect src, dest;
src.x = 0;
src.y = 0;
src.w = image[44]->w-src.x;
src.h = image[44]->h-src.y;
dest.x = (scrwi-image[44]->w)/2;
dest.y = (scrhi-image[44]->h)/2;
dest.w = image[44]->w-src.x;
dest.h = image[44]->h-src.y;
//GLDrow ((scrwi-image[44]->w)/2, (scrhi-image[44]->h)/2, 0, 44);
SDL_BlitSurface(image[44], &src, screen, &dest);
/*
const int solars=1;
int solarx[solars];
int solary[solars];
int selectedsolar=0;
*/

for (int s=0; s<solars; s++)
{

src.w = image[45]->w;
src.h = image[45]->h;
dest.x = (scrwi-image[44]->w)/2+solarx[s];
dest.y = (scrhi-image[44]->h)/2+solary[s];
dest.w = image[45]->w;
dest.h = image[45]->h;
//GLDrow ((scrwi-image[44]->w)/2+solarx[s],(scrhi-image[44]->h)/2+solary[s], 0, 45);

//glClear( GL_COLOR_BUFFER_BIT );
SDL_BlitSurface(image[45], &src, screen, &dest);
}
/*	SDL_BlitSurface(image[76], NULL, screen, NULL);
	textlinenote( scrwi/2-200, scrhi/2-300, 45, "LOADING");


	for (int i=255; i>-1; i-=2)
	{
	drowalphaborder (image[78], screen, image[77],i, 0, 0,0,3);


	}*/

if (onlineTesting)
{

apply_surface((scrwi-image[72]->w)/2, (scrhi-image[72]->h)/2,image[72],screen);
apply_surface((scrwi-image[73]->w)/2, (scrhi-image[73]->h)/2+110,image[73],screen);
}
if (!Betatesting)
{
apply_surface(scrwi-image[83]->w, 0,image[83],screen);
apply_surface(scrwi-image[84]->w, 200,image[84],screen);
apply_surface(scrwi-image[84]->w, 400,imgBank.devITF.MegavoxDesk,screen);
}
//SDL_BlitSurface(mergeimage[31], NULL, screen, 0);

if (enableStereo)
if (!stereo) 
	SDL_BlitSurface(mergeimage[31+pnt], NULL, screen, 0); 
else	
{
	dest.x=10+3*pnt;
	dest.y=0;
	SDL_BlitSurface(mergeimage[31+pnt], NULL, screen, &dest);
	dest.y++;
	dest.x=10-3*pnt;
	SDL_BlitSurface(mergeimage[31+pnt], NULL, screen, &dest);

}

if (onlineTesting)
if (!inet.onlinemode)
textlinenote( (scrwi)/2-150, (scrhi)/2-200, 73, "Single player");
else 
	if (inet.onlinemode==1)
		textlinenote( (scrwi)/2-150, (scrhi)/2-200, 73, "Host");
	else
			textlinenote( (scrwi)/2-150, (scrhi)/2-200, 73, "Join");

showtext("Like & follow at: facebook.com/playdecay", 5,10);

return 0;
}
void actualPlanetSelector ()
{
	PlayMySound(&sounds.bank.coolNoise[6]);
bool selected=false;		
bool pointed=false;
int x; 
int y;
int i;
bool pnt=0, forceRepaint=0;
//MaterialsLoaded
SDL_Surface * StillLoadingText;
bool previousPointed=1;
StillLoadingText=TextOnlyToImage("Loading..."); 
bool stillLoadingDisplayed=1;
	// ################################################################### Select planet

if ((inet.onlinemode!=2) && (player.NextGameState!=3) && (!quit))
{	
	player.NextGameState=1;
	selected=false;
SDL_Rect src;
src.x = 0;
src.y = 0;
apply_surface((scrwi-image[46]->w)/2, (scrhi-image[46]->h)/2,image[46],screen);
//GLDrow ((scrwi-image[46]->w)/2,(scrhi-image[46]->h)/2, 0, 46);

// Drow sun
apply_surface((scrwi-image[solarsun[selectedsolar]]->w)/2, (scrhi-image[solarsun[selectedsolar]]->h)/2,image[solarsun[selectedsolar]],screen);
//GLDrow ((scrwi-image[solarsun[selectedsolar]]->w)/2,(scrhi-image[solarsun[selectedsolar]]->h)/2, 0, solarsun[selectedsolar]);
//Drow planets
for (int i=0; i<planets[selectedsolar]; i++)
{
apply_surface(scrwi/2+planetx[selectedsolar][i], scrhi/2+planety[selectedsolar][i],image[48+planettype[selectedsolar][i]],screen);
//GLDrow (scrwi/2+planetx[selectedsolar][i],scrhi/2+planety[selectedsolar][i], 0, 48+planettype[selectedsolar][i]);
}
//SDL_GL_SwapBuffers();


while ((!selected) && (!quit))
{
SDL_Delay(50);
	while ( SDL_PollEvent( &event ) ) 
	{

if( event.type ==  SDL_KEYDOWN ) 
	if ( event.key.keysym.sym == SDLK_ESCAPE )
				{
					quit=true;
					player.NextGameState=0;
			}

if( event.type == SDL_QUIT )
			{ 
				quit = true;
				player.NextGameState=0;
			} 
		// motion
if(event.type == SDL_MOUSEMOTION  )
{ 
x = event.button.x; 
y = event.button.y;
//pointed=false;




}
//((((((((((((((((((((((((((((((((((((((((((((( 
		 if( event.type == SDL_MOUSEBUTTONDOWN ) 
		 {
x = event.button.x; 
y = event.button.y;
for (i=0; i<planets[selectedsolar]; i++)
if ((x>planetx[selectedsolar][i]+scrwi/2) && (y>planety[selectedsolar][i]+scrhi/2) && (x<planetx[selectedsolar][i]+image[48+planettype[selectedsolar][i]]->w+scrwi/2) && (y<planety[selectedsolar][i]+image[48+planettype[selectedsolar][i]]->h+scrhi/2))
{
	selected=true;
	planetname=planame[selectedsolar][i];
	selectedsurface=planetsurface[selectedsolar][i];
	selectedplanet=i;
	forceRepaint=1;
}
		 }
	}

pointed=0;
for (i=0; i<planets[selectedsolar]; i++)
if ((x>planetx[selectedsolar][i]+scrwi/2) && (y>planety[selectedsolar][i]+scrhi/2) && (x<planetx[selectedsolar][i]+image[48+planettype[selectedsolar][i]]->w+scrwi/2) && (y<planety[selectedsolar][i]+image[48+planettype[selectedsolar][i]]->h+scrhi/2))
{
	if (!previousPointed)
	{
	textlinenote( planetx[selectedsolar][i]+scrwi/2, planety[selectedsolar][i]+scrhi/2, 48+planettype[selectedsolar][i], planetname=planame[selectedsolar][i]);
swapscreens();
	}
	previousPointed=pointed=true;
}

if (stillLoadingDisplayed && MaterialsLoaded)
{
	stillLoadingDisplayed=0;
	forceRepaint=1;
}

if (((!pointed) && (previousPointed)) || forceRepaint)
{
apply_surface((scrwi-image[46]->w)/2, (scrhi-image[46]->h)/2,image[46],screen);
apply_surface((scrwi-image[solarsun[selectedsolar]]->w)/2, (scrhi-image[solarsun[selectedsolar]]->h)/2,image[solarsun[selectedsolar]],screen);
for (int i=0; i<planets[selectedsolar]; i++)
apply_surface(scrwi/2+planetx[selectedsolar][i], scrhi/2+planety[selectedsolar][i],image[48+planettype[selectedsolar][i]],screen);
if (!MaterialsLoaded)
apply_surface((scrwi-StillLoadingText->w)/2, StillLoadingText->h, StillLoadingText , screen);
swapscreens();
previousPointed=pointed=false;
forceRepaint=0;
}

}
}


}

void waiter()
{
	while ((!createData.returnHolder) && (LoadGameAssets))
	{
while ( SDL_PollEvent( &event ) )
	SDL_Delay(1);
//SDL_Delay(1);
	}
	
}

int planetselector ()
{
	bool drowone=1;
selected=false;
inet.onlinemode=0;

bool previousPointed=0;

DrowGalaxies();
swapscreens();

while ((!selected) && (!quit))
{
//SDL_Delay(50);
while ( SDL_PollEvent( &event ) ) 
	{


if( event.type ==  SDL_KEYDOWN ) 
	if ( event.key.keysym.sym == SDLK_ESCAPE )
				{
					quit=true;
					player.NextGameState=0;
			}


	if( event.type == SDL_QUIT )
			{ 
				quit = true;
				player.NextGameState=0;
			} 

if(event.type == SDL_MOUSEMOTION  )
{ 

//drowone=0;
//x = event.button.x; 
//y = event.button.y;
//pointed=false;
	GetMouse(&mouse.pos.x, &mouse.pos.y);






}
		// if galaxy selected
		 if( event.type == SDL_MOUSEBUTTONDOWN ) 
		 {
GetMouse(&mouse.pos.x, &mouse.pos.y);

if (enableStereo)
if ( mouseInSquere(0, 0, mergeimage[31]->w, mergeimage[31]->h) && (stereo==0))	{stereo=1; }  //setstereo();}

if (mouseInSquere((scrwi-image[72]->w)/2, (scrhi-image[72]->h)/2, image[72]->w, image[72]->h)) 
{
	if ((onlineTesting) && clientfunk(0))
	{
	inet.onlinemode=2; // 0 - single player, 1 - server, 2 - join
		//clientfunk(); 
		player.NextGameState=1;
		selectedsurface=planetsurface[selectedsolar][selectedplanet];
		planetname=planame[selectedsolar][selectedplanet];
		selected=true;
	}
}

if (mouseInSquere((scrwi-image[73]->w)/2, (scrhi-image[73]->h)/2+110, image[73]->w, image[73]->h) && (onlineTesting)) 
	{
		
		inet.onlinemode=1; // 0 - single player, 1 - server, 2 - multiplayer
}

if (mouseInSquere(scrwi-image[83]->w, 0, image[83]->w, image[83]->h) && (!Betatesting)) 
	{
player.NextGameState=3;
selected=true;
}

if (mouseInSquere(scrwi-image[84]->w, 200, image[84]->w, image[84]->h) && (!Betatesting)) 
	{
player.NextGameState=5;
selected=true;
}

if (mouseInSquere(scrwi-image[84]->w, 400, image[84]->w, image[84]->h) && (!Betatesting)) 
	{
player.NextGameState=6;
selected=true;
}

for (int i=0; i<solars; i++)
	if ( mouseInSquere(solarx[i]+(scrwi-image[44]->w)/2, solary[i]+(scrhi-image[44]->h)/2, image[45]->w, image[45]->h))
{
	selected=true;
	selectedsolar=i;
	actualPlanetSelector ();
goto pss;
}


		 }
	

	}

pointed=0;

if (mouseInSquere((scrwi-image[72]->w)/2, (scrhi-image[72]->h)/2, image[72]->w, image[72]->h)	 && (onlineTesting))
{ 
	if (!previousPointed)
	{
	textlinenote( (scrwi-image[72]->w)/2, (scrhi-image[72]->h)/2+50, 72, "Try at your own risk");
swapscreens();
	}
	previousPointed=pointed=true;
}

if (mouseInSquere((scrwi-image[73]->w)/2, (scrhi-image[73]->h)/2+110, image[73]->w, image[73]->h)	&& (onlineTesting))
{ 
	if (!previousPointed)
	{
	textlinenote( (scrwi-image[72]->w)/2+20, (scrhi-image[72]->h)/2+150, 73, "Server will freezze the game");
swapscreens();
	}
	previousPointed=pointed=true;
}

if (mouseInSquere(scrwi-image[83]->w, 0, image[83]->w, image[83]->h) && (!Betatesting)) 
	{
		if (!previousPointed)
	{
	textlinenote( scrwi-400, 200, 73, "Voxel Desk");
swapscreens();
	}
	previousPointed=pointed=true;
}

if (mouseInSquere(scrwi-image[84]->w, 200, image[84]->w, image[84]->h) && (!Betatesting)) 
	{
if (!previousPointed)
	{
	textlinenote( scrwi-400, 200, 73, "Height Desk");
		swapscreens();
	}
	previousPointed=pointed=true;
}


for (int i=0; i<solars; i++)
if (mouseInSquere(solarx[i]+(scrwi-image[44]->w)/2, solary[i]+(scrhi-image[44]->h)/2, image[44]->w, image[44]->h)	)
{
	if (!previousPointed)
	{
selected=0;
textlinenote( solarx[i]+(scrwi-image[44]->w)/2, solary[i]+(scrhi-image[44]->h)/2, 45, solarname[i]);
swapscreens();
}
	previousPointed=pointed=true;
}

if (mouseInSquere(0, 0, mergeimage[31]->w, mergeimage[31]->h))
{	
	if (!previousPointed)
	{
	pnt=1;
	DrowGalaxies();
swapscreens();
//previousPointed=false;
	}
	previousPointed=pointed=true;
}


if ((!pointed) && (previousPointed))
{
	pnt=0;
DrowGalaxies();
previousPointed=false;

swapscreens();
}
SDL_Delay(50);
}
pss:
waiter();
return 0;
}
void initdrow()
{
	if (!stereo){
	drowstyle=&anydrow; tolendstyle=&drowtolendpixcheck;
	mdrowstyle=&manydrow;
	sdldrowstyle=&sdlanydrow;
	lightStyle=&lighten;
	setpixelstyle=&setpixel;
	DisplacementStyle=&drowdisplacement;
	drowalphaborderStyle=&drowalphaborder;
	}
	else {
	drowstyle=&anydrow3d; tolendstyle=&drowtolendpixcheck3d;
	mdrowstyle=&manydrow3d;
	sdldrowstyle=&sdlanydrow3d;
	lightStyle=&lighten3D;
	setpixelstyle=&setpixel3d;
	DisplacementStyle=&drowdisplacement3d;
	drowalphaborderStyle=&drowalphaborder3d;
	}
}


//Drow assists
void DrowMethodForVexel( int gx, int gy)
{
	mrg.pos.x*=accuracy;
	mrg.pos.y*=accuracy;
	QuickDrow8bitVex(palette.Buffer8bit,&mrg.pos, &gayVoxel[10][0], 0, 0, 0, 1,  mrg.globx ,  mrg.globy , mrg.glob, PhisObjectsDefault);
	mrg.pos.x/=accuracy;
	mrg.pos.y/=accuracy;
BufferToGround(gayVoxel[10][0].palette,mrg.pos.x , mrg.pos.y, storedmp[gx+1][gy+1]);
}

void DrowMethodSelector(int gx, int gy)
{
mrg.tempage=285-(now-mrg.tempage)/400;
// ********************** If Decayed image is present:
if (mrg.type>=voxInEditor) 
{
	DrowMethodForVexel( gx, gy);
	return;
}
if ((mergeimaged[mrg.type]) && (!imageVoxel[mrg.type]))
{
	SDL_Surface* decsf=decayBG[mrg.type];
	if (createData.snowTyle) 
		decsf=SnowydecayBG[mrg.type];
drowDecayedBG (decsf, storedmp[gx+1][gy+1], mergeimaged[mrg.type], mrg.pos.x, mrg.pos.y); // 85
}
if ((imageVoxel[mrg.type]) && (mrg.type<900))
MergeVexelUpper(imageVoxel[mrg.type], mrg.pos.x+mergeimage[mrg.type]->w/2, mrg.pos.y+mergeimage[mrg.type]->h/2,mrg.pos.h, storedmp[gx+1][gy+1],mrg.globx, mrg.globy, mrg.glob, &shadUp[gx+1][gy+1], mergeimage[mrg.type]);
else
{
//if (mergeimaged[mrg.type])

	/*if (mrg.tempage>0)
	drowalphamix (mergeimage[mrg.type],mergeimaged[mrg.type], storedmp[gx+1][gy+1], mergeimagem[mrg.type],mrg.tempage , mrg.pos.x, mrg.pos.y, 0, 100); // 255-(now+tempage)/86400
	else
		apply_surface( mrg.pos.x  , mrg.pos.y, mergeimaged[mrg.type], storedmp[gx+1][gy+1]);*/

// ********************* If height image is present
//BlitWithDHInprint(SDL_Surface *layer, SDL_Surface *source, SDL_Surface *uplayer, SDL_Surface *upsource, SDL_Surface *heightMask, int dx, int dy, int h)
if ( heightimage [mrg.type] )
{
if (heightimage [mrg.type])
		if  ((mrg.type>100) && (overlayimage[mrg.type]))
		{
if (mrg.temps==1)
//BlitNormalAndAlphaInprint(storedmp[gx+1][gy+1], mergeimage[mrg.type], &shadUp[gx+1][gy+1], overlayimage[mrg.type], heightimage[mrg.type], mrg.pos.x, mrg.pos.y,mrg.pos.h);
BlitNormalAndAlphaInprint(storedmp[gx+1][gy+1], mergeimage[mrg.type], uppermp[gx+1][gy+1], overlayimage[mrg.type], heightimage[mrg.type], mrg.pos.x, mrg.pos.y,mrg.pos.h);
else
{
if (mrg.tempfall)
	//FlatSurfaceFolliage
BlitNormalAndAlphaInprintScaleFolliage(storedmp[gx+1][gy+1], mergeimage[mrg.type], uppermp[gx+1][gy+1], overlayimage[mrg.type], heightimage[mrg.type],mrg.pos.x, mrg.pos.y,mrg.pos.h,mrg.temps);
//BlitNormalAndAlphaInprintScaleFolliage(storedmp[gx+1][gy+1], mergeimage[mrg.type], &shadUp[gx+1][gy+1], overlayimage[mrg.type], heightimage[mrg.type], mrg.pos.x, mrg.pos.y,mrg.pos.h,mrg.temps);
else
	if (-mrg.pos.h<256*mrg.temps)
BlitNormalAndAlphaInprintScale(storedmp[gx+1][gy+1], mergeimage[mrg.type], uppermp[gx+1][gy+1], overlayimage[mrg.type], heightimage[mrg.type], mrg.pos.x, mrg.pos.y,mrg.pos.h,mrg.temps);
//BlitNormalAndAlphaInprintScale(storedmp[gx+1][gy+1], mergeimage[mrg.type], &shadUp[gx+1][gy+1], overlayimage[mrg.type], heightimage[mrg.type], mrg.pos.x, mrg.pos.y,mrg.pos.h,mrg.temps);
}
		}
		else
if (mrg.temps==1)
BlitWithAlphaInprint(storedmp[gx+1][gy+1], mergeimage[mrg.type], heightimage[mrg.type], mrg.pos.x, mrg.pos.y,mrg.pos.h);  //height mask
else
BlitWithAlphaInprintScale(storedmp[gx+1][gy+1], mergeimage[mrg.type], heightimage[mrg.type], mrg.pos.x, mrg.pos.y,mrg.pos.h, mrg.temps);

}
	else	
apply_surface( mrg.pos.x  , mrg.pos.y, mergeimage[mrg.type], storedmp[gx+1][gy+1]); 
// *********************************** If overlay image is present
if ((overlayimage[mrg.type]) && (mrg.type>100))
	if (!heightimage [mrg.type]) //&&  (!dhimage [mrg.type]))
	//blittoalpha(overlayimage[mrg.type], &shadUp[gx+1][gy+1], mrg.pos.x, mrg.pos.y);
		blittoalpha(overlayimage[mrg.type], uppermp[gx+1][gy+1], mrg.pos.x, mrg.pos.y);
}
}
void UpperDrowMethodSelector(int gx, int gy)
{
	//image[85]  dhDecay[mrg.type]
	if (dhimage [mrg.type]) 
	{
		if (dhDecay[mrg.type])
BlitWithDHDecay(storedmp[gx+1][gy+1], image[mrg.type], uppermp[gx+1][gy+1],dhDecay[mrg.type], dhimage[mrg.type], mrg.pos.x, mrg.pos.y,mrg.pos.h);
		else
BlitWithDHInprint(storedmp[gx+1][gy+1], image[mrg.type], uppermp[gx+1][gy+1], overlayimage[mrg.type], dhimage[mrg.type], mrg.pos.x, mrg.pos.y,mrg.pos.h);
	}//BlitWithDHInprint(storedmp[gx+1][gy+1], image[mrg.type], &shadUp[gx+1][gy+1], overlayimage[mrg.type], dhimage[mrg.type], mrg.pos.x, mrg.pos.y,mrg.pos.h);
else
	if (mrg.type<300 ) apply_surface( mrg.pos.x, mrg.pos.y, image[mrg.type], storedmp[gx+1][gy+1]);
else 
	if (image[mrg.type])
	//blittoalpha(image[mrg.type], &shadUp[gx+1][gy+1], mrg.pos.x, mrg.pos.y);
		blittoalpha(image[mrg.type], uppermp[gx+1][gy+1], mrg.pos.x, mrg.pos.y);

if ((mrg.type<300) && (image[mrg.type]!=image[101]) && 
	(image[mrg.type]!=image[99]) && (overlaylap[mrg.type-100]==1)) 
	//apply_surface( mrg.pos.x, mrg.pos.y, image[mrg.type], storedmp[gx+1][gy+1]);
	alphasizeupperlayer(image[mrg.type], uppermp[gx+1][gy+1], mrg.pos.x, mrg.pos.y);
if ((mrg.type>900) && (mrg.type<1100) && (uppermerge[mrg.type-900])) 
	apply_surface( mrg.pos.x, mrg.pos.y, uppermerge[mrg.type-900], storedmp[gx+1][gy+1]);

}

// ****************************************************************************  Temporary content generation
void temporaryHostSpawner()
{

		int offx=scrwi*((rand() % 5) - 2)*2;
		int offy=scrhi*((rand() % 5) - 2)*2;
	addobject(offx+20,offy+20,607);

}
void temporaryBetaSpawning()
{
	if (monstersToKill<0) return;
	betaSpawnTimer--;
	if (betaSpawnTimer<0)
	{
		monstersToKill--;
//if (gunstats[4].delay>0.36) gunstats[4].delay-=0.08;
//else gunstats[4].delay=0.36;

		int offx=scrwi*((rand() % 5) - 2)*2;
		int offy=scrhi*((rand() % 5) - 2)*2;
		betaSpawnTimer=betaSpawnDelay;
		//betaSpawnDelay=max(200,betaSpawnDelay*0.8);
	/*	int type=rand() % 100;
	if (type<3)
		type=613;
	else
		if (type<30)
		type=611;

	else
	if (type<95)
	{
		if (type>45)
			type=609;
			else 
			{

			type=601;
			for (int i=0; i<3; i++) addobject(offx-i*8,offy-i*8,type);
		}
			
	}
		else
			type=605;
//addobjectdata (type, nq->qotx, nq->qoty, nq->qox-i*8, nq->qoy-i*8, 0, 0, 0, 0, now, 0);*/
addobject(offx+20,offy+20,609+5*(rand() % 2));	
}
}

void randUp(int gx, int gy, int tx, int ty)
{
	for (int minusx=1; minusx>=0; minusx--)
	for (int minusy=1; minusy>=0; minusy--)
	{
mrg.pos.tx=tx-minusx;
mrg.pos.ty=ty-minusy; 
mrg.type=106; // crater
mrg.temps=1;
world.rx=forDHrx[minusx*2+minusy];
world.ry=forDHry[minusx*2+minusy];
for (int i=GetSpawnAmmount(world.brykivka); i>0; i--)
{
mrg.tempfall=0;
setRandTerrain(mergeimage[mrg.type]->w, mergeimage[mrg.type]->h, minusx, minusy);
mrg.pos.h=0;
UpperDrowMethodSelector( gx, gy);
}
	}
}
void randGeneration (int gx, int gy, int tx, int ty)
{
bool refreshing=createData.refreshingTyle;
	createData.randomGenerated=1;
monstersToKill++;
for (int minusx=1; minusx>=0; minusx--)
	for (int minusy=1; minusy>=0; minusy--)
	{
 mrg.pos.tx=tx-minusx;
 mrg.pos.ty=ty-minusy; 
world.rx=(mrg.pos.ty<<16+mrg.pos.tx<<8)+mrg.pos.ty*666;
world.ry=mrg.pos.tx<<16+(mrg.pos.tx*(mrg.pos.ty*2))<<8;
mrg.pos.h=0; mrg.temps=1; mrg.tempfall=0;

	mrg.type=104; // ground tyle bricks



	for (int i=GetSpawnAmmount(world.brykivka); i>0; i--) 
{
setRandTerrain(mergeimage[mrg.type]->w, mergeimage[mrg.type]->h, minusx, minusy);
//apply_surface( mrg.pos.x, mrg.pos.y , mergeimage[mrg.type], storedmp[gx+1][gy+1]); 
DrowMethodSelector(gx,gy);
}

grid.snow[gx+1][gy+1]=createData.snowTyle=myRandom() % 2;
	

mrg.type=114; // Big moss hils // THIS ONE IS GUILTY
for (int i=GetSpawnAmmount(world.brykivka); i>0; i--)
{
	setRandTerrain(mergeimage[mrg.type]->w, mergeimage[mrg.type]->h, minusx, minusy);
	DrowMethodSelector(gx,gy);
	//BlitNormalAndAlphaInprint(storedmp[gx+1][gy+1], mergeimage[mrg.type], uppermp[gx+1][gy+1], overlayimage[mrg.type], heightimage[mrg.type], mrg.pos.x, mrg.pos.y,0);
}

grid.water[gx+1][gy+1]=GetTyleFlag(tx, ty, grid.BGWaterMask);

mrg.type=110;//rock
for (int i=GetSpawnAmmount(world.brykivka); i>0; i--)
{setRandTerrain(mergeimage[mrg.type]->w, mergeimage[mrg.type]->h, minusx, minusy);
DrowMethodSelector(gx,gy);
//BlitNormalAndAlphaInprint(storedmp[gx+1][gy+1], mergeimage[mrg.type], uppermp[gx+1][gy+1], overlayimage[mrg.type], heightimage[mrg.type], mrg.pos.x, mrg.pos.y,-(myRandom() % 60));
}

if (createData.snowTyle) //snow
{
mrg.tempfall=0;
mrg.temps=1;
mrg.pos.h=0; 
mrg.type=108;
for (int i=GetSpawnAmmount(world.brykivka); i>0; i--)
{
setRandTerrain(image[mrg.type]->w, image[mrg.type]->h, minusx, minusy);
DrowMethodSelector(gx,gy);
}
}


mrg.type=109; // grass
mrg.temps=0.55;
for (int i=GetSpawnAmmount(world.brykivka); i>0; i--)
{

mrg.tempfall=0;
setRandTerrain(mergeimaged[mrg.type]->w, mergeimaged[mrg.type]->h, minusx, minusy);
mrg.temps=0.1;
mrg.pos.h=-255;
DrowMethodSelector(gx,gy);

mrg.tempfall=0;
setRandTerrain(mergeimaged[mrg.type]->w, mergeimaged[mrg.type]->h, minusx, minusy);
mrg.temps=1;
mrg.pos.h=-15;
DrowMethodSelector(gx,gy);

mrg.tempfall=1;
setRandTerrain(mergeimaged[mrg.type]->w, mergeimaged[mrg.type]->h, minusx, minusy);
mrg.temps=0.55;
mrg.pos.h=-150-(myRandom() % 10);
DrowMethodSelector(gx,gy);
}

mrg.type=115;// small bushes
for (int i=GetSpawnAmmount(world.brykivka); i>0; i--)
{
setRandTerrain(mergeimage[mrg.type]->w, mergeimage[mrg.type]->h, minusx, minusy);
 mrg.pos.h=0; mrg.temps=1; mrg.tempfall=0;
 mrg.glob=myRandom(); mrg.glob/=314;
  mrg.globx=myRandom(); mrg.globx/=314;
   mrg.globy=myRandom(); mrg.globy/=314;
DrowMethodSelector(gx,gy);
UpperDrowMethodSelector( gx, gy);
}



mrg.type=117; // Voxel Rocks
for (int i=GetSpawnAmmount(world.voxelRocks); i>0; i--)
{
		mrg.pos.h=-80-(myRandom() % 80);
		mrg.globx=myRandom(); mrg.globx/=314;//GetRandomRotation(2*M_PI/20);
		mrg.globy=myRandom(); mrg.globy/=314;
		mrg.glob=myRandom(); mrg.glob/=314;
setRandTerrain(mergeimage[mrg.type]->w, mergeimage[mrg.type]->h, minusx, minusy);
		DrowMethodSelector(gx,gy);
}

//forDHrx[4],forDHry[4];
mrg.type=106; // crater
mrg.temps=1;
forDHrx[minusx*2+minusy]=world.rx;
forDHry[minusx*2+minusy]=world.ry;
for (int i=GetSpawnAmmount(world.brykivka); i>0; i--)
{
mrg.tempfall=0;
setRandTerrain(mergeimage[mrg.type]->w, mergeimage[mrg.type]->h, minusx, minusy);
mrg.pos.h=0;
DrowMethodSelector(gx,gy);
}


}


// ####################################################### Local stuff 

if ((!player.corporial) && (Betatesting))	
	{
	/*	int chance=myRandom() % 100;
		if (chance>10)
		temporaryHostSpawner();*/
	} 

if (grid.water[gx+1][gy+1]) // water
{
mrg.type=118;
setRandTerrainLocal(mergeimage[mrg.type]->w, mergeimage[mrg.type]->h);
mrg.pos.h=0; mrg.temps=1; mrg.tempfall=0;
DrowMethodSelector(gx,gy);
UpperDrowMethodSelector( gx, gy);
}



mrg.type=904+7*grid.snow[gx+1][gy+1]; //Voxel Trees
createData.anglex=0;
createData.angley=0;
createData.randomGenerated=1;
	for (int i=GetSpawnAmmount(world.voxelTree); i>0; i--)
{
	createData.angleh=MyRandomRotation(2*M_PI/20);
	createData.size=0.5+(myRandom() % 13)*0.05;
	createData.anglex=0;
	createData.angley=0;
setRandTerrainLocal(mergeimage[mrg.type]->w, mergeimage[mrg.type]->h);
		DrowMethodSelector(gx,gy);
if (!refreshing) addobjectdata (mrg.type, mrg.pos.tx, mrg.pos.ty, mrg.pos.x, mrg.pos.y, mrg.pos.h, -1, 100,0,now,0);
}

mrg.type=903; //MCRocks
for (int i=GetSpawnAmmount(world.mcrock); i>0; i--)
{
setRandTerrainLocal(mergeimage[mrg.type]->w, mergeimage[mrg.type]->h);
mrg.pos.h=0; mrg.temps=1; mrg.tempfall=0;
		DrowMethodSelector(gx,gy);
		UpperDrowMethodSelector( gx, gy);
if (!refreshing) addobjectdata (mrg.type, mrg.pos.tx, mrg.pos.ty, mrg.pos.x, mrg.pos.y, mrg.pos.h, -1, 100,0,now,0);
}

mrg.type=902; //blocks
for (int i=GetSpawnAmmount(world.mcrock); i>0; i--)
{
setRandTerrainLocal(mergeimage[mrg.type]->w, mergeimage[mrg.type]->h);
mrg.pos.h=0; mrg.temps=1; mrg.tempfall=0;
		DrowMethodSelector(gx,gy);
		UpperDrowMethodSelector( gx, gy);
if (!refreshing) addobjectdata (mrg.type, mrg.pos.tx, mrg.pos.ty, mrg.pos.x, mrg.pos.y, mrg.pos.h, -1, 100,0,now,0);
}

mrg.type=905; //flying island
if (!(gx==gy==1))
for (int i=GetSpawnAmmount(world.mcrock); i>0; i--)
{
setRandTerrainLocal(mergeimage[mrg.type]->w, mergeimage[mrg.type]->h);
mrg.pos.h=0; mrg.temps=1; mrg.tempfall=0;
		DrowMethodSelector(gx,gy);
		UpperDrowMethodSelector( gx, gy);
if (!refreshing) addobjectdata (mrg.type, mrg.pos.tx, mrg.pos.ty, mrg.pos.x, mrg.pos.y, mrg.pos.h, -1, 100,0,now,0);
}
/*
mrg.type=906; //displacement hill
if  (!(gx==gy==1))
for (int i=GetSpawnAmmount(world.mcrock); i>0; i--)
{
setRandTerrainLocal(mergeimage[mrg.type]->w, mergeimage[mrg.type]->h);
mrg.pos.h=0; mrg.temps=1; mrg.tempfall=0;
		DrowMethodSelector(gx,gy);
		UpperDrowMethodSelector( gx, gy);
if (!refreshing) addobjectdata (mrg.type, mrg.pos.tx, mrg.pos.ty, mrg.pos.x, mrg.pos.y, mrg.pos.h, -1, 100,0,now,0);
}*/



createData.refreshingTyle=1;

}

//**********************************************************Important stuff

int tyleopen (int gx, int gy, int tx, int ty)
{
	int i=1;


now= time(0)-1355550000;

grid.water[gx+1][gy+1]=GetTyleFlag(tx, ty, grid.BGWaterMask);
grid.iscaved[gx+1][gy+1]=GetTyleFlag(tx, ty, grid.BGLavaMask);

grid.objectx[gx][gy]=tx;
grid.objecty[gx][gy]=ty;

int thisbg=GetTyleBackground(tx, ty);
memcpy ( storedmp[gx+1][gy+1]->pixels, tera.DecayBG[thisbg].pixels, 1048576*4 );
int leftbg=GetTyleBackground(tx-1, ty);
if (leftbg>thisbg)
DrowMyIndAlpha (&imgBank.BGborderLeft,0,0 ,&tera.DecayBG[leftbg] ,0, 0, storedmp[gx+1][gy+1], 1.3);

int rightbg=GetTyleBackground(tx+1, ty);
if (rightbg>thisbg)
DrowMyIndAlpha (&imgBank.BGborderRight,tyleSize-imgBank.BGborderRight.w,0 ,&tera.DecayBG[rightbg] ,tyleSize-imgBank.BGborderRight.w, 0, storedmp[gx+1][gy+1], 1.3);

int topbg=GetTyleBackground(tx, ty-1);
if (topbg>thisbg)
	DrowMyIndAlpha (&imgBank.BGborderTop,0,0 ,&tera.DecayBG[topbg] ,0, 0, storedmp[gx+1][gy+1], 1.3);

int bottombg=GetTyleBackground(tx, ty+1);
if (bottombg>thisbg)
	DrowMyIndAlpha (&imgBank.BGborderBottom,0,tyleSize-imgBank.BGborderBottom.h,&tera.DecayBG[bottombg] ,0,tyleSize-imgBank.BGborderBottom.h, storedmp[gx+1][gy+1], 1.3);

//randGeneration ( gx,  gy, tx, ty);

//Loading Terrain
sTyleStroke* tyle=&Terrain[tylextogrid(tx)][tyleytogrid(ty)];

std::stringstream sss;
sss<<"world/"<<planetname<<"/Terrain/"<<planetname<<"T_"<<tx<<"v"<<ty<<".map";
std::string strx2;
sss>>strx2; 
tyle->max=0;
tyle->updated=0;
ifstream file (strx2, ios::binary);	
if (file){
file.read ((char*)&tyle->max, 4);
file.read ((char*)tyle->stroke, sizeof(sectorStrokes)*tyle->max);
file.close();

for (int i=0; i<tyle->max; i++){
	readInGameStroke(&tyle->stroke[i], gx, gy, 0);
}
}
//Terrain over


	

	std::stringstream ss;
    ss<<"world/"<<planetname<<"/"<<planetname<<"_"<<tx<<"v"<<ty<<".map";
    std::string strx;
    ss>>strx;    
	
//	std::ifstream map(strx);
ifstream map(strx,ios::binary);
if ( map ) 
{
do {
mrg.type=0;
map.read((char*)&mrg.type, 4);
if (mrg.type==1) 
	LoadWall(&map);
} while (mrg.type);
/*map>>mrg.type;
while (mrg.type>0)
{	
map >> mrg.pos.tx; //x coord of the tile, obcect is located
map >> mrg.pos.ty; 
map >> mrg.pos.x;
map >> mrg.pos.y;
map >> mrg.tempage;
map >> mrg.pos.h;
map >> mrg.temps;

if (imageVoxel[mrg.type])
{
map >> mrg.globx; 
map >>mrg.globy;
map >>mrg.glob;
}

map>>mrg.tempfall;
DrowMethodSelector(gx,gy);

mrg.type=0;MO
map >> mrg.type;

}*/
}
map.close();

//Normal images

i=1;
//randUp(gx,gy,tx,ty);
/*
std::ifstream mapup(strx);


if ( mapup ) 
{
mapup>>mrg.type;
while (mrg.type>0)
{
mapup >> mrg.pos.x; //x coord of the tile, object is located
mapup >> mrg.pos.y; 
mapup >> mrg.pos.x;
mapup >> mrg.pos.y; 
mapup >> mrg.tempage; 
mapup >> mrg.pos.h; 
mapup >> mrg.temps; 
if (imageVoxel[mrg.type])
{
mapup >> mrg.globx; 
mapup >>mrg.globy;
mapup >>mrg.glob;
}

mapup>>mrg.tempfall;

UpperDrowMethodSelector( gx, gy);
mrg.type=0;
mapup >> mrg.type;
}

}

map.close();
*/
return 0;
}
int tylefromfile(int tx, int ty)
{


std::stringstream ss;
ss<<"world/"<<planetname<<"/obj/"<<planetname<<"_"<<tx<<"v"<<ty<<".map";
std::string strx;
ss>>strx;    
//std::ifstream map(strx);
ifstream map(strx,ios::binary);

createData.randomGenerated=0;
//int temptype, temptx, tempty, tempx, tempy,temph,tempdata, temphp, tempelevation, tempage;

if ( map  ) 
{
do {
mrg.type=0;
map.read((char*)&mrg.type, 4);
if (mrg.type==1) 
	LoadWall(&map);
} while (mrg.type);
/*map>>temptype;
while (temptype>0)
{
	map>>temptx;
	map>>tempty;
		map>>tempx;
		map>>tempy;
		map>>temph;
		map>>tempdata;
		map>>temphp;
		map>>tempelevation;
		map>>tempage;
if ((imageVoxel[temptype]) && (temptype>900))
{
map >> createData.angleh;
map >> createData.anglex;
map >> createData.angley;
map >> createData.size;
}
addobjectdata (temptype, temptx, tempty, tempx, tempy, temph, tempdata, temphp,tempelevation,tempage,0);
		temptype=0;
map>>temptype;*/

map.close(); 
return 1;
	}
	else
map.close(); 
return 0;
}
int tyletofile (int tx, int ty)
{
	
QO * n =  qedit.QOhead;
QO * del;
QO * fuckk= qedit.QOhead;
bool nothing=1;
int ci=0;
int i=1;

// Save Terrain
sTyleStroke* tyle=&Terrain[tylextogrid(tx)][tyleytogrid(ty)];
if ((tyle->max>0) && (tyle->updated)){
std::stringstream ss;
ss<<"world/"<<planetname<<"/Terrain/"<<planetname<<"T_"<<tx<<"v"<<ty<<".map";
std::string strx;
ss>>strx; 

ofstream file (strx, ios::binary);	
file.write ((char*)&tyle->max, 4);
file.write ((char*)tyle->stroke, sizeof(sectorStrokes)*tyle->max);
/*file->write ((char *)&w[i].type, 4);
file->write ((char *)&w[i].pos, sizeof(worldcoordinates));
file->write ((char *)w[i].Megavoxel->piece, 8*8*8);
file->write ((char *)w[i].Megavoxel->hp, 8*8*8*4);
file->write ((char *)&w[i].Megavoxel->paletteNo, 4);*/
file.close();
}
tyle->max=0;
//Save Terrain end

std::stringstream ss;
ss<<"world/"<<planetname<<"/obj/"<<planetname<<"_"<<tx<<"v"<<ty<<".map";
std::string strx;
ss>>strx; 

const std::string& tmp = ss.str();
const char* cstr = tmp.c_str();

	i=1;
// remove all randomly generated stuff
	while (i<=activew) 
	{
	if	((w[i].pos.tx==tx) && (w[i].pos.ty==ty) && (w[i].type) && (w[i].randGenerated)) 
		w[i].RemoveMe=1;
		i++;
	}

//cavecache; 
while (((cave[ci].type==0) || (cave[ci].tx!=tx) || (cave[ci].ty!=ty)) && (ci<cavecache))
	 {ci++;}

	if (ci<cavecache) nothing=0;


if (nothing)
{
	i=1;
// see  if any walls are there
	while (((w[i].pos.tx!=tx) || (w[i].pos.ty!=ty) || (w[i].type<1) || (w[i].randGenerated)) && (i<=activew) )	
		i++;

// If not, see if any quest objects
if (i>activew)
{
while ((n!= 0) && (nothing))
	if ((n->qotx==tx) && (n->qoty==ty)) {nothing=0; n=0;}
	else 
n=n->next; 	



if (nothing==1)
{
//remove( cstr );
	return 0; // delete the file 
}
}
}

// ********************************** save walls and quests to file
   
//	std::ofstream map( strx );

ofstream file (strx, ios::binary);	

for (i=i; i<=activew; i++)
	if ((w[i].pos.tx==tx) && (w[i].pos.ty==ty) && (w[i].type>0) && (!w[i].randGenerated))
{
if (w[i].type==1) 
	SaveWall(&file, i);
/*map << w[i].type;
map << " ";
map << tx;
map << " ";
map << ty;
map << " ";
map << w[i].pos.x;
map << " ";
map << w[i].pos.y;
map << " ";
map << w[i].height;
map << " ";
map << w[i].solid;
map << " ";
map << w[i].hp;
map << " ";
map << w[i].pos.h*pixInPhis;
map << " ";
map << w[i].age;
map << " ";
if (imageVoxel[w[i].type])
{
map << w[i].angles.Ah;
map << " ";
map << w[i].angles.Ax;
map << " ";
map << w[i].angles.Ay;
map << " ";
map << w[i].angles.size;
map << " ";
}*/
w[i].RemoveMe=1;
		}
	n=0;
n =  qedit.QOhead;

while(n!= 0) {
	if ((n->qotx==tx) && (n->qoty==ty))
	{del=n; if (n== qedit.QOhead)  qedit.QOhead=n->next; 	n = n->next; 
/*map << del->qot;
map << " ";
map << del->qotx;
map << " ";
map << del->qoty;
map << " ";
map << del->qox;
map << " ";
map << del->qoy;
map << " ";
map << del->qoh;
map << " ";
map << del->charge;
map << " ";
map << del->qotimer;
map << " ";
map << 0;
map << " ";
map << del->age;
map << " ";*/
del->DeleteMe=1;
}
	else 
	{
		if (fuckk->next==n) fuckk=fuckk->next;
		n=n->next; 	
	}
}

for (i = ci; i< cavecache; i++)
	if ((cave[i].type!=0) && (cave[i].tx==tx) && (cave[i].ty==ty))
	{
/*map << (cave[i].type+1100);

map << " ";
map << tx;
map << " ";
map << ty;
map << " ";
map << cave[i].x;
map << " ";
map << cave[i].y;
map << " ";
map << cave[i].h;
map << " ";
map << 0;
map << " ";
map << 0;
map << " ";
map << cave[i].elevation;
map << " ";
map << cave[i].age;
map << " ";*/
cave[i].type=0;
	}




//map.close();
	file.close();
	return 1;
}
void setgrid(int x, int y)
{
	int f,k;
int tgridx=gridx;
int tgridy=gridy;
	tgridx+=x;
    tgridy+=y;
	
	if (tgridx>4) tgridx=0;
	if (tgridx<0) tgridx=4;
	if (tgridy>4) tgridy=0;
	if (tgridy<0) tgridy=4;
	gridx=tgridx;
	gridy=tgridy;
	int sgrx=(tgridx+2*x);
	// Which row to delete 
	int sgry=(gridy+2*y);
	if (sgrx>4) sgrx-=5;
	if (sgrx<0) sgrx+=5;
	if (sgry>4) sgry-=5;
	if (sgry<0) sgry+=5;
//	int f;

	if (x)
	{
		// clean quest objects

// clean normal objects
for (int k=0; k<5; k++)
{
	tyletofile (grid.objectx[sgrx][k], grid.objecty[sgrx][k]);

for(int ii=0;ii<phArSize;ii++)
                 for(int jj=0;jj<phArSize;jj++)
					 for(int ll=0;ll<matrdepth;ll++)
						mapmatrix[sgrx][k][ii][jj][ll]=1;

for(int ii=0;ii<phArSize;ii++)
                 for(int jj=0;jj<phArSize;jj++)
					 for(int ll=matrdepth;ll<matrhig+matrdepth;ll++)
						mapmatrix[sgrx][k][ii][jj][ll]=0;


}



// normal clean finished
for (int k=0;k<5;k++)
		{
			int f=gridy-2+k;
			if (f<0) f+=5;
			if (f>4) f-=5;

grid.isshadow[sgrx+1][f+1]=0;

clearUpShadLayer(&shadUp[sgrx+1][f+1]);

memcpy ( uppermp[sgrx+1][f+1]->pixels, image[10]->pixels, 1048576*4 );

createData.refreshingTyle=0;
tyleopen (sgrx,f,cameratylx+2*x,cameratyly+k-2);	
tylefromfile (cameratylx+2*x,cameratyly+k-2);

		}
int mf=sgrx-1*x;
if (mf<0) mf+=5;
if (mf>4) mf-=5;
for (int k=1;k<4;k++)
{
f=gridy-2+k;
			if (f<0) f+=5;
			if (f>4) f-=5;
if (!grid.isshadow[mf+1][f+1]) setshadow(mf,f,cameratylx+x,cameratyly+k-2);
/*
memcpy ( mainmaps[mf+1][f+1]->pixels, storedmp[mf+1][f+1]->pixels, 1048576*4 );

if (MultiplyUpperblit)
ApplyUpperMask(mainmaps[mf+1][f+1], uppermp[mf+1][f+1]);
*/
UpdateSector (mf+1,f+1);


ApplyShadow (&shadUp[mf+1][f+1], mainmaps[mf+1][f+1], 0, 0, tyleSize, tyleSize);

if ((stereo) && (grid.iscaved[mf+1][f+1])) stereoscopizebg(mainmaps[mf+1][f+1]);

}
	}



	if (y)
	{
		//
		//
			// clean quest objects
/*QO * n =  qedit.QOhead;
QO * del;
QO * fuckk= qedit.QOhead;
while(n!= 0) {
	if (n->qoty==objecty[sgrx][sgry]) 
	{del=n; if (n== qedit.QOhead)  qedit.QOhead=n->next; 	n = n->next; delete del;
	if (n== qedit.QOhead) fuckk=n; else fuckk->next=n;}
	else 
	{
		if (fuckk->next==n) fuckk=fuckk->next;
		n=n->next; 	
	}
}*/
// clean normal objects
for (k=0; k<5; k++)
{
	tyletofile (grid.objectx[k][sgry], grid.objecty[k][sgry]);

for(int ii=0;ii<phArSize;ii++)
                 for(int jj=0;jj<phArSize;jj++)
					 for(int ll=0;ll<matrdepth;ll++)
						mapmatrix[k][sgry][ii][jj][ll]=1;

for(int ii=0;ii<phArSize;ii++)
                 for(int jj=0;jj<phArSize;jj++)
					 for(int ll=matrdepth;ll<matrhig+matrdepth;ll++)
						mapmatrix[k][sgry][ii][jj][ll]=0;
}
// normal clean finished


		for (k=0;k<5;k++)
		
	{	
		f=gridx-2+k;
			if (f<0) f+=5;
			if (f>4) f-=5;
grid.isshadow[f+1][sgry+1]=0;


clearUpShadLayer(&shadUp[f+1][sgry+1]);

memcpy ( uppermp[f+1][sgry+1]->pixels, image[10]->pixels, 1048576*4 );
//SDL_FreeSurface(tempholder);


createData.refreshingTyle=0;
tyleopen (f,sgry,cameratylx+k-2,cameratyly+2*y);
tylefromfile (cameratylx+k-2,cameratyly+2*y);
//setshadow(f,sgry,cameratylx+k-2,cameratyly+2*y);
//SDL_FreeSurface(tempholder1);
//SDL_FreeSurface(tempholder2);
	}

int mfy=sgry-1*y;
if (mfy<0) mfy+=5;
if (mfy>4) mfy-=5;

for (k=1;k<4;k++)
{
f=gridx-2+k;
			if (f<0) f+=5;
			if (f>4) f-=5;
if (!grid.isshadow[f+1][mfy+1])  setshadow(f,mfy,cameratylx+k-2,cameratyly+y);
/*
memcpy ( mainmaps[f+1][mfy+1]->pixels, storedmp[f+1][mfy+1]->pixels, 1048576*4 );

if (MultiplyUpperblit)
ApplyUpperMask(mainmaps[f+1][mfy+1], uppermp[f+1][mfy+1]);
else
	apply_surface( 0, 0, uppermp[f+1][mfy+1], mainmaps[f+1][mfy+1] );
*/
UpdateSector (f+1,mfy+1);

ApplyShadow (&shadUp[f+1][mfy+1], mainmaps[f+1][mfy+1], 0, 0, tyleSize, tyleSize);


if ((stereo) && (grid.iscaved[f+1][mfy+1])) stereoscopizebg(mainmaps[f+1][mfy+1]);
//SDL_FreeSurface(tempholder1);
}



	}
		
}
void ApplyBG(){
worldcoordinates temp;
MouseToPos (&temp);
SetTyleBackground(temp.tx, temp.ty, tera.Edit.mb[mgvox.LastUsedTool].DecaySfc);
SaveTerrain(temp.tx, temp.ty);
redrowupperstuff (temp.tx, temp.ty, tylextogrid(temp.tx), tyleytogrid(temp.ty));
}

//****************************************Quest objects
void SetZeroData()
{
	//nq->datax=0;
	//nq->datay=0;
	nq->charge=101;
}
// Quest object functions
void spitLava()
{
//nq->qotimer--;

nq->charge++;
createData.tx=nq->qotx; createData.ty=nq->qoty; createData.x=(nq->qox+31)*accuracy; createData.y=(nq->qoy+27)*accuracy; createData.h=0;
createData.type=840; createData.xspeed=nq->datax*accuracy/20; 
createData.yspeed=nq->datay*accuracy/20;  createData.hspeed=((float)(nq->charge-(nq->charge-50)*2*(nq->charge>50))/15)*accuracy;
createData.randosity=15; createData.numberof=nq->charge/5; createData.flooidness=0; createData.slowing=0.98;
	createData.lightIntensity=8;
createData.lightPisObj=1;
		
if (nq->charge>50)
				addphisicalrandom();

//nq->qotx, nq->qoty, (nq->qox+31)*accuracy, (nq->qoy+27)*accuracy, 0 , 840, nq->datax*accuracy/20,nq->datay*accuracy/20,((float)(nq->charge-(nq->charge-50)*2*(nq->charge>50))/15)*accuracy, 
//15, nq->charge/5,0, 0.98);

createData.xspeed=0; 
createData.yspeed=0;  createData.hspeed=((float)(nq->charge-(nq->charge-50)*2*(nq->charge>50))/30)*accuracy;
createData.randosity=270; createData.numberof=nq->charge/10; createData.flooidness=0; createData.slowing=0.98;

addphisicalrandom();//nq->qotx, nq->qoty, (nq->qox+31)*accuracy, (nq->qoy+27)*accuracy, 0 , 840, 
//0,0,((float)(nq->charge-(nq->charge-50)*2*(nq->charge>50))/30)*accuracy, 270, nq->charge/10,0, 0.98);
				//}
//************************* NOW ADD SMOKE EFFECT
/*smoke[i].type=createData.type;
	smoke[i].tx=createData.tx;
	smoke[i].ty=createData.ty;
	smoke[i].x=createData.x;
	smoke[i].y=createData.y;
	smoke[i].h=createData.h;
	smoke[i].gravity=createData.gravity;
	//smoke[i].slowing=createData.slowing
	smoke[i].hp=createData.hp;*/
if (nq->charge>100) 
{nq->charge=rand() % 100;
nq->datax=rand() % 60 - 30;
nq->datay=katet(nq->datax,rand() % 60 - 30,30);
//nq->datax=
createData.x=(nq->qox-60)*accuracy; 
createData.y=(nq->qoy-90)*accuracy;
createData.type=843;
createData.gravity=-0.03;
//createData.slowing=0.
createData.hp=1599;
createData.boolFunction=&timedDeath;
createData.voidfunction=&basicDrow;
createData.xaccel=(rand() % 500);
createData.yaccel=(rand() % 500);
createData.staticPos=0;
addEffect();
}

}
void rain()
{
nq->qotimer--;

	drowstyle(offx-50,offy-120,140*accuracy,9);

				if (nq->qotimer<bloodness) 
				{
					nq->qotimer=maxbloodness;
				qx=rand() % 300 - 150;
				qy=katet(qx,rand() % 300 - 150,150);
if (qy)
{
createData.tx=nq->qotx; createData.ty=nq->qoty; createData.x=(nq->qox+qx+140)*accuracy; createData.y=(nq->qoy+qy+70)*accuracy; createData.h=200*accuracy;
createData.type=801; createData.xspeed=0; 
createData.yspeed=0;  createData.hspeed=0;
createData.randosity=20; createData.numberof=1; createData.flooidness=0; createData.slowing=0.98;
createData.lightPisObj=0;
addphisicalrandom();
//nq->qotx, nq->qoty, (nq->qox+qx+140)*accuracy, (nq->qoy+qy+70)*accuracy, 200*accuracy , 801, 0, 0, 0, 20, 1,0, 0.98);
}
}

}
void espawnfunk()
{
	nq->qotimer--;
//drow(offx,offy,nq->qot,0);
	if (player.corporial)
if (nq->qotimer<1) 
	if (nq->charge>0) 
	{
		nq->qotimer=100; 
int matr=getmematrix (nq->qotx, nq->qoty, nq->qox, nq->qoy, nq->qoh);
if ((!matr) || (matr>=phinmatr))
{
	
	//	int type=rand() % 100;
	/*if (type<3)
		type=613;
	else*/
//		if (type<20)
//		type=611;

//	else
//	if (type<95)
//	{
		//if (type>40)
	//		type=609;
		/*else 
			{

			type=601;
			addobjectdata (type, nq->qotx, nq->qoty, nq->qox, nq->qoy, 0, 0, 0, 0, now, 0);
		}*/
			
//	}
	//	else
	//		type=605;
	//+5*(rand() % 2)
	addobjectdata (609+5*(rand() % 2), nq->qotx, nq->qoty, nq->qox, nq->qoy, 0, 0, 0, 0, now, 0);
	//609+5*(rand() % 2)
	
	//addobject(offx+20,offy+20,type);		

	nq->charge--;
}
return;
}
	else
	{
		//remove quest object
	}


}
void beinghole()
{
//drow(offx,offy,nq->qot,0);

}
void PickableObject()
{
	drowstyle(offx,offy,0,nq->qot);
	SuperFastVoxel360(&listOfStuff[nq->qotimer].InvVoxel, offx,offy,0,  screen,0, 0, 0);
	bool InteractionPossible=(ipifagor(nq->qox-accurate(player.host->pos.x)+(player.host->pos.tx-nq->qotx)*tyleSize, nq->qoy-accurate(player.host->pos.y)+(player.host->pos.ty-nq->qoty)*tyleSize)<70);
	if (InteractionPossible && (!nq->interacting)) 
		addToBottomList(nq);

	if (!InteractionPossible && (nq->interacting)) 
		removeFromBottomList(nq);
}
void functionassign()
{
	nq->OnCreate=&beinghole;
	if (nq->qot==301) {
		nq->foo=&espawnfunk; return;}
	if (nq->qot==305) { nq->foo = PickMe; nq->interacting=0; return;}
	if (nq->qot==306) {nq->foo=&rain; return;}
	if ((nq->qot==302) || (nq->qot==307)) { nq->foo=&beinghole; return;}
	if (nq->qot==308) {nq->foo=&spitLava; nq->OnCreate=&SetZeroData; return;}
	nq->qotimer=100;

}
void AllComplexObjects()
{
QO* prev=nq= qedit.QOhead;
while(nq) {
	qobjects++;
offx=nq->qox - actcamx - (cameratylx - nq->qotx)*tyleSize;
	offy=nq->qoy - actcamy - (cameratyly - nq->qoty)*tyleSize;	
	nq->foo();
	if ((!play) && (nq->scriptText)) apply_surface( offx, offy, nq->scriptText, screen );
	if (nq->DeleteMe) 
	{
		QO* del=nq; 
		
		if (nq== qedit.QOhead) 
		{
			 qedit.QOhead=prev=nq->next; 
		}
		else
		{
			prev->next=nq->next;
		}
		nq = nq->next;
		delete del;

		
	}
	else
	{
prev=nq;
	nq = nq->next;
        }
}
}

// NET
void GuestCharAssigner(int no)
{
	int i=1;
		while ((e[i].type) && (i<elim)) i++;
	e[i].type=613;
	eassigner(i);
	
	e[i].i=i;

	visiter[no].host=&e[i];
	visiter[no].guestW=&e[i].walk;
	visiter[no].host->targetOnScreen=&visiter[no].target; 
	e[i].controled=1;
//	e[i].g8lineup.bfirst=0;
//	e[i].g8lineup.pfirst=0;
//	e[i].g8lineup.PEfirst=0;


	visiter[no].bag.LeftHand.overheating=1;
visiter[no].bag.RightHand.overheating=1;

for (int bx=0; bx<8; bx++)
				for (int by=0; by<8; by++)
					visiter[no].bag.pack[bx][by].count=0;
visiter[no].bag.LeftHand.count=0;
visiter[no].bag.RightHand.count=0;

visiter[no].host->walk.itemInHand=&visiter[no].bag.RightHand;



int hig=0+matrdepth;
	if (efirst[hig]) efirst[hig]->prev=&e[i];
	e[i].prev=NULL;
	e[i].next=efirst[hig];
	
	efirst[hig]=&e[i]; 
	if (i>ecache) ecache=i+1;
visiter[no].host->hp=5000;

}

void GetGuestCoordinates(int no){

	short *charDTA=(short*)indata[no].what;
	charDTA+=2;
int i=*charDTA; 
int* nmbr = new int[i];

for (int j=0; j<i; j++){
	charDTA+=2;
int vno=*charDTA; 
nmbr[j]=vno;
visiter[vno].type=1;
inet.client.MaxGuests=max(inet.client.MaxGuests,vno+1);
charDTA+=2;
visiter[vno].tx=*charDTA; 
charDTA+=2;
visiter[vno].ty=*charDTA; 
}
charDTA++;
for (int j=0; j<i; j++){
int vno=nmbr[j];
charDTA++;
visiter[vno].x=*charDTA; 
visiter[vno].x*=accuracy;
charDTA++;
visiter[vno].y=*charDTA; 
visiter[vno].y*=accuracy;
charDTA++;
visiter[vno].h=*charDTA; 
visiter[vno].h*=accuracy;



if (vno!=inet.client.MySlotOnServer){
		if ((visiter[vno].host==0) || (!visiter[vno].host->type))
		GuestCharAssigner(vno);
		else
			ClearArrayFromEnemy(visiter[vno].host);

		visiter[vno].host->clock=0;

		charDTA++;	
		visiter[vno].host->force.speedx=*charDTA*accuracy;
		charDTA++;	
		visiter[vno].host->force.speedy=*charDTA*accuracy;
		charDTA++;	
		visiter[vno].host->targetOnScreen->x=*charDTA;
		charDTA++;	
		visiter[vno].host->targetOnScreen->y=*charDTA;
		charDTA++;
		visiter[vno].guestW->shooting=*charDTA;
		visiter[vno].host->pos.tx=visiter[vno].tx; 
		visiter[vno].host->pos.ty=visiter[vno].ty; 
		visiter[vno].host->pos.x=visiter[vno].x;
		visiter[vno].host->pos.y=visiter[vno].y;
		visiter[vno].host->pos.h=visiter[vno].h;
		FillArrayWithEnemy(visiter[vno].host);
		visiter[vno].bag.LeftHand.cooldown=-1;
		if (visiter[vno].guestW->shooting)
		GuestRifle(1, &visiter[vno].bag.LeftHand, vno);
		}else charDTA+=5;

}

}
void SendGuestCoordinates(){
memset(&packer,0,sizeof(pack));

packer.countchar=0;
packer.countint=1; // first will store number of characters
packer.countshort=0;

	for (int i=0; i<inet.serv.connections; i++)
		if ((inet.serv.clientConected[i]) && (visiter[i].type))
		{

packer.ints[packer.countint]=i;
packer.ints[packer.countint+1]=visiter[i].tx;
packer.ints[packer.countint+2]=visiter[i].ty;
packer.shorts[packer.countshort]=visiter[i].x/accuracy;
packer.shorts[packer.countshort+1]=visiter[i].y/accuracy;
packer.shorts[packer.countshort+2]=visiter[i].h/accuracy;
packer.shorts[packer.countshort+3]=visiter[i].host->force.speedx/accuracy;
packer.shorts[packer.countshort+4]=visiter[i].host->force.speedy/accuracy;
packer.shorts[packer.countshort+5]=visiter[i].host->targetOnScreen->x;
packer.shorts[packer.countshort+6]=visiter[i].host->targetOnScreen->y;
packer.shorts[packer.countshort+7]=visiter[i].guestW->shooting;
//packer.shorts[packer.countshort+7]=visiter[i].guestW;

packer.countint+=intsperChar;
packer.countshort+=shortsperchar;


if (packer.countint*4+packer.countshort*2>=messagefat-intsperChar*4-shortsperchar*2-8){
	packer.ints[0]=packer.countshort/shortsperchar;
	packinfo (&packer,3);
	packer.countchar=0;
    packer.countint=1; // first will store number of characters
    packer.countshort=0;
	}
}
if (packer.countshort) {
	packer.ints[0]=packer.countshort/shortsperchar;
	packinfo (&packer,3);
}
}

void SendCharCoordinates(){
packer.countint=2;
packer.countshort=8;
packer.countchar=0;
packer.ints[0]=player.host->pos.tx;
packer.ints[1]=player.host->pos.ty;
packer.shorts[0]=player.host->pos.x/accuracy;
packer.shorts[1]=player.host->pos.y/accuracy;
packer.shorts[2]=player.host->pos.h/accuracy;
packer.shorts[3]=player.host->force.speedx/accuracy;
packer.shorts[4]=player.host->force.speedy/accuracy;
packer.shorts[5]=player.host->targetOnScreen->x;
packer.shorts[6]=player.host->targetOnScreen->y;
packer.shorts[7]=playerW->shooting;
packinfo (&packer,2);
}

void serverstuff()
{
SendGuestCoordinates();

	int i=0;
while ((i<dataslots-1) && (flagoutmail (&indata[i] ,0, 0))  )
{ 
	if (*((int*)indata[i].what)==2)  // coordinates and stuff
	{
		int no=indata[i].ClientNo;
		visiter[no].type=1;
		
		visiter[no].tx=*((int*)indata[i].what+1);
		visiter[no].ty=*((int*)indata[i].what+2);
		visiter[no].x=*((short*)indata[i].what+6);
		visiter[no].x*=accuracy;
		visiter[no].y=*((short*)indata[i].what+7);
		visiter[no].y*=accuracy;
		visiter[no].h=*((short*)indata[i].what+8);
		visiter[no].h*=accuracy;

if ((visiter[no].host==0) || (!visiter[no].host->type))
GuestCharAssigner(no);
			else
			ClearArrayFromEnemy(visiter[no].host);

visiter[no].host->pos.tx=visiter[no].tx; 
visiter[no].host->pos.ty=visiter[no].ty; 
visiter[no].host->pos.x=visiter[no].x;
visiter[no].host->pos.y=visiter[no].y;
visiter[no].host->pos.h=visiter[no].h;

FillArrayWithEnemy(visiter[no].host);

visiter[no].host->force.speedx=*((short*)indata[i].what+9)*accuracy;
		visiter[no].host->force.speedy=*((short*)indata[i].what+10)*accuracy;
		visiter[no].host->targetOnScreen->x=*((short*)indata[i].what+11);
		visiter[no].host->targetOnScreen->y=*((short*)indata[i].what+12);
		visiter[no].host->clock=0;
		visiter[no].bag.LeftHand.cooldown=-1;
if (*((short*)indata[i].what+13)){
		GuestRifle(1, &visiter[no].bag.LeftHand, no);
		visiter[no].guestW->shooting=1;}
else visiter[no].guestW->shooting=0;

		



		flagoutmail (&indata[i] ,0, 1);
	}else
	  if (*((int*)indata[i].what)==3) // test and stuff
		inet.client.PackageCounter++;
	  else
		flagoutmail (&indata[i] ,0, 1);
i++;
}

	if (inet.serv.listening==false) {
		SDL_WaitThread(inet.serv.LookForClient, &inet.dump);
		inet.serv.LookForClient = SDL_CreateThread( serverfunk, NULL);
	}

}

void clientstuff()
{

SendCharCoordinates();

int i=0;
while ((i<dataslots-1) && (flagoutmail (&indata[i] ,0, 0))  )
{ 
	if (*((int*)indata[i].what)==1) // add object
	{
addobjectdata (*((short*)indata[i].what+6), *((int*)indata[i].what+1), *((int*)indata[i].what+2), *((short*)indata[i].what+7), *((short*)indata[i].what+8), 0, 0, 400, *((short*)indata[i].what+9), 0,1);
flagoutmail (&indata[i] ,0, 1);
	}
	else
	if (*((int*)indata[i].what)==2)  // coordinates and stuff
	{
		visiter[0].type=1;
		visiter[0].tx=*((int*)indata[i].what+1);
		visiter[0].ty=*((int*)indata[i].what+2);
		visiter[0].x=*((short*)indata[i].what+6);
		visiter[0].x*=accuracy;
		visiter[0].y=*((short*)indata[i].what+7);
		visiter[0].y*=accuracy;
		visiter[0].h=*((short*)indata[i].what+8);
		visiter[0].h*=accuracy;
		inet.client.PackageCounter=visiter[0].x;
		flagoutmail (&indata[i] ,0, 1);
	}
	else
    if (*((int*)indata[i].what)==3){ // test and stuff
		GetGuestCoordinates(i);
		flagoutmail (&indata[i] ,0, 1);
	}else
	flagoutmail (&indata[i] ,0, 1);
	i++;
}



if (inet.client.TimeToClose) {

	if (inet.client.ClearInOutThreads){
	SDL_WaitThread(inet.client.ThreadIn, &inet.dump);
	SDL_WaitThread(inet.client. ThreadOut, &inet.dump);
	inet.client.ClearInOutThreads=0;
	}

	if (inet.client.ClearConnectThread)  SDL_WaitThread(inet.client.ThreadConnect, &inet.dump);

	
	inet.client.ThreadConnect= SDL_CreateThread( clientfunk, NULL);
	inet.client.ClearConnectThread=1;
}

}

// INITIALIZATION PART
int DynamicGroundLoading(void *data)
{
	while (!WorldLoaded) SDL_Delay(100);

	while (!closegame)
	{
		if (!grid.redrowing)
		{
grid.redrowing=1;
	if ((camerax>1020-scrwid) && (actcamx>1020-scrwid) ) {	
		cameratylx+=1;	camerax-=tyleSize; actcamx-=tyleSize;  setgrid(1,0);	}
	if ((cameray>1150-scrhid) && (actcamy>1150-scrhid) ) {	
		cameratyly+=1;	cameray-=tyleSize; actcamy-=tyleSize;	 setgrid(0,1);}
	if ((camerax<-150-scrwid) && (actcamx<-150-scrwid) ){	
		cameratylx-=1;	camerax=tyleSize+camerax; actcamx=tyleSize+actcamx;	 setgrid(-1,0);}
	if ((cameray<-150-scrhid) && (actcamy<-150-scrhid) ){	
		cameratyly-=1;	cameray=tyleSize+cameray; actcamy=tyleSize+actcamy;	 setgrid (0,-1); }
grid.redrowing=0;	
		}
	SDL_Delay(20);
	}
		return 0;
}
void LoadSounds()
{
	

	const char snameSG[]="audio/softgutstrack.wav";
	LoadMySound (snameSG, 50, -0.3, -100, &sounds.bank.softGuts, 120);
//	const char snameG[]="audio/gutstrack.wav";
//	LoadMySound (snameG, 60, -10, -100, &sounds.bank.guts, 110);
	const char snameAmb[]="audio/ambient/ambience_grass_field_breeze-001.wav";
	LoadMySound (snameAmb, 10, 0, 10, &sounds.bank.ambientField, 100);

char soundName[100] = {};

for (int i=1; i<=SoundSteps; i++)
{sprintf_s   (soundName, "audio/step/footstep_grass_boots_jog-00%d.wav", i);
LoadMySound (soundName, 10, -1, 10, &sounds.bank.step[i-1], 24);}

if (soundEffects)
{
LoadMySound ("audio/minigun.wav", 100, -1, 70, &sounds.bank.minigun, 20);

for (int i=1; i<=7; i++)
{sprintf_s   (soundName, "audio/boom/explosion_large_debris-00%d.wav", i);
LoadMySound (soundName, 100, -0.001, 50, &sounds.bank.boom[i-1], 100);}

for (int i=1; i<=6; i++)
{sprintf_s   (soundName, "audio/boomFlat/explosion_large_long_decay-00%d.wav", i);
LoadMySound (soundName, 100, -0.01, 50, &sounds.bank.boomFlat[i-1], 100);}


for (int i=1; i<=rifleSounds; i++)
{sprintf_s   (soundName, "audio/rifle/Kick%d.wav", i);
LoadMySound (soundName, 60, -1, 55, &sounds.bank.chaingun[i-1], 10);}

for (int i=1; i<5; i++)
{sprintf_s   (soundName, "audio/fire/fire_ignite-00%d.wav", i);
LoadMySound (soundName, 40, -1, 30, &sounds.bank.fire[i-1], 20);}

LoadMySound ("audio/fire/fire_burning-001.wav", 40, -1, 30, &sounds.bank.flameBurning, 100);

for (int i=1; i<13; i++)
{sprintf_s   (soundName, "audio/cloth/cloth_rustle_short-00%d.wav", i);
LoadMySound (soundName, 45, -1, 40, &sounds.bank.cloth[i-1], 60);}

for (int i=1; i<=9; i++)
{sprintf_s   (soundName, "audio/impact/%d.wav", i);
LoadMySound (soundName, 55, -0.2, 50, &sounds.bank.impact[i-1], 60);}
}
}
int LoadPicturesNShit(void *data)
{

srand (time(NULL));	
// Time check
now= time(0)-1355550000;
// Internet
testbody.tx=testbody.ty=testbody.x=testbody.y=0;

std::ifstream getaddress( "juststuff/serverip.txt" );
getaddress>>inet.client.serverip;
getaddress.close(); 	

//LoadMissionPacks();

if (!OpenGLscreen) SDL_WM_SetCaption( "Loading Vexels", NULL );
curCreation=0;//particles
for (int i=0; i<9; i++) G_p[curCreation][i]=TryOpenVexel(i);

curCreation=2;//Hydralisk
for (int i=0; i<=17; i++) 
	{
//G_p[curCreation][i]=TryOpenVexel(i);
gayVoxel[curCreation][i]=TryOpen8bitVexel(i);
}
curCreation=3; // player
for (int i=0; i<7; i++)
{
	G_p[curCreation][i]=TryOpenVexel(i);
	gayVoxel[curCreation][i]=TryOpen8bitVexel(i);
}
G_p[curCreation][8]=TryOpenVexel(8);
gayVoxel[curCreation][8]=TryOpen8bitVexel(8);// for cloud
G_p[curCreation][7]=TryOpenVexel(7);
gayVoxel[curCreation][7]=TryOpen8bitVexel(7);// for cloud

listOfStuff[2].InvVoxel=TryOpenVexel(6); // minigun
G_p[curCreation][7]=TryOpenVexel(7);

curCreation=4; // rock
for (int i=0; i<1; i++) G_p[curCreation][i]=TryOpenVexel(i);

QGpalette[0]=loadany("pictures/other/projectileLight.png");
curCreation=5;//blood?
QGpalette[curCreation]=loadany("Voxels/5/pald.png");
for (int i=0; i<19; i++) QG_p[curCreation][i]=TryOpenQVexel(i, 0, 0);
gayVoxel[curCreation][0]=TryOpen8bitVexel(0);
//G_p[curCreation][0]=TryOpenVexel(0);

curCreation=6;
QGpalette[curCreation]=loadany("Voxels/6/pal.png");
for (int i=0; i<1; i++) QG_p[curCreation][i]=TryOpenQVexel(i, 1, 1);
 G_p[curCreation][2]=TryOpenVexel(2);

curCreation=7; // quest objects/ INVENTORY OBJECTS
for (int i=0; i<2; i++) listOfStuff[i].InvVoxel=TryOpenVexel(i);
listOfStuff[3].InvVoxel=TryOpenVexel(3);//blade
listOfStuff[4].InvVoxel=TryOpenVexel(4);
gayVoxel[curCreation][7]=TryOpen8bitVexel(7);
//gayVoxel[curCreation][5]=TryOpen8bitVexel(5);


leavesQV=loadany("pictures/other/leaves.png");
curCreation=8; // tree
for (int i=0; i<1; i++) G_p[curCreation][i]=TryOpenVexel(i);

curCreation=9;
QGpalette[curCreation]=loadany("Voxels/9/pal.png");
//for (int i=0; i<19; i++) QG_p[curCreation][i]=TryOpenQVexel(i, 0, 0);
for (int i=0; i<6; i++)
G_p[curCreation][i]=TryOpenVexel(i);
for (int i=6; i<12; i++)
QG_p[curCreation][i]=TryOpenQVexel(i, 0, 0);

curCreation=10;
for (int i=0; i<2; i++)
gayVoxel[curCreation][i]=TryOpen8bitVexel(i);
// Load Voxels End

curCreation=11;
for (int i=0; i<2; i++)
gayVoxel[curCreation][i]=TryOpen8bitVexel(i);

curCreation=12;  // animation markers
for (int i=0; i<9; i++)
gayVoxel[curCreation][i]=TryOpen8bitVexel(i);

int i,j;
/*for (i=0; i<BagSize; i++)
	for (j=0; j<BagSize; j++)
	{	bag.type[i][j]=0;
		bag.count[i][j]=0;}*/
//QuickDrow8bitVex(palette.SmokeBuffer8bit,&testbody, &gayVoxel[3][8], 0, 0, 10, 1,  0.1, 0.1, 0.1,1);
//CopyBufLightDirection (palette.SmokeBuffer8bit, BufferAnglesCopy, E8bitBufferDiam, E8bitBufferDiam);

if (!OpenGLscreen) SDL_WM_SetCaption( "Initializing inventory", NULL );


fireGlobImg=loadany("pictures/other/fireGlob.png");
//flameLight=loadany("pictures/other/fireLight.png");
flameDecay=loadany("pictures/other/burnDecay.png");
shootDecay=loadany("pictures/other/shootDecay.png");
imgBank.bulletWallFuck=loadany("pictures/other/wallfuckersmaskblt.png");
imgBank.decay1024.snowBG=loadany("pictures/bg/snowbg.png");
imgBank.ScreenBlicSmoke=loadany("pictures/other/ScreenBlickSmoke.png");
imgBank.OnImpactGoreHeight=loadany("pictures/other/impactNewHeight.png");
imgBank.DefaultPhisicalBlur=loadany("pictures/other/defBlur.png");
//VolumetricLightBlit(SDL_Surface *layer, SDL_Surface *source, int dx, int dy )
//testPseudoAnim=loadany("pictures/other/pseudoAnimationTest.png");


for (i=0;i<dataslots; i++) 
{ 
indata[i].checkmail=NULL; 
indata[i].datalock=NULL;
outdata[i].checkmail=NULL; 
outdata[i].datalock=NULL;
} 



Idisplay.materials.invImge=loadany("pictures/other/invplate.png");
Idisplay.rectInventoryBlock.y=scrhi-150-Idisplay.materials.invImge->h;
Idisplay.rectInventoryBlock.h=Idisplay.rectInventoryBlock.y+Idisplay.materials.invImge->h;
Idisplay.rectInventoryBlock.x=(scrwi/2-Idisplay.materials.invImge->w)/2;
Idisplay.rectInventoryBlock.w=Idisplay.rectInventoryBlock.x+Idisplay.materials.invImge->w;
Idisplay.materials.leftMB=loadany("pictures/other/mouseLeft.png");
Idisplay.materials.rightMB=loadany("pictures/other/mouseRight.png");
Idisplay.materials.Glassdisplacement=loadany("pictures/other/inventoryGlassDisplacement.png");




if (!OpenGLscreen) SDL_WM_SetCaption( "OldBackgrounds And World Array", NULL );
loadimages("pictures/bg/cracksbg.png",60);
loadimages("pictures/SoftMask.png",74);
loadimages("pictures/charMask.png",75);
loadimages("pictures/flashlight.png",79);
loadimages("pictures/Inventory.png",80);
loadimages("pictures/icebgh.png",81);
loadimages("pictures/clouds.png",82);
//loadimages("pictures/VoxelDesk.png",83); loaded outside
//loadimages("pictures/HeightDesk.png",84);
loadimages("pictures/bg/grbg.png",85);
loadimages("pictures/bg/snowGrassbg.png",86);
loadimages("pictures/bg/shootwall.png",87);


/*	for (i=0;i<guns;i++)
		ammo[i]=0;
	srand ( time(NULL) );
for (i=0;i<elim;i++) e[i].type=0;
*/
	for(i=0;i<5;i++)
		for(j=0;j<5;j++)
			for(int ii=0;ii<phArSize;ii++)
                 for(int jj=0;jj<phArSize;jj++)
					 for(int ll=0;ll<matrdepth;ll++)
						mapmatrix[i][j][ii][jj][ll]=1;

/*	for(i=0;i<5;i++)
		for(j=0;j<5;j++)
			for(int ii=0;ii<phArSize;ii++)
                 for(int jj=0;jj<phArSize;jj++)
					 for(int ll=matrdepth;ll<matrhig+matrdepth;ll++)
						mapmatrix[i][j][ii][jj][ll]=0;*/


for (i=0;i<huds;i++) hudstate[i]=1;

// images
/*image[0]=NULL;
	for (i=1; i<images; i++)
	{
	mergeimage[i]=NULL;
	mergeimaged[i]=NULL;
	}*/

loadimages("pictures/zero.png",1 );
//image[1]=NULL;
loadimages("pictures/vexblood1.png",4 );
loadimages("pictures/vexblood2.png",5 );
loadimages("pictures/vexblood3.png",6 );


mergeimaged[1] = loadany("pictures/bg/crambledbg.png");
mergeimaged[2] = loadany("pictures/bg/crambledbg2.png");

mergeimaged[4] = loadany("pictures/other/vexblood1.png");
mergeimaged[5] = loadany("pictures/other/vexblood2.png");
mergeimaged[6] = loadany("pictures/other/vexblood3.png");

mergeimaged[7] = loadany("pictures/bg/bloodybg.png");
mergeimaged[8] = loadany("pictures/bg/bloodybg1.png");
mergeimaged[9] = loadany("pictures/bg/bloodybg2.png");

	loadimages("pictures/shadowmap.png",2 );
	loadimages("pictures/cracksup.png",8);
	loadimages("pictures/transparentmap.png",10);
	loadimages("pictures/princeshadow.png",11);
	loadimages("pictures/dot0.png",12);
	loadimages("pictures/dot2.png",14);
	loadimages("pictures/shoot.png",17);
	shadowimage[17]=loadany("pictures/shootLightTrack.png");
	loadmergeimage("pictures/shootmerge.png",18);
	loadimages("pictures/shoot0.png",18);
	loadimages("pictures/shoot1.png",19);
	loadmergeimage("pictures/shootlight.png",19);
	loadimages("pictures/blood.png",20);
	loadimages("pictures/blood0.png",21);
	loadimages("pictures/blood1.png",22);
	loadimages("pictures/ehead.png",23);

	loadimages("pictures/ehead0.png",24);
	loadimages("pictures/ehead1.png",25);
	 //Enemy hit
	loadimages("pictures/erarm.png",27);

	loadimages("pictures/erarm0.png",28);
	loadimages("pictures/erarm1.png",29);
	loadimages("pictures/etors.png",30);

	loadimages("pictures/etors0.png",31);
	loadimages("pictures/etors1.png",32);
	loadimages("pictures/frame.png",33);
	
	loadimages("pictures/elarm.png",35);

	loadimages("pictures/elarm0.png",36);
	loadimages("pictures/elarm1.png",37);
	loadimages("pictures/bodychunka.png",38);

	loadimages("pictures/bodychunka0.png",39);
	loadimages("pictures/bodychunka1.png",40);
	loadimages("pictures/expblood.png",41);
	
	loadimages("pictures/expblood0.png",42);
	loadimages("pictures/expblood1.png",43);
	// 100+ is decor images (in merge images)
	// merge images

	//loadmergeimage("pictures/mergemcrock.png",4);
	//loadmergeimage("pictures/mergeground.png",7);
	//loadmergeimage("pictures/mergecracks.png",8);
	loadmergeimage("pictures/hitsplat.png",16);
	loadmergeimage("pictures/mergeblood.png",20);
	loadmergeimage("pictures/mergeehead.png",23);
	loadmergeimage("pictures/mergeerarm.png",27);
	mergeimage[35]=mergeimage[30]=mergeimage[23];
		loadmergeimage("pictures/pal.png",36);
	//lays
	loadimages("pictures/layup.png",101);
	loadimages("pictures/layupbig.png",99);
	loadmergeimage("pictures/other/cloudmask.png",7);
	loadimages("pictures/other/cloud.png",7);



	loadimages("pictures/other/smallSmoke.png",1502);
	loadmergeimage("pictures/other/smallSmokeMask.png",1502);
	mergeimaged[1502]=loadany("pictures/other/smallBloodCloud.png");
	loadimages("pictures/other/smallSmoke1.png",1503);
	loadmergeimage("pictures/other/smallSmokeMask1.png",1503);
	mergeimaged[1503]=loadany("pictures/other/smallBloodCloud1.png");
	loadimages("pictures/other/smallSmoke2.png",1504);
	loadmergeimage("pictures/other/smallSmokeMask2.png",1504);
	mergeimaged[1504]=loadany("pictures/other/smallBloodCloud2.png");
	loadimages("pictures/other/smallSmoke3.png",1505);
	loadmergeimage("pictures/other/smallSmokeMask3.png",1505);
	mergeimaged[1505]=loadany("pictures/other/smallBloodCloud3.png");
	loadimages("pictures/other/smallSmoke4.png",1506);
	loadmergeimage("pictures/other/smallSmokeMask4.png",1506);
	mergeimaged[1506]=loadany("pictures/other/smallBloodCloud4.png");
	// impact smoke
	loadimages("pictures/other/smallSmoke5.png",1507);
	loadmergeimage("pictures/other/smallSmokeMask5.png",1507);
	loadimages("pictures/other/smallSmoke6.png",1508);
	loadmergeimage("pictures/other/smallSmokeMask6.png",1508);
	//mergeimaged[1507]=loadany("pictures/other/smallBloodCloud5.png"); - later
	loadmergeimage("pictures/bg/cavedLava.png",1501);
std::stringstream ss;
    std::string strx;

wallFmask[0]=loadany("pictures/other/wallfuckersmask.png");
wallFmask[1]=loadany("pictures/other/plasmaburn.png");

basicDisplace=loadany("pictures/other/forMicroDisplacements.png");

loadImageLineUp(&fireSmokeMask[0], "pictures/other/SmokeParticles/sp", 1 );
loadImageLineUp(&shootAnim[0], "pictures/other/", 1 );
loadImageLineUp(&shootMASK[0], "pictures/other/m", 1 );
loadImageLineUp(&shootMASK[11], "pictures/other/m", 12 );
loadImageLineUp(&flameMask[0], "pictures/other/flameParticles/m", 1 );
loadImageLineUp(&flameLight[0], "pictures/other/flameLight/fireLight", 1 );
loadImageLineUp(&PseudoAnim[0], "pictures/other/pseudoAnim/", 1 );
loadImageLineUp(&PseudoAnimBig[0], "pictures/other/pseudoAnim/big/", 1 );
DecayGutsMask=loadImageLineUp(&DecayMaskForGuts[0], "pictures/other/gutsDecay/", 1 );
int testSM=0;
QuickDrow8bitVex(palette.Buffer8bit,&testbody, &gayVoxel[3][7], 20, 5, 10, 1,  0, 0, 0,0);
for (int i=0; i<4; i++)
{
ConvertBufLightToImag (fireSmokeMask[i], &dLISmoke[i], (rand() % 40) - 20, (rand() % 40) - 20 );
OverlapUp[i].h=fireSmokeMask[testSM]->h; OverlapUp[i].w=fireSmokeMask[testSM]->w; 
OverlapUp[i].DnA=ImageAlphaToUint8Mask(fireSmokeMask[testSM]);

OverlapUpFlame[i].h=flameMask[i]->h; OverlapUpFlame[i].w=flameMask[i]->w; 
OverlapUpFlame[i].DnA=ImageAlphaToUint8Mask(flameMask[i]);
//fireSmokeMask
}


//fireGlobImg
SDL_Surface* hold=loadany("pictures/other/smokeUnderMask.png");
OverlapDown.h=hold->h; OverlapDown.w=hold->w; 
OverlapDown.DnA=ImageAlphaToUint8Mask(hold);
SDL_FreeSurface(hold);
//RealAnimatedSmoke
hold=loadany("pictures/other/pseudoAnim/animated.png");
RealAnimatedSmoke.h=hold->h/7; RealAnimatedSmoke.w=hold->w/7;
RealAnimatedSmoke.DnA=ImageAlphaToUint8Mask(hold);
SDL_FreeSurface(hold);
	//createShadedSmoke(&gayVoxel[3][0], &dLISmoke[i],  fireSmokeMask[i]);
	/*for (int i=1; i<15; i++)
	{
ss<<i;
ss>>strx; 
//shootAnim[i-1]=loadany("pictures/other/"+strx+".png");
shootMASK[i-1]=loadany("pictures/other/m"+strx+".png");
ss.clear();
	strx.clear();
	}*/

if (!OpenGLscreen) SDL_WM_SetCaption( "Layers", NULL );
    i=0;
	//std::ifstream map( "pictures/layers/overbool.txt" );
	//overlaylap[200]
	//heightimage
	while (!i)
	{
		ss.clear();
	strx.clear();
		ss<<lays;
		ss>>strx; 
	if (loadAllmergeimages("pictures/layers/lay"+strx+".png","pictures/layers/layd"+strx+".png","pictures/layers/laym"+strx+".png","pictures/layers/layover"+strx+".png" ,101+lays)) 
		{
			heightimage[101+lays] = loadany("pictures/layers/h"+strx+".png");
			dhimage[101+lays] = loadany("pictures/layers/dh"+strx+".png");
decayBG[101+lays]=image[85];
SnowydecayBG[101+lays]=image[86];
dhDecay[101+lays]=0;
		imageVoxel[101+lays]=0;
	overlaylap[1+lays]=0;
	//map>>overlaylap[1+lays];
			lays++;
	
	if (!loadimages("pictures/layers/layup"+strx+".png",100+lays)) 
	{  
		if(mergeimage[100+lays]){
		if ((mergeimage[100+lays]->w<171) && (mergeimage[100+lays]->h<171))
		image[100+lays]=image[101];
	else image[100+lays]=image[99];
		}else
			if(mergeimaged[100+lays]){
		if ((mergeimaged[100+lays]->w<171) && (mergeimaged[100+lays]->h<171))
		image[100+lays]=image[101];
	else image[100+lays]=image[99];
			}
	}
	ss.clear();
	strx.clear();
	}
	else i=1;
	
	}
//	map.close();

// Connecting Voxels to images
	imageVoxel[117]=&G_p[4][0];
	SnowydecayBG[106]=SnowydecayBG[107]=decayBG[106]=decayBG[107]=image[60];
	SnowydecayBG[108]=decayBG[108]=imgBank.decay1024.snowBG;
	dhDecay[119]=imgBank.decay1024.snowBG;
if (!OpenGLscreen) SDL_WM_SetCaption( "quest obj", NULL );
	//Quest objects 300+
	ss.clear();
	strx.clear();
    i=0;
	while (i==0)
	{
		ss<<qolays;
		ss>>strx; 
	if (loadmergeimage("pictures/qlay/qlay"+strx+".png",300+qolays)) 
		{qolays++;
	if (loadimages("pictures/qlay/qlayup"+strx+".png",299+qolays)==0) 
	{   if ((mergeimage[299+qolays]->w<171) && (mergeimage[299+qolays]->h<171))
		image[299+qolays]=image[101];
	else image[299+qolays]=image[99];
	}
	ss.clear();
	strx.clear();
	}
	else i=1;
	}
// Units/somebodys 600+
	//loadimages("pictures/e0.png",15);
	//loadimages("pictures/eh0.png",26);
	//loadimages("pictures/e0b.png",34); // Blured enemy
if (!OpenGLscreen) SDL_WM_SetCaption( "chars", NULL );
ss.clear();
strx.clear();
i=0;
	chars=1;
	while (i==0)
	{
		ss<<chars;
		ss>>strx; 
	if (loadimages("pictures/char/char_"+strx+".png",600+1+(chars-1)*2 )) 
	{
	generateImageIndexation(600+1+(chars-1)*2);
	loadimages("pictures/char/char_"+strx+"b.png",600+2+(chars-1)*2 );
	loadmergeimage("pictures/char/mchar_"+strx+".png",600+1+(chars-1)*2);
	loadmergeimage("pictures/char/gutschar_"+strx+".png",600+2+(chars-1)*2);
	heightimage[600+1+(chars-1)*2] = loadany("pictures/char/s_"+strx+".png");
	chars++;	
	ss.clear();
	strx.clear();
	}
	else i=1;
	
	}


	if (!OpenGLscreen) SDL_WM_SetCaption( "phisx obj", NULL );
//phisical objects
	ss.clear();
	strx.clear();
    i=0;
	drops=1;
	while (i==0)
	{
		ss<<drops;
		ss>>strx; 
	if (loadimages("pictures/drops/drop_"+strx+".png",800+1+(drops-1)*3 )) 
	{
	loadmergeimage("pictures/drops/dropmerge_"+strx+".png",800+1+(drops-1)*3); // shadow
	loadmergeimage("pictures/drops/dropmerge2_"+strx+".png",800+2+(drops-1)*3); // Like a splash or a spark - special case
	loadmergeimage("pictures/drops/dropmerge3_"+strx+".png",800+3+(drops-1)*3); // lightning effect

	loadimages("pictures/drops/drop0_"+strx+".png",800+2+(drops-1)*3);
	loadimages("pictures/drops/drop1_"+strx+".png",800+3+(drops-1)*3);
	mergeimaged[800+1+(drops-1)*3] = loadany("pictures/drops/dropD_"+strx+".png"); // Decaying part (alpha 0-16)
	shadowimage[800+1+(drops-1)*3]=loadany("pictures/drops/shadow_"+strx+".png");
	drops++;	
	ss.clear();
	strx.clear();
	}
	else i=1;
	
	}


// ########################################################################################## Loading of the wall's
	//int wallined=1; 
//int totalwalls=0; 

	if (!OpenGLscreen) SDL_WM_SetCaption( "walls", NULL );
ss.clear();
strx.clear();

std::stringstream kk;
std::string strk;
i=0;
	 totalwalls=1;

std::ifstream mapp( "pictures/walls/solidness.map" );
	//if ( map != NULL ) 
//{
	//map >> x; 


	while (i==0)
	{
		ss<<totalwalls;
		ss>>strx; 
	if (loadmergeimage("pictures/walls/mergewall_"+strx+".png",900+totalwalls)) 
	{
		heightimage[900+totalwalls] = loadany("pictures/walls/h"+strx+".png");
		dhimage[900+totalwalls] = loadany("pictures/walls/dh"+strx+".png");
		shadowimage[900+totalwalls]=loadany("pictures/walls/shadow"+strx+".png");
		mergeimaged[900+totalwalls]=loadany("pictures/walls/guts"+strx+".png");

		loadimages("pictures/walls/layup"+strx+".png",900+totalwalls);
		loadoverlay("pictures/walls/ruin_"+strx+".png",900+totalwalls);
		loaddisplacement("pictures/walls/dis"+strx+".png",totalwalls);
	uppermerge[totalwalls]=loadany ("pictures/walls/uppermerge"+strx+".png");

strk.clear();
		
wallh[totalwalls]=-1; 
		if (loadwall("pictures/walls/brick"+strx+".png", totalwalls))	
		{
		wallh[totalwalls]=1;
		InnerWall[totalwalls]=loadany("pictures/walls/inner"+strx+".png");
		}
		//wallsolid[totalwalls]=40;
		wallparticle[totalwalls]=804;
		mapp>>wallsolid[totalwalls];

		


	totalwalls++;	
	ss.clear();
	strx.clear();
	}
	else i=1;
	
	}
wallh[10]=2;
wallparticle[10]=20;
imageVoxel[911]=imageVoxel[904]=&G_p[8][0];

mapp.close();
	
displacement[1]=loadany("pictures/other/explosionDisplacement.png");


if (!OpenGLscreen) SDL_WM_SetCaption( "caves", NULL );
// ********************************************************* Underground part
//underg[5][3];

ss.clear();
strx.clear();
    i=0;
	caves=1;
	//loadany
	while (i==0)
	{
		ss<<caves;
		ss>>strx; 
	underg[caves][0] = loadany("pictures/underground/underdown_"+strx+".png");
	if (underg[caves][0]==0) 
		underg[caves][0]=loadany("pictures/underground/underdown_"+strx+".jpg");
	underg[caves][1] = loadany("pictures/underground/undermask_"+strx+".png");
	underg[caves][2] = loadany("pictures/underground/underup_"+strx+".png");
	underg[caves][3] = loadany("pictures/underground/merge_"+strx+".png");
	if ((underg[caves][0]!=0) || (underg[caves][1]!=0) || (underg[caves][2]!=0))
	{
	heightimage[1100+caves] = loadany("pictures/underground/h"+strx+".png");
	overlayimage[1100+caves] = image[1100+caves]=underg[caves][2];
	mergeimage[1100+caves]=underg[caves][3];
	dhimage[1100+caves] = loadany("pictures/underground/dh_"+strx+".png");
	caves++;	
	ss.clear();
	strx.clear();
	}
	else i=1;
	
	}

	for (i=0; i<cavelim; i++) cave[i].type=0; 
// ***********************************************

	if (!OpenGLscreen) SDL_WM_SetCaption( "hud", NULL );
	i=10;
	music=0;

	//Mix_PlayMusic( music, 2 );
	//HUD
	loadhud ("pictures/place.png",1);
	hudx[1]=120;
	hudy[1]=280;
	loadhud ("pictures/place1.png",2);
	hudx[2]=120;
	hudy[2]=280;
	loadhud ("pictures/rock.png",11);
	hudstate[11]=11; 
	hudx[11]=1200;
	hudy[11]=70;
	loadhud ("pictures/groundsample.png",12);
	hudstate[12]=12;
	hudx[12]=1100;
	hudy[12]=70;
	loadhud ("pictures/crackssample.png",13);
	hudstate[13]=13;
	hudx[13]=tyleSize;
	hudy[13]=70;
	loadhud ("pictures/esample.png",14);
	hudstate[14]=14;
	hudx[14]=900;
	hudy[14]=70;
	loadhud ("pictures/holesample.png",15);
	hudstate[15]=15;
	hudx[15]=800;
	hudy[15]=70;
	loadhud ("pictures/shootsample.png",16);
	hudstate[16]=16;
	hudx[16]=700;
	hudy[16]=70;
	loadhud ("pictures/brushsample.png",17);
	hudstate[17]=17;
	hudx[17]=70;
	hudy[17]=350;
	loadhud ("pictures/yup.png",18);
	hudstate[18]=18;
	hudx[18]=70;
	hudy[18]=460;
	loadhud ("pictures/ydown.png",19);
	hudstate[19]=19;
	hudx[19]=70;
	hudy[19]=410;
	loadhud ("pictures/play.png",20);
	hudstate[20]=20;
	hudx[20]=50;
	hudy[20]=80;
	loadhud ("pictures/frame.png",21);
	hudstate[21]=21;
	hudx[21]=400;
	hudy[21]=200;

	loadhud ("pictures/barrelsample.png",22);
	hudstate[22]=22;
	hudx[22]=600;
	hudy[22]=70;
	/*
	barrelsample
hud (120,280,1);// Okey button 120 120 // layers 1100, 430 y to 280 530
*/
	
	gamestate=0;
	curlay=lays;


	
	


	//Uint32 *pixmem32;
	//pixmem32 = (Uint32*) image[2]->pixels;
	//shadowmapcolor=*pixmem32;
	shadowmapcolor=16777216*sunshadow;

if (!OpenGLscreen) SDL_WM_SetCaption( "WaterMasks", NULL );
/*	for (i=0; i<matrhig+matrdepth; i++)
	{
		efirst[i]=NULL;
		elast[i]=NULL;
		//bfirst[i]=NULL;
		//blast[i]=NULL;
		phfirst[i]=NULL;
		phlast[i]=NULL;
	}*/
loadimages("pictures/bg/soilbg.png",3);
loadimages("pictures/bg/waterMask1.png",62);
picturesloaded=1;


if (!OpenGLscreen) SDL_WM_SetCaption( "audio", NULL );
// audio

	

	
	int song=rand() % 4;
	if (wantmusic)
	switch (song)
	{
	case 0: music = Mix_LoadMUS( "audio/song.ogg" ); break;
	case 1: music = Mix_LoadMUS( "audio/song1.ogg" ); break;
	case 2: music = Mix_LoadMUS( "audio/song2.ogg" ); break;
	case 3: music = Mix_LoadMUS( "audio/song3.ogg" ); break;
//	case 4: music = Mix_LoadMUS( "audio/song4.ogg" ); break;
	}


	low = Mix_LoadWAV( "test.wav" );
    gunboom[0] = Mix_LoadWAV("audio/gun0.wav");
	gunboom[1] = Mix_LoadWAV("audio/gun1.wav");
	gunboom[2] = Mix_LoadWAV("audio/gun2.wav");
	
	
LoadSounds();	
	//sounds.guts.sound=Mix_LoadWAV("audio/gutstrack.wav");
	//sounds.softguts.sound=Mix_LoadWAV("audio/softgutstrack.wav");
	//Mix_Volume(1,MIX_MAX_VOLUME/3);
	//Mix_Volume(2,MIX_MAX_VOLUME/3);
	//Mix_Volume(3,MIX_MAX_VOLUME/3);
	//Mix_Volume(4,MIX_MAX_VOLUME/3);

//SetUpsounds();

loadGame();

if (!OpenGLscreen) SDL_WM_SetCaption( "Tread Load Finish", NULL );
PlayMySound(&sounds.bank.coolNoise[3]);
createData.returnHolder=1;
MaterialsLoaded=1;
return 0;
}
bool LoadGround ()
{

	initdrow();

	if (!WorldLoaded)
	{

if (!OpenGLscreen) SDL_WM_SetCaption( "Getting surfaces", NULL );
for (int i=1; i<6; i++)
	for (int j=1; j<6; j++)
	{
	storedmp[i][j]=SDL_ConvertSurface(image[2], image[2]->format, SDL_SWSURFACE);
	uppermp[i][j]=SDL_ConvertSurface(image[10], image[10]->format, SDL_SWSURFACE);
	grid.isshadow[i][j]=0;
	grid.iscaved[i][j]=0;

	}	



// ************************************* Screenlike surfaces


if (!OpenGLscreen) SDL_WM_SetCaption( "creating world", NULL );
for (int i=0; i<5; i++)
	for (int j=0; j<5; j++)
	{
createData.refreshingTyle=0;
if (!OpenGLscreen) SDL_WM_SetCaption( "creating world: open tyle", NULL );  // the longest
tyleopen (i,j,cameratylx+i-2,cameratyly+j-2);
if (!OpenGLscreen) SDL_WM_SetCaption( "creating world: open objects", NULL );
tylefromfile(cameratylx+i-2, cameratyly+j-2);
if (!OpenGLscreen) SDL_WM_SetCaption( "creating world: To Main map", NULL );
if ((i==0) || (j==0) || (i==4) || (j==4)) mainmaps[i+1][j+1]=SDL_ConvertSurface(storedmp[i+1][j+1], storedmp[i+1][j+1]->format, SDL_SWSURFACE);
	}

	if (!OpenGLscreen) SDL_WM_SetCaption( "painting", NULL );
for (int i=1; i<4;i++)
	for (int j=1; j<4; j++)
	{
		if (!OpenGLscreen) SDL_WM_SetCaption( "painting: shadow", NULL );  // the longest
		setshadow(i,j,cameratylx+i-2,cameratyly+j-2);
			if (!OpenGLscreen) SDL_WM_SetCaption( "painting: To Main map", NULL );
		mainmaps[i+1][j+1]=SDL_ConvertSurface(storedmp[i+1][j+1], storedmp[i+1][j+1]->format, SDL_SWSURFACE);
		if (!OpenGLscreen) SDL_WM_SetCaption( "painting: apply upper and shadows", NULL );		
		
		//
		if (MultiplyUpperblit) 
		ApplyUpperMask(mainmaps[i+1][j+1], uppermp[i+1][j+1]);
		else
			apply_surface( 0, 0, uppermp[i+1][j+1], mainmaps[i+1][j+1] );

		ApplyShadow (&shadUp[i+1][j+1], mainmaps[i+1][j+1], 0, 0, tyleSize, tyleSize);
	//	apply_surface( 0, 0, shadowmaps[i+1][j+1], mainmaps[i+1][j+1] );
		if ((stereo) && (grid.iscaved[i+1][j+1])) stereoscopizebg(mainmaps[i+1][j+1]);
	}

// *************************************** Correct resolution screen
if (!OpenGLscreen) SDL_WM_SetCaption( "Decay v.0.030", NULL );
WorldLoaded=true;

return 1;
	}
	return 0;


}

void initconditions ()
{
foophi=&phisfuncassigner;
foonull=&functionassign;
tyopenfucktion=&tyleopen;
beinsideobjectp=&beinsideobject;
beinginsideenemyp=&beinginsideenemy;
beinginsideph=&beinginsidephis;
eassigner=&eDataAssign;
wallfunk=&wallfuncassigner;
DirectedShadowLDrow=&DirectionalShadedDrow; 
MouseClick=&mouseeffect;
WhatIsOn=&playison;
bloodstyle=&bloodphisics;
voxelVallConvertor=&SuperFastVoxelToBrick;
addShootEffectPTR=&addShootEffect;

ZeroCharAssigner();


SDL_EnableUNICODE( SDL_ENABLE );
	
// world

	world.brykivka.chance=0.2;
	world.brykivka.max=20;

	world.mcrock.chance=0.5;
	world.mcrock.max=3;

	world.voxelTree.chance=0.6;
	world.voxelTree.max=7;

	world.voxelRocks.chance=0.3;
	world.voxelRocks.max=90;

	world.water.chance=0.05;
	world.water.max=2;
// Idisplay

	//Idisplay.recoil.cooldownPercent=0.998;

for (int i=0; i<bottomListSize; i++)
{
	Idisplay.IOGframe[i].x= 10+i*(180+10);
	Idisplay.IOGframe[i].y= scrhi-75-10;
	Idisplay.IOGframe[i].w= 10+i*(180+10)+180;
	Idisplay.IOGframe[i].h= scrhi-10;
}	
Idisplay.halfh=37;
Idisplay.halfw=90;


	//	apply_surface( 10+i*(image[80]->w+10), scrhi-image[80]->h-10, image[80], screen );


	PickMe=&PickableObject;
	//bag.RightHand.invVoxel=0;
	

	listOfStuff[0].inArmDrow=&rifleDrow;
	listOfStuff[1].inArmDrow=&granadeDrow;
	listOfStuff[2].inArmDrow=&minigunDrow;
	listOfStuff[3].inArmDrow=&bladeDrow;
	listOfStuff[4].inArmDrow=&rifleDrow;
	listOfStuff[0].passiveDrow=listOfStuff[1].passiveDrow=listOfStuff[2].passiveDrow=listOfStuff[3].passiveDrow=listOfStuff[4].passiveDrow=&bladeDrowBack;
	for (int i=0; i<bottomListSize; i++)
	Idisplay.ItemsOnTheGround[i]=0x00000000;
	
// wind
//	dddcoords

	


	// shake.antishake=0.1;
	 shake.shakerCoolDown=0.006;
	 shake.antishakeCoolUp=0.006;

	edit.editor=102;
	edit.scrolling=0; // 0-ylev, 1-mlev, 2-slev
	edit.ylev=0; // Camera rise above y level
	edit.mlev=0; // elevation of decorative objects above ground
	edit.slev=1;
grid.redrowing=0;

// Set deffault camera and player 


for (int i=0; i<=guns; i++)
{
	gunstats[i].recoilAftershootPercent=0.3;
	gunstats[i].aimdown=2;
	gunstats[i].aimup=1;
	gunstats[i].delay=2.2;
	gunstats[i].feedback=accuracy/20;
	gunstats[i].mass=3;
	gunstats[i].aimlim=20;
	gunstats[i].shootspeed=20;
	gunstats[i].gunHoleDist=1.35;
}
gunstats[1].delay=100; // granade launcher
gunstats[4].aimdown=10;
gunstats[4].delay=1.55;//12.25;//2.33; - rifle
gunstats[4].aimlim=255;
gunstats[4].shootspeed=50;//50;
gunstats[4].aimup=1.7;
gunstats[2].aimlim=150;
gunstats[2].aimdown=5;
gunstats[2].gunHoleDist=1.8;
gunstats[3].gunHoleDist=2.8;

gunstats[5].aimlim=10;
gunstats[5].aimdown=0.8;
gunstats[5].aimup=0.2;





mouse.state=0;
Idisplay.active=false;


player.isunderground=false;
play=1;

for (int i=0; i<matrhig+matrdepth; i++)
{
	PEfirst[i].next=&PElast[i];
	PElast[i].prev=&PEfirst[i];
	PEfirst[i].prev=PElast[i].next=0;
	PElast[i].deathcondition=&bnothing;
	PElast[i].drowing=&vnothing;

	bfirst[i].next=&blast[i];
	blast[i].prev=&bfirst[i];
	bfirst[i].prev=blast[i].next=0;
}

solarx[0]=500;
solary[0]=400;
solarname[0]="Awesome solar system 1";

solarx[1]=1200;
solary[1]=700;
solarname[1]="Awesome solar system 2";

solarsun[0]=47;
planets[0]=2;
planame[0][0]="shore"; planetx[0][0]=-400; planety[0][0]=100; planettype[0][0]=0; planetsurface[0][0]=3;
planame[0][1]="Seth"; planetx[0][1]=400; planety[0][1]=-100; planettype[0][1]=1; planetsurface[0][1]=3;

solarsun[1]=61;
planets[1]=1;
planame[1][0]="Tobuscus"; planetx[1][0]=400; planety[1][0]=-100; planettype[1][0]=2; planetsurface[1][0]=3;

Empty8BitBuffer();
palette.G8BufferRadius=0;

for (int i=0; i<maxLights; i++)
	light[i].light=0;

    dustScreen = new Uint32 [scrwi*scrhi];
	if (ZoomShadows) screenShadow = new Uint8 [scrwi*scrhi];
	zoomHscreen = new Uint16 [scrwi*scrhi];
	ZoomDisplacementMaskX = new float [scrwi];
	ZoomDisplacementMaskY = new float [scrhi];
	FillZoomDisplaceArray (ZoomBGDisplacement);


loadImageLineUp(&dustTemp[0], "pictures/other/dust/", 1 );

	for (int i=0; i<4; i++)
	{
dustMask[i].w=dustTemp[i]->w;
dustMask[i].h=dustTemp[i]->h;
dustMask[i].DnA=ImageAlphaToUint8Mask(dustTemp[i]);
		/*
	dustMask[i].DnA= new Uint8[(DustDiam+1+i*2)*(DustDiam+1+i*2)];
	dustMask[i].h=dustMask[i].w=(DustDiam+i*2);
	MakeAlphaPortionMask(dustMask[i].DnA, (DustDiam)/2+i,DustAlpha);*/
	}

	darkmask   = new Uint32 [scrhi*scrwi];
	memset(darkmask, 0, sizeof(darkmask));
	darkmaskGrid = new bool [scrhi*scrwi/64];
	memset(darkmaskGrid, 0, sizeof(darkmaskGrid));
	wind.h=scrhi/8;
	wind.w=scrwi/8;
	wind.mask = new dddcoords [wind.h*wind.w];
	memset(wind.mask, 0, sizeof(dddcoords)*wind.h*wind.w );
	memset(Wblow,0,sizeof(BlowingParticle)*windBlowArSize);
	memset(dust,0,sizeof(SmokeParticle)*dustCount);
	memset(smoke,0,sizeof(ParticleEffects)*PElim);
	memset( bl,0,sizeof(blood)*bphlim);
	memset( ph,0,sizeof(part)*phlim);
 
	wind.global.x=10;//1900;
	wind.global.y=5;//500;
	wind.global.h=0;
	wind.smoothness=50;
	wind.treeslowing=0.99;
	wind.maxBend=65535;
	wind.treeBendDelay=81;
	
    darkIndexStart = new int [scrhi];
	darkIndexEnd = new int [scrhi];
	

	for (int i=0; i<scrhi; i++)
	{
darkIndexStart[i]=scrwi;
darkIndexEnd[i]=0;
	}
	//boolMask = new bool [scrhi*scrwi];



loadimages("pictures/solar.png",45);
//loadimages("pictures/logo01.png",76);
loadimages("pictures/logomask.png",77);
//loadimages("pictures/logo02.png",78);
loadimages("pictures/solarsystem.png",46);
loadimages("pictures/earthsun.png",47);
loadimages("pictures/planet1.png",48);
loadimages("pictures/planet2.png",49);
loadimages("pictures/planet3.png",50);
loadimages("pictures/sun2.png",61);
loadimages("pictures/textline.png",70);
loadimages("pictures/hitsplat.png",71);
loadimages("pictures/server.png",72);
loadimages("pictures/client.png",73);
loadimages("pictures/VoxelDesk.png",83);
loadimages("pictures/HeightDesk.png",84);

loadmergeimage("pictures/stereo.png", 31);
loadmergeimage("pictures/stereoon.png", 32);

imgBank.devITF.MegavoxDesk=loadany("pictures/MegavoxDesk.png");
imgBank.LightNingForLightDrow=loadany("pictures/other/lightning.png");

stick = SDL_JoystickOpen( 0 );
if (stick) mousesmoothing=30;
createData.returnHolder=0;


//************************ Inet
inet.port=7700;
inet.serv.connections=0;
// ************************ Get monitor refresh rate
DEVMODE dm;
EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);
refrate=1000/dm.dmDisplayFrequency;
refrate=min(refrate,7);
//******************************** Sounds
Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ); 
	const char snameAmb[]="audio/ambient/ambience_mood_eerie_radio-001.wav";
	LoadMySound (snameAmb, 10, 0, 10, &sounds.bank.ambientSpace, 50);
char soundName[100] = {};
for (int i=1; i<=12; i++)
{sprintf_s   (soundName, "audio/Cool_noises/noise_hit-00%d.wav", i);
LoadMySound (soundName, 10, -1, 10, &sounds.bank.coolNoise[i-1], 100);}

	PlayAmbient(&sounds.bank.ambientSpace, 6, -1);
TTF_Init();
font = TTF_OpenFont( "juststuff/lazy.ttf", 23 );
	if (LoadGameAssets){
	if (OpenGLscreen)
		LoadPicturesNShit(NULL);
	else
thread = SDL_CreateThread( LoadPicturesNShit, NULL);
	}

if (!OpenGLscreen)
planetselector();
if ((Betatesting) || (OpenGLscreen))
{
	planetname=planame[0][0];
	selectedsurface=planetsurface[0][0];
	selectedplanet=0;
}
//while (!createData.returnHolder) 
//	SDL_Delay(50);	

if (thread)	SDL_WaitThread( thread, NULL );

gridx=2;
gridy=2;

conf3d.lefteye=scrwid-conf3d.eyewight/2;
conf3d.righteye=scrwid+conf3d.eyewight/2;

playerW->legs.lleg.tx=player.host->pos.tx;
playerW->legs.lleg.ty=player.host->pos.ty;
playerW->legs.lleg.x=player.host->pos.x;
playerW->legs.lleg.y=player.host->pos.y;

playerW->legs.rleg.tx=player.host->pos.tx;
playerW->legs.rleg.ty=player.host->pos.ty;
playerW->legs.rleg.x=player.host->pos.x;
playerW->legs.rleg.y=player.host->pos.y;


edit.Megavox.initialised=0;
InitSegment(&edit.Megavox);
edit.mgvlev=8;
edit.ConvertedPalette=0;
mgvox.curpiece=1;



for (int m=0; m<6; m++) edit.MGVpalette[m]=rand() + 0x555500;
	edit.MGVpalette[6]=0x222222;
	edit.MGVpalette[7]=0xAA0000;
	Palette(&edit.MGVpalette[0], &edit.ConvertedPalette);

if (!OpenGLscreen) SDL_WM_SetCaption( "LoadTerrain", NULL );

imgBank.decay1024.oldBG=loadany("pictures/bg/oldbg.png");
if (PixelatedScreen) imgBank.decay1024.PixelationMask=loadany("pictures/bg/PixelationMask.png");
InitialiseTerrain();

if (!OpenGLscreen) SDL_WM_SetCaption( "Initialise Editor", NULL );

if (!Betatesting){

	InitMagavoxMenus();

edit.MegavoxMaker=InitEditor (700, scrhid-100, "Ingame", 8*24, 8*24, DrowIngameEditor,0);
edit.FullScreen=InitEditor (0, 0, "FullScreen", scrwi, scrhi-40, FullScreenWindow,0);
edit.DollHouse=InitEditor (400, 100, "DollHouse", 300, 600, DollHouse,0);
edit.Puppetier=InitEditor (500, 100, "Puppetier", 300, 600, Puppetier,PuppetierEventHandle);

edit.AddVox=InitMyButton("pictures/editor/vexpick.png", DrowInGameEd,1,&edit.MegavoxMaker);
edit.RemoveVox=InitMyButton("pictures/editor/eraseVox.png", EraseInGameEd,1,&edit.MegavoxMaker);
edit.BrakeVox=InitMyButton("pictures/editor/broken.png", brakeInGameEd,1,&edit.MegavoxMaker);
edit.PlaceSite=InitMyButton("pictures/editor/buildSite.png", SelectSite,1,&edit.FullScreen); // equip full screen
edit.UpdateConverted=InitMyButton("pictures/editor/UpdateOnGround.png", TransferIngameMegavox,0,&edit.MegavoxMaker);
edit.spawnMGVOX=InitMyButton("pictures/editor/spawn.png", CreateWall,1,&edit.FullScreen);
edit.clearAllInGame=InitMyButton("pictures/editor/EraseAll.png", EraseAllInGame,0,&edit.MegavoxMaker);
edit.ApplyBGtoTyle=InitMyButton("pictures/editor/setBackGround.png", ApplyBG,1,&edit.FullScreen);

edit.spamCraft=InitMyButton("pictures/editor/spamCraft.png", SelectSiteAndSpawn,1,&edit.FullScreen);
edit.AddLimb=InitMyButton("pictures/editor/LimbPick.png", AddRaggLimb,1,&edit.DollHouse);
edit.RotateLimb=InitMyButton("pictures/editor/LimbRotate.png", RotateLimb,1,&edit.DollHouse);
edit.ConnectLimb=InitMyButton("pictures/editor/LimbConnect.png", ConnectLimb,1,&edit.DollHouse);
edit.SelectLimb=InitMyButton("pictures/editor/LimbSelect.png", SelectLimb,1,&edit.DollHouse);
edit.TakeForAWalk=InitMyButton("pictures/editor/LimbTakeForAWalk.png", TakeForAWalk,0,&edit.DollHouse);
edit.DeleteLimb=InitMyButton("pictures/editor/LimbDelete.png", DeleteLimb,0,&edit.DollHouse);
edit.DragLimb=InitMyButton("pictures/editor/DragTool.png", DragLimb,1,&edit.DollHouse);
edit.AngleFlexibility=InitMyButton("pictures/editor/ConnectionFlexibility.png", ConnectionFlexibility,1,&edit.DollHouse);
edit.MoveX=InitMyButton("pictures/editor/MoveX.png", MoveX,0,&edit.DollHouse);

edit.VoxToUp=InitMyButton("pictures/editor/voxToUp.png", VoxToUpeer,1,&edit.FullScreen);
edit.MgVoxToUp=InitMyButton("pictures/editor/MgVoxToUp.png", MgVoxToUpper,1,&edit.FullScreen);

edit.anim.AddExternal=InitMyButton("pictures/editor/AddExternal.png", AnimAddExternal,1,&edit.Puppetier);
edit.anim.AddLimb=InitMyButton("pictures/editor/LimbPick.png", AnimAddLimb,1,&edit.Puppetier);
edit.anim.Delete=InitMyButton("pictures/editor/LimbDelete.png",AnimDelete ,0,&edit.Puppetier);
edit.anim.Select=InitMyButton("pictures/editor/LimbSelect.png", AnimSelect,1,&edit.Puppetier);
edit.anim.ConnectToAnimation=InitMyButton("pictures/editor/LinkToAnimation.png", LinkToAnimation,1,&edit.Puppetier);
edit.anim.AllignAfter=InitMyButton("pictures/editor/AllignAfter.png", AllignAfter,0,&edit.Puppetier);
edit.anim.AllignBefore=InitMyButton("pictures/editor/AllignBefore.png", AllignBefore,0,&edit.Puppetier);
edit.anim.play=InitMyButton("pictures/editor/play.png", AnimPlay,0,&edit.Puppetier);
edit.anim.pause=InitMyButton("pictures/editor/pause.png", AnimPause,0,&edit.Puppetier);
edit.anim.Animate=InitMyButton("pictures/editor/Animate.png", AnimateTool,1,&edit.Puppetier);

edit.FullScreen.closed=edit.MegavoxMaker.closed=0;
edit.creationPiece=0;

ReadAllWindowsConfiguration();
}

LoadRagDoll(edit.CurDollGroup,edit.CurDoll);
LoadAnimation(0);
ConvertAnimationToUsable (&animEd.usableTestingAnimation);

if (!OpenGLscreen) SDL_WM_SetCaption( "Loading MEGAVOXELS", NULL );

AddPiece("Megavoxels/test/", 0,true);
AddPiece("Megavoxels/BasicGeometry/", 1,true);
AddPiece("Megavoxels/Destroyed/", 2,true);
AddPiece("Megavoxels/SmallBorders/", 3,true);
AddPiece("Megavoxels/Green/", 4,true);
AddPiece("Megavoxels/decorative/", 5,true);
AddPiece("Megavoxels/BrickWall/", 6,true);
AddPiece("Megavoxels/ThinWall/", 7,true);
AddPiece("Megavoxels/ZoomedTrees/", 8,true);
AddPiece("Megavoxels/Snow/", 9,true);
AddPiece("Megavoxels/SnowDecorative/", 10,true);
AddPiece("Megavoxels/Debris/", 11,true);
AddPiece("Megavoxels/FloatingParticles/", 12,true);
AddPiece("Megavoxels/GutsParticles/", 13,true);
AddPiece("Megavoxels/GagDollEnemies/", 14,true);
AddPiece("Megavoxels/Rocks/", 15,true);
int piececount=16;

for (int i=0; i<mgvpieces; i++){
	mgvDB.curType[i]=0;
	mgvDB.decorative[i]=0;//(i==5);
	for (int j=0; j<mgvpiecestypes; j++){
		mgvDB.mgvarray[i][j].initialised=0;
		mgvDB.mgvarray[i][j].Converted.count=0;
		mgvDB.locationText[i]=0;
	}
}

mgvDB.decorative[5]=1;
mgvDB.decorative[10]=1;

w[0].Megavoxel=&InGMGVPool[0];

for (int i=0; i<piececount; i++)
	for (int j=0; j<50; j++){
		Load(i, j);
		if ((mgvDB.mgvarray[i][j].Converted.count==0) && (mgvDB.mgvarray[i][j].initialised)){
			//Save(i, j);
		MegavoxMatrixTo8BitParticle(&mgvDB.mgvarray[i][j].Converted, i, j, 0);	
		SaveConvertedMGV(i, j);
		}
}

edit.CurDoll=0;
edit.CurDollGroup=0;

	edit.mgvPlay.drowable=0;
	mgvox.DrowDepth=0;
	for (int i=0; i<inGameMrgavoxPoolSize; i++) { InGMGVPool[i].InUse=0; InGMGVPool[i].paletteNo=0;}

	for (int i=0; i<DebrisListMax; i++) DebrisList[i].free=1;
	
//player.corporial=true;
MouseClick=&SingleShoot;
if (!OpenGLscreen) SDL_WM_SetCaption( "Setting lightning...", NULL );

//COOLtwansitionEffect();
if (wantmusic) Mix_PlayMusic( music, 0 );
Mix_VolumeMusic(MIX_MAX_VOLUME/4);
//thread = SDL_CreateThread( DynamicGroundLoading, NULL);
//float temprot=0.1;

sunlight.color=DayTimeLight;
sunlight.dinamic=0;
sunlight.distanceFading=0;
sunlight.angleLight=0;
sunlight.brightness=3-flashlight;
AddLight(&sun, &sunlight);
if (flashlight) AddLight(&player.host->pos, &player.flashLight);

SetUpsounds();
// cleaning up the events
if (hideMouse) SDL_ShowCursor(0);

if (inet.onlinemode==1)	
{	

for (int ii=0; ii<maxconnections; ii++){
	inet.serv.clientConected[ii]=0;
	inet.serv.clientTreadFree[ii]=1;
	inet.serv.ThreadNeedsCleanup[ii]=0;
	inet.entrymessage[ii]=0;
}

	inet.serv.Socket = StartServerListening(7700); 
    if (inet.serv.Socket == -1)
    {
		EndServer(inet.serv.Socket); 
		inet.serv.listening=false;
       
    }	
	else
	{
	inet.serv.LookForClient = SDL_CreateThread( serverfunk, NULL);  
	inet.serv.ThreadOut = SDL_CreateThread( Servoutflow, NULL); 
	}

}

ReadTyleFlags();
PreCreateShadowMasks();


PlayMySound(&sounds.bank.coolNoise[9]);
if (!OpenGLscreen) SDL_WM_SetCaption( "Decay v.030", NULL );

}
void COOLtwansitionEffect()
{
	return;
screencopy=SDL_DisplayFormatAlpha(screen);
//now we will drow everything once
for (int i=0;i<observ;i++)
{
	if (i==matrdepth)
		{
			ecvatordrow();
			waterdrow();
//wallsdrow(i);
		}
	wallsdrow(i);
}
screencopy2=SDL_DisplayFormatAlpha(screen);
SDL_BlitSurface(screencopy, NULL, screen, NULL);
//	SDL_BlitSurface(image[76], NULL, screen, NULL);
//	textlinenote( scrwi/2-200, scrhi/2-300, 45, "LOADING");
swapscreens();
	for (int i=255; i>-1; i-=2)
	{
	drowalphaborder (screencopy2, screen, image[77],i, 0, 0,0,16);
swapscreens();
	}
SDL_FreeSurface(screencopy);
SDL_FreeSurface(screencopy2);

//SDL_Delay(tyleSize);

}

void saveOnQuit()
{
	for (int ii=0; ii<5; ii++)
	for (int jj=0; jj<5; jj++)
		tyletofile (cameratylx+ii-2, cameratyly+jj-2);
		SaveTyleFlags();
	saveGame();
}

//GAME CYCLE:
void MainLoopEventHandling(){
	
while(SDL_PollEvent(&event)) 
		{
 if (WhatIsOn==editoron) EditorEventMGMT();

		if( event.type == SDL_QUIT ){ 
				quit = true;
				SDL_ShowCursor(1);
			} 

if (event.type == SDL_KEYDOWN ) 
if	(!qedit.WritingQuestDescription){
if ( keystates[ SDLK_ESCAPE ] ) JumpToSpace();
if ((event.key.keysym.sym == SDLK_e ) && (play)){
	if (!Idisplay.active)	{
		MouseClick=&ClickBrowseInventory;
		WhatIsOn=&bagison;
		SDL_ShowCursor(1);
		Idisplay.active=true;
	}	else	{
		MouseClick=&SingleShoot;
		WhatIsOn=&playison;
		if (hideMouse) SDL_ShowCursor(0);
		Idisplay.active=false;
	}
}else
	if ((event.key.keysym.sym == SDLK_h ) && (!Betatesting))
		edit.folliage=!edit.folliage;
	else
	if ((event.key.keysym.sym == SDLK_f ) && (!Betatesting) && (!Idisplay.active)) 
	if (play) {
			play=0;
			SDL_ShowCursor(1);
			MouseClick=&mouseeffect;
			WhatIsOn=&editoron;
			mouse.state=0;
		camerax=actcamx;
		cameray=actcamy;
	}
	else {
		play=1;
		if (hideMouse) SDL_ShowCursor(0);
		WhatIsOn=&playison;
		MouseClick=&SingleShoot;
	}


if (event.key.keysym.sym == SDLK_1 ) edit.scrolling=1;
if (event.key.keysym.sym == SDLK_2 ) edit.scrolling=2;
if (event.key.keysym.sym == SDLK_3 ) edit.scrolling=3;
}
else
	readMission();	

	

 if( event.type == SDL_MOUSEBUTTONDOWN ) 
 {
MouseClick(event.button.button);	

if (!play)
{
if( event.button.button == SDL_BUTTON_WHEELDOWN)
	if (edit.scrolling==1) {if (edit.ylev>-matrdepth*pixInPhis) edit.ylev-=pixInPhis;} else
		if (edit.scrolling==2) {edit.mlev+=3; if (edit.mlev>0) edit.mlev=0; edit.slev=max(edit.slev, (float)(edit.mlev+255)/255) ;  } else
			if (edit.scrolling==3) {edit.slev+=0.05; if (edit.slev>1) edit.slev=1;  }

if( event.button.button == SDL_BUTTON_WHEELUP)
	if (edit.scrolling==1)  {if (edit.ylev<matrhig*pixInPhis-11) edit.ylev+=pixInPhis;} else
		if (edit.scrolling==2) {edit.mlev-=3; if (edit.mlev<-254) edit.mlev=-254;    } else
			if (edit.scrolling==3) {edit.slev-=0.05; if (edit.slev<0.1) edit.slev=0.1; edit.mlev=min(edit.mlev, -255+255*edit.slev); }
}


mouse.buttonNo=event.button.button;

			if( event.button.button == SDL_BUTTON_LEFT )			{ 
				mouse.active=1;
				GetMouse(&mouse.pos.x, &mouse.pos.y);
				mouse.downx =mouse.pos.x;//= event.button.x;
				mouse.downy =mouse.pos.y;//= event.button.y;
				
				if ((!HUDuse) && (!draggingWindow) && (!edit.UsingTool)) mouse.state=1; 
				HUDuse=0;
			}

			if( event.button.button == SDL_BUTTON_RIGHT )			{
				mouse.active=1;
				if (!play)				{
					
					GetMouse(&mouse.pos.x, &mouse.pos.y);
					if ((!mouse.editorinterference) && (!HUDuse) &&  (edit.FullScreen.closed))					{	
					if (mergeimage[edit.editor])
							addobject(mouse.pos.x-mergeimage[edit.editor]->w/2,mouse.pos.y-mergeimage[edit.editor]->h/2,edit.editor);
					else
						if (image[edit.editor])
					addobject(mouse.pos.x-image[edit.editor]->w/2,mouse.pos.y-image[edit.editor]->h/2,edit.editor);	
					}
					HUDuse=0;
				}
			}
 }
 if (event.type == SDL_MOUSEMOTION  ) { 
				GetMouse(&mouse.pos.x, &mouse.pos.y);
			if ((mouse.state == 1) && (!play))			{
			actcamx = camerax + mouse.downx - mouse.pos.x;
			actcamy = cameray + mouse.downy - mouse.pos.y;
			}
		}
 	if (event.type == SDL_MOUSEBUTTONUP )
		if (event.button.button == SDL_BUTTON_LEFT ) 
		{ 
		mouse.state=0;
		camerax=actcamx;
		cameray=actcamy;
		}


if (( event.type ==SDL_JOYBUTTONDOWN) || (event.type ==SDL_JOYBUTTONUP))
    {
		bool state=0;
		if ( event.type ==SDL_JOYBUTTONDOWN) state=1;
		
		switch (event.jbutton.button)
		{
		case 0: MyJoystick.x=state; break; 
		case 1: MyJoystick.a=state; break; 
		case 2: MyJoystick.b=state; break; 
		case 3: MyJoystick.y=state; break; 
		case 4: MyJoystick.lb=state; break; 
		case 5: MyJoystick.rb=state; break; 
		case 6: MyJoystick.lt=state; break; 
		case 7: MyJoystick.rt=state; break; 
		case 8: MyJoystick.back=state; break; 
		case 9: MyJoystick.start=state; break; 
		case 10: MyJoystick.ls=state; break; 
		case 11: MyJoystick.rs=state; break; 
		}
    }


		if( event.type == SDL_JOYAXISMOTION )
			{
				if( event.jaxis.which == 0 )
				{ 
					if( event.jaxis.axis == 0 ) 
					{ 
						if( ( event.jaxis.value > -sens ) && ( event.jaxis.value < sens ) )
 { MyJoystick.lStickX=0; }
						else {
							MyJoystick.lStickX=event.jaxis.value;
						} 
					}


		 if( event.jaxis.axis == 1 ) 
					{ //If the Y axis is neutral
						if( ( event.jaxis.value > -sens ) && ( event.jaxis.value < sens ) )
						{ MyJoystick.lStickY=0;} 
						else { 
							MyJoystick.lStickY=event.jaxis.value;	
						} 
		 }

		

		 	if( event.jaxis.axis == 2 ) 
					{ 
							MyJoystick.rStickX=event.jaxis.value;
	
					}


		 if( event.jaxis.axis == 3 ) 
					{ //If the Y axis is neutral		
							MyJoystick.rStickY=event.jaxis.value;	
		 }

				} }

if (((MyJoystick.lStickX) || (MyJoystick.lStickY)) && (!mouse.active))
{
	mouse.active=0;
	double mpos=MyJoystick.rStickX*JoyAutoAimRadius;
		mpos/=joyStickMaxValue;
		mouse.pos.x=scrwid+scrwid*mpos;

		mpos=MyJoystick.rStickY*JoyAutoAimRadius;
		mpos/=joyStickMaxValue;
		mouse.pos.y=scrhid+scrhid*mpos;
}

}
}

void OriginalGameLoop(){


	
	LoadGround();
	PlayAmbient(&sounds.bank.ambientField, 6, -1);
	//if (!OpenGLscreen) SDL_WM_SetCaption( "Starting Cycle", NULL );

while( !quit ) 
{
	
//DynamicGroundLoading(0);

sun.tx=player.host->pos.tx-5;
sun.ty=player.host->pos.ty;
sun.x=player.host->pos.x;
sun.y=player.host->pos.y;

	
	
	mgmtSound();
mouse.prevx=mouse.pos.x;
mouse.prevy=mouse.pos.y;
CalculateCameraOffset();
if (!OpenGLscreen)
mouse.u8ms= SDL_GetMouseState(&mouse.pos.x, &mouse.pos.y);



if (!OpenGLscreen) MainLoopEventHandling();	
else if (keystates[ SDLK_e ]) {
	if (tera.ZOOMED) 
		Idisplay.active=false;
	else
		Idisplay.active=true;
	keystates[ SDLK_e ]=0;
}

zoomedInView();



SetWindToGlobal ();



RepointRenderingOfLight();

if (!tera.ZOOMED)
drowcaves();

shortp=player.host->pos.h>>16;
shortp/=pixInPhis;
heimod=player.host->pos.h*(player.host->pos.h<0);

if ((!tera.ZOOMED) && (player.host->pos.h<0))
	ecvatordrow();

// ****************************************************************************** CycleBegin
for (int i=0;i<observ;i++)
{
GlobalI=i;

	if ((i==matrdepth) && (!tera.ZOOMED))
		{
			if (player.host->pos.h>=0) ecvatordrow();
			dustphisics ();
			WblowPhisics ();
			RagdollDrowing();
			if ((!Betatesting) && (!edit.DollHouse.closed)) TestingLimbs();
			edit.mgvPlay.paletteNo=mgvox.curpiece;
			DrowIngameMegavox(&edit.mgvPlay, &edit.buildSite); 
		}

	enemiesAndAI (i);

if ((i==matrdepth) && (!tera.ZOOMED))
{
if	(player.host->pos.h>=0) waterdrow();
softblitting();
}

wallsdrow(i);

	if (shortp==i-matrdepth) playerdrow();


	nphisicsfucktion (phfirst[i], i); // recraft for zoomed
	
	if ((i==matrdepth) && (player.host->pos.h<0)) 
		{
drowstyle=&GLDrow;
sdldrowstyle=&nodrow;
	}

	if (!tera.ZOOMED){
	bloodstyle (i);
	PEphisics (PEfirst[i].next, i);
	}
}

if(!stereo)
{
drowstyle=&anydrow; 
sdldrowstyle=&sdlanydrow;
}
else
	drowstyle=&anydrow3d;

//DustAndLight(&QG_p[5][3], mouse.pos.x, mouse.pos.y, 128, screen, 0.3, smokeRotation*2, smokeRotation*2, smokeRotation*2);
if (!tera.ZOOMED){
AllComplexObjects();
darkenscreenIgnoreBakedShadow();//darkenscreen ();
linkedout();
}

voxTextureOffsetCounter++;
voxTextureOffset=voxTextureOffsetCounter;
if (voxTextureOffset>255) 
{
	voxTextureOffsetCounter=0;
	voxTextureOffset=0;
}

epointer=player.host;

WhatIsOn();

if ((!play) && (!tera.ZOOMED)) developerdata();

if (!tera.ZOOMED){
	clearLights();
	TuningList();
	for (int bl=0; bl<10; bl++)
		AddWindBlow(rand() % scrwi, rand() % scrhi);//mouse.pos.x, mouse.pos.y);//
}
//if (inet.onlinemode==1) drowvisiters(inet.serv.connections);
//if (inet.onlinemode==2) drowvisiters(inet.client.MaxGuests);


//VolumetricImageNLightBlitBorder(screen,imgBank.LightNingForLightDrow, mouse.pos.x-100, mouse.pos.y-100, temptest, 0);

//G8bitVexToLight(&gayVoxel[11][0], 1,1, smokeRotation*2, 0, smokeRotation, mouse.pos.x, mouse.pos.y);



swapscreens();


if ((Betatesting) && (!tera.ZOOMED)) temporaryBetaSpawning();

FramerateManagement();
UpdateFramerateInfo();

if (!tera.ZOOMED)
UpdateSmokeAngles();

 testCountEfects=0;
 testCountBlood=0;
 testCountEnemies=0;
 testCountPhisics=0;

if (inet.onlinemode==1) serverstuff(); 

if (inet.onlinemode==2) clientstuff();

// GAMECODE END
}


}

int OldMain(void *data){

	
//if (Betatesting) player.host->hp=100;
//planetselector ()
if ((player.NextGameState==1) && (player.corporial)) player.NextGameState=2; 
//if ((player.NextGameState<3) && (!quit)) LoadGround();
// **********************************************************************************************************************************************************************
// ************************************************MAIN PART*************************************************************************************************************
// **********************************************************************************************************************************************************************
if (OpenGLscreen) player.NextGameState=1;
thread = SDL_CreateThread( DynamicGroundLoading, NULL);

//player.NextGameState=7;

while (player.NextGameState)
switch (player.NextGameState)
{
case 1: {quit=player.NextGameState=0; OriginalGameLoop(); break;}//spiritualGameCycle(); break;}
case 2: {quit=player.NextGameState=0; OriginalGameLoop(); break;}
case 3: {quit=player.NextGameState=0; VoxelDesk();		  break;}
case 4: {quit=player.NextGameState=0; planetselector ();  break;}
case 5: {quit=player.NextGameState=0; HightDesk();		  break;}
case 6: {quit=player.NextGameState=0; MegaVoxelEditor();  break;} // change to MegavoxDesk when ready
//case 7: {quit=player.NextGameState=0; TerrainEditor ();   break;} 
//case 3: {quit=player.NextGameState=0; DecayBlitEngineGameCycle(); break;}
}



// **********************************************************************************************************************************************************************
// ************************************************AFTER GAME END*******************************************************************************************************
// **********************************************************************************************************************************************************************
if (WorldLoaded) saveOnQuit();

closegame=1;

//SDL_Delay(200);
if (inet.onlinemode==1) SDL_KillThread( thread );
if (inet.onlinemode==2) EndClient(inet.client.mySocket);   

if (!Betatesting) StoreAllWindowsConfiguration();

	Mix_CloseAudio();
	Mix_FreeChunk( low );
	SDL_FreeSurface( screen );
	SDL_JoystickClose( stick );
	SDL_Quit();
	return 0; 
}


int main( int argc, char* args[] ) 
{

if (OpenGLscreen) InitTestingGrounds( argc,  args); else InitOldSDLScreen( argc,  args);
initconditions ();

if (OpenGLscreen){
	ImageStreamer = SDL_CreateThread( OldMain, NULL);
	while (!quit){
		glutTimerFunc( 1000 / 60, runMainLoop, 0 );
		glutMainLoop();
		}
}else	
OldMain(0);


return 0;
}
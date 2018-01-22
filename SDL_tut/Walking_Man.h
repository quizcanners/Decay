void AnimateWalking(int offx, int offy)
{
	/// ********************************************************* Voxel animation simulation
//SDL_Rect qdest;

double aview=getAngle(mouse.x-scrwid, mouse.y-scrhid)+0.8; 
double adir=getAngle(player.force.speedx, player.force.speedy)+0.8;
//shake.spx+=
//double cleandir=adir;
double ah;
const int fpstep=30; //30
int offh;
int poffx;
int poffy;
double angx=0;
double angy=0;
playerW.CoefForMidStepAsTop=(fpstep-abs(playerW.legs.negPosStep));
playerW.CoefForMidStepAsTop/=fpstep;
//double tilt=0;
//bool armraise=0;

// TILTING To SIDE WHILE SHOOTING OR RUNING
//angx=(accurate(player.force.speedx))*0.2;
//angy=+(accurate(player.force.speedy))*0.2;

if ((keystates[ SDLK_LSHIFT]) && !(playerW.shooting ))
{ 


	playerW.armholddelay=fpstep*2;
	ah=adir; 
if (playerW.forAngleX) ah+=((getAngle(playerW.forAngleX, playerW.forAngleY)+0.8)-ah)*0.6; 
}
else {ah=aview; 
if (playerW.shooting) 
	ah+=0.8;    }

//RISE/lower arms while shooting;
if (playerW.shooting) 
	   playerW.armraise-=10; if (playerW.armraise<0) playerW.armraise=0;
	   else
	   {playerW.armraise++; if (playerW.armraise>100) playerW.armraise=100;}

double rottationspeed=0.09;
//Fixing angle
double teorAngl=playerW.angles.Ah-2*M_PI;
if (abs(teorAngl-ah)<abs(playerW.angles.Ah-ah)) playerW.angles.Ah=teorAngl;
teorAngl=playerW.angles.Ah+2*M_PI;
if (abs(teorAngl-ah)<abs(playerW.angles.Ah-ah)) playerW.angles.Ah=teorAngl;
playerW.angles.Ah+=((ah-playerW.angles.Ah)*rottationspeed);

int speed=ipifagor(accurate(abs(player.force.speedx)),accurate(abs(player.force.speedy)));

shake.x+=playerW.legs.negPosStep*speed*camerashkespeed;


//************************************************************drow Torso
 
//If player shooting take cool side position
double torsoangle=playerW.angles.Ah+stepsize*playerW.legs.negPosStep*(playerW.armraise+20)/100;
int torsox=offx;
int torsoy=offy;
offh=0;
//calculate angle between aim and torso
//double teorVang=aview;
double minangle=abs(aview-torsoangle);
minangle=min(abs(minangle-2*M_PI),minangle);
//if (teorVang<torsoangle) 
//end of that
int torsooff=-(55*(100-playerW.armraise)/100)*minangle/M_PI;
rotateOffXOffYh(0, 0,aview ,  &torsox,  &torsoy,torsooff, -torsooff/2,  &offh);
torsox+=playerW.torso.x+accurate(playerW.torso.disx);
torsoy+=playerW.torso.y+accurate(playerW.torso.disy);

int comeBackTime=10;
float comeBackForce=0.9;

playerW.torso.disx*=comeBackForce;
playerW.torso.disy*=comeBackForce;

playerW.torso.disx+=playerW.torso.spdisx;
playerW.torso.disy+=playerW.torso.spdisy;

playerW.torso.spdisx-=playerW.torso.disx/comeBackTime;
playerW.torso.spdisy-=playerW.torso.disy/comeBackTime;

//********************************************************************************* Now the Fucking Legs
poffx=offx;
poffy=offy;

// Tilt While moving and leg placement

if ( !keystates[ SDLK_SPACE ] )
{
		if (playerW.legs.rightLegUp)
		{playerW.legs.negPosStep+=speed; if (playerW.legs.negPosStep>fpstep) playerW.legs.rightLegUp=0;}
		else {playerW.legs.negPosStep-=speed; if (playerW.legs.negPosStep<-fpstep) 
		playerW.legs.rightLegUp=1;}
} 
else
{
	playerW.legs.negPosStep*=0.6;
			if (playerW.legs.rightLegUp==1)
			{
				playerW.legs.lleg.tx=player.pos.tx;
				playerW.legs.lleg.ty=player.pos.ty;
				playerW.legs.lleg.x=player.pos.x;
				playerW.legs.lleg.y=player.pos.y;
			}
			else
			{
				playerW.legs.rleg.tx=player.pos.tx;
				playerW.legs.rleg.ty=player.pos.ty;
				playerW.legs.rleg.x=player.pos.x;
				playerW.legs.rleg.y=player.pos.y;
			}
}
if (speed>0)
if (!playerW.legs.rightLegUp)
{
offh=-10;
poffx=offx;
poffy=offy;
//First find future coordinates of character, then find its leg with its direction, and move to side with angle fo moving direction
rotateOffXOffYh(0, 0, adir+M_PI/2,  &poffx,  &poffy, -20, -20,  &offh);
rotateOffXOffYh(0, 0, playerW.angles.Ah-stepsize*playerW.legs.negPosStep+0.8,  &poffx,  &poffy, -15, 0,  &offh);
//playerW.legs.lleg.tx=player.pos.tx;
putSameTyle(&playerW.legs.lleg, &player.pos);
//playerW.legs.lleg.ty=player.pos.ty;
playerW.legs.lleg.x+=((player.pos.x+(poffx-offx)*accuracy)-playerW.legs.lleg.x)/(fpstep*2)*abs(playerW.legs.negPosStep-fpstep);
playerW.legs.lleg.y+=((player.pos.y+(poffy-offy)*accuracy)-playerW.legs.lleg.y)/(fpstep*2)*abs(playerW.legs.negPosStep-fpstep);
}
else
{
offh=-10;
poffx=offx;
poffy=offy;
//First find future coordinates of character, then find its leg with its direction, and move to side with angle fo moving direction
rotateOffXOffYh(0, 0, adir+M_PI/2,  &poffx,  &poffy, -20, -20,  &offh);
rotateOffXOffYh(0, 0, playerW.angles.Ah-stepsize*playerW.legs.negPosStep+0.8,  &poffx,  &poffy, 15, 0,  &offh);
//playerW.legs.rleg.tx=player.pos.tx;
putSameTyle(&playerW.legs.rleg, &player.pos);
//playerW.legs.rleg.ty=player.pos.ty;
playerW.legs.rleg.x+=((player.pos.x+(poffx-offx)*accuracy)-playerW.legs.rleg.x)/(fpstep*2)*abs(playerW.legs.negPosStep+fpstep);
playerW.legs.rleg.y+=((player.pos.y+(poffy-offy)*accuracy)-playerW.legs.rleg.y)/(fpstep*2)*abs(playerW.legs.negPosStep+fpstep);
}

poffx= accurate(playerW.legs.lleg.x) - actcamx - cameratylx*tyleSize + playerW.legs.lleg.tx*tyleSize;
poffy= accurate(playerW.legs.lleg.y) - actcamy - cameratyly*tyleSize + playerW.legs.lleg.ty*tyleSize;
double footangle;

if (!playerW.legs.rightLegUp) 
{
//calculating leg rise

pseudiDDDdisplace (&poffx, &poffy, (player.high+feetRiseHeight*accuracy*playerW.CoefForMidStepAsTop));
//legrise
	footangle=torsoangle-1.8;
}
else 
{
elevation=player.high/dddstength;
poffx+=((poffx-scrwid)*elevation)>>16;
poffy+=((poffy-scrhid)*elevation)>>16;
	footangle=playerW.angles.Ah-1.8;
}
SuperFastVoxel(&G_p[3][2], poffx, poffy, -132,  screen, 0 , 0, footangle-0.8);

int legupx=torsox;
int legupy=torsoy;
offh=-10;
rotateOffXOffYh(0, 0, torsoangle,  &legupx,  &legupy, -10, -10,  &offh); // coordinates off upper part
legupy-=(legupy-poffy)/4;
legupx-=(legupx-poffx)/4;

angx=-(poffx-legupx)*0.03;
angy=(poffy-legupy)*0.03;

SuperFastVoxel(&G_p[3][5], legupx, legupy, -25,  screen, angx , angy, playerW.angles.Ah-1.8);
playerW.legs.legonex=poffx;
playerW.legs.legoney=poffy;
// ******************************Another leg

poffx= accurate(playerW.legs.rleg.x) - actcamx - cameratylx*tyleSize + playerW.legs.rleg.tx*tyleSize;
poffy= accurate(playerW.legs.rleg.y) - actcamy - cameratyly*tyleSize + playerW.legs.rleg.ty*tyleSize;
if (playerW.legs.rightLegUp) 
{
//calculating leg rise
elevation=(player.high+feetRiseHeight*accuracy*playerW.CoefForMidStepAsTop)/dddstength;
poffx+=((poffx-scrwid)*elevation)>>16;
poffy+=((poffy-scrhid)*elevation)>>16;
//legrise
	footangle=torsoangle-1.8;
}
else 
{
elevation=player.high/dddstength;
poffx+=((poffx-scrwid)*elevation)>>16;
poffy+=((poffy-scrhid)*elevation)>>16;
	footangle=playerW.angles.Ah-1.8;
}

SuperFastVoxel(&G_p[3][2], poffx, poffy, -132,  screen, 0 , 0, footangle);

legupx=torsox;
legupy=torsoy;
offh=-10;
rotateOffXOffYh(0, 0, torsoangle,  &legupx,  &legupy, 10, 10,  &offh); // coordinates off upper part
//rarmangle=getAngle(poffx-legupx, poffy-legupy)-0.8; 
//legupy=(legupy+poffy)/2;
//legupx=(legupx+poffx)/2;
legupy-=(legupy-poffy)/4;
legupx-=(legupx-poffx)/4;
angx=-(poffx-legupx)*0.03;
angy=(poffy-legupy)*0.03;
SuperFastVoxel(&G_p[3][5], legupx, legupy, -25,  screen, angx , angy, playerW.angles.Ah-1.8);

float percent=double(fpstep-abs(playerW.legs.negPosStep))/fpstep*0.4*playerW.armraise/100;


if (!playerW.legs.rightLegUp) 
{
playerW.torso.x=((torsox-playerW.torso.x)-playerW.legs.legonex)*percent;
playerW.torso.y=((torsoy-playerW.torso.y)-playerW.legs.legoney)*percent;
}
else
{
playerW.torso.x=((torsox-playerW.torso.x)-poffx)*percent;
playerW.torso.y=((torsoy-playerW.torso.y)-poffy)*percent;
}


//**************************************************************************************** LEGS OVER
CastShadowForDarkenfFunk(image[11],screen,xx-45,yy-50);


//************************************************************************************Drow Gun
poffx=offx;
poffy=offy;

/*
double minangle=abs(aview-torsoangle);
minangle

*/

double passivegun=(aview+(stepsize*playerW.legs.negPosStep - 0.8)*playerW.armraise/100);

teorAngl=playerW.gunpoint-2*M_PI;
if (abs(teorAngl-passivegun)<abs(playerW.gunpoint-passivegun)) playerW.gunpoint=teorAngl;
teorAngl=playerW.gunpoint+2*M_PI;
if (abs(teorAngl-passivegun)<abs(playerW.gunpoint-passivegun)) playerW.gunpoint=teorAngl;

playerW.gunpoint+=(passivegun-playerW.gunpoint)/40;
if (playerW.forAngleX) playerW.gunpoint=getAngle(playerW.forAngleX, playerW.forAngleY)+0.8; 
offh=0;
float pos=M_PI*playerW.armraise/600;
//playerW.legs negPosStep
int gundist=min(((-25+playerW.legs.negPosStep/10)*(100-playerW.armraise)/100),-17);

if (playerW.shooting) 
rotateOffXOffYh(0, 0, aview+M_PI/2,  &poffx,  &poffy, gundist, gundist,  &offh);
else
{
double gunangle=(aview*(100-playerW.armraise)/100)+(playerW.angles.Ah+stepsize*playerW.legs.negPosStep)*playerW.armraise/100+M_PI/2;
rotateOffXOffYh(0, 0,gunangle ,  &poffx,  &poffy, gundist, gundist,  &offh);  
}
//poffx+=shake.x;
//poffy+=shake.y;
int gunx=poffx;
int guny=poffy;

playerW.gunholex=(poffx-offx)*accuracy*1.3;
playerW.gunholey=(poffy-offy)*accuracy*1.3;
if (playerW.forAngleX)
addShootEffect(player.pos.x+playerW.gunholex,player.pos.y+playerW.gunholey,player.pos.tx,player.pos.ty);


playerW.forAngleX=0;
int rarmx=poffx; int rarmy=poffy;
rotateOffXOffYh(0, 0,playerW.gunpoint+M_PI/2 ,  &rarmx,  &rarmy, 10, 10,  &offh);

//	
//Drow upper right arm
int ruparx=torsox;
int rupary=torsoy;
offh=0;
rotateOffXOffYh(0, 0, torsoangle,  &ruparx,  &rupary, 14, 14,  &offh); // coordinates off upper part
double rarmangle=getAngle(rarmx-ruparx, rarmy-rupary)-0.8; 
SuperFastVoxel(&G_p[3][3], ruparx, rupary, 25,  screen, 0 , 0, rarmangle);

//Drow lower right arm
//int rdarx=poffx;
//int rdary=poffy;
offh=0;
rotateOffXOffYh(0, 0,rarmangle,  &ruparx,  &rupary, -5, -5,  &offh);

//rotateOffXOffYh(0, 0, torsoangle,  &rdarx,  &rdary, 14, 14,  &offh);
rarmangle=getAngle(ruparx-rarmx, rupary-rarmy); 
SuperFastVoxel(&G_p[3][4], rarmx, rarmy, 30,  screen, 0 , 0, rarmangle-0.8);

// ***************************************************************************    Left Arm
offh=0;
rarmx=poffx; rarmy=poffy;
rotateOffXOffYh(0, 0,playerW.gunpoint+M_PI/2 ,  &rarmx,  &rarmy, -2, -2,  &offh);
ruparx=torsox;
rupary=torsoy;
offh=0;
rotateOffXOffYh(0, 0, torsoangle,  &ruparx,  &rupary, -14, -14,  &offh); // coordinates off upper part
rarmangle=getAngle(rarmx-ruparx, rarmy-rupary)-0.8; 
//angx=20-(rarmx-ruparx)*0.03;
//angy=20+(rarmy-rupary)*0.03;

SuperFastVoxel(&G_p[3][3], ruparx, rupary, 25,  screen, 0 , 0, rarmangle);

offh=30;
rotateOffXOffYh(0, 0,rarmangle,  &ruparx,  &rupary, 5, 5,  &offh); // end part of upper arm part


int ToGunDist=pifagor (ruparx-rarmx, rupary-rarmy);


rarmangle=getAngle(ruparx-rarmx, rupary-rarmy);
//player.armholddelay=max(1,player.armholddelay-1);
//if ((ToGunDist<40) && (player.armholddelay<2))
SuperFastVoxel(&G_p[3][4], rarmx, rarmy, 30,  screen, 0 ,0, rarmangle-0.8);
/*else
{

if (ToGunDist<60) 
{
	/f (player.armholddelay<fpstep)
	{
	ruparx+=(rarmx-ruparx)*(60-ToGunDist)/20*(fpstep-player.armholddelay)/fpstep;
	rupary+=(rarmy-rupary)*(60-ToGunDist)/20*(fpstep-player.armholddelay)/fpstep;
	}	
}
else 
	player.armholddelay=fpstep*2;*/

//DrowVexNohtpBelly(&G_p[3][4], ruparx, rupary, 25,  screen, 0 , 0, rarmangle-0.8);
//}
listOfStuff[playerW.weaponUsing].inArmDrow(gunx, guny, pos);
//if (bag.LeftHand.count)

//+M_PI/2); 
//else
//SuperFastVoxel(&G_p[3][1], gunx, guny,55,  screen, pos , pos, playerW.gunpoint+M_PI/2);  // gun
SuperFastVoxel(&G_p[3][0], torsox, torsoy,10,  screen,0, 0, torsoangle);

playerW.shooting=0;
}
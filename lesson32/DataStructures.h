#define GL_UNSIGNED_INT_8_8_8_8_REV       0x8367
#define MGV_WORLD_OCCUPIED 255

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "SDL_rotozoom.h"
#include <sstream>
#include <fstream>
#include <ctime>
#include <winsock2.h>
#include <iostream>
#include <string>
#include "SDL_thread.h"
#include <math.h>
#include "freeglut.h" 
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>

bool OpenGLscreen=0;
bool Betatesting=1; 
bool onlineTesting=0;
bool LoadGameAssets=1;
int ZoomBGDisplacement=55; //0-no displacement
bool ZoomShadows=1;
bool VoxelShadows=1;
bool CNSTminigun=0;
const float GameSpeedModifier=2;
float GameSpeed=1.1;
double GameSpeedAdjustmentSpeed=0.005;//0.1;//portion every frame
bool capframerate= 1;
const int msPerFrame=17;
const float AnimationModifier=(double)10/((double)1000/(double)msPerFrame); 
bool frameIndependentSpeed=1;
bool flashlight=0;
bool WallGlobalIllumination=1;
bool LoadZoomed=1;
bool MultiplyUpperblit=1;
const int scrwi=1024;
const int scrhi=650;
int GLwi=1000;
int GLhi=700;
float GLWtoH=float( GLwi)/float(GLhi);
bool PixelatedScreen=0;

int GLtoCPUx=0, GLtoCPUy=0; // mouse position
int dddstength=512;  //change to >>7   //256
const float RagDollEditorUpsize=6;
// SDL to OpenGL pixel format
SDL_PixelFormat *SDLtoGLpixForm;

// Limits 
const int windBlowArSize=4096;//10000;
const int elim=50;//512;//2000; // Max amount of enemies
const int dustCount=4096;//12000;
const int phlim=2048;//5000;//10000; // Max amount of physical objects
const int bphlim=4096;//50000;// 50000;//50000;
const int wlim=100;//2000;
//Effects
const int PElim=512;//4000;//10000
const int numberOfRagLimbs=1024;

//#include <cmath>
//#include <PathFinder.h>
//**************** TODO:
//avarageAlpha for smokes
//correct feet placing
// different brightness for bullets. Make them more invisible
// constants to be replaced by char stats dependencies
// Have player cast bluish light
const int duIn9=512;
const int duIn10_1024=duIn9*2;
const int duIn11_2048=2048;
const int duIn12_4096=4096;
const int duIn13_8192=8192;
const int duIn14_16384=16384;
const int duIn15_32768=32768;
int testCountEfects=0;
int testCountBlood=0;
int testCountEnemies=0;
int testCountPhisics=0;

bool enableStereo=0;
bool sloMo=0;

double swingBacking=10.1;
const int rocketRadius=60;
const int MinigunRadius=30;
double swingSizeConst=-5.2;
double bladeRotationSpeed=0.86;
int bladeDamage=100;
int bladeRadius=18;
//stats dependencies end
int comeBackTime=10;
double comeBackForce=0.4;
int temptest=20;
double testx= 3.1, testy=1;//1.49;
int bladeSwingTimeDef=25;
int bladeSwingTime=35;
int swingCooldown=10;
int MaxBladeCombo=10;
int GutsRandForce=100;//250;
int heightLightOnSmokePreservation=4; // bigger for less preservation
bool splatPerFrame=1;//Dont spawn more then one splatter per frame
float slomoFADE=0.92;
int VoxelBloodRandosity=184;//7;
double VoxBloodGravity=0.56;
double VoxBloodUpDrop=1.3;
double VoxBloodSlowing=0.987;
const float bulletPierceSlowing=0.7;

const int E8bitBufferDiam=256;
const int E8bitBufferSize=E8bitBufferDiam*E8bitBufferDiam;
const int E8bitBufferRad=E8bitBufferDiam/2;

// light constants (888888 - vfx)


Uint32 CrambleRocksSmoke=0xAA9988;
Uint32 EnemyImpactColor=0x00FF0000;
Uint32 wallSmokeColor=0x00555555;
Uint32 RocketShootColor=0x00BBBBBB;
Uint32 PostExplosionSmoke=0x446666;
Uint32 RocketTrackSmoke=0x00AAAAAA;//0x2a2e24;
Uint32 UpGoingSmoke=0x00AABB66;
Uint32 PhisObjectsDefault=0x00222222;
Uint32 UpgoingSmokeColor=0x00666666;
Uint32 BoomLight=0x00FFEE99;
Uint32 BoomDroppingsLight=0x00FFFFBB;
Uint32 ForColorMultiplyDefColor=0x00222222;
Uint32 DayTimeLight=0x00BBBBBB-0x00222222*flashlight;//0x00444444;
Uint32 PlayerFlashLightColor=(OpenGLscreen) ? 0x00FFAA55 : 0x0055AAFF;
Uint32 FlameDarkToungsColor=0x00010000;
Uint32 fireLightColor=0x00FFDD99;
Uint32 DebrisDefLight=0x00999999;
Uint32 ExplosionSmokeColor=0x00888888;//666666;

//dust const
int fireDustHspeed=150000; //*1000

const int VoxUpResize=100; //(divide by)
//light sources

//ang fades
float defAngleFade=0.84-0.4*flashlight;//0.93; - day
const float smokeAngleFade=0.89;//0.979; - day
 float BulletBrightnes=2.5; 
 int FireBrightnes=3;
 float gunPointBrightness=4;
 double UpgoingSmokeSlowingRisePerFrame=0.0008;
 double UpgoingAngularFadechange=0.000003;

 //distance fades
 double distanceFadingRiseOnDrop=0.0047;
double distanceFadingRisePerFrame=0.00065;
double BoomDistanceFade=0.0017;
double fireDistFade=0.0083;




//light blicks
 float minigunLightIntensity=1.858;
const float RifleBulletBrightness=0.746;
const float fireBrightness=0.7;
const float flameBrightness=0.55;//0.3;




//smoke

int ExplosionoriginHeightRaise=11;
int defaultMaxLight=128;
int smokeMaxLight=64;
int HeightDrowChance=4;
// EXPLOSION effect CONSTANTS
int chanceOfSmokyParticleDrop=4;//less for more
int healthLossDrop=51;


// Displacements
int BoomDisplacementHealth=40;


const int DollGroups=1;
const int DollsInGroup=10;
const int maxLimbs=32; 

const double autoaimRad=20;
const int fireTrowRandosityOnExplosion=700;
int mousesmoothing=5;
int mouseToCanGLsmoothing=1;
const float JoyAutoAimRadius=0.3; //screen percantage;
const int joyStickMaxValue=32767;
const int tyleSize=duIn10_1024  ;
const int phArSize=128;
const int pixInPhis=tyleSize/phArSize;
const int accuracy=65536;
const int coorToTyle=accuracy*tyleSize;
const int RedMask=0xFF0000;
const int GreenMask=0x00FF00;
const int BlueMask=0x0000FF;
const Uint32 AlphaMask=0xFF000000;

const Uint32 deAlphaMask=0x00FFFFFF;
const Uint32 deRedMask=  0xFF00FFFF;
const Uint32 deGreenMask=0xFFFF00FF;
const Uint32 deBlueMask= 0xFFFFFF00;
const Uint32 shadowMax=160+flashlight*60;//210;//129;
const int shadowDivPoint=255-(256-shadowMax)*2;
int ShadowMin=0+flashlight*146;// 170;//64
double impactShake=0.000001;
double impactslomo=0;
int phisLinkConnectionStrength=1500; //max distance they can part
int phisConnectionDeStretchForce=570;
int phisConnectionDeSquizeForce=570;
int GibsHealth=52;
double flameLightStability=0.02;
double fireFromCenterBlowForce=0.018;
int fireDirectionChangeDiv=3000;
double upgoingSmokeSlowingChange=0.0035;

// mECHANICS CONSTANTS
int onImpactHealthLoss=3;//phisics


using namespace std;
 // Change to switch to playable mode
int monstersToKill=500;//duIn10_1024;

bool betaExitOnDeath=1;
bool wantmusic=0;
bool hideMouse=0;
bool soundEffects=0;
int betaSpawnTimer=0;

int betaSpawnDelay=50;
const bool gotofullscreenbeta=0;

bool MaterialsLoaded=0;

bool stereo=0;
const int huds=30;// number of HUD images
SDL_Event event;
const int images=1550;
const int matrhig=10;
const int matrdepth = 4; 
int observ=matrhig+matrdepth;
bool WorldLoaded=false; 
//bool pooledEvent;
Uint32 * __restrict darkmask;
bool * darkmaskGrid;
const int scrwi8=scrwi/8;
const int scrhi8=scrhi/8;
struct dddcoords
{
	int x, y, h;
};
struct dddcoordsDouble
{
	double x, y, h;
};
const int windDynamic=50000;
struct windStructure
{
dddcoords * __restrict mask;
int w,h;
int smoothness;
int treeBendDelay;
int maxBend;//afterThisValue it bends only by square root
double treeslowing;
dddcoords global;
dddcoords globalAccel;
float ang;
float intensity;
} wind;

struct DirectionalLightetImage
{
	Uint8* DnA;// direction and alpha;
	int h, w;
	std::string location;
}; 

int * __restrict darkIndexStart;
int * __restrict darkIndexEnd;
//bool * boolMask;
//bool darkTurn=false;
 // To BLIT ALL the stuff to

SDL_Surface* __restrict screen = NULL;
SDL_Surface* __restrict screencopy = NULL; // To BLIT ALL the stuff to
SDL_Surface* __restrict screencopy2 = NULL; // To BLIT ALL the stuff to
SDL_Surface* __restrict temp = NULL; // To adapt image files
Uint16* zoomHscreen; 
SDL_Surface* ZoomBG;
SDL_Surface* ZoomBGUp;
Uint8* screenShadow;
float * ZoomDisplacementMaskX;
float * ZoomDisplacementMaskY;
SDL_Surface* image [images]; // Background and decor images
//Indecsation for enemies
// dont forget to +1 for lineup
SDL_Surface* shootAnim[18];
SDL_Surface* shootMASK[16];
SDL_Surface* flameMask[7];
SDL_Surface* fireGlobImg;
SDL_Surface* flameLight[6];
SDL_Surface* fireSmokeMask[5];
SDL_Surface* flameDecay;
SDL_Surface* shootDecay;

struct MyIndexedAlphaSurface 
{
	// No need for storing transparent pixels any more
	Uint8* pixels;
	int w,h;
	int pixCount;
	std::string location;
};


struct ImageDataBank
{
	SDL_Surface* bulletWallFuck;
	SDL_Surface* ScreenBlicSmoke;
	SDL_Surface* OnImpactGoreHeight;
	SDL_Surface* DefaultPhisicalBlur;
	MyIndexedAlphaSurface voxelBlood[5];
	MyIndexedAlphaSurface displacementGhost;
	MyIndexedAlphaSurface DefaultShadow;
	MyIndexedAlphaSurface BGborderLeft, BGborderRight,BGborderTop, BGborderBottom;
	SDL_Surface* LightNingForLightDrow;
	struct DevelopInterfaceIMGs
	{
		SDL_Surface* MegavoxDesk;
	} devITF;

	struct decayBackgroundsFullScreen
	{
		SDL_Surface* snowBG;
		SDL_Surface* oldBG;
		SDL_Surface* OverGuts;
		SDL_Surface* LeavesZoom;
		SDL_Surface* Hex;
		SDL_Surface* HexZoom;
		SDL_Surface* ClassicBG;
		SDL_Surface* web;
		SDL_Surface* grassZOOM;
		SDL_Surface* CrambledBGUpeer;
		SDL_Surface* PixelationMask;
	} decay1024;
}imgBank;

DirectionalLightetImage dLISmoke[5];
DirectionalLightetImage OverlapUp[5];
DirectionalLightetImage OverlapUpFlame[4];
DirectionalLightetImage OverlapDown;
DirectionalLightetImage RealAnimatedSmoke;
SDL_Surface* PseudoAnim[9];
SDL_Surface* PseudoAnimBig[9];
int DecayGutsMask=0;
SDL_Surface* DecayMaskForGuts[6];
Uint8 BufferAnglesCopy[E8bitBufferSize]; 

SDL_Surface* wallFmask[2];

SDL_Surface* mergeimage [images]; // Merge part to place under image
SDL_Surface* mergeimaged [images]; // Merge part to place under image decayed
SDL_Surface* decayBG[images];
SDL_Surface* SnowydecayBG[images];
SDL_Surface* dhDecay[images]; // Decay surface to be drown during DH instead of upper one.
SDL_Surface* mergeimagem [images]; // This will be 8 bit mask
SDL_Surface* heightimage [images]; // Mask, which tell's, how hight image pixel is
SDL_Surface* dhimage[images]; // to remove height
SDL_Surface* overlayimage[images]; // The ones to be drown on top of ground layers (-100)
SDL_Surface* shadowimage[images]; // shadow
SDL_Surface* displacement[images]; 
SDL_Surface* uppermerge[images];  

SDL_Surface* basicDisplace; 

struct LRSurfaceInd
{
bool indexed;
bool arraySet;
int * IMGindexStart;
int * IMGindexEnd;
} ImageIndexation[images];

bool closegame=0;
bool picturesloaded=0;
bool overlaylap[200]; // 1 if images can't crossover
// ************************************* HUD
SDL_Surface* hudd [huds]; // HUD images
int hudstate[huds], hudx[huds], hudy[huds];
SDL_Surface *message = NULL;
// stored map and game map array ****************************************** MATRIX
int mapmatrix[5][5][phArSize][phArSize][matrdepth+matrhig+1]; // 65,535
SDL_Surface* __restrict storedmp[6][6];
SDL_Surface* __restrict mainmaps[6][6];
SDL_Surface* __restrict uppermp[6][6];
//SDL_Surface* __restrict shadowmaps[6][6];



struct UpShadLayer
{
Uint8 pix[1024*1024];
Uint8 Rborder[1024];
Uint8 Distance[1024];
}shadUp[6][6];

struct StartAndEnd
{
	short start;
	short end;
};

struct IndexingForWaterAndCaves
{
bool upToDate;
StartAndEnd line [5][5][tyleSize];
}bgIndex;
	//bool grid.redrowing[6][6];

const int tyleBGFlagsSize=128;
struct GridInfo
{
static const Uint32 BGnumberMask =0x000000FF; 
static const Uint32 BGWaterMask  =0x00000100;
static const Uint32 BGLavaMask   =0x00000200;
bool redrowing;
bool isshadow[6][6];
bool iscaved[6][6];
bool water[6][6];
bool snow[6][6];
LRSurfaceInd LavaIndex[6][6];
int objectx[5][5];
int objecty[5][5]; 
Uint32 TyleInfo[tyleBGFlagsSize][tyleBGFlagsSize];

} grid;


// ////////////////////////////////////////////////////////////////AUDIO
Mix_Music *music = NULL;
Mix_Chunk *low = NULL;



//Font
TTF_Font *font = NULL;
SDL_Color textColor = { 255, 255, 255 };
std::string Inputstr;
SDL_Surface *Inputtext=NULL;
//int editor=4;
int HUDuse=0;
SDL_Joystick *stick = NULL;

//const int l=30;  // Max amount of objects on the tile before program starts to draw tiles ti one .png 

const int sens= duIn12_4096;  //5000; //Joystick dead zone (less, smaller zone default - 8000)
//int scrwi, scrhi;
 // Accuracy of physics calculations
const Uint32 einmatr=10000; // + i to write enemy in matrix
const Uint32 phinmatr=20000; // + i to write enemy in matrix
const Uint32 limbinmatr=30000;
const Uint32 voxInEditor=3000;
const int physickaldamage=1; // at what speed physical objects do damage
const int savephisicstotyle=0; // if I want to save blood on lyle

int alpha = SDL_ALPHA_OPAQUE;
	int camerax=0, cameray=0, actcamx=0, actcamy=0, cameratylx=0, cameratyly=0;
const int unitsIntyle=tyleSize*accuracy;//tyleSize*accuracy


	int GlobalI=0;
	int play=1;
// Camera vibration
int camxoffset=0, camyoffset=0, camhaccel=0, camstabil=50;

int gravity=25; int speedoftime=300;// The more the slower
int redirect=0;

struct worldcoordinates
{
	int tx, ty, x, y, h;
};

struct offsets
{
	int x, y;
};

const int cameraFollowSpeed=8;
struct spilberg
{ 
	int x, y, spx, spy;
	double antishake;
	double antishakeCoolUp;
	double shaker;//reset at shake;
	double shakerCoolDown;
	worldcoordinates cameraPrevious;
	offsets cameraDisplacementPerFrame;
	//double returner; // start at zero and rize;
} shake;
	
struct spawnStats 
{
	int max;
	float chance;
};

struct RandomAreaGeneratorData
{
int rx, ry;	
spawnStats brykivka; 
spawnStats mcrock;
spawnStats voxelTree;
spawnStats voxelRocks;
spawnStats water;
} world;

struct ObjectBlitMethodData
{
int type, tempage; 
worldcoordinates pos;
bool tempfall;
float temps;
double globx, globy, glob;
} mrg;

bool quit = false; 
	// objects
	
	
	
	
	
	//objects end
	int  gridx, gridy, gamestate=-1;
	int offx, offy, preoffx, preoffy;
	double doffx, doffy;
	//Mouse
	struct rat
	{
		
		offsets pos;
		int downx, downy, state, editorinterference, selectedinarray, siah, prevx, prevy, buttonNo;
	bool active; //becomes active on Button press to snap out of joystick; 
		Uint8 u8ms;
		bool states[10];
	}mouse;
	struct GamingPad
	{
		int lStickX, lStickY, rStickX, rStickY;
		bool lb,lt, rb, rt, a,b,y,x, ls, rs, cl,cr,cu,cd, back, start;
	} MyJoystick;
	// ********************************************************************Char Data structures:
struct xyhCoordinates
{
	int x,y,h;
};

struct offsetcoords
{
	int x,y, disx, disy, spdisx, spdisy;
};

struct phisicalMe
{
	int speedx, speedy, speedh;
	float mass;
	float pr;
};

struct limbToBody
{
	int x, y, z;
	int speedx, speedy, speedh;
};

const int feetRiseHeight=5;
const float stepsize=0.01;
struct FeetPlacing
{
bool rightLegUp;
float negPosStep;
worldcoordinates rleg, lleg;
int legonex, legoney;
};

struct rotatableMe
{
float Ah, Ax, Ay, aSpX, aSpY, aSpH, size;
};

struct castData
	{
		bool inHand; // curently holding blade
		bool activated;
		int frame;
		int poseffectsCharge; // like dripping blood
		int combo;
		char swingDirection; // 1 or -1
	//	bool hitSomebody;
	};

struct Item
{
int type;
int count;
float cooldown;
float overheating; // aim
int ammo;
bool passiveCooldown; // no need to be active in hand to cooldown
};

struct objectLight
{
Uint32 color; 
float angleLight;
float angleFading;
float distanceFading;
float brightness;
bool dinamic;
bool active;
};

struct walkingHero
{
castData blade;
phisicalMe forceAccel;
Item* itemInHand;
float gunpoint;//angleh, anglex, angley, 
float CoefForMidStepAsTop;
float gunBarelRotation;
float armraise, lastStepAngle;
int weaponUsing;
int weaponPassive;
offsetcoords torso;
FeetPlacing legs;

//LIMB TO BODY
//limbToBody larm, rarm, 
rotatableMe angles;
int forAngleX, forAngleY, gunholex, gunholey;
int armholddelay;
bool shooting;
bool jumping; 
bool runing;
};

//struct ActiveRagdollHuman
//{
//	worldcoordinates torso, leftArm, rightArm, leftFoot, rightFoot, 
//};	
	int shortp;

	const int guns=5;
int displ=0; // used to set the displacement of a bullet from gun
	struct weapon
	{
		double delay; 
		float aimup;
		float aimdown;
		float feedback;
		float mass;
		float aimlim;
		float shootspeed;
		float recoilAftershootPercent;//
		float gunHoleDist; // 1.3 for rifle
	} gunstats[guns+1];


	// movement
	int shootingslowing=0;
	int buttonspressed=0;
	// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>GUNS
	int gun=5;
	float aim=0;
	
	
	int inventory[guns];
	int ammo[guns]; 
	float nextshoot=0;
	Uint8 *keystates = SDL_GetKeyState( NULL );
	bool GluKeys[256];
	// gun sounds
	Mix_Chunk *gunboom[guns];
	 // hero movement 
	const int paccel = accuracy;
	int topspeed = accuracy*6;
	int camxaccel=0;
	int camyaccel=0;
	// Productivity trigger
	int repaintmerge=1; // If 1 - redrows the ground with every new object on it



	// *****************************************************************************STRUCTURES************************************************
	struct WeaponRecoil
{
	double spx, spy;
	double offx, offy;
	double cooldownPercent;
	double coolx, cooly;

};
struct someBag
{
Item LeftHand, RightHand;
Item pack[8][8];
WeaponRecoil recoil;

int veight;
};


	//***********************************************************Enemies and player 
int blur=2;
int getione;
int getitwo;

// Original walls

SDL_Surface* wall[200];
SDL_Surface* InnerWall[200];

int wallh[200];
int wallsolid[200];
int wallparticle[200];

struct G8_Particle
{
Uint32 * palette;
Uint32 * pixels; 
	//Use phisical object as basis for G_Particle
int radius; // To use function without border check
int count;
Uint32 truecount; 
//SDL_Surface * palette;
};



	// ***************************************************************************              Phisical objects
	struct part
	{
 worldcoordinates pos;
 phisicalMe force;
 	objectLight light;
	rotatableMe angles;
	G8_Particle* g8particle; 
	int type, solidness, power, data, i, starthp;
	double hp;
	int casterDNA; // number in array of a guy who created object
	int G_p, G_c;
	Uint32* g;
	SDL_Surface* voxelPalette;
	part * next;
	part * prev;
	int (*foo) ();
	int (*defaultfoo) ();
	int (*collide) ();
	int (*defaultCollide) ();
	void (*aftermove) ();
	void (*finalDrow) ();

	float lightIntensity;
	float displayDelay;
	float clock,slowing;
	bool mortal;
	bool BlitOnDeath;
	bool lightTrack;
	bool voxelPaletteCopy; // if true - free surface
	bool skip;
	} ph[phlim]; 
	int drops;
	part *curphisobj;		

part* phfirst[matrhig+matrdepth];
part* phlast[matrhig+matrdepth];

int (*foophi) ()=NULL;
void (*setpixelstyle) (SDL_Surface *screen, int x, int y,int h, Uint32 colour) = NULL;
int (*beinsideobjectp) () = NULL;
int (*beinginsideenemyp) ()= NULL;
int (*beinginsideph) ()= NULL;

int curph=22;
bool phHorizontal;
// ////////////////////////////////////////////////////////////////// G-PARTICLES AND VOXELS

struct FaToBconversion {
float fsz, fcz, fsx, fcx, fsy, fcy;
float fxtox, fxtoy, fxtoh,
		fytox, fytoy, fytoh,
		fhtox, fhtoy, fhtoh;
};
FaToBconversion NtoN, TNtoN, T2NtoN;
// float fsz, fcz, fsx, fcx, fsy, fcy;
  //float tfsz, tfcz, tfsx, tfcx, tfsy, tfcy;

/*float fxtox, fxtoy, fxtoh,
	fytox, fytoy, fytoh,
	fhtox, fhtoy, fhtoh;
float tfxtox, tfxtoy, tfxtoh,
	tfytox, tfytoy, tfytoh,
	tfhtox, tfhtoy, tfhtoh;
float pfxtox, pfxtoy, pfxtoh,
	pfytox, pfytoy, pfytoh,
	pfhtox, pfhtoy, pfhtoh;*/

const int VoxelsCount=20;
const int CreationsCount=20;
//const float RotationSpeed=0.01;


const int maxvSize=100;
int vSize=10;
int pixsize=4;
int curh=vSize/2;
int curVoxel=0;

int curCreation=0;

struct Gpixel
{
Uint32 color;
Uint32 TextureImagePixel;
Uint32 xyhPI;
};

struct G_Particle
{
Gpixel * gpixels; 
	//Use phisical object as basis for G_Particle
int radius; // To use function without border check
int count;

};

struct QG_Particle
{
Uint32 * pixels; 
	//Use phisical object as basis for G_Particle
int radius; // To use function without border check
int count;
//SDL_Surface * palette;
};
SDL_Surface* QGpalette[CreationsCount];



struct voxelSegment
{
xyhCoordinates massCenter;
int x,y,h; // as part of structure
int size;
float ah, ax, ay;
bool active;
bool toBeIncluded[maxvSize][maxvSize][maxvSize];
Uint32 VoxelMatrix[maxvSize][maxvSize][maxvSize];
Uint32 VoxelMatrixTP[maxvSize][maxvSize][maxvSize];
Uint8 eightColMatr[maxvSize][maxvSize][maxvSize];
Uint32 eightPalette[9];
G_Particle G_p;
} vox[VoxelsCount];

G_Particle G_p[CreationsCount][VoxelsCount]; 
QG_Particle QG_p[CreationsCount][VoxelsCount]; 
SDL_Surface * vexelAura[CreationsCount][VoxelsCount];	
G_Particle* imageVoxel[images]; 
G8_Particle gayVoxel[CreationsCount][VoxelsCount];
double glob=0, globxh=0, globyh=0;



struct PaletteEditor
{
	dddcoordsDouble rotVec;
	dddcoordsDouble rotVecDef;
SDL_Surface * image;
SDL_Surface * coordinatesText;
SDL_Surface* imageForDirectionalLight; 
int toolInHand;
int palettex, palettey;
Uint32 * currentPixel;
Uint32 * currentPixelTP;
Uint8 * currentG8pixel;
Gpixel * currentGpixel;
G8_Particle * currentG8;
int G8BufferRadius;
int ThreadG8BufferRadius;
Uint32 Buffer8bit[E8bitBufferSize];
Uint32 ThreadBuffer8bit[E8bitBufferSize];
Uint16 SmokeBuffer8bit[E8bitBufferSize];
int currentH;
bool rebuildGpixel;
int curColor;
int BrushPlaneH;
//Uint32 color[9];
Uint8 R;
Uint8 G;
Uint8 B;
Uint8 A;
int brushSize;
int zoom;
} palette;

// lights to highlight the object in editor
objectLight mouseLight, li1, li2, li3;
worldcoordinates mousebody, l1bod, l2bod, l3bod;
worldcoordinates testbody;

// ********************************************************* Blood phisics
int VoxBloodDistanceDiv=3000;
struct blood
	{
	int type;
	worldcoordinates pos;
	int clock, speedy, speedh, speedx, flooidness;
	blood * next;
	blood * prev;
	bool lightParticle;
	//unsigned char pr;
	} bl[bphlim];

	blood *blpoint;
	blood bfirst[matrhig+matrdepth];
	blood blast[matrhig+matrdepth];

// Collection of objects to be blitted to one buffer



	int bphcache=0;
	int bloodness=20;//10 // Amount of blood 
	double vbloodness=0.3;
	int maxbloodness=40;//20;
	int camerashkespeed=610;
	int voxTextureOffset=0;
	int voxTextureOffsetCounter=0;

bool noBloodVacancyThisTurn=0;

struct DataTransferer
{
int offx, offy;
int tx, ty, x, y, h, hp, type, xspeed, yspeed, hspeed, xaccel, yaccel, randosity, numberof, flooidness, mask;
//double size; // Used to set size of Voxel wall;
int decaySurfaceNo;
int G_c, G_p;
G8_Particle* G8t;
int MaxLight;
float slowing, gravity;
int displayTime;
double anglex, angley, angleh, rotationx, rotationy, rotationh, size;
bool randomGenerated;
bool refreshingTyle;
bool returnHolder;
bool blitToTyle;
bool staticPos;
bool snowTyle;
bool lightPisObj;
float lightIntensity;
objectLight light;
Uint32* g;
SDL_Surface* maskIMG;
SDL_Rect animationImageParams;
bool (*boolFunction) ();
void (*voidfunction) ();
} createData;

int forDHrx[4],forDHry[4];//  world.rx
int jhx, jhy, jhh, jspx, jspy, jsph;
int slowwing=0, ja, ji=0, drtype, adrtype, blura=accuracy*2, blurb=accuracy*4,  matr, damage, slowing=96, randy=25, bodythick=10;
	// *************************************************************************************************Linked phisx
	struct LP 
{
    int ione, itwo, dist,force; // forst and second objects
    LP * next;
	bool broken;
};
	LP * linkhead = 0; 

//LP * del;
//LP * fuckk=linkhead;
//LP * lnq = linkhead;


	// ****************************************************** Editor
const int itfcH=20; // width of the editor window
int segmSize=20;
const int toolhands=10;
struct EditorInterface
{
short x, y;
short w, h;
SDL_Surface* text;
bool closed;
bool dragged;
bool autoclose;
short dragx, dragy;
void (*functional) (EditorInterface* caller);
bool (*eventFuctional) (EditorInterface* caller);
void (*mouseTool[toolhands])();
bool toolON[toolhands];
int toolSizeAll;
short toolColor[toolhands];
};
struct MyButton
{
SDL_Surface* icon;
bool active;
int x,y;
void (*functional) ();
int type; //0-press; 1-equip ; 2-full screen tool
EditorInterface* assignTo;
};
//*******************************************************************8 MEGAVOXELS
const int mgvpieces=20;
const int mgvpiecestypes=50;

const int surRotMask[9]={ 2, 4, 6,
				   -2, 0, 2,
				   -6,-4,-2}; 

struct SurroundArray
{
int p[27];
};

int CubeArOne[27];
int CubeArTwo[27];

struct MegavoxSegment
{
G8_Particle Converted;
int landing[27];
Uint16 * matrix;//[24*24*24];
Uint16 * mgvoxmatrix;
Uint16 * PieceNtypeNang;
Uint16 MyPieceNDtype;
bool initialised;
bool symetrical; // check if any rotation is nessesary;

};
const int inGameMrgavoxPoolSize=600;
struct IngameMegavox
{//change all this to 8 8 8 
MegavoxSegment* drowType[8*8*8];
char piece[8*8*8];
char angle[8*8*8];
int hp[8*8*8]; // if piece is 0, hp could store distance to next vox 
int paletteNo;
bool NeedUpdate;
bool drowable;
bool InUse;
} InGMGVPool[inGameMrgavoxPoolSize];

struct MegaVoxelDataBase
{
MegavoxSegment mgvarray[mgvpieces][mgvpiecestypes];
std::string location[mgvpieces];
SDL_Surface* locationText[mgvpieces];
Uint32 palette[mgvpieces][8];
Uint32 *ConvertedPalette[mgvpieces];
bool exist[mgvpieces];
bool decorative[mgvpieces];
bool LoadUnconverted[mgvpieces];
int curType[mgvpieces];
} mgvDB;

struct MegavoxEditor
{
EditorInterface segmenter;
EditorInterface palette;
EditorInterface tools;
EditorInterface pieceSelector;
EditorInterface editMegavox;
EditorInterface segmentLanding;
EditorInterface creationViewer;
MyButton bttnLoad;
MyButton bttnSave;
MyButton drowTool;
MyButton eraserTool;
MyButton noRenderTool;
MyButton ToolSize;
MyButton NextType;
MyButton PrevType;
MyButton NextMgvoxType, PrevMgvoxType, NextMgvoxPiece, PrevMgvoxPiece;
MyButton bttnLoadMgvox;
MyButton bttnSaveMgvox;
MyButton pickvox, eraseVox, BrakeVox;
MyButton landany, landfill, landempty, fillFullMegavox;
MyButton NextPiece, PrevPiece;
MyButton Copy, paste;
MegavoxSegment * curSegment;
MegavoxSegment * curMgvoxSegment;
EditorInterface* windowManageQ[20];
int windmanQLastIn;
MyButton* buttonManageQ[50];
int buttManQLastIn;
int curpiece;
int curpiecetype;
int curMgvoxpiece;
int curMgvoxpiecetype;
int h;
int curcolor;
int LastUsedTool;
int landingH;
int MgvoxH;
int DrowDepth;

MegavoxSegment* copySegment;
}mgvox;

struct DamageAfterdrowStorer
{
	int Maxi;
	MegavoxSegment* Applicant;
	int x, y;
} brakageQ[500];
int brakeQMax=0;
bool draggingWindow=0;

struct MenuScrollQ
{
	EditorInterface* window;
	int* value;
	int maxi;
	int min;
} scrollQ[20];
int scrollMax=0;

int ApplicCount=0;
int maxCoal;
int maxI[10];
MegavoxSegment* ApplicantSegm[10];

	int poweroflinks=1000;
	long tanint;
int hashh;
float calcforcex,calcforcey,calcforceh;
	int lays=1; // Number of decor images loaded
	int curlay=1; // laythat is used at the moment;
//************************************************************************************************ height
	struct EditorParameters
	{
	int scrolling; // 1-ylev, 2-mlev, 3-slev
	int ylev; // Camera rise above y level
	int mlev; // elevation of decorative objects above ground
	float slev;
	int editor;
	bool folliage;
	int TuningParameter;
	EditorInterface MegavoxMaker, FullScreen, DollHouse, Puppetier; 
	MegavoxSegment Megavox;
	int mgvlev;
	Uint32 MGVpalette[8];
    Uint32 *ConvertedPalette;
	MyButton AddVox, RemoveVox, BrakeVox, PlaceSite, UpdateConverted, spawnMGVOX, clearAllInGame, spamCraft, VoxToUp, MgVoxToUp;
	MyButton ApplyBGtoTyle, AddLimb, RotateLimb, SelectLimb, ConnectLimb, TakeForAWalk, DeleteLimb, DragLimb, AngleFlexibility, MoveX;
	struct animationButtons{
	MyButton AddLimb, AddExternal, Delete, Select, ConnectToAnimation,
		AllignBefore, AllignAfter, play, pause, Animate;
	bool MoveX;
	}anim; 
	worldcoordinates buildSite;
	IngameMegavox mgvPlay;
	int CurDollGroup;
	int CurDoll;
	int Curlimb;
	int lastPressMouseX, lastPressMouseY;
	bool UsingTool;
	int creationPiece;
	bool MBPressed;
	bool BuildSiteUpdated;
	SDL_Surface * selectedLimbNo;
	/// add creation selector
	} edit;
	float waterDis=0;
	float waterEl=0;
	bool waterRise=true;
	
	
	int topdddstrength=dddstength; // Used only for OpenGL
	int GLddd=dddstength;// changed inside GL function
	float TimePerFrame=16;
//*************************************************************** Questobjects
	//void (*foo) ();
struct MissionDescription
{
//int index;
std::string text;
};
const int missionsperpack=100;
struct MissionPack
{
std::string packName;
SDL_Surface* nameIMG;
MissionDescription mission[missionsperpack];
};

MissionPack* QPack;

struct QO 
{
    int qotx, qoty, qox, qoy, qoh, qot, qotimer, charge;
	int datax,datay;
	Uint32 age;
    QO * next;
	MissionDescription* scriptPtr;
	SDL_Surface * scriptText;// for editor
	SDL_Surface * TextToDisplay; // The text to display for player
	void (*OnCreate) ();
	void (*foo) ();
	bool DeleteMe, interacting;
};

struct questEditor
{
int MissonPacksCount;
int CurMissionPack;
int curMission;
bool WritingQuestDescription;
QO* targetQO; 
QO* QOhead; 
}qedit;

void (*PickMe) ()=NULL;
	 //QO * head = 0; 
	 int qolays=1; 
	 int qlay=1; 
	 QO * nq = qedit.QOhead;
int qx,qy;
void (*foonull) ()=NULL;

// **************************************************************** Framerate
 //SDL_Rect qdest; // Зовнішня змінна для drow функції
 
 int refrate=4;
	  int start = 0;
	  int frames =0;
	  int second = 0;
	  std::stringstream Fpscount;
	  int sumframes = 0;
	  int qobjects = 0;
	 
	  	int GravCalc=gravity*accuracy/speedoftime;
		//int bloodarrayvacancy=0;
		int repainttimer=0;
		int repaintdelay=3;
		int lqobjects=0;
		//Betatesting; 
		int targetfps=120;
		// ********************************************** THRETS
		SDL_Thread *thread = NULL;
		//SDL_Thread *threadb = NULL;
		bool threadquit = false;
		// Galaxies
//int solarx=500, solary=700;
		const int solars=2;
		int solarx[solars];
		int solary[solars];
		int planets[solars];
		int solarsun[solars];
		string solarname[solars];
		int selectedsolar=0, selectedplanet=0;
		// planets
string planetname;
int selectedsurface=0;
bool intread=true;

bool selected=false;		
bool pointed=false;
bool pnt=0;

// ******************************************************** Math
int bestdist=9999, bestno=0, divfuck, xx, yy;
//SDL_Rect src, src2;//, dest;

float theta,thetb;
int radd, ie, je, deepi; 
int stx,sty;
int hig;
int radiusxx; int radiusy; int radiush;
long double fuck;
//int globgx, globgy;
int shortx,shorty,shorth,dshortx, dshorty, dshorth;
//int flipppsy;
int d;
//int d,z, dx, dy, dh, cx,cy;
int (*tyopenfucktion) (int,int,int,int) =NULL;
Uint32 g;
int freephisslot=1;
//Thread
//int threadgx, threadgy;
const int scrwid=scrwi/2;
const int scrhid=scrhi/2;

// **************************************************************************************** WALL STRUCTURE

int wallined=1; 
int totalwalls=1; 

struct Windshake
{
int offx, offy;
int speedx, speedy;
int mass;
};
const int DebrisListMax=10;
const int RelatedParticlesLim=512;

struct MegavoxRelatedParticles{
Uint16 no[RelatedParticlesLim];
int Max;
bool free;
}DebrisList[DebrisListMax];

SDL_Surface* leavesQV;

struct wally
	{
	int type, height, solid, hp,i, rebendVoxelTimer;
	worldcoordinates pos;
	objectLight light;
	Windshake _shake;
	int brickcopy;
	int impparticle;
	IngameMegavox* Megavoxel;
	int Debris;
	int brickhp;
	Uint32 age; 
	float size;
	wally * next;
	wally * prev;
	SDL_Surface* brick; // Will point to picture of current state of the object 
	SDL_Surface* gutsout;
	void (*decay) ();
	void (*drowing) ();
	void (*OnDeath) ();
	bool drowAbove;
	bool RemoveMe, randGenerated;
	rotatableMe angles;
	} w[wlim]; 


wally* wfirst[matrhig+matrdepth];
wally* wlast[matrhig+matrdepth];
wally* curwall;
	void (*wallfunk) () = NULL;


// ************************************************************ Function Pointers
	void (*lightStyle) (SDL_Surface *source, SDL_Surface *layer, int dx, int dy, float dh) = NULL;
	void (*sdldrowstyle) (int onscreenx, int onscreeny, int elevation, SDL_Surface* surface) = NULL;
	void (*drowstyle) (int onscreenx, int onscreeny, int elevation, int source) = NULL;
	void (*mdrowstyle) (int onscreenx, int onscreeny, int elevation, int msource) = NULL;
	void (*tolendstyle) (int onscreenx, int onscreeny, int k, int gx, int gy) = NULL; 
	void (*DisplacementStyle) (SDL_Surface* source, int x, int y, int h, SDL_Surface * alphasource, int offx, int offy) = NULL; 
	void (*drowalphaborderStyle) (SDL_Surface* source, SDL_Surface * screen, SDL_Surface * alphasource, int border , int offx0, int offy, int elevation, int w) = NULL;
	void (*bloodstyle) (int i) = NULL;
	void (*MouseHoldAction) () = NULL;
	void (*MouseClick) (int MB) = NULL;
	void (*WhatIsOn) () = NULL;
	void (*DirectedShadowLDrow) () = NULL;
	void (*EventHandles) () = NULL;
	int activew=0;	

	

// Time of day
float timeofdayx=1, timeofdayy=0.3;
unsigned char sunshadow=200;
Uint32 shadowmapcolor;
	// pixmem32 = (Uint32*) screen->pixels  + y*screen->w + x;
    //*pixmem32 = colour;

  //
  struct guncharacteristics
  {
	unsigned char type;
	unsigned int ammo, recoil[10];	
  };

   time_t now; // time stuff


   // ******************************************************* CONNECTION STUFF *****************
const int maxconnections=100;
int sendcounter=0;
 

const int streamfat=128;


int StartServerListening(unsigned short port);
void EndServer(int socket);

const int messagefat=streamfat;

struct DecayConnection
{
char* entrymessage[maxconnections];
struct ServerData{
	int clientSocket[maxconnections];
	bool clientConected[maxconnections];
	bool clientTreadFree[maxconnections];
	bool ThreadNeedsCleanup[maxconnections];
	SDL_Thread *ThreadIn[maxconnections];
	int numberHolder[maxconnections];
	int Socket;
	int connections;
	bool listening;
	SDL_Thread *ThreadOut;
	SDL_Thread *LookForClient;
	
} serv;
struct ClientData{
int PackageCounter;
int mySocket;
string serverip;
bool TimeToClose;
bool connected;
SDL_Thread *ThreadOut;
SDL_Thread *ThreadIn;
SDL_Thread *ThreadConnect;
bool ClearInOutThreads;
bool ClearConnectThread;
int MaxGuests;
int MySlotOnServer;
} client;
	
	string text;
	int port;
	short onlinemode;
	int dump;

	//Uint32 SenderCounter
}inet;

const int dataslots=50;

struct sync
{
SDL_sem *datalock;
bool checkmail; 
char what [messagefat];
int bitesfilled;
int ClientNo;
} indata[dataslots], outdata[dataslots];

struct pack
{
	int ints[messagefat/4];
	short shorts[messagefat/2];
	char chars [messagefat];
	char countint, countshort, countchar;
} packer;

//To send data between characters
const int intsperChar=3;
const int shortsperchar=8;

// ***************************************************************** Stereoscopic 3D stuff *************
const bool gbresize=true; 
bool row;
int disx;
int disy;
float portion;
struct stereo_options
{
	int tomonitor;
	int eyewight;
	int lefteye;
	int righteye;
	int bgdepth;
	float bgportion;
	float bgprepare;
}conf3d;
float portions[(matrdepth+matrhig)*pixInPhis*2*3];
int precalcecvator;

//********************************************************************* Underground stuff *******************************

SDL_Surface * underg[5][4]; // 5 surfaces, 3 levels. 0 - under, 1- upper, 3 - mask, 4 - merge
int caves=0, curcave=1;
int cavecache=0;
const int cavelim = 100; 

struct caveclassing
	{
	int type, tx, ty, x, y, h, elevation;
	Uint32 age; 
	bool RemoveMeType0;
	} cave[cavelim];
//*********************************************************************** OpenGL stuff
bool openglon=false; 
bool startWithgl=0; 
//Uint32 flagss;
//************************************************************************* Softblit
float SoftBlitSpeed=-5;
const int SoftBlitLim = 100; 
int SoftBlitCount=0;
struct softblitter
	{
		SDL_Surface * destination;
		SDL_Surface * source;
		SDL_Surface * alpha;
	int tx, ty, x, y, h, type;
	float border, speed;
	int borderw;
	//Uint32 age;
	bool FreeIfDone;
	bool AddObjectIfDone;
	} SoftBlit[SoftBlitLim];

//************************************************************************ Inventory
const int BagSize=8;
const int inventoryobjects=50;


struct item
{
G_Particle InvVoxel; 
void (*inArmDrow) (int gunx, int guny, float pos);
void (*passiveDrow) (int torsox, int torsoy, float torsoangle);
}listOfStuff[inventoryobjects];



const int bottomListSize=5;



struct GlobInventory
{ 
	
	rotatableMe objectRotation;
	QO* ItemsOnTheGround[bottomListSize];
	SDL_Rect IOGframe[bottomListSize];
	SDL_Rect rectInventoryBlock;
	int IOGfading[bottomListSize];
	int halfw, halfh;
	//SDL_Surface* description[BagSize][BagSize];
	struct mat
	{
	SDL_Surface* invImge;
	SDL_Surface* rightMB;
	SDL_Surface* leftMB;
	SDL_Surface* Glassdisplacement;
	} materials;
	bool active;
} Idisplay;


int heimod=0;
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//temp
Uint32 bloods;
void (*voxelVallConvertor) (G_Particle * __restrict G, int offx, int offy, int offh, SDL_Surface * screen, double globxh, double globyh, double glob, float resize, SDL_Surface* decayed) = NULL;

// Full screen wind mask

//*************************************************** MISSIONS ************************************




int PEcache=1;
struct ParticleEffects
{
worldcoordinates pos; //tx,ty,x,y,h
G8_Particle* G8t;
//DirectionalLightetImage* DLImage;
int type,mask,speedx,speedy,speedh, xaccel, yaccel, creation, G_c, G_p;
double hp;
int decaySurface;
int offx, offy; // 
int displayTime;//for light blick

Uint32* g;

objectLight light;
double anglex, angley,angleh, rotatex, rotatey, rotateh;
//double maskx, masky, maskspx, maskspy; // used to drow fire
SDL_Surface* maskIMG;
ParticleEffects * next;
ParticleEffects * prev;
float gravity, slowing;
Uint8 MaxLight;
bool skip;
bool blitToTyle;
bool staticPos;
bool (*deathcondition) ();
void (*drowing) ();
} smoke[PElim];
//bl[bphlim];



ParticleEffects *PEpoint;
ParticleEffects PEfirst[matrhig+matrdepth];
ParticleEffects PElast[matrhig+matrdepth];

int planetx[solars][9], planety[solars][9], planettype[solars][9], planetsurface[solars][9];
string planame[solars][9];

const int maxLights=500;
const float heightLightFade=0.02;
const int heightWindLessing=60;
int lights=0;

const int lightArSize=64;
const int lightArSizex2=lightArSize*2;
Uint32 lightArray[255+1];
int lightsR[255+1];
int lightsG[255+1];
int lightsB[255+1];
int OnelightR;
int OnelightG;
int OnelightB;
int lightDist;
int lightGrad;

struct ThreadLights{
Uint32 lightArray[255+1];
int lightsR[255+1];
int lightsG[255+1];
int lightsB[255+1];
int OnelightR;
int OnelightG;
int OnelightB;
int lightDist;
int lightGrad;
} thrL;

struct LightSources
{
worldcoordinates* coords;
objectLight* light;
} light[maxLights];

struct ConvertedImage
{
SDL_Surface* image[20];
int images;
};

struct MySound
{
	int ActualVolume;
	double volume;
	double modifier;
	int min;
	Mix_Chunk *track;
};

struct TrackData
{
double playedSoundVolume;
int playedActualVolume;
double playedSoundModifier;
int playedSoundMin;
MySound* playedSound;
bool ambient;
int life;
};

const double hearingAdjustSpeed=0.3;
const int SoundSteps=12;


const int rifleSounds=5;
struct audioStructure
{
	
struct Soundbank
{

MySound chaingun[rifleSounds];
MySound minigun;
MySound softGuts;
MySound guts;
MySound ambientField;
MySound ambientSpace;
MySound step[SoundSteps];
MySound coolNoise[12];
MySound cloth[12];
MySound boom[7];
MySound boomFlat[6];
MySound fire[4];
MySound flameBurning;
MySound impact[9];

}bank;
//Mix_SetPanning(int channel, Uint8 left, Uint8 right)
struct Related
{
	int lastChaingunSound;
} info;

int stereoVividness;
double distanceMod;
double GlobalMaxVolume;

 TrackData OnTrack[8];
}sounds;

struct ScreenFitDrowFrame
{
	int fromx,fromy,tox,toy, ImgJump, scrJump;
}sFF; // screen fit frame

struct G8lineup
	{
		blood *bfirst; 
		part *pfirst;
		ParticleEffects *PEfirst;
	};	


struct somebody
{
worldcoordinates pos;
phisicalMe force;
objectLight light;
walkingHero walk;
rotatableMe angles;
offsets *targetOnScreen; // off
worldcoordinates prevAim;
int limbsSpawned;
Uint16 SpawnedLimbsNombers[maxLimbs];
//G8lineup g8lineup;

int type, hp,delay, curstate, i, acceleration, aim, shootstodo, sight, size, height;
somebody* targ;
//int targi;
offsets off;
	int bleedx, bleedy, bleedspx, bleedspy;
	int bleeding;

	somebody * next;
	somebody * prev;
	int (*damaged) ();
	int (*movement) ();
	int (*defaultMovement) ();
	//int (*ifCantMove) ();
	int (*clockzero) ();
	int (*death) ();
	int (*drow) ();
	SDL_Surface* gutsout;
	float slowwing;
	float clock;
	float animationTime;
	char voxGut; // to tell whitch damage was takken
	bool imgcopy;
	bool voxcopy;
	bool friendly;
	bool controled;
	
	} e[elim]; 
//const int what=sizeof(someBag);
//PlayerInfo
struct hero
	{
  //  worldcoordinates pos;
	//phisicalMe force;
	//int prevtx, prevty, prevcx, prevcy;
	int NextGameState; // used when quiting one game cycle into other
	bool isunderground, corporial;
	SDL_Surface* taskText;
	somebody* host;
	objectLight flashLight;
	someBag bag;
	} 	player;
	walkingHero* playerW;
	int LaserThick=5;

   struct ghost
{
	int tx,ty,x,y,h,type;
	somebody* host;
	someBag bag;
	walkingHero* guestW;
	offsets target;
}visiter[maxconnections];


	somebody* efirst[matrhig+matrdepth];
	somebody* elast[matrhig+matrdepth];
	somebody* epointer, *creature; 
	const int obsorbDamPostDeath=-200;
	int chars=1;
	int curchar=1;
	void (*eassigner) (int i)=NULL;

	int ecache=0, hurt;
	float hc;
int av,bv;

//smoke mask dinamics
bool smokeRendered=0;
double smokeRotation=0;
bool increasing=1;
float SMsize=30;//1;
float SMsize2=50;//1.4;
bool smokeAngMask1=0, smokeAngMask2=0;
float smokeRotSpeed=0.01;//0.005
int increaseCount=4;


//some stuff
int lgx=0,lgy=0,lx=0,ly=0; // to find original tyle and coordinates of where to take pixels from

//Smoke particles

const int DustDiam=6;
const int DustAlpha=255;
const int impactDustRand=75000;
const int CloudSpawnRandy=250000;
const int FireDustRandy=66000;
const float dustGravity=0.00005;
const float DustSlowing=0.95;
const int SmokeHP=50;
const int minBurnBrightness=23;
struct SmokeParticle
{
	worldcoordinates pos;
	phisicalMe force;
	int prevOffx, prevOffy;
	int prevOffx1, prevOffy1;
	int prevji;
	int type;
	float burning;
	float hp;
	bool newborn;
} dust[dustCount];

Uint32 * dustScreen;

struct BodyPartDrow
{
	int drowMethod;
	int itemInList;
	G8_Particle* G8;
	G_Particle* G;
	int x,y,h;
	double angle, xang, yang;
};

struct BodyPiecesCoordinates
{
int count;
BodyPartDrow limb[30];
}LimbsCoords;

SmokeParticle* duPoint;
int dustArVacancy=1;
int dustArMax=1;
bool dustVacThisTurn=1;
bool dustTurn=0;
SDL_Surface* dustTemp[4];
DirectionalLightetImage dustMask[4];
int mgoh=0;
int mgsa=0;
int minigunSMKx=0;
int minigunSMKy=0;

float xtrform=(float)scrwi/GLwi;
float ytrform=(float)scrhi/GLhi;
bool updatedGL=1;
SDL_Thread *ImageStreamer = NULL;
void (*updateFunk)();
void (*renderFunk)();

//TG-testing grounds
//Surface TGscreen1, TGscreen2;
//Surface *TGscreen, *TGGLscreen;
GLuint textureId;                   // ID of texture
GLubyte* imageData = 0;             // pointer to texture buffer
SDL_Surface* GLscreen;
worldcoordinates sun;
objectLight sunlight;

struct GLScreenOffsetData{
int tx, ty, x, y;
float offx, offy;
}GLoffset[2];
float MaxEnlarge;
bool curoffset=1;
int SDLredrowtime=0;
int timer=0;

// ################################################# Terrain Editor Stuff
const int maxStrokes=2048;
struct sectorStrokes{
Uint32 age; //4     // angles
float alpha; //2    // size
short x,y; //4
Uint8 mask; //1     // decay
Uint8 surface; //1  // voxel piece
Uint8 type; //1     //
Uint8 upsurface;    // voxel type
};

struct sTyleStroke{
sectorStrokes stroke[maxStrokes];
int max;
bool updated;
} Terrain[5][5];

const int TerrainBrushes=64;
struct MyIndexedRGBASurface 
{
	// No need for storing transparent pixels any more
	Uint32* pixels;
	int w,h;
	int pixCount;
	Uint32 i;
	std::string location;
};



struct Surface
{
	Uint32* pixels;
	int w,h,count;
	std::string location;
};

struct TunableDisplayValue{
int val;
int renderedVal;
SDL_Surface* textIMG;
};

struct TunableDisplayValueDouble{
double val;
double renderedVal;
SDL_Surface* textIMG;
};

const int ZoomGran=16;
const int ZoomVoxShadR=3;
const int ZVdi=ZoomVoxShadR*2+1;
struct TerrainEditorPalette{
	MyIndexedAlphaSurface IndDecay[TerrainBrushes];
	MyIndexedAlphaSurface ZoomDecay[ZoomGran];
	int zoomGrains;
	//Height
	DirectionalLightetImage height[TerrainBrushes];
	SDL_Surface* icons[TerrainBrushes]; 
	SDL_Surface* uppermap[TerrainBrushes];
	struct ZoomedVersionsOfImage{
	DirectionalLightetImage height[TerrainBrushes];
	SDL_Surface* uppermap[TerrainBrushes];
	} zoomedIMG;
	// DH
	DirectionalLightetImage DHheight[TerrainBrushes];
	SDL_Surface* DHuppermap[TerrainBrushes];

	Surface DecayBG[32];
	Surface DecayZoomBG[32];
	Surface UpperBG[32];
	Uint8 ShadowRad[ZoomVoxShadR*2+1];
	sectorStrokes teststroke;
	struct Editorial{
		EditorInterface selectorIntDecay;
		EditorInterface selectorHDecay;
		//MyButton nextIndD, prevIndD, nextSFC, prevSFC;
		MyButton RaiseSoil, LowerSoil, WaterLayer, LavaLayer;
		int SFCn;
		int drx, dry;
		int prevHexX, prevHexY;
		float Ax,Ay,Ah, G8resize;
		int G8h;
			struct TerrainToolConfig{
			
			bool brush[TerrainBrushes];
			bool AnyBrush;
			Uint8 DecaySfc;
			Uint8 UpperSfc;
			bool useSideUpper;
			int hdec;
			int dhdec;
			TunableDisplayValue IndAlpha;
			TunableDisplayValue Hheight;
			} mb[toolhands];
	} Edit;
	int dx, dy; // to remember which strokes must be redrown;
	SDL_Rect Outline;
	bool firstZoomRefresh;
	bool ZOOMED;
	int ptx, pty, px, py;
	Uint32* thisbg;
	bool waterOnZoom;
} tera;

// Dynamic wind generators


int WblowVacant=0;

int WblowMax=0; 

struct BlowingParticle{
worldcoordinates pos; 
phisicalMe force;
double speed;
float passFindSize;
float thick;
} Wblow[windBlowArSize];

struct GLMouseShaking{
int shkdX, shkdY;
float zoom;
} GLM;


// Limb shit will be here

int limbsInUse=0;
int firstFreeLimb=1; // Make Zero Limb out of limits

const int ambientR=12<<8;
const int ambientG=12<<16;
const int ambientB=13<<8;
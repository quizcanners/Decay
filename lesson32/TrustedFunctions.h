void ShowQuestObject()
{
QO* nq= qedit.QOhead;
while (nq)
{
int offx=nq->qox - actcamx - (cameratylx - nq->qotx)*tyleSize;
int	offy=nq->qoy - actcamy - (cameratyly - nq->qoty)*tyleSize;

DrowFrame (offx, offy, image[nq->qot]->w, image[nq->qot]->h, 0x000000); 
//qedit.targetQO=nq; 
//qedit.WritingQuestDescription=1;

 nq = nq->next;
}

}

void SelectQuestObject()
{
QO* nq= qedit.QOhead;
while (nq)
{
int offx=nq->qox - actcamx - (cameratylx - nq->qotx)*tyleSize;
int	offy=nq->qoy - actcamy - (cameratyly - nq->qoty)*tyleSize;

//DrowFrame (offx, offy, image[nq->qot]->w, image[nq->qot]->h, 0x000000); 
if ((mouse.pos.x>=offx) && (mouse.pos.x<offx+image[nq->qot]->w) && (mouse.pos.y>=offy) && (mouse.pos.y<offy+image[nq->qot]->h))
{
qedit.targetQO=nq; 
qedit.WritingQuestDescription=1;
	nq=0;
}
else
 nq = nq->next;
}

}




string myToString(int i)
{
std::stringstream s;
std::string st;
s<<i;
s>>st;
return st;
}

void generateImageIndexation(int no)
{
ImageIndexation[no].IMGindexStart=(new int[image[no]->h]);
ImageIndexation[no].IMGindexEnd=(new int[image[no]->h]);
Uint32* pixel = (Uint32*)image[no]->pixels;
for (int i=0; i<image[no]->h; i++)
{
	int x=0;
//+i*image[no]->w;
while ((!(*pixel & 0xFF000000)) && (x<image[no]->w))
{
	x++;
	pixel++;
}
ImageIndexation[no].IMGindexStart[i]=x;

while (x<image[no]->w)
{
	if (*pixel & 0xFF000000) 
		ImageIndexation[no].IMGindexEnd[i]=x;
x++;
pixel++;
}

}
}

void LavaImageIndexation(LRSurfaceInd* index, SDL_Surface* image, int max )
{
	index->indexed=1;
	if (!index->arraySet)
	{
index->IMGindexStart=(new int[image->h]);
index->IMGindexEnd=(new int[image->h]);
index->arraySet=1;
	}
Uint32* pixel = (Uint32*)image->pixels;
for (int i=0; i<image->h; i++)
{
	int x=0;
//+i*image[no]->w;
while ((!(*pixel>>24>=max)) && (x<image->w))
{
	x++;
	pixel++;
}
index->IMGindexStart[i]=x;
index->IMGindexEnd[i]=x;
while (x<image->w)
{
	if (*pixel>>24>=max) 
		index->IMGindexEnd[i]=x;
x++;
pixel++;
}

}
}



void setRandTerrain( int w, int h, int minusx, int minusy)
{
mrg.pos.x=(myRandom() % (tyleSize*2-w)) - minusx*tyleSize;
mrg.pos.y=(myRandom() % (tyleSize*2-h)) - minusy*tyleSize;
}
void setRandTerrainLocal( int w, int h)
{
mrg.pos.x=(myRandom() % (tyleSize-w));
mrg.pos.y=(myRandom() % (tyleSize-h));
}
int GetSpawnAmmount(spawnStats chance)
{
int bigNumber=chance.max/chance.chance;
bigNumber=((myRandom() % (bigNumber)) - bigNumber + chance.max);  
if (bigNumber>0) return bigNumber; else return 0;
}

Uint8 GetAlphaValue(int x, int y, SDL_Surface *map)
{
	//Uint32 *pixel; 
	//pixel=(Uint32*)map->pixels + y*map->w+x;
	//Uint8 bb; 
	//bb=*((Uint8 * )pixel+3);
	if ((x>=0) && (y>=0) && (y<tyleSize) && (x<tyleSize))
	return *((Uint8*)map->pixels + (y*map->w+x)*4+3);
	return 0;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Global Wind ops
void SetWindToGlobal ()  // later youll add stuff like intensity, wawe length, and wawe bottomn point
{
	int to=wind.h*wind.w;
	dddcoords * windpoint=wind.mask;
	int divide=wind.smoothness+1;
	for (int i=0; i<to; i++)
	{
		windpoint->x=(wind.global.x+windpoint->x*wind.smoothness)/divide;
		windpoint->y=(wind.global.y+windpoint->y*wind.smoothness)/divide;
		windpoint->h=(wind.global.h+windpoint->h*wind.smoothness)/divide;
		windpoint++;
	}
	wind.intensity=100+(rand() % 10);//max=100

wind.intensity-=(float)(wind.intensity-100)*0.00001;

wind.ang+=(rand() % 100)*0.0001 - 0.005;

if (wind.ang<0) wind.ang+=2*M_PI;
if (wind.ang>2*M_PI) wind.ang-=2*M_PI;

// dynamics
	/*
	
	//GetFloatAngleDistance(float sourceA, float targetA)
	//getAngle(x, y);
	// You should calculate displacement from main speed, not other
	float diff=1-GetFloatAngleDistance(getAngle(wind.globalAccel.x, wind.globalAccel.y), wind.ang)/10;

	wind.globalAccel.x*=diff;
	wind.globalAccel.y*=diff;

	wind.globalAccel.x+=radiusx(100)-wind.global.x/windDynamic;	
    wind.globalAccel.y+=radiusy-wind.global.y/windDynamic;

	wind.global.x+=wind.globalAccel.x;//=wind.global.x*diff+wind.globalAccel.x; //
	wind.global.y+=wind.globalAccel.y;//=wind.global.y*diff+wind.globalAccel.y;//
	*/
}
dddcoords * GetWindGlobal (int offx, int offy)
{
int i=offx/8;
int j=offy/8;
if ((i>=0) && (j>=0) && (i<wind.w) && (j<wind.h))
return &wind.mask[j*wind.w+i];
return &wind.global;
}
void createRadialWind(int offx, int offy, int power, int radius)
{
int i=offx/8;
int j=offy/8;
//SDL_Rect boom;
int bx=max(0,i-radius);
int by=max(0,j-radius);
int bw=min(wind.w,i+radius);
int bh=min(wind.h,j+radius);
//power;
for (int y=by; y<bh; y++)
{
	int yy=y-j+1*(j==y);
	dddcoords*point=&wind.mask[y*wind.w+bx];
	for (int x=bx; x<bw; x++)
	{
		int pif=pifagor (x-i, yy);
		if (pif<radius)
		{
		int temppower=power*(radius-pif)/radius;
		point->x=(temppower*(x-i)/pif + point->x)/2;
		point->y=(temppower*(yy)/pif + point->y)/2;
		point->h=(temppower/pif + point->h)/2;
		}
		
point++;
	}
}
}
void createVortex(int offx, int offy, int power, int radius)
{
int i=offx/8;
int j=offy/8;
//SDL_Rect boom;
int bx=max(0,i-radius);
int by=max(0,j-radius);
int bw=min(wind.w,i+radius);
int bh=min(wind.h,j+radius);
//power;
for (int y=by; y<bh; y++)
{
	int yy=y-j+1*(j==y);
	dddcoords*point=&wind.mask[y*wind.w+bx];
	for (int x=bx; x<bw; x++)
	{
		int pif=pifagor (x-i, yy);
		if (pif<radius)
		{
		int temppower=power*(radius-pif)/radius;
		point->x=((temppower*(x-i)/pif)+point->x)/2;
		point->y=((temppower*(yy)/pif)+point->y)/2;
		point->h=((temppower/pif)+point->h)/2;
		}
		
point++;
	}
}
}
void createDirectionalWind(int offx, int offy, int sx, int sy, int sh, int radius)
{
int i=offx/8;
int j=offy/8;
//SDL_Rect boom;
int bx=max(0,i-radius);
int by=max(0,j-radius);
int bw=min(wind.w,i+radius);
int bh=min(wind.h,j+radius);
//power;
for (int y=by; y<bh; y++)
{
	int yy=y-j+1*(j==y);
	dddcoords*point=&wind.mask[y*wind.w+bx];
	for (int x=bx; x<bw; x++)
	{
		int pif=pifagor (x-i, yy);
		if (pif<radius)
		{
		//int temppower=power*(radius-pif)/radius;
		point->x=((sx/pif)+point->x)/2;
		point->y=((sy/pif)+point->y)/2;
		point->h=((sh/pif)+point->h)/2;
		}
		
point++;
	}
}
}
void createTornado(int offx, int offy, int power, int radius, int direction) // its really a tornado
{
int i=offx/8;
int j=offy/8;
//SDL_Rect boom;
int bx=max(0,i-radius);
int by=max(0,j-radius);
int bw=min(wind.w,i+radius);
int bh=min(wind.h,j+radius);
//power;
power*=direction;
for (int y=by; y<bh; y++)
{
	int yy=y-j+1*(j==y);
	dddcoords*point=&wind.mask[y*wind.w+bx];
	for (int x=bx; x<bw; x++)
	{
		int pif=ipifagor (x-i, yy);
		if ((pif<radius) && (pif))
		{

double force=(double)power*(1/(double)pif-1/(double)radius); 
point->x=((-yy*force)+point->x)/2;//*power;
point->y=(((x-i)*force)+point->y)/2;//*power;

		/*int temppower=power*(radius-pif)/radius;
		point->x+=(temppower*(x-i)/pif - point->x)/2;
		point->y+=(temppower*(yy)/pif - point->y)/2;
		point->h+=(temppower/pif - point->h)/2;*/
		}
		
point++;
	}
}
}
void putWindBlowValue(int offx, int offy, int forcex, int forcey){
offx/=8; 
offy/=8;
offx=offy*wind.w+offx;

wind.mask[offx].x=(forcex+wind.mask[offx].x)/2;
wind.mask[offx].y=(forcey+wind.mask[offx].y)/2;
}
// @@@@@@@@@ WindBlowingParticles

void AddWindBlow(int x, int y)
{
	while ((Wblow[WblowVacant].speed>0) && (WblowVacant<windBlowArSize-1))  WblowVacant++;
if (WblowVacant>=windBlowArSize) return;

int ti=WblowVacant;

Wblow[ti].pos.h=accuracy;//*(rand() % 64);
Wblow[ti].pos.x=(camerax+x)*accuracy;
Wblow[ti].pos.y=(cameray+y)*accuracy;
Wblow[ti].pos.tx=cameratylx; 
Wblow[ti].pos.ty=cameratyly;

if (getmematrix (Wblow[ti].pos.tx, Wblow[ti].pos.ty, 
	Wblow[ti].pos.x>>16, 
	Wblow[ti].pos.y>>16,
	Wblow[ti].pos.h>>16)) return;

Wblow[ti].force.speedx=accuracy/100*sin(wind.ang)*wind.intensity;
Wblow[ti].force.speedy=accuracy/100*cos(wind.ang)*wind.intensity;
Wblow[ti].force.speedh=0;
Wblow[ti].passFindSize=0;
Wblow[ti].thick=0.1*(rand() % 500)+50;
Wblow[ti].speed=ipifagor (Wblow[ti].force.speedx/100, Wblow[ti].force.speedy/100)*100;
WblowMax=max(WblowVacant,WblowMax);
/*
const int windBlowArSize=2000;
int WblowMax=0; 
struct BlowingParticle{
worldcoordinates pos; 
phisicalMe force;
int hp;
} Wblow[windBlowArSize];*/

}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Wind Shit End



void untransparentizeImage (SDL_Surface * __restrict unwantedAlpha)
{
	int size=unwantedAlpha->w*unwantedAlpha->h-1;
	Uint8 * __restrict alpha=(Uint8*)unwantedAlpha->pixels+3;
		//AH=*((Uint8 * )sourpic+3);
	for (int x=0; x<size;)
	{
		*alpha=255;
		alpha+=4;
		x++;
	}
}

// precalculate height displacement before this function



void CastShadowForDarkenfFunk (SDL_Surface *source, SDL_Surface *layer, int dx, int dy, int h, bool directAlphaValue)
{
	//Globali
	float a;

	if (directAlphaValue)
		{
		//h=min(max(h,0),255);
		a=min(max(h,0),255);
	}
	else
	{
		h=accurate(h)*4;
		h=min(max(h,0),255);
	a=255-h;
	}
	a/=255;
Uint32 * __restrict mask;
Uint8 *  __restrict sourpic;
if ((dx+source->w<0) || (dx>scrwi) || (dy+source->h<0) || (dy>scrhi) || (a<0.05)) return;
//dh/=102400;
//accurate: >>16
// 1 matrix square is 64 height (pixels of length)
int x=0;
int y=0;
//if (dh>1)
//x=y;
if (dx<0) x-=dx;
if (dy<0) y-=dy;
int destx=source->w;
int desty=source->h;
if (dx+destx>layer->w) destx=(layer->w-dx);
if (dy+desty>layer->h) desty=(layer->h-dy);

fillDarkMaskGrid(dx, dy, source->w, source->h);
//dh=1;
for (y; y<desty; y++)
{

//darkIndexStart[dy+y]=min(darkIndexStart[dy+y],dx+x);
//darkIndexEnd[dy+y]=max(darkIndexEnd[dy+y], destx+dx);

	sourpic=(Uint8*) source->pixels  + y*source->w*4 + x*4+3;
	mask=darkmask+(dy+y)*scrwi+dx+x;
register int xx=destx-x;
//for (int xx=x; xx<destx;)
if (xx>0)
while(xx)
{
	//if (((*mask>>8)&0xFF)<GlobalI)
		*mask+=(*sourpic * a);//(*mask & 0xFFFFFF00) | max(*mask & 0xFF,(*sourpic * a / 255)); //& ~(shorth)); // h<<24;
	//*mask&=0x0000FFFF;
xx--; 
mask++;
sourpic+=4;

}
}	
}


void CastLightForDarkenfFunk (SDL_Surface *source, SDL_Surface *layer, int dx, int dy, float displayTime)
{
	//int displayTime=5;
	//Globali
//	displayTime=max(displayTime,1);
Uint32 * __restrict mask;
Uint32 *  __restrict sourpic;
Uint32 *  __restrict screenpic;
if ((dx+source->w<0) || (dx>scrwi) || (dy+source->h<0) || (dy>scrhi) || (!displayTime)) return;
//dh/=102400;
//accurate: >>16
// 1 matrix square is 64 height (pixels of length)
int x=0-dx*(dx<0);
int y=0-dy*(dy<0);
//if (dh>1)
//x=y;
//if (dx<0) x-=dx;
//if (dy<0) y-=dy;
int destx=source->w;
int desty=source->h;
if (dx+destx>layer->w) destx=(layer->w-dx);
if (dy+desty>layer->h) desty=(layer->h-dy);

fillDarkMaskGrid(dx, dy, source->w, source->h);
//dh=1;
for (y; y<desty; y++)
{
//darkIndexStart[dy+y]=min(darkIndexStart[dy+y],dx+x);
//darkIndexEnd[dy+y]=max(darkIndexEnd[dy+y], destx+dx);
	sourpic=(Uint32*)source->pixels + y*source->w + x;
	screenpic=(Uint32*)layer->pixels + (dy+y)*layer->w +dx+ x;
	mask=(Uint32*)darkmask+(dy+y)*scrwi+dx+x;
register int xx=destx-x;
//for (int xx=x; xx<destx;)
if (xx>0)
while(xx)
{

	//if	(*sourpic>*mask)
	//{
//Uint32 part=max(*sourpic,*mask)  & 0xFF000000; //max((*sourpic),(*mask)) & 0xFF000000;
	
//	int volume=
	//int spm=
	int sp=(*screenpic>>24);
	sp=(sp<128)*2*(sp-128);
	int spm=(*sourpic>>24) + sp;//,(*mask>>24));
	//int vol=spm ;
if  (spm>0)
{
	//int mdt=((*mask>>16) & 0xFF);
	//*displayTime/mdt);
	//Uint32 part=min(spm,255)<<16;
	//Uint32 dispTime=max(displayTime<<16,*mask & 0xFF0000);
	//	Uint32 dispTime=(0x00FF0000 & (((part/displayTime)>>8)))+0x00010000;
	*mask+=((Uint32)(spm*displayTime)<<16);//(*mask &  0x0000FFFF) + part; //| dispTime; 
}	//}
xx--; 
mask++;
sourpic++;
screenpic++;
}
}	
}

void CastLightForDarkenfFunkNoVolume (SDL_Surface *source, int dx, int dy, float displayTime)
{
	//int displayTime=5;
	//Globali
	//displayTime=max(1,displayTime);
//Uint32 *  __restrict screenpic;
	if ((dx+source->w<0) || (dx>scrwi) || (dy+source->h<0) || (dy>scrhi) || (!displayTime)) return;
//dh/=102400;
//accurate: >>16
Uint32 * __restrict mask;
Uint32 *  __restrict sourpic;
// 1 matrix square is 64 height (pixels of length)
int x=0-dx*(dx<0);
int y=0-dy*(dy<0);
//if (dh>1)
//x=y;
//if (dx<0) x-=dx;
//if (dy<0) y-=dy;
int destx=source->w;
int desty=source->h;
if (dx+destx>scrwi) destx=(scrwi-dx);
if (dy+desty>scrhi) desty=(scrhi-dy);

fillDarkMaskGrid(dx, dy, source->w, source->h);
//dh=1;
for (y; y<desty; y++)
{
//darkIndexStart[dy+y]=min(darkIndexStart[dy+y],dx+x);
//darkIndexEnd[dy+y]=max(darkIndexEnd[dy+y], destx+dx);
	sourpic=(Uint32*)source->pixels + y*source->w + x;
	//screenpic=(Uint32*)layer->pixels + (dy+y)*layer->w +dx+ x;
	mask=(Uint32*)darkmask+(dy+y)*scrwi+dx+x;
register int xx=destx-x;
//for (int xx=x; xx<destx;)
if (xx>0)
while(xx)
{
	//if ((((*mask>>8)&0xFF)<GlobalI) &&	

	//int vol=spm + (*screenpic>>24) - 223;




	//if (*sourpic>*mask)
	//{
	//	Uint32 spm=max((*sourpic),(*mask));
	 //max((*sourpic),(*mask)) & 0xFF000000;
if ((*sourpic & 0xFF000000))
{
	//Uint32 part=(max(*sourpic,*mask)  & 0xFF000000)>>8;
	//Uint32 dispTime=max(displayTime<<16,(*mask & 0x00FF0000));//(0x00FF0000 & (((part/displayTime)>>8)))+0x00010000;	
	*mask+=((Uint32)((*sourpic>>24)*displayTime) <<16);//(*mask &  0x0000FFFF) | part | dispTime; 
}
	//}

xx--; 
mask++;
sourpic++;
//screenpic++;
}
}	
}

void IndexedDrowWithShadowErasing (SDL_Surface *source, int dx, int dy, int imgNo)
{
Uint32 * __restrict sourpic, * __restrict destpic;
	int x=0;
	int y=0;
	if (dx<0) x-=dx;
	if (dy<0) y-=dy;
	int destx=source->w;
	int desty=source->h;
	if (dx+destx>screen->w) destx=(screen->w-dx);
	if (dy+desty>screen->h) desty=(screen->h-dy);
for (y; y<desty; y++)
{
	int st=max(x,ImageIndexation[imgNo].IMGindexStart[y]);
	sourpic=(Uint32*) source->pixels  + y*source->w + st;
	destpic=(Uint32*) screen->pixels  + (dy+y)*screen->w + dx+st;
Uint32 * __restrict mask=darkmask+(dy+y)*scrwi+dx+st;
//	IMGindexStart[no][i]=x;
int et=min(destx,ImageIndexation[imgNo].IMGindexEnd[y]);
xx=et-st;
if (xx>0)
while(xx)
		{
if (*sourpic & 0xFF000000) 
{
	*destpic=*sourpic & 0x00FFFFFF;	
	*mask&=0xFFFF0000;
}
sourpic++;
destpic++;
mask++;
xx--;
		}
}	
}

void DrowWithShadowErasing (SDL_Surface *source, int dx, int dy)
{
Uint32 * __restrict sourpic, * __restrict destpic;
	int x=0;
	int y=0;
	if (dx<0) x-=dx;
	if (dy<0) y-=dy;
	int destx=source->w;
	int desty=source->h;
	if (dx+destx>screen->w) destx=(screen->w-dx);
	if (dy+desty>screen->h) desty=(screen->h-dy);
for (y; y<desty; y++)
{
	//int st=max(x,ImageIndexation[imgNo].IMGindexStart[y]);
	sourpic=(Uint32*) source->pixels  + y*source->w + x;
	destpic=(Uint32*) screen->pixels  + (dy+y)*screen->w + dx+x;
Uint32 * __restrict mask=darkmask+(dy+y)*scrwi+dx+x;
//	IMGindexStart[no][i]=x;
//int et=min(destx,ImageIndexation[imgNo].IMGindexEnd[y]);
xx=destx-x;
if (xx>0)
while(xx)
		{
if (*sourpic & 0xFF000000) 
{
	*destpic=*sourpic & 0x00FFFFFF;	
	*mask=0;
}
sourpic++;
destpic++;
mask++;
xx--;
		}
}	
}

void DinamicShadowObject (SDL_Surface * objectHeigh, SDL_Surface *object, UpShadLayer *shadowMap, int dx, int dy, int offx, int offy, int dh)
{
SDL_Rect src, src2, dest;
	int x;


Uint32 * __restrict mask;
// src - object, src2- shadowMap, dest-screen
	// src.w and h is not size its final coordinate
// You'll have to init all shadow maps
	src.x=max(max(0,-offx),-dx);
	src.y=max(max(0,-offy),-dy);

	int srcw=min(min(tyleSize-dx, object->w),scrwi-offx);
	int srch=min(min(tyleSize-dy, object->h),scrhi-offy);

	src2.x=dx+src.x;
	src2.y=dy+src.y;

	dest.x=max(offx+src.x,0);
	dest.y=max(offy+src.y,0);
if ((srcw<=src.x) | (srch<=src.y) | (!shadowMap) | (src.y<0) | (src.x<0)) return;

int  dark; //shadow, screenXLimit;
Uint32 * __restrict srcpic, // * __restrict hpic,
	* __restrict screenpic;
Uint8* __restrict shadpic;
//srcpic=
int objdis;
//screenXLimit=screen->w-dest.x;
if ((srcw>0) && (srch>0))
for (int y=0; y<srch-src.y; y++)
{
objdis=(src.y+y)*object->w + src.x;
srcpic=(Uint32*)object->pixels+objdis;
//hpic=(Uint32*)objectHeigh->pixels+objdis;
shadpic=&shadowMap->pix[(src2.y+y)*tyleSize + src2.x];
screenpic=(Uint32*)screen->pixels+(dest.y+y)*screen->w + dest.x;
mask=darkmask;
mask+=(dest.y+y)*scrwi+dest.x;
for (x=0; x<srcw-src.x; x++)
{
//diff=*((Uint8 * )screenpic+3)-*((Uint8 * )hpic+3)-dh;
//if ((*((Uint8 * )screenpic+3)-*((Uint8 * )hpic+3)>0) &&
if	(*((Uint8 * )srcpic+3)>0)
{
*mask&=0xFFFF0000;//GlobalI<<8;
dark=255-*shadpic;
register int isrc = *srcpic;
*screenpic =  ((RedMask & (((isrc & RedMask)*dark)>>8)) | (GreenMask & (((isrc & GreenMask)*dark)>>8)) | (BlueMask & (((isrc & BlueMask)*dark)>>8))) & 0x00FFFFFF;// | (*hpic & 0xFF000000 );
//*((Uint8 * )screenpic+3)=*((Uint8*)hpic+3);	
}
shadpic++;
//hpic++;
screenpic++;
srcpic++;
mask++;
}
}
}



void BlitWithDHInprintPreview(SDL_Surface *layer, SDL_Surface *source, SDL_Surface *heightMask, int dx, int dy, int h)
{
Uint32 * __restrict sourpic, * __restrict destpic, * __restrict hpic;
	int x=0;
	int y=0;
	if (dx<0) x-=dx;
	if (dy<0) y-=dy;
	int destx=source->w;
	int desty=source->h;
	if (dx+destx>layer->w) destx=(layer->w-dx);
	if (dy+desty>layer->h) desty=(layer->h-dy);
Uint8 AH;
float contactShadow;
int diff;
for (y; y<desty; y++)
{
	sourpic=(Uint32*) source->pixels  + y*source->w + x;
	
	hpic=(Uint32*) heightMask->pixels  + y*heightMask->w + x;
	destpic=(Uint32*) layer->pixels  + (dy+y)*layer->w + dx+x;

	for (int xx=x; xx<destx; xx++)
		{
			
	//*((Uint8 * )destpic+3)=max(*((Uint8 * )sourpic+3),*((Uint8 * )destpic+3));
			diff=*((Uint8 * )destpic+3)-*((Uint8 * )hpic+3)+h+512*(*((Uint8 * )hpic+3)<16);
	if (diff<=0)//(*((Uint8 * )hpic+3)<=*((Uint8 * )destpic+3)+10)
	{
		diff=-diff;
		//diff=abs(diff);
		*((Uint8 * )destpic+3)=max(0,min((int)(*((Uint8*)hpic+3))-h,255));
		//contactShadow=1;
	//if (*destpic<*sourpic)
	//*destpic=*sourpic;
		if ((diff<16))
		{
			contactShadow=(diff*16);
			contactShadow/=255;
			AH=*((Uint8 * )sourpic+3) * contactShadow;
		}
		else
AH=*((Uint8 * )sourpic+3);

	
*((Uint8 * )destpic) = (((*((Uint8 * )sourpic) -  *((Uint8 * )destpic)) * (AH))/255 + *((Uint8 * )destpic));
*((Uint8 * )destpic+1) = (((*((Uint8 * )sourpic+1) -  *((Uint8 * )destpic+1)) * (AH))/255 + *((Uint8 * )destpic+1));
*((Uint8 * )destpic+2) = (((*((Uint8 * )sourpic+2) -  *((Uint8 * )destpic+2)) * (AH))/255 + *((Uint8 * )destpic+2));


	}

//}
sourpic++;
destpic++;

hpic++;
	//if ((x<0) || (y<0) || (x>=screen->w) || (y>=screen->h)) return 0;
    //Uint32 *pixmem32;
    //Uint32 colour;  
 
    //colour = SDL_MapRGB( screen->format, r, g, b );
  
   // pixmem32 = (Uint32*) screen->pixels  + y*screen->w + x;
  //  *pixmem32 = colour;
		}
}
	
}
void BlitWithDHDecay(SDL_Surface *layer, SDL_Surface *source, SDL_Surface *uplayer, SDL_Surface *DecaySFC, SDL_Surface *heightMask, int dx, int dy, int h)
{
Uint32 * __restrict sourpic,* __restrict updecay, * __restrict destpic, * __restrict updestpic, * __restrict hpic;
	int x=0;
	int y=0;
	if (dx<0) x-=dx;
	if (dy<0) y-=dy;
	int destx=source->w;
	int desty=source->h;
	if (dx+destx>layer->w) destx=(layer->w-dx);
	if (dy+desty>layer->h) desty=(layer->h-dy);
Uint8 AH;

int diff;
for (y; y<desty; y++)
{
	sourpic=(Uint32*) source->pixels  + y*source->w + x;
	updecay=(Uint32*) DecaySFC->pixels  + (dy+y)*uplayer->w + dx+x;
	hpic=(Uint32*) heightMask->pixels  + y*heightMask->w + x;
	destpic=(Uint32*) layer->pixels  + (dy+y)*layer->w + dx+x;
	updestpic=(Uint32*) uplayer->pixels  + (dy+y)*uplayer->w + dx+x;
	for (int xx=x; xx<destx; xx++)
		{
			
			diff=-((*destpic>>24)-(*hpic>>24)+h+512*((*hpic>>24)<16));
	if (diff>0)
	{
		//diff=-diff;
		*((Uint8 * )destpic+3)=max(0,min((int)(*hpic>>24)-h,255));
		if ((diff<=32))	{
			AH=(diff*8)-1;
		
*updestpic=(*updestpic & 0x00FFFFFF) |  (((*updestpic>>24)*(255-AH)/255)<<24);
			/*
*((Uint8 * )updestpic) = (((*((Uint8 * )sourpic) -  *((Uint8 * )updestpic)) * (AH))/255 + *((Uint8 * )updestpic));
*((Uint8 * )updestpic+1) = (((*((Uint8 * )sourpic+1) -  *((Uint8 * )updestpic+1)) * (AH))/255 + *((Uint8 * )updestpic+1));
*((Uint8 * )updestpic+2) = (((*((Uint8 * )sourpic+2) -  *((Uint8 * )updestpic+2)) * (AH))/255 + *((Uint8 * )updestpic+2));
*((Uint8 * )updestpic+3) = (((*((Uint8 * )sourpic+3) -  *((Uint8 * )updestpic+3)) * (AH))/255 + *((Uint8 * )updestpic+3));*/

		}
		else
		{ AH=255; *updestpic&=0x00FFFFFF; }//*updestpic=*sourpic; }
		/*
*((Uint8 * )destpic) = (((*((Uint8 * )updecay) -  *((Uint8 * )destpic)) * (AH))/255 + *((Uint8 * )destpic));
*((Uint8 * )destpic+1) = (((*((Uint8 * )updecay+1) -  *((Uint8 * )destpic+1)) * (AH))/255 + *((Uint8 * )destpic+1));
*((Uint8 * )destpic+2) = (((*((Uint8 * )updecay+2) -  *((Uint8 * )destpic+2)) * (AH))/255 + *((Uint8 * )destpic+2));*/
		AH=AH*(*updecay>>24)/255;
		register int idst=*destpic;
		register int mcolor=*updecay;
		*destpic = ((RedMask & ((idst & RedMask) + ((int)(((int)(((mcolor & RedMask))) - (int)(idst & RedMask)) * AH)>>8))) |
		(GreenMask & ((idst & GreenMask) + ((int)(((((int)(mcolor & GreenMask))) - (int)(idst & GreenMask)) * AH)>>8))) |
		(BlueMask & ((idst & BlueMask) + ((int)(((((int)(mcolor & BlueMask))) - (int)(idst & BlueMask)) * AH)>>8)))) |
		(*destpic & 0xFF000000);
}
sourpic++;
destpic++;
updecay++;
updestpic++;
hpic++;

		}
}
		
}
void BlitWithDHInprint(SDL_Surface *layer, SDL_Surface *source, SDL_Surface *uplayer, SDL_Surface *upsource, SDL_Surface *heightMask, int dx, int dy, int h)
{
Uint32 * __restrict sourpic,* __restrict upsourpic, * __restrict destpic, * __restrict updestpic, * __restrict hpic;
	int x=0;
	int y=0;
	if (dx<0) x-=dx;
	if (dy<0) y-=dy;
	int destx=source->w;
	int desty=source->h;
	if (dx+destx>layer->w) destx=(layer->w-dx);
	if (dy+desty>layer->h) desty=(layer->h-dy);
Uint8 AH;
float contactShadow;
int diff;
for (y; y<desty; y++)
{
	sourpic=(Uint32*) source->pixels  + y*source->w + x;
	upsourpic=(Uint32*) upsource->pixels  + y*upsource->w + x;
	hpic=(Uint32*) heightMask->pixels  + y*heightMask->w + x;
	destpic=(Uint32*) layer->pixels  + (dy+y)*layer->w + dx+x;
	updestpic=(Uint32*) uplayer->pixels  + (dy+y)*uplayer->w + dx+x;
	for (int xx=x; xx<destx; xx++)
		{
			
	//*((Uint8 * )destpic+3)=max(*((Uint8 * )sourpic+3),*((Uint8 * )destpic+3));
			diff=(*destpic>>24)-(*hpic>>24)+h+512*((*hpic>>24)<16);
	if (diff<=0)//(*((Uint8 * )hpic+3)<=*((Uint8 * )destpic+3)+10)
	{
		diff=-diff;
		//diff=abs(diff);
		*((Uint8 * )destpic+3)=max(0,min((int)(*hpic>>24)-h,255));
		//contactShadow=1;
	//if (*destpic<*sourpic)
	//*destpic=*sourpic;
		if ((diff<16))
		{
			contactShadow=(diff*16);
			contactShadow/=255;
			AH=*((Uint8 * )sourpic+3) * contactShadow;
*((Uint8 * )updestpic) = (((*((Uint8 * )upsourpic) -  *((Uint8 * )updestpic)) * (AH))/255 + *((Uint8 * )updestpic));
*((Uint8 * )updestpic+1) = (((*((Uint8 * )upsourpic+1) -  *((Uint8 * )updestpic+1)) * (AH))/255 + *((Uint8 * )updestpic+1));
*((Uint8 * )updestpic+2) = (((*((Uint8 * )upsourpic+2) -  *((Uint8 * )updestpic+2)) * (AH))/255 + *((Uint8 * )updestpic+2));
*((Uint8 * )updestpic+3) = (((*((Uint8 * )upsourpic+3) -  *((Uint8 * )updestpic+3)) * (AH))/255 + *((Uint8 * )updestpic+3));
		}
		else
		{

	//contactShadow=1;
*updestpic=*upsourpic;
/*AH=*((Uint8 * )upsourpic+3);
*((Uint8 * )updestpic) = (((*((Uint8 * )upsourpic) -  *((Uint8 * )updestpic)) * (AH))/255 + *((Uint8 * )updestpic));
*((Uint8 * )updestpic+1) = (((*((Uint8 * )upsourpic+1) -  *((Uint8 * )updestpic+1)) * (AH))/255 + *((Uint8 * )updestpic+1));
*((Uint8 * )updestpic+2) = (((*((Uint8 * )upsourpic+2) -  *((Uint8 * )updestpic+2)) * (AH))/255 + *((Uint8 * )updestpic+2));*/
AH=*((Uint8 * )sourpic+3);
		}
	
*((Uint8 * )destpic) = (((*((Uint8 * )sourpic) -  *((Uint8 * )destpic)) * (AH))/255 + *((Uint8 * )destpic));
*((Uint8 * )destpic+1) = (((*((Uint8 * )sourpic+1) -  *((Uint8 * )destpic+1)) * (AH))/255 + *((Uint8 * )destpic+1));
*((Uint8 * )destpic+2) = (((*((Uint8 * )sourpic+2) -  *((Uint8 * )destpic+2)) * (AH))/255 + *((Uint8 * )destpic+2));


	}

//}
sourpic++;
destpic++;
upsourpic++;
updestpic++;
hpic++;
	//if ((x<0) || (y<0) || (x>=screen->w) || (y>=screen->h)) return 0;
    //Uint32 *pixmem32;
    //Uint32 colour;  
 
    //colour = SDL_MapRGB( screen->format, r, g, b );
  
   // pixmem32 = (Uint32*) screen->pixels  + y*screen->w + x;
  //  *pixmem32 = colour;
		}
}
		
}
void BlitNoInprint(SDL_Surface *layer, SDL_Surface *source, int dx, int dy, int h)
{
	h=255-h;
Uint32 * __restrict sourpic, * __restrict destpic, * __restrict hpic;
	int x=0;
	int y=0;
	if (dx<0) x-=dx;
	if (dy<0) y-=dy;
	int destx=source->w;
	int desty=source->h;
	if (dx+destx>layer->w) destx=(layer->w-dx);
	if (dy+desty>layer->h) desty=(layer->h-dy);
Uint8 AH;
float contactShadow;
int diff;
for (y; y<desty; y++)
{
	sourpic=(Uint32*) source->pixels  + y*source->w + x;
	//hpic=(Uint32*) heightMask->pixels  + y*heightMask->w + x;
	destpic=(Uint32*) layer->pixels  + (dy+y)*layer->w + dx+x;
	for (int xx=x; xx<destx; xx++)
		{
	//*((Uint8 * )destpic+3)=max(*((Uint8 * )sourpic+3),*((Uint8 * )destpic+3));
			diff=*((Uint8 * )destpic+3)-h;
	if (diff>=0)
	{
		//diff=abs(diff);
		//*((Uint8 * )destpic+3)=*((Uint8*)hpic+3)-h;
		//contactShadow=1;
	//if (*destpic<*sourpic)
	//*destpic=*sourpic;
		if ((diff>31))
			{
AH=*((Uint8 * )sourpic+3);
		}
		else
		{
			contactShadow=(diff*8);
			contactShadow/=255;
			AH=*((Uint8 * )sourpic+3) * contactShadow;
		}
		
		
	
*((Uint8 * )destpic) = (((*((Uint8 * )sourpic) -  *((Uint8 * )destpic)) * (AH))/255 + *((Uint8 * )destpic));
*((Uint8 * )destpic+1) = (((*((Uint8 * )sourpic+1) -  *((Uint8 * )destpic+1)) * (AH))/255 + *((Uint8 * )destpic+1));
*((Uint8 * )destpic+2) = (((*((Uint8 * )sourpic+2) -  *((Uint8 * )destpic+2)) * (AH))/255 + *((Uint8 * )destpic+2));
	}

//}
sourpic++;
destpic++;
//hpic++;
	//if ((x<0) || (y<0) || (x>=screen->w) || (y>=screen->h)) return 0;
    //Uint32 *pixmem32;
    //Uint32 colour;  
 
    //colour = SDL_MapRGB( screen->format, r, g, b );
  
   // pixmem32 = (Uint32*) screen->pixels  + y*screen->w + x;
  //  *pixmem32 = colour;
		}
}
		
}
void BWAITextureDisplacement(SDL_Surface *layer, SDL_Surface *source, SDL_Surface *reflection, UpShadLayer *shadow, int dx, int ddy, int h, int TDy)
{

//height mask should be dinamic
int hcenterx=(1920-scrwi)/2;
int hcentery=(1080-scrhi)/2;
	h=12;
Uint32 * __restrict sourpic, * __restrict destpic,  * __restrict clodpix;
Uint8* __restrict shpix;
int dy=ddy-tyleSize+TDy;
int cly=0;
	int x=0;
	int y=0;
	if (dx<0) x-=dx;
	if (dy<0) y-=dy;  
	if (ddy<0) cly=-ddy;
	y=max(y,tyleSize-TDy);
	int destx=source->w;
	int desty=source->h;
	if (dx+destx>layer->w) destx=(layer->w-dx);
	if (dy+desty>layer->h) desty=(layer->h-dy);
//Uint8 AS;
int diff, diff2;


clodpix=(Uint32*) reflection->pixels +(cly)*reflection->w + x;
shpix=&shadow->pix[(cly)*tyleSize + x];
int cladd= reflection->w-(destx-x);
int shadd= tyleSize-(destx-x);
for (y; y<desty; y++)
{
	sourpic=(Uint32*) source->pixels  + y*source->w + x;
	destpic=(Uint32*) layer->pixels  + (dy+y)*layer->w + dx+x;
	
	
xx=x;
xx=destx-x;
if (xx>0)
while (xx)
{
int diff=(*destpic>>24)-233-((*sourpic>>8) & 0xFF)/16;

if (diff>0)
{
int AH=diff<<3;
register int spic=*sourpic;
diff2=((spic & 0xFF) * (*clodpix & 0xFF)) ;
								//| ((((spic & 0x8000)>>8) | ((int)(*((Uint8*)clodpix+1)) & (spic>>8)   ))>>1))
								//*(256-(*shpix))>>8 ;
register Uint32 tempdest;
if (diff<9){
	int rev=8-diff;
	diff2=min(255, diff2+ (((*clodpix>>8) & BlueMask)*((*sourpic>>8) & 0xFF)*rev>>11));
	//brightenBlueChannel(&tempdest, ((*clodpix>>8) & BlueMask)*((*sourpic>>8) & 0xFF)>>8 );
     AH+=(rev*diff)*9;
	tempdest=diff2;
	tempdest|=(diff2<<8);
tempdest+=(diff2<<16);
} else {
tempdest=diff2;
tempdest|=((diff2<<7)) & 0xFF00;
tempdest+=0x0800;
}






register int idst=*destpic;
register int isrc=tempdest;
*destpic= ((RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * AH) >>8))) 
	| (GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * AH) >>8))) 
	| (BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * AH) >>8)))) | 
	0xF0000000;
//}
//*destpic=((*destpic) & 0x00FFFFFF) | ((*hpic & 0xFF000000) - h*0x01000000 );
}
sourpic++;
destpic++;
clodpix++;
xx--;
shpix++;
}
	clodpix+=cladd;
	shpix+=shadd;
}


dy=ddy+TDy;
y=0;
if (dy<0) y-=dy;
desty=source->h-TDy;
if (dy+desty>layer->h) desty=(layer->h-dy);

//clodpix=(Uint32*) reflection->pixels +(cly)*reflection->w + x;

for (y; y<desty; y++)
{
	sourpic=(Uint32*) source->pixels  + y*source->w + x;
	destpic=(Uint32*) layer->pixels  + (dy+y)*layer->w + dx+x;
	for (int xx=x; xx<destx;)
		{
diff=(*destpic>>24)-233-((*sourpic>>8) & 0xFF)/16;
	if (diff>0)
	{
register int spic=*sourpic;
diff2=((spic & 0xFF) * (*clodpix & 0xFF)) ;
								//| ((((spic & 0x8000)>>8) | ((int)(*((Uint8*)clodpix+1)) & (spic>>8)   ))>>1))
								//*(256-(*shpix))>>8 ;
int AH=diff<<3;
register Uint32 tempdest;
if (diff<9){
	int rev=8-diff;
	diff2=min(255, diff2+ (((*clodpix>>8) & BlueMask)*((*sourpic>>8) & 0xFF)*rev>>11));
	//brightenBlueChannel(&tempdest, ((*clodpix>>8) & BlueMask)*((*sourpic>>8) & 0xFF)>>8 );
    AH+=(rev*diff)*9;
	tempdest=diff2;
	tempdest|=(diff2<<8);
tempdest+=(diff2<<16);
} else {
tempdest=diff2;
tempdest|=((diff2<<7)) & 0xFF00;
tempdest+=0x0800;
}

register int idst=*destpic;
//register int isrc=tempdest;
*destpic= ((RedMask & ((idst & RedMask) + ((int)(((int)(tempdest & RedMask) - (int)(idst & RedMask)) * AH) >>8)))
	| (GreenMask & ((idst & GreenMask) + ((int)(((int)(tempdest & GreenMask) - (int)(idst & GreenMask)) * AH) >>8))) 
	| (BlueMask & ((idst & BlueMask) + ((int)(((int)(tempdest & BlueMask) - (int)(idst & BlueMask)) * AH) >>8))))
	| 0xF0000000;
	;
//}
	
	}
sourpic++;
destpic++;
clodpix++;
xx++;
shpix++;
}
clodpix+=cladd;
shpix+=cladd;
}

}
void BlitNormalAndAlphaInprintShad(SDL_Surface *layer, SDL_Surface *source, UpShadLayer *uplayer, SDL_Surface *upsource, SDL_Surface *heightMask, int dx, int dy, int h)
{
	//if (uplayer->w!=1024) return;
Uint32 * __restrict sourpic,* __restrict upsourpic, * __restrict destpic, * __restrict hpic;
Uint8 * __restrict updestpic;
	int x=0;
	int y=0;
	if (dx<0) x-=dx;
	if (dy<0) y-=dy;
	int destx=source->w;
	int desty=source->h;
	if (dx+destx>layer->w) destx=(layer->w-dx);
	if (dy+desty>layer->h) desty=(layer->h-dy);
Uint8 AH;
int diff;
for (y; y<desty; y++)
{
	sourpic=(Uint32*) source->pixels  + y*source->w + x;
	upsourpic=(Uint32*) upsource->pixels  + y*upsource->w + x;
	hpic=(Uint32*) heightMask->pixels  + y*heightMask->w + x;
	destpic=(Uint32*) layer->pixels  + (dy+y)*layer->w + dx+x;
	updestpic=&uplayer->pix[(dy+y)*tyleSize + dx+x];
	for (int xx=x; xx<destx; xx++)
		{
			
	//*((Uint8 * )destpic+3)=max(*((Uint8 * )sourpic+3),*((Uint8 * )destpic+3));
		diff=(*destpic>>24)-(*hpic>>24)+h;
	if (diff>=0)//(*((Uint8 * )hpic+3)<=*((Uint8 * )destpic+3)+10)
	{
		//diff=abs(diff);
		*((Uint8 * )destpic+3)=*((Uint8*)hpic+3)-h;
		//contactShadow=1;
	//if (*destpic<*sourpic)
	//*destpic=*sourpic;
		if ((diff<32))
		{
			//contactShadow=(diff*8);
			//contactShadow/=255;
			//AH=*((Uint8 * )sourpic+3) * contactShadow;
			AH=(*sourpic>>24)*diff/32;
*updestpic=((((*upsourpic>>24) -  *updestpic) * (AH))/255 + *updestpic);
	
	//(Uint8)((*upsourpic>>24)*AH/256);
/*
*((Uint8 * )updestpic) = (((*((Uint8 * )upsourpic) -  *((Uint8 * )updestpic)) * (AH))/255 + *((Uint8 * )updestpic));
*((Uint8 * )updestpic+1) = (((*((Uint8 * )upsourpic+1) -  *((Uint8 * )updestpic+1)) * (AH))/255 + *((Uint8 * )updestpic+1));
*((Uint8 * )updestpic+2) = (((*((Uint8 * )upsourpic+2) -  *((Uint8 * )updestpic+2)) * (AH))/255 + *((Uint8 * )updestpic+2));
*((Uint8 * )updestpic+3) = (((*((Uint8 * )upsourpic+3) -  *((Uint8 * )updestpic+3)) * (AH))/255 + *((Uint8 * )updestpic+3));*/
		}
		else
		{
//AH=*((Uint8 * )sourpic+3);
AH=*sourpic>>24;
	//contactShadow=1;
*updestpic=(*upsourpic>>24);
		}
	
*((Uint8 * )destpic) = (((*((Uint8 * )sourpic) -  *((Uint8 * )destpic)) * (AH))/255 + *((Uint8 * )destpic));
*((Uint8 * )destpic+1) = (((*((Uint8 * )sourpic+1) -  *((Uint8 * )destpic+1)) * (AH))/255 + *((Uint8 * )destpic+1));
*((Uint8 * )destpic+2) = (((*((Uint8 * )sourpic+2) -  *((Uint8 * )destpic+2)) * (AH))/255 + *((Uint8 * )destpic+2));


	}

//}
sourpic++;
destpic++;
upsourpic++;
updestpic++;
hpic++;
	//if ((x<0) || (y<0) || (x>=screen->w) || (y>=screen->h)) return 0;
    //Uint32 *pixmem32;
    //Uint32 colour;  
 
    //colour = SDL_MapRGB( screen->format, r, g, b );
  
   // pixmem32 = (Uint32*) screen->pixels  + y*screen->w + x;
  //  *pixmem32 = colour;
		}
}
		
}
void BWAITextureDisplacementZoom(SDL_Surface *layer, SDL_Surface *source, SDL_Surface *reflection, int dx, int ddy, int h, int TDy)
{
//height mask should be dinamic
int hcenterx=(1920-scrwi)/2;
int hcentery=(1080-scrhi)/2;
	h=12;
Uint32 * __restrict sourpic, * __restrict destpic, * __restrict clodpix;
int dy=ddy-tyleSize+TDy;
int cly=0;
	int x=0;
	int y=0;
	if (dx<0) x-=dx;
	if (dy<0) y-=dy;  
	if (ddy<0) cly=-ddy;
	y=max(y,tyleSize-TDy);
	int destx=source->w;
	int desty=source->h;
	if (dx+destx>layer->w) destx=(layer->w-dx);
	if (dy+desty>layer->h) desty=(layer->h-dy);
//Uint8 AS;
int diff, diff2;


clodpix=(Uint32*) reflection->pixels +(cly)*reflection->w + x;
int cladd= reflection->w-(destx-x);
int shadd= tyleSize-(destx-x);
for (y; y<desty; y++)
{
	sourpic=(Uint32*) source->pixels  + y*source->w + x;
	destpic=(Uint32*) layer->pixels  + (dy+y)*layer->w + dx+x;
	
	
xx=x;
xx=destx-x;
if (xx>0)
while (xx)
{
int diff=(*destpic>>24)-233;

if (diff>=0)
{
register int spic=*sourpic;
register Uint32 tempdest=diff2=(((spic & 0xFF) * (*clodpix & 0xFF)) | ((((spic & 0x8000)>>8) 
	| ((int)(*((Uint8*)clodpix+1)) & (spic>>8)   ))>>1));
tempdest|=(diff2<<7) & 0xFF00;
tempdest+=0x0800;

//if (diff>>5) *destpic=tempdest;

//else
//{
int AH=diff<<3;
register int idst=*destpic;
register int isrc=tempdest;
*destpic= ((RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * AH) >>8))) 
	| (GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * AH) >>8))) 
	| (BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * AH) >>8))));
//}
//*destpic=((*destpic) & 0x00FFFFFF) | ((*hpic & 0xFF000000) - h*0x01000000 );
}
sourpic++;
destpic++;
clodpix++;
xx--;
}
	clodpix+=cladd;
	
}


dy=ddy+TDy;
y=0;
if (dy<0) y-=dy;
desty=source->h-TDy;
if (dy+desty>layer->h) desty=(layer->h-dy);

//clodpix=(Uint32*) reflection->pixels +(cly)*reflection->w + x;

for (y; y<desty; y++)
{
	sourpic=(Uint32*) source->pixels  + y*source->w + x;
	destpic=(Uint32*) layer->pixels  + (dy+y)*layer->w + dx+x;
	for (int xx=x; xx<destx;)
		{
diff=(*destpic>>24)-233;
	if (diff>=0)
	{
register int spic=*sourpic;
register Uint32 tempdest=diff2=(((spic & 0xFF) * (*clodpix & 0xFF)) | ((((spic & 0x8000)>>8) | ((int)(*((Uint8*)clodpix+1)) & (spic>>8)   ))>>1));
tempdest|=((diff2<<7)) & 0xFF00;
tempdest+=0x0800;


int AH=diff<<3;
register int idst=*destpic;
register int isrc=tempdest;
*destpic= ((RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * AH) >>8))) 
	| (GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * AH) >>8))) 
	| (BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * AH) >>8))));

	//	*destpic=((*destpic) & 0x00FFFFFF) | ((*hpic & 0xFF000000) - h*0x01000000 );
	}

sourpic++;
destpic++;
clodpix++;
xx++;
}
clodpix+=cladd;

}

}


void BlitNormalAndAlphaInprint(SDL_Surface *layer, SDL_Surface *source, SDL_Surface *uplayer, SDL_Surface *upsource, SDL_Surface *heightMask, int dx, int dy, int h)
{
	if (uplayer->w!=1024) return;
Uint32 * __restrict sourpic,* __restrict upsourpic, * __restrict destpic, * __restrict updestpic, * __restrict hpic;
	int x=0;
	int y=0;
	if (dx<0) x-=dx;
	if (dy<0) y-=dy;
	int destx=source->w;
	int desty=source->h;
	if (dx+destx>layer->w) destx=(layer->w-dx);
	if (dy+desty>layer->h) desty=(layer->h-dy);
Uint8 AH;
int diff;
for (y; y<desty; y++)
{
	sourpic=(Uint32*) source->pixels  + y*source->w + x;
	upsourpic=(Uint32*) upsource->pixels  + y*upsource->w + x;
	hpic=(Uint32*) heightMask->pixels  + y*heightMask->w + x;
	destpic=(Uint32*) layer->pixels  + (dy+y)*layer->w + dx+x;
	updestpic=(Uint32*) uplayer->pixels  + (dy+y)*uplayer->w + dx+x;
	for (int xx=x; xx<destx; xx++)
		{
			
	//*((Uint8 * )destpic+3)=max(*((Uint8 * )sourpic+3),*((Uint8 * )destpic+3));
			diff=*((Uint8 * )destpic+3)-*((Uint8 * )hpic+3)+h;
	if (diff>=0)//(*((Uint8 * )hpic+3)<=*((Uint8 * )destpic+3)+10)
	{
		//diff=abs(diff);
		*((Uint8 * )destpic+3)=*((Uint8*)hpic+3)-h;
		//contactShadow=1;
	//if (*destpic<*sourpic)
	//*destpic=*sourpic;
		if (diff<32)
		{
			//contactShadow=(diff*8);
			//contactShadow/=255;
			//AH=*((Uint8 * )sourpic+3) * contactShadow;
AH=(*sourpic>>24)*diff*8/255;
int destAH=(*updestpic>>24);
if (AH)
{
int fullAH=AH+destAH;
*((Uint8 * )updestpic) = (((*((Uint8 * )upsourpic) -  *((Uint8 * )updestpic)) * (AH))/255 + *((Uint8 * )updestpic));
//DoubleAlphaBlit(updestpic, *upsourpic,  AH, destAH);


//*((Uint8 * )updestpic+1)=((*((Uint8 * )updestpic+1)*destAH)+(*((Uint8 * )upsourpic+1)*AH))/fullAH;
//*((Uint8 * )updestpic+2)=((*((Uint8 * )updestpic+2)*destAH)+(*((Uint8 * )upsourpic+2)*AH))/fullAH;
//*((Uint8 * )updestpic+3)=((*((Uint8 * )updestpic+3)*destAH)+(*((Uint8 * )upsourpic+3)*AH))/fullAH;

*((Uint8 * )updestpic+1) = (((*((Uint8 * )upsourpic+1) -  *((Uint8 * )updestpic+1)) * (AH))/255 + *((Uint8 * )updestpic+1));
*((Uint8 * )updestpic+2) = (((*((Uint8 * )upsourpic+2) -  *((Uint8 * )updestpic+2)) * (AH))/255 + *((Uint8 * )updestpic+2));
*((Uint8 * )updestpic+3) = (((*((Uint8 * )upsourpic+3) -  *((Uint8 * )updestpic+3)) * (AH))/255 + *((Uint8 * )updestpic+3));
}
		}
		else
		{
//AH=*((Uint8 * )sourpic+3);
AH=*sourpic>>24;
	//contactShadow=1;
*updestpic=*upsourpic;
		}
	RegularBlit(destpic, *sourpic,  AH);
/*
*((Uint8 * )destpic) = (((*((Uint8 * )sourpic) -  *((Uint8 * )destpic)) * (AH))/255 + *((Uint8 * )destpic));
*((Uint8 * )destpic+1) = (((*((Uint8 * )sourpic+1) -  *((Uint8 * )destpic+1)) * (AH))/255 + *((Uint8 * )destpic+1));
*((Uint8 * )destpic+2) = (((*((Uint8 * )sourpic+2) -  *((Uint8 * )destpic+2)) * (AH))/255 + *((Uint8 * )destpic+2));
*/

	}

//}
sourpic++;
destpic++;
upsourpic++;
updestpic++;
hpic++;
	//if ((x<0) || (y<0) || (x>=screen->w) || (y>=screen->h)) return 0;
    //Uint32 *pixmem32;
    //Uint32 colour;  
 
    //colour = SDL_MapRGB( screen->format, r, g, b );
  
   // pixmem32 = (Uint32*) screen->pixels  + y*screen->w + x;
  //  *pixmem32 = colour;
		}
}
		
}
void BlitNormalAndAlphaInprintScaleShad(SDL_Surface *layer, SDL_Surface *source, UpShadLayer *uplayer, SDL_Surface *upsource, SDL_Surface *heightMask, int dx, int dy, int h, float scale)
{
	//if ((uplayer->w!=1024) || (layer->w!=1024)) return;
Uint32 * __restrict sourpic,* __restrict upsourpic, * __restrict destpic, * __restrict hpic;
Uint8* __restrict updestpic;
	int x=0;
	int y=0;
	if (dx<0) x-=dx;
	if (dy<0) y-=dy;
	int destx=source->w;
	int desty=source->h;
	if (dx+destx>layer->w) destx=(layer->w-dx);
	if (dy+desty>layer->h) desty=(layer->h-dy);
int AH;
int thtwo=32*scale;
float eit=255/thtwo;

float contactShadow;
int diff, heg;
for (y; y<desty; y++)
{
	sourpic=(Uint32*) source->pixels  + y*source->w + x;
	upsourpic=(Uint32*) upsource->pixels  + y*upsource->w + x;
	hpic=(Uint32*) heightMask->pixels  + y*heightMask->w + x;
	destpic=(Uint32*) layer->pixels  + (dy+y)*layer->w + dx+x;
	updestpic=&uplayer->pix[(dy+y)*tyleSize + dx+x];
	for (int xx=x; xx<destx; xx++)
		{
			
	//*((Uint8 * )destpic+3)=max(*((Uint8 * )sourpic+3),*((Uint8 * )destpic+3));

			heg=*((Uint8*)hpic+3)*scale-h;
			diff=*((Uint8 * )destpic+3)-heg;
	if (diff>=0)//(*((Uint8 * )hpic+3)<=*((Uint8 * )destpic+3)+10)
	{

		if ((AH=*((Uint8 * )sourpic+3)))
		//*((Uint8 * )destpic+3)=*((Uint8*)hpic+3)*scale-h;



		//AH=*((Uint8 * )sourpic+3);
		*((Uint8 * )destpic+3)=((heg -  *((Uint8 * )destpic+3)) * (AH))/255 + *((Uint8 * )destpic+3);
		

		if ((diff<thtwo))
		{
			contactShadow=(diff*eit);
			contactShadow/=255;
			AH*= contactShadow;
		}

*updestpic=(Uint8)((*upsourpic>>24)*AH/256);

RegularBlit(destpic, *sourpic, AH);

	}

//}
sourpic++;
destpic++;
upsourpic++;
updestpic++;
hpic++;
	//if ((x<0) || (y<0) || (x>=screen->w) || (y>=screen->h)) return 0;
    //Uint32 *pixmem32;
    //Uint32 colour;  
 
    //colour = SDL_MapRGB( screen->format, r, g, b );
  
   // pixmem32 = (Uint32*) screen->pixels  + y*screen->w + x;
  //  *pixmem32 = colour;
		}
}
		
}
void BlitNormalAndAlphaInprintScale(SDL_Surface *layer, SDL_Surface *source, SDL_Surface *uplayer, SDL_Surface *upsource, SDL_Surface *heightMask, int dx, int dy, int h, float scale)
{
	if ((uplayer->w!=1024) || (layer->w!=1024)) return;
Uint32 * __restrict sourpic,* __restrict upsourpic, * __restrict destpic, * __restrict updestpic, * __restrict hpic;
	int x=0;
	int y=0;
	if (dx<0) x-=dx;
	if (dy<0) y-=dy;
	int destx=source->w;
	int desty=source->h;
	if (dx+destx>layer->w) destx=(layer->w-dx);
	if (dy+desty>layer->h) desty=(layer->h-dy);
Uint8 AH;
int thtwo=32*scale;
float eit=255/thtwo;

float contactShadow;
int diff, heg;
for (y; y<desty; y++)
{
	sourpic=(Uint32*) source->pixels  + y*source->w + x;
	upsourpic=(Uint32*) upsource->pixels  + y*upsource->w + x;
	hpic=(Uint32*) heightMask->pixels  + y*heightMask->w + x;
	destpic=(Uint32*) layer->pixels  + (dy+y)*layer->w + dx+x;
	updestpic=(Uint32*) uplayer->pixels  + (dy+y)*uplayer->w + dx+x;
	for (int xx=x; xx<destx; xx++)
		{

			heg=*((Uint8*)hpic+3)*scale-h;
			diff=*((Uint8 * )destpic+3)-heg;
	if (diff>=0)
	{

		if ((AH=*((Uint8 * )sourpic+3)))

		*((Uint8 * )destpic+3)=((heg -  *((Uint8 * )destpic+3)) * (AH))/255 + *((Uint8 * )destpic+3);
		

		if ((diff<thtwo))
		{
			contactShadow=(diff*eit);
			contactShadow/=255;
			AH*= contactShadow;
*((Uint8 * )updestpic) = (((*((Uint8 * )upsourpic) -  *((Uint8 * )updestpic)) * (AH))/255 + *((Uint8 * )updestpic));
*((Uint8 * )updestpic+1) = (((*((Uint8 * )upsourpic+1) -  *((Uint8 * )updestpic+1)) * (AH))/255 + *((Uint8 * )updestpic+1));
*((Uint8 * )updestpic+2) = (((*((Uint8 * )upsourpic+2) -  *((Uint8 * )updestpic+2)) * (AH))/255 + *((Uint8 * )updestpic+2));
*((Uint8 * )updestpic+3) = (((*((Uint8 * )upsourpic+3) -  *((Uint8 * )updestpic+3)) * (AH))/255 + *((Uint8 * )updestpic+3));
		}
		else
		{

			*((Uint8 * )updestpic) = (((*((Uint8 * )upsourpic) -  *((Uint8 * )updestpic)) * (AH))/255 + *((Uint8 * )updestpic));
*((Uint8 * )updestpic+1) = (((*((Uint8 * )upsourpic+1) -  *((Uint8 * )updestpic+1)) * (AH))/255 + *((Uint8 * )updestpic+1));
*((Uint8 * )updestpic+2) = (((*((Uint8 * )upsourpic+2) -  *((Uint8 * )updestpic+2)) * (AH))/255 + *((Uint8 * )updestpic+2));
*((Uint8 * )updestpic+3) = (((*((Uint8 * )upsourpic+3) -  *((Uint8 * )updestpic+3)) * (AH))/255 + *((Uint8 * )updestpic+3));
		}
	
*((Uint8 * )destpic) = (((*((Uint8 * )sourpic) -  *((Uint8 * )destpic)) * (AH))/255 + *((Uint8 * )destpic));
*((Uint8 * )destpic+1) = (((*((Uint8 * )sourpic+1) -  *((Uint8 * )destpic+1)) * (AH))/255 + *((Uint8 * )destpic+1));
*((Uint8 * )destpic+2) = (((*((Uint8 * )sourpic+2) -  *((Uint8 * )destpic+2)) * (AH))/255 + *((Uint8 * )destpic+2));


	}

//}
sourpic++;
destpic++;
upsourpic++;
updestpic++;
hpic++;
	//if ((x<0) || (y<0) || (x>=screen->w) || (y>=screen->h)) return 0;
    //Uint32 *pixmem32;
    //Uint32 colour;  
 
    //colour = SDL_MapRGB( screen->format, r, g, b );
  
   // pixmem32 = (Uint32*) screen->pixels  + y*screen->w + x;
  //  *pixmem32 = colour;
		}
}
		
}
void BlitNormalAndAlphaInprintScaleFolliageShad(SDL_Surface *layer, SDL_Surface *source, UpShadLayer *uplayer, SDL_Surface *upsource, SDL_Surface *heightMask, int dx, int dy, int h, float scale)
{
//	if ((uplayer->w!=1024) || (layer->w!=1024)) return;
Uint32 * __restrict sourpic,* __restrict upsourpic, * __restrict destpic, * __restrict hpic;
Uint8*   __restrict updestpic;
	int x=0;
	int y=0;
	if (dx<0) x-=dx;
	if (dy<0) y-=dy;
	int destx=source->w;
	int desty=source->h;
	if (dx+destx>layer->w) destx=(layer->w-dx);
	if (dy+desty>layer->h) desty=(layer->h-dy);
Uint8 AH;
int thtwo=32*scale;
float eit=255/thtwo;

float contactShadow;
int diff, heg;
for (y; y<desty; y++)
{
	sourpic=(Uint32*) source->pixels  + y*source->w + x;
	upsourpic=(Uint32*) upsource->pixels  + y*upsource->w + x;
	hpic=(Uint32*) heightMask->pixels  + y*heightMask->w + x;
	destpic=(Uint32*) layer->pixels  + (dy+y)*layer->w + dx+x;
	updestpic=&uplayer->pix[(dy+y)*tyleSize + dx+x];
	for (int xx=x; xx<destx; xx++)
		{

			heg=*((Uint8*)hpic+3)*scale-h;
			diff=255-heg;
	if (diff>=0)
	{

		if (AH=*((Uint8 * )sourpic+3))
		{
		*((Uint8 * )destpic+3)=max(0,*((Uint8 * )destpic+3)-diff);
		//((heg -  *((Uint8 * )destpic+3)) * (AH))/255 + *((Uint8 * )destpic+3);
		

		if ((diff<thtwo))
		{
			contactShadow=(diff*eit);
			contactShadow/=255;
			AH*= contactShadow;
		}

*(updestpic) = (((*upsourpic>>24) -  *updestpic) * (AH))/255 + *updestpic;


register int idst = *destpic;
register int isrc = *sourpic;
*destpic = ((RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * AH) >>8))) | (GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * AH) >>8))) | (BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * AH) >>8))) | (idst & 0xFF000000) );


	}
	}
//}
sourpic++;
destpic++;
upsourpic++;
updestpic++;
hpic++;
	//if ((x<0) || (y<0) || (x>=screen->w) || (y>=screen->h)) return 0;
    //Uint32 *pixmem32;
    //Uint32 colour;  
 
    //colour = SDL_MapRGB( screen->format, r, g, b );
  
   // pixmem32 = (Uint32*) screen->pixels  + y*screen->w + x;
  //  *pixmem32 = colour;
		}
}
		
}
void BlitNormalAndAlphaInprintScaleFolliage(SDL_Surface *layer, SDL_Surface *source, SDL_Surface *uplayer, SDL_Surface *upsource, SDL_Surface *heightMask, int dx, int dy, int h, float scale)
{
	if ((uplayer->w!=1024) || (layer->w!=1024)) return;
Uint32 * __restrict sourpic,* __restrict upsourpic, * __restrict destpic, * __restrict updestpic, * __restrict hpic;
	int x=0;
	int y=0;
	if (dx<0) x-=dx;
	if (dy<0) y-=dy;
	int destx=source->w;
	int desty=source->h;
	if (dx+destx>layer->w) destx=(layer->w-dx);
	if (dy+desty>layer->h) desty=(layer->h-dy);
Uint8 AH;
int thtwo=32*scale;
float eit=255/thtwo;

float contactShadow;
int diff, heg;
for (y; y<desty; y++)
{
	sourpic=(Uint32*) source->pixels  + y*source->w + x;
	upsourpic=(Uint32*) upsource->pixels  + y*upsource->w + x;
	hpic=(Uint32*) heightMask->pixels  + y*heightMask->w + x;
	destpic=(Uint32*) layer->pixels  + (dy+y)*layer->w + dx+x;
	updestpic=(Uint32*) uplayer->pixels  + (dy+y)*uplayer->w + dx+x;
	for (int xx=x; xx<destx; xx++)
		{

			heg=*((Uint8*)hpic+3)*scale-h;
			diff=255-heg;
	if (diff>=0)
	{

		if (AH=*((Uint8 * )sourpic+3))
		{
		*((Uint8 * )destpic+3)=max(0,*((Uint8 * )destpic+3)-diff);


		if ((diff<thtwo))
		{
			contactShadow=(diff*eit);
			contactShadow/=255;
			AH*= contactShadow;
		}

*((Uint8 * )updestpic) = (((*((Uint8 * )upsourpic) -  *((Uint8 * )updestpic)) * (AH))/255 + *((Uint8 * )updestpic));

register int idst = *updestpic;
register int isrc = *upsourpic;
*updestpic = ((RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * AH) >>8))) | (GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * AH) >>8))) | (BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * AH) >>8))) | (idst & 0xFF000000) );
	
idst = *destpic;
isrc = *sourpic;
*destpic = ((RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * AH) >>8))) | (GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * AH) >>8))) | (BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * AH) >>8))) | (idst & 0xFF000000) );


	}
	}
//}
sourpic++;
destpic++;
upsourpic++;
updestpic++;
hpic++;
	//if ((x<0) || (y<0) || (x>=screen->w) || (y>=screen->h)) return 0;
    //Uint32 *pixmem32;
    //Uint32 colour;  
 
    //colour = SDL_MapRGB( screen->format, r, g, b );
  
   // pixmem32 = (Uint32*) screen->pixels  + y*screen->w + x;
  //  *pixmem32 = colour;
		}
}
		
}
int CalculateHill(Uint32 * bg)
{
int desth=*bg>>24;
int side=(*(bg+1)>>24)-desth;
int sum=-side*(side<0)*side;

side=(*(bg-1)>>24)-desth;
sum-=side*(side<0)*side;

side=(*(bg-1024)>>24)-desth;
sum-=side*(side<0)*side;

side=(*(bg+1024)>>24)-desth;
sum-=side*(side<0)*side;

side=(*(bg-1023)>>24)-desth;
sum-=side*(side<0)*side;

side=(*(bg+1023)>>24)-desth;
sum-=side*(side<0)*side;

side=(*(bg-1025)>>24)-desth;
sum-=side*(side<0)*side;

side=(*(bg+1025)>>24)-desth;
sum-=side*(side<0)*side;
sum+=2;
return sum*(sum<0);
}
void FlatSurfaceFolliage(SDL_Surface *layer, SDL_Surface *source, UpShadLayer *uplayer, SDL_Surface *upsource, SDL_Surface *heightMask, int dx, int dy, int h, float scale)
{
//	if ((uplayer->w!=1024) || (layer->w!=1024)) return;
Uint32 * __restrict sourpic,* __restrict upsourpic, * __restrict destpic,  * __restrict hpic;
Uint8* __restrict updestpic;
	int x=0;
	int y=0;
	if (dx<0) x-=dx;
	if (dy<0) y-=dy;
	int destx=source->w;
	int desty=source->h;
	if (dx+destx>layer->w) destx=(layer->w-dx);
	if (dy+desty>layer->h) desty=(layer->h-dy);
Uint8 AH;
int prevA=0;
int thtwo=32*scale;
float eit=255/thtwo;

float contactShadow;
int diff, heg;
y++;
x++;
for (y; y<desty-1; y++)
{
	sourpic=(Uint32*) source->pixels  + y*source->w + x;
	upsourpic=(Uint32*) upsource->pixels  + y*upsource->w + x;
	hpic=(Uint32*) heightMask->pixels  + y*heightMask->w + x;
	destpic=(Uint32*) layer->pixels  + (dy+y)*layer->w + dx+x;
	updestpic=&uplayer->pix[(dy+y)*tyleSize + dx+x];
	for (int xx=x; xx<destx-1; xx++)
		{
AH=0;
			heg=*((Uint8*)hpic+3)*scale-h;
			diff=255-heg;
	if (diff>=0)
	{
		AH=*((Uint8 * )sourpic+3);

		if (AH)
		{
		*((Uint8 * )destpic+3)=max(0,*((Uint8 * )destpic+3)-diff);
	

		if ((diff<thtwo))
		{
			contactShadow=(diff*eit);
			contactShadow/=255;
			AH*= contactShadow;
			if (AH<5) goto skipBlit;
		}

		AH=max(AH*(30+CalculateHill(destpic))/30,prevA/2);//0.5
		AH=min(AH,prevA+AH/10);//0.12
if (!AH) goto skipBlit;


*updestpic = (((*upsourpic>>24) -  *updestpic) * (AH))/255 + *updestpic;


	
register int  idst = *destpic;
register int  isrc = *sourpic;
*destpic = ((RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * AH) >>8))) |
(GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * AH) >>8))) |
(BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * AH) >>8))) | (idst & 0xFF000000) );

		} 
	} 

skipBlit:
//}
prevA=AH;
sourpic++;
destpic++;
upsourpic++;
updestpic++;
hpic++;
	//if ((x<0) || (y<0) || (x>=screen->w) || (y>=screen->h)) return 0;
    //Uint32 *pixmem32;
    //Uint32 colour;  
 
    //colour = SDL_MapRGB( screen->format, r, g, b );
  
   // pixmem32 = (Uint32*) screen->pixels  + y*screen->w + x;
  //  *pixmem32 = colour;
		}
}
		
}


void BlitWithAlphaInprint(SDL_Surface *layer, SDL_Surface *source, SDL_Surface *heightMask, int dx, int dy, int h)
{
Uint32 * __restrict sourpic, * __restrict destpic, * __restrict hpic;

if (!heightMask) return;

	int x=0;
	int y=0;
	if (dx<0) x-=dx;
	if (dy<0) y-=dy;
	int destx=source->w;
	int desty=source->h;
	if (dx+destx>layer->w) destx=(layer->w-dx);
	if (dy+desty>layer->h) desty=(layer->h-dy);
int AH;
//float contactShadow;
int diff;

sourpic=(Uint32*) source->pixels  + y*source->w + x;
hpic=(Uint32*) heightMask->pixels  + y*heightMask->w + x;
destpic=(Uint32*) layer->pixels  + (dy+y)*layer->w + dx+x;

int spNL=source->w+x-destx;
int dpNL=layer->w-destx+x;
for (y; y<desty; y++)
{
	

	
	for (int xx=x; xx<destx;)
		{
		diff=(*destpic>>24)-(*hpic>>24)+h;
	if (diff>0)
	{
		AH=*sourpic>>24;
		if ((diff<32))
			AH= AH * (diff<<3) / 255;
		
register int idst = *destpic;
register int isrc = *sourpic;
*destpic = ((RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * AH) >>8))) | (GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * AH) >>8))) | (BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * AH) >>8))));
*((Uint8 * )destpic+3)=*((Uint8*)hpic+3)-h;	
	}

sourpic++;
destpic++;
hpic++;
xx++;

		}
	sourpic+=spNL;
	hpic+=spNL;
	destpic+=dpNL;
}
		
}
void BlitWithAlphaInprintNoCantactSoftness(SDL_Surface *layer, SDL_Surface *source, SDL_Surface *heightMask, int dx, int dy, int h)
{
Uint32 * __restrict sourpic, * __restrict destpic, * __restrict hpic;

	int x=0;
	int y=0;
	if (dx<0) x-=dx;
	if (dy<0) y-=dy;
	int destx=source->w;
	int desty=source->h;
	if (dx+destx>layer->w) destx=(layer->w-dx);
	if (dy+desty>layer->h) desty=(layer->h-dy);
int AH;
//float contactShadow;
int diff;

sourpic=(Uint32*) source->pixels  + y*source->w + x;
hpic=(Uint32*) heightMask->pixels  + y*heightMask->w + x;
destpic=(Uint32*) layer->pixels  + (dy+y)*layer->w + dx+x;

int spNL=source->w+x-destx;
int dpNL=layer->w-destx+x;
for (y; y<desty; y++)
{
	

	
	for (int xx=x; xx<destx;)
		{
		AH=*sourpic>>24;
		if (AH)
		{		
register int idst = *destpic;
register int isrc = *sourpic;
Uint32 tpic=max(min(*sourpic, *hpic),*destpic);
*destpic = ((RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * AH) >>8)))
| (GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * AH) >>8))) 
| (BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * AH) >>8))))
| (tpic & AlphaMask);
//*((Uint8 * )destpic+3)=*((Uint8*)hpic+3)-h;	
		}	
sourpic++;
destpic++;
hpic++;
xx++;

		}
	sourpic+=spNL;
	hpic+=spNL;
	destpic+=dpNL;
}
		
}
void BlitWithAlphaInprintScale(SDL_Surface *layer, SDL_Surface *source, SDL_Surface *heightMask, int dx, int dy, int h, float scale)
{
//float scale=0.2;
Uint32 * __restrict sourpic, * __restrict destpic, * __restrict hpic;

	int x=0;
	int y=0;
	if (dx<0) x-=dx;
	if (dy<0) y-=dy;
	int destx=source->w;
	int desty=source->h;
	if (dx+destx>layer->w) destx=(layer->w-dx);
	if (dy+desty>layer->h) desty=(layer->h-dy);
Uint8 AH;
float contactShadow;
int diff;

sourpic=(Uint32*) source->pixels  + y*source->w + x;
hpic=(Uint32*) heightMask->pixels  + y*heightMask->w + x;
destpic=(Uint32*) layer->pixels  + (dy+y)*layer->w + dx+x;

int spNL=source->w+x-destx;
int dpNL=layer->w-destx+x;
for (y; y<desty; y++)
{
	

	
	for (int xx=x; xx<destx;)
		{
			diff=(*destpic>>24)-(*hpic>>24)*scale+h;
	if (diff>=0)
	{
		if ((diff>=32))
			AH=*((Uint8 * )sourpic+3);
		else
		{
			contactShadow=(diff*8);
			contactShadow/=255;
			AH=*((Uint8 * )sourpic+3) * contactShadow;
		}	
register int idst = *destpic;
register int isrc = *sourpic;
*destpic = ((RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * AH) >>8))) | (GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * AH) >>8))) | (BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * AH) >>8))));

//if (AH>=8) 
*((Uint8 * )destpic+3)=-h+*((Uint8*)hpic+3)*scale;	
	}

sourpic++;
destpic++;
hpic++;
xx++;

		}
	sourpic+=spNL;
	hpic+=spNL;
	destpic+=dpNL;
}
		
}

//Blit only some parts but Inprint without h
bool LightSpotEyeEffect (int offx, int offy, SDL_Surface *temp, int Galpha)
{
//SDL_Surface *temp = image[no];  
if ((offx+temp->w<0) || (offy+temp->h<0) || (offx>scrwi) || (offy>scrhi)) return 0; 
//register int idst;
//register int isrc;
Uint32 * __restrict dar;
Uint32 * __restrict sar;
SDL_Rect src;
src.x=max(0,-offx); 
src.y=max(0,-offy);
src.w=min(temp->w,scrwi-offx);
src.h=min(temp->h,scrhi-offy);
int alpha;
//alpha=20;
for (int yy=src.y; yy<src.h; yy++)
	{
//AH=(Uint32*)alphasource->pixels + yy*alphasource->w + src.x;
dar=(Uint32*)screen->pixels  + (offy+yy)*screen->w + offx + src.x;
sar=(Uint32*)temp->pixels  + (yy)*temp->w+src.x;
for (int xx=src.x; xx<src.w; )
		{
alpha=((*sar>>24)*Galpha)>>8;
if (alpha)

{
register int idst = *dar;
register int isrc = *sar;
//Uint32 mix=
*dar = (max(idst & RedMask, RedMask & (((isrc & RedMask) * alpha) >>8))) | (*dar & 0xFF000000);
*dar|= (max(idst & GreenMask, GreenMask & (((isrc & GreenMask) * alpha) >>8)));
*dar|= (max(idst & BlueMask, BlueMask & (((isrc & BlueMask) * alpha) >>8)));
	//| (GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * alpha) >>8))) | (BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * alpha) >>8))));
}

xx++; sar+=1; dar+=1;
		}
	}
return 1;
}
// use light color only, like smoke
Uint8* ImageAlphaToUint8Mask(SDL_Surface* image)
{
	int size=image->w*image->h;
	Uint8* mask=new Uint8[size];
	Uint32* pix=(Uint32*)image->pixels;
	Uint8* destPix=mask;
	while (size){
	*destPix=*pix>>24;
	destPix++; pix++;
	size--;
	}
return mask;
}

void VolumetricSmokeBlitBorder(SDL_Surface *layer, SDL_Surface *source,SDL_Surface *heightMask, int dx, int dy, int h, int ls)
{
	ls+=16;
//	h-=128;
Uint32 * __restrict sourpic, * __restrict destpic, * __restrict hpic, * __restrict dark;

	int x=0;
	int y=0;
	if (dx<0) x-=dx;
	if (dy<0) y-=dy;
	int destx=source->w;
	int desty=source->h;
	if (dx+destx>layer->w) destx=(layer->w-dx);
	if (dy+desty>layer->h) desty=(layer->h-dy);
int AH;
//float contactShadow;
//int diff;

sourpic=(Uint32*) source->pixels  + y*source->w + x;
hpic=(Uint32*) heightMask->pixels  + y*heightMask->w + x;
destpic=(Uint32*) layer->pixels  + (dy+y)*layer->w + dx+x;
dark=(Uint32*)darkmask+ (dy+y)*layer->w + dx+x;

int spNL=source->w+x-destx;
int dpNL=layer->w-destx+x;
for (y; y<desty; y++)
{
	for (int xx=x; xx<destx;)
		{
			AH=(((*destpic>>24)*(*sourpic>>24))>>8)+h-(*hpic>>24);
	if (AH>=0)
	{
Uint32 col=FlameDarkToungsColor;
brightenColor(&col, *dark>>16);//>>ls
register int idst = *destpic;
register int isrc = col;//*sourpic;
*destpic = ((RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * AH) >>8))) | (GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * AH) >>8))) | (BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * AH) >>8)))) | (*destpic & 0xFF000000);


	}

sourpic++;
destpic++;
dark++;
hpic++;
xx++;

		}
	sourpic+=spNL;
	hpic+=spNL;
	destpic+=dpNL;
	dark+=dpNL;
}
		
}
void VolumetricSmokeBlit(SDL_Surface *layer, SDL_Surface *source,SDL_Surface *heightMask, int dx, int dy, int h, int ls)
{
	ls+=16;
//	h-=128;
Uint32 * __restrict sourpic, * __restrict destpic, * __restrict hpic, * __restrict dark;

	int x=0;
	int y=0;
	if (dx<0) x-=dx;
	if (dy<0) y-=dy;
	int destx=source->w;
	int desty=source->h;
	if (dx+destx>layer->w) destx=(layer->w-dx);
	if (dy+desty>layer->h) desty=(layer->h-dy);
int AH;
//float contactShadow;
//int diff;

sourpic=(Uint32*) source->pixels  + y*source->w + x;
hpic=(Uint32*) heightMask->pixels  + y*heightMask->w + x;
destpic=(Uint32*) layer->pixels  + (dy+y)*layer->w + dx+x;
dark=(Uint32*)darkmask+ (dy+y)*layer->w + dx+x;


int spNL=source->w+x-destx;
int dpNL=layer->w-destx+x;
for (y; y<desty; y++)
{
	for (int xx=x; xx<destx;)
		{
			//AH=(((*destpic>>24)*(*destpic>>24)*(*sourpic>>24))>>16)+h-(*hpic>>24);
			int diff=(*destpic>>24)+h-(*hpic>>24);
AH=(((*destpic>>24)*(*sourpic>>24))>>8)+h-(*hpic>>24); 
				if (AH>=0)
	{
if (AH<=64) 	
	//AH=*sourpic>>24;
AH=(AH*diff)>>8;

		//if ((diff>=32))
		//{
			//AH=*sourpic>>24;
//			if (AH>64) 
//*((Uint8 * )destpic+3)=*((Uint8*)hpic+3);	
//		}
//		else
//		{
			//contactShadow=(diff*8);
			//contactShadow/=255;
			//AH=AH * diff / 255;
//		}	
Uint32 col=FlameDarkToungsColor;//*sourpic;//FlameDarkToungsColor;
brightenColor(&col, *dark>>16);
//MultiplyBy(*dark>>16, &col);
register int idst = *destpic;
register int isrc = col;//*sourpic;
*destpic = ((RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * AH) >>8))) | (GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * AH) >>8))) | (BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * AH) >>8)))) | (*destpic & 0xFF000000);
*dark=(*dark & 0xFFFF) | (((*dark>>16)*(1024-(AH))/1024) << 16);

	}

sourpic++;
destpic++;
dark++;
hpic++;
xx++;

		}
	sourpic+=spNL;
	hpic+=spNL;
	destpic+=dpNL;
	dark+=dpNL;
}
		
}
void VolOverlapSmokeBlit(DirectionalLightetImage* bigIMG, DirectionalLightetImage* smlIMG, int* dx, int* dy, int offx, int offy, int color)
{
	if (*dx<0) *dx+=bigIMG->w/2;//bigIMG->w/2-smlIMG->w+*dx;
	if (*dx+smlIMG->w > bigIMG->w) *dx-=bigIMG->w/2;//*dx+smlIMG->w-bigIMG->w; 

	if (*dy<0) *dy+=bigIMG->h/2;//=bigIMG->h-smlIMG->h+*dy;
	if (*dy+smlIMG->h > bigIMG->h) *dy-=bigIMG->h/2;//=*dy+smlIMG->h-bigIMG->h;

	Uint8* __restrict SMpix;
	Uint8* __restrict BGpix;
	Uint32* __restrict scrpix;

ScreenFitDrowFrame* frm=GenerateFrame (screen->w, screen->h, smlIMG->w, smlIMG->h, offx, offy);
int bigJump=bigIMG->w-frm->tox+frm->fromx;

for (int j=frm->fromy; j<frm->toy; j++)
{
int xoz=frm->tox-frm->fromx;
scrpix=(Uint32*)screen->pixels+(offy+j)*scrwi+offx+frm->fromx; 
SMpix=smlIMG->DnA+j*smlIMG->w+frm->fromx;
BGpix=bigIMG->DnA+(*dy+j)*bigIMG->w+frm->fromx+*dx;
int AH=0;
while (xoz)
{

//AH=*SMpix;
//AH*=*BGpix;AH>>=8;
int diff=*SMpix-*BGpix;
if (diff>=0)
{
AH=255-(diff)+(128*(!diff));
register int idst=*scrpix;
*scrpix = ((RedMask & ((idst & RedMask) + ((int)(((int)(color & RedMask) - (int)(idst & RedMask)) * AH) >>8)))
			| (GreenMask & ((idst & GreenMask) + ((int)(((int)(color & GreenMask) - (int)(idst & GreenMask)) * AH) >>8))) 
			| (BlueMask & ((idst & BlueMask) + ((int)(((int)(color & BlueMask) - (int)(idst & BlueMask)) * AH) >>8))))
			| (*scrpix & AlphaMask);
}
scrpix++; SMpix++; BGpix++;	xoz--;
}


}


}

void VolumetricLightBlitBorder(SDL_Surface *layer, SDL_Surface *source, int dx, int dy, int h, int ls)
{
	ls+=17;
//	h-=128;
Uint32 * __restrict sourpic, * __restrict destpic,  * __restrict dark;

	int x=0;
	int y=0;
	if (dx<0) x-=dx;
	if (dy<0) y-=dy;
	int destx=source->w;
	int desty=source->h;
	if (dx+destx>layer->w) destx=(layer->w-dx);
	if (dy+desty>layer->h) desty=(layer->h-dy);
int AH;
//float contactShadow;
//int diff;

sourpic=(Uint32*) source->pixels  + y*source->w + x;
destpic=(Uint32*) layer->pixels  + (dy+y)*layer->w + dx+x;
dark=(Uint32*)darkmask+ (dy+y)*layer->w + dx+x;

int spNL=source->w+x-destx;
int dpNL=layer->w-destx+x;
for (y; y<desty; y++)
{
	for (int xx=x; xx<destx;)
		{
			AH=(*sourpic>>24)+h;
	if (AH>=0)
	{
AH=(*dark>>15)*AH/255;
MultiplyBy(AH+256, destpic);
//Uint32 col=0;
/*brightenColor(&col, *dark>>ls);
register int idst = *destpic;
register int isrc = col;//*sourpic;
*destpic = ((RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * AH) >>8))) | (GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * AH) >>8))) | (BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * AH) >>8)))) | (*destpic & 0xFF000000);
*/

	}

sourpic++;
destpic++;
dark++;
xx++;

		}
	sourpic+=spNL;
	
	destpic+=dpNL;
	dark+=dpNL;
}
		
}
void VolumetricLightBlit(SDL_Surface *layer, SDL_Surface *source, int dx, int dy )
{
Uint32 * __restrict sourpic, * __restrict destpic, * __restrict dark;

	int x=0;
	int y=0;
	if (dx<0) x-=dx;
	if (dy<0) y-=dy;
	int destx=source->w;
	int desty=source->h;
	if (dx+destx>layer->w) destx=(layer->w-dx);
	if (dy+desty>layer->h) desty=(layer->h-dy);
int AH;
//float contactShadow;
//int diff;

sourpic=(Uint32*) source->pixels  + y*source->w + x;
destpic=(Uint32*) layer->pixels  + (dy+y)*layer->w + dx+x;
dark=(Uint32*)darkmask+ (dy+y)*layer->w + dx+x;


int spNL=source->w+x-destx;
int dpNL=layer->w-destx+x;
for (y; y<desty; y++)
{
	for (int xx=x; xx<destx;)
		{
			//AH=(((*destpic>>24)*(*destpic>>24)*(*sourpic>>24))>>16)+h-(*hpic>>24);
			//int diff=(*destpic>>24)+h-(*hpic>>24);
//AH=(((*destpic>>24)*(*sourpic>>24))>>8)+h+(*hpic>>24)-255; 
Uint32 AH=*sourpic>>24;
if (AH)
	{
AH=(*dark>>15)*AH/255;
MultiplyBy(AH+256, destpic);

		/*
AH=(*dark>>16)*AH/256;
if (AH>0)
brightenColor(destpic, min(AH,255));*/
/*
Uint32 col=0;
brightenColor(&col, *dark>>ls);
register int idst = *destpic;
register int isrc = col;//*sourpic;
*destpic = ((RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * AH) >>8))) | (GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * AH) >>8))) | (BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * AH) >>8)))) | (*destpic & 0xFF000000);
*/

	}

sourpic++;
destpic++;
dark++;

xx++;

		}
	sourpic+=spNL;

	destpic+=dpNL;
	dark+=dpNL;
}
		
}

void VolumetricImageNLightBlitBorder(SDL_Surface *layer, SDL_Surface *source, int dx, int dy, int h, int ls)
{
	ls+=16;
//	h-=128;
	h=min(h,0);
Uint32 * __restrict sourpic, * __restrict destpic, * __restrict dark;

	int x=0;
	int y=0;
	if (dx<0) x-=dx;
	if (dy<0) y-=dy;
	int destx=source->w;
	int desty=source->h;
	if (dx+destx>layer->w) destx=(layer->w-dx);
	if (dy+desty>layer->h) desty=(layer->h-dy);
int AH;
//float contactShadow;
//int diff;


sourpic=(Uint32*) source->pixels  + y*source->w + x;
destpic=(Uint32*) layer->pixels  + (dy+y)*layer->w + dx+x;
dark=(Uint32*)darkmask+ (dy+y)*layer->w + dx+x;

int spNL=source->w+x-destx;
int dpNL=layer->w-destx+x;
for (y; y<desty; y++)
{
	for (int xx=x; xx<destx;)
		{
			//AH=(((*destpic>>24)*(*sourpic>>24))>>8)+h;
			AH=min(((int)(*sourpic>>24)*((int)(*dark>>15)-512))/255,255);
	if (AH>0)
	{

Uint32 col=*sourpic;
//brightenColor(&col, *dark>>16);//>>ls
MultiplyBy(*dark>>15,&col);
//MultiplyFloatColor(*dark>>16,&col);
register int idst = *destpic;
register int isrc = col;//*sourpic;
*destpic = ((RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * AH) >>8))) | (GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * AH) >>8))) | (BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * AH) >>8)))) | (*destpic & 0xFF000000);


	}

sourpic++;
destpic++;
dark++;
xx++;

		}
	sourpic+=spNL;
	destpic+=dpNL;
	dark+=dpNL;
}
		
}

void VolumetricFireMovingSectorMaskBlit(SDL_Surface *layer, SDL_Surface *source, int sourcex, int sourcey, SDL_Surface *heightMask, int dx, int dy, int h)
{
//	h-=128;
Uint32 * __restrict sourpic, * __restrict destpic, * __restrict hpic;

	int x=0;
	int y=0;
	if (dx<0) x-=dx;
	if (dy<0) y-=dy;
	int destx=source->w;
	int desty=source->h;
	if (dx+destx>layer->w) destx=(layer->w-dx);
	if (dy+desty>layer->h) desty=(layer->h-dy);
int AH;
//float contactShadow;
//int diff;

sourpic=(Uint32*) source->pixels  + y*source->w + x;
hpic=(Uint32*) heightMask->pixels  + y*heightMask->w + x;
destpic=(Uint32*) layer->pixels  + (dy+y)*layer->w + dx+x;

int spNL=source->w+x-destx;
int dpNL=layer->w-destx+x;
for (y; y<desty; y++)
{
	for (int xx=x; xx<destx;)
		{
			AH=(((*destpic>>24)*(*sourpic>>24))>>8)+h-(*hpic>>24);
	if (AH>=0)
	{
		//if ((diff>=32))
		//{
			//AH=*sourpic>>24;
//			if (AH>64) 
//*((Uint8 * )destpic+3)=*((Uint8*)hpic+3);	
//		}
//		else
//		{
			//contactShadow=(diff*8);
			//contactShadow/=255;
			//AH=AH * diff / 255;
//		}	
register int idst = *destpic;
register int isrc = *sourpic;
*destpic = ((RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * AH) >>8))) | (GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * AH) >>8))) | (BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * AH) >>8)))) | (idst & 0xFF000000);


	}

sourpic++;
destpic++;
hpic++;
xx++;

		}
	sourpic+=spNL;
	hpic+=spNL;
	destpic+=dpNL;
}
		
}

void EffectBlitWithAlphaInprint(SDL_Surface *layer, SDL_Surface *source, SDL_Surface *heightMask, int dx, int dy, int h)
{
//	h-=128;
Uint32 * __restrict sourpic, * __restrict destpic, * __restrict hpic;

	int x=0;
	int y=0;
	if (dx<0) x-=dx;
	if (dy<0) y-=dy;
	int destx=source->w;
	int desty=source->h;
	if (dx+destx>layer->w) destx=(layer->w-dx);
	if (dy+desty>layer->h) desty=(layer->h-dy);
Uint8 AH;
int diff;

sourpic=(Uint32*) source->pixels  + y*source->w + x;
hpic=(Uint32*) heightMask->pixels  + y*heightMask->w + x;
destpic=(Uint32*) layer->pixels  + (dy+y)*layer->w + dx+x;

int spNL=source->w+x-destx;
int dpNL=layer->w-destx+x;
for (y; y<desty; y++)
{
	

	
	for (int xx=x; xx<destx;)
		{
			diff=*((Uint8 * )destpic+3)-*((Uint8 * )hpic+3)+h;
	if (diff>=0)
	{
		//if ((diff>=32))
		//{
			AH=*sourpic>>24;
//			if (AH>64) 
//*((Uint8 * )destpic+3)=*((Uint8*)hpic+3);	
//		}
//		else
//		{
			//contactShadow=(diff*8);
			//contactShadow/=255;
			AH=AH * diff / 255;
//		}	
register int idst = *destpic;
register int isrc = *sourpic;
*destpic = ((RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * AH) >>8))) | (GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * AH) >>8))) | (BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * AH) >>8)))) | (idst & 0xFF000000);


	}

sourpic++;
destpic++;
hpic++;
xx++;

		}
	sourpic+=spNL;
	hpic+=spNL;
	destpic+=dpNL;
}
		
}

void myRedblit (int offx, int offy, int h, int no)
{
SDL_Surface *temp = image[no];  
h/=dddstength;
offx+=((offx-scrwid)*h)>>16;
offy+=((offy-scrhid)*h)>>16;
if ((offx+temp->w<0) || (offy+temp->h<0) || (offx>scrwi) || (offy>scrhi)) return; 
register int idst;
register int isrc;
Uint32 * dar;
Uint32 * sar;
SDL_Rect src;
src.x=max(0,-offx); 
src.y=max(0,-offy);
src.w=min(temp->w,scrwi-offx);
src.h=min(temp->h,scrhi-offy);
int alpha;
//alpha=20;
for (int yy=src.y; yy<src.h; yy++)
	{
//AH=(Uint32*)alphasource->pixels + yy*alphasource->w + src.x;
dar=(Uint32*)screen->pixels  + (offy+yy)*screen->w + offx + src.x;
sar=(Uint32*)temp->pixels  + (yy)*temp->w+src.x;
//alpha=*sar>>24;
for (int xx=src.x; xx<src.w; xx++, sar+=1, dar+=1)
		{
alpha=*sar>>24;
idst = *dar;
isrc = *sar;
*dar = ((RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * alpha) >>8))) | (GreenMask & (int)(idst & GreenMask)*(255-alpha) >>8) | (BlueMask & (int)(idst & BlueMask)*(255-alpha) >>8));
		}
	}

}
void myblit (int offx, int offy, SDL_Surface *temp, LRSurfaceInd * index) // only for caved backgrounds
{
	if (!index->indexed) return;
offsets lava;
lava.x=((offx+tyleSize/2)-scrwid)/1.2;
lava.x=lava.x+scrwid-tyleSize;
if (offx<0) lava.x=-lava.x;
else lava.x=offx-lava.x;
//lava.x=max(0, -lava.x);

lava.y=((offy+tyleSize/2)-scrhid)/1.2;
lava.y=lava.y+scrhid-tyleSize;
if (offy<0) lava.y=-lava.y;
else lava.y=offy-lava.y;

//SDL_Surface *temp = image[no];  
if ((offx+temp->w<0) || (offy+temp->h<0) || (offx>scrwi) || (offy>scrhi)) return; 
//register int idst;
//register int isrc;
Uint32 * __restrict dar;
Uint32 * __restrict sar;
Uint32 * __restrict lav;
SDL_Rect src;
src.x=max(0,-offx); 
src.y=max(0,-offy);
src.w=min(temp->w,scrwi-offx);
src.h=min(temp->h,scrhi-offy);
int alpha;
//alpha=20;
for (int yy=src.y; yy<src.h; yy++)
	{
		int tempx=max(src.x, index->IMGindexStart[yy]);
		int tempw=min(src.w, index->IMGindexEnd[yy]);
//AH=(Uint32*)alphasource->pixels + yy*alphasource->w + src.x;
dar=(Uint32*)screen->pixels  + (offy+yy)*screen->w + offx + tempx;
sar=(Uint32*)temp->pixels  + (yy)*temp->w+tempx;
lav=(Uint32*)mergeimage[1501]->pixels+lava.y*(tyleSize)*2+lava.x+(tempx-src.x);
lava.y++;
 

for (int xx=tempx; xx<tempw; )
		{
			alpha=*sar>>24;
			//alpha=255-alpha;
			//alpha=20;
			//if (alpha>200)
			  // *dar=*sar;
			//else
			//if (alpha & 0x000020)
			//{
if (alpha>224)//==0xFF)
*dar=(*lav & deAlphaMask) | (*dar & AlphaMask);
/*else
if (alpha<0xF8)
*dar=*sar;
else
{
	alpha=(7-(alpha-0xF8))*32;
register int idst = *lav;
register int isrc = *sar;

*dar = ((RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * alpha) >>8))) |
	(GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * alpha) >>8))) |
	(BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * alpha) >>8)))) |
	(*sar & AlphaMask);
}*/

xx++; sar++; dar++; lav++;
		}
	}

}

void myIndexedMemCopy (int offx, int offy, SDL_Surface *temp, LRSurfaceInd * index) // only for caved backgrounds
{
	if (!index->indexed) return;
offsets lava;
lava.x=((offx+tyleSize/2)-scrwid)/1.2;
lava.x=lava.x+scrwid-tyleSize;
if (offx<0) lava.x=-lava.x;
else lava.x=offx-lava.x;
//lava.x=max(0, -lava.x);

lava.y=((offy+tyleSize/2)-scrhid)/1.2;
lava.y=lava.y+scrhid-tyleSize;
if (offy<0) lava.y=-lava.y;
else lava.y=offy-lava.y;
 
if ((offx+temp->w<0) || (offy+temp->h<0) || (offx>scrwi) || (offy>scrhi)) return; 

Uint32 * __restrict dar;
Uint32 * __restrict sar;
Uint32 * __restrict lav;

SDL_Rect src;
src.x=max(0,-offx); 
src.y=max(0,-offy);
src.w=min(temp->w,scrwi-offx);
src.h=min(temp->h,scrhi-offy);
int alpha;
for (int yy=src.y; yy<src.h; yy++)
	{
int tempx=max(src.x, min(index->IMGindexStart[yy], src.w));
		
dar=(Uint32*)screen->pixels  + (offy+yy)*screen->w + offx + src.x;
sar=(Uint32*)temp->pixels  + (yy)*temp->w+src.x;

memcpy ( dar, sar, (tempx-src.x)*4);

tempx=max(src.x, index->IMGindexStart[yy]);
int tempw=min(src.w, index->IMGindexEnd[yy]);

dar=(Uint32*)screen->pixels  + (offy+yy)*screen->w + offx + tempx;
sar=(Uint32*)temp->pixels  + (yy)*temp->w+tempx;
lav=(Uint32*)mergeimage[1501]->pixels+lava.y*(tyleSize)*2+lava.x+(tempx-src.x);
lava.y++;

for (int xx=tempx; xx<tempw; )
		{
			alpha=*sar>>24;
if (alpha!=0xFF)
if (alpha<0xF8)
*dar=*sar;
else
{
	alpha=(7-(alpha-0xF8))*32;
register int idst = *lav;
register int isrc = *sar;

*dar = 	((RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * alpha) >>8))) |
	(GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * alpha) >>8))) |
	(BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * alpha) >>8)))) |
	(*sar & AlphaMask);
}

xx++; sar++; dar++; lav++;
		}

tempx=max(index->IMGindexEnd[yy], src.x);
if (tempx<src.w)
{
//int tempw=max(src.x, min(index->IMGindexEnd[yy], src.w));
	
//=max(src.x, min(index->IMGindexStart[yy], src.w));
		
//dar=(Uint32*)screen->pixels  + (offy+yy)*screen->w + offx + src.x;
//sar=(Uint32*)temp->pixels  + (yy)*temp->w+src.x;

dar=(Uint32*)screen->pixels  + (offy+yy)*screen->w + offx + tempx;
sar=(Uint32*)temp->pixels  + (yy)*temp->w+tempx;



//the problematic one:
memcpy ( dar, sar, (src.w-tempx)*4);//(src.w-index->IMGindexEnd[yy])/2);
}

	}

}

void myhblit (int offx, int offy, int h, SDL_Surface *__restrict temp)
{
//SDL_Surface *temp = image[no];  
h/=dddstength;
offx+=((offx-scrwid)*h)>>16;
offy+=((offy-scrhid)*h)>>16;
if ((offx+temp->w<0) || (offy+temp->h<0) || (offx>scrwi) || (offy>scrhi)) return; 
register int idst;
register int isrc;
Uint32 * dar;
Uint32 * sar;
SDL_Rect src;
src.x=max(0,-offx); 
src.y=max(0,-offy);
src.w=min(temp->w,scrwi-offx);
src.h=min(temp->h,scrhi-offy);
int alpha;
//alpha=20;
for (int yy=src.y; yy<src.h; yy++)
	{
//AH=(Uint32*)alphasource->pixels + yy*alphasource->w + src.x;
dar=(Uint32*)screen->pixels  + (offy+yy)*screen->w + offx + src.x;
sar=(Uint32*)temp->pixels  + (yy)*temp->w+src.x;
for (int xx=src.x; xx<src.w; xx++, sar+=1, dar+=1)
		{
			alpha=*sar>>24;
			//alpha=255-alpha;
			//alpha=20;
			//if (alpha>200)
			  // *dar=*sar;
			//else
			//if (alpha & 0x000020)
			//{
idst = *dar;
isrc = *sar;
*dar = 0x000000FF | ((RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * alpha) >>8))) | (GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * alpha) >>8))) | (BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * alpha) >>8))));
//dar= (((int)((int)(idst & GreenMask)*alpha)>>8) & GreenMask) | (((int)((int)(idst & BlueMask)*alpha)>>8)& BlueMask) | (((int)((int)(idst & RedMask)*alpha)>>8)& RedMask);
//*dar= ((int)((int)(idst & GreenMask) * alpha)>>8) | ((int)((int)(idst & BlueMask) * alpha)>>8) | (*dar & RedMask);
//*dar= (idst & 0x00FFFF) + (int)(0x880000 & RedMask);
//*dar =(RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * alpha) >>8))) | (GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * alpha) >>8))) | (BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * alpha) >>8)));
			//*dar+=(*sar-*dar)*temp;
			//*(dar+1)+=(*(sar+1)-*(dar+1))*temp;
			//*(dar+2)+=(*(sar+2)-*(dar+2))*temp;
			//}
		}
	}

}
//ShadowScreen
void CopyBrightStoredOnscreen ()//no indexing
{
	
	int i;
	Uint32*__restrict ar;
	Uint32 *__restrict mask=(Uint32*)darkmask;
Uint32 * shadpic;//actually its a sstored tyle
//Uint32* shadow;
	for (int j=0; j<5; j++)
	for (int ii=0; ii<5; ii++)
	{	
	offx= - actcamx - (cameratylx - grid.objectx[j][ii])*tyleSize;
	offy= - actcamy - (cameratyly - grid.objecty[j][ii])*tyleSize;
	if ((offx<scrwi) && (offy<scrhi) && (offx+tyleSize>0) && (offy+tyleSize>0))	
	{
		int offset=offy*(offy>0);
		int shadOff=max(0,-offy);
i=min(tyleSize+offy*(offy<0),scrhi);
i=min (i, scrhi-offy);
bool first=1;
while (i>0)
{
int srcew=min(offx+tyleSize,scrwi);//min(die,offx+tyleSize);
int setoffxScr=max(offx,0);//max(dis,offx);
int endscreen=srcew-setoffxScr;
mask+=setoffxScr;
//shadpic+=max(-offx,0);
int xcount=0;
int sum=0;
int grid;
offsets sector;
sector=GetPixelsTillEnd(setoffxScr, offset);
while ((endscreen>=0) && (offset<scrhi)  && (i>0) )
{
grid=GetDarkIndex(setoffxScr+xcount, offset);
sum=0;
while ((grid) && (endscreen>0))
	{
sum+=grid;
endscreen-=grid;
xcount+=grid;
grid=GetDarkIndex(setoffxScr+xcount, offset);
	}
if (sum)
{
//shadpic+=sum;
mask+=sum;
}
sector=GetPixelsTillEnd(setoffxScr+xcount, offset);
int minx=min(endscreen,sector.x);
int miny=min(i-1,sector.y);
bool lightOn=0;
for (int ay=0; ay<=miny; ay++)
{
ar=(Uint32*)screen->pixels+offset*scrwi;
mask=(Uint32*)darkmask+offset*scrwi;
shadpic=(Uint32*)storedmp[j+1][ii+1]->pixels+shadOff*tyleSize;
mask+=setoffxScr+xcount;
ar+=setoffxScr+xcount;
int zzz=max(-offx,0)+xcount;
shadpic+=zzz;
int ax=minx+1;

int msk=*mask>>16;
int skipper=0;
if (ax>0)
while (ax) // (int ax=0; ax<=minx; ax++)
{
 

 while((!(*mask>>16)) && (ax)){
 ax--;
 mask++;
 skipper++;
 }
ar+=skipper;
shadpic+=skipper;
skipper=0;

msk=*mask>>16;
while ((msk>127) && (ax)){
*ar=*shadpic;
ar++;
shadpic++;
mask++;
ax--;
msk=*mask>>16;
}

while ((msk<128) && (ax) && (msk))
{
msk*=2;
register int dst;
dst=*ar;
register int isrc = *shadpic;
*ar = ((RedMask & ((dst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(dst & RedMask)) * msk) >>8))) 
	 | (GreenMask & ((dst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(dst & GreenMask)) * msk) >>8))) 
	 | (BlueMask & ((dst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(dst & BlueMask)) * msk) >>8))))
	 | (dst & 0xFF000000);
ar++;
shadpic++;
mask++;
ax--;

msk=*mask>>16;
}



}
offset++;
shadOff++;

}

// Previous tyle zeroing next one
// Maybe next one clearing previous now
endscreen-=(minx+1);
miny++;
offset-=miny;
shadOff-=miny;
//if ((!lightOn) && (endscreen>8) && (xcount>8) && (i>8) && (!first)) 
//	SetZeroDarkIndex(setoffxScr+xcount, offset);
xcount+=minx+1;
//finalp:
}
offset+=sector.y+1;
shadOff+=sector.y+1;
i-=sector.y;
i--;
first=0;
}
}
}
}
void darkenscreen ()//no indexing
{
	int i;
	float fading=0.999-(0.2-0.05*OpenGLscreen)*GameSpeed-flashlight*(0.3);//blicfght
	//float softfading=0.999-0.2*GameSpeed;// light
	float brghtMod=5.1/min(GameSpeed,3);
	Uint32*__restrict ar;
	Uint32 *__restrict mask=(Uint32*)darkmask;
//Uint32 * shadpic;//actually its a sstored tyle
Uint8* shadow;
	for (int j=0; j<5; j++)
	for (int ii=0; ii<5; ii++)
	{	
	offx= - actcamx - (cameratylx - grid.objectx[j][ii])*tyleSize;
	offy= - actcamy - (cameratyly - grid.objecty[j][ii])*tyleSize;
	if ((offx<scrwi) && (offy<scrhi) && (offx+tyleSize>0) && (offy+tyleSize>0))	
	{
		int offset=offy*(offy>0);
		int shadOff=max(0,-offy);
i=min(tyleSize+offy*(offy<0),scrhi);
i=min (i, scrhi-offy);
bool first=1;
while (i>0)
{
//ar=(Uint32*)screen->pixels+offset*scrwi;
//mask=(Uint32*)darkmask+offset*scrwi;
//shadpic=(Uint32*)storedmp[j+1][ii+1]->pixels+shadOff*tyleSize;
	//(Uint32*)shadowmaps[j+1][ii+1]->pixels+shadOff*tyleSize;
int srcew=min(offx+tyleSize,scrwi);//min(die,offx+tyleSize);
int setoffxScr=max(offx,0);//max(dis,offx);
int endscreen=srcew-setoffxScr-1;
mask+=setoffxScr;
//shadpic+=max(-offx,0);
int xcount=0;
int sum=0;
int grid;
offsets sector;
sector=GetPixelsTillEnd(setoffxScr, offset);
//if (endscreen>0)
while ((endscreen>0) && (offset<scrhi)  && (i>0) )
{
grid=GetDarkIndex(setoffxScr+xcount, offset);
sum=0;
while ((grid) && (endscreen>0))
	{
sum+=grid;
endscreen-=grid;
xcount+=grid;
grid=GetDarkIndex(setoffxScr+xcount, offset);
	}
if (sum)
{
mask+=sum;
}
//if (endscreen<=0) goto finalp;
//*ar = 0;
sector=GetPixelsTillEnd(setoffxScr+xcount, offset);
int minx=min(endscreen,sector.x);
int miny=min(i-1,sector.y);
//if (minx)
bool lightOn=0;
for (int ay=0; ay<=miny; ay++)
{
ar=(Uint32*)screen->pixels+offset*scrwi;
//*ar=0;
mask=(Uint32*)darkmask+offset*scrwi;
shadow=&shadUp[j+1][ii+1].pix[shadOff*tyleSize];
mask+=setoffxScr+xcount;
ar+=setoffxScr+xcount;
int zzz=max(-offx,0)+xcount;
shadow+=zzz;
for (int ax=0; ax<=minx; ax++)
{
if (*mask)
{
register int dst;
 int msk=*mask>>16;
 
if (msk>255)//64)  // if light is strong
{

dst=*ar;

/*
msk=(msk-255)/brghtMod + 255;
Uint32 tmp=(((dst & RedMask)>>8)*(msk));
*ar = (tmp<RedMask ? (tmp & RedMask) : RedMask); 
tmp=((dst & GreenMask)*(msk))>>8;
*ar|= (tmp<GreenMask ? (tmp & GreenMask) : GreenMask);
 tmp=((dst & BlueMask)*(msk))>>9;
*ar|= (tmp<BlueMask ? tmp   : BlueMask);
*/
//brightenColor(ar, msk);
	
msk-=255;
msk/=brghtMod;
	Uint32 tmp=(dst & RedMask)+(msk<<16);
*ar = (tmp<RedMask ? tmp : RedMask) | ((256-msk)<<24); 
 tmp=(dst & GreenMask)+(msk<<8);
*ar|= (tmp<GreenMask ? tmp : GreenMask);
 tmp=(dst & BlueMask)+(msk/2);
*ar|= (tmp<BlueMask ? tmp : BlueMask);
*mask=(Uint32)(*mask*fading) & 0xFFFF0000;
}
else
{
//msk*=4;
int drk=(255 - min((*mask & 0xFFFF),0xFF) + (*shadow)+msk);

if   (drk<256)
{
//drk=max(128,drk);
if (drk<128)
*ar=(*ar>>1) & 0x007F7F7F;
else
{
dst=*ar;
*ar =(RedMask & (((dst & RedMask)* drk) >>8)) | (GreenMask & (((dst & GreenMask) * drk) >>8))  
	| (BlueMask & (((dst & BlueMask) * drk) >>8));// | (*ar & 0xFF000000) ;
}
}
*mask=(Uint32)(*mask*fading) & 0xFFFF0000;
} 

if (*mask)//((*mask & 0x00FF0000)>0x00010000)
{
//*mask-=0x01000000;
/*int dt=((*mask>>16) & 0xFF);
*mask/=dt;
dt--;
*mask*=(dt);
*mask=(*mask & 0xFF000000) | (dt<<16); */
lightOn=true;
}
/*
else
	*mask=0;*/
}
ar++;
//shadpic++;
shadow++;
mask++;
}
offset++;
shadOff++;

}

// Previous tyle zeroing next one
// Maybe next one clearing previous now
endscreen-=(minx+1);
miny++;
offset-=miny;
shadOff-=miny;
if ((!lightOn) && (endscreen>8) && (xcount>8) && (i>8) && (!first)) 
	SetZeroDarkIndex(setoffxScr+xcount, offset);
xcount+=minx+1;
//finalp:
}
offset+=sector.y+1;
shadOff+=sector.y+1;
i-=sector.y;
i--;
first=0;
}
}
}
	/*bool * mzero=darkmaskGrid;
for (int i=0; i<scrwi8*scrhi8; i++)
{*mzero=0; mzero++;}*/

}


void darkenscreenIgnoreBakedShadow()//no indexing
{
	int i;
	float fading=0.999-(0.15-0.02*OpenGLscreen)*GameSpeed-flashlight*(0.3);//blicfght
	//float softfading=0.999-0.2*GameSpeed;// light
	float brghtMod=5.1/min(GameSpeed,3);

	Uint32*__restrict ar;
	Uint32 *__restrict mask=(Uint32*)darkmask;

int realy=0, realx=0;
int grid;

bool lightsOn[scrwi8];
memset(&lightsOn[0], 0, sizeof(lightsOn));

while (realy<scrhi)
{


do	{

grid=GetDarkIndex(realx % scrwi, realy+realx/scrwi);
realx+=grid;
	} while (grid);

if (realx>=scrwi){

if (((realy % 8)==7) || (realy / 8 != (realy+realx/scrwi) / 8)) {
int i=scrwi8;
do{
	i--;
	if (lightsOn[i]) {SetTrueDarkIndex(i*8, realy);  lightsOn[i]=0;}
	
} while (i);
}

realy+=realx/scrwi;
realx%=scrwi;
if (realy>=scrhi) return;
}

//*((Uint32*)screen->pixels+realy*scrwi+realx)=0;

if ((realy % 8)==7) { SetZeroDarkIndex(realx, realy);}

//bool lightOn=0;
int i=8;
ar=(Uint32*)screen->pixels+realy*scrwi+realx;
mask=(Uint32*)darkmask    +realy*scrwi+realx;
while (i)
{

if (*mask)
{
register int dst;
 int msk=*mask>>16;
 
if (msk>255)
{

dst=*ar;
msk-=255;
msk/=brghtMod;
	Uint32 tmp=(dst & RedMask)+(msk<<16);
*ar = (tmp<RedMask ? tmp : RedMask) | ((256-msk)<<24); 
 tmp=(dst & GreenMask)+(msk<<8);
*ar|= (tmp<GreenMask ? tmp : GreenMask);
 tmp=(dst & BlueMask)+(msk/2);
*ar|= (tmp<BlueMask ? tmp : BlueMask);
*mask=(Uint32)(*mask*fading) & 0xFFFF0000;
}
else
{
//msk*=4;
int drk=(255 - min((*mask & 0xFFFF),0xFF)+msk);

if   (drk<256)
{
if (drk<128)
*ar=(*ar>>1) & 0x007F7F7F;
else
{
dst=*ar;
*ar =(RedMask & (((dst & RedMask)* drk) >>8)) | (GreenMask & (((dst & GreenMask) * drk) >>8))  
	| (BlueMask & (((dst & BlueMask) * drk) >>8));// | (*ar & 0xFF000000) ;
}
}
*mask=(Uint32)(*mask*fading) & 0xFFFF0000;
} 

if (*mask) lightsOn[realx/8]=true;

}
ar++;
mask++;

i--;
}
realx+=8;
//SetTrueDarkIndex(realx-8, realy);
}
}
//darkenscreen
void Notdarkenscreen ()
{
	register int dst;
	register int msk;
	int endscreen;
	int i=screen->h;

	Uint32*__restrict ar;
	Uint32 *__restrict mask=darkmask;

	ar=(Uint32*)screen->pixels;



while (i)
{
endscreen=scrwi-darkIndexStart[scrhi-i] - (scrwi-darkIndexEnd[scrhi-i])*(darkIndexEnd[scrhi-i]>0);
mask+=darkIndexStart[scrhi-i];
ar+=darkIndexStart[scrhi-i];
	//endscreen=scrwi-darkIndexEnd[i-1];


while (endscreen)
{
if (!(*(++mask)))	*ar=0;
else
{
msk=255-*mask;
*mask=0;
dst=*ar;
*ar = (RedMask & (((dst & RedMask)* msk) >>8)) | (GreenMask & (((dst & GreenMask) * msk) >>8))  | (BlueMask & (((dst & BlueMask) * msk) >>8)) ;
} 
//mask++;
ar++;
endscreen--;
}
mask+=(scrwi-darkIndexEnd[scrhi-i])*(darkIndexEnd[scrhi-i]>0);
ar+=(scrwi-darkIndexEnd[scrhi-i])*(darkIndexEnd[scrhi-i]>0);
darkIndexStart[scrhi-i]=scrwi;
darkIndexEnd[scrhi-i]=0;

i--;
}

}

void transparentizeUpper(SDL_Surface * tyle, SDL_Surface * mask, int offx, int offy){

SDL_Rect src;

src.x=max(0,-offx);
src.y=max(0,-offy);
src.h=min(mask->h,max(tyle->h-offy,0));
src.w=min(mask->w,max(tyle->w-offx,0));

Uint32* __restrict tpic=(Uint32*)tyle->pixels+max(offy,0)*tyle->w+max(0, offx); 
Uint32* __restrict mpic=(Uint32*)mask->pixels+(src.y)*mask->w+src.x;
int diff=src.w-src.x;
for (int y=src.y; y<src.h; y++)
{
	for (int x=src.x; x<src.w; x++)
	{

float AH=255-(*mpic>>24);
AH/=255;

Uint32 dsst=*tpic;
*tpic =  ((Uint32)(dsst * AH) & AlphaMask) | ((Uint32)((dsst & RedMask) * AH) & RedMask ) | ((Uint32)((dsst & GreenMask) * AH) & GreenMask ) | ((Uint32)((dsst & BlueMask) * AH) & BlueMask );

tpic++;
mpic++;
	}
tpic+=tyleSize-diff;
mpic+=mask->w-diff;
}

}
void drowDecayedBG (SDL_Surface* Decayed, SDL_Surface * tyle, SDL_Surface * mask, int offx, int offy)
{
SDL_Rect src;

src.x=max(0,-offx);
src.y=max(0,-offy);
src.h=min(mask->h,max(tyle->h-offy,0));
src.w=min(mask->w,max(tyle->w-offx,0));

Uint32* __restrict dpic=(Uint32*)Decayed->pixels+max(offy,0)*Decayed->w+max(0, offx); 
Uint32* __restrict tpic=(Uint32*)tyle->pixels+max(offy,0)*tyle->w+max(0, offx); 
Uint32* __restrict mpic=(Uint32*)mask->pixels+(src.y)*mask->w+src.x;
int diff=src.w-src.x;
int shade;
for (int y=src.y; y<src.h; y++)
{
	for (int x=src.x; x<src.w; x++)
	{
shade=(*tpic>>24);
if (shade>128)
{
int AH=(((*mpic)>>24)*((*dpic)>>24))>>8;	
register int idst = *tpic;
register int isrc = *dpic;

if (shade<192)
{
	AH*=shade-128;
	AH/=64;
}
*tpic = ((RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * AH) >>8))) | (GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * AH) >>8))) | (BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * AH) >>8)))) | (*tpic & 0xFF000000);
}
dpic++;
tpic++;
mpic++;
	}
dpic+=tyleSize-diff;
tpic+=tyleSize-diff;
mpic+=mask->w-diff;
}


}
void drowalphamix (SDL_Surface* source, SDL_Surface* uppersource, SDL_Surface * screen, SDL_Surface * alphasource, int border , int offx, int offy, int elevation, int w)
{
	//w=50;
Uint32 * __restrict sourpic,* __restrict sourpic2, * __restrict destpic, * __restrict alphapic;
int fade=border+w;
int shorth=elevation>>16;
//portion=portions[shorth+precalcecvator];

elevation/=dddstength;
int dshortx=(offx-scrwid)*elevation;
offx+=dshortx>>16;
int dshorty=(offy-scrhid)*elevation;
offy+=dshorty>>16;


//int limx, limy;
if ((offx+alphasource->w<0) || (offy+alphasource->h<0) || (offx>screen->w) || (offy>screen->h)) return; 

SDL_Rect src;//, dest;

src.x=max(0,-offx); 
src.y=max(0,-offy);
src.w=min(min(alphasource->w,screen->w-offx),source->w);
src.h=min(min(alphasource->h,screen->h-offy),source->h);
int AH;
int AA;
//int MA;
//border=0;
for (int yy=src.y; yy<src.h; yy++)
	{
alphapic=(Uint32*)alphasource->pixels + yy*alphasource->w + src.x;
destpic=(Uint32*)screen->pixels  + (offy+yy)*screen->w + offx + src.x;
sourpic=(Uint32*)source->pixels  + (yy)*source->w+src.x;
sourpic2=(Uint32*)uppersource->pixels  + (yy)*uppersource->w+src.x;
	for (int xx=src.x; xx<src.w; xx++, destpic++, sourpic++, sourpic2++, alphapic++)
		{
		//if (*((Uint8 * )alphapic+3)<border-w) continue;	
			//MA=*((Uint8 * )alphapic+3)-border;
	AA=*((Uint8 * )alphapic+3)-border-w;
if (AA<0) 
{
AH=*((Uint8 * )sourpic+3);	
//if (AH<10) continue;	
if (AH>5)
{
*((Uint8 * )destpic) = (((*((Uint8 * )sourpic) -  *((Uint8 * )destpic)) * (AH))/255 + *((Uint8 * )destpic));
*((Uint8 * )destpic+1) = (((*((Uint8 * )sourpic+1) -  *((Uint8 * )destpic+1)) * (AH))/255 + *((Uint8 * )destpic+1));
*((Uint8 * )destpic+2) = (((*((Uint8 * )sourpic+2) -  *((Uint8 * )destpic+2)) * (AH))/255 + *((Uint8 * )destpic+2));
}
}


if (AA>-w)
{
AH=*((Uint8 * )sourpic2+3);	
if (AA<0) 
	{
		AH*=(w+AA);
		AH/=w;
		//AH=255;
}
if (AH>0) 
{
	//(1+60/MA);	
	//if (AH==255) continue;
	//AH-=(MA==0);
*((Uint8 * )destpic) = (((*((Uint8 * )sourpic2) -  *((Uint8 * )destpic)) * (AH))/255 + *((Uint8 * )destpic));
*((Uint8 * )destpic+1) = (((*((Uint8 * )sourpic2+1) -  *((Uint8 * )destpic+1)) * (AH))/255 + *((Uint8 * )destpic+1));
*((Uint8 * )destpic+2) = (((*((Uint8 * )sourpic2+2) -  *((Uint8 * )destpic+2)) * (AH))/255 + *((Uint8 * )destpic+2));
}

}



		}
	}

}
void drowalphaborder (SDL_Surface* source, SDL_Surface * screen, SDL_Surface * alphasource, int border , int offx, int offy, int elevation, int w)
{
w=32;
//Uint32 *sourpic, *destpic, *alphapic;
int shorth=elevation>>16;
//portion=portions[shorth+precalcecvator];

elevation/=dddstength;
int dshortx=(offx-scrwid)*elevation;
offx+=dshortx>>16;
int dshorty=(offy-scrhid)*elevation;
offy+=dshorty>>16;


//int limx, limy;
if ((offx+alphasource->w<0) || (offy+alphasource->h<0) || (offx>screen->w) || (offy>screen->h)) return; 

SDL_Rect src;//, dest;

src.x=max(0,-offx); 
src.y=max(0,-offy);
src.w=min(min(alphasource->w,screen->w-offx),source->w);
src.h=min(min(alphasource->h,screen->h-offy),source->h);
Uint8 AH;
//int MA;
//border=0;
for (int yy=src.y; yy<src.h; yy++)
	{
Uint32* __restrict alphapic=(Uint32*)alphasource->pixels + yy*alphasource->w + src.x;
Uint32* __restrict destpic=(Uint32*)screen->pixels  + (offy+yy)*screen->w + offx + src.x;
Uint32* __restrict sourpic=(Uint32*)source->pixels  + (yy)*source->w+src.x;
	for (int xx=src.x; xx<src.w; xx++, destpic++, sourpic++, alphapic++)
		{
		if (*((Uint8 * )alphapic+3)<border-w) continue;	
		//AH=*((Uint8 * )sourpic+3);
		int diff=*((Uint8 * )alphapic+3)-border;
if (*((Uint8 * )alphapic+3)>border)
{
*destpic=(*sourpic & 0x00FFFFFF) | (*destpic & 0xFF000000);
}
//if (diff<16)
//else
if (diff<16)
{

if (diff>0) AH=(32-diff)*8;
else
AH=(32+diff)*2;

register int idst = *destpic;
register int isrc = *alphapic;
*destpic = ((RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * AH) >>8)))
		| (GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * AH) >>8))) 
		| (BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * AH) >>8))))
		| (AlphaMask & *destpic);

}


		}
	}

}
void drowalphaborder3d (SDL_Surface* source, SDL_Surface * screen, SDL_Surface * alphasource, int border , int offx0, int offy, int elevation, int w)
{
Uint32 *sourpic, *destpic, *alphapic;

if (elevation>0)
elevation/=dddstength;
int dshortx=(offx0-scrwid)*elevation;
offx0+=dshortx>>16;
int dshorty=(offy-scrhid)*elevation;
offy+=dshorty>>16;
elevation=elevation>>16;
portion=portions[elevation+precalcecvator];
offx=offx0-(offx0-conf3d.righteye)*(portion);

//int limx, limy;
if ((offx+alphasource->w<0) || (offy+alphasource->h<0) || (offx>scrwi) || (offy>scrhi)) return; 

SDL_Rect src;//, dest;

src.x=max(0,-offx); 
src.y=max(0,-offy);
src.w=min(min(alphasource->w,scrwi-offx),source->w);
src.h=min(min(alphasource->h,scrhi-offy),source->h);
Uint8 AH;
//int MA;
//onscreeny+=abs(onscreeny % 2);
//border=0;
src.y+=!abs(src.y % 2);
bool row=abs((src.y+offy) % 2);
offy+=row;
for (int yy=src.y; yy<src.h-row; yy+=2)
	{
alphapic=(Uint32*)alphasource->pixels + yy*alphasource->w + src.x;
destpic=(Uint32*)screen->pixels  + (offy+yy)*screen->w + offx + src.x;
sourpic=(Uint32*)source->pixels  + (yy)*source->w+src.x;
	for (int xx=src.x; xx<src.w; xx++)
		{
			
			//MA=*((Uint8 * )alphapic+3)-border;
			if (*((Uint8 * )alphapic+3)>=border)
{
	//(1+60/MA);
	AH=*((Uint8 * )sourpic+3);
	//if (AH==255) continue;
	//AH-=(MA==0);
*((Uint8 * )destpic) = (((*((Uint8 * )sourpic) -  *((Uint8 * )destpic)) * (AH))/255 + *((Uint8 * )destpic));
*((Uint8 * )destpic+1) = (((*((Uint8 * )sourpic+1) -  *((Uint8 * )destpic+1)) * (AH))/255 + *((Uint8 * )destpic+1));
*((Uint8 * )destpic+2) = (((*((Uint8 * )sourpic+2) -  *((Uint8 * )destpic+2)) * (AH))/255 + *((Uint8 * )destpic+2));
}

			destpic++; 
			sourpic++;
			alphapic++;
		}
	}

// RIGHTEYE
offx=offx0-(offx0-conf3d.lefteye)*(portion);
src.x=max(0,-offx); 
src.y=max(0,-offy);
src.w=min(min(alphasource->w,scrwi-offx),source->w);
src.h=min(min(alphasource->h,scrhi-offy),source->h);
src.y+=!abs(src.y % 2);
row=abs((src.y+offy) % 2);
offy+=!row;
if ((offx+alphasource->w<0) || (offy+alphasource->h<0) || (offx>scrwi) || (offy>scrhi)) return;
for (int yy=src.y; yy<src.h-row; yy+=2)
	{
alphapic=(Uint32*)alphasource->pixels + yy*alphasource->w + src.x;
destpic=(Uint32*)screen->pixels  + (offy+yy)*screen->w + offx + src.x;
sourpic=(Uint32*)source->pixels  + (yy)*source->w+src.x;
	for (int xx=src.x; xx<src.w; xx++)
		{
			
			//MA=*((Uint8 * )alphapic+3)-border;
			if (*((Uint8 * )alphapic+3)>=border)
{
	//(1+60/MA);
	AH=*((Uint8 * )sourpic+3);
	//if (AH==255) continue;
	//AH-=(MA==0);
*((Uint8 * )destpic) = (((*((Uint8 * )sourpic) -  *((Uint8 * )destpic)) * (AH))/255 + *((Uint8 * )destpic));
*((Uint8 * )destpic+1) = (((*((Uint8 * )sourpic+1) -  *((Uint8 * )destpic+1)) * (AH))/255 + *((Uint8 * )destpic+1));
*((Uint8 * )destpic+2) = (((*((Uint8 * )sourpic+2) -  *((Uint8 * )destpic+2)) * (AH))/255 + *((Uint8 * )destpic+2));
}

			destpic++; 
			sourpic++;
			alphapic++;
		}
	}

}
void SameSurfaceDisplacement (SDL_Surface* screen, int x, int y, SDL_Surface * alphasource, int offx, int offy)
{

if ((offx+alphasource->w<0) || (offy+alphasource->h<=0) || (offx>=scrwi) || (offy>=scrhi) ||
	(x+alphasource->w<=0) || (y+alphasource->h<=0) || (x>=scrwi) || (y>=scrhi)) return; 
int negx=1-(offx>x)*2;
int negy=1-(offy>y)*2;
Uint32 * AH;
Uint32 * dar;
Uint32 * sar;
SDL_Rect src;
src.x=max(max(0,-offx),-x); 
src.y=max(max(0,-offy),-y);
src.w=min(min(alphasource->w,scrwi-offx),screen->w-x);
src.h=min(min(alphasource->h,scrhi-offy),screen->h-y);
int alpha;

int yy=src.y;
int yfinish=src.h;
if (negy<0) { yy=src.h-1; yfinish=src.y;}

int xstart=src.x;
int xfinish=src.w;
if (negx<0) { xstart=src.w-1; xfinish=src.x;}

for ( true; yy!=yfinish; yy+=negy)
	{
AH=(Uint32*)alphasource->pixels + yy*alphasource->w + xstart;
dar=(Uint32*)screen->pixels  + (offy+yy)*screen->w + offx + xstart;
sar=(Uint32*)screen->pixels  + (y+yy)*screen->w+xstart+x;
for (int xx=xstart; xx!=xfinish; xx+=negx, sar+=negx, dar+=negx, AH+=negx)
		{
			alpha=*AH>>24;
		 if (alpha>250)
		   *dar=(*sar & 0x00FFFFFF) | (*dar & 0xFF000000);
			else
			if (alpha)
			{
register int idst= *dar;
register int isrc= *sar;

*dar = ((RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * alpha) >>8))) 
| (GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * alpha) >>8)))
| (BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * alpha) >>8))) )
| (isrc & AlphaMask);

			}
		}
	}

}

// NEW SOFT DISPLACEMENTS

void DrowMyIndAlphaDisplacement (MyIndexedAlphaSurface* image, int x, int y, int h, SDL_Surface* screen, int srcx, int srcy, SDL_Surface* source)
{
if ((x>screen->w) || (x+image->w<0) || (y>screen->h) || (y+image->h<0)) return;
if ((srcx>source->w) || (srcx+image->w<0) || (srcy>source->h) || (srcy+image->h<0)) return;

doffx=x;
doffy=y;

int elevation=h*accuracy/dddstength;
x=(((x-scrwid)*elevation)/accuracy);
y=(((y-scrhid)*elevation)/accuracy);
srcx+=x;
srcy+=y;
x+=doffx;
y+=doffy;
float xpera=doffx-x; xpera/=255;
float ypera=doffy-y; ypera/=255;

Uint8 * __restrict  src=image->pixels;
Uint32 * __restrict  scrpix;
Uint32 * __restrict  mappix;
int gap;
int fill, skip;
int total=0;
int i;

for (i=0; i<image->pixCount-1;)
{

gap=*src;
src++; i++;
int left=fill=*src;
src++; i++;
total+=gap;
////// Border calculation:
while (left)
{
int imgy=total/image->w;

if (imgy+y>=screen->h) return; 
if (imgy+srcy>=source->h) return; 

int imgx=total % image->w;
int drow=min(left, image->w-imgx);

if ((imgy<-y) || (imgy<-srcy)) 
	skip=drow;
else
{
skip=0;

if ((imgx+x>=screen->w) || (imgx+srcx>=source->w)) skip=drow;
else
{
if (imgx+drow+x>screen->w) drow=(screen->w-imgx-x);
if (imgx+drow+srcx>source->w) drow=(source->w-imgx-srcx);

if (imgx<-x) skip=min(-(imgx+x),drow);
if (imgx<-srcx) skip=max(skip,min(-(imgx+srcx),drow));

}
}
if (skip)
{
	 drow-=skip; i+=skip; src+=skip; left-=skip; total+=skip; imgx+=skip;
}

scrpix=(Uint32*)screen->pixels + (imgy+y)*screen->w+imgx+x;
mappix=(Uint32*)source->pixels + (imgy+srcy)*source->w+imgx+srcx;
left-=drow;
total+=drow;
for (int j=0; j<drow;)
{
int AH=*src;
*scrpix = *(mappix + ((int)(AH*ypera))*source->w+(int)(xpera*AH));
	
	scrpix++;
	mappix++;
	src++;
	i++;
	j++;
}

}
}

}
void DrowMyIndAlphaDisplacementSameSurface (MyIndexedAlphaSurface* image, int x, int y, int h, SDL_Surface* screen)
{
if ((x>screen->w) || (x+image->w<0) || (y>screen->h) || (y+image->h<0)) return;
h=max(1,h);
doffx=x;
doffy=y;

int elevation=h*accuracy/dddstength;
x=x+(((x-scrwid)*elevation)/accuracy);
y=y+(((y-scrhid)*elevation)/accuracy);
float xpera=doffx-x; xpera/=255;
float ypera=doffy-y; ypera/=255;
x-=image->w/2;
y-=image->h/2;


Uint8 * __restrict  src=image->pixels;
Uint32 * __restrict  scrpix;
int gap;
int fill, skip;
int total=0;
int i;
// ********************************* x1 y1 half of the screen
if ((xpera>=0) && (ypera>=0))
{
for (i=0; i<image->pixCount-1;)
{
gap=*src;
src++; i++;
int left=fill=*src;
src++; i++;
total+=gap;

while (left)
{
int imgy=total/image->w;
if (imgy+y>=scrhi) return;//goto x2y1; 

int imgx=total % image->w;
int drow=min(left, image->w-imgx);

if (imgy<-y) 
	skip=drow;
else{
	skip=0;

	if (imgx+x>=scrwi) skip=drow;
	else{
		if (imgx+drow+x>scrwi) drow=(scrwi-imgx-x);
		if (imgx<-x) skip=min(-(imgx+x),drow);
	}
}
if (skip){
	 drow-=skip; i+=skip; src+=skip; left-=skip; total+=skip; imgx+=skip;
}

scrpix=(Uint32*)screen->pixels + (imgy+y)*screen->w+imgx+x;
left-=drow;
total+=drow;
int j=drow;
i+=drow;
while (j){
	int AH=*src;
	*scrpix = *(scrpix + ((int)(AH*ypera))*screen->w+(int)(xpera*AH));
		scrpix++;src++;j--;
}
}
}
return;
}

//x2y1:
// ********************************* x2 y1 half of the screen


if ((xpera<0) && (ypera>=0))
for (i=0; i<image->pixCount-1;)
{
gap=*src;
src++; i++;
int left=fill=*src;
src++; i++;
total+=gap;

while (left)
{
int imgy=total/image->w;
if (imgy+y>=scrhi) return; 

int imgx=total % image->w;
int drow=min(left, image->w-imgx);

if (imgy<-y) 
	skip=drow;
else{
	skip=0;

	if (imgx+x>=scrwi) skip=drow;
	else{
		if (imgx+drow+x>scrwi) drow=(scrwi-imgx-x);
		if (imgx<-x) skip=min(-(imgx+x),drow);
	}
}
if (skip){
	 drow-=skip; i+=skip; src+=skip; left-=skip; total+=skip; imgx+=skip;
}

scrpix=(Uint32*)screen->pixels + (imgy+y)*screen->w+imgx+x+drow;
left-=drow;
total+=drow;
int j=drow;
i+=drow;
while (j){
	int AH=*src;
	*scrpix = *(scrpix + ((int)(AH*ypera))*screen->w+(int)(xpera*AH));
		scrpix--;src++;j--;
}

}
}

// ********************************* x1 y2 half of the screen
if ((xpera>=0) && (ypera<0))
for (i=0; i<image->pixCount-1;)
{
gap=*src;
src++; i++;
int left=fill=*src;
src++; i++;
total+=gap;

while (left)
{
int imgy=image->h-total/image->w;

int imgx=total % image->w;
int drow=min(left, image->w-imgx);

if (imgy+y>=scrhi) skip=drow;
else{
	if (imgy<=-y) 
		skip=drow;
	else{
		skip=0;

		if (imgx+x>=scrwi) skip=drow;
		else{
			if (imgx+drow+x>=scrwi) drow=(scrwi-imgx-x);
			if (imgx<=-x) skip=min(-(imgx+x),drow);
		}
	}
}
if (skip){
	 drow-=skip; i+=skip; src+=skip; left-=skip; total+=skip; imgx+=skip;
}

scrpix=(Uint32*)screen->pixels + (imgy+y)*screen->w+imgx+x;
left-=drow;
total+=drow;
int j=drow;
i+=drow;
while (j){
	int AH=*src;
	*scrpix = *(scrpix + ((int)(AH*ypera))*screen->w+(int)(xpera*AH));
		scrpix++;src++;j--;
}

}
}


// ********************************* x2 y2 half of the screen

if ((xpera<0) && (ypera<0))
for (i=0; i<image->pixCount-1;)
{
gap=*src;
src++; i++;
int left=fill=*src;
src++; i++;
total+=gap;

while (left)
{
int imgy=image->h-total/image->w;

int imgx=total % image->w;
int drow=min(left, image->w-imgx);

if (imgy+y>=scrhi) skip=drow;
else{
	if (imgy<-y) 
		skip=drow;
	else{
		skip=0;

		if (imgx+x>=scrwi) skip=drow;
		else{
			if (imgx+drow+x>scrwi-1) drow=(scrwi-imgx-x-1);
			if (imgx<-x) skip=min(-(imgx+x),drow);
		}
	}
}
if (skip){
	 drow-=skip; i+=skip; src+=skip; left-=skip; total+=skip; imgx+=skip;
}

scrpix=(Uint32*)screen->pixels + (imgy+y)*screen->w+imgx+x+drow;
left-=drow;
total+=drow;
i+=drow;
int j=drow;
while (j){
	int AH=*src;
	*scrpix = *(scrpix + ((int)(AH*ypera))*screen->w+(int)(xpera*AH));
		scrpix--;src++;j--;
}

}
}

}



void drowdisplacement (SDL_Surface* source, int x, int y, int h, SDL_Surface * alphasource, int offx, int offy)
{
//offx+=15;
//offy+=10;
if ((offx+alphasource->w<0) || (offy+alphasource->h<0) || (offx>scrwi) || (offy>scrhi)) return; 

Uint32 * AH;
Uint32 * dar;
Uint32 * sar;
SDL_Rect src;
src.x=max(max(0,-offx),-x); 
src.y=max(max(0,-offy),-y);
src.w=min(min(alphasource->w,scrwi-offx),source->w-x);
src.h=min(min(alphasource->h,scrhi-offy),source->h-y);
int alpha;

for (int yy=src.y; yy<src.h; yy++)
	{
AH=(Uint32*)alphasource->pixels + yy*alphasource->w + src.x;
dar=(Uint32*)screen->pixels  + (offy+yy)*screen->w + offx + src.x;
sar=(Uint32*)source->pixels  + (y+yy)*source->w+src.x+x;
for (int xx=src.x; xx<src.w; xx++, sar+=1, dar+=1, AH+=1)
		{
			alpha=*AH>>24;
		 if (alpha>250)
		   *dar=(*sar & 0x00FFFFFF) | (*dar & 0xFF000000);
			else
			if (alpha)
			{
register int idst= *dar;
register int isrc= *sar;

*dar = ((RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * alpha) >>8))) 
| (GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * alpha) >>8)))
| (BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * alpha) >>8))) )
| (isrc & AlphaMask)
;
			//*dar+=(*sar-*dar)*temp;
			//*(dar+1)+=(*(sar+1)-*(dar+1))*temp;
			//*(dar+2)+=(*(sar+2)-*(dar+2))*temp;
			}
		}
	}

}
void drowdisplacement3d (SDL_Surface* source, int x, int y, int h, SDL_Surface * alphasource, int offx0, int offy0)
{
SDL_Rect src;
int offx;
int offy;
portion=portions[h+precalcecvator];
offx=offx0-(offx0-conf3d.righteye)*(portion);

Uint32 *sourpic, *destpic, *alphapic;

if ((offx+alphasource->w<0) || (offy0+alphasource->h<0) || (offx>scrwi) || (offy0>scrhi)) return; 

src.x=max(max(0,-offx),-x); 
src.y=max(max(0,-offy0),-y);
src.w=min(min(alphasource->w,scrwi-offx),source->w-x);
src.h=min(min(alphasource->h,scrhi-offy0),source->h-y);

src.y+=abs((src.y + y) % 2);
row=abs((offy0+src.y) % 2);
offy=offy0+row;
//offy=offy0+row;
for (int yy=src.y; yy<src.h-row; yy+=2)
	{
alphapic=(Uint32*) alphasource->pixels + (yy)*alphasource->w + src.x;
destpic=(Uint32*) screen->pixels  + (offy+yy)*screen->w + offx + src.x;
sourpic=(Uint32*) source->pixels  + (y+yy)*source->w+src.x+x;
		for (int xx=src.x; xx<src.w; xx++)
		{
			if (*((Uint8 * )alphapic+3)>10)
			if (*((Uint8 * )alphapic+3)<240)
{
*((Uint8 * )destpic) = ((*((Uint8 * )sourpic) -  *((Uint8 * )destpic)) * (*((Uint8 * )alphapic+3))/255 + *((Uint8 * )destpic));
*((Uint8 * )destpic+1) = ((*((Uint8 * )sourpic+1) -  *((Uint8 * )destpic+1)) * (*((Uint8 * )alphapic+3))/255 + *((Uint8 * )destpic+1));
*((Uint8 * )destpic+2) = ((*((Uint8 * )sourpic+2) -  *((Uint8 * )destpic+2)) * (*((Uint8 * )alphapic+3))/255 + *((Uint8 * )destpic+2));
}
else
*destpic=*sourpic; 
			destpic++; 
			sourpic++;
			alphapic++;
		}
	}
// RIGHT eye
offx=offx0-(offx0-conf3d.lefteye)*(portion);
src.x=max(max(0,-offx),-x); 
src.y=max(max(0,-offy0),-y);
src.w=min(min(alphasource->w,scrwi-offx),source->w-x);
//int wtf = src.w;
src.h=min(min(alphasource->h,scrhi-offy0),source->h-y);

//if ((offx+alphasource->w<0) || (offy0+alphasource->h<0) || (offx>scrwi) || (offy0>scrhi)) return; 

src.y+=abs((src.y + y) % 2);
row=abs((offy0+src.y) % 2);
offy=offy0+!row;
if ((offx+alphasource->w<0) || (offy+alphasource->h<0) || (offx>scrwi) || (offy>scrhi)) return; 
for (int yy=src.y; yy<src.h-row; yy+=2)
	{
alphapic=(Uint32*) alphasource->pixels + (yy)*alphasource->w + src.x;
destpic=(Uint32*) screen->pixels  + (offy+yy)*screen->w + offx + src.x;
sourpic=(Uint32*) source->pixels  + (y+yy)*source->w+src.x+x;
		for (int xx=src.x; xx<src.w; xx++)
		{
			if (*((Uint8 * )alphapic+3)>10)
			if (*((Uint8 * )alphapic+3)<240)
{
*((Uint8 * )destpic) = ((*((Uint8 * )sourpic) -  *((Uint8 * )destpic)) * (*((Uint8 * )alphapic+3))/255 + *((Uint8 * )destpic));
*((Uint8 * )destpic+1) = ((*((Uint8 * )sourpic+1) -  *((Uint8 * )destpic+1)) * (*((Uint8 * )alphapic+3))/255 + *((Uint8 * )destpic+1));
*((Uint8 * )destpic+2) = ((*((Uint8 * )sourpic+2) -  *((Uint8 * )destpic+2)) * (*((Uint8 * )alphapic+3))/255 + *((Uint8 * )destpic+2));
}
else
*destpic=*sourpic; 
			destpic++; 
			sourpic++;
			alphapic++;
		}
	}

}
void lighten (SDL_Surface *source, SDL_Surface *layer, int dx, int dy, float dh)
{
	return;
	/*
int x=0;
int y=0;
if (dx<0) x-=dx;
if (dy<0) y-=dy;
int destx=source->w;
int desty=source->h;
if (dx+destx>layer->w) destx=(layer->w-dx);
if (dy+desty>layer->h) desty=(layer->h-dy);
Uint32 *sourpic, *destpic;
for (y; y<desty; y++)
{
	sourpic=(Uint32*) source->pixels  + y*source->w + x;
	destpic=(Uint32*) layer->pixels  + (dy+y)*layer->w + dx+x;
	for (int xx=x; xx<destx; xx++)
		{
//darkmask[dy+y][dx+xx]=0.5;
//darkmask[dy+y][dx+xx]=max(darkmask[dy+y][dx+xx],float(*((Uint8 * )sourpic+3))/255);
*((Uint8 * )destpic+3)= min(*((Uint8 * )destpic+3),(255-*((Uint8 * )sourpic+3)));

*((Uint8 * )destpic) = min(*((Uint8 * )sourpic+3) + *((Uint8 * )destpic) , 254);
*((Uint8 * )destpic+1) = min(*((Uint8 * )sourpic+3) + *((Uint8 * )destpic+1) , 254);
*((Uint8 * )destpic+2) = min(*((Uint8 * )sourpic+3) + *((Uint8 * )destpic+2) , 254);

sourpic++;
destpic++;
}
}
	*/
}
void lightenug (SDL_Surface *source, SDL_Surface *layer, int dx, int dy, float dh)
{
//if ((dh<0.05) || (dh>1)) return;

//dh/=255;
//if (dh=1) dh=0.99;
Uint32 * mask;
int x=0;
int y=0;
if (dx<0) x-=dx;
if (dy<0) y-=dy;
int destx=source->w;
int desty=source->h;
if (dx+destx>layer->w) destx=(layer->w-dx);
if (dy+desty>layer->h) desty=(layer->h-dy);
Uint8 *sourpic;
int temp;

//dh=1;
for (y; y<desty; y++)
{

darkIndexStart[dy+y]=min(darkIndexStart[dy+y],dx+x);
darkIndexEnd[dy+y]=max(darkIndexEnd[dy+y], destx+dx);

	sourpic=(Uint8*) source->pixels  + y*source->w*4 + x*4+3;
	//
	mask=darkmask;
	mask+=(dy+y)*scrwi+dx+x;
for (int xx=x; xx<destx;)
{
if ((temp=*sourpic*dh)>*mask) 
	*mask=temp;
xx++; 
mask++;
sourpic+=4;
//*mask = 
}

//darkIndexStart[i]=scrwi;
//darkIndexEnd[i]=0;
}
	
}
void lighten3D (SDL_Surface *source, SDL_Surface *layer, int dx, int dy, float dh)
{
	SDL_Rect  dest;
//dshorth=dh>>16;
//fuck=max(0,source->w-dshorth);
//if (fuck==0)
//	return;
//fuck/=source->w;
	return;
portion=portions[0+precalcecvator];
dest.x=dx-(dx-conf3d.lefteye)*(portion);
row=(dy) % 2; 
dest.y = dy-row+1;
int x=0; 
int y=0;
if (dest.x<0) x-=dest.x;
if (dest.y<0) y-=dest.y;
int destx=source->w;
int desty=source->h;
if (dest.x+destx>layer->w) destx=(layer->w-dest.x);
if (dest.y+desty>layer->h) desty=(layer->h-dest.y);
Uint32 *sourpic, *destpic;
int counter=0;
for (y; y<desty/2; y++)
{
	sourpic=(Uint32*) source->pixels  + (y+counter+1)*source->w + x;
	
	destpic=(Uint32*) layer->pixels  + (dest.y+y+counter)*layer->w + dest.x+x;
	counter++;
	for (int xx=x; xx<destx; xx++)
		{
//*((Uint8 * )destpic+3)= min(*((Uint8 * )destpic+3),(255-*((Uint8 * )sourpic+3)));

*((Uint8 * )destpic) = min(*((Uint8 * )sourpic+3)*fuck + *((Uint8 * )destpic) , 254);
*((Uint8 * )destpic+1) = min(*((Uint8 * )sourpic+3)*fuck + *((Uint8 * )destpic+1) , 254);
*((Uint8 * )destpic+2) = min(*((Uint8 * )sourpic+3)*fuck + *((Uint8 * )destpic+2) , 254);

sourpic++;
destpic++;
}
}

dest.x=dx-(dx-conf3d.righteye)*(portion);
dest.y = dy-row;


x=0;
y=0;
if (dest.x<0) x-=dest.x;
if (dest.y<0) y-=dest.y;
destx=source->w;
desty=source->h;
if (dest.x+destx>layer->w) destx=(layer->w-dest.x);
if (dest.y+desty>layer->h) desty=(layer->h-dest.y);
counter=0;
for (y; y<desty/2; y++)
{
	sourpic=(Uint32*) source->pixels  + (y+counter+1)*source->w + x;
	
	destpic=(Uint32*) layer->pixels  + (dest.y+y+counter)*layer->w + dest.x+x;
	counter++;
	for (int xx=x; xx<destx; xx++)
		{
*((Uint8 * )destpic+3)= min(*((Uint8 * )destpic+3),(255-*((Uint8 * )sourpic+3)));

*((Uint8 * )destpic) = min(*((Uint8 * )sourpic+3) + *((Uint8 * )destpic) , 254);
*((Uint8 * )destpic+1) = min(*((Uint8 * )sourpic+3) + *((Uint8 * )destpic+1) , 254);
*((Uint8 * )destpic+2) = min(*((Uint8 * )sourpic+3) + *((Uint8 * )destpic+2) , 254);

sourpic++;
destpic++;
}
}


}
void lightenGL (SDL_Surface *source, SDL_Surface *layer, int dx, int dy, float dh)
{
	
}
void gutsup(SDL_Surface* destination,SDL_Surface* guts, int x, int y, int radius )
	{
int dx=x+radius;
int dy=y+radius;
int cx=x+radius/2;
int cy=y+radius/2;
if (dx>=destination->w) dx=destination->w-1;
if (dy>=destination->h) dy=destination->h-1;
if (x<0) x=0;
if (y<0) y=0;
radius=(radius/2)*(radius/2);

if ((cx<=dx) && (cy<=dy))
{
Uint32 * __restrict pixmem32 = (Uint32*) destination->pixels  + cy*destination->w + cx;
*pixmem32=0;
}

for (true;y<=dy; y++)
{
Uint32 * __restrict source32 = (Uint32*) guts->pixels  + y*guts->w + x;
Uint32 * __restrict pixmem32 = (Uint32*) destination->pixels  + y*destination->w + x;
 for (int sx=x; sx<=dx; sx++)
 {
if (radius>=(sx-cx)*(sx-cx)+(y-cy)*(y-cy))	 *pixmem32=*source32;
	 source32++;
	 pixmem32++;
 }
}


	}
void gutsupCut(SDL_Surface* destination,SDL_Surface* guts, int x, int y, int radius )
	{
int dx=x+radius;
int dy=y+radius;
int cx=x+radius/2;
int cy=y+radius/2;
if (dx>=destination->w) dx=destination->w-1;
if (dy>=destination->h) dy=destination->h-1;
if (x<0) x=0;
if (y<0) y=0;
radius=(radius/2)*(radius/2);

if ((cx<=dx) && (cy<=dy))
{
Uint32 * __restrict pixmem32 = (Uint32*) destination->pixels  + cy*destination->w + cx;
*pixmem32=0;
}

for (true;y<=dy; y++)
{
Uint32 * __restrict source32 = (Uint32*) guts->pixels  + y*guts->w + x;
Uint32 * __restrict pixmem32 = (Uint32*) destination->pixels  + y*destination->w + x;
 for (int sx=x; sx<=dx; sx++)
 {
if (radius>=(sx-cx)*(sx-cx)+(y-cy)*(y-cy))	 *pixmem32=*source32;
	 source32++;
	 pixmem32++;
 }
}


	}
void removePixels(SDL_Surface* destination,SDL_Surface* guts, int x, int y, int radius )
	{
int dx=x+radius*2;
int dy=y+radius*2;
int cx=x+radius;
int cy=y+radius;
if (dx>=destination->w) dx=destination->w-1;
if (dy>=destination->h) dy=destination->h-1;
if (x<0) x=0;
if (y<0) y=0;
//radius=(radius/2)*(radius/2);
/*if ((cx<=dx) && (cy<=dy))
{
Uint32 * __restrict pixmem32 = (Uint32*) destination->pixels  + cy*destination->w + cx;
*pixmem32=0;
}*/

for (true;y<=dy; y++)
{
Uint32 * __restrict source32 = (Uint32*) guts->pixels  + y*guts->w + x;
Uint32 * __restrict pixmem32 = (Uint32*) destination->pixels  + y*destination->w + x;
 for (int sx=x; sx<=dx; sx++)
 {
long double newr=(sx-cx)*(sx-cx)+(y-cy)*(y-cy);
newr=sqrt(newr);
if (radius>newr+2) 
	*pixmem32=0;
else
	if (radius>=newr)
*pixmem32=*source32;
//*source32;
	 source32++;
	 pixmem32++;
 }
}


	}

int fuckwalls(SDL_Surface* destination,int x, int y,int damage)
	{
		
		//if (destination->pixels)
		if ((x<destination->w) && (y<destination->h) && (x>=0) && (y>=0))
		{
			Uint32* pix = (Uint32 *)destination->pixels + y * destination->w + x;
int fucking=*pix;
if (damage) *((Uint8*)pix+3)=max(0, *((Uint8*)pix+3)-damage);
return fucking;
	}
		return 0;
	}
void FuckWusingMaskWithInner(SDL_Surface* destination,SDL_Surface* inner, SDL_Surface* mask,int offx, int offy)
	{
		//int damage=0;
		//mask=wallFmask[0];
		offx-=mask->w/2;
		offy-=mask->h/2;
SDL_Rect src;

src.x=max(0,-offx);
src.y=max(0,-offy);
src.h=min(mask->h,destination->h-offy);
src.w=min(mask->w,destination->w-offx);

//Uint32* __restrict dpic=(Uint32*)Decayed->pixels+max(offy,0)*Decayed->w+max(0, offx); 
Uint32* __restrict dpic=(Uint32*)destination->pixels+max(offy,0)*destination->w+max(0, offx); 
Uint32* __restrict innpic=(Uint32*)inner->pixels+max(offy,0)*inner->w+max(0, offx); 
Uint32* __restrict mpic=(Uint32*)mask->pixels+(src.y)*mask->w+src.x;
int diff=src.w-src.x;
for (int y=src.y; y<src.h; y++)
{
	for (int x=src.x; x<src.w; x++)
	{

int AH=*mpic>>24;//255-min(*dpic>>24,255-(*mpic>>24));

//if (AH>10)
//{
	//AH=255;
if (AH)
{
	Uint32 tp=max((((*dpic>>8) & 0xFF0000)*(255-AH)),*innpic) & 0xFF000000;//,0xFFFFFFFF-(*mpic));//(255-AH);//max(((255-(Uint32)AH)<<24),*innpic>>24);
	//tp<<=24;
	
	int DAH=abs(int(*innpic & 0xFF) - int(*dpic & 0xFF));
	DAH*=DAH;
	DAH=min(DAH*DAH,255)*AH/255;
//if (*innpic>>24>32)
	register int idst;
register int isrc;
	
/*if (AH>128)
{
AH=(AH-128)*2;
*/
 idst=*dpic;
 isrc=*mpic;

*dpic = ((RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * DAH) >>8))) | 
		 (GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * DAH) >>8))) |
		 (BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * DAH) >>8)))) | (tp);


AH=AH*(*innpic>>24)/255;
idst=*dpic;
 isrc=*innpic;
*dpic = ((RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * AH) >>8))) 
| (GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * AH) >>8))) 
| (BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * AH) >>8))))
| (tp);



//}

//else
	//*dpic=0;//(*innpic & deAlphaMask) | tp;
}
dpic++;
mpic++;
innpic++;
	}
dpic+=destination->w-diff;
innpic+=inner->w-diff;
mpic+=mask->w-diff;
}	
	}
void FuckWusingMask(SDL_Surface* destination, SDL_Surface* mask,int offx, int offy)
	{

		//mask=wallFmask[0];
		offx-=mask->w/2;
		offy-=mask->h/2;
SDL_Rect src;

src.x=max(0,-offx);
src.y=max(0,-offy);
src.h=min(mask->h,destination->h-offy);
src.w=min(mask->w,destination->w-offx);

//Uint32* __restrict dpic=(Uint32*)Decayed->pixels+max(offy,0)*Decayed->w+max(0, offx); 
Uint32* __restrict dpic=(Uint32*)destination->pixels+max(offy,0)*destination->w+max(0, offx); 
Uint32* __restrict mpic=(Uint32*)mask->pixels+(src.y)*mask->w+src.x;
int diff=src.w-src.x;
for (int y=src.y; y<src.h; y++)
{
	for (int x=src.x; x<src.w; x++)
	{

int AH=*mpic>>24;//255-min(*dpic>>24,255-(*mpic>>24));

//if (AH>10)
//{
	//AH=255;
if (AH>32)
{
	Uint32 tp=min(*dpic>>24,255-(*mpic>>24));//(255-AH);//max(((255-(Uint32)AH)<<24),*innpic>>24);
	tp<<=24;
if (AH>10)
{
	

//if (*innpic>>24>32)
register int idst=*dpic;
register int isrc=*mpic;
*dpic = ((RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * AH) >>8))) | 
		 (GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * AH) >>8))) |
		 (BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * AH) >>8)))) | (tp);
}
//else
	//*dpic=0;//(*innpic & deAlphaMask) | tp;
}
dpic++;
mpic++;
	}
dpic+=destination->w-diff;
mpic+=mask->w-diff;
}	
	}

void FuckVexelGutsOutDoubleImage(SDL_Surface* destination, SDL_Surface* mask,int offx, int offy,int damage)
	{
		int half=destination->h/2*destination->w; 
		offx-=mask->w/2;
		offy-=mask->h/2;
SDL_Rect src;
src.x=max(0,-offx);
src.y=max(0,-offy);
src.h=min(mask->h,destination->h/2-offy);
src.w=min(mask->w,destination->w-offx);
Uint32* __restrict dpic=(Uint32*)destination->pixels+max(offy,0)*destination->w+max(0, offx); 
Uint32* __restrict innpic=(Uint32*)destination->pixels+half+max(offy,0)*destination->w+max(0, offx); 
Uint32* __restrict mpic=(Uint32*)mask->pixels+(src.y)*mask->w+src.x;
int diff=src.w-src.x;
for (int y=src.y; y<src.h; y++)
{
	for (int x=src.x; x<src.w; x++)
	{
int AH=255-min(*dpic>>24,255-(*mpic>>24));
if (AH>32)
if (AH<250)
{
	int tp=(255-(Uint32)AH)<<24;//max(((255-(Uint32)AH)<<24),*innpic>>24);
register int idst=*innpic;
register int isrc=*dpic;


*dpic=*innpic;
//*innpic=0;//(*innpic & 0x00FFFFFF) | ((*innpic/=2) & 0xFF000000);
}
dpic++;
mpic++;
innpic++;
	}
dpic+=destination->w-diff;
innpic+=destination->w-diff;
mpic+=mask->w-diff;
}	
	}

void setpixel(SDL_Surface *screen, int x, int y, int h,  Uint32 colour)
{
	
  
//shorth=h>>16;
//portion=portions[shorth+precalcecvator];

h/=dddstength;
int dshortx=(x-scrwid)*h;
x+=dshortx>>16;
int dshorty=(y-scrhid)*h;
y+=dshorty>>16;
    //Uint32 colour;  
 if ((x<0) || (y<0) || (x>=screen->w) || (y>=screen->h)) return;
    //colour = SDL_MapRGB( screen->format, r, g, b );

Uint32*    pixmem32 =(Uint32*) screen->pixels  + y*screen->w + x;
		*pixmem32= (colour & 0x00FFFFFF) | (*pixmem32 & 0xFF000000) ;

}
void setpixel3d(SDL_Surface *screen, int x, int y, int h, Uint32 colour)
{
	SDL_Rect  dest;
	Uint32*    pixmem32;
fuck=h/dddstength;
int dshortx=(x-scrwid)*fuck;
dshortx=dshortx>>16;
int dshorty=(y-scrhid)*fuck;
dshorty=dshorty>>16;
int dshorth=h>>16;
//disx = dshortx;
//disy = dshorty;
//elevation=elevation>>16;
	
portion=portions[dshorth+precalcecvator];

dest.x=x-(x-conf3d.lefteye)*(portion)+dshortx;

row=(y+dshorty) % 2; 
dest.y = y+dshorty-row;
//SDL_BlitSurface(image[no], NULL, screen, &dest);
if ((dest.x<0) || (dest.y<0) || (dest.x>=screen->w) || (dest.y>=screen->h)) return; 
pixmem32 = (Uint32*) screen->pixels  + dest.y * screen->w + dest.x;
*pixmem32 = colour;

dest.x=x-(x-conf3d.righteye)*(portion)+dshortx;//+avportion;
dest.y = y+dshorty+1-row;
if ((dest.x<0) || (dest.y<0) || (dest.x>=screen->w) || (dest.y>=screen->h)) return; 
pixmem32 = (Uint32*) screen->pixels  + dest.y * screen->w + dest.x;
*pixmem32 = colour;	
 
/*portion=portions[h+precalcecvator];
dest.x=x-(x-conf3d.lefteye)*(portion)+disx;
row=(y+disy) % 2; 
dest.y = y+disy-row;
if ((dest.x<0) || (dest.y<0) || (dest.x>=screen->w) || (dest.y>=screen->h)) return; 
pixmem32 = (Uint32*) screen->pixels  + dest.y * screen->w + dest.x;
*pixmem32 = colour;

dest.x=x-(x-conf3d.righteye)*(portion)+disx;//+avportion;
dest.y = y+disy+1-row;
if ((dest.x<0) || (dest.y<0) || (dest.x>=screen->w) || (dest.y>=screen->h)) return; 
pixmem32 = (Uint32*) screen->pixels  + dest.y * screen->w + dest.x;
*pixmem32 = colour;
*/

}
void blittoalphaShad(SDL_Surface *source, UpShadLayer *layer, int dx, int dy)
{

Uint32 *sourpic;
Uint8 *destpic;
	int x=0;
	int y=0;
	if (dx<0) x-=dx;
	if (dy<0) y-=dy;
	int destx=source->w;
	int desty=source->h;
	if (dx+destx>tyleSize) destx=(tyleSize-dx);
	if (dy+desty>tyleSize) desty=(tyleSize-dy);
for (y; y<desty; y++)
{
	sourpic=(Uint32*) source->pixels  + y*source->w + x;
	destpic=&layer->pix[(dy+y)*tyleSize + dx+x];
	for (int xx=x; xx<destx; xx++)
		{
//AH
//AH=*((Uint8 * )sourpic+3);
//if (AH>>3)
*destpic=max(*sourpic>>24,*destpic);



//}
sourpic++;
destpic++;
	//if ((x<0) || (y<0) || (x>=screen->w) || (y>=screen->h)) return 0;
    //Uint32 *pixmem32;
    //Uint32 colour;  
 
    //colour = SDL_MapRGB( screen->format, r, g, b );
  
   // pixmem32 = (Uint32*) screen->pixels  + y*screen->w + x;
  //  *pixmem32 = colour;
		}
}
		
}
int blittoalpha(SDL_Surface *source, SDL_Surface *layer, int dx, int dy)
{

Uint32 *sourpic, *destpic;
	int x=0;
	int y=0;
	if (dx<0) x-=dx;
	if (dy<0) y-=dy;
	int destx=source->w;
	int desty=source->h;
	if (dx+destx>layer->w) destx=(layer->w-dx);
	if (dy+desty>layer->h) desty=(layer->h-dy);
Uint8 AH;
for (y; y<desty; y++)
{
	sourpic=(Uint32*) source->pixels  + y*source->w + x;
	destpic=(Uint32*) layer->pixels  + (dy+y)*layer->w + dx+x;
	for (int xx=x; xx<destx; xx++)
		{
//AH
AH=*((Uint8 * )sourpic+3);
if (AH)
{
/*
*((Uint8 * )destpic+3)=max(*((Uint8 * )sourpic+3),*((Uint8 * )destpic+3));
*((Uint8 * )destpic) = (((*((Uint8 * )sourpic) -  *((Uint8 * )destpic)) * (AH))/255 + *((Uint8 * )destpic));
*((Uint8 * )destpic+1) = (((*((Uint8 * )sourpic+1) -  *((Uint8 * )destpic+1)) * (AH))/255 + *((Uint8 * )destpic+1));
*((Uint8 * )destpic+2) = (((*((Uint8 * )sourpic+2) -  *((Uint8 * )destpic+2)) * (AH))/255 + *((Uint8 * )destpic+2));
*/
register int idst= *destpic;
register int isrc= *sourpic;

*destpic = ((RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * AH) >>8))) 
| (GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * AH) >>8)))
| (BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * AH) >>8))) )
| max(*destpic & AlphaMask,*sourpic & AlphaMask);
	
	/*((RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * alpha) >>8))) 
| (GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * alpha) >>8)))
| (BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * alpha) >>8))) )
| max(*destpic & AlphaMask,*sourpic & AlphaMask);*/
}

//}
sourpic++;
destpic++;
	//if ((x<0) || (y<0) || (x>=screen->w) || (y>=screen->h)) return 0;
    //Uint32 *pixmem32;
    //Uint32 colour;  
 
    //colour = SDL_MapRGB( screen->format, r, g, b );
  
   // pixmem32 = (Uint32*) screen->pixels  + y*screen->w + x;
  //  *pixmem32 = colour;
		}
}
		return 1;
}
int alphasizeupperlayerWrong(SDL_Surface *source, UpShadLayer *layer, int dx, int dy)
{
	//if (
	int x=0;
	int y=0;
	if (dx<0) x-=dx;
	if (dy<0) y-=dy;
	int destx=source->w;
	int desty=source->h;
	if (dx+destx>tyleSize) destx=(tyleSize-dx);
	if (dy+desty>tyleSize) desty=(tyleSize-dy);
	Uint32 *sourpic;
	Uint8 *destpic;
for (y; y<desty; y++)
{
	sourpic=(Uint32*) source->pixels  + y*source->w + x;
	destpic=&layer->pix[(dy+y)*tyleSize + dx+x];
	for (int xx=x; xx<destx; xx++)
		{
if ((*sourpic>0xF8000000) && ((*sourpic>>24)>*destpic))
	*destpic= (*sourpic>>24);
sourpic++;
destpic++;
}
}
		return 1;
}
int alphasizeupperlayer(SDL_Surface *source, SDL_Surface *layer, int dx, int dy)
{
	//if (
	int x=0;
	int y=0;
	if (dx<0) x-=dx;
	if (dy<0) y-=dy;
	int destx=source->w;
	int desty=source->h;
	if (dx+destx>layer->w) destx=(layer->w-dx);
	if (dy+desty>layer->h) desty=(layer->h-dy);
	Uint32 *sourpic, *destpic;
for (y; y<desty; y++)
{
	sourpic=(Uint32*) source->pixels  + y*source->w + x;
	destpic=(Uint32*) layer->pixels  + (dy+y)*layer->w + dx+x;
	for (int xx=x; xx<destx; xx++)
		{
if ((*sourpic>0xF8000000) && (*sourpic>*destpic))
	*destpic=(*destpic & 0x00FFFFFF) | (*sourpic & 0xFF000000);
sourpic++;
destpic++;
}
}
		return 1;
}

void stereoscopizeimage(SDL_Surface * input )
{
Uint32*    pixmem32;
Uint32* source32;
	SDL_Surface * shitty;
	//Get offsets
	//Blit
shitty=rotozoomSurfaceXY(input, 0, 1, 0.5, 1);	

    pixmem32 = (Uint32*) input->pixels;
	source32 = (Uint32*) shitty->pixels;
for (int i=0; i<input->h/2;i++)
{
	for (int j=0; j<input->w;j++)
	{
	*pixmem32=0; pixmem32++;
	}
	 memcpy ( pixmem32, source32, input->w );

	 pixmem32+=input->w;
	source32+=input->w;

}

if (input->h % 2) 
	for (int j=0; j<input->w;j++)
	{
	*pixmem32=0; pixmem32++;
	}

SDL_FreeSurface (shitty);
}
void stereoscopizebg(SDL_Surface * input )
{
	SDL_Surface * shitty;
	//Get offsets
	//Blit
shitty=rotozoomSurfaceXY(input, 0, 1, 0.5, 1);	
Uint32*    pixmem32;
Uint32* source32;
    pixmem32 = (Uint32*) input->pixels;
	source32 = (Uint32*) shitty->pixels;
for (int i=0; i<input->h/2;i++)
{
	 memcpy ( pixmem32, source32, input->w );
	 pixmem32+=input->w;
	source32+=input->w;
		for (int j=0; j<input->w;j++)
	{
	*pixmem32=0; pixmem32++;
	}

}

SDL_FreeSurface (shitty);
}

// Remove sector change chack for productivity

void prodata(string someshit, double value, int no)
{

//***************************************
std::stringstream text;
text<<someshit<<"="<<value;
message = TTF_RenderText_Solid( font, text.str().c_str(), textColor ); 
apply_surface( 5, scrhid+15*no, message, screen );
SDL_FreeSurface( message );
}
void showtext(string someshit, int x, int y)
{

//***************************************
std::stringstream text;
text<<someshit;
message = TTF_RenderText_Solid( font, text.str().c_str(), textColor ); 
apply_surface( x, y, message, screen );
SDL_FreeSurface( message );
}

int makeshadow (SDL_Surface* shadow)
{

	//Uint8 aB;
	Uint32* pixmem32;
	Uint32* pixpre;
	Uint32 *pixup;
	pixmem32=(Uint32*) shadow->pixels +1+shadow->w;
	pixpre=(Uint32*) shadow->pixels+shadow->w;
	pixup=(Uint32*) shadow->pixels+1;
	for (int x=2; x<shadow->w-1; x++)
		for (int y=2; y<shadow->h-1; y++)
		{
//Uint8 = 
*pixmem32 = (int)(((*((Uint8* )pixmem32 + 3))+(*((Uint8* )pixpre + 3))+(*((Uint8* )pixup + 3)))/3)<<24;
//*pixmem32 = (*pixmem32/16777216)*16777216;
//pixmem32=pixmem32+1;
			pixmem32++;
			pixpre++;
			pixup++;
		}
		return 1;
}

// ***************************************************************** Load stuff

int loadoverlay(std::string filename, int no)
{
	//overlayimage[200]
temp = IMG_Load( filename.c_str()   );
if( temp != NULL )
{
if (OpenGLscreen){
overlayimage[no] = temp;
return 1;}
overlayimage[no]=temp;
return 1;
}
else 
	SDL_FreeSurface(temp);
	return 0;
}
void loaddisplacement (std::string filename, int no)
{
	temp = IMG_Load( filename.c_str()   );
if( temp != NULL )
{
		//Create an optimized image
if (OpenGLscreen){
displacement[no] = temp;
return;}
displacement[no]=temp;

}
else
	displacement[no]=NULL;

}
int loadwall (std::string filename, int no)
{
	temp = IMG_Load( filename.c_str()   );
if( temp != NULL )
{
		//Create an optimized image
if (OpenGLscreen){
wall[no] = temp;
return 1;}
wall[no]=temp;
		return 1;
}

//brickshadow
	return 0;
}
SDL_Surface * loadany (std::string filename)
{
	SDL_Surface * dtemp;
	temp = IMG_Load( filename.c_str()   );


if (temp)	
return temp;


	return 0;
}
int loadimages (std::string filename, int no)
{

SDL_Surface *temp;
temp= IMG_Load( filename.c_str()   );
	if (OpenGLscreen){
	image[no]= temp;return (temp!=0);}//IMG_Load( filename.c_str()   );


/*if ( temp!=NULL ) 
{ 
        // get the number of channels in the SDL surface
        nOfColors = temp->format->BytesPerPixel;
        if (nOfColors == 4)     // contains an alpha channel
        {
                if (temp->format->Rmask == 0x000000ff)
                        texture_format = GL_RGBA;
                else
                        texture_format = GL_BGRA;
        } else if (nOfColors == 3)     // no alpha channel
        {
                if (temp->format->Rmask == 0x000000ff)
                        texture_format = GL_RGB;
                else
                        texture_format = GL_BGR;
        } else {
                printf("warning: the image is not truecolor..  this will probably break\n");
                // this error should not go unhandled
        }
 
	// Have OpenGL generate a texture object handle for us
	glGenTextures( 1, &temptex);
 
	// Bind the texture object
	glBindTexture( GL_TEXTURE_2D, temptex );
	glimage[no] = temptex;
 
	// Set the texture's stretching properties
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
 
	// Edit the texture object's image data using the information SDL_Surface gives us
	glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, temp->w, temp->h, 0,
                      texture_format, GL_UNSIGNED_BYTE, temp->pixels );
} */

 
// Free the SDL_Surface only if it was successfully created



if( temp )
{
image[no] = temp;
return 1;
}
else 
SDL_FreeSurface(temp);
return 0;
}
int loadmergeimage (std::string filename, int no)
{
temp = IMG_Load( filename.c_str()   );
if (temp)
{
	if (OpenGLscreen){
mergeimage[no]= temp;
return (temp!=0);}
		mergeimage[no] = temp;
return 1;
}
return 0;
}
bool loadAllmergeimages (std::string filename, std::string filenamed, std::string filenamem, std::string filenameo, int no)
{
	bool success=false;

temp = IMG_Load( filenameo.c_str()   );
if (temp)
{
if (OpenGLscreen){
overlayimage[no] = temp;
}else{
overlayimage[no]=temp;
}
success=1;
}


temp = IMG_Load( filenamed.c_str()   );
if (temp)
{

	if (OpenGLscreen){
mergeimaged[no] = temp;
}else{
mergeimaged[no]	= temp; 
	}

success=1;
}

temp = IMG_Load( filenamem.c_str()   );
if (temp)
{
	if (OpenGLscreen){
mergeimagem[no] = temp;
}else{
mergeimagem[no]	= temp;

	}

success=1;
}

temp = IMG_Load( filename.c_str()   );
if ( temp) 

{ 
    /*    // get the number of channels in the SDL surface

        nOfColors = temp->format->BytesPerPixel;
        if (nOfColors == 4)     // contains an alpha channel
        {
                if (temp->format->Rmask == 0x000000ff)
                        texture_format = GL_RGBA;
                else
                        texture_format = GL_BGRA;
        } else if (nOfColors == 3)     // no alpha channel
        {
                if (temp->format->Rmask == 0x000000ff)
                        texture_format = GL_RGB;
                else
                        texture_format = GL_BGR;
        } else {
                printf("warning: the image is not truecolor..  this will probably break\n");
                // this error should not go unhandled
        }
 
	// Have OpenGL generate a texture object handle for us
	glGenTextures( 1, &temptex);
 
	// Bind the texture object
	glBindTexture( GL_TEXTURE_2D, temptex );
	mglimage[no] = temptex;
 
	// Set the texture's stretching properties
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
 
	// Edit the texture object's image data using the information SDL_Surface gives us
	glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, temp->w, temp->h, 0,
                      texture_format, GL_UNSIGNED_BYTE, temp->pixels );
					  */

		if (OpenGLscreen){
mergeimage[no] = temp;
}else{
mergeimage[no]	= temp;

		}

success=1;
}

return success;
}

// ********************************************************************** Drow functions
void drow (int onscreenx, int onscreeny, SDL_Surface * k, int elevation)
{
	SDL_Rect qdest;
	onscreenx+=((onscreenx-scrwi/2)*elevation)/dddstength;
	onscreeny+=((onscreeny-scrhi/2)*elevation)/dddstength;
qdest.x=onscreenx;
	qdest.y=onscreeny;
	SDL_BlitSurface(k, NULL, screen, &qdest);

}
int nooffsetdrow (int onscreenx, int onscreeny, int k)
{
SDL_Rect qdest;
if (image[k]==NULL) return 0;
//sdlblitstyle(onscreenx,onscreeny,image[k],screen);
qdest.x=onscreenx;
	qdest.y=onscreeny;
	SDL_BlitSurface(image[k], NULL, screen, &qdest);
return 1;
}
void GLDrow (int x, int y, int z, int no)
{
	return; 
//	if (no==0) return;
	//y=scrhi-y;
/*	int h = image[no]->h+y;
	int w = image[no]->w+x;
	GLfloat zz=z/accuracy;
//	glBindTexture( GL_TEXTURE_2D, glimage[no]);
  //glTranslatef( -gCameraX, -gCameraY, 0.f );
glBegin( GL_QUADS );
	//Bottom-left vertex (corner)
	glTexCoord2i( 0, 0 );
	glVertex3f( (GLfloat)x, (GLfloat)y, zz );
 
	//Bottom-right vertex (corner)
	glTexCoord2i( 1, 0 );
	glVertex3f( (GLfloat)w, (GLfloat)y, zz );
 
	//Top-right vertex (corner)
	glTexCoord2i( 1, 1 );
	glVertex3f( (GLfloat)w, (GLfloat)h, zz );
 
	//Top-left vertex (corner)
	glTexCoord2i( 0, 1 );
	glVertex3f( (GLfloat)x, (GLfloat)h, zz );
glEnd();
*/
}
void mGLDrow (int x, int y, int z, int no)
{

}
void sdlnoblit (int onscreenx, int onscreeny, SDL_Surface * surface, SDL_Surface* destination)
{
}
void nodrow (int onscreenx, int onscreeny, int elevation, SDL_Surface * surface)
{

}
void nodrow1 (int onscreenx, int onscreeny, int elevation, int x, int y)
{

}
// ****************************** SDL
void sdlanyblit (int onscreenx, int onscreeny, SDL_Surface * surface, SDL_Surface* destination)
{
	SDL_Rect qdest;
qdest.x = onscreenx;
qdest.y = onscreeny;
SDL_BlitSurface(surface, NULL, destination, &qdest);
}
void sdlanydrow (int onscreenx, int onscreeny, int elevation, SDL_Surface * surface)
{
	SDL_Rect  qdest;
//3d effect:
fuck=elevation/dddstength;
int dshortx=(onscreenx-scrwid)*fuck;
dshortx=dshortx>>16;
int dshorty=(onscreeny-scrhid)*fuck;
dshorty=dshorty>>16;
qdest.x = onscreenx + dshortx;
qdest.y = onscreeny + dshorty;

SDL_BlitSurface(surface, NULL, screen, &qdest);
}
void sdlanydrow3d (int onscreenx, int onscreeny, int elevation, SDL_Surface * surface)
{
	SDL_Rect dest;
fuck=elevation/dddstength;
int dshortx=(onscreenx-scrwid)*fuck;
dshortx=dshortx>>16;
int dshorty=(onscreeny-scrhid)*fuck;
dshorty=dshorty>>16;
disx = dshortx;
disy = dshorty;
elevation=elevation>>16;

portion=portions[elevation+precalcecvator];
dest.x=onscreenx-(onscreenx-conf3d.lefteye)*(portion)+disx;
row=(onscreeny+disy) % 2; 
dest.y = onscreeny+disy-row;
SDL_BlitSurface(surface, NULL, screen, &dest);
dest.x=onscreenx-(onscreenx-conf3d.righteye)*(portion)+disx;//+avportion;
dest.y = onscreeny+disy+1-row;
SDL_BlitSurface(surface, NULL, screen, &dest);
}
void anydrow (int onscreenx, int onscreeny, int elevation, int no)
{
	//return;
	SDL_Rect qdest;
elevation/=dddstength;
qdest.x=onscreenx+=((onscreenx-scrwid)*elevation)>>16;
qdest.y=onscreeny+=((onscreeny-scrhid)*elevation)>>16;
//qdest.x=onscreenx;
//qdest.y=onscreeny;
SDL_BlitSurface(image[no], NULL, screen, &qdest);
}
void anydrownooffset (int onscreenx, int onscreeny, int elevation, int no)
{
	SDL_Rect qdest;
qdest.x = onscreenx;
qdest.y = onscreeny;
SDL_BlitSurface(image[no], NULL, screen, &qdest);

}
void manydrow (int onscreenx, int onscreeny, int elevation, int mno)
{
	SDL_Rect qdest;
//3d effect:
elevation/=dddstength;

//float x=onscreenx+(onscreenx-scrwid)*elevation;
//float y=onscreeny+(onscreeny-scrhid)*elevation;



qdest.x=onscreenx+=((onscreenx-scrwid)*elevation)>>16;
qdest.y=onscreeny+=((onscreeny-scrhid)*elevation)>>16;

SDL_BlitSurface(mergeimage[mno], NULL, screen, &qdest);

}
void anydrow3d (int onscreenx, int onscreeny, int elevation, int no)
{
SDL_Rect dest;
fuck=elevation;
fuck/=dddstength;
int dshortx=(onscreenx-scrwid)*fuck;
dshortx=dshortx>>16;
int dshorty=(onscreeny-scrhid)*fuck;
dshorty=dshorty>>16;
elevation=elevation>>16;
	
portion=portions[elevation+precalcecvator];
dest.x=onscreenx-(onscreenx-conf3d.lefteye)*(portion)+dshortx;

//dest.y=onscreeny+dshorty;
dshorty+=onscreeny;
row=dshorty % 2; 
dest.y=dshorty-=row;

//dest.y-=row;
SDL_BlitSurface(image[no], NULL, screen, &dest);
dest.x=onscreenx-(onscreenx-conf3d.righteye)*(portion)+dshortx;//+avportion;
dest.y=dshorty+1;// = onscreeny+dshorty+1-row;
SDL_BlitSurface(image[no], NULL, screen, &dest);
}
void manydrow3d (int onscreenx, int onscreeny, int elevation, int mno)
{
SDL_Rect dest;
fuck=elevation/dddstength;
int dshortx=(onscreenx-scrwid)*fuck;
dshortx=dshortx>>16;
int dshorty=(onscreeny-scrhid)*fuck;
dshorty=dshorty>>16;
disx = dshortx;
disy = dshorty;
elevation=elevation>>16;
	
	
portion=portions[elevation+precalcecvator];
dest.x=onscreenx-(onscreenx-conf3d.lefteye)*(portion)+disx;

row=(onscreeny+disy) % 2; 
dest.y = onscreeny+disy-row;
SDL_BlitSurface(mergeimage[mno], NULL, screen, &dest);
dest.x=onscreenx-(onscreenx-conf3d.righteye)*(portion)+disx;//+avportion;
dest.y = onscreeny+disy+1-row;
SDL_BlitSurface(mergeimage[mno], NULL, screen, &dest);
}


//************************** Drow BACKGROUND
void drowbg (int onscreenx, int onscreeny, int gx, int gy)
{
	SDL_Rect  dest;
/*src.x = 0;
if (onscreenx<0) src.x-=onscreenx;
src.y = 0;
if (onscreeny<0) src.y-=onscreeny;
src.w = tyleSize-src.x;
src.h = tyleSize-src.y;*/
dest.x = onscreenx;//+src.x;
dest.y = onscreeny;//+src.y;
//dest.w = tyleSize+onscreenx-src.x;
//dest.h = tyleSize+onscreeny-src.y;

//	try
	//	{
	//	if (storedmp[gx+1][gy+1]!=NULL)
if (player.host->pos.h<0)		
SDL_BlitSurface(storedmp[gx+1][gy+1], NULL, screen, &dest);
else

SDL_BlitSurface(mainmaps[gx+1][gy+1], NULL, screen, &dest);
	
//}
//catch(int a)
//{
	//gx=gx;
//}

}
void drowMemCoby(int x, int y, Uint32*  scr, Uint32* source32 ){

SDL_Rect src;
src.x = 0;
if (x<0) src.x-=x;
src.y = 0;
if (y<0) src.y-=y;
if ((src.x>tyleSize) || (src.y>tyleSize))
	return;

src.w = tyleSize-src.x;
if (src.w+x*(x>0)>scrwi) src.w=(scrwi-x*(x>0));
src.h = tyleSize-src.y;
if (src.h+y*(y>0)>scrhi) 
	src.h=(scrhi-y*(y>0));
if ((src.w<0) || (src.h<0) || (src.w>tyleSize) || (src.h>tyleSize)) 
	return;

source32+=src.y*1024 +src.x;
scr+=(y+src.y)*scrwi+x+src.x;

for (int i=0; i<src.h; i++)
{
	memcpy ( scr, source32, src.w*4 );
	source32+=1024;
	scr+=scrwi;
}

return;
}
void drowMemCobyOutline(int x, int y, Uint32*  dst, Uint32* source32, int outx, int outy, int outw, int outh ){

SDL_Rect src;
src.x = 0;
if (x<0) src.x-=x;
src.y = 0;
if (y<0) src.y-=y;
if ((src.x>tyleSize) || (src.y>tyleSize))
	return;

src.w = tyleSize-src.x;
if (src.w+x*(x>0)>scrwi) src.w=(scrwi-x*(x>0));
src.h = tyleSize-src.y;
if (src.h+y*(y>0)>scrhi) 
	src.h=(scrhi-y*(y>0));
if ((src.w<0) || (src.h<0) || (src.w>tyleSize) || (src.h>tyleSize)) 
	return;



for (int i=0; i<src.h; i++)
{
int realY=y+src.y+i;
Uint32* sour=source32+(src.y+i)*1024 +src.x;
Uint32* dest=dst+(realY)*scrwi+x+src.x;

if ((realY<outy) || (realY>outh))
memcpy ( dest, sour, src.w*4 );
else{
int realX=x+src.x;
int before=min(outx-realX,src.w);
if (before>0) memcpy ( dest, sour, before*4 );

int after=max(0, outw - realX);
if (src.w>after) memcpy ( dest+after, sour+after, (src.w-after)*4 );

}
}

return;
}
void drowbgmmcpy (int onscreenx, int onscreeny, int gx, int gy)
{
Uint32* pixmem32;
Uint32* source32;


if (player.isunderground)
source32 = (Uint32*) storedmp[gx+1][gy+1]->pixels;
else
source32 = (Uint32*) mainmaps[gx+1][gy+1]->pixels;
pixmem32 = (Uint32*) screen->pixels;

drowMemCoby(onscreenx, onscreeny ,pixmem32, source32 );



return;
}
void drowbg3dbase (int onscreenx, int onscreeny, int gx, int gy, bool iscaved)
{
		onscreeny+=abs(onscreeny % 2)-1;  
//onscreeny=onscreeny/2*2-1;
//float portion=conf3d.bgdepth;
//portion/=(conf3d.bgdepth+conf3d.tomonitor); 
onscreenx-=conf3d.bgprepare;

SDL_Rect src, dest;
src.x = 0;
if (onscreenx<0) src.x-=onscreenx;
src.y = 0;
if (onscreeny<0) src.y-=onscreeny;
src.w = tyleSize-src.x;
src.h = tyleSize-src.y;
dest.x = onscreenx+src.x;
dest.y = onscreeny+src.y;
SDL_BlitSurface(mainmaps[gx+1][gy+1], &src, screen, &dest);

if (!iscaved) return;
dest.y+=1;
onscreenx+=2*conf3d.bgprepare;
src.x = 0;
if (onscreenx<0) src.x-=onscreenx;
src.w = tyleSize-src.x;
dest.x = onscreenx+src.x;
SDL_BlitSurface(mainmaps[gx+1][gy+1], &src, screen, &dest);


}
void drowbg3d (int onscreenx, int onscreeny, int gx, int gy)
{
	Uint32*    pixmem32;
Uint32* source32;
	//int mod=0;
//onscreeny=onscreeny/2*2;
	//if (onscreeny % 2)  
		onscreeny+=abs(onscreeny % 2);  
		//if (onscreeny>0)
			//mod=-1;
		//else mod=1;
		//onscreeny+=mod;
SDL_Rect src, dest;
onscreenx+=conf3d.bgprepare;
src.x = 0;
if (onscreenx<0) src.x-=onscreenx;
src.y = 0;
if (onscreeny<0) src.y-=onscreeny;
if ((src.x>tyleSize) || (src.y>tyleSize))
	return;
src.w = tyleSize-src.x;
if (src.w+onscreenx*(onscreenx>0)>scrwi) src.w=(scrwi-onscreenx*(onscreenx>0));
src.h = tyleSize-src.y;
if (src.h+onscreeny*(onscreeny>0)>scrhi) 
	src.h=(scrhi-onscreeny*(onscreeny>0));
if ((src.w<0) || (src.h<0) || (src.w>tyleSize) || (src.h>tyleSize)) 
	return;
dest.x = onscreenx+src.x;
dest.y = onscreeny+src.y;
source32 = (Uint32*) mainmaps[gx+1][gy+1]->pixels + src.y*mainmaps[gx+1][gy+1]->w +src.x;
pixmem32 = (Uint32*) screen->pixels+dest.y*scrwi+dest.x;
for (int i=0; i<src.h/2; i++)
{
	memcpy ( pixmem32, source32, src.w*4 );
	source32+=2*mainmaps[gx+1][gy+1]->w;
	pixmem32+=scrwi*2;
}

//onscreeny--;
onscreenx-=2*conf3d.bgprepare;
src.x = 0;
if (onscreenx<0) src.x-=onscreenx;
//src.y = 0;
//if (onscreeny<0) src.y-=onscreeny;
src.w = tyleSize-src.x;
if (src.w+onscreenx*(onscreenx>0)>scrwi) src.w=(scrwi-onscreenx*(onscreenx>0));
//src.h = tyleSize-src.y;
//if (src.h+onscreeny*(onscreeny>0)>scrhi) 
//	src.h=(scrhi-onscreeny*(onscreeny>0));
if ((src.w<0) || (src.w>tyleSize)) 
	return;
dest.x = onscreenx+src.x;
//dest.y = onscreeny+src.y;
source32 = (Uint32*) mainmaps[gx+1][gy+1]->pixels + src.y*mainmaps[gx+1][gy+1]->w +src.x;
pixmem32 = (Uint32*) screen->pixels+dest.y*scrwi+dest.x;
//source32+=mainmaps[gx+1][gy+1]->w;
pixmem32+=scrwi;
for (int i=0; i<src.h/2; i++)
{

	memcpy ( pixmem32, source32, src.w*4 );
	source32+=2*mainmaps[gx+1][gy+1]->w;
	pixmem32+=scrwi*2;
}





return;
}
void drowbgGL (int onscreenx, int onscreeny, int gx, int gy)
{
	return; 
}
void drowmerge (int onscreenx, int onscreeny, int k)
{
	if (mergeimage[k]==NULL) return;
	SDL_Rect src, dest;
src.x = 0;
if (onscreenx<0) src.x-=onscreenx;
src.y = 0;
if (onscreeny<0) src.y-=onscreeny;
src.w = mergeimage[k]->w-src.x;
src.h = mergeimage[k]->h-src.y;
dest.x = onscreenx+src.x;
dest.y = onscreeny+src.y;
dest.w = mergeimage[k]->w+onscreenx-src.x;
dest.h = mergeimage[k]->h+onscreeny-src.y;

//sdlblitstyle(dest.x,dest.y,mergeimage[k],screen);
SDL_BlitSurface(mergeimage[k], NULL, screen, &dest);
}

void tyleaddobj (int type, int tx, int ty, int x, int y)
{
	return;
	std::stringstream ss;
    ss<<tx<<"v"<<ty;
    std::string blbl;
    ss>>blbl;    
	 std::ofstream tada ( "world/"+planetname+"/"+planetname+"_" + blbl + ".map", ios::app );
tada << type;
tada << " ";
tada << tx; //x coord of the tile, obcect is located
tada << " ";
tada << ty; 
tada << " ";
tada << x;
tada << " ";
tada << y; 
tada << " ";
tada << now; 
tada << " ";
tada << edit.mlev;
tada << " ";
tada << edit.slev;
tada << " ";
if (imageVoxel[type])
{
tada << globxh; 
tada << " ";
tada << globyh;
tada << " ";
tada << glob;
tada << " ";
}
tada << edit.folliage;
tada << " ";


tada.close();
}
int generatesector(int selectedsurface, std::string strx, int tx, int ty)
{
	return 1;
}

void saveGame ()
{
	std::ofstream map( "juststuff/lazy.map" );
	map << mouse.pos.x; 
	map << " ";
	map << mouse.pos.y;
	map << " ";
	map << mouse.state;
	map << " ";
	map << camerax;
	map << " ";
	map << cameray;
	map << " ";
	map << actcamx;
	map << " ";
	map << actcamy;
	map << " ";
	map << cameratylx, 
	map << " ";
	map << cameratyly; 
	map << " ";
	map << gridx; 
	map << " ";
	map << gridy; 
	map << " ";
	map << player.host->pos.tx;
	map << " ";
	map << player.host->pos.ty;
	map << " ";
	map << player.host->pos.x;
	map << " ";
	map << player.host->pos.y;
	map << " ";
	map << player.host->pos.h ;
	map << " ";
	map << player.host->hp;
	map << " ";
	map << player.corporial;
	map << " ";
	map.close();
}
void loadGame ()
{
	std::ifstream map( "juststuff/lazy.map" );
	if ( map != NULL ) 
{
	map >> mouse.pos.x; 
	map >> mouse.pos.y;
	map >> mouse.state;
	map >> camerax;
	//player.host->pos.x=camerax+200;
	map >> cameray;
	//player.host->pos.y=cameray+200;
	map >> actcamx;
	map >> actcamy;
	map >> cameratylx,
		//player.host->pos.tx=cameratylx;
	map >> cameratyly; 
	//player.host->pos.ty=cameratyly;
	map >> gridx; 
	map >> gridy; 
	map >> player.host->pos.tx;
	map >> player.host->pos.ty;
	map >> player.host->pos.x;
	map >> player.host->pos.y;
	map >> player.host->pos.h ;

	
	map >> player.host->hp;
	player.host->hp=max(player.host->hp, 5000);
	map >> player.corporial;
	//player.corporial=0;
	map.close();
}

}
void loadhud(std::string filename, int no)
{
temp = IMG_Load( filename.c_str()   );
if( temp != NULL )	
	{ 
		//Create an optimized image
		hudd[no] = temp;	
	}
	//SDL_FreeSurface(temp);
}


void JumpToSpace()
{
	quit=1;
	SDL_ShowCursor(1);
	player.NextGameState=4;
}
void TryGetCorporial()
{
	int got;
//getmematrix (player.host->pos.tx, player.host->pos.ty, x-(theta*z)/divfuck, y-(thetb*z)/divfuck, d-(dh*z)/divfuck);
	for (int i=accurate(player.host->pos.x)-128;i<accurate(player.host->pos.x)+128; i+=pixInPhis)
		for (int j=accurate(player.host->pos.y)-128;j<accurate(player.host->pos.y)+128; j+=pixInPhis)
			for (int h=accurate(player.host->pos.h)-128;h<accurate(player.host->pos.h)+128; h+=pixInPhis)
				if (((got=getmematrix (player.host->pos.tx, player.host->pos.ty, i, j, h))>=einmatr) && (got<phinmatr)) goto corporiate;

	return;

corporiate:
	//e[got-einmatr].hp=-1;
	int eeno=got-einmatr;
	player.host=&e[eeno];
	playerW=&e[eeno].walk;
	e[eeno].controled=1;
	quit=1;
	player.NextGameState=2;
	//player.health=100;
	player.corporial=1;
}
void ZeroCharAssigner()
{
	int i=0;
	e[i].type=613;
		
		eassigner(i);
		

		int hig=accurate(e[i].pos.h)/pixInPhis+matrdepth;
		if (efirst[hig]) efirst[hig]->prev=&e[i];
		e[i].prev=NULL;
		e[i].next=efirst[hig];
		
		efirst[hig]=&e[i]; 
		e[i].i=i;

		player.host=&e[i];
	playerW=&e[i].walk;
	e[i].controled=1;
	
	player.bag.RightHand.count=1;
	player.bag.RightHand.type=1;//1;
	player.bag.LeftHand.type=2+2*(!CNSTminigun);//4-rifle 2-minigun;
	player.bag.LeftHand.count=1;
	player.bag.pack[3][3].count=1;
	player.bag.pack[3][3].type=2+2*CNSTminigun;
	player.bag.pack[3][4].count=1;
	player.bag.pack[3][4].type=3;

	// character light
	
	player.flashLight.active=1;
	player.flashLight.color=PlayerFlashLightColor;
	player.flashLight.dinamic=1;
	player.flashLight.distanceFading=0.0007;//0.0005;
	player.flashLight.angleLight=1;
	player.flashLight.angleFading=0.09;
	player.flashLight.brightness=5;
	
	player.host->hp=20100;

	player.bag.LeftHand.overheating=1;
player.bag.RightHand.overheating=1;

for (int bx=0; bx<8; bx++)
				for (int by=0; by<8; by++)
					player.bag.pack[bx][by].count=0;
player.bag.LeftHand.count=1;
player.bag.RightHand.count=1;

player.host->walk.itemInHand=&player.bag.RightHand;



if ((betaExitOnDeath) || (Betatesting)) { player.corporial=true; } 
}
void TryGetDead()
{
//if (GodMode) player.host->hp=100;
	if (player.host->pos.h<0) player.host->pos.h=0;
	if (player.host->hp<1) //|| (player.host->pos.h<-accuracy*8))
	{
		player.host->pos.h=accuracy*16;
		player.host->controled=0;
		//ZeroCharAssigner();
	quit=1;
	//player.NextGameState=1;
	//player.corporial=0;
	//SDL_BlitSurface(image[46],NULL,screen,NULL);
	}
	//player.health=0;
}
// ************************************** Stuff spawners 
void placephisical(int tx, int ty, int x, int y,int h , int type, int destx, int desty, int desx, int desy, int desh, int speed, int mass, int size, int power, int solid, float slowing, int hp)
{
int i=freephisslot; 
while ((ph[i].type>0) && (i<phlim)) i++;
if (i>=phlim) i=1; 
ph[i].voxelPalette=0;
ph[i].voxelPaletteCopy=0;
ph[i].type=type;
ph[i].BlitOnDeath=true;
ph[i].mortal=true;
ph[i].i=i;
ph[i].pos.tx=tx;
ph[i].pos.ty=ty;
ph[i].hp=hp;
ph[i].starthp=ph[i].hp;
ph[i].clock=0;
ph[i].pos.x=x<<16;
ph[i].pos.y=y<<16;
ph[i].pos.h=h<<16;
//Prygnist
ph[i].force.pr=30; //100
ph[i].force.mass=mass;
ph[i].slowing=slowing;
ph[i].solidness=solid;
ph[i].power=power;
ph[i].foo=foophi;
ph[i].collide=foophi;
ph[i].force.speedx=0;
ph[i].force.speedy=0;
ph[i].force.speedh=0;
ph[i].angles.Ah=0;
ph[i].angles.Ax=0;
ph[i].angles.Ay=0;
ph[i].g8particle=0;
ph[i].skip=0;
int hig=min(accurate(ph[i].pos.h)/pixInPhis, matrhig-1)+matrdepth;
if (phfirst[hig]!=NULL) 
	phfirst[hig]->prev=&ph[i];
ph[i].prev=NULL;
ph[i].next=phfirst[hig];
phfirst[hig]=&ph[i]; 
freephisslot=i+1;

}
int addphisical(int tx, int ty, int x, int y,int h , int type, int destx, int desty, int desx, int desy, int desh, int speed, int mass, int size, int power, int solid, float slowing, int hp)
{int i=freephisslot; int tanx, tany; float tange;
while ((ph[i].type>0) && (i<phlim)) i++;
if (i>=phlim) {i=freephisslot=0; while ((ph[i].type>0) && (i<phlim)) i++;
if (i>=phlim)  return 0;} 
ph[i].G_p=0;
ph[i].G_c=0;
ph[i].type=type;
ph[i].mortal=true;
ph[i].BlitOnDeath=true;
ph[i].voxelPalette=0;
ph[i].voxelPaletteCopy=0;
ph[i].pos.tx=tx;
ph[i].pos.ty=ty;
ph[i].starthp=ph[i].hp=hp;
ph[i].starthp=ph[i].hp;
ph[i].clock=0;
ph[i].pos.x=x;
ph[i].pos.y=y;
ph[i].pos.h=h;
ph[i].angles.Ah=0;
ph[i].angles.Ax=0;
ph[i].angles.Ay=0;
ph[i].lightTrack=createData.lightPisObj;
ph[i].lightIntensity=createData.lightIntensity;
ph[i].displayDelay=createData.displayTime;
ph[i].skip=0;
//Prygnist
ph[i].g8particle=0;
ph[i].force.pr=30; //100
ph[i].force.mass=mass;
ph[i].slowing=slowing;
ph[i].solidness=solid;
ph[i].power=power;
ph[i].foo=foophi;
ph[i].collide=foophi;
ph[i].i=i;
if (type=17) ph[i].force.pr=20; 
tanx=(tx-destx)*tyleSize+(accurate(x)-desx);
tany=(ty-desty)*tyleSize+(accurate(y)-desy);
tange=(tanx*tanx+tany*tany);
tange=sqrt(tange);
if (tange!=0) ph[i].force.speedx=-speed*(accuracy*tanx/tange); else ph[i].force.speedx=accuracy*speed;
if (tange!=0) ph[i].force.speedy=-speed*(accuracy*tany/tange); else ph[i].force.speedy=accuracy*speed;
//dem=accurate(h-desh)*gravity;//*gravity;
//dem/=(tange+dem*2);
//dem=1-dem;
//if (dem>0)
ph[i].force.speedh=tange*(gravity*accuracy/speedoftime/GameSpeedModifier)/speed;
ph[i].force.speedh-= (h-desh)*speed/tange;
//ph[i].force.speedh*=(fact(ph[i].force.speedh)-dem)/(fact(ph[i].force.speedh));

//ph[i].force.speedh*=calc;
//ph[i].force.speedh*=fact(ph[i].force.speedh)
//ph[i].force.speedh*=tange*gravity*accuracy/2/speed/speedoftime;
//else 
//ph[i].force.speedh=(tange*gravity*accuracy/2/speed/speedoftime);
int hig=min(accurate(ph[i].pos.h)/pixInPhis, matrhig-1)+matrdepth;
if (phfirst[hig]!=NULL) 
	phfirst[hig]->prev=&ph[i];
ph[i].prev=NULL;
ph[i].next=phfirst[hig];
phfirst[hig]=&ph[i];
freephisslot=i+1;

return i;
}
int addphisicalrandom()//int tx, int ty, int x, int y, int h, int type, int xspeed, int yspeed, int hspeed, int randosity, int numberof, int flooidness, float slowing)
{
if (noBloodVacancyThisTurn) return 0;
	int i=bphcache; int tanx, tany, rany;  int hig;
int dosity=createData.randosity*100;
tanx=accurate(createData.x) - actcamx - (cameratylx - createData.tx)*tyleSize;
tany=accurate(createData.y) - actcamy - (cameratyly - createData.ty)*tyleSize;
if ((tanx<0) || (tanx>scrwi) || (tany<0) || (tany>scrhi)) return 0;
//if ((createData.tx<cameratylx) || (createData.ty<cameratyly))  return 0;
for (int f=0; f<createData.numberof; f++)
{
	while (bl[i].type) i++;
	if (i>=bphlim) { bphcache=2; noBloodVacancyThisTurn=1; return 0;} 
bl[i].type=createData.type;
bl[i].pos.tx=createData.tx;
bl[i].pos.ty=createData.ty;
bl[i].clock=0;
bl[i].flooidness=createData.flooidness;
bl[i].lightParticle=createData.lightPisObj;
//bl[i].slowing=createData.slowing;
bl[i].pos.x=createData.x;
bl[i].pos.y=createData.y;
bl[i].pos.h=createData.h;
//bl[i].pr=pr; //100
rany= rand() % 100;
bl[i].speedx= (createData.xspeed * rany)/100 + radiusx(dosity*5);
bl[i].speedy= (createData.yspeed * rany)/100 + radiusy;
bl[i].speedh=+rand() % dosity*accuracy/20000;
bl[i].speedh-=bl[i].speedh*(bl[i].speedh<0)*3;
bl[i].speedh+=createData.hspeed;
hig=min (accurate(bl[i].pos.h)/pixInPhis, matrhig-1)+matrdepth;
if (hig<0) 
	{
		bl[i].type=0;
		return i;
}

bl[i].prev=&bfirst[hig];
bl[i].next=bfirst[hig].next;
bfirst[hig].next=bfirst[hig].next->prev=&bl[i];

/*if (bfirst[hig]!=NULL) 
	bfirst[hig]->prev=&bl[i];
bl[i].prev=NULL;
bl[i].next=bfirst[hig];
bfirst[hig]=&bl[i]; */
i++; // in the end
}
bphcache=i;
//bloodarrayvacancy=i;
return i;
}
int addphisicalrandomold(int tx, int ty, int x, int y, int h, int type, int xspeed, int yspeed, float up ,float hor,int mass, int pr, int power, Uint32* g, int randosity, int numberof, int hp)
{
	//addphisicalrandomold(curphisobj->pos.tx,curphisobj->pos.ty, jhx, jhy, jhh, 807, -jspx, -jspy,0,1, 15, 10,0,g, randy*5, 2,100);
	int i=freephisslot;  
//int prea=randosity/2;
	//return 0;
for (int f=0; f<numberof; f++)
{
while ((ph[i].type>0) && (i<phlim)) i++;
if (i>=phlim) { i=freephisslot=1;
while ((ph[i].type>0) && (i<phlim)) i++;
if (i>=phlim) return 0; } 
ph[i].G_p=0;
ph[i].G_c=0;
ph[i].type=type;
ph[i].mortal=true;
ph[i].BlitOnDeath=true;
ph[i].voxelPalette=0;
ph[i].voxelPaletteCopy=0;
ph[i].i=i;
ph[i].pos.tx=tx;
ph[i].pos.ty=ty;
ph[i].hp=rand() % hp +hp;
ph[i].starthp=ph[i].hp;
ph[i].clock=0;
ph[i].power=power;
ph[i].pos.x=x;
ph[i].pos.y=y;
ph[i].pos.h=h;
ph[i].g=g;
ph[i].lightTrack=createData.lightPisObj;
ph[i].lightIntensity=createData.lightIntensity;
ph[i].force.pr=pr; //100
ph[i].force.mass=mass;
//ph[i].size=g;
ph[i].foo=foophi;
ph[i].collide=foophi;
ph[i].solidness=60;
ph[i].slowing=0.9;
ph[i].g8particle=0;
ph[i].skip=0;
//
ph[i].angles.Ah=0;
ph[i].angles.Ax=0;
ph[i].angles.Ay=0;
ph[i].force.speedx= xspeed+randosity*sphere(500); 
ph[i].force.speedy= yspeed+randosity*radiusy;
ph[i].force.speedh=randosity*radiush+abs(randosity*radiush)*up*hor;
int hig=max(min(accurate(ph[i].pos.h)/pixInPhis, matrhig-1)+matrdepth,0);
if (phfirst[hig]!=NULL) 
	phfirst[hig]->prev=&ph[i];
ph[i].prev=NULL;
ph[i].next=phfirst[hig];
phfirst[hig]=&ph[i]; 
}
freephisslot=i+1;
return i;
}
void addsoftblit(SDL_Surface * destination,SDL_Surface * source, SDL_Surface * alpha, int tx, int ty, int x, int y, int h, int type, int borderw, float border, float speed, bool free, bool add)
{
int i=0;
while ((i<SoftBlitLim) && (SoftBlit[i].type!=0)) i++;
if (i==SoftBlitLim) return; 
SoftBlit[i].source=source;
SoftBlit[i].destination=destination;
SoftBlit[i].alpha=alpha;
SoftBlit[i].border=border;
SoftBlit[i].speed=speed;
SoftBlit[i].borderw=borderw;
SoftBlit[i].tx=tx;
SoftBlit[i].ty=ty;
SoftBlit[i].type=type;
SoftBlit[i].x=x;
SoftBlit[i].y=y;
SoftBlit[i].h=h;
SoftBlit[i].FreeIfDone=free;
SoftBlit[i].AddObjectIfDone=add;
if (i>=SoftBlitCount) SoftBlitCount=i+1;
}

int addDust(worldcoordinates* pos, int x, int y, int randosity, int speedx, int speedy, int speedh, int count, int spawnRand, int burn)
{
int i;
	if (!dustVacThisTurn) return 0;
x+=radiusx(spawnRand)*accuracy;	
y+=radiusy*accuracy;
int spx=speedx + sphere(randosity/2);
int spy=speedy + radiusy;
int sph=speedh + radiush;
double angle=(rand() % 314) * 0.01;//MyRandomRotation(M_PI/5);//getAngle(spx, spy);
double angledir=0.032*(1-(rand() % 2)*2) * ((rand() % 3) + 3);
int cnt=count;
while (count)
{
	while ((dust[dustArVacancy].type) && (dustArVacancy<dustCount)) dustArVacancy++;
	if (dustArVacancy>=dustCount) {
		dustVacThisTurn=dustArVacancy=0; return 0; }

i=dustArVacancy;
dust[i].type=(rand() % 10)/3 + 1;
dust[i].pos.tx=pos->tx;	
dust[i].pos.ty=pos->ty;
dust[i].pos.x=x;//+radiusx(spawnRand)*accuracy;	
dust[i].pos.y=y;//+radiusy*accuracy;
dust[i].pos.h=pos->h;
dust[i].prevOffx=0;
dust[i].prevOffy=0;
dust[i].hp=SmokeHP+(rand() % SmokeHP);
dust[i].force.speedx= spx+sin(angle)*randosity;//*(0.2+count*0.005);
dust[i].force.speedy= spy+cos(angle)*randosity;//*(0.2+count*0.005);
dust[i].force.speedh= sph;
dust[i].force.mass=count;
dust[i].force.pr=0.9;//-count*0.001;
dust[i].newborn=1;
dust[i].burning=burn*count/cnt;
angle+=angledir;
if (!(rand() % 20)) angledir=-angledir;
	dustArMax=max(dustArMax, i);
	count--;
}
return i;
}


void hud (int no)
{
	if (!hudd[no]) return;
SDL_Rect src, dest;
src.x = 0;
src.y = 0;
src.w = hudd[no]->w;
src.h = hudd[no]->h;
dest.x = scrwi - hudx[no] ;
dest.y = scrhi - hudy[no];
dest.w = hudd[no]->w;//+dest.x;
dest.h = hudd[no]->h;//+dest.y;
//sdlblitstyle(dest.x,dest.y,hudd[hudstate[no]],screen);
SDL_BlitSurface(hudd[hudstate[no]], NULL, screen, &dest);
}
int textlinenote(int x, int y, int i, string towrite)
{
	//image[44]->w
std::stringstream text;
text<<towrite;
message = TTF_RenderText_Solid( font, text.str().c_str(), textColor ); 
apply_surface( min(x+image[i]->w+13, scrwi-message->w-10), y-48, message, screen );
apply_surface( x+image[i]->w-8, y-25, image[70], screen );
SDL_FreeSurface( message );
return 0;
}
void makeblood (int k, int jspx, int jspy, int damage)
{
damage=(damage*bloodness)/10;
if (damage<bloodness) damage=bloodness;
// front blood
int i=bloodness*3;
if (damage<i) i=damage;

//int tx, int ty, int x, int y, int h, int type, int xspeed, int yspeed, int hspeed, int randosity, int numberof, int flooidness, float slowing)

createData.tx=curphisobj->pos.tx; createData.ty=curphisobj->pos.ty; createData.x=jhx; createData.y=jhy; createData.h=jhh;
createData.type=20; createData.xspeed=-jspx/bodythick; createData.yspeed=-jspy/bodythick;  createData.hspeed=0;
createData.randosity=randy*11; createData.numberof=i; createData.flooidness=2; createData.slowing=0.98;

addphisicalrandom();//e[k].pos.tx, e[k].pos.ty, e[k].pos.x, e[k].pos.y, e[k].pos.h , 20, -jspx/bodythick, -jspy/bodythick,0, randy*11, i,2, 0.98); 

createData.type=813;  
createData.xspeed=-jspx/(bodythick*8); createData.yspeed=-jspy/(bodythick*8);
createData.randosity=randy*15; createData.flooidness=8; createData.slowing=0.97;

addphisicalrandom();//e[k].pos.tx, e[k].pos.ty, e[k].pos.x, e[k].pos.y, e[k].pos.h , 813, -jspx/bodythick, -jspy/bodythick,0,  randy*15, i,1, 0.97); 

damage-=i;
if (damage<1) return; 
// front projectile blood
//createData.lightPisObj=1;
//addphisicalrandomold(e[k].pos.tx, e[k].pos.ty, e[k].pos.x+10, e[k].pos.y, e[k].pos.h+accuracy*3 , 819, -jspx/bodythick, -jspy/bodythick,0,1, 20, 20,0 , 0,-e[k].hp, max(0,(rand() % 8) -5) ,100);

i=bloodness*1;
if (damage<i) i=damage;

/*createData.type=813; createData.xspeed=-jspx*2/bodythick; createData.yspeed=-jspy*2/bodythick;
createData.randosity=randy*4; createData.numberof=i; createData.flooidness=3;  createData.slowing=0.94;
*/
//addphisicalrandom();//e[k].pos.tx, e[k].pos.ty, e[k].pos.x, e[k].pos.y, e[k].pos.h, 813, -jspx*2/bodythick, -jspy*2/bodythick,0, randy*4, i,3,0.94);

createData.type=813; createData.xspeed=-jspx*3/bodythick; createData.yspeed=-jspy*3/bodythick;
createData.randosity=randy*4; createData.numberof=i; createData.flooidness=3;  createData.slowing=0.93;

addphisicalrandom();//e[k].pos.tx, e[k].pos.ty, e[k].pos.x, e[k].pos.y, e[k].pos.h, 20, -jspx*3/bodythick, -jspy*3/bodythick,0, randy*4, i,3,0.93);

damage-=i*10;
if (damage<1) return;
// Radial blood
 i=bloodness*10;
if (damage<i) i=damage;

createData.type=813; createData.xspeed=(rand() % tyleSize - 500)*tyleSize; createData.yspeed=(rand() % tyleSize - 500)*tyleSize;
createData.randosity=randy*9; createData.numberof=i; createData.flooidness=2;  createData.slowing=0.96;

addphisicalrandom();//e[k].pos.tx, e[k].pos.ty, e[k].pos.x, e[k].pos.y, e[k].pos.h, 813, (rand() % tyleSize - 500)*tyleSize, (rand() % tyleSize - 500)*tyleSize,0, 9*randy, i, 2, 0.96);
}
void makebackblood (int k, int jspx, int jspy, int damage)
{
	
damage=damage/10;
if (damage<bloodness) damage=bloodness;
// Backbone blood
int i=bloodness*7;
if (damage<i) i=damage;

createData.tx=curphisobj->pos.tx; createData.ty=curphisobj->pos.ty; createData.x=jhx; createData.y=jhy; createData.h=jhh;
//createData.tx=e[k].pos.tx; createData.ty=e[k].pos.ty; createData.x=e[k].pos.x; createData.y=e[k].pos.y; createData.h=e[k].pos.h;
createData.type=20; createData.xspeed=(jspx/2); createData.yspeed=(jspy/2);  createData.hspeed=0;
createData.randosity=randy*7; createData.numberof=i; createData.flooidness=2; createData.slowing=0.94;

addphisicalrandom();//e[k].pos.tx, e[k].pos.ty, e[k].pos.x, e[k].pos.y, e[k].pos.h, 20, (jspx/2), (jspy/2),0, 7*randy, i,2,0.94);

createData.type=813; 
createData.flooidness=5; createData.slowing=0.95;

addphisicalrandom();//e[k].pos.tx, e[k].pos.ty, e[k].pos.x, e[k].pos.y, e[k].pos.h, 813, (jspx/2), (jspy/2),0, 7*randy, i,5, 0.95);
}
void linkelements (int ione, int itwo, int dist, int force)
{
	//return;
if ((ione) && (itwo))
{
  LP * ln = new LP;
   ln->ione=ione;
	ln->itwo=itwo;
	ln->dist=dist;
	ln->force=force;
	ln->broken=0;
  ln->next=linkhead;
  linkhead = ln;
}
}




//SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSShadow Stuff
Uint8* shadowPixelMasks[5];
void PreCreateShadowMasks(){
for (int i=0; i<5; i++){
	int thick=i+2;//offh/8
	int diam=thick*2+1;
shadowPixelMasks[i]= new Uint8[(diam)*(diam)];
MakeAlphaPortionMask(shadowPixelMasks[i], thick,255-i*8);
}
}
Uint8* GetShadoMask(int* diam, int* thick, int offh){
	int i=min(4,offh/8);
	*thick=i+2;//offh/8
	*diam=*thick*2+1;
return shadowPixelMasks[i];
}

void clearUpShadLayer (UpShadLayer *source)
{
	return;
	//shadowmapcolor
	Uint8 *pixmem8;
	pixmem8=&source->pix[0];
	int i=tyleSize*tyleSize;
	while (i)
	{
		*pixmem8=0;
		i--;
		pixmem8++;
	}
	/*
	for (int x=0; x<source->w; x++)
	for (int y=0; y<source->h; y++)
		{
			//if (*pixmem32>=shadowmapcolor) *pixmem32=shadowmapcolor;
			*((Uint8*)pixmem32+3)>>=1;
			pixmem32++;
		}*/
}
void transparantizeshadowlayer (UpShadLayer *source)
{
	//shadowmapcolor
	Uint8 *pixmem8;
	pixmem8=&source->pix[0];
	int i=tyleSize*tyleSize;
	while (i)
	{
		if (*pixmem8/shadowDivPoint) *pixmem8-=(*pixmem8-shadowDivPoint)/2;
		*pixmem8=max(*pixmem8,ShadowMin);
		//else *pixmem8=max(ShadowMin,*pixmem8 );
		//*pixmem8/=2;
		pixmem8++;
		i--;
	}
	/*
	for (int x=0; x<source->w; x++)
	for (int y=0; y<source->h; y++)
		{
			//if (*pixmem32>=shadowmapcolor) *pixmem32=shadowmapcolor;
			*((Uint8*)pixmem32+3)>>=1;
			pixmem32++;
		}*/
}
int blittheshadow(SDL_Surface *source, UpShadLayer *layer, int dx, int dy)
{
		int x=0;
	int y=0;
	if (dx<0) x-=dx;
	if (dy<0) y-=dy;
	int destx=source->w;
	int desty=source->h;
	if (dx+destx>tyleSize) destx=(tyleSize-dx);
	if (dy+desty>tyleSize) desty=(tyleSize-dy);
	Uint32 *sourpic;
	Uint8 *destpic;
for (y; y<desty; y++)
{
	sourpic=(Uint32*) source->pixels  + y*source->w + x;
	destpic=&layer->pix[(dy+y)*tyleSize + dx+x];
	for (int xx=x; xx<destx; xx++)
		{
*destpic=max(*sourpic>>24, *destpic);
//if (*sourpic>*destpic)	//*destpic
//	*destpic=*sourpic;
sourpic++;
destpic++;
}
}
		return 1;
}
void ApplyShadow (UpShadLayer* sl, SDL_Surface* layer, int x, int y, int w, int h)
{
	//return;
if (x<0) {w+=x; x=0;}
if (y<0) {h+=y; y=0;}
w=min(w,tyleSize-x);
h=min(h,tyleSize-y);
if ((w<0) || (h<0)) return; 
int off=y*tyleSize+x;
int plus=tyleSize-w;
Uint8* AL=&sl->pix[off];
Uint32* spic=(Uint32*)layer->pixels+off;
int j=h;
int i;
while (j)
{
	i=w;
	while (i)
	{
if (*AL)
{
int AH=255-*AL;
register int idst = *spic;
*spic = (RedMask & (((idst & RedMask) * AH)>>8)) |
(GreenMask & (((idst & GreenMask) * AH)>>8)) |
(BlueMask & (((idst & BlueMask) * AH)>>8)) |
(*spic & AlphaMask);
}
AL++;
spic++;
		i--;
	}
	AL+=plus;
	spic+=plus;
	j--;
}

}
void MegaVoxShadowDrow(Uint8* ShadLayer, G8_Particle * G, double offx, double offy, int offh, float glob,  int angle)
{
if (G->count==0) return;
int thick;
int diam;
Uint8* mask=GetShadoMask(&diam, &thick,  offh);




float fsz = sin(glob);
float fcz = cos(glob);
float fsx = 0;
float fcx = 1;
float fsy = 0;
float fcy = 1;


double qx=1;
double qy=0;
double qh=0;
double hx = fcz*qx - fsz*qy;
double hy = fsz*qx + fcz*qy;	
int fxtoy = fcx*hy - (fsx*qh);
double xh = fsx*hy + (fcx*qh);
int fxtox = fsy*xh + (fcy*hx);

 qx=0;
 qy=1;
 qh=0;
 hx = fcz*qx - fsz*qy;
 hy = fsz*qx + fcz*qy;	
int fytoy = fcx*hy - (fsx*qh);
xh = fsx*hy + (fcx*qh);
int fytox = fsy*xh + (fcy*hx);



Uint32* __restrict gp=G->pixels;


int TrueX, TrueY,TrueH;
int countme=G->count;
if (countme>0)
do
{
	int qh=(char)(*gp>>16);
	Uint32 plane=*gp & 0xFFFF;
	gp++;
countme-=plane;

TrueH=qh+offh;
int xshad=TrueH*timeofdayx;
int yshad=TrueH*timeofdayy;
//timeofdayx=0.5, timeofdayy=0.1;
while (plane)
{
int qx=(char)(*gp & 0xFF);
int qy=(char)((*gp>>8) & 0xFF);
TrueX=qx*fxtox+qy*fytox+xshad+offx-thick;
TrueY=qx*fxtoy+qy*fytoy+yshad+offy-thick;
Uint8 *msk=mask;
Uint8* dst=ShadLayer+tyleSize*TrueY+TrueX;
int skip=tyleSize-diam;
for (int i=0; i<diam; i++){
	for (int j=0; j<diam; j++){
	*dst=max(*dst,*msk);
TrueX++;
msk++;
dst++;
	}
	TrueX-=diam;
TrueY++;	
dst+=skip;
}
gp++;
plane--;
}

}
while (countme);

}
void MegaVoxShadowDrowSafe(Uint8* ShadLayer, G8_Particle * G, double offx, double offy, int offh, float glob,  int angle)
{
	if ((G->count==0) || (G->radius==0)) return;
int thick;
int diam;
Uint8* mask=GetShadoMask(&diam, &thick,  offh);



float fsz = sin(glob);
float fcz = cos(glob);
float fsx = 0;
float fcx = 1;
float fsy = 0;
float fcy = 1;


double qx=1;
double qy=0;
double qh=0;
double hx = fcz*qx - fsz*qy;
double hy = fsz*qx + fcz*qy;	
int fxtoy = fcx*hy - (fsx*qh);
double xh = fsx*hy + (fcx*qh);
int fxtox = fsy*xh + (fcy*hx);

 qx=0;
 qy=1;
 qh=0;
 hx = fcz*qx - fsz*qy;
 hy = fsz*qx + fcz*qy;	
int fytoy = fcx*hy - (fsx*qh);
xh = fsx*hy + (fcx*qh);
int fytox = fsy*xh + (fcy*hx);



Uint32* __restrict gp=G->pixels;


int TrueX, TrueY,TrueH;
int countme=G->count;
if (countme>0)
do
{
	int qh=(char)(*gp>>16);
	Uint32 plane=*gp & 0xFFFF;
	gp++;
countme-=plane;

TrueH=qh+offh;
int xshad=TrueH*timeofdayx;
int yshad=TrueH*timeofdayy;
//timeofdayx=0.5, timeofdayy=0.1;
while (plane)
{
int qx=(char)(*gp & 0xFF);
int qy=(char)((*gp>>8) & 0xFF);
TrueX=qx*fxtox+qy*fytox+xshad+offx-thick;
TrueY=qx*fxtoy+qy*fytoy+yshad+offy-thick;
Uint8 *msk=mask;
Uint8* dst=ShadLayer+tyleSize*TrueY+TrueX;
int skip=tyleSize-diam;
for (int i=0; i<diam; i++){
	for (int j=0; j<diam; j++){

if ((TrueX>=0) && (TrueY>=0) && (TrueX<tyleSize) && (TrueY<tyleSize))
	*dst=max(*dst,*msk);
TrueX++;
msk++;
dst++;
	}
	TrueX-=diam;
TrueY++;	
dst+=skip;
}
gp++;
plane--;
}

}
while (countme);
}

void FindApplicance(SurroundArray* world, MegavoxSegment* segm, int randRot)
{
//ApplicCount=0;
//world->p[13]=0;
//int temp[27]; 
//int piece[27];
bool copy=0;
int error=0;


//int* temp=&segm->landing[0];

//memcpy(&CubeArOne,temp,27*4);

/*for ( int z=0; z<27; z++)
	 if (CubeArOne[z]!=segm->landing[z])
	 z=27;*/



//for ( int z=0; z<27; z++)
//	 CubeArTwo[z]=segm->landing[z];

memcpy(&CubeArOne,&segm->landing[0],27*4);

for ( int z=0; z<27; z++)
	 if (CubeArOne[z]!=segm->landing[z])
	 error++;

if (error)
	error++;

bool NoFit;
int *one, *two, *tempH;

one=&CubeArOne[0];
two=&CubeArTwo[0];
int lmi=0;
//bool mismach=0;
for (int ang=0; ang<4; ang++){


	int Coal=0; 
NoFit=0;
for (int z=0; z<27;z++){
	if (*(one+z)){
	if (*(one+z)==world->p[z]) Coal++;
	else {
	NoFit=1; z=27;
	}
		
	}
}

if (!NoFit){
	if (Coal>maxCoal) { 
		maxCoal=Coal; 
		ApplicCount=0; 
		maxI[0]=ang; 
		ApplicantSegm[0]=segm;
	} 
	else 
	if (Coal==maxCoal) { 
		// int j=0;
		/*while 	(j<=ApplicCount) {
			if (ApplicantSegm[j]==segm){
				copy=1;
				j=100;

			} 
			j++; 
		}*/
		if (!copy) {
			copy=1;
			ApplicCount++; 
			maxI[ApplicCount]=ang; 
			ApplicantSegm[ApplicCount]=segm; 
		} //else 
			//if (ang==randRot) maxI[ApplicCount]=ang; 
	}
}
//}//

for (int h=0; h<3; h++)
for (int z=0; z<9; z++)
	*(two+h*9+z+surRotMask[z])=*(one+h*9+z);



		
tempH=one;
one=two;
two=tempH;
}



}
void UpdateIngameMegavox(IngameMegavox* mgv, worldcoordinates* pos, int wall)
{
if ((!grid.isshadow[tylextogrid(pos->tx)+1][tyleytogrid(pos->ty)+1])  && (!grid.redrowing))
	return;
int piececount=0;
while (mgv->NeedUpdate){



bool solid[8*8*8];
memset(solid,0,sizeof(solid));
int gm;
piececount=0;
int cenx=accurate(pos->x)-4*8;
int ceny=accurate(pos->y)-4*8;

SurroundArray world;
mgv->drowable=1;
mgv->NeedUpdate=0;
int piececounter=0;
	MegavoxSegment** ltype=&mgv->drowType[0];
	int* lhp=&mgv->hp[0];
	char* lpiece=&mgv->piece[0];
	char* langle=&mgv->angle[0];
for (int h=0; h<8; h++)
for (int i=0; i<8; i++)
	for (int j=0; j<8; j++){
//if ((!gm) || (gm==wall))
		if (*lpiece)
		{
if (*lhp>0) { piececount++;

			if (h==0){
			gm=getmematrix (pos->tx, pos->ty, cenx+j*8,  ceny+i*8, -8);
			if (gm<einmatr)  solid[h*8*8+i*8+j]=1;
			} else {
			if (solid[(h-1)*8*8+i*8+j]==1) solid[(h)*8*8+i*8+j]=1;
			}
			int gm=getmematrix (pos->tx, pos->ty, cenx+j*8,  ceny+i*8, h*8);
			if ((gm) && (gm!=wall) && (gm<einmatr)) *lpiece=0; 

}
bool needToDrow=0;
*ltype=0;
//bool decor=mgvDB.decorative[*lpiece]; 
piececounter++;
for (int jj=0; jj<27; jj++ ) world.p[jj]=2;

int * wp=&world.p[0];
bool surrounding=0; // to calculate if voxel is connected to someone;
for (int hh=h-1; hh<=h+1; hh++)
for (int ii=i-1; ii<=i+1; ii++) 
	for (int jj=j-1; jj<=j+1; jj++){
		bool broken=0;
int offset=	hh*8*8 + ii*8 + jj;
		
gm =getmematrix (pos->tx, pos->ty, cenx+jj*8,  ceny+ii*8, hh*8);
//if ((hh<0) && (!mgvDB.decorative[*lpiece])) *wp=2;
//else
if ((gm) && (gm<einmatr)) { *wp=1; if ((gm==1) && (*lhp<1)) broken=1; }
else{
		if ((ii>=0) && (ii<8) && (jj>=0) && (jj<8) && (hh>=0) && (hh<8))
		{

int tarpiece=mgv->piece[offset];
tarpiece*=(!mgvDB.decorative[tarpiece]);
int tarphp=mgv->hp[offset];
if (tarpiece==0)
{*wp=2;}
else if (*lhp>0) 
*wp=1;
else
*wp=(tarphp<1) ? 2 : 1;
broken=tarphp<1;
		}
	if (*wp==2) needToDrow=1;	
}
int same=(hh==h) + (ii==i) + (jj==j);
if ((*wp==1) && (!broken) && (same==2)) surrounding=1; 


wp++;
	}



	if ((!surrounding) && (wall)) { *lpiece=0;  *ltype=0; mgv->NeedUpdate=1;	 }
	else
if (needToDrow)
	{

		int defangel=(h+i+j) % 4;
maxCoal=0;
maxI[0]=defangel;
ApplicCount=0;

if (*lhp>0){
	ApplicantSegm[0]=&mgvDB.mgvarray[*lpiece][0];
	for (int jj=0; jj<mgvpiecestypes; jj++) {
		if (mgvDB.mgvarray[*lpiece][jj].Converted.count) 
			FindApplicance(&world, &mgvDB.mgvarray[*lpiece][jj], defangel);

	}
	int ASNo=GetAsno(h,i,j);
	//((h+i)*((8+((ApplicCount+1) % 2))==0)+j) % (ApplicCount+1);
	*langle=maxI[ASNo];
	*ltype=ApplicantSegm[ASNo];
}else{
	ApplicantSegm[0]=&mgvDB.mgvarray[2][0];
	for (int jj=0; jj<mgvpiecestypes; jj++) 
		if (mgvDB.mgvarray[2][jj].Converted.count) 
		FindApplicance(&world, &mgvDB.mgvarray[2][jj], (h+(h % 2)*((ApplicCount)/2)+j) % 4);
	int ASNo=((h+i)*9+j) % (ApplicCount+1);
		*ltype=ApplicantSegm[ASNo];	
		*langle=maxI[ASNo];
}	
}
else 
	*ltype=0;


if (wall)  
	fillrmatrix (pos->tx, pos->ty, cenx+j*8,  ceny+i*8, h*8, wall*(*lhp>0) * (*lpiece!=0) * (!mgvDB.decorative[*lpiece]));

}
else *ltype=0;
lpiece++; lhp++; ltype++; langle++;
}
	if ((piececount<1) && (wall)) 
		w[wall].RemoveMe=1;
	else {
		// Checking for structural consistency
		bool Defined=1;
		bool NeedReUpdate=0;
		while (Defined){
			Defined=0;
			int* lhp=&mgv->hp[8*8];
			char* lpiece=&mgv->piece[8*8];
			int offset=	8*8;
			for (int h=1; h<8; h++)
				for (int i=0; i<8; i++)
					for (int j=0; j<8; j++){
						
					if ((*lhp>0) && (*lpiece) && (!solid[offset])){
									int gm;
									if ((j<7) && (solid[offset+1]))  {solid[offset]=1; Defined=1; goto done;}
									if ((j) && (solid[offset-1]))  {solid[offset]=1; Defined=1; goto done;}
									if ((i<7) && (solid[offset+8]))  {solid[offset]=1; Defined=1; goto done;}
									if ((i) && (solid[offset-8]))  {solid[offset]=1; Defined=1; goto done;}
									if ((h<7) && (solid[offset+8*8]))  {solid[offset]=1; Defined=1; goto done;}
									if (solid[offset-8*8])  {solid[offset]=1; Defined=1; goto done;}	
								}

							

					done:
							lhp++;
							lpiece++;
							offset++;
					}
				}

			int* lhp=&mgv->hp[8*8];
			char* lpiece=&mgv->piece[8*8];
			int offset=	8*8;
			for (int h=1; h<8; h++)
				for (int i=0; i<8; i++)
					for (int j=0; j<8; j++){
					if ((*lhp>0) && (*lpiece) && (!solid[offset])){
						*lhp=-1;
						
						w[wall].Megavoxel->NeedUpdate=1;	

// Adding Debris
						createData.lightPisObj=0;

int type=rand() % 7;
int no=addphisicalrandomold(w[wall].pos.tx,w[wall].pos.ty, 
	(cenx+j*8)*accuracy, (ceny+i*8)*accuracy, h*8*accuracy, 860, 0, 0,
	1 //up
	, 0 , 2240+type*60, 40-type*3, 0 ,0, randy*(16-type)/5, 1,50+(rand() % 30));			

ph[no].G_p=1;
ph[no].BlitOnDeath=1;
ph[no].data=*lpiece; 
ph[no].g8particle=&mgvDB.mgvarray[11][type].Converted;
ph[no].solidness=0.005; // Starting rotation amount
ph[no].voxelPalette=image[60];
int dlist;
if (w[wall].Debris!=-1) dlist=w[wall].Debris;
else dlist=w[wall].Debris=GetFreeDebris();

if (dlist>=0){
	if (DebrisList[dlist].Max<RelatedParticlesLim){
		DebrisList[dlist].no[DebrisList[dlist].Max]=no;
DebrisList[dlist].Max++;
}
}


//Debris End

								}
							lhp++;
							lpiece++;
							offset++;
							}
			}
	}

	if (wall) w[wall].light.brightness=(float)((piececount % 8)*(piececount<8) + 8*(piececount>=8) + (piececount/8))/8; 


}
void DrowMegavoxShadow(IngameMegavox* mgv, worldcoordinates* pos, int gx, int gy, int tx, int ty)
{
	if (!VoxelShadows) return;
	if (mgv->drowable)
	{

int offx=(pos->tx-tx)*tyleSize+accurate(pos->x);
int offy=(pos->ty-ty)*tyleSize+accurate(pos->y);

if ((offx<-128) || (offx>tyleSize+128) || (offy<-128) || (offy>tyleSize+128)) return;

	MegavoxSegment **drow=mgv->drowType;
	char* maxI=&mgv->angle[0];
for (int h=0; h<8; h++){
int xshad=(h*8+4)*timeofdayx;
int yshad=(h*8+4)*timeofdayy;
for (int i=0; i<8; i++)
	for (int j=0; j<8; j++){
if (*drow){
MegavoxSegment* pdow=*drow;
int tmpx=offx+j*8-4*8+4+xshad;
int tmpy=offy+i*8-4*8+4+yshad;
if ((tmpx>24) && (tmpx<tyleSize-24) && (tmpy>24) && (tmpy<tyleSize-24))
MegaVoxShadowDrow(shadUp[gx+1][gy+1].pix, &pdow->Converted, offx+j*8-4*8+4 , offy+i*8-4*8+4 , (h)*8, M_PI/2*(*maxI),  (*maxI)*16);
else
MegaVoxShadowDrowSafe(shadUp[gx+1][gy+1].pix, &pdow->Converted, offx+j*8-4*8+4 , offy+i*8-4*8+4 , (h)*8, M_PI/2*(*maxI),  (*maxI)*16);
}
drow++; maxI++;
	}

	}
	}
}
void setshadow(int gx, int gy,int tx, int ty)
{
	Uint32 *  __restrict epixel, * __restrict pixel;
	Uint8  *  __restrict prepixel;
	Uint8 *  __restrict espixel;
	int b=0, b1=0, i, j;
	int spower;
	int distance;
	int ttx,tty;
	//grid.isshadow[gx+1][gy+1]=1;
	//shadowmaps[gx+1][gy+1]
	//storedmp[6][6]
	//tylextogrid(int xx)

// ******************************************************* Set Phisical array according to h map
ttx=tylextogrid(tx);
tty=tyleytogrid(ty);

epixel=(Uint32*)storedmp[gx+1][gy+1]->pixels+4+4*storedmp[gx+1][gy+1]->w;
for (j=0; j<phArSize; j++)
{
	
		for (i=0; i<phArSize; i++)
	{
		b1=(255-(*epixel>>24))/80;
		for (b=0; b<b1; b++)
			mapmatrix[ttx][tty][i][j][b+matrdepth]=1;
		epixel+=8;
	}
epixel+=storedmp[gx+1][gy+1]->w*7; //7*4
}


// ************************************************ SHADOWS
	b= (gx==0) ? 5 : gx;
	prepixel=(Uint8 *)storedmp[b][gy+1]->pixels+3;
	espixel=(Uint8 *)shadUp[gx+1][gy+1].pix;
	memset(espixel,0,1024*1024);
	epixel=(Uint32*)storedmp[gx+1][gy+1]->pixels;
	distance=0;
for (i=0; i<tyleSize; i++)
{
spower=255;
if (!grid.isshadow[b][gy+1])
{
	prepixel+=768*4;
	for (j=0;j<256;j++)
	{
	if (spower>=*prepixel) 
	{
		distance=5;
		spower=*prepixel;
	}
	distance++;
	spower+=3;
		prepixel+=4;
	}
} else {
	spower=shadUp[b][gy+1].Rborder[i];
	distance=shadUp[b][gy+1].Distance[i];
	}

j=tyleSize;

int nob;

	while (j)	
	{
		if (spower>=*epixel>>24) 
		{
			distance=5;
			spower=*epixel>>24;
		}
		else
		{ 
			distance=min(distance,32);
			if (((int)(nob=(*epixel>>24)-spower)>=distance))
				*espixel=0xFF;
			else
			*espixel= 255*nob/distance;
		}
		if (*epixel>>24<225)
		*epixel=(*epixel & 0x00FFFFFF) | (min(225,(255-(255-(*epixel>>24))/4))<<24);
		epixel++;
		espixel++;
		spower+=3;
		distance++;
		j--;
	}
	shadUp[gx+1][gy+1].Rborder[i]=min(spower,255);
	shadUp[gx+1][gy+1].Distance[i]=min(distance,255);
}








// caves

b=0;
b1=0;
	

for (b=0; b<cavecache; b++)
	if ((cave[b].type!=0) && (tx-1<=cave[b].tx) && (cave[b].tx<=tx) && (ty-1<=cave[b].ty) && (cave[b].ty<=ty))
	{

		offx=(cave[b].x - (tx-cave[b].tx)*tyleSize);
		offy=(cave[b].y - (ty-cave[b].ty)*tyleSize);

		grid.iscaved[gx+1][gy+1]=1;

		offx/=pixInPhis;
		offy/=pixInPhis;
		
		ttx=tylextogrid(tx);
		tty=tyleytogrid(ty);

		for (j=max(0,offy); j<min(underg[cave[b].type][0]->h/pixInPhis + offy,phArSize); j++ )
		{
			
			pixel=(Uint32*)underg[cave[b].type][1]->pixels + ((j-offy)*pixInPhis+5)*underg[cave[b].type][2]->w + 4 - offx*pixInPhis*(offx<0);
			b1=min(underg[cave[b].type][0]->w/pixInPhis + offx,phArSize);
			for (i=max(0,offx); i<b1; i++ )
			{
			if ((*((Uint8*)pixel+3))>200)
			{
				for (int ff=cave[b].elevation/pixInPhis+matrdepth; ff<matrdepth; ff++)
			mapmatrix[ttx][tty][i][j][ff]=0;
			}
			pixel+=pixInPhis;
			}
		}
	}

grid.isshadow[gx+1][gy+1]=1; 
b=0;
grid.LavaIndex[gx+1][gy+1].indexed=0;
if (grid.iscaved[gx+1][gy+1])
LavaImageIndexation(&grid.LavaIndex[gx+1][gy+1], storedmp[gx+1][gy+1], 247);

	for (b=b; b<=activew; b++)
		if (  (w[b].type) && (w[b].pos.tx<=tx+1) && (w[b].pos.ty<=ty+1) && (w[b].pos.tx>tx-2) && (w[b].pos.ty>ty-2))
		{
	if (shadowimage[w[b].type])		
	blittheshadow(shadowimage[w[b].type], &shadUp[gx+1][gy+1], (w[b].pos.tx-tx)*tyleSize + w[b].pos.x + (timeofdayx*(w[b].pos.h+4)), (w[b].pos.ty-ty)*tyleSize + w[b].pos.y + (timeofdayy*(w[b].pos.h+4))/5);	
	if (w[b].brick){
		if (!imageVoxel[w[b].type]){
for (int z=0;z<=w[b].height;z++)
for (int lj=0; lj<((wall[w[b].type-900]->w+7)/pixInPhis);lj++)
for (int ljj=0; ljj<((wall[w[b].type-900]->h+7)/pixInPhis);ljj++)
	fillrmatrix (w[b].pos.tx, w[b].pos.ty, w[b].pos.x+lj*pixInPhis, w[b].pos.y+ljj*pixInPhis, (z*pixInPhis+w[b].pos.h*pixInPhis), b);
}else{

for (int z=0;z<=w[b].height;z++)
for (int lj=9; lj<12;lj++)
for (int ljj=9; ljj<12;ljj++)
	fillrmatrix (w[b].pos.tx, w[b].pos.ty, w[b].pos.x+lj*pixInPhis, w[b].pos.y+ljj*pixInPhis, (z*pixInPhis+w[b].pos.h*pixInPhis), b);

		}

	}
	if (w[b].Megavoxel)
	{
	if (!WorldLoaded)	UpdateIngameMegavox(w[b].Megavoxel, &w[b].pos, b);
		DrowMegavoxShadow(w[b].Megavoxel, &w[b].pos, gx, gy, tx,  ty);
		w[b].Megavoxel->NeedUpdate=1;
	}
if (displacement[w[b].type-900])
{
	//Matrixizing displacement

for (int ljj=0; ljj<((displacement[w[b].type-900]->h)/pixInPhis);ljj++)
{
pixel=(Uint32*)displacement[w[b].type-900]->pixels + (ljj*pixInPhis+4)*displacement[w[b].type-900]->w + 5;
for (int lj=0; lj<((displacement[w[b].type-900]->w+9)/pixInPhis);lj++)	
{
if ((*((Uint8*)pixel+3))>10)
for (int z=-matrdepth;z<(w[b].pos.h);z++)
fillrmatrix (w[b].pos.tx, w[b].pos.ty, w[b].pos.x+lj*pixInPhis, w[b].pos.y+ljj*pixInPhis, z*pixInPhis, 1);
pixel+=pixInPhis;
}
}


if (w[b].pos.h<0)
{
	int borderh=(displacement[w[b].type-900]->h);
	int borderw=(displacement[w[b].type-900]->w);
	w[b].drowAbove=0;
	if (!getmematrix(w[b].pos.tx, w[b].pos.ty, w[b].pos.x+borderw/2, w[b].pos.y+borderh/2, -pixInPhis))
		w[b].drowAbove=1;
int z=(w[b].pos.h);
for (int ljj=0; ljj<borderh/pixInPhis;ljj++)
{
pixel=(Uint32*)displacement[w[b].type-900]->pixels + (ljj*pixInPhis+5)*displacement[w[b].type-900]->w + 5;
for (int lj=0; lj<borderw/pixInPhis;lj++)	
{
if ((*((Uint8*)pixel+3))>10)
{
fillrmatrix (w[b].pos.tx, w[b].pos.ty, w[b].pos.x+lj*pixInPhis, w[b].pos.y+ljj*pixInPhis, z*pixInPhis, 0);
fillrmatrix (w[b].pos.tx, w[b].pos.ty, w[b].pos.x+lj*pixInPhis, w[b].pos.y+ljj*pixInPhis, z*pixInPhis+pixInPhis, 0);
}
pixel+=pixInPhis;
}
}
}
else
w[b].drowAbove=1;
}
		}
		transparantizeshadowlayer (&shadUp[gx+1][gy+1]);
		grid.isshadow[gx+1][gy+1]=1;
		//return 0;
}
int notremoveshadow (part * phis)
	{
	/*	int x,y,gx,gy;
		//w[matr].brick[k]
		x=accurate(jhx)+(timeofdayx*(w[matr].elevation+4+k*10))/5;
		y=accurate(jhy)+(timeofdayy*(w[matr].elevation+4+k*10))/5;
		if ((x>999) || (x<0) || (y>999) || (y<0)) return 0;
		//shadowmaps[gx+1][gy+1]
		gx=tylextogrid(phis->tx);
		gy=tyleytogrid(phis->ty);
		
 source32 = (Uint32*) shadowmaps[gx+1][gy+1]->pixels  + y*tyleSize + x;
 *source32=0;*/
 return 1;
	}


// ZOOM SHADOW STUFF
void MegaVoxShadowDrowSafeZoom(Uint8* ShadLayer, G8_Particle * G, double offx, double offy, int offh, float glob,  int angle, bool full)
{
if (G->count==0) return;


int TrueH=offh+4;
int xshad=TrueH*timeofdayx;
int yshad=TrueH*timeofdayy;
int fullx=offx+xshad;
int fully=offy+yshad;

if ((fullx+16<0) || (fullx-16>scrwi) || (fully+16<0) || (fully-16>scrhi)) return;

if (!full){
if ((fullx-12<=tera.dx) || (fullx+12>=scrwi+tera.dx) || (fully-12<=tera.dy) || (fully+12>=scrhi+tera.dy)) 
	full=full;
else
	return;
}



float fsz = sin(glob);
float fcz = cos(glob);
float fsx = 0;
float fcx = 1;
float fsy = 0;
float fcy = 1;


double qx=1;
double qy=0;
double qh=0;
double hx = fcz*qx - fsz*qy;
double hy = fsz*qx + fcz*qy;	
int fxtoy = fcx*hy - (fsx*qh);
double xh = fsx*hy + (fcx*qh);
int fxtox = fsy*xh + (fcy*hx);

 qx=0;
 qy=1;
 qh=0;
 hx = fcz*qx - fsz*qy;
 hy = fsz*qx + fcz*qy;	
int fytoy = fcx*hy - (fsx*qh);
xh = fsx*hy + (fcx*qh);
int fytox = fsy*xh + (fcy*hx);



Uint32* __restrict gp=G->pixels;


int TrueX, TrueY;

int countme=G->count;
if (countme>0)
do
{
	int qh=(char)(*gp>>16);
	Uint32 plane=*gp & 0xFFFF;
	gp++;
countme-=plane;

TrueH=qh+offh;
xshad=TrueH*timeofdayx;
yshad=TrueH*timeofdayy;
//timeofdayx=0.5, timeofdayy=0.1;
while (plane)
{
int qx=(char)(*gp & 0xFF);
int qy=(char)((*gp>>8) & 0xFF);
TrueX=qx*fxtox+qy*fytox+xshad+offx;
TrueY=qx*fxtoy+qy*fytoy+yshad+offy;
if ((TrueX>=0) && (TrueX<scrwi) && (TrueY>=0) &&  (TrueY<scrhi))
	*(ShadLayer+scrwi*TrueY+TrueX-ZoomVoxShadR)=128;
/*Uint8* dst=ShadLayer+scrwi*TrueY+TrueX-ZoomVoxShadR;
Uint8* mask=&tera.ShadowRad[0];
int hskip=scrwi-ZoomVoxShadR;
{
	int j=ZVdi;
while (j){
if ((TrueY>=0) &&  (TrueY<scrhi)){
	int i=ZVdi;
	while (i){
			if ((TrueX>=0) && (TrueX<scrwi)){
				*dst=min(128,int(*dst+*mask));
				
			}
			i--;
			TrueX++;
			dst++;
			mask++;
	}
	TrueX-=ZVdi;
} else{
dst+=ZVdi;
mask+=ZVdi;
}

dst+=hskip;
j--;
TrueY++;
}
}*/
/*
for (int i=0; i<diam; i++){
	for (int j=0; j<diam; j++){

if ((TrueX>=0) && (TrueY>=0) && (TrueX<scrwi) && (TrueY<scrhi))
*dst=max(*dst,*msk);
TrueX++;
msk++;
dst++;
	}
	TrueX-=diam;
TrueY++;	
dst+=skip;
}*/
gp++;
plane--;
}

}
while (countme);
}
// ******************** Drow to background
void updateFull (int bgw, int bgh, 
					Uint32* stored, Uint32* upper, Uint32* mayn)
{


Uint32* __restrict up=(Uint32*)upper;
Uint32* __restrict st=(Uint32*)stored;
Uint32* __restrict mn=(Uint32*)mayn;

int i=bgw*bgh;
while (i)
{

register int src=*up;
register int dst=*st;
	int AH=*up>>24;
	*mn=min(255,(dst&BlueMask) + (((((src & BlueMask)*(dst&BlueMask)/128) - (dst&BlueMask)) * AH )>>8))
		| (min(255,((dst>>8)&BlueMask) + ((((((src>>8) & BlueMask)*((dst>>8)&BlueMask)/128) - ((dst>>8)&BlueMask)) * AH )>>8))<<8)
		| (min(255,((dst>>16)&BlueMask) + ((((((src>>16) & BlueMask)*((dst>>16)&BlueMask)/128) - ((dst>>16)&BlueMask)) * AH )>>8))<<16)
		| (AlphaMask & *st);

up++;
st++;
mn++;

		i--;
	}
}

void UpdateSector (int gx, int gy){
updateFull  (1024, 1024, 
				(Uint32*)storedmp[gx][gy]->pixels,(Uint32*) uppermp[gx][gy]->pixels, 
				(Uint32*) mainmaps[gx][gy]->pixels);

}

void redrowupperstuff (int tx, int ty, int gx, int gy)
{
grid.redrowing=1;
//SDL_FreeSurface(shadowmaps[gx+1][gy+1]);	
//shadowmaps[gx+1][gy+1]=SDL_ConvertSurface(image[2], image[2]->format, SDL_SWSURFACE);
//memcpy ( shadowmaps[gx+1][gy+1]->pixels, image[2]->pixels, 1048576*4 );

clearUpShadLayer(&shadUp[gx+1][gy+1]);

memcpy ( uppermp[gx+1][gy+1]->pixels, image[10]->pixels, 1048576*4 );

tyopenfucktion(gx,gy,tx,ty);
setshadow(gx,gy,tx,ty);
/*
memcpy ( mainmaps[gx+1][gy+1]->pixels, storedmp[gx+1][gy+1]->pixels, 1048576*4 );
if (MultiplyUpperblit)
	ApplyUpperMask(mainmaps[gx+1][gy+1], uppermp[gx+1][gy+1]);
else
	apply_surface( 0, 0, uppermp[gx+1][gy+1], mainmaps[gx+1][gy+1] );
*/
UpdateSector (gx+1,gy+1);


if (grid.isshadow[gx+1][gy+1]) ApplyShadow(&shadUp[gx+1][gy+1], mainmaps[gx+1][gy+1], 0, 0, tyleSize, tyleSize);//apply_surface( 0, 0, shadowmaps[gx+1][gy+1], mainmaps[gx+1][gy+1] );
if ((stereo) && (grid.iscaved[gx+1][gy+1])) stereoscopizebg(mainmaps[gx+1][gy+1]);

grid.redrowing=0;
}

void ApplyUpper (SDL_Surface* upper, SDL_Surface* layer, int x, int y, int w, int h)
{
if (x<0) {w+=x; x=0;}
if (y<0) {h+=y; y=0;}
w=min(w,tyleSize-x);
h=min(h,tyleSize-y);
if ((w<0) || (h<0)) return; 
int off=y*tyleSize+x;
int plus=tyleSize-w;
Uint32 * __restrict ul=(Uint32*)upper->pixels+off;
Uint32 * __restrict spic=(Uint32*)layer->pixels+off;
int j=h;
int i;
while (j)
{
	i=w;
	while (i)
	{

register int src=*ul;
	register int dst=*spic;
	int AH=*ul>>24;
if (AH)
	*spic=min(255,(dst&BlueMask) + (((((src & BlueMask)*(dst&BlueMask)/128) - (dst&BlueMask)) * AH )>>8))
		| (min(255,((dst>>8)&BlueMask) + ((((((src>>8) & BlueMask)*((dst>>8)&BlueMask)/128) - ((dst>>8)&BlueMask)) * AH )>>8))<<8)
		| (min(255,((dst>>16)&BlueMask) + ((((((src>>16) & BlueMask)*((dst>>16)&BlueMask)/128) - ((dst>>16)&BlueMask)) * AH )>>8))<<16)
		| (AlphaMask & *spic);

ul++;
spic++;
		i--;
	}
	ul+=plus;
	spic+=plus;
	j--;
}

}
void ApplyUpperMask(SDL_Surface* main, SDL_Surface* upper){
	
int i=main->h*main->w;
Uint32* __restrict mpix=(Uint32*)main->pixels;
Uint32* __restrict uppix=(Uint32*)upper->pixels;
while(i){
	register int src=*uppix;
	register int dst=*mpix;
	int AH=*uppix>>24;
if (AH)
	*mpix=min(255,(dst&BlueMask) + (((((src & BlueMask)*(dst&BlueMask)/128) - (dst&BlueMask)) * AH )>>8))
		| (min(255,((dst>>8)&BlueMask) + ((((((src>>8) & BlueMask)*((dst>>8)&BlueMask)/128) - ((dst>>8)&BlueMask)) * AH )>>8))<<8)
		| (min(255,((dst>>16)&BlueMask) + ((((((src>>16) & BlueMask)*((dst>>16)&BlueMask)/128) - ((dst>>16)&BlueMask)) * AH )>>8))<<16)
		| (AlphaMask & *mpix);



	mpix++;
	uppix++;
	i--;
}




}
void ApplyUpperMasksector(SDL_Surface* main, SDL_Surface* upper, SDL_Rect* rect){
	
//int i=main->h*main->w;
Uint32* __restrict mpix;//=(Uint32*)main->pixels;
Uint32* __restrict uppix;//=(Uint32*)upper->pixels;

int j=0;
while (j<rect->h){
	mpix=(Uint32*)main->pixels+(j+rect->y)*main->w+rect->x;
	uppix=(Uint32*)upper->pixels+(j+rect->y)*main->w+rect->x;
	int i=rect->w;
	while(i>0){
	register int src=*uppix;
	register int dst=*mpix;
	int AH=*uppix>>24;
if (AH)
	*mpix=min(255,(dst&BlueMask) + (((((src & BlueMask)*(dst&BlueMask)/128) - (dst&BlueMask)) * AH )>>8))
		| (min(255,((dst>>8)&BlueMask) + ((((((src>>8) & BlueMask)*((dst>>8)&BlueMask)/128) - ((dst>>8)&BlueMask)) * AH )>>8))<<8)
		| (min(255,((dst>>16)&BlueMask) + ((((((src>>16) & BlueMask)*((dst>>16)&BlueMask)/128) - ((dst>>16)&BlueMask)) * AH )>>8))<<16)
		| (AlphaMask & *mpix);



	mpix++;
	uppix++;
	i--;
	}
	j++;
}


}

void ApplyPixelsMask(){
	
int i=1024*scrhi-1;
Uint32* __restrict mpix=(Uint32*)screen->pixels;
Uint32* __restrict uppix=(Uint32*)imgBank.decay1024.PixelationMask->pixels;
Uint32 prepixel=*mpix;
mpix++;
uppix++;
while(i){
	register int src=*uppix;
	register int dst=*mpix;
	
	*mpix=min(255,(dst&BlueMask) +        (((((src & BlueMask)*     (dst&BlueMask)/128) - (dst&BlueMask)) )))
		| (min(255,((dst>>8)&BlueMask) +  ((((((src>>8) & BlueMask)*((dst>>8)&BlueMask)/128) - ((dst>>8)&BlueMask))  )))<<8)
		| (min(255,((dst>>16)&BlueMask) + ((((((src>>16) & BlueMask)*((dst>>16)&BlueMask)/128) - ((dst>>16)&BlueMask))  )))<<16)
		| (AlphaMask & *mpix);

prepixel=*mpix;

	mpix++;
	uppix++;
	i--;
}




}

void updateMainMapT (int x, int y, int w, int h, int bgw, int bgh, 
					Uint32* stored, Uint32* upper, Uint32* mayn)
{

if (x<0) {w+=x; x=0;}
if (y<0) {h+=y; y=0;}
w=min(w,bgw-x);
h=min(h,bgh-y);
if ((w<0) || (h<0)) return; 
int off=y*bgw+x;
int plus=bgw-w;
Uint32* __restrict up=(Uint32*)upper+off;
Uint32* __restrict st=(Uint32*)stored+off;
Uint32* __restrict mn=(Uint32*)mayn+off;

int j=h;
int i;
while (j)
{
	i=w;
	while (i)
	{

register int src=*up;
register int dst=*st;
	int AH=*up>>24;
	*mn=min(255,(dst&BlueMask) + (((((src & BlueMask)*(dst&BlueMask)/128) - (dst&BlueMask)) * AH )>>8))
		| (min(255,((dst>>8)&BlueMask) + ((((((src>>8) & BlueMask)*((dst>>8)&BlueMask)/128) - ((dst>>8)&BlueMask)) * AH )>>8))<<8)
		| (min(255,((dst>>16)&BlueMask) + ((((((src>>16) & BlueMask)*((dst>>16)&BlueMask)/128) - ((dst>>16)&BlueMask)) * AH )>>8))<<16)
		| (AlphaMask & *st);

up++;
st++;
mn++;

		i--;
	}
	up+=plus;
	st+=plus;
	mn+=plus;

	j--;
}


}

void RefreshGround (int onscreenx, int onscreeny, int w, int h, int gx, int gy)
{
	/*
	SDL_Rect qdest, src;
//qdest.x = onscreenx;
//qdest.y = onscreeny;
//SDL_BlitSurface(image[k], NULL, storedmp[gx][gy], &qdest);
//Now blit to main correctly

src.x=qdest.x = onscreenx;
src.y=qdest.y = onscreeny;
src.w=qdest.w = w;
src.h=qdest.h = h;

SDL_BlitSurface(storedmp[gx][gy], &src, mainmaps[gx][gy], &qdest);

src.x=qdest.x = onscreenx;
src.y=qdest.y = onscreeny;
src.w=qdest.w = w;
src.h=qdest.h = h;

if (MultiplyUpperblit)
ApplyUpper (uppermp[gx][gy], mainmaps[gx][gy], src.x, src.y, src.w, src.h);
else
SDL_BlitSurface(uppermp[gx][gy], &src, mainmaps[gx][gy], &qdest);

if (grid.isshadow[gx][gy]) ApplyShadow (&shadUp[gx][gy], mainmaps[gx][gy], src.x, src.y, src.w, src.h);
*/
updateMainMapT (onscreenx, onscreeny, w, h, 1024, 1024, 
				(Uint32*)storedmp[gx][gy]->pixels,(Uint32*) uppermp[gx][gy]->pixels, 
				(Uint32*) mainmaps[gx][gy]->pixels);

if (grid.isshadow[gx][gy]) ApplyShadow (&shadUp[gx][gy], mainmaps[gx][gy], onscreenx, onscreeny, w, h);
}
void drowtolendpixcheck (int onscreenx, int onscreeny, int k, int gx, int gy)
{
	SDL_Rect qdest, src;
qdest.x = onscreenx;
qdest.y = onscreeny;
SDL_BlitSurface(image[k], NULL, storedmp[gx][gy], &qdest);
//Now blit to main correctly



src.x=qdest.x = onscreenx;
src.y=qdest.y = onscreeny;
src.w=qdest.w = image[k]->w;
src.h=qdest.h = image[k]->h;

SDL_BlitSurface(storedmp[gx][gy], &src, mainmaps[gx][gy], &qdest);

src.x=qdest.x = onscreenx;
src.y=qdest.y = onscreeny;
src.w=qdest.w = image[k]->w;
src.h=qdest.h = image[k]->h;

if (MultiplyUpperblit)
ApplyUpper (uppermp[gx][gy], mainmaps[gx][gy], src.x, src.y, src.w, src.h);
else
SDL_BlitSurface(uppermp[gx][gy], &src, mainmaps[gx][gy], &qdest);

if (grid.isshadow[gx][gy]) 
{
ApplyShadow (&shadUp[gx][gy], mainmaps[gx][gy], src.x, src.y, src.w, src.h);
}
//apply_surface( 0, 0, shadowmaps[i+1][j+1], mainmaps[i+1][j+1] );


}
void updateMaunMapSector (int onscreenx, int onscreeny, int k, int gx, int gy)
{
	SDL_Rect qdest, src;
/*	
qdest.x = onscreenx;
qdest.y = onscreeny;
SDL_BlitSurface(image[k], NULL, storedmp[gx][gy], &qdest);*/
//Now blit to main correctly



src.x=qdest.x = onscreenx;
src.y=qdest.y = onscreeny;
src.w=qdest.w = image[k]->w;
src.h=qdest.h = image[k]->h;

SDL_BlitSurface(storedmp[gx][gy], &src, mainmaps[gx][gy], &qdest);

src.x=qdest.x = onscreenx;
src.y=qdest.y = onscreeny;
src.w=qdest.w = image[k]->w;
src.h=qdest.h = image[k]->h;

if (MultiplyUpperblit)
ApplyUpper (uppermp[gx][gy], mainmaps[gx][gy], src.x, src.y, src.w, src.h);
else
SDL_BlitSurface(uppermp[gx][gy], &src, mainmaps[gx][gy], &qdest);

src.x=qdest.x = onscreenx;
src.y=qdest.y = onscreeny;
src.w=qdest.w = image[k]->w;
src.h=qdest.h = image[k]->h;
//if (grid.isshadow[i+1][j+1]==1) apply_surface( 0, 0, shadowmaps[i+1][j+1], mainmaps[i+1][j+1] );
if (grid.isshadow[gx][gy]) 
ApplyShadow (&shadUp[gx][gy], mainmaps[gx][gy], src.x, src.y, src.w, src.h);

//apply_surface( 0, 0, shadowmaps[i+1][j+1], mainmaps[i+1][j+1] );


}
void updateMainMap (int onscreenx, int onscreeny, int w, int h, int gx, int gy)
{
/*	SDL_Rect qdest, src;

src.x=qdest.x = onscreenx;
src.y=qdest.y = onscreeny;
src.w=qdest.w = w;
src.h=qdest.h = h;

SDL_BlitSurface(storedmp[gx][gy], &src, mainmaps[gx][gy], &qdest);

src.x=qdest.x = onscreenx;
src.y=qdest.y = onscreeny;
src.w=qdest.w = w;
src.h=qdest.h = h;

if (MultiplyUpperblit)
ApplyUpper (uppermp[gx][gy], mainmaps[gx][gy], src.x, src.y, src.w, src.h);
else
SDL_BlitSurface(uppermp[gx][gy], &src, mainmaps[gx][gy], &qdest);

src.x=qdest.x = onscreenx;
src.y=qdest.y = onscreeny;
src.w=qdest.w = w;
src.h=qdest.h = h;

if (grid.isshadow[gx][gy]) 
ApplyShadow (&shadUp[gx][gy], mainmaps[gx][gy], src.x, src.y, src.w, src.h);
*/
updateMainMapT (onscreenx, onscreeny, w, h, 1024, 1024, 
				(Uint32*)storedmp[gx][gy]->pixels,(Uint32*) uppermp[gx][gy]->pixels, 
				(Uint32*) mainmaps[gx][gy]->pixels);

if (grid.isshadow[gx][gy]) 
ApplyShadow (&shadUp[gx][gy], mainmaps[gx][gy], onscreenx, onscreeny, w, h);
}
void drowtolendpixcheck3d (int onscreenx, int onscreeny, int k, int gx, int gy)
{
	SDL_Rect qdest;
qdest.x = onscreenx;
qdest.y = onscreeny+ 1 - onscreeny % 2;
//qdest.y = onscreeny + 1 - (onscreeny % 2);

//sdlblitstyle(onscreenx,onscreeny,image[k],storedmp[gx][gy]);

SDL_BlitSurface(image[k], NULL, storedmp[gx][gy], &qdest);

qdest.x = onscreenx;
qdest.y = onscreeny+ 1 - onscreeny % 2;
//sdlblitstyle(onscreenx,onscreeny,image[k],mainmaps[gx][gy]);

SDL_BlitSurface(image[k], NULL, mainmaps[gx][gy], &qdest);

}
void drowtolend (int onscreenx, int onscreeny, SDL_Surface* source, int gx, int gy)
{
	SDL_Rect qdest;
qdest.x = onscreenx;
qdest.y = onscreeny;
//sdlblitstyle(onscreenx,onscreeny,source,storedmp[gx][gy]);
SDL_BlitSurface(source, NULL, storedmp[gx][gy], &qdest);
qdest.x = onscreenx;
qdest.y = onscreeny;
//sdlblitstyle(onscreenx,onscreeny,source,mainmaps[gx][gy]);
SDL_BlitSurface(source, NULL, mainmaps[gx][gy], &qdest);
}

// ******************************************************************************* Thread safety
int flagoutmail (sync * letter ,bool flagup, bool flagdown)
{
	
	 SDL_SemWait( letter->datalock);
	 int t;
	 if ((flagup==0) && (flagdown==0)) { t=letter->checkmail; SDL_SemPost( letter->datalock ); return t;}
	 if (flagup==1) letter->checkmail=1;
	 if (flagdown==1) letter->checkmail=0;
	 t=letter->checkmail;
	 SDL_SemPost( letter->datalock );
	 
	 return t;
}
int packinfo (pack * packer, int messagetype)
{
	int i=0, d;
int * pbytes;
short * sbytes;
char * cbytes;

while ((i<dataslots) && (flagoutmail (&outdata[i],0, 0))) i++;
if (i==dataslots) return 0;

pbytes=(int *)outdata[i].what;
*pbytes=messagetype; 
pbytes++;

for (d=0; d<packer->countint; d++)
{ *pbytes=packer->ints[d]; pbytes++;}

sbytes=(short *)pbytes;
for (d=0; d<packer->countshort; d++)
{ *sbytes=packer->shorts[d]; sbytes++;}

cbytes=(char *)sbytes;
for (d=0; d<packer->countchar; d++)
{ *cbytes=packer->chars[d]; cbytes++;}

flagoutmail (&outdata[i], 1, 0);

return 1;
}
int unpackinfo (int clientNo)
{
	
	int i=0, d;

while ((i<dataslots) && (flagoutmail (&indata[i],0, 0))) i++;
if (i==dataslots) return 0;

for (d=0; d<messagefat;d++)
	memcpy(indata[i].what, inet.entrymessage[clientNo],messagefat);
indata[i].ClientNo=clientNo;
	//indata[i].what[d]=entrymessage[d];

(flagoutmail (&indata[i], 1, 0));

return 1;
}

// ************************************************* connection Internet part

int StartServerListening(unsigned short port)
{
    WSAData wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) == SOCKET_ERROR)
    {
        return -1;
    }

    int mySocket = socket(AF_INET, SOCK_STREAM, 0);
    if (mySocket == SOCKET_ERROR)
    {
        return -1;
    }

    struct sockaddr_in server;

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(mySocket, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
    {
        closesocket(mySocket);
        return -1;
    }

    if (listen(mySocket, 5) == SOCKET_ERROR)
    {
         closesocket(mySocket);
        return -1;
    }




    return mySocket;
}

void EndServer(int socket)
{
   
    closesocket(socket);

    
    WSACleanup();

  //  cout << "Server: Server Shutting down\n";
}

int Servoutflow(void *data)
{
int i,d;
while (quit==0){
	d=0;
	while ((flagoutmail (&outdata[d] ,0, 0)) && (d<dataslots)){
		for (i=0; i<inet.serv.connections; i++)
			if (inet.serv.clientConected[i])
		{
			int error=send(inet.serv.clientSocket[i],outdata[d].what ,streamfat,0);

			if ((error==SOCKET_ERROR) || (!error)) {
				closesocket(inet.serv.clientSocket[i]); 
				inet.serv.clientConected[i]=0;
				visiter[i].type=0;
				while ((!inet.serv.clientConected[inet.serv.connections-1]) && (inet.serv.connections>0)) inet.serv.connections--;
				
			}
		}
	flagoutmail (&outdata[d] ,0, 1);
	d++;
	}
	SDL_Delay(1);
}
 return 0;
}

int ServInflow(void *data)
{
	int no=*(int*)data;
	while ((!quit) && (inet.serv.clientConected[no]))
	{
memset(	inet.entrymessage[no], 0, messagefat);

int error=recv(inet.serv.clientSocket[no], inet.entrymessage[no], messagefat, 0);



		if (error>0){

			unpackinfo (no);
				//inet.serv.packegesRecieved[no]++;
		} else{
				inet.serv.clientTreadFree[no]=1;
				inet.serv.ThreadNeedsCleanup[no]=1;
				
				return 0;
		}
	}
	inet.serv.clientTreadFree[no]=1;
	inet.serv.ThreadNeedsCleanup[no]=1;
		return 0;
	
}

int serverfunk(void *data)
{
//You lunch this thread if it is not lunched. Accept returns only when connection is created.
	
	inet.serv.listening=true;
 	
		
int acceptConnection = accept(inet.serv.Socket, 0, 0); // Accept The Client

int freeSocket=0;
	while ((freeSocket<maxconnections) && ((inet.serv.clientConected[freeSocket]) || (!inet.serv.clientTreadFree[freeSocket]))) 
		freeSocket++;

	if (freeSocket>=maxconnections) { 
		closesocket(acceptConnection);
		inet.serv.listening=false;
		return 0;
	}

 	inet.serv.clientSocket[freeSocket]=acceptConnection;

	if (!inet.entrymessage[freeSocket]) 
		inet.entrymessage[freeSocket]=new char[128];

	inet.serv.clientConected[freeSocket]=1;
    #define MESSAGE_SIZE 18 // This lets us know how much data we will be sending and recieving
    char outMessage[MESSAGE_SIZE]; //= "I received your message"; // Our message to send

 
    int * pbytes;
	short * sbytes;
	char * cbytes;

	selectedsolar++;

pbytes= (int *)outMessage;

*pbytes=player.host->pos.tx;
pbytes++;
*pbytes=player.host->pos.ty;
pbytes++;
*pbytes=freeSocket;
pbytes++;

sbytes=(short *)pbytes;
*sbytes=accurate(player.host->pos.x);
sbytes++;
*sbytes=accurate(player.host->pos.y);
sbytes++;

cbytes=(char *)sbytes;
*cbytes=selectedsolar;
cbytes++;
*cbytes=selectedplanet;
//cbytes++;


send(inet.serv.clientSocket[freeSocket],outMessage,sizeof(outMessage),0);

  selectedsolar--;
  inet.serv.numberHolder[freeSocket]=freeSocket;
  inet.serv.ThreadIn[freeSocket]=SDL_CreateThread( ServInflow, &inet.serv.numberHolder[freeSocket]);  // DONT FORGET TO FREE THREAD DATA
  inet.serv.clientTreadFree[freeSocket]=0;
  //  nBytes = send(clientSocket, outMessage, sizeof(outMessage), 0); 



	inet.serv.connections=max(freeSocket+1,inet.serv.connections);
	inet.serv.listening=false;
    return 0;
}



// ###################### All Client Stuff:

int StartClient(unsigned short port, const char* serverName)
{
    int error;

    WSAData wsaData;

    if ((error = WSAStartup(MAKEWORD(2, 2), &wsaData)) == SOCKET_ERROR)
    {
      //  cout << "Client: Winsocks being a bitch...\n";
        return -1;
    }
    int mySocket = socket(AF_INET, SOCK_STREAM, 0);

    if (mySocket == SOCKET_ERROR)
    {
       // cout << "Client: ****ing Socket is stuck again...\n";
        return -1;
    }

    struct hostent *host_entry;

    if ((host_entry = gethostbyname(serverName)) == NULL) {
        //cout << "Client: Uhm.. Who I connect to again?\n";
		//return -1;
    }

    struct sockaddr_in server;

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = *(unsigned long*) host_entry->h_addr;

    if (connect(mySocket, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
    {
        //cout << "Client: IT HATES ME!!! It wont let me in :(...\n";
		return -1;
    }

    //cout << "Client: Im up and running pal!\n";

    return mySocket;
}

void EndClient(int clientSocket)
{
    closesocket(clientSocket);

    WSACleanup();


  //  cout << "Client: Done for the day, night kidos.\n";
}

int ClientInflow(void *data)
{
	inet.client.TimeToClose=0;
	while ((!quit) && (!inet.client.TimeToClose))
	{
memset(	inet.entrymessage[0], 0, messagefat);
int error=recv(inet.client.mySocket, inet.entrymessage[0], messagefat, 0);
	if (error!=SOCKET_ERROR)
	{
		unpackinfo (0);
	} else 
		inet.client.TimeToClose=1;
	}
	return 0;
}

int ClientOutflow(void *data)
{
int i,d;
while ((quit==0) && (!inet.client.TimeToClose)){
	d=0;
	while ((flagoutmail (&outdata[d] ,0, 0)) && (d<dataslots) && (!inet.client.TimeToClose)){
		
		int error=send(inet.client.mySocket,outdata[d].what ,streamfat,0);

			if (error==SOCKET_ERROR) {
				//inet.client.TimeToClose=1;
				return 0;
			}
			else
	flagoutmail (&outdata[d] ,0, 1);
	d++;
	}
	SDL_Delay(1);
}
 return 0;
}

int clientfunk(void *data)
{
inet.client.TimeToClose=0;
bool gotit=false;
std::string dataconv;
std::stringstream kk;

const char * c;
int attampts=1;
c = inet.client.serverip.c_str();
inet.client.mySocket = -1;
    while ((inet.client.mySocket == -1) && (attampts>0))
    {
		
		inet.client.mySocket = StartClient(7700, c);
		if (inet.client.mySocket==-1)
		{
			EndClient(inet.client.mySocket);  
			attampts--;
		
		}

    }

	if (inet.client.mySocket==-1) {
		inet.client.TimeToClose=1;
		inet.client.connected=0;
	return 0;
	}
    inet.client.connected=1;
   
    #define MESSAGE_SIZE 18
    char inMessage[MESSAGE_SIZE];

	memset(inMessage,0, MESSAGE_SIZE);

int error=recv(inet.client.mySocket, inMessage, sizeof(inMessage), 0);

if (error<1) {
	EndClient(inet.client.mySocket); 
	inet.client.TimeToClose=1;
	inet.client.connected=0;
	return 0;}

selectedsolar=*((char*)inMessage+16);
selectedplanet=*((char*)inMessage+17);
	
gotit=true;
player.host->pos.tx=*((int*)inMessage);
player.host->pos.ty=*((int*)inMessage+1);
inet.client.MySlotOnServer=*((int*)inMessage+2);

player.host->pos.x=*((short*)inMessage+6)*accuracy;
player.host->pos.y=*((short*)inMessage+7)*accuracy;


selectedsolar--;

   if (!inet.entrymessage[0])
inet.entrymessage[0]=new char [messagefat]; 

inet.client.ThreadIn = SDL_CreateThread( ClientInflow, NULL);
inet.client.ThreadOut = SDL_CreateThread( ClientOutflow, NULL);
inet.client.ClearInOutThreads=1;

    return 1;
}



//***************************************************************** ADDING SPECIAL STUFF
void addTyleAndRedrow(int type, int tx, int ty, int x, int y)
{
	//int type=SoftBlit[i].type;
	if ((type<600) || (type>800))
	{
	tyleaddobj (type, tx, ty, x, y );
	redrowupperstuff (tx, ty, tylextogrid(tx), tyleytogrid(ty));

if ((image[type]!=0) || (mergeimage[type]!=0)) 
{
	if  (((image[type]) && (x+image[type]->w>tyleSize)) ||   ((mergeimage[type]) && (x+mergeimage[type]->w>tyleSize)))
	{
		/*tyleaddobj (type, SoftBlit[i].tx+1, SoftBlit[i].ty, SoftBlit[i].x-tyleSize, SoftBlit[i].y);
		redrowupperstuff (SoftBlit[i].tx+1, SoftBlit[i].ty, tylextogrid(SoftBlit[i].tx+1), tyleytogrid(SoftBlit[i].ty));*/
			tyleaddobj (type, tx+1, ty, x-tyleSize, y );
	redrowupperstuff (tx+1, ty, tylextogrid(tx+1), tyleytogrid(ty));
	}
	
	
	if (((image[type]) && (y+image[type]->h>tyleSize) ) ||   ((mergeimage[type]) && (y+mergeimage[type]->h>tyleSize)))
		{ /*tyleaddobj (type, SoftBlit[i].tx, SoftBlit[i].ty+1, SoftBlit[i].x, SoftBlit[i].y-tyleSize);
	redrowupperstuff (SoftBlit[i].tx, SoftBlit[i].ty+1, tylextogrid(SoftBlit[i].tx), tyleytogrid(SoftBlit[i].ty+1));*/
		tyleaddobj (type, tx, ty+1, x, y-tyleSize );
	redrowupperstuff (tx, ty+1, tylextogrid(tx), tyleytogrid(ty+1));
	}
	
	
	if (((image[type]!=0) && (x+image[type]->w>tyleSize) && (y+image[type]->h>tyleSize)) || ((mergeimage[type]!=0) && (y+mergeimage[type]->h>tyleSize) && (x+mergeimage[type]->w>tyleSize)))
		{/*tyleaddobj (type, SoftBlit[i].tx+1, SoftBlit[i].ty+1, SoftBlit[i].x-tyleSize, SoftBlit[i].y-tyleSize);
	redrowupperstuff (SoftBlit[i].tx+1, SoftBlit[i].ty+1, tylextogrid(SoftBlit[i].tx+1), tyleytogrid(SoftBlit[i].ty+1));*/
	tyleaddobj (type, tx+1, ty+1, x-tyleSize, y-tyleSize );
	redrowupperstuff (tx+1, ty+1, tylextogrid(tx+1), tyleytogrid(ty+1));
	}
}
	}
}

int addobjectdata (int type, int tx, int ty, int x, int y, int h, int data, int hp, int elevation, Uint32 age, bool net)
{
	if (net) createData.randomGenerated=0;
	int i;
	elevation =min(pixInPhis*matrhig, elevation);
	// data == -1 - set default data
//*************************************************************************** add enemy
	if ((type>600) && (type<800))
	{
		i=1;
		while ((e[i].type) && (i<elim)) i++;
		if (i<elim-1)
		{
		e[i].type=type;
	
		e[i].curstate=type;
		e[i].pos.tx=tx;
		e[i].pos.ty=ty;
		//curstate
		e[i].pos.x=x*accuracy;
		e[i].pos.y=y*accuracy;
		e[i].voxGut=0;
eassigner(i);
//        e[i].g8lineup.bfirst=0;
//		e[i].g8lineup.pfirst=0;
//		e[i].g8lineup.PEfirst=0;

		int hig=accurate(e[i].pos.h)/pixInPhis+matrdepth;
		if (efirst[hig]!=NULL) efirst[hig]->prev=&e[i];
		e[i].prev=NULL;
		e[i].next=efirst[hig];
		
		efirst[hig]=&e[i]; 
		e[i].i=i;
		
		if (i>ecache) ecache=i+1;
		return i;
		}
		return 0;
	}

if ((type>800) && (type<900))
	{
placephisical(tx,ty, x, y, elevation , type, tx, ty, x, y, elevation, 0, 1700, 1, 1, 80, 0.96, 300);
	return 1;
	}
// ******************************************************************************** ADD OBJECTS 

	
	
	if (net)
{
	if ((type>900) && (type<1100))
	{
	x=(x/pixInPhis)*pixInPhis;
	y=(y/pixInPhis)*pixInPhis;
	}

if (mergeimage[type]) 
addsoftblit(screen, mergeimage[type], image[74], tx, ty, x, y, 0, type,0, 255, SoftBlitSpeed,0,1);
else
if (image[type])
addsoftblit(screen, image[type], image[74], tx, ty, x, y, 0, type,0, 255, SoftBlitSpeed,0,1);
return 1;
}

int	d=2;
if ((type>900) && (type<1100))
	{
while ((w[d].type!=0) && (d<wlim)) d++;
if (d>wlim-2)
	return 0;
w[d].type=type;
w[d].i=d;
w[d].pos.tx=tx;
w[d].pos.ty=ty;
w[d].pos.x=x;
w[d].pos.y=y;
w[d].size=createData.size;
w[d].brickcopy=0;
w[d].impparticle=wallparticle[type-900];//804;
w[d].solid=wallsolid[type-900];
if (wall[type-900])
w[d].hp=(wall[type-900]->w/pixInPhis)*(wall[type-900]->h/pixInPhis)/2;
else
w[d].hp=10;
w[d].height=wallh[type-900];
w[d].pos.h=elevation/pixInPhis;
w[d].age=age;
w[d].RemoveMe=0;
w[d].randGenerated=createData.randomGenerated;
w[d].Megavoxel=0;
//w[d].drowing=&basicWallDrow;
w[d].rebendVoxelTimer=rand() % 100;
if (data==-1)
{
w[d].age=now;

if (displacement[type-900])
	if (elevation==0) w[d].pos.h=1;
	//else w[d].pos.h=min(-30,w[d].pos.h);

}

w[d].decay=wallfunk;
w[d].drowing=wallfunk;
//for (z=0;z<=w[d].pos.height;z++)
w[d].brick=wall[type-900];
w[d].gutsout=NULL;
if (imageVoxel[type]) 
{
	w[d].angles.Ah=createData.angleh;
	w[d].angles.Ax=createData.anglex;
	w[d].angles.Ay=createData.angley;
	w[d]._shake.offx=0;
	w[d]._shake.offy=0;
	w[d]._shake.speedx=0;
	w[d]._shake.speedy=0;
	w[d].brickcopy=1;
	w[d].brick=SDL_ConvertSurface(wall[type-900],wall[type-900]->format,wall[type-900]->flags);
	w[d].gutsout=SDL_ConvertSurface(mergeimaged[type],mergeimaged[type]->format, mergeimaged[type]->flags);
	voxelVallConvertor(imageVoxel[type], w[d].brick->w/2, w[d].brick->h/2, 0, w[d].brick, createData.anglex,createData.angley,createData.angleh, 1,w[d].gutsout);
}

w[d]._shake.mass=10;
int hig=w[d].pos.h+matrdepth;
if (wfirst[hig]!=NULL) wfirst[hig]->prev=&w[d];
w[d].prev=NULL;
w[d].next=wfirst[hig];
wfirst[hig]=&w[d]; 

if (activew<d) activew=d;


if (!net) return d;
}
		/// Wall load finish




if ((type>300) && (type<600))
	{
  QO * n = new QO;
   n->qotx=tx;
	n->qoty=ty;
	n->qox=x;
	n->qoy=y;
	n->qoh=h;
	n->qotimer=hp;
	n->age=age;
	n->qot = type; //2000+...
	
  n->charge=data;
  n->foo = foonull;
  n->DeleteMe=0;
  n->interacting=0;
  n->scriptPtr=NULL;
  n->scriptText=NULL;
  n->next=qedit.QOhead;
 //qedit.targetQO=
	 qedit.QOhead=n; 
// qedit.WritingQuestDescription=1; // move this to happen after object is selected
if (data==-1)
{
	n->qotimer=0;
	n->age=now;
  n->charge=tyleSize;
}
nq=n;
nq->foo();
nq->OnCreate();

if (!net)  return 1;
	}

if (type>1100)
	{
		d=0;
		while ((cave[d].type!=0) && (d<cavelim)) d++;
		cave[d].type=type-1100;
		cave[d].tx=tx;
	cave[d].ty=ty;
	cave[d].x=x;
	cave[d].y=y;
	cave[d].elevation=elevation;	
	if (d+1>cavecache) cavecache=d+1;
	if (data==-1)
	{
		cave[d].elevation=min(-40,elevation);
	}
	}

if (net)
{
	int gx=tylextogrid(tx);
	int gy=tyleytogrid(ty);
//tyopenfucktion(gx,gy,tx,ty);
redrowupperstuff (tx, ty,gx, gy);
}
return d;
}
int addobject (int a, int b, int type)
{

	// 	int objh[5][5][l];globxh, globyh, glob
	createData.angleh=glob;
	createData.anglex=globxh;
	createData.angley=globyh;
	createData.randomGenerated=0;

	int i=0, gx=gridx, gy=gridy;
	int fgx,fgy;
	int h=0;
	if (type==4) h=1;
	
	
	if (camerax+a>=tyleSize) gx++;
	if (cameray+b>=tyleSize) gy++;
	if (camerax+a>=tyleSize*2) gx++;
	if (cameray+b>=tyleSize*2) gy++;

	if (gx>4) gx=0;
	if (gx<0) gx=4;
	if (gy>4) gy=0;
	if (gy<0) gy=4;
	// Add object to file
	//calculate stuff 
	int tx=cameratylx;
	int ty=cameratyly; 
	int x=camerax+a;
	int y=cameray+b; 

	if (camerax+a>tyleSize) {x-=tyleSize;tx++;}
	if (camerax+a<0) {x+=tyleSize;tx--;	}
	if (cameray+b>tyleSize) {y-=tyleSize;	ty++;}
	if (cameray+b<0) {	y+=tyleSize;ty--;	}

	if (x>tyleSize) {x-=tyleSize;tx++;}
	if (x<0) {x+=tyleSize;tx--;	}
	if (y>tyleSize) {y-=tyleSize;	ty++;}
	if (y<0) {	y+=tyleSize;ty--;	}
	// net part
	if (inet.onlinemode==1) 
	{
		packer.countint=2;
		packer.ints[0]=tx;
		packer.ints[1]=ty;
		packer.countshort=4;
		packer.shorts[0]=type;
		packer.shorts[1]=x;
		packer.shorts[2]=y;
		packer.shorts[3]=edit.ylev;
		packer.countchar=0;
packinfo (&packer,1);
	}

	
	//*************************************************************************** add enemy
	if ((type>600) && (type<800))
	{
		i=1;
		while ((e[i].type) && (i<elim)) i++;
		if (i<elim-1)
		{
		e[i].type=type;
		e[i].curstate=type;
		e[i].pos.tx=tx;
		e[i].pos.ty=ty;
		//curstate
		e[i].pos.x=x*accuracy;
		e[i].pos.y=y*accuracy;
		e[i].voxGut=0;
//		e[i].g8lineup.bfirst=0;
//		e[i].g8lineup.pfirst=0;
//		e[i].g8lineup.PEfirst=0;
		eassigner(i);
		

		int hig=accurate(e[i].pos.h)/pixInPhis+matrdepth;
		if (efirst[hig]!=NULL) efirst[hig]->prev=&e[i];
		e[i].prev=NULL;
		e[i].next=efirst[hig];
		
		efirst[hig]=&e[i]; 
		e[i].i=i;
		
		if (i>ecache) ecache=i+1;
		}
		//addsoftblit(screen, image[type], image[75], tx, ty, x, y, 0, type, 255, -5);
		return 1;
	}

	// ******************************************************************************** ADD OBJECTS 

	// add phisical object
	if ((type>800) && (type<900))
	{
placephisical(tx,ty, x, y, edit.ylev , type, tx, ty, x, y, edit.ylev, 0, 1700, 1, 1, 80, 0.96, 300);
	return 1;
	}

	if (type>900) 
	{
	x=(x/pixInPhis)*pixInPhis;
	y=(y/pixInPhis)*pixInPhis;
	//edit.mlev=0;
	if ((edit.ylev) && (!displacement[type-900])) 
	{	
	addobjectdata (type, tx, ty, x, y, 0, -1, 0, edit.ylev, 0, 0);	
		return 0; 
	}
	}

	if (heightimage[type])
	{
if (mergeimage[type]!=0) 
addsoftblit(screen, mergeimage[type], heightimage[type], tx, ty, x, y, edit.ylev, type,edit.mlev, -255, 5,0,1);
else
if (image[type]!=0)
addsoftblit(screen, image[type], heightimage[type], tx, ty, x, y, edit.ylev, type,edit.mlev, -255, 5,0,1);
	}
	else
	{
if (mergeimage[type]!=0) 
addsoftblit(screen, mergeimage[type], image[77], tx, ty, x, y, edit.ylev, type,edit.mlev, -255, 5,0,1);
else
if (image[type]!=0)
addsoftblit(screen, image[type], image[77], tx, ty, x, y, edit.ylev, type,edit.mlev, -255, 5,0,1);
	//	addTyleAndRedrow(type, tx, ty, x, y);
	//	addobjectdata (type, tx, ty, x, y, 0, -1, 0, edit.ylev, 0, 0);
	}
	// ********************************************************************************  add quest objects


// ********************************* add underground


	return 0; 
}

int mousetoselectedconsth(int mousex, int mousey)
{
int gx=gridx, gy=gridy;
int fgx,fgy, mh=edit.ylev/pixInPhis;

fgx=mousex;
fgy=mousey;
gx=gridx;
gy=gridy;

fgx+=camerax;	
fgy+=cameray;	
int	i=0;
	while (i==0)
	{
	i=1;
	if (fgx>=tyleSize) {gx++; fgx-=tyleSize; i=0; }
	if (fgy>=tyleSize) { gy++; fgy-=tyleSize; i=0;}
	if (fgx<0) { gx--; fgx+=tyleSize; i=0;}
	if (fgy<0) { gy--; fgy+=tyleSize; i=0;}
	}

	if (gx>4) gx-=5;
	if (gx<0) gx+=5;
	if (gy>4) gy-=5;
	if (gy<0) gy+=5;

return mapmatrix[gx][gy][fgx/pixInPhis][fgy/pixInPhis][mh+matrdepth];

}
void mousetoselected(int mox, int mousey)
{
	
//= offx + accurate((offx-scrwid)*fuck); //-image[adrtype]->w/2;
//= offy + accurate((offy-scrhid)*fuck);

//if ((mox>scrwi) || (mox<0)) return 0;
	//selectedinarray   - return value x, y - mouse ccordinates
int gex=gridx, gy=gridy;
	int fgx,fgy, mh=matrhig;

do
{
fgx=mox;
fgy=mousey;
gex=gridx;
gy=gridy;
fuck=mh*pixInPhis*accuracy/dddstength;
fgx-=accurate((fgx-scrwid)*fuck);
fgy-=accurate((fgy-scrhid)*fuck);
fgx+=camerax;	
fgy+=cameray;	
int	i=0;
	while (i==0)
	{

	i=1;
	if (fgx>=tyleSize) {gex++; fgx-=tyleSize; i=0;}
	if (fgy>=tyleSize) { gy++; fgy-=tyleSize; i=0;}
	if (fgx<0) { gex--; fgx+=tyleSize; i=0;}
	if (fgy<0) { gy--; fgy+=tyleSize; i=0;}
	}

	if (gex>4) gex-=5;
	if (gex<0) gex+=5;
	if (gy>4) gy-=5;
	if (gy<0) gy+=5;
	if ((gex<5) && (gex>=0) && (gy<5) && (gy>=0))
	mouse.selectedinarray=mapmatrix[gex][gy][fgx/pixInPhis][fgy/pixInPhis][mh+matrdepth]; 
	mh--;
}
while ((mouse.selectedinarray==0) && (mh>=0));

//return mouse.selectedinarray;

}
void mousetoselectedrad(int inputx, int inputy, int rad)//, int rad)
{
	int bpp;
	//int rad=50;
//= offx + accurate((offx-scrwid)*fuck); //-image[adrtype]->w/2;
//= offy + accurate((offy-scrhid)*fuck);
//if ((mox>scrwi) || (mox<0)) return 0;
	//selectedinarray   - return value x, y - mouse ccordinates
	int x=accurate(player.host->pos.x), y=accurate(player.host->pos.y);
int gex=gridx, gy=gridy;
	int fgx,fgy, mh=matrhig-2;
	int mox;
	int moy;
	int d,dh,z;
	int holder=0;
	float dist=rad+1,tempdist; 
	mouse.selectedinarray=0;
shorth=0;
while (mh>-matrdepth) //((mouse.selectedinarray<einmatr) || (mouse.selectedinarray>phinmatr)))
{

for (mox=inputx-rad; mox<inputx+rad; mox+=pixInPhis)
	for (moy=inputy-rad; moy<inputy+rad; moy+=pixInPhis)
	{
fgx=mox;
fgy=moy;
gex=gridx;
gy=gridy;
fuck=mh*pixInPhis*accuracy/dddstength;
fgx-=accurate((fgx-scrwid)*fuck);
fgy-=accurate((fgy-scrhid)*fuck);
fgx+=camerax;	
fgy+=cameray;	
int	i=0;
	while (i==0)
	{

	i=1;
	if (fgx>=tyleSize) {gex++; fgx-=tyleSize; i=0;}
	if (fgy>=tyleSize) { gy++; fgy-=tyleSize; i=0;}
	if (fgx<0) { gex--; fgx+=tyleSize; i=0;}
	if (fgy<0) { gy--; fgy+=tyleSize; i=0;}
	}

	if (gex>4) gex-=5;
	if (gex<0) gex+=5;
	if (gy>4) gy-=5;
	if (gy<0) gy+=5;
	if ((gex<5) && (gex>=0) && (gy<5) && (gy>=0))
	holder=mapmatrix[gex][gy][fgx/pixInPhis][fgy/pixInPhis][mh+matrdepth]; 
	//if ((holder<einmatr) && (holder)) shorth=max(shorth, mh); // if no enemy aim for the hight
	if ((holder>einmatr) && (holder<phinmatr))
	{
	somebody*ep=&e[holder-einmatr];
	tempdist=(mox-inputx)*(mox-inputx)+(moy-inputy)*(moy-inputy);
	tempdist=sqrt(tempdist);
	if ((tempdist<dist) && (abs(player.host->pos.tx-ep->pos.tx)<2) && (abs(player.host->pos.ty-ep->pos.ty)<2) && (!ep->friendly))
	{
		//**** Check for fow here:
theta=(player.host->pos.tx-ep->pos.tx)*tyleSize+accurate(player.host->pos.x-ep->pos.x); //dx
thetb=(player.host->pos.ty-ep->pos.ty)*tyleSize+accurate(player.host->pos.y-ep->pos.y); //dy
d=accurate(player.host->pos.h);
dh=accurate(player.host->pos.h-ep->pos.h);
fuck=pow(theta,2)+pow(thetb,2);
fuck=sqrt(fuck); 
//if (fuck>rad) continue;
divfuck=fuck/pixInPhis;
for (z=1; z<divfuck; z++)
{
bpp=getmematrix (player.host->pos.tx, player.host->pos.ty, x-(theta*z)/divfuck, y-(thetb*z)/divfuck, d-(dh*z)/divfuck);
	if ((bpp>0) && (bpp<einmatr))
		break;
}

if (z==divfuck) 
{
dist=tempdist;
mouse.selectedinarray=holder; 
}

	}
	}
	}
	mh--;
	}
}
void mouseeffect (int MB)
{
	if (!edit.MegavoxMaker.closed) return;
if (event.button.button == SDL_BUTTON_LEFT)
{
	if ((mouse.pos.x> scrwi - 1200) && (mouse.pos.y > scrhi - 70) && (mouse.pos.x < hudd[11]->w + scrwi - 1200) && (mouse.pos.y < hudd[11]-> h + scrhi - 70)) {edit.editor=900+wallined; HUDuse=1;return;} 
if ((mouse.pos.x> scrwi - 1100) && (mouse.pos.y > scrhi - 70) && (mouse.pos.x < hudd[12]->w + scrwi - 1100) && (mouse.pos.y < hudd[12]-> h + scrhi - 70)) {edit.editor=100+curlay; HUDuse=1;return;}
if ((mouse.pos.x> scrwi - tyleSize) && (mouse.pos.y > scrhi - 70) && (mouse.pos.x < hudd[13]->w + scrwi - tyleSize) && (mouse.pos.y < hudd[13]-> h + scrhi - 70)) {edit.editor=1100 + curcave; HUDuse=1;return;} // Underground (old cracks)
if ((mouse.pos.x> scrwi - 900) && (mouse.pos.y > scrhi - 70) && (mouse.pos.x < hudd[13]->w + scrwi - 900) && (mouse.pos.y < hudd[13]-> h + scrhi - 70)) {edit.editor=601; HUDuse=1;return;}
if ((mouse.pos.x> scrwi - 800) && (mouse.pos.y > scrhi - 70) && (mouse.pos.x < hudd[13]->w + scrwi - 800) && (mouse.pos.y < hudd[13]-> h + scrhi - 70)) {edit.editor=300+qlay; HUDuse=1;return;} 
if ((mouse.pos.x> scrwi - 800) && (mouse.pos.y > scrhi - 70) && (mouse.pos.x < hudd[13]->w + scrwi - 600) && (mouse.pos.y < hudd[13]-> h + scrhi - 70)) {edit.editor=800+curph;HUDuse=1;return;}
if ((mouse.pos.x> scrwi-hudx[21]) && (mouse.pos.y > scrhi-hudy[21]) && (mouse.pos.x < scrwi-hudx[21]+hudd[21]->w) && (mouse.pos.y < scrhi-hudy[21]+hudd[21]->h)) 
{
	HUDuse=1;
if (edit.editor<300) {curlay++; if (curlay>lays) curlay=2; edit.editor=100+curlay;  return;}
if ((edit.editor>300) && (edit.editor<600)) {qlay++; if (qlay>=qolays) qlay=1; edit.editor=300+qlay;  return; }
if ((edit.editor>600) && (edit.editor<800)) {curchar+=2; if (curchar>=(chars-1)*2) curchar=1; edit.editor=600+curchar;  return; }
if ((edit.editor>800) && (edit.editor<900)) {curph+=3;
if (curph>=(drops-1)*3) curph=1; edit.editor=800+curph;  return; }
if ((edit.editor>900) && (edit.editor<1100)) {wallined++; if (wallined>=totalwalls) wallined=1; edit.editor=900+wallined;  return; }
if (edit.editor>1100) {curcave++; if (curcave>=caves) curcave=1; edit.editor=1100+curcave;  return; }
}
if ((mouse.pos.x> scrwi - 70) && (mouse.pos.y > scrhi - 410) && (mouse.pos.x < hudd[19]->w + scrwi - 70) && (mouse.pos.y < hudd[19]-> h + scrhi - 410)) { if (edit.ylev>-matrdepth*pixInPhis) edit.ylev-=pixInPhis;  HUDuse=1;  return;}
if ((mouse.pos.x> scrwi - 70) && (mouse.pos.y > scrhi - 460) && (mouse.pos.x < hudd[18]->w + scrwi - 70) && (mouse.pos.y < hudd[18]-> h + scrhi - 460)) { if (edit.ylev<matrhig*pixInPhis-11) edit.ylev+=pixInPhis;  HUDuse=1; return;}

// find if quest item was selected
SelectQuestObject();
}
else
{
	
if ((mouse.pos.x> scrwi-hudx[21]) && (mouse.pos.y > scrhi-hudy[21]) && (mouse.pos.x < scrwi-hudx[21]+hudd[21]->w) && (mouse.pos.y < scrhi-hudy[21]+hudd[21]->h)) 
{
	HUDuse=1;
if (edit.editor<300) {curlay--; if (curlay<2) curlay=lays; edit.editor=100+curlay;  return;}
if ((edit.editor>300) && (edit.editor<600)) {qlay--; if (qlay<1) qlay=qolays-1; edit.editor=300+qlay;  return; }
if ((edit.editor>600) && (edit.editor<800)) {curchar-=2; if (curchar<1) curchar=(chars-1)*2-1; edit.editor=600+curchar;  return; }
if ((edit.editor>800) && (edit.editor<900)) {curph-=3;
if (curph<1) curph=1+(drops-1)*3; edit.editor=800+curph;  return; }
if ((edit.editor>900) && (edit.editor<1100)) {wallined--; if (wallined<1) wallined=totalwalls-1; edit.editor=900+wallined;  return; }
if (edit.editor>1100) {curcave--; if (curcave<1) curcave=caves-1; edit.editor=1100+curcave;  return; }
}
}
}
void SingleShoot(int MB)
{
	/*if (gun==1) 
	{
Mix_PlayChannel( 1, gunboom[0], 0 );
addphisical(player.host->pos.tx, player.host->pos.ty, player.host->pos.x, player.host->pos.y, player.host->pos.h ,17, cameratylx, cameratyly, camerax+mouse.pos.x, cameray+mouse.pos.y, 0, 25, 30,0,3,20, 0.995,200);
	}*/
}
void NaturePowerClick(int MB)
{
	curphisobj=&ph[addphisical(player.host->pos.tx, player.host->pos.ty, player.host->pos.x, player.host->pos.y, player.host->pos.h ,838, cameratylx, cameratyly, camerax+mouse.pos.x, cameray+mouse.pos.y, 0, 30,30,1,10,40, 0.99,500)];
	curphisobj->foo();
}

//###########################################LIGHTS
void clearLights()
{
/*	for (int i=0; i<lights; i++)
		if (!light[i].light->color) 
		{
			light[i].active=0;
			light[i].light->active=0;
		}*/
	while ((lights>0) && ((!light[lights-1].light) || (!light[lights-1].light->active))){
		lights--;
		light[lights].light=0;
	}
}

void ThreadlightDistNGrad(double angle, int x, int y, int h)
{

float l=(getAngle(x, y)-angle)*lightArSize;
l/=(M_PI*2);
l=l-((int)l/lightArSize)*lightArSize;
if (l<0) l=lightArSize-1+l;
thrL.lightGrad=l;
//lightGrad=getGradus(x,y);
thrL.lightDist=ipifagor (x, y);
thrL.lightDist=ipifagor (thrL.lightDist, h*heightLightOnSmokePreservation);
}

void lightDistNGrad(double angle, int x, int y, int h)
{

float l=(getAngle(x, y)-angle)*lightArSize;
l/=(M_PI*2);
l=l-((int)l/lightArSize)*lightArSize;
if (l<0) l=lightArSize-1+l;
lightGrad=l;
//lightGrad=getGradus(x,y);
lightDist=ipifagor (x, y);
lightDist=ipifagor (lightDist, h*heightLightOnSmokePreservation);
}

//float Lights

void ThreadSetMax(int i,float angleFade)
{
	thrL.lightsR[i]=thrFrgb.R=thrL.lightsR[i]>thrFrgb.R ? thrL.lightsR[i] : thrFrgb.R;
	thrL.lightsG[i]=thrFrgb.G=thrL.lightsG[i]>thrFrgb.G ? thrL.lightsG[i] : thrFrgb.G;
	thrL.lightsB[i]=thrFrgb.B=thrL.lightsB[i]>thrFrgb.B ? thrL.lightsB[i] : thrFrgb.B;

thrFrgb.R*=angleFade;
	thrFrgb.G*=angleFade;
	thrFrgb.B*=angleFade;

}

void SetMax(int i,float angleFade)
{
	lightsR[i]=frgb.R=lightsR[i]>frgb.R ? lightsR[i] : frgb.R;
	lightsG[i]=frgb.G=lightsG[i]>frgb.G ? lightsG[i] : frgb.G;
	lightsB[i]=frgb.B=lightsB[i]>frgb.B ? lightsB[i] : frgb.B;

frgb.R*=angleFade;
	frgb.G*=angleFade;
	frgb.B*=angleFade;

}

void ThreadInsertLightPortion(Uint32 Light, int i, float portion, Uint32 def)
{
float temp=(((Light>>16 & 0xFF)*(def>>16 & 0xFF))>>8)*portion;
	thrL.lightsR[i]=temp>thrL.lightsR[i] ? temp : thrL.lightsR[i];
	temp=(((Light>>8 & 0xFF)*(def>>8 & 0xFF))>>8)*portion;
	thrL.lightsG[i]=temp>thrL.lightsG[i] ? temp : thrL.lightsG[i];
	temp=(((Light & 0xFF)*(def & 0xFF))>>8)*portion;
	thrL.lightsB[i]=temp>thrL.lightsB[i] ? temp : thrL.lightsB[i];

	/*

float temp=(defaultLight>>16 & 0xFF)*portion;
	lightsR[i]=temp>lightsR[i] ? temp : lightsR[i];
	temp=(defaultLight>>8 & 0xFF)*portion;
	lightsG[i]=temp>lightsG[i] ? temp : lightsG[i];
	temp=(defaultLight & 0xFF)*portion;
	lightsB[i]=temp>lightsB[i] ? temp : lightsB[i];


	*/
}
void ThreadInsertLightOnePortion(Uint32 Light, float portion, Uint32 def)
{
	float temp=(((Light>>16 & 0xFF)*(def>>16 & 0xFF))>>8)*portion;
	thrL.OnelightR=temp>thrL.OnelightR ? temp : thrL.OnelightR;
	temp=(((Light>>8 & 0xFF)*(def>>8 & 0xFF))>>8)*portion;
	thrL.OnelightG=temp>thrL.OnelightG ? temp : thrL.OnelightG;
	temp=(((Light & 0xFF)*(def & 0xFF))>>8)*portion;
	thrL.OnelightB=temp>thrL.OnelightB ? temp : thrL.OnelightB;

	//lightArray[i]=defaultLight;//0x00010101;
}

void InsertLightPortion(Uint32 Light, int i, float portion, Uint32 def)
{
float temp=(((Light>>16 & 0xFF)*(def>>16 & 0xFF))>>8)*portion;
	lightsR[i]=temp>lightsR[i] ? temp : lightsR[i];
	temp=(((Light>>8 & 0xFF)*(def>>8 & 0xFF))>>8)*portion;
	lightsG[i]=temp>lightsG[i] ? temp : lightsG[i];
	temp=(((Light & 0xFF)*(def & 0xFF))>>8)*portion;
	lightsB[i]=temp>lightsB[i] ? temp : lightsB[i];

	/*

float temp=(defaultLight>>16 & 0xFF)*portion;
	lightsR[i]=temp>lightsR[i] ? temp : lightsR[i];
	temp=(defaultLight>>8 & 0xFF)*portion;
	lightsG[i]=temp>lightsG[i] ? temp : lightsG[i];
	temp=(defaultLight & 0xFF)*portion;
	lightsB[i]=temp>lightsB[i] ? temp : lightsB[i];


	*/
}
void InsertLightOnePortion(Uint32 Light, float portion, Uint32 def)
{
	float temp=(((Light>>16 & 0xFF)*(def>>16 & 0xFF))>>8)*portion;
	OnelightR=temp>OnelightR ? temp : OnelightR;
	temp=(((Light>>8 & 0xFF)*(def>>8 & 0xFF))>>8)*portion;
	OnelightG=temp>OnelightG ? temp : OnelightG;
	temp=(((Light & 0xFF)*(def & 0xFF))>>8)*portion;
	OnelightB=temp>OnelightB ? temp : OnelightB;

	//lightArray[i]=defaultLight;//0x00010101;
}

void ThreadInsertLight(Uint32 defaultLight, int i)
{

	thrL.lightsR[i]=(defaultLight>>16 & 0xFF);
	thrL.lightsG[i]=(defaultLight>>8 & 0xFF);
	thrL.lightsB[i]=(defaultLight & 0xFF);

	//lightArray[i]=defaultLight;//0x00010101;
}
void InsertLight(Uint32 defaultLight, int i)
{

	lightsR[i]=(defaultLight>>16 & 0xFF);
	lightsG[i]=(defaultLight>>8 & 0xFF);
	lightsB[i]=(defaultLight & 0xFF);

	//lightArray[i]=defaultLight;//0x00010101;
}

void ThreadLightsTo180arrayF(worldcoordinates* pos, double angleFade, double angle, bool upsideDown, Uint32 def, bool limit)
{
thrL.OnelightR=0;
thrL.OnelightG=0;
thrL.OnelightB=0;
float playerAng=GetAngleFromTo (&player.host->pos, pos);
//float fadeportion=255;
//fadeportion*=(1-angleFade);
for (int i=0; i<lightArSize; i++)
	ThreadInsertLight(0,i); 

for (int i=0; i<lights; i++)
	if ((light[i].light) && (light[i].light->active) && (light[i].coords!=pos)){
LightSources* curL=&light[i];
worldcoordinates* source=curL->coords;
int x=(source->x>>16) - (pos->x>>16) - (pos->tx - source->tx)*tyleSize;
int y=(source->y>>16) - (pos->y>>16) - (pos->ty - source->ty)*tyleSize;
int h=(source->h>>16) - (pos->h>>16);
	ThreadlightDistNGrad(angle, x,y,h);
	 // because we use 180 not 360
	double fadeness=-curL->light->distanceFading*thrL.lightDist+1;
	if (curL->light->angleLight) { if (thrL.lightDist<=1) fadeness=0;
		fadeness-=GetAngleDistance(curL->light->angleLight, ConvertVectorTo360(x,y))*curL->light->angleFading;
	} else fadeness-=(GetAngleDistance(playerAng, ConvertVectorTo360(x,y))*0.002);
	if (fadeness>0)	{
	ThreadInsertLightPortion(curL->light->color,thrL.lightGrad, fadeness*curL->light->brightness, def);
	ThreadInsertLightOnePortion(curL->light->color, fadeness*curL->light->brightness, def);
	}


}
//Uint32 biggest=lightArray[0];
thrFrgb.B=0;
thrFrgb.G=0;
thrFrgb.R=0;
for (int i=0; i<lightArSize;){
	ThreadSetMax(i,angleFade);
	/*frgb.R-=fadeportion;
	frgb.G-=fadeportion;
	frgb.B-=fadeportion;*/
	
	i++;
}
ThreadSetMax(0,angleFade);


for (int i=0; i<lightArSize/2;){
	ThreadSetMax(i,angleFade);

	i++;
}

thrFrgb.B=0;
thrFrgb.G=0;
thrFrgb.R=0;

//biggest=lightArray[179];
for (int i=lightArSize-1; i>=0;){
	ThreadSetMax(i,angleFade);

	i--;
}
ThreadSetMax(lightArSize-1,angleFade);

for (int i=lightArSize-1; i>=lightArSize/2;){
	ThreadSetMax(i,angleFade);

	i--;
}
int OLR=thrL.OnelightR*0.4;//0.7;
int OLG=thrL.OnelightG*0.4;//0.65;
int OLB=thrL.OnelightB*0.4;//0.6;

int *lr1=&thrL.lightsR[0],*lg1=&thrL.lightsG[0],*lb1=&thrL.lightsB[0],
	*lr2=&thrL.lightsR[lightArSize],*lg2=&thrL.lightsG[lightArSize],*lb2=&thrL.lightsB[lightArSize],
	*lr3=&thrL.lightsR[lightArSizex2],*lg3=&thrL.lightsG[lightArSizex2],*lb3=&thrL.lightsB[lightArSizex2];

thrL.lightsR[lightArSizex2+64]=OLR;
thrL.lightsG[lightArSizex2+64]=OLG;
thrL.lightsB[lightArSizex2+64]=OLB;

for (int i=0; i<lightArSize;){

*lr2=(*lr1*2+OLR)/3;
*lg2=(*lg1*2+OLG)/3;
*lb2=(*lb1*2+OLB)/3;

*lr3=(*lr1+OLR*2)/3; 
*lg3=(*lg1+OLG*2)/3;
*lb3=(*lb1+OLB*2)/3;

lr1++; lg1++; lb1++;
lr2++; lg2++; lb2++;
lr3++; lg3++; lb3++;

i++;
}

}
void LightsTo180arrayF(worldcoordinates* pos, double angleFade, double angle, bool upsideDown, Uint32 def, bool limit)
{
OnelightR=0;
OnelightG=0;
OnelightB=0;
float playerAng=GetAngleFromTo (&player.host->pos, pos);
//float fadeportion=255;
//fadeportion*=(1-angleFade);
for (int i=0; i<lightArSize; i++)
	InsertLight(0,i); 

for (int i=0; i<lights; i++)
	if ((light[i].light) && (light[i].light->active) && (light[i].coords!=pos)){
LightSources* curL=&light[i];
worldcoordinates* source=curL->coords;
int x=(source->x>>16) - (pos->x>>16) - (pos->tx - source->tx)*tyleSize;
int y=(source->y>>16) - (pos->y>>16) - (pos->ty - source->ty)*tyleSize;
int h=(source->h>>16) - (pos->h>>16);
	lightDistNGrad(angle, x,y,h);
	 // because we use 180 not 360
	double fadeness=-curL->light->distanceFading*lightDist+1;
	if (curL->light->angleLight) { if (lightDist<=1) fadeness=0;
		fadeness-=GetAngleDistance(curL->light->angleLight, ConvertVectorTo360(x,y))*curL->light->angleFading;
	} else fadeness-=(GetAngleDistance(playerAng, ConvertVectorTo360(x,y))*0.002);
	if (fadeness>0)	{
	InsertLightPortion(curL->light->color,lightGrad, fadeness*curL->light->brightness, def);
	InsertLightOnePortion(curL->light->color, fadeness*curL->light->brightness, def);
	}


}
//Uint32 biggest=lightArray[0];
frgb.B=0;
frgb.G=0;
frgb.R=0;
for (int i=0; i<lightArSize;){
	SetMax(i,angleFade);
	/*frgb.R-=fadeportion;
	frgb.G-=fadeportion;
	frgb.B-=fadeportion;*/
	
	i++;
}
SetMax(0,angleFade);


for (int i=0; i<lightArSize/2;){
	SetMax(i,angleFade);

	i++;
}

frgb.B=0;
frgb.G=0;
frgb.R=0;

//biggest=lightArray[179];
for (int i=lightArSize-1; i>=0;){
	SetMax(i,angleFade);

	i--;
}
SetMax(lightArSize-1,angleFade);

for (int i=lightArSize-1; i>=lightArSize/2;){
	SetMax(i,angleFade);

	i--;
}
int OLR=OnelightR*0.4;//0.7;
int OLG=OnelightG*0.4;//0.65;
int OLB=OnelightB*0.4;//0.6;

int *lr1=&lightsR[0],*lg1=&lightsG[0],*lb1=&lightsB[0],
	*lr2=&lightsR[lightArSize],*lg2=&lightsG[lightArSize],*lb2=&lightsB[lightArSize],
	*lr3=&lightsR[lightArSizex2],*lg3=&lightsG[lightArSizex2],*lb3=&lightsB[lightArSizex2];

lightsR[lightArSizex2+64]=OLR;
lightsG[lightArSizex2+64]=OLG;
lightsB[lightArSizex2+64]=OLB;

for (int i=0; i<lightArSize;){

*lr2=(*lr1*2+OLR)/3;
*lg2=(*lg1*2+OLG)/3;
*lb2=(*lb1*2+OLB)/3;

*lr3=(*lr1+OLR*2)/3; 
*lg3=(*lg1+OLG*2)/3;
*lb3=(*lb1+OLB*2)/3;

lr1++; lg1++; lb1++;
lr2++; lg2++; lb2++;
lr3++; lg3++; lb3++;

i++;
}

}
void LightsToSingleValueF(worldcoordinates* pos,  float colorMax )
{
	Uint32 returnLight=0;
colorMax/=128;
OnelightR=0;//(returnLight & RedMask)>>16;
OnelightG=0;//(returnLight & GreenMask)>>8;
OnelightB=0;//(returnLight & BlueMask);
//lightArray[255]=0;
//EmptyLight180Array();
//if (upsideDown)
//angle=angle +M_PI;
for (int i=0; i<lights; i++)
	if ((light[i].light) && (light[i].light->active))
{
	LightSources* curL=&light[i];
	// BROKE IT LOOK AR OTHER AND COPY X,Y,H calculation
	worldcoordinates* source=curL->coords;
int x=(source->x>>16) - (pos->x>>16) - (pos->tx - source->tx)*tyleSize;
int y=(source->y>>16) - (pos->y>>16) - (pos->ty - source->ty)*tyleSize;
int h=(source->h>>16) - (pos->h>>16);
lightDist=ipifagor (x, y);
lightDist=ipifagor (lightDist, h*heightLightOnSmokePreservation);
	double fadeness=-curL->light->distanceFading*lightDist+1;
	if (curL->light->angleLight) 
	{

		fadeness-=GetAngleDistance(curL->light->angleLight, ConvertVectorTo360(x,y))*curL->light->angleFading;

	}
	fadeness=fadeness*colorMax;

	//lightGrad/=2; // because we use 180 not 360
	if (fadeness>0)
	{
	//Uint32 l=lightArray[lightGrad];
	//Uint32 curCol=curL->light->color;
	//lightArray[lightGrad]
		InsertLightOnePortion(curL->light->color, fadeness*curL->light->brightness, deAlphaMask);
	/*returnLight=max(returnLight & 0xFF,(Uint32)((curCol & 0xFF)*fadeness) & 0xFF) | 
	max(returnLight & 0xFF00,(Uint32)((curCol & 0xFF00) *fadeness) & 0xFF00) | 
	max(returnLight & 0xFF0000,(Uint32)((curCol & 0xFF0000)*fadeness) & 0xFF0000);*/
	}


}

}
// ################################## COMPLEX COMBOS

void shootUpperTyles(int tx, int ty, int x, int y, SDL_Surface* src, SDL_Surface* decayBG)//mergeimaged[no]//image[60]
{
x-=src->w/2;
y-=src->h/2;
if (x<0) {x+=tyleSize; tx--;}
if (y<0) {y+=tyleSize; ty--;}

transparentizeUpper ( uppermp[tylextogrid(tx)+1][tyleytogrid(ty)+1], src, x, y);
if (x+src->w>tyleSize)  transparentizeUpper (  uppermp[tylextogrid(tx+1)+1][tyleytogrid(ty)+1], src, x-tyleSize, y);
if (y+src->h>tyleSize)  transparentizeUpper (  uppermp[tylextogrid(tx)+1][tyleytogrid(ty+1)+1], src, x, y-tyleSize);
if ((y+src->h>tyleSize) && (x+src->w>tyleSize)) 
	transparentizeUpper ( uppermp[tylextogrid(tx+1)+1][tyleytogrid(ty+1)+1], src, x-tyleSize, y-tyleSize);

}

void decayTyles(int tx, int ty, int x, int y, SDL_Surface* src, SDL_Surface* decayBG)//mergeimaged[no]//image[60]
{
x-=src->w/2;
y-=src->h/2;
if (x<0) {x+=tyleSize; tx--;}
if (y<0) {y+=tyleSize; ty--;}

drowDecayedBG (decayBG, storedmp[tylextogrid(tx)+1][tyleytogrid(ty)+1], src, x, y);
if (x+src->w>tyleSize)  drowDecayedBG (decayBG, storedmp[tylextogrid(tx+1)+1][tyleytogrid(ty)+1], src, x-tyleSize, y);
if (y+src->h>tyleSize)  drowDecayedBG (decayBG, storedmp[tylextogrid(tx)+1][tyleytogrid(ty+1)+1], src, x, y-tyleSize);
if ((y+src->h>tyleSize) && (x+src->w>tyleSize)) 
	drowDecayedBG (decayBG, storedmp[tylextogrid(tx+1)+1][tyleytogrid(ty+1)+1], src, x-tyleSize, y-tyleSize);

}

void ToGroundNoRedrowHeight (int tx, int ty, int x, int y, int h, int no)
{

decayTyles(tx, ty, x, y,mergeimaged[no],image[60]);



x-=image[no]->w/2;
y-=image[no]->h/2;
if (x<0) {x+=tyleSize; tx--;}
if (y<0) {y+=tyleSize; ty--;}
int tempx,tempy;
//tx=tylextogrid(tx)+1;
//ty=tyleytogrid(ty)+1;
h=h*2-(rand() % 20);


BlitWithDHInprint(storedmp[tylextogrid(tx)+1][tyleytogrid(ty)+1], image[no], uppermp[tylextogrid(tx)+1][tyleytogrid(ty)+1], overlayimage[no], dhimage[no], x, y,h);
RefreshGround (x, y, image[no]->w, image[no]->h, tylextogrid(tx)+1, tyleytogrid(ty)+1);


if (x+image[no]->w>tyleSize)
{
tempx=x-tyleSize;
tempy=y;
//BlitNormalAndAlphaInprint(storedmp[tylextogrid(tx+1)+1][tyleytogrid(ty)+1], mergeimage[no], uppermp[tylextogrid(tx+1)+1][tyleytogrid(ty)+1], overlayimage[no], heightimage[no], tempx, tempy,h);
BlitWithDHInprint(storedmp[tylextogrid(tx+1)+1][tyleytogrid(ty)+1], image[no],uppermp[tylextogrid(tx+1)+1][tyleytogrid(ty)+1], overlayimage[no], dhimage[no], tempx, tempy,h);
RefreshGround (tempx, tempy, image[no]->w, image[no]->h, tylextogrid(tx+1)+1, tyleytogrid(ty)+1);

}

if (y+image[no]->h>tyleSize)
{
tempx=x;
tempy=y-tyleSize;
//BlitNormalAndAlphaInprint(storedmp[tylextogrid(tx)+1][tyleytogrid(ty+1)+1], mergeimage[no], uppermp[tylextogrid(tx)+1][tyleytogrid(ty+1)+1], overlayimage[no], heightimage[no], x, y-tyleSize,h);
BlitWithDHInprint(storedmp[tylextogrid(tx)+1][tyleytogrid(ty+1)+1], image[no], uppermp[tylextogrid(tx)+1][tyleytogrid(ty+1)+1], overlayimage[no], dhimage[no], x, y-tyleSize,h);
RefreshGround (tempx, tempy, image[no]->w, image[no]->h, tylextogrid(tx)+1, tyleytogrid(ty+1)+1);

}

if ((y+image[no]->h>tyleSize) && (x+image[no]->w>tyleSize))
{
tempx=x-tyleSize;
tempy=y-tyleSize;
//BlitNormalAndAlphaInprint(storedmp[tylextogrid(tx+1)+1][tyleytogrid(ty+1)+1], mergeimage[no], uppermp[tylextogrid(tx+1)+1][tyleytogrid(ty+1)+1], overlayimage[no], heightimage[no], x-tyleSize, y-tyleSize,h);
BlitWithDHInprint(storedmp[tylextogrid(tx+1)+1][tyleytogrid(ty+1)+1], image[no], uppermp[tylextogrid(tx+1)+1][tyleytogrid(ty+1)+1], overlayimage[no], dhimage[no], x-tyleSize, y-tyleSize,h);
RefreshGround (tempx, tempy, image[no]->w, image[no]->h, tylextogrid(tx+1)+1, tyleytogrid(ty+1)+1);

}
//if (mergeimage[no]->w>tyleSize)

}

void offsToAccuratedCreateDatas(int a, int b)
{
	
	createData.x=a+camerax;
	int shift=createData.x/tyleSize;
	createData.x-=shift*tyleSize;
	createData.tx=cameratylx+shift;
	
	createData.y=b+cameray;
	shift=createData.y/tyleSize;
	createData.y-=shift*tyleSize;
	createData.ty=cameratyly+shift;

	/*if (createData.x>tyleSize) { createData.tx++; createData.x-=tyleSize; if (createData.x>tyleSize) { createData.tx++; createData.x-=tyleSize;} }
	if (createData.x<0) { createData.tx--; createData.x+=tyleSize; if (createData.x<0) { createData.tx--; createData.x+=tyleSize;}}
	
	
	if (createData.y>tyleSize) { createData.ty++; createData.y-=tyleSize;	if (createData.y>tyleSize) { createData.ty++; createData.y-=tyleSize;}}
	if (createData.y<0) { createData.ty--; createData.y+=tyleSize; 	if (createData.y<0) { createData.ty--; createData.y+=tyleSize;}}
	*/
	
}

void voxNothing(int offx, int offy)
{
}
void drowBloodByOffxOffy(int offx, int offy)
{
	int type=4+rand() % 3;
offsToAccuratedCreateDatas(offx, offy);
int ttx=tylextogrid(createData.tx)+1;
int tty=tyleytogrid(createData.ty)+1;
shortx=createData.x-image[type]->w/2;
shorty=createData.y-image[type]->h/2;
tolendstyle (shortx, shorty, type, ttx, tty);
if (shortx>tyleSize) 
tolendstyle (shortx-tyleSize, shorty, type, tylextogrid(createData.tx+1)+1, tty);
if (shorty>tyleSize) 
tolendstyle (shortx, shorty-tyleSize, type, ttx, tyleytogrid(createData.ty+1)+1);
if (shortx<0) 
tolendstyle (shortx+tyleSize, shorty, type, tylextogrid(createData.tx-1)+1, tty);
if (shorty<0) 
tolendstyle (shortx, shorty+tyleSize, type, ttx, tyleytogrid(createData.ty-1)+1);
}



void CalculateCameraOffset()
{
shake.cameraDisplacementPerFrame.x=(cameratylx-shake.cameraPrevious.tx)*tyleSize+camerax-shake.cameraPrevious.x;
shake.cameraDisplacementPerFrame.y=(cameratyly-shake.cameraPrevious.ty)*tyleSize+cameray-shake.cameraPrevious.y;

shake.cameraPrevious.tx=cameratylx;
shake.cameraPrevious.x=camerax;
shake.cameraPrevious.ty=cameratyly;
shake.cameraPrevious.y=cameray;

}

void Displacement (SDL_Surface * displacement, int tx, int ty, int x, int y, int h )
{
bool outx=(displacement->w>tyleSize-x);
bool outy=(displacement->h>tyleSize-y);
int offx=x - actcamx - (cameratylx - tx)*tyleSize;
int offy=y - actcamy - (cameratyly - ty)*tyleSize;

if (player.host->pos.h>0)
{
fuck=h*pixInPhis;
fuck/=dddstength;
dshortx=(offx-scrwid)*fuck;
dshorty=(offy-scrhid)*fuck;
offx+=dshortx;
offy+=dshorty;
}

if (outx && outy)
	DisplacementStyle (storedmp[tylextogrid(tx+1)+1][tyleytogrid(ty+1)+1], x-tyleSize, y-tyleSize, h*pixInPhis, displacement, offx, offy);
	DisplacementStyle (storedmp[tylextogrid(tx)+1][tyleytogrid(ty)+1], x, y, h*pixInPhis, displacement, offx, offy);
if (outx)
	DisplacementStyle (storedmp[tylextogrid(tx+1)+1][tyleytogrid(ty)+1], x-tyleSize, y, h*pixInPhis, displacement, offx, offy);
if (outy)
	DisplacementStyle (storedmp[tylextogrid(tx)+1][tyleytogrid(ty+1)+1], x, y-tyleSize, h*pixInPhis, displacement, offx, offy);

}

bool FireDisplacement (DirectionalLightetImage* mask, SDL_Surface* FireGlob, int maskX, int maskY, SDL_Surface* screen, int offx, int offy, int fade )
{
offx-=mask->w/2;
offy-=mask->h/2;
if ((maskX<0) || (maskX+mask->w>FireGlob->w) || (maskY<0) || (maskY+mask->h>FireGlob->h)) return 1;
if  ((offx+mask->w<0) || (offx>screen->w) ||	(offy+mask->h<0) || (offy>screen->h)) return 0;		

	SDL_Rect maskToScreen; 
	maskToScreen.x=0;
	maskToScreen.y=0;
	maskToScreen.h=mask->h;
	maskToScreen.w=mask->w;
	fitRectangleToSurfaceCoords (&maskToScreen, screen, offx, offy);
	offx=max(offx,0);
	offy=max(offy,0);
	

	Uint32* __restrict scrPix=(Uint32*)screen->pixels + offy*screen->w + offx; 
	Uint8* __restrict maskPix=(Uint8*)mask->DnA + maskToScreen.y*mask->w + maskToScreen.x;
	Uint32* __restrict firePix=(Uint32*)FireGlob->pixels+(maskY+maskToScreen.y)*FireGlob->w + maskX+maskToScreen.x;
	Uint32* __restrict dark=(Uint32*)darkmask+ offy*screen->w + offx; 

	fillDarkMaskGrid(offx, offy, mask->w, mask->h);
//*mask=(*mask &  0x0000FFFF) | part | dispTime;


	int line=maskToScreen.w-maskToScreen.x;
	int scrPlus=screen->w-line;
	int maskPlus=mask->w-line;
	int firePlus=FireGlob->w-line;
	int j=maskToScreen.h-maskToScreen.y;
	//int up, down;
	int diff, AH;
	while (j)
{
int i=line;
diff=(int)*maskPix - (*firePix>>24)-fade;
while (i)
{
while ((diff < 8) && i) //16
{
scrPix++;
maskPix++;
firePix++;
dark++;
i--;
diff=(int)*maskPix - (*firePix>>24)-fade;
}
//*scrPix=*firePix; 
//down=*firePix>>24;
//up=*maskPix;
//up;
//diff-=down;
while ((diff>=64) && i)
{
 *scrPix=(*firePix & 0x00FFFFFF) | (*scrPix & 0xFF000000);
 //htpBlit(scrPix,*firePix);
*dark+=(diff<<18);//((*firePix<<16) & 0xFF0000);//*8;//(*dark &  0x0000FFFF) | (max(*firePix<<24,*dark )& 0xFF000000) | 0x30000;//((*dark &  0xFF0000) + 2);
	scrPix++;
	maskPix++;
	firePix++;
	dark++;
i--;

diff=(int)*maskPix - (*firePix>>24)-fade;
}

while ((!(diff>>6)) && i && (diff>>3))//>>4))
{
register int idst = *scrPix;
register int isrc = *firePix;
AH=(((diff*diff)>>4)-1);

*scrPix = (RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * AH) >>8))) | 
(GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * AH) >>8))) |
(BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * AH) >>8)))
| (*scrPix & 0xFF000000); 
*dark+=(diff<<18);
//*dark+=((((*firePix & 0xFF)*AH)/128)<<16);//(AH<<16);//AH<<16;//((*firePix<<19) & 0xFF0000);

scrPix++;
	maskPix++;
	firePix++;
	dark++;
i--;

diff=(int)*maskPix - (*firePix>>24)-fade;
}
// AlphaMask & ((*scrPix & AlphaMask) - ((RedMask - (*firePix & RedMask)))  


//(AlphaMask & ((*scrPix & AlphaMask) + ((int)(((int)((*firePix & AlphaMask) - (int)(*scrPix & AlphaMask)) * AH) >>8))));
//((*scrPix/2) & 0xFF000000) );
	

	
}

scrPix+=scrPlus;
maskPix+=maskPlus;
firePix+=firePlus;
dark+=scrPlus;
j--;
}

return 0;
}

bool RealAnimatedVolume (worldcoordinates* pos, int dx, int dy, SDL_Surface* screen, DirectionalLightetImage* mask, Uint8* angles , int progress, float fade, int dax, int day, Uint32 defLight, float alpha)
{
int countx=7, county=7, size=128;
if ((dx>scrwi) || (dy>scrhi) || (dx+mask->w<0) || (dy+mask->h<0)) return 0;	
	smokeRendered=1;
	progress=progress % (countx*county);
	int srcx=(progress % countx) * size; 
	int srcy=(progress / countx) * size;
	int srcskip=(countx-1)*size;

	alpha=min(alpha,1);
	int deductLight=max(160-accurate(pos->h)*6,64);
	 LightsTo180arrayF(pos, 0.95, 0, 0,  defLight,0); 
	 int MaxMultiply;
MaxMultiply=255*(254.9/(float)max(OnelightR,max(OnelightG,OnelightB)));

	Uint8 * __restrict sourpic;
	Uint32* __restrict dstpix, * __restrict destpic , * __restrict dark;
	Uint8* __restrict angl;
int x=0;
int y=0;
	if (dx<0) x-=dx;
	if (dy<0) y-=dy;
	int destx=mask->w;
	int desty=mask->h;
	if (dx+destx>screen->w) destx=(screen->w-dx);
	if (dy+desty>screen->h) desty=(screen->h-dy);
	
	int AH=255;
sourpic=(Uint8*) mask->DnA  + (y+srcy)*mask->w*countx +x+ srcx;
angl=angles + (y+E8bitBufferDiam/2-mask->h/2+day)*E8bitBufferDiam + x+E8bitBufferDiam/2-mask->w/2+dax;
destpic=(Uint32*) screen->pixels  + (dy+y)*screen->w + dx+x;
dstpix=dustScreen + (dy+y)*screen->w + dx+x;
dark=darkmask+ (dy+y)*screen->w + dx+x;
int spNL=mask->w+x-destx+srcskip;
int dpNL=screen->w-destx+x;
int agNL=E8bitBufferDiam-destx+x;
int diff, cA;
register int idst;
register int isrc;
int prang=0;
int skipper=0;
// execution
for (y; y<desty-1;){
int xes=destx-x;

	AH=*sourpic;
while (xes){

	while ((AH<=0xF) && (xes)){
	skipper++;
	sourpic++;
	xes--;
	AH=*sourpic;
	}
dark+=skipper;
destpic+=skipper;
angl+=skipper;
dstpix+=skipper;
skipper=0;



	while ((AH*=alpha) && (xes)){
		
		//AH+=(((*dstpix & 0xFFFF)*AH)>>9);
		//AH=min(255,AH);
		 //MaxMultiply
		
		float fcA=AH;
		fcA/=256; 
		float mp=1+fcA*(0.3-fcA); 
		*dark=(int)((*dark & 0xFFFF)*(1-fcA)) | ((int)(*dark*mp) & 0xFFFF0000);
		int mltpl=min(MaxMultiply,((*dark>>16)+deductLight));
		idst = *destpic;	
		*destpic = ((RedMask & ((idst & RedMask) + (int)((int)(((int)((((int)(lightsR[*angl]*mltpl)<<8))) - (int)(idst & RedMask))) * fcA))) |
		(GreenMask & ((idst & GreenMask) +((int)(((((int)(((int)(lightsG[*angl]*mltpl)))) - (int)(idst & GreenMask)) * AH)>>8)))) |
		(BlueMask & ((idst & BlueMask) + ((int)((((int)(lightsB[*angl]*mltpl)>>8) - (int)(idst & BlueMask)) * AH)>>8)))) |
		(*destpic & 0xFF000000);
		sourpic++; destpic++; dark++; angl++; xes--; dstpix++;

		AH=*sourpic;
	}


}
sourpic+=spNL;
angl+=agNL;
destpic+=dpNL;
dark+=dpNL;
dstpix+=dpNL;
y++;
}
return 1;
}

bool PseudoAnimatedDust (worldcoordinates* pos, int dx, int dy, SDL_Surface* screen, SDL_Surface* mask, Uint8* angles , int progress, float fade, int dax, int day, Uint32 defLight)
{
if ((dx>scrwi) || (dy>scrhi) || (dx+mask->w<0) || (dy+mask->h<0)) return 0;	
	smokeRendered=1;
	progress= progress % (255*5);
	int frame=8*((progress>=510) + (progress>=765) + (progress>=1020));
	int frameA1, frameA2=0;
	int deductLight=max(160-accurate(pos->h)*6,64);
	//int deductLight=128;
	if (progress<255) frameA1=progress;
	else {frameA2=progress % 255; frameA1=255-frameA2;}
	if (frame>16) frameA2=0;
	//One frame could fade in, second border fade in
//	int slofade=frameA1;
		/*=frameA1+(255-frameA1)*frameA1/255;
	 slofade=slofade+(255-slofade)*slofade/255;*/
	//int border=frameA1;
	//defLight=0x00777777;
	 LightsTo180arrayF(pos, min(0.99,0.91+(0.7/(255*5))*progress), 0, 0,  defLight,0); //defLight, //min(0.99,0.82)+(0.7/(255*5))*progress

	 int MaxMultiply;
MaxMultiply=255*(254.9/(float)max(OnelightR,max(OnelightG,OnelightB)));

	Uint32 * __restrict sourpic, * __restrict destpic , * __restrict dark;
	Uint32* __restrict dstpix;
	Uint8* __restrict angl;
int x=0;
int y=0;
	if (dx<0) x-=dx;
	if (dy<0) y-=dy;
	int destx=mask->w;
	int desty=mask->h;
	if (dx+destx>screen->w) destx=(screen->w-dx);
	if (dy+desty>screen->h) desty=(screen->h-dy);
	
	int AH=255;
sourpic=(Uint32*) mask->pixels  + y*mask->w + x;
angl=angles + (y+E8bitBufferDiam/2-mask->h/2+day)*E8bitBufferDiam + x+E8bitBufferDiam/2-mask->w/2+dax;
destpic=(Uint32*) screen->pixels  + (dy+y)*screen->w + dx+x;
dstpix=dustScreen + (dy+y)*screen->w + dx+x;
dark=darkmask+ (dy+y)*screen->w + dx+x;
int spNL=mask->w+x-destx;
int dpNL=screen->w-destx+x;
int agNL=E8bitBufferDiam-destx+x;
int diff, cA;
register int idst;
register int isrc;
int prang=0;
int skipper=0;
// execution
for (y; y<desty-1;){
int xes=destx-x;

	AH=(*dstpix & 0xFFFF);
		//(((((*sourpic>>(frame+8)) & 0xFF)*frameA2+((((*sourpic>>frame) & 0xFF)*frameA1))))*(*dstpix & 0xFFFF))>>16; 
//AH=((((*sourpic>>(frame+8)) & 0xFF)*frameA2+((((*sourpic>>frame) & 0xFF)*slofade)))*(*dstpix & 0xFFFF))>>16;
//AH=((((*sourpic>>(frame+8)) & 0xFF)*frameA2+((((*sourpic>>frame) & 0xFF)*slofade))))>>8;
while (xes){

	while ((AH<=0xF) && (xes)){
	skipper++;
	
	xes--;
	dstpix++;
	//AH=((((*sourpic>>(frame+8)) & 0xFF)*frameA2+((((*sourpic>>frame) & 0xFF)*slofade))))>>8;
	//AH=((((*sourpic>>(frame+8)) & 0xFF)*frameA2+((((*sourpic>>frame) & 0xFF)*slofade)))*(*dstpix & 0xFFFF))>>16;
	AH=*dstpix & 0xFFFF;
		//(((((*sourpic>>(frame+8)) & 0xFF)*frameA2+((((*sourpic>>frame) & 0xFF)*frameA1))))*(*dstpix & 0xFFFF))>>16; 
	}
dark+=skipper;
destpic+=skipper;
angl+=skipper;
sourpic+=skipper;
skipper=0;



	while ((AH>0xF) && (xes)){
	AH=(((((*sourpic>>(frame+8)) & 0xFF)*frameA2+((((*sourpic>>frame) & 0xFF)*frameA1))))*AH)>>16;	
	if (AH>0xF){
		AH=min(255,AH);
		float fcA=AH;
		fcA/=256; 
		float mp=1+fcA*(0.3-fcA); 
		*dark=(int)((*dark & 0xFFFF)*(1-fcA)) | ((int)(*dark*mp) & 0xFFFF0000);
		int mltpl=min(MaxMultiply,((*dark>>16)+deductLight));
		idst = *destpic;	
		*destpic = ((RedMask & ((idst & RedMask) + (int)((int)(((int)((((int)(lightsR[*angl]*mltpl)<<8))) - (int)(idst & RedMask))) * fcA))) |
		(GreenMask & ((idst & GreenMask) +((int)(((((int)(((int)(lightsG[*angl]*mltpl)))) - (int)(idst & GreenMask)) * AH)>>8)))) |
		(BlueMask & ((idst & BlueMask) + ((int)((((int)(lightsB[*angl]*mltpl)>>8) - (int)(idst & BlueMask)) * AH)>>8)))) |
		(*destpic & 0xFF000000);
		}
		sourpic++; destpic++; dark++; angl++; xes--; dstpix++;

		AH=*dstpix & 0xFFFF; 
	}


}
sourpic+=spNL;
angl+=agNL;
destpic+=dpNL;
dark+=dpNL;
dstpix+=dpNL;
y++;
}
return 1;
}
bool PseudoAnimatedTrans (worldcoordinates* pos, int dx, int dy, SDL_Surface* screen, SDL_Surface* mask, Uint8* angles , int progress, float fade, int dax, int day, Uint32 defLight, float trans)
{
if ((dx>scrwi) || (dy>scrhi) || (dx+mask->w<0) || (dy+mask->h<0)) return 0;	
	smokeRendered=1;
	float detrans=1;
	detrans-=trans;
	detrans/=(1+trans);
	progress= progress % (255*5);
	int frame=8*((progress>=510) + (progress>=765) + (progress>=1020));
	int frameA1, frameA2=0;
	int deductLight=max(160-accurate(pos->h)*6,64);
	//int deductLight=128;
	if (progress<255) frameA1=progress;
	else {frameA2=progress % 255; frameA1=255-frameA2;}
	if (frame>16) frameA2=0;
	//One frame could fade in, second border fade in
//	int slofade=frameA1;
		/*=frameA1+(255-frameA1)*frameA1/255;
	 slofade=slofade+(255-slofade)*slofade/255;*/
	//int border=frameA1;
	//defLight=0x00777777;
	 LightsTo180arrayF(pos, min(0.99,0.91+(0.7/(255*5))*progress), 0, 0,  defLight,0); //defLight, //min(0.99,0.82)+(0.7/(255*5))*progress

	 int MaxMultiply;
MaxMultiply=255*(254.9/(float)max(OnelightR,max(OnelightG,OnelightB)));

	Uint32 * __restrict sourpic, * __restrict destpic , * __restrict dark;
	Uint32* __restrict dstpix;
	Uint8* __restrict angl;
int x=0;
int y=0;
	if (dx<0) x-=dx;
	if (dy<0) y-=dy;
	int destx=mask->w;
	int desty=mask->h;
	if (dx+destx>screen->w) destx=(screen->w-dx);
	if (dy+desty>screen->h) desty=(screen->h-dy);
	
	int AH=255;
sourpic=(Uint32*) mask->pixels  + y*mask->w + x;
angl=angles + (y+E8bitBufferDiam/2-mask->h/2+day)*E8bitBufferDiam + x+E8bitBufferDiam/2-mask->w/2+dax;
destpic=(Uint32*) screen->pixels  + (dy+y)*screen->w + dx+x;
dstpix=dustScreen + (dy+y)*screen->w + dx+x;
dark=darkmask+ (dy+y)*screen->w + dx+x;
int spNL=mask->w+x-destx;
int dpNL=screen->w-destx+x;
int agNL=E8bitBufferDiam-destx+x;
int diff, cA;
register int idst;
register int isrc;
int prang=0;
int skipper=0;
// execution
for (y; y<desty-1;){
int xes=destx-x;


AH=((((*sourpic>>(frame+8)) & 0xFF)*frameA2+((((*sourpic>>frame) & 0xFF)*frameA1))))>>8;
AH=AH*trans+(((*dstpix & 0xFFFF)*AH)>>8)*detrans;
	//AH=(*dstpix & 0xFFFF);
		//(((((*sourpic>>(frame+8)) & 0xFF)*frameA2+((((*sourpic>>frame) & 0xFF)*frameA1))))*(*dstpix & 0xFFFF))>>16; 
//AH=((((*sourpic>>(frame+8)) & 0xFF)*frameA2+((((*sourpic>>frame) & 0xFF)*slofade)))*(*dstpix & 0xFFFF))>>16;
//AH=((((*sourpic>>(frame+8)) & 0xFF)*frameA2+((((*sourpic>>frame) & 0xFF)*slofade))))>>8;
while (xes){

	while ((AH<=0xF) && (xes)){
	skipper++;
	sourpic++;
	xes--;
	dstpix++;

	AH=((((*sourpic>>(frame+8)) & 0xFF)*frameA2+((((*sourpic>>frame) & 0xFF)*frameA1))))>>8;
AH=AH*trans+((*dstpix & 0xFFFF)*AH>>8)*detrans;
	}
dark+=skipper;
destpic+=skipper;
angl+=skipper;

skipper=0;



	while ((AH>0xF) && (xes)){


		AH=min(255,AH);
		float fcA=AH;
		fcA/=256; 
		float mp=1+fcA*(0.3-fcA); 
		*dark=(int)((*dark & 0xFFFF)*(1-fcA)) | ((int)(*dark*mp) & 0xFFFF0000);
		int mltpl=min(MaxMultiply,((*dark>>16)+deductLight));
		idst = *destpic;	
		*destpic = ((RedMask & ((idst & RedMask) + (int)((int)(((int)((((int)(lightsR[*angl]*mltpl)<<8))) - (int)(idst & RedMask))) * fcA))) |
		(GreenMask & ((idst & GreenMask) +((int)(((((int)(((int)(lightsG[*angl]*mltpl)))) - (int)(idst & GreenMask)) * AH)>>8)))) |
		(BlueMask & ((idst & BlueMask) + ((int)((((int)(lightsB[*angl]*mltpl)>>8) - (int)(idst & BlueMask)) * AH)>>8)))) |
		(*destpic & 0xFF000000);
		
		sourpic++; destpic++; dark++; angl++; xes--; dstpix++;

		AH=((((*sourpic>>(frame+8)) & 0xFF)*frameA2+((((*sourpic>>frame) & 0xFF)*frameA1))))>>8;
AH=AH*trans+((*dstpix & 0xFFFF)*AH>>8)*detrans;
	}


}
sourpic+=spNL;
angl+=agNL;
destpic+=dpNL;
dark+=dpNL;
dstpix+=dpNL;
y++;
}
return 1;
}
bool PseudoAnimatedVolume (worldcoordinates* pos, int dx, int dy, SDL_Surface* screen, SDL_Surface* mask, Uint8* angles , int progress, float fade, int dax, int day, Uint32 defLight)
{
if ((dx>scrwi) || (dy>scrhi) || (dx+mask->w<0) || (dy+mask->h<0)) return 0;	
	smokeRendered=1;
	progress= progress % (255*5);
	int frame=8*((progress>=510) + (progress>=765) + (progress>=1020));
	int frameA1, frameA2=0;
	int deductLight=max(160-accurate(pos->h)*6,64);
	//int deductLight=128;
	if (progress<255) frameA1=progress;
	else {frameA2=progress % 255; frameA1=255-frameA2;}
	if (frame>16) frameA2=0;
	Uint32 drowh=max(0,235-(pos->h/accuracy))<<24;
	//One frame could fade in, second border fade in
//	int slofade=frameA1;
		/*=frameA1+(255-frameA1)*frameA1/255;
	 slofade=slofade+(255-slofade)*slofade/255;*/
	//int border=frameA1;
	//defLight=0x00777777;
	 LightsTo180arrayF(pos, min(0.99,0.91+(0.7/(255*5))*progress), 0, 0,  defLight,0); //defLight, //min(0.99,0.82)+(0.7/(255*5))*progress

	 int MaxMultiply;
MaxMultiply=255*(254.9/(float)max(OnelightR,max(OnelightG,OnelightB)));

	Uint32 * __restrict sourpic, * __restrict destpic , * __restrict dark;
	Uint32* __restrict dstpix;
	Uint8* __restrict angl;
int x=0;
int y=0;
	if (dx<0) x-=dx;
	if (dy<0) y-=dy;
	int destx=mask->w;
	int desty=mask->h;
	if (dx+destx>screen->w) destx=(screen->w-dx);
	if (dy+desty>screen->h) desty=(screen->h-dy);
	
	int AH=255;
sourpic=(Uint32*) mask->pixels  + y*mask->w + x;
angl=angles + (y+E8bitBufferDiam/2-mask->h/2+day)*E8bitBufferDiam + x+E8bitBufferDiam/2-mask->w/2+dax;
destpic=(Uint32*) screen->pixels  + (dy+y)*screen->w + dx+x;
dstpix=dustScreen + (dy+y)*screen->w + dx+x;
dark=darkmask+ (dy+y)*screen->w + dx+x;
int spNL=mask->w+x-destx;
int dpNL=screen->w-destx+x;
int agNL=E8bitBufferDiam-destx+x;
int diff, cA;
register int idst;
register int isrc;
int prang=0;
int skipper=0;
// execution
for (y; y<desty-1;){
int xes=destx-x;

	AH=(((((*sourpic>>(frame+8)) & 0xFF)*frameA2+((((*sourpic>>frame) & 0xFF)*frameA1))))>>8); 
//AH=((((*sourpic>>(frame+8)) & 0xFF)*frameA2+((((*sourpic>>frame) & 0xFF)*slofade)))*(*dstpix & 0xFFFF))>>16;
//AH=((((*sourpic>>(frame+8)) & 0xFF)*frameA2+((((*sourpic>>frame) & 0xFF)*slofade))))>>8;
while (xes){

	while (((AH<=0xF) || (drowh-(AH<<21)>=*destpic) ) && (xes) ){ //|| (drowh>=*destpic)  (drowh-*destpic>>24)
	skipper++;
	sourpic++;
	destpic++;
	xes--;
	//AH=((((*sourpic>>(frame+8)) & 0xFF)*frameA2+((((*sourpic>>frame) & 0xFF)*slofade))))>>8;
	//AH=((((*sourpic>>(frame+8)) & 0xFF)*frameA2+((((*sourpic>>frame) & 0xFF)*slofade)))*(*dstpix & 0xFFFF))>>16;
	AH=(((((*sourpic>>(frame+8)) & 0xFF)*frameA2+((((*sourpic>>frame) & 0xFF)*frameA1))))>>8); 
	}
dark+=skipper;

angl+=skipper;
dstpix+=skipper;
skipper=0;



	while ((AH>0xF) && (drowh-(AH<<21)<*destpic) && (xes) ){  //&& (drowh<*destpic)
		
		AH+=(((*dstpix & 0xFFFF)*AH)>>9);
		AH=min(255,AH);
		 //MaxMultiply
		
		float fcA=AH;
		fcA/=256; 
		float mp=1+fcA*(0.3-fcA); 
		*dark=(int)((*dark & 0xFFFF)*(1-fcA)) | ((int)(*dark*mp) & 0xFFFF0000);
		int mltpl=min(MaxMultiply,((*dark>>16)+deductLight));
		idst = *destpic;	
		*destpic = ((RedMask & ((idst & RedMask) + (int)((int)(((int)((((int)(lightsR[*angl]*mltpl)<<8))) - (int)(idst & RedMask))) * fcA))) |
		(GreenMask & ((idst & GreenMask) +((int)(((((int)(((int)(lightsG[*angl]*mltpl)))) - (int)(idst & GreenMask)) * AH)>>8)))) |
		(BlueMask & ((idst & BlueMask) + ((int)((((int)(lightsB[*angl]*mltpl)>>8) - (int)(idst & BlueMask)) * AH)>>8)))) |
		(*destpic & 0xFF000000);
		sourpic++; destpic++; dark++; angl++; xes--; dstpix++;

		AH=(((((*sourpic>>(frame+8)) & 0xFF)*frameA2+((((*sourpic>>frame) & 0xFF)*frameA1))))>>8); 
	}


}
sourpic+=spNL;
angl+=agNL;
destpic+=dpNL;
dark+=dpNL;
dstpix+=dpNL;
y++;
}
return 1;
}
bool PseudoAnimatedOneAngle (worldcoordinates* pos, int dx, int dy, SDL_Surface* screen, SDL_Surface* mask, Uint8* angles , int progress, float fade, int dax, int day, Uint32 defLight, int Max)
{
if ((dx>scrwi) || (dy>scrhi) || (dx+mask->w<0) || (dy+mask->h<0)) return 0;	
	
	progress= progress % (255*4);
	int frame=8*((progress>=510) + (progress>=765));
	int frameA1, frameA2=0;
	
	if (progress<255) frameA1=progress;
	else {frameA2=progress % 255; frameA1=255-frameA2;}
	if (frame>8) frameA2=0;
	//One frame could fade in, second border fade in
	int slofade=frameA1;
		/*=frameA1+(255-frameA1)*frameA1/255;
	 slofade=slofade+(255-slofade)*slofade/255;*/
	//int border=frameA1;
	 LightsToSingleValueF(pos, Max);
int lR=min((int)OnelightR,1024);
int lG=min((int)OnelightG,1024);
int lB=min((int)OnelightB,1024);
lR<<=14;
lG<<=6;
lB>>=2;
	Uint32 * __restrict sourpic, * __restrict destpic, * __restrict dstpix;
int x=0;
int y=0;
	if (dx<0) x-=dx;
	if (dy<0) y-=dy;
	int destx=mask->w;
	int desty=mask->h;
	if (dx+destx>screen->w) destx=(screen->w-dx);
	if (dy+desty>screen->h) desty=(screen->h-dy);
	
	int AH=255;
sourpic=(Uint32*) mask->pixels  + y*mask->w + x;
destpic=(Uint32*) screen->pixels  + (dy+y)*screen->w + dx+x;
dstpix=dustScreen + (dy+y)*screen->w + dx+x;
int spNL=mask->w+x-destx;
int dpNL=screen->w-destx+x;
int diff, cA;
register int idst;
register int isrc;
for (y; y<desty;){
int xes=destx-x;
AH=(((*sourpic>>(frame+8)) & 0xFF)*frameA2+((((*sourpic>>frame) & 0xFF)*slofade)+AH))>>9;
while (xes){
	int skipper=0;
while ((!AH) && (xes)){
	sourpic++;
	skipper++;
	xes--;
	AH=(((*sourpic>>(frame+8)) & 0xFF)*frameA2+((((*sourpic>>frame) & 0xFF)*slofade)+AH))>>9;
	}
destpic+=skipper;
dstpix+=skipper;

/*	while ((AH>>7) && (xes)){
	//*destpic = OneLight;
	htpBlit(destpic, OneLight);
	sourpic++; destpic++; xes--;
	AH=(((*sourpic>>(frame+8)) & 0xFF)*frameA2+((((*sourpic>>frame) & 0xFF)*slofade)+AH))>>8;
	}*/
/* 
*/


	while((AH) && (xes)){
		//AH+=(((*dstpix & 0xFFFF)*AH)>>8);
		//AH=min(AH,255);
		idst = *destpic;
		*destpic = ((RedMask & ((idst & RedMask) + ((int)(((int)(((lR ))) - (int)(idst & RedMask)) * AH) >>8))) |
		(GreenMask & ((idst & GreenMask) + ((int)(((((int)(lG))) - (int)(idst & GreenMask)) * AH) >>8))) |
		(BlueMask & ((idst & BlueMask) + ((int)(((((int)(lB))) - (int)(idst & BlueMask)) * AH) >>8)))) | (idst & 0xFF000000);
		sourpic++; destpic++; xes--; dstpix++;
		AH=(((*sourpic>>(frame+8)) & 0xFF)*frameA2+((((*sourpic>>frame) & 0xFF)*slofade)+AH))>>9;
	}
}
sourpic+=spNL;
destpic+=dpNL;
dstpix+=dpNL;
y++;
}
return 1;
}
// **********************************************EFFECTS

void AddLight(worldcoordinates* pos, objectLight * objlight)
{
	int i=0;
	while ((light[i].light) && (light[i].light->active) && (i<maxLights)) i++;
	if (i==maxLights) return; 
	
	light[i].coords=pos;
	light[i].light=objlight;
	light[i].light->active=1;
	if (i>=lights) lights=i+1;
}


int addEffect()
{
	//return 0;
	int i=PEcache;
	while ((smoke[i].type) && (i<PElim)) i++;
	if (i>=PElim) { PEcache=2;
	while ((smoke[i].type) && (i<PElim)) i++;
	if (i>=PElim)
	return 0;
	} 
	smoke[i].blitToTyle=createData.blitToTyle;
	smoke[i].type=createData.type;
	if (!smoke[i].type) 
		smoke[i].type=1;
	smoke[i].pos.tx=createData.tx;
	smoke[i].pos.ty=createData.ty;
	smoke[i].pos.x=createData.x;
	smoke[i].pos.y=createData.y;
	smoke[i].pos.h=createData.h;
	smoke[i].gravity=createData.gravity;
	smoke[i].slowing=createData.slowing;
	smoke[i].hp=createData.hp;
	smoke[i].deathcondition=createData.boolFunction;
	smoke[i].drowing=createData.voidfunction;
	smoke[i].xaccel=createData.xaccel;
	smoke[i].yaccel=createData.yaccel;
	smoke[i].speedx=createData.xspeed;
	smoke[i].speedy=createData.yspeed;
	smoke[i].speedh=createData.hspeed;
	smoke[i].anglex=createData.anglex;
	smoke[i].angley=createData.angley;
	smoke[i].angleh=createData.angleh;
	smoke[i].rotatex=createData.rotationx;
	smoke[i].rotatey=createData.rotationy;
	smoke[i].rotateh=createData.rotationh;
	smoke[i].g=createData.g;
	smoke[i].mask=createData.mask;
	smoke[i].maskIMG=createData.maskIMG;
	smoke[i].decaySurface=createData.decaySurfaceNo;
	smoke[i].offx=createData.offx;
	smoke[i].offy=createData.offy;
	smoke[i].staticPos=createData.staticPos;
	smoke[i].displayTime=createData.displayTime;
	smoke[i].G_c=createData.G_c;
	smoke[i].G_p=createData.G_p;
	smoke[i].MaxLight=createData.MaxLight;
	smoke[i].G8t=createData.G8t;
	if (createData.light.active)
	{
	smoke[i].light.color=createData.light.color;
	smoke[i].light.dinamic=createData.light.dinamic;
	smoke[i].light.distanceFading=createData.light.distanceFading;
	smoke[i].light.active=1;
	smoke[i].light.angleLight=0;
	smoke[i].light.brightness=createData.light.brightness;
	smoke[i].light.angleFading=createData.light.angleFading;
	AddLight(&smoke[i].pos, &smoke[i].light);
	createData.light.active=0;
	}
	hig=min (accurate(smoke[i].pos.h)/pixInPhis, matrhig-1)+matrdepth;
hig=max(0,hig);
smoke[i].prev=&PEfirst[hig];
smoke[i].next=PEfirst[hig].next;
PEfirst[hig].next=PEfirst[hig].next->prev=&smoke[i];

/*bl[i].prev=&bfirst[hig];
bl[i].next=bfirst[hig].next;
bfirst[hig].next->prev=bfirst[hig].next=&bl[i];*/

 // in the end

PEcache=i+1;

return i;
}
void SmokeDinamics()
{
	

//PEpoint->slowing=max(0,PEpoint->slowing-UpgoingSmokeSlowingRisePerFrame*GameSpeed);
dddcoords accel;
 GetWind(&accel, offx, offy, PEpoint->pos.h);

PEpoint->speedx+=(accel.x-PEpoint->speedx)/20;
PEpoint->speedy+=(accel.y-PEpoint->speedx)/20;

if (!PEpoint->staticPos){
PEpoint->anglex+=(double)PEpoint->speedx*2/accuracy;
PEpoint->angley+=(double)PEpoint->speedy*2/accuracy;
}

int rad=36;
if (PEpoint->anglex>rad) PEpoint->anglex=rad;
if (PEpoint->anglex<-rad) PEpoint->anglex=-rad;

if (PEpoint->angley>rad) PEpoint->angley=rad;
if (PEpoint->angley<-rad) PEpoint->angley=-rad;

speedLimitCheckINT(&PEpoint->speedx, &PEpoint->speedy, &PEpoint->speedh, 1.5 );

}
//bool 
bool bnothing()
{
return 0;
}
bool timedDeath()
{
	if (PEpoint->hp<=0) return 1;
	PEpoint->hp-=GameSpeed;
	return 0;
}
bool offScreenDeath()
{
int elevation=PEpoint->pos.h/dddstength;
offx=offx+=((offx-scrwid)*elevation)>>16;
offy=offy+=((offy-scrhid)*elevation)>>16;
return (!(OnScreen(offx, offy, 1, 1)));
	//if (PEpoint->hp<0) return 1;
	//PEpoint->hp-=1;
	//return 0;
}
bool belowgroundmuch ()
{
	if (PEpoint->pos.h<0) 
		return 1;
	//addFireEffect(&curphisobj->pos, &curphisobj->force, 0.85);
	return 0;
}
bool belowgroundToomuch ()
{
	if (PEpoint->pos.h<-accuracy*16) 
		return 1;
	//addFireEffect(&curphisobj->pos, &curphisobj->force, 0.85);
	return 0;
}

bool noHealth()
{
	if (PEpoint->hp<0) return 1;
	return 0;
}
//VolumetricImageNLightBlitBorder
void TrueHeightDrow()
{

	int elevation=max(0,PEpoint->hp-accuracy*15)/dddstength;
offx+=(((offx-scrwid)*elevation)>>16)-PEpoint->maskIMG->w/2;
offy+=(((offy-scrhid)*elevation)>>16)-PEpoint->maskIMG->h/2;

//offx=offx+=((offx-scrwid)*elevation)>>19;
//offy=offy+=((offy-scrhid)*elevation)>>19;

elevation=(PEpoint->pos.h>>12) - 255;
//elevation-=255;
if (elevation>0) elevation=0;
/*
if (PEpoint->speedh<0)
	VolumetricSmokeBlit(screen, image[PEpoint->type], mergeimage[PEpoint->type],offx, offy, elevation, PEpoint->MaxLight);
else*/
//VolumetricSmokeBlitBorder

    VolumetricImageNLightBlitBorder(screen, PEpoint->maskIMG,offx, offy, elevation,PEpoint->MaxLight);

//EffectBlitWithAlphaInprint(screen, image[PEpoint->type], mergeimage[PEpoint->type], offx-image[PEpoint->type]->w/2, offy-image[PEpoint->type]->h/2, elevation);

//SmokeDinamics();

//if (elevation>-128)
//CastShadowForDarkenfFunk(image[PEpoint->type+1+(elevation<-64)],screen,offx,offy-image[PEpoint->type+1]->h/2);

}
void HeightDrow()
{

	int elevation=max(0,PEpoint->hp-accuracy*15)/dddstength;
offx+=(((offx-scrwid)*elevation)>>16)-image[PEpoint->type]->w/2;
offy+=(((offy-scrhid)*elevation)>>16)-image[PEpoint->type]->h/2;

//offx=offx+=((offx-scrwid)*elevation)>>19;
//offy=offy+=((offy-scrhid)*elevation)>>19;

elevation=(PEpoint->pos.h>>12) - 255;
//elevation-=255;
if (elevation>0) elevation=0;

if (PEpoint->speedh<0)
	VolumetricSmokeBlit(screen, image[PEpoint->type], mergeimage[PEpoint->type],offx, offy, elevation, PEpoint->MaxLight);
else
VolumetricSmokeBlitBorder(screen, image[PEpoint->type], mergeimage[PEpoint->type],offx, offy, elevation,PEpoint->MaxLight);

//EffectBlitWithAlphaInprint(screen, image[PEpoint->type], mergeimage[PEpoint->type], offx-image[PEpoint->type]->w/2, offy-image[PEpoint->type]->h/2, elevation);

//SmokeDinamics();

//if (elevation>-128)
//CastShadowForDarkenfFunk(image[PEpoint->type+1+(elevation<-64)],screen,offx,offy-image[PEpoint->type+1]->h/2);

}
void HeightLight()
{
	SDL_Surface* img=PEpoint->maskIMG;
int elevation=max(0,PEpoint->hp-accuracy*15)/dddstength;
offx+=(((offx-scrwid)*elevation)>>16)-img->w/2;
offy+=(((offy-scrhid)*elevation)>>16)-img->h/2;
offx-=img->w/2;
offy-=img->h/2;
//offx=offx+=((offx-scrwid)*elevation)>>19;
//offy=offy+=((offy-scrhid)*elevation)>>19;

elevation=(PEpoint->pos.h>>12) - 255;
//elevation-=255;
if (elevation>0) elevation=0;

//if (PEpoint->speedh<0)
VolumetricLightBlit(screen, img,offx, offy);
//else
//VolumetricLightBlitBorder(screen, img, offx, offy, elevation,PEpoint->MaxLight);

//EffectBlitWithAlphaInprint(screen, image[PEpoint->type], mergeimage[PEpoint->type], offx-image[PEpoint->type]->w/2, offy-image[PEpoint->type]->h/2, elevation);

SmokeDinamics();

//if (elevation>-128)
//CastShadowForDarkenfFunk(image[PEpoint->type+1+(elevation<-64)],screen,offx,offy-image[PEpoint->type+1]->h/2);

}
void HeightDrowFormergeimaged()
{
int elevation=((PEpoint->pos.h>>13))-255;
VolumetricSmokeBlit(screen, mergeimaged[PEpoint->type], mergeimage[PEpoint->type],offx-mergeimaged[PEpoint->type]->w/2, offy-mergeimaged[PEpoint->type]->h/2, elevation,PEpoint->MaxLight);
if (elevation<-115) PEpoint->pos.h=0;
}
void LightBlickDrow()
{
int elevation=PEpoint->pos.h/dddstength;
offx+=(((offx-scrwid)*elevation)>>16);
offy+=(((offy-scrhid)*elevation)>>16);



if (PEpoint->hp<=150)
{
//	CastLightForDarkenfFunk (shootAnim[PEpoint->type], screen, offx-shootAnim[PEpoint->type]->w/2, offy-shootAnim[PEpoint->type]->h/2, PEpoint->displayTime);	

PEpoint->hp-=40*GameSpeed;
}
else
if (PEpoint->hp>180)
{
/*	CastLightForDarkenfFunk (shootAnim[PEpoint->type+2], screen, offx-shootAnim[PEpoint->type+2]->w/2, offy-shootAnim[PEpoint->type+2]->h/2, PEpoint->displayTime);
	EffectBlitWithAlphaInprint(screen, shootAnim[PEpoint->type], shootMASK[PEpoint->mask], offx-shootAnim[PEpoint->type]->w/2, offy-shootAnim[PEpoint->type]->h/2,255);
*/
	PEpoint->hp-=36*GameSpeed;
}

if ((PEpoint->hp<=220) && (PEpoint->hp>120))
	{
	//	LightSpotEyeEffect (offx-shootAnim[PEpoint->type+1]->w/2, offy-shootAnim[PEpoint->type+1]->h/2, shootAnim[PEpoint->type+1], PEpoint->hp);


	PEpoint->hp-=30*GameSpeed;
}
	

}


void DroudPseudoAnimatedUpgoing()
{
	/*if (!OnScreen( offx,  offy, 100,100))
	{	PEpoint->hp=-1;
		return;
	}*/

SmokeDinamics();

CastShadowForDarkenfFunk(flameLight[PEpoint->type-1],screen,offx+accurate(PEpoint->pos.h),offy,max(PEpoint->pos.h,(PEpoint->hp-255*4.5)*accuracy),0);
int progress=1279-PEpoint->hp;
// Gravity change -0.04 -> -0.01;

PEpoint->hp-=accurate(abs(PEpoint->speedx)+abs(PEpoint->speedy));
int elevation=PEpoint->pos.h/dddstength;
PEpoint->hp-=PEpoint->angleh*GameSpeed;
//asCastShadowForDarkenfFunk(fireSmokeMask[PEpoint->type-1],screen,offx+accurate(PEpoint->pos.h),offy,min(PEpoint->hp,(255*4-PEpoint->hp)*6),1);
doffx-=PEpoint->maskIMG->w/2;
doffy-=PEpoint->maskIMG->h/2;
double fade=0.992;//-UpgoingAngularFadechange*7500;//0.995;
//progress=progress*progress/1024;

//if (progress<200)
//PseudoAnimatedDust(&PEpoint->pos,doffx, doffy, screen, PEpoint->maskIMG,BufferAnglesCopy,progress , fade,PEpoint->anglex,PEpoint->angley, *PEpoint->g);
//else
	if (progress<400)
 PseudoAnimatedTrans(&PEpoint->pos,doffx, doffy, screen, PEpoint->maskIMG,BufferAnglesCopy,progress , fade,PEpoint->anglex,PEpoint->angley, *PEpoint->g, (float)(progress)/400);
	else
PseudoAnimatedVolume (&PEpoint->pos,doffx, doffy, screen, PEpoint->maskIMG,BufferAnglesCopy,progress , fade,PEpoint->anglex,PEpoint->angley, *PEpoint->g);

//	LightMaskFromBuffer(offx, offy, &dLISmoke[PEpoint->type-1], &PEpoint->pos, (float)min(PEpoint->hp,(300-PEpoint->hp)*6)/128);

}
void DrowFlame()
{
	if (!OnScreen( doffx,  doffy, PEpoint->maskIMG->w,PEpoint->maskIMG->h))
	{

		PEpoint->hp=-1;
	
		return;
	}

	int fl=rand() % 4;
	//if (fl<4)
	CastLightForDarkenfFunk (flameLight[fl], screen, doffx-flameLight[fl]->w/2,  doffy-flameLight[fl]->h/2, PEpoint->angleh);
//VolumetricLightBlit(screen, flameLight[3],offx-flameLight[3]->w/2, offy-flameLight[3]->h/2);

//addDust(&PEpoint->pos, PEpoint->pos.x, PEpoint->pos.y, FireDustRandy, PEpoint->speedx , PEpoint->speedy, fireDustHspeed, 3,2, 0);

PEpoint->offx-=accurate(PEpoint->speedx*15);
PEpoint->offy-=accurate(PEpoint->speedy*15);
FireDisplacement (&OverlapUpFlame[PEpoint->type-1], fireGlobImg, PEpoint->anglex, PEpoint->angley, screen, PEpoint->offx, PEpoint->offy,max(0, 40-PEpoint->hp*3 ));

PEpoint->offx=doffx;
PEpoint->offy=doffy;

double d=fireGlobImg->w/2;
PEpoint->anglex=((PEpoint->anglex-d)/(double)160.1*double(159.1))+d;
PEpoint->angley=(PEpoint->angley-d)/(double)160.1*double(159.1)+d;


PEpoint->hp-=ipifagor (accurate(PEpoint->speedx), accurate(PEpoint->speedy))*GameSpeed;



/*dddcoords accel=wind.mask[max(0,offy/8*wind.w)+max(0,offx/8)];
if (abs(accel.x)>abs(PEpoint->rotatex))
{
//acc=accel.x;
//acc/=accuracy;
	PEpoint->speedx+=accel.x*GameSpeed;
}
if (abs(accel.y)>abs(PEpoint->rotatey))
{
PEpoint->speedy+=accel.y*GameSpeed;
}*/



}

void addDLSmoke(worldcoordinates* pos)
{

createData.tx=pos->tx;
createData.ty=pos->ty;
createData.x=pos->x; 
createData.y=pos->y;
createData.h=pos->h;
createData.type=(rand() % 4) + 1;
bool pseudoSmoke=1;//(rand() % 3); // change to Rand 3 to return old smoke // change to int to have small smoke
if (pseudoSmoke) 
{
	if (pseudoSmoke==2)
createData.maskIMG=PseudoAnim[rand() % 8];
	else
createData.maskIMG=PseudoAnimBig[1+(rand() % 6)];
createData.hp=255*5;
createData.anglex=(rand() % 30) - 15;//fireGlobImg->w/2+((rand() % 50) - 25);
createData.angley=(rand() % 30) - 15;//fireGlobImg->h/2+((rand() % 50) - 25);
createData.angleh=(rand() % 2) + 4; // speed of dissolve
createData.voidfunction=DroudPseudoAnimatedUpgoing;//DirectedShadowLDrow;
}
else
{
createData.maskIMG=flameMask[rand() % 6];
createData.hp=200;//100+(rand() % 150);
createData.anglex=PEpoint->anglex;//fireGlobImg->w/2+((rand() % 50) - 25);
createData.angley=PEpoint->angley;//fireGlobImg->h/2+((rand() % 50) - 25);
createData.rotationx=0;//-force->speedx/5;
//createData.rotationx/=accuracy;
createData.rotationy=0;//-force->speedy/5;
//createData.rotationy/=accuracy;
createData.voidfunction=DirectedShadowLDrow;
}
//createData.type=0;
//int ax=accurate(frce->speedx), ay=accurate(force->speedy);
//if (!ax) ax=1;
//if (!ay) ay=1;
//int pif=ipifagor (ax, ay);
createData.xspeed=-PEpoint->rotatex*accuracy/6;//force->speedx+force->speedh/pif*ax;
createData.yspeed=-PEpoint->rotatey*accuracy/6;//force->speedy+force->speedh/pif*ay;
createData.hspeed=accuracy;//*temptest;



createData.g=&UpgoingSmokeColor;
createData.boolFunction=&timedDeath;


createData.slowing=0.9;
createData.boolFunction=&timedDeath;

createData.gravity=-1;//-0.01* (rand() % 10);

createData.light.active=0;
createData.light.color=0;
createData.light.dinamic=1;
createData.light.distanceFading=0.0033;
/*createData.light.dinamic=1;
createData.light.distanceFading=0.00033;*/

createData.staticPos=0;
addEffect();
}
int addFlameEffect(worldcoordinates* pos, float brightness)
{
createData.maskIMG=flameMask[rand() % 6];
	createData.type=(rand() % 4) + 1;
createData.tx=pos->tx;
createData.ty=pos->ty;
createData.x=pos->x; 
createData.y=pos->y;
createData.h=pos->h;
//int ax=accurate(force->speedx), ay=accurate(force->speedy);
//if (!ax) ax=1;
//if (!ay) ay=1;
//int pif=ipifagor (ax, ay);
createData.xspeed=-PEpoint->rotatex*accuracy+radiusx(30)*(accuracy/80);//force->speedx+force->speedh/pif*ax;
createData.yspeed=-PEpoint->rotatey*accuracy+radiusy*(accuracy/80);//force->speedy+force->speedh/pif*ay;
createData.xaccel=0;
createData.yaccel=0;
createData.hspeed=0;
createData.hp=30 + (rand() % 30);
createData.anglex=PEpoint->anglex;//fireGlobImg->w/2+((rand() % 50) - 25);
createData.angley=PEpoint->angley;//fireGlobImg->h/2+((rand() % 50) - 25);
createData.angleh=brightness;
createData.rotationx=0;//-force->speedx/5;
//createData.rotationx/=accuracy;
createData.rotationy=0;//-force->speedy/5;
//createData.rotationy/=accuracy;
createData.slowing=0.99;
createData.gravity=-0.1;
createData.boolFunction=&timedDeath;
createData.voidfunction=&DrowFlame;
createData.offx=-100;
createData.offy=-100;
createData.light.active=0;

/*createData.light.dinamic=1;
createData.light.distanceFading=0.00033;*/

createData.staticPos=0;
return addEffect();
}
bool onHealthFireDecay()
{
	if (PEpoint->hp>0) return 0;


//decayTyles(int tx, int ty, int x, int y, SDL_Surface* src, SDL_Surface* decayBG)
return 1;
}

void drowNewDisplacement(){

	DrowMyIndAlphaDisplacementSameSurface (&imgBank.displacementGhost, offx, offy, hillGrapth(PEpoint->hp,BoomDisplacementHealth)*30, screen);
	//PEpoint->hp--;
}
void basicDrow()
{
SDL_Rect qdest, src;
int elevation=PEpoint->pos.h/dddstength;
qdest.x=offx+=((offx-scrwid)*elevation)>>16;
qdest.y=offy+=((offy-scrhid)*elevation)>>16;
src.y=PEpoint->hp/400;
src.x=PEpoint->hp/100-src.y*4;
src.y=(3-src.y)*300;
src.x=(3-src.x)*300;
src.w=300;
src.h=300;
SDL_BlitSurface(image[PEpoint->type], &src, screen, &qdest);
//BlitNoInprint(screen, image[PEpoint->type], int dx, int dy, int h)
//drowstyle(offx,offy,PEpoint->h,PEpoint->type);
}
void boomDrow()
{
	SDL_Rect qdest, src;
int elevation=PEpoint->pos.h/dddstength;
qdest.x=offx+=((offx-scrwid)*elevation)>>16;
qdest.y=offy+=((offy-scrhid)*elevation)>>16;
src.y=PEpoint->hp/18;
src.x=PEpoint->hp/3-src.y*6;
src.y=(7-src.y)*69;
src.x=(5-src.x)*69;
src.w=69;
src.h=69;
SDL_BlitSurface(image[PEpoint->type], &src, screen, &qdest);

//drowstyle(offx,offy,PEpoint->h,PEpoint->type);
}

void FireBlackSmoke()
{
createData.tx=PEpoint->pos.tx; createData.ty=PEpoint->pos.ty;
createData.slowing=1;

createData.x=PEpoint->pos.x;
createData.y=PEpoint->pos.y;
createData.hp=PEpoint->pos.h;
createData.h=PEpoint->pos.h+accuracy*25;//max(curphisobj->pos.h,accuracy*35);
createData.type=1502 + (rand() % 5); 
createData.hspeed=(rand() % accuracy) - accuracy/2;
createData.xaccel=0;
createData.yaccel=0;
createData.hp=PEpoint->pos.h;
createData.MaxLight=rand() % 2;
createData.gravity=0.1;//30+rand() % 40;
//createData.gravity/=100;
createData.boolFunction=&belowgroundmuch;
if (rand() % HeightDrowChance)
createData.voidfunction=&HeightDrow;
else
{
	createData.gravity=0.05;
	//createData.h=PEpoint->pos.h+accuracy*15;
	createData.voidfunction=&HeightLight;  createData.maskIMG=flameLight[rand() % 4];
}
createData.staticPos=0;
addEffect();
}
void BlitFire()
{
decayTyles(PEpoint->pos.tx, PEpoint->pos.ty, accurate(PEpoint->pos.x), accurate(PEpoint->pos.y),flameDecay,image[87]);//87]);
int totylex=tylextogrid(PEpoint->pos.tx)+1;
int totyley=tyleytogrid(PEpoint->pos.ty)+1;
RefreshGround (accurate(PEpoint->pos.x)-flameDecay->w/2, accurate(PEpoint->pos.y)-flameDecay->h/2, flameDecay->w, flameDecay->h, totylex, totyley);
/*
SDL_Surface* img=DecayMaskForGuts[rand() % DecayGutsMask];
decayTyles(curphisobj->pos.tx, curphisobj->pos.ty, accurate(curphisobj->pos.x), accurate(curphisobj->pos.y), img, mergeimaged[8]);//7+(rand() % 3)

int totylex=tylextogrid(curphisobj->pos.tx)+1;
int totyley=tyleytogrid(curphisobj->pos.ty)+1;

RefreshGround (accurate(curphisobj->pos.x)-img->w/2, accurate(curphisobj->pos.y)-img->h/2, img->w, img->h, totylex, totyley);*/


//PEpoint->drowing=&DrowFire;
//DrowFire();
}
void DrowFire()
{
	/*if (!OnScreen( offx,  offy, PEpoint->maskIMG->w,PEpoint->maskIMG->h))
	{

		PEpoint->hp=-1;
		return;
	}*/

//PEpoint->offx+=rand() % 5 - 2;
//PEpoint->offy+=rand() % 5 - 2;
	float newSlo=1; newSlo-=0.005*GameSpeed;
	//if ((!(rand() % 40)) && (PEpoint->light.active))


	
	if (soundEffects) PlayAmbient(&sounds.bank.flameBurning, 6, 10);
	PEpoint->anglex+=PEpoint->rotatex*GameSpeed;
	PEpoint->angley+=PEpoint->rotatey*GameSpeed;
	PEpoint->rotatex*=newSlo;
	PEpoint->rotatey*=newSlo;
	createDirectionalWind(offx, offy, 0, 0, 200/GameSpeed, 3);
	
	if (!(rand() % 5))	
	{
addDust(&PEpoint->pos, PEpoint->pos.x, PEpoint->pos.y, FireDustRandy, PEpoint->speedx , PEpoint->speedy, fireDustHspeed, 10,15, 15);
			//if (!(rand() % 50))	
	//PEpoint->
createVortex(offx, offy, PEpoint->hp*5000, 10);
	//	speedLimitCheck(&PEpoint->rotatex, &PEpoint->rotatey, &PEpoint->rotateh, GameSpeed/2);
createDirectionalWind(offx, offy, 0, 0, 4000/GameSpeed, 3);
//addDust(&PEpoint->pos, PEpoint->pos.x, PEpoint->pos.y, FireDustRandy, PEpoint->speedx , PEpoint->speedy, fireDustHspeed*3, 20,20, 0);
		addFlameEffect(&PEpoint->pos, 0.2);
		//addDust(&PEpoint->pos, PEpoint->pos.x, PEpoint->pos.y, FireDustRandy, PEpoint->speedx , PEpoint->speedy, fireDustHspeed, 20,5, 15);
		//FireBlackSmoke();
	}
	else
		createTornado(offx, offy, 250, 5, (int)PEpoint->hp % 2);

	if (FireDisplacement (&OverlapUp[PEpoint->type-1], fireGlobImg, PEpoint->anglex, PEpoint->angley, screen, offx, offy, PEpoint->displayTime/20))
	{
		PEpoint->hp-=1;
		PEpoint->anglex=fireGlobImg->w/2;
PEpoint->angley=fireGlobImg->h/2;
BlitFire();
	}
	else
		
		{
			int rad=fireGlobImg->w/2;
			int dis=ipifagor (PEpoint->anglex-fireGlobImg->w/2, PEpoint->angley-fireGlobImg->h/2);
			PEpoint->light.distanceFading-=(PEpoint->light.distanceFading-0.005)*flameLightStability+GetRandomNegPos(20, 1)*0.00001;
			PEpoint->displayTime=(PEpoint->displayTime)*0.8+(rand() % 50);
		/*	if (!(rand() % 15))
			{
createData.xspeed=radiusx(20)*(accuracy/80) - PEpoint->rotatex*accuracy/4;
createData.yspeed=radiusy*(accuracy/80) - PEpoint->rotatey*accuracy/4;
			FireBlackSmoke();
			}*/

			int fl=rand() % 4;
			//int fl=1;
		CastLightForDarkenfFunk (flameLight[fl], screen, offx-flameLight[fl]->w/2, offy-flameLight[fl]->h/2, fireBrightness);

	
		}

newSlo=1; newSlo-=0.002*GameSpeed;
PEpoint->rotatex*=newSlo;
PEpoint->rotatey*=newSlo;
//int scrwi=1200;
//int scrhi=600;
// Displace away from center
int Blowoffx=((offx-scrwid));
int Blowoffy=((offy-scrhid));
dddcoords accel;
if (OnScreen(offx,  offy, 1,1))
accel=wind.mask[max(0,offy/8*wind.w)+max(0,offx/8)];
else accel=wind.global;
/*double acc;
if ((accel.x - PEpoint->rotatex)>1000)
{
acc=accel.x*GameSpeed;
acc/=1000;
PEpoint->rotatex-=acc;
}
if ($$$(accel.y-PEpoint->rotatey))
{
acc=accel.y*GameSpeed;
acc/=1000;
PEpoint->rotatey-=acc;
}*/
PEpoint->rotatex-=((accel.x*GameSpeed)-PEpoint->rotatex)/fireDirectionChangeDiv+Blowoffx*fireFromCenterBlowForce;
PEpoint->rotatey-=((accel.y*GameSpeed)-PEpoint->rotatey)/fireDirectionChangeDiv+Blowoffy*fireFromCenterBlowForce;

speedLimitCheck(&PEpoint->rotatex, &PEpoint->rotatey, &PEpoint->rotateh, GameSpeed/1.5);
if (PEpoint->creation<0){ 
	 addDLSmoke(&PEpoint->pos); 
	// addDust(&PEpoint->pos, PEpoint->pos.x, PEpoint->pos.y, FireDustRandy, PEpoint->speedx , PEpoint->speedy, fireDustHspeed, 40,5, 2);
	 PEpoint->creation=10;}
PEpoint->creation--;
}

void DrowPseudo()
{
/*if (!OnScreen( offx,  offy, PEpoint->maskIMG->w,PEpoint->maskIMG->h))
	{
		PEpoint->hp=-1;
		return;
	}*/
SmokeDinamics();
CastShadowForDarkenfFunk(flameLight[0],screen,offx+accurate(PEpoint->pos.h),offy,128-(255-PEpoint->hp)*(PEpoint->hp<255)/2,1);

//CastShadowForDarkenfFunk(fireSmokeMask[1],screen,offx+accurate(PEpoint->pos.h),offy,min(PEpoint->hp,(200-PEpoint->hp)*6),1);
PEpoint->hp-=PEpoint->angleh;
PseudoAnimatedVolume (&PEpoint->pos,doffx-PEpoint->maskIMG->w/2, doffy-PEpoint->maskIMG->h/2, screen, PEpoint->maskIMG,BufferAnglesCopy, 255*5-PEpoint->hp, smokeAngleFade,PEpoint->anglex,PEpoint->angley, *PEpoint->g); //fade 0.996

}
void DrowPseudoWithLightBlic()
{
/*if (!OnScreen( offx,  offy, PEpoint->maskIMG->w,PEpoint->maskIMG->h))
	{
		PEpoint->hp=-1;
		return;
	}*/
SmokeDinamics();
PEpoint->hp-=PEpoint->angleh;
int msk=PEpoint->mask;
//PseudoAnimatedVolume (&PEpoint->pos,doffx-PEpoint->maskIMG->w/2, doffy-PEpoint->maskIMG->h/2, screen, PEpoint->maskIMG,BufferAnglesCopy, 255*5-PEpoint->hp, smokeAngleFade,PEpoint->anglex,PEpoint->angley, *PEpoint->g); //fade 0.996
if (PEpoint->hp>255*4+128+64)//192-32
	CastLightForDarkenfFunkNoVolume (shootAnim[msk], doffx-shootAnim[msk]->w/2, doffy-shootAnim[msk]->h/2,hillGrapth(PEpoint->hp-255*4-128-64, 64)*PEpoint->rotateh);// (PEpoint->hp-255*3-192)/(64));

else
{
	int shx=timeofdayx*PEpoint->pos.h*2/accuracy;
	int shy=timeofdayy*PEpoint->pos.h*2/accuracy;
	CastShadowForDarkenfFunk(flameLight[0],screen,shx+offx,shy+offy,128-(255-PEpoint->hp)*(PEpoint->hp<255)/2,1);
}
int progress=max(PEpoint->hp, 128);
double fade=0.992;
if (progress<200)
PseudoAnimatedDust(&PEpoint->pos,doffx-PEpoint->maskIMG->w/2, doffy-PEpoint->maskIMG->h/2, screen, PEpoint->maskIMG,BufferAnglesCopy,progress , fade,PEpoint->anglex,PEpoint->angley, *PEpoint->g);
else
	if (progress<400){
 PseudoAnimatedTrans(&PEpoint->pos,doffx-PEpoint->maskIMG->w/2, doffy-PEpoint->maskIMG->h/2, screen, PEpoint->maskIMG,BufferAnglesCopy,progress , fade,PEpoint->anglex,PEpoint->angley, *PEpoint->g, (float)(progress-200)/200);
	}
	else
PseudoAnimatedVolume (&PEpoint->pos,doffx-PEpoint->maskIMG->w/2, doffy-PEpoint->maskIMG->h/2, screen, PEpoint->maskIMG,BufferAnglesCopy,progress , fade,PEpoint->anglex,PEpoint->angley, *PEpoint->g);

const int from=510;
const int to=330;


if ((progress<from) && (progress>to))
	addDust(&PEpoint->pos, PEpoint->pos.x, PEpoint->pos.y, CloudSpawnRandy/3, 0 , 0, 0, (progress-to)*36/(from-to),10, 0);

/*int el=jhh;
el/=dddstength;
offx+=((offx-scrwid)*el)>>16;
offy+=((offy-scrhid)*el)>>16;*/

}
void DrowPseudoSmall()
{
/*if (!OnScreen( offx,  offy, PEpoint->maskIMG->w,PEpoint->maskIMG->h))
	{
		PEpoint->hp=-1;
		return;
	}*/
SmokeDinamics();
int elevation=PEpoint->pos.h/dddstength;
offx+=(((offx-scrwid)*elevation)>>16);
offy+=(((offy-scrhid)*elevation)>>16);
PEpoint->hp-=PEpoint->angleh;
PseudoAnimatedOneAngle (&PEpoint->pos,offx-PEpoint->maskIMG->w/2, offy-PEpoint->maskIMG->h/2, screen, PEpoint->maskIMG,BufferAnglesCopy, 255*4-PEpoint->hp, 0.996,PEpoint->anglex,PEpoint->angley, *PEpoint->g, PEpoint->MaxLight);
	
}
void gunshoot()
{
SDL_Rect qdest;
int elevation=PEpoint->pos.h/dddstength;
offx+=(((offx-scrwid)*elevation)>>16);
offy+=(((offy-scrhid)*elevation)>>16);
qdest.x=offx-shootAnim[PEpoint->type]->w/2;//-shootAnim[PEpoint->type]->w/2;
qdest.y=offy-shootAnim[PEpoint->type]->h/2;//-shootAnim[PEpoint->type]->h/2;
//SDL_BlitSurface(shootAnim[PEpoint->type], NULL, screen, &qdest); PEpoint->
//PEpoint->hp;
//PEpoint->slowing;
int half=PEpoint->slowing/2+1;
if (half<PEpoint->hp)
{
//EffectBlitWithAlphaInprint(screen, shootAnim[PEpoint->type], shootMASK[PEpoint->mask], qdest.x, qdest.y,255-255*(PEpoint->hp-half)/half);
//int ddx=offx+hillGrapth(PEpoint->hp-half, half)*(PEpoint->speedx-offx);
//int ddy=offy+hillGrapth(PEpoint->hp-half, half)*(PEpoint->speedy-offy);
//SameSurfaceDisplacement (screen, offx-basicDisplace->w/2 , offy-basicDisplace->h/2, basicDisplace,ddx-basicDisplace->w/2, ddy-basicDisplace->h/2);
//CastLightForDarkenfFunk (shootAnim[PEpoint->type], screen, qdest.x, qdest.y, PEpoint->displayTime);

int fl=rand() % 4;
	CastLightForDarkenfFunk (flameLight[fl], screen, offx-flameLight[fl]->w/2, offy-flameLight[fl]->h/2, 1);

	VolumetricLightBlit(screen, shootAnim[PEpoint->type],offx-shootAnim[PEpoint->type]->w/2, offy-shootAnim[PEpoint->type]->h/2);
}
else
{

CastLightForDarkenfFunk (shootAnim[PEpoint->type+1], screen, qdest.x, qdest.y, PEpoint->displayTime);
//PEpoint->hp=128;
//PEpoint->offx=offx;//+shootAnim[PEpoint->type]->w/2;
//PEpoint->offy=offy;//+shootAnim[PEpoint->type]->h/2;
//CastLightForDarkenfFunk (mergeimage[19], screen, PEpoint->offx-mergeimage[19]->w/2, PEpoint->offy-mergeimage[19]->h/2,2);
//PEpoint->displayTime=5;
//PEpoint->drowing=&LightBlickDrow;




}

}




void addShootEffect(int x,int y,int tx,int ty)
{
createData.tx=tx;
createData.ty=ty;
createData.x=x; 
createData.y=y;
createData.h=player.host->pos.h+accuracy*8;
createData.type=rand() % 7;
createData.mask=0;
createData.gravity=0;
createData.slowing=createData.hp=10;
createData.displayTime=1;
createData.boolFunction=&timedDeath;
createData.voidfunction=&gunshoot;
createData.xaccel=0;
createData.yaccel=0;

createData.light.active=1;
Uint32 intensity=(rand() % 38) + 90;
//curphisobj->light.color=intensity/3+(intensity<<8) + (intensity<<16);
createData.light.color=intensity/3+(intensity<<8) + (intensity<<16);
createData.light.dinamic=1;
createData.light.distanceFading=0.0033;
createData.light.brightness=gunPointBrightness;
//createData.xspeed=dx;
//createData.yspeed=dy;
createData.staticPos=1;
addEffect();

}
void addBlicEffect(int offx, int offy, int type, float distanceFading)
{
createData.offx=offx;
createData.offy=offy;
createData.mask=createData.type=type;
createData.h=createData.h;
createData.hspeed=0;
createData.gravity=0;
createData.slowing=createData.hp=255;
createData.boolFunction=&timedDeath;
createData.voidfunction=&LightBlickDrow;
createData.xaccel=0;
createData.yaccel=0;
createData.xspeed=0;
createData.yspeed=0;
createData.staticPos=1;

createData.light.active=1;
createData.light.color=BoomLight;
createData.light.dinamic=1;
createData.light.distanceFading=distanceFading;//0.0033;
createData.light.brightness=4;
addEffect();
}
int addFireEffect(worldcoordinates* pos, phisicalMe* force, double slowing)
{
//createData.maskIMG=image[1502+(rand() % 5)];
createData.type=(rand() % 4)+1;
createData.displayTime=0; 
createData.tx=pos->tx;
createData.ty=pos->ty;
createData.x=pos->x; 
createData.y=pos->y;
createData.h=pos->h;
createData.hp=7;//1+(rand() % 2);
int ax=accurate(force->speedx), ay=accurate(force->speedy);
if (!ax) ax=1;
if (!ay) ay=1;
int pif=ipifagor (ax, ay);
createData.xspeed=force->speedx+force->speedh/pif*ax;
createData.yspeed=force->speedy+force->speedh/pif*ay;
createData.hspeed=0;
createData.anglex=fireGlobImg->w/2+((rand() % 50) - 25);
createData.angley=fireGlobImg->h/2+((rand() % 50) - 25);
createData.rotationx=-force->speedx/5;
createData.rotationx/=accuracy;
createData.rotationy=-force->speedy/5;
createData.rotationy/=accuracy;
createData.slowing=slowing;
createData.boolFunction=&onHealthFireDecay;
createData.voidfunction=&DrowFire;
createData.gravity=0;

createData.light.active=1;
createData.light.color=fireLightColor;
createData.light.dinamic=1;
createData.light.distanceFading=fireDistFade;
createData.light.brightness=FireBrightnes;
createData.staticPos=0;
return addEffect();
}
void addFireEffectRandom(worldcoordinates* pos, double slowing, int randosity)
{
//createData.maskIMG=image[1502+(rand() % 5)];
	createData.type=(rand() % 4)+1;
	createData.displayTime=0; // use this to make fire dinamic
createData.tx=pos->tx;
createData.ty=pos->ty;
createData.x=pos->x; 
createData.y=pos->y;
createData.h=pos->h;
//int ax=randosity*sphere(500), ay=randosity*radiusy;
//ph[i].force.speedx= xspeed+randosity*sphere(500); 
//ph[i].force.speedy= yspeed+randosity*radiusy;
//if (!ax) ax=1;
//if (!ay) ay=1;
//int pif=ipifagor (ax, ay);
createData.xspeed=randosity*sphere(500);
createData.yspeed=randosity*radiusy;
createData.hspeed=0;
createData.anglex=fireGlobImg->w/2+((rand() % 50) - 25);
createData.angley=fireGlobImg->h/2+((rand() % 50) - 25);
createData.rotationx=createData.xspeed;
createData.rotationx/=accuracy;
createData.rotationy=createData.yspeed;
createData.rotationy/=accuracy;
createData.slowing=slowing;
createData.boolFunction=&onHealthFireDecay;
createData.voidfunction=&DrowFire;
createData.gravity=0;
createData.hp=1+(rand() % 4);
createData.light.active=1;
createData.light.color=fireLightColor;
createData.light.dinamic=1;
createData.light.distanceFading=fireDistFade;
createData.light.brightness=FireBrightnes;
createData.staticPos=0;
smoke[addEffect()].creation=1;
}
void addDisplacementEffect(worldcoordinates* pos){
createData.type=1;
createData.displayTime=0; 
createData.tx=pos->tx;
createData.ty=pos->ty;
createData.x=pos->x; 
createData.y=pos->y;
createData.h=0;
createData.hp=BoomDisplacementHealth;
//int ax=accurate(force->speedx), ay=accurate(force->speedy);
//if (!ax) ax=1;
//if (!ay) ay=1;
//int pif=ipifagor (ax, ay);
createData.xspeed=radiusx (accuracy*2);//force->speedx+force->speedh/pif*ax;
createData.yspeed=radiusy;//force->speedy+force->speedh/pif*ay;
createData.hspeed=0;
createData.xaccel=0;
createData.yaccel=0;
createData.slowing=1;
createData.hspeed=0;
createData.boolFunction=&timedDeath;
createData.voidfunction=&drowNewDisplacement;
createData.gravity=0;

createData.light.active=0;
createData.staticPos=0;
addEffect();
}

int addPseudoAnimatedSmoke(worldcoordinates* pos, phisicalMe* force, double slowing, Uint32* defaultColor, int fadeSpeed, bool big, int MaxLight)
{
	if (big) createData.maskIMG=PseudoAnimBig[rand() % 8];//rand() % 8];
	else
createData.maskIMG=PseudoAnim[rand() % 8];
createData.hp=255*(4+big*1);
createData.tx=pos->tx;
createData.ty=pos->ty;
createData.x=pos->x; 
createData.y=pos->y;
createData.h=pos->h;
createData.g=defaultColor;
//int ax=accurate(force->speedx), ay=accurate(force->speedy);
//if (!ax) ax=1;
//if (!ay) ay=1;
//int pif=ipifagor (ax, ay);
createData.xspeed=force->speedx/40;
createData.yspeed=force->speedy/40;
createData.hspeed=0;
createData.MaxLight=MaxLight;
// displacement for angle copy:
createData.anglex=(rand() % 60) - 30;//fireGlobImg->w/2+((rand() % 50) - 25);
createData.angley=(rand() % 60) - 30;//fireGlobImg->h/2+((rand() % 50) - 25);
createData.angleh=(rand() % 10) + 1 + fadeSpeed; // speed of dissolve
//createData.rotationx=-force->speedx/5;
//createData.rotationx/=accuracy;
//createData.rotationy=-force->speedy/5;
//createData.rotationy/=accuracy;

createData.slowing=0.9;//slowing;
createData.boolFunction=&timedDeath;
createData.voidfunction=(big) ? (&DrowPseudo) : (&DrowPseudoSmall);
createData.gravity=0;

createData.light.active=0;
//createData.light.color=0;
//createData.light.dinamic=1;
//createData.light.distanceFading=0.0053;

createData.staticPos=0;
return addEffect();
}


// world effects

bool treeChop (worldcoordinates* pos, int rad, int damage)
{
	int h=pos->h>>16;
int x=pos->x>>16;
int y=pos->y>>16;
h+=2;
bool killedone=0;
for (int cx=-rad; cx<rad; cx++)
	for (int cy=-rad; cy<rad; cy++)
		if (ipifagor (cx, cy)<rad)
		{
int lmatr=getmematrix (pos->tx, pos->ty, x+cx*4, y+cy*4,h); 
//(matr>=einmatr) 
if ((lmatr<einmatr) && (lmatr>1) && (w[lmatr].brick))
{
//point->x+=-yy*power*direction;
//point->y+=(x-i)*power*direction;

	w[lmatr]._shake.speedx-=((w[lmatr].pos.y+w[lmatr].brick->h/2)*accuracy-pos->y);
	w[lmatr]._shake.speedy+=((w[lmatr].pos.x+w[lmatr].brick->w/2)*accuracy-pos->x);
	w[lmatr].hp-=damage; 
	player.bag.LeftHand.cooldown=10;
	player.bag.RightHand.cooldown=10;
	return 1;
}
/*if ((lmatr>=einmatr) && (lmatr<phinmatr) && (lmatr!=DNA+einmatr))
{
somebody* epo=&e[lmatr-einmatr];
if ((player.host!=epo) || (!GodMode))
{
	epo->hp=-damage;
}
	//epointer->walk.blade.frame-=2;
	//epointer->walk.itemInHand->cooldown+=2;
	//if (oneKill) return 1;
	else killedone=1;
}*/
}
//return killedone;
		return 1;
}



void UpdateFramerateInfo(){
if (!OpenGLscreen)
if ((SDL_GetTicks()-second)>500)  
{
second=SDL_GetTicks();
Fpscount.str(std::string());
Fpscount << "FPS: " << (frames*2);
if (frames<8)
	Fpscount << " <-This is your fault, you have homo porn viruses on your PC, SHAME! ";

Fpscount <<" Effect particles: "<<testCountEfects 
<<" Blood particles: "<<testCountBlood
<<" Phisics particles: "<<testCountPhisics
<<" enemies: "<<testCountEnemies
<<" dustMax: "<<dustArMax
<<" WblowVac: "<<WblowVacant
<<" WblowMax: "<<WblowMax
<<" Lights: "<<lights
<<" Limbs: "<<limbsInUse;



if (inet.onlinemode==1) {Fpscount << " [server]: "<< inet.serv.connections << " connected ";

int clients=0;
for (int i=0; i<inet.serv.connections; i++ ) if (inet.serv.clientConected[i]) clients++;
Fpscount <<clients;
}

if (inet.onlinemode==2) {Fpscount << " [client]: "; if (inet.client.connected) Fpscount<< " connected: " << inet.client.PackageCounter;}
//WblowVacant,WblowMax
//text.str().c_str()
SDL_WM_SetCaption( Fpscount.str().c_str(), NULL );
sumframes=frames*2;
frames=1;
Fpscount.clear();
}
}
void FramerateManagement(){
GameSpeed/= GameSpeedModifier;

bloodness=maxbloodness-(SDL_GetTicks()-start-refrate); 
if (bloodness<5) bloodness=5;
float micsecPF=SDL_GetTicks()-start;

if ((capframerate) && (micsecPF<msPerFrame)) 	
{
	SDL_Delay(msPerFrame-micsecPF-1);
//if (frameIndependentSpeed) 
	GameSpeed=1;
}
else
if (frameIndependentSpeed) 
	//GameSpeed=min(micsecPF/1.3,30);
		GameSpeed=GameSpeed+((min(micsecPF/(float)msPerFrame,3)-GameSpeed)*GameSpeedAdjustmentSpeed);
else
	GameSpeed=1;


if(sloMo)
{
	GameSpeed/=(1+impactslomo);
impactslomo=max(0,impactslomo*slomoFADE);
}
//else GameSpeed/=GameSpeedModifier;

start = SDL_GetTicks();

frames++;
splatPerFrame=1;

GameSpeed*= GameSpeedModifier;
GravCalc=gravity*accuracy*GameSpeed/speedoftime;

}

// Linked Phisics
bool phisCollision (worldcoordinates* pos1, worldcoordinates* pos2, phisicalMe* force1, phisicalMe* force2, int dist, int deBendForce)// rotatableMe angles1
{
	int ixc, ygr;
		//if not deleted
	    ixc=(pos1->tx-pos2->tx)*accuracy+(pos1->x-pos2->x+force1->speedx-force2->speedx)/tyleSize;
		ygr=(pos1->ty-pos2->ty)*accuracy+(pos1->y-pos2->y+force1->speedy-force2->speedy)/tyleSize;
		hashh=(pos1->h-pos2->h+force1->speedh-force2->speedh)/tyleSize;
		double tang=ixc*ixc+ygr*ygr+hashh*hashh;
		tang=sqrt(tang);
		if (tang) 
		{
		int masabla=max((force1->mass+force2->mass),1);
		int curdist=tang-dist*64;//accuracy/tyleSize;
	/*	if (curdist>phisLinkConnectionStrength) return 1;
		if (curdist>phisLinkConnectionStrength/2) // limited force will be applied
		{
curdist=phisLinkConnectionStrength/2*(curdist/abs(curdist));
		}*/

if (curdist>0) return 1;
else
curdist*=deBendForce;

		double fuck=(curdist)/tang;
		calcforcex=fuck*ixc;
		calcforcey=fuck*ygr;
		calcforceh=fuck*hashh;
	
		//	lqobjects++;
		
			//int test=0;
		force1->speedx-=calcforcex*force2->mass/masabla;
		force2->speedx+=calcforcex*force1->mass/masabla;
			
		
		force1->speedy-=calcforcey*force2->mass/masabla;
		force2->speedy+=calcforcey*force1->mass/masabla;

				masabla*=2;
		force1->speedh-=calcforceh*force2->mass/masabla;
		force2->speedh+=calcforceh*force1->mass/masabla;

		}
		return 0;//tang*tyleSize/accuracy;
}

bool phisLink (worldcoordinates* pos1, worldcoordinates* pos2, phisicalMe* force1, phisicalMe* force2, float dist, int deStretchForce, int deBendForce)// rotatableMe angles1
{
	int ixc, ygr;
		//if not deleted
	    ixc=(pos1->tx-pos2->tx)*accuracy+(pos1->x-pos2->x+force1->speedx-force2->speedx)/tyleSize;
		ygr=(pos1->ty-pos2->ty)*accuracy+(pos1->y-pos2->y+force1->speedy-force2->speedy)/tyleSize;
		hashh=(pos1->h-pos2->h+force1->speedh-force2->speedh)/tyleSize;
		double tang=ixc*ixc+ygr*ygr+hashh*hashh;
		if (tang<0) return 0;//tang=100;
		tang=sqrt(tang);
		
		if (tang) 
		{
		int masabla=(force1->mass+force2->mass+1);
		int curdist=tang-dist*64;//accuracy/tyleSize;
		//if (curdist>phisLinkConnectionStrength) return 1;
/*if (curdist>phisLinkConnectionStrength/2) // limited force will be applied
		{
curdist=phisLinkConnectionStrength/2*(curdist/abs(curdist));
		}*/

if (curdist>0) curdist*=deStretchForce;
else
curdist*=deBendForce;

		double fuck=(curdist)/tang;
		calcforcex=fuck*ixc;
		calcforcey=fuck*ygr;
		calcforceh=fuck*hashh;
	
		//	lqobjects++;
		
			//int test=0;
		/*int temp=calcforcex*force1->mass/masabla;
		force1->speedx-=temp*(abs(temp)<(temptest*temptest));
		temp=calcforcex*force2->mass/masabla;
		force2->speedx+=temp*(abs(temp)<(temptest*temptest));*/
		
		force1->speedx-=calcforcex*force2->mass/masabla;
		force2->speedx+=calcforcex*force1->mass/masabla;

		/*temp=calcforcey*force1->mass/masabla;
		force1->speedy-=temp*(abs(temp)<(temptest*temptest));
		temp=calcforcey*force2->mass/masabla;
		force2->speedy+=temp*(abs(temp)<(temptest*temptest));*/
		
		force1->speedy-=calcforcey*force2->mass/masabla;
		force2->speedy+=calcforcey*force1->mass/masabla;

		force1->speedh-=calcforceh*force2->mass/masabla;
		force2->speedh+=calcforceh*force1->mass/masabla;

		}
		return 0;//tang*tyleSize/accuracy;
}
bool phisLink2D (worldcoordinates* pos1, worldcoordinates* pos2, phisicalMe* force1, phisicalMe* force2, float dist, int deStretchForce, int deBendForce)// rotatableMe angles1
{
	int ixc, ygr;
		//if not deleted
	    ixc=(pos1->tx-pos2->tx)*accuracy+(pos1->x-pos2->x+force1->speedx-force2->speedx)/tyleSize;
		ygr=(pos1->ty-pos2->ty)*accuracy+(pos1->y-pos2->y+force1->speedy-force2->speedy)/tyleSize;
		double tang=ixc*ixc+ygr*ygr;
		if (tang<0) return 0;
		tang=sqrt(tang);
		
		if (tang) 
		{
		int masabla=(force1->mass+force2->mass+1);
		int curdist=tang-dist*64;

if (curdist>0) curdist*=deStretchForce;
else
curdist*=deBendForce;

		double fuck=(curdist)/tang;
		calcforcex=fuck*ixc;
		calcforcey=fuck*ygr;
		
		force1->speedx-=calcforcex*force2->mass/masabla;
		force2->speedx+=calcforcex*force1->mass/masabla;
		
		force1->speedy-=calcforcey*force2->mass/masabla;
		force2->speedy+=calcforcey*force1->mass/masabla;

		}
		return 0;
}


int loadImageLineUp(SDL_Surface** arrayPoint, char * tytle , int i)
{
std::stringstream ss;
 std::string strx;
 
 do
 {
//*arrayPoint=0;
 ss<<tytle<<i<<".png";
 ss>>strx;
 *arrayPoint=loadany(strx);
arrayPoint++;
i++;
ss.clear();
	strx.clear();
 } while (*(arrayPoint-1));
 return i-2;
}

SDL_Surface* CopyIntoNewTexture(SDL_Surface* source, int x, int y, int w, int h)
{
	SDL_Surface* tempsurf=SDL_CreateRGBSurface(0, w, h, 32,0,0,0,0);
	Uint32* dest=(Uint32*)tempsurf->pixels;
	Uint32* sour=(Uint32*)source->pixels+y*source->w+x;
	int mx=min(w,source->w-x);
	int my=min(h,source->h-y);
	int displ=source->w-mx;
	for (int j=0; j<my;)
	{
		for (int i=0; i<mx;)
		{
			*dest=*sour;
			dest++;
			sour++;
			i++;
		}
		sour+=displ;
		j++;

	}

return tempsurf;

}

void TreeShake(){
	if (!curwall) return;
dddcoords * accel=GetWindGlobal (offx+32, offy+32);
curwall->_shake.speedx+=accel->x/curwall->_shake.mass;
curwall->_shake.speedy+=accel->y/curwall->_shake.mass;

curwall->_shake.speedx*=wind.treeslowing;
curwall->_shake.speedy*=wind.treeslowing;

curwall->_shake.speedx-=curwall->_shake.offx/wind.treeBendDelay;
curwall->_shake.speedy-=curwall->_shake.offy/wind.treeBendDelay;

int temp=0;
speedLimitCheckI(&curwall->_shake.speedx, &curwall->_shake.speedy, &temp, accuracy*5);
speedLimitCheckI(&curwall->_shake.offx, &curwall->_shake.offy, &temp, accuracy*90);
curwall->_shake.offx+=curwall->_shake.speedx;
curwall->_shake.offy+=curwall->_shake.speedy;
}

void FillArrayWithEnemy(somebody* smb){
worldcoordinates* pos=&smb->pos;
int size=smb->size;
int hs=smb->size/2;
shortx=pos->x>>16;
shorty=pos->y>>16;
shorth=pos->h>>16;

for (int w=-hs; w<size-hs; w++)
	for (int h=-hs; h<size-hs; h++)
		for (int tall=0; tall<smb->height; tall++)		
	TryFillrmatrix (pos->tx, pos->ty, shortx+w*pixInPhis, shorty+h*pixInPhis, shorth+tall*pixInPhis, smb->i+einmatr);
	
}

void ClearArrayFromEnemy(somebody* smb){
worldcoordinates* pos=&smb->pos;
int size=smb->size;
int hs=smb->size/2;
shortx=pos->x>>16;
shorty=pos->y>>16;
shorth=pos->h>>16;

for (int w=-hs; w<size-hs; w++)
	for (int h=-hs; h<size-hs; h++)
		for (int tall=0; tall<smb->height; tall++)		
	TryClearmatrix (pos->tx, pos->ty, shortx+(w<<3), shorty+(h<<3), shorth+(tall<<3), smb->i+einmatr);

}

void FillZoomDisplaceArray (int height){

double elevation=height;
elevation/=dddstength;
elevation/=255;

float* madkp=ZoomDisplacementMaskX;

for (int i=0; i<scrwi; i++){
	*madkp=-((double)(i-scrwid)*elevation); 
	madkp++;
	}

madkp=ZoomDisplacementMaskY;

for (int i=0; i<scrhi; i++){
	*madkp=-((double)(i-scrhid)*elevation); 
	madkp++;
	}


}

void BlitZoomBGToScreenDisplacement(){

float* madkpY=ZoomDisplacementMaskY;
Uint32* screenPix=(Uint32*)screen->pixels; 
Uint32* ZBGPix=(Uint32*)ZoomBG->pixels; 
for (int j=0; j<scrhi; j++){
	float* madkpX=ZoomDisplacementMaskX;
	for (int i=0; i<scrwi; i++){

		int AH=*ZBGPix>>24;

		if (AH<223){
		AH=223-AH;
		AH=((int)(j+*madkpY*AH))*scrwi + i + (int)((*madkpX)*AH);
		} else AH=j*scrwi+i;
		*screenPix=*((Uint32*)ZoomBG->pixels + AH);
			
		madkpX++;
		screenPix++;
		ZBGPix++;
	}

	madkpY++;
}

}

SDL_Surface* TextToImage(string someshit, int value, string somemoreshit)
{
SDL_Surface* message;
std::stringstream text;
text<<someshit<<value<<somemoreshit;
message = TTF_RenderText_Solid( font, text.str().c_str(), textColor ); 
//apply_surface( 80, 50*no, message, screen );
return message;
}
SDL_Surface* TextOnlyToImage(string someshit)
{
SDL_Surface* message;
std::stringstream text;
text<<someshit;
message = TTF_RenderText_Solid( font, text.str().c_str(), textColor ); 
//apply_surface( 80, 50*no, message, screen );
return message;
}
void DrowUpdatableValue(TunableDisplayValue* tdv, int x, int y, string info){
	if ((tdv->renderedVal!=tdv->val) || (!tdv->textIMG)){
		if (tdv->textIMG) SDL_FreeSurface(tdv->textIMG);
		tdv->textIMG=TextToImage(info,tdv->val, ";");
		tdv->renderedVal=tdv->val;
}
sdlanyblit (x, y, tdv->textIMG, screen);

}

void DrowMyIndShadow (MyIndexedAlphaSurface* image, int x, int y)
{
if ((x>screen->w) || (x+image->w<0) || (y>screen->h) || (y+image->h<0)) return;

fillDarkMaskGrid(x, y, image->w, image->h);

Uint8 * __restrict  src=image->pixels;
Uint32 * __restrict mask;

int gap;
int fill, skip;
int total=0;
int i;
for (i=0; i<image->pixCount-1;)
{

gap=*src;
src++; i++;
int left=fill=*src;
src++; i++;
total+=gap;
////// Border calculation:
while (left)
{
int imgy=total/image->w;
if (imgy+y>=screen->h) return; 

int imgx=total % image->w;
int drow=min(left, image->w-imgx);

if (imgy<-y) 
	skip=drow;
else
{
skip=0;

if (imgx+x>=screen->w) skip=drow;
else
{
if (imgx+drow+x>screen->w) drow=(screen->w-imgx-x);
if (imgx<-x) skip=min(-(imgx+x),drow);
}
}
if (skip)
{
	 drow-=skip; i+=skip; src+=skip; left-=skip; total+=skip; imgx+=skip;
}

//scrpix=(Uint32*)screen->pixels + (imgy+y)*screen->w+imgx+x;
//dcpix=(Uint32*)Decay->pixels + (imgy+srcy)*Decay->w+imgx+srcx;
mask=darkmask+(imgy+y)*screen->w+imgx+x;
left-=drow;
total+=drow;
//int j=drow;
//while (j)
i+=drow;
for (int j=0; j<drow;)
{
*mask+=*src;// * a);
	
	mask++;
	src++;
	j++;
}

}
}

}

void DrowMyIndShadowA (MyIndexedAlphaSurface* image, int x, int y,  float alpha)
{
if ((x>screen->w) || (x+image->w<0) || (y>screen->h) || (y+image->h<0)) return;

fillDarkMaskGrid(x, y, image->w, image->h);

Uint8 * __restrict  src=image->pixels;
Uint32 * __restrict mask;

int gap;
int fill, skip;
int total=0;
int i;
for (i=0; i<image->pixCount-1;)
{

gap=*src;
src++; i++;
int left=fill=*src;
src++; i++;
total+=gap;
////// Border calculation:
while (left)
{
int imgy=total/image->w;
if (imgy+y>=screen->h) return; 

int imgx=total % image->w;
int drow=min(left, image->w-imgx);

if (imgy<-y) 
	skip=drow;
else
{
skip=0;

if (imgx+x>=screen->w) skip=drow;
else
{
if (imgx+drow+x>screen->w) drow=(screen->w-imgx-x);
if (imgx<-x) skip=min(-(imgx+x),drow);
}
}
if (skip)
{
	 drow-=skip; i+=skip; src+=skip; left-=skip; total+=skip; imgx+=skip;
}

//scrpix=(Uint32*)screen->pixels + (imgy+y)*screen->w+imgx+x;
//dcpix=(Uint32*)Decay->pixels + (imgy+srcy)*Decay->w+imgx+srcx;
mask=darkmask+(imgy+y)*screen->w+imgx+x;
left-=drow;
total+=drow;
//int j=drow;
//while (j)
i+=drow;
for (int j=0; j<drow;)
{
*mask+=*src*alpha;
	
	mask++;
	src++;
	j++;
}

}
}

}
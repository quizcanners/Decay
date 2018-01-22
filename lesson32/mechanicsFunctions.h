struct RGBfloat
{
float R, G, B;
}frgb;

RGBfloat thrFrgb;
std::string intTotext(int n)
{
std::string strx;
std::stringstream text;
text<<n;
text>>strx;
return strx;
}


void drowsquareFill(int x,int y,int w,int h, Uint32 col)
{
	SDL_Rect temp;
	temp.x=x;
	temp.y=y;
	temp.w=w;
	temp.h=h;
	SDL_FillRect(screen,&temp, col); 
}

void colorToFloats(Uint32 color)
{
	int temp=((color>>16) & BlueMask);
	if (temp>frgb.R) frgb.R=temp;
	temp=((color>>8) & BlueMask);
	if (temp>frgb.G) frgb.G=temp;
	temp=(color & BlueMask);
	if (temp>frgb.B) frgb.B=temp;
};

void brightenBlueChannel(Uint32 * ar, Uint32 msk)
{
Uint32 dst=*ar;
Uint32 tmp=(*ar & RedMask)+(msk<<16);
*ar = (tmp<RedMask ? tmp : RedMask) | ((256-msk)<<24); // access vioation writing to ar
 tmp=(dst & GreenMask)+(msk<<8);
*ar|= (tmp<GreenMask ? tmp : GreenMask);//(min(tmp, 0x0000FF00));
 tmp=(dst & BlueMask)+(msk);
*ar|= (tmp<BlueMask ? tmp : BlueMask);
}
void brightenColor(Uint32 * ar, Uint32 msk)
{
Uint32 dst=*ar;
Uint32 tmp=(*ar & RedMask)+(msk<<16);
*ar = (tmp<RedMask ? tmp : RedMask) | ((256-msk)<<24); // access vioation writing to ar
 tmp=(dst & GreenMask)+(msk<<8);
*ar|= (tmp<GreenMask ? tmp : GreenMask);//(min(tmp, 0x0000FF00));
 tmp=(dst & BlueMask)+(msk/2);
*ar|= (tmp<BlueMask ? tmp : BlueMask);
}
void MultiplyFloatColorSingle(Uint32* dest)
{
Uint32 dst=*dest & 0x00FFFFFF;
*dest&=0xFF000000;
Uint32 temp=OnelightR*(dst>>16);
*dest|=((temp<0xFF00) ? (temp<<8) : 0xFF0000) & 0xFF0000;

temp=OnelightG*(dst & 0xFF00);
*dest|=((temp<0xFF0000) ? (temp>>8) : 0xFF00) & 0xFF00;

temp=OnelightB*(dst & 0xFF);
*dest|=((temp<0xFF00) ? (temp>>8) : 0xFF);


}

void ThreadGetGreyTint(int i ,Uint32* dest)
{
//Uint32 dst=*dest & 0x00FFFFFF;
//*dest&=0xFF000000;
Uint32 temp=thrL.lightsR[i]/2;
*dest=((temp<0xF0) ? (temp<<16) : 0xF00000);

temp=thrL.lightsG[i]/2;
*dest|=((temp<0xF0) ? (temp<<8) : 0xF000);

temp=thrL.lightsB[i]/2;
*dest|=((temp<0xF0) ? (temp) : 0xF0);


}
void ThreadMultiplyFloatColor(int i ,Uint32* dest)
{
Uint32 dst=*dest & 0x00FFFFFF;
*dest&=0xFF000000;
Uint32 temp=thrL.lightsR[i]*(dst>>16)+ambientR;
*dest|=((temp<0xF000) ? (temp<<8) : 0xF00000) & 0xFF0000;

temp=thrL.lightsG[i]*(dst & 0xFF00)+ambientG;
*dest|=((temp<0xF00000) ? (temp>>8) : 0xF000) & 0xFF00;

temp=thrL.lightsB[i]*(dst & 0xFF)+ambientB;
*dest|=((temp<0xF000) ? (temp>>8) : 0xF0);


}
void MultiplyFloatColor(int i ,Uint32* dest)
{
Uint32 dst=*dest & 0x00FFFFFF;
*dest&=0xFF000000;
Uint32 temp=lightsR[i]*(dst>>16)+ambientR;
*dest|=((temp<0xF000) ? (temp<<8) : 0xF00000) & 0xFF0000;

temp=lightsG[i]*(dst & 0xFF00)+ambientG;
*dest|=((temp<0xF00000) ? (temp>>8) : 0xF000) & 0xFF00;

temp=lightsB[i]*(dst & 0xFF)+ambientB;
*dest|=((temp<0xF000) ? (temp>>8) : 0xF0);


}
void MultiplyColor1(Uint32 source, Uint32* dest)
{
 Uint8* __restrict src;
src=(Uint8*)&source; 
Uint8* __restrict dst=(Uint8*)dest; 
Uint32 sum=(Uint32)(*src**dst)>>6;
*dst=sum | (0xFF * (sum>>8));//min(((Uint32)(*src**dst)>>6),255); 
dst++; src++;
sum=(Uint32)(*src**dst)>>6;
*dst=sum | (0xFF * (sum>>8));//min(((Uint32)(*src**dst)>>6),255); 
dst++; src++;
sum=(Uint32)(*src**dst)>>6;
*dst=sum | (0xFF * (sum>>8));//min(((Uint32)(*src**dst)>>6),255);
	/*
Uint32 l=*dest;
*dest=((((l & 0xFF)*((source & 0xFF)>>6))) & 0xFF) | 
(((((l>>8) & 0xFF)*((source>>8) & 0xFF))<<2) & 0xFF00) | 
(((((l>>16) & 0xFF)*((source>>16) & 0xFF))<<10) & 0xFF0000);*/
/*
((((l & 0xFF)/16)*((source & 0xFF)/16)) & 0xFF) | 
((((l & 0xFF00)/16)*((source & 0xFF00)/16)) & 0xFF00) | 
((((l & 0xFF0000)/16)*((source & 0xFF0000)/16)) & 0xFF0000);
*/
}
void MultiplyBy(int msk, Uint32* ar)
{
Uint32 dst=*ar;

Uint32 tmp=(((dst & RedMask)>>8)*(msk));
*ar = (tmp<RedMask ? (tmp & RedMask) : RedMask); 
tmp=((dst & GreenMask)*(msk))>>8;
*ar|= (tmp<GreenMask ? (tmp & GreenMask) : GreenMask);
 tmp=((dst & BlueMask)*(msk))>>8;
*ar|= (tmp<BlueMask ? tmp   : BlueMask);
*ar|=(dst & AlphaMask);
}
void FloatsToColors (Uint32* color)
{
	Uint32 temp=frgb.R; temp<<=16;
	if (temp>(*color & RedMask)) *color=(*color & deRedMask) | temp;
	temp=frgb.G; temp<<=8;
	if (temp>(*color & GreenMask)) *color=(*color & deGreenMask) | temp;
	temp=frgb.B; 
	if (temp>(*color & BlueMask)) *color=(*color & deBlueMask) | temp;

}
void AvgColor(Uint32 source, Uint32* dest)
{
 Uint8* __restrict src;
src=(Uint8*)&source; 
Uint8* __restrict dst=(Uint8*)dest; 

*dst=((int)*src+*dst)/2; 
dst++; src++;

*dst=((int)*src+*dst)/2; 
dst++; src++;

*dst=((int)*src+*dst)/2; 

}

void fitRectangleToSurfaceCoords (SDL_Rect* rect, SDL_Surface* screen, int offx, int offy)
{
	rect->x+=-offx*(offx<0);
	rect->y+=-offy*(offy<0);
	if (offx+rect->w>screen->w) rect->w=screen->w-offx; 
	if (offy+rect->h>screen->h) rect->h=screen->h-offy; 
}
inline int accurate (int k)
{
return (int)(k>>16);
}
void GetWind(dddcoords* accel, int offx, int offy, int h)
{
h=accurate(h);
float portion=(heightWindLessing-h);
portion/=heightWindLessing;
if ((offx<scrwi) && (offy<scrhi) && (offy>=0) && (offx>=0) && (portion<=1) && (portion>0))
{
*accel=wind.mask[offy/8*wind.w+offx/8];
//wind.mask[offy/8*wind.w+offx/8];
}
else
*accel=wind.global;

accel->x*=portion;
accel->y*=portion;
accel->h*=portion;
}

void ShapeRectToScreen( int w, int h, SDL_Rect* rct, SDL_Surface* screen, int offx, int offy)
{
rct->x=max(0,-offx); 
rct->y=max(0,-offy);
rct->w=min(w,max(scrwi-offx,0));
rct->h=min(h,max(scrhi-offy,0));
}
inline void E8bitBufFill(Uint32* BufPic, int heg, Uint32 gp)
{

int diff=(int)(*BufPic & 0xFFFF)-(int)heg;

*BufPic= (diff>0) ? ((gp) | heg | ((diff>1) ? 0 : (*BufPic & 0xFF000000) )) : *BufPic;

}
inline void E8bitBufFillSmooth(Uint32* BufPic, int heg, Uint32 gp)
{

int diff=(int)(*BufPic & 0xFFFF)-(int)heg;

*BufPic= (diff>-8) ? ((gp) | heg | ((diff>8) ? 0 : (*BufPic & 0xFF000000) )) : *BufPic;

}
inline void E8bitBufFillMega(Uint32* BufPic, int heg, Uint32 gp)
{
	
int diff=(int)(*BufPic & 0xFFFF)-(int)heg;
if (diff>=0)
*BufPic= gp | heg | ((diff) ? 0 : (*BufPic & 0xFF000000));

}

void E8StaticBitBufFill(Uint32* BufPic, int heg, Uint32 gp)
{	
int diff=(int)(*BufPic & 0xFFFF)-(int)heg;
*BufPic= (gp) | heg | ((diff<=8)*(*BufPic & 0xFF000000));
}
void RegularBlit(Uint32* destpic, Uint32 color, int AH)
{
register int idst = *destpic;
register int isrc = color;
*destpic = ((RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * AH) >>8))) | 
	(GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * AH) >>8))) |
	(BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * AH) >>8))))
	| (AlphaMask & *destpic);
//*((Uint8 * )destpic+3)=*((Uint8*)hpic+3)-h;	
}
void htpBlit(Uint32*scrpx, Uint32 color)
{
	register int sp=*scrpx;
	*scrpx=((((sp & RedMask) + (color & RedMask))>>1) & RedMask) | 
	((((sp & GreenMask) + (color & GreenMask))>>1)& GreenMask) |
	(((sp & BlueMask) + (color & BlueMask))>>1) | (*scrpx & 0xFF000000);
}
void htpBlitToUpper(Uint32*scrpx, Uint32 color)
{
	int a=*scrpx>>24;
	if (a<16){
	*scrpx=(color & 0x00FFFFFF) | 0x77000000;
	return;
	}
	a=a/2;
	RegularBlit(scrpx,  color, a);
	*scrpx=(*scrpx & 0x00FFFFFF) | ((a+255/2)<<24);
	//register int sp=*scrpx;
	//*scrpx=((((sp & RedMask) + (color & RedMask))>>1) & RedMask) | 
	//((((sp & GreenMask) + (color & GreenMask))>>1)& GreenMask) |
	//(((sp & BlueMask) + (color & BlueMask))>>1) | (*scrpx & 0xFF000000);
}
void MaxColor(Uint32 source, Uint32* dest)
{
register Uint32 l=*dest;
*dest=max(l & 0xFF,source & 0xFF) | 
max(l & 0xFF00,source & 0xFF00) | 
max(l & 0xFF0000,source & 0xFF0000) |
(l & 0xFF000000);
}
void AddColor(Uint32 source, Uint32* dest)
{
//register Uint32 l=*dest;

*dest+=source;

/*
*dest=(((l & 0xFF)+(source & 0xFF)) & 0xFF) | 
(((l & 0xFF00)+(source & 0xFF00)) & 0xFF00) | 
(((l & 0xFF0000)+(source & 0xFF0000)) & 0xFF0000); */
/*
l&=0x00F0F0F0;
l>>=4;
source&=0x00F0F0F0;
source>>=4;
*dest=l*source;*/
/*
*dest=((((l & 0xFF)>>4)*((source & 0xFF)>>4)) & 0xFF) | 
((((l & 0xFF00)>>4)*((source & 0xFF00)>>4)) & 0xFF00) | 
((((l & 0xFF0000)>>4)*((source & 0xFF0000)>>4)) & 0xFF0000);
*/
}

int SoftUpperBlit (int diff, Uint32*updestpic, Uint32*upsourpic, Uint32* destpic, Uint32 * sourpic , int h){
int AH=0;
	if (diff<16)
		{

 AH=diff*16;

int MaxPercent=min((223-h)*8,255);

 AH=AH*max(MaxPercent, (*sourpic>>24))/255;




*((Uint8 * )updestpic+3) = (((255 -  *((Uint8 * )updestpic+3)) * (AH))/255 + *((Uint8 * )updestpic+3));
RegularBlit(updestpic, *upsourpic,  AH);
RegularBlit(destpic, *sourpic,  AH);



		}
		else		{
			AH=255;
*updestpic=*upsourpic;
*destpic=(*sourpic & 0xFFFFFF) | (*destpic & 0xFF000000);
//if (AH<64) AH=127+(AH-32)*4;
		}
		

return AH;
}


void DoubleAlphaBlit(Uint32* destpic, Uint32 color, int AH, int antiHH)
{
	int full=AH+antiHH;
register int idst = *destpic;
register int isrc = color;
*destpic = (RedMask & (((idst & RedMask) * antiHH + (isrc & RedMask) * AH) /full)) | 
	(GreenMask & (((idst & GreenMask) * antiHH + (isrc & GreenMask) * AH) /full)) |
	(BlueMask & (((idst & BlueMask) * antiHH + (isrc & BlueMask) * AH) /full)) |
	//(GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * AH) >>8))) |
	//(BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * AH) >>8))))
	 (AlphaMask & *destpic);
//*((Uint8 * )destpic+3)=*((Uint8*)hpic+3)-h;	
}

Uint32 MaxColorFadeDest(Uint32 still, Uint32 away, float fadeness)
{
Uint32 maxi;
maxi=max(still & 0xFF,(Uint32)((away & 0xFF)*fadeness) & 0xFF) | 
	max(still & 0xFF00,(Uint32)((away & 0xFF00) *fadeness) & 0xFF00) | 
	max(still & 0xFF0000,(Uint32)((away & 0xFF0000)*fadeness) & 0xFF0000);
return maxi;
}


void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination ) 
{
	SDL_Rect qdest;
	qdest.x=x;
	qdest.y=y;
	SDL_BlitSurface(source, NULL, destination, &qdest);
}

void UpdateTextImage(SDL_Surface * image, string NewText )
{
std::stringstream text;
text<<NewText;
if (image) SDL_FreeSurface(image);
image = TTF_RenderText_Solid( font, text.str().c_str(), textColor ); 
//apply_surface( x, y, message, screen );
//SDL_FreeSurface( message );
}

void optimizeCoordinates(worldcoordinates * pos)
{
	if (pos->x>unitsIntyle) {pos->x-=unitsIntyle; pos->tx++;}
if (pos->x<0) {pos->x+=unitsIntyle; pos->tx--;}
if (pos->y>unitsIntyle) {pos->y-=unitsIntyle; pos->ty++;}
if (pos->y<0) {pos->y+=unitsIntyle; pos->ty--;}
}

int pulseGrapth(int x, int max)
{
int qm=max/4;
int rx=-x+2*(x-qm)*(x>qm)-2*(x-qm*3)*(x>qm*3);

//if (x)
//	x=x-max/2;
//	x*=2*(x>0);
	return rx;
}
double hillGrapth(double x, int max)//This returns percent, not the value
{
	x=max/2-abs(x-max/2);
	x/=(max/2);
	//x*=2*(x>0);
	return x;
}
Uint32 PixOnSurface(int offx, int offy)
{
if ((offx<0) || (offx>=scrwi) || (offy<0) || (offy>=scrhi)) return 0;
//(*(darkmask+offy*scrwi+offx)>>16)
return *((Uint32*)screen->pixels + offy*scrwi + offx);

}
bool OnScreen(int offx, int offy, int w, int h)
{
if ((offx+w/2<0) || (offx-w/2>scrwi) || (offy+h/2<0) || (offy-h/2>scrhi)) return 0;
return 1;
}
bool PosOnScreen(worldcoordinates* pos, int rad)
{
int offx=(pos->x>>16) - actcamx + ((pos->tx - cameratylx)<<10);
int offy=(pos->y>>16) - actcamy + ((pos->ty - cameratyly)<<10);
if ((offx+rad>0) && (offy+rad>0) && (offx-rad<scrwi) && (offy-rad<scrhi))
return 1;
return 0;
}
bool FullyOnScreen(int offx, int offy, float w, float h)
{
if ((offx-w/2<0) || (offx+w/2>=scrwi) || (offy-h/2<0) || (offy+h/2>=scrhi)) return 0;
return 1;
}
bool coordsInRectum(SDL_Rect *r, int x, int y)
{
	if ((r->x<=x) && (r->w>x) && (r->y<=y) && (r->h>y)) return 1;
	return 0;
}
bool mouseInSquere(int x, int y, int w, int h){
if ((x<mouse.pos.x) && (x+w>mouse.pos.x) &&
		(y<mouse.pos.y) && (y+h>mouse.pos.y)) return 1;
return 0;
}

void NormalizeAngle(float* a){
while (*a<0) *a+=M_PI*2;
while (*a>=M_PI*2) *a-=M_PI*2;

}
double getAngle(double x, double y)
{
/*if (!x) x++;
double ah = y;
ah=atan(double(ah/x));

if (x<0) ah+=M_PI;
else if (y<0) ah+=M_PI*2;
return ah;*/
	return (atan2(x,-y)-M_PI/2);
}
double getAngleFromPos(worldcoordinates* pos1, worldcoordinates* pos2)
{
	double x=((pos1->tx-pos2->tx)<<14)+((pos1->x-pos2->x)>>12);
	double y=((pos1->ty-pos2->ty)<<14)+((pos1->y-pos2->y)>>12);

	return (atan2(x,-y)-M_PI/2);
}
int getSector(int x, int y, int ArSize)
{

double l=getAngle(x, y)*ArSize;
l/=(M_PI*2);
l=l-((int)l/ArSize)*ArSize;
if (l<0) l=ArSize+l;

return l;
}
int getGradus(int x, int y)
{
int grad=getAngle(x, y)*180/M_PI;
grad=grad-(grad/360)*360;
if (grad<0) grad=360+grad;
//if ((grad<0) || (grad>=180)) 
	//grad=1;
return grad;
}
int ConvertVectorTo360(int x, int y)
{
double a=getAngle(x, y);
a=a*180/M_PI;
a=a-((int)a/360)*360;
if (a<=0) a+=360;
return a;
}

int GetAngleDistance(int sourceA, int targetA)
{
int a = targetA - sourceA;
a -= 360*(a > 180);
a += 360*(a < -180);
return abs(a);
}

float GetFloatAngleDistance(float sourceA, float targetA)
{
float a = targetA - sourceA;
a -= 2*M_PI*(a > M_PI);
a += 2*M_PI*(a < -M_PI);
return abs(a);
}

float GetFloatAngleDistanceNegPos(float sourceA, float targetA)
{
float a = targetA - sourceA;
a -= 2*M_PI*(a > M_PI);
a += 2*M_PI*(a < -M_PI);
return a;
}

float GetAngleDifferance(float sourceA, float targetA)
{
float a = targetA - sourceA;
a -= 2*M_PI*(a > M_PI);
a += 2*M_PI*(a < -M_PI);
return a;
}

Uint32 myRandom()
{
    world.rx = 36969 * (world.rx & 65535) + (world.rx >> 16);
    world.ry = 18000 * (world.ry & 65535) + (world.ry >> 16);
    return (world.rx << 16) + world.ry;
}
inline int GetRandomNegPos(int randosity, int multiplier)
{
	return ((rand() % randosity) - randosity/2)*multiplier;
}

double GetRandomDoubleFrom(double speed)
{
return	((rand() % 255)*speed-(speed*128))/128;
}
double GetRandomRotation(double speed)
{
return	(rand() % 20)*speed-(speed*10);
}
double MyRandomRotation(double speed)
{
return	(myRandom() % 20)*speed-(speed*10);
}

inline int tylextogrid(int xx)
{
//int
xx-=grid.objectx[0][0];
//xx=x-xx;
//if (abs(xx)<5)
	return (abs(xx)<5)*(xx+5*(xx<0));
//if (xx<0) return (5+xx); else return xx;
//else return 0;
}
inline int tyleytogrid(int yy)
{
//int 
yy=yy - grid.objecty[0][0];
//yy=y-yy;
//if ((yy>-5)&&(yy<5))
//if (yy<0) return (5+yy); else return yy;
//else return 0;
return (abs(yy)<5)*(yy+5*(yy<0));
}

void PosToGrid(int* __restrict gx, int* __restrict gy, int* __restrict x, int* __restrict y, worldcoordinates* pos)
{
	*gx=pos->tx;
	*gy=pos->ty;
	*x=accurate(pos->x);
	*y=accurate(pos->y);
	bool fix=1;
while (fix)
{
	fix=0;
if (*x<0) {x+=tyleSize; *gx--; fix=1;}
if (*x>=tyleSize) {	*x-=tyleSize; *gx++;fix=1;	}


if (*y<0) {y+=tyleSize; *gy--; fix=1;}
if (*y>=tyleSize) {	*y-=tyleSize; *gy++;	 fix=1;}
}
	*gx=tylextogrid(*gx);
	*gy=tyleytogrid(*gy);

}

int radiusx (int radius)
{
theta=(rand() % 628);
theta/=100;
radd=(radius*(rand() % 100))/100;
radiusy=radd*cos(theta);
return radd*sin(theta);
}
int sphere (int radius)
{
	unsigned long rada=1,radb;
	int b=0;
	//radb=0;
	radb=radius*radius;
	while (((rada)>(radb)) || (b==0))
	{
	
		radiusxx=(radius*(rand() % 1028))/512-radius;
		radiusy=(radius*(rand() % 1028))/512-radius;
		radiush=(radius*(rand() % 1028))/512-radius;
		b++;
		rada=radiusxx*radiusxx+radiusy*radiusy+radiush*radiush;
		
	}
return radiusxx;
}
void shakeMeBaby(int power)
{
shake.spx=shake.spx*shake.antishake+radiusx(power);
shake.spy=shake.spx*shake.antishake+radiusy;
	shake.antishake/=2;
	shake.shaker=min(1,shake.shaker+0.4);

	shake.x*=shake.antishake;
	shake.y*=shake.antishake;

}

int katet (int rx, int ry, int max)
{
	// Get Radial
				//int rx=rand()%50,ry=rand()%50,max=70;
				
		//fuck=(float)rx*(float)rx+(float)ry*(float)ry;
		fuck=rx*rx+ry*ry;
				fuck=sqrt(fuck);
				if ((fuck==0) || (fuck<=max)) return ry;
				else return 0;
				//ry/=fuck*max;
				//Get Radial

				return ry;
}
long double JustKatet (int rx, int ry)
{
	// Get Radial
				//int rx=rand()%50,ry=rand()%50,max=70;
				
		//fuck=(float)rx*(float)rx+(float)ry*(float)ry;
		fuck=rx*rx+ry*ry;
		fuck=sqrt(fuck);
			

	return fuck;
}
void katet3d (int rx, int ry, int rh)
{
	rx=rx>>16;
	ry=ry>>16;
	rh=rh>>16;
		fuck=rx*rx+ry*ry+rh*rh;
				fuck=sqrt(fuck);
				
}
void katet3dD (double rx, double ry, double rh)
{
	//rx=rx>>16;
//	ry=ry>>16;
	//rh=rh>>16;
		fuck=rx*rx+ry*ry+rh*rh;
				fuck=sqrt(fuck);
				
}
void dddistance (int rx, int ry, int rh)
{
	//rx=rx>>16;
	//ry=ry>>16;
	//rh=rh>>16;
		fuck=rx*rx+ry*ry+rh*rh;
				fuck=sqrt(fuck);
				
}
double pifagor (float rx, float ry)
{
	
	// Get Radial
				//int rx=rand()%50,ry=rand()%50,max=70;
				
		//fuck=(float)rx*(float)rx+(float)ry*(float)ry;
		fuck=rx*rx+ry*ry;
				fuck=sqrt(fuck);
				//if ((fuck==0) || (fuck<=max)) return ry;
				//else return 0;
				//ry/=fuck*max;
				//Get Radial

				return fuck;

}
long double ipifagor (int rx, int ry)
{
	
	// Get Radial
				//int rx=rand()%50,ry=rand()%50,max=70;
				
		//fuck=(float)rx*(float)rx+(float)ry*(float)ry;
		long double variab=rx*rx+ry*ry;
				variab=sqrt(variab);
				//if ((fuck==0) || (fuck<=max)) return ry;
				//else return 0;
				//ry/=fuck*max;
				//Get Radial

				return variab;

}
void ShortenRadBend(int * x, int * y, int mustB, int is )
{
	double fullx=*x;
	double fully=*y;
	double percent=mustB;
	percent/=is;
	*x=*x*mustB/is;
//	fullx-=*x;
//	if (*x<0) *x-=sqrt(-fullx); else
//	if ((percent>1) || (percent<0)) 
//		*x+=sqrt(fullx);
	*y=*y*mustB/is;
	//*y*=percent;
/*	fully-=*y;
	if (*y<0) *y-=sqrt(-fully); else
	*y+=sqrt(fully);
*/
}

void pointAngles(int x, int y, int h, int toX, int toY, int toH)
{
	int cenx=toX-x;
	int ceny=toY-y;
//X
createData.angley=getAngle(ipifagor(cenx, ceny), toH-h);
createData.angleh=getAngle(cenx, ceny);
}

void NegPosDisplacement2DLimit(int *  x,int *  y, float limit)
{

long double percent=ipifagor (*x, *y);
percent/=limit;
if (percent>1)
{
	*x/=percent;
	*y/=percent;
}

}

int getmematrix (int tx, int ty, int x, int y, float fh)
{

	 int h=floor(fh/pixInPhis);
    /*__asm
    {
        fld fh
        fistp h
    }*/

	if (h > matrhig-1) return 0;
	h = (h >= -matrdepth) ? h : -matrdepth;

	if (x<0) {x+=tyleSize; tx--;}
	x>>=3;
	if (x>=phArSize) {	x-=phArSize; tx++;	}


	if (y<0) {y+=tyleSize; ty--;}
	y>>=3;
	if (y>=phArSize) {y-=phArSize; ty++;}


	if ((x<phArSize) && (y<phArSize) && (x>=0) && (y>=0))
	return (mapmatrix[tylextogrid(tx)][tyleytogrid(ty)][x][y][h+matrdepth]);
	else
		return 0; 
}
int getmematrixNotMe (int tx, int ty, int x, int y, float fh)
{

	 int h=floor(fh/pixInPhis);
    /*__asm
    {
        fld fh
        fistp h
    }*/

	h = (h <= matrhig-1) ? h : matrhig-1;
	h = (h >= -matrdepth) ? h : -matrdepth;

	if (x<0) {x+=tyleSize; tx--;}
	x>>=3;
	if (x>=phArSize) {	x-=phArSize; tx++;	}


	if (y<0) {y+=tyleSize; ty--;}
	y>>=3;
	if (y>=phArSize) {y-=phArSize; ty++;}


	if ((x<phArSize) && (y<phArSize) && (x>=0) && (y>=0))
	{
	int mmm=(mapmatrix[tylextogrid(tx)][tyleytogrid(ty)][x][y][h+matrdepth]);
	if ((mmm>=einmatr) && (mmm<phinmatr) && (&e[mmm-einmatr]==player.host)) return 0;
	return mmm;
	}
	else
		return 0; 
}
void clearmatrix (int tx, int ty, int x, int y, float fh)
{
 int h=floor(fh/pixInPhis);
	h = (h <= matrhig-1) ? h : matrhig-1;
	h = (h >= -matrdepth) ? h : -matrdepth;

	if (x<0) {x+=tyleSize; tx--;}
	x>>=3;
	if (x>=phArSize) {	x-=phArSize; tx++;	}


	if (y<0) {y+=tyleSize; ty--;}
	y>>=3;
	if (y>=phArSize) {y-=phArSize; ty++;}


	if ((x<phArSize) && (y<phArSize) && (x>=0) && (y>=0))
	mapmatrix[tylextogrid(tx)][tyleytogrid(ty)][x][y][h+matrdepth]=0;
}
void fillrmatrix (int tx, int ty, int x, int y, float fh, int i)
{
	 int h=floor(fh/pixInPhis);
	h = (h <= matrhig-1) ? h : matrhig-1;
	h = (h >= -matrdepth) ? h : -matrdepth;

	if (x<0) {x+=tyleSize; tx--;}
	x>>=3;
	if (x>=phArSize) {	x-=phArSize; tx++;	}


	if (y<0) {y+=tyleSize; ty--;}
	y>>=3;
	if (y>=phArSize) {y-=phArSize; ty++;}


	if ((x<phArSize) && (y<phArSize) && (x>=0) && (y>=0))
	mapmatrix[tylextogrid(tx)][tyleytogrid(ty)][x][y][h+matrdepth]=i;
}	
void TryClearmatrix (int tx, int ty, int x, int y, float fh, int i)
{
	 int h=floor(fh/pixInPhis);
	h = (h <= matrhig-1) ? h : matrhig-1;
	h = (h >= -matrdepth) ? h : -matrdepth;

	if (x<0) {x+=tyleSize; tx--;}
	x>>=3;
	if (x>=phArSize) {	x-=phArSize; tx++;	}


	if (y<0) {y+=tyleSize; ty--;}
	y>>=3;
	if (y>=phArSize) {y-=phArSize; ty++;}


	if ((x<phArSize) && (y<phArSize) && (x>=0) && (y>=0))
	{
		int * mat;
	mat=&mapmatrix[tylextogrid(tx)][tyleytogrid(ty)][x][y][h+matrdepth];
	if (*mat==i)
	*mat=0;
	}
}
void TryFillrmatrix (int tx, int ty, int x, int y, float fh, int i)
{
 int h=floor(fh/pixInPhis);
	h = (h <= matrhig-1) ? h : matrhig-1;
	h = (h >= -matrdepth) ? h : -matrdepth;

	if (x<0) {x+=tyleSize; tx--;}
	x>>=3;
	if (x>=phArSize) {	x-=phArSize; tx++;	}


	if (y<0) {y+=tyleSize; ty--;}
	y>>=3;
	if (y>=phArSize) {y-=phArSize; ty++;}


	if ((x<phArSize) && (y<phArSize) && (x>=0) && (y>=0))
	//if (!mapmatrix[tylextogrid(tx)][tyleytogrid(ty)][x][y][h+matrdepth])
	if ((mapmatrix[tylextogrid(tx)][tyleytogrid(ty)][x][y][h+matrdepth]>i) || (!mapmatrix[tylextogrid(tx)][tyleytogrid(ty)][x][y][h+matrdepth])) 
	mapmatrix[tylextogrid(tx)][tyleytogrid(ty)][x][y][h+matrdepth]=i;
}	


int signinmatrix (int tx, int ty, int x,int y, int ei, int slot)
{
//if (x<0) {x+=tyleSize; tx--;	}
//if (y<0) {y+=tyleSize; ty--;}
	return 0; 
	x=x>>16;
	y=y>>16;
x=(x/phArSize)*pixInPhis;
y=(y/phArSize)*pixInPhis;
mapmatrix[tylextogrid(tx)][tyleytogrid(ty)][x][y][matrhig+matrdepth]=ei;
return 0;
}
int signoutmatrix (int tx, int ty, int x,int y)
{
	return 0;
//if (x<0) {x+=tyleSize; tx--;	}
//if (y<0) {y+=tyleSize; ty--;}
	x=x>>16;
	y=y>>16;
x=(x/phArSize)*pixInPhis;
y=(y/phArSize)*pixInPhis;

mapmatrix[tylextogrid(tx)][tyleytogrid(ty)][x][y][matrhig+matrdepth]=0;
return 0;
}
int getsignedintrix (int tx, int ty, int x,int y, int rad)
{
int bpp;
	bestdist=9999;
	bestno=0;
	
	// av, bv, xx, yy, cx, cy, ie,je, rad, z, d, dx, dy
	//if (x<0) {x+=tyleSize; tx--;	}
//if (y<0) {y+=tyleSize; ty--;}
x=x>>16;
y=y>>16;
xx=(x/phArSize)*pixInPhis;
yy=(y/phArSize)*pixInPhis;

for (int cx=-(rad/phArSize)-1; cx<=(rad/phArSize)+1; cx++)
	for (int cy=-(rad/phArSize)-1; cy<=(rad/phArSize)+1; cy++)
	{
		ie=tx;
		je=ty;
av=xx+pixInPhis*cx;
bv=yy+pixInPhis*cy;
if (bv>=phArSize) {bv-=phArSize; je++;}
if (av>=phArSize) {av-=phArSize; ie++;	}
if (bv<0) {bv+=phArSize; je--;}
if (av<0) {av+=phArSize; ie--;}

g=mapmatrix[tylextogrid(ie)][tyleytogrid(je)][av][bv][matrhig+matrdepth];

if ((g==0) || (e[g].type==0) || (e[g].type==epointer->type)) continue; 
//theta,thetb;
theta=(epointer->pos.tx-e[g].pos.tx)*tyleSize+accurate(epointer->pos.x-e[g].pos.x); //dx
thetb=(epointer->pos.ty-e[g].pos.ty)*tyleSize+accurate(epointer->pos.y-e[g].pos.y); //dy
int d=accurate(epointer->pos.h);
int dh=accurate(epointer->pos.h-e[g].pos.h);
fuck=pow(theta,2)+pow(thetb,2);
fuck=sqrt(fuck); 
if (fuck>rad) continue;
divfuck=fuck/pixInPhis;
int z;
for (z=1; z<divfuck; z++)
{
bpp=getmematrix (tx, ty, x-(theta*z)/divfuck, y-(thetb*z)/divfuck, d-(dh*z)/divfuck);
	if ((bpp>0) && (bpp<einmatr))
		break;
}

if (z<divfuck) continue;

if (bestdist>fuck) {
	bestdist=fuck; bestno=g;
}

}
return bestno; 
}

bool GetDarkMaskGrid(int offx, int offy, int w, int h)
{
w=(offx+w)/8+1;
h=(offy+h)/8+1;
offx/=8;
offy/=8;

offx=max(0,offx);
w=min(scrwi8,w);
offy=max(0,offy);
h=min(scrhi8,h);
//bool * __restrict darkmaskGrid;
for (int i=offy; i<h; i++)
{
	bool* sector=darkmaskGrid+i*scrwi8+offx;
	for (int j=offx; j<w; j++)
	{
if (*sector) return 1;
sector++;
	}

}
return 0;
}
void fillDarkMaskGrid(int offx, int offy, int w, int h)
{
w=(offx+w)/8+1;
h=(offy+h)/8+1;
offx/=8;
offy/=8;

offx=max(0,offx);
w=min(scrwi8,w);
offy=max(0,offy);
h=min(scrhi8,h);
//bool * __restrict darkmaskGrid;
for (int i=offy; i<h; i++)
{
	bool* sector=darkmaskGrid+i*scrwi8+offx;
	for (int j=offx; j<w; j++)
	{
*sector=1;
sector++;
	}

}
}
int GetDarkIndex(int offx, int offy)
{
int x=offx/8;
int y=offy/8;
if ((x>=0) && (y>=0) && (x<scrwi8) && (y<scrhi8))
if (*(darkmaskGrid+y*scrwi8+x)==0) 
return (8-offx+x*8); 
return 0;
}
void SetZeroDarkIndex(int offx, int offy)
{
int x=offx/8;
int y=offy/8;
if ((x>0) && (y>0) && (x<scrwi8) && (y<scrhi8))
*(darkmaskGrid+y*scrwi8+x)=0;
}
void SetTrueDarkIndex(int offx, int offy)
{
int x=offx/8;
int y=offy/8;
if ((x>0) && (y>0) && (x<scrwi8) && (y<scrhi8))
*(darkmaskGrid+y*scrwi8+x)=1;
}
offsets GetPixelsTillEnd(int offx, int offy)
{
offsets ret;
int x=offx/8*8;
int y=offy/8*8;
ret.x=8-(offx-x);
ret.y=8-(offy-y);
//if ((x>0) && (y>0) && (x<scrwi8) && (y<scrhi8))
//if (*(darkmaskGrid+y*scrwi8+x)==0) 
//return (8-offx+x*8); 
return ret;
}

void phisicInteraction (phisicalMe *one, phisicalMe *two)
{
fuck=one->mass;
fuck/=two->mass;
//fuck=min(fuck,0.5);
float Heat=one->pr*two->pr/10000;
int temp=one->speedx;
one->speedx+=two->speedx/fuck*Heat;
two->speedx+=temp*fuck*Heat;

temp=one->speedy;
one->speedy+=two->speedy/fuck*Heat;
two->speedy+=temp*fuck*Heat;

temp=one->speedh;
one->speedh+=two->speedh/fuck*Heat;
two->speedh+=temp*fuck*Heat;
//ph[k].force.speedy+=curphisobj->force.speedy*fuck;
//ph[k].force.speedh+=curphisobj->force.speedh*fuck;

}
void phisicInteractionOneWay (phisicalMe *one, phisicalMe *two)
{
fuck=one->mass;
fuck/=two->mass;
//fuck=min(fuck,0.5);
float Heat=one->pr*two->pr/10000;

two->speedx+=one->speedx*fuck*Heat;

two->speedy+=one->speedy*fuck*Heat;

two->speedh+=one->speedh*fuck*Heat;
//ph[k].force.speedy+=curphisobj->force.speedy*fuck;
//ph[k].force.speedh+=curphisobj->force.speedh*fuck;

}
// ********************************** Decay Coordinates math
float GetAngleFromTo (worldcoordinates* from, worldcoordinates* to){
	int x=(from->tx-to->tx)*tyleSize+accurate(from->x-to->x);
	int y=(from->ty-to->ty)*tyleSize+accurate(from->y-to->y);
	return ConvertVectorTo360( x,  y);
}
void putSameTyle(worldcoordinates *Plegs, worldcoordinates *Ppos)
{
	if (Plegs->tx>Ppos->tx) {Plegs->tx--; Plegs->x+=tyleSize*accuracy;}
if (Plegs->ty>Ppos->ty) {Plegs->ty--; Plegs->y+=tyleSize*accuracy;}
if (Plegs->tx<Ppos->tx) {Plegs->tx++; Plegs->x-=tyleSize*accuracy;}
if (Plegs->ty<Ppos->ty) {Plegs->ty++; Plegs->y-=tyleSize*accuracy;}
}
void copyPos(worldcoordinates* posfrom, worldcoordinates* posto)
{
	posto->tx=posfrom->tx;
	posto->ty=posfrom->ty;
	posto->x=posfrom->x;
	posto->y=posfrom->y;
	
}
void speedLimitCheck(double* speedx, double *speedy, double *speedh, double max )
{
	katet3dD (*speedx, *speedy, *speedh);
	if (fuck>max)
	{
		fuck=max/fuck;
		*speedx*=fuck;
		*speedy*=fuck;
		*speedh*=fuck;
	}
}
void speedLimitCheckINT(int* speedx, int *speedy, int *speedh, float max )
{
	katet3d (*speedx, *speedy, *speedh);
	if (fuck>max)
	{
		fuck=max/fuck;
		*speedx*=fuck;
		*speedy*=fuck;
		*speedh*=fuck;
	}
}
void speedLimitCheckI(int* speedx, int *speedy, int *speedh, double max )
{
	katet3dD (*speedx, *speedy, *speedh);
	if (fuck>max)
	{
		fuck=max/fuck;
		*speedx*=fuck;
		*speedy*=fuck;
		*speedh*=fuck;
	}
}

void normalizeCoordinates1(int* tx, int* ty, int* x, int* y )
{
	*tx+=*x/1024 - (*x<0);
	*x=(*x % 1024) + (*x<0)*1024;

	*ty+=*y/1024 - (*y<0);
	*y=(*y % 1024) + (*y<0)*1024;
}

void normalizeCoordinates(worldcoordinates* pos)
{
	pos->tx+=pos->x/coorToTyle - (pos->x<0);
	pos->x=pos->x % coorToTyle + (pos->x<0)*coorToTyle;

	pos->ty+=pos->y/coorToTyle- (pos->y<0);
	pos->y=pos->y % coorToTyle+ (pos->y<0)*coorToTyle;
}
int PosToPosDistance(worldcoordinates* pos1, worldcoordinates* pos2)
{
	int distx=(pos1->tx-pos2->tx)*tyleSize+accurate(pos1->x-pos2->x);
	int disty=(pos1->ty-pos2->ty)*tyleSize+accurate(pos1->y-pos2->y);
	return ipifagor(distx,disty); 
}
int PosToMouseDistance(worldcoordinates* pos1)
{
	int distx=(pos1->tx-cameratylx)*tyleSize+accurate(pos1->x)-actcamx;
	int disty=(pos1->ty-cameratyly)*tyleSize+accurate(pos1->y)-actcamy;
	return ipifagor(distx,disty); 
}
void MouseToPos (worldcoordinates* pos){
	pos->tx=cameratylx;
	pos->ty=cameratyly;
	pos->x=(actcamx+mouse.pos.x)*accuracy;
	pos->y=(actcamy+mouse.pos.y)*accuracy;
	pos->h=0;
	normalizeCoordinates(pos);

}
void preoffxs(worldcoordinates* pos){
preoffx=-(actcamx<<16);
preoffx-=(cameratylx - pos->tx)*tyleSize*accuracy;
preoffy=-(actcamy<<16);
preoffy-=(cameratyly - pos->ty)*tyleSize*accuracy;

}
void SefOffs(worldcoordinates* pos){
offx=(pos->x+preoffx)/accuracy;
offy=(pos->y+preoffy)/accuracy;
int elevation=pos->h/dddstength;
offx+=(((offx-scrwid)*elevation)/accuracy);
offy+=(((offy-scrhid)*elevation)/accuracy);
} // Use only with preoffxs
void PosToCreateData(worldcoordinates* pos)
{
createData.tx=pos->tx; createData.ty=pos->ty;
createData.x=pos->x; createData.y=pos->y; 
createData.h=pos->h;
}
void PosToFlatOffs(worldcoordinates* pos){
offx=(pos->x>>16) - actcamx + ((pos->tx - cameratylx)<<10);
offy=(pos->y>>16) - actcamy + ((pos->ty - cameratyly)<<10);
}



// sound shit
void PlayMySound(MySound* sound)
{



	int Ch;
	//int Ch=Mix_PlayChannel(-1, sound->track, 0 );
//if (Ch==-1)
//{
Ch=0;
int minVol=999;
for (int i=0; i<6; i++) 
	if (sounds.OnTrack[i].playedSoundVolume<minVol) { minVol=sounds.OnTrack[i].playedSoundVolume; Ch=i;}
if (minVol>sound->volume)
	return;

TrackData* OT=&sounds.OnTrack[Ch];

int mod=sound->volume-sounds.GlobalMaxVolume;
Mix_Volume(Ch,sound->ActualVolume+mod);

Mix_PlayChannel( Ch, sound->track, 0 );
//}

OT->playedActualVolume=sound->ActualVolume;
OT->playedSoundVolume=sound->volume;
OT->playedSoundModifier=sound->modifier;
OT->playedSoundMin=sound->min;
OT->ambient=0;
OT->playedSound=sound;

}
void PlayAmbient(MySound* sound, int track, int life) // 6 or 7
{
TrackData* OT=&sounds.OnTrack[track];

if ((OT->playedSound==sound) && (Mix_Playing(track))) {OT->life=life; return;}
Mix_PlayChannel( track, sound->track, -1 );

OT->playedActualVolume=sound->ActualVolume;
OT->playedSoundVolume=sound->volume;
OT->playedSoundModifier=sound->modifier;
OT->playedSoundMin=sound->min;
OT->ambient=1;
OT->playedSound=sound;
OT->life=life;
}

void LoadMySound (const char* filename, double volume, double modifier, int min, MySound* sound, int Actual)
{
	sound->track=Mix_LoadWAV(filename);
	sound->volume=volume;
	sound->modifier=modifier;
	sound->min=min;
	sound->ActualVolume=Actual;
}
void SetUpsounds()
{
//sounds.playedActualVolume[6]=sounds.playedSoundVolume[6]=sounds.bank.guts.volume;

//sounds.playedActualVolume[7]=sounds.playedSoundVolume[7]=sounds.bank.softGuts.volume;
//Mix_PlayChannel( 7, sounds.bank.softGuts.track, -1 );
//Mix_PlayChannel( 6, sounds.bank.ambientField.track, -1 );

//PlayAmbient(&sounds.bank.softGuts, 7, -1);

Mix_Volume(7,0);
sounds.OnTrack[7].playedSoundVolume=0;
//	Mix_PlayChannel( 6, sounds.bank.guts.track, -1 );
}
void mgmtUint8 (int *vol, double fade)
{
	*vol*=(-fade)+1;
	if (*vol>255) *vol=255;
	if (*vol<0) *vol=0;
}
void MakeBloodyNoise (int offx, int offy)
{
	/*
sounds.playedActualVolume[6]=sounds.playedSoundVolume[6]=sounds.bank.guts.volume;
sounds.playedSoundMin[6]=sounds.bank.guts.min;
sounds.playedSoundModifier[6]=sounds.bank.guts.modifier;*/

}
void MakegutsNoise (int offx, int offy)
{
	
	sounds.OnTrack[7].playedActualVolume=sounds.OnTrack[7].playedSoundVolume=sounds.bank.softGuts.volume;
	sounds.OnTrack[7].playedSoundMin=sounds.bank.softGuts.min;
	sounds.OnTrack[7].playedSoundModifier=sounds.bank.softGuts.modifier;

}
void mgmtSound()
{

int MaxVol=-100;
for (int i=0; i<8; i++)
{
	TrackData* OT=&sounds.OnTrack[i];
	if (Mix_Playing(i))
{
	
	OT->playedSoundVolume+=OT->playedSoundModifier;
	OT->playedSoundVolume=max(OT->playedSoundVolume,OT->playedSoundMin);
	MaxVol=max(MaxVol,OT->playedSoundVolume);
	if ((OT->ambient) && (OT->life!=-1))
		{
			OT->life--;
	if  (!OT->life) 
		Mix_FadeOutChannel(i, 500);
	}
	//Mix_Volume(i,MIX_MAX_VOLUME/3);
}
	else
		OT->playedSoundVolume=0;
}
sounds.GlobalMaxVolume+=(MaxVol-sounds.GlobalMaxVolume)*hearingAdjustSpeed;

for (int i=0; i<7; i++) // 7 - is an ambient track
{
	int mod=min(sounds.OnTrack[i].playedSoundVolume-sounds.GlobalMaxVolume, 0);
	Mix_Volume(i,max(0,sounds.OnTrack[i].playedActualVolume*(128+mod)/128));
}

if  (!(Mix_Playing(6)))  PlayAmbient(&sounds.bank.ambientField, 6, -1);
//Dynamic track
 int mod=sounds.OnTrack[7].playedSoundVolume-sounds.GlobalMaxVolume*(sounds.OnTrack[7].playedSoundVolume>0);
	Mix_Volume(7,max(0,sounds.OnTrack[7].playedSoundVolume+mod));
 }


 offsets AngleAcceleration(double angle, int force)
 {
	 offsets offs;
	 offs.x=force*sin(angle); 
	 offs.y=force*cos(angle);
return offs;
 }
 
int E8bitMatrixCalculateGradus(int x, int y, int h, int radius)
{
int xe=x+radius;
int ye=y+radius;
int he=h+radius;

double countx=0, county=0, counth=0;

for (int x1=max(0, x-radius); x1<=xe; x1++)
	for (int y1=max(0, y-radius); y1<=ye; y1++)
		for (int h1=max(0, h-radius); h1<=he; h1++)
		{
			if ((vox[curVoxel].VoxelMatrix[x1][y1][h1]) && (x1!=x || y1!=y || h1!=h))
			{
dddistance (x1-x, y1-y, h1-h);
countx+=((x-x1)*100)/fuck;
county+=((y-y1)*100)/fuck;
counth+=((h-h1)*100)/fuck;
			}
		}
return (getSector(countx,county,lightArSize)+lightArSize*(counth>4500)+lightArSize*(counth>7000));

}


void DrowLine (int x1, int y1, int x2, int y2, Uint32 color)
{
if ( (y1 >=scrhi) || (x1 >=scrwi)) return;
if ( (y2>=scrhi) || (x2>=scrwi)) return;

x1=max(x1,0);
y1=max(y1,0);
x2=max(x2,0);
y2=max(y2,0);

int steps=max(abs(x2-x1), abs(y2-y1)); 
double xperstep=(double)(x2-x1)/steps;
double yperstep=(double)(y2-y1)/steps;
double x=x1;
double y=y1;

while (steps){
*((Uint32*)screen->pixels+((int)y*screen->w)+(int)x)=color;
x+=xperstep;
y+=yperstep;
steps--;
}

}
void DrowHTPLine (int x1, int y1, int x2, int y2, Uint32 color)
{
if ( (y1 >=scrhi) || (x1 >=scrwi)) return;
if ( (y2>=scrhi) || (x2>=scrwi)) return;

x1=max(x1,0);
y1=max(y1,0);
x2=max(x2,0);
y2=max(y2,0);

int steps=max(abs(x2-x1), abs(y2-y1)); 
double xperstep=(double)(x2-x1)/steps;
double yperstep=(double)(y2-y1)/steps;
double x=x1;
double y=y1;

while (steps){
*((Uint32*)screen->pixels+((int)y*screen->w)+(int)x)|=color;
x+=xperstep;
y+=yperstep;
steps--;
}

}
void DrowFrame (int x, int y, int w, int h, Uint32 color)
{
if ( (y>=scrhi) || (x>=scrwi)) return;
w=min(w,scrwi-x-1);
h=min(h,scrhi-y-1);
	//if (((y+h/2)<scrhi) && )
//*((Uint32*)screen->pixels+(y+h/2)*screen->w+x+w/2)=0xFFFFFF;

x=max(x,0);
//if (x+w>=scrwi) w=scrwi-x-1;
	y=max(y,0);
//if (y+h>=scrhi) h=scrhi-y-1;
Uint32* screenPix=(Uint32*)screen->pixels+y*screen->w+x;
Uint32* screenPix2=(Uint32*)screen->pixels+(y+h)*screen->w+x;
for (int i=0; i<w; i++)
{
//*screenPix=0xFFFFFF;
//*screenPix2=0xFFFFFF;
*screenPix2=(((RedMask & ((*screenPix2 & RedMask) + ((int)(((int)(color & RedMask) - (int)(*screenPix2 & RedMask))) >>1))) | (GreenMask & ((*screenPix2 & GreenMask) + ((int)(((int)(color & GreenMask) - (int)(*screenPix2 & GreenMask))) >>1))) | (BlueMask & ((*screenPix2 & BlueMask) + ((int)(((int)(color & BlueMask) - (int)(*screenPix2 & BlueMask))) >>1))))) | 0xFF000000 ;
*screenPix=(((RedMask & ((*screenPix & RedMask) + ((int)(((int)(color & RedMask) - (int)(*screenPix & RedMask))) >>1))) | (GreenMask & ((*screenPix & GreenMask) + ((int)(((int)(color & GreenMask) - (int)(*screenPix & GreenMask))) >>1))) | (BlueMask & ((*screenPix & BlueMask) + ((int)(((int)(color & BlueMask) - (int)(*screenPix & BlueMask))) >>1))))) | 0xFF000000 ;
//*screenPix=color;
screenPix++;
screenPix2++;
}

screenPix2=(Uint32*)screen->pixels+(y)*screen->w+x;
for (int i=0; i<h; i++)
{
*screenPix2=((RedMask & ((*screenPix2 & RedMask) + ((int)(((int)(color & RedMask) - (int)(*screenPix2 & RedMask))) >>1))) | (GreenMask & ((*screenPix2 & GreenMask) + ((int)(((int)(color & GreenMask) - (int)(*screenPix2 & GreenMask))) >>1))) | (BlueMask & ((*screenPix2 & BlueMask) + ((int)(((int)(color & BlueMask) - (int)(*screenPix2 & BlueMask))) >>1)))) | 0xFF000000;
	*screenPix=((RedMask & ((*screenPix & RedMask) + ((int)(((int)(color & RedMask) - (int)(*screenPix & RedMask))) >>1))) | (GreenMask & ((*screenPix & GreenMask) + ((int)(((int)(color & GreenMask) - (int)(*screenPix & GreenMask))) >>1))) | (BlueMask & ((*screenPix & BlueMask) + ((int)(((int)(color & BlueMask) - (int)(*screenPix & BlueMask))) >>1)))) | 0xFF000000;
screenPix+=screen->w;
screenPix2+=screen->w;
}

}
void DrowHTPFrame (int x, int y, int w, int h, Uint32 color)
{
if ( (y>=scrhi) || (x>=scrwi)) return;
w=min(w,scrwi-x-1);
h=min(h,scrhi-y-1);
	//if (((y+h/2)<scrhi) && )
//*((Uint32*)screen->pixels+(y+h/2)*screen->w+x+w/2)=0xFFFFFF;

x=max(x,0);
//if (x+w>=scrwi) w=scrwi-x-1;
	y=max(y,0);
//if (y+h>=scrhi) h=scrhi-y-1;
Uint32* screenPix=(Uint32*)screen->pixels+y*screen->w+x;
Uint32* screenPix2=(Uint32*)screen->pixels+(y+h)*screen->w+x;
for (int i=0; i<w; i++)
{
//*screenPix=0xFFFFFF;
//*screenPix2=0xFFFFFF;
*screenPix2|=(((RedMask & ((*screenPix2 & RedMask) + ((int)(((int)(color & RedMask) - (int)(*screenPix2 & RedMask))) >>1))) | (GreenMask & ((*screenPix2 & GreenMask) + ((int)(((int)(color & GreenMask) - (int)(*screenPix2 & GreenMask))) >>1))) | (BlueMask & ((*screenPix2 & BlueMask) + ((int)(((int)(color & BlueMask) - (int)(*screenPix2 & BlueMask))) >>1))))) | 0xFF000000 ;
*screenPix|=(((RedMask & ((*screenPix & RedMask) + ((int)(((int)(color & RedMask) - (int)(*screenPix & RedMask))) >>1))) | (GreenMask & ((*screenPix & GreenMask) + ((int)(((int)(color & GreenMask) - (int)(*screenPix & GreenMask))) >>1))) | (BlueMask & ((*screenPix & BlueMask) + ((int)(((int)(color & BlueMask) - (int)(*screenPix & BlueMask))) >>1))))) | 0xFF000000 ;
//*screenPix=color;
screenPix++;
screenPix2++;
}

screenPix2=(Uint32*)screen->pixels+(y)*screen->w+x;
for (int i=0; i<h; i++)
{
*screenPix2|=((RedMask & ((*screenPix2 & RedMask) + ((int)(((int)(color & RedMask) - (int)(*screenPix2 & RedMask))) >>1))) | (GreenMask & ((*screenPix2 & GreenMask) + ((int)(((int)(color & GreenMask) - (int)(*screenPix2 & GreenMask))) >>1))) | (BlueMask & ((*screenPix2 & BlueMask) + ((int)(((int)(color & BlueMask) - (int)(*screenPix2 & BlueMask))) >>1)))) | 0xFF000000;
	*screenPix|=((RedMask & ((*screenPix & RedMask) + ((int)(((int)(color & RedMask) - (int)(*screenPix & RedMask))) >>1))) | (GreenMask & ((*screenPix & GreenMask) + ((int)(((int)(color & GreenMask) - (int)(*screenPix & GreenMask))) >>1))) | (BlueMask & ((*screenPix & BlueMask) + ((int)(((int)(color & BlueMask) - (int)(*screenPix & BlueMask))) >>1)))) | 0xFF000000;
screenPix+=screen->w;
screenPix2+=screen->w;
}

}
void SelfDisplaceTexture (Uint32* src, int w, int h, int dx, int dy){
			if ((dx==0) && (dy==0)) 
		return;
int fromX, fromY, dispx, dispy, strtx, strty;
if (dx<0) {fromX=-dx; strtx=0; dispx=1;}
else {fromX=w-dx-1; strtx=w-1; dispx=-1;}

if (dy<0) {fromY=-dy; strty=0; dispy=1;}
else {fromY=h-dy-1; strty=h-1; dispy=-1;}

for (int i=0; i<(h-abs(dy)); i++){
Uint32* sr=src+(fromY+i*dispy)*w+fromX; 
Uint32* ds=src+(strty+i*dispy)*w+strtx;
int j=w-abs(dx);

if (dispx>0)
memcpy(ds,sr,j*4);
else
memcpy(ds+dispx*j,sr+dispx*j,j*4);
/*
while (j){
*ds=*sr;
j--; ds+=dispx; sr+=dispx;
}*/

}

}
void SelfDisplaceTextureCleanBorder (Uint32* src, int w, int h, int dx, int dy){
	if ((dx==0) && (dy==0)) 
		return;
int fromX, fromY, dispx, dispy, strtx, strty;
if (dx<0) {fromX=-dx; strtx=0; dispx=1;}
else {fromX=w-dx-1; strtx=w-1; dispx=-1;}

if (dy<0) {fromY=-dy; strty=0; dispy=1;}
else {fromY=h-dy-1; strty=h-1; dispy=-1;}



for (int i=0; i<(h-abs(dy)); i++){
Uint32* sr=src+(fromY+i*dispy)*w+fromX; 
Uint32* ds=src+(strty+i*dispy)*w+strtx;
int j=w-abs(dx);

if (dispx>0)
memcpy(ds,sr,j*4);
else
memcpy(ds+dispx*j,sr+dispx*j,j*4);

if (dx>0) memset(src+(strty+i*dispy)*w,0, dx*4);
if (dx<0) memset(src+(strty+i*dispy)*w+scrwi+dx-1,0, -dx*4);
}

if (dy>0) memset(src,0, scrwi*(dy)*4);
if (dy<0) memset(src+scrwi*(scrhi+dy-1),0, scrwi*(-dy+1)*4);

}
void SelfDisplaceUin8 (Uint8* src, int w, int h, int dx, int dy){
	if ((dx==0) && (dy==0)) 
		return;
int fromX, fromY, dispx, dispy, strtx, strty;
if (dx<0) {fromX=-dx; strtx=0; dispx=1;}
else {fromX=w-dx-1; strtx=w-1; dispx=-1;}

if (dy<0) {fromY=-dy; strty=0; dispy=1;}
else {fromY=h-dy-1; strty=h-1; dispy=-1;}



for (int i=0; i<(h-abs(dy)); i++){
Uint8* sr=src+(fromY+i*dispy)*w+fromX; 
Uint8* ds=src+(strty+i*dispy)*w+strtx;
int j=w-abs(dx);

if (dispx>0)
memcpy(ds,sr,j);
else
memcpy(ds+dispx*j,sr+dispx*j,j);

if (dx>0) memset(src+(strty+i*dispy)*w,0, dx);
if (dx<0) memset(src+(strty+i*dispy)*w+scrwi+dx-1,0, -dx);
}

if (dy>0) memset(src,0, scrwi*(dy));
if (dy<0) memset(src+scrwi*(scrhi+dy-1),0, scrwi*(-dy));

}
// Later make collision check for autoaimed enemy

float GetCollision(worldcoordinates* pos, float vx, float vy, float vh)
{
//theta=(epointer->pos.tx-e[g].pos.tx)*tyleSize+accurate(epointer->pos.x-e[g].pos.x); //dx
//thetb=(epointer->pos.ty-e[g].pos.ty)*tyleSize+accurate(epointer->pos.y-e[g].pos.y); //dy
//int d=accurate(epointer->pos.h);
//int dh=accurate(epointer->pos.h-e[g].pos.h);
fuck=vx*vx+vy*vy;//pow(theta,2)+pow(thetb,2);
fuck=sqrt(fuck); 
//if (fuck>rad) continue;
//divfuck=fuck/pixInPhis;
Uint32 steps=fuck;
float dx=vx/steps;
float dy=vy/steps;
float dh=vh/steps;
int x=accurate(pos->x);
int y=accurate(pos->y);
int h=accurate(pos->h);
vx=0;
vy=0;
vh=0;
while (steps)
{
int GM=getmematrix (pos->tx, pos->ty, x+vx, y+vy, h+vh);
	if ((GM) && (GM<einmatr)) break;
vx+=dx;
vy+=dy;
vh+=dh;
steps--;
}
return (fuck-steps)/fuck;
}
void MakeAlphaRoundMask(Uint8* mask, double radius)
{
Uint8* point=mask;
float katet;
for (int j=-radius; j<=radius; j++)
	for (int i=-radius; i<=radius; i++)
	{
		katet=pifagor(j,i)+1;
		//katet=(katet!=0) ? (katet) : 1;
		*point=(Uint8)(255/katet);
		point++;
	}
	}
void MakeAlphaPortionMask(Uint8* mask, float radius, int max)
{
Uint8* point=mask;
float katet;
for (int j=-radius; j<=radius; j++)
	for (int i=-radius; i<=radius; i++)
	{
		katet=pifagor(j,i)+1;
		//katet=(katet!=0) ? (katet) : 1;
		if (katet<=radius)
		*point=(Uint8)(max*(radius+1-katet)/radius); //42
		else *point=0;
		point++;
	}
	}
void DrowLaserLine (Uint32 radius, double x1, double y1, double x2, double y2, worldcoordinates* pos)
{
return;

if ((x1<radius) || (y1<radius) || (x2<radius) || (y2<radius)) return;
y1=min(y1, scrhi-radius*2);
y2=min(y2, scrhi-radius*2);

x2-=x1;
y2-=y1;
Uint32 dist=pifagor (x2, y2);
double dx=x2/dist;
double dy=y2/dist;
dist*=GetCollision(pos, x2, y2, 4);

Uint8* mask;
while (dist)
{
mask= new Uint8[(radius*2+1)*(radius*2+1)];
MakeAlphaRoundMask(mask, radius);
int FTH=min(150,dist);
dist-=FTH;
while(FTH)
{
Uint8* maskP=mask;
for (int y=y1-radius; y<=y1+radius; y++)
{
Uint32* scrpix=((Uint32*)screen->pixels+((int)y*scrwi)+(int)x1-radius);
int x=radius*2+1;
while (x) {
	*scrpix|= *maskP;
	//(BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * alpha) >>
x--; *scrpix++; maskP++;}

}

x1+=dx;
y1+=dy;
FTH--;
}

if (!dist)
{
Uint8* maskP=mask;
for (int y=y1-radius; y<=y1+radius; y++)
{
Uint32* scrpix=((Uint32*)screen->pixels+((int)y*scrwi)+(int)x1-radius);
int x=radius*2+1;
while (x) {
	*scrpix=(*scrpix & 0xFFFFFF00) | min(0xFF, (*scrpix & BlueMask) + *maskP) ;
	//(BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * alpha) >>
x--; *scrpix++; maskP++;}
}
}


radius=max(1,radius-1);
}

}

void DrowFlashLight (Uint32 radius, double x1, double y1, double x2, double y2, worldcoordinates* pos)
{
if ((x1<radius) || (y1<radius) || (x2<radius) || (y2<radius)) return;
y1=min(y1, scrhi-radius*2);
y2=min(y2, scrhi-radius*2);
int tx2;
int ty2;
x2-=x1;
y2-=y1;

double dist=pifagor (x2, y2);
double dx=x2/dist;
double dy=y2/dist;

//dist=scrwid;
dist=scrwid*GetCollision(pos, x2*scrwid/dist, y2*scrwid/dist, 4);
tx2=x1+dx*dist;
ty2=y1+dy*dist;
Uint8* mask;

while (dist>1)
{
mask= new Uint8[(radius*2+1)*(radius*2+1)];
MakeAlphaPortionMask(mask, radius,120);
int FTH=min(3,dist);
//dist-=FTH;
while(FTH)
{
Uint8* maskP=mask;
fillDarkMaskGrid(x1-radius, y1-radius, radius*2, radius*2);
if ((x1<radius) || (y1<radius) || (x1+radius>=scrwi) || (y1+radius>=scrhi)) return;
for (int y=y1-radius; y<=y1+radius; y++)
{
	Uint32 * __restrict dmask=darkmask+((int)y*scrwi)+(int)x1-radius;

int x=radius*2+1;
while (x) {
	*dmask+= *maskP<<16;
	//(BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * alpha) >>
x--; *dmask++; maskP++;}

}



x1+=dx;
y1+=dy;
dist-=pifagor (dx, dy);




FTH--;
}
/*
if (!dist)
{
Uint8* maskP=mask;
for (int y=y1-radius; y<=y1+radius; y++)
{
Uint32* scrpix=((Uint32*)screen->pixels+((int)y*scrwi)+(int)x1-radius);
int x=radius*2+1;
while (x) {
	*scrpix=(*scrpix & 0xFFFFFF00) | min(0xFF, (*scrpix & BlueMask) + *maskP) ;
	//(BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * alpha) >>
x--; *scrpix++; maskP++;}
}
}
*/

radius+=3;

dx=dx*(radius+5)/radius;
dy=dy*(radius+5)/radius;

//min(1,radius+1);

}

mask= new Uint8[(radius*2+1)*(radius*2+1)];
MakeAlphaPortionMask(mask, radius,42);
Uint8* maskP=mask;

if ((tx2<radius) || (ty2<radius) || (tx2+radius>=scrwi) || (ty2+radius>=scrhi)) return;
for (int y=ty2-radius; y<=ty2+radius; y++)
{
	Uint32 * __restrict dmask=darkmask+((int)y*scrwi)+(int)tx2-radius;
int x=radius*2+1;
while (x) {
	*dmask+= *maskP<<16;
	//(BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * alpha) >>
x--; *dmask++; maskP++;}

}


}

ScreenFitDrowFrame* GenerateFrame (int scrw, int scrh, int imgw, int imgh, int offx, int offy)
{
	ScreenFitDrowFrame frame;
	frame.fromx=0;
	frame.fromy=0;
	if (offx<0) frame.fromx-=offx;
	if (offy<0) frame.fromy-=offy;
	 frame.tox=imgw;
	frame.toy=imgh;
	if (offx+frame.tox>screen->w) frame.tox=(scrw-offx);
	if (offy+frame.toy>screen->h) frame.toy=(scrh-offy);

	frame.ImgJump=imgw-frame.tox+frame.fromx;
	frame.scrJump=scrw-frame.tox+frame.fromx;
return &frame;
}
int GetAsno(int h, int i, int j)
{

if (ApplicCount==2) 
	return ApplicCount;
ApplicCount++;
return (h+(i % 2)*((ApplicCount)/2)+j) % (ApplicCount);
	//(((h)*((8+((ApplicCount+1) % 2))==0)+(j-8)) % (ApplicCount+1));
// j-x
}

int GetFreeDebris(){
	int i=0;
	while ((!DebrisList[i].free) && (i<DebrisListMax)) i++;
	if (i<DebrisListMax) { DebrisList[i].free=0; DebrisList[i].Max=0; return i;}
	return -1;
}
int GetTyleBackground(int tx, int ty){
tx%=tyleBGFlagsSize;
if (tx<0) tx+=tyleBGFlagsSize;
ty%=tyleBGFlagsSize;
if (ty<0) ty+=tyleBGFlagsSize;

return (grid.TyleInfo[tx][ty] & grid.BGnumberMask);

}
void SetTyleBackground(int tx, int ty, int i){
tx%=tyleBGFlagsSize;
if (tx<0) tx+=tyleBGFlagsSize;
ty%=tyleBGFlagsSize;
if (ty<0) ty+=tyleBGFlagsSize;

grid.TyleInfo[tx][ty]=(grid.TyleInfo[tx][ty] & (~grid.BGnumberMask)) | i;
}
bool GetTyleFlag(int tx, int ty, Uint32 flag){
tx%=tyleBGFlagsSize;
if (tx<0) tx+=tyleBGFlagsSize;
ty%=tyleBGFlagsSize;
if (ty<0) ty+=tyleBGFlagsSize;
return (grid.TyleInfo[tx][ty] & flag);
}
void ToggleTyleFlag(int tx, int ty, Uint32 flag){
tx%=tyleBGFlagsSize;
if (tx<0) tx+=tyleBGFlagsSize;
ty%=tyleBGFlagsSize;
if (ty<0) ty+=tyleBGFlagsSize;
grid.TyleInfo[tx][ty] ^= flag;
}
void SaveTerrain(int tx, int ty){
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
}



void adjustToPentagon(int* x, int* y){

while (*x<0) *x+=tyleSize; 
while (*y<0) *y+=tyleSize;

*x+=16;
bool rowplus=(int)(*x/(21.33333)) % 2; 
*x/=(21.33333);
*x*=21.3333;
*x-=6;


*y+=9-12.8*rowplus;
int origY=*y;

*y/=(25.6);
*y*=25.6;

if (rowplus){ if (origY<*y) *y-=12.8; else *y+=12.8;}  
*y+=9;

}

bool editorCheckPressDistance(int dist){

	if (JustKatet(edit.lastPressMouseX-mouse.pos.x, edit.lastPressMouseY-mouse.pos.y)<dist) return 1;
	edit.lastPressMouseX=mouse.pos.x;
	edit.lastPressMouseY=mouse.pos.y;
	 
	return 0; 
}
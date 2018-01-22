
void VoxPreview(){
	worldcoordinates tmppos;
	MouseToPos(&tmppos);

QuickDrow8bitVex(palette.Buffer8bit,&tmppos,
				 &mgvDB.mgvarray[mgvox.curpiece][mgvox.curpiecetype].Converted 
				 , 0, 0, tera.Edit.G8h-22,tera.Edit.G8resize, tera.Edit.Ax, tera.Edit.Ay, tera.Edit.Ah, 0);


	LightsTo180arrayF(&tmppos, defAngleFade,tera.Edit.Ah , 0,deAlphaMask,0);
	DrowBufferByHeight(mgvDB.ConvertedPalette[mgvox.curpiece], mouse.pos.x,  mouse.pos.y, screen,1);	

}
void MegavoxBlit (MegavoxSegment* voxel, float resize, int h, float ax, float ay, float ah){
FloatAnglesToCosSin(ax, ay, ah,&T2NtoN);
New8BitNtoN(resize,&T2NtoN);
float TrueX,TrueY, TrueH, x, y, z;
for (int h=23; h>=0; h--){
Uint16* pta=&voxel->PieceNtypeNang[h*24*24];	
Uint16* col=&voxel->matrix[h*24*24];
for (int i=0; i<24; i++)
	for (int j=0; j<24; j++){
		if (*pta){
int pc=*pta/mgvpiecestypes/4;
int tp=(*pta % (mgvpiecestypes*4))/4;
if ((pc<mgvpieces) && (tp<mgvpiecestypes)){
MegavoxSegment* subvoxel=&mgvDB.mgvarray[*pta/mgvpiecestypes/4][(*pta % (mgvpiecestypes*4))/4];
if (subvoxel->Converted.count){
	int ang=(*pta) % 4; 
x=j-12;
y=i-12;
z=h-8;
		TrueX=x*T2NtoN.fxtox+z*T2NtoN.fhtox+y*T2NtoN.fytox;
		TrueY=x*T2NtoN.fxtoy+z*T2NtoN.fhtoy+y*T2NtoN.fytoy;
		TrueH=x*T2NtoN.fxtoh+z*T2NtoN.fhtoh+y*T2NtoN.fytoh;

		QuickDrow8bitVexOneColor(palette.ThreadBuffer8bit,0,&subvoxel->Converted, TrueX*8, TrueY*8, TrueH*8+8*4-h,
				 resize, ax, ay, ah+M_PI/2*(ang), (0x00000001<<(*col+23)), (*pta % 4)*16);
	
}
}
		}
pta++;
col++;

	}
}
}
void MgVoxPreview(){
	MegavoxBlit (&mgvDB.mgvarray[mgvox.curpiece][mgvox.curpiecetype],
		tera.Edit.G8resize, tera.Edit.G8h, tera.Edit.Ax, tera.Edit.Ay,tera.Edit.Ah );
	worldcoordinates tmppos;
	MouseToPos(&tmppos);
	LightsTo180arrayF(&tmppos, defAngleFade,tera.Edit.Ah , 0,deAlphaMask,0);
	DrowBufferByHeight(mgvDB.ConvertedPalette[mgvox.curpiece], mouse.pos.x,  mouse.pos.y, screen,1);	
}

void ReadIndexedImages()
{
	ifstream file("pictures/TerrainBrushes/ConvertedIndBrushes.lr",ios::binary);
Uint32 i=0;
while (file.read((char*)&i, 2))
{
file.read ((char *)&tera.IndDecay[i].w, 2);
file.read ((char *)&tera.IndDecay[i].h, 2);
file.read ((char *)&tera.IndDecay[i].pixCount, 4);

tera.IndDecay[i].pixels=new Uint8 [tera.IndDecay[i].pixCount];
file.read((char*)tera.IndDecay[i].pixels, tera.IndDecay[i].pixCount);
}
 file.close (); 
}
void ReadHeightImages()
{
ifstream file("pictures/TerrainBrushes/ConvertedHeight.lr",ios::binary);
Uint32 i=0;
while (file.read((char*)&i, 2))
{
file.read ((char *)&tera.height[i].w, 2);
file.read ((char *)&tera.height[i].h, 2);
int size=tera.height[i].w*tera.height[i].h;
tera.height[i].DnA=new Uint8 [size];
file.read((char*)tera.height[i].DnA, size);
}
 file.close (); 
}
void ReadDHImages()
{
ifstream file("pictures/TerrainBrushes/ConvertedDH.lr",ios::binary);
Uint32 i=0;
while (file.read((char*)&i, 2))
{
file.read ((char *)&tera.DHheight[i].w, 2);
file.read ((char *)&tera.DHheight[i].h, 2);
int size=tera.DHheight[i].w*tera.DHheight[i].h;
tera.DHheight[i].DnA=new Uint8 [size];
file.read((char*)tera.DHheight[i].DnA, size);
}
 file.close (); 
}

void ReadTyleFlags()
{
ifstream file("world/shore/TyleFlags.lr",ios::binary);
file.read ((char *)grid.TyleInfo, tyleBGFlagsSize*tyleBGFlagsSize*4);
file.close (); 

/*int i=tyleBGFlagsSize*tyleBGFlagsSize;
while (i){
grid.TyleInfo[i/tyleBGFlagsSize][i % tyleBGFlagsSize]=10;
i--;
}*/

}
void SaveTyleFlags(){
ofstream file ("world/shore/TyleFlags.lr", ios::binary);	
file.write ((char *)grid.TyleInfo, tyleBGFlagsSize*tyleBGFlagsSize*4);
file.close (); 
}
void StoreIndexedImages()
{
 ofstream file ("pictures/TerrainBrushes/ConvertedIndBrushes.lr", ios::binary);	
for (int i=0; i<TerrainBrushes; i++)
	if (tera.IndDecay[i].pixCount)
	{
file.write ((char *)&i, 2);
file.write ((char *)&tera.IndDecay[i].w, 2);
file.write ((char *)&tera.IndDecay[i].h, 2);
file.write ((char *)&tera.IndDecay[i].pixCount, 4);
file.write ((char *)tera.IndDecay[i].pixels, tera.IndDecay[i].pixCount);

	}
 file.close (); 
}
void StoreHeightImages()
{
 ofstream file ("pictures/TerrainBrushes/ConvertedHeight.lr", ios::binary);	
for (int i=0; i<TerrainBrushes; i++)
	if (tera.height[i].DnA)
	{
file.write ((char *)&i, 2);
file.write ((char *)&tera.height[i].w, 2);
file.write ((char *)&tera.height[i].h, 2);
int size=tera.height[i].w*tera.height[i].h;
file.write ((char *)tera.height[i].DnA, size);
	}
 file.close (); 
}
void StoreDHImages()
{
 ofstream file ("pictures/TerrainBrushes/ConvertedDH.lr", ios::binary);	
for (int i=0; i<TerrainBrushes; i++)
	if (tera.DHheight[i].DnA)
	{
file.write ((char *)&i, 2);
file.write ((char *)&tera.DHheight[i].w, 2);
file.write ((char *)&tera.DHheight[i].h, 2);
int size=tera.DHheight[i].w*tera.DHheight[i].h;
file.write ((char *)tera.DHheight[i].DnA, size);
	}
 file.close (); 
}
bool loadanyAlphaIndex (MyIndexedAlphaSurface* converted)
{
std::string filename=converted->location;
SDL_Surface * dtemp;
dtemp = IMG_Load( filename.c_str()   );
if (!dtemp) return 0;
//MyIndexedAlphaSurface* converted=image;
converted->h=dtemp->h; converted->w=dtemp->w;
Uint32* pixel=(Uint32*)dtemp->pixels;
Uint32 pixelCount=0;
bool untransparent=0; 
int consecutiveFilled=0;
int consecutiveTransparent=0;
for (int j=0; j<dtemp->h; j++)
	for (int i=0; i<dtemp->w; i++)
	{
if ((*pixel>>24)>16) 
{
	if (!untransparent) {pixelCount+=2; consecutiveTransparent=0;} 
	pixelCount++;
	consecutiveFilled++;
	if (consecutiveFilled>=255){ pixelCount+=2; consecutiveFilled=0; }
	untransparent=1;
} else {untransparent=0; consecutiveFilled=0; consecutiveTransparent++; if (consecutiveTransparent>=255) {pixelCount+=2; consecutiveTransparent=0;}}

		pixel++;
	}
consecutiveTransparent=0;
consecutiveFilled=0;
converted->pixels=new Uint8 [pixelCount]; converted->pixCount=pixelCount; 
pixel=(Uint32*)dtemp->pixels;
Uint8* NewPixel=converted->pixels;
untransparent=0; 
int gap=0;
Uint32 fill=0;
Uint8* store;
for (int j=0; j<dtemp->h; j++)
	for (int i=0; i<dtemp->w; i++)
	{
if ((*pixel>>24)>16) 
{
	if (!untransparent) {store=NewPixel; *NewPixel=gap; NewPixel+=2;}
	*NewPixel=(*pixel>>24);
	NewPixel++;
	gap=0;
	fill++;
	if (fill>=255) { *(store+1)=fill; 
		store=NewPixel; *NewPixel=0; *(store+1)=0; NewPixel+=2; fill=0; }
	untransparent=1;
} else {
	untransparent=0;
	if (fill) *(store+1)=fill;
	gap++;
	if (gap>=255) {
	store=NewPixel; 
	*(store+1)=0; *NewPixel=255;  NewPixel+=2; gap=0; }
	fill=0;
}
pixel++;
}
if (fill) *(store+1)=fill;
return 1;
}
bool ImageAlphaToDirLightImage(DirectionalLightetImage* dlm){
SDL_Surface * img;
if (dlm->DnA) return 1;
img = IMG_Load( dlm->location.c_str()   );
if (img){
	dlm->h=img->h;
	dlm->w=img->w;
	dlm->DnA=ImageAlphaToUint8Mask(img);
	return 1;
}
return 0;
}

void ApplyWater(){
worldcoordinates temp;
MouseToPos (&temp);
ToggleTyleFlag(temp.tx, temp.ty, grid.BGWaterMask);
SaveTerrain(temp.tx, temp.ty);
redrowupperstuff (temp.tx, temp.ty, tylextogrid(temp.tx), tyleytogrid(temp.ty));
}
void ApplyLava(){
worldcoordinates temp;
MouseToPos (&temp);
ToggleTyleFlag(temp.tx, temp.ty, grid.BGLavaMask);
SaveTerrain(temp.tx, temp.ty);
redrowupperstuff (temp.tx, temp.ty, tylextogrid(temp.tx), tyleytogrid(temp.ty));
}
// Drow Functions
void DrowPieceOfSurface(Uint32* src, int srcw,int w, int x, int y){
	//Uint32* src=sfc->pixels;
	Uint32* dest=(Uint32*)screen->pixels+y*screen->w + x;
	
	int i=w;
	while (i){
		memcpy(dest, src, w*4);
		dest+=screen->w;
		src+=srcw;
	i--;
	}
}
void DrowIndTool (MyIndexedAlphaSurface* image, Surface* Decay, int x, int y, SDL_Surface* screen, float alpha)
{
int srcx=1024-32;
int srcy=1024-32;
if ((x>screen->w) || (x+image->w<0) || (y>screen->h) || (y+image->h<0)) return;
if ((srcx>Decay->w) || (srcx+image->w<0) || (srcy>Decay->h) || (srcy+image->h<0)) return;


Uint8 * __restrict  src=image->pixels;
Uint32 * __restrict  scrpix;
Uint32 * __restrict  dcpix;
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
if (imgy+srcy>=Decay->h) return; 

int imgx=total % image->w;
int drow=min(left, image->w-imgx);

if ((imgy<-y) || (imgy<-srcy)) 
	skip=drow;
else
{
skip=0;

if ((imgx+x>=screen->w) || (imgx+srcx>=Decay->w)) skip=drow;
else
{
if (imgx+drow+x>screen->w) drow=(screen->w-imgx-x);
if (imgx+drow+srcx>Decay->w) drow=(Decay->w-imgx-srcx);

if (imgx<-x) skip=min(-(imgx+x),drow);
if (imgx<-srcx) skip=max(skip,min(-(imgx+srcx),drow));
}
}
if (skip)
{
	 drow-=skip; i+=skip; src+=skip; left-=skip; total+=skip; imgx+=skip;
}

scrpix=(Uint32*)screen->pixels + (imgy+y)*screen->w+imgx+x;
dcpix=(Uint32*)Decay->pixels + (imgy+srcy)*Decay->w+imgx+srcx;
left-=drow;
total+=drow;
for (int j=0; j<drow;)
{
int AH=*src*(*dcpix>>24)*alpha/255;
AH=min(AH,255);
register int idst = 0;//*scrpix;
register int isrc = *dcpix;
*scrpix = ((RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * AH) >>8))) 
		 | (GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * AH) >>8))) 
		| (BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * AH) >>8))))
         | (*scrpix & 0xFF000000);
	scrpix++;
	dcpix++;
	src++;
	i++;
	j++;
}

}
}

}

// Indexed
void DrowMyIndRiseGround (MyIndexedAlphaSurface* image, int x, int y, SDL_Surface* screen, float alpha)
{
if ((x>screen->w) || (x+image->w<0) || (y>screen->h) || (y+image->h<0)) return;

Uint8 * __restrict  src=image->pixels;
Uint32 * __restrict  scrpix;
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

scrpix=(Uint32*)screen->pixels + (imgy+y)*screen->w+imgx+x;
left-=drow;
total+=drow;
//int j=drow;
//while (j)
for (int j=0; j<drow;)
{
int AH=*src*alpha/8;
register int idst = *scrpix>>24;
*scrpix =  (*scrpix & deAlphaMask) | ((Uint32)max(0,idst-AH)<<24);
	scrpix++;
	src++;
	i++;
	j++;
}

}
}

}
void DrowMyIndLowerGround (MyIndexedAlphaSurface* image, int x, int y, SDL_Surface* screen, float alpha)
{
if ((x>screen->w) || (x+image->w<0) || (y>screen->h) || (y+image->h<0)) return;

Uint8 * __restrict  src=image->pixels;
Uint32 * __restrict  scrpix;
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

scrpix=(Uint32*)screen->pixels + (imgy+y)*screen->w+imgx+x;
left-=drow;
total+=drow;
//int j=drow;
//while (j)
for (int j=0; j<drow;)
{
int AH=*src*alpha/8;
register int idst = *scrpix>>24;
*scrpix =  (*scrpix & deAlphaMask) | ((Uint32)min(255,idst+AH)<<24);
	scrpix++;
	src++;
	i++;
	j++;
}

}
}

}

void DrowMyIndAlphaWithUpper (MyIndexedAlphaSurface* image, int srcx, int srcy, Surface* Decay, Surface* upDecay, int x, int y, SDL_Surface* screen, SDL_Surface* upscreen, float alpha)
{
if ((x>screen->w) || (x+image->w<0) || (y>screen->h) || (y+image->h<0)) return;
if ((srcx>Decay->w) || (srcx+image->w<0) || (srcy>Decay->h) || (srcy+image->h<0)) return;

Uint8 * __restrict  src=image->pixels;
Uint32 * __restrict  scrpix;
Uint32 * __restrict  upscrpix;
Uint32 * __restrict  dcpix;
Uint32 * __restrict  updcpix;
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
if (imgy+srcy>=Decay->h) return; 

int imgx=total % image->w;
int drow=min(left, image->w-imgx);

if ((imgy<-y) || (imgy<-srcy)) 
	skip=drow;
else
{
skip=0;

if ((imgx+x>=screen->w) || (imgx+srcx>=Decay->w)) skip=drow;
else
{
if (imgx+drow+x>screen->w) drow=(screen->w-imgx-x);
if (imgx+drow+srcx>Decay->w) drow=(Decay->w-imgx-srcx);

if (imgx<-x) skip=min(-(imgx+x),drow);
if (imgx<-srcx) skip=max(skip,min(-(imgx+srcx),drow));
}
}
if (skip)
{
	 drow-=skip; i+=skip; src+=skip; left-=skip; total+=skip; imgx+=skip;
}

scrpix=(Uint32*)screen->pixels + (imgy+y)*screen->w+imgx+x;
upscrpix=(Uint32*)upscreen->pixels + (imgy+y)*upscreen->w+imgx+x;
dcpix=(Uint32*)Decay->pixels + (imgy+srcy)*Decay->w+imgx+srcx;
updcpix=(Uint32*)upDecay->pixels + (imgy+srcy)*upDecay->w+imgx+srcx;
left-=drow;
total+=drow;
//int j=drow;
//while (j)
for (int j=0; j<drow;)
{
int diff=(*scrpix>>24)+32-191;
if (diff>0){
int AH=*src*diff*alpha/32;
AH=min(255,AH);
*((Uint8 * )upscrpix+3) = (((*((Uint8 * )updcpix+3) -  *((Uint8 * )upscrpix+3)) * (AH))/255 + *((Uint8 * )upscrpix+3));
RegularBlit(upscrpix, *updcpix,  AH);
}
if (*dcpix>0x0F000000){
int AH=*src*(*dcpix>>24)*alpha/255;
AH=min(255,AH);
register int idst = *scrpix;
register int isrc = *dcpix;
*scrpix = ((RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * AH) >>8))) 
		 | (GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * AH) >>8))) 
		| (BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * AH) >>8))))
         | (*scrpix & 0xFF000000);
}

	scrpix++;
	upscrpix++;
	dcpix++;
	updcpix++;
	src++;
	i++;
	j++;
}

}
}

}
void DrowMyIndAlpha (MyIndexedAlphaSurface* image, int srcx, int srcy, Surface* Decay, int x, int y, SDL_Surface* screen, float alpha)
{
if ((x>screen->w) || (x+image->w<0) || (y>screen->h) || (y+image->h<0)) return;
if ((srcx>Decay->w) || (srcx+image->w<0) || (srcy>Decay->h) || (srcy+image->h<0)) return;

Uint8 * __restrict  src=image->pixels;
Uint32 * __restrict  scrpix;
Uint32 * __restrict  dcpix;
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
if (imgy+srcy>=Decay->h) return; 

int imgx=total % image->w;
int drow=min(left, image->w-imgx);

if ((imgy<-y) || (imgy<-srcy)) 
	skip=drow;
else
{
skip=0;

if ((imgx+x>=screen->w) || (imgx+srcx>=Decay->w)) skip=drow;
else
{
if (imgx+drow+x>screen->w) drow=(screen->w-imgx-x);
if (imgx+drow+srcx>Decay->w) drow=(Decay->w-imgx-srcx);

if (imgx<-x) skip=min(-(imgx+x),drow);
if (imgx<-srcx) skip=max(skip,min(-(imgx+srcx),drow));
}
}
if (skip)
{
	 drow-=skip; i+=skip; src+=skip; left-=skip; total+=skip; imgx+=skip;
}

scrpix=(Uint32*)screen->pixels + (imgy+y)*screen->w+imgx+x;
dcpix=(Uint32*)Decay->pixels + (imgy+srcy)*Decay->w+imgx+srcx;
left-=drow;
total+=drow;
//int j=drow;
//while (j)
for (int j=0; j<drow;)
{
if (*dcpix>0x0F000000){
int AH=*src*(*dcpix>>24)*alpha/255;
AH=AH>128 ? 255 : AH < 64 ? 0 : (AH-64) * 4;
//AH=min(255,AH);
register int idst = *scrpix;
register int isrc = *dcpix;
*scrpix = ((RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * AH) >>8))) 
		 | (GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * AH) >>8))) 
		| (BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * AH) >>8))))
         | (*scrpix & 0xFF000000);
}
	scrpix++;
	dcpix++;
	src++;
	i++;
	j++;
}

}
}

}
void DrowMyInd (MyIndexedAlphaSurface* image, int srcx, int srcy, Surface* Decay, int x, int y, SDL_Surface* screen)
{
if ((x>screen->w) || (x+image->w<0) || (y>screen->h) || (y+image->h<0)) return;

Uint8 * __restrict  src=image->pixels;
Uint32 * __restrict  scrpix;
Uint32 * __restrict  dcpix;
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
if (imgy+srcy>=Decay->h) return; 

int imgx=total % image->w;
int drow=min(left, image->w-imgx);

if ((imgy<-y) || (imgy<-srcy)) 
	skip=drow;
else
{
skip=0;

if ((imgx+x>=screen->w) || (imgx+srcx>=Decay->w)) skip=drow;
else
{
if (imgx+drow+x>screen->w) drow=(screen->w-imgx-x);
if (imgx+drow+srcx>Decay->w) drow=(Decay->w-imgx-srcx);

if (imgx<-x) skip=min(-(imgx+x),drow);
if (imgx<-srcx) skip=max(skip,min(-(imgx+srcx),drow));
}
}
if (skip)
{
	 drow-=skip; i+=skip; src+=skip; left-=skip; total+=skip; imgx+=skip;
}

scrpix=(Uint32*)screen->pixels + (imgy+y)*screen->w+imgx+x;
dcpix=(Uint32*)Decay->pixels + (imgy+srcy)*Decay->w+imgx+srcx;
left-=drow;
total+=drow;
//int j=drow;
//while (j)
for (int j=0; j<drow;)
{
if (*dcpix>0x0F000000){
int AH=*src*(*dcpix>>24)/255;
register int idst = *scrpix;
register int isrc = *dcpix;
*scrpix = ((RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * AH) >>8))) 
		 | (GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * AH) >>8))) 
		| (BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * AH) >>8))))
         | (*scrpix & 0xFF000000);
}
	scrpix++;
	dcpix++;
	src++;
	i++;
	j++;
}

}
}

}
void DrowMyIndOutline (MyIndexedAlphaSurface* image, int srcx, int srcy, Surface* Decay, int x, int y, SDL_Surface* screen)
{
if ((x>screen->w) || (x+image->w<0) || (y>screen->h) || (y+image->h<0)) return;

Uint8 * __restrict  src=image->pixels;
Uint32 * __restrict  scrpix;
Uint32 * __restrict  dcpix;
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
bool OutOfLine=0;
////// Border calculation:
while (left)
{
OutOfLine=0;
int imgy=total/image->w;
if (imgy+y>=screen->h) return; 
if ((imgy+y>=tera.Outline.h) && (tera.Outline.h)) { if (!tera.dx) return; OutOfLine=1;}

if (imgy+srcy>=Decay->h) return; 

int imgx=total % image->w;
int drow=min(left, image->w-imgx);
skip=0;
if ((imgy+y<=tera.Outline.y) && (tera.Outline.y)) { OutOfLine=1; if (!tera.dx) skip=drow;}


if ((OutOfLine) || (!tera.dy)){
	if (tera.Outline.w){
		if (imgx+x>=tera.Outline.w)  skip=drow;
		else 
			drow=min(drow, tera.Outline.w-imgx-x);
	}else
	if (tera.Outline.x){
		if ((imgx+x<=tera.Outline.x)) 
			skip=min(drow,tera.Outline.x-imgx-x);
	}else
	skip=drow;
}

if (skip!=drow)
if ((imgy<-y) || (imgy<-srcy))
	skip=drow;
else
{

if ((imgx+x>=screen->w) || (imgx+srcx>=Decay->w)) skip=drow;
else
{
if (imgx+drow+x>screen->w) drow=(screen->w-imgx-x);
if (imgx+drow+srcx>Decay->w) drow=(Decay->w-imgx-srcx);

if (imgx<-x) skip=max(skip,min(-(imgx+x),drow));
if (imgx<-srcx) skip=max(skip,min(-(imgx+srcx),drow));
}
}
if (skip)
{
	 drow-=skip; i+=skip; src+=skip; left-=skip; total+=skip; imgx+=skip;
}

scrpix=(Uint32*)screen->pixels + (imgy+y)*screen->w+imgx+x;
dcpix=(Uint32*)Decay->pixels + (imgy+srcy)*Decay->w+imgx+srcx;
left-=drow;
total+=drow;
//int j=drow;
//while (j)
for (int j=0; j<drow;)
{
if (*dcpix>0x0F000000){
int AH=*src*(*dcpix>>24)/255;
register int idst = *scrpix;
register int isrc = *dcpix;
*scrpix = ((RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * AH) >>8))) 
		 | (GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * AH) >>8))) 
		| (BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * AH) >>8))))
         | (*scrpix & 0xFF000000);
}
	scrpix++;
	dcpix++;
	src++;
	i++;
	j++;
}

}
}

}

void DrowDHDecayWithUpOutlineZoomed(int DecaySurface, int dsx, int dsy, int height,  SDL_Surface *layer,  SDL_Surface *uplayer,  int dx, int dy, int h)
{
//	if (uplayer->w!=1024) return;


Uint32 * __restrict sourpic,* __restrict upsourpic, * __restrict destpic, * __restrict updestpic;
Uint8* __restrict hpic;

Surface* decay;//=&tera.DecayBG[DecaySurface];
SDL_Surface* upsrc;
DirectionalLightetImage* hsrc;

	upsrc=tera.zoomedIMG.uppermap[height];
	hsrc=&tera.zoomedIMG.height[height];
	decay=tera.DecayZoomBG[DecaySurface].pixels ? &tera.DecayZoomBG[DecaySurface] :  &tera.DecayBG[DecaySurface];
	//decay=&tera.DecayBG[DecaySurface];




	int x=0;
	int y=0;
	if (dx<0) x-=dx;
	if (dsx<0) x=max(x,-dsx);
	if (dy<0) y-=dy;
	if (dsy<0) y=max(y,-dsy);
	int destx=upsrc->w;
	int desty=upsrc->h;

int maxy=layer->h;
if (tera.dy>0) maxy=tera.dy; else
if (y+dy<layer->h+tera.dy) y=layer->h+tera.dy-dy;

	if (dx+destx>layer->w) destx=(layer->w-dx);
	if (dy+desty>maxy) desty=(maxy-dy);
	if (dsx+destx>decay->w) destx=(decay->w-dsx);
	if (dsy+desty>decay->h) desty=(decay->h-dsy);

Uint8 AH;
int diff;
for (y; y<desty; y++)
{
	upsourpic=(Uint32*) upsrc->pixels  + y*upsrc->w + x;
	hpic=hsrc->DnA  + y*hsrc->w + x;
	destpic=(Uint32*)layer->pixels  + (dy+y)*layer->w + dx+x;
	updestpic=(Uint32*) uplayer->pixels  + (dy+y)*uplayer->w + dx+x;
	sourpic=decay->pixels+(dsy+y)*decay->w + dsx+x;
	for (int xx=x; xx<destx; xx++)
		{
diff=(*destpic>>24)-*hpic-h;
	if (diff<=0)
	{

		*((Uint8 * )destpic+3)=*hpic+h;

		if (diff>-32)
		{

AH=-diff*8;
if (AH)
{
*((Uint8 * )updestpic+3) = (((*((Uint8 * )upsourpic+3) -  *((Uint8 * )updestpic+3)) * (AH))/255 + *((Uint8 * )updestpic+3));
RegularBlit(updestpic, *upsourpic,  AH);
}
		}
		else
		{
AH=255;
*updestpic=*upsourpic;
		}
	RegularBlit(destpic, *sourpic,  AH);
	}

sourpic++;
destpic++;
upsourpic++;
updestpic++;
hpic++;
		}
}
	
// X part 

x=0;
y=0;
if (dx<0) x-=dx;
if (dsx<0) x=max(x,-dsx);
if (dy<0) y-=dy;
if (dsy<0) y=max(y,-dsy);
destx=upsrc->w;
desty=upsrc->h;

int maxx=layer->w;
//maxy=layer->h;
if (tera.dx>0) maxx=tera.dx; else
if (x+dx<layer->w+tera.dx-1) x=layer->w+tera.dx-dx-1;

//if (tera.dy>0) maxy=layer->h-tera.dx; else
//if (x+dx<layer->w+tera.dx) x=layer->w+tera.dx-dx;

	if (dx+destx>maxx) destx=(maxx-dx);
	if (dy+desty>layer->h) desty=(layer->h-dy);
	if (dsx+destx>decay->w) destx=(decay->w-dsx);
	if (dsy+desty>decay->h) desty=(decay->h-dsy);

for (y; y<desty; y++)
{
	upsourpic=(Uint32*) upsrc->pixels  + y*upsrc->w + x;
	hpic=hsrc->DnA  + y*hsrc->w + x;
	destpic=(Uint32*)layer->pixels  + (dy+y)*layer->w + dx+x;
	updestpic=(Uint32*) uplayer->pixels  + (dy+y)*uplayer->w + dx+x;
	sourpic=decay->pixels+(dsy+y)*decay->w + dsx+x;
	for (int xx=x; xx<destx; xx++)
		{
diff=(*destpic>>24)-*hpic-h;
	if (diff<=0)
	{

		*((Uint8 * )destpic+3)=*hpic+h;

		if (diff>-32)
		{

AH=-diff*8;
if (AH)
{
*((Uint8 * )updestpic+3) = (((*((Uint8 * )upsourpic+3) -  *((Uint8 * )updestpic+3)) * (AH))/255 + *((Uint8 * )updestpic+3));
RegularBlit(updestpic, *upsourpic,  AH);
}
		}
		else
		{
AH=255;
*updestpic=*upsourpic;
		}
	RegularBlit(destpic, *sourpic,  AH);
	}

sourpic++;
destpic++;
upsourpic++;
updestpic++;
hpic++;
		}
}

}
void DrowDHDecayWithUpZoomed(int DecaySurface, int dsx, int dsy, int height,  SDL_Surface *layer,  SDL_Surface *uplayer,  int dx, int dy, int h)
{
//	if (uplayer->w!=1024) return;
Uint32 * __restrict sourpic,* __restrict upsourpic, * __restrict destpic, * __restrict updestpic;
Uint8* __restrict hpic;

Surface* decay;//=&tera.DecayBG[DecaySurface];
SDL_Surface* upsrc;
DirectionalLightetImage* hsrc;

	upsrc=tera.zoomedIMG.uppermap[height];
	hsrc=&tera.zoomedIMG.height[height];
	decay=tera.DecayZoomBG[DecaySurface].pixels ? &tera.DecayZoomBG[DecaySurface] :  &tera.DecayBG[DecaySurface];
	//decay=&tera.DecayBG[DecaySurface];


	int x=0;
	int y=0;
	if (dx<0) x-=dx;
	if (dsx<0) x=max(x,-dsx);
	if (dy<0) y-=dy;
	if (dsy<0) y=max(y,-dsy);
	int destx=upsrc->w;
	int desty=upsrc->h;
	if (dx+destx>layer->w) destx=(layer->w-dx);
	if (dy+desty>layer->h) desty=(layer->h-dy);
	if (dsx+destx>decay->w) destx=(decay->w-dsx);
	if (dsy+desty>decay->h) desty=(decay->h-dsy);
Uint8 AH;
int diff;
for (y; y<desty; y++)
{
	upsourpic=(Uint32*) upsrc->pixels  + y*upsrc->w + x;
	hpic=hsrc->DnA  + y*hsrc->w + x;
	destpic=(Uint32*)layer->pixels  + (dy+y)*layer->w + dx+x;
	updestpic=(Uint32*) uplayer->pixels  + (dy+y)*uplayer->w + dx+x;
	sourpic=decay->pixels+(dsy+y)*decay->w + dsx+x;

	for (int xx=x; xx<destx; xx++)
		{

diff=(*destpic>>24)-*hpic-h;
	if (diff<=0)
	{

		*((Uint8 * )destpic+3)=*hpic+h;

		if (diff>-32)
		{

AH=-diff*8;
if (AH)
{
*((Uint8 * )updestpic+3) = (((*((Uint8 * )upsourpic+3) -  *((Uint8 * )updestpic+3)) * (AH))/255 + *((Uint8 * )updestpic+3));
RegularBlit(updestpic, *upsourpic,  AH);
}
		}
		else
		{
AH=255;
*updestpic=*upsourpic;
		}
	RegularBlit(destpic, *sourpic,  AH);


	}


sourpic++;
destpic++;
upsourpic++;
updestpic++;
hpic++;

		}
}
		
}
void DrowDHDecayWithUp(int DecaySurface,  int height,  SDL_Surface *layer,  SDL_Surface *uplayer,  int dx, int dy, int h)
{
	if (uplayer->w!=1024) return;
Uint32 * __restrict sourpic,* __restrict upsourpic, * __restrict destpic, * __restrict updestpic;
Uint8* __restrict hpic;

Surface* decay=&tera.DecayBG[DecaySurface];
SDL_Surface* upsrc;//=tera.uppermap[height];
DirectionalLightetImage* hsrc;//=&tera.height[height];

	upsrc=tera.uppermap[height];
	hsrc=&tera.height[height];



	int x=0;
	int y=0;
	if (dx<0) x-=dx;
	if (dy<0) y-=dy;
	int destx=upsrc->w;
	int desty=upsrc->h;
	if (dx+destx>layer->w) destx=(layer->w-dx);
	if (dy+desty>layer->h) desty=(layer->h-dy);
Uint8 AH;
int diff;
for (y; y<desty; y++)
{
	upsourpic=(Uint32*) upsrc->pixels  + y*upsrc->w + x;
	hpic=hsrc->DnA  + y*hsrc->w + x;
	destpic=(Uint32*)layer->pixels  + (dy+y)*layer->w + dx+x;
	updestpic=(Uint32*) uplayer->pixels  + (dy+y)*uplayer->w + dx+x;
	sourpic=decay->pixels+(dy+y)*decay->w + dx+x;

	for (int xx=x; xx<destx; xx++)
		{

diff=(*destpic>>24)-*hpic-h;
	if (diff<=0)
	{

		*((Uint8 * )destpic+3)=*hpic+h;

		if (diff>-32)
		{

AH=-diff*8;
if (AH)
{
*((Uint8 * )updestpic+3) = (((*((Uint8 * )upsourpic+3) -  *((Uint8 * )updestpic+3)) * (AH))/255 + *((Uint8 * )updestpic+3));
RegularBlit(updestpic, *upsourpic,  AH);
}
		}
		else
		{
AH=255;
*updestpic=*upsourpic;
		}
	RegularBlit(destpic, *sourpic,  AH);


	}

//}
sourpic++;
destpic++;
upsourpic++;
updestpic++;
hpic++;

		}
}
		
}


void DrowHeightDecayWithUpOutlineZoomed(int DecaySurface, int dsx, int dsy, int height,  SDL_Surface *layer,  SDL_Surface *uplayer,  int dx, int dy, int h)
{
//	if (uplayer->w!=1024) return;
		if (*tera.height[height].DnA<128) {
DrowDHDecayWithUpOutlineZoomed( DecaySurface, dsx,  dsy,  height,  layer,  uplayer,   dx,  dy,  h);
		return;
	}

Uint32 * __restrict sourpic,* __restrict upsourpic, * __restrict destpic, * __restrict updestpic;
Uint8* __restrict hpic;

Surface* decay;//=&tera.DecayBG[DecaySurface];
SDL_Surface* upsrc;
DirectionalLightetImage* hsrc;

	upsrc=tera.zoomedIMG.uppermap[height];
	hsrc=&tera.zoomedIMG.height[height];
	decay=tera.DecayZoomBG[DecaySurface].pixels ? &tera.DecayZoomBG[DecaySurface] :  &tera.DecayBG[DecaySurface];





	int x=0;
	int y=0;
	if (dx<0) x-=dx;
	if (dsx<0) x=max(x,-dsx);
	if (dy<0) y-=dy;
	if (dsy<0) y=max(y,-dsy);
	int destx=upsrc->w;
	int desty=upsrc->h;

int maxy=layer->h;
if (tera.dy>0) maxy=tera.dy; else
if (y+dy<layer->h+tera.dy) y=layer->h+tera.dy-dy;

	if (dx+destx>layer->w) destx=(layer->w-dx);
	if (dy+desty>maxy) desty=(maxy-dy);
	if (dsx+destx>decay->w) destx=(decay->w-dsx);
	if (dsy+desty>decay->h) desty=(decay->h-dsy);

Uint8 AH;
int diff;
for (y; y<desty; y++)
{
	upsourpic=(Uint32*) upsrc->pixels  + y*upsrc->w + x;
	hpic=hsrc->DnA  + y*hsrc->w + x;
	destpic=(Uint32*)layer->pixels  + (dy+y)*layer->w + dx+x;
	updestpic=(Uint32*) uplayer->pixels  + (dy+y)*uplayer->w + dx+x;
	sourpic=decay->pixels+(dsy+y)*decay->w + dsx+x;
	for (int xx=x; xx<destx; xx++)
		{
diff=(*destpic>>24)-*hpic+h;
	if (diff>=0)	{
		*((Uint8 * )destpic+3)=*hpic-h;
		if (diff<32)		{
AH=diff*8;
if (AH){
*((Uint8 * )updestpic+3) = (((*((Uint8 * )upsourpic+3) -  *((Uint8 * )updestpic+3)) * (AH))/255 + *((Uint8 * )updestpic+3));
RegularBlit(updestpic, *upsourpic,  AH);
}
		}else{
AH=255;
*updestpic=*upsourpic;
		}
	RegularBlit(destpic, *sourpic,  AH);
	}


sourpic++;
destpic++;
upsourpic++;
updestpic++;
hpic++;
		}
}
	
// X part 

x=0;
y=0;
if (dx<0) x-=dx;
if (dsx<0) x=max(x,-dsx);
if (dy<0) y-=dy;
if (dsy<0) y=max(y,-dsy);
destx=upsrc->w;
desty=upsrc->h;

int maxx=layer->w;
//maxy=layer->h;
if (tera.dx>0) maxx=tera.dx; else
if (x+dx<layer->w+tera.dx-1) x=layer->w+tera.dx-dx-1;

//if (tera.dy>0) maxy=layer->h-tera.dx; else
//if (x+dx<layer->w+tera.dx) x=layer->w+tera.dx-dx;

	if (dx+destx>maxx) destx=(maxx-dx);
	if (dy+desty>layer->h) desty=(layer->h-dy);
	if (dsx+destx>decay->w) destx=(decay->w-dsx);
	if (dsy+desty>decay->h) desty=(decay->h-dsy);

for (y; y<desty; y++)
{
	upsourpic=(Uint32*) upsrc->pixels  + y*upsrc->w + x;
	hpic=hsrc->DnA  + y*hsrc->w + x;
	destpic=(Uint32*)layer->pixels  + (dy+y)*layer->w + dx+x;
	updestpic=(Uint32*) uplayer->pixels  + (dy+y)*uplayer->w + dx+x;
	sourpic=decay->pixels+(dsy+y)*decay->w + dsx+x;
	for (int xx=x; xx<destx; xx++)
		{
diff=(*destpic>>24)-*hpic+h;
	if (diff>=0)	{
		*((Uint8 * )destpic+3)=*hpic-h;
		if (diff<32)		{
AH=diff*8;
if (AH){
*((Uint8 * )updestpic+3) = (((*((Uint8 * )upsourpic+3) -  *((Uint8 * )updestpic+3)) * (AH))/255 + *((Uint8 * )updestpic+3));
RegularBlit(updestpic, *upsourpic,  AH);
}
		}else{
AH=255;
*updestpic=*upsourpic;
		}
	RegularBlit(destpic, *sourpic,  AH);
	}


sourpic++;
destpic++;
upsourpic++;
updestpic++;
hpic++;
		}
}

}
void DrowHeightDecayWithUp(int DecaySurface, int dsx, int dsy, int height,  SDL_Surface *layer,  SDL_Surface *uplayer,  int dx, int dy, int h, bool zoomed)
{

	if ((*tera.height[height].DnA<128) && (zoomed)) {
DrowDHDecayWithUpZoomed( DecaySurface, dsx,  dsy,  height,  layer,  uplayer,   dx,  dy,  h);
		return;
	}
//	if (uplayer->w!=1024) return;
Uint32 * __restrict sourpic,* __restrict upsourpic, * __restrict destpic, * __restrict updestpic;
Uint8* __restrict hpic;

Surface* decay;//=&tera.DecayBG[DecaySurface];
SDL_Surface* upsrc;
DirectionalLightetImage* hsrc;
if (zoomed){
	upsrc=tera.zoomedIMG.uppermap[height];
	hsrc=&tera.zoomedIMG.height[height];
	decay=tera.DecayZoomBG[DecaySurface].pixels ? &tera.DecayZoomBG[DecaySurface] :  &tera.DecayBG[DecaySurface];
}
else{
	upsrc=tera.uppermap[height];
	hsrc=&tera.height[height];
	decay=&tera.DecayBG[DecaySurface];
}




	int x=0;
	int y=0;
	if (dx<0) x-=dx;
	if (dsx<0) x=max(x,-dsx);
	if (dy<0) y-=dy;
	if (dsy<0) y=max(y,-dsy);
	int destx=upsrc->w;
	int desty=upsrc->h;
	if (dx+destx>layer->w) destx=(layer->w-dx);
	if (dy+desty>layer->h) desty=(layer->h-dy);
	if (dsx+destx>decay->w) destx=(decay->w-dsx);
	if (dsy+desty>decay->h) desty=(decay->h-dsy);
Uint8 AH;
int diff;
for (y; y<desty; y++)
{
	upsourpic=(Uint32*) upsrc->pixels  + y*upsrc->w + x;
	hpic=hsrc->DnA  + y*hsrc->w + x;
	destpic=(Uint32*)layer->pixels  + (dy+y)*layer->w + dx+x;
	updestpic=(Uint32*) uplayer->pixels  + (dy+y)*uplayer->w + dx+x;
	sourpic=decay->pixels+(dsy+y)*decay->w + dsx+x;

	for (int xx=x; xx<destx; xx++)
		{

diff=(*destpic>>24)-*hpic+h;
	if (diff>=0)
	{

		*((Uint8 * )destpic+3)=*hpic-h;

		if (diff<32)
		{
AH=diff*8;
if (AH)
{
*((Uint8 * )updestpic+3) = (((*((Uint8 * )upsourpic+3) -  *((Uint8 * )updestpic+3)) * (AH))/255 + *((Uint8 * )updestpic+3));
RegularBlit(updestpic, *upsourpic,  AH);
RegularBlit(destpic, *sourpic,  AH);
}
		}
		else
		{
*updestpic=*upsourpic;
*destpic=(*destpic & 0xFF000000) | (*sourpic & 0x00FFFFFF);
		}
	
	}

sourpic++;
destpic++;
upsourpic++;
updestpic++;
hpic++;

		}
}
		
}
void DrowHeightDecayWithSideUp(int DecaySurface, int SideUpSurface, int dsx, int dsy, int height,  SDL_Surface *layer,  SDL_Surface *uplayer,  int dx, int dy, int h)
{
	if (tera.UpperBG[SideUpSurface].pixels == 0 ) return;
Uint32 * __restrict sourpic,* __restrict upsourImg,* __restrict upsourpic, * __restrict destpic, * __restrict updestpic;
Uint8* __restrict hpic;

Surface* decay=&tera.DecayBG[DecaySurface];
SDL_Surface* upsrc=tera.uppermap[height];
Surface* upsurf=&tera.UpperBG[SideUpSurface];
DirectionalLightetImage* hsrc=&tera.height[height];

	int x=0;
	int y=0;
	if (dx<0) x-=dx;
	if (dsx<0) x=max(x,-dsx);
	if (dy<0) y-=dy;
	if (dsy<0) y=max(y,-dsy);
	int destx=hsrc->w;
	int desty=hsrc->h;
	if (dx+destx>layer->w) destx=(layer->w-dx);
	if (dy+desty>layer->h) desty=(layer->h-dy);
	if (dsx+destx>decay->w) destx=(decay->w-dx);
	if (dsy+desty>decay->h) desty=(decay->h-dy);
Uint8 AH;
int diff;
for (y; y<desty; y++)
{
	hpic=hsrc->DnA  + y*hsrc->w + x;
	upsourImg=(Uint32*)upsrc->pixels + y*hsrc->w + x;
	destpic=(Uint32*)layer->pixels  + (dy+y)*layer->w + dx+x;
	updestpic=(Uint32*) uplayer->pixels  + (dy+y)*uplayer->w + dx+x;
	sourpic=decay->pixels+(dsy+y)*decay->w + dsx+x;
	upsourpic=upsurf->pixels  + (dsy+y)*upsurf->w + dsx+x;

	for (int xx=x; xx<destx; xx++)
		{

		diff=(*destpic>>24)-*hpic+h;
	if (diff>=0)
	{
int upperA, upsrfA, AvA; //upsourImg
upperA=*upsourImg>>24;
upsrfA=255-upperA;

*((Uint8 * )destpic+3)=*hpic-h;

	if (diff<32){
			AH=diff*8;
			if (AH){
int destA=*((Uint8 * )updestpic+3);
*((Uint8 * )updestpic+3) = (((AH -  destA) * (AH))/255 + destA);
			RegularBlit(destpic, *sourpic,  AH);
			}
		}
		else{
			AH=255;
			*updestpic=*upsourpic;
			*((Uint8 * )updestpic+3) = 255; 
			*destpic=(*destpic & 0xFF000000) | (*sourpic & 0x00FFFFFF);
		}
RegularBlit(updestpic, *upsourImg,  AH*upperA/255);
RegularBlit(updestpic, *upsourpic,  AH*upsrfA/255);


	}

//}
sourpic++;
destpic++;
upsourpic++;
updestpic++;
hpic++;
upsourImg++;

		}
}
		
}

void DrowMyIndZoom (MyIndexedAlphaSurface* image, int srcx, int srcy, Surface* Decay, int x, int y, SDL_Surface* screen, float alpha, bool OutlineOnly)
{
if ((x>screen->w) || (x+image->w*8<0) || (y>screen->h) || (y+image->h*8<0)) return;
while (srcx<0) srcx+=Decay->w;
while (srcy<0) srcy+=Decay->h;
srcx%=Decay->w;
srcy%=Decay->h;
//if ((srcx>Decay->w) || (srcx+image->w*8<0) || (srcy>Decay->h) || (srcy+image->h*8<0)) return;

Uint8 * __restrict  src=image->pixels;
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
int truy=imgy*8;
if (truy+y>=screen->h) return; 
//if (imgy+srcy>=Decay->h) return; 

int imgx=total % image->w;
int trux=imgx*8;
int drow=min(left, image->w-imgx);

if ((truy+16<-y) || (truy+16<-srcy)) 
	skip=drow;
else
{
skip=0;

if (trux+x-16>=screen->w) skip=drow;
else
{
if (trux+drow+x-16>screen->w) drow=(screen->w-trux-x+16);
if (trux+16<-x) skip=min((-trux-x-16)/8,drow);
//if (trux+16<-srcx) skip=max(skip,min(-(trux+srcx)/8,drow));
}
}
if (skip)
{
	 drow-=skip; i+=skip; src+=skip; left-=skip; total+=skip; imgx+=skip;
}

left-=drow;
total+=drow;
for (int j=0; j<drow;)
{
int AH=*src*alpha;
AH=min(255,AH);

/*register int idst = *scrpix;
register int isrc = *dcpix;
*scrpix = ((RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * AH) >>8))) 
		 | (GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * AH) >>8))) 
		| (BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * AH) >>8))))
         | (*scrpix & 0xFF000000);*/
AH=(AH*tera.zoomGrains/256);

int sndx=srcx+(imgx+j)*8;
int sndy=srcy+imgy*8;
while (sndx<0) sndx+=Decay->w;
while (sndx>=Decay->w) sndx-=Decay->w;
while (sndy<0) sndy+=Decay->h;
while (sndy>=Decay->h) sndy-=Decay->h;
sndx%=Decay->w;
sndy%=Decay->h;

//if (sndx>1000) sndx-=1000;
//if (sndy>1000) sndy-=1000;
int fullx=x+(imgx+j)*8;
int fully=y+imgy*8;
if (!OutlineOnly){
DrowMyInd (&tera.ZoomDecay[AH], sndx , sndy, Decay, fullx, fully, screen);
if (sndx>Decay->w-24) DrowMyInd (&tera.ZoomDecay[AH], sndx-Decay->w , sndy, Decay, fullx, fully, screen);
if (sndy>Decay->h-24) DrowMyInd (&tera.ZoomDecay[AH], sndx , sndy-Decay->h, Decay, fullx, fully, screen);
} else if (((fullx<=tera.dx+24) && (tera.dx>0)) || ((fullx>=scrwi+tera.dx-16)&& (tera.dx<0)) 
	|| ((fully<=tera.dy+24) && (tera.dy>0)) || ((fully>=scrhi+tera.dy-16) && (tera.dy<0)))
{
DrowMyIndOutline (&tera.ZoomDecay[AH], sndx , sndy, Decay, fullx, fully, screen);
if (sndx>Decay->w-24) DrowMyIndOutline (&tera.ZoomDecay[AH], sndx-Decay->w , sndy, Decay, fullx, fully, screen);
if (sndy>Decay->h-24) DrowMyIndOutline (&tera.ZoomDecay[AH], sndx , sndy-Decay->h, Decay, fullx, fully, screen);

}

//DrowMyIndOutline
	src++;
	i++;
	j++;
}

}
}

}



void readInGameStroke(sectorStrokes* stroke, int gx, int gy, bool update){
//	
	Surface* DBG=&tera.DecayBG[stroke->surface];

	if (stroke->type==1){
		MyIndexedAlphaSurface* Dimge= &tera.IndDecay[stroke->mask];
		if (tera.UpperBG[stroke->surface].pixels)
DrowMyIndAlphaWithUpper (Dimge,stroke->x,stroke->y ,&tera.DecayBG[stroke->surface], &tera.UpperBG[stroke->surface], stroke->x, stroke->y, storedmp[gx+1][gy+1], uppermp[gx+1][gy+1], stroke->alpha);
		else
DrowMyIndAlpha (Dimge,stroke->x,stroke->y ,&tera.DecayBG[stroke->surface] ,stroke->x, stroke->y, storedmp[gx+1][gy+1], stroke->alpha);
	 

	if (stroke->x+Dimge->w>DBG->w) 
		if (tera.UpperBG[stroke->surface].pixels)
	 DrowMyIndAlphaWithUpper (Dimge,stroke->x-DBG->w,stroke->y ,&tera.DecayBG[stroke->surface], &tera.UpperBG[stroke->surface], stroke->x, stroke->y, storedmp[gx+1][gy+1], uppermp[gx+1][gy+1], stroke->alpha);
else DrowMyIndAlpha (Dimge,stroke->x-DBG->w,stroke->y ,DBG ,stroke->x, stroke->y, storedmp[gx+1][gy+1], stroke->alpha);

if (stroke->y+Dimge->h>DBG->h) 
		if (tera.UpperBG[stroke->surface].pixels)
	 DrowMyIndAlphaWithUpper (Dimge,stroke->x,stroke->y-DBG->h ,&tera.DecayBG[stroke->surface], &tera.UpperBG[stroke->surface], stroke->x, stroke->y, storedmp[gx+1][gy+1], uppermp[gx+1][gy+1], stroke->alpha);
else DrowMyIndAlpha (Dimge,stroke->x,stroke->y-DBG->h ,DBG ,stroke->x, stroke->y, storedmp[gx+1][gy+1], stroke->alpha);
	
if ((stroke->y+Dimge->h>DBG->h) && (stroke->x+Dimge->w>DBG->w) )
		if (tera.UpperBG[stroke->surface].pixels)
	 DrowMyIndAlphaWithUpper (Dimge,stroke->x-DBG->w,stroke->y-DBG->h ,&tera.DecayBG[stroke->surface], &tera.UpperBG[stroke->surface], stroke->x, stroke->y, storedmp[gx+1][gy+1], uppermp[gx+1][gy+1], stroke->alpha);
else DrowMyIndAlpha (Dimge,stroke->x-DBG->w,stroke->y-DBG->h ,DBG ,stroke->x, stroke->y, storedmp[gx+1][gy+1], stroke->alpha);


if (update)	updateMainMap (stroke->x,stroke->y, tera.IndDecay[stroke->mask].w, tera.IndDecay[stroke->mask].h, gx+1, gy+1);
	}
	else
		if (stroke->type==2){
			SDL_Surface* hei=tera.uppermap[stroke->mask];

			if (*tera.height[stroke->mask].DnA > 128 ){
				DrowHeightDecayWithUp(stroke->surface, stroke->x, stroke->y, stroke->mask, storedmp[gx+1][gy+1],  uppermp[gx+1][gy+1],  stroke->x, stroke->y, stroke->alpha,0);
				
				
				if (stroke->x+hei->w>DBG->w) 
					DrowHeightDecayWithUp (stroke->surface,stroke->x-DBG->w, stroke->y, stroke->mask, storedmp[gx+1][gy+1],  uppermp[gx+1][gy+1],  stroke->x, stroke->y, stroke->alpha,0);
				if (stroke->y+hei->h>DBG->h) 
					DrowHeightDecayWithUp (stroke->surface,stroke->x, stroke->y-DBG->h, stroke->mask, storedmp[gx+1][gy+1],  uppermp[gx+1][gy+1],  stroke->x, stroke->y, stroke->alpha,0);
				if ((stroke->y+hei->h>DBG->h) && (stroke->x+hei->w>DBG->w) )
					DrowHeightDecayWithUp (stroke->surface,stroke->x-DBG->w, stroke->y-DBG->h, stroke->mask, storedmp[gx+1][gy+1],  uppermp[gx+1][gy+1],  stroke->x, stroke->y, stroke->alpha,0);
				if (update) updateMainMap (stroke->x,stroke->y, tera.uppermap[stroke->mask]->w, tera.uppermap[stroke->mask]->h, gx+1, gy+1);
			} else
			{
				DrowDHDecayWithUp(stroke->surface,  stroke->mask,  storedmp[gx+1][gy+1],  uppermp[gx+1][gy+1],  stroke->x, stroke->y, stroke->alpha);
					if (stroke->x+hei->w>DBG->w) 
					DrowDHDecayWithUp(stroke->surface,  stroke->mask,  storedmp[gx+1][gy+1],  uppermp[gx+1][gy+1],  stroke->x-DBG->w, stroke->y, stroke->alpha);
				if (stroke->y+hei->h>DBG->h) 
					DrowDHDecayWithUp(stroke->surface,  stroke->mask,  storedmp[gx+1][gy+1],  uppermp[gx+1][gy+1],  stroke->x, stroke->y-DBG->h, stroke->alpha);
				if ((stroke->y+hei->h>DBG->h) && (stroke->x+hei->w>DBG->w) )
					DrowDHDecayWithUp(stroke->surface,  stroke->mask,  storedmp[gx+1][gy+1],  uppermp[gx+1][gy+1],  stroke->x-DBG->w, stroke->y-DBG->h, stroke->alpha);
				if (update) updateMainMap (stroke->x,stroke->y, tera.uppermap[stroke->mask]->w, tera.uppermap[stroke->mask]->h, gx+1, gy+1);

			}
		
		
		
		}
		else 
			if (stroke->type==3){
				DrowHeightDecayWithSideUp(stroke->surface, stroke->upsurface, stroke->x, stroke->y, stroke->mask, storedmp[gx+1][gy+1],  uppermp[gx+1][gy+1],  stroke->x, stroke->y, stroke->alpha);
	
	SDL_Surface* hei=tera.uppermap[stroke->mask];

	if (stroke->x+hei->w>DBG->w) 
		DrowHeightDecayWithSideUp(stroke->surface, stroke->upsurface,stroke->x-DBG->w, stroke->y, stroke->mask, storedmp[gx+1][gy+1],  uppermp[gx+1][gy+1],  stroke->x, stroke->y, stroke->alpha);
	if (stroke->y+hei->h>DBG->h) 
		DrowHeightDecayWithSideUp(stroke->surface, stroke->upsurface,stroke->x, stroke->y-DBG->h, stroke->mask, storedmp[gx+1][gy+1],  uppermp[gx+1][gy+1],  stroke->x, stroke->y, stroke->alpha);
	if ((stroke->y+hei->h>DBG->h) && (stroke->x+hei->w>DBG->w) )
		DrowHeightDecayWithSideUp(stroke->surface, stroke->upsurface,stroke->x-DBG->w, stroke->y-DBG->h, stroke->mask, storedmp[gx+1][gy+1],  uppermp[gx+1][gy+1],  stroke->x, stroke->y, stroke->alpha);
	if (update) updateMainMap (stroke->x,stroke->y, tera.uppermap[stroke->mask]->w, tera.uppermap[stroke->mask]->h, gx+1, gy+1);
			}
			else
				if (stroke->type==4){
		MyIndexedAlphaSurface* Dimge= &tera.IndDecay[stroke->mask];
DrowMyIndLowerGround (Dimge,stroke->x,stroke->y, storedmp[gx+1][gy+1], stroke->alpha);

/*	if (stroke->x+Dimge->w>DBG->w) 
		DrowMyIndAlpha (Dimge,stroke->x-DBG->w,stroke->y ,DBG ,stroke->x, stroke->y, storedmp[gx+1][gy+1], stroke->alpha);
	if (stroke->y+Dimge->h>DBG->h) 
		DrowMyIndAlpha (Dimge,stroke->x,stroke->y-DBG->h ,DBG ,stroke->x, stroke->y, storedmp[gx+1][gy+1], stroke->alpha);
	if ((stroke->y+Dimge->h>DBG->h) && (stroke->x+Dimge->w>DBG->w) )
		DrowMyIndAlpha (Dimge,stroke->x-DBG->w,stroke->y-DBG->h ,DBG ,stroke->x, stroke->y, storedmp[gx+1][gy+1], stroke->alpha);
		*///DrowMyIndRiseGround
if (update)	updateMainMap (stroke->x,stroke->y, tera.IndDecay[stroke->mask].w, tera.IndDecay[stroke->mask].h, gx+1, gy+1);
	}
				else 
						if (stroke->type==5){
		MyIndexedAlphaSurface* Dimge= &tera.IndDecay[stroke->mask];
DrowMyIndRiseGround (Dimge,stroke->x,stroke->y, storedmp[gx+1][gy+1], stroke->alpha);

/*	if (stroke->x+Dimge->w>DBG->w) 
		DrowMyIndAlpha (Dimge,stroke->x-DBG->w,stroke->y ,DBG ,stroke->x, stroke->y, storedmp[gx+1][gy+1], stroke->alpha);
	if (stroke->y+Dimge->h>DBG->h) 
		DrowMyIndAlpha (Dimge,stroke->x,stroke->y-DBG->h ,DBG ,stroke->x, stroke->y, storedmp[gx+1][gy+1], stroke->alpha);
	if ((stroke->y+Dimge->h>DBG->h) && (stroke->x+Dimge->w>DBG->w) )
		DrowMyIndAlpha (Dimge,stroke->x-DBG->w,stroke->y-DBG->h ,DBG ,stroke->x, stroke->y, storedmp[gx+1][gy+1], stroke->alpha);
		*///DrowMyIndRiseGround
if (update)	updateMainMap (stroke->x,stroke->y, tera.IndDecay[stroke->mask].w, tera.IndDecay[stroke->mask].h, gx+1, gy+1);
	}
						else
							if (stroke->type==6){
float ah=(stroke->age & 0xFF); ah/=32;
float ax=((stroke->age & 0xFF00)>>8); ax/=32;
float ay=((stroke->age & 0xFF0000)>>16); ay/=32;
int H=((stroke->age & 0xFF000000)>>24)-65; 
//str->age=(int)(tera.Edit.Ah*64) | ((int)(tera.Edit.Ax*64)<<8) | ((int)(tera.Edit.Ay*64)<<16);

ThreadDrow8bitVex(&curphisobj->pos, &mgvDB.mgvarray[stroke->surface][stroke->upsurface].Converted 
				 , 0, 0, H, stroke->alpha, ax, ay, ah, 0);
int rad=palette.ThreadG8BufferRadius;

stroke->x+=rad;
stroke->y+=rad;

worldcoordinates tmppos;
tmppos.tx=grid.objectx[gx][gy]; tmppos.ty=grid.objecty[gx][gy]; tmppos.x=stroke->x*accuracy; tmppos.y=stroke->y*accuracy;

ThreadLightsTo180arrayF(&tmppos, 0.92,ah,0,deAlphaMask,0);

	int totylex=tylextogrid(tmppos.tx)+1;
	int totyley=tyleytogrid(tmppos.ty)+1;
	
	ToUpperDecayBufferHeight(&palette.ThreadG8BufferRadius,&palette.ThreadBuffer8bit[0],mgvDB.ConvertedPalette[stroke->surface], stroke->x, stroke->y,
						 storedmp[gx+1][gy+1], uppermp[gx+1][gy+1],	&tera.DecayBG[stroke->mask], 1);
stroke->x-=rad;
stroke->y-=rad;

if (update)	RefreshGround (stroke->x, stroke->y, rad*2,rad*2, totylex, totyley);
}

							else if (stroke->type==7){
float ah=(stroke->age & 0xFF); ah/=32;
float ax=((stroke->age & 0xFF00)>>8); ax/=32;
float ay=((stroke->age & 0xFF0000)>>16); ay/=32;
int H=((stroke->age & 0xFF000000)>>24)-65; 

MegavoxBlit (&mgvDB.mgvarray[stroke->surface][stroke->upsurface], stroke->alpha,H, ax, ay, ah);
//ThreadDrow8bitVex(&curphisobj->pos, &mgvDB.mgvarray[stroke->surface][stroke->upsurface].Converted 
//				 , 0, 0, H, stroke->alpha, ax, ay, ah, 0);
int rad=palette.ThreadG8BufferRadius;

stroke->x+=rad;
stroke->y+=rad;

worldcoordinates tmppos;
tmppos.tx=grid.objectx[gx][gy]; tmppos.ty=grid.objecty[gx][gy]; tmppos.x=stroke->x*accuracy; tmppos.y=stroke->y*accuracy;

ThreadLightsTo180arrayF(&tmppos, 0.92,ah,0,deAlphaMask,0);

	int totylex=tylextogrid(tmppos.tx)+1;
	int totyley=tyleytogrid(tmppos.ty)+1;
	
	ToUpperDecayBufferHeight(&palette.ThreadG8BufferRadius, &palette.ThreadBuffer8bit[0],mgvDB.ConvertedPalette[stroke->surface], stroke->x, stroke->y,
						 storedmp[gx+1][gy+1], uppermp[gx+1][gy+1],	&tera.DecayBG[stroke->mask], 1);
stroke->x-=rad;
stroke->y-=rad;

if (update)	RefreshGround (stroke->x, stroke->y, rad*2,rad*2, totylex, totyley);				
							
							

								
/*	MegavoxBlit (&edit.mgvPlay);
	worldcoordinates tmppos;
	MouseToPos(&tmppos);
	LightsTo180arrayF(&tmppos, defAngleFade,tera.Edit.Ah , 0,deAlphaMask,0);
	DrowBufferByHeight(mgvDB.ConvertedPalette[mgvox.curpiece], mouse.pos.x,  mouse.pos.y, screen,1);*/
							}

		
}

//Editor Tools
void drowIngameLower()
{
	mgvox.LastUsedTool=mgvox.LastUsedTool;

	if (tera.Edit.mb[mgvox.LastUsedTool].AnyBrush){
		int br;
		do{ br=rand() % TerrainBrushes;} while (tera.Edit.mb[mgvox.LastUsedTool].brush[br]==0);

if (ipifagor(mouse.pos.x-tera.Edit.drx,mouse.pos.y-tera.Edit.dry)<tera.IndDecay[br].w/2) return;

int tx=cameratylx;
int x=actcamx+mouse.pos.x-tera.IndDecay[br].w/2;

int ty=cameratyly;
int y=actcamy+mouse.pos.y-tera.IndDecay[br].h/2;


// Calculate Pentago

if (br==5) {
	while (x<0) {x+=tyleSize; tx--;}
	while (y<0) {y+=tyleSize; ty--;}
	adjustToPentagon(&x, &y);
	if ((tera.Edit.prevHexX==x) && (tera.Edit.prevHexY==y)) return;
	tera.Edit.prevHexX=x;
	tera.Edit.prevHexY=y;
}

// End Calculate pentagon 

while (x>=tyleSize) {x-=tyleSize; tx++;}
while (x<0) {x+=tyleSize; tx--;}

while (y>=tyleSize) {y-=tyleSize; ty++;}
while (y<0) {y+=tyleSize; ty--;}

for (int dty=0; dty<2; dty++)
for (int dtx=0; dtx<2; dtx++)
if ((x+tera.IndDecay[br].w>=dtx*tyleSize) && (y+tera.IndDecay[br].h>=dty*tyleSize)) 
{
sTyleStroke* tyle=&Terrain[tylextogrid(tx+dtx)][tyleytogrid(ty+dty)];
if (tyle->max>=maxStrokes-1)  return; // make sure to check this for all tyles affected;
}


tera.Edit.drx=mouse.pos.x;
tera.Edit.dry=mouse.pos.y;

for (int dty=0; dty<2; dty++)
for (int dtx=0; dtx<2; dtx++)
if ((x+tera.IndDecay[br].w>=dtx*tyleSize) && (y+tera.IndDecay[br].h>=dty*tyleSize)) 
{
sTyleStroke* tyle=&Terrain[tylextogrid(tx+dtx)][tyleytogrid(ty+dty)];

sectorStrokes* str=&tyle->stroke[tyle->max];
tyle->max++;
tyle->updated=1;
str->age=now;
str->alpha=tera.Edit.mb[mgvox.LastUsedTool].IndAlpha.val; 
str->alpha/=32; 
str->x=x-dtx*tyleSize;
str->y=y-dty*tyleSize;
str->mask=br;
str->surface=tera.Edit.mb[mgvox.LastUsedTool].DecaySfc;
str->type=4;
readInGameStroke(str, tylextogrid(tx+dtx), tyleytogrid(ty+dty), 1);
}
}
return;
}
void drowIngameRise()
{
	mgvox.LastUsedTool=mgvox.LastUsedTool;

	if (tera.Edit.mb[mgvox.LastUsedTool].AnyBrush){
		int br;
		do{ br=rand() % TerrainBrushes;} while (tera.Edit.mb[mgvox.LastUsedTool].brush[br]==0);

if (ipifagor(mouse.pos.x-tera.Edit.drx,mouse.pos.y-tera.Edit.dry)<tera.IndDecay[br].w/2) return;

int tx=cameratylx;
int x=actcamx+mouse.pos.x-tera.IndDecay[br].w/2;

int ty=cameratyly;
int y=actcamy+mouse.pos.y-tera.IndDecay[br].h/2;


// Calculate Pentago

if (br==5) {
	while (x<0) {x+=tyleSize; tx--;}
	while (y<0) {y+=tyleSize; ty--;}
	adjustToPentagon(&x, &y);
	if ((tera.Edit.prevHexX==x) && (tera.Edit.prevHexY==y)) return;
	tera.Edit.prevHexX=x;
	tera.Edit.prevHexY=y;
}

// End Calculate pentagon 

while (x>=tyleSize) {x-=tyleSize; tx++;}
while (x<0) {x+=tyleSize; tx--;}

while (y>=tyleSize) {y-=tyleSize; ty++;}
while (y<0) {y+=tyleSize; ty--;}

for (int dty=0; dty<2; dty++)
for (int dtx=0; dtx<2; dtx++)
if ((x+tera.IndDecay[br].w>=dtx*tyleSize) && (y+tera.IndDecay[br].h>=dty*tyleSize)) 
{
sTyleStroke* tyle=&Terrain[tylextogrid(tx+dtx)][tyleytogrid(ty+dty)];
if (tyle->max>=maxStrokes-1)  return; // make sure to check this for all tyles affected;
}


tera.Edit.drx=mouse.pos.x;
tera.Edit.dry=mouse.pos.y;

for (int dty=0; dty<2; dty++)
for (int dtx=0; dtx<2; dtx++)
if ((x+tera.IndDecay[br].w>=dtx*tyleSize) && (y+tera.IndDecay[br].h>=dty*tyleSize)) 
{
sTyleStroke* tyle=&Terrain[tylextogrid(tx+dtx)][tyleytogrid(ty+dty)];

sectorStrokes* str=&tyle->stroke[tyle->max];
tyle->max++;
tyle->updated=1;
str->age=now;
str->alpha=tera.Edit.mb[mgvox.LastUsedTool].IndAlpha.val; 
str->alpha/=32; 
str->x=x-dtx*tyleSize;
str->y=y-dty*tyleSize;
str->mask=br;
str->surface=tera.Edit.mb[mgvox.LastUsedTool].DecaySfc;
str->type=5;
readInGameStroke(str, tylextogrid(tx+dtx), tyleytogrid(ty+dty), 1);
}
}
return;
}
void drowIngameInd()
{
	mgvox.LastUsedTool=mgvox.LastUsedTool;

	if (tera.Edit.mb[mgvox.LastUsedTool].AnyBrush){
		int br;
		do{ br=rand() % TerrainBrushes;} while (tera.Edit.mb[mgvox.LastUsedTool].brush[br]==0);

if (ipifagor(mouse.pos.x-tera.Edit.drx,mouse.pos.y-tera.Edit.dry)<tera.IndDecay[br].w/2) return;

int tx=cameratylx;
int x=actcamx+mouse.pos.x-tera.IndDecay[br].w/2;

int ty=cameratyly;
int y=actcamy+mouse.pos.y-tera.IndDecay[br].h/2;


// Calculate Pentago

if (br==5) {
	while (x<0) {x+=tyleSize; tx--;}
	while (y<0) {y+=tyleSize; ty--;}
	adjustToPentagon(&x, &y);
	if ((tera.Edit.prevHexX==x) && (tera.Edit.prevHexY==y)) return;
	tera.Edit.prevHexX=x;
	tera.Edit.prevHexY=y;
}

// End Calculate pentagon 

while (x>=tyleSize) {x-=tyleSize; tx++;}
while (x<0) {x+=tyleSize; tx--;}

while (y>=tyleSize) {y-=tyleSize; ty++;}
while (y<0) {y+=tyleSize; ty--;}

for (int dty=0; dty<2; dty++)
for (int dtx=0; dtx<2; dtx++)
if ((x+tera.IndDecay[br].w>=dtx*tyleSize) && (y+tera.IndDecay[br].h>=dty*tyleSize)) 
{
sTyleStroke* tyle=&Terrain[tylextogrid(tx+dtx)][tyleytogrid(ty+dty)];
if (tyle->max>=maxStrokes-1)  return; // make sure to check this for all tyles affected;
}


tera.Edit.drx=mouse.pos.x;
tera.Edit.dry=mouse.pos.y;

for (int dty=0; dty<2; dty++)
for (int dtx=0; dtx<2; dtx++)
if ((x+tera.IndDecay[br].w>=dtx*tyleSize) && (y+tera.IndDecay[br].h>=dty*tyleSize)) 
{
sTyleStroke* tyle=&Terrain[tylextogrid(tx+dtx)][tyleytogrid(ty+dty)];

sectorStrokes* str=&tyle->stroke[tyle->max];
tyle->max++;
tyle->updated=1;
str->age=now;
str->alpha=tera.Edit.mb[mgvox.LastUsedTool].IndAlpha.val; 
str->alpha/=32; 
str->x=x-dtx*tyleSize;
str->y=y-dty*tyleSize;
str->mask=br;
str->surface=tera.Edit.mb[mgvox.LastUsedTool].DecaySfc;
str->type=1;
readInGameStroke(str, tylextogrid(tx+dtx), tyleytogrid(ty+dty), 1);
}
}
return;
}
void drowInGameH(){

int br=tera.Edit.mb[mgvox.LastUsedTool].hdec;

if (ipifagor(mouse.pos.x-tera.Edit.drx,mouse.pos.y-tera.Edit.dry)<tera.uppermap[br]->w/2) return;

int tx=cameratylx;
int x=actcamx+mouse.pos.x-tera.uppermap[br]->w/2;
while (x>=tyleSize) {x-=tyleSize; tx++;}
while (x<0) {x+=tyleSize; tx--;}

int ty=cameratyly;
int y=actcamy+mouse.pos.y-tera.uppermap[br]->h/2;
while (y>=tyleSize) {y-=tyleSize; ty++;}
while (y<0) {y+=tyleSize; ty--;}

for (int dty=0; dty<2; dty++)
for (int dtx=0; dtx<2; dtx++)
if ((x+tera.uppermap[br]->w>=dtx*tyleSize) && (y+tera.uppermap[br]->h>=dty*tyleSize)) 
{
sTyleStroke* tyle=&Terrain[tylextogrid(tx+dtx)][tyleytogrid(ty+dty)];
if (tyle->max>=maxStrokes-1)  return; // make sure to check this for all tyles affected;
}


if (br==7) {
	while (x<0) {x+=tyleSize; tx--;}
	while (y<0) {y+=tyleSize; ty--;}
	adjustToPentagon(&x, &y);
	y+=2;
	if ((tera.Edit.prevHexX==x) && (tera.Edit.prevHexY==y)) return;
	tera.Edit.prevHexX=x;
	tera.Edit.prevHexY=y;
}


tera.Edit.drx=mouse.pos.x;
tera.Edit.dry=mouse.pos.y;

for (int dty=0; dty<2; dty++)
for (int dtx=0; dtx<2; dtx++)
if ((x+tera.uppermap[br]->w>=dtx*tyleSize) && (y+tera.uppermap[br]->h>=dty*tyleSize)) 
{
sTyleStroke* tyle=&Terrain[tylextogrid(tx+dtx)][tyleytogrid(ty+dty)];

sectorStrokes* str=&tyle->stroke[tyle->max];
tyle->max++;
tyle->updated=1;
str->age=now;
str->alpha=tera.Edit.mb[mgvox.LastUsedTool].Hheight.val; 
str->x=x-dtx*tyleSize;
str->y=y-dty*tyleSize;
str->mask=br;
str->surface=tera.Edit.mb[mgvox.LastUsedTool].DecaySfc;
str->type=2;
if (tera.Edit.mb[mgvox.LastUsedTool].useSideUpper){
	str->type=3;
	str->upsurface=tera.Edit.mb[mgvox.LastUsedTool].UpperSfc;
}
readInGameStroke(str, tylextogrid(tx+dtx), tyleytogrid(ty+dty), 1);
}

}

void VoxToUpeer(){

	if (editorCheckPressDistance(32)) return; 

	worldcoordinates tmppos;
	MouseToPos(&tmppos);

QuickDrow8bitVex(palette.Buffer8bit,&tmppos,
				 &mgvDB.mgvarray[mgvox.curpiece][mgvox.curpiecetype].Converted 
				 , 0, 0, 0, tera.Edit.G8resize, tera.Edit.Ax, tera.Edit.Ay, tera.Edit.Ah, 0);

	int totylex=tylextogrid(tmppos.tx)+1;
	int totyley=tyleytogrid(tmppos.ty)+1;
	int x=accurate(tmppos.x);
	int y=accurate(tmppos.y);
	int rad=palette.G8BufferRadius;

	int tx=tmppos.tx;
	int ty=tmppos.ty;
	x-=rad;
	y-=rad;

normalizeCoordinates1(&tx, &ty, &x, &y );

for (int dty=0; dty<2; dty++)
for (int dtx=0; dtx<2; dtx++)
if ((x+rad*2>=dtx*tyleSize) && (y+rad*2>=dty*tyleSize)) 
{
sTyleStroke* tyle=&Terrain[tylextogrid(tx+dtx)][tyleytogrid(ty+dty)];
if (tyle->max>=maxStrokes-1)  return; 
}

NormalizeAngle(&tera.Edit.Ah);
NormalizeAngle(&tera.Edit.Ax);
NormalizeAngle(&tera.Edit.Ay);

tera.Edit.drx=mouse.pos.x;
tera.Edit.dry=mouse.pos.y;

for (int dty=0; dty<2; dty++)
for (int dtx=0; dtx<2; dtx++)
if ((x+rad*2>=dtx*tyleSize) && (y+rad*2>=dty*tyleSize)) 
{
sTyleStroke* tyle=&Terrain[tylextogrid(tx+dtx)][tyleytogrid(ty+dty)];

sectorStrokes* str=&tyle->stroke[tyle->max];
tyle->max++;
tyle->updated=1;

str->age=(int)(tera.Edit.Ah*32) | ((int)(tera.Edit.Ax*32)<<8) | ((int)(tera.Edit.Ay*32)<<16) 
	| ((tera.Edit.G8h+65)<<24);
str->alpha=tera.Edit.G8resize;
str->x=x-dtx*tyleSize;
str->y=y-dty*tyleSize;
str->mask=tera.Edit.mb[mgvox.LastUsedTool].DecaySfc;
str->surface=mgvox.curpiece;
str->upsurface=mgvox.curpiecetype;
str->type=6;
readInGameStroke(str, tylextogrid(tx+dtx), tyleytogrid(ty+dty), 1);
}

}
void MgVoxToUpper(){

	if (editorCheckPressDistance(32)) return; 

	worldcoordinates tmppos;
	MouseToPos(&tmppos);

	int totylex=tylextogrid(tmppos.tx)+1;
	int totyley=tyleytogrid(tmppos.ty)+1;
	int x=accurate(tmppos.x);
	int y=accurate(tmppos.y);
	MegavoxBlit (&mgvDB.mgvarray[mgvox.curpiece][mgvox.curpiecetype],
		tera.Edit.G8resize, tera.Edit.G8h, tera.Edit.Ax, tera.Edit.Ay, tera.Edit.Ah);
	int rad=palette.G8BufferRadius;

	int tx=tmppos.tx;
	int ty=tmppos.ty;
	x-=rad;
	y-=rad;

normalizeCoordinates1(&tx, &ty, &x, &y );

for (int dty=0; dty<2; dty++)
for (int dtx=0; dtx<2; dtx++)
if ((x+rad*2>=dtx*tyleSize) && (y+rad*2>=dty*tyleSize)) 
{
sTyleStroke* tyle=&Terrain[tylextogrid(tx+dtx)][tyleytogrid(ty+dty)];
if (tyle->max>=maxStrokes-1)  return; 
}

NormalizeAngle(&tera.Edit.Ah);
NormalizeAngle(&tera.Edit.Ax);
NormalizeAngle(&tera.Edit.Ay);

tera.Edit.drx=mouse.pos.x;
tera.Edit.dry=mouse.pos.y;

for (int dty=0; dty<2; dty++)
for (int dtx=0; dtx<2; dtx++)
if ((x+rad*2>=dtx*tyleSize) && (y+rad*2>=dty*tyleSize)) 
{
sTyleStroke* tyle=&Terrain[tylextogrid(tx+dtx)][tyleytogrid(ty+dty)];

sectorStrokes* str=&tyle->stroke[tyle->max];
tyle->max++;
tyle->updated=1;

str->age=(int)(tera.Edit.Ah*32) | ((int)(tera.Edit.Ax*32)<<8) | ((int)(tera.Edit.Ay*32)<<16) 
	| ((tera.Edit.G8h+65)<<24);
str->alpha=tera.Edit.G8resize;
str->x=x-dtx*tyleSize;
str->y=y-dty*tyleSize;
str->mask=tera.Edit.mb[mgvox.LastUsedTool].DecaySfc;
str->surface=mgvox.curpiece;
str->upsurface=mgvox.curpiecetype;
str->type=7;
readInGameStroke(str, tylextogrid(tx+dtx), tyleytogrid(ty+dty), 1);
}



	
}

// Editor windows
bool SelectorIntDEventHandle(EditorInterface* caller){
int caly=caller->y+itfcH;
if (event.type==SDL_MOUSEBUTTONDOWN){
int TexturesInRow=caller->w/37;
float Alpha=tera.Edit.mb[1].IndAlpha.val; Alpha/=32;

for (int i=0; i<32; i++)
	if (tera.IndDecay[i].pixels) {
int x=caller->x+(i % TexturesInRow)*37;
int y=caly+i/TexturesInRow*37;

if (mouseInSquere( x, y, 32, 32)) {
		EquipToolInvoke( &edit.FullScreen, event.button.button, drowIngameInd);

 //if ((SDL_BUTTON(SDL_BUTTON_LEFT) & mouse.u8ms) && () && (edit.MBPressed))
		tera.Edit.mb[event.button.button].brush[i]=!tera.Edit.mb[event.button.button].brush[i];

tera.Edit.mb[event.button.button].AnyBrush=0;
for (int i=0; i<TerrainBrushes; i++)
	if (tera.Edit.mb[event.button.button].brush[i]) tera.Edit.mb[event.button.button].AnyBrush=1;
return 1;
	}
	}

TexturesInRow=caller->w/69;
for (int i=0; i<32; i++)
	if (tera.DecayBG[i].pixels) {
int x=caller->x+(i % TexturesInRow)*69;
int y=caly+200+i/TexturesInRow*69;

if  (mouseInSquere( x, y, 64, 64)){
//EquipToolInvoke( &edit.FullScreen, event.button.button, drowIngameInd);
	tera.Edit.mb[event.button.button].DecaySfc=i;
	mgvox.LastUsedTool=event.button.button;
return 1;
}
	}
}
	return 0;

}

void NumRotation(){
const float  G8RotSpeed=0.04;
	if (keystates[ SDLK_KP7 ]) tera.Edit.Ah+=G8RotSpeed;
	if (keystates[ SDLK_KP9 ]) tera.Edit.Ah-=G8RotSpeed;
	if (keystates[ SDLK_KP8 ]) tera.Edit.Ay+=G8RotSpeed;
	if (keystates[ SDLK_KP2 ]) tera.Edit.Ay-=G8RotSpeed;
	if (keystates[ SDLK_KP4 ]) tera.Edit.Ax+=G8RotSpeed;
	if (keystates[ SDLK_KP6 ]) tera.Edit.Ax-=G8RotSpeed;
	if (keystates[ SDLK_KP1 ]) tera.Edit.G8resize-=G8RotSpeed;
	if (keystates[ SDLK_KP3 ]) tera.Edit.G8resize+=G8RotSpeed;
	GetScroll(&edit.FullScreen, &tera.Edit.G8h, -64, 128);
}

void SelectorIntD(EditorInterface* caller){
int caly=caller->y+itfcH;
DrowFrame (caller->x, caller->y, caller->w, caller->h+itfcH, 0x7777FF);

if (edit.FullScreen.mouseTool[mgvox.LastUsedTool]==VoxToUpeer){
	VoxPreview();
	NumRotation();
}
if (edit.FullScreen.mouseTool[mgvox.LastUsedTool]==MgVoxToUpper){
	NumRotation();
	MgVoxPreview();
}
//if (tera.Edit.mb[mgvox.LastUsedTool].

drowButton(&edit.ApplyBGtoTyle, caller->x+caller->w , caly+60);
drowButton(&tera.Edit.WaterLayer, caller->x+caller->w , caly+92);
	drowButton(&tera.Edit.LavaLayer, caller->x+caller->w +32, caly+92);
drowButton(&tera.Edit.LowerSoil, caller->x+caller->w , caly+124);
	drowButton(&tera.Edit.RaiseSoil, caller->x+caller->w +32, caly+124);
drowButton(&edit.VoxToUp, caller->x+caller->w, caly+156);
	drowButton(&edit.MgVoxToUp, caller->x+caller->w + 32, caly+156);
//drowButton(&tera.Edit.nextIndD, caller->x+caller->w , caly);
//drowButton(&tera.Edit.prevIndD, caller->x+caller->w + 30, caly);

//drowButton(&tera.Edit.prevSFC, caller->x+caller->w , caly+30);
//drowButton(&tera.Edit.nextSFC, caller->x+caller->w + 30, caly+30);

//DrowMyIndAlpha (&tera.IndDecay[tera.Edit.IndN],&tera.DecayBG[tera.Edit.SFCn] ,caller->x, caly, screen);
DrowUpdatableValue(&tera.Edit.mb[mgvox.LastUsedTool].IndAlpha, caller->x+caller->w, caly, "Alpha (32=1)");
int TexturesInRow=caller->w/37;
GetScroll(&tera.Edit.selectorIntDecay, &tera.Edit.mb[mgvox.LastUsedTool].IndAlpha.val, 0, 1024);
float Alpha=tera.Edit.mb[1].IndAlpha.val; Alpha/=32;
for (int i=0; i<12; i++)
	if (tera.IndDecay[i].pixels) {
int x=caller->x+(i % TexturesInRow)*37;
int y=caly+i/TexturesInRow*37;

 DrowIndTool (&tera.IndDecay[i], &tera.DecayBG[tera.Edit.SFCn], x, y, screen,Alpha);
 if (tera.Edit.mb[mgvox.LastUsedTool].brush[i]) DrowFrame (x, y, 32, 32, 0x7777FF);
/*
 if ((SDL_BUTTON(SDL_BUTTON_LEFT) & mouse.u8ms) && (mouseInSquere( x, y, 32, 32)) && (edit.MBPressed))
	tera.Edit.mb[1].brush[i]=!tera.Edit.mb[1].brush[i];
tera.Edit.mb[1].AnyBrush=0;
for (int i=0; i<TerrainBrushes; i++)
	if (tera.Edit.mb[1].brush[i]) tera.Edit.mb[1].AnyBrush=1;*/
	}


	TexturesInRow=caller->w/69;
for (int i=0; i<32; i++)
	if (tera.DecayBG[i].pixels) {
int x=caller->x+(i % TexturesInRow)*69;
int y=caly+200+i/TexturesInRow*69;
DrowPieceOfSurface(tera.DecayBG[i].pixels,tera.DecayBG[i].w , 64, x, y);
if (tera.Edit.mb[mgvox.LastUsedTool].DecaySfc==i) DrowFrame (x, y, 64, 64, 0x7777FF);
}

}

bool SelectorHeightDEventHandle(EditorInterface* caller){
int caly=caller->y+itfcH;

if (event.type==SDL_MOUSEBUTTONDOWN){

for (int i=0; i<12; i++)
	if (tera.height[i].DnA) {
int x=caller->x+(i % 6)*37;
int y=caly+i/6*37;

 if (mouseInSquere( x, y, 32, 32)){
	 tera.Edit.mb[event.button.button].hdec=i;
	 EquipToolInvoke( &edit.FullScreen, event.button.button, drowInGameH);
	 return 1;
 }
}

int TexturesInRow=caller->w/69;
	for (int i=0; i<12; i++)
	if (tera.UpperBG[i].pixels) {
		int x=caller->x+(i % TexturesInRow)*69;
		int y=caly+200+i/TexturesInRow*69;

		if  (mouseInSquere( x, y, 64, 64)){
			if ((tera.Edit.mb[event.button.button].useSideUpper) && (tera.Edit.mb[event.button.button].UpperSfc==i))
				tera.Edit.mb[event.button.button].useSideUpper=0;
			else{
			tera.Edit.mb[event.button.button].UpperSfc=i;
			tera.Edit.mb[event.button.button].useSideUpper=1;
			mgvox.LastUsedTool=event.button.button;
			}
			return 1;
		}
	}
}

return 0;
}
void SelectorHeightD(EditorInterface* caller){
int caly=caller->y+itfcH;
DrowFrame (caller->x, caller->y, caller->w, caller->h+itfcH, 0x7777FF);

//drowButton(&tera.Edit.nextIndD, caller->x+caller->w , caly);
//drowButton(&tera.Edit.prevIndD, caller->x+caller->w + 30, caly);

//drowButton(&tera.Edit.prevSFC, caller->x+caller->w , caly+30);
//drowButton(&tera.Edit.nextSFC, caller->x+caller->w + 30, caly+30);

//DrowMyIndAlpha (&tera.IndDecay[tera.Edit.IndN],&tera.DecayBG[tera.Edit.SFCn] ,caller->x, caly, screen);

DrowUpdatableValue(&tera.Edit.mb[mgvox.LastUsedTool].Hheight, caller->x+caller->w, caly, "Height: ");
GetScroll(&tera.Edit.selectorHDecay, &tera.Edit.mb[mgvox.LastUsedTool].Hheight.val, -200, 0);

int no=tera.Edit.mb[mgvox.LastUsedTool].hdec;
if (edit.FullScreen.mouseTool[mgvox.LastUsedTool]==&drowInGameH)
DrowHeightDecayWithUp(tera.Edit.mb[mgvox.LastUsedTool].DecaySfc, 0,0, no,  screen,  screen,  mouse.pos.x-tera.uppermap[no]->w/2, mouse.pos.y-tera.uppermap[no]->h/2, tera.Edit.mb[mgvox.LastUsedTool].Hheight.val,0);

for (int i=0; i<12; i++)
	if (tera.height[i].DnA) {
int x=caller->x+(i % 6)*37;
int y=caly+i/6*37;
if (tera.icons[i])
	sdlanyblit (x, y, tera.icons[i], screen);

if (tera.Edit.mb[mgvox.LastUsedTool].hdec==i) DrowFrame (x, y, 32, 32, 0x7777FF);
	}

int TexturesInRow=caller->w/69;
for (int i=0; i<12; i++)
	if (tera.UpperBG[i].pixels) {
int x=caller->x+(i % TexturesInRow)*69;
int y=caly+200+i/TexturesInRow*69;
DrowPieceOfSurface(tera.UpperBG[i].pixels,tera.UpperBG[i].w , 64, x, y);
if ((tera.Edit.mb[mgvox.LastUsedTool].useSideUpper) && (tera.Edit.mb[mgvox.LastUsedTool].UpperSfc==i)) DrowFrame (x, y, 64, 64, 0x7777FF);
}

}

void SDLtoMy(SDL_Surface* sdl, Surface* srf){
//tera.DecayBG[0].location="pictures/bg/grbg.png";
	srf->pixels=(Uint32*)sdl->pixels;
	srf->w=sdl->w;
	srf->h=sdl->h;
	srf->count=srf->h*srf->w;
	//tera.DecayBG[0].pixels=(Uint32*)image[85]->pixels;
}



void InitialiseTerrain(){
	memset(&tera, 0, sizeof(TerrainEditorPalette));
	MakeAlphaPortionMask(tera.ShadowRad, ZoomVoxShadR,8);

	for (int i=0; i<toolhands; i++)	tera.Edit.mb[i].IndAlpha.val=32;

tera.Edit.G8resize=1;
palette.ThreadG8BufferRadius=0;
memset(&palette.ThreadBuffer8bit[0]	, 0xFF, E8bitBufferSize*4);

tera.Edit.selectorIntDecay=InitEditor (0, 40, "IndDec Brush", 69*3, 600, SelectorIntD,SelectorIntDEventHandle);	
tera.Edit.selectorHDecay=InitEditor (scrwid, 40, "HDec Brush", 250, 600, SelectorHeightD,SelectorHeightDEventHandle);
tera.Edit.LowerSoil=InitMyButton("pictures/editor/lowerSoil.png", drowIngameLower,1,&edit.FullScreen);
tera.Edit.RaiseSoil=InitMyButton("pictures/editor/raiseSoil.png", drowIngameRise,1,&edit.FullScreen);
tera.Edit.WaterLayer=InitMyButton("pictures/editor/RenderWater.png", ApplyWater,1,&edit.FullScreen);
tera.Edit.LavaLayer=InitMyButton("pictures/editor/fireWalkWithMe.png", ApplyLava,1,&edit.FullScreen);

for (int i=0; i<32; i++){
	tera.DecayBG[i].h=tera.DecayBG[i].w=1024;
	tera.DecayBG[i].count=1024*1024;
	tera.DecayBG[i].pixels=0;
	tera.UpperBG[i].h=tera.UpperBG[i].w=1024;
	tera.UpperBG[i].count=1024*1024;
	tera.UpperBG[i].pixels=0;
	tera.DecayZoomBG[i].h=tera.DecayZoomBG[i].w=1024;
	tera.DecayZoomBG[i].count=1024*1024;
	tera.DecayZoomBG[i].pixels=0;
}

ZoomBG=CreateSurface(scrwi, scrhi,image[85]);
ZoomBGUp=CreateSurface(scrwi, scrhi,image[85]);

imgBank.decay1024.ClassicBG=loadany("pictures/bg/classicBG.png");
SDLtoMy(imgBank.decay1024.ClassicBG, &tera.DecayBG[0]);


tera.DecayBG[1].location="pictures/bg/snowbg.png";
tera.DecayBG[1].pixels=(Uint32*)imgBank.decay1024.snowBG->pixels;

tera.DecayBG[2].location="pictures/bg/shootwall.png";
tera.DecayBG[2].pixels=(Uint32*)image[87]->pixels;

imgBank.decay1024.OverGuts=loadany("pictures/bg/overGuts.png");
SDLtoMy(imgBank.decay1024.OverGuts, &tera.UpperBG[2]);

tera.DecayBG[3].location="pictures/bg/cracksbg.png";
tera.DecayBG[3].pixels=(Uint32*)image[60]->pixels;

tera.DecayBG[4].location="pictures/bg/soilbg.png";
tera.DecayBG[4].pixels=(Uint32*)image[3]->pixels;

tera.DecayBG[5].location="pictures/bg/crambledbg.png";
tera.DecayBG[5].pixels=(Uint32*)mergeimaged[1]->pixels;

imgBank.decay1024.CrambledBGUpeer=loadany("pictures/bg/crambledbgUpper.png");
SDLtoMy(imgBank.decay1024.CrambledBGUpeer, &tera.UpperBG[5]);

tera.DecayBG[6].location="pictures/bg/crambledbg2.png";
tera.DecayBG[6].pixels=(Uint32*)mergeimaged[2]->pixels;

loadimages("pictures/bg/autumn.png",86);
SDLtoMy(image[86], &tera.DecayBG[7]);

imgBank.decay1024.LeavesZoom=loadany("pictures/bg/autumnZoom.png");
if (LoadZoomed) SDLtoMy(imgBank.decay1024.LeavesZoom, &tera.DecayZoomBG[7]);

tera.DecayBG[8].location="pictures/bg/oldbg.png";
tera.DecayBG[8].pixels=(Uint32*)imgBank.decay1024.oldBG->pixels;

imgBank.decay1024.Hex=loadany("pictures/bg/hex.png");
SDLtoMy(imgBank.decay1024.Hex, &tera.DecayBG[9]);
imgBank.decay1024.HexZoom=loadany("pictures/bg/hexZoom.png");
if (LoadZoomed) SDLtoMy(imgBank.decay1024.HexZoom, &tera.DecayZoomBG[9]);

tera.DecayBG[10].location="pictures/bg/grbg.png";
tera.DecayBG[10].pixels=(Uint32*)image[85]->pixels;

imgBank.decay1024.grassZOOM=loadany("pictures/bg/grassZOOM.png");
if (LoadZoomed) SDLtoMy(imgBank.decay1024.grassZOOM, &tera.DecayZoomBG[10]);

imgBank.decay1024.web=loadany("pictures/bg/web.png");
SDLtoMy(imgBank.decay1024.web, &tera.DecayBG[11]);

//SDLtoMy(image[10], &tera.UpperBG[17]);
/*
mergeimaged[7] = loadany("pictures/bg/bloodybg.png");
mergeimaged[8] = loadany("pictures/bg/bloodybg1.png");
mergeimaged[9] = loadany("pictures/bg/bloodybg2.png");
*/
SDLtoMy(mergeimaged[7], &tera.DecayBG[12]);
SDLtoMy(mergeimaged[8], &tera.DecayBG[13]);
SDLtoMy(mergeimaged[9], &tera.DecayBG[14]);

// *************************    Borders for background

imgBank.BGborderLeft.location="pictures/editor/Borderlines/left.png";
loadanyAlphaIndex (&imgBank.BGborderLeft);

imgBank.BGborderRight.location="pictures/editor/Borderlines/right.png";
loadanyAlphaIndex (&imgBank.BGborderRight);

imgBank.BGborderTop.location="pictures/editor/Borderlines/top.png";
loadanyAlphaIndex (&imgBank.BGborderTop);




imgBank.BGborderBottom.location="pictures/editor/Borderlines/bottom.png";
loadanyAlphaIndex (&imgBank.BGborderBottom);

// ***************************** Voxel blood load

int j=0;

do{
	imgBank.voxelBlood[j].location="pictures/other/voxelBlood/"+intTotext(j)+".png";
	loadanyAlphaIndex (&imgBank.voxelBlood[j]);
	j++;
} while (imgBank.voxelBlood[j-1].pixCount);


// ****************************** END




imgBank.DefaultShadow.location="pictures/drops/dropmerge_13.png";
loadanyAlphaIndex(&imgBank.DefaultShadow);

//loadimages("pictures/transparentmap.png",10);


//loadimages("pictures/bg/overGuts.png",86);
/*
tera.DecayBG[7].location="pictures/bg/autumn.png";
tera.DecayBG[7].pixels=(Uint32*)image[86]->pixels;*/

ReadIndexedImages();

int indi=0;
tera.IndDecay[0].location="pictures/TerrainBrushes/Ind/ind0.png";

while (loadanyAlphaIndex (&tera.IndDecay[indi])){
	indi++;
tera.IndDecay[indi].location="pictures/TerrainBrushes/Ind/ind"+intTotext(indi)+".png";
}
/*
tera.IndDecay[3].location="pictures/TerrainBrushes/ind3.png";
loadanyAlphaIndex (&tera.IndDecay[3]);

tera.IndDecay[5].location="pictures/TerrainBrushes/ind5.png";
loadanyAlphaIndex (&tera.IndDecay[5]);
*/
StoreIndexedImages();

ReadHeightImages();

int li=0;

do {
	tera.uppermap[li]=loadany("pictures/TerrainBrushes/"+intTotext(li)+"up.png"); 

	tera.height[li].location="pictures/TerrainBrushes/"+intTotext(li)+"h.png";
    ImageAlphaToDirLightImage(&tera.height[li]);
	std::string tmp="pictures/TerrainBrushes/"+intTotext(li)+"icon.png";
	tera.icons[li]=IMG_Load(tmp.c_str());
	tmp.clear();
	tera.zoomedIMG.height[li].location="pictures/TerrainBrushes/"+intTotext(li)+"hZ.png";
	ImageAlphaToDirLightImage(&tera.zoomedIMG.height[li]);
	tera.zoomedIMG.uppermap[li]=loadany("pictures/TerrainBrushes/"+intTotext(li)+"upZ.png");

	li++;
} while (tera.uppermap[li-1]);


//StoreHeightImages();

//tera.height[4].location="pictures/TerrainBrushes/h4.png";
//ImageAlphaToDirLightImage(&tera.height[4]);
//tera.uppermap[4]=loadany("pictures/TerrainBrushes/up4.png");
/*
tera.zoomedIMG.height[4].location="pictures/TerrainBrushes/h4Z.png";
ImageAlphaToDirLightImage(&tera.zoomedIMG.height[4]);
tera.zoomedIMG.uppermap[4]=loadany("pictures/TerrainBrushes/up4Z.png");*/

//tera.height[5].location="pictures/TerrainBrushes/h5.png";
//ImageAlphaToDirLightImage(&tera.height[5]);
/*
tera.zoomedIMG.height[5].location="pictures/TerrainBrushes/h5Z.png";
ImageAlphaToDirLightImage(&tera.zoomedIMG.height[5]);
tera.zoomedIMG.uppermap[5]=loadany("pictures/TerrainBrushes/up5Z.png");*/

//int nomer=6;
//tera.height[nomer].location="pictures/TerrainBrushes/h6.png";
//ImageAlphaToDirLightImage(&tera.height[nomer]);
/*
tera.zoomedIMG.height[nomer].location="pictures/TerrainBrushes/h6Z.png";
ImageAlphaToDirLightImage(&tera.zoomedIMG.height[nomer]);
tera.zoomedIMG.uppermap[nomer]=loadany("pictures/TerrainBrushes/up6Z.png");
*/



//tera.DHheight[0].location="pictures/TerrainBrushes/dh0.png";
//ImageAlphaToDirLightImage(&tera.DHheight[0]);

/*
tera.IndDecay[0].location="pictures/TerrainBrushes/ind0.png";
loadanyAlphaIndex (&tera.IndDecay[0]);*/

// ************************************ DH images

//StoreDHImages();
ReadDHImages();
tera.DHuppermap[0]=loadany("pictures/TerrainBrushes/dwn0.png");



imgBank.displacementGhost.location="pictures/other/NewDisplacementMasks/ghost.png";
loadanyAlphaIndex (&imgBank.displacementGhost);

bool yep=1;
int zbc=0;
while (yep){
tera.ZoomDecay[zbc].location="pictures/TerrainBrushes/zoomBrush/"+intTotext(zbc)+".png";	//intTotext(int n)
yep=loadanyAlphaIndex (&tera.ZoomDecay[zbc]);
zbc++;
}
tera.zoomGrains=zbc-1;
/*loadanyAlphaIndex (&tera.ZoomDecay[0]);
loadanyAlphaIndex (&tera.ZoomDecay[1]);
loadanyAlphaIndex (&tera.ZoomDecay[2]);
loadanyAlphaIndex (&tera.ZoomDecay[3]);*/


//StoreIndexedImages();
//ReadIndexedImages();
}

void zoomedInView(){



if (!Idisplay.active) {
	if (tera.ZOOMED){
	
	tera.firstZoomRefresh=0;
	Empty8BitBuffer();
	tera.ZOOMED=0;
	}
	return;
}

int ztx, zty, zx, zy, zw, zh;
int bgx;
int bgy;

if (!tera.ZOOMED){
memset((char*)palette.Buffer8bit,0,E8bitBufferDiam*E8bitBufferDiam*4);
if (ZoomShadows) memset(screenShadow,0, scrwi*scrhi);
memset((Uint32*)ZoomBGUp->pixels, 0, scrwi*scrhi*4);

tera.firstZoomRefresh=1;
		tera.ptx=ztx=player.host->pos.tx;
		tera.pty=zty=player.host->pos.ty;
		tera.px=zx=player.host->pos.x/accuracy - scrwi/16;
		tera.py=zy=player.host->pos.y/accuracy - scrhi/16;
		while (zx<0) {zx+=1024; ztx--; }
		while (zy<0) {zy+=1024; zty--; };
		zw=scrwi/8;
		zh=scrhi/8;



		bgx=(zx-zx/128*128)*8;
		bgy=(zy-zy/128*128)*8;

		
		int getno=GetTyleBackground(ztx, zty);
		tera.thisbg=((tera.DecayZoomBG[getno].h==1024) && (tera.DecayZoomBG[getno].pixels)) 
			? (Uint32*)tera.DecayZoomBG[getno].pixels : (Uint32*)tera.DecayBG[getno].pixels;


		tera.waterOnZoom=grid.water[tylextogrid(ztx)][tylextogrid(zty)];
for (int i=0; i<3; i++)
	for (int j=0; j<2; j++)
		drowMemCoby(-bgx+i*1024, -bgy+j*1024, (Uint32*)ZoomBG->pixels, tera.thisbg );


//memcpy ( storedmp[gx+1][gy+1]->pixels, tera.DecayBG[thisbg].pixels, 1048576*4 );

for (int i=0; i<5; i++)
	for (int j=0; j<5; j++)
		if ((abs(grid.objectx[i][j]-ztx)<2) && (abs(grid.objecty[i][j]-zty)<2)){
			sTyleStroke* tyle=&Terrain[i][j];
			int grtx=(grid.objectx[i][j]-ztx)*tyleSize;
			int grty=(grid.objecty[i][j]-zty)*tyleSize;
for (int i=0; i<tyle->max; i++){
	//readInGameStroke(&tyle->stroke[i], gx, gy);
	sectorStrokes* stroke=&tyle->stroke[i];
	int x=(grtx+stroke->x-zx)*8;
	int y=(grty+stroke->y-zy)*8;

if ((x<scrwi) && (x>-scrwi) && (y<scrhi) && (y>-scrhi)){
		if (stroke->type==1)
					DrowMyIndZoom (&tera.IndDecay[stroke->mask],x+bgx,y+bgy  ,
					tera.DecayZoomBG[stroke->surface].pixels ? &tera.DecayZoomBG[stroke->surface] :  &tera.DecayBG[stroke->surface]
				,x, y, ZoomBG, stroke->alpha, 0);
		else if ((stroke->type==2) && (tera.zoomedIMG.height[stroke->mask].DnA)){
						DirectionalLightetImage* himg=&tera.zoomedIMG.height[stroke->mask];
						int fullbgx=x+bgx;
						while (fullbgx<0) fullbgx+=tera.DecayBG[stroke->surface].w;
						while (fullbgx>tera.DecayBG[stroke->surface].w) fullbgx-=tera.DecayBG[stroke->surface].w;
						int fullbgy=y+bgy;
						while (fullbgy<0) fullbgy+=tera.DecayBG[stroke->surface].h;
						while (fullbgy>tera.DecayBG[stroke->surface].h) fullbgy-=tera.DecayBG[stroke->surface].h;					
					
							DrowHeightDecayWithUp(stroke->surface,fullbgx,fullbgy,  stroke->mask, ZoomBG,  ZoomBGUp,  x, y, stroke->alpha,1);
							if (fullbgx+himg->w>tera.DecayBG[stroke->surface].w)
								DrowHeightDecayWithUp(stroke->surface,fullbgx-tera.DecayBG[stroke->surface].w,fullbgy,  stroke->mask, ZoomBG,  ZoomBGUp,  x, y, stroke->alpha,1);

							if (fullbgy+himg->h>tera.DecayBG[stroke->surface].h)
								DrowHeightDecayWithUp(stroke->surface,fullbgx,fullbgy-tera.DecayBG[stroke->surface].h,  stroke->mask, ZoomBG,  ZoomBGUp,  x, y, stroke->alpha,1);

							if ((fullbgx+himg->w>tera.DecayBG[stroke->surface].w) && (fullbgy+himg->h>tera.DecayBG[stroke->surface].h))
								DrowHeightDecayWithUp(stroke->surface,fullbgx-tera.DecayBG[stroke->surface].w,fullbgy-tera.DecayBG[stroke->surface].h,  stroke->mask, ZoomBG,  ZoomBGUp,  x, y, stroke->alpha,1);
			}
	}
}
}


//memcpy ( ZoomBG->pixels, ZoomBGUp->pixels, scrwi*scrhi*4 );
//apply_surface( 0,0, ZoomBGUp, ZoomBG );
ApplyUpperMask(ZoomBG, ZoomBGUp);
tera.ZOOMED=1;
}

		ztx=player.host->pos.tx;
		zty=player.host->pos.ty;
		zx=player.host->pos.x/accuracy - scrwi/16;
		zy=player.host->pos.y/accuracy - scrhi/16;
		while (zx<0) {zx+=1024; ztx--; }
		while (zy<0) {zy+=1024; zty--; };
		zw=scrwi/8;
		zh=scrhi/8;

		 bgx=(zx-zx/128*128)*8;
		 bgy=(zy-zy/128*128)*8;

int dx=	((tera.ptx-ztx)*tyleSize+tera.px-zx)*8;
int dy= ((tera.pty-zty)*tyleSize+tera.py-zy)*8;

tera.dx=dx;
tera.dy=dy;
tera.Outline.x=tera.Outline.y=tera.Outline.h=tera.Outline.w=0;

if (tera.dx)
if (tera.dx>0){
	tera.Outline.x=0; tera.Outline.w=tera.dx;
}else{
	tera.Outline.x=scrwi+tera.dx-1;
	//tera.Outline.w=-tera.dx+1;
}

if (tera.dy)
if (tera.dy>0){
	tera.Outline.y=0; tera.Outline.h=tera.dy;
}else{
	tera.Outline.y=scrhi+tera.dy-1;
	//tera.Outline.h=0;//-tera.dy+1;
}


		SelfDisplaceTexture ((Uint32*)ZoomBG->pixels, scrwi, scrhi, dx, dy );
		SelfDisplaceTextureCleanBorder ((Uint32*)ZoomBGUp->pixels, scrwi, scrhi, dx, dy );
		if (ZoomShadows) SelfDisplaceUin8 (screenShadow, scrwi, scrhi, dx, dy );
	
for (int i=0; i<3; i++)
	for (int j=0; j<2; j++)
	//	drowMemCoby(-bgx+i*1024, -bgy+j*1024, (Uint32*)screen->pixels, tera.DecayBG[4].pixels );
		drowMemCobyOutline(-bgx+i*1024, -bgy+j*1024, (Uint32*)ZoomBG->pixels,  tera.thisbg,
		max(0,dx), max(0,dy),  min(scrwi, scrwi+dx)-1, min(scrhi, scrhi+dy)-1 );


tera.px=zx;
tera.ptx=ztx;
tera.py=zy;
tera.pty=zty;
//Terrain[5][5];


if ((tera.dx) || (tera.dy))
for (int i=0; i<5; i++)
	for (int j=0; j<5; j++)
		if ((abs(grid.objectx[i][j]-ztx)<2) && (abs(grid.objecty[i][j]-zty)<2)){
			sTyleStroke* tyle=&Terrain[i][j];
			int grtx=(grid.objectx[i][j]-ztx)*tyleSize;
			int grty=(grid.objecty[i][j]-zty)*tyleSize;
for (int i=0; i<tyle->max; i++){
	//readInGameStroke(&tyle->stroke[i], gx, gy);
	sectorStrokes* stroke=&tyle->stroke[i];
	int x=(grtx+stroke->x-zx)*8;
	int y=(grty+stroke->y-zy)*8;
	if ((x<scrwi) && (x>-scrwi) && (y<scrhi) && (y>-scrhi)){

	if (stroke->type==1)
		DrowMyIndZoom (&tera.IndDecay[stroke->mask],x+bgx,y+bgy  ,
				tera.DecayZoomBG[stroke->surface].pixels ? &tera.DecayZoomBG[stroke->surface] :  &tera.DecayBG[stroke->surface]
			,x, y, ZoomBG, stroke->alpha, 1);
else if ((stroke->type==2) && (tera.zoomedIMG.height[stroke->mask].DnA)){

			DirectionalLightetImage* himg=&tera.zoomedIMG.height[stroke->mask];
			int fullbgx=x+bgx;
			while (fullbgx<0) fullbgx+=tera.DecayBG[stroke->surface].w;
			while (fullbgx>tera.DecayBG[stroke->surface].w) fullbgx-=tera.DecayBG[stroke->surface].w;
			int fullbgy=y+bgy;
			while (fullbgy<0) fullbgy+=tera.DecayBG[stroke->surface].h;
			while (fullbgy>tera.DecayBG[stroke->surface].h) fullbgy-=tera.DecayBG[stroke->surface].h;
			DrowHeightDecayWithUpOutlineZoomed(stroke->surface,fullbgx,fullbgy,  stroke->mask, ZoomBG,  ZoomBGUp,  x, y, stroke->alpha);

			if (fullbgx+himg->w>tera.DecayBG[stroke->surface].w)
				DrowHeightDecayWithUpOutlineZoomed(stroke->surface,fullbgx-tera.DecayBG[stroke->surface].w,fullbgy,  stroke->mask, ZoomBG,  ZoomBGUp,  x, y, stroke->alpha);

			if (fullbgy+himg->h>tera.DecayBG[stroke->surface].h)
				DrowHeightDecayWithUpOutlineZoomed(stroke->surface,fullbgx,fullbgy-tera.DecayBG[stroke->surface].h,  stroke->mask, ZoomBG,  ZoomBGUp,  x, y, stroke->alpha);

			if ((fullbgx+himg->w>tera.DecayBG[stroke->surface].w) && (fullbgy+himg->h>tera.DecayBG[stroke->surface].h))
				DrowHeightDecayWithUpOutlineZoomed(stroke->surface,fullbgx-tera.DecayBG[stroke->surface].w,fullbgy-tera.DecayBG[stroke->surface].h,  stroke->mask, ZoomBG,  ZoomBGUp,  x, y, stroke->alpha);
		}


	}
}
}

// BLITTING UPPER TO BG OUTLINE
if (tera.dx){
SDL_Rect qdest;
if (tera.dx>0){
	qdest.x=0; qdest.w=tera.dx;
}else{
	qdest.x=scrwi+tera.dx-1;
	qdest.w=-tera.dx;
}
qdest.y=0;
qdest.h=scrhi;
ApplyUpperMasksector(ZoomBG, ZoomBGUp, &qdest);
}

if (tera.dy){
SDL_Rect qdest;
if (tera.dy>0){
	qdest.y=0; qdest.h=tera.dy;
}else{
	qdest.y=scrhi+tera.dy;
	qdest.h=-tera.dy;
}
qdest.x=0;
qdest.w=scrwi;
ApplyUpperMasksector(ZoomBG, ZoomBGUp, &qdest);
//SDL_BlitSurface(ZoomBGUp, &qdest, ZoomBG, &qdest);
}
// BLITTING BG TO SCREEN
if (ZoomBGDisplacement)
		BlitZoomBGToScreenDisplacement();
else
	memcpy ( screen->pixels, ZoomBG->pixels, scrwi*scrhi*4 );

// Drowing water
if (tera.waterOnZoom)
{
	waterDis+=0.3;
	waterEl+=(waterRise*0.02)-0.01;
	if (waterDis>998) waterDis=0;
	if (waterEl<0) {waterEl=0; waterRise=true;}
	if (waterEl>5) {waterEl=5; waterRise=false;}
for (int i=-1; i<2; i++)
	for (int j=-1; j<3; j++)
	BWAITextureDisplacementZoom(screen, image[62],  image[82],-bgx+tyleSize*j,-bgy+tyleSize*i, waterEl-5, waterDis);
	
}	

//Setting Shadow
if (ZoomShadows){
int scs;
scs=scrwi*scrhi;
Uint32* scrp=(Uint32*)screen->pixels;
Uint8* shp=screenShadow;
while (scs){
if (*shp)
{
int AH=255-*shp;
register int idst = *scrp;
*scrp = (RedMask & (((idst & RedMask) * AH)>>8)) |
(GreenMask & (((idst & GreenMask) * AH)>>8)) |
(BlueMask & (((idst & BlueMask) * AH)>>8)) |
(*scrp & AlphaMask);
}
shp++;
scrp++;
scs--;
}
}


// Drowing Displacement 
DrowMyIndAlphaDisplacementSameSurface (&imgBank.displacementGhost, mouse.pos.x, mouse.pos.y, temptest, screen);

//DrowMyIndAlphaDisplacement (&tera.IndDecay[0], mouse.pos.x, mouse.pos.y, temptest, screen, mouse.pos.x, mouse.pos.y, ZoomBG);



//drowing voxels
for (int i=0;i<observ;i++)
{
int j;
//for (j=2; j<=activew; j++)
curwall=wfirst[i];
	//part pretype;



////zoomHscreen

memset((char*)zoomHscreen,0,scrwi*scrhi*2);

while (curwall)
{
if (curwall->type) 
{
	/*if ((!player.isunderground) && (displacement[curwall->type-900]) && (curwall->drowAbove))
	{
	// displacement here
		Displacement (displacement[curwall->type-900], curwall->pos.tx, curwall->pos.ty, curwall->pos.x, curwall->pos.y, curwall->pos.h );
	}*/
	d=i-matrdepth-curwall->pos.h;
	//curwall->drowing();
	if (curwall->Megavoxel)
	{
		//int ztx, zty, zx, zy, zw, zh;
		offx=((curwall->pos.tx-ztx)*tyleSize+accurate(curwall->pos.x)-zx)*8;
		offy=((curwall->pos.ty-zty)*tyleSize+accurate(curwall->pos.y)-zy)*8;
		 DrowIngameMegavoxZoom(curwall->Megavoxel, &curwall->pos);
	}
}
curwall=curwall->next;
}	
}  


tera.firstZoomRefresh=0;
	// swapscreens();

if (PixelatedScreen) ApplyPixelsMask();

}
void DecayByOffxOffy(int offx, int offy)
{

//drowDecayedBG (image[decayBG[mrg.type]], storedmp[gx+1][gy+1], mergeimaged[mrg.type], mrg.pos.x, mrg.pos.y);
int type=rand() % 4;
MyIndexedAlphaSurface* tempIMG=&imgBank.voxelBlood[type];
offx-=tempIMG->w/2;
offy-=tempIMG->h/2;
offsToAccuratedCreateDatas(offx, offy);
shortx=createData.x;
shorty=createData.y;

while (shortx<0){shortx+=tyleSize; createData.tx--; }
while (shorty<0){shorty+=tyleSize; createData.ty--; }

int ttx=tylextogrid(createData.tx)+1;
int tty=tyleytogrid(createData.ty)+1;
int ttxp=tylextogrid(createData.tx+1)+1;
int ttyp=tyleytogrid(createData.ty+1)+1;


DrowMyInd (tempIMG,shortx,shorty ,&tera.DecayBG[createData.decaySurfaceNo],shortx,shorty, storedmp[ttx][tty]);
RefreshGround (shortx, shorty, tempIMG->w, tempIMG->w, ttx, tty);


if (shortx+tempIMG->w>tyleSize) 
{
DrowMyInd (tempIMG,shortx-tyleSize,shorty ,&tera.DecayBG[createData.decaySurfaceNo],shortx-tyleSize,shorty, storedmp[ttxp][tty]);
RefreshGround (shortx-tyleSize, shorty, tempIMG->w, tempIMG->w, ttxp, tty);

//drowDecayedBG (mergeimaged[createData.decaySurfaceNo], storedmp[ttxp][tty], mergeimaged[type], shortx-tyleSize, shorty);
//	updateMaunMapSector (shortx-tyleSize, shorty, type, ttxp, tty);
}

if (shorty+tempIMG->h>tyleSize) 
{
//drowDecayedBG (mergeimaged[createData.decaySurfaceNo], storedmp[ttx][ttyp], mergeimaged[type], shortx, shorty-tyleSize);
//updateMaunMapSector (shortx, shorty-tyleSize, type, ttx, ttyp);
DrowMyInd (tempIMG,shortx,shorty-tyleSize ,&tera.DecayBG[createData.decaySurfaceNo],shortx,shorty-tyleSize, storedmp[ttx][ttyp]);
RefreshGround (shortx, shorty-tyleSize, tempIMG->w, tempIMG->w, ttx, ttyp);

   if (shortx+tempIMG->w>tyleSize) 
	{
DrowMyInd (tempIMG,shortx-tyleSize,shorty-tyleSize ,&tera.DecayBG[createData.decaySurfaceNo],shortx-tyleSize,shorty-tyleSize, storedmp[ttxp][ttyp]);
RefreshGround (shortx-tyleSize, shorty-tyleSize, tempIMG->w, tempIMG->w, ttxp, ttyp);

	//drowDecayedBG (mergeimaged[createData.decaySurfaceNo], storedmp[ttxp][ttyp], mergeimaged[type], shortx-tyleSize, shorty-tyleSize);
	//updateMaunMapSector (shortx-tyleSize, shorty-tyleSize, type, ttxp, ttyp);
	}

}

}
bool QuickDrow8bitVexWithGutsDecay(G8_Particle * G, int offh, double resize, float globxh, float globyh, float glob, int offx, int offy)//, Uint32* __restrict palpx)//, void (*onImpactFunk) (int ,int))
{	


if ((G->radius*resize>=E8bitBufferRad) || (-G->radius*resize<=-E8bitBufferRad)
	|| (G->radius*resize>=E8bitBufferRad) || (-G->radius*resize<=-E8bitBufferRad)) 
	return 0;

palette.G8BufferRadius=max(palette.G8BufferRadius,(int)(G->radius*resize+1));

FloatAnglesToCosSin(globxh, globyh, glob,&NtoN);
New8BitNtoN(resize,&NtoN);

Uint32* __restrict gp=G->pixels;
Uint32 * __restrict bufPoint=&palette.Buffer8bit[(int)(E8bitBufferSize/2+E8bitBufferDiam/2)];
//palette.Buffer8bit
Uint32* __restrict BufPic;

int TrueX, TrueY, TrueH;
int heg;
int countme=G->count;
if (countme>0)
do
{
	int qh=(char)(*gp>>16);
	Uint32 plane=*gp & 0xFFFF;
	gp++;
countme-=plane;
while (plane)
{
int qx=(char)(*gp & 0xFF);
int qy=(char)((*gp>>8) & 0xFF);

TrueX=qx*NtoN.fxtox+qh*NtoN.fhtox+qy*NtoN.fytox;
TrueY=qx*NtoN.fxtoy+qh*NtoN.fhtoy+qy*NtoN.fytoy;
TrueH=qx*NtoN.fxtoh+qh*NtoN.fhtoh+qy*NtoN.fytoh;

heg=223-TrueH-offh;

BufPic=bufPoint+(TrueY<<8)+TrueX;
//Testing End
E8bitBufFill(BufPic, heg, *gp & 0xFFFF0000);
BufPic+=E8bitBufferDiam;
E8bitBufFill(BufPic, heg, *gp & 0xFFFF0000);
BufPic++;
E8bitBufFill(BufPic, heg, *gp & 0xFFFF0000);

if (heg==223) DecayByOffxOffy(offx+TrueX, offy+TrueY);

gp++;
plane--;
}
}
while (countme);


return 1;
}


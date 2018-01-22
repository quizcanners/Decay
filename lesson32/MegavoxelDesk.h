

IngameMegavox* errortest;
int ertst;

EditorInterface InitEditor (int x, int y, string someshit, int w,int h, void (*functional) (EditorInterface* caller), bool (*eventfuncional) (EditorInterface* caller)){
EditorInterface temp;
temp.text=TextOnlyToImage(someshit);
for (int i=0; i<toolhands; i++){
temp.mouseTool[i]=0;
temp.toolON[i]=0;
temp.toolColor[i]=min(i,7);
}
temp.toolSizeAll=0;
temp.x=x;
temp.y=y;
temp.w=w;
temp.h=h;
temp.functional=*functional;
temp.eventFuctional=*eventfuncional;
temp.dragged=0;
temp.closed=0;
if (temp.h+temp.y+itfcH>=scrhi) temp.closed=temp.autoclose=1;
else temp.closed=temp.autoclose=0;
return temp;
}
MyButton InitMyButton( std::string location ,void (*functional) (), int type, EditorInterface* AssignTo)
{
	MyButton temp;
	temp.icon=loadany(location);
	temp.functional=*functional;
	temp.assignTo=AssignTo;
	temp.type=type;
	return temp;
}

void GetScroll(EditorInterface* window, int* val, int min, int max)
{
	scrollQ[scrollMax].maxi=max;
	scrollQ[scrollMax].value=val;
	scrollQ[scrollMax].window=window;
	scrollQ[scrollMax].min=min;
	scrollMax++;
}

void ManageScrollableValue(int min, int max, int x, int y, TunableDisplayValue* tune, string someshit ){

if (tune->val<min) tune->val=min;
if (tune->val>=max) tune->val=max-1;

if ((tune->textIMG) && (tune->renderedVal==tune->val)) 
	sdlanyblit (x , y, tune->textIMG, screen);
else {
	if (tune->textIMG)
		SDL_FreeSurface(tune->textIMG);
	tune->textIMG=TextToImage(someshit, tune->val , "]");
	tune->renderedVal=tune->val;
}

if (mouseInSquere(x, y, tune->textIMG->w,  tune->textIMG->h))
	GetScroll(&edit.FullScreen, &tune->val, min, max);

}


int E8bitMegavoxelMatrixCalculateGradus(int x, int y, int h, int radius, MegavoxSegment* segm)
{
int xe=min(23,x+radius);
int ye=min(23,y+radius);
int he=min(23,h+radius);

double countx=0, county=0, counth=0;
//int underdog=1;



for (int h1=max(0, h-radius); h1<=he; h1++)
	for (int y1=max(0, y-radius); y1<=ye; y1++){
		int x1=max(0, x-radius);
		Uint16* bp=segm->matrix + h1*24*24 + y1*24+x1;
			for (x1; x1<=xe; x1++)
		{
			if ((*bp) && ((x1!=x) || (y1!=y) || (h1!=h)))
			{
dddistance (x1-x, y1-y, h1-h);
if (h1>=h){
countx+=((x-x1)*100)/fuck;
county+=((y-y1)*100)/fuck;
} else {
countx+=((x-x1)*100)/fuck;
county+=((y-y1)*100)/fuck;
}
counth+=((h-h1)*100)/fuck;
			}
			bp++;
		}
	}

bool emptyabove=((h==23) || !(segm->matrix + (h+1)*24*24 + y*24+x));
bool emptybelove=((h==0) || !(segm->matrix + (h-1)*24*24 + y*24+x));
bool eup=((y==0) || !(segm->matrix + (h)*24*24 + (y-1)*24+x));
bool edwn=((y==23) || !(segm->matrix + (h)*24*24 + (y+1)*24+x));
bool eleft=((x==0) || !(segm->matrix + (h)*24*24 + (y)*24+x-1));
bool eright=((x==23) || !(segm->matrix + (h)*24*24 + (y)*24+x+1));
bool undef=((abs(countx)<20) && (abs(county)<20));

if (( emptyabove &&  !eup && !edwn && !eleft && !eright) || undef || ((emptyabove) && (emptybelove))) 
	return 192;


int angl=getSector(countx,county,lightArSize);

if ((!emptyabove) && (!emptybelove)) return angl;

return (angl+lightArSize+lightArSize*(counth>900));

}

void Palette(Uint32 * colors8, Uint32** palette)
{
	// &&&&&&&&&&&&&&&& Now do the colors
Uint32* temp;
if (!(*palette)) temp=new Uint32[256]; else temp=*palette;

for (int i=1; i<256; i++)
{
Uint32 red=0,green=0,blue=0;
int colors=0;
for (int j=0; j<8; j++)
if ((i>>j) & 0x1) 
{
	red+=((*(colors8+j)) & RedMask);
	green+=((*(colors8+j)) & GreenMask);
	blue+=((*(colors8+j)) & BlueMask);
	colors++;
}

if (colors) temp[i]=(red/colors & RedMask) | (green/colors & GreenMask) | (blue/colors & BlueMask); 

}
*palette=temp;
//return temp;
}

void  MegavoxMatrixTo8BitParticle (G8_Particle* myG, int gbpiece, int dbtype, int dimm)
{
	if (myG->count) delete [] myG->pixels;

myG->radius=0;

int count=0;
int vSize=24;
// to prepare Voxel
int tempi=24*24*24;
Uint16 * bufp=mgvDB.mgvarray[gbpiece][dbtype].matrix;//mgvox.curSegment->matrix;
while (tempi)
{
if ((*bufp) && (*bufp<0xFF00)) count++;
	bufp++; tempi--;
}
	
// To use voxel
myG->truecount=count+vSize+1;
myG->pixels=new Uint32 [myG->truecount];
myG->count=count;

count=0;

Uint32 vSized=vSize/2;
int hcountStore;

for (int l=vSize-1; l>-1; l--)
{
bufp=mgvDB.mgvarray[gbpiece][dbtype].matrix+l*vSize*vSize;
	hcountStore=count;
	for (Uint32 j=0; j<vSize; j++)
	{

for (Uint32 i=0; i<vSize; i++){
	if ((*bufp) && (*bufp<0xFF00)){
		
count++;
int x=i-vSized;
int y=j-vSized;
Uint32 col=E8bitMegavoxelMatrixCalculateGradus(i, j, l, 2,&mgvDB.mgvarray[gbpiece][dbtype]);
while (col<64*dimm) col+=64;
if (!col)
	col=1;
Uint8* tmp=(Uint8*)&myG->pixels[count];
*tmp=x; tmp++; *tmp=y; tmp++; *tmp=col; tmp++; *tmp= (0x00000001<<(*bufp-1));
	/*	myG->pixels[count]=(((x+(y<<8))) | (col<<16)
			| (0x00000001<<(*bufp+23)));*/
		

	}
bufp++;
	}


	}
	myG->pixels[hcountStore]=(count-hcountStore) | ((l-8)<<16); 
count++;
}

myG->radius=vSize;

// &&&&&&&&&&&&&&&& Now do the colors

myG->palette=new Uint32[256];

Uint32* pcolors=&mgvDB.palette[gbpiece][0];
for (int i=1; i<256; i++)
{
Uint32 red=0,green=0,blue=0;
int colors=0;
for (int j=0; j<8; j++)
if ((i>>j) & 0x1) 
{
	red+=((*(pcolors+j)) & RedMask);
	green+=((*(pcolors+j)) & GreenMask);
	blue+=((*(pcolors+j)) & BlueMask);
	colors++;
}

if (colors) myG->palette[i]=(red/colors & RedMask) | (green/colors & GreenMask) | (blue/colors & BlueMask); 

}
}



void DrowColorAdjustStripes(int x, int y, int thick, int w , bool pressed)
{
int greenh=thick+2, blueh=greenh*2;
Uint32* color=&mgvDB.palette[mgvox.curpiece][mgvox.curcolor];
int r=*color & 0xFF0000, g=*color & 0xFF00, b=*color & 0xFF;

if (pressed){
	bool update=0;
	if (mouseInSquere(x, y, w, thick)) {r=255*(mouse.pos.x-x)/w; r<<=16; update=1; }
	if (mouseInSquere(x, y+greenh, w, thick)) {g=255*(mouse.pos.x-x)/w; g<<=8; update=1; }
	if (mouseInSquere(x, y+blueh, w, thick)) {b=255*(mouse.pos.x-x)/w; update=1;}
	if (update) { *color=r+g+b;   Palette(&mgvDB.palette[mgvox.curpiece][0], &mgvDB.ConvertedPalette[mgvox.curpiece]);}
}



Uint32 ** scri;
scri= new Uint32* [thick*3]; 
for (int i=0; i<thick; i++){
	scri[i]=(Uint32*)screen->pixels + (y+i)*scrwi+x;
scri[i+thick]=(Uint32*)screen->pixels + (y+i+greenh)*scrwi+x;
scri[i+thick*2]=(Uint32*)screen->pixels + (y+i+blueh)*scrwi+x;
}
for (int j=0; j<=w; j++)
{
Uint32 portion=255*j;
portion/=w;
for (int i=0; i<thick; i++)
{
*scri[i]=(portion<<16) + g + b;
scri[i]+=1;
*scri[i+thick]=(portion<<8) + r + b;
scri[i+thick]+=1;
*scri[i+thick*2]=(portion) + r + g;
scri[i+thick*2]+=1;
}
}
for (int i=0; i<thick; i++)
{
*((Uint32*)screen->pixels + (y+i)*scrwi+x+(r>>16)*w/255)=0;
*((Uint32*)screen->pixels + (y+i+greenh)*scrwi+x+(g>>8)*w/255)=0;
*((Uint32*)screen->pixels + (y+i+blueh)*scrwi+x+(b)*w/255)=0;
}

}
// Voxel Painter




void MegaVox8bitZoomDrow(Uint32* Buffer8bit, G8_Particle * G, int bufoffx, int bufoffy, int offh, float glob, double offx, double offy, int angle, int dx, int dy, Uint32 colr)//, Uint32* __restrict palpx)//, void (*onImpactFunk) (int ,int))
{	
offx+=bufoffx;
offy+=bufoffy;
if ((offx+4<0) || (offx-4>scrwi) || (offy+4<0) || (offy-4>scrhi)) return; 

colr=(0x00000001<<((colr-1)+24));

if (G->count==0) return;
double expansion=1;
expansion/=dddstength;
double expx=((double)(offx-scrwid)*expansion);
double expy=((double)(offy-scrhid)*expansion); 
int smoothx=1;
int smoothy=1;
if (angle==16) bufoffx-=1;
if (angle==32) {bufoffx-=1;bufoffy-=1; }
if (angle==48) bufoffy-=1;

//if (expx>0) { bufoffx-=0; }  else smoothx=1;
//if (expy>0) { bufoffy-=0; } else  smoothy=1; 

smoothy=(E8bitBufferDiam)*smoothy-smoothx;
int tempradius=max(palette.G8BufferRadius,max(abs(bufoffx+ expx*(8+offh) + dx)+24,abs(bufoffy+expy*(8+offh) + dy)+24));
tempradius=max(palette.G8BufferRadius,max(abs(bufoffx+ expx*(offh) + dx)+24,abs(bufoffy+expy*(offh) + dy)+24));
if (tempradius>E8bitBufferDiam/2) return;

palette.G8BufferRadius=tempradius;
FloatAnglesToCosSinMega( glob);
New8BitNtoNMega();

Uint32* __restrict gp=G->pixels;
Uint32 * __restrict bufPoint;//=&Buffer8bit[E8bitBufferSize/2+E8bitBufferDiam/2 + dy*E8bitBufferDiam + dx];
Uint32* __restrict BufPic;
int BuffCenter=E8bitBufferSize/2+E8bitBufferDiam/2+bufoffx + (dy+bufoffy)*E8bitBufferDiam + dx;
int TrueX, TrueY,TrueH;
//offh=1704-offh;
int countme=G->count;
int qh=0;
Uint32 plane=0;
if (countme>0)
do
{
plane=0;
while (!plane){
	qh=(char)(*gp>>16);
	plane=*gp & 0xFFFF;
	gp++;
}
countme-=plane;

TrueH=offh+qh+32;


bufPoint=&Buffer8bit[BuffCenter+(int)(expx*(qh+offh))+(((int)(expy*(qh+offh)))*E8bitBufferDiam)];

double exp=((qh+offh)*expansion+1);
double Tfxtox=NtoN.fxtox*exp;
double Tfxtoy=NtoN.fxtoy*exp;
double Tfytox=NtoN.fytox*exp;
double Tfytoy=NtoN.fytoy*exp;
while (plane)
{
int qx=(char)(*gp & 0xFF);
int qy=(char)((*gp>>8) & 0xFF);
TrueX=qx*Tfxtox+qy*Tfytox;
TrueY=qx*Tfxtoy+qy*Tfytoy;


BufPic=bufPoint+((TrueY)<<8)+TrueX;
//E8bitBufFillMega(BufPic, TrueH, *gp & 0xFFFF0000);

//(Uint32* BufPic, int heg, Uint32 gp)

	
short diff=TrueH-(short)(*BufPic);


Uint8 ang=*gp>>16;
if (ang!=192) ang=((ang/64*64) + ((ang + angle) % 64));
//Uint32 gpp=(*gp & 0xFF000000) | ((Uint32)ang<<16);  //((Uint32)ang<<16) ;//((*gp & 0xFF0000) + 0x100000);
Uint32 gpp=TrueH | ((Uint32)ang<<16);
if (diff>-3){
*BufPic= colr | (diff>0 ? gpp : (*BufPic));
//*BufPic= gpp | (diff>0 ? 0 :(*BufPic & 0xFF000000));

BufPic+=smoothx;
diff=TrueH-(short)(*BufPic);
if (diff>-3)
*BufPic= colr | (diff>0 ? gpp : (*BufPic));
//*BufPic= gpp | (diff>0 ? 0 :(*BufPic & 0xFF000000));

BufPic+=smoothy;
diff=TrueH-(short)(*BufPic);
if (diff>-3)
	*BufPic= colr | (diff>0 ? gpp : (*BufPic));

BufPic+=smoothx;
diff=TrueH-(short)(*BufPic);
if (diff>-3)
*BufPic= colr | (diff>0 ? gpp : (*BufPic));

}

gp++;
plane--;
}

}
while (countme);
}


// Loading and saving
void InitSegment(MegavoxSegment* segm)
{
if (!segm->initialised){
		segm->initialised=1;
		segm->matrix=new Uint16[24*24*24];
		segm->mgvoxmatrix=new Uint16[24*24*24];
		segm->PieceNtypeNang=new Uint16[24*24*24];
		for (int i=0; i<24*24*24; i++){ segm->matrix[i]=0;
		segm->mgvoxmatrix[i]=0;
		}
		for (int z=0; z<27; z++) segm->landing[z]=0;
	}
}
void SavePalette(int piece){
std::stringstream ss;
std::string strx;
ss<<mgvDB.location[piece]<<"palette.mgv";
ss>>strx;
ofstream file (strx, ios::binary);	
file.write ((char *)mgvDB.palette[piece], 8*4);
file.close (); 
}
void LoadPalette(int piece){
std::stringstream ss;
std::string strx;
ss<<mgvDB.location[piece]<<"palette.mgv";
ss>>strx;
ifstream file(strx,ios::binary);
file.read ((char *)mgvDB.palette[piece], 8*4);
file.close (); 
}
void SaveConvertedMGV(int piece, int type){
	MegavoxSegment* segm=&mgvDB.mgvarray[piece][type];
	if (segm->Converted.count){
std::stringstream ss;
std::string strx;
ss<<mgvDB.location[piece]<<type<<"converted.mgv";
ss>>strx;
ofstream file (strx, ios::binary);	
file.write ((char *)&segm->Converted.radius, 4);
file.write((char *)&segm->Converted.count, 4);
file.write ((char *)&segm->Converted.truecount, 4);
file.write ((char *)segm->Converted.pixels, segm->Converted.truecount*4);
file.close (); 
}
}

void Save(int piece, int type){
	MegavoxSegment* segm=&mgvDB.mgvarray[piece][type];
//	memset(segm->PieceNtypeNang, 0, 24*24*24*2);
	if (segm->initialised){
std::stringstream ss;
std::string strx;
ss<<mgvDB.location[piece]<<type<<".mgv";
ss>>strx;
ofstream file (strx, ios::binary);	
file.write ((char *)segm->matrix, 24*24*24*2);
file.write ((char *)segm->mgvoxmatrix, 24*24*24*2);
file.write ((char *)&segm->landing[0], 27*4);
file.write((char *)segm->PieceNtypeNang, 24*24*24*2);
file.close (); 
SavePalette(piece);
MegavoxMatrixTo8BitParticle(&segm->Converted, piece, type, 0);
SaveConvertedMGV( piece, type);


	}


	
}
void LoadConverted (std::string* location, int type, MegavoxSegment* segm){	
std::stringstream ss;
std::string strx;
ss<<*location<<type<<"converted.mgv";
ss>>strx;
ifstream file(strx,ios::binary);
if (file){
file.read ((char *)&segm->Converted.radius, 4);
file.read((char *)&segm->Converted.count, 4);
file.read ((char *)&segm->Converted.truecount, 4);
segm->Converted.pixels=new Uint32 [segm->Converted.truecount];
file.read ((char *)segm->Converted.pixels, segm->Converted.truecount*4);
}
file.close (); 


}
void Load(int piece, int type)
{
MegavoxSegment* segm=&mgvDB.mgvarray[piece][type];



std::stringstream ss;
std::string strx;
ss<<mgvDB.location[piece]<<type<<".mgv";
ss>>strx;

ifstream file(strx,ios::binary);

if (file){
InitSegment(segm);
file.read((char*)segm->matrix, 24*24*24*2);
file.read((char*)segm->mgvoxmatrix, 24*24*24*2);
file.read((char *)&segm->landing[0], 27*4);
file.read((char *)segm->PieceNtypeNang, 24*24*24*2);
}
file.close (); 
segm->MyPieceNDtype=piece*mgvpiecestypes+type;

LoadConverted (&mgvDB.location[piece], type, segm);


}
bool MouseOnScreen(EditorInterface* caller)
{
	if ((caller->x<mouse.pos.x) && (caller->x+caller->w>mouse.pos.x) && (caller->y+itfcH<mouse.pos.y) && (caller->y+caller->h+itfcH>mouse.pos.y)) return 1;
	return 0;
}
bool MouseOnButton(MyButton* caller)
{
	if ((caller->x<mouse.pos.x) && (caller->x+caller->icon->w>mouse.pos.x) && (caller->y<mouse.pos.y) && (caller->y+caller->icon->h>mouse.pos.y)) return 1;
	return 0;
}
void AddPiece(std::string location, int no, bool LoadUnconverted)
{
	mgvDB.exist[no]=1;
	mgvDB.location[no]=location;
	mgvDB.ConvertedPalette[no]=0;
	mgvDB.LoadUnconverted[no]=LoadUnconverted;
	LoadPalette(no);
	Palette(&mgvDB.palette[no][0], &mgvDB.ConvertedPalette[no]);
}
bool LoadPiece(int no)
{
	if (mgvDB.exist[no])
	{
		SDL_FreeSurface(mgvox.segmenter.text);
		mgvox.segmenter.text=TextOnlyToImage(mgvDB.location[no]);
		return 1;
	}
return 0;
}
// vox wraps

void LoadPieceType()
{

Load(mgvox.curpiece, mgvox.curpiecetype);
}
void SavePieceType()
{
	Save(mgvox.curpiece, mgvox.curpiecetype);
/*
	if (mgvox.curSegment->initialised){
std::stringstream ss;
std::string strx;
ss<<mgvDB.location[mgvox.curpiece]<<mgvox.curpiecetype<<".mgv";
ss>>strx;
ofstream file (strx, ios::binary);	
file.write ((char *)mgvox.curSegment->matrix, 24*24*24*2);
file.write ((char *)mgvox.curSegment->mgvoxmatrix, 24*24*24*2);
file.write ((char *)mgvox.curSegment->landing.p[0], 27*4);
file.close (); 

MegavoxMatrixTo8BitParticle(&mgvox.curSegment->Converted);

	}*/

}

void NextType(){
	if (mgvox.curpiecetype<mgvpiecestypes-1) mgvox.curpiecetype++;
	mgvox.curSegment=&mgvDB.mgvarray[mgvox.curpiece][mgvox.curpiecetype];
}
void PrevType(){
	if (mgvox.curpiecetype>0) mgvox.curpiecetype--;
	mgvox.curSegment=&mgvDB.mgvarray[mgvox.curpiece][mgvox.curpiecetype];
}

void NextPiece(){
	if (mgvox.curpiece<mgvpieces-1)
	if (mgvDB.exist[mgvox.curpiece+1]){
	mgvDB.curType[mgvox.curpiece]=mgvox.curpiecetype;
	mgvox.curpiece++;
	LoadPiece(mgvox.curpiece);
	mgvox.curpiecetype=mgvDB.curType[mgvox.curpiece];
	mgvox.curSegment=&mgvDB.mgvarray[mgvox.curpiece][mgvox.curpiecetype];
	if (!mgvDB.ConvertedPalette[mgvox.curpiece]) 
		Palette(&mgvDB.palette[mgvox.curpiece][0], &mgvDB.ConvertedPalette[mgvox.curpiece]);
	}
}
void PrevPiece(){
	if (mgvox.curpiece>0)
	if (mgvDB.exist[mgvox.curpiece-1]){
	mgvDB.curType[mgvox.curpiece]=mgvox.curpiecetype;
	mgvox.curpiece--;
	LoadPiece(mgvox.curpiece);
	mgvox.curpiecetype=mgvDB.curType[mgvox.curpiece];
	mgvox.curSegment=&mgvDB.mgvarray[mgvox.curpiece][mgvox.curpiecetype];
	if (!mgvDB.ConvertedPalette[mgvox.curpiece])Palette(&mgvDB.palette[mgvox.curpiece][0],  &mgvDB.ConvertedPalette[mgvox.curpiece]);
	}
}

// **************** Megavox things

void LoadPieceTypeMGVOX()
{
Load(mgvox.curMgvoxpiece, mgvox.curMgvoxpiecetype);
}
void SavePieceTypeMGVOX()
{
Save(mgvox.curMgvoxpiece, mgvox.curMgvoxpiecetype);

/*	if (mgvox.curMgvoxSegment->initialised){
std::stringstream ss;
std::string strx;
ss<<mgvDB.location[mgvox.curMgvoxpiece]<<mgvox.curMgvoxpiecetype<<".mgv";
ss>>strx;
ofstream file (strx, ios::binary);	
file.write ((char *)mgvox.curMgvoxSegment->matrix, 24*24*24*2);
file.write ((char *)mgvox.curMgvoxSegment->mgvoxmatrix, 24*24*24*2);
file.write ((char *)mgvox.curSegment->landing.p[0], 27*4);
file.close (); 

//MegavoxMatrixTo8BitParticle(&mgvox.curMgvoxSegment->Converted);

	}*/

}

void NextTypeMGVOX(){
	if (mgvox.curMgvoxpiecetype<mgvpiecestypes-1) mgvox.curMgvoxpiecetype++;
	mgvox.curMgvoxSegment=&mgvDB.mgvarray[mgvox.curMgvoxpiece][mgvox.curMgvoxpiecetype];
}
void PrevTypeMGVOX(){
	if (mgvox.curMgvoxpiecetype>0) mgvox.curMgvoxpiecetype--;
	mgvox.curMgvoxSegment=&mgvDB.mgvarray[mgvox.curMgvoxpiece][mgvox.curMgvoxpiecetype];
}
void NextPieceMGVOX(){
	//if (mgvox.curMgvoxpiecetype<mgvpiecestypes-1) mgvox.curMgvoxpiecetype++;
	//mgvox.curMgvoxSegment=&mgvDB.mgvarray[mgvox.curMgvoxpiece][mgvox.curMgvoxpiecetype];
	int p=mgvox.curMgvoxpiece;
	int t=mgvox.curMgvoxpiecetype;
	if (p<mgvpieces-1)
	if (mgvDB.exist[p+1]){
	//mgvDB.curType[mgvox.curpiece]=mgvox.curpiecetype;
	mgvox.curMgvoxpiece++;
	p++;
	LoadPiece(p);
	mgvox.curMgvoxpiecetype=mgvDB.curType[p];
	mgvox.curMgvoxSegment=&mgvDB.mgvarray[p][t];
	if (!mgvDB.ConvertedPalette[p]) 
		Palette(&mgvDB.palette[p][0], &mgvDB.ConvertedPalette[p]);
	}



}
void PrevPieceMGVOX(){
int p=mgvox.curMgvoxpiece;
int t=mgvox.curMgvoxpiecetype;
	if (p>0)//mgvpieces-1)

	if (mgvDB.exist[p-1]){
	//mgvDB.curType[mgvox.curpiece]=mgvox.curpiecetype;
	mgvox.curMgvoxpiece--;
	p--;
	LoadPiece(p);
	mgvox.curMgvoxpiecetype=mgvDB.curType[p];
	mgvox.curMgvoxSegment=&mgvDB.mgvarray[p][t];
	if (!mgvDB.ConvertedPalette[p]) 
		Palette(&mgvDB.palette[p][0], &mgvDB.ConvertedPalette[p]);
	}
}
//********************* IngameConversion
void DrowIngameVoxZoom(worldcoordinates* pos, Uint16 PnTnA, int offx, int offy, int offh, int paletteNo)//IngameMegavox* mgv, worldcoordinates* pos)
{
//PnTnA=204;
MegavoxSegment* voxel=&mgvDB.mgvarray[PnTnA/mgvpiecestypes/4][(PnTnA % (mgvpiecestypes*4))/4];


if ((offx+8*8*4<0) || (offx-8*8*4>scrwi) || (offy+8*8*4<0) || (offy-8*8*4>scrhi) || (!voxel->Converted.count)) return;

int vxtox=0, vxtoy=0, vytoy=0, vytox=0;

switch (PnTnA % 4){
case 0: vxtox=1; vytoy=1; break;
case 1: vxtoy=1; vytox=-1; break;
case 2: vxtox=-1; vytoy=-1; break;
case 3: vxtoy=-1; vytox=1; break;
}

int callCenX=offx;
int callCenY=offy;
int WindBendx=offx;
int WindBendy=offy;
//if (curwall){
//WindBendx+=accurate(curwall->_shake.offx*10);
//WindBendy+=accurate(curwall->_shake.offy*10);
//}

double expansion=1;
expansion/=dddstength;
double expx=((double)(offx-scrwid)*expansion);
double expy=((double)(offy-scrhid)*expansion); 

int buffdx=-expx*(offh+4*8);//-((callCenX-scrwid)*1350*(offh+64)/scrwid/dddstength)/2;
int buffdy=-expy*(offh+4*8);//-((callCenY-scrhid)*1350*(offh+64)/scrhid/dddstength)/2;


	//MegavoxSegment **drow=mgv->drowType;
bool bend=0;
	
for (int h=23; h>=0; h--){
Uint16* pta=&voxel->PieceNtypeNang[h*24*24];	
Uint16* col=&voxel->matrix[h*24*24];
for (int i=0; i<24; i++)
	for (int j=0; j<24; j++){
		if (*pta){
MegavoxSegment* subvoxel=&mgvDB.mgvarray[*pta/mgvpiecestypes/4][(*pta % (mgvpiecestypes*4))/4];
if (subvoxel->Converted.count){
	int ang=(PnTnA + *pta) % 4; 
	int x=j*8-12*8+4;
	int y=i*8-12*8+4;
MegaVox8bitZoomDrow(palette.Buffer8bit,&subvoxel->Converted ,x*vxtox+y*vytox , y*vytoy+x*vxtoy, (h-8)*8+offh
	, M_PI/2*(ang), 
	bend ? WindBendx : callCenX, 
	bend ? WindBendy : callCenY,(ang)*16,buffdx,buffdy, *col);
if (ZoomShadows) MegaVoxShadowDrowSafeZoom(screenShadow, &subvoxel->Converted, x*vxtox+y*vytox+offx , y*vytoy+x*vxtoy+offy, (h-8)*8+offh, M_PI/2*(ang),  (ang)*16, tera.firstZoomRefresh);
}
		}
pta++;
col++;
	}
}
	LightsTo180arrayF(pos, defAngleFade, 0, 0,deAlphaMask,0); 
	DrowBufferByHeightZoom(mgvDB.ConvertedPalette[paletteNo], callCenX - buffdx, callCenY - buffdy, screen);
	palette.G8BufferRadius=0;
}

void DrowIngameMegavoxZoom(IngameMegavox* mgv, worldcoordinates* pos)
{
	if (mgv->drowable)
	{
	if ((offx+8*4*8<0) || (offx-8*4*8>scrwi) || (offy+8*4*8<0) || (offy-8*4*8>scrhi)) return;
		for (int h=7; h>=0; h--){
			int mgdt=h*8*8;
			char* maxI=&mgv->angle[mgdt];
			for (int i=0; i<8; i++)
			for (int j=0; j<8; j++){
				MegavoxSegment* pdow=mgv->drowType[mgdt];
				if ((Uint32)pdow>0xFF){
					bool bend=(mgv->piece[mgdt]==4);
					DrowIngameVoxZoom(pos, pdow->MyPieceNDtype*4+*maxI, offx+(j*8-4*8+4)*8, offy+(i*8-4*8+4)*8,h*8*8,mgv->paletteNo );
				}
				maxI++; mgdt++;
			}
		}
	}
}

void DrowIngameMegavox(IngameMegavox* mgv, worldcoordinates* pos)
{
	if (mgv->drowable)
	{

errortest=mgv;
preoffxs(pos);
SefOffs(pos);
TreeShake();

if ((offx+8*4<0) || (offx-8*4>scrwi) || (offy+8*4<0) || (offy-8*4>scrhi)) return;

int callCenX=offx;
int callCenY=offy;
int WindBendx=offx;
int WindBendy=offy;
if (curwall){
WindBendx+=accurate(curwall->_shake.offx*3);
WindBendy+=accurate(curwall->_shake.offy*3);
}
int buffdx=-((callCenX-scrwid)*64/scrwid)/2;
int buffdy=-((callCenY-scrhid)*64/scrhid)/2;


	//MegavoxSegment **drow=mgv->drowType;
	
if ((!curwall) || (!curwall->RemoveMe)){
for (int h=7; h>=0; h--){
int mgdt=h*8*8;
char* maxI=&mgv->angle[mgdt];
for (int i=0; i<8; i++)
	for (int j=0; j<8; j++){
//mgv->drowType
MegavoxSegment* pdow=mgv->drowType[mgdt];
if ((Uint32)pdow>0xFF){
//*drow;
	bool bend=(mgv->piece[mgdt]==4);
	ertst=mgdt;

MegaVox8bitDrow(palette.Buffer8bit,&pdow->Converted , j*8-4*8+4, i*8-4*8+4, (h)*8, M_PI/2*(*maxI), 
	bend ? WindBendx : callCenX, 
	bend ? WindBendy : callCenY,(*maxI)*16,buffdx,buffdy);

}

//drow++; 
maxI++; mgdt++;
	}
}
}

// Drow Phisical Particles

if ((curwall) && (curwall->Debris>=0)){
part* phis;
int dl=curwall->Debris;
	for (int i=0; i<DebrisList[dl].Max; i++)
		if (DebrisList[dl].no[i]){
			phis=&ph[DebrisList[dl].no[i]];
			if ((phis->hp>0) && (phis->g8particle) && (phis->type)){
				offx=accurate(phis->pos.x) - actcamx + (phis->pos.tx - cameratylx )*tyleSize;
				offy=accurate(phis->pos.y) - actcamy + (phis->pos.ty - cameratyly )*tyleSize;

phis->pos.h=max(phis->pos.h,0);
int lh=phis->pos.h/dddstength;
	offx+=((offx-scrwid)*lh)>>16;
	offy+=((offy-scrhid)*lh)>>16;
	offx=offx-callCenX + buffdx;
	offy=offy-callCenY + buffdy;

	//if ((abs(offx)<92) && (abs(offy)<92)) {

	if	
		(QuickDrow8bitVex(palette.Buffer8bit,&phis->pos, phis->g8particle, offx, offy, accurate(phis->pos.h),
		1+(double)accurate(phis->pos.h)/VoxUpResize, phis->angles.Ax, phis->angles.Ay, phis->angles.Ah, 0))
				phis->power=1;
	else {
DebrisList[dl].no[i]=0;
phis->hp/=2;
	}

	//} else DebrisList[dl].no[i]=0;
				}else
			DebrisList[dl].no[i]=0;
			}

		phis=&ph[DebrisList[dl].no[DebrisList[dl].Max]];
		while (((DebrisList[dl].no[DebrisList[dl].Max]==0) || (!phis->type) || (!phis->g8particle) || (phis->hp<0)) && (DebrisList[dl].Max>0)) {
DebrisList[dl].Max--;
phis=&ph[DebrisList[dl].no[DebrisList[dl].Max]];
}
if (DebrisList[dl].Max<1) {
	DebrisList[dl].free=1;
curwall->Debris=-1;
}
}

	LightsTo180arrayF(pos, defAngleFade, 0, 0,deAlphaMask,0); 
	DrowBufferByHeight(mgvDB.ConvertedPalette[mgv->paletteNo], callCenX - buffdx, callCenY - buffdy, screen, 1);

if (curwall){
Uint32 mgvcol=*(mgvDB.ConvertedPalette[mgv->paletteNo]+2);

OnelightR=min(255, (OnelightR*((mgvcol>>16) & 0xFF)>>8));
OnelightG=min(255, (OnelightG*((mgvcol>>8) & 0xFF)>>8));
OnelightB=min(255, (OnelightB*((mgvcol) & 0xFF)>>8));

curwall->light.color=OnelightB | (OnelightG<<8) | (OnelightR<<16);
}

	}
}

void TransferIngameMegavox()
{
int piececounter=0;
edit.mgvPlay.NeedUpdate=1;

	Uint16* bufp=&edit.Megavox.matrix[24*24*8+8*24+8];
	Uint16* bufpmgv=&edit.Megavox.mgvoxmatrix[24*24*8+8*24+8];
	
	int* hp=&edit.mgvPlay.hp[0];
	char* piece=&edit.mgvPlay.piece[0];
for (int h=0; h<8; h++)
{
for (int j=0; j<8; j++)
{
for (int i=0; i<8; i++)
{
if ((*bufp<255) && (*bufp)) {
	if (*bufpmgv<16) {*piece=*bufpmgv; *hp=1000; piececounter++;} 
	else {*piece=1; *hp=0; }
}else{ *piece=0; *hp=0;}


bufp++; bufpmgv++; hp++; piece++;
}
bufp+=16; bufpmgv+=16;
}
bufp+=24*16; bufpmgv+=24*16;
}

UpdateIngameMegavox(&edit.mgvPlay, &edit.buildSite, 0);

}

bool DamageWallInGame(worldcoordinates* pos, int x, int y, int h, wally* wall, int damage)
{
	int vx=((((pos->tx-wall->pos.tx)*tyleSize+accurate(x-wall->pos.x)))+8*4)/8; 
	int vy=((((pos->ty-wall->pos.ty)*tyleSize+accurate(y-wall->pos.y)))+8*4)/8;
	h=accurate(h)/8;
	if ((h<0) || (h>7) || (vx<0) || (vx>7) || (vy<0) || (vy>7))
		h=0;
	wall->Megavoxel->hp[h*8*8+vy*8+vx]-=damage;
	
	if (wall->Megavoxel->hp[h*8*8+vy*8+vx]<1) { wall->Megavoxel->NeedUpdate=1; return 1;}
	return 0;
}
void TakeWallDamage()
{
}
void WallMGVDrow(){
if (curwall->Megavoxel->NeedUpdate) 
UpdateIngameMegavox(curwall->Megavoxel, &curwall->pos, curwall->i);
if (curwall->Megavoxel->NeedUpdate) 
UpdateIngameMegavox(curwall->Megavoxel, &curwall->pos, curwall->i);

	DrowIngameMegavox(curwall->Megavoxel, &curwall->pos);
}

int GetWall(){
int d=2;
while ((w[d].type) && (d<wlim)) d++;
if (d>wlim-2)
	return 0;

int j=1;
while ((j<inGameMrgavoxPoolSize) && (InGMGVPool[j].InUse)) j++;
if (j==inGameMrgavoxPoolSize) return 0;

w[d].Megavoxel=&InGMGVPool[j];
w[d].Megavoxel->InUse=1;
w[d].Megavoxel->NeedUpdate=1;
w[d].i=d;
w[d].type=1;
w[d].size=1;
w[d].brickcopy=0;
w[d].impparticle=wallparticle[1];//804;
w[d].solid=wallsolid[1];
w[d].hp=10;
w[d].height=0;
w[d].pos.h=0;
w[d].age=now;
w[d].RemoveMe=0;
w[d].randGenerated=0;
w[d].OnDeath=TakeWallDamage;
w[d].decay=TakeWallDamage;
w[d].drowing=WallMGVDrow;
w[d].brick=0;
w[d].gutsout=NULL;
w[d]._shake.mass=50;
w[d].Debris=-1;
int hig=w[d].pos.h+matrdepth;
if (wfirst[hig]!=NULL) wfirst[hig]->prev=&w[d];
w[d].prev=NULL;
w[d].next=wfirst[hig];
wfirst[hig]=&w[d]; 
activew=max(d,activew);

	w[d].light.color=0xFF;
	w[d].light.dinamic=0;
	w[d].light.distanceFading=0.005;
	w[d].light.angleLight=0;
	w[d].light.brightness=4;
if (WallGlobalIllumination)	AddLight(&w[d].pos, &w[d].light);

return d;
}
void SaveWall(std::ofstream* file, int i){
file->write ((char *)&w[i].type, 4);
file->write ((char *)&w[i].pos, sizeof(worldcoordinates));

//Save indexed wall  

int j=0, skip=0, read=0, from=0;

while (j<8*8*8){
	while ((w[i].Megavoxel->piece[j]==0) && (j<8*8*8)) {j++; skip++;}
from=j;
while ((w[i].Megavoxel->piece[j]!=0) && (j<8*8*8)) { j++; read++;}
file->write ((char *)&skip, 2);
if (read>0){
file->write ((char *)&read, 2);
file->write ((char *)&w[i].Megavoxel->piece[from], read);
file->write ((char *)&w[i].Megavoxel->hp[from], 4*read);
}
skip=0;
read=0;

}

//file->write ((char *)w[i].Megavoxel->piece, 8*8*8);

//file->write ((char *)w[i].Megavoxel->hp, 8*8*8*4);

//End of save indexed wall
file->write ((char *)&w[i].Megavoxel->paletteNo, 4);
}
void LoadWall(std::ifstream* file){
int d=GetWall();	
file->read((char *)&w[d].pos, sizeof(worldcoordinates));

//Load wall

memset(w[d].Megavoxel->piece, 0, 8*8*8);
memset(w[d].Megavoxel->hp, 0, 8*8*8*4);

int j=0, skip=0, read=0, from=0;

while (j<8*8*8){

file->read ((char *)&skip, 2);
j+=skip;
if (j<8*8*8){
file->read ((char *)&read, 2);
file->read ((char *)&w[d].Megavoxel->piece[j], read);
file->read ((char *)&w[d].Megavoxel->hp[j], 4*read);
j+=read;
skip=0;
read=0;
}
}

//file->read ((char *)w[d].Megavoxel->piece, 8*8*8);
//file->read ((char *)w[d].Megavoxel->hp, 8*8*8*4);



file->read ((char *)&w[d].Megavoxel->paletteNo, 4);

w[d].Megavoxel->NeedUpdate=1;
}

void EraseAllInGame(){
Uint16* bufp=&edit.Megavox.matrix[0];
Uint16* bufpmgv=&edit.Megavox.mgvoxmatrix[0];
memset(bufp, 0, 24*24*24*2);
memset(bufpmgv, 0, 24*24*24*2);
}
void PieceTypeDrowing(int offx, int offy)
{
	int displ=250;
	//MegaVox8bitDrow(palette.Buffer8bit, &mgvox.curSegment->Converted, 0, 0, 128, M_PI/2, scrwid, scrhid);
	//MegaVox8bitDrow(palette.Buffer8bit, &mgvox.curSegment->Converted, 24, 24, 128, 0.2, scrwi-24, scrhi-24);
	//MegaVox8bitDrow(palette.Buffer8bit, &mgvox.curSegment->Converted, -4, -4, 0, 0, displ, displ,0,0,0);
	//	MegaVox8bitDrow(palette.Buffer8bit, &mgvox.curSegment->Converted, -4, 12, 0, 0, displ, scrhi-displ,0,0,0);
	//		MegaVox8bitDrow(palette.Buffer8bit, &mgvox.curSegment->Converted, 12, -4, 0, 0, scrwi-displ, displ,0,0,0);
		//		MegaVox8bitDrow(palette.Buffer8bit, &mgvox.curSegment->Converted, 12, 12, 0, 0, scrwi-displ, scrhi-displ,0,0,0);



				MegaVox8bitDrow(palette.Buffer8bit, &mgvox.curSegment->Converted, 4, 4, 0, 0, mouse.pos.x, mouse.pos.y,0,0,0);
	
	LightsTo180arrayF(&testbody, defAngleFade, 0, 0,deAlphaMask,0); 
	DrowBufferByHeight(mgvDB.ConvertedPalette[mgvox.curpiece], offx, offy, screen, 1);



	/*MegaVox8bitDrow(palette.Buffer8bit, &gayVoxel[2][0], 0, 0, 128, 0.2, mouse.pos.x, mouse.pos.y);
	LightsTo180arrayF(&testbody, defAngleFade, 0, 0,deAlphaMask,0); 
	DrowBuffer(gayVoxel[2][0].palette, mouse.pos.x, mouse.pos.y, screen);*/
}

// ****************Tools
void fillWith(int value)
{
EditorInterface* caller=&mgvox.segmenter;
int sizeDisp=(1+caller->toolSizeAll);
int fromx=(mouse.pos.x-caller->x)/segmSize-sizeDisp/2;
int fromy=(mouse.pos.y-caller->y-itfcH)/segmSize-sizeDisp/2;
int fromh=max(0,min(mgvox.h+mgvox.DrowDepth,mgvox.h));
int toh=min(23,max(mgvox.h+mgvox.DrowDepth,mgvox.h));
for (int h=fromh; h<=toh; h++)
for (int i=max(0,fromy); i<min(24,fromy+sizeDisp); i++) 
	for (int j=max(0,fromx); j<min(24,fromx+sizeDisp); j++)
	{
		int off=h*24*24+i*24+j;
		mgvox.curSegment->matrix[off]=value;
		if (!mgvox.curSegment->mgvoxmatrix[off])mgvox.curSegment->mgvoxmatrix[off]=1;
	}
}
void fillMGVOXWith(int value)
{
	int segmSize=8;
EditorInterface* caller=&mgvox.editMegavox;
int x=(mouse.pos.x-caller->x)/segmSize;
int y=(mouse.pos.y-caller->y-itfcH)/segmSize;
int inar=mgvox.MgvoxH*24*24+y*24+x;
mgvox.curMgvoxSegment->mgvoxmatrix[inar]=value;
//mgvox.curMgvoxSegment->matrix[inar]=1*(value>0);//temporary
}
void fillIngameWith(int value)
{
	int segmSize=8;
	EditorInterface* caller=&edit.MegavoxMaker;
int x=(mouse.pos.x-caller->x)/segmSize;
int y=(mouse.pos.y-caller->y-itfcH)/segmSize;
	int disp=edit.mgvlev*24*24+y*24+x;
	if ((x>7) && (x<16) && (y>7) && (y<16))
if (edit.Megavox.matrix[disp]!=255)
{

edit.Megavox.mgvoxmatrix[disp]=value;
edit.Megavox.matrix[disp]=1*(value>0);
}
}
void fillLandingWith(int value){
int segmSize=24;
EditorInterface* caller=&mgvox.segmentLanding;
int x=(mouse.pos.x-caller->x)/segmSize;
int y=(mouse.pos.y-caller->y-itfcH)/segmSize;
mgvox.curSegment->landing[mgvox.landingH*9+y*3+x]=value;
}
bool fillIngameWithWorld(){
	worldcoordinates* pos=&edit.buildSite;
	Uint16* bufp=&edit.Megavox.matrix[0];
	bool collision=0;
int cenx=accurate(pos->x)-12*8;
int ceny=accurate(pos->y)-12*8;
for (int h=0; h<24; h++)
	for (int y=0; y<24; y++)
		for (int x=0; x<24; x++)
		{
			int gm =getmematrix (pos->tx, pos->ty, cenx+x*8,  ceny+y*8, (h-8)*8);
			if ((gm<einmatr) && (gm>0))
			{
					if (*bufp==0)
					*bufp=MGV_WORLD_OCCUPIED;
					else  if ((h>=8) && (*bufp!=MGV_WORLD_OCCUPIED)) collision=1;
			} 
			else
			{
				if (*bufp==MGV_WORLD_OCCUPIED) *bufp=0;
			}
bufp++;
		}
		return collision;
}

void fillWholeMegavox(){
Uint16 * bufpoint=mgvox.curMgvoxSegment->matrix;
Uint16 * MGVbufpoint=mgvox.curMgvoxSegment->mgvoxmatrix;
	for (int i=0; i<24*24*24; i++)
	{

//Uint16 * PieceNtype=mgvox.curMgvoxSegment->PieceNtypeNang+i;

		// AutoConversionPoint
		if ((*bufpoint) && (*bufpoint<0xF)) *MGVbufpoint=mgvox.curpiece;
bufpoint++;
MGVbufpoint++;
	}
}

void DrowToolUse()
{
fillWith(mgvox.curcolor+1);
}
void EraseToolUse()
{
	fillWith(0);
	/*
	int X=(mouse.pos.x-mgvox.segmenter.x)/segmSize;
	int Y=(mouse.pos.y-mgvox.segmenter.y-itfcH)/segmSize;
	mgvox.curSegment->matrix[mgvox.h*24*24+Y*24+X]=0;*/
	
}
void NoRenderToolUse()
{
	fillWith(0xFF00);
	
}
void DrowIntoMGVOX(){
	fillMGVOXWith(mgvox.editMegavox.toolColor[mgvox.LastUsedTool]);//mgvox.curpiece);

}
void EraseMGVOX()
{
fillMGVOXWith(0);
}
void brakeMgvox()
{
fillMGVOXWith(255);
}
void DrowInGameEd(){
	fillIngameWith(mgvox.curpiece);
	TransferIngameMegavox();
	//mgvox.curcolor);

}
void EraseInGameEd()
{
fillIngameWith(0);
}
void brakeInGameEd()
{
fillIngameWith(255);
}
void CreateWall()
{
	if (!edit.BuildSiteUpdated) return;
	edit.BuildSiteUpdated=0;
int d=GetWall();	

memcpy ( w[d].Megavoxel, &edit.mgvPlay, sizeof(IngameMegavox));

w[d].Megavoxel->InUse=1;
w[d].Megavoxel->NeedUpdate=1;
w[d].Megavoxel->paletteNo=mgvox.curpiece;
w[d].pos.tx=edit.buildSite.tx;
w[d].pos.ty=edit.buildSite.ty;
w[d].pos.x=edit.buildSite.x;
w[d].pos.y=edit.buildSite.y;
}
void SelectSite()
{

	MouseToPos (&edit.buildSite);
	edit.buildSite.x=edit.buildSite.x/accuracy/8*accuracy*8;
		edit.buildSite.y=edit.buildSite.y/accuracy/8*accuracy*8;
fillIngameWithWorld();
edit.BuildSiteUpdated=1;
//edit.FullScreen.toolON[mgvox.LastUsedTool]=0;

}
void SelectSiteAndSpawn()
{

	MouseToPos (&edit.buildSite);
	edit.buildSite.x=edit.buildSite.x/accuracy/8*accuracy*8;
		edit.buildSite.y=edit.buildSite.y/accuracy/8*accuracy*8;

if (fillIngameWithWorld()) 
	return;

edit.BuildSiteUpdated=1;
int d=GetWall();	

memcpy ( w[d].Megavoxel, &edit.mgvPlay, sizeof(IngameMegavox));

w[d].Megavoxel->InUse=1;
w[d].Megavoxel->NeedUpdate=1;
w[d].Megavoxel->paletteNo=mgvox.curpiece;
w[d].pos.tx=edit.buildSite.tx;
w[d].pos.ty=edit.buildSite.ty;
w[d].pos.x=edit.buildSite.x;
w[d].pos.y=edit.buildSite.y;

}
void CopySegment()
{
	mgvox.copySegment=mgvox.curSegment;
}
void PasteSegment()
{
	if ((mgvox.copySegment->initialised) && (mgvox.curSegment->initialised)){
		memcpy ( mgvox.curSegment->matrix, mgvox.copySegment->matrix, 24*24*24*2);
		memcpy ( mgvox.curSegment->mgvoxmatrix, mgvox.copySegment->mgvoxmatrix, 24*24*24*2);
	}
}
void AddRaggLimb(){

	//RagTemplate[edit.CurDollGroup][edit.CurDoll].LimbCount++;

if (editorCheckPressDistance(32)) return; 


	Uint8 * no=&RagTemplate[edit.CurDollGroup][edit.CurDoll].LimbCount;
	if (*no>=maxLimbs) return;

	AnyRagLimb* rag=&RagTemplate[edit.CurDollGroup][edit.CurDoll].limb[*no];
	RagTemplate[edit.CurDollGroup][edit.CurDoll].mass[*no].val=20;
	rag->pos.tx=rag->pos.ty=0;
	rag->pos.x=mouse.pos.x-edit.DollHouse.x;
	rag->pos.y=mouse.pos.y-edit.DollHouse.y;
	rag->G8piece=mgvox.curpiece;
	rag->G8type=mgvox.curpiecetype;

	rag->LinkData=&RagTemplate[edit.CurDollGroup][edit.CurDoll].limbDTA[*no];
	//rag->lockSize=2.5;
	//rag->animation=0;
	rag->force.mass=20;
	*no+=1;



}
void SelectLimb(){ 
	
int x=mouse.pos.x-edit.DollHouse.x;
int y=mouse.pos.y-edit.DollHouse.y;
int h=edit.DollHouse.y+edit.DollHouse.h-mouse.pos.y;

	AnyRagLimb* rag=&RagTemplate[edit.CurDollGroup][edit.CurDoll].limb[edit.Curlimb];
if ((abs(rag->pos.x-x)<24) && (abs(rag->pos.y-y)<24)){
	rag->pos.x=x;
	rag->pos.y=y;
	return;
}

if ((abs(rag->pos.x-x)<24) && (abs(h-rag->pos.h)<24)){
	if (edit.anim.MoveX) rag->pos.x=x;
	rag->pos.h=h;
	return;
}

int no=RagTemplate[edit.CurDollGroup][edit.CurDoll].LimbCount;
float dist=25;
bool hei=(mouse.pos.y-edit.DollHouse.y>edit.DollHouse.h/2);
	for (int i=0; i<no; i++)
	{
		AnyRagLimb* rag=&RagTemplate[edit.CurDollGroup][edit.CurDoll].limb[i];
		float tempdist=JustKatet(rag->pos.x-x, rag->pos.y-y);
		if (tempdist<dist)  {
			edit.Curlimb=i;
			dist=tempdist;
			hei=0;
			//rag->pos.x=x;
			//rag->pos.y=y;
			//return;
		}
tempdist=JustKatet(rag->pos.x-x, h-rag->pos.h);
		if (tempdist<dist)  {
			edit.Curlimb=i;
			dist=tempdist;
			hei=1;
			//rag->pos.x=x;
			//rag->pos.h=h;
			//return;
		}
	}
rag=&RagTemplate[edit.CurDollGroup][edit.CurDoll].limb[edit.Curlimb];

if (hei) {if (edit.anim.MoveX) rag->pos.x=x;  rag->pos.h=h;}
else {rag->pos.x=x;  rag->pos.y=y;}

}
void DragLimb(){ 
	
int x=mouse.pos.x-edit.DollHouse.x;
int y=mouse.pos.y-edit.DollHouse.y;
int h=edit.DollHouse.y+edit.DollHouse.h-mouse.pos.y;

	AnyRagLimb* rag=&RagTemplate[edit.CurDollGroup][edit.CurDoll].limb[edit.Curlimb];
	/*
if ((abs(rag->pos.x-x)<24) && (abs(rag->pos.y-y)<24)){
	rag->pos.x=x;
	rag->pos.y=y;
	return;
}

if ((abs(rag->pos.x-x)<24) && (abs(h-rag->pos.h)<24)){
	rag->pos.x=x;
	rag->pos.h=h;
	return;
}*/

//rag=&RagTemplate[edit.CurDollGroup][edit.CurDoll].limb[edit.Curlimb];

if (mouse.pos.y-edit.DollHouse.y<edit.DollHouse.h/2){rag->pos.x=x;  rag->pos.y=y;}
else {if (edit.anim.MoveX) rag->pos.x=x;  rag->pos.h=h;}

}
void ConnectLimb(){
	if (edit.Curlimb==0) return;
int no=RagTemplate[edit.CurDollGroup][edit.CurDoll].LimbCount;
	for (int i=0; i<no; i++)	{
	AnyRagLimb* rag=&RagTemplate[edit.CurDollGroup][edit.CurDoll].limb[i];
		if ((abs(rag->pos.x+edit.DollHouse.x-mouse.pos.x)<24) && (abs(rag->pos.y+edit.DollHouse.y-mouse.pos.y)<24) 
			&& (edit.Curlimb!=i)){
			RagTemplate[edit.CurDollGroup][edit.CurDoll].limb[edit.Curlimb].LinkData->connected=i;
		}

		if ((abs(rag->pos.x+edit.DollHouse.x-mouse.pos.x)<24) && (abs(edit.DollHouse.y+edit.DollHouse.h-rag->pos.h-mouse.pos.y)<24) 
			&& (edit.Curlimb!=i)){
			RagTemplate[edit.CurDollGroup][edit.CurDoll].limb[edit.Curlimb].LinkData->connected=i;
		}

	}

}
void RotateLimb(){

AnyRagLimb* rag=&RagTemplate[edit.CurDollGroup][edit.CurDoll].limb[edit.Curlimb];
LimbConnectionData * LCD=&RagTemplate[edit.CurDollGroup][edit.CurDoll].limbDTA[edit.Curlimb]; //	LCD->
	
MouseToPos (&MousePos);
//rag->angle=getAngle(mouse.pos.x-edit.DollHouse.x-rag->pos.x, mouse.pos.y-edit.DollHouse.y-rag->pos.y);
LCD->DisplayAngle=GetFloatAngleDistanceNegPos(rag->angle, getAngle(mouse.pos.x-edit.DollHouse.x-rag->pos.x, mouse.pos.y-edit.DollHouse.y-rag->pos.y));
//LCD->lockSize=min(3.1, JustKatet(mouse.pos.x-edit.DollHouse.x-rag->pos.x,  mouse.pos.y-edit.DollHouse.y-rag->pos.y)/25);

//rag->lockAngle=rag->angle+3.14;  // calculate this with current angle to connected part if such present


//rag->lockAngle=rag->angle+3.14;  // calculate this with current angle to connected part if such present

}
void ConnectionFlexibility(){

AnyRagLimb* rag=&RagTemplate[edit.CurDollGroup][edit.CurDoll].limb[edit.Curlimb];
LimbConnectionData * LCD=&RagTemplate[edit.CurDollGroup][edit.CurDoll].limbDTA[edit.Curlimb]; //	LCD->
	
MouseToPos (&MousePos);
//rag->angle=getAngle(mouse.pos.x-edit.DollHouse.x-rag->pos.x, mouse.pos.y-edit.DollHouse.y-rag->pos.y);
//LCD->DisplayAngle=GetFloatAngleDistanceNegPos(rag->angle, getAngle(mouse.pos.x-edit.DollHouse.x-rag->pos.x, mouse.pos.y-edit.DollHouse.y-rag->pos.y));
LCD->lockSize=min(3.1, JustKatet(mouse.pos.x-edit.DollHouse.x-rag->pos.x,  mouse.pos.y-edit.DollHouse.y-rag->pos.y)/25);

//rag->lockAngle=rag->angle+3.14;  // calculate this with current angle to connected part if such present


//rag->lockAngle=rag->angle+3.14;  // calculate this with current angle to connected part if such present

}
void TakeForAWalk(){

	RagDoll* doll=&RagTemplate[edit.CurDollGroup][edit.CurDoll];

if (AssignedTestPcs==doll->LimbCount) {
AssignedTestPcs=0;
return;
}


doll->limb[0].angle=0; // to make sure voxel is facing left
AssignedTestPcs=doll->LimbCount;
MouseToPos (&MousePos);
for (int i=0; i<AssignedTestPcs; i++){
	memcpy(&test[i].pos,&MousePos, sizeof(worldcoordinates));
	test[i].LinkData=&RagTemplate[edit.CurDollGroup][edit.CurDoll].limbDTA[i];
	test[i].angle=doll->limb[i].angle; 
	//test[i].lockSize=doll->limb[i].lockSize;
	test[i].pos.h=doll->limb[i].pos.h*accuracy/RagDollEditorUpsize;
	test[i].G8piece=doll->limb[i].G8piece;	
	test[i].G8type=doll->limb[i].G8type;
	//test[i].animation=doll->limb[i].animation;
	test[i].force.mass=doll->mass[i].val;
	
//LimbConnectionData * LCD=&RagTemplate[edit.CurDollGroup][edit.CurDoll].limbDTA[edit.Curlimb]; //	LCD->

	if ((doll->limb[i].LinkData->connected!=-1) && (i>0))	 {
		AnyRagLimb* connected=&RagTemplate[edit.CurDollGroup][edit.CurDoll].limb[doll->limb[i].LinkData->connected];
		katet3dD (doll->limb[i].pos.x-connected->pos.x, doll->limb[i].pos.y-connected->pos.y,
			doll->limb[i].pos.h-connected->pos.h);
		test[i].LinkData->defdist=test[i].dist=fuck/RagDollEditorUpsize;
	//	test[i].dist=JustKatet (doll->limb[i].pos.x-connected->pos.x, 
		//						doll->limb[i].pos.y-connected->pos.y)/RagDollEditorUpsize;
		test[i].LinkData->connected=doll->limb[i].LinkData->connected;
		
		test[i].LinkData->lockAngle=GetFloatAngleDistanceNegPos(connected->angle, doll->limb[i].angle)+3.14;
	}
	else 
		test[i].LinkData->connected=0;
	//test[i].DisplayAngle=doll->limb[i].DisplayAngle;
}
test[0].LinkData->connected=-1;

}
void DeleteLimb(){
if (RagTemplate[edit.CurDollGroup][edit.CurDoll].LimbCount>0)
RagTemplate[edit.CurDollGroup][edit.CurDoll].LimbCount--;
for (int i=1; i<RagTemplate[edit.CurDollGroup][edit.CurDoll].LimbCount; i++)
	if (RagTemplate[edit.CurDollGroup][edit.CurDoll].limb[i].LinkData->connected>=RagTemplate[edit.CurDollGroup][edit.CurDoll].LimbCount)
		RagTemplate[edit.CurDollGroup][edit.CurDoll].limb[i].LinkData->connected=0;
}
void MoveX(){
	edit.anim.MoveX=!edit.anim.MoveX;
}

void AnimAddLimb(){
	if (editorCheckPressDistance(32)) return; 
	int no=animEd.limbs.val;
	if (no>=MaxAnimationLimbs) return;
for (int i=0; i<maxAnimFrames; i++){
	animEd.FrameLimb[i][no].x=mouse.pos.x-edit.Puppetier.x;
	animEd.FrameLimb[i][no].y=mouse.pos.y-edit.Puppetier.y;
	animEd.FrameLimb[i][no].h=0;
}
animEd.local[no]=1;
animEd.limbs.val++;
	return;
}
void AnimAddExternal(){
	if (editorCheckPressDistance(32)) return; 
	int no=animEd.limbs.val;
	if (no>=MaxAnimationLimbs) return;
for (int i=0; i<maxAnimFrames; i++){
	animEd.FrameLimb[i][no].x=mouse.pos.x-edit.Puppetier.x;
	animEd.FrameLimb[i][no].y=mouse.pos.y-edit.Puppetier.y;
	animEd.FrameLimb[i][no].h=0;
}
animEd.local[no]=0;
animEd.limbs.val++;
	return;
}
void PrePosAnimAllign(int limb){
limbFrame* flmb=&animEd.FrameLimb[animEd.CurrentFrame.val][limb];
int x=mouse.pos.x-edit.Puppetier.x;
int y=mouse.pos.y-edit.Puppetier.y;
		flmb->x=x;
		flmb->y=y;

	if (animEd.AllignBefore){
		for (int i=0; i<animEd.CurrentFrame.val; i++){
			flmb=&animEd.FrameLimb[i][limb];
			flmb->x=x;
			flmb->y=y;
			flmb->power=animEd.AnimatorPower.val;
			}
	
		}

	if (animEd.AllignAfter){
		for (int i=animEd.CurrentFrame.val; i<maxAnimFrames; i++){
			flmb=&animEd.FrameLimb[i][limb];
			flmb->x=x;
			flmb->y=y;
			flmb->power=animEd.AnimatorPower.val;
			}
	
		}
}
void PrePosAnimAllignHeight(int limb){
	limbFrame* flmb=&animEd.FrameLimb[animEd.CurrentFrame.val][limb];
	int x=mouse.pos.x-edit.Puppetier.x;
	int h=edit.Puppetier.h+edit.Puppetier.y-mouse.pos.y;
	if (edit.anim.MoveX) flmb->x=x;
	flmb->h=h;

	if (animEd.AllignBefore){
		for (int i=0; i<animEd.CurrentFrame.val; i++){
			flmb=&animEd.FrameLimb[i][limb];
			if (edit.anim.MoveX) flmb->x=x;
			flmb->h=h;
			flmb->power=animEd.AnimatorPower.val;
			}
	
		}

	if (animEd.AllignAfter){
		for (int i=animEd.CurrentFrame.val; i<maxAnimFrames; i++){
			flmb=&animEd.FrameLimb[i][limb];
			if (edit.anim.MoveX) flmb->x=x;
			flmb->h=h;
			flmb->power=animEd.AnimatorPower.val;
			}
		}
}
void AnimSelect(){
	if (animEd.playing) return;
	int toH=edit.Puppetier.y+edit.Puppetier.h-mouse.pos.y;

	limbFrame* flmb=&animEd.FrameLimb[animEd.CurrentFrame.val][animEd.curLimb.val];
	if ((abs(flmb->x+edit.Puppetier.x-mouse.pos.x)<24) && (abs(flmb->y+edit.Puppetier.y-mouse.pos.y)<24)){
		PrePosAnimAllign(animEd.curLimb.val);
		return;
	}

	if ((abs(flmb->x+edit.Puppetier.x-mouse.pos.x)<24) && (abs(toH-flmb->h)<24)){
		PrePosAnimAllignHeight(animEd.curLimb.val);
		return;
	}

int no=animEd.limbs.val;
	for (int i=0; i<no; i++)	{
		limbFrame* flmb=&animEd.FrameLimb[animEd.CurrentFrame.val][i];
		if ((abs(flmb->x+edit.Puppetier.x-mouse.pos.x)<24) && (abs(flmb->y+edit.Puppetier.y-mouse.pos.y)<24))	{
			animEd.curLimb.val=i; 
			PrePosAnimAllign(i);
			return;
		}
		if ((abs(flmb->x+edit.Puppetier.x-mouse.pos.x)<24) && (abs(toH-flmb->h)<24)){
			animEd.curLimb.val=i; 
			PrePosAnimAllignHeight(i);
			return;
		}
	}
	if (mouse.pos.y-edit.Puppetier.y<edit.Puppetier.h/2)
		PrePosAnimAllign(animEd.curLimb.val);
	else
		PrePosAnimAllignHeight(animEd.curLimb.val);

}
void AnimDelete(){
	if (animEd.limbs.val>0) animEd.limbs.val--;
}
void LinkToAnimation(){
	int no=animEd.limbs.val;
	for (int i=0; i<no; i++)
	{
		limbFrame* flmb=&animEd.FrameLimb[animEd.CurrentFrame.val][i];
		if ((abs(flmb->x+edit.Puppetier.x-mouse.pos.x)<24) && (abs(flmb->y+edit.Puppetier.y-mouse.pos.y)<24))	{
			AnyRagLimb* rag=&RagTemplate[edit.CurDollGroup][edit.CurDoll].limb[edit.Curlimb];	
			rag->LinkData->animation=i;
			return;
		}

		if ((abs(flmb->x+edit.Puppetier.x-mouse.pos.x)<24) && (abs(edit.Puppetier.y+edit.Puppetier.h-flmb->h-mouse.pos.y)<24))	{
			AnyRagLimb* rag=&RagTemplate[edit.CurDollGroup][edit.CurDoll].limb[edit.Curlimb];	
			rag->LinkData->animation=i;
			return;
		}
	}

}
void AllignBefore(){
	animEd.AllignBefore=!animEd.AllignBefore;

}
void AllignAfter(){
	animEd.AllignAfter=!animEd.AllignAfter;

}
void AnimPlay(){
	animEd.playing=!animEd.playing;
	animEd.playingTime=(double)animEd.CurrentFrame.val*((double)animEd.AnimationTime.val/(double)animEd.frames.val);
	if (animEd.playing){
	
		ConvertAnimationToUsable (&animEd.usableTestingAnimation);
	}
}
void AnimPause(){
	animEd.playing=0;
	animEd.playingTime=(double)animEd.CurrentFrame.val*((double)animEd.AnimationTime.val/(double)animEd.frames.val);
}
void AnimateTool(){
//	if (editorCheckPressDistance(animEd.AnimatorDensity.val) || animEd.animatedLastFrame) return;  //
//	if (animEd.AllignAfter) animEd.AllignBefore=0;

	if (animEd.CurrentFrame.val>=animEd.frames.val-1)
	 {animEd.playing=0; animEd.animatedLastFrame=1; if (editorCheckPressDistance(100)) return;}
	else
	animEd.playing=1;

if (mouse.pos.y-edit.Puppetier.y<edit.Puppetier.h/2)
		PrePosAnimAllign(animEd.curLimb.val);
	else
		PrePosAnimAllignHeight(animEd.curLimb.val);


//if (animEd.AllignBefore) {if (animEd.CurrentFrame.val>0) {animEd.CurrentFrame.val-=1; animEd.animatedLastFrame=0;} else animEd.animatedLastFrame=1;} 
//else {if (animEd.CurrentFrame.val<animEd.frames.val-1) {animEd.CurrentFrame.val+=1; animEd.animatedLastFrame=0;} else animEd.animatedLastFrame=1;} 

}

void landanyToolUse()
{
fillLandingWith(0);
}
void landfillToolUse()
{
fillLandingWith(1);
}
void landemptyToolUse()
{
fillLandingWith(2);
}

void ResizeButton()
{
	//mgvox.segmenter.toolSize[event.button.button]=(mouse.pos.x-mgvox.ToolSize.x)/10;
	//mgvox.LastUsedTool=event.button.button;
}
void PickVex()
{
	mgvox.editMegavox.mouseTool[event.button.button]=DrowIntoMGVOX;
	mgvox.editMegavox.toolColor[event.button.button]=mgvox.curpiece;
}
// **********************  Interface drow

void drowButton(MyButton* button, int x , int y)
{
	button->active=1;
	button->x=x;
	button->y=y;
	sdlanyblit (x, y, button->icon, screen);
mgvox.buttonManageQ[mgvox.buttManQLastIn]=button;
mgvox.buttManQLastIn++;
}

void DrowToolDesk(EditorInterface* caller)
{
int caly=caller->y+itfcH;
DrowFrame (caller->x, caller->y, caller->w, caller->h+itfcH, 0x7777FF);

drowButton(&mgvox.drowTool, caller->x,caly);
drowButton(&mgvox.eraserTool, caller->x+40,caly);
drowButton(&mgvox.noRenderTool, caller->x,caly+40);
drowButton(&mgvox.ToolSize, caller->x,caly+80);
if (MouseOnButton(&mgvox.ToolSize))
	GetScroll(&mgvox.tools, &mgvox.DrowDepth, -7,7);
else
GetScroll(&mgvox.tools, &mgvox.segmenter.toolSizeAll,0, 9);

for (int i=0; i<4; i++)//drow size selector
DrowFrame (caller->x, caller->y, caller->w, caller->h+itfcH, 0x7777FF);

if (MouseOnScreen(caller)){
int sizeDisp=(1+mgvox.segmenter.toolSizeAll)*segmSize;
DrowFrame (mouse.pos.x-sizeDisp/2 , mouse.pos.y - sizeDisp/2, 
sizeDisp, sizeDisp, 0xFFFFFF);//Outline square

for (int i=0; i<toolhands; i++)
		if (mouse.states[i]){ 
		mgvox.LastUsedTool=i;
		mgvox.curcolor=mgvox.segmenter.toolColor[i];
		}
}
}
void DrowPalette(EditorInterface* caller)
{
int caly=caller->y+itfcH;
DrowFrame (caller->x, caller->y, caller->w, caller->h+itfcH, 0x7777FF);

for (int i=0; i<8; i++)
	drowsquareFill(caller->x+10*i,caly,10,10, mgvDB.palette[mgvox.curpiece][i]);
DrowFrame (caller->x+10*mgvox.curcolor, caly, 9, 9, 0x00FFFFFF);


bool pressed=0;
bool colorChange=0;
//if (mouse.states[SDL_BUTTON_LEFT])
/*
if ((caller->x<mouse.pos.x) && (caller->x+caller->w>mouse.pos.x) &&
		(caly<mouse.pos.y) && (caly+10>mouse.pos.y))*/
 
for (int i=1; i<toolhands; i++)
	if (mouse.states[i]){
		pressed=1;
		mgvox.LastUsedTool=i;
		
	}
if ((pressed) && (mouseInSquere(caller->x, caly, caller->w, 10)))
		mgvox.segmenter.toolColor[mgvox.LastUsedTool]=mgvox.curcolor=(mouse.pos.x-caller->x)/10;	 
// update palette if change
DrowColorAdjustStripes(caller->x, caly + 15, 25, caller->w , pressed);
}
void DrowSegment(EditorInterface* caller)
{


	
int caly=caller->y+itfcH;
DrowFrame (caller->x, caller->y, caller->w, caller->h+itfcH, 0x7777FF);



	drowButton(&mgvox.bttnLoad, caller->x+caller->w , caly);
	drowButton(&mgvox.bttnSave, caller->x+caller->w , caly+mgvox.bttnLoad.icon->h+10);

if (mgvox.curSegment->initialised)
{

	GetScroll(&mgvox.segmenter, &mgvox.h, 0, 23);


// Drow level

	for (int i=1; i<24; i++)
	for (int j=1; j<24; j++){
		*((Uint32*)screen->pixels + (caly+i*segmSize)*scrwi + caller->x + j*segmSize)=0;
	}

if (mgvox.h)	
{
Uint16 * bufpoint=mgvox.curSegment->matrix+(mgvox.h-1)*24*24;
for (int i=0; i<24; i++)
	for (int j=0; j<24; j++){
		if (*bufpoint)
			 if (*bufpoint<0xF){
				 drowsquareFill(caller->x+ j*segmSize+segmSize/4,(caly+i*segmSize)+segmSize/4,segmSize/2,segmSize/2, mgvDB.palette[mgvox.curpiece][*bufpoint-1]);}
	else 
DrowFrame (caller->x+j*segmSize+segmSize/4,(caly+i*segmSize)+segmSize/4,segmSize/2,segmSize/2, 0x000000);
			 bufpoint++;
	}
}

Uint16 * bufpoint=mgvox.curSegment->matrix+mgvox.h*24*24;
for (int i=0; i<24; i++)
	for (int j=0; j<24; j++){
		if (*bufpoint)
        if (*bufpoint<0xF){
			drowsquareFill(caller->x+ j*segmSize,(caly+i*segmSize),segmSize,segmSize, mgvDB.palette[mgvox.curpiece][*bufpoint-1]);}
		else 
DrowFrame (caller->x+j*segmSize+1, (caly+i*segmSize)+1, segmSize-2, segmSize-2, 0x000000);
		bufpoint++;
	}

if ((mgvox.h>7) && (mgvox.h<16))
DrowFrame (caller->x+segmSize*8, caly+segmSize*8, segmSize*8, segmSize*8, 0x000077);//Midle Area
int from=max(caly,caly+segmSize*(23-mgvox.h)+segmSize/4 -mgvox.DrowDepth*segmSize*(mgvox.DrowDepth>0));
int to=min(caly+caller->h-from, segmSize/2+abs(min(mgvox.DrowDepth,23-mgvox.h ))*segmSize);

DrowFrame (caller->x+caller->w-segmSize/8,from , segmSize/8,  to , 0xFFFF00);//Depth point
DrowFrame (caller->x+caller->w-segmSize/4, caly+segmSize*(23-mgvox.h)+segmSize/4, segmSize/4, segmSize/2, 0x000000);//Height point
if (MouseOnScreen(caller))
{
int sizeDisp=(1+caller->toolSizeAll)*segmSize;
DrowFrame (caller->x+(mouse.pos.x-caller->x)/segmSize*segmSize-sizeDisp/2/segmSize*segmSize,	caly+(mouse.pos.y-caly)/segmSize*segmSize-sizeDisp/2/segmSize*segmSize, 
sizeDisp, sizeDisp, 0xFFFFFF);//Outline square
}

/*
if (MouseOnScreen(caller))
{

for (int i=0; i<toolhands; i++)
	if ( (caller->toolON[i]) && (caller->mouseTool[i])) {
		if (mouse.states[i]){ 
		mgvox.LastUsedTool=i;
		mgvox.curcolor=caller->toolColor[i];
		caller->mouseTool[i]();
		}
		else caller->toolON[i]=0;
	}
}
else 
for (int i=0; i<toolhands; i++) 
	if (!mouse.states[i]) 	
	caller->toolON[i]=0;*/

}


	
}
void DrowPieceSelector(EditorInterface* caller)
{

int caly=caller->y+itfcH;
DrowFrame (caller->x, caller->y, caller->w, caller->h+itfcH, 0x7777FF);



drowButton(&mgvox.PrevPiece, caller->x, caly);
drowButton(&mgvox.NextPiece, caller->x+5+mgvox.NextPiece.icon->w, caly);

drowButton(&mgvox.PrevType, caller->x, caly+25);
PieceTypeDrowing(caller->x+34, caly+32);
drowButton(&mgvox.NextType, caller->x+mgvox.PrevType.icon->w+5+32, caly+25);

drowButton(&mgvox.pickvox, caller->x + 5+60, caly);
drowButton(&mgvox.eraseVox, caller->x + 40+60, caly);
drowButton(&mgvox.BrakeVox, caller->x + 75+60, caly);

drowButton(&mgvox.Copy, caller->x + 5, caly+60);
drowButton(&mgvox.paste, caller->x + 35, caly+60);
}
void DrowMegavoxEditor(EditorInterface* caller)
{

int segmSize=8;

	
int caly=caller->y+itfcH;
DrowFrame (caller->x, caller->y, caller->w, caller->h+itfcH, 0x7777FF);

//Piece
GetScroll(&mgvox.editMegavox, &mgvox.MgvoxH,0, 23);	
    drowButton(&mgvox.bttnLoadMgvox, caller->x+caller->w , caly);
	drowButton(&mgvox.bttnSaveMgvox, caller->x+caller->w , caly+mgvox.bttnLoad.icon->h+10);
	drowButton(&mgvox.PrevMgvoxType, caller->x+caller->w, caly+80);
	drowButton(&mgvox.NextMgvoxType, caller->x+caller->w+mgvox.PrevType.icon->w+5, caly+80);
	drowButton(&mgvox.PrevMgvoxPiece, caller->x+caller->w, caly+100);
	drowButton(&mgvox.NextMgvoxPiece, caller->x+caller->w+mgvox.PrevType.icon->w+5, caly+100);


if (mgvox.curMgvoxSegment->initialised)
{
// Drow level
	for (int i=1; i<24; i++)
	for (int j=1; j<24; j++){
		*((Uint32*)screen->pixels + (caly+i*segmSize)*scrwi + caller->x + j*segmSize)=0;
	}
if (mgvox.MgvoxH)	
{
Uint16 * bufpoint=mgvox.curMgvoxSegment->matrix+(mgvox.MgvoxH-1)*24*24;
for (int i=0; i<24; i++)
	for (int j=0; j<24; j++){
		if (*bufpoint)
			 if (*bufpoint<0xF){
				 drowsquareFill(caller->x+ j*segmSize+segmSize/4,(caly+i*segmSize)+segmSize/4,segmSize/2,segmSize/2, mgvDB.palette[mgvox.curpiece][*bufpoint-1]);}
			 bufpoint++;
	}
}

SurroundArray world;
int arpos=mgvox.MgvoxH*24*24;
Uint16 * bufpoint=mgvox.curMgvoxSegment->matrix+arpos;
Uint16 * MGVbufpoint=mgvox.curMgvoxSegment->mgvoxmatrix+arpos;
Uint16 * PieceNtype=mgvox.curMgvoxSegment->PieceNtypeNang+arpos;
for (int i=0; i<24; i++)
	for (int j=0; j<24; j++){
		// AutoConversionPoint
//if ((*bufpoint) && (*bufpoint<0xF)) *MGVbufpoint=4;
	//*MGVbufpoint=2;	
*PieceNtype=0;	
		if ((*bufpoint) && (*MGVbufpoint))
        {
for (int jj=0; jj<27; jj++ ) world.p[jj]=2;

int * wp=&world.p[0];
int h=mgvox.MgvoxH;
for (int hh=mgvox.MgvoxH-1; hh<=mgvox.MgvoxH+1; hh++)
for (int ii=i-1; ii<=i+1; ii++) 
	for (int jj=j-1; jj<=j+1; jj++){
		if ((ii>=0) && (ii<24) && (jj>=0) && (jj<24) && (hh>=0) && (hh<24))
		{
int temp=*(mgvox.curMgvoxSegment->matrix+hh*24*24 + ii*24 + jj);
int brtemp=*(mgvox.curMgvoxSegment->mgvoxmatrix+hh*24*24 + ii*24 + jj);
if (temp==0)
*wp=2;
else if (brtemp<16) 
*wp=1;
else{
*wp=(*MGVbufpoint>=16) ? 2 : 1;
}
		
		}
		
wp++;
	}

//used maxCoal instead of *MGVbufpoint

	
if (*MGVbufpoint<16)
{
maxCoal=0;
ApplicCount=0;
maxI[0]=0;

ApplicantSegm[0]=&mgvDB.mgvarray[*MGVbufpoint][0];

for (int jj=0; jj<mgvpiecestypes; jj++) {
	if (mgvDB.mgvarray[*MGVbufpoint][jj].Converted.count) 
		FindApplicance(&world, &mgvDB.mgvarray[*MGVbufpoint][jj], (h+(h % 2)*((ApplicCount)/2)+j) % 4);
}
int ASNo=GetAsno(mgvox.MgvoxH,i,j);
*PieceNtype=((ApplicantSegm[ASNo]->MyPieceNDtype)*4+maxI[ASNo])*(*bufpoint>0)*(*bufpoint<0xFF);
if (*PieceNtype){
	if 	(ApplicantSegm[ASNo]->Converted.count)
		MegaVox8bitDrow(palette.Buffer8bit, &mgvDB.mgvarray[*PieceNtype/mgvpiecestypes/4][(*PieceNtype % (mgvpiecestypes*4))/4].Converted //&ApplicantSegm[ASNo]->Converted //
, j*8-12*8+4, i*8-12*8+4, 0, M_PI/2*maxI[ASNo], scrwid, scrhid,(*PieceNtype % 4)*16,0,0);
else
drowsquareFill(caller->x+ j*segmSize,(caly+i*segmSize),segmSize,segmSize, mgvDB.palette[mgvox.curpiece][*bufpoint-1]);
}
}else	
	MegaVox8bitDrow(palette.Buffer8bit, &mgvDB.mgvarray[2][0].Converted, j*8-12*8+4, i*8-12*8+4, 0, 0, scrwid, scrhid,0,0,0);
	
			
	//MegaVox8bitDrowBrakage	
		}
	
		bufpoint++;
		MGVbufpoint++;
		PieceNtype++;
	}

LightsTo180arrayF(&testbody, defAngleFade, 0, 0,deAlphaMask,0); 
DrowBuffer(mgvDB.ConvertedPalette[mgvox.curpiece], caller->x+caller->w/2, caly+caller->h/2, screen);	

if ((mgvox.MgvoxH>7) && (mgvox.MgvoxH<16))
DrowFrame (caller->x+segmSize*8, caly+segmSize*8, segmSize*8, segmSize*8, 0x000077);//Midle Area
DrowFrame (caller->x+caller->w-segmSize/4, caly+segmSize*(23-mgvox.MgvoxH)+segmSize/4, segmSize/4, segmSize/2, 0x000000);//Height point
if (MouseOnScreen(caller))
{
int sizeDisp=(1+caller->toolSizeAll)*segmSize;
DrowFrame (caller->x+(mouse.pos.x-caller->x)/segmSize*segmSize-sizeDisp/2/segmSize*segmSize,	caly+(mouse.pos.y-caly)/segmSize*segmSize-sizeDisp/2/segmSize*segmSize, 
sizeDisp, sizeDisp, 0xFFFFFF);//Outline square
}

}


	
}
void DrowSegmentLanding(EditorInterface* caller)
{

int segmSize=24;

int caly=caller->y+itfcH;
DrowFrame (caller->x, caller->y, caller->w, caller->h+itfcH, 0x7777FF);

if (mgvox.curSegment->initialised)
{

	drowButton(&mgvox.landany, caller->x+caller->w +10 , caly);
	drowButton(&mgvox.landfill, caller->x+caller->w +10, caly+24);
	drowButton(&mgvox.landempty, caller->x+caller->w +10, caly+48);
	GetScroll(&mgvox.segmentLanding, &mgvox.landingH, 0,2);
	for (int i=1; i<3; i++)
	for (int j=1; j<3; j++){
		*((Uint32*)screen->pixels + (caly+i*segmSize)*scrwi + caller->x + j*segmSize)=0;
	}

	int * bufpoint=&mgvox.curSegment->landing[0+mgvox.landingH*9];
for (int i=0; i<3; i++)
	for (int j=0; j<3; j++){

		if (!(*bufpoint)) sdlanyblit (caller->x+segmSize*j, caly+segmSize*i, mgvox.landany.icon, screen);
		if (*bufpoint==1) sdlanyblit (caller->x+segmSize*j, caly+segmSize*i, mgvox.landfill.icon, screen);
		if (*bufpoint==2) sdlanyblit (caller->x+segmSize*j, caly+segmSize*i, mgvox.landempty.icon, screen);
		bufpoint++;
	}

if (mgvox.landingH==1) 
 drowsquareFill(caller->x+segmSize,(caly+segmSize),segmSize,segmSize, 0);

DrowFrame (caller->x+caller->w-segmSize/6, caly+segmSize*(2-mgvox.landingH)+segmSize/4, segmSize/6, segmSize/2, 0x000000);
//Height point

/*
if (MouseOnScreen(caller))
{

for (int i=0; i<toolhands; i++)
	if ( (caller->toolON[i]) && (caller->mouseTool[i])) {
		if (mouse.states[i]){ 
		mgvox.LastUsedTool=i;
		mgvox.curcolor=caller->toolColor[i];
		caller->mouseTool[i]();
		}
		else caller->toolON[i]=0;
	}
}
else 
for (int i=0; i<toolhands; i++) 
	if (!mouse.states[i]) 	
	caller->toolON[i]=0;*/

}


}
void DrowCreationViewer(EditorInterface* caller)
{
int caly=caller->y+itfcH;
DrowFrame (caller->x, caller->y, caller->w, caller->h+itfcH, 0x7777FF);

if (mgvox.curMgvoxSegment->initialised)
{
	
	drowButton(&mgvox.fillFullMegavox, caller->x+caller->w , caly);

int callCenX=caller->x+caller->w/2;
int callCenY=caly+caller->h/2;
int buffdx=-((callCenX-scrwid)*64/scrwid);
int buffdy=-((callCenY-scrhid)*64/scrhid);
SurroundArray world;
Uint16 * bufpoint=mgvox.curMgvoxSegment->matrix;
Uint16 * MGVbufpoint=mgvox.curMgvoxSegment->mgvoxmatrix;
for (int h=0; h<24; h++)
{
brakeQMax=0;
for (int i=0; i<24; i++)
	for (int j=0; j<24; j++){
		if (*bufpoint)
        if (*bufpoint<0xF){

for (int jj=0; jj<27; jj++ ) world.p[jj]=2;

int * wp=&world.p[0];
for (int hh=h-1; hh<=h+1; hh++)
for (int ii=i-1; ii<=i+1; ii++) 
	for (int jj=j-1; jj<=j+1; jj++){
		if ((ii>=0) && (ii<24) && (jj>=0) && (jj<24) && (hh>=0) && (hh<24))
		{
int temp=*(mgvox.curMgvoxSegment->matrix+hh*24*24 + ii*24 + jj);
int brtemp=*(mgvox.curMgvoxSegment->mgvoxmatrix+hh*24*24 + ii*24 + jj);
if (temp==0)
*wp=2;
else if (brtemp<16) 
*wp=1;
else{
*wp=(*MGVbufpoint>=16) ? 2 : 1;
}
		}	
wp++;
	}
int defangel=(h+i+j) % 4;
maxCoal=0;
maxI[0]=defangel;
ApplicCount=0;
if (*MGVbufpoint<16){
	ApplicantSegm[0]=&mgvDB.mgvarray[*MGVbufpoint][0];
	for (int jj=0; jj<mgvpiecestypes; jj++) 
		if (mgvDB.mgvarray[*MGVbufpoint][jj].Converted.count) 
	FindApplicance(&world, &mgvDB.mgvarray[*MGVbufpoint][jj], defangel);
int ASNo=GetAsno(mgvox.MgvoxH,i,j);


	MegaVox8bitDrow(palette.Buffer8bit, &ApplicantSegm[ASNo]->Converted, j*8-12*8+4, i*8-12*8+4, (h-8)*8, M_PI/2*maxI[ASNo], callCenX, callCenY,maxI[ASNo]*16,buffdx,buffdy);
}else{
	ApplicantSegm[0]=&mgvDB.mgvarray[2][0];
	for (int jj=0; jj<mgvpiecestypes; jj++) 
		if (mgvDB.mgvarray[2][jj].Converted.count) 
		FindApplicance(&world, &mgvDB.mgvarray[2][jj], (h+(h % 2)*((ApplicCount)/2)+j) % 4);

	int ASNo=GetAsno(mgvox.MgvoxH,i,j);

		brakageQ[brakeQMax].Applicant=ApplicantSegm[ASNo];
		brakageQ[brakeQMax].Maxi=maxI[ASNo];
		brakageQ[brakeQMax].x=j*8-12*8+4;
		brakageQ[brakeQMax].y=i*8-12*8+4;
		brakeQMax++;
}		
}

		bufpoint++;
		MGVbufpoint++;
	}
	for (int jj=0; jj<brakeQMax; jj++)
		MegaVox8bitDrow(palette.Buffer8bit, &brakageQ[jj].Applicant->Converted, brakageQ[jj].x, brakageQ[jj].y, (h-8)*8, M_PI/2*brakageQ[jj].Maxi, callCenX, callCenY,brakageQ[jj].Maxi*16,buffdx,buffdy);

}
LightsTo180arrayF(&testbody, defAngleFade, 0, 0,deAlphaMask,0); 
DrowBuffer(mgvDB.ConvertedPalette[mgvox.curpiece], callCenX - buffdx, callCenY - buffdy, screen);	
}

}

void FullScreenWindow(EditorInterface* caller)
{

	edit.FullScreen.mouseTool[1]=0;


}
void DrowIngameEditor(EditorInterface* caller)
{
int segmSize=8;

	
int caly=caller->y+itfcH;
DrowFrame (caller->x, caller->y, caller->w, caller->h+itfcH, 0x7777FF);

drowButton(&edit.AddVox, caller->x+caller->w , caly);
drowButton(&edit.RemoveVox, caller->x+caller->w , caly+30);
drowButton(&edit.BrakeVox, caller->x+caller->w, caly+60);
drowButton(&mgvox.PrevPiece, caller->x+caller->w, caly+90);
drowButton(&mgvox.NextPiece, caller->x+caller->w+mgvox.NextPiece.icon->w+24, caly+90);
drowButton(&edit.PlaceSite, caller->x+caller->w, caly+125);
drowButton(&edit.UpdateConverted, caller->x+caller->w, caly+160);
drowButton(&edit.spawnMGVOX, caller->x+caller->w+32, caly+160);
drowButton(&edit.clearAllInGame, caller->x+caller->w+32, caly+195);
drowButton(&edit.spamCraft, caller->x+caller->w+64, caly+195);
	//drowButton(&mgvox.NextMgvoxType, caller->x+caller->w+mgvox.PrevType.icon->w+5, caly+80);*/
GetScroll(&edit.MegavoxMaker, &edit.mgvlev, 0,23);	
/*
preoffxs(&edit.buildSite);
SefOffs(&edit.buildSite);
sdlanyblit (offx, offy, edit.PlaceSite.icon, screen);*/

	for (int i=1; i<24; i++)
	for (int j=1; j<24; j++){
		*((Uint32*)screen->pixels + (caly+i*segmSize)*scrwi + caller->x + j*segmSize)=0;
	}

if (edit.mgvlev)	
{
	Uint16 * bufpoint=edit.Megavox.matrix+(edit.mgvlev-1)*24*24;
for (int i=0; i<24; i++)
	for (int j=0; j<24; j++){
		if (*bufpoint)
			 if (*bufpoint<0xF){
				 drowsquareFill(caller->x+ j*segmSize+segmSize/4,(caly+i*segmSize)+segmSize/4,segmSize/2,segmSize/2, edit.MGVpalette[*bufpoint-1]);}
			 bufpoint++;
	}
}

SurroundArray world;
Uint16 * bufpoint=edit.Megavox.matrix+edit.mgvlev*24*24;
Uint16 * MGVbufpoint=edit.Megavox.mgvoxmatrix+edit.mgvlev*24*24;
for (int i=0; i<24; i++)
	for (int j=0; j<24; j++){
		if (*bufpoint){
       // if (*bufpoint<0xF){


if (*bufpoint!=255){

for (int jj=0; jj<27; jj++ ) world.p[jj]=2;

int * wp=&world.p[0];
int h=edit.mgvlev;
for (int hh=edit.mgvlev-1; hh<=edit.mgvlev+1; hh++)
for (int ii=i-1; ii<=i+1; ii++) 
	for (int jj=j-1; jj<=j+1; jj++){
		if ((ii>=0) && (ii<24) && (jj>=0) && (jj<24) && (hh>=0) && (hh<24))
		{
if (*(edit.Megavox.matrix+hh*24*24 + ii*24 + jj))
*wp=1;
else *wp=2;
		}
		
wp++;
	}


ApplicCount=0;

if (*MGVbufpoint<16)
{
maxCoal=0;
maxI[0]=0;
ApplicantSegm[0]=&mgvDB.mgvarray[*MGVbufpoint][0];

for (int jj=0; jj<mgvpiecestypes; jj++) 
	if (mgvDB.mgvarray[*MGVbufpoint][jj].Converted.count) 
FindApplicance(&world, &mgvDB.mgvarray[*MGVbufpoint][jj] , (h+(h % 2)*((ApplicCount)/2)+j) % 4);
//used maxCoal instead of *MGVbufpoint

int ASNo=GetAsno(edit.mgvlev,i,j);

if 	(ApplicantSegm[ASNo]->Converted.count)
MegaVox8bitDrow(palette.Buffer8bit, &ApplicantSegm[ASNo]->Converted, j*8-12*8+4, i*8-12*8+4, 0, M_PI/2*maxI[ASNo], scrwid, scrhid,maxI[ASNo]*16,0,0);
else
drowsquareFill(caller->x+ j*segmSize,(caly+i*segmSize),segmSize,segmSize, mgvDB.palette[mgvox.curpiece][*bufpoint-1]);
		}	
else	
	MegaVox8bitDrow(palette.Buffer8bit, &mgvDB.mgvarray[2][0].Converted, j*8-12*8+4, i*8-12*8+4, 0, 0, scrwid, scrhid,0,0,0);
} else 
drowsquareFill(caller->x+ j*segmSize,(caly+i*segmSize),segmSize,segmSize, 0x001111AA);
		
	//MegaVox8bitDrowBrakage	
		}

		bufpoint++;
		MGVbufpoint++;
	}

LightsTo180arrayF(&player.host->pos, defAngleFade, 0, 0,deAlphaMask,0); 
//DrowBuffer(edit.ConvertedPalette, caller->x+caller->w/2, caly+caller->h/2, screen);	
DrowBufferByHeight(edit.ConvertedPalette, caller->x+caller->w/2, caly+caller->h/2, screen,1);
//Drow current piece
MegaVox8bitDrow(palette.Buffer8bit, &mgvDB.mgvarray[mgvox.curpiece][0].Converted, 0, 0, 8*23,0, scrwid, scrhid,0,0,0);
//DrowBuffer(edit.ConvertedPalette, caller->x+caller->w+mgvox.NextPiece.icon->w+12, caly+90+mgvox.NextPiece.icon->h/2, screen);	
DrowBufferByHeight(edit.ConvertedPalette, caller->x+caller->w+mgvox.NextPiece.icon->w+12, caly+90+mgvox.NextPiece.icon->h/2, screen,1);

if ((edit.mgvlev>7) && (edit.mgvlev<16))
DrowFrame (caller->x+segmSize*8, caly+segmSize*8, segmSize*8, segmSize*8, 0x000077);//Midle Area
DrowFrame (caller->x+caller->w-segmSize/4, caly+segmSize*(23-edit.mgvlev)+segmSize/4, segmSize/4, segmSize/2, 0x000000);//Height point
/*
if (MouseOnScreen(caller))
{
int sizeDisp=(1+caller->toolSize[mgvox.LastUsedTool])*segmSize;
DrowFrame (caller->x+(mouse.pos.x-caller->x)/segmSize*segmSize-sizeDisp/2/segmSize*segmSize,	caly+(mouse.pos.y-caly)/segmSize*segmSize-sizeDisp/2/segmSize*segmSize, 
sizeDisp, sizeDisp, 0xFFFFFF);//Outline square

for (int i=0; i<toolhands; i++)
	if ( (caller->toolON[i]) && (caller->mouseTool[i])) {
		if (mouse.states[i]){ 
		mgvox.LastUsedTool=i;
		mgvox.curcolor=caller->toolColor[i];
		caller->mouseTool[i]();
		}
		else caller->toolON[i]=0;
	}
}
else 
for (int i=0; i<toolhands; i++) 
	if (!mouse.states[i]) 	
	caller->toolON[i]=0;

	*/

	
}
void DollHouse(EditorInterface* caller)
{
int caly=caller->y+itfcH;
int calx=caller->x+caller->w;
DrowFrame (caller->x, caller->y, caller->w, caller->h+itfcH, 0x7777FF);

drowButton(&edit.AddLimb, calx , caly);
drowButton(&edit.ConnectLimb, calx , caly+32);
	drowButton(&edit.anim.ConnectToAnimation, calx+32 , caly+32);
drowButton(&edit.RotateLimb, calx , caly+64);
	drowButton(&edit.AngleFlexibility, calx+32 , caly+64);
drowButton(&edit.SelectLimb, calx , caly+96);
	drowButton(&edit.DragLimb, calx+32 , caly+96);
drowButton(&edit.TakeForAWalk, calx , caly+128);

drowButton(&edit.MoveX, calx , caly+caller->h*3/4);
if (edit.anim.MoveX) 
	DrowFrame (calx, caly+caller->h*3/4, 30, 30, 0xFF00FF);


RagDoll* doll=&RagTemplate[edit.CurDollGroup][edit.CurDoll];


if (edit.selectedLimbNo) 
	sdlanyblit (caller->x+caller->w , caly+160, edit.selectedLimbNo, screen);
drowButton(&edit.DeleteLimb, caller->x+caller->w , caly+192);



// Outline main limb
AnyRagLimb* selectedLimb=&RagTemplate[edit.CurDollGroup][edit.CurDoll].limb[0];


DrowHTPFrame (caller->x+selectedLimb->pos.x-12, caller->y+selectedLimb->pos.y-12, 24, 24, 0x0000FF);
DrowHTPFrame (caller->x+selectedLimb->pos.x-12, caller->y+caller->h-selectedLimb->pos.h-12, 24, 24, 0x0000FF);

selectedLimb=&RagTemplate[edit.CurDollGroup][edit.CurDoll].limb[edit.Curlimb];

ManageScrollableValue(1, 10000, caller->x+caller->w, caly+223, &RagTemplate[edit.CurDollGroup][edit.CurDoll].mass[edit.Curlimb], "Mass(kg):");


DrowHTPFrame (caller->x+selectedLimb->pos.x-12, caller->y+selectedLimb->pos.y-12, 24, 24, 0x88FF00);

DrowHTPFrame (caller->x+selectedLimb->pos.x-12, caller->y+caller->h-selectedLimb->pos.h-12, 24, 24, 0x88FF00);

GetScroll(&edit.DollHouse, &selectedLimb->pos.h, 0,192);	

if (selectedLimb->LinkData->connected!=-1){
	AnyRagLimb* connectedLimb=&RagTemplate[edit.CurDollGroup][edit.CurDoll].limb[selectedLimb->LinkData->connected];
	int x=caller->x+connectedLimb->pos.x;
	int y=caller->y+connectedLimb->pos.y;
	int h=caller->y+caller->h-connectedLimb->pos.h;
	DrowLine (caller->x+selectedLimb->pos.x, caller->y+selectedLimb->pos.y,
		  x, y, 0x00FFFF);

	DrowLine (caller->x+selectedLimb->pos.x, caller->y+caller->h-selectedLimb->pos.h,
		  x, h, 0x00FFFF);

//limb->angle
float flexibility=M_PI-selectedLimb->LinkData->lockSize;
DrowLine (x, y, x-30*cos(selectedLimb->angle-flexibility), y-30*sin(selectedLimb->angle-flexibility), 0xFF0000);
DrowLine (x, y, x-30*cos(selectedLimb->angle+flexibility), y-30*sin(selectedLimb->angle+flexibility), 0xFF0000);
}

if (selectedLimb->LinkData->animation){
	limbFrame* flmb=&animEd.FrameLimb[animEd.CurrentFrame.val][selectedLimb->LinkData->animation];
	int x=caller->x+selectedLimb->pos.x;
	int y=caller->y+selectedLimb->pos.y;
	int h=caller->y+caller->h-selectedLimb->pos.h;
	DrowHTPLine (x, y, flmb->x+edit.Puppetier.x, flmb->y+edit.Puppetier.y, 0x999999);
	DrowHTPLine (x, h, flmb->x+edit.Puppetier.x, edit.Puppetier.y+edit.Puppetier.h-flmb->h, 0x999999);

}

for (int i=0; i<doll->LimbCount; i++){
AnyRagLimb* limb=&doll->limb[i];
	offx=caller->x+limb->pos.x;
	offy=caller->y+limb->pos.y;
	limb->pos.h=min(192,limb->pos.h);

	if (limb->LinkData->connected!=-1){
		AnyRagLimb* connected=&RagTemplate[edit.CurDollGroup][edit.CurDoll].limb[limb->LinkData->connected];
	//rag->angle=getAngle(mouse.pos.x-edit.DollHouse.x-rag->pos.x, mouse.pos.y-edit.DollHouse.y-rag->pos.y);
		limb->angle=getAngle(connected->pos.x-limb->pos.x, connected->pos.y-limb->pos.y );
	DrowHTPLine (caller->x+limb->pos.x, caller->y+limb->pos.y,
		  caller->x+connected->pos.x, caller->y+connected->pos.y, 0x000088);

	}

	float Angle=limb->angle+limb->LinkData->DisplayAngle;
	MegaVox8bitDrow(palette.Buffer8bit,&mgvDB.mgvarray[limb->G8piece][limb->G8type].Converted 
	, 0, 0, 0,Angle ,offx,offy,64*(Angle + (M_PI*2*(Angle<0)))/M_PI/2,0,0);
	LightsTo180arrayF(&player.host->pos, defAngleFade,Angle , 0,deAlphaMask,0);//glob
	DrowBufferByHeight(mgvDB.ConvertedPalette[limb->G8piece],  offx, offy, screen, 0);

	offx=caller->x+limb->pos.x;
	offy=caller->h+caller->y-limb->pos.h;
	DrowBufferByHeight(mgvDB.ConvertedPalette[limb->G8piece],  offx, offy, screen, 1);
}

}
bool PuppetierEventHandle(EditorInterface* caller)
{
	if ((event.type==SDL_MOUSEBUTTONUP) && (animEd.animatedLastFrame)){
		animEd.animatedLastFrame=0;
		animEd.CurrentFrame.val=0;
		animEd.playingTime=0; 
	}
return 0;
}
void Puppetier(EditorInterface* caller)
{
int caly=caller->y+itfcH;
int calx=caller->x+caller->w;
DrowFrame (caller->x, caller->y, caller->w, caller->h+itfcH, 0x7777FF);

ManageScrollableValue(10, 100, calx, caly, &animEd.AnimationTime, "AnimationTime (sec/10):[");
ManageScrollableValue(0, animEd.frames.val, calx, caly+23, &animEd.CurrentFrame, "Frame No:[");
ManageScrollableValue(1, maxAnimFrames, calx+20, caly+43, &animEd.frames, "/[");
	
DrowUpdatableValue(&animEd.limbs, calx , caly+63, "Limbs" );

drowButton(&edit.anim.AddLimb, calx , caly+93);
		drowButton(&edit.anim.AddExternal, calx + 31 , caly+93);
drowButton(&edit.anim.AllignBefore, calx , caly+123);
if (animEd.AllignBefore) DrowFrame (calx, caly+123, 28, 28, 0x88AAFF);
		drowButton(&edit.anim.AllignAfter, calx+32 , caly+123);
		if (animEd.AllignAfter) DrowFrame (calx+33, caly+123, 29, 29, 0x88AAFF);

drowButton(&edit.anim.Select, calx , caly+153);
	drowButton(&edit.anim.Animate, calx + 31 , caly+153);
drowButton(&edit.anim.Delete, calx , caly+183);
drowButton(&edit.anim.play, calx , caly+206);
if (animEd.playing) DrowFrame (calx , caly+206, 28, 28, 0x88AAFF);
	drowButton(&edit.anim.pause, calx + 32 , caly+206);
	if (!animEd.playing) DrowFrame (calx + 32, caly+206, 28, 28, 0x88AAFF);
DrowUpdatableValue(&animEd.curLimb, calx , caly+240, "CurLimb:" );
ManageScrollableValue(1, scrwid, calx, caly+260, &animEd.AnimatorDensity, "Density: ");
ManageScrollableValue(1, 256, calx, caly+280, &animEd.AnimatorPower, "Power: ");

drowButton(&edit.MoveX, calx , caly+caller->h*3/4);
if (edit.anim.MoveX) 
	DrowFrame (calx, caly+caller->h*3/4, 30, 30, 0xFF00FF);

limbFrame* selectedlimb=&animEd.FrameLimb[animEd.CurrentFrame.val][animEd.curLimb.val];
GetScroll(&edit.Puppetier, &animEd.CurrentFrame.val, 0, animEd.frames.val);
DrowHTPFrame (caller->x+selectedlimb->x-12, caller->y+selectedlimb->y-12, 24, 24, 0x88FF00);
DrowHTPFrame (caller->x+selectedlimb->x-12, caller->y+caller->h-selectedlimb->h-12, 24, 24, 0x88FF00);

selectedlimb=&animEd.FrameLimb[animEd.CurrentFrame.val][0];
selectedlimb->h=defaultAnimationCenterHeight*RagDollEditorUpsize;
DrowHTPFrame (caller->x+selectedlimb->x-12, caller->y+selectedlimb->y-12, 24, 24, 0xFF);
DrowHTPFrame (caller->x+selectedlimb->x-12, caller->y+caller->h-selectedlimb->h-12, 24, 24, 0xFF);

for (int i=0; i<animEd.limbs.val; i++){
limbFrame* limbo=&animEd.FrameLimb[animEd.CurrentFrame.val][i];
	offx=caller->x+limbo->x;
	offy=caller->y+limbo->y;
	limbo->h=min(192,limbo->h);
	float Angle=0;
	MegaVox8bitDrow(palette.Buffer8bit,&gayVoxel[12][i]
	, 0, 0, 0,Angle ,offx,offy,64*(Angle + (M_PI*2*(Angle<0)))/M_PI/2,0,0);
	LightsTo180arrayF(&player.host->pos, defAngleFade,Angle , 0,deAlphaMask,0);//glob
	DrowBufferByHeight(gayVoxel[12][0].palette,  offx, offy, screen, 0);

	offx=caller->x+limbo->x;
	offy=caller->h+caller->y-limbo->h;
	DrowBufferByHeight(gayVoxel[12][0].palette,  offx, offy, screen, 1);
}
}
//window
void manageWindow(EditorInterface* window)
{
	if (window->dragged){
		window->x=mouse.pos.x-window->dragx;
		window->y=mouse.pos.y-window->dragy;
		if (mouse.states[SDL_BUTTON_LEFT]==0) window->dragged=0;
		window->x=max(window->x,0);
		window->x=min(window->x,scrwi-window->w);
		window->y=max(window->y,0);
		window->y=min(window->y,scrhi-itfcH);
		if (window->h+window->y+itfcH>=scrhi)
		{if (!window->closed) {window->autoclose=window->closed=1;}}
		else if (window->autoclose) window->closed=0;
	}
if (!window->closed)	{
	window->functional(window);

if (MouseOnScreen(window))
{

for (int i=0; i<toolhands; i++)
	if ( (window->toolON[i]) && (window->mouseTool[i])) {
		if (mouse.states[i]){ 
		mgvox.LastUsedTool=i;
		mgvox.curcolor=window->toolColor[i];
		window->mouseTool[i]();
		}
		else window->toolON[i]=0;
	}
}
else 
for (int i=0; i<toolhands; i++) 
	if (!mouse.states[i]) 	
	window->toolON[i]=0;
}

SDL_Rect sqr;
sqr.x=window->x;
sqr.y=window->y;
sqr.w=window->w;
sqr.h=itfcH;
SDL_FillRect(screen,&sqr, window->closed*128); 
sqr.x=window->x+window->w/2-window->text->w/2;
sqr.y=window->y+itfcH/2-window->text->h/2;
SDL_BlitSurface(window->text, NULL, screen, &sqr);

mgvox.windowManageQ[mgvox.windmanQLastIn]=window; 
mgvox.windmanQLastIn++;


}
bool dragWindow(EditorInterface* window)
{
int x=mouse.pos.x;
int y=mouse.pos.y;

	if ((window->x<x) && (window->x+window->w>x) && (window->y<y) && (window->y+itfcH>y))  
	{
		if (event.button.button == SDL_BUTTON_RIGHT) { window->closed=!window->closed; if (window->closed) window->autoclose=0;
		else if (window->h+window->y+itfcH>=scrhi) window->closed=window->autoclose=1;
		}
		else{
			window->dragged=1;
			draggingWindow=1;
			window->dragx=x-window->x;
			window->dragy=y-window->y;	
			}
		return 1;
	}
return 0;
}
bool UseTool(EditorInterface* window)
{
	int x=mouse.pos.x;
	int y=mouse.pos.y;

	if ((window->x<x) && (window->x+window->w>x) && (window->y+itfcH<y) && (window->y+window->h+itfcH>y))
		{
			if (window->mouseTool[event.button.button]) {window->toolON[event.button.button]=1;
			edit.UsingTool=1;
			
			return 1;}
		}
	return 0;
}
//buttons
bool PressButton(MyButton* button)
{
	int x=mouse.pos.x;
	int y=mouse.pos.y;
	if (button->active){
		if ((x>button->x) && (x<button->x+button->icon->w) && (y>button->y) && (y<button->y+button->icon->h)){
			button->functional();
			return 1;
		}
	}
	button->active=0;
return 0;
}
bool EquipTool(MyButton* button)
{	
if (button->active)	{
	int x=mouse.pos.x;
	int y=mouse.pos.y;
		if ((x>button->x) && (x<button->x+button->icon->w) && (y>button->y) && (y<button->y+button->icon->h) )
			{
				button->assignTo->mouseTool[event.button.button]=button->functional;
				 mouse.states[event.button.button]=0;
				 button->assignTo->toolON[event.button.button]=0;
				 mgvox.LastUsedTool=event.button.button;
				 return 1;
		}
	}
	button->active=0;
	return 0;
}
void EquipToolInvoke(EditorInterface* to, int mb, void (*func)()){
to->mouseTool[mb]=func;
mouse.states[mb]=0;
to->toolON[mb]=0;
mgvox.LastUsedTool=mb;
}

// Event handling

void ButtonPressCheck()
{
if (draggingWindow)  return;
edit.UsingTool=0;

if (event.button.button==SDL_BUTTON_WHEELDOWN)
for (int i=0; i<scrollMax; i++){
	if (MouseOnScreen(scrollQ[i].window)) {
		*scrollQ[i].value=min(scrollQ[i].maxi, *scrollQ[i].value+1+4*(keystates[SDLK_LSHIFT])); return;}
} else
if (event.button.button==SDL_BUTTON_WHEELUP)
	for (int i=0; i<scrollMax; i++){
		if (MouseOnScreen(scrollQ[i].window)){
			*scrollQ[i].value=max(scrollQ[i].min, *scrollQ[i].value-1-4*(keystates[SDLK_LSHIFT])); return; }
}

for (int i=0; i<mgvox.buttManQLastIn; i++){
	if (mgvox.buttonManageQ[i]->type==0) {if (PressButton(mgvox.buttonManageQ[i])) return;}
	else
	{if (EquipTool(mgvox.buttonManageQ[i])) return;}
}



for (int i=0; i<mgvox.windmanQLastIn; i++) 
	if (dragWindow(mgvox.windowManageQ[i])) return;

for (int i=0; i<mgvox.windmanQLastIn; i++) 
	if ((mgvox.windowManageQ[i]->eventFuctional) && (!mgvox.windowManageQ[i]->closed))
	{	
	if (mgvox.windowManageQ[i]->eventFuctional(mgvox.windowManageQ[i]))	return;
	}

for (int i=0; i<mgvox.windmanQLastIn; i++) {
	if (!mgvox.windowManageQ[i]->closed){
	if (UseTool(mgvox.windowManageQ[i])) return;
	}
}

}


void EditorEventMGMT()
{

	switch (event.type){
		case SDL_MOUSEBUTTONDOWN: mouse.states[event.button.button]=1;   ButtonPressCheck();  edit.MBPressed=1;
			break;
		case SDL_MOUSEBUTTONUP:  mouse.states[event.button.button]=0; tera.Edit.drx=-100; tera.Edit.dry=-100; 
			edit.lastPressMouseX=edit.lastPressMouseY=-200; 
			break;
		case SDL_MOUSEMOTION: mouse.pos.x=event.button.x; mouse.pos.y=event.button.y; break;
		case SDL_QUIT: quit = true; break;	
		}

	if (event.type!=SDL_MOUSEBUTTONDOWN){
		for (int i=0; i<mgvox.windmanQLastIn; i++) 
			if ((mgvox.windowManageQ[i]->eventFuctional) && (!mgvox.windowManageQ[i]->closed))
				mgvox.windowManageQ[i]->eventFuctional(mgvox.windowManageQ[i]);
	}


}
void EventControl()
{
draggingWindow=0;
edit.MBPressed=0;
	while(SDL_PollEvent(&event)){
		EditorEventMGMT();
		if( event.type == SDL_QUIT ) quit = true;	
		}
mgvox.windmanQLastIn=0;
mgvox.buttManQLastIn=0;
scrollMax=0;
}

// *********** Main

void writeWindow(ofstream* file, EditorInterface* window){
file->write ((char *)&window->closed,1);
file->write ((char *)&window->autoclose,1);
file->write ((char *)&window->x,2);
file->write ((char *)&window->y,2);
}

void StoreAllWindowsConfiguration(){
std::stringstream ss;
std::string strx;
ss<<"pictures/editor/WindConfig.mgv";
ss>>strx;
ofstream file (strx, ios::binary);	
writeWindow(&file, &edit.MegavoxMaker);
writeWindow(&file, &edit.FullScreen);
writeWindow(&file, &edit.DollHouse);
writeWindow(&file, &mgvox.segmenter);
writeWindow(&file, &mgvox.palette);
writeWindow(&file, &mgvox.tools);
writeWindow(&file, &mgvox.pieceSelector);
writeWindow(&file, &mgvox.editMegavox);
writeWindow(&file, &mgvox.segmentLanding);
writeWindow(&file, &mgvox.creationViewer);
writeWindow(&file, &tera.Edit.selectorIntDecay);
writeWindow(&file, &tera.Edit.selectorHDecay);
file.write ((char *)&mgvox.curpiece,2);
file.write ((char *)&mgvox.curpiecetype,2);
writeWindow(&file, &edit.Puppetier);
file.write ((char *)&animEd.AnimatorDensity.val,4);
file.write ((char *)&animEd.AnimatorPower.val,4);


file.close (); 

StoreRagDoll(edit.CurDollGroup,edit.CurDoll);
StoreAnimation(0);
}

void readWindow(ifstream* file, EditorInterface* window){
file->read ((char *)&window->closed,1);
file->read ((char *)&window->autoclose,1);
file->read ((char *)&window->x,2);
file->read ((char *)&window->y,2);
window->x=max(window->x,0);
window->x=min(window->x,scrwi-window->w);
window->y=max(window->y,0);
window->y=min(window->y,scrhi-itfcH);
}

void ReadAllWindowsConfiguration(){
std::stringstream ss;
std::string strx;
ss<<"pictures/editor/WindConfig.mgv";
ss>>strx;
ifstream file (strx, ios::binary);	
readWindow(&file, &edit.MegavoxMaker);
readWindow(&file, &edit.FullScreen);
readWindow(&file, &edit.DollHouse);
readWindow(&file, &mgvox.segmenter);
readWindow(&file, &mgvox.palette);
readWindow(&file, &mgvox.tools);
readWindow(&file, &mgvox.pieceSelector);
readWindow(&file, &mgvox.editMegavox);
readWindow(&file, &mgvox.segmentLanding);
readWindow(&file, &mgvox.creationViewer);
readWindow(&file, &tera.Edit.selectorIntDecay);
readWindow(&file, &tera.Edit.selectorHDecay);
file.read ((char *)&mgvox.curpiece,2);
file.read ((char *)&mgvox.curpiecetype,2);
readWindow(&file, &edit.Puppetier);
file.read ((char *)&animEd.AnimatorDensity.val,4);
file.read ((char *)&animEd.AnimatorPower.val,4);
file.close (); 

for (int i=0; i<DollGroups; i++)
 for (int j=0; j<DollsInGroup; j++)
	 RagTemplate[i][j].LimbCount=0;


}

void InitMagavoxMenus(){

mgvox.editMegavox=InitEditor (700, 200, "MegavoxEditor", 8*24, 8*24, DrowMegavoxEditor,0);
mgvox.segmentLanding=InitEditor (500, 520, "Segm", 3*24, 3*24, DrowSegmentLanding,0);
mgvox.creationViewer=InitEditor (700, 450, "Creation View", E8bitBufferDiam, E8bitBufferDiam, DrowCreationViewer,0);
mgvox.segmenter=InitEditor (10, 30, "segmenter", segmSize*24, segmSize*24, DrowSegment,0);
mgvox.palette=InitEditor (500, 360, "palette", 80, 80, DrowPalette,0);
mgvox.tools=InitEditor (500, 140, "tools", 80, 120, DrowToolDesk,0);
mgvox.pieceSelector=InitEditor (600, 30, "PieceSelector", 380, 120, DrowPieceSelector,0);

mgvox.bttnLoad=InitMyButton("pictures/editor/bttnload.png", LoadPieceType,0,&mgvox.segmenter);
mgvox.bttnSave=InitMyButton("pictures/editor/bttnsave.png", SavePieceType,0,&mgvox.segmenter);//0-pressable
mgvox.drowTool=InitMyButton("pictures/editor/drowTool.png", DrowToolUse,1,&mgvox.segmenter);//1-equipable
mgvox.eraserTool=InitMyButton("pictures/editor/eraserTool.png", EraseToolUse,1,&mgvox.segmenter);
mgvox.noRenderTool=InitMyButton("pictures/editor/notRenderedTool.png", NoRenderToolUse,1,&mgvox.segmenter);
mgvox.ToolSize=InitMyButton("pictures/editor/resizeTool.png", ResizeButton,0,&mgvox.segmenter);
mgvox.NextType=InitMyButton("pictures/editor/next.png", NextType,0,&mgvox.segmenter);
mgvox.PrevType=InitMyButton("pictures/editor/prev.png", PrevType,0,&mgvox.segmenter);
mgvox.Copy=InitMyButton("pictures/editor/copy.png", CopySegment,0,&mgvox.segmenter);
mgvox.paste=InitMyButton("pictures/editor/paste.png", PasteSegment,0,&mgvox.segmenter);

mgvox.NextPiece=InitMyButton("pictures/editor/next.png", NextPiece,0, &mgvox.segmenter);
mgvox.PrevPiece=InitMyButton("pictures/editor/prev.png", PrevPiece,0, &mgvox.segmenter);

//mgvox editor
mgvox.bttnLoadMgvox=InitMyButton("pictures/editor/bttnload.png", LoadPieceTypeMGVOX,0,&mgvox.editMegavox);
mgvox.bttnSaveMgvox=InitMyButton("pictures/editor/bttnsave.png", SavePieceTypeMGVOX,0,&mgvox.editMegavox);
mgvox.NextMgvoxType=InitMyButton("pictures/editor/next.png", NextTypeMGVOX,0,&mgvox.editMegavox);
mgvox.PrevMgvoxType=InitMyButton("pictures/editor/prev.png", PrevTypeMGVOX,0,&mgvox.editMegavox);
mgvox.NextMgvoxPiece=InitMyButton("pictures/editor/next.png", NextPieceMGVOX,0,&mgvox.editMegavox);
mgvox.PrevMgvoxPiece=InitMyButton("pictures/editor/prev.png", PrevPieceMGVOX,0,&mgvox.editMegavox);

mgvox.pickvox=InitMyButton("pictures/editor/vexpick.png", PickVex,0,&mgvox.segmenter);
mgvox.eraseVox=InitMyButton("pictures/editor/eraseVox.png", EraseMGVOX,1,&mgvox.editMegavox);
mgvox.BrakeVox=InitMyButton("pictures/editor/broken.png", brakeMgvox,1,&mgvox.editMegavox);


mgvox.landany=InitMyButton("pictures/editor/any.png", landanyToolUse,1, &mgvox.segmentLanding);
mgvox.landfill=InitMyButton("pictures/editor/fill.png", landfillToolUse,1, &mgvox.segmentLanding);
mgvox.landempty=InitMyButton("pictures/editor/empty.png", landemptyToolUse,1, &mgvox.segmentLanding);
mgvox.fillFullMegavox=InitMyButton("pictures/editor/fillAll.png", fillWholeMegavox,0, &mgvox.creationViewer);

mgvox.curpiece=mgvox.curMgvoxpiece=0;
mgvox.curMgvoxpiecetype=mgvox.curpiecetype=0;
mgvox.curMgvoxSegment=mgvox.curSegment=&mgvDB.mgvarray[mgvox.curpiece][mgvox.curpiecetype];
mgvox.h=15; mgvox.MgvoxH=15; mgvox.landingH=1;
edit.selectedLimbNo=0;
}
void MegaVoxelEditor(){
EditorLightningInit();
InitMagavoxMenus();

Palette(&mgvDB.palette[0][0], &mgvDB.ConvertedPalette[0]);

LoadPiece(0);

for (int i=0; i<3; i++)
	for (int j=0; j<28; j++){
Load(i, j);
MegavoxMatrixTo8BitParticle(&mgvDB.mgvarray[i][j].Converted, i, j, 0);	
	}


	while (!quit){
	mouseLight.angleLight=0;
	SDL_FillRect(screen,NULL, 0xFFA0A0A0); 
	mousebody.x=mouse.pos.x*accuracy;
	mousebody.y=mouse.pos.y*accuracy;
	manageWindow(&mgvox.segmenter);
	manageWindow(&mgvox.pieceSelector);
	manageWindow(&mgvox.palette);
	manageWindow(&mgvox.tools);
	manageWindow(&mgvox.segmentLanding);

	manageWindow(&mgvox.editMegavox);

	manageWindow(&mgvox.creationViewer);
	//manageWindow(&edit.MegavoxMaker);
	//CreationDrowing();

	EventControl();
swapscreens();
	}
}
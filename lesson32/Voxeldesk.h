#define _USE_MATH_DEFINES

// Voxel object
//int tempppp=0;
const float BigVexPixSize=1.2;
const int BVx=450, BVy=200;
const float Vox3Deff=0.003;
int vSized;

bool bit8Mode=1;

 int sz;
 int cz;
 int sx;
 int cx;
 int sy;
 int cy;


 int xtox, xtoy, xtoh,
	ytox, ytoy, ytoh,
	htox, htoy, htoh;

 

G_Particle * testG;


//0-pencil
//1-resizer
//2-sample
//3-stamp



Gpixel * GlobGp;
Uint32 * GlobG8;
struct myUnboundedScreenButton
{
	int  x;
	int  y;
	int  w;
	int  h;
	SDL_Surface* ButtonImage;
	void (*Buttondrow) ();
	void (*ButtonPressedCheck) (int x, int y, int mb);
	void (*ButtonEffec) ();
} uButt[10];




// Voxel Structure

bool saving=false;
bool halfransparent=false;
string CreationsNames[CreationsCount];
string VoxelNames[VoxelsCount];// = new string[3];
// Temporary vector test variables

double sglob=0, sglobxh=0, sglobyh=0;
void dddkatet (long double rx, long double ry, long double rh)
{
		fuck=rx*rx+ry*ry+rh*rh;
				fuck=sqrt(fuck);				
}

void tiltSideAngle(double *ax, double* ay, double coef, int x, int y)
{
*ax=*ax+coef*(x);
*ay=*ay-coef*(y);
}
//VtestEnd
void storeG8Particle(int no, G8_Particle QG)
{
std::stringstream ss;
    ss<<"Voxels/"<<curCreation<<"/converted/cv8_"<<no<<".lr";
    std::string strx;
    ss>>strx;  
	std::ofstream file(strx);

file << QG.count;
file << " ";
file << QG.radius;
file << " ";
file << QG.truecount;
file << " ";
for (int i=0; i<QG.truecount; i++)
{
	file << QG.pixels[i];
file << " ";
}

for (int i=1; i<256; i++)
{
	file<<QG.palette[i];
	file << " ";
}

//QG.pixels;//=new Uint32 [count+1];
//QG.count=count;
}
void storeQGParticle(int no, QG_Particle QG)
{
std::stringstream ss;
    ss<<"Voxels/"<<curCreation<<"/converted/cv_"<<no<<".lr";
    std::string strx;
    ss>>strx;  
	std::ofstream file(strx);

file << QG.count;
file << " ";
file << QG.radius;
file << " ";
for (int i=0; i<QG.count+1; i++)
{
	file << QG.pixels[i];
file << " ";
}


//QG.pixels;//=new Uint32 [count+1];
//QG.count=count;
}
void storeGParticle(int no, G_Particle G)
{
std::stringstream ss;
    ss<<"Voxels/"<<curCreation<<"/converted/cgv_"<<no<<".lr";
    std::string strx;
    ss>>strx;  
	std::ofstream file(strx);

file << G.count;
file << " ";
file << G.radius;
file << " ";
for (int i=0; i<G.count+1; i++)
{
file << G.gpixels[i].color;
file << " ";
file << G.gpixels[i].TextureImagePixel;
file << " ";
file << G.gpixels[i].xyhPI;
file << " ";
}


//QG.pixels;//=new Uint32 [count+1];
//QG.count=count;
}

//
QG_Particle  MatrixToSuperFastG_ParticleSimetrical (int curVoxel, bool inGame)
{
QG_Particle myG;
myG.radius=0;

int count=0;
int vSize=vox[curVoxel].size;
// to prepare Voxel

for (int l=0; l<vSize; l++)
	for (int i=0; i<vSize/2; i++)
for (int j=0; j<vSize; j++)
	if ((vox[curVoxel].VoxelMatrix[i][j][l]) && (vox[curVoxel].VoxelMatrix[i][j][l]!=0xFFFFFF) && ((i==0)||(i==vSize-1)||(l==0)||(l==vSize-1)||(j==0)||(j==vSize-1) ||		
		((!vox[curVoxel].VoxelMatrix[i+1][j][l]) || (!vox[curVoxel].VoxelMatrix[i-1][j][l])
		|| (!vox[curVoxel].VoxelMatrix[i][j+1][l]) || (!vox[curVoxel].VoxelMatrix[i][j-1][l]) || (!vox[curVoxel].VoxelMatrix[i][j][l+1])
		|| (!vox[curVoxel].VoxelMatrix[i][j][l-1])))) 
	
		count++;
	//	if (vox[curVoxel].VoxelMatrixTP[i][j][l]>)
	

// To use voxel
myG.pixels=new Uint32 [count+1];
myG.count=count;

/*for (int i=0; i<count+1; i++)
{
myG.gpixels[i].lastOne=false;
}*/

count=-1;

Uint32 vSized=vSize/2;
int dx=vox[curVoxel].massCenter.x;
int dy=vox[curVoxel].massCenter.y;
int dh=vox[curVoxel].massCenter.h;
for (Uint32 l=0; l<vSize; l++)
{
	Uint32 recalcmask=0xFFFFFFFF;
	for (Uint32 j=0; j<vSize; j++)
	{
		recalcmask |= 0xFF00FFFF;
for (Uint32 i=0; i<vSize/2; i++)
	if ((vox[curVoxel].VoxelMatrix[i][j][l]) && (vox[curVoxel].VoxelMatrix[i][j][l]!=0xFFFFFF) && ((i==0)||(i==vSize-1)||(l==0)||(l==vSize-1)||(j==0)||(j==vSize-1) ||		
		((!vox[curVoxel].VoxelMatrix[i+1][j][l]) || (!vox[curVoxel].VoxelMatrix[i-1][j][l])
		|| (!vox[curVoxel].VoxelMatrix[i][j+1][l]) || (!vox[curVoxel].VoxelMatrix[i][j-1][l]) || (!vox[curVoxel].VoxelMatrix[i][j][l+1])
		|| (!vox[curVoxel].VoxelMatrix[i][j][l-1]))))
	{
		count++;


		myG.pixels[count]=(((i+dx-vSized+((j+dy-vSized)<<8)+((l+dh-vSized)<<16)) & 0x00FFFFFF )|(vox[curVoxel].VoxelMatrixTP[i][j][l]<<24)) & recalcmask;
		recalcmask=0xFF0000FF;
	}
/*	else 
	{
		if (vox[curVoxel].VoxelMatrix[i][j][l])
		vox[curVoxel].VoxelMatrix[i][j][l]=0x00FF00;
	}*/

	}

}
myG.pixels[count]|=0x00FFFF00;
//myG.pixels[count]|=
//myG.gpixels[count+1].lastOne=true;
myG.radius=vSize;
if (myG.count==1634) 
	myG.radius=vSize;

if (inGame) storeQGParticle(curVoxel, myG);

return myG;
}
QG_Particle  MatrixToSuperFastG_Particle (int curVoxel, bool inGame)
{
QG_Particle myG;
myG.radius=0;

int count=0;
int vSize=vox[curVoxel].size;
// to prepare Voxel

for (int l=0; l<vSize; l++)
	for (int i=0; i<vSize; i++)
for (int j=0; j<vSize; j++)
	if ((vox[curVoxel].VoxelMatrix[i][j][l]) && (vox[curVoxel].VoxelMatrix[i][j][l]!=0xFFFFFF) && ((i==0)||(i==vSize-1)||(l==0)||(l==vSize-1)||(j==0)||(j==vSize-1) ||		
		((!vox[curVoxel].VoxelMatrix[i+1][j][l]) || (!vox[curVoxel].VoxelMatrix[i-1][j][l])
		|| (!vox[curVoxel].VoxelMatrix[i][j+1][l]) || (!vox[curVoxel].VoxelMatrix[i][j-1][l]) || (!vox[curVoxel].VoxelMatrix[i][j][l+1])
		|| (!vox[curVoxel].VoxelMatrix[i][j][l-1])))) 
	
		count++;
	//	if (vox[curVoxel].VoxelMatrixTP[i][j][l]>)
	

// To use voxel
myG.pixels=new Uint32 [count+1];
myG.count=count;

/*for (int i=0; i<count+1; i++)
{
myG.gpixels[i].lastOne=false;
}*/

count=-1;

Uint32 vSized=vSize/2;
int dx=vox[curVoxel].massCenter.x;
int dy=vox[curVoxel].massCenter.y;
int dh=vox[curVoxel].massCenter.h;
for (Uint32 l=0; l<vSize; l++)
{
	Uint32 recalcmask=0xFFFFFFFF;
	for (Uint32 j=0; j<vSize; j++)
	{
		recalcmask |= 0xFF00FFFF;
for (Uint32 i=0; i<vSize; i++)
	if ((vox[curVoxel].VoxelMatrix[i][j][l]) && (vox[curVoxel].VoxelMatrix[i][j][l]!=0xFFFFFF) && ((i==0)||(i==vSize-1)||(l==0)||(l==vSize-1)||(j==0)||(j==vSize-1) ||		
		((!vox[curVoxel].VoxelMatrix[i+1][j][l]) || (!vox[curVoxel].VoxelMatrix[i-1][j][l])
		|| (!vox[curVoxel].VoxelMatrix[i][j+1][l]) || (!vox[curVoxel].VoxelMatrix[i][j-1][l]) || (!vox[curVoxel].VoxelMatrix[i][j][l+1])
		|| (!vox[curVoxel].VoxelMatrix[i][j][l-1]))))
	{
		count++;


		myG.pixels[count]=(((i+dx-vSized+((j+dy-vSized)<<8)+((l+dh-vSized)<<16)) & 0x00FFFFFF )|(vox[curVoxel].VoxelMatrixTP[i][j][l]<<24)) & recalcmask;
		recalcmask=0xFF0000FF;
	}
/*	else 
	{
		if (vox[curVoxel].VoxelMatrix[i][j][l])
		vox[curVoxel].VoxelMatrix[i][j][l]=0x00FF00;
	}*/

	}

}
myG.pixels[count]|=0x00FFFF00;
//myG.pixels[count]|=
//myG.gpixels[count+1].lastOne=true;
myG.radius=vSize;
if (myG.count==1634) 
	myG.radius=vSize;

if (inGame) storeQGParticle(curVoxel, myG);

return myG;
}
QG_Particle  MatrixToQG_Particle (int curVoxel, bool inGame)
{
QG_Particle myG;
myG.radius=0;

int count=0;
int vSize=vox[curVoxel].size;
// to prepare Voxel

for (int l=0; l<vSize; l++)
	for (int i=0; i<vSize; i++)
for (int j=0; j<vSize; j++)
	if ((vox[curVoxel].VoxelMatrix[i][j][l]) && (vox[curVoxel].VoxelMatrix[i][j][l]!=0xFFFFFF) && ((i==0)||(i==vSize-1)||(l==0)||(l==vSize-1)||(j==0)||(j==vSize-1) ||		
		((!vox[curVoxel].VoxelMatrix[i+1][j][l]) || (!vox[curVoxel].VoxelMatrix[i-1][j][l])
		|| (!vox[curVoxel].VoxelMatrix[i][j+1][l]) || (!vox[curVoxel].VoxelMatrix[i][j-1][l]) || (!vox[curVoxel].VoxelMatrix[i][j][l+1])
		|| (!vox[curVoxel].VoxelMatrix[i][j][l-1])))) 
	
		count++;
	//	if (vox[curVoxel].VoxelMatrixTP[i][j][l]>)
	

// To use voxel
myG.pixels=new Uint32 [count+1];
myG.count=count;

/*for (int i=0; i<count+1; i++)
{
myG.gpixels[i].lastOne=false;
}*/

count=-1;

Uint32 vSized=vSize/2;

int dx=vox[curVoxel].massCenter.x;
int dy=vox[curVoxel].massCenter.y;
int dh=vox[curVoxel].massCenter.h;

for (Uint32 l=0; l<vSize; l++)
{
	//Uint32 recalcmask=0xFFFFFFFF;
	for (Uint32 j=0; j<vSize; j++)
	{
	//	recalcmask |= 0xFF00FFFF;
for (Uint32 i=0; i<vSize; i++)
	if ((vox[curVoxel].VoxelMatrix[i][j][l]) && (vox[curVoxel].VoxelMatrix[i][j][l]!=0xFFFFFF) && ((i==0)||(i==vSize-1)||(l==0)||(l==vSize-1)||(j==0)||(j==vSize-1) ||		
		((!vox[curVoxel].VoxelMatrix[i+1][j][l]) || (!vox[curVoxel].VoxelMatrix[i-1][j][l])
		|| (!vox[curVoxel].VoxelMatrix[i][j+1][l]) || (!vox[curVoxel].VoxelMatrix[i][j-1][l]) || (!vox[curVoxel].VoxelMatrix[i][j][l+1])
		|| (!vox[curVoxel].VoxelMatrix[i][j][l-1]))))
	{
		count++;


		myG.pixels[count]=(((i+dx-vSized+((j+dy-vSized)<<8)+((l+dh-vSized)<<16)) & 0x00FFFFFF )|(vox[curVoxel].VoxelMatrixTP[i][j][l]<<24));
		//recalcmask=0xFF0000FF;
	}
/*	else 
	{
		if (vox[curVoxel].VoxelMatrix[i][j][l])
		vox[curVoxel].VoxelMatrix[i][j][l]=0x00FF00;
	}*/

	}

}
//myG.pixels[count]|=0x00FFFF00;
//myG.pixels[count]|=
//myG.gpixels[count+1].lastOne=true;
myG.radius=vSize;
if (myG.count==1634) 
	myG.radius=vSize;

if (inGame)  storeQGParticle(curVoxel, myG);

return myG;
}
G_Particle  MatrixToG_Particle (int curVoxel, bool inGame)
{
G_Particle myG;
myG.radius=0;

int count=0;
int vSize=vox[curVoxel].size;
// to prepare Voxel

for (int l=0; l<vSize; l++)
for (int i=0; i<vSize; i++)
for (int j=0; j<vSize; j++)
if ((vox[curVoxel].VoxelMatrix[i][j][l]) && (vox[curVoxel].VoxelMatrix[i][j][l]!=0xFFFFFF))
{
	vox[curVoxel].toBeIncluded[i][j][l]=0;

	if	((i==0)||(i==vSize-1)||(l==0)||(l==vSize-1)||(j==0)||(j==vSize-1)) count++; 
else
{
int friends=0;
for (int inl=l-1; inl<=l+1; inl++)
for (int ini=i-1; ini<=i+1; ini++)
for (int inj=j-1; inj<=j+1; inj++)
	if (vox[curVoxel].VoxelMatrix[ini][inj][inl]) friends++;
//((!vox[curVoxel].VoxelMatrix[i+1][j][l]) || (!vox[curVoxel].VoxelMatrix[i-1][j][l])
//|| (!vox[curVoxel].VoxelMatrix[i][j+1][l]) || (!vox[curVoxel].VoxelMatrix[i][j-1][l]) || (!vox[curVoxel].VoxelMatrix[i][j][l+1])
//|| (!vox[curVoxel].VoxelMatrix[i][j][l-1])))) 	
if (friends<25)	
{
	vox[curVoxel].toBeIncluded[i][j][l]=1;
	vox[curVoxel].VoxelMatrix[i][j][l]=(vox[curVoxel].VoxelMatrix[i][j][l] & 0x7FFFFFFF) 
		| (0x8F000000*(friends>20));
		count++;
}
}
}	

// To use voxel
myG.gpixels=new Gpixel [count+1];
myG.count=count;

/*for (int i=0; i<count+1; i++)
{
myG.gpixels[i].lastOne=false;
}*/

count=-1;

Uint32 vSized=vSize/2;

int dx=vox[curVoxel].massCenter.x;
int dy=vox[curVoxel].massCenter.y;
int dh=vox[curVoxel].massCenter.h;

for (Uint32 l=0; l<vSize; l++)
{
	Uint32 recalc=0x02000000;
	for (Uint32 j=0; j<vSize; j++)
	{
		recalc|=0x01000000;
for (Uint32 i=0; i<vSize; i++)
	if ((vox[curVoxel].VoxelMatrix[i][j][l]) && (vox[curVoxel].VoxelMatrix[i][j][l]!=0xFFFFFF)
		&& ((i==0)||(i==vSize-1)||(l==0)||(l==vSize-1)||(j==0)||(j==vSize-1) ||		
		(vox[curVoxel].toBeIncluded[i][j][l])))
	{
		count++;
		myG.gpixels[count].color=vox[curVoxel].VoxelMatrix[i][j][l];
//		myG.gpixels[count].htp=vox[curVoxel].VoxelMatrixA[i][j][l];
		myG.gpixels[count].TextureImagePixel = vox[curVoxel].VoxelMatrixTP[i][j][l];
		//myG.gpixels[count].h=l-vSized;
		//myG.gpixels[count].x=i-vSized;
		//myG.gpixels[count].y=j-vSized;
		int x=i+dx-vSized;
		int y=j+dy-vSized + (x<0);
		int h=l+dh-vSized + (y<(x<0));
		myG.gpixels[count].xyhPI=((x + (y<<8) + (h<<16)) & 0x00FFFFFF ) | recalc;
		recalc=0;
		//myG.gpixels[count].xyhPI=((i-vSized+((j-vSized)<<8)+((l-vSized)<<16)) & 0x00FFFFFF )|(vox[curVoxel].VoxelMatrixTP[i][j][l]<<24);
		
	}
	

/*	else 
	{
		if (vox[curVoxel].VoxelMatrix[i][j][l])
		vox[curVoxel].VoxelMatrix[i][j][l]=0x00FF00;
	}*/

	}


}
myG.gpixels[count].xyhPI|=0x03000000;
//myG.gpixels[count+1].lastOne=true;
myG.radius=vSize;
if (myG.count==1634) 
	myG.radius=vSize;

if (inGame)  storeGParticle(curVoxel, myG);

return myG;
}
G8_Particle  MatrixTo8BitParticle (int curVoxel, bool inGame)
{
G8_Particle myG;
myG.radius=0;

int count=0;
int vSize=vox[curVoxel].size;
// to prepare Voxel

for (int l=1; l<vSize-1; l++)
	for (int i=1; i<vSize-1; i++)
for (int j=1; j<vSize-1; j++)
	if ((vox[curVoxel].VoxelMatrix[i][j][l]) && (vox[curVoxel].VoxelMatrix[i][j][l]!=0xFFFFFF) && ((i==0)||(i==vSize-1)||(l==0)||(l==vSize-1)||(j==0)||(j==vSize-1) ||		
		((!vox[curVoxel].VoxelMatrix[i+1][j][l]) || (!vox[curVoxel].VoxelMatrix[i-1][j][l])
		|| (!vox[curVoxel].VoxelMatrix[i][j+1][l]) || (!vox[curVoxel].VoxelMatrix[i][j-1][l]) || (!vox[curVoxel].VoxelMatrix[i][j][l+1])
		|| (!vox[curVoxel].VoxelMatrix[i][j][l-1])))) 
	
		count++;
	//	if (vox[curVoxel].VoxelMatrixTP[i][j][l]>)
	

// To use voxel
myG.truecount=count+vSize+1;
myG.pixels=new Uint32 [myG.truecount];
myG.count=count;

/*for (int i=0; i<count+1; i++)
{
myG.gpixels[i].lastOne=false;
}*/

count=0;

Uint32 vSized=vSize/2;

int dx=vox[curVoxel].massCenter.x;
int dy=vox[curVoxel].massCenter.y;
int dh=vox[curVoxel].massCenter.h;
int hcountStore;
for (Uint32 l=1; l<vSize-1; l++)
{
	hcountStore=count;
	//count++;
	//Uint32 recalcmask=0xFFFFFFFF;
	for (Uint32 j=1; j<vSize-1; j++)
	{
	//	recalcmask |= 0xFF00FFFF;
for (Uint32 i=1; i<vSize-1; i++)
	if ((vox[curVoxel].VoxelMatrix[i][j][l]) && (vox[curVoxel].VoxelMatrix[i][j][l]!=0xFFFFFF) && ((i==0)||(i==vSize-1)||(l==0)||(l==vSize-1)||(j==0)||(j==vSize-1) ||		
		((!vox[curVoxel].VoxelMatrix[i+1][j][l]) || (!vox[curVoxel].VoxelMatrix[i-1][j][l])
		|| (!vox[curVoxel].VoxelMatrix[i][j+1][l]) || (!vox[curVoxel].VoxelMatrix[i][j-1][l]) || (!vox[curVoxel].VoxelMatrix[i][j][l+1])
		|| (!vox[curVoxel].VoxelMatrix[i][j][l-1]))))
	{
		
count++;
int x=i+dx-vSized;
int y=j+dy-vSized+(x<0);
int col=E8bitMatrixCalculateGradus(i, j, l, 3);//getGradus(x,y)/2;
//if (col>179)
//	col=0;
if (!col)
	col=1;
		vox[curVoxel].eightColMatr[i][j][l]=max(vox[curVoxel].eightColMatr[i][j][l],1);
		myG.pixels[count]=(((x+(y<<8)+(col<<16)) & 0x00FFFFFF)
			| (0x00000001<<(vox[curVoxel].eightColMatr[i][j][l]+23)));
		
		//recalcmask=0xFF0000FF;
	}
/*	else 
	{
		if (vox[curVoxel].VoxelMatrix[i][j][l])
		vox[curVoxel].VoxelMatrix[i][j][l]=0x00FF00;
	}*/

	}
	myG.pixels[hcountStore]=(count-hcountStore) | ((l+dh-vSized)<<16); 
count++;
}
//myG.pixels[count]|=0x00FFFF00;
//myG.pixels[count]|=
//myG.gpixels[count+1].lastOne=true;
myG.radius=vSize;
//if (myG.count==1634) 
//	myG.radius=vSize;


// Now do the colors

myG.palette=new Uint32[256];


for (int i=1; i<256; i++)
{
Uint32 red=0,green=0,blue=0;
int colors=0;
for (int j=0; j<8; j++)
if ((i>>j) & 0x1) 
{
	red+=((vox[curVoxel].eightPalette[j+1]) & RedMask);
	green+=((vox[curVoxel].eightPalette[j+1]) & GreenMask);
	blue+=((vox[curVoxel].eightPalette[j+1]) & BlueMask);
	colors++;
}

if (colors) myG.palette[i]=(red/colors & RedMask) | (green/colors & GreenMask) | (blue/colors & BlueMask); 

	//if ((bool*)&i+j)
{

}


}


// That's going to be hard


if (inGame)  
	storeG8Particle(curVoxel, myG);

return myG;
}

void CastShadowOnScreen()
{
	// ************************************************ SHADOWS
//int b= (gx==0) ? 5 : gx;
Uint32*	epixel=(Uint32 *)screen->pixels;
	//prepixel=(Uint8 *)storedmp[b][gy+1]->pixels+3;
	//espixel=(Uint32 *)shadowmaps[gx+1][gy+1]->pixels;
	
for (int i=0; i<scrhid; i++)
{
int spower=255;
//prepixel+=744*4;
/*for (j=0;j<256;j++)
{
if (spower>=*prepixel) spower=*prepixel;
spower+=1;
	prepixel+=4;
}*/

	for (int j=0; j<scrwi; j++)
	{
		int scrshad=*((Uint8*)epixel+3);
		if (spower>=scrshad) spower=scrshad;
		else
		{ 
			int msk, b;
			if ((int)(b=scrshad-spower)>>6)
				msk=128;
			else
			msk=256-b*2;

register int	dst=*epixel;
*epixel = (RedMask & (((dst & RedMask)* msk) >>8)) | (GreenMask & (((dst & GreenMask) * msk) >>8))  | (BlueMask & (((dst & BlueMask) * msk) >>8)) | (dst & 0xFF000000) ;
} 	
		epixel++;
		spower+=4;

	}
}
}

void InputText()
{
	   //If a key was pressed 
 

		  std::string temp = Inputstr;
		  if( Inputstr.length() <= 256 ) 
		  {
			   //If the key is a space
			  if( event.key.keysym.unicode == (Uint16)' ' )
			  { 
				   Inputstr += '_'; 
				  //Append the character
				 // Inputstr += (char)event.key.keysym.unicode; 
			  }
			   //If the key is a number 
			  else
				  if( ( event.key.keysym.unicode >= (Uint16)'0' ) && ( event.key.keysym.unicode <= (Uint16)'9' ) ) 
				  { 
					  //Append the character 
					  Inputstr += (char)event.key.keysym.unicode; 
				  } //If the key is a uppercase letter 
				  else 
					  if( ( event.key.keysym.unicode >= (Uint16)'A' ) && ( event.key.keysym.unicode <= (Uint16)'Z' ) ) { 
						  //Append the character
						  Inputstr += (char)event.key.keysym.unicode;
					  } 
					  //If the key is a lowercase letter 
					  else if( ( event.key.keysym.unicode >= (Uint16)'a' ) && ( event.key.keysym.unicode <= (Uint16)'z' ) ) {
						  //Append the character
						  Inputstr += (char)event.key.keysym.unicode;
					  } 
		  }
		   //If backspace was pressed and the string isn't blank
		  if( ( event.key.keysym.sym == SDLK_BACKSPACE ) && ( Inputstr.length() != 0 ) ) { 
			  //Remove a character from the end
			  
			 Inputstr.erase( Inputstr.length() - 1 ); }

		   //If the string was changed 
		  if( Inputstr != temp ) { 
			  //Free the old surface 
			  SDL_FreeSurface( Inputtext );
			  //Render a new text surface
			  Inputtext = TTF_RenderText_Solid( font, Inputstr.c_str(), textColor );
	
		  }
		
}

void LoadVoxel(int no)
{
std::stringstream ss;
    ss<<"Voxels/"<<curCreation<<"/v_"<<no<<".lr";
    std::string strx;
    ss>>strx;  
	std::ifstream map2(strx);

map2 >> vSize;
map2 >> vox[no].massCenter.x;
map2 >> vox[no].massCenter.y;
map2 >> vox[no].massCenter.h;



vox[no].size=vSize;

if (map2)
{

for (int i=0; i<vSize; i++)
for (int j=0; j<vSize; j++)
for (int l=0; l<vSize; l++)
{map2 >> vox[no].VoxelMatrix[i][j][l];
map2 >> vox[no].VoxelMatrixTP[i][j][l];}

for (int i=1; i<9; i++) map2 >> vox[no].eightPalette[i]; //palette.color[i];

}
else
{

for (int i=0; i<vSize; i++)
for (int j=0; j<vSize; j++)
for (int l=0; l<vSize; l++)
	vox[no].VoxelMatrix[i][j][l]=0;

for (int i=1; i<9; i++)  vox[no].eightPalette[i]=0; //palette.color[i];


}

for (int i=0; i<vSize; i++)
for (int j=0; j<vSize; j++)
for (int l=0; l<vSize; l++)
{
	map2 >> vox[no].eightColMatr[i][j][l];
	//map2 << " ";
	//map2 << vox[no].VoxelMatrixTP[i][j][l];
	//map2 << " ";
}

map2.close();

vox[no].G_p=MatrixToG_Particle (no, 0); 

}
void SaveVoxel(int no)
{
	std::stringstream ss;
    ss<<"Voxels/"<<curCreation<<"/v_"<<no<<".lr";
    std::string strx;
    ss>>strx;  
	std::ofstream map2(strx);
	vSize=vox[no].size;

map2 << vSize;
map2 << " ";
map2 << vox[no].massCenter.x;
map2 << " ";
map2 << vox[no].massCenter.y;
map2 << " ";
map2 << vox[no].massCenter.h;
map2 << " ";

for (int i=0; i<vSize; i++)
for (int j=0; j<vSize; j++)
for (int l=0; l<vSize; l++)
{
	map2 << vox[no].VoxelMatrix[i][j][l];
	map2 << " ";
	map2 << vox[no].VoxelMatrixTP[i][j][l];
	map2 << " ";
}

for (int i=1; i<9; i++)
{
map2 << vox[no].eightPalette[i];//palette.color[i];
map2 << " ";
}

for (int i=0; i<vSize; i++)
for (int j=0; j<vSize; j++)
for (int l=0; l<vSize; l++)
{
	map2 << vox[no].eightColMatr[i][j][l];
	map2 << " ";
	//map2 << vox[no].VoxelMatrixTP[i][j][l];
	//map2 << " ";
}

map2.close();
}

QG_Particle TryOpenQVexel(int no, bool symetrical, bool superfast)
{
std::stringstream ss;
ss<<"Voxels/"<<curCreation<<"/converted/cv_"<<no<<".lr";
std::string strx;
ss>>strx;  
std::ifstream file(strx);

if (!file) 
{	
LoadVoxel(no);
if (!superfast) return MatrixToQG_Particle (no,1);
else
if (!symetrical) return MatrixToSuperFastG_Particle (no, 1);
else 
return MatrixToSuperFastG_ParticleSimetrical  (no, 1);
}

QG_Particle QG;
file >> QG.count;
file >> QG.radius;
QG.pixels=new Uint32 [QG.count+1];
for (int i=0; i<QG.count+1; i++)
	file >>QG.pixels[i];
return QG;
}
G_Particle TryOpenVexel(int no)
{
std::stringstream ss;
ss<<"Voxels/"<<curCreation<<"/converted/cgv_"<<no<<".lr";
std::string strx;
ss>>strx;  
std::ifstream file(strx);

if (!file) 
{	
LoadVoxel(no);
return MatrixToG_Particle (no, 1);
}

G_Particle G;
file >> G.count;
file >> G.radius;
G.gpixels=new Gpixel [G.count+1];
for (int i=0; i<G.count+1; i++)
{
	file >>G.gpixels[i].color;
	file >>G.gpixels[i].TextureImagePixel;
	file >>G.gpixels[i].xyhPI;
}
return G;
}
G8_Particle TryOpen8bitVexel(int no)
{
std::stringstream ss;
ss<<"Voxels/"<<curCreation<<"/converted/cv8_"<<no<<".lr";
std::string strx;
ss>>strx;  
std::ifstream file(strx);

if (!file) 
{	
LoadVoxel(no);
return MatrixTo8BitParticle  (no, 1);
}

G8_Particle QG;
file >> QG.count;
file >> QG.radius;
file >> QG.truecount;
QG.pixels=new Uint32 [QG.truecount];
QG.palette=new Uint32 [256];
for (int i=0; i<QG.truecount; i++)
	file >>QG.pixels[i];

for (int i=1; i<256; i++)
	file >>QG.palette[i];


return QG;
}

void RefreshVoxelPixels(int no)
{
	int pixelssum=palette.image->w*palette.image->h;
Uint32* imgpix=(Uint32*)palette.image->pixels;
int vSize=vox[no].size;
for (int i=0; i<vSize; i++)
for (int j=0; j<vSize; j++)
for (int l=0; l<vSize; l++)
	if ((vox[no].VoxelMatrix[i][j][l]))
{
	//if (!vox[no].VoxelMatrixTP[i][j][l]) vox[no].VoxelMatrixTP[i][j][l]=1;
	
	if ((pixelssum<=vox[no].VoxelMatrixTP[i][j][l]) || (vox[no].VoxelMatrixTP[i][j][l]<0))
		vox[no].VoxelMatrixTP[i][j][l]=0;
	else 
		vox[no].VoxelMatrix[i][j][l]=*(imgpix+vox[no].VoxelMatrixTP[i][j][l]);
}
	SaveVoxel(no);
LoadVoxel(no);
}
void LoadVoxelNames()
{
	int i=0;
	std::stringstream sss;
    sss<<"Voxels/"<<curCreation<<"/VoxelNames.lr";
    std::string strx2;
    sss>>strx2;  
		std::ifstream map( strx2 );
	if (map) 
{
	VoxelNames[0]="";
	map >> VoxelNames[0]; 
	while (VoxelNames[i]!="")
	{
		LoadVoxel(i);
		i++;
VoxelNames[i]="";
      map >> VoxelNames[i]; 
	}
	}
	map.close();

// Now load the fucking Voxels
}
void LoadCreationNames()
{
	int i=0;

		std::ifstream map( "Voxels/CreationsNames.lr" );
	if (map) 
{
	CreationsNames[0]="";
	map >> CreationsNames[0]; 
	while (CreationsNames[i]!="")
	{
		i++;
CreationsNames[i]="";
      map >> CreationsNames[i]; 
	}
	}
	map.close();

// Now load the fucking Voxels
}
void SaveVoxelNames()
{
	std::stringstream sss;
    sss<<"Voxels/"<<curCreation<<"/VoxelNames.lr";
    std::string strx2;
    sss>>strx2;  
	//CreationsNames[curCreation]
std::ofstream map(strx2);
int i=0;
while (VoxelNames[i]!="")
	{
    map << VoxelNames[i]; 
	map << " ";
	i++;
	}
VoxelNames[i]=Inputstr;
i++;
VoxelNames[i]="";
	map << Inputstr;
	map << " ";
map.close();
i--;
// Now save the new Voxel you created
std::stringstream ss;
    ss<<"Voxels/"<<curCreation<<"/"<<"v_"<<i<<".lr";
    std::string strx;
    ss>>strx;  
	std::ofstream map2(strx);

for ( int ii=0; ii<vSize; ii++)
for (int j=0; j<vSize; j++)
for (int l=0; l<vSize; l++)
{
	map2 << vox[i].VoxelMatrix[ii][j][l];
	map2 << " ";
}

map2.close();
}
void SaveCreationNames()
{
std::ofstream map( "Voxels/CreationsNames.lr" );
int i=0;
while (CreationsNames[i]!="")
	{
    map << CreationsNames[i]; 
	map << " ";
	i++;
	}
CreationsNames[i]=Inputstr;
i++;
CreationsNames[i]="";
	map << Inputstr;
	map << " ";
map.close();
}

bool checkForButtonPress (int bno)
{
	if ((mouse.pos.x>uButt[bno].x) && (mouse.pos.x<uButt[bno].x+uButt[bno].w) && (mouse.pos.y>uButt[bno].y) && (mouse.pos.y<uButt[bno].y+uButt[bno].h))
		return 1;
	return 0;
}
bool TryMoveObject (int x, int y)
{

/*
		drowBigPixel (i+scrwid*1.75+vox[ii].x,l+scrhid/2+vox[ii].h,1, VoxelMatrix[i][j][l], VoxelMatrixA[i][j][l], l+128+vox[ii].y);
		drowBigPixel (l+scrwid*1.25+vox[ii].h,j+scrhid*1.75+vox[ii].y,1, VoxelMatrix[i][j][l], VoxelMatrixA[i][j][l], l+128+vox[ii].x);
		drowBigPixel (i+scrwid*1.75+vox[ii].x,j+scrhid*1.75+vox[ii].y,1, VoxelMatrix[i][j][l], VoxelMatrixA[i][j][l], l+128+vox[ii].h);
*/
	
	if (x>=scrwid*1.5) 
	
		if (y>scrhid)
	{
		vox[curVoxel].x=x-scrwid*1.75;
		vox[curVoxel].y=y-scrhid*1.5;
		//vox[curVoxel].h=curh2;
		vox[curVoxel].active=1;
		mouse.state=2;
		return 1;
	}
		else
		{
        vox[curVoxel].x=x-scrwid*1.75;
		//vox[curVoxel].y=y-scrhid*1.5;
		vox[curVoxel].h=y-scrhid/2;
		vox[curVoxel].active=1;
		mouse.state=2;
		return 1;
		}

	else
		if ((x>scrwid) && (y>scrhid))
		{
		vox[curVoxel].y=y-scrhid*1.5;
		vox[curVoxel].h=x-scrwid*1.25;
		mouse.state=2;
		return 1;
		}
		return 0;
}
bool TrySelectObject(int x, int y)
{
//int tx,ty;
	int newVoxel=0;
	bool selection=false;
if (x>=scrwid*1.5) 
if (y>scrhid)
	{

x-=scrwid*1.75;
y-=scrhid*1.5;

for (int i=0; i<VoxelsCount; i++)
	if (vox[i].active)
		if ((abs(x-vox[i].x)<vSize) && (abs(y-vox[i].y)<vSize))
		{
			selection=1;
			newVoxel=i;
			mouse.state=2;
		}

}
		else
		{
        //vox[curVoxel].x=x-scrwid*1.75;
		//vox[curVoxel].h=y-scrhid/2;
		


x-=scrwid*1.75;
y-=scrhid/2;

for (int i=0; i<VoxelsCount; i++)
	if (vox[i].active)
		if ((abs(x-vox[i].x)<vSize) && (abs(y-vox[i].h)<vSize))
		{
			selection=1;
			newVoxel=i;
			mouse.state=2;
		}



		}

	else
		if ((x>scrwid) && (y>scrhid))
		{
		//vox[curVoxel].y=y-scrhid*1.5;
		//vox[curVoxel].h=x-scrwid*1.25;
		
x-=scrwid*1.25;
y-=scrhid*1.5;

for (int i=0; i<VoxelsCount; i++)
	if (vox[i].active)
		if ((abs(x-vox[i].h)<vSize) && (abs(y-vox[i].y)<vSize))
		{
			selection=1;
			newVoxel=i;
			mouse.state=2;
		}

		}

if (selection)
{
//SaveVoxel(curVoxel);
//oadVoxel(curVoxel);
SaveVoxel(curVoxel);

curVoxel=newVoxel;
LoadVoxel(curVoxel);


return 1;
}
return 0;
}

void DrowGradient (SDL_Surface* pallette, int R, int G, int B)
{
	Uint32* gpixel = (Uint32 *) pallette->pixels;

}



void saveTogle()
{
	saving=!saving;
}
void Vinit()
{
for (int v=0; v<VoxelsCount; v++)
for (int i=0; i<vSize; i++)
for (int j=0; j<vSize; j++)
for (int l=0; l<vSize; l++)
	vox[v].VoxelMatrix[i][j][l]=0;

uButt[0].x=scrwi-256;
uButt[0].y=10;
uButt[0].h=256;
uButt[0].w=256;
uButt[0].ButtonImage=loadany("pictures/twofi.png");


uButt[1].ButtonImage=TextToImage("CreateNew",0,"");
uButt[1].h=uButt[1].ButtonImage->h;
uButt[1].w=uButt[1].ButtonImage->w;
uButt[1].x=scrwid-uButt[1].w;
uButt[1].y=scrhi-uButt[1].h;
uButt[1].ButtonEffec=&saveTogle;

uButt[2].ButtonImage=TextToImage("CANCEL",0,"");
uButt[2].h=uButt[2].ButtonImage->h;
uButt[2].w=uButt[2].ButtonImage->w;
uButt[2].x=scrwid-uButt[2].w;
uButt[2].y=scrhi-uButt[2].h-uButt[1].h;
uButt[2].ButtonEffec=&saveTogle;

uButt[3].ButtonImage=TextToImage("Done",0,"");
uButt[3].h=uButt[3].ButtonImage->h;
uButt[3].w=uButt[3].ButtonImage->w;
uButt[3].x=scrwid-uButt[3].w;
uButt[3].y=scrhi-uButt[1].h-uButt[2].h-uButt[3].h;
uButt[3].ButtonEffec=&saveTogle;

LoadVoxelNames();
}

// PixelOperation Functions:
void drowCatchNOutline (int x,int y,int w, int color, int h)
{
if ((y<0) || (y+w>scrhi) || (x<0) || (x+w>scrwi)) return;
//h+=128;

color=(color & 0x00FFFFFF) | (h<<24);
if ((mouse.pos.x>=x) && (mouse.pos.x<x+w) && (mouse.pos.y>=y) && (mouse.pos.y<y+w))
{
//Uint32* screenPix=(Uint32*)screen->pixels+mouse.pos.y*screen->w+mouse.pos.x;
//int th=*screenPix>>24;
if (palette.currentH>h) 
{
palette.currentGpixel=GlobGp;
palette.currentH=h;
color|=0x00FFFFFF;
}
}

h<<=24;
int off=screen->w-w;
Uint32* screenPix=(Uint32*)screen->pixels+y*screen->w+x;
	for (int i=0; i<w; i++)
	{
		for (int j=0; j<w; j++)
		{
			if (*screenPix>h) *screenPix=color; 
screenPix++;
		}
	screenPix+=off;
	}
}

void drowBigPixel (int x,int y,int w, int color, int h)
{
	if ((y<0) || (y+w>scrhi) || (x<0) || (x+w>scrwi)) return;
	Uint32* screenPix=(Uint32*)screen->pixels+y*screen->w+x;
	//h+=128;
h<<=24;
int off=screen->w-w;
color=(color & 0x00FFFFFF) | h;
	for (int i=0; i<w; i++)
	{
		for (int j=0; j<w; j++)
		{
			if (*screenPix>h) *screenPix=color; 
screenPix++;
		}
	screenPix+=off;
	}
}
void CatchBigGPixel (int x,int y,int w, int color, int h)
{
/*	x-=vox[curVoxel].massCenter.x;
	y-=vox[curVoxel].massCenter.y;
	h-=vox[curVoxel].massCenter.h;*/
	if ((y<0) || (y+w>scrhi) || (x<0) || (x+w>scrwi)) return;
	
	//h+=128;
if ((mouse.pos.x>=x) && (mouse.pos.x<x+w) && (mouse.pos.y>=y) && (mouse.pos.y<y+w))
{
//Uint32* screenPix=(Uint32*)screen->pixels+mouse.pos.y*screen->w+mouse.pos.x;
//int th=*screenPix>>24;
if (palette.currentH>h) 
{
palette.currentGpixel=GlobGp;
palette.currentH=h;
}
}
/*
	for (int i=0; i<w; i++)
	{
		for (int j=0; j<w; j++)
		{
			int th=*((Uint8 * )screenPix+3);
			if ((th==h) && (palette.currentH>th)) 
			{
				if ((mouse.pos.x>x) && (mouse.pos.x<x+w) && (mouse.pos.y>y) && (mouse.pos.y<y+w))
				{
					palette.currentGpixel=GlobGp;
					palette.currentH=th;
				}

//*screenPix=color;
//*((Uint8 * )screenPix+3)=h;
			}
screenPix++;
		}
	screenPix+=screen->w-w;
	}*/

}
void TryCatchPixelData (int x,int y,int w, int color, int h)
{

	if ((y<0) || (y>scrhi) || (x<0) || (x>scrwi)) return;
	//	if ((y<mouse.pos.y-brushSize/2) || (y>mouse.pos.y+brushSize/2) || (x<0) || (x>scrwi)) return;
	if ((x<mouse.pos.x-palette.brushSize/2) || (x>mouse.pos.x+palette.brushSize/2) || (y<mouse.pos.y-palette.brushSize/2) || (y>mouse.pos.y+palette.brushSize/2)) return;
	int py=y-(mouse.pos.y-palette.palettey); 
	int px=x-(mouse.pos.x-palette.palettex); 
	if ((px<0) || (py<0) || (px>=palette.image->w) || (py>=palette.image->h)) return;

	Uint32* screenPix=(Uint32*)screen->pixels+y*screen->w+x;
	Uint32* sourcepic=(Uint32*)palette.image->pixels+py*palette.image->w+px;
	//h+=128;

			if ((int)(*((Uint8 * )screenPix+3))>=h-1)
			{
if (*sourcepic)
{
*palette.currentPixel=*sourcepic;
*palette.currentPixelTP=py*palette.image->w+px;


// Drow pixel to G8
int difference=255*255*255;
int no=1;
for (int i=1; i<9; i++)
{
	int rediff=(vox[curVoxel].eightPalette[i] & RedMask)-(*sourcepic & RedMask); 
	int BlueDiff=(vox[curVoxel].eightPalette[i] & BlueMask)-(*sourcepic & BlueMask); 
	int Greendiff=(vox[curVoxel].eightPalette[i] & GreenMask)-(*sourcepic & GreenMask); 
	rediff>>=16;
	Greendiff>>=8;
	rediff*=BlueDiff*Greendiff;
	rediff=abs(rediff);
	if (rediff<difference) {difference=rediff; no=i;}

}
*palette.currentG8pixel=no;
//*palette.currentG8pixel=
/*
for (int i=1; i<256; i++)
{
Uint32 red=0,green=0,blue=0;
int colors=0;
for (int j=0; j<8; j++)
if ((i>>j) & 0x1) 
{
	red+=((vox[curVoxel].eightPalette[j+1]) & RedMask);
	green+=((vox[curVoxel].eightPalette[j+1]) & GreenMask);
	blue+=((vox[curVoxel].eightPalette[j+1]) & BlueMask);
*/
}
}

}


void drowDimentions ()
{
	//return;
//for (int ii=0; ii<VoxelsCount; ii++)
	//if (vox[ii].active)
int ii=curVoxel;
		//DrovVexel(vox[i].x, vox[i].y, vox[i].h, 0xFFFFFF, 0, scrwid+scrwid/4, scrhid/2);
	
/*
for (int l=0; l<vox[ii].size; l++)
for (int i=0; i<vox[ii].size; i++)
for (int j=0; j<vox[ii].size; j++)
	if (vox[ii].VoxelMatrix[i][j][l])
	{
		drowBigPixel (i-vox[ii].size/2+scrwid*1.75+vox[ii].x,l-vox[ii].size/2+scrhid/2+vox[ii].h,1, vox[ii].VoxelMatrix[i][j][l],  l);//+vox[ii].y);
		drowBigPixel (l-vox[ii].size/2+scrwid*1.25+vox[ii].h,j-vox[ii].size/2+scrhid*1.5+vox[ii].y,1, vox[ii].VoxelMatrix[i][j][l],  l);//+vox[ii].x);
		drowBigPixel (i-vox[ii].size/2+scrwid*1.75+vox[ii].x,j-vox[ii].size/2+scrhid*1.5+vox[ii].y,1, vox[ii].VoxelMatrix[i][j][l],  l);//+vox[ii].h);
	}*/
/*
	if (ii==curVoxel)
	{
	DrowFrame (-vox[ii].size/2+scrwid*1.75+vox[ii].x, -vox[ii].size/2+scrhid/2+vox[ii].h, vox[ii].size, vox[ii].size, 0xFFFFFF);
	DrowFrame (-vox[ii].size/2+scrwid*1.25+vox[ii].h,-vox[ii].size/2+scrhid*1.5+vox[ii].y, vox[ii].size, vox[ii].size, 0xFFFFFF);
	DrowFrame (-vox[ii].size/2+scrwid*1.75+vox[ii].x,-vox[ii].size/2+scrhid*1.5+vox[ii].y, vox[ii].size, vox[ii].size, 0xFFFFFF);
	}
	*/
}

void rotateOffXOffYh(float globxh, float globyh, float glob, int * offx, int * offy, int x, int y, int * offh)
{
//globxh=globxh+Vox3Deff*(*offx-scrwid);

//globyh=globyh-Vox3Deff*(*offy-scrhid);

double sx = sin(globyh);
double cx = cos(globyh);
double sy = sin(globxh);
double cy = cos(globxh);
double sz = sin(glob);
double cz = cos(glob);

double xy, xh, yx, yh, hx, hy;

hx = cz*x - sz*y;
hy = sz*x + cz*y;	
xy = cx*hy - sx*(*offh);
xh = sx*hy + cx*(*offh);
yh = cy*xh - sy*hx;
yx = sy*xh + cy*hx;

//yx+=offx;
//xy+=offy;
*offx+=yx;
*offy+=xy;
*offh=yh;
}
void rotateVectorI(int *x, int *y, int *h, double angleh, double anglex, double angley)
{
double sx = sin(anglex);
double cx = cos(anglex);
double sy = sin(angley);
double cy = cos(angley);
double sz = sin(angleh);
double cz = cos(angleh);

double xh, hx, hy;

hx = cz**x - sz**y;
hy = sz**x + cz**y;	
xh = sx*hy + cx*(*h);
*y = cx*hy - sx*(*h);
*h = cy*xh - sy*hx;
*x = sy*xh + cy*hx;
}
void rotateVectorD(double *x, double *y, double *h, double angleh, double anglex, double angley)
{
double sx = sin(anglex);
double cx = cos(anglex);
double sy = sin(angley);
double cy = cos(angley);
double sz = sin(angleh);
double cz = cos(angleh);

double xh, hx, hy;

hx = cz**x - sz**y;
hy = sz**x + cz**y;	
xh = sx*hy + cx*(*h);
*y = cx*hy - sx*(*h);
*h = cy*xh - sy*hx;
*x = sy*xh + cy*hx;
}


void QuickAnglesToCosSin(float globxh, float globyh, float glob)
{
sz = 0xFF*sin(glob);
cz = 0xFF*cos(glob);
sx = 0xFF*sin(globyh);
cx = 0xFF*cos(globyh);
sy = 0xFF*sin(globxh);
cy = 0xFF*cos(globxh);
}
bool GetUpsideDownFromQuick(float * glob)
{
int qx=0, qy=0, qh=1;
int hx = cz*qx - sz*qy;
int hy = sz*qx + cz*qy;	
int xy = cx*hy - (sx*qh<<8);
int xh = sx*hy + (cx*qh<<8);
int yh = cy*xh - (sy*hx<<8);
int yx = sy*xh + (cy*hx<<8);

xy>>=(12);
yx>>=(20);
if (yh<0) 
{	
 qx=1, qy=0, qh=0;
 hx = cz*qx - sz*qy;
 hy = sz*qx + cz*qy;	
 xy = cx*hy - (sx*qh<<8);
 xh = sx*hy + (cx*qh<<8);

 yx = sy*xh + (cy*hx<<8);
	*glob=getAngle(yx, xy);
}
return yh<0;
}

void FloatAnglesToCosSin(float globxh, float globyh, float glob, FaToBconversion* NtoN)
{
NtoN->fsz = sin(glob);
NtoN->fcz = cos(glob);
NtoN->fsx = sin(globyh);
NtoN->fcx = cos(globyh);
NtoN->fsy = sin(globxh);
NtoN->fcy = cos(globxh);
}

bool GetUpsideDownFromFloat()
{
float qx=0;
float qy=0;
float qh=1;
float hx = NtoN.fcz*qx - NtoN.fsz*qy;
float hy = NtoN.fsz*qx + NtoN.fcz*qy;	
float xh = NtoN.fsx*hy + (NtoN.fcx*qh);
float yh = NtoN.fcy*xh - (NtoN.fsy*hx);
return yh<0;
}

void SuperFastNtoN()
{
// X==1;

xtoh=-sy*0xFF;
int yx=cy;

xtox=cz*yx;
xtoy=sz*yx;

// Y==1;

int xy=cx;
int xh=sx;

ytoh=cy*xh;
yx=sy*xh;

ytox=((cz*yx)>>8)-(sz*xy);
ytoy=((sz*yx)>>8)+(cz*xy);

// H==1;

xy=- sx;
xh=cx;

htoh=cy*xh;
yx=sy*xh;

htox=((cz*yx)>>8)-(sz*xy);
htoy=((sz*yx)>>8)+(cz*xy);
}
void SuperFastNtoN_float()
{
// X==1;

NtoN.fxtoh=-NtoN.fsy;
float yx=NtoN.fcy;

NtoN.fxtox=NtoN.fcz*yx;
NtoN.fxtoy=NtoN.fsz*yx;

// Y==1;

float xy=NtoN.fcx;
float xh=NtoN.fsx;

NtoN.fytoh=NtoN.fcy*xh;
yx=NtoN.fsy*xh;

NtoN.fytox=(NtoN.fcz*yx)-(NtoN.fsz*xy);
NtoN.fytoy=(NtoN.fsz*yx)+(NtoN.fcz*xy);

// H==1;

xy=-NtoN.fsx;
xh=NtoN.fcx;

NtoN.fhtoh=NtoN.fcy*xh;
yx=NtoN.fsy*xh;

NtoN.fhtox=(NtoN.fcz*yx)-(NtoN.fsz*xy);
NtoN.fhtoy=(NtoN.fsz*yx)+(NtoN.fcz*xy);
}
void SuperFastNtoNhLast()
{
// h==1;

htox=-sy*0xFF;
int yh=cy;

htoh=cx*yh;
htoy=sx*yh;

// Y==1;

int hy=cz;
int hx=sz;

ytox=cy*hx;
yh=sy*hx;

ytoh=((cx*yh)>>8)-(sx*hy);
ytoy=((sx*yh)>>8)+(cx*hy);

// x==1;

hy=- sz;
hx=cz;

xtox=cy*hx;
yh=sy*hx;

xtoh=((cx*yh)>>8)-(sx*hy);
xtoy=((sx*yh)>>8)+(cx*hy);
}

//################################################# 8-bit Voxels
void DrowBufferByLight(Uint32* palpx, int offx, int offy, SDL_Surface* dest)
{

int radius=min(palette.G8BufferRadius+1,E8bitBufferRad);

if ((offx-radius>=dest->w) || (offx+radius<=0) ||
	(offy-radius>=dest->h) || (offy+radius<=0)) return;

Uint32* bufPoint=&palette.Buffer8bit[0];
int size=radius*2;
int ix=offx-radius;
int iy=offy-radius;

//fillDarkMaskGrid(offx-radius, offy-radius, radius*2+128, radius*2);



int x=0;
int y=0;
if (ix<0) x-=(ix);
if (iy<0) y-=(iy);
int destx=radius*2;
int desty=radius*2;
if (ix+size>dest->w)
	destx=(dest->w-ix);
if (iy+size>dest->h) desty=(dest->h-iy);



int scroffset=(iy+y)*dest->w+ix+x;
Uint32* __restrict scrpx=(Uint32*)dest->pixels+scroffset;
Uint32* __restrict mask=darkmask+scroffset;



int xfrom=E8bitBufferRad-radius+x;
int xto=xfrom+destx-x;//128+radius;
int yfrom=E8bitBufferRad-radius+y;
int yto=yfrom+desty-y;//128+radius;
//int jsize=radius*2;	
bufPoint+=(E8bitBufferDiam)*yfrom;

//int shadow=255;
int j=yto-yfrom;
int ylev=0;
int bufskip=E8bitBufferDiam-xto;

if (xto>xfrom)
	if (j>0)
while (j>0)
{
bufPoint+=xfrom;
int i=xto-xfrom;
//for (int i=0; i<w; i++)
//while (shadow<255)
Uint32 color;
Uint32 prevcolor=0;
while (i>0)	
{
		//from
//buffer: height|height|angle|color
	//to
		//buffer: color|angle|height|height
//height=0;
color=*bufPoint>>24;

while ((color<255) && (i))
{
//*mask&=0xFFFF0000;
if (*mask)
{
int AH=min(255,*mask>>14);
int ccl=(*bufPoint>>16) & 0xFF;
prevcolor=*(palpx+color);
MultiplyFloatColor(ccl ,&prevcolor);
register int idst=*scrpx;
register int src=prevcolor;
*scrpx = ((RedMask & ((idst & RedMask) + ((int)(((int)(((src & RedMask ))) - (int)(idst & RedMask)) * AH) >>8))) |
		(GreenMask & ((idst & GreenMask) + ((int)(((((int)(src & GreenMask ))) - (int)(idst & GreenMask)) * AH) >>8))) |
		(BlueMask & ((idst & BlueMask) + ((int)(((((int)(src & BlueMask ))) - (int)(idst & BlueMask)) * AH) >>8)))) | ((*bufPoint<<24));

//*scrpx=(prevcolor & 0x00FFFFFF) | (*bufPoint<<24);
}

*bufPoint=0xFFFFFFFF;
bufPoint++;
scrpx++;
mask++;
i--;
color=*bufPoint>>24;
}


int skips=0;
if (i>0)
while (((*bufPoint & 0xFF000000)==0xFF000000) && (i))
{
	bufPoint++;
	i--;
	skips++;
}
scrpx+=skips;
mask+=skips;

}


j--;
ylev++;
scrpx=(Uint32*)dest->pixels+scroffset+ylev*dest->w;
mask=darkmask+scroffset+ylev*scrwi;
bufPoint=&palette.Buffer8bit[0]+(E8bitBufferDiam)*(yfrom+ylev);
//bufPoint=;
//bufPoint+=bufskip;
}

palette.G8BufferRadius=0;
}
void DrowBufferByHeight(Uint32* palpx, int offx, int offy, SDL_Surface* dest, bool clearAfter)
{
if (!palpx) return;
int radius=min(palette.G8BufferRadius+1,E8bitBufferRad);

int size=radius*2;
int ix=offx-radius;
int iy=offy-radius;

int x=0;
int y=0;
if (ix<0) x-=(ix);
if (iy<0) y-=(iy);
int destx=radius*2;
int desty=radius*2;
if (ix+size>dest->w)
	destx=(dest->w-ix);
if (iy+size>dest->h) desty=(dest->h-iy);

int xfrom=E8bitBufferRad-radius+x;
int xto=xfrom+destx-x;
int yfrom=E8bitBufferRad-radius+y;
int yto=yfrom+desty-y;



int scroffset=(iy+y)*dest->w+ix+x;
Uint32* __restrict scrpx=(Uint32*)dest->pixels+scroffset;
Uint32* __restrict mask=darkmask+scroffset;

Uint32* __restrict bufPoint=&palette.Buffer8bit[0]+E8bitBufferDiam*(E8bitBufferRad-radius)+(E8bitBufferRad-radius);//yfrom;
//int shadow=255;

int ylev=0;
int bufskip=E8bitBufferDiam-xto;

// pre Y cleaning
int bufcleanskip=E8bitBufferDiam-radius*2;
/*if (y>0) while (y)
{
int i=radius*2;
while (i)
{
*bufPoint=0x0000FFFF;
bufPoint++;
i--;
}
y--;
bufPoint+=bufcleanskip;
}*/

int cleanx=E8bitBufferRad-radius;
bufPoint=&palette.Buffer8bit[0]+E8bitBufferDiam*(yfrom);
int j=yto-yfrom;
if (xto>xfrom)
	if (j>0)
while (j>0)
{
bufPoint+=cleanx;//xfrom;
int i=xfrom-cleanx;

//while (i) {*bufPoint=0x0000FFFF; bufPoint++; i--;}
bufPoint+=i;

i=xto-xfrom;
Uint32 color;
Uint32 prevcolor=0;
while (i>0)	
{

color=*bufPoint>>24;
if ((color<255 && (*bufPoint<<24<*scrpx)))
{
prevcolor=*(palpx+color);
MultiplyFloatColor((*bufPoint>>16) & 0xFF ,&prevcolor);
htpBlit(scrpx, prevcolor);
*scrpx=(*scrpx & 0x00FFFFFF) + ((((*scrpx>>24) + (*bufPoint & 0x000000FF))/2) <<24);
//*bufPoint=0x0000FFFF;
bufPoint++;
scrpx++;
mask++;
i--;


color=*bufPoint>>24;
if (i>0)
while ((color<255) && (i) && (*bufPoint<<24<*scrpx))
{
int ccl=(*bufPoint>>16) & 0xFF;
//prevcolor=lightArray[ccl];
//AddColor(*(palpx+color), &prevcolor);
//prevcolor+=*(palpx+color);
//MultiplyColor1(*(palpx+color), &prevcolor);
prevcolor=*(palpx+color);
MultiplyFloatColor(ccl ,&prevcolor);

*scrpx=(prevcolor & 0x00FFFFFF) | (*bufPoint<<24);

*mask=0;//&=0xFFFF0000;
//*bufPoint=0x0000FFFF;
bufPoint++;
scrpx++;
mask++;
i--;
color=*bufPoint>>24;
}

if  (i>0)
htpBlit(scrpx, prevcolor);
prevcolor=0;

}




if (i>0)
while ( (i) && (*bufPoint<<24>=*scrpx))
{
//	*bufPoint=0x0000FFFF;
	bufPoint++;	
	i--;
	scrpx++;
	mask++;
}

int skips=0;
if (i>0)
while ( (i) && ((*bufPoint & 0xFF000000)==0xFF000000)   )
{
//	*bufPoint=0x0000FFFF;
	bufPoint++;	
	i--;
	skips++;
}
scrpx+=skips;
mask+=skips;

}

 i=E8bitBufferRad+radius-xto;

//while (i) {*bufPoint=0x0000FFFF; bufPoint++; i--;}



j--;
ylev++;
scrpx=(Uint32*)dest->pixels+scroffset+ylev*dest->w;
mask=darkmask+scroffset+ylev*scrwi;
bufPoint=&palette.Buffer8bit[0]+(E8bitBufferDiam)*(yfrom+ylev);
//bufPoint=;
//bufPoint+=bufskip;
}

y=E8bitBufferRad+radius-yto;
// y afterclean

/*
if (y>0)
{
bufPoint=&palette.Buffer8bit[0]+E8bitBufferDiam*(yto)+(E8bitBufferRad-radius);//yfrom;

while (y)
{
int i=radius*2;
while (i)
{
*bufPoint=0x0000FFFF;
bufPoint++;
i--;
}
y--;
bufPoint+=bufcleanskip;
}
}
*/

if (!clearAfter) return;

int i=radius*2;
while (i)
{	
memset(&palette.Buffer8bit[0]+E8bitBufferDiam*(E8bitBufferRad+radius-i)+(E8bitBufferRad-radius)	, 0xFF, radius*8);
i--;
}
palette.G8BufferRadius=0;
i--;
}
void DrowBufferByHeightZoom(Uint32* palpx, int offx, int offy, SDL_Surface* dest)
{
int radius=min(palette.G8BufferRadius+1,E8bitBufferRad-1);

int size=radius*2;
int ix=offx-radius;
int iy=offy-radius;

int x=0;
int y=0;
if (ix<0) x-=(ix);
if (iy<0) y-=(iy);
int destx=radius*2;
int desty=radius*2;
if (ix+size>dest->w)
	destx=(dest->w-ix);
if (iy+size>dest->h) desty=(dest->h-iy);

int xfrom=E8bitBufferRad-radius+x;
int xto=xfrom+destx-x;
int yfrom=E8bitBufferRad-radius+y;
int yto=yfrom+desty-y;
int scroffset=(iy+y)*dest->w+ix+x;
Uint32* __restrict scrpx=(Uint32*)dest->pixels+scroffset;
Uint32* __restrict mask=darkmask+scroffset;
Uint16* __restrict hpoint=zoomHscreen+scroffset; 

Uint32*  __restrict bufPoint=&palette.Buffer8bit[0]+E8bitBufferDiam*(E8bitBufferRad-radius)+(E8bitBufferRad-radius);//yfrom;
//int shadow=255;

int ylev=0;
int bufskip=E8bitBufferDiam-xto;

bufPoint=&palette.Buffer8bit[0]+E8bitBufferDiam*(yfrom);
int j=yto-yfrom;
if (xto>xfrom)
	if (j>0)
while (j>0)
{
bufPoint+=xfrom;//xfrom;
int i;
i=xto-xfrom;
Uint32 color;
Uint32 prevcolor=0;
while (i>0)	
{
color=*bufPoint>>24;
if (color && ((*bufPoint & 0xFFFF)>*hpoint))
{
prevcolor=*(palpx+color);
MultiplyFloatColor((*bufPoint>>16) & 0xFF ,&prevcolor);
htpBlit(scrpx, prevcolor);
bufPoint++;
scrpx++;
mask++;
hpoint++;
i--;
color=*bufPoint>>24;

if (i>0)
while ((color) && (i) && ((*bufPoint & 0xFFFF)>*hpoint))
{
int ccl=(*bufPoint>>16) & 0xFF;
prevcolor=*(palpx+color);
MultiplyFloatColor(ccl ,&prevcolor);
*scrpx=prevcolor;
*hpoint=*bufPoint;
*mask&=0xFFFF0000;
bufPoint++;
scrpx++;
mask++;
hpoint++;
i--;
color=*bufPoint>>24;
}

if  (i>0)
htpBlit(scrpx, prevcolor);
prevcolor=0;

}


int skips=0;

if (i>0)
while ( (i) && ((*bufPoint & 0xFFFF)<=*hpoint))
{
	bufPoint++;	
	i--;
	hpoint++;
	skips++;
}
scrpx+=skips;
mask+=skips;

skips=0;
if (i>0)
while ( (i) && (!(*bufPoint & 0xFF000000))   )
{
	bufPoint++;	
	i--;
	skips++;
}
scrpx+=skips;
hpoint+=skips;
mask+=skips;

}

j--;
ylev++;
scrpx=(Uint32*)dest->pixels+scroffset+ylev*dest->w;
hpoint=zoomHscreen+scroffset+ylev*dest->w;
mask=darkmask+scroffset+ylev*scrwi;
bufPoint=&palette.Buffer8bit[0]+(E8bitBufferDiam)*(yfrom+ylev);
}
memset((char*)palette.Buffer8bit,0,E8bitBufferDiam*E8bitBufferDiam*4);
//memset((char*)&palette.Buffer8bit[E8bitBufferRad-radius],0,radius*E8bitBufferDiam*2*4);
palette.G8BufferRadius=0;


}
void DrowBuffer(Uint32* palpx, int offx, int offy, SDL_Surface* dest)
{

int radius=min(palette.G8BufferRadius+1,E8bitBufferRad);

if ((offx-radius>=dest->w) || (offx+radius<=0) ||
	(offy-radius>=dest->h) || (offy+radius<=0))
{
	palette.G8BufferRadius=0;
	return;
}


int size=radius*2;
int ix=offx-radius;
int iy=offy-radius;

int x=0;
int y=0;
if (ix<0) x-=(ix);
if (iy<0) y-=(iy);
int destx=radius*2;
int desty=radius*2;
if (ix+size>dest->w)
	destx=(dest->w-ix);
if (iy+size>dest->h) desty=(dest->h-iy);

int xfrom=E8bitBufferRad-radius+x;
int xto=xfrom+destx-x;
int yfrom=E8bitBufferRad-radius+y;
int yto=yfrom+desty-y;



int scroffset=(iy+y)*dest->w+ix+x;
Uint32* __restrict scrpx=(Uint32*)dest->pixels+scroffset;
Uint32* __restrict mask=darkmask+scroffset;

Uint32* __restrict bufPoint=&palette.Buffer8bit[0]+E8bitBufferDiam*(E8bitBufferRad-radius)+(E8bitBufferRad-radius);//yfrom;
//int shadow=255;

int ylev=0;
int bufskip=E8bitBufferDiam-xto;

// pre Y cleaning
int bufcleanskip=E8bitBufferDiam-radius*2;
if (y>0) while (y)
{
int i=radius*2;
while (i)
{
*bufPoint=0x0000FFFF;
bufPoint++;
i--;
}
y--;
bufPoint+=bufcleanskip;
}

int cleanx=E8bitBufferRad-radius;
bufPoint=&palette.Buffer8bit[0]+E8bitBufferDiam*(yfrom);
int j=yto-yfrom;
if (xto>xfrom)
	if (j>0)
while (j>0)
{
bufPoint+=cleanx;//xfrom;
int i=xfrom-cleanx;

while (i) {*bufPoint=0x0000FFFF; bufPoint++; i--;}

i=xto-xfrom;
Uint32 color;
Uint32 prevcolor=0;
while (i>0)	
{
color=*bufPoint>>24;
if (color)
{
prevcolor=*(palpx+color);
MultiplyFloatColor((*bufPoint>>16) & 0xFF ,&prevcolor);
htpBlit(scrpx, prevcolor);
*scrpx=(*scrpx & 0x00FFFFFF) + ((((*scrpx>>24) + (*bufPoint & 0x000000FF))/2) <<24);
*bufPoint=0x0000FFFF;
bufPoint++;
scrpx++;
mask++;
i--;


color=*bufPoint>>24;
if (i>0)
while ((color) && (i) )
{
int ccl=(*bufPoint>>16) & 0xFF;
//prevcolor=lightArray[ccl];
//AddColor(*(palpx+color), &prevcolor);
//prevcolor+=*(palpx+color);
//MultiplyColor1(*(palpx+color), &prevcolor);
prevcolor=*(palpx+color);
MultiplyFloatColor(ccl ,&prevcolor);

*scrpx=(prevcolor & 0x00FFFFFF) | (*bufPoint<<24);
*bufPoint=0x0000FFFF;
*mask&=0xFFFF0000;
bufPoint++;
scrpx++;
mask++;
i--;
color=*bufPoint>>24;
}
}

if ((prevcolor) && (i>0))
{
htpBlit(scrpx, prevcolor);
prevcolor=0;
}

int skips=0;
if (i>0)
while ((!(*bufPoint & 0xFF000000)) && (i))  //|| (*bufPoint<<24>=*scrpx)
{
	bufPoint++;
	i--;
	skips++;
	scrpx++;
}
//scrpx+=skips;
mask+=skips;

}

 i=E8bitBufferRad+radius-xto;

while (i) {*bufPoint=0x0000FFFF; bufPoint++; i--;}



j--;
ylev++;
scrpx=(Uint32*)dest->pixels+scroffset+ylev*dest->w;
mask=darkmask+scroffset+ylev*scrwi;
bufPoint=&palette.Buffer8bit[0]+(E8bitBufferDiam)*(yfrom+ylev);
//bufPoint=;
//bufPoint+=bufskip;
}

y=E8bitBufferRad+radius-yto;
// y afterclean


if (y>0)
{
bufPoint=&palette.Buffer8bit[0]+E8bitBufferDiam*(yto)+(E8bitBufferRad-radius);//yfrom;
while (y)
{
int i=radius*2;
while (i)
{
*bufPoint=0x0000FFFF;
bufPoint++;
i--;
}
y--;
bufPoint+=bufcleanskip;
}
}


palette.G8BufferRadius=0;
}
void ConvertBufLightToImag (SDL_Surface* dest, DirectionalLightetImage* dlm, int dx, int dy)
{

dlm->h=dest->h;
dlm->w=dest->w;
dlm->DnA=new Uint8[dlm->h*dlm->w*2]; 
Uint8* dlmpix=&dlm->DnA[0];
Uint32* srf=(Uint32*) dest->pixels;
int wdif=E8bitBufferDiam-dest->w;
int hdif=E8bitBufferDiam-dest->h;

Uint32* bufPoint=&palette.Buffer8bit[0];
bufPoint+=(dy+hdif/2)*E8bitBufferDiam+wdif/2+dx;

int i=dest->w;
int j=dest->h;
while (j)
{
i=dest->w;
while (i)
{
//alpha
*dlmpix=*srf>>24;
srf++;
dlmpix++;

//direction
*dlmpix=(*bufPoint>>16) & 0xFF;
bufPoint++;
dlmpix++;



	i--;
}
bufPoint+=wdif;
j--;
}
}
void ConvertBufLightDirectionToAlpha (SDL_Surface* image)
{

//dlm->h=dest->h;
//dlm->w=dest->w;
//dlm->DnA=new Uint8[dlm->h*dlm->w*2]; 
//Uint8* dlmpix=&dlm->DnA[0];
Uint32* srf=(Uint32*) image->pixels;
int wdif=E8bitBufferDiam-image->w;
int hdif=E8bitBufferDiam-image->h;

Uint32* bufPoint=&palette.Buffer8bit[0];
bufPoint+=hdif/2*E8bitBufferDiam+wdif/2;

int i=image->w;
int j=image->h;
while (j)
{
i=image->w;
while (i)
{
	*srf=(*srf & 0x00FFFFFF) | ((*bufPoint<<8) & (0xFF000000));
//alpha
//*dlmpix=*srf>>24;
srf++;
//dlmpix++;

//direction
//*dlmpix=(*bufPoint>>16) & 0xFF;
bufPoint++;
//dlmpix++;



	i--;
}
bufPoint+=wdif;
j--;
}
}
void CopyBufLightDirection (Uint16* buffer, Uint8* copy, int w, int h)
{
Uint8* __restrict srf=copy;
int wdif=E8bitBufferDiam-w;
int hdif=E8bitBufferDiam-h;

Uint16* __restrict bufPoint=buffer;//&palette.Buffer8bit[0];
bufPoint+=hdif/2*E8bitBufferDiam+wdif/2;
//int buffup=SMsize*10*increasing;
int i=w;
int j=h;
Uint8 prev=0;
Uint8 p64=0;
while (j)
{
i=w;
while (i)
{
	//prev=(*bufPoint>>16) & 0xFF;
	*srf=(*bufPoint>>8);//prev;// & 0x3F | ((p64+prev/64)/2)*64;
	//p64=prev/64;
	*bufPoint+=1*((*bufPoint & 0xFF)<250);
//alpha
//*dlmpix=*srf>>24;
srf++;
//dlmpix++;

//direction
//*dlmpix=(*bufPoint>>16) & 0xFF;
bufPoint++;
//dlmpix++;



	i--;
}
bufPoint+=wdif;
j--;
}
}
void LightMaskFromBuffer(int offx, int offy, DirectionalLightetImage* dlm, worldcoordinates* pos, float ffade)
{
ffade=max(0,min(ffade,1));
int fade=255-(255*ffade);
int	h=pos->h>>15;
SDL_Rect rect;
ShapeRectToScreen(dlm->w, dlm->h , &rect,  screen, offx, offy);
if ((rect.x>=rect.w) || (rect.y>=rect.h)) return;


//if (pos) 
//Uint32 light=LightsToSingleValue(pos, UpGoingSmoke,128 );
	//LightsTo180array(pos, 1-0.000135*accurate(pos->h), 0, 0,UpGoingSmoke,defaultMaxLight);
 LightsToSingleValueF(pos, 128);
OnelightR=min(OnelightR,512)/2;
OnelightG=min(OnelightG,512)/2;
OnelightB=min(OnelightB,512)/2;


int wdif=scrwi-rect.w+rect.x;
int wdifB=(rect.x+dlm->w-rect.w)*2;
int i=rect.w-rect.x;
int j=rect.h-rect.y;
Uint32* __restrict scrpix=(Uint32*) screen->pixels + scrwi*(offy+rect.y)+ offx+rect.x;
//Uint32* dark=(Uint32*)darkmask + scrwi*(offy+rect.y)+ offx+rect.x;
Uint8* __restrict dlmpix=&dlm->DnA[(rect.y*dlm->w+rect.x)*2];
//int isrc=light;
while (j)
{
i=rect.w-rect.x;
while (i)
{
	int newh=*dlmpix/2+h;
int dif=newh-(255-(*scrpix>>24));
int AH=(*dlmpix-fade)/2;
dlmpix++;

if ((dif>0) && (AH>0))
{

//MultiplyBy(*dark>>16, (Uint32*)(&isrc));
if (dif<=32) AH=AH*dif/32;

register int idst=*scrpix;
*scrpix= ((RedMask & ((idst & RedMask) + ((int)(((int)((int)OnelightR<<15) - (int)(idst & RedMask)) * AH) >>8))) 
	| (GreenMask & ((idst & GreenMask) + ((int)(((int)((int)OnelightG<<7) - (int)(idst & GreenMask)) * AH) >>8))) 
	| (BlueMask & ((idst & BlueMask) + ((int)(((int)((int)OnelightB>>1) - (int)(idst & BlueMask)) * AH) >>8))))
	| (idst & AlphaMask);
}	


dlmpix++;
scrpix++;
//dark++;
i--;
}
scrpix+=wdif;
//dark+=wdif;
dlmpix+=wdifB;
j--;
}
}
void BufferToGround(Uint32* palpx, int offx, int offy, SDL_Surface* dest)

{

int radius=palette.G8BufferRadius+1;

if ((offx-radius>=dest->w) || (offx+radius<=0) ||
	(offy-radius>=dest->h) || (offy+radius<=0)) return;

if (radius>=E8bitBufferRad) 
	return;

Uint32* __restrict bufPoint=&palette.Buffer8bit[0];
int size=radius*2;
int ix=offx-radius;
int iy=offy-radius;

//fillDarkMaskGrid(offx-radius, offy-radius, radius*2+128, radius*2);



int x=0;
int y=0;
if (ix<0) x-=(ix);
if (iy<0) y-=(iy);
int destx=radius*2;
int desty=radius*2;
if (ix+size>dest->w)
	destx=(dest->w-ix);
if (iy+size>dest->h) desty=(dest->h-iy);



int scroffset=(iy+y)*dest->w+ix+x;
Uint32* __restrict scrpx=(Uint32*)dest->pixels+scroffset;



int xfrom=E8bitBufferRad-radius+x;
int xto=xfrom+destx-x;//128+radius;
int yfrom=E8bitBufferRad-radius+y;
int yto=yfrom+desty-y;//128+radius;
//int jsize=radius*2;	
bufPoint+=(E8bitBufferDiam)*yfrom;

//int shadow=255;
int j=yto-yfrom;
int ylev=0;
int bufskip=E8bitBufferDiam-xto;

if (xto>xfrom)
	if (j>0)
while (j>0)
{
bufPoint+=xfrom;
int i=xto-xfrom;
//for (int i=0; i<w; i++)
//while (shadow<255)
Uint32 color;
Uint32 prevcolor=0;
//Uint32 height=0;//=min(*bufPoint & 0x0000FFFF, 255);
while (i>0)	
{
		//from
//buffer: height|height|angle|color
	//to
		//buffer: color|angle|height|height
//height=0;
color=*bufPoint>>24;
//*scrpx=0;
if (color<255)
{
if ((*bufPoint & 0x0000FFFF)<(*scrpx>>24))
{
//prevcolor=lightArray[(*bufPoint>>16) & 0xFF];
//AddColor(*(palpx+color), &prevcolor);
//MultiplyColor1(*(palpx+color), &prevcolor);

prevcolor=*(palpx+color);
MultiplyFloatColor((*bufPoint>>16) & 0xFF ,&prevcolor);

htpBlit(scrpx, prevcolor);
//*bufPoint=0x0000FFFF;
bufPoint++;
scrpx++;
i--;
}

color=*bufPoint>>24;
while ((color<255) && (i>0) && ((*bufPoint & 0x0000FFFF)<(*scrpx>>24)))
{

//prevcolor=lightArray[(*bufPoint>>16) & 0xFF];
//MultiplyColor1(*(palpx+color), &prevcolor);
	prevcolor=*(palpx+color);
MultiplyFloatColor((*bufPoint>>16) & 0xFF ,&prevcolor);
//AddColor(*(palpx+color), &prevcolor);
*scrpx=(prevcolor & 0x00FFFFFF) | (*scrpx & 0xFF000000 ); //min((*scrpx & 0xFF000000 ), (*bufPoint<<24) ); //| 0xFF000000;//(*bufPoint<<24);

//*bufPoint=0x0000FFFF;
bufPoint++;
scrpx++;
i--;

color=*bufPoint>>24;

}
}

if ((prevcolor) && (i>0))
{
htpBlit(scrpx, prevcolor);
prevcolor=0;
}

int skips=0;
while (((*bufPoint & 0xFF000000)==0xFF000000) && (i>0))
{
	bufPoint++;
	i--;
	skips++;
}
scrpx+=skips;

//(*bufPoint & 0x0000FFFF<*scrpx>>24)

while (((*bufPoint & 0x0000FFFF)>=(*scrpx>>24)) && (i>0))
{
	bufPoint++;
	scrpx++;
	i--;
	//skips++;
}
//scrpx+=skips;

}


j--;
ylev++;
scrpx=(Uint32*)dest->pixels+scroffset+ylev*dest->w;
bufPoint=&palette.Buffer8bit[0]+(E8bitBufferDiam)*(yfrom+ylev);
//bufPoint=;
//bufPoint+=bufskip;
}

}

void ToUpperDecayBufferHeight(int* rad,Uint32* buffer, Uint32* palpx, int offx, int offy, SDL_Surface* mainmp ,SDL_Surface* dest, Surface* decay, bool clearAfter)
{
if ((!palpx) || (decay->h!=1024)) return;
int radius=min(*rad+1,E8bitBufferRad);

int size=radius*2;
int ix=offx-radius;
int iy=offy-radius;

int x=0;
int y=0;
if (ix<0) x-=(ix);
if (iy<0) y-=(iy);
int destx=radius*2;
int desty=radius*2;
if (ix+size>dest->w)
	destx=(dest->w-ix);
if (iy+size>dest->h) desty=(dest->h-iy);

int xfrom=E8bitBufferRad-radius+x;
int xto=xfrom+destx-x;
int yfrom=E8bitBufferRad-radius+y;
int yto=yfrom+desty-y;



int scroffset=(iy+y)*dest->w+ix+x;
Uint32* __restrict scrpx=(Uint32*)dest->pixels+scroffset;
Uint32* __restrict mainscrpx=(Uint32*)mainmp->pixels+scroffset;
Uint32* __restrict dpx=(Uint32*)decay->pixels+scroffset;
Uint32* __restrict bufPoint=buffer+E8bitBufferDiam*(E8bitBufferRad-radius)+(E8bitBufferRad-radius);//yfrom;
int ylev=0;
int bufskip=E8bitBufferDiam-xto;

// pre Y cleaning
int bufcleanskip=E8bitBufferDiam-radius*2;

int cleanx=E8bitBufferRad-radius;
bufPoint=buffer+E8bitBufferDiam*(yfrom);
int j=yto-yfrom;
if (xto>xfrom)
	if (j>0)
while (j>0)
{
bufPoint+=cleanx;//xfrom;
int i=xfrom-cleanx;
bufPoint+=i;

i=xto-xfrom;
Uint32 color;
Uint32 prevcolor=0;
Uint32 prevcolor1=0;
Uint32 tmpgrey;
int diff;
while (i>0)	
{

color=*bufPoint>>24;
if (color<255)
{
	/*
diff=(int)(*mainscrpx>>24) - (int)(*bufPoint & 0x0000FFFF);
if  (diff>32){
	
prevcolor=*(palpx+color);
MultiplyColor1(*dpx, &prevcolor);
htpBlit(mainscrpx, prevcolor);

tmpgrey=0xFF777777;
ThreadMultiplyFloatColor((*(bufPoint-1)>>16) & 0xFF ,&tmpgrey);
htpBlitToUpper(scrpx, tmpgrey);
*scrpx= (*scrpx & 0x00FFFFFF) | 0x77000000;

bufPoint++;
scrpx++;
mainscrpx++;
dpx++;
i--;
}*/





//color=*bufPoint>>24;
diff=(int)(*mainscrpx>>24) - (int)(*bufPoint & 0x0000FFFF);
if (i>0)
while ((i) && (diff>0))
{
color=*bufPoint>>24;	

prevcolor1=prevcolor=*(palpx+color) | (*dpx & 0xFF000000);
MultiplyColor1(*dpx, &prevcolor);

if (diff<128){
AvgColor(*dpx, &prevcolor1);
RegularBlit(&prevcolor, prevcolor1, (128-diff)*2);
}

int ccl=(*bufPoint>>16) & 0xFF;
tmpgrey=0xFF777777;
ThreadMultiplyFloatColor(ccl ,&tmpgrey);

diff=diff*
SoftUpperBlit (diff, scrpx, &tmpgrey, mainscrpx, &prevcolor, *bufPoint & 0x0000FFFF )/255;

diff<<=24;
if (diff<*mainscrpx)
*mainscrpx -= (diff);
else
	*mainscrpx&=0x00FFFFFF;

bufPoint++;
scrpx++;
mainscrpx++;
dpx++;
i--;

diff=(int)(*mainscrpx>>24) - (int)(*bufPoint & 0x0000FFFF);
}

if  ((i>0) && (diff>32)){
	
htpBlit(mainscrpx, prevcolor);
htpBlitToUpper(scrpx, tmpgrey);
*scrpx= (*scrpx & 0x00FFFFFF) | 0x77000000;


}

}

int skips;

skips=0;
if (i>0)
while ( (i) && ((*bufPoint & 0xFF000000)==0xFF000000)   )
{
	skips++;
	bufPoint++;	
	i--;
}
scrpx+=skips;
mainscrpx+=skips;
dpx+=skips;

 skips=0;
while ((i>0) && ((*bufPoint & 0x0000FFFF)>=(*mainscrpx>>24)))
{
	bufPoint++;
	mainscrpx++;
	skips++;
	i--;
}
scrpx+=skips;
dpx+=skips;



}

 i=E8bitBufferRad+radius-xto;


j--;
ylev++;
int stoff=scroffset+ylev*dest->w;
dpx=(Uint32*)decay->pixels+stoff;
scrpx=(Uint32*)dest->pixels+stoff;
mainscrpx=(Uint32*)mainmp->pixels+stoff;
bufPoint=buffer+(E8bitBufferDiam)*(yfrom+ylev);

}

y=E8bitBufferRad+radius-yto;

if (!clearAfter) return;

int i=radius*2;
while (i)
{	
	memset(buffer+E8bitBufferDiam*(E8bitBufferRad+radius-i)+(E8bitBufferRad-radius)	, 0xFF, radius*8);
i--;
}
*rad=0;
//palette.ThreadG8BufferRadius=0;
i--;
}

void CleanBuffer(){

int radius=palette.G8BufferRadius+1;



radius=min(E8bitBufferRad-1,radius); 




	int i=radius*2;
	while (i){	
		memset(&palette.Buffer8bit[0]+E8bitBufferDiam*(E8bitBufferRad+radius-i)+(E8bitBufferRad-radius)	, 0xFF, radius*8);
		i--;
	}
	palette.G8BufferRadius=0;

}

void Empty8BitBuffer()
{
	int i=65536;
	Uint32* __restrict b8p=palette.Buffer8bit;
	Uint16* __restrict b8sp=palette.SmokeBuffer8bit;
while (i)
{
	//palette.Buffer8bit[i]=0x0000FFFF;
*b8p=0x0000FFFF;
*b8sp=0x00FF;
i--;
b8p++;
b8sp++;
}

}



void New8BitNtoN( double resize, FaToBconversion* NtoN)
{
double qx=1;
double qy=0;
double qh=0;
double hx = NtoN->fcz;
double hy = NtoN->fsz;
NtoN->fxtoy = NtoN->fcx*hy;
NtoN->fxtoy*=resize;
double xh = NtoN->fsx*hy;
NtoN->fxtoh = NtoN->fcy*xh - (NtoN->fsy*hx);
NtoN->fxtoh*=resize;
NtoN->fxtox = NtoN->fsy*xh + (NtoN->fcy*hx);
NtoN->fxtox=(NtoN->fxtox*resize);

 qx=0;
 qy=1;
 hx = - NtoN->fsz;
 hy =  NtoN->fcz;	
NtoN->fytoy = NtoN->fcx*hy; 
NtoN->fytoy*=resize;
 xh = NtoN->fsx*hy; 
NtoN->fytoh = NtoN->fcy*xh - (NtoN->fsy*hx);
NtoN->fytoh*=resize;
NtoN->fytox = NtoN->fsy*xh + (NtoN->fcy*hx);
NtoN->fytox*=resize;

 qy=0;
 qh=1;
 hx = 0;
 hy = 0;
NtoN->fhtoy = - (NtoN->fsx); 
NtoN->fhtoy*=resize;
 xh =  (NtoN->fcx); 
NtoN->fhtoh = NtoN->fcy*xh; 
NtoN->fhtoh*=resize;
NtoN->fhtox = NtoN->fsy*xh; 
NtoN->fhtox*=resize;
}

// Megavoxels (Latest)
void FloatAnglesToCosSinMega(float glob)
{
NtoN.fsz = sin(glob);
NtoN.fcz = cos(glob);
NtoN.fsy =  NtoN.fsx = sin(0.0);
NtoN.fcy = NtoN.fcx = cos(0.0);
//fsy = sin(0.0);
//fcy = cos(0.0);
}
void New8BitNtoNMega( )
{
// Now lets get coeficients

double qx=1;
double qy=0;
double qh=0;
double hx = NtoN.fcz*qx - NtoN.fsz*qy;
double hy = NtoN.fsz*qx + NtoN.fcz*qy;	
NtoN.fxtoy = NtoN.fcx*hy - (NtoN.fsx*qh);
double xh = NtoN.fsx*hy + (NtoN.fcx*qh);
NtoN.fxtoh = NtoN.fcy*xh - (NtoN.fsy*hx);
NtoN.fxtox = NtoN.fsy*xh + (NtoN.fcy*hx);

 qx=0;
 qy=1;
 qh=0;
 hx = NtoN.fcz*qx - NtoN.fsz*qy;
 hy = NtoN.fsz*qx + NtoN.fcz*qy;	
NtoN.fytoy = NtoN.fcx*hy - (NtoN.fsx*qh);
xh = NtoN.fsx*hy + (NtoN.fcx*qh);
NtoN.fytoh = NtoN.fcy*xh - (NtoN.fsy*hx);
NtoN.fytox = NtoN.fsy*xh + (NtoN.fcy*hx);



 qx=0;
 qy=0;
 qh=1;
 hx = NtoN.fcz*qx - NtoN.fsz*qy;
 hy = NtoN.fsz*qx + NtoN.fcz*qy;	
NtoN.fhtoy = NtoN.fcx*hy - (NtoN.fsx*qh);
xh = NtoN.fsx*hy + (NtoN.fcx*qh);
NtoN.fhtoh = NtoN.fcy*xh - (NtoN.fsy*hx);
NtoN.fhtox = NtoN.fsy*xh + (NtoN.fcy*hx);


// coeficients end
}

void MegaVox8bitDrow(Uint32* Buffer8bit, G8_Particle * G, int bufoffx, int bufoffy, int offh, float glob, double offx, double offy, int angle, int dx, int dy)//, Uint32* __restrict palpx)//, void (*onImpactFunk) (int ,int))
{	
offx+=bufoffx;
offy+=bufoffy;
if ((offx+4<0) || (offx-4>scrwi) || (offy+4<0) || (offy-4>scrhi) || (!G->radius)) return; 

while (angle<0) 
	angle+=32;

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
int tempradius=max(palette.G8BufferRadius,max(abs(bufoffx+ expx*(8+offh) + dx)+34,abs(bufoffy+expy*(8+offh) + dy)+34));
//  tempradius=max(palette.G8BufferRadius,max(abs(bufoffx+ expx*(offh) + dx)+24,abs(bufoffy+expy*(offh) + dy)+24));
if (tempradius>=E8bitBufferDiam/2) return;

palette.G8BufferRadius=tempradius;
FloatAnglesToCosSinMega( glob);
New8BitNtoNMega();

Uint32* __restrict gp=G->pixels;
Uint32 * __restrict bufPoint;//=&Buffer8bit[E8bitBufferSize/2+E8bitBufferDiam/2 + dy*E8bitBufferDiam + dx];
Uint32* __restrict BufPic;
int BuffCenter=E8bitBufferSize/2+E8bitBufferDiam/2+bufoffx + (dy+bufoffy)*E8bitBufferDiam + dx;
int TrueX, TrueY,TrueH;
int countme=G->count;
int qh=0;
Uint32 plane=0;
Uint32 gpp;
Uint32 mdgp;
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

TrueH=213-offh-qh;


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

	



Uint8 ang=*gp>>16;
if (ang!=192) ang=((ang/64*64) + ((ang + angle) % 64));

gpp=TrueH | ((Uint32)ang<<16);
mdgp=*gp & 0xFF000000;

int diff=(int)(*BufPic & 0xFFFF)-TrueH;
if (diff>-3)
*BufPic= (mdgp) | (diff>0 ? gpp : (*BufPic));


BufPic+=smoothx;
diff=(int)(*BufPic & 0xFFFF)-TrueH;
if (diff>-3)
*BufPic= (mdgp) | (diff>0 ? gpp : (*BufPic));


BufPic+=smoothy;
diff=(int)(*BufPic & 0xFFFF)-TrueH; 
if (diff>-3)
	*BufPic= (mdgp) | (diff>0 ? gpp : (*BufPic));   



gp++;
plane--;
}

}
while (countme);
}


//super fast no good
void SuperFast8bitVex(G8_Particle * G, int offx, int offy, int offh, SDL_Surface * screen, float resize, float globxh, float globyh, float glob, Uint32* __restrict palpx)
{

resize=min(resize,1);
int tempradius=G->radius*resize;
if ((offx-G->radius>=screen->w) || (offx+G->radius<=0) || (offy-G->radius>=screen->h) || (offy+G->radius<=0)) return;
if ((offx+G->radius>screen->w) || (offx-G->radius<0) || (offy+G->radius>screen->h) || (offy-G->radius<0))
	{
		//MergeGParticleSafe( G, offx, offy, globxh, globyh, gx, gy);
		return;
	}
//
float part=resize;
float locxh=globxh+0.001*(offx-scrwid);

FloatAnglesToCosSin(globxh, globyh, glob, &NtoN);
SuperFastNtoN_float();

float offhmin128=128-offh;


//offh=128-offh;
//Gpixel * Gp=G->gpixels;
Uint32* __restrict Gp=G->pixels;
Uint32* __restrict srcPix=(Uint32*)screen->pixels+screen->w*offy+offx;
Uint32 * __restrict bufPoint=&palette.Buffer8bit[32768+128];
Uint32* __restrict BufPic;


int lscrw=screen->w;

int mycount=G->count-1;
if (mycount>0)
do
{
float qh=(char)((*Gp>>16) & 0xFF);
qh*=part;
float hforh=(qh*NtoN.fhtoh);
float dyh=(qh*NtoN.fhtoy);
float dxh=(qh*NtoN.fhtox);
//int hforxy=dyh*lscrw+dxh;

	do
	{//yh*4
	float qy=(char)((*Gp>>8) & 0xFF);
	qy*=part;
	float yforh=offhmin128-hforh-(qy*NtoN.fytoh);
	float dyy=(qy*NtoN.fytoy)+dyh;
	float dxy=(qy*NtoN.fytox)+dxh;
	//Uint32* yforxy=dyy*lscrw+dxy+srcPix;
	
		do
		{
	

float qx=(char)(*Gp & 0xFF);
qx*=part;
Uint32 h=yforh-(qx*NtoN.fxtoh);
float dyx=(qx*NtoN.fxtoy)+dyy;
float dxx=(qx*NtoN.fxtox)+dxy;
//Uint32*screenPix=srcPix+(int)((int)dyx*lscrw+dxx);
//if ((int)(*screenPix>>24)>=h)
//{
BufPic=bufPoint+(int)dyx*256+(int)dxx;
int diff=(int)(*BufPic>>16)-h;
//Uint32 heg=h;
h=h<<16;
if (diff>8) *BufPic=(*BufPic & 0x00000000) | (*Gp>>24) | (h);
else
if (diff>-8) *BufPic=(*BufPic & 0x000000FF) | (*Gp>>24) | (h);

*BufPic++;
diff=(*BufPic>>16)-h;
if (diff>8) *BufPic=(*BufPic & 0x00000000) | (*Gp>>24) | (h);
else
if (diff>-8) *BufPic=(*BufPic & 0x000000FF) | (*Gp>>24) | (h);

//	*screenPix=((*((Uint32*)palpx+((*Gp>>24) & 0xFF)))& 0x00FFFFFF) | (h<<24);
//register int col=(*((Uint32*)palpx+((*Gp>>24) & 0xFF)))& 0x00FFFFFF;
//register int sp=*screenPix;
//*screenPix=0;
//*screenPix=((((sp & RedMask) + (col & RedMask))>>1) & RedMask) | ((((sp & GreenMask) + (col & GreenMask))>>1)& GreenMask) | (((sp & BlueMask) + (col & BlueMask))>>1) | (sp & 0xFF000000);//| (yh<<24);
//else
//if ((int)(*screenPix>>24)==h)
//	 DecayByOffxOffy(dxx+offx, offy+dyx);
//drowBloodByOffxOffy(yx+offx, offy+xy);
//}


		Gp++;
		mycount--;
		}
		while (!(*Gp & 0x0000FF00));
	}
	while (!(*Gp & 0x00FF0000));
}
while (mycount);

//DrowBuffer(&palette.Buffer8bit[0], palpx, offx, offy, tempradius);
//DrowBuffer(&palette.Buffer8bit[0], palpx, (Uint32*)screen->pixels+offy*scrwi+offx - 128);

}
void G8bitVexToLight(G8_Particle * G, int offh, double resize, float globxh, float globyh, float glob, int offx, int offy, Uint32 power)//, Uint32* __restrict palpx)//, void (*onImpactFunk) (int ,int))
{	
	float radtores=G->radius*resize;
if ((radtores+offx>=scrwi) || (-radtores+offx<=0)
	|| (radtores+offy>=scrhi) || (-radtores+offy<=0)) return; 

power=min(32,power);
power=max(4,power);
power<<=16;

FloatAnglesToCosSin(globxh, globyh, glob, &NtoN);
New8BitNtoN(resize, &NtoN);

fillDarkMaskGrid(offx-radtores, offy-radtores, radtores*2, radtores*2);

Uint32* __restrict gp=G->pixels;
Uint32 * __restrict bufPoint=darkmask+(offy-16)*scrwi+(offx-16);
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

TrueX=qx*NtoN.fxtox+qh*NtoN.fhtox+qy*NtoN.fytox+16;
TrueY=qx*NtoN.fxtoy+qh*NtoN.fhtoy+qy*NtoN.fytoy+16;
//TrueH=qx*fxtoh+qh*fhtoh+qy*fytoh;

//heg=223-TrueH-offh;

BufPic=bufPoint+(TrueY*scrwi)+TrueX;

*BufPic+=power;

gp++;
plane--;
}
}
while (countme);



}


// This is The Shieeeet (Good ones)
bool QuickDrow8bitVexOneColor(Uint32* Buffer8bit,worldcoordinates* pos, G8_Particle * G, int bufoffx, int bufoffy, int offh,
							  double resize, float globxh, float globyh, float glob, Uint32 color, int angl)//, Uint32* __restrict palpx)//, void (*onImpactFunk) (int ,int))
{	
if ((G->radius*resize+bufoffx>=E8bitBufferRad) || (-G->radius*resize+bufoffx<=-E8bitBufferRad)
	|| (G->radius*resize+bufoffy>=E8bitBufferRad) || (-G->radius*resize+bufoffy<=-E8bitBufferRad)) 
	return 0;

palette.ThreadG8BufferRadius=max(palette.ThreadG8BufferRadius,G->radius*resize+1+max(abs(bufoffx),abs(bufoffy)));

FloatAnglesToCosSin(globxh, globyh, glob,&TNtoN);
New8BitNtoN(  resize,  &TNtoN);

Uint32* __restrict gp=G->pixels;
Uint32 * __restrict bufPoint=&Buffer8bit[E8bitBufferSize/2+E8bitBufferDiam/2+bufoffy*E8bitBufferDiam + bufoffx];
//palette.Buffer8bit
Uint32* __restrict BufPic;
int side=max((resize),1);
int setoff=E8bitBufferDiam-side*2;
int TrueX, TrueY, TrueH;
int heg;
int countme=G->count;
int i;
int diff, tempHeg;
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

TrueX=qx*TNtoN.fxtox+qh*TNtoN.fhtox+qy*TNtoN.fytox;
TrueY=qx*TNtoN.fxtoy+qh*TNtoN.fhtoy+qy*TNtoN.fytoy;
TrueH=qx*TNtoN.fxtoh+qh*TNtoN.fhtoh+qy*TNtoN.fytoh;

heg=223-TrueH-offh;

BufPic=bufPoint+(TrueY*E8bitBufferDiam)+TrueX;
Uint8 ang=*gp>>16;
if (ang!=192) ang=((ang/64*64) + ((ang + angl) % 64));
Uint32 gpp=(ang<<16); //| color;


i=side;
while (i>-side) {
int j=side;
while (j>-side){
	tempHeg=(sqrt((double)(i*i+j*j)));
	if (tempHeg<=side){
	tempHeg*=tempHeg;

	tempHeg+=heg;
diff=(int)(*BufPic & 0xFFFF)-(int)tempHeg;
if ((diff>=-3))
*BufPic= color |  ((diff>=0) ? tempHeg | gpp : (*BufPic)); //& 0xFF00FFFF));
	}
	BufPic++;
	j--;
}
BufPic+=setoff;
i--;
}



/*
//Testing End
E8bitBufFill(BufPic, heg, gpp);
BufPic+=E8bitBufferDiam;
E8bitBufFill(BufPic, heg, gpp);
BufPic++;
E8bitBufFill(BufPic, heg, gpp);
*/
gp++;

plane--;
}
}
while (countme);


return 1;
}
bool QuickDrow8bitVex(Uint32* Buffer8bit,worldcoordinates* pos, G8_Particle * G, int bufoffx, int bufoffy, int offh, double resize, float globxh, float globyh, float glob, bool lights)//, Uint32* __restrict palpx)//, void (*onImpactFunk) (int ,int))
{	
if ((G->radius*resize+bufoffx>=E8bitBufferRad) || (-G->radius*resize+bufoffx<=-E8bitBufferRad)
	|| (G->radius*resize+bufoffy>=E8bitBufferRad) || (-G->radius*resize+bufoffy<=-E8bitBufferRad)) 
	return 0;

palette.G8BufferRadius=max(palette.G8BufferRadius,G->radius*resize+1+max(abs(bufoffx),abs(bufoffy)));

FloatAnglesToCosSin(globxh, globyh, glob, &NtoN);
New8BitNtoN(resize, &NtoN);

if (lights) LightsTo180arrayF(pos, defAngleFade, glob, 0,deAlphaMask,0); 
Uint32* __restrict gp=G->pixels;
Uint32 * __restrict bufPoint=&Buffer8bit[E8bitBufferSize/2+E8bitBufferDiam/2+bufoffy*E8bitBufferDiam + bufoffx];
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
Uint32 gpp=*gp & 0xFFFF0000;
//Testing End
E8bitBufFill(BufPic, heg, gpp);
BufPic+=E8bitBufferDiam;
E8bitBufFill(BufPic, heg, gpp);
BufPic++;
E8bitBufFill(BufPic, heg, gpp);
gp++;
plane--;
}
}
while (countme);


return 1;
}
bool ThreadDrow8bitVex(worldcoordinates* pos, G8_Particle * G, int bufoffx, int bufoffy, int offh, double resize, float globxh, float globyh, float glob, bool lights)//, Uint32* __restrict palpx)//, void (*onImpactFunk) (int ,int))
{	
if ((G->radius*resize+bufoffx>=E8bitBufferRad) || (-G->radius*resize+bufoffx<=-E8bitBufferRad)
	|| (G->radius*resize+bufoffy>=E8bitBufferRad) || (-G->radius*resize+bufoffy<=-E8bitBufferRad)) 
	return 0;

palette.ThreadG8BufferRadius=max(palette.ThreadG8BufferRadius,G->radius*resize+1+max(abs(bufoffx),abs(bufoffy)));

FloatAnglesToCosSin(globxh, globyh, glob, &TNtoN);
New8BitNtoN(resize, &TNtoN);

Uint32* __restrict gp=G->pixels;
Uint32 * __restrict bufPoint=&palette.ThreadBuffer8bit[E8bitBufferSize/2+E8bitBufferDiam/2+bufoffy*E8bitBufferDiam + bufoffx];
//palette.Buffer8bit
Uint32* __restrict BufPic;
int side=max((resize)*1.5,1);
int setoff=E8bitBufferDiam-side*2;

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

TrueX=qx*TNtoN.fxtox+qh*TNtoN.fhtox+qy*TNtoN.fytox;
TrueY=qx*TNtoN.fxtoy+qh*TNtoN.fhtoy+qy*TNtoN.fytoy;
TrueH=qx*TNtoN.fxtoh+qh*TNtoN.fhtoh+qy*TNtoN.fytoh;

heg=max(0,255-TrueH*2-offh);

BufPic=bufPoint+TrueY*E8bitBufferDiam+TrueX;
//Testing End
Uint32 gpp=*gp & 0xFFFF0000;

int i=side;
int diff, tempHeg;
//if (heg<255)
while (i>-side) {
int j=side;
while (j>-side){
	tempHeg=(sqrt((double)(i*i+j*j)));
	if (tempHeg<=side){
	tempHeg*=tempHeg;///((float)temptest/10+1);

	tempHeg+=heg;
int diff=(int)(*BufPic & 0xFFFF)-(int)tempHeg;
if ((diff>=0))
*BufPic= gpp | tempHeg | ((diff>16) ? 0 : (*BufPic & 0xFF000000));
	}
	BufPic++;
	j--;
}
BufPic+=setoff;
//E8bitBufFill(BufPic, heg, *gp & 0xFFFF0000);
//BufPic++;
//E8bitBufFill(BufPic, heg, *gp & 0xFFFF0000);
i--;
}
gp++;
plane--;
}
}
while (countme);


return 1;
}

void QuickDrow8bitVexStaticH(worldcoordinates* pos, G8_Particle * G, int bufoffx, int bufoffy, int offh, double resize, float globxh, float globyh, float glob, bool lights) //, Uint32* __restrict palpx)//, void (*onImpactFunk) (int ,int))
{	
if ((G->radius*resize+bufoffx>=E8bitBufferRad) || (-G->radius*resize+bufoffx<=-E8bitBufferRad)
	|| (G->radius*resize+bufoffy>=E8bitBufferRad) || (-G->radius*resize+bufoffy<=-E8bitBufferRad)) 
	return;

palette.G8BufferRadius=max(palette.G8BufferRadius,G->radius*resize+1+max(abs(bufoffx),abs(bufoffy)));

FloatAnglesToCosSin(globxh, globyh, glob, &NtoN);
New8BitNtoN(resize, &NtoN);

if (lights) LightsTo180arrayF(pos, defAngleFade, glob, 0,deAlphaMask,0); 
Uint32* __restrict gp=G->pixels;
Uint32 * __restrict bufPoint=&palette.Buffer8bit[E8bitBufferSize/2+E8bitBufferDiam*bufoffy+E8bitBufferDiam/2+bufoffx];
Uint32* __restrict BufPic;

int TrueX, TrueY, TrueH;
int heg;
int countme=G->count;
if (countme>0)
do
{
	int qh=(char)(*gp>>16);
	//TrueH=qh*fhtoh;
heg=128-qh-offh;
float htx=qh*NtoN.fhtox;
float hty=qh*NtoN.fhtoy;
	Uint32 plane=*gp & 0xFFFF;
	gp++;
countme-=plane;
while (plane)
{
int qx=(char)(*gp & 0xFF);
int qy=(char)((*gp>>8) & 0xFF);
/*float hx = fcz*qx - fsz*qy;
float hy = fsz*qx + fcz*qy;	
float xy = fcx*hy - (fsx*qh);
xy=(xy*resize);
float xh = fsx*hy + (fcx*qh);
float yh = fcy*xh - (fsy*hx);
Uint32 heg=384-((int)(yh*resize))-offh;
float yx = fsy*xh + (fcy*hx);
yx=(yx*resize);*/
//BufPic=bufPoint+((int)(xy)<<8)+(int)yx;

// Testing New Version
TrueX=qx*NtoN.fxtox+htx+qy*NtoN.fytox;
TrueY=qx*NtoN.fxtoy+hty+qy*NtoN.fytoy;


BufPic=bufPoint+(TrueY<<8)+TrueX;
//Testing End
E8StaticBitBufFill(BufPic, heg, *gp & 0xFFFF0000);
BufPic++;
E8StaticBitBufFill(BufPic, heg, *gp & 0xFFFF0000);
BufPic+=E8bitBufferDiam;
E8StaticBitBufFill(BufPic, heg, *gp & 0xFFFF0000);
gp++;
plane--;
}
}
while (countme);

//DrowBuffer(&palette.Buffer8bit[0], palpx, offx, offy, tempradius);
 // (Uint32*)screen->pixels+offy*scrwi+offx - 128);


}
void QuickDrow8bitVexHeightOnly(Uint16* Buffer8bit,worldcoordinates* pos, G8_Particle * G, int bufoffx, int bufoffy, int offh, double resize, float globxh, float globyh, float glob, bool lights)//, Uint32* __restrict palpx)//, void (*onImpactFunk) (int ,int))
{	
while (glob>2*M_PI) glob-=2*M_PI;
// Rotation
//M_PI/2*maxI, callCenX, callCenY,maxI*16
int langle=glob*64/(M_PI*2);


palette.G8BufferRadius=max(palette.G8BufferRadius,G->radius*resize+1+max(abs(bufoffx),abs(bufoffy)));
if ((G->radius*resize+bufoffx>=E8bitBufferRad) || (-G->radius*resize+bufoffx<=-E8bitBufferRad)
	|| (G->radius*resize+bufoffy>=E8bitBufferRad) || (-G->radius*resize+bufoffy<=-E8bitBufferRad)) 
	return;

FloatAnglesToCosSin(globxh, globyh, glob, &NtoN);
New8BitNtoN(resize, &NtoN);

//if (lights) LightsTo180arrayF(pos, defAngleFade, glob, 0,deAlphaMask,0); 
Uint32* __restrict gp=G->pixels;
Uint16 * __restrict bufPoint=&Buffer8bit[E8bitBufferSize/2+E8bitBufferDiam/2];
//palette.Buffer8bit
Uint16* __restrict BufPic;

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
/*float hx = fcz*qx - fsz*qy;
float hy = fsz*qx + fcz*qy;	
float xy = fcx*hy - (fsx*qh);
xy=(xy*resize);
float xh = fsx*hy + (fcx*qh);
float yh = fcy*xh - (fsy*hx);
Uint32 heg=384-((int)(yh*resize))-offh;
float yx = fsy*xh + (fcy*hx);
yx=(yx*resize);*/
//BufPic=bufPoint+((int)(xy)<<8)+(int)yx;

// Testing New Version
TrueX=qx*NtoN.fxtox+qy*NtoN.fytox+bufoffx;//+qh*fhtox
TrueY=qx*NtoN.fxtoy+qy*NtoN.fytoy+bufoffy;//+qh*fhtoy
TrueH=qh*NtoN.fhtoh;//qx*fxtoh++qy*fytoh;

heg=255-TrueH-offh;
//if (ang!=192) ang=((ang/64*64) + ((ang + angle) % 64));
//langle
// ((Uint32)ang<<16)
BufPic=bufPoint+(TrueY<<8)+TrueX;
//Testing End
if 	((int)(*BufPic & 0xFF)>heg) // -8 ??
{
Uint32 ang=(*gp & 0xFF0000)>>16;
ang=((ang/64*64) + ((ang + langle) % 64));
	 *BufPic=heg  | (ang<<8); //| (*gp & 0xFF000000)
}



gp++;
plane--;
}
}
while (countme);

//DrowBuffer(&palette.Buffer8bit[0], palpx, offx, offy, tempradius);
 // (Uint32*)screen->pixels+offy*scrwi+offx - 128);


}
int QuickDrow8bitLayer(worldcoordinates* pos, G8_Particle * G, int bufoffx, int bufoffy, int offh, float resize, float globxh, float globyh, float glob, Uint32 lights, double* count)//, Uint32* __restrict palpx)//, void (*onImpactFunk) (int ,int))
{	
if (*count>=G->count) return 0;	
	palette.G8BufferRadius=max(palette.G8BufferRadius,G->radius*resize+1+max(abs(bufoffx),abs(bufoffy)));

resize=min(resize,1);


FloatAnglesToCosSin(globxh, globyh, glob, &NtoN);
//if (lights) LightsTo180array(pos, 0.95, glob, GetUpsideDownFromFloat(), lights,defaultMaxLight);

Uint32* __restrict gp=G->pixels+(int)(*count);
//*count+=1;
Uint32 * __restrict bufPoint=&palette.Buffer8bit[E8bitBufferSize/2+E8bitBufferDiam*bufoffy+E8bitBufferDiam/2+bufoffx];
Uint32* __restrict BufPic;

//int countme=G->count;

	float qh=(char)(*gp>>16);
	Uint32 plane=*gp & 0xFFFF;
	gp++;
*count+=plane+1;
//countme-=plane;
while (plane)
{
float qx=(char)(*gp & 0xFF);
float qy=(char)((*gp>>8) & 0xFF);
float hx = NtoN.fcz*qx - NtoN.fsz*qy;
float hy = NtoN.fsz*qx + NtoN.fcz*qy;	
float xy = NtoN.fcx*hy - (NtoN.fsx*qh);
xy=(xy*resize);
float xh = NtoN.fsx*hy + (NtoN.fcx*qh);
float yh = NtoN.fcy*xh - (NtoN.fsy*hx);
Uint32 heg=384-((int)(yh*resize))-offh;
float yx = NtoN.fsy*xh + (NtoN.fcy*hx);
yx=(yx*resize);

BufPic=bufPoint+((int)(xy)<<8)+(int)yx;

E8bitBufFill(BufPic, heg, *gp & 0xFFFF0000);
BufPic++;
E8bitBufFill(BufPic, heg, *gp & 0xFFFF0000);

gp++;
plane--;
}


//DrowBuffer(&palette.Buffer8bit[0], palpx, offx, offy, tempradius);
 // (Uint32*)screen->pixels+offy*scrwi+offx - 128);

return *count; 
}

//################################################# Quick Voxel Experiment
void BloodSuperFast(QG_Particle * G, int doffx, int doffy, int offh, SDL_Surface * screen, float resize, float globxh, float globyh, float glob, Uint32* __restrict palpx, void (*onImpactFunk) (int ,int), int brush)
{
	resize=min(1,resize);
int tempradius=G->radius*resize;
if ((doffx+tempradius>=screen->w) || (doffx-tempradius<=0) ||
	(doffy+tempradius>=screen->h) || (doffy-tempradius<=0)) return;
globxh=globxh+Vox3Deff*(doffx-scrwid);
globyh=globyh-Vox3Deff*(doffy-scrhid);
//Uint32 bright=max(0,(int)(32-resize*64))<<16;
FloatAnglesToCosSin(globxh, globyh, glob,&NtoN);
New8BitNtoN(resize,&NtoN);
//fhtoh*=brush;
//fxtoh*=brush;
//fytoh*=brush;
Uint32* __restrict gp=G->pixels;
Uint32* __restrict scrpx=(Uint32*)screen->pixels+(doffy-tempradius)*scrwi+(doffx-tempradius);
Uint32* __restrict darkcenter=darkmask+(offy-tempradius)*scrwi+(offx-tempradius);
int TrueX, TrueY, TrueH;
int heg;
offy-=tempradius;
offx-=tempradius;
//fillDarkMaskGrid(offx, offy, tempradius*2, tempradius*2);

int countme=G->count;
if (countme>0)
do
{
int qx=(char)(*gp & 0xFF);
int qy=(char)((*gp>>8) & 0xFF);
int qh=(char)((*gp>>16) & 0xFF);

TrueX=qx*NtoN.fxtox+qh*NtoN.fhtox+qy*NtoN.fytox+tempradius;
TrueY=qx*NtoN.fxtoy+qh*NtoN.fhtoy+qy*NtoN.fytoy+tempradius;
TrueH=qx*NtoN.fxtoh+qh*NtoN.fhtoh+qy*NtoN.fytoh;

heg=TrueH+offh;
int off=TrueY*scrwi+TrueX;
Uint32* __restrict screenPix=scrpx+off;

if (heg>=0)
{
//*(darkcenter+off)+=bright;
register int col=*((Uint32*)palpx+((*gp>>24) & 0xFF));
//MultiplyFloatColorSingle((Uint32*)&col);
register int sp=*screenPix;
*screenPix=((((sp & RedMask)*3 + (col & RedMask))>>2) & RedMask)
	| ((((sp & GreenMask)*3 + (col & GreenMask))>>2)& GreenMask) 
	| (((sp & BlueMask)*3 + (col & BlueMask))>>2) 
	| (sp & 0xFF000000);//| (yh<<24);


if (!heg)
	onImpactFunk(TrueX+offx, offy+TrueY);
}
gp++;
countme--;
}
while (countme);
}
void CleanSmoke(int offx, int offy, int w, int h ){
offx=max(0,offx);
w=min(w, scrwi-offx);
offy=max(0,offy);
h=min(h, scrhi-offy);
Uint32 TurnMask=0x1<<(30+dustTurn);
Uint32* __restrict dspix=dustScreen + offy*scrwi+offx;
int scrofy=scrwi-w;
	for (int j=0; j<h; j++){
		for (int jj=0; jj<w; jj++){
			if ((*dspix & 0xFF000000)!=TurnMask)
			*dspix = TurnMask;
			 dspix++;
		}
		dspix+=scrofy;
	}
}
void DustAndLight(QG_Particle * G, int offx, int offy, int offh, SDL_Surface * screen, float resize, float globxh, float globyh, float glob)
{

int tempradius=G->radius*resize;
if ((offx+tempradius>=screen->w) || (offx-tempradius<=0) ||
	(offy+tempradius>=screen->h) || (offy-tempradius<=0)) return;
globxh=globxh+Vox3Deff*(offx-scrwid);
globyh=globyh-Vox3Deff*(offy-scrhid);
Uint32 bright=max(0,(int)(12-resize*8))<<16;
FloatAnglesToCosSin(globxh, globyh, glob,&NtoN);
New8BitNtoN(resize,&NtoN);
Uint32* __restrict gp=G->pixels;
Uint32* __restrict darkcenter=darkmask+(offy-tempradius)*scrwi+(offx-tempradius);
Uint32* __restrict dustcenter=dustScreen + (offy)*scrwi+offx;
int TrueX, TrueY, TrueH;
int heg;
offy-=tempradius;
offx-=tempradius;
fillDarkMaskGrid(offx, offy, tempradius*2, tempradius*2);
CleanSmoke(offx, offy, tempradius*2, tempradius*2);

int countme=G->count;
if (countme>0)
do
{
int qx=(char)(*gp & 0xFF);
int qy=(char)((*gp>>8) & 0xFF);
int qh=(char)((*gp>>16) & 0xFF);

TrueX=qx*NtoN.fxtox+qh*NtoN.fhtox+qy*NtoN.fytox+tempradius;
TrueY=qx*NtoN.fxtoy+qh*NtoN.fhtoy+qy*NtoN.fytoy+tempradius;

int off=TrueY*scrwi+TrueX;
*(darkcenter+off)+=0xF<<16; // 0xF Value was temptes, tune it before use

gp++;
countme--;
}
while (countme);
}


void SuperFastQVoxel(QG_Particle * G, int offx, int offy, int offh, SDL_Surface * screen, double globxh, double globyh, double glob, float resize, Uint32* __restrict palpx)
{

resize=min(resize,1);
if ((offx-G->radius>=screen->w) || (offx+G->radius<=0) || (offy-G->radius>=screen->h) || (offy+G->radius<=0)) return;
if ((offx+G->radius>screen->w) || (offx-G->radius<0) || (offy+G->radius>screen->h) || (offy-G->radius<0))
	{
		//MergeGParticleSafe( G, offx, offy, globxh, globyh, gx, gy);
		return;
	}
float part=resize;
double locxh=globxh+0.001*(offx-scrwid);

FloatAnglesToCosSin(globxh, globyh, glob,&NtoN);
SuperFastNtoN_float();

float offhmin128=128-offh;


//offh=128-offh;
//Gpixel * Gp=G->gpixels;
Uint32* __restrict Gp=G->pixels;
Uint32* __restrict srcPix=(Uint32*)screen->pixels+screen->w*offy+offx;
int lscrw=screen->w;

int mycount=G->count-1;
if (mycount>0)
do
{
float qh=(char)((*Gp>>16) & 0xFF);
qh*=part;
float hforh=(qh*NtoN.fhtoh);
float dyh=(qh*NtoN.fhtoy);
float dxh=(qh*NtoN.fhtox);
//int hforxy=dyh*lscrw+dxh;

	do
	{//yh*4
	float qy=(char)((*Gp>>8) & 0xFF);
	qy*=part;
	float yforh=offhmin128-hforh-(qy*NtoN.fytoh);
	float dyy=(qy*NtoN.fytoy)+dyh;
	float dxy=(qy*NtoN.fytox)+dxh;
	//Uint32* yforxy=dyy*lscrw+dxy+srcPix;
	
		do
		{
	

float qx=(char)(*Gp & 0xFF);
qx*=part;
Uint32 h=yforh-(qx*NtoN.fxtoh);
float dyx=(qx*NtoN.fxtoy)+dyy;		
float dxx=(qx*NtoN.fxtox)+dxy;
Uint32*screenPix=srcPix+(int)((int)dyx*lscrw+dxx);
if ((int)(*screenPix>>24)>=h)
{
	*screenPix=((*((Uint32*)palpx+((*Gp>>24) & 0xFF)))& 0x00FFFFFF) | (h<<24);
//register int col=(*((Uint32*)palpx+((*Gp>>24) & 0xFF)))& 0x00FFFFFF;
//register int sp=*screenPix;
//*screenPix=0;
//*screenPix=((((sp & RedMask) + (col & RedMask))>>1) & RedMask) | ((((sp & GreenMask) + (col & GreenMask))>>1)& GreenMask) | (((sp & BlueMask) + (col & BlueMask))>>1) | (sp & 0xFF000000);//| (yh<<24);
//else
//if ((int)(*screenPix>>24)==h)
//	 DecayByOffxOffy(dxx+offx, offy+dyx);
//drowBloodByOffxOffy(yx+offx, offy+xy);
}


		Gp++;
		mycount--;
		}
		while (!(*Gp & 0x0000FF00));
	}
	while (!(*Gp & 0x00FF0000));
}
while (mycount);
}
void SuperFastQVoxelSimetrical(QG_Particle * G, int offx, int offy, int offh, SDL_Surface * screen, double globxh, double globyh, double glob, Uint32* __restrict palpx)
{
if ((offx-G->radius>=screen->w) || (offx+G->radius<=0) || (offy-G->radius>=screen->h) || (offy+G->radius<=0)) return;
if ((offx+G->radius>screen->w) || (offx-G->radius<0) || (offy+G->radius>screen->h) || (offy-G->radius<0))
	{
		//MergeGParticleSafe( G, offx, offy, globxh, globyh, gx, gy);
		return;
	}

double locxh=globxh+0.001*(offx-scrwid);

QuickAnglesToCosSin(globxh, globyh, glob);
SuperFastNtoN();




offh=128-offh;
//Gpixel * Gp=G->gpixels;
Uint32* __restrict Gp=G->pixels;
Uint32* __restrict srcPix=(Uint32*)screen->pixels+screen->w*offy+offx;
int lscrw=screen->w;

int mycount=G->count-1;
if (mycount>0)
do
{
int qh=(char)((*Gp>>16) & 0xFF);
int hforxy=((qh*htoy)>>16)*lscrw+((qh*htox)>>16);
//int hforh=(qh*htoh);
	do
	{//yh*4
	int qy=(char)((*Gp>>8) & 0xFF);
	Uint32* yforxy=hforxy+((qy*ytoy)>>16)*lscrw+((qy*ytox)>>16)+srcPix;
	//int yforh=hforh+(qy*ytoh);
		do
		{
	//	int h=offh-((yforh+(qx*xtoh))>>14);

		int qx=(char)(*Gp & 0xFF);
		//int y=
		//Uint32* scrp=srcPix+yforxy+((qx*xtoy)>>16)*lscrw+((qx*xtox)>>16); 
		int xoff=((qx*xtoy)>>16)*lscrw+((qx*xtox)>>16);
		*(yforxy+xoff+1)=*(yforxy-xoff+1)=*(yforxy+xoff)=*(yforxy-xoff)=(*((Uint32*)palpx+((*Gp>>24) & 0xFF))) & 0x00FFFFFF;


	/*	if ((int)(*scrp>>24)>=h)
{
register int col=*((Uint32*)palpx+((*Gp>>24) & 0xFF));
register int sp=*scrp;
*scrp=((((sp & RedMask) + (col & RedMask))>>1) & RedMask) | ((((sp & GreenMask) + (col & GreenMask))>>1)& GreenMask) | (((sp & BlueMask) + (col & BlueMask))>>1) | (sp & 0xFF000000);//| (yh<<24);
}*/
		//*scrp=*((Uint32*)palpx+((*Gp>>24) & 0xFF));
		//}
		Gp++;
		mycount--;
		}
		while (!(*Gp & 0x0000FF00));
	}
	while (!(*Gp & 0x00FF0000));
}
while (mycount);
}

void QuickDrowVexPalette(QG_Particle * G, int offx, int offy, int h, SDL_Surface * screen, float globxh, float globyh, float glob, SDL_Surface* palette, Uint32 defColor)
{

if ((offx+G->radius>screen->w) || (offx-G->radius<0) || (offy+G->radius>screen->h) || (offy-G->radius<0))
	{
		//DrowVexSafe( G, offx, offy, offh, screen, globxh, globyh, glob);
		return;
	}

Uint32 heightMod=max(255-accurate(h)*8,0);
heightMod<<=24;
defColor=min(OnelightB,511)/2+(((int)min(OnelightG,511)/2)<<8) +(((int)min(OnelightR,511)/2)<<16)+heightMod;
int w=palette->w;
Uint32* palpx=(Uint32*)palette->pixels+w*(palette->h/2)+w/2;



globxh=globxh+Vox3Deff*(offx-scrwid);

globyh=globyh-Vox3Deff*(offy-scrhid);
//QuickAnglesToCosSin(globxh, globyh, glob);
FloatAnglesToCosSin(globxh, globyh, glob,&NtoN);
Uint32* __restrict gp=G->pixels;
Uint32* __restrict scrpx=(Uint32*)screen->pixels+offy*scrwi+offx;;
int countme=G->count;
float qh=0;
if (countme>0)
do
{
float qx=(char)(*gp & 0xFF);
float qy=(char)((*gp>>8) & 0xFF);
float hx = NtoN.fcz*qx - NtoN.fsz*qy;
float hy = NtoN.fsz*qx + NtoN.fcz*qy;	
qh=(char)((*gp>>16) & 0xFF);
float xy = NtoN.fcx*hy - (NtoN.fsx*qh);
//xy=(int)(xy)>>16;
float xh = NtoN.fsx*hy + (NtoN.fcx*qh);
float yx = NtoN.fsy*xh + (NtoN.fcy*hx);
//yx=(int)(yx)>>24;
int off=(int)xy*scrwi+(int)yx;
Uint32* __restrict screenPix=scrpx+off;
Uint32 color=(*(palpx+w*(int)qy+(int)qx));
 MultiplyFloatColorSingle(&color);
*screenPix=(color & 0x00FFFFFF) | heightMod;
gp++;
countme--;
}
while (countme && qh<=-5);

OnelightB/=1.5;
OnelightR/=1.5;
OnelightG/=1.5;

if (countme>0)
do
{
float qx=(char)(*gp & 0xFF);
float qy=(char)((*gp>>8) & 0xFF);
float hx = NtoN.fcz*qx - NtoN.fsz*qy;
float hy = NtoN.fsz*qx + NtoN.fcz*qy;	
qh=(char)((*gp>>16) & 0xFF);
float xy = NtoN.fcx*hy - (NtoN.fsx*qh);
//xy=(int)(xy)>>16;
float xh = NtoN.fsx*hy + (NtoN.fcx*qh);
float yx = NtoN.fsy*xh + (NtoN.fcy*hx);
//yx=(int)(yx)>>24;
int off=(int)xy*scrwi+(int)yx;
Uint32* __restrict screenPix=scrpx+off;
Uint32 color=(*(palpx+w*(int)qy+(int)qx));
 MultiplyFloatColorSingle(&color);
*screenPix=(color & 0x00FFFFFF) | heightMod;
gp++;
countme--;
}
while (countme && qh<=3);

OnelightB/=1.5;
OnelightR/=1.5;
OnelightG/=1.5;

if (countme>0)
do
{
float qx=(char)(*gp & 0xFF);
float qy=(char)((*gp>>8) & 0xFF);
float hx = NtoN.fcz*qx - NtoN.fsz*qy;
float hy = NtoN.fsz*qx + NtoN.fcz*qy;	
qh=(char)((*gp>>16) & 0xFF);
float xy = NtoN.fcx*hy - (NtoN.fsx*qh);
//xy=(int)(xy)>>16;
float xh = NtoN.fsx*hy + (NtoN.fcx*qh);
float yx = NtoN.fsy*xh + (NtoN.fcy*hx);
//yx=(int)(yx)>>24;
int off=(int)xy*scrwi+(int)yx;
Uint32* __restrict screenPix=scrpx+off;
Uint32 color=(*(palpx+w*(int)qy+(int)qx));
 MultiplyFloatColorSingle(&color);
*screenPix=(color & 0x00FFFFFF) | heightMod;
gp++;
countme--;
}
while (countme);

}
void QuickDrowVexNoBelly(QG_Particle * G, int offx, int offy, SDL_Surface * screen, float globxh, float globyh, float glob, Uint32* __restrict palpx)
{

if ((offx+G->radius>screen->w) || (offx-G->radius<0) || (offy+G->radius>screen->h) || (offy-G->radius<0))
	{
		//DrowVexSafe( G, offx, offy, offh, screen, globxh, globyh, glob);
		return;
	}
globxh=globxh+Vox3Deff*(offx-scrwid);

globyh=globyh-Vox3Deff*(offy-scrhid);
QuickAnglesToCosSin(globxh, globyh, glob);
Uint32* __restrict gp=G->pixels;
Uint32* __restrict scrpx=(Uint32*)screen->pixels+offy*scrwi+offx;;
int countme=G->count;
if (countme>0)
do
{
int qx=(char)(*gp & 0xFF);
int qy=(char)((*gp>>8) & 0xFF);
int hx = cz*qx - sz*qy;
int hy = sz*qx + cz*qy;	
int qh=(char)((*gp>>16) & 0xFF);
int xy = cx*hy - (sx*qh<<8);
xy=(int)(xy)>>16;
int xh = sx*hy + (cx*qh<<8);
int yx = sy*xh + (cy*hx<<8);
yx=(int)(yx)>>24;
int off=xy*scrwi+yx;
Uint32* __restrict screenPix=scrpx+off;
*screenPix=((*((Uint32*)palpx+((*gp>>24) & 0xFF))));
gp++;
countme--;
}
while (countme);
}
void QuickDrowVex(QG_Particle * G, int offx, int offy, int offh, SDL_Surface * screen, float resize, float globxh, float globyh, float glob, Uint32* __restrict palpx, void (*onImpactFunk) (int ,int))
{

int tempradius=G->radius*resize;
if ((offx+tempradius>=screen->w) || (offx-tempradius<=0) ||
	(offy+tempradius>=screen->h) || (offy-tempradius<=0)) return;
globxh=globxh+Vox3Deff*(offx-scrwid);
globyh=globyh-Vox3Deff*(offy-scrhid);
QuickAnglesToCosSin(globxh, globyh, glob);
Uint32* __restrict gp=G->pixels;
Uint32* __restrict scrpx=(Uint32*)screen->pixels+offy*scrwi+offx;;
int countme=G->count;
if (countme>0)
do
{
int qx=(char)(*gp & 0xFF);
int qy=(char)((*gp>>8) & 0xFF);
int hx = cz*qx - sz*qy;
int hy = sz*qx + cz*qy;	
int qh=(char)((*gp>>16) & 0xFF);
int xy = cx*hy - (sx*qh<<8);
xy=(int)(xy*resize)>>16;
int xh = sx*hy + (cx*qh<<8);
int yh = cy*xh - (sy*hx<<8);
yh=128-((int)(yh*resize)>>22)-offh;
int yx = sy*xh + (cy*hx<<8);
yx=(int)(yx*resize)>>24;

int off=xy*scrwi+yx;
Uint32* __restrict screenPix=scrpx+off;
if ((int)(*screenPix>>24)>=yh)
{
register int col=*((Uint32*)palpx+((*gp>>24) & 0xFF));
register int sp=*screenPix;
*screenPix=((((sp & RedMask) + (col & RedMask))>>1) & RedMask) | ((((sp & GreenMask) + (col & GreenMask))>>1)& GreenMask) | (((sp & BlueMask) + (col & BlueMask))>>1) | (sp & 0xFF000000);//| (yh<<24);
//else

if ((int)(*screenPix>>24)==yh)
	onImpactFunk(yx+offx, offy+xy);
}
gp++;
countme--;
}
while (countme);
}

void QuickDrowVexXYH(QG_Particle * G, int offx, int offy, int offh, SDL_Surface * screen, float globxh, float globyh, float glob, Uint32* __restrict palpx)
{

if ((offx-G->radius>=screen->w) || (offx+G->radius<=0) || (offy-G->radius>=screen->h) || (offy+G->radius<=0)) return;
if ((offx+G->radius>screen->w) || (offx-G->radius<0) || (offy+G->radius>screen->h) || (offy-G->radius<0))
	{
		//DrowVexSafe( G, offx, offy, offh, screen, globxh, globyh, glob);
		return;
	}

globxh=globxh+Vox3Deff*(offx-scrwid);

globyh=globyh-Vox3Deff*(offy-scrhid);

 QuickAnglesToCosSin(globxh, globyh, glob);

Uint32* __restrict gp=G->pixels;
Uint32* __restrict scrpx=(Uint32*)screen->pixels+offy*scrwi+offx;;

int countme=G->count;
if (countme>0)
do
{
	int qx=(char)(*gp & 0xFF);
	int qy=(char)((*gp>>8) & 0xFF);
	int qh=(char)((*gp>>16) & 0xFF);

int xy=cx*qy - sx*qh;
int xh=sx*qy + cx*qh;

int yh=cy*xh-(sy*qx<<8);
int yx=sy*xh+(cy*qx<<8);

int hx=cz*yx-(sz*xy<<8);
int hy=sz*yx+(cz*xy<<8);

yh=128-((int)(yh)>>14)-offh;
hy=(int)(hy)>>24;
hx=(int)(hx)>>24;
/*
int hx = cz*qx - sz*qy;
int hy = sz*qx + cz*qy;	
int xy = cx*hy - (sx*qh<<8);
int xh = sx*hy + (cx*qh<<8);
int yh = cy*xh - (sy*hx<<8);
int yx = sy*xh + (cy*hx<<8);

yh=128-((int)(yh*resize)>>22)-offh;
xy=(int)(xy*resize)>>16;
yx=(int)(yx*resize)>>24; */

int off=hy*scrwi+hx;
Uint32* __restrict screenPix=scrpx+off;
if ((int)(*screenPix>>24)>=yh)
{
register int col=*((Uint32*)palpx+((*gp>>24) & 0xFF));
register int sp=*screenPix;
*screenPix=((((sp & RedMask) + (col & RedMask))>>1) & RedMask) | ((((sp & GreenMask) + (col & GreenMask))>>1)& GreenMask) | (((sp & BlueMask) + (col & BlueMask))>>1) | (sp & 0xFF000000);//| (yh<<24);
if ((int)(*screenPix>>24)==yh)
drowBloodByOffxOffy(yx+offx, offy+xy);
}
gp++;
countme--;
}
while (countme);
}

//################################################ Vocsels With Color Variable
void DrowBigVexFullColor(G_Particle * G, int offx, int offy, SDL_Surface * screen, float globxh, float globyh, float glob, float size, void (*PixManipilation) (int ,int ,int , int, int))
{
if ((offx-G->radius>=screen->w) || (offx+G->radius<=0) || (offy-G->radius>=screen->h) || (offy+G->radius<=0)) return;
if ((offx+G->radius>screen->w) || (offx-G->radius<0) || (offy+G->radius>screen->h) || (offy-G->radius<0))
	{
	//	DrowGParticleSafe( G, offx, offy, screen, globxh, globyh);
		return;
	}

//float locxh=globxh+0.002*(offx-scrwid);
FloatAnglesToCosSin(globxh, globyh, glob,&NtoN);
 double xy, xh, yx, yh, hx, hy;
GlobGp=G->gpixels;
int mycount=G->count;
if (mycount>0)
do
{
int qx=(char)(GlobGp->xyhPI & 0xFF);
int qy=(char)((GlobGp->xyhPI>>8) & 0xFF);
int qh=(char)((GlobGp->xyhPI>>16) & 0xFF); 

hx = NtoN.fcz*qx - NtoN.fsz*qy;
hy = NtoN.fsz*qx + NtoN.fcz*qy;	
xy = NtoN.fcx*hy - NtoN.fsx*qh;
xh = NtoN.fsx*hy + NtoN.fcx*qh;
yh = NtoN.fcy*xh - NtoN.fsy*hx;
yx = NtoN.fsy*xh + NtoN.fcy*hx;

xy*=palette.zoom;
yx*=palette.zoom;
//yh>>=(22);

/*
float Defct=-(yh>>2);
Defct=1+Defct/32;

xy*=Defct;
yx*=Defct;*/

float Defct=-yh/4;
Defct=1+Defct/32;

xy*=Defct;
yx*=Defct;

//int temph=size-yh/2;
//temph=max(1,temph);
	yh+=128;
PixManipilation((yx)+offx,xy+offy,(size*palette.zoom)*Defct, GlobGp->color, yh);
GlobGp++;
mycount--;
}
while (mycount);
}
void DrowBigVex(G8_Particle * G, int offx, int offy, SDL_Surface * screen, float globxh, float globyh, float glob, float size, void (*PixManipilation) (int ,int ,int , int, int))
{
worldcoordinates voxelBody;
voxelBody.tx=0;
voxelBody.ty=0;
voxelBody.x=offx*accuracy;
voxelBody.y=offy*accuracy;

//QuickAnglesToCosSin(globxh, globyh, glob);
FloatAnglesToCosSin(globxh, globyh, glob,&NtoN);
//double newgl=GetUpsideDownFromQuick();
bool ud=GetUpsideDownFromQuick(&glob);
LightsTo180arrayF(&voxelBody, 0.98, glob, ud, deAlphaMask, 0);

 double xy, xh, yx, yh, hx, hy;
GlobG8=G->pixels;
int mycount=G->count;
if (mycount>0)
do
{
	//myG.pixels[hcountStore]=(count-hcountStore) | ((l+dh-vSized)<<16); 
	int qh=(char)(*GlobG8>>16);
	Uint32 plane=*GlobG8 & 0xFFFF;
	GlobG8++;
while (plane)
{
int qx=(char)(*GlobG8 & 0xFF);
int qy=(char)((*GlobG8>>8) & 0xFF);
//int qh=(char)((*GlobG8>>16) & 0xFF); 

hx = NtoN.fcz*qx - NtoN.fsz*qy;
hy = NtoN.fsz*qx + NtoN.fcz*qy;	
xy = NtoN.fcx*hy - NtoN.fsx*qh;
xh = NtoN.fsx*hy + NtoN.fcx*qh;
yh = NtoN.fcy*xh - NtoN.fsy*hx;
yx = NtoN.fsy*xh + NtoN.fcy*hx;


xy*=palette.zoom;
yx*=palette.zoom;
//xy<<=palette.zoom;
//yx<<=palette.zoom;
//yh>>=(22);


float Defct=-yh/4;
Defct=1+Defct/32;

xy*=Defct;
yx*=Defct;

//float Defct=1;
//int temph=size-yh/2;
//temph=max(1,temph);
//int grad=getGradus(x,y);
Uint32 col=((*GlobG8>>16) & 0xFF);//getGradus(yx,xy)/2;
//if (col>179) 
//	col=179; 
Uint32 Macolor=G->palette[*GlobG8>>24];//lightArray[col];
//MaxColorcolor=*bufPoint>>24;
//	AddColor(G->palette[*GlobG8>>24],&Macolor);
MultiplyFloatColor(col ,&Macolor);
//MultiplyColor1(G->palette[*GlobG8>>24],&Macolor);
//vox[curVoxel].eightPalette[*GlobG8>>24], &Macolor);
	//vox[curVoxel].eightPalette[*GlobG8>>24];
	yh+=128;
PixManipilation((yx)+offx,xy+offy,(size*palette.zoom)*Defct, Macolor, yh);
GlobG8++;
mycount--;
plane--;
}
}
while (mycount);
}
void BigVexGetBrushPlaneCoords(int* qx, int* qy, int* qh, int offx, int offy, float globxh, float globyh, float glob, float size)
{

FloatAnglesToCosSin(-globxh, -globyh, -glob,&NtoN);


 double xy, xh, yx, yh, hx, hy;

 /// backwards:
 *qx-=offx;
 *qy-=offy;

*qh-=128;


float Defct=-*qh/4;
Defct=1+Defct/32;

*qy/=Defct;
*qx/=Defct;
//float Defct=1;

*qy/=palette.zoom;
*qx/=palette.zoom;

yh = NtoN.fcy**qh - NtoN.fsy**qx;
yx = NtoN.fsy**qh + NtoN.fcy**qx;

xy = NtoN.fcx**qy - NtoN.fsx*yh;
*qh=xh = NtoN.fsx**qy + NtoN.fcx*yh;

*qx=hx = NtoN.fcz*yx - NtoN.fsz*xy;
*qy=hy = NtoN.fsz*yx + NtoN.fcz*xy;	


}


void DrowVexSafe(G_Particle * G, int offx, int offy, int offh, SDL_Surface * screen, float globxh, float globyh, float glob)
{
	offh/=2;

globxh=globxh+Vox3Deff*(offx-scrwid);

globyh=globyh-Vox3Deff*(offy-scrhid);

//float locxh=globxh+0.002*(offx-scrwid);
 QuickAnglesToCosSin(globxh, globyh, glob);

 int xy, xh, yx, yh, hx, hy;
 Uint32* screenPix;
Gpixel * Gp=G->gpixels;
int mycount=G->count;
if (mycount>0)
do
{
 int qx=(char)(Gp->xyhPI & 0xFF);
	int qy=(char)((Gp->xyhPI>>8) & 0xFF);
	int qh=(char)((Gp->xyhPI>>16) & 0xFF); 

hx = cz*qx - sz*qy;
hy = sz*qx + cz*qy;	
xy = cx*hy - (sx*qh<<8);
xh = sx*hy + (cx*qh<<8);
yh = cy*xh - (sy*hx<<8);
yx = sy*xh + (cy*hx<<8);

xy>>=16;
yx>>=24;
yh>>=24;
yh=128-yh-offh;


xy+=offy;
yx+=offx;
if ((xy>=0) && (xy<scrhi) && (yx>=0) && (yx<scrwi))
{
screenPix=(Uint32*)screen->pixels+xy*scrwi+yx;
if ((int)(*((Uint8 * )screenPix+3))>(yh))
{


*screenPix=Gp->color;
*((Uint8*)screenPix+3)=yh;

}
}
Gp++;
mycount--;
}
while (mycount);
}
void DrowVexNohtpBelly(G_Particle * G, int offx, int offy, int offh, SDL_Surface * screen, float globxh, float globyh, float glob)
{
//	offh/=2;
if ((offx-G->radius>=screen->w) || (offx+G->radius<=0) || (offy-G->radius>=screen->h) || (offy+G->radius<=0)) return;
if ((offx+G->radius>screen->w) || (offx-G->radius<0) || (offy+G->radius>screen->h) || (offy-G->radius<0))
	{
		DrowVexSafe( G, offx, offy, offh, screen, globxh, globyh, glob);
		return;
	}

globxh=globxh+Vox3Deff*(offx-scrwid);

globyh=globyh-Vox3Deff*(offy-scrhid);

int accuracy=256;
int accuracys=accuracy*accuracy*accuracy;
//float locxh=globxh+0.002*(offx-scrwid);
QuickAnglesToCosSin(globxh, globyh, glob);

 int xy, xh, yx, yh, hx, hy;
 Uint32* __restrict screenPix;
Gpixel * __restrict Gp=G->gpixels;
Uint32* __restrict scrpx=(Uint32*)screen->pixels;

int mycount=G->count;
if (mycount>0)
do
{
 int qx=(char)(Gp->xyhPI & 0xFF);
	int qy=(char)((Gp->xyhPI>>8) & 0xFF);
	int qh=(char)((Gp->xyhPI>>16) & 0xFF); 
hx = cz*qx - sz*qy;
hy = sz*qx + cz*qy;	

xy = cx*hy - (sx*qh<<8);
xy>>=16;
xy+=offy;

xh = sx*hy + (cx*qh<<8);

yh = cy*xh - (sy*hx<<8);
yh=128-(yh>>24)-offh;

yx = sy*xh + (cy*hx<<8);
yx>>=24;
yx+=offx;

int off=(xy)*scrwi+yx;


screenPix=scrpx+off;
//int temph=;

if ((*screenPix>>24)>yh)
{

*screenPix=Gp->color & 0x00FFFFFF | (yh<<24);
*(darkmask+off+1)&=0xFFFFFF00;
//*((Uint8*)screenPix+3)=yh;
/*
*screenPix=gp->color;
*((Uint8*)screenPix+3)=yh;
}
else
if (((int)(*((Uint8 * )screenPix+3))==(yh)))
{*/
/*	register int col=gp->color;
	register int sp=*screenPix;
*screenPix=((((sp & RedMask)>>1) + ((col & RedMask)>>1))& RedMask) | ((((sp & GreenMask)>>1) + ((col & GreenMask)>>1))& GreenMask) | ((((sp & BlueMask)>>1) + ((col & BlueMask)>>1))& BlueMask);
*((Uint8*)screenPix+3)=yh;	*/
}
/*else
{
screenPix++;
if ((*screenPix>>24)>yh)
	*screenPix=gp->color & 0x00FFFFFF | (yh<<24);
}*/

Gp++;
mycount--;
}
while (mycount);
}

void BufferDrowVexSafe(G_Particle * G, int offx, int offy, int offh, SDL_Surface * screen, float globxh, float globyh, float glob, SDL_Surface* source)
{
	offh/=2;

globxh=globxh+Vox3Deff*(offx-scrwid);

globyh=globyh-Vox3Deff*(offy-scrhid);

QuickAnglesToCosSin(globxh, globyh, glob);

 int xy, xh, yx, yh, hx, hy;
 Uint32* screenPix;
Gpixel * Gp=G->gpixels;
Uint32* __restrict scrpx=(Uint32*)screen->pixels;
Uint32* __restrict imgpx=(Uint32*)source->pixels;
int mycount=G->count;
if (mycount>0)
do
{
int qx=(char)(Gp->xyhPI & 0xFF);
	int qy=(char)((Gp->xyhPI>>8) & 0xFF);
	int qh=(char)((Gp->xyhPI>>16) & 0xFF); 

hx = cz*qx - sz*qy;
hy = sz*qx + cz*qy;	

xy = cx*hy - (sx*qh<<8);
xy>>=16;
xy+=offy;

xh = sx*hy + (cx*qh<<8);

yh = cy*xh - (sy*hx<<8);
yh=128-(yh>>24)-offh;

yx = sy*xh + (cy*hx<<8);
yx>>=24;
yx+=offx;
if ((xy>=0) && (xy<scrhi) && (yx>=0) && (yx<scrwi))
{
screenPix=scrpx+xy*scrwi+yx;
if (*screenPix>>24>yh)
{
*screenPix=*(imgpx+Gp->TextureImagePixel) & 0x00FFFFFF | (yh<<24);

}
}
Gp++;
mycount--;
}
while (mycount);
}
void BufferDrowVex(G_Particle * G, int offx, int offy, int offh, SDL_Surface * screen, float globxh, float globyh, float glob, SDL_Surface* source)
{
	offh/=2;
if ((offx-G->radius>=screen->w) || (offx+G->radius<=0) || (offy-G->radius>=screen->h) || (offy+G->radius<=0)) return;
if ((offx+G->radius>screen->w) || (offx-G->radius<0) || (offy+G->radius>screen->h) || (offy-G->radius<0))
	{
		BufferDrowVexSafe( G, offx, offy, offh, screen, globxh, globyh, glob, source);
		return;
	}

globxh=globxh+Vox3Deff*(offx-scrwid);

globyh=globyh-Vox3Deff*(offy-scrhid);

QuickAnglesToCosSin(globxh, globyh, glob);

// int xy, xh, yx, yh, hx, hy;
// Uint32* __restrict screenPix;
Gpixel * __restrict Gp=G->gpixels;
Uint32* __restrict scrpx=(Uint32*)screen->pixels;
Uint32* __restrict imgpx=(Uint32*)source->pixels;
int mycount=G->count;
if (mycount>0)
do
{
 int qx=(char)(Gp->xyhPI & 0xFF);
	int qy=(char)((Gp->xyhPI>>8) & 0xFF);
	int qh=(char)((Gp->xyhPI>>16) & 0xFF); 

int hx = cz*qx - sz*qy;
int hy = sz*qx + cz*qy;	

int xy = cx*hy - (sx*qh<<8);
xy>>=16;
xy+=offy;

int xh = sx*hy + (cx*qh<<8);

int yh = cy*xh - (sy*hx<<8);
yh=128-(yh>>24)-offh;

int yx = sy*xh + (cy*hx<<8);
yx>>=24;
yx+=offx;


int off=(xy)*scrwi+yx;
*(darkmask+off)&=0xFFFFFF00;
Uint32* __restrict screenPix=scrpx+off;
//register int col= *(imgpx+gp->TextureImagePixel);
//if ((*screenPix>>24>=yh)) //&& (from>>24==0xFF))
//if (gp->TextureImagePixel>source->w*source->h)
	//yh=0;
Uint32 color= *(imgpx+Gp->TextureImagePixel);
if (color>>24)
		*screenPix= color & 0x00FFFFFF;

Gp++;
mycount--;
}
while (mycount);
}
void BufferDrowVexHTP(G_Particle * G, int offx, int offy, int offh, SDL_Surface * screen, float globxh, float globyh, float glob, SDL_Surface* source)
{
	offh/=2;
if ((offx-G->radius>=screen->w) || (offx+G->radius<=0) || (offy-G->radius>=screen->h) || (offy+G->radius<=0)) return;
if ((offx+G->radius>screen->w) || (offx-G->radius<0) || (offy+G->radius>screen->h) || (offy-G->radius<0))
	{
		//BufferDrowVexSafe( G, offx, offy, offh, screen, globxh, globyh, glob, source);
		return;
	}

globxh=globxh+Vox3Deff*(offx-scrwid);

globyh=globyh-Vox3Deff*(offy-scrhid);

QuickAnglesToCosSin(globxh, globyh, glob);

// int xy, xh, yx, yh, hx, hy;
// Uint32* __restrict screenPix;
Gpixel * __restrict Gp=G->gpixels;
Uint32* __restrict scrpx=(Uint32*)screen->pixels;
Uint32* __restrict imgpx=(Uint32*)source->pixels;
int mycount=G->count;
if (mycount>0)
do
{
 int qx=(char)(Gp->xyhPI & 0xFF);
	int qy=(char)((Gp->xyhPI>>8) & 0xFF);
	int qh=(char)((Gp->xyhPI>>16) & 0xFF); 

int hx = cz*qx - sz*qy;
int hy = sz*qx + cz*qy;	

int xy = cx*hy - (sx*qh<<8);
xy>>=16;
xy+=offy;

int xh = sx*hy + (cx*qh<<8);

int yh = cy*xh - (sy*hx<<8);
yh=128-(yh>>24)-offh;

int yx = sy*xh + (cy*hx<<8);
yx>>=24;
yx+=offx;


int off=(xy)*scrwi+yx;
//*(darkmask+off+1)=0;
Uint32* __restrict screenPix=scrpx+off;
//register int col= *(imgpx+gp->TextureImagePixel);
//if ((*screenPix>>24>=yh)) //&& (from>>24==0xFF))
//if (gp->TextureImagePixel>source->w*source->h)
	//yh=0;


	//	*screenPix= *(imgpx+Gp->TextureImagePixel) & 0x00FFFFFF | (yh<<24);
register int col=*(imgpx+Gp->TextureImagePixel);
if (col)
{
register int sp=*screenPix;
*screenPix=((((sp & RedMask)>>1) + ((col & RedMask)>>1))& RedMask) | ((((sp & GreenMask)>>1) + ((col & GreenMask)>>1))& GreenMask) | ((((sp & BlueMask)>>1) + ((col & BlueMask)>>1)));	
}

Gp++;
mycount--;
}
while (mycount);
}

// For guts:
void MergeVexelSafe(G_Particle * G, int offx, int offy, int offh, SDL_Surface * screen, float globxh, float globyh, float glob)
{
	//offh/=2;

QuickAnglesToCosSin(globxh, globyh, glob);

 int xy, xh, yx, yh, hx, hy;
 Uint32* __restrict screenPix;
Gpixel * __restrict Gp=G->gpixels;
Uint32* __restrict scrpx=(Uint32*)screen->pixels;
int mycount=G->count;
if (mycount>0)
do
{
int qx=(char)(Gp->xyhPI & 0xFF);
	int qy=(char)((Gp->xyhPI>>8) & 0xFF);
	int qh=(char)((Gp->xyhPI>>16) & 0xFF); 

hx = cz*qx - sz*qy;
hy = sz*qx + cz*qy;	

xy = cx*hy - (sx*qh<<8);
xy>>=16;
xy+=offy;

xh = sx*hy + (cx*qh<<8);

yh = cy*xh - (sy*hx<<8);
yh=128-(yh>>24)-offh;

yx = sy*xh + (cy*hx<<8);
yx>>=24;
yx+=offx;

if ((xy<screen->h) && (yx<screen->w) && (xy>=0) && (yx>=0))
{
screenPix=scrpx+xy*screen->w+yx;
//int diff=(*screenPix>>24)-yh;
if (*screenPix>>24<247)
{
Uint32 diff=*screenPix & 0xFF000000;
if ((diff>>24)>30)
*screenPix=(Gp->color & 0x00FFFFFF) | (diff-0x01000000);
}
}

Gp++;
mycount--;
}
while (mycount);
}
void MergeVexel(G_Particle * G, int offx, int offy, int offh, SDL_Surface * screen, float globxh, float globyh, float glob)
{
	//offh/=2;
if ((offx-G->radius>=screen->w) || (offx+G->radius<=0) || (offy-G->radius>=screen->h) || (offy+G->radius<=0)) return;
if ((offx+G->radius>screen->w) || (offx-G->radius<0) || (offy+G->radius>screen->h) || (offy-G->radius<0))
	{
		MergeVexelSafe( G, offx, offy, offh, screen, globxh, globyh, glob);
		return;
	}

QuickAnglesToCosSin(globxh, globyh, glob);

 int xy, xh, yx, yh, hx, hy;
Uint32* __restrict screenPix;
Gpixel * __restrict Gp=G->gpixels;
Uint32* __restrict scrpx=(Uint32*)screen->pixels;
int mycount=G->count;
if (mycount>0)
do
{
 int qx=(char)(Gp->xyhPI & 0xFF);
	int qy=(char)((Gp->xyhPI>>8) & 0xFF);
	int qh=(char)((Gp->xyhPI>>16) & 0xFF);
hx = cz*qx - sz*qy;
hy = sz*qx + cz*qy;	

xy = cx*hy - (sx*qh<<8);
xy>>=16;
xy+=offy;

xh = sx*hy + (cx*qh<<8);

yh = cy*xh - (sy*hx<<8);
yh=128-(yh>>24)-offh;

yx = sy*xh + (cy*hx<<8);
yx>>=24;
yx+=offx;
screenPix=scrpx+xy*screen->w+yx;
Uint32 diff=*screenPix & 0xFF000000;
if ((diff>>24)<240)
*screenPix=(Gp->color & 0x00FFFFFF) | (diff-0x01000000);
Gp++;
mycount--;
}
while (mycount);
}
// For Background
void MergeVexelUpperSafe(G_Particle * G, int offx, int offy, int offh, SDL_Surface * screen, float globxh, float globyh, float glob, UpShadLayer* uppermap, SDL_Surface* upimage)
{


QuickAnglesToCosSin(globxh, globyh, glob);

 int xy, xh, yx, yh, hx, hy;
Uint32* __restrict screenPix;
Gpixel * __restrict Gp=G->gpixels;
Uint32* __restrict scrpx=(Uint32*)screen->pixels;
Uint8* __restrict updpx=&uppermap->pix[0];
Uint32* __restrict uppic=(Uint32*)upimage->pixels;
int mycount=G->count;
if (mycount>0)
do
{
 int qx=(char)(Gp->xyhPI & 0xFF);
	int qy=(char)((Gp->xyhPI>>8) & 0xFF);
	int qh=(char)((Gp->xyhPI>>16) & 0xFF);
hx = cz*qx - sz*qy;
hy = sz*qx + cz*qy;	

xy = cx*hy - (sx*qh<<8);
xy>>=16;
xy+=offy;

xh = sx*hy + (cx*qh<<8);

yh = cy*xh - (sy*hx<<8);
yh=128-(yh>>24)-offh;

yx = sy*xh + (cy*hx<<8);
yx>>=24;
yx+=offx;
//int smooth=2; 
screenPix=scrpx+xy*screen->w+yx;
//upperPix=updpx+xy*screen->w+yx;

if ((xy<screen->h) && (yx<screen->w) && (xy>=0) && (yx>=0))
{
int diff=(*screenPix>>24)-yh;

if (diff>=0)
	if ((diff>=8))
{
*screenPix=Gp->color & 0x00FFFFFF | (yh<<24);
*(updpx+xy*screen->w+yx)=*(uppic+Gp->TextureImagePixel)>>24;
	}
	else
	{
int AH=diff*32;
register int idst = *screenPix;
register int isrc = Gp->color;
*screenPix = ((RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * AH) >>8))) | (GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * AH) >>8))) | (BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * AH) >>8)))) | (idst & 0xFF000000);
*(updpx+xy*screen->w+yx)=*(uppic+Gp->TextureImagePixel)>>24;
}
		

}




Gp++;
mycount--;
}
while (mycount);
}
void MergeVexelUpper(G_Particle * G, int offx, int offy, int offh, SDL_Surface * screen, float globxh, float globyh, float glob, UpShadLayer* uppermap, SDL_Surface* upimage)
{
	//offh/=2;
if ((offx-G->radius>=screen->w) || (offx+G->radius<=0) || (offy-G->radius>=screen->h) || (offy+G->radius<=0)) return;
if ((offx+G->radius+4>screen->w) || (offx-G->radius<0) || (offy+G->radius+4>screen->h) || (offy-G->radius<0))
	{
		MergeVexelUpperSafe( G, offx, offy, offh, screen, globxh, globyh, glob,uppermap,upimage);
		return;
	}

//QuickAnglesToCosSin(globxh, globyh, glob);
FloatAnglesToCosSin(globxh, globyh, glob, &NtoN);

 float xy, xh, yx, yh, hx, hy;
Uint32* screenPix;
Gpixel * __restrict Gp=G->gpixels;
Uint32* scrpx=(Uint32*)screen->pixels;
Uint8*  updpx=&uppermap->pix[0];
Uint32* __restrict uppic=(Uint32*)upimage->pixels;
int mycount=G->count;
if (mycount>0)
do
{
 float qx=(char)(Gp->xyhPI & 0xFF);
	float qy=(char)((Gp->xyhPI>>8) & 0xFF);
	float qh=(char)((Gp->xyhPI>>16) & 0xFF);

hx = NtoN.fcz*qx - NtoN.fsz*qy;
hy = NtoN.fsz*qx + NtoN.fcz*qy;	

xy = NtoN.fcx*hy - (NtoN.fsx*qh);
//xy>>=16;
xy+=offy;

xh = NtoN.fsx*hy + (NtoN.fcx*qh);

yh = NtoN.fcy*xh - (NtoN.fsy*hx);
yh=128-(yh)-offh;

yx = NtoN.fsy*xh + (NtoN.fcy*hx);
//yx>>=24;
yx+=offx;
int smooth=4; 
screenPix=scrpx+(int)xy*screen->w+(int)yx;
//upperPix=updpx+xy*screen->w+yx;

while (smooth)
{
	smooth--;
int diff=(int)(*screenPix>>24)-yh;

if (diff>=0)
	if ((diff>=16))
{
//*screenPix=gp->color & 0x00FFFFFF | (yh<<24);

register int col=Gp->color;
	register int sp=*screenPix;
*screenPix=((((sp & RedMask)>>1) + ((col & RedMask)>>1))& RedMask) | ((((sp & GreenMask)>>1) + ((col & GreenMask)>>1))& GreenMask) | ((((sp & BlueMask)>>1) + ((col & BlueMask)>>1))) | ((int)yh<<24);	
*(updpx+(int)xy*screen->w+(int)yx)=*(uppic+Gp->TextureImagePixel)>>24;
	
	}
	else
	{
int AH=diff*16;
register int idst = *screenPix;
register int isrc = Gp->color;
*screenPix = ((RedMask & ((idst & RedMask) + ((int)(((int)(isrc & RedMask) - (int)(idst & RedMask)) * AH) >>8))) | (GreenMask & ((idst & GreenMask) + ((int)(((int)(isrc & GreenMask) - (int)(idst & GreenMask)) * AH) >>8))) | (BlueMask & ((idst & BlueMask) + ((int)(((int)(isrc & BlueMask) - (int)(idst & BlueMask)) * AH) >>8)))) | (idst & 0xFF000000);
*(updpx+(int)xy*screen->w+(int)yx)=*(uppic+Gp->TextureImagePixel)>>24;

}
	xy++;	
screenPix+=screen->w;

}




Gp++;
mycount--;
}
while (mycount);
}

// #################################################  SuperFastTest

void SuperFastVoxelHTP(G_Particle * __restrict G, int offx, int offy, int offh, SDL_Surface * screen, double globxh, double globyh, double glob)
{
if ((offx-G->radius>=screen->w) || (offx+G->radius<=0) || (offy-G->radius>=screen->h) || (offy+G->radius<=0)) return;
if ((offx+G->radius>screen->w) || (offx-G->radius<0) || (offy+G->radius>screen->h) || (offy-G->radius<0))
	{
		//MergeGParticleSafe( G, offx, offy, globxh, globyh, gx, gy);
		return;
	}

double locxh=globxh+0.001*(offx-scrwid);

QuickAnglesToCosSin(globxh, globyh, glob);
SuperFastNtoN();




offh=128-offh;
Gpixel * __restrict Gp=G->gpixels;
Uint32* __restrict srcPix=(Uint32*)screen->pixels+screen->w*offy+offx;
int lscrw=screen->w;

int mycount=G->count-1;
if (mycount>0)
do
{
int qh=(char)((Gp->xyhPI>>16) & 0xFF);
int hforxy=((qh*htoy)>>16)*lscrw+((qh*htox)>>16);
//int hforh=(qh*htoh);
	do
	{
	int qy=(char)((Gp->xyhPI>>8) & 0xFF);
	Uint32* yforxy=hforxy+((qy*ytoy)>>16)*lscrw+((qy*ytox)>>16)+srcPix;
//	int yforh=hforh+(qy*ytoh);
		do
		{
	//	int h=offh-((yforh+(qx*xtoh))>>16);

		int qx=(char)(Gp->xyhPI & 0xFF);

		//Uint32* scrp=srcPix+yforxy+((qx*xtoy)>>16)*lscrw+((qx*xtox)>>16); 

		//if ((int)(*scrp>>24)>=h)
		//{
		//*scrp=Gp->color; 
		//scrp++;
		//*scrp=Gp->color;
		Uint32* point=(yforxy+((qx*xtoy)>>16)*lscrw+((qx*xtox)>>16));
		//*point=
register int col=Gp->color;
register int sp=*point;
*point=((((sp & RedMask)>>1) + ((col & RedMask)>>1))& RedMask) | ((((sp & GreenMask)>>1) + ((col & GreenMask)>>1))& GreenMask) | ((((sp & BlueMask)>>1) + ((col & BlueMask)>>1)));	

//*(point+1)=*point=Gp->color | 0xFF000000;
/*point--;
register int col=Gp->color;
register int sp=*point;
*point=((((sp & RedMask)>>1) + ((col & RedMask)>>1))& RedMask) | ((((sp & GreenMask)>>1) + ((col & GreenMask)>>1))& GreenMask) | ((((sp & BlueMask)>>1) + ((col & BlueMask)>>1)));	

point+=2;
sp=*point;
*point=((((sp & RedMask)>>1) + ((col & RedMask)>>1))& RedMask) | ((((sp & GreenMask)>>1) + ((col & GreenMask)>>1))& GreenMask) | ((((sp & BlueMask)>>1) + ((col & BlueMask)>>1)));	
*/
		Gp++;
		mycount--;
		}
		while (!(Gp->xyhPI & 0x01000000));
	}
	while (!(Gp->xyhPI & 0x02000000));
}
while (mycount);
}
void SuperFastVoxel(G_Particle * __restrict G, int offx, int offy, int offh, SDL_Surface * screen, double globxh, double globyh, double glob)
{
if ((offx-G->radius>=screen->w) || (offx+G->radius<=0) || (offy-G->radius>=screen->h) || (offy+G->radius<=0)) return;
if ((offx+G->radius>screen->w) || (offx-G->radius<0) || (offy+G->radius>screen->h) || (offy-G->radius<0))
	{
		//MergeGParticleSafe( G, offx, offy, globxh, globyh, gx, gy);
		return;
	}

double locxh=globxh+0.001*(offx-scrwid);

QuickAnglesToCosSin(globxh, globyh, glob);
SuperFastNtoN();




offh=128-offh;
Gpixel * __restrict Gp=G->gpixels;
Uint32* __restrict srcPix=(Uint32*)screen->pixels+screen->w*offy+offx;
int lscrw=screen->w;

int mycount=G->count-1;
if (mycount>0)
do
{
int qh=(char)((Gp->xyhPI>>16) & 0xFF);
int hforxy=((qh*htoy)>>16)*lscrw+((qh*htox)>>16);
//int hforh=(qh*htoh);
	do
	{
	int qy=(char)((Gp->xyhPI>>8) & 0xFF);
	Uint32* yforxy=hforxy+((qy*ytoy)>>16)*lscrw+((qy*ytox)>>16)+srcPix;
//	int yforh=hforh+(qy*ytoh);
		do
		{
	//	int h=offh-((yforh+(qx*xtoh))>>16);

		int qx=(char)(Gp->xyhPI & 0xFF);

		//Uint32* scrp=srcPix+yforxy+((qx*xtoy)>>16)*lscrw+((qx*xtox)>>16); 

		//if ((int)(*scrp>>24)>=h)
		//{
		//*scrp=Gp->color; 
		//scrp++;
		//*scrp=Gp->color;
		Uint32* point=(yforxy+((qx*xtoy)>>16)*lscrw+((qx*xtox)>>16));
		//*point=
*(point+1)=*point=Gp->color  & 0x00FFFFFF;
/*point--;
register int col=Gp->color;
register int sp=*point;
*point=((((sp & RedMask)>>1) + ((col & RedMask)>>1))& RedMask) | ((((sp & GreenMask)>>1) + ((col & GreenMask)>>1))& GreenMask) | ((((sp & BlueMask)>>1) + ((col & BlueMask)>>1)));	

point+=2;
sp=*point;
*point=((((sp & RedMask)>>1) + ((col & RedMask)>>1))& RedMask) | ((((sp & GreenMask)>>1) + ((col & GreenMask)>>1))& GreenMask) | ((((sp & BlueMask)>>1) + ((col & BlueMask)>>1)));	
*/
		Gp++;
		mycount--;
		}
		while (!(Gp->xyhPI & 0x01000000));
	}
	while (!(Gp->xyhPI & 0x02000000));
}
while (mycount);
}
void SuperFastVoxel360(G_Particle * __restrict G, int offx, int offy, int offh, SDL_Surface * screen, double globxh, double globyh, double glob)
{
if ((offx-G->radius>=screen->w) || (offx+G->radius<=0) || (offy-G->radius>=screen->h) || (offy+G->radius<=0)) return;
if ((offx+G->radius>screen->w) || (offx-G->radius<0) || (offy+G->radius>screen->h) || (offy-G->radius<0))
	{
		//MergeGParticleSafe( G, offx, offy, globxh, globyh, gx, gy);
		return;
	}

double locxh=globxh+0.001*(offx-scrwid);

QuickAnglesToCosSin(globxh, globyh, glob);
SuperFastNtoN();




offh=255-offh;
Gpixel * __restrict Gp=G->gpixels;
Uint32* __restrict srcPix=(Uint32*)screen->pixels+screen->w*offy+offx;
int lscrw=screen->w;

int mycount=G->count-1;
if (mycount>0)
do
{
int qh=(char)((Gp->xyhPI>>16) & 0xFF);
int hforxy=((qh*htoy)>>16)*lscrw+((qh*htox)>>16);
int hforh=(qh*htoh);
	do
	{
	int qy=(char)((Gp->xyhPI>>8) & 0xFF);
	Uint32* yforxy=hforxy+((qy*ytoy)>>16)*lscrw+((qy*ytox)>>16)+srcPix;
	int yforh=hforh+(qy*ytoh);
		do
		{
		int h=offh-((yforh+(qx*xtoh))>>16);

		int qx=(char)(Gp->xyhPI & 0xFF);

		Uint32* scrp=yforxy+((qx*xtoy)>>16)*lscrw+((qx*xtox)>>16);

		if ((int)(*scrp>>24)>=h)
		{

*scrp=Gp->color & 0x00FFFFFF | (h<<24);
//*(darkmask+off+1)=0;
		//*scrp=Gp->color; 
		//scrp++;
		//*scrp=Gp->color;
		//Uint32* point=(yforxy+((qx*xtoy)>>16)*lscrw+((qx*xtox)>>16));
		//*point=*(point+1)=Gp->color;
		}
		Gp++;
		mycount--;
		}
		while (!(Gp->xyhPI & 0x01000000));
	}
	while (!(Gp->xyhPI & 0x02000000));
}
while (mycount);
}
void SuperFastVoxelToBrick(G_Particle * __restrict G, int offx, int offy, int offh, SDL_Surface * screen, double globxh, double globyh, double glob, float resize, SDL_Surface* palit)
{
//float tempradius=G->radius*resize;
QuickAnglesToCosSin(globxh, globyh, glob);
SuperFastNtoNhLast(); //SuperFastNtoN();

SDL_FillRect(screen,NULL,0);

int depth[160][160];
for (int i=0; i<160; i++)
	for (int j=0; j<160; j++)
		depth[i][j]=0;
//offh=128-offh;
Gpixel * __restrict Gp=G->gpixels;
Uint32* __restrict srcPix=(Uint32*)screen->pixels+screen->w*offy+offx;
int lscrw=screen->w;

int mycount=G->count-1;
if (mycount>0)
do
{
int qh=((char)((Gp->xyhPI>>16) & 0xFF))*resize;
int hforxy=((qh*htoy)>>16)*lscrw+((qh*htox)>>16);
int hforh=(qh*htoh);
	do
	{
	int qy=((char)((Gp->xyhPI>>8) & 0xFF))*resize;
	int yx=hforxy+((qy*ytoy)>>16)*lscrw+((qy*ytox)>>16);
	Uint32* yforxy=yx+srcPix;
	int yforh=hforh+(qy*ytoh);
		do
		{
		int h=128+((yforh+(qx*xtoh))>>16);

		int qx=((char)(Gp->xyhPI & 0xFF))*resize;

		//Uint32* scrp=srcPix+yforxy+((qx*xtoy)>>16)*lscrw+((qx*xtox)>>16); 

		//if ((int)(*scrp>>24)>=h)
		//{
		//*scrp=Gp->color; 
		//scrp++;
		//*scrp=Gp->color;
		int x=((qx*xtoy)>>16)*lscrw+((qx*xtox)>>16);
		Uint32* point=(yforxy+x);
		Uint32 Pcolor=*((Uint32*)palit->pixels+Gp->TextureImagePixel); //& (0x00FFFFFF | Gp->color);

//if (Pcolor)
//{
//int* todepth=((int*)depth+yx+x+80*160);
//if (h>*todepth)
if ((Gp->color & 0x80000000))
{
*(point+1)=*point=Pcolor; 
//*((int*)depth+yx+x+80*160+80)=h;
}
else
{
register int col=Pcolor;
register int sp=*point;
*point=((((sp & RedMask) + (col & RedMask))>>1) & RedMask) | ((((sp & GreenMask) + (col & GreenMask))>>1)& GreenMask) | (((sp & BlueMask) + (col & BlueMask))>>1) | ((*point & 0xFF000000) | 0xC0000000);//0x88000000);

point++;
sp=*point;
*point=((((sp & RedMask) + (col & RedMask))>>1) & RedMask) | ((((sp & GreenMask) + (col & GreenMask))>>1)& GreenMask) | (((sp & BlueMask) + (col & BlueMask))>>1) | ((*point & 0xFF000000) | 0xC0000000);//0x88000000);

}
//}
/*point--;
register int col=Gp->color;
register int sp=*point;
*point=((((sp & RedMask)>>1) + ((col & RedMask)>>1))& RedMask) | ((((sp & GreenMask)>>1) + ((col & GreenMask)>>1))& GreenMask) | ((((sp & BlueMask)>>1) + ((col & BlueMask)>>1)));	

point+=2;
sp=*point;
*point=((((sp & RedMask)>>1) + ((col & RedMask)>>1))& RedMask) | ((((sp & GreenMask)>>1) + ((col & GreenMask)>>1))& GreenMask) | ((((sp & BlueMask)>>1) + ((col & BlueMask)>>1)));	
*/
		Gp++;
		mycount--;
		}
		while (!(Gp->xyhPI & 0x01000000));
	}
	while (!(Gp->xyhPI & 0x02000000));
}
while (mycount);

/*
Uint32* point=(Uint32*)screen->pixels;
for (int i=0; i<160; i++)
{
	int previous=0;
	for (int j=0; j<160; j++)
	{
		int diff=previous-depth[i][j];
		if ((diff>0) && (depth[i][j]))
		{
diff=max(255-diff*30, 128);
register int sp=*point;
*point=((((sp & RedMask)*diff)>>8) & RedMask) | ((((sp & GreenMask)*diff)>>8) & GreenMask) 
	| (((sp & BlueMask)*diff)>>8) | (sp & 0xFF000000);
		}
		else
		previous=depth[i][j];

		previous--;
		point++;
	}
}*/
}
int SuperFastVoxelGetImpactCoords(G_Particle * __restrict G, int offx, int offy, int offh, SDL_Surface * screen, double globxh, double globyh, double glob, float resize, Uint32 x, Uint32 y, SDL_Surface* guts)
{
	int impact=0;
//float tempradius=G->radius*resize;
QuickAnglesToCosSin(globxh, globyh, glob);
SuperFastNtoNhLast();

offh=128-offh;
Gpixel * __restrict Gp=G->gpixels;
Uint32 srcPix=screen->w*offy+offx;
int lscrw=screen->w;

int mycount=G->count-1;
if (mycount>0)
do
{
int qh=((char)((Gp->xyhPI>>16) & 0xFF))*resize;
int hforxy=((qh*htoy)>>16)*lscrw+((qh*htox)>>16);
//int hforh=(qh*htoh);
	do
	{
	int qy=((char)((Gp->xyhPI>>8) & 0xFF))*resize;
	int yforxy=hforxy+((qy*ytoy)>>16)*lscrw+((qy*ytox)>>16)+srcPix;
//	int yforh=hforh+(qy*ytoh);
		do
		{
	//	int h=offh-((yforh+(qx*xtoh))>>16);

		int qx=((char)(Gp->xyhPI & 0xFF))*resize;

	
		//Uint32* point=(yforxy+((qx*xtoy)>>16)*lscrw+((qx*xtox)>>16));
		if (((yforxy+((qx*xtoy)>>16)*lscrw+((qx*xtox)>>16))==y*lscrw+x) && (*((Uint32*)guts->pixels + Gp->TextureImagePixel)))
		//if ((yforxy+((qx*xtoy)>>16)*lscrw) && (((qx*xtox)>>16)==x))
		{
			impact=Gp->TextureImagePixel;
		}



		Gp++;
		mycount--;
		}
		while (!(Gp->xyhPI & 0x01000000));
	}
	while (!(Gp->xyhPI & 0x02000000));
}
while (mycount);
return impact;
}

//################################################# DrowWhole Creation
void FastDrowCreation (int offx, int offy, double globxh, double globyh, double glob)
{
double sx = sin(globyh);
double cx = cos(globyh);
double sy = sin(globxh);
double cy = cos(globxh);
double sz = sin(glob);
double cz = cos(glob);

double xy, xh, yx, yh, hx, hy;

 for (int no=0; no<VoxelsCount; no++)
	 if ((vox[no].active) && (vox[no].G_p.gpixels))
	{
	hx = cz*vox[no].x - sz*vox[no].y;
		hy = sz*vox[no].x + cz*vox[no].y;	
		xy = cx*hy - sx*vox[no].h;
		xh = sx*hy + cx*vox[no].h;
		yh = cy*xh - sy*hx;
		yx = sy*xh + cy*hx;

yx+=offx;
xy+=offy;

//DrowVex(&vox[no].G_p, yx, xy, yh , screen, globxh, globyh, glob);

	}
}



//####################################################### Testing Stuff
void VectorTest(int x, int y, int h, Uint32 color, bool halfTp, int offx, int offy, double globxh, double globyh, double glob)
{


// For every object every frame

 double sx = sin(globyh);
 double cx = cos(globyh);
 double sy = sin(globxh);
 double cy = cos(globxh);

int xh, xtox, ytox, htox, ytoy, htoy, xtoh, ytoh, htoh;

xtoh = - sy*accuracy;
xtox = cy*accuracy;

htoy = -(xh = sx*accuracy);
ytoy = cx*accuracy;
ytoh = cy*xh;
ytox = sy*xh;

htoh = cy*ytoy;
htox = sy*ytoy;

// For Every Pixel:


x=(h*htox+y*ytox+x*xtox)>>16;
y=(h*htoy+y*ytoy)>>16;
h=(h*htoh+y*ytoh+x*xtoh)>>16;

drowBigPixel (x+offx,y+offy,4, color, h);

//this one creates bug

//+300; +150
// Make render check for if object can cross out of borders, if no - no need to check per pixel;
// USE RENDERING FORMULA *((Uint32*)(screen->pixels+screen->W*y*sin... + x )) = color;
//drowBigPixel (600+katet*cos(xx+glob*y),300+katet*sin(yy+glob*x),4, color, halfTp );

}
void NOT_VectorTest(int x, int y, int h, Uint32 color, bool halfTp, int offx, int offy, double globxh, double globyh, double glob, int size)
{
/*fuck=x*x+y*y;
fuck=sqrt(fuck);	
double kyt=acos(x/fuck);
if (y<0) kyt=2*M_PI-kyt;
y=0;
x=fuck;
*/
double	kyt=0;


	//glob=0;//M_PI/2;
 double sz = sin(kyt);
 double cz = cos(kyt);
 double sx = sin(globyh);
 double cx = cos(globyh);
 double sy = sin(globxh);
 double cy = cos(globxh);

	double xh, hx, hy;

	/*	hx = cz*x - sz*y;
		hy = sz*x +	cz*y;
		xy = cx*hy - sx*h;
		xh = sx*hy + cx*h;
		yh = cy*xh - sy*hx;
		yx = sy*xh + cy*hx;  */
// Convert all 3 coordinates to all 3 coordinates:
double xtox, ytox, htox;
double xtoy, ytoy, htoy;
double xtoh, ytoh, htoh;

/*double tx=0;
double ty=accuracy;
double th=0;

hx = cz*tx - sz*ty;
hy = sz*tx + cz*ty;
ytoy = cx*hy - sx*th;
xh = sx*hy + cx*th;
ytoh = cy*xh - sy*hx;
ytox = sy*xh + cy*hx;

tx=accuracy;
ty=0;
th=0;

hx = cz*tx - sz*ty;
hy = sz*tx + cz*ty;
xtoy = cx*hy - sx*th;
xh = sx*hy + cx*th;
xtoh = cy*xh - sy*hx;
xtox = sy*xh + cy*hx;

tx=0;
ty=0;
th=accuracy;

hx = cz*tx - sz*ty;
hy = sz*tx + cz*ty;
htoy = cx*hy - sx*th;
xh = sx*hy + cx*th;
htoh = cy*xh - sy*hx;
htox = sy*xh + cy*hx;
*/


/*hx = cz*tx - sz*ty;
hy = sz*tx + cz*ty;
xy = cx*hy - sx*th;
xh = sx*hy + cx*th;
yh = cy*xh - sy*hx;
yx = sy*xh + cy*hx;*/

hx = cz*accuracy;
hy = sz*accuracy;
xtoy = cx*hy;
xh = sx*hy;
xtoh = cy*xh - sy*hx;
xtox = sy*xh + cy*hx;

hx = - sz*accuracy;
hy = cz*accuracy;
ytoy = cx*hy;
xh = sx*hy;
ytoh = cy*xh - sy*hx;
ytox = sy*xh + cy*hx;

htoy = - sx*accuracy;
xh = + cx*accuracy;
htoh = cy*xh;
htox = sy*xh;

sz = sin(glob+kyt);
cz = cos(glob+kyt);

x=(h*htox+y*ytox+x*xtox)/accuracy;
y=(h*htoy+y*ytoy+x*xtoy)/accuracy;
h=(h*htoh+y*ytoh+x*xtoh)/accuracy;

drowBigPixel (x+offx,y+offy,size, color, h);

//+300; +150
// Make render check for if object can cross out of borders, if no - no need to check per pixel;
// USE RENDERING FORMULA *((Uint32*)(screen->pixels+screen->W*y*sin... + x )) = color;
//drowBigPixel (600+katet*cos(xx+glob*y),300+katet*sin(yy+glob*x),4, color, halfTp );

}
void VectorRotPerPixelWorking(float x, float y, float h, Uint32 color, int offx, int offy, float globxh, float globyh, float glob, int size, void (*PixManipilation) (int ,int ,int , int, int))
{
// Pre-calculations
fuck=x*x+y*y;
fuck=sqrt(fuck);	
float kyt=acos(x/fuck);
if (y<0) kyt=2*M_PI-kyt;
y=0;
x=fuck;

// real-time calculations

 float sz = sin(glob+kyt);
 float cz = cos(glob+kyt);
 float sx = sin(globyh);
 float cx = cos(globyh);
 float sy = sin(globxh);
 float cy = cos(globxh);

	 float xy, xh, yx, yh, hx, hy;

		hx = cz*x;
		hy = sz*x;	
		xy = cx*hy - sx*h;
		xh = sx*hy + cx*h;
		yh = cy*xh - sy*hx;
		yx = sy*xh + cy*hx;
		
PixManipilation (yx+offx,xy+offy,size, color, yh);

//+300; +150
// Make render check for if object can cross out of borders, if no - no need to check per pixel;
// USE RENDERING FORMULA *((Uint32*)(screen->pixels+screen->W*y*sin... + x )) = color;
//drowBigPixel (600+katet*cos(xx+glob*y),300+katet*sin(yy+glob*x),4, color, halfTp );

}


void DrovWholeCreation(void (*DrowFunction) (int ,int ,int , int, int))//double x, double y, double h, Uint32 no, int offx, int offy)
{
//scrwid+scrwid/4, scrhid/2
int offx=scrwid+scrwid/4;
int offy=scrhid/2;

double sx = sin(globyh);
 double cx = cos(globyh);
 double sy = sin(globxh);
 double cy = cos(globxh);
  double sz = sin(glob);
 double cz = cos(glob);

	 double xy, xh, yx, yh, hx, hy;

//for (int no=0; no<VoxelsCount; no++)
	 int no=curVoxel;
	if (vox[no].active)
	{
		int x=vox[no].x;
		int y=vox[no].y;
		int h=vox[no].h;


 

		hx = cz*x - sz*y;
		hy = sz*x + cz*y;	
		xy = cx*hy - sx*h;
		xh = sx*hy + cx*h;
		yh = cy*xh - sy*hx;
		yx = sy*xh + cy*hx;

yx+=offx;
xy+=offy;
//drowBigPixel (yx+offx,xy+offy,10, color, halfTp, yh);
for (int l=0; l<vox[no].size; l++)
for (int i=0; i<vox[no].size; i++)
for (int j=0; j<vox[no].size; j++)
	if (vox[no].VoxelMatrix[i][j][l])
	{
		palette.currentPixel=&vox[no].VoxelMatrix[i][j][l];
		palette.currentPixelTP=&vox[no].VoxelMatrixTP[i][j][l];
		palette.currentG8pixel=&vox[no].eightColMatr[i][j][l];
		VectorRotPerPixelWorking (i-vox[no].size/2, j-vox[no].size/2, l-vox[no].size/2, vox[no].VoxelMatrix[i][j][l], yx, xy, vox[no].ax+globxh, vox[no].ay+globyh, vox[no].ah+glob, 1,DrowFunction);
	}

if (no==curVoxel)
	DrowFrame (yx-vSize/2, xy-vSize/2, vSize, vSize, 0xFFFFFF);
	//+300; +150
// Make render check for if object can cross out of borders, if no - no need to check per pixel;
// USE RENDERING FORMULA *((Uint32*)(screen->pixels+screen->W*y*sin... + x )) = color;
//drowBigPixel (600+katet*cos(xx+glob*y),300+katet*sin(yy+glob*x),4, color, halfTp );
	}
}

void SaveCreation ()
{
std::stringstream sss;
    sss<<"Voxels/"<<curCreation<<"/VoxelStructure.lr";
    std::string strx2;
    sss>>strx2;  
std::ofstream map(strx2);

for (int i=0; i<VoxelsCount; i++)
{
	map<<vox[i].x;
	map<<" ";
	map<<vox[i].y;
	map<<" ";
	map<<vox[i].h;
	map<<" ";
	map<<vox[i].active;
	map<<" ";
}
/*
struct voxelSegment
{
int x,y,h;
bool active;
double ah, ax, ay;
} 
vox[VoxelsCount];*/
map.close();
}
void LoadCreation ()
{
std::stringstream sss;
    sss<<"Voxels/"<<curCreation<<"/VoxelStructure.lr";
    std::string strx2;
    sss>>strx2;  
//std::ofstream map(strx2);
	std::ifstream map( strx2 );

for (int i=0; i<VoxelsCount; i++)
{
	map>>vox[i].x;
	//map<<" ";
	map>>vox[i].y;
	//map<<" ";
	map>>vox[i].h;
	//map<<" ";
	map>>vox[i].active;
	//map<<" ";
}
/*
struct voxelSegment
{
int x,y,h;
bool active;
double ah, ax, ay;
} 
vox[VoxelsCount];*/
map.close();
}

void loadPalette(int i)
{
if (palette.image)
	SDL_FreeSurface(palette.image);
if (palette.imageForDirectionalLight)
	SDL_FreeSurface(palette.imageForDirectionalLight);

std::stringstream ss;
std::string strx;
ss<<i;
ss>>strx; 
palette.image=loadany("Voxels/"+strx+"/pal.png");
palette.imageForDirectionalLight=loadany("Voxels/"+strx+"/dl.png");
ss.clear();
strx.clear();
}

void SelectCreation()
{
	bool lquit=false;
	int i=0;
	LoadCreationNames();
	uButt[1].ButtonImage=TextToImage("Create New Voxel Object",0,"");
uButt[1].h=uButt[1].ButtonImage->h;
uButt[1].w=uButt[1].ButtonImage->w;
uButt[1].x=scrwid-uButt[1].w;
uButt[1].y=scrhi-uButt[1].h;
uButt[1].ButtonEffec=&SaveCreationNames;

while (!lquit)
	{
		SDL_FillRect(screen,NULL, 0x000000);
		prodata(CreationsNames[curCreation],curCreation,5);
		//sdlanydrow (uButt[1].x, uButt[1].y, 0, uButt[1].ButtonImage);
		if (Inputtext) SDL_BlitSurface(Inputtext, NULL, screen, NULL);
		
			while( SDL_PollEvent( &event ) ) 
		{
			InputText();
if( event.type == SDL_MOUSEBUTTONDOWN ) 
 {

	 mouse.pos.x=event.button.x;
	 mouse.pos.y=event.button.y;
	  
			if( event.button.button == SDL_BUTTON_LEFT ) 
				if (checkForButtonPress (1)) SaveCreationNames();
}
if( event.type ==  SDL_KEYDOWN ) 
{

	

				if (event.key.keysym.sym == SDLK_RIGHT)
	{
//SaveVoxel(curCreation);
curCreation++;
//LoadVoxel(curVoxel);
	}

	if ((event.key.keysym.sym == SDLK_LEFT) && (curCreation>0))
	{
//SaveVoxel(curVoxel);
curCreation--;
//LoadVoxel(curVoxel);
	}


			if ( event.key.keysym.sym == SDLK_ESCAPE )
				{
					lquit=true;
					JumpToSpace();
			}

			if ( event.key.keysym.sym == SDLK_RETURN )
				{
					lquit=true;
			}




}

	if( event.type == SDL_QUIT )
			{ 
				quit = true;
				lquit=true;
				
			} 

			}
swapscreens();
	}

LoadCreation ();
Vinit();
LoadVoxel(curVoxel);
mouse.state=0;
}

void BreakColor(int i)
{
	palette.curColor=i;
	palette.R=vox[curVoxel].eightPalette[palette.curColor] & 0x0000FF;
	palette.G=(vox[curVoxel].eightPalette[palette.curColor] & 0x00FF00)>>8;
	palette.B=(vox[curVoxel].eightPalette[palette.curColor] & 0xFF0000)>>16;
}

bool TryBlitPixels()
{
	if ((mouse.pos.x>scrwid) && (mouse.pos.y<scrhid) && (mouse.pos.x<scrwid*1.5) && (palette.toolInHand==3))
{
 DrovWholeCreation(&TryCatchPixelData);
 //tempppp++;
 palette.rebuildGpixel=1;
 return 1;
}

	return 0;
}
void GrowPixels(int size, int value, int x, int y, int h, bool htp)
{
int sized=size/2; 
for (int i=0; i<=size; i++)
{
	int mi=x-sized+i;
	if ((mi>=0) && (mi<vox[curVoxel].size))
for (int j=0; j<=size; j++)
{
	int mj=y-sized+j;
	if ((mj>=0) && (mj<vox[curVoxel].size))
for (int l=0; l<=size; l++)
{
	int ml=h-sized+l;
	if ((ml>=0) && (ml<vox[curVoxel].size))
	if (!vox[curVoxel].VoxelMatrix[mi][mj][ml])
	{
		int si,sj,sl;
		si*=si=i-sized;
		sj*=sj=j-sized;
		sl*=sl=l-sized;
			fuck=si+sj+sl;
				fuck=sqrt(fuck);
if (fuck<=sized)
{
		vox[curVoxel].VoxelMatrix[mi][mj][ml]=value;
		vox[curVoxel].eightColMatr[mi][mj][ml]=palette.curColor;
}
	}
}
}
}


}
void ErasePixels(int size, int value, int x, int y, int h)
{
	int sized=size/2;
	//int size=3;
/*for (int i=palette.currentGpixel->x-size+vSized; i<=palette.currentGpixel->x+size+vSized; i++)
	if ((i>=0) && (i<vox[curVoxel].size))
for (int j=palette.currentGpixel->y-size+vSized; j<=palette.currentGpixel->y+size+vSized; j++)
	if ((j>=0) && (j<vox[curVoxel].size))
for (int l=palette.currentGpixel->h-size+vSized; l<=palette.currentGpixel->h+size+vSized; l++)
	if ((l>=0) && (l<vox[curVoxel].size))
	if (!vox[curVoxel].VoxelMatrix[i][j][l])*/ 
for (int i=0; i<=size; i++)
{
	int mi=x-sized+i;
	if ((mi>=0) && (mi<vox[curVoxel].size))
for (int j=0; j<=size; j++)
{
	int mj=y-sized+j;
	if ((mj>=0) && (mj<vox[curVoxel].size))
for (int l=0; l<=size; l++)
{
	int ml=h-sized+l;
	if ((ml>=0) && (ml<vox[curVoxel].size))
	{
		int si,sj,sl;
		si*=si=i-sized;
		sj*=sj=j-sized;
		sl*=sl=l-sized;
			fuck=si+sj+sl;
				fuck=sqrt(fuck);
if (fuck<=sized)
		vox[curVoxel].VoxelMatrix[mi][mj][ml]=0;
	}
}
}
}


}
bool TryDrowVoxels(bool firstClick)
{
if (palette.toolInHand>2) return 0;
//palette.currentH=256;
//palette.currentGpixel=0;
//if (mouse.pos.x<scrwid)
//DrowBigVexFullColor(testG, BVx, BVy, screen, vox[curVoxel].ax, vox[curVoxel].ay, vox[curVoxel].ah, BigVexPixSize, CatchBigGPixel);

if (palette.currentGpixel)
{
int qx=(char)(palette.currentGpixel->xyhPI & 0xFF);
int qy=(char)((palette.currentGpixel->xyhPI>>8) & 0xFF);
int qh=(char)((palette.currentGpixel->xyhPI>>16) & 0xFF);
qx-=(vox[curVoxel].massCenter.x-vSized);
qy-=(vox[curVoxel].massCenter.y-vSized);
qh-=(vox[curVoxel].massCenter.h-vSized);

mouse.state=8;

	if (palette.toolInHand==0)
	{
		vox[curVoxel].VoxelMatrix[qx][qy][qh]=vox[curVoxel].eightPalette[palette.curColor]; //palette.currentGpixel->color=
		vox[curVoxel].eightColMatr[qx][qy][qh]=palette.curColor;
		palette.rebuildGpixel=1;
		return 1;
	}
	else
if (firstClick)
{
GrowPixels(palette.brushSize, vox[curVoxel].eightPalette[palette.curColor], qx, qy, qh,halfransparent);//palette.currentGpixel->color=0x000000;
palette.rebuildGpixel=1;
palette.BrushPlaneH=palette.currentH;
return 1;
}		
}

if ((!firstClick) && (palette.toolInHand==1))
{
	
	int nqx=mouse.pos.x;
	int nqy=mouse.pos.y;
	int nqh=palette.BrushPlaneH;
BigVexGetBrushPlaneCoords(&nqx, &nqy, &nqh, BVx, BVy, vox[curVoxel].ax, vox[curVoxel].ay, vox[curVoxel].ah, BigVexPixSize);

nqx+=(-vox[curVoxel].massCenter.x+vSized);
nqy+=(-vox[curVoxel].massCenter.y+vSized);
nqh+=(-vox[curVoxel].massCenter.h+vSized);

GrowPixels(palette.brushSize, vox[curVoxel].eightPalette[palette.curColor], nqx, nqy, nqh,halfransparent);
palette.rebuildGpixel=1;
return 1;
}



	return palette.currentGpixel;
}
bool TryDeleteVoxels()
{
if (palette.toolInHand!=1) return 0;
//palette.currentH=256;
//palette.currentGpixel=0;
//if (mouse.pos.x<scrwid)
//DrowBigVexFullColor(testG, BVx, BVy, screen, vox[curVoxel].ax, vox[curVoxel].ay, vox[curVoxel].ah, BigVexPixSize, CatchBigGPixel);

if (palette.currentGpixel)
{
    mouse.state=9;

	/*if (palette.toolInHand==0)
	{
	
	vox[curVoxel].VoxelMatrix[palette.currentGpixel->x+vSized][palette.currentGpixel->y+vSized][palette.currentGpixel->h+vSized]=palette.currentGpixel->color=palette.color[palette.curColor];
	}
	else*/
		//if (palette.toolInHand==1)
		//{
int qx=(char)(palette.currentGpixel->xyhPI & 0xFF);
	int qy=(char)((palette.currentGpixel->xyhPI>>8) & 0xFF);
	int qh=(char)((palette.currentGpixel->xyhPI>>16) & 0xFF);

qx-=vox[curVoxel].massCenter.x;
qy-=vox[curVoxel].massCenter.y;
qh-=vox[curVoxel].massCenter.h;

	ErasePixels(palette.brushSize, 0, qx+vSized, qy+vSized, qh+vSized);
		///	GrowPixels(5, 0x000000);//palette.currentGpixel->color=0x000000;
palette.rebuildGpixel=1;
		//}
		//else 
			return 1;
}

	return palette.currentGpixel;
}
bool TryGetColor()
{
	if (palette.toolInHand==0)
	{
/*palette.currentGpixel=0;
if (mouse.pos.x<scrwid)
	if (!bit8Mode)
DrowBigVexFullColor(testG, BVx, BVy, screen, vox[curVoxel].ax, vox[curVoxel].ay, vox[curVoxel].ah, BigVexPixSize, CatchBigGPixel);
	else
		DrowBigVex(palette.currentG8, BVx, BVy, screen, vox[curVoxel].ax, vox[curVoxel].ay, vox[curVoxel].ah, BigVexPixSize, CatchBigGPixel);
*/

if (palette.currentGpixel)
{
	vox[curVoxel].eightPalette[palette.curColor]=palette.currentGpixel->color;
	BreakColor(palette.curColor);
		
	return 1;
}
	}
	return 0;
}
bool TryUseResizer()
{
 if ((mouse.pos.x<scrwid-20) && (mouse.pos.x>scrwid-40) && (mouse.pos.y<20))
	 {
		 palette.toolInHand=1;
		 //palette.palettex=mouse.pos.x;
		 //palette.palettey=palette.image->h-(scrhi-mouse.pos.y);
		
		 return 1;
	 }
	 return 0;
}
bool TryUseBrush()

{
	 if ((mouse.pos.x<scrwid) && (mouse.pos.x>scrwid-20) && (mouse.pos.y<20))
	 {
		 palette.toolInHand=0;
		 //palette.palettex=mouse.pos.x;
		 //palette.palettey=palette.image->h-(scrhi-mouse.pos.y);
		
		 return 1;
	 }
	 return 0;
}
bool TryUseBlitter()

{
	 if ((mouse.pos.x<palette.image->w) && (mouse.pos.y>scrhi-palette.image->h))
	 {
		 palette.toolInHand=3;
		 palette.palettex=mouse.pos.x;
		 palette.palettey=palette.image->h-(scrhi-mouse.pos.y);
		
		 return 1;
	 }
	 return 0;
}
bool TryDrowVoxel (int x, int y, Uint32 color)
{
	int i=(x-10)/pixsize;
	int j=(y-10)/pixsize;
	if ((i>=0) && (j>=0) && (i<vSize) && (j<vSize))
	{
GrowPixels(palette.brushSize, color, i, j, curh, halfransparent);


//		vox[curVoxel].VoxelMatrixA[i][j][curh]=halfransparent;
		vox[curVoxel].VoxelMatrix[i][j][curh]=color;
		vox[curVoxel].eightColMatr[i][j][curh]=palette.curColor;
		mouse.state=3;
		return 1;
	}
	return 0;
}
bool TryDeleteVoxel (int x, int y)
{
	int i=(x-10)/pixsize;
	int j=(y-10)/pixsize;
	if ((i>=0) && (j>=0) && (i<vSize) && (j<vSize))
	{
		ErasePixels(palette.brushSize, 0x000000, i, j, curh);
//		vox[curVoxel].VoxelMatrix[i][j][curh]=0;
		mouse.state=4;
		return 1;
	}
	return 0;
	
}
bool mousecoordsPlus(int disp)
{
		int si,sj;
		si*=si=abs(mouse.pos.x-mouse.downx);
		sj*=sj=abs(mouse.pos.y-mouse.downy);
			fuck=si+sj;
				fuck=sqrt(fuck);
if (fuck>disp) 
{
	mouse.downx=mouse.pos.x;
		mouse.downy=mouse.pos.y;
		return 1;
}
	return 0;
}

void CreationDescEventHandling()
{
if( event.type == SDL_MOUSEBUTTONDOWN ) 
 {

	 mouse.pos.x=mouse.downx=event.button.x;
	 mouse.pos.y=mouse.downy=event.button.y;
	
	
	  if( event.button.button == SDL_BUTTON_WHEELDOWN)
curh++;
	if( event.button.button == SDL_BUTTON_WHEELUP)
curh--;
	 if (curh<0) curh=0;
	else
	if (curh>=vSize) curh=vSize-1;

			
			
if( event.button.button == SDL_BUTTON_RIGHT ) 
	if (!TryDeleteVoxels())
	if (!TryGetColor())
	if (!TryDeleteVoxel (event.button.x, event.button.y))	
	if (!TryMoveObject (mouse.pos.x, mouse.pos.y))
	{
		if (mouse.pos.x>scrwid)
		{
		mouse.state=1;
		sglobxh=globxh; //-(mouse.downx-event.motion.x)*0.01;
		sglobyh=globyh;//+(mouse.downy-event.motion.y)*0.01;
		}
		else
		{
sglobxh=vox[curVoxel].ax; 
sglobyh=vox[curVoxel].ay;
mouse.state=5;
		}

	}
	
		
	if( event.button.button == SDL_BUTTON_LEFT ) 
			{
				if (checkForButtonPress (1)) 
					saveTogle();
				if (checkForButtonPress (2)) { 
					saveTogle(); Inputstr.clear(); }
				if (checkForButtonPress (3)) 
					SaveVoxelNames();


if (!TryDrowVoxels(true))  
if (!TryUseBrush())
if (!TryUseResizer())
if (!TryBlitPixels())
if (!TryUseBlitter())
			 if  (!TryDrowVoxel (event.button.x, event.button.y, vox[curVoxel].eightPalette[palette.curColor]))
			 {
				if (TrySelectObject(mouse.pos.x, mouse.pos.y))
				{
palette.rebuildGpixel=1;
				}
				if (mouse.pos.x<scrwid) 
				{
					mouse.state=6;
					sglobxh=vox[curVoxel].ah; 
					//sglobyh=vox[curVoxel].ay;
				}
				else 
					if ((mouse.pos.x<scrwid*1.5) && (mouse.pos.y<scrhid))
					{
						mouse.state=7;
						sglob=glob;
					}

			 }
	}

			
}

// moving prespective

if( event.type == SDL_MOUSEMOTION ) 
{
	if (mouse.state==1) // 1-rotating creation perspective
	{
		globxh=sglobxh-(mouse.downx-event.motion.x)*0.01;
		globyh=sglobyh+(mouse.downy-event.motion.y)*0.01;
	}
	else
		if (mouse.state==2)	
			TryMoveObject (event.motion.x, event.motion.y);
		else
			if (mouse.state==3) 
				TryDrowVoxel(event.motion.x, event.motion.y, vox[curVoxel].eightPalette[palette.curColor] );
	else
			if (mouse.state==4) 
				TryDeleteVoxel(event.motion.x, event.motion.y);
			else
		if (mouse.state==5) // 1-rotating creation perspective
	{
		vox[curVoxel].ax=sglobxh-(mouse.downx-event.motion.x)*0.01;
		vox[curVoxel].ay=sglobyh+(mouse.downy-event.motion.y)*0.01;
	/*	palette.rotVec.x=palette.rotVecDef.x;
		palette.rotVec.y=palette.rotVecDef.y;
		palette.rotVec.h=palette.rotVecDef.h;
		rotateVectorD(&palette.rotVec.x, &palette.rotVec.y, &palette.rotVec.h, 0, (mouse.downy-event.motion.y)*0.01, -(mouse.downx-event.motion.x)*0.01);
	*/}
		else if (mouse.state==6) 
			{
	vox[curVoxel].ah=sglobxh-(mouse.downx-event.motion.x)*0.01;
	/*palette.rotVec.x=palette.rotVecDef.x;
	palette.rotVec.y=palette.rotVecDef.y;
	palette.rotVec.h=palette.rotVecDef.h;
	rotateVectorD(&palette.rotVec.x, &palette.rotVec.y, &palette.rotVec.h, -(mouse.downx-event.motion.x)*0.01, 0, 0);
		*/
		}
		else if (mouse.state==7) glob=sglob-(mouse.downx-event.motion.x)*0.01;
		else if (mouse.state==8) 
			TryDrowVoxels(false);
		else if (mouse.state==9) 
		{
			//if (mousecoordsPlus(10))
			TryDeleteVoxels();
		}
/*
		katet3dD (palette.rotVec.x, palette.rotVec.y, palette.rotVec.h);
vox[curVoxel].ah=getAngle(palette.rotVec.x, palette.rotVec.y);
vox[curVoxel].ay=getAngle(palette.rotVec.y, fuck);
vox[curVoxel].ax=getAngle(fuck, palette.rotVec.x);*/
}

if( event.type == SDL_MOUSEBUTTONUP ) 
{
mouse.state=0;
/*
palette.rotVecDef.x=palette.rotVec.x;
palette.rotVecDef.y=palette.rotVec.y;
palette.rotVecDef.h=palette.rotVec.h;*/
}
}

void CreationDesk()
{
SDL_Rect dest, src;
// mouse states:
	// 1-moving creation perspective
	// 2-moving creation
	// 3-drowing
	// 4-deleting
	// 5-moving voxel direction xh, yh
	// 6-moving voxel xy
	// 7-moving creation xy


	//{
		//DrovVexel(i, scrwid+scrwid/4, scrhid/2);
	//}
DrovWholeCreation(&drowBigPixel);

if (palette.toolInHand==3)
	 {
		 src.x=palette.palettex-palette.brushSize/2;
		 src.y=palette.palettey-palette.brushSize/2;
		 src.w=src.h=palette.brushSize;
		 dest.x=mouse.pos.x-palette.brushSize/2;
		 dest.y=mouse.pos.y-palette.brushSize/2;
		 SDL_BlitSurface(palette.image, &src, screen, &dest);

		  //SDL_BlitSurface(palette, &src, screen, &dest);
	 }
palette.currentH=256;
palette.currentGpixel=0;
if (bit8Mode)
{
DrowBigVex(palette.currentG8,BVx, BVy, screen, vox[curVoxel].ax, vox[curVoxel].ay, vox[curVoxel].ah, BigVexPixSize, &drowBigPixel);
DrowBigVexFullColor(testG, BVx, BVy, screen, vox[curVoxel].ax, vox[curVoxel].ay, vox[curVoxel].ah, BigVexPixSize, &CatchBigGPixel);
}
else
DrowBigVexFullColor(testG, BVx, BVy, screen, vox[curVoxel].ax, vox[curVoxel].ay, vox[curVoxel].ah, BigVexPixSize, &drowCatchNOutline);


int cx,cy;
Uint8 ms = SDL_GetMouseState(&cx, &cy);
if (!(ms & SDL_BUTTON(SDL_BUTTON_LEFT)) && (!(ms & SDL_BUTTON(SDL_BUTTON_RIGHT)))) mouse.state=0;


if (palette.toolInHand==3)
	DrowFrame(mouse.pos.x-palette.brushSize/2,mouse.pos.y-palette.brushSize/2,palette.brushSize,palette.brushSize,0xFFFFFFFF);
else
	DrowFrame(mouse.pos.x-(palette.brushSize/2)*pixsize,mouse.pos.y-(palette.brushSize/2)*pixsize,palette.brushSize*pixsize,palette.brushSize*pixsize,0xFFFFFFFF);


}



void EditorLightningInit()
{
//	objectLight mouseLight, li1, li2, li3;
//worldcoordinates mousebody, l1bod, l2bod, l3bod;
//worldcoordinates testbody;
mouseLight.dinamic=1;
mouseLight.distanceFading=0;
testbody.tx=mousebody.tx=0;
testbody.ty=mousebody.ty=0;
testbody.x=scrwid*accuracy;
testbody.y=scrhid*accuracy;
mouseLight.angleLight=0;
mouseLight.brightness=3;
mouseLight.color=0x00FFFFFF;
AddLight(&mousebody, &mouseLight);

li1.dinamic=0;
li1.color=0x00330000;
li1.distanceFading=0;
l1bod.tx=0;
l1bod.ty=0;
l1bod.x=scrwi*accuracy;
l1bod.y=scrhi*accuracy;
li1.angleLight=0;
li1.brightness=2;
AddLight(&l1bod, &li1);

li2.dinamic=0;
li2.color=0x00003300;
li2.distanceFading=0;
l2bod.tx=0;
l2bod.ty=0;
l2bod.x=0;
l2bod.y=scrhi*accuracy;
li2.angleLight=0;
li2.brightness=2;
AddLight(&l2bod, &li2);

li3.dinamic=0;
li3.color=0x00000033;
li3.distanceFading=0;
l3bod.tx=0;
l3bod.ty=0;
l3bod.x=scrwid*accuracy;
l3bod.y=0;
li3.angleLight=0;
li3.brightness=2;
AddLight(&l3bod, &li3);
}
int VoxelDesk ()
{
SDL_Rect src,qdest;
float offsetxm=0;
int testPseudoAnframes=0;
SelectCreation();

//palette.R=0xF2;
//palette.G=0x02;
//palette.B=0x02;
palette.brushSize=5;
testG=&MatrixToG_Particle (curVoxel, 0);
palette.currentG8=&MatrixTo8BitParticle (curVoxel, 0);
//delete (testG->gpixels);
palette.curColor=1;
loadPalette(curCreation);
BreakColor(1);

palette.rotVecDef.x=palette.rotVec.x=100;
palette.rotVecDef.h=palette.rotVecDef.y=palette.rotVec.h=palette.rotVec.y=0;


EditorLightningInit();
DirectionalLightetImage dlm;
/*if (curCreation==4) RefreshVoxelPixels(curVoxel);
if (curCreation==2) 
{
	RefreshVoxelPixels(0);
	RefreshVoxelPixels(1);
	RefreshVoxelPixels(2);
}*/
while (!quit)
{
mouseLight.color=vox[curVoxel].eightPalette[palette.curColor];
mousebody.x=mouse.pos.x*accuracy;
mousebody.y=mouse.pos.y*accuracy;
mouseLight.angleLight=0;
if (!mouseLight.active) AddLight(&mousebody, &mouseLight);

vSized=vSize/2;
SDL_FillRect(screen,NULL, 0xFFA0A0A0); 

drowDimentions ();

//FastDrowCreation (250, 300, globxh,globyh,glob);

CreationDesk();
// *****************************************************************************Palette:
apply_surface( 0, scrhi-palette.image->h, palette.image, screen);
//apply_surface( scrwid, scrhi-palette.image->h, palette.imageForDirectionalLight, screen);
DrowFrame (0, scrhi-palette.image->h, palette.image->w, palette.image->h, 0xFFFFFF);

//*******************************************************************************G-Particles test
int somebodys=7;
int testareax=300;
int testareay=scrhid+100;//scrhid+0;

offsetxm+=2;
if (offsetxm>scrwi) offsetxm=0;

qdest.x=testareax-10;
qdest.y=testareay-10;
//qdest.w=vox[curVoxel].size *somebodys;
//qdest.h=vox[curVoxel].size *somebodys;
src.x=0;
src.y=0;
src.w=scrwid-qdest.x;
src.h=scrhi-qdest.y;

SDL_BlitSurface(image[3], &src, screen, &qdest);
//apply_surface( 300, 300, samplePalette, screen);

//if (testG->count)
/*for (int i=0; i<somebodys; i++)
	for (int j=0; j<somebodys; j++)
		if (palette.currentG8->count)
			QuickDrow8bitVex(palette.currentG8, testareax + (vox[curVoxel].size+40)*i, testareay + (vox[curVoxel].size+30)*j, 0, screen, 1, vox[curVoxel].ax, vox[curVoxel].ay, vox[curVoxel].ah,palette.currentG8->palette);
	*/		
testbody.x=(testareax + (vox[curVoxel].size+40))*accuracy;
testbody.y=(testareay + (vox[curVoxel].size+30) - 100)*accuracy;
offx=testareax + (vox[curVoxel].size+40);
offy=testareay + (vox[curVoxel].size+30) - 100;

if (bit8Mode)
{
QuickDrow8bitVex(palette.Buffer8bit,&testbody, palette.currentG8, 0, 0, 10, 1,  vox[curVoxel].ax+Vox3Deff*(offx-scrwid), vox[curVoxel].ay-Vox3Deff*(offy-scrhid), vox[curVoxel].ah,1);
DrowBuffer(palette.currentG8->palette,offx , offy, screen);
}
//ConvertBufLightToImag (palette.imageForDirectionalLight, &dlm);
//ConvertBufLightDirectionToAlpha (testPseudoAnim);
//createShadedSmoke(palette.currentG8, &dlm, palette.imageForDirectionalLight);
//LightMaskFromBuffer(mouse.pos.x-25, mouse.pos.y, &dlm, &testbody, 0);
//PseudoAnimatedVolume (mouse.pos.x-testPseudoAnim->w/2, mouse.pos.y-testPseudoAnim->h/2, screen, testPseudoAnim,BufferAnglesCopy, testPseudoAnframes, 0.995);
//testPseudoAnframes+=4;

//QuickDrow8bitVex(&testbody, palette.currentG8, 0, 0, 10, 1,  vox[curVoxel].ax+Vox3Deff*(offx-scrwid), vox[curVoxel].ay-Vox3Deff*(offy-scrhid), vox[curVoxel].ah);
//DrowBuffer(palette.currentG8->palette,mouse.pos.x , mouse.pos.y, screen);
/*QuickDrow8bitVex(&testbody, palette.currentG8, 0, 0, 10, 1,  vox[curVoxel].ax+Vox3Deff*(offx-scrwid), vox[curVoxel].ay-Vox3Deff*(offy-scrhid), vox[curVoxel].ah);
DrowBuffer(palette.currentG8->palette,0 , scrhi, screen);
QuickDrow8bitVex(&testbody, palette.currentG8, 0, 0, 10, 1,  vox[curVoxel].ax+Vox3Deff*(offx-scrwid), vox[curVoxel].ay-Vox3Deff*(offy-scrhid), vox[curVoxel].ah);
DrowBuffer(palette.currentG8->palette,scrwi , 0, screen);
QuickDrow8bitVex(&testbody, palette.currentG8, 0, 0, 10, 1,  vox[curVoxel].ax+Vox3Deff*(offx-scrwid), vox[curVoxel].ay-Vox3Deff*(offy-scrhid), vox[curVoxel].ah);
DrowBuffer(palette.currentG8->palette,scrwi , scrhi, screen);*/
//QuickDrow8bitVex(&testbody,palette.currentG8, testareax + (vox[curVoxel].size+40), testareay + (vox[curVoxel].size+30) - 100, 0, screen, 1, vox[curVoxel].ax, vox[curVoxel].ay, vox[curVoxel].ah,palette.currentG8->palette);
//SuperFast8bitVex(palette.currentG8, testareax + (vox[curVoxel].size+40) - 128, testareay + (vox[curVoxel].size+30) - 100, 0, screen, 0.8, vox[curVoxel].ax, vox[curVoxel].ay, vox[curVoxel].ah,palette.currentG8->palette);
//		if (testG->count)
//SuperFastVoxel360(testG, testareax + (vox[curVoxel].size+40)*i, testareay + (vox[curVoxel].size+30)*j, 0, screen, vox[curVoxel].ax, vox[curVoxel].ay, vox[curVoxel].ah);

	
	
	//DrowVex(testG, testareax + (vox[curVoxel].size+40)*i, testareay + (vox[curVoxel].size+30)*j, 0, screen, vox[curVoxel].ax, vox[curVoxel].ay, vox[curVoxel].ah);
//if (testG->count)
//DrowVex(testG, offsetxm, 310, 0, screen, vox[curVoxel].ax, vox[curVoxel].ay, vox[curVoxel].ah);


// palette test
//Uint32 * scrpxl=(Uint32*)screen->pixels +
for (int i=0; i<8; i++)
	drowBigPixel (scrwid-20*i-20,scrhi-70,20, palette.currentG8->palette[0x1<<i], 2);

for (int i=0; i<16; i++)
	for (int j=0; j<17; j++)
	drowBigPixel (scrwi-2*i-2,scrhi-2*j-2,2, palette.currentG8->palette[j+i*16], 2);

// rotating vector test
drowBigPixel (scrwid+palette.rotVec.x,scrhid+palette.rotVec.y,20, 0xFFFFFFFF, 2);


prodata("curh (Z,X):",curh,7);
prodata("halftransparent(T):",halfransparent,8);
prodata(VoxelNames[curVoxel],curVoxel,9);
prodata(CreationsNames[curCreation],curCreation,10);
prodata("mstate",mouse.state,11);
 prodata("zoom",palette.zoom,12);
//sdlanydrow (uButt[1].x, uButt[1].y, 0, uButt[1].ButtonImage);
if (saving) 
{
	sdlanydrow (uButt[2].x, uButt[2].y, 0, uButt[2].ButtonImage);
	sdlanydrow (uButt[3].x, uButt[3].y, 0, uButt[3].ButtonImage);
	if (Inputtext) SDL_BlitSurface(Inputtext, NULL, screen, NULL); 
}
 



for (int l=0; l<=curh; l++)
for (int i=0; i<vSize; i++)
for (int j=0; j<vSize; j++)
	if (vox[curVoxel].VoxelMatrix[i][j][l])
	{
		//VectorTest((i-vSize/2)*10+1, (j-vSize/2)*10+1, (l-vSize/2)*10+1, VoxelMatrix[i][j][l], VoxelMatrixA[i][j][l]);
		drowBigPixel (i*pixsize+10,j*pixsize+10,pixsize-(curh-l), vox[curVoxel].VoxelMatrix[i][j][l], vSize-l);
	}




/*for (int l=0; l<vSize; l++)
for (int i=0; i<vSize; i++)
for (int j=0; j<vSize; j++)
	if (vox[curVoxel].VoxelMatrix[i][j][l])
	{*/

		//VectorRotPerPixelWorking((i-vox[curVoxel].size/2)*10+1, (j-vox[curVoxel].size/2)*10+1, (l-vox[curVoxel].size/2)*10+1, vox[curVoxel].VoxelMatrix[i][j][l], vox[curVoxel].VoxelMatrixA[i][j][l], 400, 100, vox[curVoxel].ax, vox[curVoxel].ay, vox[curVoxel].ah, 9);
		//NOT_VectorTest ((i-vox[curVoxel].size/2)*10+1, (j-vox[curVoxel].size/2)*10+1, (l-vox[curVoxel].size/2)*10+1, vox[curVoxel].VoxelMatrix[i][j][l], vox[curVoxel].VoxelMatrixA[i][j][l], 400, 100, vox[curVoxel].ax, vox[curVoxel].ay, vox[curVoxel].ah, 9);
	//}


DrowFrame (10, 10, vSize*(pixsize), vSize*(pixsize), 0xFFFFFF);
drowBigPixel (vSize*(pixsize)+20,10+(vSize*(pixsize)-curh*pixsize),pixsize, 0xFFFFFF, vSize);

DrowFrame (scrwid, 0, scrwid/2, scrhi/2, 0xFFFFFF);
DrowFrame (scrwid, scrhi/2, scrwid/2, scrhi/2-1, 0xFFFFFF);
DrowFrame (scrwid+scrwid/2, 0, scrwid/2-1, scrhi/2, 0xFFFFFF);
DrowFrame (scrwid+scrwid/2, scrhi/2, scrwid/2-1, scrhi/2-1, 0xFFFFFF);

drowBigPixel (scrwid-20,0,20, vox[curVoxel].eightPalette[palette.curColor], 1);



for (int i=1; i<9; i++)	drowBigPixel (scrwid-20*i,scrhi-20,12, vox[curVoxel].eightPalette[i], 1);


drowBigPixel (scrwid-40,0,20, 0x000000, 1);
drowBigPixel (scrwid-265+palette.B,40,5, 0xFF0000, 1);
drowBigPixel (scrwid-265+palette.G,50,5, 0x00FF00, 1);
drowBigPixel (scrwid-265+palette.R,60,5, 0x0000FF,  1);
vox[curVoxel].eightPalette[palette.curColor]=palette.R+palette.G*256+palette.B*256*256;
//drowBigPixel (600,300,4, 0xFFFFFF, 0,1  );


//
if (palette.toolInHand==0)
if ((mouse.pos.x>10) && (mouse.pos.y>10) && (mouse.pos.x<10+vSize*pixsize) && (mouse.pos.y<10+vSize*pixsize))
	drowBigPixel (mouse.pos.x-5,mouse.pos.y-5,5, vox[curVoxel].eightPalette[palette.curColor], 1);





if (( keystates[  SDLK_r ] ) && (palette.B<255)) palette.B++;
if (( keystates[  SDLK_e ]) && (palette.B>0)) palette.B--;
if (( keystates[  SDLK_g ]) && (palette.G<255))palette.G++;
if (( keystates[  SDLK_f ]) && (palette.G>0)) palette.G--;
if (( keystates[ SDLK_b ]) && (palette.R<255)) palette.R++;
if (( keystates[ SDLK_v ]) && (palette.R>0)) palette.R--;

//glob+=0.003;
//globh+=0.002;


  DrowFrame (0, scrhi-palette.image->h, palette.image->w, palette.image->h, 0xFFFFFF);
while( SDL_PollEvent( &event ) ) 
{

CreationDescEventHandling();

if( event.type == SDL_MOUSEBUTTONDOWN ) 
 {
	 if ((mouse.pos.x<palette.image->w) && (mouse.pos.y>scrhi-palette.image->h))
	 {
		   if( event.button.button == SDL_BUTTON_WHEELDOWN)
palette.brushSize++;
	if( event.button.button == SDL_BUTTON_WHEELUP)
palette.brushSize--;
	 if (palette.brushSize<1) palette.brushSize=1;
	if (palette.brushSize>100) palette.brushSize=100;

	 }
	 else
if (mouse.pos.x<scrwid)
	 {
	  if( event.button.button == SDL_BUTTON_WHEELDOWN)
curh++;
	if( event.button.button == SDL_BUTTON_WHEELUP)
curh--;
	 if (curh<0) curh=0;
	if (curh>=vSize) curh=vSize-1;
	 }
else
{
	  if( event.button.button == SDL_BUTTON_WHEELDOWN)
		  palette.zoom++;
	if( event.button.button == SDL_BUTTON_WHEELUP)
 palette.zoom--;
	 if ( palette.zoom<0)  palette.zoom=0;
	if ( palette.zoom>8)  palette.zoom=8;
}

}


if( event.type == SDL_MOUSEMOTION ) 
 {
	 mouse.pos.x=event.button.x;
	 mouse.pos.y=event.button.y;
}
// Changing Pencil color:
if (event.type == SDL_KEYDOWN ) 
{

if (!saving)
{
	if (keystates[SDLK_1])  BreakColor(1);// palette.curColor=1;
	if (keystates[SDLK_2])  BreakColor(2);//palette.curColor=2;
	if (keystates[SDLK_3])  BreakColor(3);//palette.curColor=3;
	if (keystates[SDLK_4])  BreakColor(4);//palette.curColor=4;
	if (keystates[SDLK_5])  BreakColor(5);//palette.curColor=5;
	if (keystates[SDLK_6])  BreakColor(6);//palette.curColor=6;
	if (keystates[SDLK_7])  BreakColor(7);
	if (keystates[SDLK_8])  BreakColor(8);
if ( keystates[ SDLK_ESCAPE ] ) JumpToSpace();
if (keystates[SDLK_9])  bit8Mode=!bit8Mode;

if (keystates[SDLK_w]) {vox[curVoxel].massCenter.y--;palette.rebuildGpixel=1;}
if (keystates[SDLK_s]) {vox[curVoxel].massCenter.y++;palette.rebuildGpixel=1;}
if (keystates[SDLK_a]) {vox[curVoxel].massCenter.x--;palette.rebuildGpixel=1;}
if (keystates[SDLK_d]) {vox[curVoxel].massCenter.x++;palette.rebuildGpixel=1;}
if (keystates[SDLK_x]) {vox[curVoxel].massCenter.h--;palette.rebuildGpixel=1;}
if (keystates[SDLK_z]) {vox[curVoxel].massCenter.h++; palette.rebuildGpixel=1;}


	if (event.key.keysym.sym == SDLK_RIGHT)
	{
SaveVoxel(curVoxel);
curVoxel++;
LoadVoxel(curVoxel);
palette.rebuildGpixel=1;
BreakColor(palette.curColor);
	}

	if ((event.key.keysym.sym == SDLK_LEFT) && (curVoxel>0))
	{
SaveVoxel(curVoxel);
curVoxel--;
LoadVoxel(curVoxel);
palette.rebuildGpixel=1;
BreakColor(palette.curColor);
	}



if (event.key.keysym.sym == SDLK_UP)
{
	vSize++;
	if (vSize>maxvSize) vSize=maxvSize;
	palette.rebuildGpixel=1;
	vox[curVoxel].size=vSize;
}

if (event.key.keysym.sym == SDLK_DOWN)
{
	vSize--;
	if (vSize<5) vSize=5;
	palette.rebuildGpixel=1;
	vox[curVoxel].size=vSize;
}

}
else
InputText();

	if (event.key.keysym.sym == SDLK_t ) halfransparent=!halfransparent;
	//if (event.key.keysym.sym == SDLK_p ) curh++;
	//if (event.key.keysym.sym == SDLK_l ) curh--;
	if (curh<0) curh=0;
	else
	if (curh>=vSize) curh=vSize-1;
	//if ( keystates[ SDLK_F12 ] )



}

	

if( event.type == SDL_QUIT )
{ 
quit = true;
				
} 
			
}

if ((palette.rebuildGpixel) && (mouse.state==0))
	{
palette.rebuildGpixel=0;
//delete (testG->gpixels);
testG=&MatrixToG_Particle (curVoxel, 0);
//*palette.currentG8=TryOpen8bitVexel(curVoxel);
palette.currentG8=&MatrixTo8BitParticle (curVoxel, 0);
storeG8Particle(curVoxel, *palette.currentG8);
	}

		CastShadowOnScreen();
clearLights();
swapscreens();
		}


		SaveVoxel(curVoxel);
		SaveCreation ();
	return 0;
}



void DirectionalShadedDrow()
{
if (!OnScreen( offx,  offy, 100,100))
	{	PEpoint->hp=-1;
		return;
	}

//SmokeDinamics();



PEpoint->hp-=accurate(abs(PEpoint->speedx)+abs(PEpoint->speedy));
int elevation=PEpoint->pos.h/dddstength;
PEpoint->slowing=0.9+upgoingSmokeSlowingChange*accurate(PEpoint->pos.h);

//CastShadowForDarkenfFunk(flameLight[1],screen,offx+accurate(PEpoint->pos.h),offy,min(PEpoint->hp,(200-PEpoint->hp)*6),1);
offx+=(((offx-scrwid)*elevation)>>16)-dLISmoke[PEpoint->type-1].w/2;
offy+=(((offy-scrhid)*elevation)>>16)-dLISmoke[PEpoint->type-1].h/2;

LightMaskFromBuffer(offx, offy, &dLISmoke[PEpoint->type-1], &PEpoint->pos, (float)min(PEpoint->hp,(300-PEpoint->hp)*6)/128);


}
void UpdateSmokeAngles(){
if (smokeRendered){
//float SMsize=1;//25, 35;
SMsize+=1;//(20.1/(SMsize-10));//+=0.03;
SMsize2+=1;//(20.1/(SMsize2-10));//+=0.03;
//(0.08*increasing-0.05)/SMsize;
//smokeAngMask1=0, smokeAngMask2=0;
if (SMsize2>60) {SMsize2=20; smokeAngMask2=!smokeAngMask2;}
if (SMsize>60) { SMsize=20; smokeAngMask1=!smokeAngMask1; }/*increasing=0;
if (SMsize<0.5) increasing=1;*/
int realSize1;realSize1=SMsize-19; realSize1*=realSize1; 
int realSize2;realSize2=SMsize2-19; realSize2*=realSize2; //7 8-voxels
QuickDrow8bitVexHeightOnly(palette.SmokeBuffer8bit,&testbody, &gayVoxel[3][7+smokeAngMask1], 0, 0, SMsize- (realSize1*0.0125), 0.03*(SMsize - (realSize1*0.0125)) ,  0.1,  0.1, M_PI+smokeRotation,0);
QuickDrow8bitVexHeightOnly(palette.SmokeBuffer8bit,&testbody, &gayVoxel[3][7+smokeAngMask2], 0, 0, SMsize2- (realSize2*0.0125), 0.03*(SMsize2- (realSize2*0.0125)),  0.1,  0.1, -smokeRotation*2,0);
smokeRotation+=smokeRotSpeed;
CopyBufLightDirection (palette.SmokeBuffer8bit, BufferAnglesCopy, E8bitBufferDiam, E8bitBufferDiam);
while (smokeRotation>2*M_PI) smokeRotation-=2*M_PI;
smokeRendered=0;
}
}
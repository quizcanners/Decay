//#include "Voxeldesk.h"
int imageInWorks=106;
int partedImage[1000][1000];
int partedHeight[1000][1000];
int brush[1000][1000];
int dimx,dimy;
int dimxd,dimyd;
int curhd;
void partImage()
{
dimx=mergeimage[imageInWorks]->w;
dimy=mergeimage[imageInWorks]->h;
dimxd=dimx/2;
dimyd=dimy/2;

Uint32 *pixel = (Uint32*)mergeimage[imageInWorks]->pixels;
	Uint32 *hpixel = (Uint32*)heightimage[imageInWorks]->pixels;
		for (int j=0; j<mergeimage[imageInWorks]->h; j++)
			for (int i=0; i<mergeimage[imageInWorks]->w; i++)
		{
			partedImage[i][j]=*pixel;
			partedHeight[i][j]=*((Uint8*)hpixel+3)*256;
			pixel++;
			hpixel++;
		}


}

void DrowParted()
{
	int x,y,h;
	double kyt;
	
for (int i=0; i<mergeimage[imageInWorks]->w; i++)
		for (int j=0; j<mergeimage[imageInWorks]->h; j++)
		{
x=i-dimxd;
y=j-dimyd;
h=partedHeight[i][j]>>8;
	//		partedImage[i][j];
	//		partedHeight[i][j];

fuck=x*x+y*y;
fuck=sqrt(fuck);	
kyt=acos(x/fuck);
if (y<0) kyt=2*M_PI-kyt;
y=0;
x=fuck;

// real-time calculations

 double sz = sin(glob+kyt);
 double cz = cos(glob+kyt);
 double sx = sin(globyh);
 double cx = cos(globyh);
 double sy = sin(globxh);
 double cy = cos(globxh);

double xy, xh, yx, yh, hx, hy;

		hx = cz*x;
		hy = sz*x;	
		xy = cx*hy - sx*h;
		xh = sx*hy + cx*h;
		yh = cy*xh - sy*hx;
		yx = sy*xh + cy*hx;
		
//drowBigPixel (yx+200,xy+200,1, partedImage[i][j], 0, yh);
Uint32* screenPix=(Uint32*)screen->pixels+((int)xy+200)*screen->w+(int)yx+500;
	
			if ((int)(*((Uint8 * )screenPix+3))<h)
			{
*screenPix=partedImage[i][j];
*((Uint8 * )screenPix+3)=h;
			}

		}
}

void calculateBrush()
{
curhd=curh/2;
for (int i=0; i<curh; i++)	
	for (int j=0; j<curh; j++)
	{
		int ii=i-curhd;
		int jj=j-curhd;
		fuck=ii*ii+jj*jj;
		fuck=sqrt(fuck);
		brush[i][j]=255*(fuck/curhd-1);

	}
}

void heightEditing()
{
SDL_Rect dest;
	dest.x=200;
	dest.y=200;
SDL_BlitSurface(mergeimage[imageInWorks], NULL, screen, &dest);

if( event.type == SDL_MOUSEMOTION ) 
{
mouse.pos.x=event.button.x;
mouse.pos.y=event.button.y;
if (mouse.state)
{
	int x=mouse.pos.x-200;
	int y=mouse.pos.y-200;
	dest.x=max(0,x-curhd);
	dest.y=max(0,y-curhd);
	dest.w=max(0,min(dimx,x+curhd));
	dest.h=max(0,min(dimy,y+curhd));
	int mx=max(0,-(x-curhd));
	x=mx;
	y=max(0,-(y-curhd));
	if (mouse.state==1)
	for (int i=dest.x; i<dest.w; i++,y++,x=mx)	
		for (int j=dest.y; j<dest.h; j++, x++)
		{
			partedHeight[i][j]=max(partedHeight[i][j]-brush[x][y],0);
			
		}
	else
		for (int i=dest.x; i<dest.w; i++,y++,x=mx)	
		for (int j=dest.y; j<dest.h; j++, x++)
		{
			partedHeight[i][j]=max(partedHeight[i][j]+brush[x][y],0);
			
		}

}
	 

}

if( event.button.button == SDL_BUTTON_LEFT ) 
{
	mouse.state=1;
}

if( event.button.button == SDL_BUTTON_RIGHT ) 
{
	mouse.state=2;
}

if( event.type == SDL_MOUSEBUTTONUP ) 
{
	mouse.state=0;
}
}

void HightDesk ()
{


partImage();
while (!quit)
{
SDL_FillRect(screen,NULL, 0x000000); 
prodata("dimx",dimx,5);
prodata("dimy",dimy,6);
prodata("curh",curh,7);
if ( keystates[  SDLK_KP7 ] ) glob-=0.03;
if ( keystates[  SDLK_KP9 ] ) glob+=0.03;
if ( keystates[  SDLK_KP6 ] ) globxh+=0.03;
if ( keystates[  SDLK_KP4 ] ) globxh-=0.03;
if ( keystates[  SDLK_KP8 ] ) globyh+=0.03;
if ( keystates[  SDLK_KP2 ] ) globyh-=0.03;

	while( SDL_PollEvent( &event ) ) 
	{
if( event.type == SDL_QUIT )	quit = true;	
  if( event.button.button == SDL_BUTTON_WHEELDOWN)
  { curh++; calculateBrush();}
  else
	if( ( event.button.button == SDL_BUTTON_WHEELUP) && (mouse.state!=2))
	{curh--; calculateBrush(); }
	 if (curh<0) curh=0;
	else
	if (curh>=1000) curh=999;
	}

DrowParted();

heightEditing();
swapscreens();
}
}
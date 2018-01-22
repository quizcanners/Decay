SDL_Surface* CreateSurface(int width,int height,const SDL_Surface* display)
{
  // 'display' is the surface whose format you want to match
  //  if this is really the display format, then use the surface returned from SDL_SetVideoMode

  const SDL_PixelFormat& fmt = *(display->format);
  return SDL_CreateRGBSurface(display->flags,width,height,
                  fmt.BitsPerPixel,
                  fmt.Rmask,fmt.Gmask,fmt.Bmask,fmt.Amask );
}

float GLzoom=1;

void UpdateSDLScreen()
{

const float depth=0.5;

GLScreenOffsetData * glof=&GLoffset[curoffset];
int passed=glutGet(GLUT_ELAPSED_TIME)-SDLredrowtime;
SDLredrowtime=glutGet(GLUT_ELAPSED_TIME);
passed=min(passed,TimePerFrame);
glof->offx-=glof->offx*passed/TimePerFrame/(1+tera.ZOOMED*2); 
glof->offy-=glof->offy*passed/TimePerFrame/(1+tera.ZOOMED*2);
//recoil calculation
POINT p;
GetCursorPos(&p);
for (int gt=0; gt<passed; gt++){
player.bag.recoil.offx+=player.bag.recoil.spx+player.bag.recoil.coolx;
player.bag.recoil.offy+=player.bag.recoil.spy+player.bag.recoil.cooly;

player.bag.recoil.coolx*=0.99;
player.bag.recoil.cooly*=0.99;

player.bag.recoil.spx*=0.9;
player.bag.recoil.spy*=0.9;

player.bag.recoil.coolx-=(player.bag.recoil.offx*0.05);
player.bag.recoil.cooly-=(player.bag.recoil.offy*0.05);
//player.bag.recoil.offx*=0.99;
//player.bag.recoil.offy*=0.99;
}
//
int winX=p.x-glutGet(GLUT_WINDOW_X);
int winY=p.y-glutGet(GLUT_WINDOW_Y);

float rdx=min(GLwi,max(0,(int)(winX)))-GLwi/2;
float dx=rdx*(xtrform-1);//+player.bag.recoil.offx/32;
float rdy=min(GLhi,max(0,(int)(winY)))-GLhi/2;
float dy=rdy*(ytrform-1);//+player.bag.recoil.offy/32;
/*
int shx=player.bag.recoil.offx*GLM.zoom/32;
int shy=player.bag.recoil.offy*GLM.zoom/32;

glutWarpPointer((int)(winX-shx+GLM.shkdX), (int) (winY-shy+GLM.shkdY));

GLM.shkdX=shx;
GLM.shkdY=shy;

GLtoCPUx=dx;
GLtoCPUy=dy;
*/
float offx=(scrwi-GLwi)/2;
float stx=offx+dx;
float endx=scrwi-offx+dx;

float offy=(scrhi-GLhi)/2;
float sty=offy+dy;
float endy=scrhi-offy+dy;


float zoom=rdx*rdx+rdy*rdy;
zoom=sqrt(zoom);
zoom=1.2-zoom/GLwi;

GLzoom-=(float)((GLzoom-zoom)*passed)*0.001;
zoom=GLzoom;
//topdddstrength=dddstength;
GLddd=topdddstrength/zoom/zoom;
if (zoom<1){
	float enlargex=(MaxEnlarge-1)*(1-zoom);
	float enlargey=(float)GLhi*(enlargex);
	enlargex*=(float)GLwi;
float upfromX=(float)(GLwi/2+rdx)/(GLwi);
float upfromY=(float)(GLhi/2+rdy)/(GLhi);
stx-=enlargex*upfromX;
endx+=enlargex*(-upfromX+1);
sty-=enlargey*upfromY;
endy+=enlargey*(-upfromY+1); 



zoom=1;
}

GLM.zoom=zoom;

stx/=scrwi; stx=max(stx,0)+glof->offx/scrwi;
endx/=scrwi; endx=min(endx,1)+glof->offx/scrwi;
sty/=scrhi; sty=max(sty,0)+glof->offy/scrhi;
endy/=scrhi; endy=min(endy,1)+glof->offy/scrhi;

glBegin(GL_QUADS);
glTexCoord2f(stx, endy);
glVertex3f(-zoom, -zoom, depth);
glTexCoord2f(stx, sty);
glVertex3f(-zoom, zoom, depth);
glTexCoord2f(endx, sty);
glVertex3f(zoom, zoom, depth);
glTexCoord2f(endx, endy);
glVertex3f(zoom, -zoom, depth);
glEnd();
glFlush();

float minimaps=0.7;
float minimape=0.98;

glBegin(GL_QUADS);
glTexCoord2f(0, 1);
glVertex3f(minimaps, -minimape, depth);
glTexCoord2f(0, 0);
glVertex3f(minimaps, -minimaps, depth);
glTexCoord2f(1, 0);
glVertex3f(minimape, -minimaps, depth);
glTexCoord2f(1, 1);
glVertex3f(minimape, -minimape, depth);
glEnd();
glFlush();

glutSwapBuffers();
}

void LoadScreen()
{
while (updatedGL) SDL_Delay(1);

   glBindTexture(GL_TEXTURE_2D, textureId);
  // int temptimer=glutGet(GLUT_ELAPSED_TIME);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, GLscreen->w, GLscreen->h, 0x80E1, GL_UNSIGNED_BYTE, (GLvoid*)GLscreen->pixels);//imageData);
		
	//	timer=glutGet(GLUT_ELAPSED_TIME)-temptimer;
		
updatedGL=1;


Uint32 passed=glutGet(GLUT_ELAPSED_TIME);	

do {  UpdateSDLScreen(); SDL_Delay(10);} while (updatedGL);
 passed=glutGet(GLUT_ELAPSED_TIME)-passed;
TimePerFrame=((float)passed+TimePerFrame*4)/5;

curoffset=!curoffset;
}
void runMainLoop( int val )
{

 
LoadScreen();
 timer=glutGet(GLUT_ELAPSED_TIME)-timer;
 glutSetWindowTitle(intTotext(timer).c_str());//timer
//glutSetWindowTitle(intTotext(updates).c_str());
 
//updates=0;
glutTimerFunc( 0 , runMainLoop, val );
timer=glutGet(GLUT_ELAPSED_TIME);

}
void keyDown(unsigned char ch, int x, int y)
{
    GluKeys[ch] = true;    
	keystates[ ch ] = true;
}

void keyUp(unsigned char ch, int x, int y)
{
    GluKeys[ch] = false;
	keystates[ ch ] = false;
}

void mousehandleMove(int x, int y){
mouse.pos.x=(x)*xtrform;
mouse.pos.y=(y)*ytrform;
}
void mousehandlePress (int button, int state, int x, int y){

mouse.state=mouse.active=!state;
if (!state)
mouse.u8ms|=SDL_BUTTON(button+1);
else
mouse.u8ms&=(~SDL_BUTTON(button+1));
//GLUT_LEFT_BUTTON
	//GLUT_UP or GLUT_DOWN
mouse.pos.x=(x)*xtrform;
mouse.pos.y=(y)*ytrform;
}
void InitTestingGrounds(int argc, char* args[])
{
//SDL_Init ( 0);
//SDL_EnableUNICODE( SDL_ENABLE );



glutInit( &argc, args );
glutInitContextVersion( 2, 1 );
glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA); 
glutInitWindowSize( GLwi, GLhi );

glutCreateWindow( "Decay OpenGL version 0.00001" );
glMatrixMode( GL_PROJECTION );
glLoadIdentity();
glMatrixMode( GL_MODELVIEW );
glLoadIdentity();
glClearColor( 0.f, 0.f, 0.f, 1.f );
glEnable(GL_TEXTURE_2D);

	// imageData = new GLubyte[scrwi*scrhi*4];
    //memset(imageData, 0, scrwi*scrhi*4);

glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, scrwi, scrhi, 0,  0x80E1, GL_UNSIGNED_BYTE, (GLvoid*)imageData);
    glBindTexture(GL_TEXTURE_2D, 0);

	glEnable(GL_TEXTURE_2D);
glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

float stogx=(float)scrwi/(float)GLwi;
float stogy=(float)scrhi/(float)GLhi;
 MaxEnlarge=min(stogx,stogy);


SDL_Init( SDL_INIT_EVERYTHING );
//CreateSurface(scrwi, scrhi,image[44]); //SDL_SetVideoMode( scrwi, scrhi, 32, SDL_SWSURFACE );// change to initialise surface
image[44]=loadany ("pictures/loading.png");
screen= SDL_CreateRGBSurface(SDL_HWSURFACE, scrwi, scrhi, 32, 0xFF, 0xFF00, 0xFF0000, 0xFF000000);
	//CreateSurface(scrwi, scrhi,image[44]);
GLscreen= SDL_CreateRGBSurface(SDL_HWSURFACE, scrwi, scrhi, 32, 0xFF, 0xFF00, 0xFF0000, 0xFF000000);
	//CreateSurface(scrwi, scrhi,image[44]);
glutPassiveMotionFunc(mousehandleMove);
glutMouseFunc(mousehandlePress);
glutMotionFunc(mousehandleMove);
glutKeyboardFunc(keyDown);
glutKeyboardUpFunc(keyUp);
int sob=sizeof(bool);
memset(GluKeys, 0, sob*256);
//glutMouseFunc(handle);
//glutKeyboardFunc( handleKeys );
}
//OR
void InitOldSDLScreen(int argc, char* args[])
{
	
SDL_Init( SDL_INIT_EVERYTHING );
screen = SDL_SetVideoMode( scrwi, scrhi, 32, SDL_SWSURFACE);//SDL_HWSURFACE | SDL_DOUBLEBUF );
loadimages("pictures/loading.png",44);

SDL_Surface* icon = IMG_Load("pictures/icon.png");
SDL_WM_SetIcon(icon, NULL);

}
void CalculateGLOffs(){
GLScreenOffsetData * glof=&GLoffset[!curoffset];
GLScreenOffsetData * prev=&GLoffset[curoffset];
	glof->offx=prev->offx-((cameratylx-prev->tx)*tyleSize+camerax-prev->x);
	glof->offy=prev->offy-((cameratyly-prev->ty)*tyleSize+cameray-prev->y);
	glof->tx=cameratylx;
	glof->ty=cameratyly;
	glof->x=camerax;
	glof->y=cameray;

	if ((abs(glof->offx)>1000) || (abs(glof->offy)>1000)) {
		glof->offx=0; glof->offy=0;}

	if (tera.ZOOMED) {glof->offx=prev->offx+tera.dx; glof->offy=prev->offy+tera.dy; }

actcamx+=camxaccel;//+accurate(shake.x);; 
camerax=actcamx;
actcamy+=camyaccel;//+accurate(shake.y);;
cameray=actcamy;
}
void swapscreens()
{
	if (OpenGLscreen){
	while (!updatedGL) SDL_Delay(1);
	//topdddstrength=dddstength;
//GLddd=topdddstrength/zoom/zoom;
	dddstength=tera.ZOOMED ? topdddstrength : GLddd;
	SDL_Surface* temp=screen;
	screen=GLscreen;
	GLscreen=temp; 
	CalculateGLOffs();
	updatedGL=0;
	}else SDL_Flip( screen );

}

void GetMouse(int * x, int * y){
if (OpenGLscreen){
POINT p;
GetCursorPos(&p);
*x=(p.x-glutGet(GLUT_WINDOW_X))*xtrform;
*y=(p.y-glutGet(GLUT_WINDOW_Y))*ytrform;}
else{
*x = event.button.x; 
*y = event.button.y;
}
}







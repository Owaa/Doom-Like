/*!\file window.c
 *
 * \brief Sample d'utilisation de la bibliothèque GL4Dummies avec SDL2
 * et en OpenGL 3.3+ 
 * 
 * \author Farès BELHADJ, amsi@ai.univ-paris8.fr
 *
 * \date October 30 2014
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include"cam.h"
#include"laby.h"

#include <gl4du.h>

/*
 * Prototypes des fonctions statiques contenues dans ce fichier C
 */
static SDL_Window * initWindow(int w, int h, SDL_GLContext * poglContext);
static void quit(void);
static void initGL(SDL_Window * win);
static void initData(void);
static void resizeGL(SDL_Window * win);
static void loop(SDL_Window * win);
static void manageEvents(SDL_Window * win);
static void draw(GLfloat a0);
int colision_balle(int k);
/*!\brief pointeur vers la (future) fenêtre SDL */
static SDL_Window * _win = NULL;
static int _windowWidth = 800, _windowHeight = 600;
/*!\brief pointeur vers le (futur) contexte OpenGL */
static SDL_GLContext _oglContext = NULL;
/*!\brief identifiant du (futur) vertex array object */
static GLuint _vao = 0;
/*!\brief identifiant du (futur) buffer de data */
static GLuint _buffer = 0;
/*!\brief identifiant du (futur) GLSL program */
static GLuint _pId = 0;



/*!\brief identifiant de la texture */
static GLuint _tId_mur     = 0;
static GLuint _tId_balle   = 0;
static GLuint _tId_depart  = 0;
static GLuint _tId_arriver = 0;
static GLuint _tId_sol = 0;
static GLuint _tId_fantome = 0;
static GLuint _tId_skybox[5];


static GLuint _pause = 0;




static GLuint _skybox_vao[5];
static GLuint _vao_sol = 0;
static GLuint _buffer_sol = 0;

static GLuint tir = 0;
static GLfloat hautbasmonstre = 0;

static GLuint rotate = 0;
static int life = 100;
static int test = 0;
static GLuint help = 0;

static GLuint nb_balle = 0;
static GLuint nb_monstre = 25;

float  position_arme[3];
float  forward_arme[3];



struct balle {
  GLfloat x, y, z;
  float   position[3];
  float forward[3];
  GLuint active;
  int vie;
  int dir;

};
typedef struct balle balle;

balle tab_balle[20];
balle tab_monstre[50];


enum kyes_t {
  KLEFT = 0,
  KRIGHT,
  KUP,
  KDOWN
};

static GLuint _keys[] = {0, 0, 0, 0};

typedef struct cam_t cam_t;
struct cam_t {
  GLfloat x, z;
  GLfloat theta;
};

//static cam_t _cam = {0, 0, 0};

/*!\brief La fonction principale initialise la bibliothèque SDL2,
 * demande la création de la fenêtre SDL et du contexte OpenGL par
 * l'appel à \ref initWindow, initialise OpenGL avec \ref initGL et
 * lance la boucle (infinie) principale.
 */


void direction(int i){

  int x , before;
  before = tab_monstre[i].dir ;
  tab_monstre[i].dir = -1;

  
  if( tab_monstre[i].active == 0  || tab_monstre[i].vie < 0){
    	  tab_monstre[i].forward[0] =  0; 
	  tab_monstre[i].forward[1] =  1; 
	  tab_monstre[i].forward[2] =  0;
	  return;
}
  
  do{
    x = rand()% 4;
      switch(x){

      case 0:
	//	if(  before == x) break;

	if(laby[(int) (tab_monstre[i].position[0] / 20)+1 ][(int)(tab_monstre[i].position[2] / 20) ] ==  0 ){

	  tab_monstre[i].forward[0] =  1; 
	  tab_monstre[i].forward[1] =  0; 
	  tab_monstre[i].forward[2] =  0;
	  tab_monstre[i].dir = 0;
	}
	break;
	
	
      case 1:
	//	if(  before == x) break;
	if(laby[(int) (tab_monstre[i].position[0] / 20)-1 ][(int)(tab_monstre[i].position[2] / 20) ] ==  0 ){


	  tab_monstre[i].forward[0] =  1; 
	  tab_monstre[i].forward[1] =  0; 
	  tab_monstre[i].forward[2] =  0;
	  tab_monstre[i].dir = 1;

	}
	break;
      case 2:
	//	if(  before == x) break;

	if(laby[(int) (tab_monstre[i].position[0] / 20) ][(int)(tab_monstre[i].position[2] / 20) +1] ==  0 ){

	  tab_monstre[i].forward[0] =  0; 
	  tab_monstre[i].forward[1] =  0; 
	  tab_monstre[i].forward[2] =  1;
	  tab_monstre[i].dir = 2;

	}
	break;
      case 3:
	//	if(  before == x) break;

	if(laby[(int) (tab_monstre[i].position[0] / 20) ][(int)(tab_monstre[i].position[2] / 20) -1] ==  0 ){

	  tab_monstre[i].forward[0] =  0; 
	  tab_monstre[i].forward[1] =  0; 
	  tab_monstre[i].forward[2] =  1;
	  tab_monstre[i].dir = 3;

	}
	break;   
      }


      
    }while( tab_monstre[i].dir == -1);;
  test++;

}


void init_monstre(void){
  
  int i , x , z;

  for( i = 0; i< nb_monstre ; i++){
    /* verifier si je suis sur une case libre */
    tab_monstre[i].vie = 100;
    tab_monstre[i].active = 1;
    
    do{
    x = (rand()% MT ) * 20 ;
    z = (rand()% MT ) * 20 ;
    }while( laby[(int ) x/20][  (int) z/20] != 0);
    tab_monstre[i].position[0] = x ;
    tab_monstre[i].position[1] = 5;
    tab_monstre[i].position[2] = z ;
  
  tab_monstre[i].dir = 2;
    direction(i);


  }
   
  
  
}

int main(int argc, char ** argv) {
  srand(time(NULL));
  init_laby(laby);
  creat_laby();
  
  init_monstre();

  if(SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "Erreur lors de l'initialisation de SDL :  %s", SDL_GetError());
    return -1;
  }
  atexit(SDL_Quit);
  if((_win = initWindow(_windowWidth, _windowHeight, &_oglContext))) {
    initGL(_win);
    _pId = gl4duCreateProgram("<vs>shaders/basic.vs", "<fs>shaders/basic.fs", NULL);
    initData();
    loop(_win);
  } else 
    fprintf(stderr, "Erreur lors de la creation de la fenetre\n");
  return 0;
}

/*!\brief Cette fonction est appelée au moment de sortir du programme
 *  (atexit), elle libère la fenêtre SDL \ref _win et le contexte
 *  OpenGL \ref _oglContext.
 */
static void quit(void) {
  if(_tId_mur)
    glDeleteTextures(1, &_tId_mur);
  if(_vao)
    glDeleteVertexArrays(1, &_vao);
  if(_buffer)
    glDeleteBuffers(1, &_buffer);
  if(_oglContext)
    SDL_GL_DeleteContext(_oglContext);
  if(_win)
    SDL_DestroyWindow(_win);
  gl4duClean(GL4DU_ALL);
}

/*!\brief Cette fonction créé la fenêtre SDL de largeur \a w et de
 *  hauteur \a h, le contexte OpenGL \a poglContext et stocke le
 *  pointeur dans poglContext. Elle retourne le pointeur vers la
 *  fenêtre SDL.
 *
 * Le contexte OpenGL créé est en version 3 pour
 * SDL_GL_CONTEXT_MAJOR_VERSION, en version 2 pour
 * SDL_GL_CONTEXT_MINOR_VERSION et en SDL_GL_CONTEXT_PROFILE_CORE
 * concernant le profile. Le double buffer est activé et le buffer de
 * profondeur est en 24 bits.
 *
 * \param w la largeur de la fenêtre à créer.
 * \param h la hauteur de la fenêtre à créer.
 * \param poglContext le pointeur vers la case où sera référencé le
 * contexte OpenGL créé.
 * \return le pointeur vers la fenêtre SDL si tout se passe comme
 * prévu, NULL sinon.
 */
static SDL_Window * initWindow(int w, int h, SDL_GLContext * poglContext) {
  SDL_Window * win = NULL;
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  if( (win = SDL_CreateWindow("Fenetre GL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
			      w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | 
			      SDL_WINDOW_SHOWN)) == NULL )
    return NULL;
  if( (*poglContext = SDL_GL_CreateContext(win)) == NULL ) {
    SDL_DestroyWindow(win);
    return NULL;
  }
  fprintf(stderr, "Version d'OpenGL : %s\n", glGetString(GL_VERSION));
  fprintf(stderr, "Version de shaders supportes : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));  
  atexit(quit);
  return win;
}

/*!\brief Cette fonction initialise les paramètres OpenGL.
 *
 * \param win le pointeur vers la fenêtre SDL pour laquelle nous avons
 * attaché le contexte OpenGL.
 */
static void initGL(SDL_Window * win) {
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glEnable(GL_DEPTH_TEST);
  // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
  gl4duGenMatrix(GL_FLOAT, "modelViewMatrix");
  gl4duGenMatrix(GL_FLOAT, "projectionMatrix");
  resizeGL(win);
}

static void initData(void) {

  //  SDL_Surface * texSurface_balle;

  GLfloat s3 = 100 , data_skybox_top[] = {

   -s3, s3*2, -s3,
    s3 , s3*2, -s3,
    -s3, s3*2,  s3,
    s3 , s3*2,  s3,


    /* 4 normales */
    0.0f, 1.0f, 0.0f, 
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,

    0.0f, 0.0f, 
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,

   
  };

  glGenVertexArrays(5,_skybox_vao );
  glBindVertexArray(_skybox_vao[0]);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glGenBuffers(1, &_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, _buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof data_skybox_top, data_skybox_top, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0);  
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE,  0, (const void *)(4* 3 * sizeof *data_skybox_top));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (const void *)(4 * 6 * sizeof *data_skybox_top));
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  
  SDL_Surface * texSurface;  
  GLfloat s = 10.0, s2 = 10.0 ,data[] = { 
    /* 4 coordonnées de sommets */
    
    -s2, s2*2, -s2,
    s2 , s2*2, -s2,
    -s2, s2*2,  s2,
    s2 , s2*2,  s2,
    
    /* 4 coordonnées de sommets */
    s2 ,s2*2,  s2,
    s2 , s2*2, -s2,
    s2 , 0,  s2,
    s2 , 0, -s2 ,
    /* 4 coordonnées de sommets */
    s2 , 0, -s2 ,
    s2 , s2*2, -s2,
    -s2, 0, -s2,
    -s2, s2*2, -s2,
    /* 4 coordonnées de sommets */
    -s2, s2*2, -s2,
    -s2, s2*2,  s2,
    -s2, 0, -s2,
    -s2, 0,  s2,
    /* 4 coordonnées de sommets */  
    -s2, 0,  s2,
    -s2, s2*2,  s2,
    s2 , 0,  s2,
    s2 , s2*2,  s2,



    
    /* 4 normales */
    0.0f, 1.0f, 0.0f, 
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,

    /* 4 normales */
    0.0f, 0.0f, -1.0f, 
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    
    /* 4 normales */
    -1.0f, 0.0f, 0.0f, 
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
        /* 4 normales */
    /* Normale a gere problemme de lumier les tecture de s affiche pas */
    0.0f, 0.0f, -1.0f, 
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
        /* 4 normales */
    -1.0f, 0.0f, 0.0f, 
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    
    /* 4 coordonnées de texture, une par sommet */
    0.0f, 0.0f, 
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    /* 4 coordonnées de texture, une par sommet */
    0.0f, 0.0f, 
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
 /* 4 coordonnées de texture, une par sommet */
    
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    
 /* 4 coordonnées de texture, une par sommet */
    0.0f, 0.0f, 
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,   
 /* 4 coordonnées de texture, une par sommet */
     1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    0.0f, 0.0f
   
  };


  glGenVertexArrays(1, &_vao);
  glBindVertexArray(_vao);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glGenBuffers(1, &_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, _buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof data, data, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0);  
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE,  0, (const void *)(20* 3 * sizeof *data));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (const void *)(20 * 6 * sizeof *data));
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  
  glGenTextures(1, &_tId_mur);
  glBindTexture(GL_TEXTURE_2D, _tId_mur);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  if( (texSurface = SDL_LoadBMP("images/mur5.bmp")) == NULL ) {
    fprintf(stderr, "Impossible d'ouvrir le fichier : %s\n", SDL_GetError());
    exit(1);
  }
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texSurface->w, texSurface->h, 0, GL_BGR, GL_UNSIGNED_BYTE, texSurface->pixels); 
  glBindTexture(GL_TEXTURE_2D, 0);

  glGenTextures(1, &_tId_fantome);
  glBindTexture(GL_TEXTURE_2D, _tId_fantome);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  if( (texSurface = SDL_LoadBMP("images/fantome.bmp")) == NULL ) {
    fprintf(stderr, "Impossible d'ouvrir le fichier : %s\n", SDL_GetError());
    exit(1);
  }
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texSurface->w, texSurface->h, 0, GL_BGR, GL_UNSIGNED_BYTE, texSurface->pixels); 
  glBindTexture(GL_TEXTURE_2D, 0);


  
  

 glGenTextures(1, &_tId_balle);
  glBindTexture(GL_TEXTURE_2D, _tId_balle);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  if( (texSurface = SDL_LoadBMP("images/index.bmp")) == NULL ) {
    fprintf(stderr, "Impossible d'ouvrir le fichier : %s\n", SDL_GetError());
    exit(1);
  }
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texSurface->w, texSurface->h, 0, GL_BGR, GL_UNSIGNED_BYTE, texSurface->pixels);
  glBindTexture(GL_TEXTURE_2D, 0);


  // depart
  glGenTextures(1, &_tId_depart);
  glBindTexture(GL_TEXTURE_2D, _tId_depart);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  if( (texSurface = SDL_LoadBMP("images/rouge.bmp")) == NULL ) {
    fprintf(stderr, "Impossible d'ouvrir le fichier : %s\n", SDL_GetError());
    exit(1);
  }
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texSurface->w, texSurface->h, 0, GL_BGR, GL_UNSIGNED_BYTE, texSurface->pixels);
  glBindTexture(GL_TEXTURE_2D, 0);


 // arriver
  glGenTextures(1, &_tId_arriver);
  glBindTexture(GL_TEXTURE_2D, _tId_arriver);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  if( (texSurface = SDL_LoadBMP("images/vert.bmp")) == NULL ) {
    fprintf(stderr, "Impossible d'ouvrir le fichier : %s\n", SDL_GetError());
    exit(1);
  }
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texSurface->w, texSurface->h, 0, GL_BGR, GL_UNSIGNED_BYTE, texSurface->pixels);
  glBindTexture(GL_TEXTURE_2D, 0);

  
  // Skybox

  glGenTextures(5, _tId_skybox);

  // back
  glBindTexture(GL_TEXTURE_2D, _tId_skybox[0]);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  if( (texSurface = SDL_LoadBMP("images/back.bmp")) == NULL ) {
    fprintf(stderr, "Impossible d'ouvrir le fichier : %s\n", SDL_GetError());
    exit(1);
  }
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texSurface->w, texSurface->h, 0, GL_BGR, GL_UNSIGNED_BYTE, texSurface->pixels); 
  glBindTexture(GL_TEXTURE_2D, 0);


 // Front
  glBindTexture(GL_TEXTURE_2D, _tId_skybox[1]);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  if( (texSurface = SDL_LoadBMP("images/front.bmp")) == NULL ) {
    fprintf(stderr, "Impossible d'ouvrir le fichier : %s\n", SDL_GetError());
    exit(1);
  }
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texSurface->w, texSurface->h, 0, GL_BGR, GL_UNSIGNED_BYTE, texSurface->pixels); 
  glBindTexture(GL_TEXTURE_2D, 0);


 // left
  glBindTexture(GL_TEXTURE_2D, _tId_skybox[2]);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  if( (texSurface = SDL_LoadBMP("images/left.bmp")) == NULL ) {
    fprintf(stderr, "Impossible d'ouvrir le fichier : %s\n", SDL_GetError());
    exit(1);
  }
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texSurface->w, texSurface->h, 0, GL_BGR, GL_UNSIGNED_BYTE, texSurface->pixels); 
  glBindTexture(GL_TEXTURE_2D, 0);


 // rifht
  glBindTexture(GL_TEXTURE_2D, _tId_skybox[3]);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  if( (texSurface = SDL_LoadBMP("images/right.bmp")) == NULL ) {
    fprintf(stderr, "Impossible d'ouvrir le fichier : %s\n", SDL_GetError());
    exit(1);
  }
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texSurface->w, texSurface->h, 0, GL_BGR, GL_UNSIGNED_BYTE, texSurface->pixels); 
  glBindTexture(GL_TEXTURE_2D, 0);

 // top
  glBindTexture(GL_TEXTURE_2D, _tId_skybox[4]);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  if( (texSurface = SDL_LoadBMP("images/top.bmp")) == NULL ) {
    fprintf(stderr, "Impossible d'ouvrir le fichier : %s\n", SDL_GetError());
    exit(1);
  }
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texSurface->w, texSurface->h, 0, GL_BGR, GL_UNSIGNED_BYTE, texSurface->pixels); 
  glBindTexture(GL_TEXTURE_2D, 0);

  

  SDL_FreeSurface(texSurface);

  
SDL_Surface * texSurface_sol;
 /*GLfloat s_sol = 10.0,*/GLfloat data_sol[] = { 
  /* 4 coordonnées de sommets */
  -s, 0.0f, -s,
  s , 0.0f, -s,
  -s, 0.0f,  s,
  s , 0.0f,  s,
  /* 4 normales */
  0.0f, 1.0f, 0.0f, 
  0.0f, 1.0f, 0.0f,
  0.0f, 1.0f, 0.0f,
  0.0f, 1.0f, 0.0f,
  /* 4 coordonnées de texture, une par sommet */
  0.0f, 0.0f, 
  1.0f, 0.0f,
  0.0f, 1.0f,
  1.0f, 1.0f
};

glGenVertexArrays(1, &_vao_sol);
glBindVertexArray(_vao_sol);

glEnableVertexAttribArray(0);
glEnableVertexAttribArray(1);
glEnableVertexAttribArray(2);

glGenBuffers(1, &_buffer_sol);
glBindBuffer(GL_ARRAY_BUFFER, _buffer_sol);
glBufferData(GL_ARRAY_BUFFER, sizeof data_sol, data_sol, GL_STATIC_DRAW);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0);  
glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE,  0, (const void *)(4 * 3 * sizeof *data_sol));
glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (const void *)(4 * 6 * sizeof *data_sol));
glBindBuffer(GL_ARRAY_BUFFER, 0);
glBindVertexArray(0);


// Sol/ Balle

glGenTextures(1, &_tId_sol);
glBindTexture(GL_TEXTURE_2D, _tId_sol);
glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
if( (texSurface_sol = SDL_LoadBMP("images/noir.bmp")) == NULL ) {
  fprintf(stderr, "Impossible d'ouvrir le fichier : %s\n", SDL_GetError());
  exit(1);
 }
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texSurface_sol->w, texSurface_sol->h, 0, GL_BGR, GL_UNSIGNED_BYTE, texSurface_sol->pixels);

 
SDL_FreeSurface(texSurface_sol);



 
glBindTexture(GL_TEXTURE_2D, 0);
 
}


/*!\brief Cette fonction paramétrela vue (viewPort) OpenGL en fonction
 * des dimensions de la fenêtre SDL pointée par \a win.
 *
 * \param win le pointeur vers la fenêtre SDL pour laquelle nous avons
 * attaché le contexte OpenGL.
 */
static void resizeGL(SDL_Window * win) {
  SDL_GetWindowSize(win, &_windowWidth, &_windowHeight);
  glViewport(0, 0, _windowWidth, _windowHeight);
  gl4duBindMatrix("projectionMatrix");
  gl4duLoadIdentityf();
  gl4duFrustumf(-0.5, 0.5, -0.5 * _windowHeight / _windowWidth, 0.5 * _windowHeight / _windowWidth, 1.0, 1000.0);
}

/*!\brief Boucle infinie principale : gère les évènements, dessine,
 * imprime le FPS et swap les buffers.
 *
 * \param win le pointeur vers la fenêtre SDL pour laquelle nous avons
 * attaché le contexte OpenGL.
 */


int colision_balle(int k){
  
  int i , j  ;

  for(i = 0; i< MT; i++){
    for(j = 0 ; j< MT; j++){    
      if(laby[i][j]== -1){
	
	       
	    if(fabs( tab_balle[k].position[0] - ((i*20) )) < 10) 
	      if(fabs( tab_balle[k].position[1] - 10 ) < 10)
		if(fabs( tab_balle[k].position[2] - ((j*20) )) < 10){
		  printf("colision balle\n");

		  return 0;

		}
	  }
	}
      }
    
  
  
  for(i = 0; i< nb_monstre; i++){
    if(tab_monstre[i].vie < 1 ) continue;
    
    if(fabs(     tab_balle[k].position[0] - tab_monstre[i].position[0]) < 1 + 5)
      if(fabs(   tab_balle[k].position[1] - tab_monstre[i].position[1] ) < 1 + 5)
	if(fabs( tab_balle[k].position[2] - tab_monstre[i].position[2]) < 1 + 5){
	  printf("Tu a tire sur un monstre !!! \n");
	  tab_monstre[i].vie -= 34;
	  printf("Il lui reste %d HP!!! \n",tab_monstre[i].vie );
	  if(  tab_monstre[i].vie < 1) direction(i);

	  return 0;
	  
	}
    
    
  }
  

    
  
  return 1;
}

void mouvBalle (void ){
  int i;
    for(i = 0; i< nb_balle; i++){
      if(tab_balle[i].active){
	
      	tab_balle[i].active = colision_balle(i);
	if(tab_balle[i].position[1] >20 || tab_balle[i].position[1] < -5 )   	tab_balle[i].active = 0;
	addvecteur(tab_balle[i].position ,tab_balle[i].position , tab_balle[i].forward, 2.5);     
      }
    }   
}



void mouvMonstre (void ){
  int i = 0;
   for(i = 0; i< nb_monstre; i++){

     if(tab_monstre[i].vie < 0 ) {
       if(tab_monstre[i].position[1] < 200)
	 addvecteur(tab_monstre[i].position ,tab_monstre[i].position , tab_monstre[i].forward, 0.2);

       continue; 
     }
     if(tab_monstre[i].active == 0 ) {
       if(tab_monstre[i].position[1] > -50)
	 rmvecteur(tab_monstre[i].position ,tab_monstre[i].position , tab_monstre[i].forward, 0.2);
       continue; 
     }

  
        

    switch (tab_monstre[i].dir) {
    case 0 :
       if(laby[(int) (tab_monstre[i].position[0] / 20)+1 ][(int)(tab_monstre[i].position[2] / 20) ] != 0  )
       	 direction(i);

      addvecteur(tab_monstre[i].position ,tab_monstre[i].position , tab_monstre[i].forward, 1.5);
      break;     
    case 1 :
        if(laby[(int) (tab_monstre[i].position[0] / 20) ][(int)(tab_monstre[i].position[2] / 20) ] != 0  )
      	 direction(i);
      rmvecteur(tab_monstre[i].position ,tab_monstre[i].position , tab_monstre[i].forward,1.5);
      
      break;
    case 2:
      if(laby[(int) (tab_monstre[i].position[0] / 20) ][(int)(tab_monstre[i].position[2] / 20) +1] != 0  )
      	direction(i);
	        addvecteur(tab_monstre[i].position ,tab_monstre[i].position , tab_monstre[i].forward, 1.5);

      break;
    case 3:
       if(laby[(int) (tab_monstre[i].position[0] / 20) ][(int)(tab_monstre[i].position[2] / 20) ] != 0  )
       	direction(i);
        rmvecteur(tab_monstre[i].position ,tab_monstre[i].position , tab_monstre[i].forward, 1.5);

      break;
    }
    }
}



void colision( float x ,float  y , float z ,float  xf,float  yf , float zf){

  int i , j;

  for(i = 0; i< MT; i++){
    for(j = 0 ; j< MT; j++){

      
      if(laby[i][j] == -1   ) {
       
	if(fabs(getpos()[0] - ((i*20) )) < 1 +10) 
	  if(fabs(getpos()[1]- 10 ) < 1+ 10)
	    if(fabs(getpos()[2] - ((j*20) )) < 1 + 10){
	      getpos()[0] = x;
	      getpos()[1] = y;
	      getpos()[2] = z;
	      
	      gettarget()[0] = xf;
	      gettarget()[1] = yf;
	      gettarget()[2] = zf;
	      
	      return;
	
	    }
	    
      }
    
    }
  }


  for(i = 0; i< nb_monstre; i++){
    if(tab_monstre[i].vie < 1 ) continue;
    if(  tab_monstre[i].active == 0) continue;
    if(fabs(getpos()[0]     - tab_monstre[i].position[0]) < 1 +5)
      if(fabs(getpos()[1]   - tab_monstre[i].position[1] ) < 1+ 5)
	if(fabs(getpos()[2] - tab_monstre[i].position[2] ) < 1 + 5){

	  life -= 21;
	  printf("Attention  monstre !!! %d\n", life);
	  if(	  tab_monstre[i].active == 1){
	    tab_monstre[i].active = 0;
	  }
	  direction(i);
	  getpos()[0] = x ;
	  getpos()[1] = y;
	  getpos()[2] = z;
	  
	  gettarget()[0] = xf;
	  gettarget()[1] = yf;
	  gettarget()[2] = zf;
	  
	  return;
	  
	}



  }


}





static void loop(SDL_Window * win) {
  GLfloat a = 0.0 , dt = 0.0; //, dtheta = M_PI ; , pas = 5.0;
  int monte  = 1;
  Uint32 t0 = SDL_GetTicks(), t;
  SDL_GL_SetSwapInterval(1);
  SDL_SetRelativeMouseMode(SDL_TRUE);
  /* permet 1/ ne pas voir la souris mais surtout
            2/ que la sourie reste dans l ecran */

  /* Point de depart du perso */
  getpos()[1] = 10;
  getpos()[0] = 25;
  getpos()[2] = 25;
  laby[1][1] = 10; // Depart
  laby[MT-2][MT-2] = 12; // Arriver
    
 float b , c, cc , at , bt, ct;
  for(;;) {
    dt = ((t = SDL_GetTicks()) - t0) / 1000.0;
    t0 = t;

    cc =  getpos()[0];
    b =   getpos()[1];
    c =   getpos()[2];
    at =  gettarget()[0] ;
    bt =  gettarget()[1];
    ct =  gettarget()[2];



    
    manageEvents(win);
    /* 
       pour que les monstre bouge un peut haut bas

     */
    if (monte == 1 )
      if(hautbasmonstre >2)	monte = -1;
      else hautbasmonstre += 0.1;
    else if (monte == -1 )
      if ( hautbasmonstre < -2)	monte = 1;
      else	hautbasmonstre -= 0.1;



    
    keybardfuncplay(_keys[KUP], _keys[KDOWN],_keys[KLEFT], _keys[KRIGHT]);  
    colision(cc, b ,c , at, bt, ct);

    float * temp1 = getpos(); 
    position_arme[0] = temp1[0];
    position_arme[1] = temp1[1];
    position_arme[2] = temp1[2];
  
  
    temp1 = getforward(); 
    forward_arme[0]	= temp1[0];
    forward_arme[1]	= temp1[1];	
    forward_arme[2]	= temp1[2];
    addvecteur( position_arme ,  position_arme,  forward_arme, 2.5);
    
    if(laby[(int)(getpos()[0] / 20)][ (int)(getpos()[2] / 20) ] == 12 ){

      printf("Bravo !!!!!!!\nVous avez reussi a trouver la sortie\n");
      return ;
    }
    if( life < 1){
      printf("Vous ete mort\n\n");
      return ;
  }
   
    mouvBalle();
    mouvMonstre();

   
    
    draw(a + 0);
    gl4duPrintFPS(stderr);
    SDL_GL_SwapWindow(win);
    gl4duUpdateShaders();
    if(!_pause)
      a += 0.1 * 2.0 * M_PI * dt;
  }
}

/*!\brief Cette fonction permet de gérer les évènements clavier et
 * souris via la bibliothèque SDL et pour la fenêtre pointée par \a
 * win.
 *
 * \param win le pointeur vers la fenêtre SDL pour laquelle nous avons
 * attaché le contexte OpenGL.
 */
static void manageEvents(SDL_Window * win) {
  SDL_Event event;
  while(SDL_PollEvent(&event)) 
    switch (event.type) {
    case SDL_KEYDOWN:
      switch(event.key.keysym.sym) {
      case SDLK_LEFT:
	_keys[KLEFT] = 1;
	break;
      case SDLK_RIGHT:
	_keys[KRIGHT] = 1;
	break;
      case SDLK_UP:
	_keys[KUP] = 1;
	break;
      case SDLK_DOWN:
	_keys[KDOWN] = 1;
	break;
      case SDLK_ESCAPE:
      case 'q':
	exit(0);
      case ' ':
	getpos()[1] = 100;
	break;
      case 'h':
	help = 1;
	break;
      case 'p':
	_pause = !_pause;
	break;
	case 't':
        tir = 1;
	break;
      default:
	fprintf(stderr, "La touche %s a ete pressee\n",
		SDL_GetKeyName(event.key.keysym.sym));
	break;
      }
      break;
    case SDL_KEYUP:
      switch(event.key.keysym.sym) {
      case SDLK_LEFT:
	_keys[KLEFT] = 0;
	break;
      case SDLK_RIGHT:
	_keys[KRIGHT] = 0;
	break;
      case SDLK_UP:
	_keys[KUP] = 0;
	break;
      case SDLK_DOWN:
	_keys[KDOWN] = 0;
	break;
      case ' ':
	getpos()[1] = 10;
	break;
	case 'h':
	  help = 0;
	  break;
      
      }
      break;
      
    case SDL_MOUSEBUTTONDOWN:
      if (event.button.button == SDL_BUTTON_LEFT)
	{
       	float * temp1 = getpos(); 
      	tab_balle[nb_balle].position[0] = temp1[0];
	tab_balle[nb_balle].position[1] = temp1[1];
	tab_balle[nb_balle].position[2] = temp1[2];

	temp1 = getforward(); 
	tab_balle[nb_balle].forward[0] = temp1[0];
	tab_balle[nb_balle].forward[1] = temp1[1];	
	tab_balle[nb_balle].forward[2] = temp1[2];
	
	tab_balle[nb_balle].active   = 1;
	      
        nb_balle++;
	if(nb_balle > 19) nb_balle = 0;

	}
      break;
      
      
    case SDL_MOUSEMOTION:
      mouss(event.motion.xrel, event.motion.yrel);
      break;
    case SDL_WINDOWEVENT:
      if(event.window.windowID == SDL_GetWindowID(win)) {
	switch (event.window.event)  {
	case SDL_WINDOWEVENT_RESIZED:
	  resizeGL(win);
	  break;
	case SDL_WINDOWEVENT_CLOSE:
	  event.type = SDL_QUIT;
	  SDL_PushEvent(&event);
	  break;
	}
      }
      break;
    case SDL_QUIT:
      exit(0);
    }
}

/*!\brief Cette fonction dessine dans le contexte OpenGL actif.
 */
static void draw(GLfloat a0) {
  GLfloat * mv, temp[4] = {5 * sin(a0), 0.5+50, -5, 1.0}, lumpos[4];
  //    GLfloat * mv, temp[4] = {-10,-10, -5, 1.0}, lumpos[4];

  int i , j;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram(_pId);
  glActiveTexture(GL_TEXTURE0);
  //  glBindTexture(GL_TEXTURE_2D, _tId_mur);
  glUniform1i(glGetUniformLocation(_pId, "myTexture"), 0);

  gl4duBindMatrix("modelViewMatrix");
  gl4duLoadIdentityf();
  /* Avec des rotate et translate faire :
     gl4duRotatef(-_cam.theta * 180.0f / M_PI, 0.0, 1.0, 0.0);
     gl4duTranslatef(-_cam.x, -1.0, -_cam.z);
     A la place du LookAt */

  gl4duLookAtf(getpos()[0]    , getpos()[1]   , getpos()[2],
	       gettarget()[0] , gettarget()[1], gettarget()[2],
	       0.0, 1.0, 0.0);

  mv = gl4duGetMatrixData();
  MMAT4XVEC4(lumpos, mv, temp);
  glUniform4fv(glGetUniformLocation(_pId, "lumpos"), 1, lumpos);
  
  gl4duSendMatrices();

  /* 
     Affichage de la Skybox 

     Problemme 2 face mauvaise rotation !!!!!!!!!
  */
 
  //top
  gl4duPushMatrix();
  gl4duTranslatef(200, 0,  200);
  gl4duScalef(40, 40, 40);
  glBindTexture(GL_TEXTURE_2D, _tId_skybox[4]);
  glUniformMatrix4fv(glGetUniformLocation(_pId, "modelViewMatrix"), 1, GL_TRUE, gl4duGetMatrixData());  
  glBindVertexArray(_vao);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
  gl4duPopMatrix();
  
  gl4duPushMatrix();
  gl4duTranslatef( 200, 00,  200);
  gl4duScalef(40, 40, 40);
  glBindTexture(GL_TEXTURE_2D, _tId_skybox[3]);
  glUniformMatrix4fv(glGetUniformLocation(_pId, "modelViewMatrix"), 1, GL_TRUE, gl4duGetMatrixData());  
  glBindVertexArray(_vao);
  glDrawArrays(GL_TRIANGLE_STRIP, 4, 8);
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
  gl4duPopMatrix();
  
  gl4duPushMatrix();
  gl4duTranslatef( 200, 0,  200);
  gl4duScalef(40, 40, 40);
  glBindTexture(GL_TEXTURE_2D, _tId_skybox[2]);
  glUniformMatrix4fv(glGetUniformLocation(_pId, "modelViewMatrix"), 1, GL_TRUE, gl4duGetMatrixData());  
  glBindVertexArray(_vao);
  glDrawArrays(GL_TRIANGLE_STRIP, 8, 12);
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
  gl4duPopMatrix();

 

   /* 
     Affichage des balles
  */
  
  for(i = 0; i< nb_balle; i++){
 
    if (tab_balle[i].active == 0) continue; 
  
      
    gl4duPushMatrix();
    gl4duTranslatef( tab_balle[i].position[0] ,  tab_balle[i].position[1],  tab_balle[i].position[2] );

    gl4duScalef(0.005, 0.005, 0.005);
    glBindTexture(GL_TEXTURE_2D, _tId_sol);
    glUniformMatrix4fv(glGetUniformLocation(_pId, "modelViewMatrix"), 1, GL_TRUE, gl4duGetMatrixData());
  
    glBindVertexArray(_vao);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 20);
    glBindVertexArray(0);
    gl4duPopMatrix();
  }

  /* 
     Affichage des monstres
  */ 
  for(i = 0; i< nb_monstre; i++){
 
    // if (tab_monstre[i].vie < 0) continue; 
    if (tab_monstre[i].position[1] > 200) continue ;
    if (tab_monstre[i].position[1] < -50) continue ;
    gl4duPushMatrix();
    gl4duTranslatef( tab_monstre[i].position[0] ,  tab_monstre[i].position[1] + hautbasmonstre ,  tab_monstre[i].position[2] );
   
    gl4duScalef(0.3, 0.5, 0.3);
    glBindTexture(GL_TEXTURE_2D, _tId_fantome);
    glUniformMatrix4fv(glGetUniformLocation(_pId, "modelViewMatrix"), 1, GL_TRUE, gl4duGetMatrixData());
  
    glBindVertexArray(_vao);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 20);
    glBindVertexArray(0);
    gl4duPopMatrix();
  }



  /*
     Affichage du  viseur (tjr devant soi)
     a modifier probleme
  */

  gl4duPushMatrix();
  gl4duTranslatef( position_arme[0],  position_arme[1]  ,  position_arme[2] );

  gl4duRotatef(rotate, 1.0, 0.0, 0.0);
  gl4duRotatef(rotate++, 0.0, 1.0, 0.0);
  gl4duScalef(0.003, 0.003, 0.003);
  glBindTexture(GL_TEXTURE_2D, _tId_sol);
  glUniformMatrix4fv(glGetUniformLocation(_pId, "modelViewMatrix"), 1, GL_TRUE, gl4duGetMatrixData());
  glBindVertexArray(_vao_sol);
  glDrawArrays(GL_TRIANGLE_STRIP, 0,4);
  glBindVertexArray(0);
  gl4duPopMatrix();

  gl4duPushMatrix();
  gl4duTranslatef( position_arme[0],  position_arme[1]  ,  position_arme[2] );
  gl4duRotatef(rotate, 1.0, 0.0, 0.0);
  gl4duRotatef(rotate++, 0.0, 1.0, 0.0);
  gl4duRotatef(90, 1.0, 0.0, 0.0);
  gl4duScalef(0.003, 0.003, 0.003);

 
  glBindTexture(GL_TEXTURE_2D, _tId_sol);
  glUniformMatrix4fv(glGetUniformLocation(_pId, "modelViewMatrix"), 1, GL_TRUE, gl4duGetMatrixData());
  glBindVertexArray(_vao_sol);
  glDrawArrays(GL_TRIANGLE_STRIP, 0,4);

  
  glBindVertexArray(0);
  gl4duPopMatrix();

   
  
  /* 
     Affichage de la Map ( mur ou sol ) 
   
  */
 
  for(i = 0; i< MT; i++){
    for(j = 0 ; j< MT; j++){
      
      gl4duPushMatrix();
      gl4duTranslatef((i*20) , 0, (j*20) ); 
      glUniformMatrix4fv(glGetUniformLocation(_pId, "modelViewMatrix"), 1, GL_TRUE, gl4duGetMatrixData());
  
      if(laby[i][j] == -1){	// mur
	glBindTexture(GL_TEXTURE_2D, _tId_mur);
	glBindVertexArray(_vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 20);
      }
         
      else if(laby[i][j] == 0){ // sol
	glBindTexture(GL_TEXTURE_2D, _tId_balle);
	glBindVertexArray(_vao_sol);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	if (help == 1) { // Si aide
	  gl4duPopMatrix();

	  gl4duPushMatrix();      
	  gl4duTranslatef((i*20) , 200, (j*20) );
	  glUniformMatrix4fv(glGetUniformLocation(_pId, "modelViewMatrix"), 1, GL_TRUE, gl4duGetMatrixData());
      	   
	  if((int)(getpos()[0] / 20) == i && (int)(getpos()[2] / 20)  == j ) // la ou je suis 
	    glBindTexture(GL_TEXTURE_2D, _tId_balle);
	  else
	    glBindTexture(GL_TEXTURE_2D, _tId_sol);
	  
	  glBindVertexArray(_vao_sol);
	  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	  
	  glBindVertexArray(0);
	  gl4duPopMatrix();	  
	}	
      }   
      glBindVertexArray(0);
      gl4duPopMatrix();
  
    
    }
  }

  /*
    arriver et depart 
  */
  
  gl4duPushMatrix();
  gl4duTranslatef((1*20) , 0, (1*20) ); 
  glUniformMatrix4fv(glGetUniformLocation(_pId, "modelViewMatrix"), 1, GL_TRUE, gl4duGetMatrixData());
  glBindTexture(GL_TEXTURE_2D, _tId_depart);
  glBindVertexArray(_vao_sol);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray(0);
  gl4duPopMatrix();
  
  gl4duPushMatrix();
  gl4duTranslatef(( (MT-2)*20) , 0, ( (MT-2)*20) ); 
  glUniformMatrix4fv(glGetUniformLocation(_pId, "modelViewMatrix"), 1, GL_TRUE, gl4duGetMatrixData());      
  glBindTexture(GL_TEXTURE_2D, _tId_arriver);
  glBindVertexArray(_vao_sol);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray(0);
  gl4duPopMatrix();
  
  if (help == 1) { 
    gl4duPushMatrix();
    gl4duTranslatef((1*20) , 200, (1*20) ); 
    glUniformMatrix4fv(glGetUniformLocation(_pId, "modelViewMatrix"), 1, GL_TRUE, gl4duGetMatrixData());
    glBindTexture(GL_TEXTURE_2D, _tId_depart);
    glBindVertexArray(_vao_sol);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
    gl4duPopMatrix();
  
    gl4duPushMatrix();
    gl4duTranslatef(( (MT-2)*20) , 200, ( (MT-2)*20) ); 
    glUniformMatrix4fv(glGetUniformLocation(_pId, "modelViewMatrix"), 1, GL_TRUE, gl4duGetMatrixData());      
    glBindTexture(GL_TEXTURE_2D, _tId_arriver);
    glBindVertexArray(_vao_sol);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
    gl4duPopMatrix();

  }
  glBindTexture(GL_TEXTURE_2D,0);

  
}


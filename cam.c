#include<math.h>


static float position[3];
static float target[3];

static float forward[3];
static float left[3];

static float theta = 0.0f;
static float phi = 0.0f;

void addvecteur( float * res , float * vect1 , float * vect2, float speed){

  res[0] = vect1[0] + vect2[0] * speed;
  res[1] = vect1[1] + vect2[1] * speed;
  res[2] = vect1[2] + vect2[2] * speed;

}

void rmvecteur( float * res , float * vect1 , float * vect2, float speed){

  res[0] = vect1[0]- vect2[0] * speed;
  res[1] = vect1[1]- vect2[1] * speed;
  res[2] = vect1[2]- vect2[2] * speed;

}

void normalisevect( float * vect ){
  
  float tmp = sqrt(vect[0] * vect[0] + vect[1] * vect[1] + vect[2] * vect[2]);
  
  vect[0] /= tmp;
  vect[1] /= tmp;
  vect[2] /= tmp;
  
  
}

void mouss( int x, int y){  
  theta -= (float)x * 0.25f * M_PI / 180;
  phi   -= (float)y * 0.25f * M_PI / 180;
  
  if ( phi >=  M_PI / 2) phi = 89.0f * M_PI / 180;
  else if (phi <= - M_PI  / 2) phi = -89.0f * M_PI / 180;

  float tmp = cos(phi);
  forward[0] = tmp * sin(theta);
  forward[1] = sin( phi);
  forward[2] = tmp * cos( theta);
  
  left[0] = forward[2];
  left[1] = 0;
  left[2] = -forward[0];
  
  normalisevect(left);
  addvecteur(target , position, forward, 1.0f);  
}

void keybardfunclibre(int f , int b , int l , int r){

  float speed = 1.0;
  if (f)
    addvecteur(position , position , forward, speed);   
  if (b)
    rmvecteur(position , position , forward,  speed);
  if (l)
    addvecteur(position, position, left, speed );
  if(r)
    rmvecteur(position, position, left,  speed);
   
  addvecteur(target , position, forward, 1.0f);       
}

void keybardfuncplay(int f , int b , int l , int r){
  float speed = 1.0;

  if(f)
    {
      position[0] -= left[2] *   speed;
      position[2] += left[0] *  speed;
    }
  if(b)
    {
      position[0] += left[2] * speed ;
      position[2] -= left[0] * speed;
    }
  if (l)
    addvecteur(position, position, left, speed );
  if(r)
    rmvecteur(position, position, left,  speed);
   
  addvecteur(target , position, forward, 1.0f);       
}

float * getpos(void){
  return position;

}
float * gettarget(void){
  return target;

}
float * getforward(void){
  return forward;

}

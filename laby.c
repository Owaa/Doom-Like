#include<stdio.h>
#include<stdlib.h>
#include"laby.h"


 

void init_laby(void){
  
  int i , j , acc = 0;
  
  for (i = 0; i <  MT ; i++){
    for (j = 0; j < MT; j++){
      if((i&1) && (j&1))      
	laby[i][j] = acc++;
      else
      	laby[i][j] = -1;
    }
  }
  
}

void creat_laby(void){
  int x , y ;
  //int h =0, v=0 ,old_value = 0 , ob;
  int  nb_point_laby = 0;
  printf("--------%d  = nb\n",  nb_point_laby  );

  int delete;
  int voisin =0;
  while (    nb_point_laby < M*M-1){
    // Rand a modifier randMax ...
    // Je choisi une point aleatoire impaire( noueud)
    x = (rand()% M ) *2 +1 ;
    y = (rand()% M ) *2 +1 ;

    voisin = rand() %4;// je choisin un voisin
    
    switch(voisin){

    case 0 : // Haut
      if( (y - 2) > 0 ) { // Si je sort pas
	if(  laby[x][y] != laby[x][ y-2]){ // je regarde si son est different
	  if(  laby[x][y] < laby[x][ y-2]){// si je suis plus petit que mon voisin je me propage
	    delete =  laby[x][y-2];
	    laby[x][y-1] = laby[x][ y];
	    laby[x][y-2] = laby[x][ y];
	    propage(laby[x][ y],delete , &nb_point_laby );
	  }
	  else 	{
	    delete =  laby[x][y];

	    laby[x][ y-1]= laby[x][y-2] ;
	    laby[x][ y]= laby[x][y-2];
	    propage(laby[x][y-2],delete, &nb_point_laby );
	  }
	  
	  break;
	}
      }
    case 1 : // Bas
      if( (y + 2)  < MT ) { // Si je sort pas
	if(  laby[x][y] != laby[x][ y+2]){ // je regarde si son est different
	 
	  if(  laby[x][y] < laby[x][ y+2]){
	    delete =  laby[x][y+2];

	    laby[x][y+1] = laby[x][ y];
	    laby[x][y+2] = laby[x][ y];
	    propage(laby[x][ y],delete , &nb_point_laby );
	  }
	  else 	{
	    delete =  laby[x][y];

	    laby[x][ y+1]= laby[x][y+2] ;
	    laby[x][ y]= laby[x][y+2];
	    propage(laby[x][y+2],delete, &nb_point_laby );
	  }
	  break;
	}
      }
      
    case 2 : // Droite
      if( (x + 2)  < MT ) { // Si je sort pas
	if(  laby[x][y] != laby[x +2 ][ y]){ // je regarde si son est different
        

	  if(  laby[x][y] < laby[x+2][ y]){
	    delete =  laby[x+2][y];
		    
	    laby[x+1][y] = laby[x][ y];
	    laby[x+2][y] = laby[x][ y];
	    propage(laby[x][ y],delete , &nb_point_laby );
	  }
	  else 	{
	    delete =  laby[x][y];

	    laby[x+1][ y]= laby[x+2][y] ;
	    laby[x][ y]= laby[x+2][y];
	    propage(laby[x+2][y],delete, &nb_point_laby );
	  }
	
	  break;
	}
      }
      
    case 3 : //Gauche
      if( (x - 2) > 0 ) { // Si je sort pas
	if(  laby[x][y] != laby[x -2 ][ y]){ // je regarde si son est different
	  //laby[x-1][y] = laby[x][ y];
	  // laby[x-2][y] = laby[x][ y];
	  //nb_point_laby--;

	  if(  laby[x][y] < laby[x-2][ y]){
	     delete =  laby[x-2][y];

	    laby[x-1][y] = laby[x][ y];
	    laby[x-2][y] = laby[x][ y];
	    propage(laby[x][ y],delete , &nb_point_laby );
	  }
	  else 	{
	    delete =  laby[x][y];

	    laby[x-1][ y]= laby[x-2][y] ;
	    laby[x][ y]= laby[x-2][y];
	    propage(laby[x-2][y],delete, &nb_point_laby );
	  }
	
	  break;
	}
      }
    }

  }
}
    

   

void propage(int propage , int delete , int * nb){

int i , j;
	if(propage == 0)  (*nb)++;
  for (i = 0; i <  MT ; i++){
    for (j = 0; j < MT; j++){
      if( laby[i][j] == delete)  {    
	laby[i][j] = propage;
      	if(laby[i][j] == 0 && ( (i&1) && (j&1)) )  (*nb)++;
      }
    }
  }
  
  
}
 


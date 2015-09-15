#include<stdio.h>
#include<stdlib.h>
#include"laby.h"




void init_laby(void){
  
  int i , j , acc = 1;
  for (i = 0; i <  MT ; i++){
    for (j = 0; j < MT; j++){
      laby[i][j] = -1;	
    }
  }

  /*
  
  for (i = 0; i <  MT ; i++)
     laby[0][i] = -10;
  
  for (i = 0; i <  MT ; i++)
     laby[MT][i] = -10;
  
  for (i = 0; i <  MT ; i++)
    laby[MT][i] = -10;
  
  for (i = 0; i <  MT ; i++)
    laby[i][MT] = -10;	

  */



  
  for (i = 1; i < MT; i += 2){
    for (j = 1; j < MT; j+=2){
      laby[i][j] = acc++;
    }
  }
  laby[3][1] = 2;
  laby[3][3] = 2;
  laby[3][5] = 2;
  laby[3][7] = 2;
  laby[3][9] = 2;
  laby[1][5] = 2;
}

void creat_laby(void){
  int x , y ;
  int h =0, v=0 ,old_value = 0 , ob, direction = 0;;
 
  int  nb_point_laby = M*M;
   while (    nb_point_laby !=1){

  x = (rand()% 5 ) *2 +1 ;
  y = (rand()% 5 ) *2 +1 ;
 

  ob = rand()%  2;

  if(ob ==0)
    h =  (rand()%2)*2 -1;
  else
    v =  (rand()%2)*2 -1;

  if(h ==-1) direction = 4;
  if(h == 1) direction = 2;

  
  if( x+ h*2 > MT-1 ||x+ h*2< 0 ||  y+ v*2 > MT-1 ||  y+ v*2< 0 )continue ;
  
  if(h ==-1) direction = 4;
  else if(h == 1) direction = 2;
  if(v ==-1) direction = 1;
  else if(v == 1) direction = 3;
  
  
  if( laby[x][y] == laby[x + h*2][y + v*2] ){printf("  %d    %d\n",laby[x][y],  laby[x + h*2][y + v*2]   );  continue;} 
 
 
 
  
  if( laby[x][y] < laby[x + h*2][y + v*2] ){
    printf("ENtre dans le test plsu petir\n\n"  );

    
    laby[x][y] = laby[x + h*2][y + v*2];
    laby[x+h][y+v] = laby[x + h*2][y + v*2];
    nb_point_laby--;
     continue;

  
    // return;
  }
  printf("Apres le Ir hor = %d   , vert = %d  \n\n" , h , v   );


  old_value =laby[x + h*2][y + v*2] ;
  printf("oldvalue %d  x = %d  y = %d \n\n",old_value , x + h*2 ,y + v*2   );

  laby[x + h][y + v] = laby[x][y] ;//laby[x][y];
  laby[x + h*2][y + v*2] =  laby[x][y];//laby[x][y];
  nb_point_laby--;

    printf("ENtre dans Propage \n\n"  );

    propage(x + h* 2,y + v*2 ,old_value , direction);
    printf("Sort de Propage \n\n"  );

  
   }



   
    //   printf("",old_value ,  laby[x + h*2][y + v*2]   );

}

void propage(int x , int y , int val, int dir){
    printf(" -------- %d %d \n\n", x , y   );

  //gauche
  if(dir!= 4 &&  x -2 >0 && laby[x-2][y] == val){
    

    laby[x -1][y] =  laby[x][y];
    laby[x -2][y] = laby[x][y];
    //    nb_point_laby--;

    propage(x-2,y  ,val,2);

  }

  // droite
  if (dir!= 2 &&  x+2 < MT-1 && laby[x+2][y] == val){
    
    laby[x +1][y] = laby[x][y];
    laby[x +2][y] = laby[x][y];
    //  nb_point_laby--;

    propage(x+2,y  ,val , 4);

  }

  //haut

  if (dir!= 1 &&  y-2 >0 &&laby[x][y-2] == val){
    
    laby[x][y - 1] = laby[x][y];
    laby[x][y - 2] = laby[x][y];
    // nb_point_laby--;

    propage(x,y-2,val, 3);

  }
  
  
  //bas

  

  if (dir!= 3 && y+2 < MT-1 && laby[x][y+2] == val){
    
    laby[x][y + 1] = laby[x][y];
    laby[x][y + 2] = laby[x][y];
    //  nb_point_laby--;

    propage(x,y+2,val,1);

  }
  else return;
}


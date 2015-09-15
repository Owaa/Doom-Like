

struct cell{ 
  int nb ;

  int haut, bas , droite, gauche;



};
#define M 10
#define MT ((M*2)+1)


typedef struct cell cell ;


int laby[MT+1][MT+1];

void init_laby();
void creat_laby();
void propage(int x , int y , int *nb);

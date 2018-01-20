#include "syscall.h"

void produire(){
     PutString("un producteur a produit un objet\n\n");
}

void envoyer(int *ob){
     PutString("un producteur a envoye un objet\n");
     *ob += 1;
     PutString("Il y a ");
     PutInt(*ob);
     PutString(" objets\n\n");
}

void consommer(int *ob){
     PutString("le consommateur a consomme un objet\n");
     *ob -= 1;
     PutString("Il y a ");
     PutInt(*ob);
     PutString(" objets\n\n");
}

void utiliser(){
     PutString("le consommateur a utiliser un objet\n\n");
}

void producteur(void *tab){

     int *tableau = (int *)tab;

     while(1){
	  //produire();
	  P((int)(tableau[1]));
	  P((int)(tableau[0]));
	  envoyer((int *)(tableau[3]));
	  V((int)(tableau[0]));
	  V((int)(tableau[2]));
     }
}

void consommateur(void *tab){

     int *tableau = (int *)tab;

     while(1){
	  P((int)(tableau[2]));
	  P((int)(tableau[0]));
	  if(*(int *)tableau[3] != 0){
	       consommer((int *)(tableau[3]));
	  }
	  V((int)(tableau[0]));
	  V((int)(tableau[1]));
	  //utiliser();
     }
}


int main() {

     int objets = 0;
     int tab[4];

     PutString("Debut du test\n nombre d'objets = ");
     PutInt(objets);
     PutString("\n\n");

     tab[0] = SemaphoreCreate(1);
     tab[1] = SemaphoreCreate(1);
     tab[2] = SemaphoreCreate(1);
     tab[3] = (int)&objets;

     ThreadCreate(producteur, (void *) tab);
     ThreadCreate(producteur, (void *) tab);
     ThreadCreate(producteur, (void *) tab);
     ThreadCreate(consommateur, (void *) tab);
     ThreadCreate(consommateur, (void *) tab);
     ThreadCreate(consommateur, (void *) tab);

     return 0;
}

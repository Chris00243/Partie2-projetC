#include<stdlib.h>
#include<stdio.h>
#include"Struct_Liste.h"

void Init_Liste(Cell_entier**L){
  *L=NULL;
}


int estLileVide(Cell_entier *L){
  return (L==NULL);
}

void ajoute_en_tete(Cell_entier **L, int u){
 Cell_entier *nouvsom=(Cell_entier*) malloc(sizeof(Cell_entier));
  nouvsom->u=u;
  nouvsom->suiv=*L;
  *L=nouvsom;
}


void desalloue(Cell_entier **L){
  Cell_entier *cour, *prec;
  cour=*L;
  while(cour!=NULL){
    prec=cour;
    cour=cour->suiv;
    free(prec);
  }
  *L=NULL;
}

// fonction ajoutée : affichage de ListeEntier

void afficherListeEntier(Cell_entier *L)
{
	if(L==NULL) printf("\nPAS DE LISTE");
	else{
		printf("\n Liste :");
		
		

		while(L){

			printf(" %d", L->u);
			L = L->suiv;
		}
	}

	printf("\n\n");

}

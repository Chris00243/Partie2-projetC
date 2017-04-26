#ifndef _STRUCT_LISTE_H
#define _STRUCT_LISTE_H

/* Structure d une file contenant un entier */

typedef struct cell_entier{
  int u;
  struct cell_entier *suiv;
} Cell_entier;

//typedef Cell_entier ListeEntier;

/* Initialisation d une file */
void Init_Liste( Cell_entier **L);

/* Teste si la file est vide */
int estLileVide(Cell_entier *L);

/* Ajoute un element don`e en tete de liste */
void ajoute_en_tete(Cell_entier** L, int u);

/* Vide et desalloue une liste */
void desalloue(Cell_entier **L);


// fonction ajoutée : affichage de ListeEntier
void afficherListeEntier(Cell_entier* L);

#endif 

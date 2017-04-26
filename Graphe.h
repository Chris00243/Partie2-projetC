#ifndef __GRAPHE_H__
#define __GRAPHE_H__
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"Graphe.h"
#include"SVGwriter.h"
#include"entree_sortie.h"
#include"Struct_Liste.h"
#include"Struct_File.h"


typedef struct{
  int u,v;        /* Numeros des sommets extremité */
  double longueur;    /* longueur de l arete */

 /**************************************************/
 /* Emplacement des donnees utiles aux algorithmes */

  int calc_gamma; 

 /**************************************************/

} Arete;


typedef struct cellule_arete{
  Arete *a;     /* pointeur sur l'arete */
  struct cellule_arete *suiv;
} Cellule_arete;


typedef struct {
  int num;                 /* Numero du sommet (le meme que dans T_som) */
  double x, y;
  Cellule_arete* L_voisin;  /* Liste chainee des voisins */

 /**************************************************/
 /* Emplacement des donnees utiles aux algorithmes */

 

 /**************************************************/

} Sommet;


typedef struct{
  int e1,e2;    /* Les deux extremites de la commodite */
} Commod;


typedef struct{
  int nbsom;        /* Nombre de sommets */
  Sommet* *T_som;    /* Tableau de pointeurs sur sommets */
  int gamma;     

  int nbcommod;     /* Nombre de commodites */
  Commod *T_commod; /* Tableau des commodites */
} Graphe;


void ajout_voisin(Graphe* G, int u, int v);

/* retourne un pointeur sur l'arete uv si elle existe
   et NULL sinon */
Arete* acces_arete(Graphe* G, int u, int v);

void lecture_graphe(Graphe *G, FILE * f);

void affichageGrapheSVG(Graphe *G, char* nomInstance);


// fonction de la question 7.2 : fonction codée
int plus_petit_nbr_arete(Graphe *G, int u, int v);

//fonction 7.3 et 7.4 : la transformation de la fonction 7.2 Je propose de stocker l'aborescence issue de u créant un tableau des précédents
Cell_entier* chemin(Graphe *G, int u, int v);

// fonction : question 7.4 et 7.5
void ecrire_file(char* filenamencha, Graphe* G);


#endif

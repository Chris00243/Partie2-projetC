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
#include "evaluation_NChaines.h" // j'ai ajouté

#define MAX 25

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


/*************** Pour Dijisktra et consort : les ajouts **************************/

/* Element dans le tas */

typedef struct element Element;
struct element{
	
	int i; // le numéro du sommet ( correspond à num dans la structure Sommet)

	double c; // la distance ( correspond à la variable longueur dans la structure Arête)

};

/* Le tas  */

typedef struct tas Tas;
struct tas{
	
	Element *tab; // tableau d'Elements
	int n; // nombre d'Elements dans tab

	int *Marque; // tableau pour marquer la présence de l'Element d'indice i ( i de Element)
		     // 1 si l'Element est là, -1 s'il est passée par là, 0 s'il n'est jamais passée par là

	// la taille Max de tab est definie par un macro MAX que j'ai initialisée à 25 dans Graphe.h
};



/* initialisation du tas */
void Init_Tas( Tas** T);

/***************** Quelques fonctions indispensables ********************** */


/* Fonction d'entassement : pour déplacer de 1 les valeurs entre i et j avec j = *T->n */
void avancer(Tas** T, int i, int j);

/* Fonction d'entassement : pour déplacer de -1 les valeurs entre i et j avec j = *T->n */
void reculer(Tas** T, int i, int j);

/* insertion dun Element dans un tas */
void insert(Tas** T, Element E);

/* Tester la présence d'un Element dans le tas */
int test(Tas* T, int i);

/* Accès à l'Element ayant le plus petit c */
Element* petit_c(Tas*);

/* Suppresion de l'Element ayant le plus petit c */
void delecte(Tas** T);

/* suppresion de l'Elent d'indice i s'il existe */
void suppression(Tas**T, int i);

/*  Fonction DIJIKSTRA : retourne la longueur (longueur totale) de la chaine de e1 à e2 */
double dijikstra(Graphe* G, int e1, int e2);

/* Permet d'écrire les évaluations des commodités dans un fichier avec extension .eval  Ce fichier contiendra par ligne longueur ( de Dijikstra) evaluation. Donc deux valeurs par ligne */

void ecrire_eval(Graphe *G, char* fichier, char* fic_eval ); // fic_eval correspond au fichier pour utiliser la fonction evaluation



#endif

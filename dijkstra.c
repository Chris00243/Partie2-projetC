#include <stdio.h>

#include <stdlib.h>

#define TAILLE 5 /* nombre de sommets du graphe */

#define MAXINT 1000 /* un tres grand entier */



typedef struct cellule	/* noeud, poids et pointeur */
{	int numero;
	
							int poids;

							struct cellule *suivant;
} Cellule, *LISTE;


LISTE graphe[TAILLE];	/* graphe = tableau de listes */

int D[TAILLE];	/* Les distances trouvees a chaque instant */

int queue[TAILLE]; /* file pour les sommets selon D croissant */

int ordre[TAILLE]; /* Ordre de chaque sommet dans la file selon D croissant */

int explore[TAILLE]; /* Pour les sommets deja ajoutes */

int C[TAILLE];	/* pour le predecesseur de chaque sommet ajoute */



/* Alloue un noeud sans initialiser */

Cellule *AlloueCellule()
{
  
Cellule *cell = (Cellule *) malloc(sizeof(Cellule));
  
return cell;
}



/* Cree un noeud en l'initialisant avec deux valeurs. */

Cellule *CreeCellule(int cle, int lon)
{
  
Cellule *n = AlloueCellule();
  
n->numero = cle;
  
n->poids = lon;
  
n->suivant = NULL;
  
return n;
}



/* Initialisation des diverses variables */

void initialisation(void)
{
  
int i;
  
for(i = 0; i<TAILLE; i++)
{
    
	D[i] = MAXINT; 
	queue[i] = i; 
	ordre[i] = i; 
	explore[i] = 0;
  }
  
D[0] = 0; 
C[0] = -1;	
/* On commence par 0 */
}



/* Operation sur les files d'attente */

void echanger(int i, int j)/* echanger deux sommets dans la queue */
{
  
int a;
  
ordre[queue[i]] = j; 
ordre[queue[j]] = i;

a = queue[i]; 
queue[i] = queue[j];
queue[j] = a;
}



/* retablit la condition de file de maniere descendante sur les longueur positions de la file */

void echange_descendant(int longueur, int i)
{
  
int fils=2*i+1;
  
if(fils>=longueur) return;
  
if((fils<longueur-1)&&(D[queue[fils]]>D[queue[fils+1]])) fils=fils+1;
  
if(D[queue[i]]>D[queue[fils]])
   {
      echanger(i,fils);

      					echange_descendant(longueur, fils);
   }

}



/* retablit la condition de file de maniere ascendante sur les longueur positions de la file */

void echange_montant(int longueur, int i)
{
 
int pere=(i-1)/2;
  
if(D[queue[i]]<D[queue[pere]])
 {
 echanger(pere,i);
      
				echange_montant(longueur, pere);
   }

}



/* retourne le minimum dans la file et retablit l'ordre */

int minimum(int longueur)
{
  
int a = queue[0];

explore[a] = 1;
  
if(longueur > 1)
  {
    
echanger(0, longueur-1);
    
echange_descendant(longueur-1, 0);
  }
  
return a;
}



/* algorithme de Dijkstra. Premiere etape, on ajoute un sommet, puis on actualise */

void dijkstra_iteration(int longueur)	/* nombre de sommets non encore inclus */
{
  Cellule *c;
  int a = minimum(longueur);
/* extraction du minimum */
  
int j;

for(c=graphe[a]; c != NULL; c=c->suivant)
{

j = c->numero;

if((explore[j] == 0)&&(D[a] + c->poids < D[j]))
{

D[j] = D[a] + c->poids; 
C[j] = a; 
echange_montant(longueur-1, ordre[j]); /* si actualisation */
    }
  }

}



void dijkstra(void)
{
  
int i;

initialisation();

for(i=TAILLE; i>0; i--) 
dijkstra_iteration(i);

}



int main(void)
{
  
int i;
  
Cellule *c1, *c2, *c3;
  
c1 = CreeCellule(1,1); 
c2 = CreeCellule(3,2); 
c3 = CreeCellule(4,9);
  
graphe[0]=c1; 
c1->suivant=c2; 
c2->suivant=c3;
  
c1 = CreeCellule(2,5);
 
 graphe[1]=c1;

  c1 = CreeCellule(4,1);

  graphe[2]=c1;
 
 c1 = CreeCellule(2,2); 
c2 = CreeCellule(4,6);

  graphe[3]=c1; c1->suivant=c2;
 
 graphe[4]=NULL;
 
 dijkstra();
  
for(i=0; i<TAILLE; i++)
  
{
    printf("sommet %d, distance %d, origine %d\n", i, D[i], C[i]);
  }


return 0;
}




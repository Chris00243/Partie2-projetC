#include "Graphe.h"



void ajout_voisin(Graphe* G, int u, int v){
  Cellule_arete *pca;
  Arete *pa;

  pa=(Arete *) malloc(sizeof(Arete));
  pa->u=u;
  pa->v=v;
  pa->longueur=sqrt( (G->T_som[u]->x - G->T_som[v]->x)*(G->T_som[u]->x - G->T_som[v]->x) + (G->T_som[u]->y - G->T_som[v]->y)*(G->T_som[u]->y - G->T_som[v]->y));

  pca=(Cellule_arete*) malloc(sizeof(Cellule_arete));
  pca->a=pa;
  pca->suiv=G->T_som[u]->L_voisin;
  G->T_som[u]->L_voisin=pca;

  pca=(Cellule_arete*) malloc(sizeof(Cellule_arete));
  pca->a=pa;
  pca->suiv=G->T_som[v]->L_voisin;
  G->T_som[v]->L_voisin=pca;

}

Arete* acces_arete(Graphe* G, int u, int v)
{
  Cellule_arete *coura;

  coura=G->T_som[u]->L_voisin;

  while(coura!=NULL){
	if( ( (coura->a->u=u) && (coura->a->v=v) ) || ( (coura->a->u=v) && (coura->a->v=u) ) ){
		
		printf("\nOUI\n"); 
		return coura->a;
	}
   
	 coura=coura->suiv;
  }

  	printf("\nNON\n"); 
	return NULL;

}

void lecture_graphe(Graphe *G, FILE * f){

  char temp[101];
  int nbarete;   /* stockage temporaire du nombre d aretes */
  int i;
  int num;       /* lecture dans le fichier d un numero */

 
  GetChaine(f,100,temp);
  G->nbsom=GetEntier(f);
  GetChaine(f,100,temp);
  nbarete=GetEntier(f);
  GetChaine(f,100,temp);
  G->nbcommod=GetEntier(f);
  GetChaine(f,100,temp);
  G->gamma=GetEntier(f);

  G->T_som=(Sommet**) malloc((G->nbsom+1)*sizeof(Sommet*));
  for (i=1;i<=G->nbsom;i++){
    G->T_som[i]=(Sommet*) malloc(sizeof(Sommet));
    G->T_som[i]->L_voisin=NULL;
  }

  G->T_commod=(Commod*)malloc(G->nbcommod*sizeof(Commod));

  for (i=0;i<G->nbsom;i++){
    GetChaine(f,100,temp);
    num=GetEntier(f);
    G->T_som[num]->num=num;
    G->T_som[num]->x=GetReel(f);
    G->T_som[num]->y=GetReel(f);
  }

  for (i=0;i<nbarete;i++){
    GetChaine(f,100,temp);
    ajout_voisin(G,GetEntier(f),GetEntier(f));
  }

  for (i=0;i<G->nbcommod;i++){
    GetChaine(f,100,temp);
    G->T_commod[i].e1=GetEntier(f);
    G->T_commod[i].e2=GetEntier(f);
  }

}


void affichageGrapheSVG(Graphe *G, char* nomInstance){

  SVGwriter svg;
  double maxx=0,maxy=0,minx=1e6,miny=1e6;
  int i;
  double xu,yu,xv,yv;
  Cellule_arete *coura;

  for(i=1;i<=G->nbsom;i++){
    if (maxx<G->T_som[i]->x) maxx=G->T_som[i]->x;
    if (maxy<G->T_som[i]->y) maxy=G->T_som[i]->y;
    if (minx>G->T_som[i]->x) minx=G->T_som[i]->x;
    if (miny>G->T_som[i]->y) miny=G->T_som[i]->y;
  }

  SVGinit(&svg,nomInstance,500,500);

  SVGpointColor(&svg, Red);
  SVGlineColor(&svg, Black);

  for(i=1;i<=G->nbsom;i++){

    xu=G->T_som[i]->x;
    yu=G->T_som[i]->y;
    
    printf("%d %d %f %f\n",i,G->T_som[i]->num,xu,yu);
    SVGpoint(&svg,500*(xu-minx)/(maxx-minx),500*(yu-miny)/(maxy-miny));

    coura=G->T_som[i]->L_voisin;
    while (coura!=NULL){

      if (coura->a->u==i){
	xv=G->T_som[coura->a->v]->x;
	yv=G->T_som[coura->a->v]->y;
      }
      else{
	xv=G->T_som[coura->a->u]->x;
	yv=G->T_som[coura->a->u]->y;
      }

      SVGline(&svg,500*(xu-minx)/(maxx-minx),500*(yu-miny)/(maxy-miny),500*(xv-minx)/(maxx-minx),500*(yv-miny)/(maxy-miny));
      
      coura=coura->suiv;
    }

  }

  SVGfinalize(&svg);

}

// fonction de la question 7.2 : fonction codée

int plus_petit_nbr_arete(Graphe *G, int u, int v){

	// u et v sont les numéros de sommets. le sommet numéro u correspond aux tableaux visit et AR à l'indice u-1

	Cellule_arete *cour;

	int n, m,i;

	S_file F; Init_file(&F); // la file 

	printf("\n G = %d; %d; %d", G->T_som[u]->num, G->T_som[u]->L_voisin->a->u, G->T_som[u]->L_voisin->a->v);

	int *visit =(int *)malloc(sizeof(int)*G->nbsom); // tableau pour marquer la visite d'un point 
	
	int *AR =(int *)malloc(sizeof(int)*G->nbsom); // tableau pour marquer le nombre d'arretes entre u à tious les autres points y compris v. 

	if(visit == NULL ) return -404; // -404 c'est pour signaler l'echec d'allocation
	if(AR == NULL ) return -405; // -405 c'est pour signaler l'echec d'allocation

	for(i=0;i<G->nbsom;i++){ visit[i]=0; AR[i]= 600; } // initialisation du tableau visit à 0 et AR à 600 : considéré comme la distance MAX...l'infini 
printf("\nDDDDDDD %d   kkkk %d", AR[11], G->nbsom);
	AR[u-1] = 0; // un point est à distance de 0 de lui-même
	//visit[u] = 1; 
	enfile(&F, u); // on marque la visite du point u et on enfile 

	while( !(estFileVide(&F)) ){

		do{
			if(!(estFileVide(&F))) n = defile(&F);
			else return AR[v-1];

		}while(visit[n-1]==1);
		
				
		printf("\nn = %d", n);
		cour = G->T_som[n]->L_voisin;
		int nbr = AR[n-1]+1;
		

		while(cour){ // cour != NULL

			m = cour->a->u;          // m prend la valeur de l'extreme de l'arête voisine de u donc de n
printf("\nm = %d", m);			
			
			if(m==n) m = cour->a->v;   // si m = n alors m doit prendre la valeur de l'autre extreme de l'arete voisine de u 
printf("\nm = %d", m);
			if(m==n) cour = cour->suiv; // si c'est toujours le cas, on passe au suivant
printf("\nm = %d", m);
				
			if(visit[m-1]==0) enfile(&F, m);

			if(AR[m-1] > nbr) AR[m-1] = nbr; // actualisation du plus petit nbr d'arêtes
			printf("\nJJJJJJJJJJ AR m  %d", AR[m-1]);
			if(m==v) return AR[v-1]; // on a trouvé ce qu'on cherche, on peut arreter le processus 
			
			cour = cour->suiv; 		
		}
		
		visit[n-1] = 1; // n a été visté


	}
		
	return AR[v-1];
} 

//fonction 7.3 et 7.4 : la transformation de la fonction 7.2 Je propose de stocker l'aborescence issue de u créant un tableau des précédents

Cell_entier* chemin(Graphe *G, int u, int v){

	// u et v sont les numéros de sommets. le sommet numéro u correspond aux tableaux visit et AR à l'indice u-1

	Cellule_arete *cour;

	int c = v; // IMPORTANT
	int n, m,i;

	S_file F; Init_file(&F); // la file 

	printf("\n G = %d; %d; %d", G->T_som[u]->num, G->T_som[u]->L_voisin->a->u, G->T_som[u]->L_voisin->a->v);

	int *visit =(int *)malloc(sizeof(int)*G->nbsom); // tableau pour marquer la visite d'un point 
	
	int *AR =(int *)malloc(sizeof(int)*G->nbsom); // tableau pour marquer le nombre d'arretes entre u à tous les autres points y compris v. 
	
	int *PREC =(int *)malloc(sizeof(int)*G->nbsom); // tableau contenant les précédants

	if(visit == NULL ){ printf("Echec Alloc visit"); return NULL;} 
	if(AR == NULL ) { printf("Echec Alloc AR"); return NULL;}   
	if(PREC == NULL ){ printf("Echec Alloc PREC"); return NULL;} 

	for(i=0;i<G->nbsom;i++){ visit[i]=0; AR[i]= 600; PREC[i]=-1; } // initialisation du tableau visit à 0 et AR à 600 : considéré comme la distance MAX...l'infini et PREC à -1 
printf("\nDDDDDDD %d   kkkk %d", AR[11], G->nbsom);
	AR[u-1] = 0; // un point est à distance de 0 de lui-même
	//visit[u] = 1; 
	enfile(&F, u); // on marque la visite du point u et on enfile 

	while( !(estFileVide(&F)) ){

		do{
			if(!(estFileVide(&F))) n = defile(&F);
			else return NULL;

		}while(visit[n-1]==1);
		
				
		printf("\nn = %d", n);
		cour = G->T_som[n]->L_voisin;
		int nbr = AR[n-1]+1;
		

		while(cour){ // cour != NULL

			m = cour->a->u;          // m prend la valeur de l'extreme de l'arête voisine de u donc de n
printf("\nm = %d", m);			
			
			if(m==n) m = cour->a->v;   // si m = n alors m doit prendre la valeur de l'autre extreme de l'arete voisine de u 
printf("\nm = %d", m);
			if(m==n) cour = cour->suiv; // si c'est toujours le cas, on passe au suivant
printf("\nm = %d", m);
				
			if(visit[m-1]==0) enfile(&F, m);

			if(AR[m-1] > nbr){

				 AR[m-1] = nbr; // actualisation du plus petit nbr d'arêtes
				PREC[m-1] = n;  // actualisation du précédent de m par n

				if(m==c){// pas la peine de continuer les autres tours, on a trouvé ce qu'on cherchait
					
					Cell_entier *L;

					Init_Liste(&L);

					
					int j = PREC[c-1]; // i prend le précédent de v
	
					while(j!=-1){ // j = -1 c à d pas de précédent
		
						 // ajout de v et précédents
						ajoute_en_tete(&L,c);
						
						c = j;
						j = PREC[c-1];

					}
					ajoute_en_tete(&L,c);
					return L;



				}

			}
			printf("\nJJJJJJJJJJ AR m  %d", AR[m-1]);
			cour = cour->suiv; 		
		}
		
		visit[n-1] = 1; // n a été visté


	}
	
	return NULL;
}

 


// question 7.4 et 7.5

void ecrire_file(char* filenamencha, Graphe* G)
{
	
	FILE* f = fopen(filenamencha, "w+");
	if(f==NULL){ printf("\n error ecrire_file "); return;}

	int nbcom = G->nbcommod;
	int i, a, b;

	Cell_entier *L;
	Init_Liste(&L);
	
	for (i=0;i<G->nbcommod;i++){
   
		 	
   		a = G->T_commod[i].e1;
    		b = G->T_commod[i].e2;

		L = chemin(G,a,b);

		while(L){

			fprintf(f,"%d ", L->u);
			L=L->suiv;
	
		}

		fprintf(f,"-1\n");
	}

	fclose(f);
}


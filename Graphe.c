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
			
			
			if(m==n) m = cour->a->v;   // si m = n alors m doit prendre la valeur de l'autre extreme de l'arete voisine de u 

			if(m==n) cour = cour->suiv; // si c'est toujours le cas, on passe au suivant

				
			if(visit[m-1]==0) enfile(&F, m);

			if(AR[m-1] > nbr) AR[m-1] = nbr; // actualisation du plus petit nbr d'arêtes
			
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

	
	int *visit =(int *)malloc(sizeof(int)*G->nbsom); // tableau pour marquer la visite d'un point 
	
	int *AR =(int *)malloc(sizeof(int)*G->nbsom); // tableau pour marquer le nombre d'arretes entre u à tous les autres points y compris v. 
	
	int *PREC =(int *)malloc(sizeof(int)*G->nbsom); // tableau contenant les précédants

	if(visit == NULL ){ printf("Echec Alloc visit"); return NULL;} 
	if(AR == NULL ) { printf("Echec Alloc AR"); return NULL;}   
	if(PREC == NULL ){ printf("Echec Alloc PREC"); return NULL;} 

	for(i=0;i<G->nbsom;i++){ visit[i]=0; AR[i]= 600; PREC[i]=-1; } // initialisation du tableau visit à 0 et AR à 600 : considéré comme la distance MAX...l'infini et PREC à -1 

	AR[u-1] = 0; // un point est à distance de 0 de lui-même
	//visit[u] = 1; 
	enfile(&F, u); // on marque la visite du point u et on enfile 

	while( !(estFileVide(&F)) ){

		do{
			if(!(estFileVide(&F))) n = defile(&F);
			else return NULL;

		}while(visit[n-1]==1);
		
		
		cour = G->T_som[n]->L_voisin;
		int nbr = AR[n-1]+1;
		

		while(cour){ // cour != NULL

			m = cour->a->u;          // m prend la valeur de l'extreme de l'arête voisine de u donc de n
			
			
			if(m==n) m = cour->a->v;   // si m = n alors m doit prendre la valeur de l'autre extreme de l'arete voisine de u 

			if(m==n) cour = cour->suiv; // si c'est toujours le cas, on passe au suivant

				
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



/*************** Pour Dijisktra et consort : les ajouts **************************/


/* initialisation du tas */

void Init_Tas( Tas** T)
{
	*T =(Tas*)malloc(sizeof(Tas));
	if(*T==NULL){ printf("\nerror alloc Tas\n"); return;}

	(*T)->n = 0; // pas d'Element dans le tas

	(*T)->tab =(Element*)malloc(sizeof(Element)*(MAX+1));
	if((*T)->tab==NULL){printf("\nerror alloc Tas->tab\n"); return;}
		
	(*T)->Marque =(int*)malloc(sizeof(int)*(MAX+1));
	if((*T)->Marque==NULL){printf("\nerror alloc Tas->Marque\n"); return;}

	int i;
	for(i=0;i<=MAX;i++){

		(*T)->Marque[i]=0;
	}
}


/******************* Quelques fonctions indispensables ********************************* */


/* Fonction d'entassement : pour déplacer de 1 les valeurs entre i et j avec j = *T->n */

void avancer(Tas** T, int i, int j)
{
	int k;
	for(k=j;k>=i;k--){

		(*T)->tab[k+1].i =(*T)->tab[k].i;
		(*T)->tab[k+1].c =(*T)->tab[k].c;		
	}
}

/* Fonction d'entassement : pour déplacer de -1 les valeurs entre i et j avec j = *T->n */

void reculer(Tas** T, int i, int j)
{
	int k;
	for(k=i;k<=j;k++){

		(*T)->tab[k-1].i = (*T)->tab[k].i;
		(*T)->tab[k-1].c = (*T)->tab[k].c;		
	}

	(*T)->tab[j].c = 1000; // considéré comme linfini
}

/* insertion dun Element dans un tas */

void insert(Tas** T, Element E)
{	
	if((*T)->n>=MAX){ printf("\ninsert impossible, Tas plein\n"); return;}
	
	(*T)->Marque[E.i] = 1; // on marque l'insertion de E

	if(E.c <= (*T)->tab[1].c){

		avancer(T, 1,(*T)->n);
		(*T)->tab[1].i = E.i;
		(*T)->tab[1].c = E.c;
		(*T)->n++;
		return;
	}

	if(E.c >= (*T)->tab[(*T)->n].c){

		
		(*T)->tab[(*T)->n+1].i =E.i;
		(*T)->tab[(*T)->n+1].c =E.c;
		(*T)->n++;
		return;
	}

	int i;

	for(i=2;i<=(*T)->n-1;i++){

		if(E.c <= (*T)->tab[i].c){

			avancer(T, i, (*T)->n);
			(*T)->tab[i].i = E.i;
			(*T)->tab[i].c = E.c;
			(*T)->n++;
			return;
		}
	
	}
}

/* Tester la présence d'un Element dans le tas */

int test(Tas* T, int i)
{
	return T->Marque[i];

}

/* Accès à l'Element ayant le plus petit c */

Element* petit_c(Tas* T)
{
	return &(T->tab[1]);

}

/* Suppresion de l'Element ayant le plus petit c */

void delecte(Tas** T)
{
	int i=(*T)->tab[1].i;
	(*T)->Marque[i] = -1; // demarquage 
	reculer(T,2, (*T)->n);
	(*T)->n--;
}

/* suppresion de l'Element d'indice i s'il existe */

void suppression(Tas**T, int i)
{
	if( test((*T), i)!=1){ printf("\n pas présent\n"); return;}

	if((*T)->tab[1].i == i){ delecte(T); return; }

	if((*T)->tab[(*T)->n].i == i){

		int s =(*T)->tab[1].i;
		(*T)->Marque[s] = -1; // demarquage 
		(*T)->n--;
	}

	int j;

	for(j=2;j<=(*T)->n-1;j++){

		if(i == (*T)->tab[j].i){

			(*T)->Marque[j] = -1; // demarquage 
			reculer(T, j+1, (*T)->n);
			
			(*T)->n--;
			return;
		}
	}
}


/*  Fonction DIJIKSTRA : retourne la longueur (longueur totale) de la chaine de e1 à e2 */

double dijikstra(Graphe* G, int e1, int e2)
{
	int nb = G->nbsom; // nombre de sommets

	int *pred = (int*)malloc(sizeof(int)*nb+1); // tableau des précédans

	double *distance = (double*)malloc(sizeof(double)*nb+1);

	if(pred==NULL || distance==NULL){

		printf("\n problème d'alloc pred ou distance\n");
		return -1;
	}
	
	/* initialisation de pred et distance  */

	int i;
	for(i=1;i<=nb;i++){

		pred[i]=-1; // pas encore de prédecesseur
		distance[i]=50000; // considéré comme la disatnce max... l'infini
		
	}

	distance[e1]=0; // e1 est de distance 0 de lui-même

	/* initialisation du tas */

	Tas *T;
	Init_Tas(&T);

	T->Marque[e1]=-1; // déjà visité

	int p, q, s;
	double dis;

	p = e1; q = e2;

	Cellule_arete *cour;

	Element E, *F;

	do{

		dis = distance[p];

		cour = G->T_som[p]->L_voisin;
		
		while(cour){
			
			s=cour->a->u;

			if(p==s) s=cour->a->v;

			if(p==s) return -107; // pour marquer l'erreur			



			if(test(T,s)==0){ // s'il n'est jamais marqué alors l'insérer
				
				E.i = s;
				E.c = cour->a->longueur;				
				insert(&T, E);

			}		
			
			if(cour->a->longueur+dis < distance[s]){ 

				distance[s] = cour->a->longueur+dis; // mise à jour de la distance de s par rapport au point de départ

				pred[s] = p;	// mise à jour du précédant de s
 
			}
		
			cour = cour->suiv;
		}

		F = petit_c(T); // Element ayant le plus petit c

		delecte(&T);	// on supprime cet Element de T
		
		p=F->i;
	
	
	}while( (T->n>=0 && T->n<=MAX) || T->Marque[p]==1);
	
	return distance[q]; // longueur de la chaine de e1 à e2

}

/* Permet d'écrire les évaluations des commodités dans un fichier avec extension .eval  Ce fichier contiendra par ligne longueur ( de Dijikstra) evaluation. Donc deux valeurs par ligne */

void ecrire_eval(Graphe *G, char* fichier, char* fic_eval ) // fic_eval correspond au fichier pour utiliser la fonction evaluation
{
	FILE *f = fopen(fichier, "w+");
	
	double longueur, eval;
	int i, a, b;
	
	int gamma =0;
	
	if(f!=NULL){

		for(i=0;i<G->nbcommod;i++){
   
			a = G->T_commod[i].e1;
    			b = G->T_commod[i].e2;
			
			gamma = G->T_som[a]->gamma_total;			
						
			longueur = dijikstra(G, a, b);
			eval = evaluation_NChaines(25, 96282.54,fic_eval);
		
			fprintf(f,"%lf %lf\n", longueur , eval);

		
		}

	}

	fclose(f);
}


/* Calcul le gamma et actualise dans le graphe */

Graphe* cal_gamma(Graphe *G)
{
	

	int nb=G->nbsom;
	
	int** M =(int**)malloc(sizeof(int*)*nb+1);

	 /* M:  la matrice contenant les gamma */
	
	int i, j;	

	for(i=1;i<=nb;i++){

		M[i]=(int*)malloc(sizeof(int)*nb+1);

		for(j=1;j<=nb;j++){

			M[i][j]=0;
		}
		
	}

	int n = G->nbcommod; // nombre de commodités

	Cell_entier* L;
	int a, b;

	for(i=0;i<n;i++){

		a = G->T_commod[i].e1;
    		b = G->T_commod[i].e2;
		L = chemin(G,a,b);
	
		while(L!=NULL && L->suiv!=NULL){

			a=L->u;
			b=L->suiv->u;

			M[a][b] = M[a][b]+1; // actualisation de gamma
			M[b][a] = M[b][a]+1; // actualisation de gamma
		
			L=L->suiv;	
		}

	}

	Cellule_arete *cour;

	int gamma_total =0;

	for(i=1;i<=nb;i++){
		
		cour = G->T_som[i]->L_voisin;

		while(cour){
			
			j=cour->a->u;
			if(j==i) j=cour->a->v;

			cour->a->calc_gamma = M[i][j];
			if(cour->a->calc_gamma == 0) cour->a->calc_gamma = 1; // par relation de voisinage

			gamma_total += cour->a->calc_gamma;
			cour = cour->suiv;
		}
		

		G->T_som[i]->gamma_total=gamma_total;
		gamma_total =0;
	}

	
	return G;
}





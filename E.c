

int plus_petit_nbr_arete(Graphe *G, int u, int v){

	Cellule_arete *cour;

	int n, m,i, nbr;

	S_file F; Init_file(&F); // la file 

	

	int *visit =(int *)malloc(sizeof(int)*G->nbsom); // tableau pour marquer la visite d'un point 
	
	int *AR =(int *)malloc(sizeof(int)*G->nbsom); // tableau pour marquer le nombre d'arretes entre u à tious les autres points y compris v. 

	if(visit == NULL ) return -404 // -404 c'est pour signaler l'echec d'allocation
	if(AR == NULL ) return -405 // -405 c'est pour signaler l'echec d'allocation

	for(i=0;i<G->nbsom;i++){ visit[i]=0; AR[i]= 600; } // initialisation du tableau visit à 0 et AR à 600 : considéré comme la distance MAX... 

	AR[u] = 0; // un point est à distance de 0 de lui-même
	visit[u] = 1; enfile(&F, u); // on marque la visite du point u et on enfile 

	while( !(estFileVide(&F)) ){

		n = defile(&F);
		
		cour = G->T_som[u]->L_voisin;

		while(cour){ // cour != NULL

			m = cour->a->u;          // m prend la valeur de l'extreme de l'arête voisine de u donc de n
			if(m=n) m = cour->a->v;   // si m = n alors m doit prendre la valeur de l'autre extreme de l'arete voisine de u 

			if(visit[m] == 0){ // donc m pas encore visité
			
				visit[m] = 1;
				enfile(&F, m);
				 nbr = AR[n]+1;

			}
			
			if(AR[m]> nbr) AR[m] = nbr; // actualisation du plus petit nbr d'arêtes

			cour = cour->suiv; 		
		}


	}
		
	return AR[v];
} 


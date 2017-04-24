#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "Graphe.h"
#include "Struct_Liste.h"

#define MENU()  puts(" \n\n***********   Bonjour  ************* \n\n"); \
		puts(" GRAPHE \n "); \
 		puts(" 1. Ajoutez un voisin \n "); \
	        puts(" 2. Vérifier l'existence d'une arête \n"); \
	        puts(" 3. Afficher un graphe SVG \n"); \
	        puts(" 4. Ecrire un graphedans un fichier \n"); \
		puts(" 5. Plus petit nombre d'arêtes\n"); \
		puts(" 15. Pour sortir \n"); \

int main(int argc,char**argv)
{

  char filename[104],filenameres[104],filenamencha[104];
  //int chmeth;

  if(argc!=2){
    printf("usage: %s <file> \n",argv[0]); // J'ai oté pour cette forme, le numéro du choix sera demandé ne fois le menu afficher, c'est plus simple et maniable
   // printf("usage: %s <file> <numeromethod>\n",argv[0]);
    //printf("where numeromethode = 1 if using Width firs search\n");
    return 1;
  }

  //chmeth=atoi(argv[2]);
  strcpy(filename,argv[1]);
  strcpy(filenameres,argv[1]);
  strcpy(filenamencha,argv[1]);
  strcat(filenameres,".res");
  strcat(filenamencha,".ncha");

  FILE *f=fopen(filenameres,"r");

  if (f==NULL){
    printf("Probleme lecture du fichier %s\n",filenameres);
    exit(1);
  }

  

  Graphe G;

  lecture_graphe(&G,f);

  fclose(f);

	Arete * a;
  

	int choix;
	

	int u, v, nbr;

	do{


		MENU()
		printf("\nEntrez votre choix : ");
		scanf("%d", &choix);

		switch(choix){
		
			case 1 :{
					printf("Entrez le premier entier : ");
					scanf("%d", &u);
					printf("Entrez le second entier : ");
					scanf("%d", &v);
					ajout_voisin(&G,u,v);
					break;
			}

			case 2 :{

					printf("Entrez le premier entier : ");
					scanf("%d", &u);
					printf("Entrez le second entier : ");
					scanf("%d", &v);
					a = acces_arete(&G,u,v);
					break;
			}

			case 3 :{
	 				affichageGrapheSVG(&G,filename); // le nom du filename est passé en argument lors de l'exécution du programme
					break;
			}

			case 4 :{
				
					//ecrire_file(filenamencha, &G);
	
			}

			case 5 :{
					printf("Entrez le premier entier : ");
					scanf("%d", &u);
					printf("Entrez le second entier : ");
					scanf("%d", &v);

					nbr = plus_petit_nbr_arete(&G,u,v);
					printf(" nombre d'arêtes de %d à %d est %d\n", u,v,nbr); // si nbr == 600; d'après la fonction il n'y a pas de chemin de u à v
					
					break;

			}

			case 15 : {

					printf(" Au revoir, à très bientôt chef !!!!\n\n");
					break;
			}

			default : {

					printf("\n Je ne connais pas cette commande Chef\n\n !!!");
					break;
					
			}		
		}

	}while(choix!=15);

  return 0;

}

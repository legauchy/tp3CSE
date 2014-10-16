#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#define NB_THREAD 8


struct vecteur {
	int * tab;
	int taille;
	int element;
	int * trouve;
};

struct vect_search {
	struct vecteur * vect;
	int debut_rech;
	int fin_rech;
};

void* search (void *arg){
	struct vect_search * vect_search = (struct vect_search*) arg;
	int i = 0;
	for(i = vect_search->debut_rech ; i < vect_search->fin_rech && !*vect_search->vect->trouve; i++) {
		if(vect_search->vect->tab[i] == vect_search->vect->element) {
			*vect_search->vect->trouve = 1;
		}
	}
	free(vect_search);
	return NULL;
	//return (void*)(vect_search->vect->trouve);
}

int main (int argc, char **argv) {
  FILE* fichier = malloc(sizeof(FILE));
	fichier = fopen (argv[1], "r+");
	if (fichier != NULL) {
		int i;
		int taille_vecteur;
		struct vecteur * vecteur = malloc( sizeof(struct vecteur) );
		
		fscanf(fichier, "%d\n", &taille_vecteur);
		vecteur->tab = malloc( sizeof(int) * taille_vecteur );
    vecteur->taille = taille_vecteur;
    
    for(i = 0 ; i < taille_vecteur ; i++) {
    	fscanf(fichier, "%d\n", &vecteur->tab[i]);
    }
    vecteur->element = atoi(argv[2]);
    vecteur->trouve = malloc(sizeof(int));
    *vecteur->trouve = 0;
    
    pthread_t thread_tab[NB_THREAD];
    int taille_secteur = vecteur->taille / NB_THREAD;
    for (i = 0; i < NB_THREAD; i ++) {
    	struct vect_search * vect_search = malloc(sizeof(struct vect_search));
    	vect_search->vect = vecteur;
    	vect_search->debut_rech = i * taille_secteur;
    	vect_search->fin_rech = (i+1) * taille_secteur;
    	if (vecteur->taille - vect_search->fin_rech < taille_secteur) {
    		vect_search->fin_rech = vecteur->taille;
    	}
    	//printf("Debut %d, Fin %d\n", vect_search->debut_rech, vect_search->fin_rech);
    	pthread_create (&thread_tab[i], NULL, search, (void *)vect_search);
    }
    
    for (i = 0; i < NB_THREAD; i ++) {
    	pthread_join (thread_tab[i], NULL);
    }
    
    
    printf("%d\n", *vecteur->trouve);
    free(vecteur->tab);
    free(vecteur->trouve);
    free(vecteur);
    
    fclose(fichier);
  } else {
      printf("Erreur lors de l'ouverture du fichier %s", argv[1]);
  }
  return EXIT_SUCCESS;
}

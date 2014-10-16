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
};

struct vect_mult {
	struct vecteur * vect1;
	struct vecteur * vect2;
	int debut_mult;
	int fin_mult;
};

void* mult (void *arg){
	struct vect_mult * vect_mult = (struct vect_mult*) arg;
	int i = 0;
	int *somme_partiel = malloc(sizeof(int));
	for(i = vect_mult->debut_mult ; i < vect_mult->fin_mult; i++) {
		*somme_partiel += vect_mult->vect1->tab[i] * vect_mult->vect2->tab[i];
	}
	free(vect_mult);
	pthread_exit(somme_partiel);
	return NULL;
}

int main (int argc, char **argv) {
  FILE* fichier1 = fopen (argv[1], "r+");
  FILE* fichier2 = fopen (argv[2], "r+");
	if (fichier1 != NULL && fichier2 != NULL) {
		int i;
		int taille_vecteur;
		struct vecteur * vecteur1 = malloc( sizeof(struct vecteur) );
		struct vecteur * vecteur2 = malloc( sizeof(struct vecteur) );
		
		fscanf(fichier1, "%d\n", &taille_vecteur);
		vecteur1->tab = malloc( sizeof(int) * taille_vecteur );
    vecteur1->taille = taille_vecteur;
    fscanf(fichier2, "%d\n", &taille_vecteur);
    vecteur2->tab = malloc( sizeof(int) * taille_vecteur );
    vecteur2->taille = taille_vecteur;
    if(vecteur2->taille != vecteur1->taille) {
    	printf("Erreur de taille");
    	return EXIT_FAILURE;
    }
    for(i = 0 ; i < taille_vecteur ; i++) {
    	fscanf(fichier1, "%d\n", &vecteur1->tab[i]);
    }
    for(i = 0 ; i < taille_vecteur ; i++) {
    	fscanf(fichier2, "%d\n", &vecteur2->tab[i]);
    }
    
    pthread_t thread_tab[NB_THREAD];
    int taille_secteur = vecteur1->taille / NB_THREAD;
    for (i = 0; i < NB_THREAD; i ++) {
    	struct vect_mult * vect_mult = malloc(sizeof(struct vect_mult));
    	vect_mult->vect1 = vecteur1;
    	vect_mult->vect2 = vecteur2;
    	vect_mult->debut_mult = i * taille_secteur;
    	vect_mult->fin_mult = (i+1) * taille_secteur;
    	if (vecteur1->taille - vect_mult->fin_mult < taille_secteur) {
    		vect_mult->fin_mult = vecteur1->taille;
    	}
    	
    	pthread_create (&thread_tab[i], NULL, &mult, (void *)vect_mult);
    }
    
    int somme = 0;
    int * somme_partiel = NULL;
    for (i = 0; i < NB_THREAD; i ++) {
    	pthread_join(thread_tab[i], (void *)&somme_partiel );
    	somme += *somme_partiel;
    }
    printf("Somme finale : %d\n", somme);
    
    free(vecteur1->tab);
    free(vecteur1);
    free(vecteur2->tab);
    free(vecteur2);
    
    fclose(fichier1);
    fclose(fichier2);
  } else {
      printf("Erreur lors de l'ouverture du fichier %s\n", argv[1]);
  }
  return EXIT_SUCCESS;
}

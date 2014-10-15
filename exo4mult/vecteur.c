#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

struct vect_search {
	int * tab;
	int taille;
	int element;
};

void* search (void *arg){
	struct vect_search * vecteur = (struct vect_search*)arg;
	int i;
	int * trouve = malloc(sizeof(int));
	*trouve = 0;
	for(i = 0 ; i < vecteur->taille && !*trouve; i++) {
		if(vecteur->tab[i] == vecteur->element) {
			*trouve = 1;
		}
	}
	
	return (void*)trouve;
}

int main (int argc, char **argv){
  FILE* fichier = malloc(sizeof(FILE));
	fichier = fopen (argv[1], "r+");
	if (fichier != NULL) {
		int i;
		int taille_vecteur;
		struct vect_search* vecteur = malloc(sizeof(struct vect_search));
		
		fscanf(fichier, "%d\n", &taille_vecteur);
		vecteur->tab = malloc(sizeof(int)*taille_vecteur);
    vecteur->taille = taille_vecteur;
    
    for(i = 0 ; i < taille_vecteur ; i++) {
    	fscanf(fichier, "%d\n", &vecteur->tab[i]);
    }
    vecteur->element = atoi(argv[2]);
    printf("%d",*((int *)search(vecteur)));
    fclose(fichier);
  } else {
      printf("Erreur lors de l'ouverture du fichier %s", argv[1]);
  }
  return EXIT_SUCCESS;
}

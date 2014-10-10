#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

struct chant {
	int nbchant;
	char * paroles;
};

void *supporter (void *arg){
  struct chant* chant = (struct chant*) arg;
  char      *str = chant->paroles ;
  int       i ;
  int       pid ;
  pthread_t tid ;  
  pid = getpid () ;
  tid = pthread_self () ;
  srand ((int) tid) ;

  for (i = 0; i < chant->nbchant; i++){
    printf ("Processus %d Thread %x : %s \n", pid, (unsigned int) tid, str) ;
    usleep (rand() / RAND_MAX * 1000000.) ;
  }
  return (void *) tid ;
}

int main (int argc, char **argv){
	
	int team1;
	int team2;
  struct chant chantteam1 ;
  struct chant chantteam2 ;
  int i ;
  int nb_threads = 0 ;
  pthread_t *tids ;

  if (argc != 5){
    fprintf(stderr, "usage : %s team1 nb_chant1 team2 nbchant2\n", argv[0]) ;
    exit (-1) ;
  }

	team1 =  atoi (argv[1]);
  chantteam1.nbchant = atoi (argv[2]);
  chantteam1.paroles = "Allons enfants de la patrie";
  
  team2 =  atoi (argv[3]);
  chantteam2.nbchant = atoi (argv[4]);
  chantteam2.paroles = "Swing low, sweet chariot";
  
  nb_threads = team1 + team2; 
  tids = malloc (nb_threads*sizeof(pthread_t)) ;

  /* Create the threads for team1 */
  for (i = 0 ; i < team1; i++){
    pthread_create (&tids[i], NULL, supporter, &chantteam1) ;
  }
  /* Create the other threads (ie. team2) */
  for ( ; i < nb_threads; i++){
    pthread_create (&tids[i], NULL, supporter, &chantteam2) ;
  }  

  /* Wait until every thread ened */ 
  for (i = 0; i < nb_threads; i++){
    pthread_join (tids[i], NULL) ;
  }
  
  free (tids) ;
  return EXIT_SUCCESS;
}

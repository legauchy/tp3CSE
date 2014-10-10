#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#define NB_SONG 3

void *supporter (void *arg){
  char      *str = (char *) arg ;
  int       i ;
  int       pid ;
  pthread_t tid ;  
  pid = getpid () ;
  tid = pthread_self () ;
  srand ((int) tid) ;

  for (i = 0; i < NB_SONG; i++){
    printf ("Processus %d Thread %x : %s \n", pid, (unsigned int) tid, str) ;
    usleep (rand() / RAND_MAX * 1000000.) ;
  }
  return (void *) tid ;
}

int main (int argc, char **argv){

  int team1 ;
  int team2 ;
  char * chaineteam1;
  char * chaineteam2;
  int i ;
  int nb_threads = 0 ;
  pthread_t *tids ;
	
	pid_t pid = fork();
	
  if (argc != 5){
    fprintf(stderr, "usage : %s team1_match1 team2_match1 team1_match2 team2_match2\n", argv[0]) ;
    exit (-1) ;
  }
	
	if (pid == 0) {
		team1 = atoi (argv[1]) ;
		team2  = atoi (argv[2]) ;
		nb_threads = team1 + team2; 
		tids = malloc (nb_threads*sizeof(pthread_t)) ;
		chaineteam1 = "Match 1 : Allons enfants de la patrie";
		chaineteam2 = "Match 1 : Swing low, sweet chariot";
	} else {
		team1 = atoi (argv[3]) ;
		team2  = atoi (argv[4]) ;
		nb_threads = team1 + team2; 
		tids = malloc (nb_threads*sizeof(pthread_t)) ;
		chaineteam1 = "Match 2 : Allons enfants de la patrie";
		chaineteam2 = "Match 2 : Swing low, sweet chariot";
	}


  /* Create the threads for team1 */
  for (i = 0 ; i < team1; i++){
    pthread_create (&tids[i], NULL, supporter, chaineteam1) ;
  }
  /* Create the other threads (ie. team2) */
  for ( ; i < nb_threads; i++){
    pthread_create (&tids[i], NULL, supporter, chaineteam2) ;
  }  

  /* Wait until every thread ened */ 
  for (i = 0; i < nb_threads; i++){
    pthread_join (tids[i], NULL) ;
  }
  
  free (tids) ;
  return EXIT_SUCCESS;
}

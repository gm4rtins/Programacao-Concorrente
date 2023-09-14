#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

int aux = 0;
pthread_mutex_t mutex;
pthread_cond_t cond;


typedef struct{
	int id;
	int nthreads;
} t_args;



void barreira(int nthreads){
	pthread_mutex_lock(&mutex);
	aux++;

	if(aux < nthreads){
		pthread_cond_wait(&cond, &mutex);
	}
	
	else{
		aux = 0;
		pthread_cond_broadcast(&cond);
	}
	
	pthread_mutex_unlock(&mutex);

}


void * mensagem(void*arg){
	int t_id;
	int nthreads;

	t_args args = *(t_args*) arg;
	t_id = args.id;
	nthreads = args.nthreads;

	printf("Ola da thread %d\n", t_id);
	
	barreira(nthreads);
	
	printf("Que dia bonito %d\n", t_id);
	
	barreira(nthreads);

	printf("Ate breve %d\n", t_id);

}

int main(int argc, char *argv[]){
	int nthreads, i, j;
	nthreads = atoi(argv[1]);
	pthread_t tid_sistema[nthreads];
	t_args *args;

	//inicializando variaveis de lock e condição
	pthread_mutex_init(&mutex, NULL);
  	pthread_cond_init (&cond, NULL);

	//inicializando threads
	for(i = 0; i< nthreads; i++){
		args = malloc(sizeof(t_args));
    		if (args == NULL) {
			printf("--ERRO: malloc()\n"); exit(-1);
    		}
   		args->id = i; 
    		args->nthreads = nthreads; 
		
		pthread_create(&tid_sistema[i], NULL, mensagem, (void*) args);
	}

	for (int j = 0; j < nthreads; j++) {
    	   	pthread_join(tid_sistema[j], NULL);
    	}
	
	//desaloca variaveis
  	pthread_mutex_destroy(&mutex);
 	pthread_cond_destroy(&cond);

	return 0;
}
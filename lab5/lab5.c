/*le um arquivo teste.txt e imprime cada linha em ordem. 
para executar no terminal compile e digite [executavel] [nthreads]. */


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>

#define TAMANHO 100 //tamanho de cada string
#define QUANTIDADE 5 //quantidades de strings armazenadas no buffer
char buffer[QUANTIDADE][TAMANHO]; //array que vai guardar as strings
int i_cons = 0; //index que vao indicar onde deve-se inserir ou consumir
sem_t sem_prod, sem_cons;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;



void* consumidor(){
	while(1){
	static int i_cons = 0;
	sem_wait(&sem_cons);
	pthread_mutex_lock(&mutex);
	printf(buffer[i_cons]);
	strcpy(buffer[i_cons], "\0");
	i_cons = (i_cons+1)%QUANTIDADE;
	pthread_mutex_unlock(&mutex);
	sem_post(&sem_prod);}
}


int main(int argc, char *argv[]){
	long long int i;
	int NTHREADS, j;
	FILE* ptr;
	char str_aux[TAMANHO];

	int i_prod = 0;
	NTHREADS = atoi(argv[1]);
	pthread_t tid_sistema[NTHREADS];

	//lendo arquivo
	ptr = fopen("teste.txt", "a+");
	
	if (NULL == ptr) {
        	printf("file can't be opened \n");
		return 1;
    	}

	//inicializando semaforos
	sem_init(&sem_prod, 0, QUANTIDADE);
	sem_init(&sem_cons, 0, 0);

	//criando threads de consumo
	for(i=0;i<NTHREADS;i++){
		pthread_create(tid_sistema+i, NULL, consumidor, NULL);
	}
	

	
	//produzindo

	while (fgets(str_aux, 50, ptr) != NULL) {
		sem_wait(&sem_prod);
		pthread_mutex_lock(&mutex);
        	strcpy(buffer[i_prod], str_aux);
		pthread_mutex_unlock(&mutex);
		sem_post(&sem_cons);
		i_prod = (i_prod+1)%QUANTIDADE;
    	}
	
	for(j=0;j<NTHREADS;j++){
		pthread_join(tid_sistema[j], NULL); 
	}
	

    	fclose(ptr);
 	return 0;
}
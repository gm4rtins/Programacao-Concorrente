#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>

int i_global = 0;
pthread_mutex_t mutex;
pthread_mutex_t mutex2;
int contador = 0;
clock_t inicio, fim;
double delta;

int ehPrimo(long long int n) {
	int i;
	if (n<=1) return 0;
	if (n==2) return 1;
	if (n%2==0) return 0;
	for (i=3; i<sqrt(n)+1; i+=2){
		if(n%i==0) return 0;
	}
	return 1;
}

void * tarefa(void*arg){
	int i_local;
	long long int total = 0;
	long long int valor = (long long int)arg;
	

	//secao critica
	pthread_mutex_lock(&mutex);
	i_local = i_global; i_global++;
	pthread_mutex_unlock(&mutex);
	

	while(i_local < valor) {
		if(ehPrimo(i_local)){
			total++;
		}

		//secao critica
		pthread_mutex_lock(&mutex);
		i_local = i_global; i_global++;
		pthread_mutex_unlock(&mutex);
		
		}
	pthread_mutex_lock(&mutex2);
	contador = contador + total;
	pthread_mutex_unlock(&mutex2);
	
	pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
	inicio = clock();
	// verificando se todos os argumentos foram passados
	if (argc < 3) {
        	printf("Argumentos insuficentes!\nPasse via terminal programa numero_threads numero\n");
        	return 1; // Retorne um código de erro
    	}

	int nthread = atoi(argv[1]);
	long long int valor = atoll(argv[2]);
	int j;
	

	pthread_t threads[nthread];

	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_init(&mutex2, NULL);

	for (int i = 0; i < nthread; i++) {       

        	pthread_create(&threads[i], NULL, tarefa, (void*)valor);
    	}

	for (int j = 0; j < nthread; j++) {
    	   	pthread_join(threads[j], NULL);
    	}
	
	printf("%d\n", contador);
	fim = clock();
	delta = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
	printf("tempo: %lf\n", delta);


    return 0;
}
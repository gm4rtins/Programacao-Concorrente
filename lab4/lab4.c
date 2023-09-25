#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_esc = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_le = PTHREAD_COND_INITIALIZER;

int leitores = 0;
int escritores = 0;

void entra_escrita(){
	pthread_mutex_lock(&mutex); //seção critica (acessar vars globais)
	while(leitores > 0 || escritores > 0){
		pthread_cond_wait(&cond_esc, &mutex);
	}
	escritores++;
	pthread_mutex_unlock(&mutex);
	
}

void sai_escrita(){
	pthread_mutex_lock(&mutex); // entrando na seção critica
	escritores --;
	pthread_cond_broadcast(&cond_le);
	pthread_cond_signal(&cond_esc);
	pthread_mutex_unlock(&mutex);

}

void entra_leitura(){
	pthread_mutex_lock(&mutex); // acessando vars globais
	if (escritores > 0){
		pthread_cond_wait(&cond_le, &mutex);
	}
	escritores++;
	pthread_mutex_unlock(&mutex);

}

void sai_leitura(){
	pthread_mutex_lock(&mutex);
	leitores--;
	if(leitores == 0){
		pthread_cond_signal(&cond_le);
	}
	pthread_mutex_unlock(&mutex);

}

void main(){
	printf("que historia e essa de ygona nova rainha dos memes\n");
}
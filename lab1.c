#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define TAM 10000
#define NTHREADS 8

//estrutura que vai guardar os argumentos da thread
typedef struct {
    int *array, inicio, fim;
} t_args;

//função que a thread vai executar
void *quadrado(void *arg) {
    t_args *args = (t_args *)arg;
    
    for (int i = args->inicio; i < args->fim; i++) {
        args->array[i] = args->array[i] * args->array[i];
    }
    
    pthread_exit(NULL);
}

//inicializa o vetor a ser alterado pela thread e do vetor teste
void inicializa_vetor(int array[TAM], int teste[TAM]){
    int i;
    for(i=0; i < TAM; i++){
        array[i] = i;
        teste[i] = i*i;
    }
}

//compara se 2 arrays possuem os mesmos elementos
int testagem(int array[TAM], int teste[TAM]){
    int i;
    for(i=0; i < TAM; i++){
        if(array[i] != teste[i]){
            return 1;
        }
    }
    return 0;
}

int main() {
    int i, result, array[TAM], teste[TAM];
    pthread_t threads[NTHREADS];
    t_args thread_args[NTHREADS]; //uma lista de structs, pois cada thread precisa do seu próprio
    
    inicializa_vetor(array, teste);

    int elementos_por_thread = TAM / NTHREADS;

    //inicializando variaveis dos structs e disparando fluxos
    for (int i = 0; i < NTHREADS; i++) {
        thread_args[i].array = array;
        thread_args[i].inicio = i * elementos_por_thread;
        thread_args[i].fim = (i + 1) * elementos_por_thread;

        pthread_create(&threads[i], NULL, quadrado, &thread_args[i]);
    }
    
    //esperando todos os fluxos terminarem
    for (int i = 0; i < NTHREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    //testando resultado
    result = testagem(array, teste);
    if(result == 0){
        printf("Resultado correto!\n");
    }
    else{
        printf("Resultado incorreto...\n");
    }
    

    return 0;
}

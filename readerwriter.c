/**
Este programa trata de simular el problema de los lectores-escritores. El 
problema de los lectores-escritores busca modelar un problema de exclusion 
mutua categorica.

Este problema establece que mientras un escritor este escribiendo nadie mas 
puede ni leer, ni escribir. De otro lado, si hay alguien esta leyendo, pueden
llegar mas lectores a leer pero mientras alguien esta leyendo un escritor no 
puede escribir.

*/

#include "readerwriter.h" // archivo de cabecera con funciones auxiliares
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>

#define MAXPERSONS 10
#define SEMNAME "/semaforo122"
#define SEMNAME1 "/semaforo112"


int contador;
int numreaders;
int numwriters;

sem_t *s;
sem_t *s1;

void *reading(void*);
void *writing(void*);
void INThandler(int);

int main() {

	pthread_t personas[MAXPERSONS];
	long tiempo = 0.0;
	int ids[MAXPERSONS];
	int i;
	contador = 0;
	numreaders = 0;
	numwriters = 0;
	s1=sem_open(SEMNAME1, O_CREAT, S_IRUSR | S_IWUSR, 1);
	s=sem_open(SEMNAME, O_CREAT, S_IRUSR | S_IWUSR, 1);	
	//signal(SIGINT, INThandler);
	// Inicializo el generador de numeros aleatorios
	srand(time(NULL));
	// Se crearan MAXPERSONS 
	for (i = 0; i <  MAXPERSONS; i++) {
		ids[i] = i + 1;
		//printf("La persona %d llego en el tiempo %li\n",i,tiempo);
		// aleatoriamente se decide si se crea un lector o un escritor
		if (readerorwriter(.5) == READER) {
			pthread_create(&personas[i],NULL,reading,(void*)ids[i]);
			numreaders++;
		} else {
			pthread_create(&personas[i],NULL,writing,(void*)ids[i]);
			numwriters++;
		}
		if (i < MAXPERSONS - 1) {
			printf("Tiempo actual %li ms. ",tiempo);
			long _tiempo = __waitt__;
			printf("La proxima persona llegara en %li ms\n", _tiempo);
			__wait__((int)_tiempo);
			tiempo += _tiempo;
		} else {
#ifdef DEBUG
			printf("La ultima persona\n");
#endif
		}
	}

	for (i = 0; i < MAXPERSONS; i++) {
		pthread_join(personas[i], NULL);
	}

sem_close(s);
sem_unlink(SEMNAME);

sem_close(s1);
sem_unlink(SEMNAME1);
 return 0;

}

/**
Funcion que modela el comportamiento lectura.
*/
void *reading(void* data) {
	int id = (int) data;
	long _tiempo;
	_tiempo = __waitt__;

	sem_wait(s1);  //Semaforo 2 
	contador++;

	if (contador == 1)
	{ sem_wait(s);
	}
    sem_post(s1);
      
	printf("El lector %d estara leyendo por %li ms. Hay %d lectores en el salon\n",id,_tiempo,contador);
        sem_post(s1);

	__reading__(_tiempo);
	sem_wait(s1);
	contador--;
		if(contador == 0) {
		sem_post(s);
}
	sem_post(s1);
	printf("El lector %d termina de leer, ahora hay %d lectores leyendo\n",id, contador);
	
	return NULL;
}

/**
Funcion que modela el comportamiento escritura.
*/
void *writing(void* data) {
	int id = (int) data;
	long _tiempo = __waitt__;

	printf("La persona %d quien es escritor, intentara escribir\n",id);
sem_wait(s);
	printf("El escritor %d esta escribiendo en el salon con %d lectores y escribira por %li ms\n",id,contador,_tiempo);
	
       __writing__(_tiempo);
	printf("El escritor %d termina de escribir\n",id);
sem_post(s);
	return NULL;
}

/**
Esta funcion permite capturar la senal Ctrl-C. El Ctrl-C es una secuencia
de teclas que causa que un programa termine su ejecucion. 
Como esta funcion captura esta senal, Ctrl-C, usted puede decidir que hacer
una vez el usuario digita Ctrl-C durante la ejecucion de su programa.

Introduzca las instrucciones que al detectar y corroborar que el usuario
digito Ctrl-C se liberen los recursos asociados a los semaforos.

void  INThandler(int sig)
{
     char  c;

     signal(sig, SIG_IGN);
     printf("OUCH, did you hit Ctrl-C?\n"
            "Do you really want to quit? [y/n] ");
     c = getchar();
     if (c == 'y' || c == 'Y') {
 		sem_close(roomempty); sem_unlink(ROOMEMPTY);
 		sem_close(mutex); sem_unlink(MUTEX);
		exit(0);
     } else
          	signal(SIGINT, INThandler);
     getchar(); // Get new line character
}
*/

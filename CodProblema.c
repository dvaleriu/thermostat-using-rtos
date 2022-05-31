#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

void scanareTmpr();
void modificareTmpr();
void verificareTmpr();
void controlSem();

double TmprInit = 0, TmprMdf=0;
pthread_mutex_t mutex;
int nrCamere;
sem_t scanStart , scanStop;
sem_t modifStart , modifStop;
sem_t verifStart , verifStop;

void (*functie [])() = {scanareTmpr , modificareTmpr , verificareTmpr, controlSem};

int main(){
	
	pthread_t FIR[4];
	int i;

	printf("Introduceti numarul de camere: \n");
	scanf("%d",&nrCamere);

	sem_init (&scanStart , 1, 1);
	sem_init (&scanStop , 1, 0);
	sem_init (&modifStart , 1, 0);
	sem_init (&modifStop , 1, 0);
	sem_init (&verifStart , 1, 0);
	sem_init (&verifStop , 1, 0);

	for(i =0; i <4; i++)
		pthread_create (FIR+i, NULL, *(functie+i),NULL);
	
	for(i =0; i <4; i++)
		pthread_join (*(FIR+i), NULL);
	
	printf ("\n Zero threads \n");
	pthread_exit(NULL);

}



void scanareTmpr (){
	
	double n;
	int i;
	
	for(i=0; i<nrCamere; i++)
	{
		sem_wait(&scanStart);
		
		printf ("Introduceti temperatura initiala in grade: \n");
		fflush (stdin);
		scanf("%lf", &n);
		
		pthread_mutex_lock(&mutex);
		TmprInit = n;
		pthread_mutex_unlock(&mutex);
		
		sem_post(&scanStop);
	}

}



void modificareTmpr (){
	
	double n;
	int i;
	
	for(i=0; i<nrCamere; i++)
	{
		sem_wait(&modifStart);
		
		printf ("Cu cate grade doriti sa modificati temperatura? \n");
		fflush (stdin);
		scanf("%lf", &n);
	
		pthread_mutex_lock(&mutex);
		TmprMdf = n;
		pthread_mutex_unlock(&mutex);
	
		sem_post(&modifStop);
	}

}



void verificareTmpr (){
	int i;
	
	for(i=0; i<nrCamere; i++)
	{
		sem_wait(&verifStart);
		
		if(abs(TmprMdf)<=(abs(TmprInit)/100))
			printf("Variatia de temperatura este prea mica pentru a produce modificari.\n");
		else
			printf("Temperatura a ajuns la %lf. \n",TmprInit+TmprMdf);
		
		sem_post(&verifStop);
	}
}



void controlSem(){
	
	int i;
	
	for(i=0; i<nrCamere; i++)
	{
		sem_wait(&scanStop);
		sem_post(&modifStart);
		sem_wait(&modifStop);
		sem_post(&verifStart);
		sem_wait(&verifStop);
		sem_post(&scanStart);
	}
}

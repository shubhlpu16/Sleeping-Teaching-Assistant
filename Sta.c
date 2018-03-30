#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>

/* semaphore declaration*/
sem_t chair_sem[3];
sem_t student;
sem_t ta_sleep; 

/* threads */
pthread_t *Students;  //thread for N students 
pthread_t TA; // seprate thread for ta

int chair_count = 0;
int index_chair = 0;
pthread_mutex_t ChairAccess; // mutex to apply locks on chairs



/* this function checks that ta is sleeping wakes him up when student come  , if no student is threre i.e chair is empty we unlock chair mutex to allow for occupying seat , when chair is occupied chair count decrements which tells tht only tht number of students can come. And Ta helps the student */
void * TA_check() 
{	
	while(1)

	{
		sem_wait(&TA_Sleep);		//TA is currently sleeping.
		printf("***************TA is sleeping***************\n");
		printf("student come\n");
		printf("***************TA has been awakened by a student.***************\n");
		while(1)
		{
			// when Ta is awakend a student occupies space so we have to put lock on chairs
			pthread_mutex_lock(&ChairAccess);
			if(chairs_count == 0) // we will check tht if chairs are empty then we unlock the chairs n let them occupy
			{
				//if chairs are empty, break the loop.
				pthread_mutex_unlock(&ChairAccess);
				break;

			}

			//TA gets next student on chair.

			sem_post(&chair_sem[index_chair]); // we signal the chair tht it has been occupied
			chair_count--;  // chair occupied so count decrements
			printf("Student left his/her chair. Remaining Chairs %d\n", 3 - chair_count);
			index_chair = (index_chair + 1) % 3;
			pthread_mutex_unlock(&ChairAccess);
			// unlock
			printf("\t TA is currently helping the student.\n");
			sleep(5);
			sem_post(&student);
			sleep(10);

		}

	}

}























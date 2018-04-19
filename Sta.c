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
		sem_wait(&ta_sleep);		//TA is currently sleeping.
		printf("student come\n");
		printf("***************TA has been awakened by a student.***************\n");
		while(1)
		{
			// when Ta is awakend a student occupies space so we have to put lock on chairs
			pthread_mutex_lock(&ChairAccess);
			if(chair_count == 0) // we will check tht if chairs are empty then we unlock the chairs n let them occupy
			{
				//if chairs are empty, break the loop.
				pthread_mutex_unlock(&ChairAccess);
				break;

			}

			//TA gets next student on chair.

			sem_post(&chair_sem[index_chair]); // we signal the chair tht it has been occupied
			chair_count--;  // chair occupied so count decrements
			printf("Student  left his/her chair in waiting room n goes to ta. Remaining Chairs %d\n",3 - chair_count);
			index_chair = (index_chair + 1) % 3;
			pthread_mutex_unlock(&ChairAccess);
			// unlock chair access
			printf("\t TA is currently helping the student.\n");
			sleep(5);
			sem_post(&student);
			usleep(1000);

		}

	}

}

/* this function assume tht each student spends 10 time quanta with Ta n during this time no student can come so we put mutex  on chairs so no other can access chairs . And students occupy seats till all 3 chairs are full. when all the chairs are occupied n at this tym if student came he will return back n come again when waiting chairs become empty*/


void *Student_Check(void *threadID) 
{
	int Time_with_ta;
	while(1)

	{

		printf("Student %ld doing assignment .\n", (long)threadID);

		Time_with_ta = rand() % 10 + 1;; //let assume each student take 10 time of TA
		sleep(Time_with_ta);		
		printf("Student %ld needs help from the TA\n", (long)threadID);
		pthread_mutex_lock(&ChairAccess); // as student come he sits on chair so put lock
		int count = chair_count;

		pthread_mutex_unlock(&ChairAccess);
		if(count < 3)		//if chair count < 3 students will come sit n wait.
		{
			if(count == 0)		//If student sits on first empty chair, wake up the TA.
				sem_post(&ta_sleep); // wake up ta semaphore
			else
			printf("Student %ld sat on a chair waiting for the TA to finish. \n", (long)threadID);

			pthread_mutex_lock(&ChairAccess); // mutex lock so no one can sit on preoccupied chair

			int index = (index_chair + chair_count) % 3;
			chair_count++;
			printf("Student sat on chair.Chairs Remaining: %d\n", 3 - chair_count);
			pthread_mutex_unlock(&ChairAccess);
			// unlock
			sem_wait(&chair_sem[index]);		//Student leaves his/her chair.
			printf("\t Student %ld is getting help from the TA. \n", (long)threadID);
			sem_wait(&student);		//Student waits to go next.
			printf("Student %ld left TA room.\n",(long)threadID);

		}

		else 
			printf("Student %ld will return at another time. \n", (long)threadID);

			//If there r no chairs left .

	}

}

int main()

{

	int number_of_students = 5;		

	int id;
	srand(time(NULL));
	//Initializing Mutex Lock and Semaphores.

	sem_init(&ta_sleep, 0, 0);

	sem_init(&student, 0, 0);

	for(id = 0; id < 3; ++id)			//Chairs array of 3 semaphores.
		sem_init(&chair_sem[id], 0, 0);
	pthread_mutex_init(&ChairAccess, NULL);
	Students = (pthread_t*) malloc(sizeof(pthread_t)*number_of_students);
	//Creating TA thread
	pthread_create(&TA, NULL, TA_check, (void*) (long)id);	
	//Creating N student thread here n =5
	for(id = 0; id < number_of_students; id++)
		pthread_create(&Students[id], NULL, Student_Check,(void*) (long)id);
	//Waiting for TA thread and N Student threads.
        pthread_join(TA, NULL);
	for(id = 0; id < number_of_students; id++)
pthread_join(Students[id], NULL);
free(Students);
	return 0;

}










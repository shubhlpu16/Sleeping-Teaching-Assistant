# SleepingTeachingAssistant

A university computer science department has a teaching assistant (TA) who
helps undergraduate students with their programming assignments during
regular office hours. The TA’s office is rather small and has room for only one
desk with a chair and computer. There are three chairs in the hallway outside
the office where students can sit and wait if the TA is currently helping another
student. When there are no students who need help during office hours, the
TA sits at the desk and takes a nap. If a student arrives during office hours
and finds the TA sleeping, the student must awaken the TA to ask for help. If a
student arrives and finds the TA currently helping another student, the student
sits on one of the chairs in the hallway and waits. If no chairs are available, the
student will come back at a later time.
Using POSIX threads, mutex locks, and semaphores

semaphores used

sem_t chair_sem[3];
sem_t student;
sem_t ta_sleep; 

threads 
pthread_t Students[5];  //thread for N students 
pthread_t TA; // seprate thread for ta

pthread_mutex_t ChairAccess; // mutex to apply locks on chairs


function TA_check
this function checks that ta is sleeping wakes him up when student come  , if no student is threre i.e chair is empty we unlock chair mutex to allow for occupying seat , when chair is occupied chair count decrements which tells tht only tht number of students can come. And Ta helps the student

function student_activity

this function assume tht each student spends 10 time quanta with Ta n during this time no student can come so we put mutex  on chairs so no other can access chairs . And students occupy seats till all 3 chairs are full. when all the chairs are occupied n at this tym if student came he will return back n come again when waiting chairs become empty

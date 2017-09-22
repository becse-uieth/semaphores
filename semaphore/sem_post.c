#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
void *thread_function(void *arg);
sem_t bin_sem;
#define WORK_SIZE 1024
char work_area[WORK_SIZE];
int main() {
int res;
pthread_t a_thread;
void *thread_result;
res = sem_init(&bin_sem, 0, 0);
if (res != 0) {
perror("Semaphore initialization failed");
exit(EXIT_FAILURE);
}
res = pthread_create(&a_thread, NULL, thread_function, NULL);
if (res != 0) {
perror("Thread creation failed");
exit(EXIT_FAILURE);
}
printf("Input some text. Enter ‘end’ to finish\n");
while(strncmp("end", work_area, 3) != 0) {    //when first three letters will be caught as end, then thread will join.
fgets(work_area, WORK_SIZE, stdin);
/*below sem_post will increment value of sem everytime user goes to new line and then calls
thread func with inc value of sem as 1 each time*/
sem_post(&bin_sem);					
}
printf("\nWaiting for thread to finish...\n");
res = pthread_join(a_thread, &thread_result);
if (res != 0) {
perror("Thread join failed");
exit(EXIT_FAILURE);
}
printf("Thread joined\n");
sem_destroy(&bin_sem);
exit(EXIT_SUCCESS);
}
/*Thread function to count characters */
void *thread_function(void *arg) {
sem_wait(&bin_sem);					//this dec sem value	
while(strncmp("end", work_area, 3) != 0) {
int s=strlen(work_area);
printf("You input %d characters\n", s-1);
sem_wait(&bin_sem);					//this also dec sem value so that the loop will not be in infinite
}
pthread_exit(NULL);
}

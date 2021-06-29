#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>

typedef struct node
{
	int pid;
	int processPriority;
	int arrivalTime;
	int burstTime;
	int waitingTime;
	int turnAroundTime;
	int tempBurst;
	struct node *next;
}node;

int numOfProcesses, quantumTime, turnAroundTimeSum, waitingTimeSum;
node *HEAD = NULL;

pthread_mutex_t wtSum_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t tatSum_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t head_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t num_mutex = PTHREAD_MUTEX_INITIALIZER;

void delete_list(node *nody)
{
	int i;
	node *temp;
	for (i = 0; i < numOfProcesses && nody != NULL; ++i) {
		temp = nody;
		nody = nody->next;
		free(temp);
	}
}
node *createList(char *fname)
{
	node *head = NULL;
	node *temp = NULL;
	node *p = NULL;
	int n;
	
	FILE *myfile = fopen(fname, "r");
	
	if( myfile == NULL){
                printf("ERROR: opening file \n");
                exit(1);
        }

	fscanf(myfile, "%d", &quantumTime);

	numOfProcesses = 0;

	while (!feof(myfile)) {
		
		int brstTmp = 0;
		fscanf(myfile, "%d", &brstTmp);
		if(brstTmp == 0){break;}
		
		numOfProcesses++;
		printf("Registering Prosses num = %d\n\n", numOfProcesses);

		temp = (node *) malloc(sizeof(node));
		temp->pid = numOfProcesses - 1;
		
		//fscanf(myfile, "%d", &(temp->burstTime));
		temp->burstTime = brstTmp;
		
		fscanf(myfile, "%d", &(temp->arrivalTime));
		
		fscanf(myfile, "%d", &(temp->processPriority));

		temp->tempBurst = temp->burstTime;

		temp->next = NULL;

		if (head == NULL)
			head = temp;
		else {
			p = head;
			while (p->next != NULL)
				p = p->next;
			p->next = temp;
		}

		if(feof(myfile)){
			printf("end of file detected = %d\n", numOfProcesses);
			break;
		}
	}
	
	fclose(myfile);

	return head;
}

void FCFS_algo()
{
	clock_t time = clock();
	double sec = -1;
	node *head1 = NULL;
	node *temp1 = NULL;
	node *p1 = NULL;

	node *current = HEAD;
	
	int mynum = numOfProcesses;

	for (int i = 0; i < mynum; i++) {
		temp1 = (node *) malloc(sizeof(node));
		if (temp1 == NULL) {
			printf("Error in allocating data in FCFS_algo\n");
			exit(1);
		}
		temp1->pid = current->pid;
		temp1->burstTime = current->burstTime;
		temp1->next = NULL;
		current = current->next;

		if (head1 == NULL)
			head1 = temp1;

		else {
			p1 = head1;
			
			while (p1->next != NULL)
				p1 = p1->next;

			p1->next = temp1;
		}
	}

	node *i, *j;

	i = head1;
	j = head1->next;

	i->waitingTime = 0;

	while (j != NULL) {
		j->waitingTime = i->waitingTime + i->burstTime;
		i = i->next;
		j = j->next;
	}

	i = head1;

	while (i != NULL) {
		i->turnAroundTime = i->burstTime + i->waitingTime;
		i = i->next;
	}

	j = head1;
	
	turnAroundTimeSum = 0;

	waitingTimeSum = 0;

	while (j != NULL) {
		turnAroundTimeSum = turnAroundTimeSum + j->turnAroundTime;

		waitingTimeSum = waitingTimeSum + j->waitingTime;
		j = j->next;
	}
	
	double turnAroundTimeAvg = (float)turnAroundTimeSum / (float)numOfProcesses;

	double waitingTimeAvg = (float)waitingTimeSum / (float)numOfProcesses;

	printf("*****************************************************************************************************************\n");
	printf("First Come First Served Scheduling Algorithm in sequential run\n");
	printf("Process\t Burst Time\t Waiting Time\t Turn Around Time\n\n");
	
	for (i = head1; i != NULL; i = i->next)
		printf("Process [%d]\t %d\t\t\t %d\t\t\t %d\n", i->pid+1, i->burstTime, i->waitingTime, i->turnAroundTime);

	printf("\n\nAverage Waiting Time = %lf\n", waitingTimeAvg);
	printf("\n\nAverage Turnaround Time = %lf\n", turnAroundTimeAvg);
	sec = ((double)time)/CLOCKS_PER_SEC;
	printf("\n\ntotal Time it took = %lfs\n", sec);
	printf("\n\n");
	delete_list(temp1);
}

void *FCFS(void *arg)
{
	clock_t time = clock();
	double sec = -1;
	node *head1 = NULL;
	node *temp1 = NULL;
	node *p1 = NULL;

	pthread_mutex_lock(&head_mutex);
	node *current = HEAD;
	pthread_mutex_unlock(&head_mutex);
	
	pthread_mutex_lock(&num_mutex);
	int mynum = numOfProcesses;
	pthread_mutex_unlock(&num_mutex);

	for (int i = 0; i < mynum; i++) {
		temp1 = (node *) malloc(sizeof(node));
		temp1->pid = current->pid;
		temp1->burstTime = current->burstTime;
		temp1->next = NULL;
		current = current->next;

		if (head1 == NULL)
			head1 = temp1;

		else {
			p1 = head1;
			
			while (p1->next != NULL)
				p1 = p1->next;

			p1->next = temp1;
		}
	}

	node *i, *j;

	i = head1;
	j = head1->next;

	i->waitingTime = 0;

	while (j != NULL) {
		j->waitingTime = i->waitingTime + i->burstTime;
		i = i->next;
		j = j->next;
	}

	i = head1;

	while (i != NULL) {
		i->turnAroundTime = i->burstTime + i->waitingTime;
		i = i->next;
	}

	j = head1;
	
	pthread_mutex_lock(&tatSum_mutex);
	turnAroundTimeSum = 0;
	pthread_mutex_unlock(&tatSum_mutex);

	pthread_mutex_lock(&wtSum_mutex);
	waitingTimeSum = 0;
	pthread_mutex_unlock(&wtSum_mutex);

	while (j != NULL) {
		pthread_mutex_lock(&tatSum_mutex);
		turnAroundTimeSum = turnAroundTimeSum + j->turnAroundTime;
		pthread_mutex_unlock(&tatSum_mutex);

		pthread_mutex_lock(&wtSum_mutex);
		waitingTimeSum = waitingTimeSum + j->waitingTime;
		pthread_mutex_unlock(&wtSum_mutex);
		j = j->next;
	}
	
	pthread_mutex_lock(&tatSum_mutex);
	double turnAroundTimeAvg = (float)turnAroundTimeSum / (float)numOfProcesses;
	pthread_mutex_unlock(&tatSum_mutex);

	pthread_mutex_lock(&wtSum_mutex);
	double waitingTimeAvg = (float)waitingTimeSum / (float)numOfProcesses;
	pthread_mutex_unlock(&wtSum_mutex);

	printf("*****************************************************************************************************************\n");
	printf("First Come First Served Scheduling Algorithm in threaded run\n");
	printf("Process\t Burst Time\t Waiting Time\t Turn Around Time\n\n");
	
	for (i = head1; i != NULL; i = i->next)
		printf("Process [%d]\t %d\t\t\t %d\t\t\t %d\n", i->pid+1, i->burstTime, i->waitingTime, i->turnAroundTime);

	printf("\n\nAverage Waiting Time = %lf\n", waitingTimeAvg);
	printf("\n\nAverage Turnaround Time = %lf\n", turnAroundTimeAvg);
	sec = ((double)time)/CLOCKS_PER_SEC;
	printf("\n\ntotal Time it took = %lfs\n", sec);
	printf("\n\n");
	free(temp1);
	return 0;
}

void nonPreemtiveSJF_algo()
{
	clock_t time = clock();
	double sec = -1;
	node *head1 = NULL;
	node *temp1 = NULL;
	node *p1 = NULL;
	
	node *current1 = HEAD;


	int mynum = numOfProcesses;

	for (int i = 0; i < mynum; i++) {
		temp1 = (node *) malloc(sizeof(node));
		temp1->pid = current1->pid;
		temp1->burstTime = current1->burstTime;
		temp1->next = NULL;
		current1 = current1->next;

		if (head1 == NULL)
			head1 = temp1;

		else {
			p1 = head1;
			
			while (p1->next != NULL)
				p1 = p1->next;

			p1->next = temp1;
		}
	}

	node *i, *j;
	int tempPid, tempBurst;

	for (i = head1; i->next != NULL; i = i->next) {

		for (j = i->next; j != NULL; j = j->next) {
			
			if (i->burstTime > j->burstTime) {
				tempPid = i->pid;
				tempBurst = i->burstTime;

				i->pid = j->pid;
				i->burstTime = j->burstTime;
				
				j->pid = tempPid;
				j->burstTime = tempBurst;
			}
		}
	}

	i = head1;
	j = head1->next;

	i->waitingTime = 0;

	while (j != NULL) {
		j->waitingTime = i->waitingTime + i->burstTime;
		i = i->next;
		j = j->next;
	}

	i = head1;

	while (i != NULL) {
		i->turnAroundTime = i->burstTime + i->waitingTime;
		i = i->next;
	}

	j = head1;
	
	turnAroundTimeSum = 0;

	waitingTimeSum = 0;

	while (j != NULL) {
	
		turnAroundTimeSum = turnAroundTimeSum + j->turnAroundTime;

		waitingTimeSum = waitingTimeSum + j->waitingTime;
		j = j->next;
	}

	double turnAroundTimeAvg = (float)turnAroundTimeSum / (float)numOfProcesses;

	double waitingTimeAvg = (float)waitingTimeSum / (float)numOfProcesses;

	printf("*****************************************************************************************************************\n");
	printf("Shortest Job First Non-preemtive Scheduling Algorithm in sequential run\n");
	printf("Process\t Burst Time\t Waiting Time\t Turn Around Time\n\n");
	
	for (i = head1; i != NULL; i = i->next)
		printf("Process [%d]\t %d\t\t\t %d\t\t\t %d\n", i->pid+1, i->burstTime, i->waitingTime, i->turnAroundTime);

	printf("\n\nAverage Waiting Time = %lf\n", waitingTimeAvg);
	printf("Average Turn Around Time = %lf\n", turnAroundTimeAvg);
	sec = ((double)time)/CLOCKS_PER_SEC;
	printf("\n\ntotal Time it took = %lfs\n", sec);
	printf("\n\n");
	delete_list(temp1);
}

void *nonPreemtiveSJF(void *arg)
{
	clock_t time = clock();
	double sec = -1;
	node *head1 = NULL;
	node *temp1 = NULL;
	node *p1 = NULL;
	
	pthread_mutex_lock(&head_mutex);
	node *current1 = HEAD;
	pthread_mutex_unlock(&head_mutex);


	pthread_mutex_lock(&num_mutex);
	int mynum = numOfProcesses;
	pthread_mutex_unlock(&num_mutex);

	for (int i = 0; i < mynum; i++) {
		temp1 = (node *) malloc(sizeof(node));
		temp1->pid = current1->pid;
		temp1->burstTime = current1->burstTime;
		temp1->next = NULL;
		current1 = current1->next;

		if (head1 == NULL)
			head1 = temp1;

		else {
			p1 = head1;
			
			while (p1->next != NULL)
				p1 = p1->next;

			p1->next = temp1;
		}
	}

	node *i, *j;
	int tempPid, tempBurst;

	for (i = head1; i->next != NULL; i = i->next) {

		for (j = i->next; j != NULL; j = j->next) {
			
			if (i->burstTime > j->burstTime) {
				tempPid = i->pid;
				tempBurst = i->burstTime;

				i->pid = j->pid;
				i->burstTime = j->burstTime;
				
				j->pid = tempPid;
				j->burstTime = tempBurst;
			}
		}
	}

	i = head1;
	j = head1->next;

	i->waitingTime = 0;

	while (j != NULL) {
		j->waitingTime = i->waitingTime + i->burstTime;
		i = i->next;
		j = j->next;
	}

	i = head1;

	while (i != NULL) {
		i->turnAroundTime = i->burstTime + i->waitingTime;
		i = i->next;
	}

	j = head1;
	
	pthread_mutex_lock(&tatSum_mutex);
	turnAroundTimeSum = 0;
	pthread_mutex_unlock(&tatSum_mutex);

	pthread_mutex_lock(&wtSum_mutex);
	waitingTimeSum = 0;
	pthread_mutex_unlock(&wtSum_mutex);

	while (j != NULL) {
	
		pthread_mutex_lock(&tatSum_mutex);
		turnAroundTimeSum = turnAroundTimeSum + j->turnAroundTime;
		pthread_mutex_unlock(&tatSum_mutex);

		pthread_mutex_lock(&wtSum_mutex);
		waitingTimeSum = waitingTimeSum + j->waitingTime;
		pthread_mutex_unlock(&wtSum_mutex);
		j = j->next;
	}

	pthread_mutex_lock(&tatSum_mutex);
	double turnAroundTimeAvg = (float)turnAroundTimeSum / (float)numOfProcesses;
	pthread_mutex_unlock(&tatSum_mutex);

	pthread_mutex_lock(&wtSum_mutex);
	double waitingTimeAvg = (float)waitingTimeSum / (float)numOfProcesses;
	pthread_mutex_unlock(&wtSum_mutex);

	printf("*****************************************************************************************************************\n");
	printf("Shortest Job First Non-preemtive Scheduling Algorithm in threaded run\n");
	printf("Process\t Burst Time\t Waiting Time\t Turn Around Time\n\n");
	
	for (i = head1; i != NULL; i = i->next)
		printf("Process [%d]\t %d\t\t\t %d\t\t\t %d\n", i->pid+1, i->burstTime, i->waitingTime, i->turnAroundTime);

	printf("\n\nAverage Waiting Time = %lf\n", waitingTimeAvg);
	printf("Average Turn Around Time = %lf\n", turnAroundTimeAvg);
	sec = ((double)time)/CLOCKS_PER_SEC;
	printf("\n\ntotal Time it took = %lfs\n", sec);
	printf("\n\n");
	delete_list(temp1);
	return 0;
}

void preSRTF_algo()
{
	clock_t _time = clock();
	double sec = -1;
	int arrivalTime[numOfProcesses], burstTime[numOfProcesses + 1], tempBurst[numOfProcesses];
	int waitingTime[numOfProcesses], turnAroundTime[numOfProcesses], completionTime[numOfProcesses];
	int i, smallest, time;
	double endTime;
	node *pointer = HEAD;
	int counter = 0;

	int mynum = numOfProcesses;

	for (i = 0; i < mynum; i++) {
		if (pointer != NULL) {
			arrivalTime[i] = pointer->arrivalTime;
			burstTime[i] = pointer->burstTime;
			tempBurst[i] = burstTime[i];

			pointer = pointer->next;
		}
	}

	burstTime[mynum] = INT_MAX;

	for (time = 0; counter != mynum; time++) {
		smallest = mynum;

		for (i = 0; i < mynum; i++) {
			if (arrivalTime[i] <= time && burstTime[i] < burstTime[smallest] && burstTime[i]) 
				smallest = i;
		}

		burstTime[smallest]--;

		if (burstTime[smallest] == 0) {
			counter++;
			endTime = time+1;
			completionTime[smallest] = endTime;
			waitingTime[smallest] = endTime - arrivalTime[smallest] - tempBurst[smallest];
			turnAroundTime[smallest] = endTime - arrivalTime[smallest];	
		}
	}

	waitingTimeSum = 0, turnAroundTimeSum = 0;
	
	for (i = 0; i < mynum; i++) {
		waitingTimeSum += waitingTime[i];

		turnAroundTimeSum += turnAroundTime[i];
	}

	double waitingTimeAvg = (float)waitingTimeSum / (float)numOfProcesses;
	double turnAroundTimeAvg = (float)turnAroundTimeSum / (float)numOfProcesses;

	printf("*****************************************************************************************************************\n");
	printf("Shortest Remaining Time Scheduling Algorithm in sequential run\n");
	printf("Process ID \t Arrival Time \t Burst Time \t\t Turnaround Time \t\t Waiting Time \n");

	for (i = 0; i < mynum; i++)
		printf("%d \t %d \t %d \t\t\t %d \t\t\t %d\n", i+1, arrivalTime[i], burstTime[i], turnAroundTime[i], waitingTime[i]);
	
	printf("\n\nAverage waiting time = %lf\n", waitingTimeAvg);
	printf("Average turnaround time = %lf\n", turnAroundTimeAvg);
	sec = ((double)_time)/CLOCKS_PER_SEC;
	printf("\n\ntotal Time it took = %lfs\n", sec);
	printf("\n\n");
}

void *preSRTF(void *arg)
{
	clock_t _time = clock();
	double sec = -1;
	int arrivalTime[numOfProcesses], burstTime[numOfProcesses], tempBurst[numOfProcesses];
	int waitingTime[numOfProcesses], turnAroundTime[numOfProcesses], completionTime[numOfProcesses];
	int i, smallest, time;
	double endTime;
	
	pthread_mutex_lock(&head_mutex);
	node *pointer = HEAD;
	pthread_mutex_unlock(&head_mutex);
	int counter = 0;


	pthread_mutex_lock(&num_mutex);
	int mynum = numOfProcesses;
	pthread_mutex_unlock(&num_mutex);

	for (i = 0; i < mynum; i++) {
		if (pointer != NULL) {
			arrivalTime[i] = pointer->arrivalTime;
			burstTime[i] = pointer->burstTime;
			tempBurst[i] = burstTime[i];

			pointer = pointer->next;
		}
	}

	burstTime[mynum] = -1;

	for (time = 0; counter != mynum; time++) {
		smallest = mynum;

		for (i = 0; i < mynum; i++) {
			if (arrivalTime[i] <= time && burstTime[i] < burstTime[smallest] && burstTime[i])
				smallest = i;
		}

		burstTime[smallest]--;

		if (burstTime[smallest] == 0) {
			counter++;
			endTime = time+1;
			completionTime[smallest] = endTime;
			waitingTime[smallest] = endTime - arrivalTime[smallest] - tempBurst[smallest];
			turnAroundTime[smallest] = endTime - arrivalTime[smallest];	
		}
	}

	waitingTimeSum = 0, turnAroundTimeSum = 0;
	
	for (i = 0; i < mynum; i++) {
		pthread_mutex_lock(&wtSum_mutex);
		waitingTimeSum += waitingTime[i];
		pthread_mutex_unlock(&wtSum_mutex);

		pthread_mutex_lock(&tatSum_mutex);
		turnAroundTimeSum += turnAroundTime[i];
		pthread_mutex_unlock(&tatSum_mutex);
	}

	pthread_mutex_lock(&wtSum_mutex);
	double waitingTimeAvg = (float)waitingTimeSum / (float)numOfProcesses;
	pthread_mutex_unlock(&wtSum_mutex);
	pthread_mutex_lock(&tatSum_mutex);
	double turnAroundTimeAvg = (float)turnAroundTimeSum / (float)numOfProcesses;
	pthread_mutex_unlock(&tatSum_mutex);

	printf("*****************************************************************************************************************\n");
	printf("Shortest Remaining Time Scheduling Algorithm in threaded run\n");
	printf("Process ID \t Arrival Time \t Burst Time \t\t Turnaround Time \t\t Waiting Time \n");

	for (i = 0; i < mynum; i++)
		printf("%d \t %d \t %d \t\t\t %d \t\t\t %d\n", i+1, arrivalTime[i], burstTime[i], turnAroundTime[i], waitingTime[i]);
	
	printf("\n\nAverage waiting time = %lf\n", waitingTimeAvg);
	printf("Average turnaround time = %lf\n", turnAroundTimeAvg);
	sec = ((double)_time)/CLOCKS_PER_SEC;
	printf("\n\ntotal Time it took = %lfs\n", sec);
	printf("\n\n");
	return 0;
}

void priorityNonPre_algo()
{
	clock_t time = clock();
	double sec = -1;
	node *head2 = NULL;
	node *temp2 = NULL;
	node *p2 = NULL;
	node *current2 = HEAD;


	int mynum = numOfProcesses;

	for (int i = 0; i < mynum; i++) {
		temp2 = (node *) malloc(sizeof(node));
		temp2->pid = current2->pid;
		temp2->burstTime = current2->burstTime;
		temp2->processPriority = current2->processPriority;
		temp2->next = NULL;
		current2 = current2->next;

		if (head2 == NULL)
			head2 = temp2;

		else {
			p2 = head2;
			
			while (p2->next != NULL)
				p2 = p2->next;

			p2->next = temp2;
		}
	}

	node *i, *j;
	int tempPid, tempBurst, tempPriority;

	for (i = head2; i->next != NULL; i = i->next) {

		for (j = i->next; j != NULL; j = j->next) {

			if (i->processPriority > j->processPriority) {
				tempPid = i->pid;
				tempBurst = i->burstTime;
				tempPriority = i->processPriority;

				i->pid = j->pid;
				i->burstTime = j->burstTime;
				i->processPriority = j->processPriority;
				
				j->pid = tempPid;
				j->burstTime = tempBurst;
				j->processPriority = tempPriority;
			}
		}
	}

	i = head2;
	j = head2->next;

	i->waitingTime = 0;

	while (j != NULL) {
		j->waitingTime = i->waitingTime + i->burstTime;
		i = i->next;
		j = j->next;
	}

	i = head2;

	while (i != NULL) {
		i->turnAroundTime = i->burstTime + i->waitingTime;
		i = i->next;
	}

	j = head2;
	
	turnAroundTimeSum = 0;	

	waitingTimeSum = 0;

	while (j != NULL) {
		turnAroundTimeSum = turnAroundTimeSum + j->turnAroundTime;

		waitingTimeSum = waitingTimeSum + j->waitingTime;
		j = j->next;
	}

	double turnAroundTimeAvg = (float)turnAroundTimeSum / (float)numOfProcesses;


	double waitingTimeAvg = (float)waitingTimeSum / (float)numOfProcesses;

	printf("*****************************************************************************************************************\n");
	printf("Non-preemtive Priority Scheduling Algorithm in sequential run\n");
	printf("Process\t Burst Time\t Waiting Time\t Turn Around Time\n\n");
	
	for (i = head2; i != NULL; i = i->next)
		printf("Process [%d]\t %d\t\t\t %d\t\t\t %d\n", i->pid+1, i->burstTime, i->waitingTime, i->turnAroundTime);

	printf("\n\nAverage Waiting Time = %lf\n", waitingTimeAvg);
	printf("Average Turn Around Time = %lf\n", turnAroundTimeAvg);
	sec = ((double)time)/CLOCKS_PER_SEC;
	printf("\n\ntotal Time it took = %lfs\n", sec);
	printf("\n\n");
	delete_list(temp2);
}

void *priorityNonPre(void *arg)
{
	clock_t time = clock();
	double sec = -1;
	node *head2 = NULL;
	node *temp2 = NULL;
	node *p2 = NULL;
	pthread_mutex_lock(&head_mutex);
	node *current2 = HEAD;
	pthread_mutex_unlock(&head_mutex);


	pthread_mutex_lock(&num_mutex);
	int mynum = numOfProcesses;
	pthread_mutex_unlock(&num_mutex);

	for (int i = 0; i < mynum; i++) {
		temp2 = (node *) malloc(sizeof(node));
		temp2->pid = current2->pid;
		temp2->burstTime = current2->burstTime;
		temp2->processPriority = current2->processPriority;
		temp2->next = NULL;
		current2 = current2->next;

		if (head2 == NULL)
			head2 = temp2;

		else {
			p2 = head2;
			
			while (p2->next != NULL)
				p2 = p2->next;

			p2->next = temp2;
		}
	}

	node *i, *j;
	int tempPid, tempBurst, tempPriority;

	for (i = head2; i->next != NULL; i = i->next) {

		for (j = i->next; j != NULL; j = j->next) {

			if (i->processPriority > j->processPriority) {
				tempPid = i->pid;
				tempBurst = i->burstTime;
				tempPriority = i->processPriority;

				i->pid = j->pid;
				i->burstTime = j->burstTime;
				i->processPriority = j->processPriority;
				
				j->pid = tempPid;
				j->burstTime = tempBurst;
				j->processPriority = tempPriority;
			}
		}
	}

	i = head2;
	j = head2->next;

	i->waitingTime = 0;

	while (j != NULL) {
		j->waitingTime = i->waitingTime + i->burstTime;
		i = i->next;
		j = j->next;
	}

	i = head2;

	while (i != NULL) {
		i->turnAroundTime = i->burstTime + i->waitingTime;
		i = i->next;
	}

	j = head2;
	
	pthread_mutex_lock(&tatSum_mutex);
	turnAroundTimeSum = 0;	
	pthread_mutex_unlock(&tatSum_mutex);

	pthread_mutex_lock(&wtSum_mutex);
	waitingTimeSum = 0;
	pthread_mutex_unlock(&wtSum_mutex);

	while (j != NULL) {
		pthread_mutex_lock(&tatSum_mutex);
		turnAroundTimeSum = turnAroundTimeSum + j->turnAroundTime;
		pthread_mutex_unlock(&tatSum_mutex);

		pthread_mutex_lock(&wtSum_mutex);
		waitingTimeSum = waitingTimeSum + j->waitingTime;
		pthread_mutex_unlock(&wtSum_mutex);
		j = j->next;
	}

	pthread_mutex_lock(&tatSum_mutex);
	double turnAroundTimeAvg = (float)turnAroundTimeSum / (float)numOfProcesses;
	pthread_mutex_unlock(&tatSum_mutex);


	pthread_mutex_lock(&wtSum_mutex);
	double waitingTimeAvg = (float)waitingTimeSum / (float)numOfProcesses;
	pthread_mutex_unlock(&wtSum_mutex);

	printf("*****************************************************************************************************************\n");
	printf("Non-preemtive Priority Scheduling Algorithm in threaded run\n");
	printf("Process\t Burst Time\t Waiting Time\t Turn Around Time\n\n");
	
	for (i = head2; i != NULL; i = i->next)
		printf("Process [%d]\t %d\t\t\t %d\t\t\t %d\n", i->pid+1, i->burstTime, i->waitingTime, i->turnAroundTime);

	printf("\n\nAverage Waiting Time = %lf\n", waitingTimeAvg);
	printf("Average Turn Around Time = %lf\n", turnAroundTimeAvg);
	sec = ((double)time)/CLOCKS_PER_SEC;
	printf("\n\ntotal Time it took = %lfs\n", sec);
	printf("\n\n");
	free(temp2);
	return 0;
}

void roundRobin_algo()
{
	clock_t time = clock();
	double sec = -1;
	node *head = NULL;
	node *temp = NULL;
	node *p = NULL;
	
	
	node *current = HEAD;


	int mynum = numOfProcesses;

	for (int i = 0; i < mynum; i++) {
		temp = (node *) malloc(sizeof(node));
		temp->pid = current->pid;
		temp->burstTime = current->burstTime;
		temp->arrivalTime = current->arrivalTime;
		temp->tempBurst = current->tempBurst;
		temp->next = NULL;
		current = current->next;

		if (head == NULL)
			head = temp;

		else {
			p = head;
			
			while (p->next != NULL)
				p = p->next;

			p->next = temp;
		}
	}

	node *j, *i = head;
	int x = mynum, counter = 0, total = 0;

	printf("*****************************************************************************************************************\n");
	printf("Round Robin Scheduling Algorithm in sequential run\n");
	printf("Process ID \t Burst Time \t\t Turnaround Time \t\t Waiting Time \n");

	for (total = 0; x != 0;) {
		if (i->tempBurst <= quantumTime && i->tempBurst > 0) {
			total += i->tempBurst;
			i->tempBurst = 0;
			counter = 1;
		}

		else if (i->tempBurst > 0) {
			i->tempBurst -= quantumTime;
			total += quantumTime;
		}

		if (i->tempBurst == 0 && counter == 1) {
			x--;
			printf("Process[%d] \t %d \t\t\t %d \t\t\t %d\n", i->pid+1, i->burstTime, total - i->arrivalTime, total - i->arrivalTime - i->burstTime);
			i->waitingTime = i->waitingTime + total - i->arrivalTime - i->burstTime;
                  	i->turnAroundTime = i->turnAroundTime + total - i->arrivalTime;
                  	counter = 0;
		}

		if (i->pid == mynum - 1)
			i = head;

		else if (i->next->arrivalTime <= total)
			i = i->next;

		else
			i = head;
	}

	j = head;
	turnAroundTimeSum = 0;

	waitingTimeSum = 0;


	while (j != NULL) {
		turnAroundTimeSum = turnAroundTimeSum + j->turnAroundTime;

		waitingTimeSum = waitingTimeSum + j->waitingTime;
		j = j->next;
	}

	double turnAroundTimeAvg = (float)turnAroundTimeSum / (float)numOfProcesses;

	double waitingTimeAvg = (float)waitingTimeSum / (float)numOfProcesses;

	printf("\n\nAverage Waiting Time = %lf\n", waitingTimeAvg);
	printf("Average Turn Around Time = %lf\n", turnAroundTimeAvg);
	sec = ((double)time)/CLOCKS_PER_SEC;
	printf("\n\ntotal Time it took = %lfs\n", sec);
	printf("\n\n");
	free(temp);
}

void *roundRobin(void *arg)
{
	clock_t time = clock();
	double sec = -1;
	node *head = NULL;
	node *temp = NULL;
	node *p = NULL;
	
	
	pthread_mutex_lock(&head_mutex);
	node *current = HEAD;
	pthread_mutex_unlock(&head_mutex);


	pthread_mutex_lock(&num_mutex);
	int mynum = numOfProcesses;
	pthread_mutex_unlock(&num_mutex);

	for (int i = 0; i < mynum; i++) {
		temp = (node *) malloc(sizeof(node));
		temp->pid = current->pid;
		temp->burstTime = current->burstTime;
		temp->arrivalTime = current->arrivalTime;
		temp->tempBurst = current->tempBurst;
		temp->next = NULL;
		current = current->next;

		if (head == NULL)
			head = temp;

		else {
			p = head;
			
			while (p->next != NULL)
				p = p->next;

			p->next = temp;
		}
	}

	node *j, *i = head;
	int x = mynum, counter = 0, total = 0;

	printf("*****************************************************************************************************************\n");
	printf("Round Robin Scheduling Algorithm in threaded run\n");
	printf("Process ID \t Burst Time \t\t Turnaround Time \t\t Waiting Time \n");

	for (total = 0; x != 0;) {
		if (i->tempBurst <= quantumTime && i->tempBurst > 0) {
			total += i->tempBurst;
			i->tempBurst = 0;
			counter = 1;
		}

		else if (i->tempBurst > 0) {
			i->tempBurst -= quantumTime;
			total += quantumTime;
		}

		if (i->tempBurst == 0 && counter == 1) {
			x--;
			printf("Process[%d] \t %d \t\t\t %d \t\t\t %d\n", i->pid+1, i->burstTime, total - i->arrivalTime, total - i->arrivalTime - i->burstTime);
			i->waitingTime = i->waitingTime + total - i->arrivalTime - i->burstTime;
                  	i->turnAroundTime = i->turnAroundTime + total - i->arrivalTime;
                  	counter = 0;
		}

		if (i->pid == mynum - 1)
			i = head;

		else if (i->next->arrivalTime <= total)
			i = i->next;

		else
			i = head;
	}

	j = head;
	pthread_mutex_lock(&tatSum_mutex);
	turnAroundTimeSum = 0;
	pthread_mutex_unlock(&tatSum_mutex);

	pthread_mutex_lock(&wtSum_mutex);
	waitingTimeSum = 0;
	pthread_mutex_unlock(&wtSum_mutex);


	while (j != NULL) {
		pthread_mutex_lock(&tatSum_mutex);
		turnAroundTimeSum = turnAroundTimeSum + j->turnAroundTime;
		pthread_mutex_unlock(&tatSum_mutex);

		pthread_mutex_lock(&wtSum_mutex);
		waitingTimeSum = waitingTimeSum + j->waitingTime;
		pthread_mutex_unlock(&wtSum_mutex);
		j = j->next;
	}

	pthread_mutex_lock(&tatSum_mutex);
	double turnAroundTimeAvg = (float)turnAroundTimeSum / (float)numOfProcesses;
	pthread_mutex_unlock(&tatSum_mutex);

	pthread_mutex_lock(&wtSum_mutex);
	double waitingTimeAvg = (float)waitingTimeSum / (float)numOfProcesses;
	pthread_mutex_unlock(&wtSum_mutex);

	printf("\n\nAverage Waiting Time = %lf\n", waitingTimeAvg);
	printf("Average Turn Around Time = %lf\n", turnAroundTimeAvg);
	sec = ((double)time)/CLOCKS_PER_SEC;
	printf("\n\ntotal Time it took = %lfs\n", sec);
	printf("\n\n");
	free(temp);
	return 0;
}

void *prePriority(void *arg)
{
	clock_t _time = clock();
	double sec = -1;
	int arrivalTime[numOfProcesses], burstTime[numOfProcesses], tempBurst[numOfProcesses];
	int waitingTime[numOfProcesses], turnAroundTime[numOfProcesses], completionTime[numOfProcesses], processPriority[numOfProcesses];
	int i, smallest, time;
	double endTime;
	pthread_mutex_lock(&head_mutex);
	node *pointer = HEAD;
	pthread_mutex_unlock(&head_mutex);
	int counter = 0;


	pthread_mutex_lock(&num_mutex);
	int mynum = numOfProcesses;
	pthread_mutex_unlock(&num_mutex);

	for (i = 0; i < mynum; i++) {
		if (pointer != NULL) {
			arrivalTime[i] = pointer->arrivalTime;
			burstTime[i] = pointer->burstTime;
			tempBurst[i] = burstTime[i];
			processPriority[i] = pointer->processPriority;

			pointer = pointer->next;
		}
	}

	processPriority[mynum] = -1;

	for (time = 0; counter != mynum; time++) {
		smallest = mynum-1;

		for (i = 0; i < mynum; i++) {
			if (arrivalTime[i] <= time && processPriority[i] > processPriority[smallest] && burstTime[i])
				smallest = i;
		}

		burstTime[smallest]--;

		if (burstTime[smallest] == 0) {
			counter++;
			endTime = time+1;
			completionTime[smallest] = endTime;
			waitingTime[smallest] = endTime - arrivalTime[smallest] - tempBurst[smallest];
			turnAroundTime[smallest] = endTime - arrivalTime[smallest];	
		}
	}

	pthread_mutex_lock(&wtSum_mutex);
	waitingTimeSum = 0;
	pthread_mutex_unlock(&wtSum_mutex);

	pthread_mutex_lock(&tatSum_mutex);
	turnAroundTimeSum = 0;
	pthread_mutex_unlock(&tatSum_mutex);
	
	for (i = 0; i < mynum; i++) {
		pthread_mutex_lock(&wtSum_mutex);
		waitingTimeSum += waitingTime[i];
		pthread_mutex_unlock(&wtSum_mutex);
		pthread_mutex_lock(&tatSum_mutex);
		turnAroundTimeSum += turnAroundTime[i];
		pthread_mutex_unlock(&tatSum_mutex);
	}

	pthread_mutex_lock(&wtSum_mutex);
	double waitingTimeAvg = (float)waitingTimeSum / (float)numOfProcesses;
	pthread_mutex_unlock(&wtSum_mutex);
	pthread_mutex_lock(&tatSum_mutex);
	double turnAroundTimeAvg = (float)turnAroundTimeSum / (float)numOfProcesses;
	pthread_mutex_unlock(&tatSum_mutex);

	printf("*****************************************************************************************************************\n");
	printf("Preemtive Priority Scheduling Algorithm in threaded run\n");
	printf("Process ID \t Arrival Time \t Burst Time \t\t Turnaround Time \t\t Waiting Time \n");

	for (i = 0; i < mynum; i++)
		printf("%d \t %d \t %d \t\t\t %d \t\t\t %d\n", i+1, arrivalTime[i], burstTime[i], turnAroundTime[i], waitingTime[i]);
	
	printf("\n\nAverage waiting time = %lf\n", waitingTimeAvg);
	printf("Average turnaround time = %lf\n", turnAroundTimeAvg);
	sec = ((double)_time)/CLOCKS_PER_SEC;
	printf("\n\ntotal Time it took = %lfs\n", sec);
	printf("\n\n");
	printf("%s", buf);
	return 0;
}

void prePriority_algo()
{
	clock_t _time = clock();
	double sec = -1;
	int arrivalTime[numOfProcesses], burstTime[numOfProcesses], tempBurst[numOfProcesses];
	int waitingTime[numOfProcesses], turnAroundTime[numOfProcesses], completionTime[numOfProcesses], processPriority[numOfProcesses + 1];
	int i, smallest, time;
	double endTime;
	//pthread_mutex_lock(&head_mutex);
	node *pointer = HEAD;
	//pthread_mutex_unlock(&head_mutex);
	int counter = 0;


	//pthread_mutex_lock(&num_mutex);
	//int mynum = numOfProcesses;
	//pthread_mutex_unlock(&num_mutex);

	for (i = 0; i < numOfProcesses; i++) {
		if (pointer != NULL) {
			arrivalTime[i] = pointer->arrivalTime;
			burstTime[i] = pointer->burstTime;
			tempBurst[i] = burstTime[i];
			processPriority[i] = pointer->processPriority;

			pointer = pointer->next;
		}
	}

	processPriority[numOfProcesses] = INT_MAX;

	for (time = 0; counter != numOfProcesses; time++) {
		smallest = numOfProcesses;

		for (i = 0; i < numOfProcesses; i++) {
			if (arrivalTime[i] <= time && processPriority[i] < processPriority[smallest] && burstTime[i])
				smallest = i;
		}

		burstTime[smallest]--;

		if (burstTime[smallest] == 0) {
			counter++;
			endTime = time+1;
			completionTime[smallest] = endTime;
			waitingTime[smallest] = endTime - arrivalTime[smallest] - tempBurst[smallest];
			turnAroundTime[smallest] = endTime - arrivalTime[smallest];	
		}
	}

	//pthread_mutex_lock(&wtSum_mutex);
	waitingTimeSum = 0;
	//pthread_mutex_unlock(&wtSum_mutex);

	//pthread_mutex_lock(&tatSum_mutex);
	turnAroundTimeSum = 0;
	//pthread_mutex_unlock(&tatSum_mutex);
	
	for (i = 0; i < numOfProcesses; i++) {
		//pthread_mutex_lock(&wtSum_mutex);
		waitingTimeSum += waitingTime[i];
		//pthread_mutex_unlock(&wtSum_mutex);
		//pthread_mutex_lock(&tatSum_mutex);
		turnAroundTimeSum += turnAroundTime[i];
		//pthread_mutex_unlock(&tatSum_mutex);
	}

	//pthread_mutex_lock(&wtSum_mutex);
	double waitingTimeAvg = (float)waitingTimeSum / (float)numOfProcesses;
	//pthread_mutex_unlock(&wtSum_mutex);
	//pthread_mutex_lock(&tatSum_mutex);
	double turnAroundTimeAvg = (float)turnAroundTimeSum / (float)numOfProcesses;
	//pthread_mutex_unlock(&tatSum_mutex);

	printf("*****************************************************************************************************************\n");
	printf("Preemtive Priority Scheduling Algorithm in sequential run\n");
	printf("Process ID \t Arrival Time \t Burst Time \t\t Turnaround Time \t\t Waiting Time \n");

	for (i = 0; i < numOfProcesses; i++)
		printf("%d \t %d \t %d \t\t\t %d \t\t\t %d\n", i+1, arrivalTime[i], burstTime[i], turnAroundTime[i], waitingTime[i]);
	
	printf("\n\nAverage waiting time = %lf\n", waitingTimeAvg);
	printf("Average turnaround time = %lf\n", turnAroundTimeAvg);
	sec = ((double)_time)/CLOCKS_PER_SEC;
	printf("\n\ntotal Time it took = %lfs\n", sec);
	printf("\n\n");
}

int main(int argc, char **argv)
{
	int outputFD = open("output.txt", O_WRONLY | O_TRUNC | O_CREAT, 0666);
	if (outputFD == 0) {
		printf("Error in opening the file.\n");
		exit(0);
	}

	//dup2(outputFD, STDOUT_FILENO);
	pthread_t tid[6];
	if (argc < 2) {
		printf("Error in argv\n");
		exit(1);
	}
	HEAD = createList(argv[1]);
	nonPreemtiveSJF_algo();
	printf("\n\n");
	roundRobin_algo();
	printf("\n\n");
	FCFS_algo();
	printf("\n");
	preSRTF_algo();
	printf("\n\n");
	priorityNonPre_algo();
	printf("\n\n");
	prePriority_algo();

	pthread_create(&tid[0], NULL, &FCFS, NULL);
	pthread_create(&tid[1], NULL, &roundRobin, NULL);
	pthread_create(&tid[2], NULL, &nonPreemtiveSJF, NULL);
	pthread_create(&tid[3], NULL, &preSRTF, NULL);
	pthread_create(&tid[4], NULL, &priorityNonPre, NULL);
	pthread_create(&tid[5], NULL, &prePriority, NULL);

	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);
	pthread_join(tid[2], NULL);
	pthread_join(tid[3], NULL);
	pthread_join(tid[4], NULL);
	pthread_join(tid[5], NULL);

	return 0;
}

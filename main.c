#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
	int pid;
	int processPriority;
	int lastRunTime;
	int arrivalTime;
	int burstTime;
	int waitingTime;
	int turnAroundTime;
	int tempBurst;
	struct node *next;
}node;

int numOfProcesses, quantumTime, turnAroundTimeSum, waitingTimeSum;
double turnAroundTimeAvg, waitingTimeAvg;
node *HEAD = NULL;

node *createList()
{
	node *head = NULL;
	node *temp = NULL;
	node *p = NULL;
	int n;
	
	FILE *myfile = fopen("source.txt", "r");
	
	if( myfile == NULL){
                printf("ERROR: opening file \n");
                exit(1);
        }

	fscanf(myfile, "%d", &quantumTime);

	for (int i = 0; i < numOfProcesses; i++) {
		temp = (node *) malloc(sizeof(node));
		temp->pid = i;
		
		fscanf(myfile, "%d", &(temp->burstTime));
		
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
	}	
	
	fclose(myfile);

	return head;
}

void FCFS()
{
	node *head1 = NULL;
	node *temp1 = NULL;
	node *p1 = NULL;

	node *current = HEAD;

	for (int i = 0; i < numOfProcesses; i++) {
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

	turnAroundTimeSum = 0;
	waitingTimeSum = 0;

	while (j != NULL) {
		turnAroundTimeSum = turnAroundTimeSum + j->turnAroundTime;
		waitingTimeSum = waitingTimeSum + j->waitingTime;
		j = j->next;
	}

	turnAroundTimeAvg = (float)turnAroundTimeSum / (float)numOfProcesses;
	waitingTimeAvg = (float)waitingTimeSum / (float)numOfProcesses;

	printf("First Come First Served Scheduling Algorithm\n");
	printf("Process\t Burst Time\t Waiting Time\t Turn Around Time\n\n");
	
	for (i = head1; i != NULL; i = i->next)
		printf("Process [%d]\t %d\t\t\t %d\t\t\t %d\n", i->pid+1, i->burstTime, i->waitingTime, i->turnAroundTime);

	printf("\n\nAverage Waiting Time = %lf\n", waitingTimeAvg);
	printf("\n\nAverage Turnaround Time = %lf\n", turnAroundTimeAvg);
}

void nonPreemtiveSJF()
{
	node *head1 = NULL;
	node *temp1 = NULL;
	node *p1 = NULL;
	node *current1 = HEAD;

	for (int i = 0; i < numOfProcesses; i++) {
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

	turnAroundTimeAvg = (float)turnAroundTimeSum / (float)numOfProcesses;
	waitingTimeAvg = (float)waitingTimeSum / (float)numOfProcesses;

	printf("*****************************************************************************************************************\n");
	printf("Shortest Job First Non-preemtive Scheduling Algorithm\n");
	printf("Process\t Burst Time\t Waiting Time\t Turn Around Time\n\n");
	
	for (i = head1; i != NULL; i = i->next)
		printf("Process [%d]\t %d\t\t\t %d\t\t\t %d\n", i->pid+1, i->burstTime, i->waitingTime, i->turnAroundTime);

	printf("\n\nAverage Waiting Time = %lf\n", waitingTimeAvg);
	printf("Average Turn Around Time = %lf\n", turnAroundTimeAvg);
}

void preSRTF()
{
	int arrivalTime[numOfProcesses], burstTime[numOfProcesses], tempBurst[numOfProcesses];
	int waitingTime[numOfProcesses], turnAroundTime[numOfProcesses], completionTime[numOfProcesses];
	int i, smallest, time;
	double endTime;
	node *pointer = HEAD;
	int counter = 0;

	for (i = 0; i < numOfProcesses; i++) {
		if (pointer != NULL) {
			arrivalTime[i] = pointer->arrivalTime;
			burstTime[i] = pointer->burstTime;
			tempBurst[i] = burstTime[i];

			pointer = pointer->next;
		}
	}

	burstTime[numOfProcesses] = -1;

	for (time = 0; counter != numOfProcesses; time++) {
		smallest = numOfProcesses-1;

		for (i = 0; i < numOfProcesses; i++) {
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
	
	for (i = 0; i < numOfProcesses; i++) {
		waitingTimeSum += waitingTime[i];
		turnAroundTimeSum += turnAroundTime[i];
	}

	waitingTimeAvg = (float)waitingTimeSum / (float)numOfProcesses;
	turnAroundTimeAvg = (float)turnAroundTimeSum / (float)numOfProcesses;

	printf("*****************************************************************************************************************\n");
	printf("Shortest Remaining Time Scheduling Algorithm\n");
	printf("Process ID \t Arrival Time \t Burst Time \t\t Turnaround Time \t\t Waiting Time \n");

	for (i = 0; i < numOfProcesses; i++)
		printf("%d \t %d \t %d \t\t\t %d \t\t\t %d\n", i+1, arrivalTime[i], burstTime[i], turnAroundTime[i], waitingTime[i]);
	
	printf("\n\nAverage waiting time = %lf\n", waitingTimeAvg);
	printf("Average turnaround time = %lf\n", turnAroundTimeAvg);
}

void priorityNonPre()
{
	node *head2 = NULL;
	node *temp2 = NULL;
	node *p2 = NULL;
	node *current2 = HEAD;

	for (int i = 0; i < numOfProcesses; i++) {
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

	turnAroundTimeAvg = (float)turnAroundTimeSum / (float)numOfProcesses;
	waitingTimeAvg = (float)waitingTimeSum / (float)numOfProcesses;

	printf("*****************************************************************************************************************\n");
	printf("Non-preemtive Priority Scheduling Algorithm\n");
	printf("Process\t Burst Time\t Waiting Time\t Turn Around Time\n\n");
	
	for (i = head2; i != NULL; i = i->next)
		printf("Process [%d]\t %d\t\t\t %d\t\t\t %d\n", i->pid+1, i->burstTime, i->waitingTime, i->turnAroundTime);

	printf("\n\nAverage Waiting Time = %lf\n", waitingTimeAvg);
	printf("Average Turn Around Time = %lf\n", turnAroundTimeAvg);
}

void roundRobin()
{
	node *head = NULL;
	node *temp = NULL;
	node *p = NULL;
	node *current = HEAD;

	for (int i = 0; i < numOfProcesses; i++) {
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
	int x = numOfProcesses, counter = 0, total = 0;

	printf("*****************************************************************************************************************\n");
	printf("Round Robin Scheduling Algorithm\n");
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

		if (i->pid == numOfProcesses - 1)
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

	turnAroundTimeAvg = (float)turnAroundTimeSum / (float)numOfProcesses;
	waitingTimeAvg = (float)waitingTimeSum / (float)numOfProcesses;

	printf("\n\nAverage Waiting Time = %lf\n", waitingTimeAvg);
	printf("Average Turn Around Time = %lf\n", turnAroundTimeAvg);
}

void prePriority()
{
	int arrivalTime[numOfProcesses], burstTime[numOfProcesses], tempBurst[numOfProcesses];
	int waitingTime[numOfProcesses], turnAroundTime[numOfProcesses], completionTime[numOfProcesses], processPriority[numOfProcesses];
	int i, smallest, time;
	double endTime;
	node *pointer = HEAD;
	int counter = 0;

	for (i = 0; i < numOfProcesses; i++) {
		if (pointer != NULL) {
			arrivalTime[i] = pointer->arrivalTime;
			burstTime[i] = pointer->burstTime;
			tempBurst[i] = burstTime[i];
			processPriority[i] = pointer->processPriority;

			pointer = pointer->next;
		}
	}

	processPriority[numOfProcesses] = -1;

	for (time = 0; counter != numOfProcesses; time++) {
		smallest = numOfProcesses-1;

		for (i = 0; i < numOfProcesses; i++) {
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

	waitingTimeSum = 0, turnAroundTimeSum = 0;
	
	for (i = 0; i < numOfProcesses; i++) {
		waitingTimeSum += waitingTime[i];
		turnAroundTimeSum += turnAroundTime[i];
	}

	waitingTimeAvg = (float)waitingTimeSum / (float)numOfProcesses;
	turnAroundTimeAvg = (float)turnAroundTimeSum / (float)numOfProcesses;

	printf("*****************************************************************************************************************\n");
	printf("Preemtive Priority Scheduling Algorithm\n");
	printf("Process ID \t Arrival Time \t Burst Time \t\t Turnaround Time \t\t Waiting Time \n");

	for (i = 0; i < numOfProcesses; i++)
		printf("%d \t %d \t %d \t\t\t %d \t\t\t %d\n", i+1, arrivalTime[i], burstTime[i], turnAroundTime[i], waitingTime[i]);
	
	printf("\n\nAverage waiting time = %lf\n", waitingTimeAvg);
	printf("Average turnaround time = %lf\n", turnAroundTimeAvg);
}

int main()
{
	printf("Enter the number of processes: ");
	scanf("%d", &numOfProcesses);
	
	HEAD = createList();
	FCFS();
	printf("\n\n");
	roundRobin();
	printf("\n\n");
	nonPreemtiveSJF();
	printf("\n");
	preSRTF();
	printf("\n\n");
	priorityNonPre();
	printf("\n\n");
	prePriority();
	return 0;
}

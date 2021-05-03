#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
	int pid;
	int quantumTime;
	int processPriority;
	int lastRunTime;
	int arrivalTime;
	int burstTime;
	int waitingTime;
	int turnAroundTime;
	struct node *next;
}node;


node *createList(int numOfProcesses)
{
	node *head = NULL;
	node *temp = NULL;
	node *p = NULL;
	
	for (int i = 0; i < numOfProcesses; i++) {
		temp = (node *) malloc(sizeof(node));
		printf("Enter the process ID [%d]: ", i+1);
		scanf("%d", &(temp->pid));
		
		printf("Enter the quantum time of process [%d]: ", i+1);
		scanf("%d", &(temp->quantumTime));
		
		printf("Enter the arrival time of process [%d]: ", i+1);
		scanf("%d", &(temp->arrivalTime));
		
		printf("Enter the burst time of process [%d]: ", i+1);
		scanf("%d", &(temp->burstTime));
		
		printf("Enter the process priority of process [%d]: ", i+1);
		scanf("%d", &(temp->processPriority));
		
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

	return head;
}

void FCFS(int numOfProcesses, node *head)
{
	node *head1 = NULL;
	node *temp1 = NULL;
	node *p1 = NULL;

	for (int i = 0; i < numOfProcesses; i++) {
		temp1 = (node *) malloc(sizeof(node));
		temp1->pid = head->pid;
		temp1->burstTime = head->burstTime;
		temp1->next = NULL;

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

	int turnAroundTimeSum = 0;
	int waitingTimeSum = 0;

	while (j != NULL) {
		turnAroundTimeSum = turnAroundTimeSum + j->turnAroundTime;
		waitingTimeSum = waitingTimeSum + j->waitingTime;
		j = j->next;
	}

	double turnAroundTimeAvg = turnAroundTimeSum / numOfProcesses;
	double waitingTimeAvg = waitingTimeSum / numOfProcesses;

	printf("Process            Burst Time             Waiting Time             Turn Around Time\n\n");
	
	for (i = head1; i != NULL; i = i->next)
		printf("Process [%d]          %d                  %d                      %d\n", i->pid, i->burstTime, i->waitingTime, i->turnAroundTime);

	printf("\n\nAverage Waiting Time = %lf\n", waitingTimeAvg);
	printf("\n\nAverage Turnaround Time = %lf\n", turnAroundTimeAvg);
}

void nonPreemtiveSJF(int numOfProcesses, node *head)
{
	node *head1 = NULL;
	node *temp1 = NULL;
	node *p1 = NULL;

	for (int i = 0; i < numOfProcesses; i++) {
		temp1 = (node *) malloc(sizeof(node));
		temp1->pid = head->pid;
		temp1->burstTime = head->burstTime;
		temp1->next = NULL;

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

	int turnAroundTimeSum = 0;
	int waitingTimeSum = 0;

	while (j != NULL) {
		turnAroundTimeSum = turnAroundTimeSum + j->turnAroundTime;
		waitingTimeSum = waitingTimeSum + j->waitingTime;
		j = j->next;
	}

	double turnAroundTimeAvg = turnAroundTimeSum / numOfProcesses;
	double waitingTimeAvg = waitingTimeSum / numOfProcesses;

	printf("Process            Burst Time             Waiting Time             Turn Around Time\n\n");
	
	for (i = head1; i != NULL; i = i->next)
		printf("Process [%d]          %d                  %d                      %d\n", i->pid, i->burstTime, i->waitingTime, i->turnAroundTime);

	printf("\n\nAverage Waiting Time = %lf\n", waitingTimeAvg);
	printf("Average Turn Around Time = %lf\n", turnAroundTimeAvg);
}

int main()
{
	int numOfProcesses;
	node *HEAD = NULL;

	printf("Enter the number of processes: ");
	scanf("%d", &numOfProcesses);
	
	HEAD = createList(numOfProcesses);
	FCFS(numOfProcesses, HEAD);
	printf("\n\n");
	nonPreemtiveSJF(numOfProcesses, HEAD);

	return 0;
}

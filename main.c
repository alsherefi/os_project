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
	int n;
	
	FILE *myfile = fopen("source.txt", "r");
	
	if( myfile == NULL){
                printf("ERROR: opening file \n");
                exit(1);
        }

	int qt;
	fscanf(myfile, "%d", &qt);

	for (int i = 0; i < numOfProcesses; i++) {
		temp = (node *) malloc(sizeof(node));
		//printf("Enter the process ID [%d]: ", i+1);
		//fscanf(myfile, "%d", &(temp->pid));
		temp->pid = i;
		
		//printf("Enter the quantum time of process [%d]: ", i+1);
		//fscanf(myfile, "%d", &(temp->quantumTime));
		temp->quantumTime = qt;
		
		//printf("Enter the arrival time of process [%d]: ", i+1);
		fscanf(myfile, "%d", &(temp->burstTime));
		
		//printf("Enter the burst time of process [%d]: ", i+1);
		fscanf(myfile, "%d", &(temp->arrivalTime));
		
		//printf("Enter the process priority of process [%d]: ", i+1);
		fscanf(myfile, "%d", &(temp->processPriority));
		//printf("\n");

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
	//temp = head;
	//for(int i = 0; i < numOfProcesses; i++){
	//	printf("id = %d, qt = %d, at = %d, pp = %d\n\n", temp->pid, temp->quantumTime, temp->arrivalTime, temp->processPriority);
	//	temp = temp->next;
	//}

	return head;
}

void FCFS(int numOfProcesses, node *head)
{
	node *head1 = NULL;
	node *temp1 = NULL;
	node *p1 = NULL;

	node *current = head;

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

	int turnAroundTimeSum = 0;
	int waitingTimeSum = 0;

	while (j != NULL) {
		turnAroundTimeSum = turnAroundTimeSum + j->turnAroundTime;
		waitingTimeSum = waitingTimeSum + j->waitingTime;
		j = j->next;
	}

	double turnAroundTimeAvg = (float)turnAroundTimeSum / (float)numOfProcesses;
	double waitingTimeAvg = (float)waitingTimeSum / (float)numOfProcesses;

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
	node *current1 = head;

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

	int turnAroundTimeSum = 0;
	int waitingTimeSum = 0;

	while (j != NULL) {
		turnAroundTimeSum = turnAroundTimeSum + j->turnAroundTime;
		waitingTimeSum = waitingTimeSum + j->waitingTime;
		j = j->next;
	}

	double turnAroundTimeAvg = (float)turnAroundTimeSum / (float)numOfProcesses;
	double waitingTimeAvg = (float)waitingTimeSum / (float)numOfProcesses;

	printf("Process            Burst Time             Waiting Time             Turn Around Time\n\n");
	
	for (i = head1; i != NULL; i = i->next)
		printf("Process [%d]          %d                  %d                      %d\n", i->pid, i->burstTime, i->waitingTime, i->turnAroundTime);

	printf("\n\nAverage Waiting Time = %lf\n", waitingTimeAvg);
	printf("Average Turn Around Time = %lf\n", turnAroundTimeAvg);
}

void priorityNonPre(int numOfProcesses, node *head)
{
	node *head2 = NULL;
	node *temp2 = NULL;
	node *p2 = NULL;
	node *current2 = head;

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

	int turnAroundTimeSum = 0;
	int waitingTimeSum = 0;

	while (j != NULL) {
		turnAroundTimeSum = turnAroundTimeSum + j->turnAroundTime;
		waitingTimeSum = waitingTimeSum + j->waitingTime;
		j = j->next;
	}

	double turnAroundTimeAvg = (float)turnAroundTimeSum / (float)numOfProcesses;
	double waitingTimeAvg = (float)waitingTimeSum / (float)numOfProcesses;

	printf("Process            Burst Time             Waiting Time             Turn Around Time\n\n");
	
	for (i = head2; i != NULL; i = i->next)
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
	printf("\n");
	priorityNonPre(numOfProcesses, HEAD);
	return 0;
}

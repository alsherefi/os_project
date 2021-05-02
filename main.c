#include <stdio.h>

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

node *head = NULL;
node *temp = NULL;
node *p = NULL;

node *createList(int numOfProcesses)
{
	for (int i = 0; i < numOfProcesses; i++) {
		temp = (node *) malloc(sizeof(node));
		printf("Enter the process ID: ");
		scanf("%d", &(temp->pid));
		printf("Enter the burst time ");
		scanf("%d", &(temp->burstTime));
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

void FCFS(int numOfProcesses, int burstTime[])
{
	int turnAroundTime[numOfProcesses];
	int waitingTime[numOfProcesses];
	int turnAroundTimeSum = 0;
	int waitingTimeSum = 0;

	waitingTime[0] = 0;

	for (int i = 1; i < numOfProcesses; i++)
		waitingTime[i] = waitingTime[i - 1] + burstTime[i - 1];

	for (int j = 0; j < numOfProcesses; j++)
		turnAroundTime[j] = burstTime[j] + waitingTime[j];

	for (int k = 0; k < numOfProcesses; k++)
		turnAroundTimeSum = turnAroundTimeSum + turnAroundTime[k];
	
	double turnAroundTimeAvg = turnAroundTimeSum / numOfProcesses;
	
	for (int j = 0; j < numOfProcesses; j++)
		waitingTimeSum = waitingTimeSum + waitingTime[j];

	double waitingTimeAvg = waitingTimeSum / numOfProcesses;

	printf("Process            Burst Time             Waiting Time             Turn Around Time\n\n");
	
	for (int k = 0; k < numOfProcesses; k++)
		printf("Process [%d]          %d                  %d                      %d\n", k+1, burstTime[k], waitingTime[k], turnAroundTime[k]);

	
	printf("\n\nAverage Waiting Time = %lf\n", waitingTimeAvg);
	printf("Average Turn Around Time = %lf\n", turnAroundTimeAvg);
}

void nonPreemtiveSJF(int numOfProcesses, node *head)
{
	//float turnAroundTime[numOfProcesses];
	//float waitingTime[numOfProcesses];
	int temp1;
	node *current = head;
	node *index = NULL;
	int turnAroundTimeSum = 0;
	int waitingTimeSum = 0;

	head->waitingTime = 0;

	if (head == NULL)
		return;
	else {
		while (current != NULL) {
			index = current->next;

			while (index != NULL) {
				if (current->burstTime > index->burstTime) {
					temp1 = current->burstTime;
					current->burstTime = index->burstTime;
					index->data = temp1;
				}

				index = index->next;
			}

			current = current->next;
		}
	}

	for (int i = 1; i < numOfProcesses; i++)
		waitingTime[i] = waitingTime[i - 1] + burstTime[i - 1];

	for (int j = 0; j < numOfProcesses; j++)
		turnAroundTime[j] = burstTime[j] + waitingTime[j];

	for (int k = 0; k < numOfProcesses; k++)
		turnAroundTimeSum = turnAroundTimeSum + turnAroundTime[k];
	
	double turnAroundTimeAvg = turnAroundTimeSum / numOfProcesses;
	
	for (int j = 0; j < numOfProcesses; j++)
		waitingTimeSum = waitingTimeSum + waitingTime[j];

	double waitingTimeAvg = waitingTimeSum / numOfProcesses;

	printf("Process            Burst Time             Waiting Time             Turn Around Time\n\n");
	
	for (int k = 0; k < numOfProcesses; k++)
		printf("Process [%d]          %d                  %d                      %d\n", k+1, burstTime[k], waitingTime[k], turnAroundTime[k]);

	
	printf("\n\nAverage Waiting Time = %lf\n", waitingTimeAvg);
	printf("Average Turn Around Time = %lf\n", turnAroundTimeAvg);

}

int main()
{
	int numOfProcesses;
	
	printf("Enter The Number of Processes To Execute: ");
	scanf("%d", &numOfProcesses);

	float burstTime[numOfProcesses];
	
	printf("\n\nEnter The Burst Time of Processes:\n\n");

	for (int i = 0; i < numOfProcesses; i++) {
		printf("Process [%d]: ", i+1);
		scanf("%f", &burstTime[i]);
	}

	printf("\n\n");

	FCFS(numOfProcesses, burstTime);

	node *HEAD = NULL;
	HEAD = createList(numOfProcesses);
	return 0;
}

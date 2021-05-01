#include <stdio.h>

void FCFS(int numOfProcesses, float burstTime[])
{
	float turnAroundTime[numOfProcesses];
	float waitingTime[numOfProcesses];
	float turnAroundTimeSum = 0;
	float waitingTimeSum = 0;

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
		printf("Process [%d]          %.2f                  %.2f                      %.2f\n", k+1, burstTime[k], waitingTime[k], turnAroundTime[k]);

	
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

	return 0;
}

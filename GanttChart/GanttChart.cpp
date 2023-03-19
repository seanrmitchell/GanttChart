#include <iostream>
#include <iomanip>
#include <queue>
using namespace std;

int globalTime = 0;
int quantum = 10;


// Constructor for inputs
class GanttChart {
public:
	// Identifies given values, and applies them to the process object
	int procID, serviceTime, arrivalTime;
	// Associated parameters (not yet applied)
	int remainingService, endTime, intialWait, totalWait, turnAround, startTime;

	void CalculateIntitialWait() {
		intialWait = startTime - arrivalTime;
	}

	void CalculateTotalWait() {
		totalWait = endTime - arrivalTime - serviceTime;
	}

	void CalculateTurnAroundTime() {
		turnAround = endTime - arrivalTime;
	}

	GanttChart(int x, int y, int z) {
		procID = x;
		serviceTime = y;
		arrivalTime = z;
		
		remainingService = serviceTime;
		endTime = -1;
		intialWait = 0;
		totalWait = 0;
		turnAround = 0;
		startTime = 0;

		//cout << "ID" << " " << this->procID << " " << "Ready!" << "\n";
	}
};

void BuildTable(GanttChart arr[]) {
	cout
		<< left
		<< setw(5)
		<< "ID"
		<< setw(15)
		<< "Service Time"
		<< setw(15)
		<< "Arrival Time"
		<< setw(15)
		<< "Start Time"
		<< setw(15)
		<< "End Time"
		<< setw(15)
		<< "Intital Wait"
		<< setw(15)
		<< "Total Wait"
		<< setw(15)
		<< "Turn Around"
		<< endl;

	for (int i = 0; i < 5; i++) {
		cout
			<< left
			<< setw(5)
			<< arr[i].procID
			<< left
			<< setw(15)
			<< arr[i].serviceTime
			<< left
			<< setw(15)
			<< arr[i].arrivalTime
			<< left
			<<setw(15)
			<<arr[i].startTime
			<< left
			<< setw(15)
			<< arr[i].endTime
			<< left
			<< setw(15)
			<< arr[i].intialWait
			<< left
			<< setw(15)
			<< arr[i].totalWait
			<< left
			<< setw(15)
			<< arr[i].turnAround
			<< endl;
	}
}

//Main method
int main() {
	// Identifies processes parameters for constructor
	GanttChart arr[5] = {
		GanttChart(1, 75, 0),
		GanttChart(2, 40, 10),
		GanttChart(3, 25, 10),
		GanttChart(4, 20, 80),
		GanttChart(5, 45, 85)
	};

	queue<GanttChart> ready;
	ready.push(arr[0]);
	arr[0].startTime = 0;
	int x = 1;
	
	// Performs chart simulation of processor
	while (!ready.empty()) {
		
		// Determines whether a entity should enter ready queue based on arrival time
		if (arr[x].arrivalTime <= globalTime && x < 5) {
			ready.push(arr[x]);
			ready.push(ready.front());
			ready.pop();
			x++;
		}

		// Processes how much of service is remaining, and set globalTime to the remaining service time.
		if (ready.front().remainingService <= quantum) {
			globalTime += ready.front().remainingService;
			int temp = ready.front().procID - 1;
			arr[temp].endTime = globalTime;
			ready.pop();
		} 
		else {

			if (ready.front().remainingService == ready.front().serviceTime) {
				int temp = ready.front().procID - 1;
				arr[temp].startTime = globalTime;
			}

			ready.front().remainingService -= quantum;
			globalTime += quantum;
			ready.push(ready.front());
			ready.pop();
		}

		
	}

	for (int i = 0; i < 5; i++) {
		arr[i].CalculateIntitialWait();
		arr[i].CalculateTotalWait();
		arr[i].CalculateTurnAroundTime();
	}

	BuildTable(arr);

	//cout << "ProcOne EndTime: " << processArray[1].CalculateEndTime();

	return 0;
}
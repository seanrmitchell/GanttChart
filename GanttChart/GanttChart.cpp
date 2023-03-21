// Gantt Chart Demonstration Project
// Completed 3/20/23
// Sean Robert Mitchell

#include <iostream>
#include <iomanip>
#include <queue>
using namespace std;

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

void BuildTable(std::vector<GanttChart> arr) {
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

std::vector<GanttChart> Scheduler(std::vector<GanttChart> arr, int cxtSwitch) {

	// Total processing time
	int globalTime = 0;

	// Ready queue intitialization
	std::queue<GanttChart> ready;
	ready.push(arr[0]);
	arr[0].startTime = 0;
	int x = 1;

	while (!ready.empty()) {

		// Determines whether a entity should enter ready queue based on arrival time
		if (x < arr.size() && arr[x].arrivalTime <= globalTime && ready.front().procID == arr[0].procID) {

			ready.push(arr[x]);

			// Rearranges array to have new arrival at the head, once it is available to start
			for (int i = 1; i < ready.size(); i++) {
				ready.push(ready.front());
				ready.pop();
			}

			x++;
		} 

		// Processes how much of service is remaining, and set globalTime to the remaining service time.
		if (ready.front().remainingService <= quantum) {
			globalTime += ready.front().remainingService;
			int temp = ready.front().procID - 1;
			arr[temp].endTime = globalTime;
			ready.pop();

			globalTime += cxtSwitch;
		}
		else {

			// If the remainingService time and totalServiceTime are equal it sets the current processes startTime to the globalTime
			if (ready.front().remainingService == ready.front().serviceTime) {
				int temp = ready.front().procID - 1;
				arr[temp].startTime = globalTime;
			}

			// Subtracts quantum from remainingService, and adds the quantum and contextSwitch to globalTime
			ready.front().remainingService -= quantum;

			globalTime += (quantum + cxtSwitch);

			// Sets up following process in the queue
			ready.push(ready.front());
			ready.pop();
		}
	}

	// Sets remaining values based on known information, and inserts into vector
	for (int i = 0; i < arr.size(); i++) {
		arr[i].CalculateIntitialWait();
		arr[i].CalculateTotalWait();
		arr[i].CalculateTurnAroundTime();
	}

	return arr;
}

//Main method
int main() {
	// Identifies processes parameters for constructor
	vector<GanttChart> arr = {
		GanttChart(1, 75, 0),
		GanttChart(2, 40, 10),
		GanttChart(3, 25, 10),
		GanttChart(4, 20, 80),
		GanttChart(5, 45, 85)
	};

	// Calls the BuildTable method returning a vector from the Scheduler method containing the correct scheduled processes
	// 
	// No context switch
	cout << "Context Switch 0" << "\n";
	BuildTable(Scheduler(arr, 0));
	cout << "\n" << "\n";
	// context switch 10
	cout << "Context Switch 10" << "\n";
	BuildTable(Scheduler(arr, 10));

	return 0;
}
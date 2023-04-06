// Gantt Chart Demonstration Project
// Completed 3/20/23
// Sean Robert Mitchell

#include <iostream>
#include <iomanip>
#include <queue>
#include <deque>
using namespace std;

int quantum = 1;


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

	// Constructor for class objects
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
	}
};

// Console display of table
void BuildTable(std::vector<GanttChart> arr) {
	std::cout
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

	// first 5 values
	for (int i = 0; i < 10; i++) {
		std::cout
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

	cout << "\n\n";

	// last 5 values
	for (int i = arr.size() - 10; i < arr.size(); i++) {
		std::cout
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
			<< setw(15)
			<< arr[i].startTime
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

	int tempArr = 0;
	int tempServ = 0;
	int tempTurnA = 0;
	int tempTotalW = 0;

	

	for (int i = 0; i < arr.size(); i++) {
		tempArr += arr[i].arrivalTime;
		tempServ += arr[i].serviceTime;
		tempTurnA += arr[i].turnAround;
		tempTotalW += arr[i].totalWait;
	}

	tempArr = tempArr / arr.size();
	tempServ = tempServ / arr.size();
	tempTurnA = tempTurnA / arr.size();
	tempTotalW = tempTotalW / arr.size();

	cout << "Average Interarrival Time: " << tempArr << "\n";
	cout << "Average Service Time: " << tempServ << "\n";
	cout << "Average Turnaround Time: " << tempTurnA << "\n";
	cout << "Average Total Wait Time: " << tempTotalW << "\n";
}

// Scheduler simulator. Returns a vector of the class type.
std::vector<GanttChart> Scheduler(std::vector<GanttChart> arr, int cxtSwitch) {

	// Total processing time
	int globalTime = 0;

	// Ready queue intitialization
	std::deque<GanttChart> ready;

	ready.push_back(arr[0]);
	arr[0].startTime = 0;
	int x = 1;

	// Source Scheduler
	while (x < arr.size() || !ready.empty()) {

		// Determines whether an arrived entity may begin process
		if (x < arr.size() && arr[x].arrivalTime <= globalTime && (ready.empty() || ready.front().procID < ready.back().procID)) {
			ready.push_front(arr[x]);
			x++;
		}

		if (!ready.empty()) {

			// Processes how much of service is remaining, and set globalTime to the remaining service time.
			if (ready.front().remainingService <= quantum) {
				globalTime += ready.front().remainingService;
				int temp = ready.front().procID - 1;
				arr[temp].endTime = globalTime;
				ready.pop_front();

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

				ready.push_back(ready.front());
				ready.pop_front();
			}

		}
		else {
			globalTime += 1;
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

std::vector<int> genArrival() {

	// array of arrival times
	std::vector<int> interArrival{};

	interArrival.push_back(0);

	// initialize rand
	srand(time(0));

	std::cout << "Random Arrival Times: \n\n";

	// fills the array with random interarrival times between 4-9
	for (int i = 1; i < 100; i++) {
		int r = (rand() % 6) + 4;
		interArrival.push_back(interArrival[i - 1] + r);
		cout << interArrival[i - 1] + r << "\t";
	}

	std::cout << "\n\n";

	return interArrival;
}

std::vector<int> genService() {

	// array of service times
	std::vector<int> service{};

	// initialize rand
	srand(time(0));

	std::cout << "Random Service Times: \n\n";

	// fills the array with random values 2-6
	for (int i = 0; i < 101; i++) {
		int r = (rand() % 3) + 2;
		service.push_back(r);
		cout << r << "\t"; 
	}

	std::cout << "\n\n";

	return service;
}

//Main method
int main() {

	std::vector<int> arrival = genArrival();

	std::vector<int> service = genService();

	
	vector<GanttChart> arr;

	// Identifies processes parameters for constructor with random arrival and service times for each
	for (int i = 0; i < arrival.size(); i++) {
		arr.push_back(GanttChart(1+i, service[i], arrival[i]));
	}

	// Calls the BuildTable method returning a vector from the Scheduler method containing the correct scheduled processes
	// 
	// No context switch
	std::cout << "Context Switch 0" << "\n";
	BuildTable(Scheduler(arr, 0));
	std::cout << "\n" << "\n";
	// context switch 10
	std::cout << "Context Switch 10" << "\n";
	BuildTable(Scheduler(arr, 10));

	return 0;
}
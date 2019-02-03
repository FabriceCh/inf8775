#include <iostream>
#include <fstream>
#include <utility>
#include <string>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <ctime>
#include <chrono>
#include <map>

// quicksort implementation taken from  https://stackoverflow.com/questions/31720408/how-to-make-quick-sort-recursive
// bubblesort implementation taken from https://www.geeksforgeeks.org/bubble-sort/
using namespace std;

const unsigned int DEFAULT_SEUIL = 10;

const string COUNTING			= "counting";
const string QUICK				= "quick";
const string QUICK_SEUIL		= "quickSeuil";
const string QUICK_RANDOM_SEUIL = "quickRandomSeuil";

void printVector(const vector<int> vector) {
	for (unsigned int i = 0; i < vector.size(); i++) {
		cout << vector[i] << " ";
	}
	cout << endl;
}

void testSort(const vector<int> vec) {
	for (unsigned int i = 1; i < vec.size() - 1; i++) {
		if (vec[i - 1] > vec[i]) {
			cout << "ERROR: The vector is not sorted correctly at index " << i << endl;
		}
	}
}

int randomNumberBetween(int min, int max) {
	int randomNumber = rand() % (max - min + 1) + min;
	return randomNumber;
}

void partialBubbleSort(vector<int> & vec, int start, int end) {
	for (int i = start; i < end; i++) {
		// Last i elements are already in place    
		// here because we don't start at the begining of the array we have to add the starting index in order to make the loop go all the way through
		// original implementation had n-1 but here "end" will surely be inside of the vector's size range
		for (int j = start; j < end + start - i; j++) { 
			if (vec[j] > vec[j + 1]) {
				//swap(&arr[j], &arr[j + 1]);
				iter_swap(vec.begin() + j, vec.begin() + j + 1);
			}
		}
	}
}

int partition(vector<int> & vec, int low, int high) {
	int pivot = vec[high];//taking the last element as pivot
	int i = (low - 1);
	for (int j = low; j <= high - 1; j++) {
		// If current element is smaller than or
		// equal to pivot
		if (vec[j] <= pivot) {
			i++;
			//swap(arr[i], arr[j]);
			iter_swap(vec.begin() + i, vec.begin() + j);
		}
	}
	//swap(arr[i + 1], arr[high]);
	iter_swap(vec.begin() + (i + 1), vec.begin() + high);
	return (i + 1);
}

int partitionRdmPivot(vector<int> & vec, int low, int high) {
	int pivotIndex = randomNumberBetween(low, high);
	// in order to use the random pivot, we simply swap it with the last element of partition before using the regular partition function taking last element as pivot
	iter_swap(vec.begin() + pivotIndex, vec.begin() + high);
	return partition(vec, low, high);
}

void quickSort(vector<int> & vec, int low, int high) {
	if (low < high) {
		int pi = partition(vec, low, high);
		quickSort(vec, low, pi - 1);
		quickSort(vec, pi + 1, high);
	}
}

void quickSortSeuil(vector<int> & vec, int low, int high, const int seuil) {
	if (high - low > seuil) {
		int pi = partition(vec, low, high);
		quickSortSeuil(vec, low, pi - 1, seuil);
		quickSortSeuil(vec, pi + 1, high, seuil);
	} 
	else {
		partialBubbleSort(vec, low, high);
	}
}

void quickSortRdmPivot(vector<int> & vec, int low, int high, const int seuil) {
	if (high - low > seuil) {
		int pi = partitionRdmPivot(vec, low, high);
		quickSortRdmPivot(vec, low, pi - 1, seuil);
		quickSortRdmPivot(vec, pi + 1, high, seuil);
	}
	else {
		partialBubbleSort(vec, low, high);
	}
}

void triParDenombrement(vector<int> & vec) {
	const int max_value = *max_element(vec.begin(), vec.end());
	vector<int> valueCounters;
	vector<int> sortedVector;
	// initialize a vector the size of the largest element 
	for (int i = 0; i < max_value; i++) {
		valueCounters.push_back(0);
	}
	// fill the new vector with the amount of each number from the initial set
	for (unsigned int i = 0; i < vec.size(); i++) {
		valueCounters[vec[i] - 1] ++;
	}
	// reconstruct the initial vector with the counted values 
	for (unsigned int i = 0; i < valueCounters.size(); i++) {
		for (int j = 0; j < valueCounters[i]; j++) {
			sortedVector.push_back(i + 1);
		}
	}
	vec = sortedVector;
}

// returns elapsed time for the selected sorting
double sortNumbers(vector<int> & vec, const string desired_sorting, const int seuil) {

	chrono::steady_clock::time_point start;
	chrono::steady_clock::time_point finish;

	int low = 0;
	int high = vec.size() - 1;

	if (desired_sorting == COUNTING) {
		std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
		triParDenombrement(vec);
		std::chrono::high_resolution_clock::time_point finish = std::chrono::high_resolution_clock::now();
	} 
	else if (desired_sorting == QUICK) {
		std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
		quickSort(vec, low, high);
		std::chrono::high_resolution_clock::time_point finish = std::chrono::high_resolution_clock::now();
	} 
	else if (desired_sorting == QUICK_SEUIL) {
		std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
		quickSortSeuil(vec, low, high, seuil);
		std::chrono::high_resolution_clock::time_point finish = std::chrono::high_resolution_clock::now();
	} 
	else if (desired_sorting == QUICK_RANDOM_SEUIL) {
		// set the seed for random number generation here, before sorting and time measurement starts
		srand(time(NULL));
		std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
		quickSortRdmPivot(vec, low, high, seuil);
		std::chrono::high_resolution_clock::time_point finish = std::chrono::high_resolution_clock::now();
	}

	
	std::chrono::duration<double> elapsed = finish - start;
	return elapsed.count() * 1000;
}

double sortNumbers(vector<int> vec, string desired_sorting) {
	if (desired_sorting == "quickSeuil" || desired_sorting == "quickRandomSeuil") {
		cout << endl << "Please use the other SortNumbers function call and specify the \"seuil\"";
	}
	return sortNumbers(vec, desired_sorting, DEFAULT_SEUIL);
}

vector<int> readNumbers(const string filename) {
	vector<int> vector;
	ifstream fichier;
	unsigned int nombre;
	fichier.open(filename);
	if (!fichier) {
		cerr << "Error opening file";
	}
	unsigned int compteur = 0;
	while (fichier >> nombre && !fichier.eof()) {
		vector.push_back(nombre);
		compteur++;
	}
	fichier.close();
	return vector;
}

vector<int> compareAll(vector<int> init_vec) {
	double timeInMs;
	vector<int> vec;
	vector<int> times;

	// counting
	vec = init_vec;
	timeInMs = sortNumbers(vec, COUNTING);
	times.push_back(timeInMs);
	cout << "Elapsed time (counting): " << timeInMs << "ms" << endl;
	// Quicksort
	vec = init_vec;
	timeInMs = sortNumbers(vec, QUICK);
    times.push_back(timeInMs);
    cout << "Elapsed time (quick): " << timeInMs << "ms" << endl;
	//QuicksortSeuil
	vec = init_vec;
	timeInMs = sortNumbers(vec, QUICK_SEUIL, DEFAULT_SEUIL);
    times.push_back(timeInMs);
    cout << "Elapsed time (quickSeuil): " << timeInMs << "ms" << endl;
	// Quicksort Random Seuil
	double mean = 0;
	for (int i = 0; i < 10; i++) {
		vec = init_vec;
		timeInMs = sortNumbers(vec, QUICK_RANDOM_SEUIL, DEFAULT_SEUIL);
		cout << "Elapsed time (quickRdmSeuil): " << timeInMs << "ms" << endl;
		mean += timeInMs;
	}
	mean /= 10;
    times.push_back(timeInMs);
    cout << "Mean elapsed time (quickRdmSeuil): " << mean << "ms" << endl;
    return times;
}
void createCSVFile(string filename, vector<int> times){
    std::ofstream comparaisonCSV;
    std::ofstream seuilsCSV;
    size_t txtString = filename.find(".txt");
    vector<char> numOfNumbersString;
    int setNumber;
    for(int i = 22; i < txtString; i++){
        if (filename[i] != '_'){
            numOfNumbersString.push_back(filename[i]);
        }
        setNumber = filename[i + 1];
    }
    for (int i =0;i<numOfNumbersString.size();i++){
        cout << numOfNumbersString[i];
    }
    vector<char> fileNameVec;
    for (int i = 14; i < filename.size(); i++ ){
        fileNameVec.push_back(filename[i]);
    }
    string numbers(numOfNumbersString.begin(),numOfNumbersString.end());
    string fileName(fileNameVec.begin(), fileNameVec.end());
    comparaisonCSV.open("comparaison.csv");
    comparaisonCSV << "filename, numbers, timeCounting, timeQuick, timeQuickSeuil, timeQuickRdmSeuil\n";
    comparaisonCSV << fileName << "," << numbers << "," << times[0] << "," << times[1] << "," <<times[2] << "," << times[3] << "\n";
}

int main() {
	vector<int> vec, init_vec;
	ifstream fichier;
	unsigned int nombre;
	const string filename = "./exemplaires/testset_1000_0.txt";

	init_vec = readNumbers(filename);

// for tests:
	//initial_vector = { 3, 2, 5, 1, 8, 4, 9, 1, 6, 21, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 20, 21, 22, 25, 4, 3, 11, 1 };
	//initial_vector = { 11, 11, 11, 11, 8, 4, 1, 6, 7, 2, 1, 9, 11, 11, 11, 11, 11, 11, 11, 11 };
	//partialBubbleSort(vec, 10, 100);
	//printVector(vec);
	//testSort(vec);


	vector<int> times = compareAll(init_vec);
	createCSVFile(filename, times);

	// weird bug in VS where the terminal shuts down immediately, this allows me to see things for more than 0.02 s
	// (TO REMOVE)
	cin >> nombre;
	return 0;
}

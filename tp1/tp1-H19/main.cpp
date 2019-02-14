#include <iostream>
#include <fstream>
#include <utility>
#include <string>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <ctime>
#include <chrono>
// quicksort implementation taken from  https://stackoverflow.com/questions/31720408/how-to-make-quick-sort-recursive
// bubblesort implementation taken from https://www.geeksforgeeks.org/bubble-sort/
using namespace std;

typedef unsigned long long int hugeInt;

const unsigned int DEFAULT_SEUIL = 10;
const string COUNTING			= "counting";
const string QUICK				= "quick";
const string QUICK_SEUIL		= "quickSeuil";
const string QUICK_RANDOM_SEUIL = "quickRandomSeuil";

string findSelectedFilename(string basic_string);

vector<int> createNumOfNumbersVector();

void printVector(const vector<hugeInt> vector) {
	for (unsigned int i = 0; i < vector.size(); i++) {
		cout << vector[i] << " ";
	}
	cout << endl;
}

void testSort(const vector<hugeInt> vec) {
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

void partialBubbleSort(vector<hugeInt> & vec, int start, int end) {
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

int partition(vector<hugeInt> & vec, int low, int high) {
	hugeInt pivot = vec[high];//taking the last element as pivot
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

int partitionRdmPivot(vector<hugeInt> & vec, int low, int high) {
	int pivotIndex = randomNumberBetween(low, high);
	// in order to use the random pivot, we simply swap it with the last element of partition before using the regular partition function taking last element as pivot
	iter_swap(vec.begin() + pivotIndex, vec.begin() + high);
	return partition(vec, low, high);
}

void quickSort(vector<hugeInt> & vec, int low, int high) {
	if (low < high) {
		int pi = partition(vec, low, high);
		quickSort(vec, low, pi - 1);
		quickSort(vec, pi + 1, high);
	}
}

void quickSortSeuil(vector<hugeInt> & vec, int low, int high, const int seuil) {
	if (high - low > seuil) {
		int pi = partition(vec, low, high);
		quickSortSeuil(vec, low, pi - 1, seuil);
		quickSortSeuil(vec, pi + 1, high, seuil);
	} 
	else {
		partialBubbleSort(vec, low, high);
	}
}

void quickSortRdmPivot(vector<hugeInt> & vec, int low, int high, const int seuil) {
	if (high - low > seuil) {
		int pi = partitionRdmPivot(vec, low, high);
		quickSortRdmPivot(vec, low, pi - 1, seuil);
		quickSortRdmPivot(vec, pi + 1, high, seuil);
	}
	else {
		partialBubbleSort(vec, low, high);
	}
}

void triParDenombrement(vector<hugeInt> & vec) {
	const int max_value = *max_element(vec.begin(), vec.end());
	vector<int> valueCounters;
	vector<hugeInt> sortedVector;
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
double sortNumbers(vector<hugeInt> & vec, const string desired_sorting, const int seuil) {

	chrono::high_resolution_clock::time_point start;
	chrono::high_resolution_clock::time_point finish;
	int low = 0;
	int high = vec.size() - 1;
	if (desired_sorting == COUNTING) {
		start = std::chrono::high_resolution_clock::now();
		triParDenombrement(vec);
		finish = std::chrono::high_resolution_clock::now();
	} 
	else if (desired_sorting == QUICK) {
		start = std::chrono::high_resolution_clock::now();
		quickSort(vec, low, high);
		finish = std::chrono::high_resolution_clock::now();
	} 
	else if (desired_sorting == QUICK_SEUIL) {
		start = std::chrono::high_resolution_clock::now();
		quickSortSeuil(vec, low, high, seuil);
		finish = std::chrono::high_resolution_clock::now();
	} 
	else if (desired_sorting == QUICK_RANDOM_SEUIL) {
		// set the seed for random number generation here, before sorting and time measurement starts
		srand(time(NULL));
		start = std::chrono::high_resolution_clock::now();
		quickSortRdmPivot(vec, low, high, seuil);
		finish = std::chrono::high_resolution_clock::now();
	}

	
	std::chrono::duration<double> elapsed = finish - start;
	double time = elapsed.count() * 1000;
	return time; 
}

double sortNumbers(vector<hugeInt> vec, string desired_sorting) {
	if (desired_sorting == "quickSeuil" || desired_sorting == "quickRandomSeuil") {
		//cout << endl << "Please use the other SortNumbers function call and specify the \"seuil\"";
	}
	return sortNumbers(vec, desired_sorting, DEFAULT_SEUIL);
}

vector<hugeInt> readNumbers(const string filename) {
	vector<hugeInt> vector;
	ifstream fichier;
	hugeInt nombre;
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

vector<double> compareAll(vector<hugeInt> init_vec) {
	double timeInMs;
	vector<hugeInt> vec;
	vector<double> times;
	// counting
	vec = init_vec;
	//timeInMs = sortNumbers(vec, COUNTING);
	timeInMs = 10;
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
    //cout << "Elapsed time (quickSeuil): " << timeInMs << "ms" << endl;
	// Quicksort Random Seuil
	double mean = 0;
	for (int i = 0; i < 10; i++) {
		vec = init_vec;
		timeInMs = sortNumbers(vec, QUICK_RANDOM_SEUIL, DEFAULT_SEUIL);
		//cout << "Elapsed time (quickRdmSeuil): " << timeInMs << "ms" << endl;
		mean += timeInMs;
	}
	mean /= 10;
    times.push_back(mean);
    //cout << "Mean elapsed time (quickRdmSeuil): " << mean << "ms" << endl;
    return times;
}

struct SeuilTime {
	int seuil;
	double timeQuick;
	double timeQuickRdm;
};

void appendToSeuilCSV(string filename,
							 int taille,
							 SeuilTime seuilTime) {
	std::ofstream comparaisonCSV;
	comparaisonCSV.open("seuils.csv", ios_base::app);
	comparaisonCSV << filename << "," 
	<< taille  << "," 
	<< seuilTime.seuil << "," 
	<< seuilTime.timeQuick << "," 
	<< seuilTime.timeQuickRdm << "\n";
}

void writeToSeuilCSV(){
    std::ofstream comparaisonCSV;
    comparaisonCSV.open("seuils.csv");
    comparaisonCSV << "filename,numbers,seuil,timeQuickSeuil,timeQuickRdmSeuil\n";
}



vector<SeuilTime> compareSeuils(vector<hugeInt> init_vec) {
	double timeInMs;
	vector<hugeInt> vec;
	vector<SeuilTime> times;
	vector<int> seuils = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 32, 64, 128, 256};
	for (unsigned int i = 0; i < seuils.size(); i++) {
		SeuilTime exp;
		exp.seuil = seuils[i];
		//QuicksortSeuil
		vec = init_vec;
		timeInMs = sortNumbers(vec, QUICK_SEUIL, seuils[i]);
		exp.timeQuick = timeInMs;
		//cout << "Elapsed time (quickSeuil): " << timeInMs << "ms" << endl;
		// Quicksort Random Seuil
		double mean = 0;
		for (int i = 0; i < 10; i++) {
			vec = init_vec;
			timeInMs = sortNumbers(vec, QUICK_RANDOM_SEUIL, seuils[i]);
			//cout << "Elapsed time (quickRdmSeuil): " << timeInMs << "ms" << endl;
			mean += timeInMs;
		}
		mean /= 10;
		exp.timeQuickRdm = mean;
		times.push_back(exp);
		//cout << "Mean elapsed time (quickRdmSeuil): " << mean << "ms" << endl;
		
	}
	return times;
	
}



string createCSVStringData(string fileName, string numbers, vector<int> times, string data){
    data += fileName;
    data += ",";
    data += numbers;
    data += ",";
    data += to_string(times[0]);
    data += ",";
    data += to_string(times[1]);
    data += ",";
    data += to_string(times[2]);
    data += ",";
    data +=  to_string(times[3]);
    data += "\n";
    return data;
}

string findNumOfNumbers(string filename){
    size_t txtString = filename.find(".txt");
    vector<char> numOfNumbersString;
    int setNumber;
    for(int i = 22; i < txtString - 1; i++){
        if (filename[i] != '_'){
            numOfNumbersString.push_back(filename[i]);
        }
        setNumber = filename[i + 1];
    }
    string numbers(numOfNumbersString.begin(),numOfNumbersString.end());
    return numbers;
}

string findSelectedFilename(string filename) {
    vector<char> fileNameVec;
    for (int i = 14; i < filename.size(); i++ ){
        fileNameVec.push_back(filename[i]);
    }
    string fileName(fileNameVec.begin(), fileNameVec.end());
    return fileName;
}

void writeToComparisonCSV(){
    std::ofstream comparaisonCSV;
    comparaisonCSV.open("comparaison.csv");
    comparaisonCSV << "filename,numbers,timeCounting,timeQuick,timeQuickSeuil,timeQuickRdmSeuil\n";
}

void appendToComparisonCSV(string filename,
							 int taille,
							 double timeConuting,
							 double timeQuick,
							 double timeQuickSeuil,
							 double timeQuickRdmSeuil) {
	std::ofstream comparaisonCSV;
	comparaisonCSV.open("comparaison.csv", ios_base::app);
	comparaisonCSV << filename << "," 
	<< taille  << "," 
	<< timeConuting << "," 
	<< timeQuick << "," 
	<< timeQuickSeuil << "," 
	<< timeQuickRdmSeuil << "\n";
}

void createCSVFile(string filename, vector<int> times){
    string numbers = findNumOfNumbers(filename);
    string selectedFilename = findSelectedFilename(filename);
    string data = createCSVStringData(selectedFilename, numbers, times, data);
    //    filename, # of numbers, timeS=0, timeS=5, timeS=10, timeS=20,
}

vector<int> createNumOfNumbersVector() {
    vector<int> numOfNumbers;
    numOfNumbers.push_back(1000);
    numOfNumbers.push_back(5000);
    numOfNumbers.push_back(10000);
    numOfNumbers.push_back(50000);
    numOfNumbers.push_back(100000);
	numOfNumbers.push_back(500000);
    return numOfNumbers;
}

string createFilename(vector<int> numOfNumbers, int i, int j){
    string filename = "./exemplaires/testset_";
    filename.append(to_string(numOfNumbers[i]));
    filename.append("_");
    filename.append(to_string(j));
    filename.append(".txt");
    return filename;
}

void seuilExperiment() {
	vector<SeuilTime> seuilTimes;
<<<<<<< HEAD

	vector<hugeInt> vec;
=======
	vector<int> vec;
>>>>>>> ea9e26e237499755510960dc517a05c54f0ff149
	vector<int> numOfNumbers = createNumOfNumbersVector();
    string data = "";
	string filename;
	string numbers;
	writeToSeuilCSV();	
	for(int i = 0; i < 6; i++){
        for(int j = 0; j < 30; j++){
            filename = createFilename(numOfNumbers, i, j);
			vec = readNumbers(filename);
            cout << filename << endl;
			seuilTimes = compareSeuils(vec);
			cout << endl;
			for(unsigned int k = 0; k < seuilTimes.size(); k++) {
				appendToSeuilCSV(filename, vec.size(), seuilTimes[k]);
			}
        }
	}
}

void gatherResults() {
	vector<hugeInt> vec;
	vector<int> numOfNumbers = createNumOfNumbersVector();
    string data = "";
	vector<double> times;
	string filename;
	string numbers;
	writeToComparisonCSV();	
	for(int i = 0; i < 6; i++){
        for(int j = 0; j < 30; j++){
            filename = createFilename(numOfNumbers, i, j);
			vec = readNumbers(filename);
            cout << filename << endl;
			times = compareAll(vec);
			cout << endl;
			appendToComparisonCSV(filename, vec.size(), times[0], times [1], times [2], times[3]);
        }
	}
}

void useInterface(const char * argv[]) {
	// argv[1]: filename argv[2]: algoName argv[3-4]: show sorted / show time
	string fileName, sortAlgo, addArg1, addArg2;
	bool showSortedArray = false, showTime = false;
	if(argv[1] != nullptr) fileName = argv[1];
	if(argv[2] != nullptr) sortAlgo = argv[2];
	if(argv[3] != nullptr) addArg1 = argv[3];
	if(argv[4] != nullptr) addArg2 = argv[4];
	if(addArg1 == "-p" || addArg2 == "-p") {
		showSortedArray = true;
	}
	if(addArg1 == "-e" || addArg2 == "-e") {
		showTime = true;
	}

	vector<hugeInt> numbers = readNumbers(fileName);
	double elapsedTime = sortNumbers(numbers, sortAlgo);

	if(showSortedArray) {
		printVector(numbers);
	} 
	if(showTime) {
		cout << endl << elapsedTime << endl;
	}
}

int main(int argc, const char * argv[]) {
	// uncomment to run all quicksorts with seuils and write to seuil.csv
	//seuilExperiment();
	// uncomment to run all sorts for comparison (using DEFAULT_SEUIL)
	//gatherResults();
	//uncomment to use interface for TP
	useInterface(argv);

	return 0;
}


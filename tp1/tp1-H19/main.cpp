
#include <iostream>
#include <fstream>
#include <utility>
#include <string>
#include <cstdio>
#include <algorithm>
#include <vector>

// quicksort implementation taken from https://stackoverflow.com/questions/31720408/how-to-make-quick-sort-recursive
using namespace std;

void printVector(vector<int> vector) {
	for (unsigned int i = 0; i < vector.size(); i++) {
		cout << vector[i] << " ";
	}
}

int partition(vector<int> & vector, int low, int high) {
	int pivot = vector[high];    //taking the last element as pivot
	int i = (low - 1);
	for (int j = low; j <= high - 1; j++) {
		// If current element is smaller than or
		// equal to pivot
		if (vector[j] <= pivot) {
			i++;
			//swap(vector[i], vector[j]);
			iter_swap(vector.begin() + i, vector.begin() + j);
		}
	}
	//swap(arr[i + 1], arr[high]);
	iter_swap(vector.begin() + (i + 1), vector.begin() + high);
	return (i + 1);
}

void quickSort(vector<int> & vector, int low, int high) {
	if (low < high) {
		int pi = partition(vector, low, high);
		quickSort(vector, low, pi - 1);
		quickSort(vector, pi + 1, high);
	}
}

void triParDenombrement(vector<int> & vector) {
	const int max_value = *max_element(vector.begin(), vector.end());
	std::vector<int> valueCounters;
	std::vector<int> sortedVector;
	//initialize a vector the size of the largest element 
	for (int i = 0; i < max_value; i++) {
		valueCounters.push_back(0);
	}
	//fill the new vector with the amount of each number from the initial set
	for (unsigned int i = 0; i < vector.size(); i++) {
		valueCounters[vector[i] - 1] ++;
	}
	//reconstruct the initial vector with the counted values 
	for (unsigned int i = 0; i < valueCounters.size(); i++) {
		for (int j = 0; j < valueCounters[i]; j++) {
			sortedVector.push_back(i + 1);
		}
	}
	vector = sortedVector;
}

vector<int> sortNumbers(vector<int> vector, string desired_sorting) {
	int pointeur_gauche = 0;
	int pointeur_droit = vector.size() - 1;
	if (desired_sorting == "counting") {
		triParDenombrement(vector);
	} else if (desired_sorting == "quick") {
		quickSort(vector, pointeur_gauche, pointeur_droit);
	} else if (desired_sorting == "quickSeuil") {
		quickSort(vector, pointeur_gauche, pointeur_droit);
	} else if (desired_sorting == "quickRandomSeuil") {
		quickSort(vector, pointeur_gauche, pointeur_droit);
	}
	return vector;
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

int main() {
	vector<int> initial_vector;
	ifstream fichier;
	unsigned int nombre;
	const string filename = "./exemplaires/testset_1000_0.txt";
	initial_vector = readNumbers(filename);

// for tests:
	//initial_vector = { 3, 2, 5, 1, 8, 4, 9, 1, 6, 21 };

	vector<int> sorted_vector = sortNumbers(initial_vector, "counting");

	printVector(sorted_vector);

	cin >> nombre;
	return 0;
}
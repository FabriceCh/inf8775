
#include <iostream>
#include <fstream>
#include <utility>
#include <string>
#include <cstdio>
#include <algorithm>
#include <vector>

// quicksort implementation taken from https://stackoverflow.com/questions/31720408/how-to-make-quick-sort-recursive
using namespace std;

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

void triParDenombrement(int tableau_lu[]) {
	int nouveau_tableau[1000];
	for (int i = 0; i < 1000; i++) {
		nouveau_tableau[i] += 1;
	}
	for (int i = 0; i < 1000; i++) {
		for (int j = 0; j < nouveau_tableau[i]; j++) {
			cout << i << ", ";
		}
	}
}

vector<int> readNumbers(const char* filename) {
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
	vector<int> vector;
	ifstream fichier;
	unsigned int nombre;
	const char* filename = "./exemplaires/testset_1000_0.txt";
	vector = readNumbers(filename);

// for tests:
	//vector = { 3, 2, 5, 1, 8, 4, 9, 1, 6, 21 };

	int pointeur_gauche = 0;
	int pointeur_droit = vector.size() - 1;

	quickSort(vector, pointeur_gauche, pointeur_droit);

	for (unsigned int i = 0; i < vector.size(); i++) {
			cout << vector[i] << ", ";
	}

	cin >> nombre;
	return 0;
}
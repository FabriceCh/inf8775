#include <iostream>
#include <fstream>
#include <utility>
#include <string>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <list>
#include <ctime>
#include <chrono>
#include <random>
#include <limits>

using namespace std;

typedef std::numeric_limits< double > dbl;

struct Resto {
	int id; // numero du resto
	int r; // revenu du resto
	int q; // coût du resto
};

struct Problem {
	int N; // nombre de resto
	int capacity;
	vector<Resto> restos;
};

struct Solution {
	vector<int> restosIDs;
	double elapsedTime;
};

void useInterface(const char * argv[]);
void showProblemData(Problem problem);
Problem readProblem(string filename);
void useInterface(const char * argv[]);
Solution resolveGlouton(Problem problem);
void showSolution(Solution sol, bool showR, Problem problem);


void showProblemData(Problem problem) {
	cout << "N: " << problem.N << endl;
	cout << "capacity: " << problem.capacity << endl;
	for (int j = 0; j < problem.restos.size(); j++) {
		Resto i = problem.restos[j];
		cout << "resto i: " << i.id << ", r: " << i.r << ", q: " << i.q << endl;
	}
}

Problem readProblem(string filename) {
	Problem problem;

	ifstream fichier;
	fichier.open(filename);
	if (!fichier) {
		cerr << "Error opening file";
	}
	unsigned int compteur = 0;
	fichier >> problem.N;
	while (compteur < problem.N) {
		Resto resto;
		fichier >> resto.id;
		fichier >> resto.r;
		fichier >> resto.q;
		problem.restos.push_back(resto);
		compteur++;
	}
	fichier >> problem.capacity;
	fichier.close();

	return problem;
}


/* 
	GLOUTON PROBABILISTE
*/

struct Ratio {
	int id;
	double ratio;
};

struct Partition {
	int id;
	double min;
	double max;
};

Solution resolveGlouton(Problem problem) {
	Solution solution;
	chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

	vector<Resto> restos = problem.restos;
	// compute the ratios of all restaurant emplacements
	vector<Ratio> ratios;
	double sumRatio = 0;
	for (unsigned int i = 0; i < restos.size(); i++) {
		Ratio Ri;
		Ri.ratio = restos[i].r / restos[i].q;
		Ri.id = restos[i].id;
		sumRatio += Ri.ratio;
		ratios.push_back(Ri);
	}
	// predictable behaviour (always the same numbers)
	//unsigned seed = 126534;
	//mt19937 generator(seed);

	// different numbers
	random_device rd;
	mt19937 generator(rd());
	uniform_real_distribution<double> uniform(0.0, 1.0);

	while (solution.restosIDs.size() != problem.capacity) {
	//while (solution.restosIDs.size() != 5) {
		// create partitions for all remaining restaurants
		vector<Partition> partitions;
		double previousMax = 0;
		for (unsigned int i = 0; i < ratios.size(); i++) {
			Partition Pi;
			Pi.id = ratios[i].id;
			Pi.min = previousMax;
			Pi.max = Pi.min + ratios[i].ratio / sumRatio;
			partitions.push_back(Pi);

			previousMax = Pi.max;
		}

		// randomly select the emplacement with the probabilistic partitions
		double choice = uniform(generator);
		cout << choice << endl;

		for (int i = 0; i < partitions.size(); i++) {
			// when the chosen partition is found
			if (choice <= partitions[i].max && choice >= partitions[i].min) {
				// add the id to the solution
				solution.restosIDs.push_back(partitions[i].id);
				// remove the restaurant from the possibilities
				ratios.erase(ratios.begin() + i);
				break;
			}
		}

	}

	chrono::high_resolution_clock::time_point finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish - start;
	solution.elapsedTime = elapsed.count() * 1000;

	return solution;
}

/*
	PROGRAMMATION DYNAMIQUE
*/

// TODO

/*
	HEURISTIQUE D'AMÉLIORATION LOCALE
*/

// TODO

void useInterface(const char * argv[]) {
	// argv[1]: filename argv[2]: algoName argv[3-4]: show sorted / show time
	string fileName, algo, addArg1, addArg2;
	bool showSolution = false, showTime = false;
	if (argv[1] != nullptr) fileName = argv[1];
	if (argv[2] != nullptr) algo = argv[2];
	if (argv[3] != nullptr) addArg1 = argv[3];
	if (argv[4] != nullptr) addArg2 = argv[4];
	if (addArg1 == "-p" || addArg2 == "-p") {
		showSolution = true;
	}
	if (addArg1 == "-t" || addArg2 == "-t") {
		showTime = true;
	}

	// TODO: lire l'exemplaire et le résoudre avec le bon algo et en mesurant le temps




	if (showSolution) {
		// TODO: show the solution
	}
	if (showTime) {
		// TODO: show the time
		//cout << endl << elapsedTime << endl;
	}
}

void showSolution(Solution sol, bool showR, Problem problem) {
	cout << "Solution found: " << endl;
	cout << "elapsed time: " << sol.elapsedTime << endl;
	cout << "emplacements: " << endl;
	for (int i = 0; i < sol.restosIDs.size(); i++) {
		cout << sol.restosIDs[i] << " ";
	}
	if (showR) {
		int global_q = 0;
		int global_r = 0;
		for (int i = 0; i < sol.restosIDs.size(); i++) {

			for (int j = 0; j < problem.restos.size(); j++) {
				if (problem.restos[j].id == sol.restosIDs[i]) {
					global_q += problem.restos[j].q;
					global_r += problem.restos[j].r;
					break;
				}
			}

			
		}
		double global_R;
		try {
			 global_R = global_r / (double)global_q;
		}
		catch (exception e) {
			cout << "division by 0 lol" << endl;
			return;
		}
		cout << endl;
		cout << "Solution r: " << global_r << endl;
		cout << "Solution q: " << global_q << endl;
		cout.precision(dbl::max_digits10);
		cout << "Solution R: " << global_R << endl;
	}
}

int main(int argc, const char * argv[]) {

	Problem problem = readProblem("C:/Users/fabrice/Desktop/0TRAVAUX/INF8775/tp2/exemplaires/WC-100-10-06.txt");
	showProblemData(problem);
	Solution solution = resolveGlouton(problem);
	showSolution(solution, true, problem);

	system("pause");
	return 0;
}
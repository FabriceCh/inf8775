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

/*
	Structures et fonctions globales
*/

typedef std::numeric_limits< double > dbl;

const double MAXIMUM_LOCAL_HEURISTIC_SEARCH_TIME_IN_MS = 120000.0;

struct Resto {
	unsigned int id;	// Numero du resto
	unsigned int r;		// Revenu du resto
	unsigned int q;		// Coût du resto
};

struct Problem {
	unsigned int N;		// Nombre de resto
	unsigned int capacity;
	vector<Resto> restos;
};

struct Solution {
	vector<Resto> restos;
	unsigned int totalChickens;
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
	for (unsigned int j = 0; j < problem.restos.size(); j++) {
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
	Resto resto;
	double ratio;
};

int getSumRatio(vector<Ratio> ratios) {
	int sumRatio = 0;
	for (unsigned int i = 0; i < ratios.size(); i++) {
		sumRatio += ratios[i].ratio;
	}
	return sumRatio;
}

Solution resolveGlouton(Problem problem) {
	Solution solution;
	chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

	vector<Resto> restos = problem.restos;
	// Compute the ratios of all restaurant emplacements
	vector<Ratio> ratios;
	double sumRatio = 0;
	for (unsigned int i = 0; i < restos.size(); i++) {
		Ratio Ri;
		Ri.ratio = (double) restos[i].r / (double) restos[i].q;
		Ri.resto = restos[i];
		sumRatio += Ri.ratio;
		ratios.push_back(Ri);
	}
	// Predictable behaviour (always the same numbers)
	//unsigned seed = 126534;
	//mt19937 generator(seed);

	// different numbers
	random_device rd;
	mt19937 generator(rd());
	uniform_real_distribution<double> uniform(0.0, 1.0);

	solution.totalChickens = 0;

	while ((ratios.size() > 0) && (solution.totalChickens < problem.capacity)) {
		// Choose a restaurant
		// Note: cette façon est plus efficace que la solution naïve qu'on avait au départ qui créait des partitions pour chaque resto
		int tryIdx;
		int sumRatio = getSumRatio(ratios);
		int chosenIndex;
		while (true) {
			tryIdx = (int)(uniform(generator) * (ratios.size()-1));
			if (uniform(generator) < ((double) ratios[tryIdx].ratio / (double) sumRatio)) {
				chosenIndex = tryIdx;
				break;
			};
		}
		
		//if the selected resto surpasses the capacity
		if ((solution.totalChickens + ratios[chosenIndex].resto.q) > problem.capacity) {
			// remove the restaurant from the possibilities
			ratios.erase(ratios.begin() + chosenIndex);
		}
		else {
			// Add the id to the solution
			solution.restos.push_back(ratios[chosenIndex].resto);
			// Add the chickens to the total
			solution.totalChickens += ratios[chosenIndex].resto.q;
			// Remove the restaurant from the possibilities
			ratios.erase(ratios.begin() + chosenIndex);
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

void printArray(const vector<vector<unsigned int>> D, Problem p) {
	cout << "Array:" << endl;
	cout << "\t";
	for (unsigned int y = 0; y < D[0].size(); y++) {
		if (y  < 10) {
			cout << y  << "  ";
		} else {
			cout << y  << " ";
		}
		
	}
	cout << endl << endl;
	for (unsigned int x = 0; x < D.size(); x++) {
		cout << x + 1 << "\t";
		for (unsigned int y = 0; y < D[x].size(); y++) {
			
			if (D[x][y] < 10) {
				cout << D[x][y] << "  ";
			}
			else {
				cout << D[x][y] << " ";
			}
		}
		cout << endl;
	}
	cout << endl;
}

Solution resolveDynProg(Problem problem) {
	Solution solution;
	chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	vector<Resto> restos = problem.restos;
	const unsigned int N = restos.size();
	
	// Initialisation du tableau
	vector<vector<unsigned int>> D(N);
	for (auto&& x : D)
		x.resize(problem.capacity + 1);

	// Remplissage du tableau
	for (unsigned int i = 0; i < N; i++) {
		for (unsigned int j = 0; j < problem.capacity + 1; j++) {
			if (j == 0) {
				D[i][j] = 0;
			}
			else if (i == 0) {
				if (restos[i].q <= j)
					D[i][j] = restos[i].r;
			} else {
				// Manage unexistant values
				if (j < restos[i].q) {
					D[i][j] =D[i - 1][j];
				}
				else {
					D[i][j] = max(restos[i].r + D[i - 1][j - restos[i].q], D[i - 1][j]);
				}
				
			}
			
		}
	}
	//printArray(D, problem);
	
	// Trouver la solution à partir du tableau
	int j = problem.capacity;
	solution.totalChickens = 0;
	
	for (int i = N - 1; i >= 0; i--) {
		if (i == 0) {
			if (j >= restos[i].q) {
				solution.restos.push_back(restos[i]);
			}
		}
		else {
			
			if (D[i][j] != D[i - 1][j]) {
				if (j - restos[i].q >= 0) {
					// Add the id to the solution
					solution.restos.push_back(restos[i]);
					// Add the chickens to the total
					solution.totalChickens += restos[i].q;
					// Update j
					j -= restos[i].q;
				}
			}
		}

	}
	chrono::high_resolution_clock::time_point finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish - start;
	solution.elapsedTime = elapsed.count() * 1000;
	
	return solution;
}


/*
	HEURISTIQUE D'AMÉLIORATION LOCALE
*/

int findRevenue(const Solution & solution) {
	int revenue = 0;
	for (unsigned int i = 0; i < solution.restos.size(); i++) {
		revenue += solution.restos[i].r;
	}
	return revenue;
}

vector<Resto> findUnsusedResto(const Problem & problem, const Solution & solution) {

	vector<int> sortedIds;
	for (unsigned int i = 0; i < solution.restos.size(); i++) {
		sortedIds.push_back(solution.restos[i].id);
	}

	// Sort the ids
	sort(sortedIds.begin(), sortedIds.end());
	// Parse ids backwards to delete the right resto in unused restos
	vector<Resto> unusedRestos = problem.restos;
	for (int i = sortedIds.size() - 1; i >=0 ; i--) {
		unusedRestos.erase(unusedRestos.begin() + sortedIds[i] - 1);
	}
	return unusedRestos;
}

Solution resolveHeu(Problem problem) {
	chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	chrono::high_resolution_clock::time_point finish;
	std::chrono::duration<double> elapsed;

	Solution solution = resolveGlouton(problem);
	int currentRevenue = findRevenue(solution);

	cout << "initial glouton solution revenue:" << currentRevenue << endl;

	bool shouldContinue = true;
	vector<Resto> unusedRestos = findUnsusedResto(problem, solution);
	while (shouldContinue) {
		shouldContinue = false;
		for (unsigned int i = 0; i < solution.restos.size(); i++) {
			for (unsigned int j = 0; j < unusedRestos.size(); j++) {
				Resto newResto = unusedRestos[j];
				Resto curResto = solution.restos[i];
				if (newResto.r > curResto.r && (solution.totalChickens - curResto.q + newResto.q) < problem.capacity) {
					swap(solution.restos[i], unusedRestos[j]);
					shouldContinue = true;
				}
			}
		}
		finish = std::chrono::high_resolution_clock::now();
		elapsed = finish - start;
		if (elapsed.count() * 1000 > MAXIMUM_LOCAL_HEURISTIC_SEARCH_TIME_IN_MS) {
			shouldContinue = false;
		}
	}


	solution.elapsedTime = elapsed.count() * 1000;
	return solution;
}


/*
	AFFICHAGE ET INTERFACE
*/

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
	cout << "Problem params: " << "capacity: " << problem.capacity << " N: " << problem.N << endl;
	cout << "Solution found: " << endl;
	cout << "elapsed time: " << sol.elapsedTime << endl;
	cout << "emplacements: " << endl;
	for (unsigned int i = 0; i < sol.restos.size(); i++) {
		cout << sol.restos[i].id << " ";
	}
	if (showR) {
		int global_q = 0;
		int global_r = 0;
		for (unsigned int i = 0; i < sol.restos.size(); i++) {
			global_q += sol.restos[i].q;
			global_r += sol.restos[i].r;
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
	string joelPathP6 = R"(C:\Users\Joel\Documents\inf8775\tp2\exemplaires\WC-100-10-06.txt)";
	string joelPathP7 = R"(C:\Users\Joel\Documents\inf8775\tp2\exemplaires\WC-100-10-07.txt)";

	string fabPathP6 = "./exemplaires/WC-100-10-06.txt";
	string fabPathP7 = "./exemplaires/WC-100-10-07.txt";
	string fabPathP6_2 = R"(C:\Users\fabrice\Desktop\0TRAVAUX\INF8775\tp2\exemplaires\WC-100-10-06.txt)";
	string fabPathP7_2 = R"(C:\Users\fabrice\Desktop\0TRAVAUX\INF8775\tp2\exemplaires\WC-100-10-07.txt)";


	Problem problem_6 = readProblem(fabPathP6_2);
	Problem problem_7 = readProblem(fabPathP7_2);
	//	showProblemData(problem);
	//Solution solutionGlouton = resolveGlouton(problem_6);
	//Solution solutionDynProg = resolveDynProg(problem_6);
	Solution solutionHeu = resolveHeu(problem_6);
	//cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << endl;
	//cout << "Glouton" << endl;
	//showSolution(solutionGlouton, true, problem_6);
	//cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << endl;
	//cout << "Programmation dynamique" << endl;
	//showSolution(solutionDynProg, true, problem_6);
	//cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << endl;
	//cout << "Heuristique" << endl;
	showSolution(solutionHeu, true, problem_6);
	system("pause");
#ifdef _WIN64
	system("pause");
#endif
	return 0;
}
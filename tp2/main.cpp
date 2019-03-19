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
	unsigned int id; // numero du resto
	unsigned int r; // revenu du resto
	unsigned int q; // coût du resto
};

struct Problem {
	unsigned int N; // nombre de resto
	unsigned int capacity;
	vector<Resto> restos;
};

struct Solution {
	vector<int> restosIDs;
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

int getSumRevenu(vector<Ratio> ratios) {
	int sumRevenu = 0;
	for (unsigned int i = 0; i < ratios.size(); i++) {
		sumRevenu += ratios[i].resto.r;
	}
	return sumRevenu;
}

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
		Ri.resto = restos[i];
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

	solution.totalChickens = 0;

	while (ratios.size() > 0 && solution.totalChickens < problem.capacity) {
		// choose a restaurant
		int tryIdx;
		int sumRevenu = getSumRevenu(ratios);
		int chosenIdx;
		while (true) {
			tryIdx = (int)(uniform(generator) * ratios.size());
			if (uniform(generator) < (ratios[tryIdx].ratio)) {
				chosenIdx = tryIdx;
				break;
			};
		}

		//if the selected resto surpasses the capacity
		if (solution.totalChickens + ratios[chosenIdx].resto.q > problem.capacity) {
			// remove the restaurant from the possibilities
			ratios.erase(ratios.begin() + chosenIdx);
		}
		else {
			// add the id to the solution
			solution.restosIDs.push_back(ratios[chosenIdx].resto.id);
			// remove the restaurant from the possibilities
			ratios.erase(ratios.begin() + chosenIdx);
			// add the chickens to the total
			solution.totalChickens += ratios[chosenIdx].resto.q;
		}

	}

	chrono::high_resolution_clock::time_point finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish - start;
	solution.elapsedTime = elapsed.count() * 1000;
	vector<vector<vector<double> > > array3D;

	return solution;
}

/*
	PROGRAMMATION DYNAMIQUE
*/

void printArray( vector<vector<unsigned int>> D, Problem p) {
	cout << "Array:" << endl;
	cout << "\t";
	for (unsigned int y = 0; y < p.capacity; y++) {
		if (y + 1 < 10) {
			cout << y + 1 << "  ";
		} else {
			cout << y + 1 << " ";
		}
		
	}
	cout << endl << endl;
	for (unsigned int x = 0; x < p.restos.size(); x++) {
		cout << x + 1 << "\t";
		for (unsigned int y = 0; y < p.capacity; y++) {
			
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
	unsigned int n = restos.size();
	
	// Initialisation du tableau
	vector<vector<unsigned int>> D(n);
	for (auto&& x : D)
		x.resize(problem.capacity + 1);

	// remplissage du tableau
	for (unsigned int i = 0; i < n; i++) {
		for (unsigned int j = 0; j < problem.capacity; j++) {
			if (i == 0) {
				if (restos[i].q <= j)
					D[i][j] = restos[i].r;
			} else {
				// replace unexistant value with 0
				int trickyIndex = j - restos[i].q;
				unsigned int addTerm = 0;
				if (trickyIndex > 0) {
					addTerm = D[i - 1][trickyIndex];
				}

				D[i][j] = max(restos[i].r + addTerm, D[i - 1][j]);
				//solution.restosIDs.push_back(restos[i].id);
			}
			
		}
	}
	printArray(D, problem);

	// trouver la solution à partir du tableau

	return solution;
}

/*
	HEURISTIQUE D'AMÉLIORATION LOCALE
*/

int findRevenue(vector<int> restosIDs, vector<Resto> restos) {
	int revenue = 0;
	for (unsigned int i = 0; i < restosIDs.size(); i++) {
		for (unsigned int j = 0; j < restos.size(); j++) {
			if (restos[j].id == restosIDs[i]) {
				revenue += restos[j].r;
			}
		}
	}
	return revenue;
}

Solution resolveHeu(Problem problem_1, Problem problem_2) {
	Solution s_0 = resolveGlouton(problem_1);
	Solution s_i;
	int objFct = findRevenue(s_0.restosIDs, problem_1.restos);
	int newObjFct;
	for (unsigned int i = 0; i < problem_1.restos.size(); i++) {
		if (problem_2.restos[i].q < problem_1.capacity) {
			problem_1.restos[i] = problem_2.restos[i];
			s_i = resolveGlouton(problem_1);
			newObjFct = findRevenue(s_i.restosIDs, problem_1.restos);
			if (newObjFct > objFct) {
				objFct = newObjFct;
			}
		}
	}
	return s_i;
}

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
	for (unsigned int i = 0; i < sol.restosIDs.size(); i++) {
		cout << sol.restosIDs[i] << " ";
	}
	if (showR) {
		int global_q = 0;
		int global_r = 0;
		for (unsigned int i = 0; i < sol.restosIDs.size(); i++) {

			for (unsigned int j = 0; j < problem.restos.size(); j++) {
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
	string joelPathP6 = R"(C:\Users\Joel\Documents\inf8775\tp2\exemplaires\WC-100-10-06.txt)";
	string joelPathP7 = R"(C:\Users\Joel\Documents\inf8775\tp2\exemplaires\WC-100-10-07.txt)";

	string fabPathP6 = "./exemplaires/WC-100-10-06.txt";
	string fabPathP7 = "./exemplaires/WC-100-10-07.txt";
	string fabPathP6_2 = R"(C:\Users\fabrice\Desktop\0TRAVAUX\INF8775\tp2\exemplaires\WC-100-10-06.txt)";
	string fabPathP7_2 = R"(C:\Users\fabrice\Desktop\0TRAVAUX\INF8775\tp2\exemplaires\WC-100-10-07.txt)";


	Problem problem_6 = readProblem(fabPathP6_2);
	Problem problem_7 = readProblem(fabPathP7_2);
	//	showProblemData(problem);
	Solution solutionGlouton = resolveGlouton(problem_6);
	Solution solutionDynProg = resolveDynProg(problem_6);
	//	Solution solutionHeu = resolveHeu(problem_6, problem_7);
	cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << endl;
	cout << "Glouton" << endl;
	//showSolution(solutionGlouton, true, problem_6);
	//    cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << endl;
	//    cout << "Programmation dynamique" << endl;
	    showSolution(solutionDynProg, true, problem_6);
	//    cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << endl;
	//    cout << "Heuristique" << endl;
	//    showSolution(solutionHeu, true, problem_6);
	system("pause");
#ifdef _WIN64
	system("pause");
#endif
	return 0;
}
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

const double MAXIMUM_LOCAL_HEURISTIC_SEARCH_TIME_IN_MS = 120000.0; // 2 minutes
const string GLOUTON = "glouton";
const string PROGRAMMATION_DYNAMIQUE = "progdyn";
const string AMELIORATION_LOCALE = "local";

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
void showDebugSolution(Solution sol, bool showR, Problem problem);
Solution solve(Problem & problem, string & algo);

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

double getSumRatio(vector<Ratio> ratios) {
	double sumRatio = 0;
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
	for (unsigned int i = 0; i < restos.size(); i++) {
		Ratio Ri;
		Ri.ratio = (double)restos[i].r / (double)restos[i].q;
		Ri.resto = restos[i];
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
		int tryIdx, chosenIndex;
		double sumRatio = getSumRatio(ratios);
		while (true) {
			tryIdx = (int)(uniform(generator) * (ratios.size() - 1));
			if (uniform(generator) < ((double)ratios[tryIdx].ratio / (double)sumRatio)) {
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

void printArray(const vector<vector<unsigned int>> D) {
	cout << "Array:" << endl;
	cout << "\t";
	for (unsigned int y = 0; y < D[0].size(); y++) {
		if (y < 10) {
			cout << y << "  ";
		}
		else {
			cout << y << " ";
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
			}
			else {
				// Manage unexistant values
				if (j < restos[i].q) {
					D[i][j] = D[i - 1][j];
				}
				else {
					D[i][j] = max(restos[i].r + D[i - 1][j - restos[i].q], D[i - 1][j]);
				}

			}

		}
	}
	//printArray(D);

	// Trouver la solution à partir du tableau
	unsigned int j = problem.capacity;
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

vector<Resto> findUnusedResto(const Problem & problem, const Solution & solution) {

	vector<int> sortedIds;
	for (unsigned int i = 0; i < solution.restos.size(); i++) {
		sortedIds.push_back(solution.restos[i].id);
	}

	// Sort the ids
	sort(sortedIds.begin(), sortedIds.end());
	// Parse ids backwards to delete the right resto in unused restos
	vector<Resto> unusedRestos = problem.restos;
	for (int i = sortedIds.size() - 1; i >= 0; i--) {
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

	bool shouldContinue = true;
	vector<Resto> unusedRestos = findUnusedResto(problem, solution);
	while (shouldContinue) {
		shouldContinue = false;
		// Try 1-2 to 1-2 swaps for each possible combination 
		for (unsigned int i = 0; i < solution.restos.size(); i++) {
			for (unsigned int j = 0; j < unusedRestos.size(); j++) {
				for (unsigned int k = 0; k < solution.restos.size(); k++) {
					if (k != i) {
						for (unsigned int l = 0; l < unusedRestos.size(); l++) {
							if (k >= solution.restos.size()) {
								break;
							}
							if (l != j) {
								Resto newResto_j = unusedRestos[j];
								Resto newResto_l = unusedRestos[l];
								Resto curResto_i = solution.restos[i];
								Resto curResto_k = solution.restos[k];

								unsigned int oneToOneRevenue = currentRevenue - curResto_i.r + newResto_j.r;
								unsigned int oneToTwoRevenue = currentRevenue - curResto_i.r + newResto_j.r + newResto_l.r;
								unsigned int twoToOneRevenue = currentRevenue - curResto_i.r - curResto_k.r + newResto_j.r;

								unsigned int oneToOneCapacity = solution.totalChickens - curResto_i.q + newResto_j.q;
								unsigned int oneToTwoCapacity = solution.totalChickens - curResto_i.q + newResto_j.q + newResto_l.q;
								unsigned int twoToOneCapacity = solution.totalChickens - curResto_i.q - curResto_k.q + newResto_j.q;

								vector<unsigned int> potentialRevenues = { oneToOneRevenue, oneToTwoRevenue, twoToOneRevenue };
								sort(potentialRevenues.begin(), potentialRevenues.end());
								
								for (unsigned int r = 0; r < potentialRevenues.size(); r++) {
									if (potentialRevenues[r] <= currentRevenue) {
										break;
									}
									if (potentialRevenues[r] == oneToOneRevenue && oneToOneCapacity <= problem.capacity) {
										swap(solution.restos[i], unusedRestos[j]);
										currentRevenue = oneToOneRevenue;
										solution.totalChickens = oneToOneCapacity;
										shouldContinue = true;
										break;
									}
									else if (potentialRevenues[r] == oneToTwoRevenue && oneToTwoCapacity <= problem.capacity) {
										swap(solution.restos[i], unusedRestos[j]);
										solution.restos.push_back(newResto_l);
										unusedRestos.erase(unusedRestos.begin() + l);
										currentRevenue = oneToTwoRevenue;
										solution.totalChickens = oneToTwoCapacity;
										shouldContinue = true;
										break;
									}
									else if (twoToOneCapacity <= problem.capacity) { // potentialRevenues[r] == twoToOneRevenue
										swap(solution.restos[i], unusedRestos[j]);
										unusedRestos.push_back(curResto_k);
										solution.restos.erase(solution.restos.begin() + k);
										currentRevenue = twoToOneRevenue;
										solution.totalChickens = twoToOneCapacity;
										shouldContinue = true;
										break;
									}
								}
							}
						}
					}
				}
			}
		}
		// Contrainte de temps
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

	Problem problem = readProblem(fileName);
	Solution solution = solve(problem, algo);



	if (showSolution) {
		for (unsigned int i = 0; i < solution.restos.size(); i++) {
			cout << solution.restos[i].id << " ";
		}
		cout << endl;
	}
	if (showTime) {
		// TODO: show the time
		cout << solution.elapsedTime << endl;
	}
}

void showDebugSolution(Solution sol, bool showR, Problem problem) {
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

Solution solve(Problem & problem, string & algo) {
	if (algo == GLOUTON) {
		return resolveGlouton(problem);
	}
	else if (algo == PROGRAMMATION_DYNAMIQUE) {
		return resolveDynProg(problem);
	}
	else if (algo == AMELIORATION_LOCALE) {
		return resolveHeu(problem);
	}
	else {
		Solution solution;
		cout << "Please select a valid algo" << endl;
		return solution;
	}
}

void writeToComparisonCSV() {
	std::ofstream comparaisonCSV;
	comparaisonCSV.open("./comparaison.csv");
	comparaisonCSV << "filename,size,timeGlouton,timeProgdyn,timeLocal,revenuGlouton,revenuProgdyn,revenuLocal\n";
}

void appendToComparisonCSV(string filename,
	string size,
	double timeGlouton,
	double timeProgdyn,
	double timeLocal,
	int revenuGlouton,
	int revenuProgdyn,
	int revenuLocal) {
	std::ofstream comparaisonCSV;
	comparaisonCSV.open("./comparaison.csv", ios_base::app);
	comparaisonCSV << filename << ","
		<< size << ","
		<< timeGlouton << ","
		<< timeProgdyn << ","
		<< timeLocal << ","
		<< revenuGlouton << ","
		<< revenuProgdyn << ","
		<< revenuLocal << "\n";
}

string getFilename(string size, string serie, string exemplaire) {
	return "./exemplaires/WC-" + size + "-" + serie + "-" + exemplaire + ".txt";
}

struct Sol10Glouton {
	double timeMean;
	double revenueMean;
};

Sol10Glouton solveGlouton10Times(Problem problem) {
	double timeTotal = 0;
	int revenueTotal = 0;
	for (int i = 0; i < 10; i++) {
		Solution sol = resolveGlouton(problem);
		timeTotal += sol.elapsedTime;
		revenueTotal += findRevenue(sol);
	}
	Sol10Glouton sol10Glouton;
	sol10Glouton.timeMean = timeTotal / 10.0;
	sol10Glouton.revenueMean = revenueTotal / 10.0;

	return sol10Glouton;
}

void gatherResults() {
	Problem problem;

	vector<double> times;
	vector<int> revenues;
	vector<string> sizes = { "100", "1000", "10000" };
	vector<string> series = { "10", "100", "1000" };
	vector<string> exemplaires = { "01", "02", "03", "04", "05", "06", "07", "08", "09", "10" };
	string filename;
	string numbers;
	writeToComparisonCSV();
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 10; k++) {
				filename = getFilename(sizes[i], series[j], exemplaires[k]);
				problem = readProblem(filename);
				cout << filename << endl;
				vector<Solution> solutionsGlouton;


				Sol10Glouton solGlouton = solveGlouton10Times(problem);
				Solution solProgdyn;
				int revenueProgDyn;
				if (sizes[i] == "10000" && series[j] == "1000") {
					solProgdyn.elapsedTime = 0;
					revenueProgDyn = 0;
				}
				else {
					solProgdyn = resolveDynProg(problem);
					revenueProgDyn = findRevenue(solProgdyn);
				}
				Solution solLocal = resolveHeu(problem);

				cout << endl;
				appendToComparisonCSV(
					filename,
					sizes[i],
					solGlouton.timeMean,
					solProgdyn.elapsedTime,
					solLocal.elapsedTime,
					solGlouton.revenueMean,
					revenueProgDyn,
					findRevenue(solLocal)
				);
			}

		}
	}
}

int main(int argc, const char * argv[]) {
	string joelPathP6 = R"(C:\Users\Joel\Documents\inf8775\tp2\exemplaires\WC-100-10-06.txt)";
	string joelPathP7 = R"(C:\Users\Joel\Documents\inf8775\tp2\exemplaires\WC-100-10-07.txt)";

	string fabPathP6 = "./exemplaires/WC-100-10-06.txt";
	string fabPathP7 = "./exemplaires/WC-100-10-07.txt";
	string fabPathP6_2 = R"(C:\Users\fabrice\Desktop\0TRAVAUX\INF8775\tp2\exemplaires\WC-100-10-06.txt)";
	string fabPathP7_2 = R"(C:\Users\fabrice\Desktop\0TRAVAUX\INF8775\tp2\exemplaires\WC-100-10-07.txt)";

	//useInterface(argv);
	//gatherResults();

	Problem problem = readProblem(fabPathP7_2);
	//Problem problem_7 = readProblem(fabPathP7_2);
	//	showProblemData(problem);
	Solution solutionGlouton = resolveGlouton(problem);
	Solution solutionDynProg = resolveDynProg(problem);
	Solution solutionHeu = resolveHeu(problem);
	cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << endl;
	cout << "Glouton" << endl;
	showDebugSolution(solutionGlouton, true, problem);
	cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << endl;
	cout << "Programmation dynamique" << endl;
	showDebugSolution(solutionDynProg, true, problem);
	cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << endl;
	cout << "Heuristique" << endl;
	showDebugSolution(solutionHeu, true, problem);
	//system("pause");
#ifdef _WIN32
	system("pause");
#endif
	return 0;
}
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

using namespace std;

struct Resto {
    int i; // numero du resto
    int r; // revenu du resto
    int q; // coût du resto
};

struct Problem {
    int N; // nombre de resto
    int capacity;
    vector<Resto> restos;
};

void useInterface(const char * argv[]);
void showProblemData(Problem problem);
Problem readProblem(string filename);
void useInterface(const char * argv[]);


void showProblemData(Problem problem) {
    cout << "N: " << problem.N << endl;
    cout << "capacity: " << problem.capacity << endl;
    for (int j = 0; j < problem.restos.size(); j ++) {
        Resto i = problem.restos[j];
        cout << "resto i: " << i.i << ", r: " << i.r << ", q: " << i.q << endl;
    }
}

Problem readProblem(string filename) {
    Problem prob;

    ifstream fichier;
	fichier.open(filename);
	if (!fichier) {
		cerr << "Error opening file";
	}
	unsigned int compteur = 0;
    fichier >> prob.N;
	while (compteur < prob.N) {
        Resto resto;
        fichier >> resto.i;
        fichier >> resto.r;
        fichier >> resto.q;
		prob.restos.push_back(resto);
		compteur++;
	}
    fichier >> prob.capacity;
    fichier.close();

    return prob;
}


void useInterface(const char * argv[]) {
	// argv[1]: filename argv[2]: algoName argv[3-4]: show sorted / show time
	string fileName, algo, addArg1, addArg2;
	bool showSolution = false, showTime = false;
	if(argv[1] != nullptr) fileName = argv[1];
	if(argv[2] != nullptr) algo = argv[2];
	if(argv[3] != nullptr) addArg1 = argv[3];
	if(argv[4] != nullptr) addArg2 = argv[4];
	if(addArg1 == "-p" || addArg2 == "-p") {
		showSolution = true;
	}
	if(addArg1 == "-t" || addArg2 == "-t") {
		showTime = true;
	}

    // TODO: lire l'exemplaire et le résoudre avec le bon algo et en mesurant le temps




	if(showSolution) {
		// TODO: show the solution
	} 
	if(showTime) {
        // TODO: show the time
		//cout << endl << elapsedTime << endl;
	}
} 

int main(int argc, const char * argv[]) {
	
    Problem prob = readProblem("./exemplaires/WC-100-10-06.txt");
    showProblemData(prob);
	return 0;
}
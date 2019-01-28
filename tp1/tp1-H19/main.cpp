#include <iostream>
#include <fstream>

using namespace std;

int* chargerDonnees(){
    ifstream fichier;
    unsigned int value;
    int* tableau_lu = new int[1000];
    fichier.open("../exemplaires//testset_1000_0.txt");
    if(!fichier){
        cerr << "Error opening file";
    }
    unsigned int compteur = 0;
    while (fichier >> value){
        tableau_lu[compteur] = value;
        compteur++;
    }
    fichier.close();
    return tableau_lu;
}
int main() {
    int* tableau_lu = chargerDonnees();
    for (int i = 0; i < 1000; i++){
        cout << tableau_lu[i] << ", ";
    }
    return 0;
}
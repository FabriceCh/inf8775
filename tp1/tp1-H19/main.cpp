#include <iostream>
#include <fstream>
#include <utility>

using namespace std;

int triRapideRecursif(int tableau_lu[], int pointeur_gauche, int pointeur_droit){
    int pivot = tableau_lu[pointeur_droit];
    int i = (pointeur_gauche - 1);
    for (int j = pointeur_gauche; j <= pointeur_droit - 1; j++){
        if (tableau_lu[j] <= pivot){
            i++;
            swap(tableau_lu[i], tableau_lu[j]);
        }
    }
    swap(tableau_lu[i + 1], tableau_lu[pointeur_droit]);
    return (i + 1);
}

void triRapide(int tableau_lu[], int pointeur_gauche, int pointeur_droit){
    int pivot = triRapideRecursif(tableau_lu, pointeur_gauche, pointeur_droit);
    if (pointeur_gauche < pointeur_droit){
        triRapide(tableau_lu, pointeur_gauche, pivot - 1);
        triRapide(tableau_lu, pivot + 1, pointeur_droit);
    }
}

int main() {
    int tableau_lu[1000];
    ifstream fichier;
    unsigned int nombre;
    fichier.open("../exemplaires//testset_1000_0.txt");
    if(!fichier){
        cerr << "Error opening file";
    }
    unsigned int compteur = 0;
    while (fichier >> nombre){
        tableau_lu[compteur] = nombre;
        compteur++;
    }
    fichier.close();
    int pointeur_gauche = 0;
    int pointeur_droit = 1000;
    triRapide(tableau_lu, pointeur_gauche, pointeur_droit);
    for (int i = 0; i < 1000; i++) {
        cout << tableau_lu[i] << ", ";
    }
}
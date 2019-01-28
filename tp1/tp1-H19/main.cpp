#include <iostream>
#include <fstream>
#include <utility>
#include <vector>

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

int* lireNombres(char* filename) {
    int* nombres_lus;
    ifstream fichier;
    unsigned int nombre;
    fichier.open(filename);
    
    if(!fichier){
        cerr << "Error opening file";
    }
    unsigned int compteur = 0;
    while (fichier >> nombre){
        nombres_lus[compteur] = nombre;
        compteur++;
    }
    fichier.close();
    return nombres_lus;
}

int main(int argc, char** argv) { //utilisation: ./main [path]
    char* filename = argv[1];
    cout << "filename: " << filename << "\n";
    int* nombres_lus = lireNombres(filename);
    for (int i = 0; i < 1000; i++) {
        cout << nombres_lus[i] << ", ";
    }
    /*int pointeur_gauche = 0;
    int pointeur_droit = 1000;
    triRapide(nombres_lus, pointeur_gauche, pointeur_droit);
    for (int i = 0; i < 1000; i++) {
        cout << nombres_lus[i] << ", ";
    }*/
}
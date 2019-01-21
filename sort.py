import time

NB_EXEMPLAIRES_SERIE_1 = 1000
NB_EXEMPLAIRES_SERIE_2 = 5000
NB_EXEMPLAIRES_SERIE_3 = 10000
NB_EXEMPLAIRES_SERIE_4 = 50000
NB_EXEMPLAIRES_SERIE_5 = 100000
NB_EXEMPLAIRES_SERIE_6 = 500000

def triParDenombrement(nNombres, iEnsemble):
    compteur_fichier_ensemble_1 = 0 
    nom_fichier_ensemble_1 = "testset_" +  str(NB_EXEMPLAIRES_SERIE_1) + "_" + str(compteur_fichier_ensemble_1) + ".txt"
    with open(nom_fichier_ensemble_1) as f:
        tableau_lu = f.readlines()
        tableau_lu = [int(x.strip()) for x in tableau_lu]
    sonde_temporelle_debut = time.time() 
    taille_nouveau_tableau = max(tableau_lu)
    nouveau_tableau = [0] * taille_nouveau_tableau
    for nombre in tableau_lu:
        nouveau_tableau[nombre - 1] += 1
    result = ""
    for indice in range(len(nouveau_tableau)):
        for i in range(nouveau_tableau[indice]):
            result += str(indice) + ", "
    print(result)
    # print(nouveau_tableau)
    # sonde_temporelle_debut = timeit.it() 

if __name__ == '__main__':
    triParDenombrement(1000, 0)


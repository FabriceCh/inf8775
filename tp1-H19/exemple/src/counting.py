import sys

ex_path = sys.argv[1] # Path de l'exemplaire

# Algo ici

options = sys.argv[2:]
if '-p' in options: # On imprime les nombres triés
    print("1 2 3 4 7") # Données bidon, mais output du bon format demandé
if '-t' in options: # On imprime le temps d'exécution
    print("4.1347628746") # Données bidon, mais output du bon format demandé

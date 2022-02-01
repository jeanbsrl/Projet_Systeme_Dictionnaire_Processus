# Dictionnaire de données

Le but de ce projet est de programmer une application qui permet de stocker et de consulter
un dictionnaire de données. Les données seront de type chaîne de caractères.

Le dictionnaire sera une collection associative, c'est-à-dire que les données seront stockées
grâce à une association clé-valeur, la clé de stockage étant un entier. Plus précisément, nous
utiliserons une liste chaînée de couples clé-valeur.

Le dictionnaire sera réparti entre N processus node, N ≥ 2. Ce nombre de processus sera
choisi au lancement, passé en argument à la commande.

# Installation/Compilation

Ce projet peut être compiler en 2 modes différents :

 * **mode classique** : vous pourrez utiliser l'application sans affichage superflu

 Pour compiler le projet en version classique, utilisez simplement la commande **make**

 * **mode DEBUG** : des informations supplémentaires seront affichées, notamment dans le but de vérifier le bon fonctionnement des nodes et pipes

 Pour compiler le projet en mode DEBUG, utilisez la commande **make DEBUG**

# Utilisation

Pour lancer le projet, vous devez utiliser la commande `./projet_1 entier`, où entier représente le nombre de nodes qui
seront créés dans le programme. 

Ce nombre de node doit être **supérieur ou égal à 2**

# Auteurs

FRANCES Tom (Lien git : https://github.com/l3miage-franceto)

BOU SERHAL Jean

L3 MIAGE, 2021-2022
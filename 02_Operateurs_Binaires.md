# Partie 2 : Application avancée des opérateurs binaires en C++

## 1. Introduction

Dans cette seconde partie, nous approfondirons l'utilisation des opérateurs binaires dans des contextes plus complexes, allant de la manipulation des nombres flottants à l'optimisation des performances avec des systèmes à faible niveau. Nous explorerons des cas réels d’utilisation des opérateurs binaires dans la gestion de la mémoire, les calculs de hachage, et la manipulation de données visuelles.

---

## 2. Représentation binaire des nombres flottants

Les nombres flottants sont utilisés dans de nombreux calculs nécessitant une grande précision. Leur représentation en mémoire suit le standard IEEE 754, qui les divise en trois parties : le **signe**, l’**exposant** et la **mantisse**. Manipuler les bits de ces nombres peut être utile dans des applications à faible niveau.

### 2.1. Conversion des flottants en binaire

Les nombres flottants sont composés de 32 bits (pour les `float` en C++), et ces bits sont organisés en trois parties : un bit pour le signe, 8 bits pour l'exposant, et 23 bits pour la mantisse. Manipuler directement ces bits peut permettre d'extraire des informations sur la valeur ou d'optimiser certains calculs.

#### Exemple : Affichage des bits d'un nombre flottant

```cpp
#include <iostream>
#include <bitset>
using namespace std;

union FloatUnion {
    float f;
    uint32_t i;
};

int main() {
    FloatUnion u;
    u.f = 3.14f;

    // Affichage des bits du nombre flottant
    cout << "Bits de 3.14 : " << bitset<32>(u.i) << endl;

    return 0;
}
```

**Explication** :
- Une **union** est utilisée pour accéder au même espace mémoire soit comme un `float`, soit comme un `uint32_t` (entier 32 bits), ce qui permet de visualiser la représentation binaire du flottant.
- Le nombre `3.14f` est stocké dans `u.f` et ses bits sont affichés sous forme binaire grâce à `bitset<32>`.

#### **Exercice** :
1. Affichez les bits de plusieurs nombres flottants (`1.23f`, `-42.0f`, `0.5f`) en utilisant ce même code.
2. Utilisez des opérateurs binaires pour modifier des bits spécifiques (par exemple, modifiez le signe ou l'exposant).

---

## 3. Bitwise Hashing (Fonction de Hachage avec opérateurs binaires)

Les fonctions de hachage permettent de transformer une donnée (comme une chaîne de texte) en une valeur unique. Elles sont couramment utilisées pour optimiser les performances des tables de hachage. L'opérateur **XOR** est souvent utilisé pour cette tâche, car il répartit efficacement les bits de la donnée source.

### 3.1. Implémentation d'une fonction de hachage simple

Une fonction de hachage transforme les caractères d'une chaîne en un entier unique à l'aide d'opérations binaires.

#### Exemple : Fonction de hachage simple avec XOR

```cpp
#include <iostream>
#include <string>
using namespace std;

uint32_t simpleHash(const string& str) {
    uint32_t hash = 0;

    for (size_t i = 0; i < str.size(); ++i) {
        // XOR entre le hash courant et le caractère
        hash ^= (str[i] << (i % 8));  // Décalage à gauche pour répartir les bits
    }

    return hash;
}

int main() {
    string str = "Hello, World!";
    cout << "Hachage de '" << str << "' : " << simpleHash(str) << endl;

    return 0;
}
```

**Explication** :
- La fonction `simpleHash` prend chaque caractère de la chaîne et applique l'opération **XOR** avec un décalage à gauche.
- Le décalage permet de répartir les bits de manière différente à chaque itération, assurant une meilleure répartition des valeurs dans l'espace de hachage.

#### **Exercice** :
1. Modifiez le code pour utiliser d'autres opérations binaires comme **AND** ou **OR**.
2. Testez la fonction sur différentes chaînes et analysez les valeurs retournées.

---

## 4. Optimisation des performances avec les opérateurs binaires

Les systèmes à faible niveau et les applications qui requièrent une gestion optimisée des ressources (comme les systèmes embarqués ou les jeux vidéo) bénéficient grandement des opérations binaires, car elles sont plus rapides que les opérations arithmétiques classiques.

### 4.1. Opérations rapides avec les opérateurs binaires

Les opérations binaires permettent des optimisations importantes. Par exemple, pour multiplier ou diviser un nombre par une puissance de 2, il suffit de déplacer les bits au lieu d'effectuer une multiplication ou une division traditionnelle.

#### Exemple : Multiplication et division rapide par 2

```cpp
#include <iostream>
using namespace std;

int main() {
    int x = 10;

    // Multiplication par 2 avec décalage à gauche
    int resultMultiply = x << 1;  // Décalage de 1 bit à gauche
    cout << "Multiplication par 2 : " << resultMultiply << endl;

    // Division par 2 avec décalage à droite
    int resultDivide = x >> 1;  // Décalage de 1 bit à droite
    cout << "Division par 2 : " << resultDivide << endl;

    return 0;
}
```

**Explication** :
- Le décalage à gauche (`<<`) équivaut à une multiplication par 2, tandis que le décalage à droite (`>>`) correspond à une division par 2.
- Ces opérations sont beaucoup plus rapides que les équivalents multiplicatifs ou divisionnels classiques.

#### **Exercice** :
1. Testez d'autres puissances de 2 en modifiant le nombre de bits à décaler.
2. Comparez le temps d'exécution d'un décalage avec une multiplication/division classique.

---

## 5. Manipulation avancée des couleurs avec des opérateurs binaires

Les couleurs sont souvent représentées sous forme de **codes RGB**, où chaque composante de couleur (Rouge, Vert, Bleu) est stockée sur 8 bits. Grâce aux opérateurs binaires, on peut extraire ou modifier facilement ces composantes dans une couleur.

### 5.1. Utilisation des opérateurs binaires pour manipuler les couleurs

Les opérateurs binaires sont parfaits pour extraire ou modifier les composantes RGB dans un code hexadécimal représentant une couleur.

#### Exemple : Modification des composantes de couleur RGB

```cpp
#include <iostream>
using namespace std;

int main() {
    uint32_t color = 0xFF5733;  // Couleur RGB : #FF5733 (Rouge = 255, Vert = 87, Bleu = 51)

    // Extraction de chaque composante de couleur
    uint8_t red = (color >> 16) & 0xFF;  // Décalage à droite pour extraire le rouge
    uint8_t green = (color >> 8) & 0xFF;  // Décalage à droite pour extraire le vert
    uint8_t blue = color & 0xFF;         // Extraire le bleu

    cout << "Rouge : " << (int)red << endl;
    cout << "Vert : " << (int)green << endl;
    cout << "Bleu : " << (int)blue << endl;

    // Changer la composante bleue
    color = (color & 0xFFFF00) | (0x99);  // Modifier la composante bleue à 0x99

    cout << "Nouvelle couleur : #" << hex << color << endl;

    return 0;
}
```

**Explication** :
- Le nombre `0xFF5733` représente une couleur RGB. Le code extrait chaque composante en utilisant un décalage à droite et un masque binaire pour obtenir les 8 bits correspondants.
- La composante bleue est modifiée en utilisant une combinaison d'opérations **AND** et **OR** pour préserver les autres composantes.

#### **Exercice** :
1. Modifiez le code pour changer la composante rouge ou verte et affichez la nouvelle couleur.
2. Expérimentez avec d'autres valeurs de couleurs et observez les changements.

---

## 6. Mini-projet : Développer un convertisseur de couleurs en C++

### 6.1. Objectif

L'objectif de ce mini-projet est de créer un programme qui permet de convertir une couleur donnée en format RGB (par exemple, `#FF5733`) en différentes représentations binaires, hexadécimales et décimales, en utilisant des opérateurs binaires pour extraire chaque composante de couleur.

### 6.2. Plan de réalisation

1. **Entrée de la couleur** : Demandez à l'utilisateur de saisir une couleur en format hexadécimal
2. **Extraction des composantes** : Utilisez des opérateurs binaires pour extraire les valeurs rouge, verte et bleue.
3. **Conversion** : Affichez la couleur sous forme binaire, hexadécimale et décimale.
4. **Testez avec différentes couleurs**.

---

## 7. Conclusion de la Partie 2

Cette deuxième partie vous a permis d'explorer des applications avancées des opérateurs binaires, notamment dans la manipulation des nombres flottants, les fonctions de hachage, l'optimisation des performances, et la gestion des couleurs. Grâce à ces compétences, vous êtes désormais en mesure d'aborder des systèmes à faible niveau et des algorithmes nécessitant une gestion efficace de la mémoire et des ressources.

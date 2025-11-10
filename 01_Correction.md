# Corrections des Exercices - Partie 1 : Opérateurs Binaires et Manipulation de Données

## 1. Correction des Exercices Pratiques

### Exercice 1 : Écriture Binaire
**Énoncé :** Convertir les nombres 7, 9 et 12 en binaire.

- 7 (décimal) = 0111 (binaire)
- 9 (décimal) = 1001 (binaire)
- 12 (décimal) = 1100 (binaire)

### Exercice 2 : Table de Vérité
**Énoncé :** Effectuer les opérations suivantes : `5 & 6`, `7 | 2`, et `4 ^ 3`.

#### Calculs :
1. `5 & 6` :
   - 5 en binaire : 0101
   - 6 en binaire : 0110
   - Résultat (AND) : 0100 (4 en décimal)

2. `7 | 2` :
   - 7 en binaire : 0111
   - 2 en binaire : 0010
   - Résultat (OR) : 0111 (7 en décimal)

3. `4 ^ 3` :
   - 4 en binaire : 0100
   - 3 en binaire : 0011
   - Résultat (XOR) : 0111 (7 en décimal)

### Exercice 3 : Décalage
**Énoncé :** Multiplier 5 par 8 en utilisant le décalage à gauche (`<<`).

#### Calcul :
- 5 en binaire : 0101
- Décalage à gauche de 3 bits (`<< 3`) :
  - 0101 << 3 = 0101000 (en binaire), soit 40 en décimal.

## 2. Correction des Défis de Programmation

### Défi 1 : Compteur Binaire (0 à 15)
**Énoncé :** Créer un programme qui affiche un compteur binaire de 0 à 15 (4 bits).

#### Code :
```cpp
#include <iostream>
#include <bitset>

int main() {
    for (int i = 0; i < 16; ++i) {
        std::cout << "Décimal : " << i << " -> Binaire : " 
                  << std::bitset<4>(i) << std::endl;
    }
    return 0;
}
```

### Défi 2 : Fonction de Hachage Simple
**Énoncé :** Implémenter une fonction de hachage simple qui transforme une chaîne en une valeur binaire unique à l'aide de XOR.

#### Code :
```cpp
#include <iostream>
#include <string>

unsigned int simpleHash(const std::string& input) {
    unsigned int hash = 0;
    for (char c : input) {
        hash ^= c;  // Appliquer XOR avec chaque caractère
    }
    return hash;
}

int main() {
    std::string input = "hello";
    std::cout << "Chaîne : " << input 
              << " -> Hachage : " << std::bitset<8>(simpleHash(input)) << std::endl;
    return 0;
}
```

#### Explications :
- La fonction `simpleHash` applique l'opérateur XOR (`^`) à chaque caractère de la chaîne pour produire un hachage unique.
- Le résultat est affiché en binaire (8 bits).

---

## 3. Résumé des Concepts Utilisés
- **Opérations Binaires** : AND (`&`), OR (`|`), XOR (`^`), et Décalage (`<<`, `>>`).
- **Représentation Binaire** : Conversion et manipulation des nombres binaires.
- **Applications Pratiques** : Compteur binaire et hachage XOR.

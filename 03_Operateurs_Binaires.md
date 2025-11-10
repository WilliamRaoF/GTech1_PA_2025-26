# Projet : Simulation de pixels RVB et filtres de couleurs

## Objectif

Créer un programme en C++ qui manipule des valeurs de pixels représentées en format RGB. L’utilisateur peut modifier les composantes de chaque pixel (rouge, vert, bleu) à l’aide d’opérations binaires, appliquer des filtres, et afficher le résultat dans un tableau.

---

## Étapes du projet

### 1. Initialisation des pixels

- Créez une matrice 3x3 représentant une image simplifiée avec 9 pixels.
- Chaque pixel est une valeur RGB codée sur 32 bits (0xRRGGBB).

### 2. Afficher les pixels

- Affichez chaque pixel dans la matrice en format **RGB** (exemple : `(255, 87, 51)`).

### 3. Modifier un pixel

- L’utilisateur choisit un pixel (ligne, colonne) et peut :
  - Modifier une composante couleur spécifique (rouge, vert, bleu).
  - Appliquer un filtre (par exemple, rendre le pixel plus rouge en augmentant la valeur rouge).

### 4. Appliquer un filtre global

- L’utilisateur applique un filtre global à tous les pixels :
  - Inversion des couleurs.
  - Conversion en niveaux de gris.

---

## Instructions détaillées

1. **Initialisation de la matrice de pixels** :
   - Définissez une matrice 3x3 en utilisant des valeurs hexadécimales pour représenter les couleurs initiales.

2. **Affichage des pixels** :
   - Décomposez chaque valeur en composantes rouge, verte et bleue.
   - Affichez chaque pixel sous la forme `(R, G, B)`.

3. **Modification des pixels** :
   - L’utilisateur doit sélectionner un pixel en spécifiant une ligne et une colonne.
   - L’utilisateur peut entrer de nouvelles valeurs pour les composantes rouge, verte et bleue.

4. **Filtres globaux** :
   - Implémentez deux filtres :
     - **Inversion des couleurs** : inversez chaque composante de chaque pixel.
     - **Niveaux de gris** : remplacez chaque composante d’un pixel par la moyenne des trois composantes (R, G, B).

---

## Consignes

- Utilisez des opérations binaires pour manipuler les composantes des pixels.
- Affichez la matrice initiale, la matrice après modification d’un pixel, et la matrice après application d’un filtre global.
- Structurez le programme pour permettre une interaction simple avec l’utilisateur.

---

## Exigences techniques

- Le programme doit utiliser :
  - **Opérations binaires** pour extraire et modifier les composantes de couleur.
  - **Boucles** pour parcourir la matrice.
  - **Entrées utilisateur** pour sélectionner et modifier les pixels.
- Assurez-vous que le programme fonctionne sans erreurs pour toutes les entrées valides.

---

## Objectifs pédagogiques

- Appliquer les notions d’opérations binaires dans un contexte pratique.
- Travailler avec des structures de données multidimensionnelles.
- Renforcer la manipulation des entrées et sorties en C++.


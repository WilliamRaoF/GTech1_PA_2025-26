# Exercices – Manipulation des fenêtres Windows (API Win32)

## Niveau 1 – Bases de la fenêtre

### Exercice 1 : Fenêtre simple
1. Créez une fenêtre 400x300.
2. Changez la couleur du fond en bleu (`CreateSolidBrush` + `RGB(0,0,255)`).
3. Modifiez le titre de la fenêtre.

---

### Exercice 2 : Redimensionnement automatique
- Créez une fenêtre de 600x400 pixels.  
- Lorsque l’utilisateur redimensionne la fenêtre, affichez dans la barre de titre les nouvelles dimensions (exemple : `Fenêtre - 800x600`).  
*Indice : interceptez le message `WM_SIZE` et utilisez `SetWindowText`.*

---

### Exercice 3 : Changement de curseur
- Affichez un curseur différent selon la zone de la fenêtre :
  - Zone de gauche : curseur "main" (`LoadCursor(NULL, IDC_HAND)`).
  - Zone de droite : curseur "croix" (`LoadCursor(NULL, IDC_CROSS)`).
*Indice : interceptez `WM_MOUSEMOVE` et comparez `LOWORD(lParam)` à la moitié de la largeur de la fenêtre.*

---

### Exercice 4 : Gestion du clavier
- Affichez dans la barre de titre la dernière touche pressée.
- Si l’utilisateur appuie sur Échap, fermez la fenêtre.
*Indice : utilisez `WM_KEYDOWN` et `GetKeyNameText`.*

---

## Niveau 2 – Interaction utilisateur

### Exercice 5 : Clics de souris
- Lorsque l’utilisateur clique avec le bouton gauche, affichez un `MessageBox` indiquant les coordonnées du clic.  
Exemple : "Clic à (120, 240)".
*Indice : interceptez `WM_LBUTTONDOWN` et utilisez `LOWORD(lParam)` / `HIWORD(lParam)`.*

---

### Exercice 6 : Déplacement manuel de la fenêtre
- Quand l’utilisateur fait glisser la souris avec le bouton gauche enfoncé, déplacez la fenêtre en conséquence.
*Indices : utilisez `WM_LBUTTONDOWN`, `WM_MOUSEMOVE` et `WM_LBUTTONUP`.  
Les fonctions `SetWindowPos` ou `MoveWindow` permettent de repositionner la fenêtre.*

---

### Exercice 7 : Couleur dynamique
- Changez la couleur de fond de la fenêtre à chaque clic gauche :
  - 1er clic : rouge
  - 2e clic : vert
  - 3e clic : bleu
  - puis recommencez.
*Indice : stockez un compteur statique dans `WndProc` et utilisez `CreateSolidBrush`.*

---

## Niveau 3 – Dessin et GDI

### Exercice 8 : Dessiner un rectangle
- Dans `WM_PAINT`, dessinez un rectangle rouge au centre de la fenêtre.
*Indice : utilisez `BeginPaint`, `EndPaint` et `Rectangle`.*

---

### Exercice 9 : Dessiner selon la souris
- Chaque clic de souris dessine un petit cercle à l’endroit du clic.
*Indice : stockez les coordonnées dans une liste et redessinez dans `WM_PAINT`.*

---

### Exercice 10 : Effet "dessin à main levée"
- Pendant que le bouton gauche est maintenu, tracez une ligne continue qui suit le curseur.
*Indice : utilisez `WM_MOUSEMOVE`, `MoveToEx` et `LineTo`.*

---

## Niveau 4 – Contrôles et interactions système

### Exercice 11 : Boutons et zones de texte
- Ajoutez deux boutons ("Créer", "Quitter") et une zone de texte.
- Quand on clique sur "Créer", affichez un `MessageBox` avec le texte saisi.
- Quand on clique sur "Quitter", fermez la fenêtre.
*Indices : utilisez `CreateWindowEx` avec les classes "BUTTON" et "EDIT".  
Gérez les événements via `WM_COMMAND`.*

---

### Exercice 12 : Menu simple
- Créez un menu :
  - Fichier → Quitter
  - Aide → À propos
- Quand on clique sur "À propos", affichez un `MessageBox`.
*Indice : utilisez `CreateMenu` et `AppendMenu`.*

---

### Exercice 13 : Changement de fond via menu
- Ajoutez une option "Couleur du fond" au menu.
- Quand on la sélectionne, ouvrez une boîte de dialogue `ChooseColor`.
*Indice : la structure `CHOOSECOLOR` et la fonction `ChooseColor()` permettent de récupérer la couleur choisie.*

---

## Niveau 5 – Mini-projets

### Mini-projet 1 : Bloc-notes simplifié
- Créez une fenêtre avec :
  - Une zone de texte multi-ligne (`EDIT`).
  - Un menu "Fichier" (Nouveau, Ouvrir, Enregistrer, Quitter).
- Implémentez les boîtes de dialogue `GetOpenFileName` et `GetSaveFileName`.

---

### Mini-projet 2 : Paint minimal
- Créez une zone de dessin où l’utilisateur peut :
  - Tracer à la souris.
  - Changer de couleur via un menu.
  - Effacer tout.
*Combinez `WM_PAINT`, `WM_MOUSEMOVE` et `WM_COMMAND`.*

---

© Formation : Introduction à la programmation Windows API en C++

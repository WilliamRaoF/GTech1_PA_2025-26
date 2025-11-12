# Cours : Introduction à l’API Windows en C++

## Objectifs de la formation

À la fin de ce cours, vous serez capable de :

1. Comprendre la structure interne d’une application Windows.
2. Créer et gérer une fenêtre interactive.
3. Manipuler les événements du clavier et de la souris.
4. Dessiner des éléments graphiques avec le GDI.
5. Utiliser des contrôles standards (boutons, zones de texte…).
6. Réaliser un mini-projet complet en fin de cours.

---

## Partie 1 – Introduction : Architecture des applications Windows

### 1.1 Comprendre le modèle événementiel de Windows

Les programmes Windows sont **basés sur les messages**.  
Chaque action (clic, frappe de touche, redimensionnement de la fenêtre, etc.) est traduite par le système en **message**, envoyé à la fenêtre concernée.

#### Composants principaux :

| Élément | Rôle |
|----------|------|
| `WinMain` | Point d’entrée du programme (équivalent du `main()` en console). |
| `WNDCLASS` / `WNDCLASSEX` | Structure qui décrit le comportement et le style d’une fenêtre. |
| `RegisterClass` | Enregistre la “recette” (classe de fenêtre) auprès de Windows. |
| `CreateWindowEx` | Crée une instance concrète de la fenêtre. |
| `WndProc` | Fonction de rappel (callback) qui reçoit et traite les messages. |
| `Message Loop` | Boucle principale qui lit les messages système et les envoie à `WndProc`. |

> **Analogie :**
> Imaginez Windows comme un **bureau** qui envoie du courrier (messages) à votre application.
> Votre fonction `WndProc` joue le rôle du **secrétaire** qui ouvre le courrier et agit selon son contenu.

---

### 1.2 Structure générale d’une application Windows

Voici le squelette minimal d’une application graphique Windows :

```cpp
#include <windows.h>

// Déclaration de la fonction de traitement des messages
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Point d'entrée de l'application Windows
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // 1️ Définir la classe de fenêtre
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;             // Fonction de traitement des messages
    wc.hInstance = hInstance;             // Instance de l'application
    wc.lpszClassName = L"MainWindow";     // Nom unique de la classe
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // Couleur de fond par défaut (blanche)

    // 2️ Enregistrer la classe auprès du système
    if (!RegisterClass(&wc)) {
        MessageBox(NULL, L"Erreur : Enregistrement de la classe échoué.", L"Erreur", MB_ICONERROR);
        return 0;
    }

    // 3️ Créer une fenêtre basée sur la classe enregistrée
    HWND hwnd = CreateWindowEx(
        0, L"MainWindow", L"Ma première application", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL, hInstance, NULL
    );

    if (!hwnd) {
        MessageBox(NULL, L"Erreur : Création de la fenêtre échouée.", L"Erreur", MB_ICONERROR);
        return 0;
    }

    // 4️ Afficher la fenêtre à l'écran
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // 5️ Boucle principale de messages
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg); // Traduit les entrées clavier (ex: touches accentuées)
        DispatchMessage(&msg);  // Envoie le message à WndProc
    }

    return (int)msg.wParam;
}

// Fonction de traitement des messages (callback)
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE:
            DestroyWindow(hwnd);  // Quand l’utilisateur ferme la fenêtre
            break;

        case WM_DESTROY:
            PostQuitMessage(0);   // Termine la boucle principale
            break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam); // Traitement par défaut
    }
    return 0;
}
````

---

### 1.3 Explication étape par étape

#### ➤ Étape 1 : Enregistrer la classe de fenêtre

Avant de créer une fenêtre, vous devez **décrire son comportement** via une structure `WNDCLASS` :

```cpp
WNDCLASS wc = {};
wc.lpfnWndProc = WndProc;         // Quelle fonction gère les messages
wc.hInstance = hInstance;         // Instance du programme
wc.lpszClassName = L"MainWindow"; // Nom interne de la fenêtre
wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // Couleur du fond
```

>  **Astuce :**
> Vous pouvez aussi personnaliser :
>
> * `wc.hCursor` pour changer le curseur.
> * `wc.hIcon` pour définir une icône.
> * `wc.style` pour le comportement général (redimensionnable, double-buffer, etc.).

---

#### ➤ Étape 2 : Créer la fenêtre

Une fois la classe enregistrée avec `RegisterClass`, vous pouvez instancier une fenêtre avec :

```cpp
HWND hwnd = CreateWindowEx(
    0, L"MainWindow", L"Titre de la fenêtre", WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
    NULL, NULL, hInstance, NULL
);
```

>  **À retenir :**
>
> * `HWND` = handle de la fenêtre (identifiant unique).
> * `WS_OVERLAPPEDWINDOW` = style complet (barre de titre, redimensionnement, etc.).

---

#### ➤ Étape 3 : Boucle de messages

Chaque application Windows contient une **boucle d’attente d’événements** :

```cpp
while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
}
```

>  **Concept clé :**
> Cette boucle intercepte les messages du système (clic, redimensionnement, frappe clavier, etc.)
> et les redirige vers la fonction `WndProc`.

---

#### ➤ Étape 4 : La fonction `WndProc`

C’est le **cœur** de votre application.
Chaque fois que quelque chose se passe, Windows appelle cette fonction avec le message correspondant :

```cpp
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
```

| Message          | Signification             |
| ---------------- | ------------------------- |
| `WM_PAINT`       | Redessiner la fenêtre     |
| `WM_KEYDOWN`     | Touche du clavier pressée |
| `WM_LBUTTONDOWN` | Clic gauche de souris     |
| `WM_CLOSE`       | Fenêtre fermée            |
| `WM_DESTROY`     | Fin du programme          |

>  **Bon réflexe :**
> Toujours renvoyer `DefWindowProc` pour les messages non traités.
> Cela permet à Windows de gérer le comportement par défaut (menus, focus, redimensionnement…).

#  Cours avancé : Utilisation de l’API Windows et GDI pour une application graphique

> Ce module approfondit la création d’interfaces Windows et la manipulation d’images (BMP/PNG) avec l’API Win32.

---

##  Objectifs

1. Savoir ajouter et gérer des contrôles Windows (boutons, zones de texte, labels…).
2. Charger et afficher des images BMP et PNG.
3. Lire et modifier les pixels d’une image (avec GDI).
4. Sauvegarder une image modifiée.
5. Comprendre les bonnes pratiques d’une application Win32 autonome.

---

##  Partie 1 – Créer des contrôles interactifs (boutons, zones de texte, labels)

### 1.1 Ajouter un bouton

Un bouton est créé via **`CreateWindowEx`** comme une sous-fenêtre (`WS_CHILD`).

```cpp
HWND hButton = CreateWindowEx(
    0, L"BUTTON", L"Ouvrir une image",
    WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
    20, 20, 150, 30,
    hwnd, (HMENU)1, hInstance, NULL
);
````

* `BUTTON` → classe standard Windows.
* `(HMENU)1` → identifiant du contrôle (utile dans `WM_COMMAND`).
* `BS_PUSHBUTTON` → style de bouton simple.

### 1.2 Gérer les clics sur un bouton

Dans la fonction `WndProc` :

```cpp
case WM_COMMAND:
    if (LOWORD(wParam) == 1) { // ID du bouton
        MessageBox(hwnd, L"Ouverture d'une image demandée !", L"Info", MB_OK);
    }
    break;
```

### 1.3 Ajouter une zone de texte (EDIT) et un label (STATIC)

```cpp
HWND hEdit = CreateWindowEx(
    0, L"EDIT", L"",
    WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT,
    20, 60, 200, 25,
    hwnd, (HMENU)2, hInstance, NULL
);

HWND hLabel = CreateWindowEx(
    0, L"STATIC", L"Zone de texte :",
    WS_VISIBLE | WS_CHILD,
    20, 40, 200, 20,
    hwnd, NULL, hInstance, NULL
);
```

>  **Astuce :**
> Vous pouvez récupérer le texte d’un contrôle `EDIT` avec `GetWindowText()`.

```cpp
wchar_t buffer[256];
GetWindowText(hEdit, buffer, 256);
MessageBox(hwnd, buffer, L"Texte saisi", MB_OK);
```

---

## Partie 2 – Charger et afficher des images BMP

### 2.1 Charger un fichier BMP avec GDI

Windows fournit une API simple pour lire des BMP : **`LoadImage`**

```cpp
HBITMAP hBitmap = (HBITMAP)LoadImage(
    NULL, L"exemple.bmp",
    IMAGE_BITMAP, 0, 0,
    LR_LOADFROMFILE | LR_CREATEDIBSECTION
);
```

>  **Important :**
>
> * Le flag `LR_CREATEDIBSECTION` permet d’accéder directement aux pixels.
> * Cette fonction **ne supporte que les BMP** nativement.

---

### 2.2 Afficher un BMP dans la fenêtre

Utilisation de **BitBlt** pour copier le bitmap dans le contexte de la fenêtre :

```cpp
case WM_PAINT: {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    if (hBitmap) {
        HDC hdcMem = CreateCompatibleDC(hdc);
        HBITMAP hOld = (HBITMAP)SelectObject(hdcMem, hBitmap);

        BITMAP bmp;
        GetObject(hBitmap, sizeof(bmp), &bmp);

        BitBlt(hdc, 250, 50, bmp.bmWidth, bmp.bmHeight, hdcMem, 0, 0, SRCCOPY);

        SelectObject(hdcMem, hOld);
        DeleteDC(hdcMem);
    }

    EndPaint(hwnd, &ps);
}
break;
```

---

##  Partie 3 – Lire et modifier les pixels d’un BMP

### 3.1 Obtenir l’accès mémoire aux pixels

Lors du chargement avec `LR_CREATEDIBSECTION`, le bitmap contient une structure **DIB** accessible via `GetObject()`.

```cpp
BITMAP bmp;
GetObject(hBitmap, sizeof(bmp), &bmp);

// Accès brut aux pixels (tableau de bytes)
BYTE* pixels = (BYTE*)bmp.bmBits;
```

Chaque pixel est généralement encodé en **BGR (Blue, Green, Red)**, 1 octet par composante.

> Exemple : le pixel `[x, y]` :
>
> ```cpp
> int offset = (y * bmp.bmWidthBytes) + (x * 3);
> BYTE blue  = pixels[offset + 0];
> BYTE green = pixels[offset + 1];
> BYTE red   = pixels[offset + 2];
> ```

---

### 3.2 Modifier un pixel

```cpp
pixels[offset + 0] = 255; // Blue
pixels[offset + 1] = 0;   // Green
pixels[offset + 2] = 0;   // Red
```

Ensuite, redessinez la fenêtre avec `InvalidateRect(hwnd, NULL, TRUE);` pour rafraîchir.

---

### 3.3 Sauvegarder un BMP modifié

Vous pouvez sauvegarder manuellement avec `CreateFile`, `WriteFile`, ou utiliser GDI :

```cpp
HDC hdc = GetDC(hwnd);
HDC hdcMem = CreateCompatibleDC(hdc);
SelectObject(hdcMem, hBitmap);
BitBlt(hdcMem, 0, 0, bmp.bmWidth, bmp.bmHeight, hdcMem, 0, 0, SRCCOPY);
ReleaseDC(hwnd, hdc);
```

Mais pour un vrai export, il est préférable d’écrire **l’en-tête BMP + données pixel** manuellement (struct `BITMAPFILEHEADER` + `BITMAPINFOHEADER`).

---

##  Partie 4 – Gérer les fichiers avec la boîte de dialogue Windows

### 4.1 Ouvrir un fichier

Utilisez la fonction standard **GetOpenFileName** :

```cpp
OPENFILENAME ofn = {0};
wchar_t filename[MAX_PATH] = L"";

ofn.lStructSize = sizeof(ofn);
ofn.hwndOwner = hwnd;
ofn.lpstrFilter = L"Images BMP\0*.bmp\0Toutes\0*.*\0";
ofn.lpstrFile = filename;
ofn.nMaxFile = MAX_PATH;
ofn.Flags = OFN_FILEMUSTEXIST;

if (GetOpenFileName(&ofn)) {
    MessageBox(hwnd, filename, L"Fichier sélectionné", MB_OK);
}
```

>  Cela permet à l’utilisateur de sélectionner une image BMP sans hardcoder le chemin.

---

##  Partie 5 – Lire des PNG avec GDI+ (optionnel)

Windows GDI ne gère pas les PNG nativement.
Mais **GDI+**, intégré dans Windows depuis XP, permet de les charger simplement.

### 5.1 Initialisation de GDI+

```cpp
#include <gdiplus.h>
using namespace Gdiplus;

ULONG_PTR gdiplusToken;
GdiplusStartupInput gdiplusStartupInput;
GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
```

### 5.2 Charger et dessiner un PNG

```cpp
Graphics graphics(hdc);
Image image(L"exemple.png");
graphics.DrawImage(&image, 250, 50);
```

### 5.3 Fermer GDI+

```cpp
GdiplusShutdown(gdiplusToken);
```

>  **Note :**
> GDI+ est léger et inclus par défaut dans Windows.
> Vous pouvez donc l’utiliser sans dépendance externe.

---

##  Conclusion

Avec ces notions, vous maîtrisez désormais :

* La création d’interfaces Win32 (boutons, zones de texte…)
* Le chargement, affichage et modification d’images BMP/PNG
* La manipulation directe des pixels pour la stéganographie
* Les bases d’une architecture Win32 propre et modulaire


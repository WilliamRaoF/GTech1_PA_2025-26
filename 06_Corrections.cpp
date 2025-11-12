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
    wc.hbrBackground = CreateSolidBrush(RGB(0,0,200)); // Couleur de fond par défaut (blanche)

    // 2️ Enregistrer la classe auprès du système
    if (!RegisterClass(&wc)) {
        MessageBox(NULL, L"Erreur : Enregistrement de la classe échoué.", L"Erreur", MB_ICONERROR);
        return 0;
    }

    // 3️ Créer une fenêtre basée sur la classe enregistrée
    HWND hwnd = CreateWindowEx(
        0, L"MainWindow", L"Test", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
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
    static int windowWidth;
    static int windowHeight;
    switch (uMsg) {
    case WM_CLOSE:
        DestroyWindow(hwnd);  // Quand l’utilisateur ferme la fenêtre
        break;

    case WM_DESTROY:
        PostQuitMessage(0);   // Termine la boucle principale
        break;

    case WM_SIZE: {
        windowWidth = LOWORD(lParam);
        windowHeight = HIWORD(lParam);
        wchar_t buff[128];
        wsprintf(buff, L"Fenetre - %d x %d", windowWidth, windowHeight);
        SetWindowText(hwnd, buff);
        break;
    }

    case WM_MOUSEMOVE: {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);

        if (x < windowWidth / 2) SetCursor(LoadCursor(NULL, IDC_HAND));
        else SetCursor(LoadCursor(NULL, IDC_CROSS));
    }

    case WM_KEYDOWN: {
        if (wParam == VK_ESCAPE) {
            PostMessage(hwnd, WM_CLOSE, 0, 0);
            return 0;
        }

        wchar_t keyName[64] = L"";
        GetKeyNameText((LONG)lParam, keyName, 64);
        wchar_t title[128];
        wsprintf(title, L"Derniere touche: %s", keyName);
        SetWindowText(hwnd, title);
        break;
    }
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam); // Traitement par défaut
    }
    return 0;
}

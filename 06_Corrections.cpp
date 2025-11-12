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


    // EXERCICE 1
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

static int windowWidth;
static int windowHeight;
// EXERCICE 6
static BOOL  g_dragging = FALSE;
static POINT g_startMouse = {};   // position souris (écran) au clic
static RECT  g_startWnd = {};   // rect fenêtre (écran) au clic
// EXERCICE 7
static int g_colorIndex = 0;
static HBRUSH g_bgBrush = NULL;

// Fonction de traitement des messages (callback)
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CLOSE:
        DestroyWindow(hwnd);  // Quand l’utilisateur ferme la fenêtre
        break;

    case WM_DESTROY:
        PostQuitMessage(0);   // Termine la boucle principale
        break;

    // EXERCICE 2
    case WM_SIZE: {
        windowWidth = LOWORD(lParam);
        windowHeight = HIWORD(lParam);
        wchar_t buff[128];
        wsprintf(buff, L"Fenetre - %d x %d", windowWidth, windowHeight);
        SetWindowText(hwnd, buff);
        break;
    }

    case WM_MOUSEMOVE: {
        // EXERCICE 3
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);

        if (x < windowWidth / 2) SetCursor(LoadCursor(NULL, IDC_HAND));
        else SetCursor(LoadCursor(NULL, IDC_CROSS)); 
        // EXERCICE 6
        if (g_dragging && (wParam & MK_LBUTTON)) {
            POINT cur;
            GetCursorPos(&cur);

            int dx = cur.x - g_startMouse.x;
            int dy = cur.y - g_startMouse.y;

            int newX = g_startWnd.left + dx;
            int newY = g_startWnd.top + dy;
            int w = g_startWnd.right - g_startWnd.left;
            int h = g_startWnd.bottom - g_startWnd.top;

            SetWindowPos(hwnd, NULL, newX, newY, w, h, SWP_NOZORDER | SWP_NOACTIVATE);
        }
        return 0;
    }

    case WM_KEYDOWN: {
        // EXERCICE 4
        if (wParam == VK_ESCAPE) {
            PostMessage(hwnd, WM_CLOSE, 0, 0);
            return 0;
        }
        //wchar_t keyName[64] = L"";
        //GetKeyNameText((LONG)lParam, keyName, 64);
        //wchar_t title[128];
        //wsprintf(title, L"Derniere touche: %s", keyName);
        //SetWindowText(hwnd, title);
        break;
    }
    case WM_LBUTTONDOWN: {
        // EXERCICE 5
        //int x = LOWORD(lParam);
        //int y = HIWORD(lParam);
        //wchar_t msgText[128];
        //wsprintf(msgText, L"Clic en (%d, %d)", x, y);
        //MessageBox(hwnd, msgText, L"Info", MB_OK);

        // EXERCICE 7
        g_colorIndex = (g_colorIndex + 1) % 3;
        COLORREF c = (g_colorIndex == 0) ? RGB(255, 0, 0) :
            (g_colorIndex == 1) ? RGB(0, 255, 0) : RGB(0, 0, 255);
        g_bgBrush = CreateSolidBrush(c);
        InvalidateRect(hwnd, NULL, TRUE);

        // EXERCICE 6
        g_dragging = TRUE;
        GetCursorPos(&g_startMouse);     
        GetWindowRect(hwnd, &g_startWnd);

        return 0;
    }
    // EXERCICE 6
    case WM_LBUTTONUP:
    {
        if (g_dragging) {
            g_dragging = FALSE;
        }
        return 0;
    }

    case WM_ERASEBKGND: {
        RECT rc; GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, g_bgBrush);
        return 1;
    }

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam); // Traitement par défaut
    }
    return 0;
}

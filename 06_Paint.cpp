#include <windows.h>

#define ID_COLOR_PICK  1001
#define ID_EDIT_CLEAR  1002

// Déclaration de la fonction de traitement des messages
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Point d'entrée de l'application Windows
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // 1️ Définir la classe de fenêtrex
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
        0, L"MainWindow", L"Paint", WS_OVERLAPPEDWINDOW,
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

// Globals
HDC     g_hMemDC = NULL;
HBITMAP g_hBmp = NULL;
HPEN    g_hPen = NULL;
COLORREF g_penColor = RGB(0, 0, 0);
POINT   g_lastPt = {};
bool    g_isDraw = false;

void RecreateBackbuffer(HWND hwnd, int w, int h) {
    HDC hdc = GetDC(hwnd);
    if (g_hMemDC) { SelectObject(g_hMemDC, GetStockObject(WHITE_BRUSH)); DeleteDC(g_hMemDC); g_hMemDC = NULL; }
    if (g_hBmp) { DeleteObject(g_hBmp); g_hBmp = NULL; }

    g_hMemDC = CreateCompatibleDC(hdc);
    g_hBmp = CreateCompatibleBitmap(hdc, w, h);
    SelectObject(g_hMemDC, g_hBmp);
    // fond blanc
    HBRUSH wbr = (HBRUSH)GetStockObject(WHITE_BRUSH);
    RECT rc = { 0,0,w,h };
    FillRect(g_hMemDC, &rc, wbr);
    ReleaseDC(hwnd, hdc);
}



// Fonction de traitement des messages (callback)
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CLOSE:
        DestroyWindow(hwnd);  // Quand l’utilisateur ferme la fenêtre
        break;

    case WM_CREATE: {
        // Crée le stylo noir de départ
        g_hPen = CreatePen(PS_SOLID, 2, g_penColor);

        // Crée un menu simple
        HMENU hMenu = CreateMenu();
        AppendMenu(hMenu, MF_STRING, ID_COLOR_PICK, TEXT("Choisir la couleur"));
        AppendMenu(hMenu, MF_STRING, ID_EDIT_CLEAR, TEXT("Effacer tout"));
        SetMenu(hwnd, hMenu);
        return 0;
    }

    case WM_SIZE:
        RecreateBackbuffer(hwnd, LOWORD(lParam), HIWORD(lParam));
        return 0;

    case WM_LBUTTONDOWN:
        g_isDraw = true;
        g_lastPt.x = LOWORD(lParam);
        g_lastPt.y = HIWORD(lParam);
        return 0;

    case WM_MOUSEMOVE:
        if (g_isDraw && (wParam & MK_LBUTTON)) {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            HPEN old = (HPEN)SelectObject(g_hMemDC, g_hPen);
            MoveToEx(g_hMemDC, g_lastPt.x, g_lastPt.y, NULL);
            LineTo(g_hMemDC, x, y);
            SelectObject(g_hMemDC, old);
            g_lastPt.x = x; g_lastPt.y = y;
            InvalidateRect(hwnd, NULL, FALSE);
        }
        return 0;

    case WM_LBUTTONUP:
        g_isDraw = false;
        return 0;

    case WM_COMMAND:
        if (LOWORD(wParam) == ID_COLOR_PICK) {
            CHOOSECOLOR cc = { sizeof(cc) };
            COLORREF cust[16] = {};
            cc.hwndOwner = hwnd;
            cc.lpCustColors = cust;
            cc.rgbResult = g_penColor;
            cc.Flags = CC_FULLOPEN | CC_RGBINIT;
            if (ChooseColor(&cc)) {
                g_penColor = cc.rgbResult;
                if (g_hPen) DeleteObject(g_hPen);
                g_hPen = CreatePen(PS_SOLID, 2, g_penColor);
            }
            return 0;
        }
        else if (LOWORD(wParam) == ID_EDIT_CLEAR) {
            RECT rc; GetClientRect(hwnd, &rc);
            HBRUSH wbr = (HBRUSH)GetStockObject(WHITE_BRUSH);
            FillRect(g_hMemDC, &rc, wbr);
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;
        }
        break;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        if (g_hMemDC) {
            RECT rc; GetClientRect(hwnd, &rc);
            BitBlt(hdc, 0, 0, rc.right, rc.bottom, g_hMemDC, 0, 0, SRCCOPY);
        }
        EndPaint(hwnd, &ps);
    } return 0;

    case WM_DESTROY: {
        if (g_hPen) DeleteObject(g_hPen);
        if (g_hBmp) DeleteObject(g_hBmp);
        if (g_hMemDC) DeleteDC(g_hMemDC);
        PostQuitMessage(0);
    }return 0;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam); // Traitement par défaut
    }
    return 0;
}

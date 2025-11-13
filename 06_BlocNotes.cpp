#include <windows.h>
#include <vector>
#include <string>
#define ID_FILE_NEW  3001
#define ID_FILE_OPEN 3002
#define ID_FILE_SAVE 3003
#define ID_FILE_QUIT 3004
#define ID_EDIT_MAIN 3005

HWND g_hEditMain = NULL;

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
        0, L"MainWindow", L"Bloc Notes", WS_OVERLAPPEDWINDOW,
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


void DoFileNew() {
    SetWindowText(g_hEditMain, L"");
}

bool DoFileOpen(HWND hwnd) {
    wchar_t path[MAX_PATH] = L"";
    OPENFILENAME ofn = { sizeof(ofn) };
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = L"Texte\0*.txt\0Tous\0*.*\0";
    ofn.lpstrFile = path;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    if (!GetOpenFileName(&ofn)) return false;

    // Charger le fichier
    HANDLE h = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return false;
    DWORD size = GetFileSize(h, NULL);
    std::vector<char> buf(size + 2, 0);
    DWORD rd = 0; 
    ReadFile(h, buf.data(), size, &rd, NULL);
    CloseHandle(h);

    // Conversion simple ANSI->UTF-16 si fichier ANSI
    int wlen = MultiByteToWideChar(CP_ACP, 0, buf.data(), rd, NULL, 0);
    std::wstring wbuf(wlen, L'\0');
    MultiByteToWideChar(CP_ACP, 0, buf.data(), rd, &wbuf[0], wlen);

    SetWindowText(g_hEditMain, wbuf.c_str());
    return true;
}

bool DoFileSave(HWND hwnd) {
    wchar_t path[MAX_PATH] = L"";
    OPENFILENAME ofn = { sizeof(ofn) };
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = L"Texte\0*.txt\0Tous\0*.*\0";
    ofn.lpstrFile = path;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_OVERWRITEPROMPT;
    if (!GetSaveFileName(&ofn)) return false;

    int len = GetWindowTextLength(g_hEditMain);
    std::wstring wbuf(len, L'\0');
    GetWindowText(g_hEditMain, &wbuf[0], len + 1);

    // Conversion UTF-16 -> ANSI simple
    int alen = WideCharToMultiByte(CP_ACP, 0, wbuf.c_str(), len, NULL, 0, NULL, NULL);
    std::vector<char> abuf(alen);
    WideCharToMultiByte(CP_ACP, 0, wbuf.c_str(), len, abuf.data(), alen, NULL, NULL);

    HANDLE h = CreateFile(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return false;
    DWORD wr = 0; WriteFile(h, abuf.data(), (DWORD)abuf.size(), &wr, NULL);
    CloseHandle(h);
    return true;
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


    case WM_CREATE: {
        g_hEditMain = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"",
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
            0, 0, 0, 0, hwnd, (HMENU)ID_EDIT_MAIN, GetModuleHandle(NULL), NULL);

        HMENU hMenuBar = CreateMenu();
        HMENU hMenuFile = CreatePopupMenu();

        AppendMenu(hMenuFile, MF_STRING, ID_FILE_NEW, L"Nouveau");
        AppendMenu(hMenuFile, MF_STRING, ID_FILE_OPEN, L"Ouvrir...");
        AppendMenu(hMenuFile, MF_STRING, ID_FILE_SAVE, L"Enregistrer...");
        AppendMenu(hMenuFile, MF_SEPARATOR, 0, NULL);
        AppendMenu(hMenuFile, MF_STRING, ID_FILE_QUIT, L"Quitter");

        AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hMenuFile, L"Fichier");

        // Attacher le menu à la fenêtre
        SetMenu(hwnd, hMenuBar);

        // --- Création de la zone de texte ---
        CreateWindowEx(
            WS_EX_CLIENTEDGE,
            L"EDIT",
            L"",
            WS_CHILD | WS_VISIBLE | WS_VSCROLL |
            ES_MULTILINE | ES_AUTOVSCROLL,
            0, 0, 0, 0,
            hwnd,
            (HMENU)ID_EDIT_MAIN,
            GetModuleHandle(NULL),
            NULL
        );
    }
    return 0;

    case WM_SIZE:
        MoveWindow(g_hEditMain, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
        return 0;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_FILE_NEW:  DoFileNew(); break;
        case ID_FILE_OPEN: DoFileOpen(hwnd); break;
        case ID_FILE_SAVE: DoFileSave(hwnd); break;
        case ID_FILE_QUIT: PostMessage(hwnd, WM_CLOSE, 0, 0); break;
        }
        return 0;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam); // Traitement par défaut
    }
    return 0;
}

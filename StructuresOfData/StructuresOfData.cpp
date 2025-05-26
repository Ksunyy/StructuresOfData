// StructuresOfData.cpp : Определяет точку входа для приложения.
//
#include <locale>
#include <codecvt>
#include "framework.h"
#include "StructuresOfData.h"
#include "Storage.h"
#include "Polynomials.h"
#include "Trans.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];
Storage storage;          
Parser parser; 




// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_STRUCTURESOFDATA, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_STRUCTURESOFDATA));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_STRUCTURESOFDATA));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_STRUCTURESOFDATA);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   HWND hMainWnd = FindWindow(szWindowClass, szTitle);

   // Добавим кнопки для отображения структур
   CreateWindow(L"BUTTON", L"Показать AVL", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
       440, 10, 260, 30, hMainWnd, (HMENU)1001, hInstance, nullptr);
   CreateWindow(L"BUTTON", L"Показать RB-дерево", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
       440, 45, 260, 30, hMainWnd, (HMENU)1002, hInstance, nullptr);
   CreateWindow(L"BUTTON", L"Показать  SortedTabl", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
       440, 80, 260, 30, hMainWnd, (HMENU)1003, hInstance, nullptr);
   CreateWindow(L"BUTTON", L"Показать  HashTable (OpenAdress)", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
       440, 115, 260, 30, hMainWnd, (HMENU)1004, hInstance, nullptr);
   CreateWindow(L"BUTTON", L"Показать  UnsortedTable", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
       440, 150, 260, 30, hMainWnd, (HMENU)1004, hInstance, nullptr);


   return TRUE;
}
HWND hEditInput;        // Поле ввода
HWND hEditOutput;       // Поле вывода
HWND hButtonDeelete;        // Кнопка удаления
HWND hButtonClear;      // Кнопка очистки
HWND hButtonShowList; // кнопка для отображения спсикаа

WNDPROC oldEditProc;

LRESULT CALLBACK EditSubclassProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_KEYDOWN && wParam == VK_RETURN)
    {
        wchar_t buffer[256];
        GetWindowTextW(hWnd, buffer, 256);

        std::wstring input(buffer);
        if (input.empty()) return 0; // пустая строка — ничего не делать

        // Конвертация wstring -> string с помощью wstring_convert
        std::string inputStr;
        {
            int size_needed = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, NULL, 0, NULL, NULL);
            std::string strTo(size_needed, 0);
            WideCharToMultiByte(CP_UTF8, 0, buffer, -1, &strTo[0], size_needed, NULL, NULL);
            inputStr = strTo;
            // Удаляем завершающий нулевой символ, если есть
            if (!inputStr.empty() && inputStr.back() == '\0') inputStr.pop_back();
        }

        try {
            parser.Parse(inputStr, storage);
            SetWindowTextW(hEditOutput, L"Успешно добавлено!");
        }
        catch (const std::exception& ex) {
            std::wstring errorMsg = L"Ошибка: ";
            errorMsg += std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(ex.what());
            SetWindowTextW(hEditOutput, errorMsg.c_str());
        }

        SetWindowTextW(hWnd, L""); // очистка поля

        return 0; // предотвращаем добавление перевода строки
    }

    return CallWindowProc(oldEditProc, hWnd, msg, wParam, lParam);
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {

        hButtonShowList = CreateWindowW(
            L"BUTTON", L"Показать список",
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            320, 80, 100, 25, hWnd, (HMENU)3, hInst, NULL
        );
        // Создаем поле ввода
        hEditInput = CreateWindowW(
            L"EDIT", L"",
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL | ES_WANTRETURN,
            10, 10, 300, 25, hWnd, (HMENU)1001, hInst, NULL
        );
        oldEditProc = (WNDPROC)SetWindowLongPtr(hEditInput, GWLP_WNDPROC, (LONG_PTR)EditSubclassProc);


        // Создаем поле вывода (многострочное с прокруткой)
        hEditOutput = CreateWindowW(
            L"EDIT", L"",
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | WS_VSCROLL | ES_READONLY,
            10, 45, 300, 200, hWnd, NULL, hInst, NULL
        );

        // Создаем кнопки
        hButtonDeelete = CreateWindowW(
            L"BUTTON", L"Удалить",
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            320, 10, 100, 25, hWnd, (HMENU)1, hInst, NULL
        );

        hButtonClear = CreateWindowW(
            L"BUTTON", L"Найти",
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            320, 45, 100, 25, hWnd, (HMENU)2, hInst, NULL
        );
    }
    break;

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        int notificationCode = HIWORD(wParam);

        
        switch (wmId)
        {
        case 1: // Кнопка "Удалить"
        {
            wchar_t buffer[256];
            GetWindowTextW(hEditInput, buffer, 256);
            if (wcslen(buffer) == 0) {
                SetWindowTextW(hEditOutput, L"Введите ключ для удаления.");
                break;
            }

            // Конвертация wide -> UTF-8 string
            int size_needed = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, NULL, 0, NULL, NULL);
            std::string key(size_needed, 0);
            WideCharToMultiByte(CP_UTF8, 0, buffer, -1, &key[0], size_needed, NULL, NULL);
            if (!key.empty() && key.back() == '\0') key.pop_back();

            try {
                if (storage.find(key)) {
                    storage.erase(key);
                    SetWindowTextW(hEditOutput, L"Успешно удалено.");
                }
                else {
                    SetWindowTextW(hEditOutput, L"Ключ не найден.");
                }
            }
            catch (const std::exception& ex) {
                std::wstring errorMsg = L"Ошибка при удалении: ";
                errorMsg += std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(ex.what());
                SetWindowTextW(hEditOutput, errorMsg.c_str());
            }

            SetWindowTextW(hEditInput, L""); // очистка поля
        }
        break;


        case 2: // Кнопка "Найти"
        {
            wchar_t buffer[256];
            GetWindowTextW(hEditInput, buffer, 256);

            if (wcslen(buffer) == 0) {
                SetWindowTextW(hEditOutput, L"Введите ключ для поиска.");
                break;
            }

            // Конвертация wide -> UTF-8 string
            int size_needed = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, NULL, 0, NULL, NULL);
            std::string key(size_needed, 0);
            WideCharToMultiByte(CP_UTF8, 0, buffer, -1, &key[0], size_needed, NULL, NULL);
            if (!key.empty() && key.back() == '\0') key.pop_back();

            try {
                if (storage.find(key)) {
                    Polynomials poly = storage.get(key);
                    std::string result = key + ": " + poly.to_string();

                    // Перевод результата обратно в wstring
                    std::wstring wresult = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(result);
                    SetWindowTextW(hEditOutput, wresult.c_str());
                }
                else {
                    SetWindowTextW(hEditOutput, L"Ключ не найден.");
                }
            }
            catch (const std::exception& ex) {
                std::wstring errorMsg = L"Ошибка при поиске: ";
                errorMsg += std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(ex.what());
                SetWindowTextW(hEditOutput, errorMsg.c_str());
            }

            SetWindowTextW(hEditInput, L"");
        }
        break;


        case 3: // Кнопка "Показать список"
        {
            std::string output = storage.print();
            std::wstring woutput = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(output);
            SetWindowTextW(hEditOutput, woutput.c_str());
        }
        break;

        case 1001:
        {
            std::string output = storage.printAVL();
            std::wstring woutput = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(output);
            SetWindowTextW(hEditOutput, woutput.c_str());
            break;
        }
        case 1002:
        {
            std::string output = storage.printRB();
            std::wstring woutput = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(output);
            SetWindowTextW(hEditOutput, woutput.c_str());
            break;
        }
        case 1003:
        {
            std::string output = storage.printSortedTable();
            std::wstring woutput = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(output);
            SetWindowTextW(hEditOutput, woutput.c_str());
            break;
        }
        case 1004:
        {
            std::string output = storage.printTable();
            std::wstring woutput = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(output);
            SetWindowTextW(hEditOutput, woutput.c_str());
            break;
        }
        case 1005:
        {
            std::string output = storage.HashTableOpenPrint();
            std::wstring woutput = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(output);
            SetWindowTextW(hEditOutput, woutput.c_str());
            break;
        }

        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;

        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;


        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }

   
    }
    break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // Здесь можно добавить дополнительную отрисовку
        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

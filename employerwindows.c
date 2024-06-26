#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define IDC_IN_EDIT     101
#define IDC_OUT_EDIT    102
#define IDC_PRICE_EDIT  103
#define IDC_RESULT_EDIT 104
#define IDC_CALC_BUTTON 105
//gcc main.c -o employer_application -mwindows
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void Calculate(HWND hwnd);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    const char CLASS_NAME[] = "EmployerWindowClass";
    WNDCLASS wc = { 0 };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    
    wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 0));
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Employer Application",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
    {
        CreateWindow("STATIC", "In (hh:mm):", WS_VISIBLE | WS_CHILD, 10, 10, 100, 20, hwnd, NULL, NULL, NULL);
        CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 120, 10, 100, 20, hwnd, (HMENU)IDC_IN_EDIT, NULL, NULL);

        CreateWindow("STATIC", "Out (hh:mm):", WS_VISIBLE | WS_CHILD, 10, 40, 100, 20, hwnd, NULL, NULL, NULL);
        CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 120, 40, 100, 20, hwnd, (HMENU)IDC_OUT_EDIT, NULL, NULL);

        CreateWindow("STATIC", "Price:", WS_VISIBLE | WS_CHILD, 10, 70, 100, 20, hwnd, NULL, NULL, NULL);
        CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 120, 70, 100, 20, hwnd, (HMENU)IDC_PRICE_EDIT, NULL, NULL);

        CreateWindow("BUTTON", "=", WS_VISIBLE | WS_CHILD, 10, 100, 50, 20, hwnd, (HMENU)IDC_CALC_BUTTON, NULL, NULL);

        CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_READONLY, 10, 130, 360, 120, hwnd, (HMENU)IDC_RESULT_EDIT, NULL, NULL);
        break;
    }
    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_CALC_BUTTON)
        {
            Calculate(hwnd);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

void Calculate(HWND hwnd)
{
    char in_text[6], out_text[6], price_text[20];
    GetDlgItemText(hwnd, IDC_IN_EDIT, in_text, sizeof(in_text));
    GetDlgItemText(hwnd, IDC_OUT_EDIT, out_text, sizeof(out_text));
    GetDlgItemText(hwnd, IDC_PRICE_EDIT, price_text, sizeof(price_text));

    int in_hour, in_min, out_hour, out_min;
    float price, total_value;

    if (sscanf(in_text, "%d:%d", &in_hour, &in_min) != 2 ||
        sscanf(out_text, "%d:%d", &out_hour, &out_min) != 2 ||
        sscanf(price_text, "%f", &price) != 1)
    {
        MessageBox(hwnd, "Invalid input!", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    int total_in_minutes = in_hour * 60 + in_min;
    int total_out_minutes = out_hour * 60 + out_min;
    int diff_minutes = total_out_minutes - total_in_minutes;

    if (diff_minutes < 0)
    {
        MessageBox(hwnd, "Out time must be after In time!", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    float hours = diff_minutes / 60.0f;
    total_value = hours * price;

    char result[256];
    sprintf(result, "Out - In = %.2f hours * %.2f = %.2f", hours, price, total_value);

    SetDlgItemText(hwnd, IDC_RESULT_EDIT, result);
}


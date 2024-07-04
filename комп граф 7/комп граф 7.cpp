// комп граф 7.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "комп граф 7.h"
#include <stdio.h>
#include <stdlib.h>
#include "math.h"

#define MAX_LOADSTRING 100
// Глобальные переменные:
bool cube = 0, kRotation = 0;
double vertices[20][3], sides[20][4];//Матрицы с данными о вершинах и сторонах
double angle = 0, rotationspeed; //Угол и скорость поворота куба
int vertcount=0, sidcount=0, scale = 150;//Количество вершин, сторон и масштаб
int X = 500, Y = 300; //Расположение куба
int xPos0 = 0, yPos0 = 0, xPos1 = 0, yPos1 = 0;//Координаты нажатия и отжатия ЛКМ
double mtrx2[100][3], mtrx1[100][3]; //Вспомогательные матрицы для построения куба
void CreateCube(HWND hWnd);//Считывание данных о кубе
void PrintCube(HWND hWnd, HDC hDC);//Рисование куба
void Rotation(HWND hWnd, HDC hDC, double matrix2d[][2]);//Вращение куба
void MatrixMultiplication(double mat1[][3], double mat2[][2]); 
void MatrixMultiplication(double mat1[][3], double mat2[][3]);//Умножение матриц

HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

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

    // TODO: разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY7, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY7));

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

    return (int)msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY7));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MY7);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        100, 100, 1000, 720, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND — обработать меню приложения
//  WM_PAINT — отрисовать главное окно
//  WM_DESTROY — отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_MOUSEWHEEL://При прокрутке колеса мыши
        int wheel;//Прокрутка колеса
        wheel = GET_WHEEL_DELTA_WPARAM(wParam);
            scale += wheel / 30;//Изменение масштаба
            if (scale < 0)
                scale = 0;
        InvalidateRect(hWnd, NULL, true);
        break;
    case WM_LBUTTONDOWN://При нажатии ЛКМ
    {
        xPos0 = LOWORD(lParam);
        yPos0 = HIWORD(lParam);
        break;
    }
    case WM_LBUTTONUP: //При отжати ЛКМ
    {
        xPos1 = LOWORD(lParam);
        yPos1 = HIWORD(lParam);
            if ((xPos0 < X + scale) && (xPos0 > X - scale) && (yPos0 < Y + scale) && (yPos0 > Y - scale))//Нажатие по кубу
            {
                X = xPos1;
                Y = yPos1;
                InvalidateRect(hWnd, NULL, true);//Перемещение
            }
            else//Нажатие не по кубу
            {
                int dx = xPos1 - xPos0;
                    if (dx > 0)
                    {
                        rotationspeed = dx / 20;
                    }
                    else
                    {
                        rotationspeed = -dx / 20;
                    }
                kRotation = 1;
                InvalidateRect(hWnd, NULL, true);//Вращение
            }
        break;
    }
    case WM_CREATE:
    {

    }
    case WM_COMMAND:
    {
        if(cube==false)
        CreateCube(hWnd);
        int wmId = LOWORD(wParam);
        // Разобрать выбор в меню:
        switch (wmId)
        {
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
        HDC hDC = BeginPaint(hWnd, &ps);
        double mtrx2d[3][2] = { { 0.86 , 0.5 },{ -0.86,0.5 },{ 0,-1 } }; //Перевод в двурмерные координаты
        if (kRotation==1)//Перерисовка под углом
        {
            Rotation(hWnd,hDC, mtrx2d);
        }
        else//Рисование куба
        {
            MatrixMultiplication(vertices, mtrx2d);
            PrintCube(hWnd, hDC);
        }
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
void PrintCube(HWND hWnd, HDC hDC)//Рисует куб
{
    for (int i = 0; i < sidcount; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (j == 3)
            {
                int x1 = (int)(mtrx1[(int)sides[i][j] - 1][0] * scale);
                int y1 = (int)(mtrx1[(int)sides[i][j] - 1][1] * scale);
                int x2 = (int)(mtrx1[(int)sides[i][0] - 1][0] * scale);
                int y2 = (int)(mtrx1[(int)sides[i][0] - 1][1] * scale);
                MoveToEx(hDC, X + x1, Y + y1, NULL);
                LineTo(hDC, X + x2, Y + y2);
            }
            else
            {
                int x1 = (int)(mtrx1[(int)sides[i][j] - 1][0] * scale);
                int y1 = (int)(mtrx1[(int)sides[i][j] - 1][1] * scale);
                int x2 = (int)(mtrx1[(int)sides[i][j + 1] - 1][0] * scale);
                int y2 = (int)(mtrx1[(int)sides[i][j + 1] - 1][1] * scale);
                MoveToEx(hDC, X + x1, Y + y1, NULL);
                LineTo(hDC, X + x2, Y + y2);
            }
        }
    }
}
void MatrixMultiplication(double A[][3], double B[][2])//Умножение матриц
{
    for (int i = 0; i < vertcount; i++)
        for (int j = 0; j < 2; j++)
            mtrx1[i][j] = 0;

    for (int i = 0; i < vertcount; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < 3; k++)
                mtrx1[i][j] += A[i][k] * B[k][j];
}

void MatrixMultiplication(double A[][3], double B[][3])//Умножение матриц
{
    for (int i = 0; i < vertcount; i++)
        for (int j = 0; j < 3; j++)
            mtrx1[i][j] = 0;

    for (int i = 0; i < vertcount; i++)
        for (int j = 0; j < 3; j++)
            for (int k = 0; k < 3; k++)
                mtrx1[i][j] += A[i][k] * B[k][j];
}
void CreateCube(HWND hWnd)//Создание куба
{
    int digit, counter;
    bool minus = 0;
    char storage[255];
    FILE* Cube;
    Cube = fopen("Cube.obj", "r");//Открытие файла
    if(Cube==NULL)
        ::MessageBox(hWnd, L"Не удалось открыть файл", L"Ошибка", MB_OK);
    else
    {
        while (fgets(storage, 255, Cube))
        {
            if (storage[0] == 'v')
            {
                counter = 2;
                for (int j = 0; j < 3; j++)
                {
                    digit = 0;
                    if (storage[counter] == '-')
                    {
                        minus = 1;
                        counter++;
                    }
                    else minus = 0;
                    for (int i = counter; (storage[i] != '.') && (storage[i] != ' ') && (storage[i] != '\0'); i++)//Узнаём разряд числа
                        digit++;
                    for (counter; (storage[counter] != '\0') && (storage[counter] != ' '); counter++)
                    {
                        if (storage[counter] != '.')
                        {
                            if (minus)
                                vertices[vertcount][j] -= (storage[counter] - '0') * pow(10, digit - 1);
                            else
                                vertices[vertcount][j] += (storage[counter] - '0') * pow(10, digit - 1);
                            digit--;
                        }//Считываем число
                    }
                    counter++;
                }
                vertcount++;//Увеличиваем количество вершин
            }
            if (storage[0] == 'f')
            {
                counter = 2;
                for (int j = 0; j < 4; j++)
                {
                    digit = 0;
                    if (storage[counter] == '-')
                    {
                        minus = 1;
                        counter++;
                    }
                    else minus = 0;


                    for (int i = counter; (storage[i] != '.') && (storage[i] != ' ') && (storage[i] != '\0'); i++)//Узнаем разряд числа
                        digit++;
                    for (counter; (storage[counter] != '\0') && (storage[counter] != ' '); counter++)
                    {
                        if (storage[counter] != '.')
                        {
                            if (minus)
                                sides[sidcount][j] -= (storage[counter] - '0') * pow(10, digit - 1);
                            else
                                sides[sidcount][j] += (storage[counter] - '0') * pow(10, digit - 1);
                            digit--;
                        }//Считываем число
                    }
                    counter++;
                }
                sidcount++;//Увеличиваем количество сторон
            }
        }
        fclose(Cube);//Закрытие файла
        cube = true;
        InvalidateRect(hWnd, 0, true);
    }
}

void Rotation(HWND hWnd,HDC hDC, double  matrix2d[][2])//Вращение куба
{
    double Xrotation[3][3] = { {cos(angle), sin(angle),0},{-sin(angle),cos(angle),0},{0,0,1} };
    double Zrotation[3][3] = { {cos(angle), sin(angle),0},{-sin(angle),cos(angle),0},{0,0,1} }; 
    MatrixMultiplication(vertices, Zrotation);//Вращение матрицы вокруг оси Z
    for (int i = 0; i < 50; i++)
        for (int j = 0; j < 3; j++)
            mtrx2[i][j] = mtrx1[i][j];
    MatrixMultiplication(mtrx2, matrix2d);
    for (int i = 1; i <= sidcount / 4; i++)
    {
        PrintCube(hWnd, hDC);//Перерисовка куба
    }
    angle += (double)rotationspeed / (double)180 * 3.14;//Угол меняется в зависимости от скорости
    rotationspeed -= 0.5; //Уменьшение скорости
    if (rotationspeed > 0)
    {
        Sleep(50);
        InvalidateRect(hWnd, 0, true);
    }
    else
        rotationspeed = 0;
}
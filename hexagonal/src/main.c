/**
 * TODO:
 * цветовая схема границ в зависимости от владельца, возможно массивом из структур вроде Struct owner
 * не рисовать границу клетки, а только разделительную линию, если она того же владельца;
 * сначала рисовать чёрную сетку границ клеток, а потом уже цветные границы,
 * возможно их придётся рисовать каждую отдельным GL_LINE
 */

#include "header.h"

//#define _USE_MATH_DEFINES

#define wndWidth  512
#define wndHeight 512
#define lineHalfWidth 0.003f
#define SPLASH_TIMER 1

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

void DrawHexCells();//(Cell**, int, int, GLfloat, GLfloat);
//void GenerateHexCells();// (Cell**, int, int, GLfloat, GLfloat);
void PrivateArea(struct Globals*);

Globals  *G;

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;

    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "civ-like";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "civ-like",
                          "civ-like",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          wndWidth,
                          wndHeight,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);

    //tmp1 = tmp2 = 0;

    G = (struct Globals*) malloc(sizeof(Globals));
    *G = (Globals) {
        .tmp1 = 5,
        .tmp2 = 6,
    };

    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) //вохможно, достаточно GetMessage
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg); /* буквально переводит msg, повышая уровень абстрации виртуальных кодов */
                DispatchMessage(&msg); /* отправляет msg в WindowProc*/
            }
        }
        else
        {
            /* OpenGL animation code goes here */
            //GenerateHexCells (cells, rows, columns, R, R_sin);
            //GenerateHexCells();
            DrawHexCells(); //(cells, rows, columns, R, R_sin);
            if (G->cells != 0){
                //Sleep(10);
                ChangeColors(G);
            }
            SwapBuffers(hDC);
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    dynamic_array_free(G->cells, G->rows);
    free(G->owners);
    free(G);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}


void PrivateArea(Globals *G)
{
    int i, j;

    i = G->tmp1;j = G->tmp2;

    /*поробуем обратиться к соседям, типа город*/

//    cells[i][j].up.n_link->owner = cells[i][j].owner;

    if (G->cells[i][j].up.n_link)
        G->cells[i][j].up.n_link->owner = G->cells[i][j].owner; //cells + (i-1)*j;
    if (G->cells[i][j].upright.n_link)
        G->cells[i][j].upright.n_link->owner = G->cells[i][j].owner;
    if (G->cells[i][j].downright.n_link)
        G->cells[i][j].downright.n_link->owner = G->cells[i][j].owner;
    if (G->cells[i][j].down.n_link)
        G->cells[i][j].down.n_link->owner = G->cells[i][j].owner;
    if (G->cells[i][j].downleft.n_link)
        G->cells[i][j].downleft.n_link->owner = G->cells[i][j].owner;
    if (G->cells[i][j].upleft.n_link)
        G->cells[i][j].upleft.n_link->owner = G->cells[i][j].owner;

}



void DrawHexCells() //(Cell **cells, int rows, int columns, GLfloat R, GLfloat R_sin)
{
    /**
      *  Для построения гексагональной сетки:
      *  Задать радиус
      *  cмещение треугольника по горизонтали: 1/2 R,
      *  смещение центра гексы половинного ряда относительно главного 1.5 R
      *  смещение треугольника по высоте R sin 60
      *  смещение центра гексы по высоте 2 R sin 60
      */

    /* проверка выделения памяти под массив*/
    if (G->cells == 0)
        return;

    int i, j;

    /*
    int t = 0b11;
    char str[20];
    sprintf(str, "%i", t);
    MessageBox(NULL, str, NULL, MB_OK);
    */
    //цвет фона
    //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    for (i = 0; i < G->rows; i++)
        for(j = 0; j < G->columns; j++){
            //glColor3f(cells[i][j].bg_color_r, cells[i][j].bg_color_g, cells[i][j].bg_color_b);
            glColor3f(G->cells[i][j].bg_color_r, G->cells[i][j].bg_color_g, G->cells[i][j].bg_color_b);
            //glColor3f(G->cells[i][j].owner->bg_color_r, G->cells[i][j].owner->bg_color_g, G->cells[i][j].owner->bg_color_b);

            glBegin(GL_POLYGON); //рисуем фон гексы
            //слева сверху и по часовой стрелке
            glVertex2f(G->cells[i][j].x - (G->R / 2)  + lineHalfWidth, G->cells[i][j].y + G->R_sin  - lineHalfWidth);
            glVertex2f(G->cells[i][j].x +  G->R / 2   - lineHalfWidth, G->cells[i][j].y + G->R_sin  - lineHalfWidth);
            glVertex2f(G->cells[i][j].x +  G->R       - lineHalfWidth, G->cells[i][j].y);
            glVertex2f(G->cells[i][j].x +  G->R / 2   - lineHalfWidth, G->cells[i][j].y - G->R_sin  + lineHalfWidth);
            glVertex2f(G->cells[i][j].x - (G->R / 2)  + lineHalfWidth, G->cells[i][j].y - G->R_sin  + lineHalfWidth);
            glVertex2f(G->cells[i][j].x -  G->R       + lineHalfWidth, G->cells[i][j].y);
            glEnd();

            /* рисуем разметочную сетку */
            glColor3f(0, 0, 0); //цвет линии
            glLineWidth(1);
            glBegin(GL_LINE_LOOP);
            /* слева сверху и по часовой стрелке */
            glVertex2f(G->cells[i][j].x - (G->R / 2), G->cells[i][j].y + G->R_sin);
            glVertex2f(G->cells[i][j].x +  G->R / 2 , G->cells[i][j].y + G->R_sin);
            glVertex2f(G->cells[i][j].x +  G->R     , G->cells[i][j].y);
            glVertex2f(G->cells[i][j].x +  G->R / 2 , G->cells[i][j].y - G->R_sin);
            glVertex2f(G->cells[i][j].x - (G->R / 2), G->cells[i][j].y - G->R_sin);
            glVertex2f(G->cells[i][j].x -  G->R     , G->cells[i][j].y);
            glEnd();

            glColor3f(G->cells[i][j].owner->border_color_r, G->cells[i][j].owner->border_color_g, G->cells[i][j].owner->border_color_b); //цвет линии
            glLineWidth(2);

            //if ()

            /*границы гексы если надо */
/*
            glBegin(GL_LINE_LOOP);
            //слева сверху и по часовой стрелке
            glVertex2f(cells[i][j].x - (R / 2)  + lineHalfWidth, cells[i][j].y + R_sin  - lineHalfWidth);
            glVertex2f(cells[i][j].x +  R / 2   - lineHalfWidth, cells[i][j].y + R_sin  - lineHalfWidth);
            glVertex2f(cells[i][j].x +  R       - lineHalfWidth, cells[i][j].y);
            glVertex2f(cells[i][j].x +  R / 2   - lineHalfWidth, cells[i][j].y - R_sin  + lineHalfWidth);
            glVertex2f(cells[i][j].x - (R / 2)  + lineHalfWidth, cells[i][j].y - R_sin  + lineHalfWidth);
            glVertex2f(cells[i][j].x -  R       + lineHalfWidth, cells[i][j].y);
            glEnd();
*/
/*
            glBegin(GL_POINTS);
            glVertex2f(G->cells[i][j].x, G->cells[i][j].y);
            glEnd();
*/

        }
    //fps
    Sleep (30);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    switch (uMsg)
    {
        case WM_CREATE:
            SetTimer(hwnd, SPLASH_TIMER, 200, NULL);
        break;

        case WM_CLOSE:
            KillTimer(hwnd, SPLASH_TIMER);
            PostQuitMessage(0);
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                break;

                case VK_SPACE:
                    GenerateHexCells(G);
                break;

                case VK_RETURN:
                    //PrivateArea(G);
                    Splash(&G->cells[G->tmp1][G->tmp2]);
                break;

                case VK_UP:
                    if (G->tmp1 > 0)
                        G->tmp1--;
                    GenerateHexCells(G);
                break;

                case VK_DOWN:
                    if (G->tmp1 < G->rows-1)
                        G->tmp1++;
                    GenerateHexCells(G);
                break;

                case VK_LEFT:
                    if (G->tmp2 > 0)
                        G->tmp2--;
                    GenerateHexCells(G);
                break;

                case VK_RIGHT:
                    if (G->tmp2 < G->columns-1)
                        G->tmp2++;
                    GenerateHexCells(G);
                break;
            }
        }
        break;

        case WM_TIMER:
        {
            switch (wParam)
            {
                case SPLASH_TIMER:
                    if (G->cells != 0)
                        Splash(&G->cells[G->tmp1][G->tmp2]);
                break;
            }
        }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}


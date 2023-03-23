#include "header.h"

/**  ___     ___
  * /0,0\___/0,2\
  * \___/0,1\___/
  * /1,0\___/1,2\
  * \___/1,1\___/
  *     \___/
  */


void GenerateHexCells(struct Globals *G) // (Cell **cells, int rows, int columns, GLfloat R, GLfloat R_sin)
{
    int i, j;
    G->rows = 11; G->columns = 13; G->player_ids = 1 + 1; /* один это варвары (и все прочие игроки в будущем), другой это игрок */
    G->R = 0.1; G->R_sin = G->R * sin(M_PI/3);
    //base_x = base_y = -0.5;
    GLfloat base_x = - 1 + G->R,
            base_y = 1 - G->R_sin - 0.01;
    G->cells = dynamic_array_alloc(G->rows, G->columns);
    G->owners = (Owner*) malloc((G->player_ids + 1)*sizeof(Owner));
    SetCellsDefault(G);

    for(i = 0; i <= G->player_ids; i++)
    {
        G->owners[i].id = i;
        G->owners[i].border_color_r = G->owners[i].border_color_b = 0;
        G->owners[i].border_color_g =  G->owners[i].bg_color_g = 1;
        G->owners[i].bg_color_r = G->owners[i].bg_color_b = 0;
    }

    G->owners[2].border_color_r = 0.25;
    G->owners[2].border_color_g = 0.87;
    G->owners[2].border_color_b = 0.81;

    G->owners[2].bg_color_r = 0.25;
    G->owners[2].bg_color_g = 0.87;
    G->owners[2].bg_color_b = 0.81;

    // временно
    G->owners[0].bg_color_r = 0;
    G->owners[0].bg_color_g = 1;
    G->owners[0].bg_color_b = 0;


    // верхние ячейки строки
    for (i = 0; i < G->rows; i += 1)
        for(j = 0; j < G->columns; j += 2){
            G->cells[i][j].index_row = i; G->cells[i][j].index_column = j;
            G->cells[i][j].x = base_x + 3 * (j / 2) * G->R;
            G->cells[i][j].y = base_y - 2 * i * G->R_sin;
            /*сверху и по часовой*/
            if (i > 0)
                G->cells[i][j].up.n_link = &G->cells[i-1][j]; //cells + (i-1)*j;
            if ((i > 0) && (j < G->columns-1))
                G->cells[i][j].upright.n_link = &G->cells[i-1][j+1];
            if (j < G->columns-1)
                G->cells[i][j].downright.n_link = &G->cells[i][j+1];
            if (i < G->rows-1)
                G->cells[i][j].down.n_link = &G->cells[i+1][j];
            if (j > 0)
                G->cells[i][j].downleft.n_link = &G->cells[i][j-1];
            if ((i > 0) && (j > 0))
                G->cells[i][j].upleft.n_link = &G->cells[i-1][j-1];


            /*
            cells[i][j].border_color_r = 0.25;
            cells[i][j].border_color_g = 0.87;
            cells[i][j].border_color_b = 0.81;
            cells[i][j].bg_color_r = 0;
            cells[i][j].bg_color_g = 1;
            cells[i][j].bg_color_b = 0;
            */
        }
    // нижние ячейки строки
    for (i = 0; i < G->rows; i += 1)
        for(j = 1; j < G->columns; j += 2){
            G->cells[i][j].index_row = i; G->cells[i][j].index_column = j;
            G->cells[i][j].x = (base_x + 1.5 * G->R) +  3 * (j / 2) * G->R;
            G->cells[i][j].y = (base_y - G->R_sin) - 2 * i* G->R_sin;

            /**
              * Здесь не учитывается бесконечность по широте
              * так же нужен будет симметричный телепорт у полюсов
              */
            if (i > 0)
                G->cells[i][j].up.n_link = &G->cells[i-1][j];
            if (j < G->columns-1)
                G->cells[i][j].upright.n_link = &G->cells[i][j+1];
            if ((i < G->rows-1) && (j < G->columns-1))
                G->cells[i][j].downright.n_link = &G->cells[i+1][j+1];
            if (i < G->rows-1)
                G->cells[i][j].down.n_link = &G->cells[i+1][j];
            if ((i < G->rows-1) && (j > 0))
                G->cells[i][j].downleft.n_link = &G->cells[i+1][j-1];
            if (j > 0)
                G->cells[i][j].upleft.n_link = &G->cells[i][j-1];
        }

        for (i = 0; i < G->rows; i++)
            for(j = 0; j < G->columns; j++){
                G->cells[i][j].bg_color_r = 0;
                G->cells[i][j].bg_color_g = 1.00f;
                G->cells[i][j].bg_color_b = 0;
                G->cells[i][j].delta = 0.05f;

                G->cells[i][j].r_t = 0;
                G->cells[i][j].g_t = 0;
                G->cells[i][j].b_t = 0;
            }

        G->cells[G->tmp1][G->tmp2].owner = G->owners + 2;

/*
                char str[20];
                sprintf(str, "%i", cells[tmp1][tmp2].upright.n_link);
                MessageBox(NULL, str, NULL, MB_OK);

                MessageBox(NULL, "ПУК", NULL, MB_OK);
*/
}

void SetCellsDefault(struct Globals *G)
{
    int i, j;
    for(i = 0; i < G->rows; i++)
        for(j = 0; j < G->columns; j++){
            //cells[i][j].owner = owners; //первый элемент массива
            G->cells[i][j] = (Cell) {
                .owner = G->owners, /* первый элемент массива */
                .params = 0,
                .landscape = 0,
                .resources = 0,
                .infrastructure = 0,
                .up = {
                    .n_link = NULL,
                    .neighbour_params = 0,
                    .weight = 0
                },
                .upright = {
                    .n_link = NULL,
                    .neighbour_params = 0,
                    .weight = 0
                },
                .downright = {
                    .n_link = NULL,
                    .neighbour_params = 0,
                    .weight = 0
                },
                .down = {
                    .n_link = NULL,
                    .neighbour_params = 0,
                    .weight = 0
                },
                .downleft = {
                    .n_link = NULL,
                    .neighbour_params = 0,
                    .weight = 0
                },
                .upleft = {
                    .n_link = NULL,
                    .neighbour_params = 0,
                    .weight = 0
                }
            };
        }
}


Cell ** dynamic_array_alloc(size_t N, size_t M)
{


    Cell **A = (Cell **)malloc(N*sizeof(Cell *));
    for(int i = 0; i < N; i++)
        A[i] = (Cell *)malloc(M*sizeof(Cell));
    return A;
}

void dynamic_array_free(Cell **A, size_t N)
{
    if (A == 0)
        return;
    for(int i = 0; i < N; i++) {
        free(A[i]);
    }
    free(A);
}

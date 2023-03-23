#include "header.h"


void Splash(Cell* cell)
{
    Cell *runner;
    runner = cell;


    /* звезда
    runner = cell;
    while (runner->up.n_link){
        if (runner->up.n_link->owner != runner->owner){
            runner->up.n_link->owner  = runner->owner;
            break;
        }else runner = runner->up.n_link;
    }

    runner = cell;
    while (runner->upright.n_link){
        if (runner->upright.n_link->owner != runner->owner){
            runner->upright.n_link->owner  = runner->owner;
            //Splash(runner);
            break;
        }else runner = runner->upright.n_link;
    }

    runner = cell;
    while (runner->downright.n_link){
        if (runner->downright.n_link->owner != runner->owner){
            runner->downright.n_link->owner  = runner->owner;
            //Splash(runner);
            break;
        }else runner = runner->downright.n_link;
    }

    runner = cell;
    while (runner->down.n_link){
        if (runner->down.n_link->owner != runner->owner){
            runner->down.n_link->owner  = runner->owner;
            break;
        }else runner = runner->down.n_link;
    }

    runner = cell;
    while (runner->downleft.n_link){
        if (runner->downleft.n_link->owner != runner->owner){
            runner->downleft.n_link->owner  = runner->owner;
            break;
        }else runner = runner->downleft.n_link;
    }

    runner = cell;
    while (runner->upleft.n_link){
        if (runner->upleft.n_link->owner != runner->owner){
            runner->upleft.n_link->owner  = runner->owner;
            break;
        }else runner = runner->upleft.n_link;
    }
    */
}

void ChangeColors(Globals *G)
{
    int i, j;
    GLfloat min = 0, max = 1, delta = 0.05f;

    for (i = 0; i < G->rows; i++)
        for(j = 0; j < G->columns; j++){
            if (G->cells[i][j].owner != &G->owners[2])
                continue;


            if ((G->cells[i][j].bg_color_r >= max) &&
                (G->cells[i][j].bg_color_g <= min) &&
                (G->cells[i][j].bg_color_b <= min))
            {

                G->cells[i][j].r_t = 0;
                G->cells[i][j].g_t = delta;
                G->cells[i][j].b_t = 0;
            }

            if ((G->cells[i][j].bg_color_r >= max) &&
                (G->cells[i][j].bg_color_g >= max) &&
                (G->cells[i][j].bg_color_b <= min))
            {
                G->cells[i][j].r_t = -delta;
                G->cells[i][j].g_t = 0;
                G->cells[i][j].b_t = 0;
            }

            if ((G->cells[i][j].bg_color_r <= min) &&
                (G->cells[i][j].bg_color_g >= max) &&
                (G->cells[i][j].bg_color_b <= min))
            {
                G->cells[i][j].r_t = 0;
                G->cells[i][j].g_t = 0;
                G->cells[i][j].b_t = delta;
            }

            if ((G->cells[i][j].bg_color_r <= min) &&
                (G->cells[i][j].bg_color_g >= max) &&
                (G->cells[i][j].bg_color_b >= max))
            {
                G->cells[i][j].r_t = 0;
                G->cells[i][j].g_t = -delta;
                G->cells[i][j].b_t = 0;
            }
            if ((G->cells[i][j].bg_color_r <= min) &&
                (G->cells[i][j].bg_color_g <= min) &&
                (G->cells[i][j].bg_color_b >= max))
            {
                G->cells[i][j].r_t = delta;
                G->cells[i][j].g_t = 0;
                G->cells[i][j].b_t = 0;
            }

            if ((G->cells[i][j].bg_color_r >= max) &&
                (G->cells[i][j].bg_color_g <= min) &&
                (G->cells[i][j].bg_color_b >= max))
            {
                G->cells[i][j].r_t = 0;
                G->cells[i][j].g_t = 0;
                G->cells[i][j].b_t = -delta;
            }

            G->cells[i][j].bg_color_r += G->cells[i][j].r_t;
            G->cells[i][j].bg_color_g += G->cells[i][j].g_t;
            G->cells[i][j].bg_color_b += G->cells[i][j].b_t;

            /*хуита
            if(G->cells[i][j].bg_color_b < 1)
                    G->cells[i][j].bg_color_b += G->cells[i][j].delta;
            else if(G->cells[i][j].bg_color_g < 1){
                    G->cells[i][j].bg_color_b = 0.5f;
                    G->cells[i][j].bg_color_g += G->cells[i][j].delta;
                    }else if (G->cells[i][j].bg_color_r < 1){
                                G->cells[i][j].bg_color_g = 0.5f;
                                G->cells[i][j].bg_color_r += G->cells[i][j].delta;
                                } else G->cells[i][j].bg_color_r = 0.5f;
            */
            /*
            if((G->cells[i][j].bg_color_r > 1) || (G->cells[i][j].bg_color_r < 0))
                    G->cells[i][j].r_t *= -1;
            if((G->cells[i][j].bg_color_g > 1) || (G->cells[i][j].bg_color_g < 0))
                    G->cells[i][j].g_t *= -1;
            if((G->cells[i][j].bg_color_b > 1) || (G->cells[i][j].bg_color_b < 0))
                    G->cells[i][j].b_t *= -1;

            G->cells[i][j].bg_color_r += G->cells[i][j].r_t;
            G->cells[i][j].bg_color_g += G->cells[i][j].g_t;
            G->cells[i][j].bg_color_b += G->cells[i][j].b_t;
            */

        }
}

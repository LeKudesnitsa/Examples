#ifndef STRUCTURES_H_INCLUDED
#define STRUCTURES_H_INCLUDED

#include "header.h"
/** CELLS[][].OWNER->ID CELLS[][].(*)OWNER.ID */

struct Landscape_t
{
    GLfloat     bg_color_r, bg_color_g, bg_color_b;
}; //����� ����� � ������ ������ ��� ���������� ������� ����� ������������
//������� ����� extern

typedef struct Owner
{
    char id;  //������ ��� ����� ������, ��� ��� 0 nobody, 1 barbarian
    GLfloat     bg_color_r, bg_color_g, bg_color_b, //���� ���� ������ ���� ���������� ������� ������
                border_color_r, border_color_g, border_color_b;
}Owner;

typedef struct Cell{
    Owner           *owner;
    GLfloat         x, y;
    int             index_row, index_column;

    GLfloat bg_color_r, bg_color_g, bg_color_b;
    GLfloat delta;
    GLfloat r_t, g_t, b_t;

    int             city, city_belongs_to; //�� ���������?
    unsigned char   params, landscape;
    unsigned int    resources, infrastructure;

    struct{
        struct Cell *n_link; //n for neighbour
        //unsigned char road, railroad, river, beach;
        unsigned char neighbour_params;
        char weight;
    }
    /** �������� ���� ������, � �� �������, �� ����� ����� ������ � ���� ��*/
    up, upright, downright, down, downleft, upleft;

}Cell;

struct ColorScheme{
};

typedef struct Globals{
    int tmp1, tmp2;

    int      rows, columns, player_ids; //���������� �����, �������� �������
    GLfloat  R, R_sin;
    Cell     **cells;
    Owner    *owners;
}Globals;


#endif // STRUCTURES_H_INCLUDED

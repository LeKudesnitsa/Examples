#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#include <windows.h>
#include <gl/gl.h>
#include <math.h>
#include <stdio.h>
#include <time.h>

#include "landscape_codes.h"
#include "structures.h"



/* глобальные переменные */
/*
extern int      rows, columns, player_ids; //количество строк, столбцов игроков
extern GLfloat  R, R_sin;
extern Cell     **cells;
extern Owner    *owners;

extern int tmp1, tmp2;
*/

/* cells.c */
void GenerateHexCells(Globals*);
void SetCellsDefault(Globals*);
Cell ** dynamic_array_alloc(size_t, size_t);
void dynamic_array_free(Cell**, size_t);

/* rainbow.c */
void ChangeColors(Globals*);
void Splash(Cell*);

#endif // HEADER_H_INCLUDED

#ifndef LANDSCAPE_CODES_H_INCLUDED
#define LANDSCAPE_CODES_H_INCLUDED

/** infrastructure */
#define ROAD        0b00000001
#define RAILROAD    0b00000010
#define RIVER       0b00000100
//#define COAST       0b00001000

/** landscape */
#define WATER       0b00000000
#define LAND        0b00000001
/* lanscape for water */
#define SEA         0b00000000
#define OCEAN       0b00000010
/* lanscape for land */
#define FIELD       0b00000000
#define HILL        0b00000010
#define FOREST      0b00000000
#define MOUNTAIN    0b00000100


#endif // LANDSCAPE_CODES_H_INCLUDED

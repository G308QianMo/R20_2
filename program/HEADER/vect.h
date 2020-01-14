#ifndef _VECT_H_
#define _VECT_H_
#include "stm32f4xx.h"
#include <stdint.h>

typedef struct 
{
    float x;
    float y;
	float angle;
}EncodePointTypeDef;

typedef struct
{
    int32_t x;
    int32_t y;
}CoordsTypeDef;

typedef struct
{
    CoordsTypeDef coords;   
    float   angle;  
}PointTypeDef;

typedef struct
{
    PointTypeDef start; 
    PointTypeDef end;   
}VectorTypeDef;

#endif


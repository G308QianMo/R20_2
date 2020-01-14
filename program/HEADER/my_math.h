#ifndef __MY_MATH_H
#define __MY_MATH_H
#include <math.h>
#include "string.h"
#include "stdlib.h"
#include "stdint.h"

#define pi 										3.141592653589793

#define rad2ang(rad) 							( (double)(rad)/pi*180.0f )

#define ang2rad(ang) 							( (double)(ang)*pi/180.0f )

#define square(x)    							( (x)*(x) )

#define my_sqrt(x)  							_sqrtf(x)

#define my_sin(x)  								sinf(ang2rad(x))
#define my_cos(x)  								cosf(ang2rad(x))

#define my_acos(x) 								rad2ang(acosf(x))
#define my_asin(x) 								rad2ang(asinf(x))

#define my_max(a,b) 							( (a)>(b) ? (a) : (b) )

#define my_min(a,b)								( (a)<(b) ? (a) : (b) )

#define my_abs(num)  								(((num)>0)?(num):-(num)) 

#define my_minus_abs(num1,num2)                 ((num1>num2)?(num1-num2):(num2-num1))

#define float_equal(a,b) 					    ( fabsf((a)-(b))<0.000001f )
#define float_not_equal(a,b) 			        ( fabsf((a)-(b))>0.000001f )
#define float_greater(a,b) 				        ( (a)-(b)>0.000001f )
#define float_smaller(a,b) 				        ( (a)-(b)<-0.000001f )

#define f_equal(a,b)   (fabsf((a)-(b))<0.000001f)
#define f_unequal(a,b) (fabsf((a)-(b))>0.000001f)
#define f_greater(a,b) ((a)-(b)>0.000001f)
#define f_smaller(a,b) ((a)-(b)<-0.000001f)

void Bubble_Sort(uint32_t _data[], uint32_t size);


#endif


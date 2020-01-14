#ifndef __BASIAL_MOVE_H
#define __BASIAL_MOVE_H

#define  OFFSET_ANG (0)   //正方向与x的夹角
#define  MAX_SPEED  (15000)  //最大速度

void Move_To_Point_Set(int32_t X, int32_t Y, float Alpha, int32_t Speed);
void MOVE_As_Circle(int32_t Speed);
void Clear_Static_Wheel_Spd(void);
void Wheel_Spd_To_Motor(void);
void STOP(void);
void Print(int32_t Spd[4], int Print_Frequency);


#endif

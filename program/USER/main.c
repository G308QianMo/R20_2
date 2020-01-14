#include "include.h"
uint32_t P_liu = 500, I_Liu = 0, D_Liu = 100;
extern EncodePointTypeDef global_gyro_location;
extern __IO float global_enc_x, global_enc_y;
extern int32_t Global_Target_X, Global_Target_Y, Global_Target_Angle;

float* Eular;
extern float e[3];
int16_t Move_liu = 0;
extern float global_imu_angle_initial_value;

int main(void)
{
 Init();//初始化

  while(1)
 {
	//mission_temp();
//	 LED0_ON;
	 printf("%d\t%d\t%d\r\n",(int)global_gyro_location.x,(int)global_gyro_location.y,(int)global_gyro_location.angle);
//	  print();
//	 get_eular(Eular);
//	 printf("%f\r\n",Eular[2]);
 }
}


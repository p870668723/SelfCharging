#ifndef KALMAN_H
#define KALMAN_H

#ifdef __cplusplus
 extern "C" {
#endif


typedef struct KALMAN_COM
{
	int length;
	int data[10];
}Kalman_com;

typedef struct KALMAN
{
    double Q;       //过程噪声的协方差
    double R;       //观测噪声的协方差
    
    double x_last;  //上次的最优值，本次测量值，本次最优值
    double p_last;  //上次协方差，本次预测协方差，本次协方差
    
    double kg;      //kalman增益
    
    double A;       //系统转移矩阵，
    double H;       //观测转移矩阵   
		
	  double best;
	
	  Kalman_com com;
}Kalman_filter;



extern Kalman_filter kalman;
extern void  kalman_init(Kalman_filter* kalman);
extern double kalman_filter(Kalman_filter *kalman,double input);
extern double kalman_com_filter(Kalman_filter *);

#ifdef __cplusplus
}
#endif

#endif

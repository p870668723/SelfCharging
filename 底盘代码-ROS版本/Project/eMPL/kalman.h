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
    double Q;       //����������Э����
    double R;       //�۲�������Э����
    
    double x_last;  //�ϴε�����ֵ�����β���ֵ����������ֵ
    double p_last;  //�ϴ�Э�������Ԥ��Э�������Э����
    
    double kg;      //kalman����
    
    double A;       //ϵͳת�ƾ���
    double H;       //�۲�ת�ƾ���   
		
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

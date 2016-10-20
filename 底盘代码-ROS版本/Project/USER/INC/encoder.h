#ifndef ENCODER_H
#define ENCODER_H

#ifdef __cplusplus
 extern "C" {
#endif


#include "stm32f4xx.h"
#include "stm32f4xx_tim.h"


#define EncoderOne_TIM										TIM4
#define EncoderOne_TIM_RCC                RCC_APB1Periph_TIM4
#define EncoderOne_TIM_AF									GPIO_AF_TIM4
#define EncoderOne_GPIO                   GPIOD
#define EncoderOne_GPIO_RCC               RCC_AHB1Periph_GPIOD
#define EncoderOne_GPIO_BITs              (GPIO_Pin_12 | GPIO_Pin_13)
#define EncoderOne_GPIO_PinsourceA        GPIO_PinSource12
#define EncoderOne_GPIO_PinsourceB        GPIO_PinSource13
#define EncoderOne_GetEncoder(x)          {x = TIM4->CNT - 0X7FFF;TIM4->CNT = 0X7FFF;}
#define EncoderOne_ResetEncoder()         (TIM4->CNT = 0X7FFF)


#define EncoderTwo_TIM										TIM5
#define EncoderTwo_TIM_RCC                RCC_APB1Periph_TIM5
#define EncoderTwo_TIM_AF									GPIO_AF_TIM5
#define EncoderTwo_GPIO                   GPIOH
#define EncoderTwo_GPIO_RCC               RCC_AHB1Periph_GPIOH
#define EncoderTwo_GPIO_BITs              (GPIO_Pin_10 | GPIO_Pin_11)
#define EncoderTwo_GPIO_PinsourceA        GPIO_PinSource10
#define EncoderTwo_GPIO_PinsourceB        GPIO_PinSource11
#define EncoderTwo_GetEncoder(x)          {x = TIM5->CNT - 0X7FFF;TIM5->CNT = 0X7FFF;}
#define EncoderTwo_ResetEncoder()         (TIM5->CNT = 0X7FFF)


#define EncoderThree_TIM									TIM2
#define EncoderThree_TIM_RCC              RCC_APB1Periph_TIM2
#define EncoderThree_TIM_AF								GPIO_AF_TIM2
#define EncoderThree_GPIO                 GPIOA
#define EncoderThree_GPIO_RCC             RCC_AHB1Periph_GPIOA
#define EncoderThree_GPIO_BITs            (GPIO_Pin_0 | GPIO_Pin_1)
#define EncoderThree_GPIO_PinsourceA      GPIO_PinSource0
#define EncoderThree_GPIO_PinsourceB      GPIO_PinSource1
#define EncoderThree_GetEncoder(x)        {x = TIM2->CNT - 0X7FFF;TIM2->CNT = 0X7FFF;}
#define EncoderThree_ResetEncoder()       (TIM2->CNT = 0X7FFF)


void WheelEncoder_Init(void );


#ifdef __cplusplus
}
#endif


#endif



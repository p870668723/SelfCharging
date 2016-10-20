#ifndef OLED_H
#define OLED_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"

#define SDA_GPIO   GPIOE
#define SDA_BIT    GPIO_Pin_5

#define SCK_GPIO   GPIOE
#define SCK_BIT    GPIO_Pin_4

#define DC_GPIO    GPIOE
#define DC_BIT     GPIO_Pin_3

#define RST_GPIO   GPIOE
#define RST_BIT    GPIO_Pin_2

#define OLED_RST_H  GPIO_SetBits(RST_GPIO,RST_BIT)
#define OLED_RST_L  GPIO_ResetBits(RST_GPIO,RST_BIT)

#define OLED_SDA_H  GPIO_SetBits(SDA_GPIO,SDA_BIT)
#define OLED_SDA_L  GPIO_ResetBits(SDA_GPIO,SDA_BIT)

#define OLED_SCK_H  GPIO_SetBits(SCK_GPIO,SCK_BIT)
#define OLED_SCK_L  GPIO_ResetBits(SCK_GPIO,SCK_BIT)

#define OLED_DC_H   GPIO_SetBits(DC_GPIO,DC_BIT)
#define OLED_DC_L   GPIO_ResetBits(DC_GPIO,DC_BIT)

#define OLED_CLOCK	(RCC_AHB1Periph_GPIOE)

void OLED_Init(void);
void OLED_Pant(void);
void OLED_ShowEnglish(u8 ,u8 ,u8 *);
void OLED_Printf(u8 ,u8, char *,...);
void OLED_HorizontalScroll(u8 ,u8 ,u8);

#ifdef __cplusplus
}
#endif

#endif

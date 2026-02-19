/************************************************
* WKS Mini GD32开发板
* 2.4&3.5寸SPI模块显示实验
************************************************/

#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h"
#include "lcd.h"
#include "math.h"

int main(void)
{   
    uint8_t x = 0;
    uint8_t lcd_id[12];                     /* 存放LCD ID字符串 */
  
    delay_init(120);                        /* 初始化延时函数 */
    usart_init(115200);                     /* 初始化串口 */
    LED_Init();							                /* 初始化LED */
    lcd_init();                             /* 初始化LCD */
  
    sprintf((char *)lcd_id, "LCD ID:%04X", lcddev.id);  /* 将LCD ID打印到lcd_id数组 */

    while (1)
    {
        switch (x)
        {
            case 0: lcd_clear(WHITE);    break;

            case 1: lcd_clear(BLACK);    break;

            case 2: lcd_clear(BLUE);     break;

            case 3: lcd_clear(RED);      break;

            case 4: lcd_clear(MAGENTA);  break;

            case 5: lcd_clear(GREEN);    break;

            case 6: lcd_clear(CYAN);     break;

            case 7: lcd_clear(YELLOW);   break;

            case 8: lcd_clear(BRRED);    break;

            case 9: lcd_clear(GRAY);     break;

            case 10:lcd_clear(LGRAY);    break;

            case 11:lcd_clear(BROWN);    break;
        }

        lcd_show_string(10, 40, 200, 32, 32, "Mini GD32 ^_^", RED);
        lcd_show_string(10, 80, 200, 24, 24, "SPI LCD TEST", RED);
        lcd_show_string(10, 110, 200, 16, 16, "WKS SMART", RED);
        lcd_show_string(10, 130, 200, 16, 16, (char *)lcd_id, RED); /* 显示LCD ID */
        
        x++;
        if (x == 12)
        {
            x = 0;
        }
        
        LED0_TOGGLE();   /* LED0不停的闪烁，提示程序已经在运行了 */
        delay_ms(1000);
    }
}

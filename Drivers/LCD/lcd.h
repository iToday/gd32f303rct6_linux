/************************************************
* WKS Mini GD32���İ�
* 2.4/3.5�� TFTLCD(SPI������) ��������
* ����IC�ͺ�:ST7789/ST7796
* �汾��V1.0
************************************************/

#ifndef __LCD_H
#define __LCD_H

#include "stdlib.h"
#include "sys.h"


/******************************************************************************************/

/*������Ļ����*/
/*0Ϊ����
  1Ϊ����*/
#define LCD_DIR  1

/* LCD��Ҫ������ */
typedef struct
{
    uint16_t width;     /* LCD ���� */
    uint16_t height;    /* LCD �߶� */
    uint16_t id;        /* LCD ID */
    uint8_t dir;        /* ���������������ƣ�0��������1�������� */
    uint16_t wramcmd;   /* ��ʼдgramָ�� */
    uint16_t setxcmd;   /* ����x����ָ�� */
    uint16_t setycmd;   /* ����y����ָ�� */
} _lcd_dev;

/* LCD���� */
extern _lcd_dev lcddev; /* ����LCD��Ҫ���� */

/* LCD�Ļ�����ɫ�ͱ���ɫ */
extern uint16_t  g_point_color;     /* Ĭ�Ϻ�ɫ */
extern uint16_t  g_back_color;      /* ������ɫ.Ĭ��Ϊ��ɫ */

/* LCD������� */
#define LCD_BL(x)        do{ x ? \
                             gpio_bit_write(GPIOC, GPIO_PIN_2, SET) : \
                             gpio_bit_write(GPIOC, GPIO_PIN_2, RESET); \
                         }while(0)

/* LCD��λ���� */
#define LCD_RST(x)       do{ x ? \
                             gpio_bit_write(GPIOC, GPIO_PIN_5, SET) : \
                             gpio_bit_write(GPIOC, GPIO_PIN_5, RESET); \
                         }while(0)

#define SPI_CS(x)        do{ x ? \
									     gpio_bit_write(GPIOB, GPIO_PIN_12, SET) : \
												     gpio_bit_write(GPIOB, GPIO_PIN_12, RESET); \
                         }while(0)     /* SPI_CS */

#define SPI_SCK(x)       do{ x ? \
												     gpio_bit_write(GPIOB, GPIO_PIN_13, SET) : \
												     gpio_bit_write(GPIOB, GPIO_PIN_13, RESET); \
                         }while(0)     /* SPI_SCK */

#define SPI_MOSI(x)      do{ x ? \
												     gpio_bit_write(GPIOB, GPIO_PIN_15, SET) : \
												     gpio_bit_write(GPIOB, GPIO_PIN_15, RESET); \
                         }while(0)     /* SPI_MOSI */

#define SPI_MISO         gpio_input_bit_get(GPIOB, GPIO_PIN_14)  /* SPI_MISO */

/* ʹ��λ���������ӿ������ٶ� - ֳֻ֧赋ֵ */
#define SPI_CSX_SET      do{gpio_bit_write(GPIOB, GPIO_PIN_12, SET);}while(0)   /* ƬѡCSX = 1 */
#define SPI_CSX_CLR      do{gpio_bit_write(GPIOB, GPIO_PIN_12, RESET);}while(0) /* ƬѡCSX = 0 */
#define SPI_SCL_SET      do{gpio_bit_write(GPIOB, GPIO_PIN_13, SET);}while(0)   /* SCL = 1 */
#define SPI_SCL_CLR      do{gpio_bit_write(GPIOB, GPIO_PIN_13, RESET);}while(0) /* SCL = 0 */
#define SPI_SDA_SET      do{gpio_bit_write(GPIOB, GPIO_PIN_15, SET);}while(0)   /* SDA = 1 */
#define SPI_SDA_CLR      do{gpio_bit_write(GPIOB, GPIO_PIN_15, RESET);}while(0) /* SDA = 0 */
#define SPI_READ_SDA     gpio_input_bit_get(GPIOB, GPIO_PIN_14)                 /* ��ȡSDA */

/******************************************************************************************/
/* LCDɨ�跽�����ɫ ���� */

/* ɨ�跽���� */
#define L2R_U2D         0           /* ������,���ϵ��� */
#define L2R_D2U         1           /* ������,���µ��� */
#define R2L_U2D         2           /* ���ҵ���,���ϵ��� */
#define R2L_D2U         3           /* ���ҵ���,���µ��� */

#define U2D_L2R         4           /* ���ϵ���,������ */
#define U2D_R2L         5           /* ���ϵ���,���ҵ��� */
#define D2U_L2R         6           /* ���µ���,������ */
#define D2U_R2L         7           /* ���µ���,���ҵ��� */

#define DFT_SCAN_DIR    L2R_U2D     /* Ĭ�ϵ�ɨ�跽�� */

/* ���û�����ɫ */
#define WHITE           0xFFFF      /* ��ɫ */
#define BLACK           0x0000      /* ��ɫ */
#define RED             0xF800      /* ��ɫ */
#define GREEN           0x07E0      /* ��ɫ */
#define BLUE            0x001F      /* ��ɫ */
#define MAGENTA         0XF81F      /* Ʒ��ɫ/�Ϻ�ɫ = BLUE + RED */
#define YELLOW          0XFFE0      /* ��ɫ = GREEN + RED */
#define CYAN            0X07FF      /* ��ɫ = GREEN + BLUE */

/* �ǳ�����ɫ */
#define BROWN           0XBC40      /* ��ɫ */
#define BRRED           0XFC07      /* �غ�ɫ */
#define GRAY            0X8430      /* ��ɫ */
#define DARKBLUE        0X01CF      /* ����ɫ */
#define LIGHTBLUE       0X7D7C      /* ǳ��ɫ */
#define GRAYBLUE        0X5458      /* ����ɫ */
#define LIGHTGREEN      0X841F      /* ǳ��ɫ */
#define LGRAY           0XC618      /* ǳ��ɫ(PANNEL),���屳��ɫ */
#define LGRAYBLUE       0XA651      /* ǳ����ɫ(�м����ɫ) */
#define LBBLUE          0X2B12      /* ǳ����ɫ(ѡ����Ŀ�ķ�ɫ) */

/******************************************************************************************/
/* �������� */

void spi_send_byte(uint8_t buf);                     /* SPIд��1�ֽ����� */
uint8_t spi_read_byte(void);                         /* SPI��ȡ1�ֽ����� */
void SPI_CmdWrite(uint8_t cmd);                      /* ��LCD����ICд���� */
void SPI_DataWrite(uint8_t data);                    /* ��LCD����ICд���� */
void SPI_DataWrite_Pixel(uint16_t data);             /* дһ�����ص������ */
uint8_t lcd_read_dummy(uint8_t regno);               /* �ٶ�һ������ */
uint16_t lcd_read_ram(uint8_t regno);                /* ��GRAM���� */
uint16_t lcd_read_id(uint8_t regno);                 /* ��LCD����IC��ID */

void lcd_wr_data(volatile uint16_t data);            /* LCDд���� */
void lcd_wr_regno(volatile uint16_t regno);          /* LCDд�Ĵ������/��ַ */
void lcd_write_reg(uint16_t regno, uint16_t data);   /* LCDд�Ĵ�����ֵ */

void lcd_init(void);                        /* ��ʼ��LCD */
void exmc_sram_init(void);                  /* ��ʼ��EXMC */
void lcd_display_on(void);                  /* ����ʾ */
void lcd_display_off(void);                 /* ����ʾ */
void lcd_scan_dir(uint8_t dir);             /* ������Ļɨ�跽�� */
void lcd_display_dir(uint8_t dir);          /* ������Ļ��ʾ���� */

void lcd_write_ram_prepare(void);                               /* ׼��дGRAM */
void lcd_set_cursor(uint16_t x, uint16_t y);                    /* ���ù�� */
uint16_t lcd_read_point(uint16_t x, uint16_t y);                /* ���� */
void lcd_draw_point(uint16_t x, uint16_t y, uint16_t color);    /* ���� */

void lcd_clear(uint16_t color);                                                                 /* LCD���� */
void lcd_fill_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color);                       /* ���ʵ��Բ */
void lcd_draw_circle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color);                      /* ��Բ */
void lcd_draw_hline(uint16_t x, uint16_t y, uint16_t len, uint16_t color);                      /* ��ˮƽ�� */
void lcd_set_window(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height);                 /* ���ô��� */
void lcd_fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color);              /* ��ɫ������ */
void lcd_color_fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color);       /* ��ɫ������ */
void lcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);         /* ��ֱ�� */
void lcd_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);    /* ������ */

void lcd_show_char(uint16_t x, uint16_t y, char chr, uint8_t size, uint8_t mode, uint16_t color);                       /* ��ʾһ���ַ� */
void lcd_show_num(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint16_t color);                     /* ��ʾ���� */
void lcd_show_xnum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint8_t mode, uint16_t color);      /* ��չ��ʾ���� */
void lcd_show_string(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, char *p, uint16_t color);   /* ��ʾ�ַ��� */


#endif

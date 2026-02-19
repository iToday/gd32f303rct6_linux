/**
 * @file lv_port_disp_templ.c
 *
 */

 /*Copy this file as "lv_port_disp.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_disp_template.h"
#include "../../lvgl.h"
/* ����lcd����ͷ�ļ� */
#include "lcd.h"
#include "malloc.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
/* ��ʾ�豸��ʼ������ */
static void disp_init(void);
/* ��ʾ�豸ˢ�º��� */
static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);
//static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
//        const lv_area_t * fill_area, lv_color_t color);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
/*
��������lv_port_disp_init
���ܣ���ʼ����ע����ʾ�豸
��ע����
*/
void lv_port_disp_init(void)
{
    /*-------------------------
     * ��ʼ����ʾ�豸
     * -----------------------*/
    disp_init();

    /*-----------------------------
     * ����һ����ͼ������
     *----------------------------*/

    /**
     * LVGL ��Ҫһ����������������С����
     * �����������������ݻ�ͨ����ʾ�豸�� `flush_cb`(��ʾ�豸ˢ�º���) ���Ƶ���ʾ�豸��
     * ����������Ĵ�С��Ҫ������ʾ�豸һ�еĴ�С
     *
     * ������3�л�������:
     * 1. ��������:
     *      LVGL �Ὣ��ʾ�豸�����ݻ��Ƶ����������д����ʾ�豸��
     *
     * 2. ˫������:
     *      LVGL �Ὣ��ʾ�豸�����ݻ��Ƶ�����һ����������������д����ʾ�豸��
     *      ��Ҫʹ�� DMA ��Ҫ��ʾ����ʾ�豸������д�뻺������
     *      �����ݴӵ�һ������������ʱ������ʹ LVGL �ܹ�����Ļ����һ���ֻ��Ƶ���һ����������
     *      ����ʹ����Ⱦ��ˢ�¿��Բ���ִ�С�
     *
     * 3. ȫ�ߴ�˫������
     *      ����������Ļ��С��ȫ�ߴ绺�������������� disp_drv.full_refresh = 1��
     *      ������LVGL��ʼ���� 'flush_cb' ����ʽ�ṩ������Ⱦ��Ļ����ֻ�����֡�������ĵ�ַ��
     */

    /* ��������ʾ��) */
    static lv_disp_draw_buf_t draw_buf_dsc_1;
    static lv_color_t buf_1[MY_DISP_HOR_RES * 10];                          /* ���û������Ĵ�СΪ��Ļ��ȫ�ߴ��С */
    lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, NULL, MY_DISP_HOR_RES * 10);    /* ��ʼ����ʾ������ */


//    /* ˫������ʾ��) */
//    static lv_disp_draw_buf_t draw_buf_dsc_2;
//    static lv_color_t buf_2_1[MY_DISP_HOR_RES * 10];                         /* ���û������Ĵ�СΪ 10 ����Ļ�Ĵ�С */
//    static lv_color_t buf_2_1[MY_DISP_HOR_RES * 10];                        /* ������һ���������Ĵ�СΪ 10 ����Ļ�Ĵ�С */
//    lv_disp_draw_buf_init(&draw_buf_dsc_2, buf_2_1, buf_2_1, MY_DISP_HOR_RES * 10);   /* ��ʼ����ʾ������ */

//    /* ȫ�ߴ�˫������ʾ��) �������������� disp_drv.full_refresh = 1 */
//    static lv_disp_draw_buf_t draw_buf_dsc_3;
//    static lv_color_t buf_3_1[MY_DISP_HOR_RES * MY_DISP_VER_RES];            /* ����һ��ȫ�ߴ�Ļ����� */
//    static lv_color_t buf_3_1[MY_DISP_HOR_RES * MY_DISP_VER_RES];            /* ������һ��ȫ�ߴ�Ļ����� */
//    lv_disp_draw_buf_init(&draw_buf_dsc_3, buf_3_1, buf_3_2, MY_DISP_VER_RES * LV_VER_RES_MAX);   /* ��ʼ����ʾ������ */

    /*-----------------------------------
     * �� LVGL ��ע����ʾ�豸
     *----------------------------------*/

    static lv_disp_drv_t disp_drv;                         /* ��ʾ�豸�������� */
    lv_disp_drv_init(&disp_drv);                    /* ��ʼ��ΪĬ��ֵ */
    /* ����������ʾ�豸�ĺ���  */

    /* ������ʾ�豸�ķֱ���
     * ��������˶�̬��ȡ�ķ�ʽ��
     * ��ʵ����Ŀ�У�ͨ����ʹ�õ���Ļ��С�ǹ̶��ģ���˿���ֱ������Ϊ��Ļ�Ĵ�С */
    disp_drv.hor_res = lcddev.width;
    disp_drv.ver_res = lcddev.height;

    /* �����������������ݸ��Ƶ���ʾ�豸 */
    disp_drv.flush_cb = disp_flush;

    /* ������ʾ������ */
    disp_drv.draw_buf = &draw_buf_dsc_1;

    /* ȫ�ߴ�˫������ʾ��)*/
    //disp_drv.full_refresh = 1

    /* �������GPU����ʹ����ɫ����ڴ�����
     * ע�⣬������� lv_conf.h ��ʹ�� LVGL ����֧�ֵ� GPUs
     * ��������в�ͬ�� GPU����ô����ʹ������ص������� */
    //disp_drv.gpu_fill_cb = gpu_fill;

    /* ע����ʾ�豸 */
    lv_disp_drv_register(&disp_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*
��������disp_init
���ܣ���ʼ����ʾ�豸�ͱ�Ҫ����Χ�豸
��ע����
*/
static void disp_init(void)
{
    /*You code here*/
}

/*Flush the content of the internal buffer the specific area on the display
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_disp_flush_ready()' has to be called when finished.*/

/*
��������disp_flush
���ܣ����ڲ�������������ˢ�µ���ʾ���ϵ��ض�����
������disp_drv    : ��ʾ�豸
      area        : Ҫˢ�µ����򣬰����������εĶԽ�����
      color_p     : ��ɫ����
��ע������ʹ�� DMA �����κ�Ӳ���ں�̨����ִ���������
      ���ǣ���Ҫ��ˢ����ɺ���ú��� 'lv_disp_flush_ready()'
*/
static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
//	if(!RGB_DIR)
//	{
//		int32_t x;
//    int32_t y;
//    for(y = area->y1; y <= area->y2; y++) {
//        for(x = area->x1; x <= area->x2; x++) {
//            /*Put a pixel to the display. For example:*/
//            tli_draw_point(x, y, *color_p);
//            color_p++;
//        }
//    }
//	}
//	else
//	{
		/* ��ָ�����������ָ����ɫ�� */
	  lcd_color_fill(area->x1,area->y1,area->x2,area->y2,(uint16_t*)color_p);
//	}
    /* ��Ҫ!!!
     * ֪ͨͼ�ο⣬�Ѿ�ˢ������� */
    lv_disp_flush_ready(disp_drv);
}

/*OPTIONAL: GPU INTERFACE*/

/*If your MCU has hardware accelerator (GPU) then you can use it to fill a memory with a color*/
/*
��������gpu_fill
���ܣ�ʹ�� GPU ������ɫ���
������disp_drv    : ��ʾ�豸
      dest_buf    : Ŀ�껺����
      dest_width  : Ŀ�껺�����Ŀ���
      fill_area   : ��������
      color       : ��ɫ����
��ע������ MCU ��Ӳ�������� (GPU),��ô����������Ϊ�ڴ������ɫ���
*/
//static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
//                    const lv_area_t * fill_area, lv_color_t color)
//{
//    /*It's an example code which should be done by your GPU*/
//    int32_t x, y;
//    dest_buf += dest_width * fill_area->y1; /*Go to the first line*/
//
//    for(y = fill_area->y1; y <= fill_area->y2; y++) {
//        for(x = fill_area->x1; x <= fill_area->x2; x++) {
//            dest_buf[x] = color;
//        }
//        dest_buf+=dest_width;    /*Go to the next line*/
//    }
//}


#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif

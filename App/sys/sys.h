#ifndef __SYS_H
#define __SYS_H

#include "gd32f30x.h"

/* 兼容 Keil 代码中的 gpio_pin_set，映射到 GD32 标准库 gpio_bit_write */
#define gpio_pin_set(port, pin, val)  gpio_bit_write((port), (pin), (val) ? SET : RESET)


/**
 * SYS_SUPPORT_OS���ڶ���ϵͳ�ļ����Ƿ�֧��OS
 * 0,��֧��OS
 * 1,֧��OS
 */
#define SYS_SUPPORT_OS          0


/* ����Ϊ��ຯ�� */
void sys_wfi_set(void);                                                         /* ִ��WFIָ�� */
void sys_intx_disable(void);                                                    /* �ر������ж� */
void sys_intx_enable(void);                                                     /* ���������ж� */
void sys_msr_msp(uint32_t addr);                                                /* ����ջ����ַ */

#endif












#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // ����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    uart_init(9600);                                // ���ڳ�ʼ��Ϊ9600
    delay_init();                                   // ��ʱ������ʼ��
    LED_Init();
    printf("wo jiao ajie");
    while (1)
    {
        LED0 = 1;
        delay_ms(100);
        LED0 = 1;
        delay_ms(100);
        LED0 = 0;
        delay_ms(200);
        LED0 = 1;
        delay_ms(300);
    }
}

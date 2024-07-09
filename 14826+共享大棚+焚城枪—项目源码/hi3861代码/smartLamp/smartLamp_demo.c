/*
 * Copyright (c) 2023 Beijing HuaQing YuanJian Education Technology Co., Ltd
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "ohos_init.h"
#include "cmsis_os2.h"
#include "cJSON.h"
#include "sys_config.h"

#include "hal_bsp_wifi.h"
#include "hal_bsp_mqtt.h"
#include "hal_bsp_key.h"
#include "hal_bsp_ap3216c.h"
#include "hal_bsp_aw2013.h"
#include "hal_bsp_ssd1306.h"
#include "hal_bsp_pcf8574.h"
#include "hal_bsp_sht20.h"
#include "hal_bsp_nfc.h"
#include "hal_bsp_nfc_to_wifi.h"
#include "mqtt_send_task.h"
#include "mqtt_recv_task.h"
#include "sensor_collect_task.h"
#include "oled_show_log.h"

#include "hi_io.h"
#include "hi_gpio.h"
#include "hi_uart.h"



#define TASK_STACK_SIZE (5 * 1024)
#define TASK_INIT_DELAY 1 // s

osThreadId_t mqtt_send_task_id;      // mqtt 发布数据任务ID
osThreadId_t mqtt_recv_task_id;      // mqtt 接收数据任务ID
osThreadId_t sensor_collect_task_id; // 传感器采集任务ID

osThreadId_t my_485_id;
osThreadId_t auto_mode_id;




//rs485数组
uint8_t air_arr[8]={0x01,0x03,0x00,0x00,0x00,0x06,0xC5,0xC8};//
uint8_t co2_arr[8]={0x02,0x03,0x00,0x00,0x00,0x01,0x84,0x39};//
uint8_t soil_arr[8] = {0X03,0X03,0X00,0X00,0X00,0X04,0X45,0XEB};//
uint8_t voltage_arr[8] = {0X04,0X04,0X00,0X00,0X00,0X02,0X71,0X9E};//
//接收数组

uint8_t air_RS485buf[17]={0};
uint8_t co2_RS485buf[13]={0};
uint8_t soil_RS485buf[13]={0};
uint8_t voltage_RS485buf[13]={0};	

//定义int

//extern float voltage=0,water=0,soil_temp=0,conductivity=0,ph=0;
uint16_t temp_value,humi_value,lux_value,CO2_value;
uint16_t water_h=0,water_l=0,temp_h=0,temp_l=0,dd_h=0,dd_l=0,ph_h=0,ph_l=0,voltage_h=0,voltage_l=0;
float voltage=0,water=0,soil_temp=0,conductivity=0,ph=0;
uint16_t temp_value_max = 30,temp_value_min = 20 ,humi_value_max = 60,humi_value_min = 40;lux_value_max = 80,lux_value_min = 20,
water_max=60,water_min=10,voltage_max=10,voltage_min = 2;

 int auto_bool = 0;



void my_uart_init(void)
{
    set_D1(false);
    set_D2(false);
    set_D3(false);
    set_D4(false);
    uint32_t ret = 0;
    // 初始化串口
    hi_io_set_func(HI_IO_NAME_GPIO_11, HI_IO_FUNC_GPIO_11_UART2_TXD);
    hi_io_set_func(HI_IO_NAME_GPIO_12, HI_IO_FUNC_GPIO_12_UART2_RXD);

    hi_uart_attribute uart_param = {
        .baud_rate = 4800,
        .data_bits = 8,
        .stop_bits = 1,
        .parity = 0,
    };
    ret = hi_uart_init(HI_UART_IDX_2, &uart_param, NULL);
    if (ret != HI_ERR_SUCCESS) {
        printf("hi uart init is faild.\r\n");
    }
}

void my_uart_send_buff(unsigned char *str, unsigned short len)
{
    hi_u32 ret = 0;
    ret = hi_uart_write(HI_UART_IDX_2, (uint8_t *)str, len);
    if (ret == HI_ERR_FAILURE) {
        printf("uart send buff is failed.\r\n");
    }
}

void my_uart_recv_buff(unsigned char *str, unsigned short msg_len)
{
    
    unsigned char recv_buf[128];
    hi_u32 ret = 0;
    hi_u32 len = hi_uart_read(HI_UART_IDX_2, recv_buf, sizeof(recv_buf));
    if (ret == HI_ERR_FAILURE) {
        printf("uart send buff is failed.\r\n");
    }
    memcpy(str,recv_buf,len);

    //循环接收

}
/**/
void my_485(void)
{
    while (1)
    {
    my_uart_send_buff(air_arr,8);
    sleep(1);
    memset(air_RS485buf,0,sizeof(air_RS485buf));
    my_uart_recv_buff(air_RS485buf,32);
    
	humi_value = (( air_RS485buf[3]<< 8) | air_RS485buf[4])/10;
	temp_value = (( air_RS485buf[5]<< 8) | air_RS485buf[6])/10;
	lux_value = ((air_RS485buf[7] << 8) | air_RS485buf[8])/10;	   

    my_uart_send_buff(co2_arr,8);
    sleep(1);
    memset(co2_RS485buf,0,sizeof(co2_RS485buf));
    my_uart_recv_buff(co2_RS485buf,32);
    CO2_value =(( co2_RS485buf[5]<< 8) | co2_RS485buf[6])/10;
    
    
    my_uart_send_buff(soil_arr,8);
    sleep(1);
    memset(soil_RS485buf,0,sizeof(soil_RS485buf));
    my_uart_recv_buff(soil_RS485buf,32);
    
	temp_h = ((soil_RS485buf[5]<<8)|soil_RS485buf[6])/10;
	temp_l = ((soil_RS485buf[5]<<8)|soil_RS485buf[6])%10;
	water_h= ((soil_RS485buf[3]<<8)|soil_RS485buf[4])/10;
	water_l= ((soil_RS485buf[3]<<8)|soil_RS485buf[4])%10;
	ph_h = ((soil_RS485buf[9]<<8)|soil_RS485buf[10])/10;
	ph_l = ((soil_RS485buf[9]<<8)|soil_RS485buf[10])%10;
	dd_h = ((soil_RS485buf[7]<<8)|soil_RS485buf[8])/10;
	dd_l = ((soil_RS485buf[7]<<8)|soil_RS485buf[8])%10;    

    soil_temp=temp_h;
	water=water_h;
	conductivity=dd_h;
	ph=ph_h;
 

    my_uart_send_buff(voltage_arr,8);
    sleep(1);
    memset(voltage_RS485buf,0,sizeof(voltage_RS485buf));
    my_uart_recv_buff(voltage_RS485buf,32);
    voltage_h= ((voltage_RS485buf[3]<<8)|voltage_RS485buf[4])/10;
	voltage_l= ((voltage_RS485buf[3]<<8)|voltage_RS485buf[4])%10;
    voltage = (voltage_h+(voltage_l*0.1))/100;
    
    printf("humi_value:%d\n",humi_value);
    printf("temp_value:%d\n",temp_value);    
    printf("lux_value:%d\n",lux_value);
    printf("CO2_value:%d\n",CO2_value);
    printf("soil_temp:%.2f\n",soil_temp);
    printf("water:%.2f\n",water);
    printf("conductivity:%.2f\n",conductivity);
    printf("ph:%.2f\n",ph);    
    printf("voltage:%.2f\n",voltage); 
    sleep(1);
    }
}

//自动模式
void my_auto_mode(void)
{
    while (auto_bool)
    {
        if (temp_value >temp_value_max)
        {
            set_D3(true);//排风扇
            sleep(8);
            set_D3(false);
        }
        if ((lux_value<lux_value_min)||(humi_value > humi_value_max))
        {
           set_D2(true);//补光灯
        }
        if ((lux_value>lux_value_max)||(humi_value < humi_value_min))
        {
           set_D2(false);//补光灯
        }
        if (water>water_max)
        {
           set_D1(false);//
        }
        if (water < water_min)
        {
           set_D1(true);//
        }
        if(voltage>voltage_max)
        {
            set_D4(false);//
        }
        if(voltage>voltage_min)
        {
            set_D4(true);//
        }
    }
}



int mqtt_init(void)
{
    // 连接MQTT服务器
    while (MQTTClient_connectServer(SERVER_IP_ADDR, SERVER_IP_PORT) != WIFI_SUCCESS) {
        printf("mqttClient_connectServer\r\n");
        oled_consle_log("==mqtt ser no==");
        sleep(TASK_INIT_DELAY);
        SSD1306_CLS(); // 清屏
    }
    printf("mqttClient_connectServer\r\n");
    oled_consle_log("==mqtt ser yes=");
    sleep(TASK_INIT_DELAY);

    // 初始化MQTT客户端
    while (MQTTClient_init(MQTT_CLIENT_ID, MQTT_USER_NAME, MQTT_PASS_WORD) != WIFI_SUCCESS) {
        printf("mqttClient_init\r\n");
        oled_consle_log("==mqtt cli no==");
        sleep(TASK_INIT_DELAY);
        SSD1306_CLS(); // 清屏
    }
    printf("mqttClient_init\r\n");
    oled_consle_log("==mqtt cli yes=");
    sleep(TASK_INIT_DELAY);

    // 订阅MQTT主题
    while (MQTTClient_subscribe(MQTT_TOPIC_SUB_COMMANDS) != 0) {
        printf("mqttClient_subscribe\r\n");
        oled_consle_log("==mqtt sub no==");
        sleep(TASK_INIT_DELAY);
        SSD1306_CLS(); // 清屏
    }
    printf("mqttClient_subscribe\r\n");
    oled_consle_log("=mqtt sub yes==");
    sleep(TASK_INIT_DELAY);
    SSD1306_CLS(); // 清屏

    return 0;
}
int nfc_connect_wifi_init(void)
{
    // 通过NFC芯片进行连接WiFi
    uint8_t ndefLen = 0;      // ndef包的长度
    uint8_t ndef_Header = 0;  // ndef消息开始标志位-用不到
    uint32_t result_code = 0; // 函数的返回值
    oled_consle_log("===start nfc===");

    // 读整个数据的包头部分，读出整个数据的长度
    if (result_code = NT3HReadHeaderNfc(&ndefLen, &ndef_Header) != true) {
        printf("NT3HReadHeaderNfc is failed. result_code = %d\r\n", result_code);
        return -1;
    }
    ndefLen += NDEF_HEADER_SIZE; // 加上头部字节
    if (ndefLen <= NDEF_HEADER_SIZE) {
        printf("ndefLen <= 2\r\n");
        return -1;
    }
    
    uint8_t *ndefBuff = (uint8_t *)malloc(ndefLen + 1);
    if (ndefBuff == NULL) {
        printf("ndefBuff malloc is Falied!\r\n");
        return -1;
    }
    if (result_code = get_NDEFDataPackage(ndefBuff, ndefLen) != HI_ERR_SUCCESS) {
        printf("get_NDEFDataPackage is failed. result_code = %d\r\n", result_code);
        return -1;
    }
    // 打印读出的数据
    printf("start print ndefBuff.\r\n");
    for (size_t i = 0; i < ndefLen; i++) {
        printf("0x%x ", ndefBuff[i]);
    }
    oled_consle_log("=== end nfc ===");
    sleep(TASK_INIT_DELAY);

    oled_consle_log("== start wifi==");
    // 连接WiFi
    while (NFC_configuresWiFiNetwork(ndefBuff) != WIFI_SUCCESS) {
        printf("nfc connect wifi is failed!\r\n");
        oled_consle_log("=== wifi no ===");
        sleep(TASK_INIT_DELAY);
        SSD1306_CLS(); // 清屏
    }
    printf("nfc connect wifi is SUCCESS\r\n");
    oled_consle_log("===wifi  yes===");
    sleep(TASK_INIT_DELAY);
    return 0;
}
void SL_peripheral_init(void)
{
    // 外设的初始化
    KEY_Init();    // 按键初始化
    PCF8574_Init();  // 初始化IO扩展芯片
    AW2013_Init(); // 三色LED灯的初始化
    AW2013_Control_RGB(0, 0, 0);
    AP3216C_Init(); // 三合一传感器初始化
    SSD1306_Init(); // OLED 显示屏初始化
    SSD1306_CLS();  // 清屏
    nfc_Init();
    SHT20_Init(); 

}
static void smartLamp_Project_example(void)
{
    printf("Enter smartLamp_Project_example()!\r\n");
    p_MQTTClient_sub_callback = &mqttClient_sub_callback;
    // 全局变量的初始化
    sys_msg_data.led_light_value = 100; // RGB灯的亮度值为100%的状态
    SL_peripheral_init();
    nfc_connect_wifi_init();
    mqtt_init();

    my_uart_init();


    //  创建线程
    osThreadAttr_t options;
    options.name = "mqtt_send_task";
    options.attr_bits = 0;
    options.cb_mem = NULL;
    options.cb_size = 0;
    options.stack_mem = NULL;
    options.stack_size = TASK_STACK_SIZE;
    options.priority = osPriorityNormal;
    mqtt_send_task_id = osThreadNew((osThreadFunc_t)mqtt_send_task, NULL, &options);
    if (mqtt_send_task_id != NULL) {
        printf("ID = %d, Create mqtt_send_task_id is OK!\r\n", mqtt_send_task_id);
    }

    options.name = "mqtt_recv_task";
    mqtt_recv_task_id = osThreadNew((osThreadFunc_t)mqtt_recv_task, NULL, &options);
    if (mqtt_recv_task_id != NULL) {
        printf("ID = %d, Create mqtt_recv_task_id is OK!\r\n", mqtt_recv_task_id);
    }

    options.name = "sensor_collect_task";
    sensor_collect_task_id = osThreadNew((osThreadFunc_t)sensor_collect_task, NULL, &options);
    if (sensor_collect_task_id != NULL) {
        printf("ID = %d, Create sensor_collect_task_id is OK!\r\n", sensor_collect_task_id);
    }


    osThreadAttr_t auto_opt;
    auto_opt.name = "my_auto_mode";
    auto_opt.stack_size = 1024;
    auto_opt.priority = osPriorityNormal;
    auto_mode_id = osThreadNew((osThreadFunc_t)my_auto_mode,NULL,&auto_opt);

    osThreadAttr_t my_485_opt;
    my_485_opt.name = "my_485";
    my_485_opt.stack_size = 1024*4;
    my_485_opt.priority = osPriorityNormal;
    my_485_id = osThreadNew((osThreadFunc_t)my_485,NULL,&my_485_opt);
}
SYS_RUN(smartLamp_Project_example);
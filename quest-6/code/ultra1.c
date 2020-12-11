/*
Quest 6, Team 15
Author: Nnenna Eze, Lesbeth Roque
Date: 12/10/2020
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "driver/uart.h"
#include "driver/adc.h"
#include "driver/mcpwm.h"
#include "driver/timer.h"
#include "driver/rmt.h"
#include "driver/periph_ctrl.h"
#include "soc/mcpwm_periph.h"
#include "soc/rmt_reg.h"
#include "esp_system.h"
#include "esp_types.h"
#include "esp_vfs_dev.h"
#include "sdkconfig.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_err.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>
#include "addr_from_stdin.h"

#if CONFIG_IDF_TARGET_ESP32
#include "esp_adc_cal.h"
#endif
#if defined(CONFIG_EXAMPLE_IPV4)
#define HOST_IP_ADDR "192.168.1.74"//CONFIG_EXAMPLE_IPV4_ADDR
#elif defined(CONFIG_EXAMPLE_IPV6)
#define HOST_IP_ADDR CONFIG_EXAMPLE_IPV6_ADDR
#else
#define HOST_IP_ADDR "192.168.1.74" //address of main esp
#endif

#define PORT CONFIG_EXAMPLE_PORT
#define DEFAULT_VREF    1100        //Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES   64          //Multisampling

#define RMT_TX_CHANNEL 1 /* RMT channel for transmitter */
#define RMT_TX_GPIO_NUM PIN_TRIGGER /* GPIO number for transmitter signal */
#define RMT_RX_CHANNEL 0 /* RMT channel for receiver */
#define RMT_RX_GPIO_NUM PIN_ECHO /* GPIO number for receiver */
#define RMT_CLK_DIV 100 /* RMT counter clock divider */
#define RMT_TX_CARRIER_EN 0 /* Disable carrier */
#define rmt_item32_tIMEOUT_US 9500 /*!< RMT receiver timeout value(us) */

#define RMT_TICK_10_US (80000000/RMT_CLK_DIV/100000) /* RMT counter value for 10 us.(Source clock is APB clock) */
#define ITEM_DURATION(d) ((d & 0x7fff)*10/RMT_TICK_10_US)

#define PIN_TRIGGER 25 // Recommended trigger and echo value
#define PIN_ECHO 26

#if CONFIG_IDF_TARGET_ESP32
static esp_adc_cal_characteristics_t *adc_chars;
//static const adc_channel_t channel1 = ADC_CHANNEL_6;     //ADC1: GPIO34 (A2) = ADC1_GPIO34_CHANNEL     ADC1_CHANNEL_6
static const adc_channel_t channel2 = ADC_CHANNEL_3;     //ADC1: GPIO39 (A3) = ADC1_GPIO39_CHANNEL     ADC1_CHANNEL_3
//static const adc_channel_t channel3 = ADC_CHANNEL_0;     //ADC1: GPIO36 (A4) = ADC1_GPIO36_CHANNEL     ADC1_CHANNEL_0
#elif CONFIG_IDF_TARGET_ESP32S2BETA
//static const adc_channel_t channel1 = ADC_CHANNEL_6;     // GPIO7 if ADC1, GPIO17 if ADC2
static const adc_channel_t channel2 = ADC_CHANNEL_3;     //ADC1: GPIO34 (A2), GPIO39 (A3), GPIO36 (A4)
//static const adc_channel_t channel3 = ADC_CHANNEL_0;     //ADC1: GPIO34 (A2), GPIO39 (A3), GPIO36 (A4)
#endif
static const adc_atten_t atten = ADC_ATTEN_DB_0;
static const adc_unit_t unit = ADC_UNIT_1;

static const char *TAG = "Quest 6";
static const char *payload;

//sensor code
float dist_ultrasonic;


static void check_efuse(void)
{
    //Check TP is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK) {
        //printf("eFuse Two Point: Supported\n");
    } else {
        //printf("eFuse Two Point: NOT supported\n");
    }

    //Check Vref is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF) == ESP_OK) {
      //  printf("eFuse Vref: Supported\n");
    } else {
      //  printf("eFuse Vref: NOT supported\n");
    }
}

static void print_char_val_type(esp_adc_cal_value_t val_type)
{
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
        printf("Characterized using Two Point Value\n");
    } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
        printf("Characterized using eFuse Vref\n");
    } else {
        printf("Characterized using Default Vref\n");
    }
}

static void ultra_sensor(void){ //cat bed
  check_efuse();

  if(unit == ADC_UNIT_1){
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(channel2, atten);
  }else{
    adc2_config_channel_atten((adc2_channel_t)channel2, atten);
  }

  adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
  esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, ADC_WIDTH_BIT_12,DEFAULT_VREF, adc_chars);
  print_char_val_type(val_type);

  while(1){
        uint32_t adc_reading2 = 0;
        //Multisampling
        for (int i = 0; i < NO_OF_SAMPLES; i++) {
            if (unit == ADC_UNIT_1) {
                adc_reading2 += adc1_get_raw((adc1_channel_t)channel2);
            } else {
                int raw2;
                adc2_get_raw((adc2_channel_t)channel2, ADC_WIDTH_BIT_12, &raw2);
                adc_reading2 += raw2;
            }
        }
        //R0=10K, B=3435, T0=25 celsius->298.15
        //adc_reading/1000;
        adc_reading2 /= NO_OF_SAMPLES;
        //Ultrasonic Sensor
        dist_ultrasonic = (adc_reading2) * 5;
        dist_ultrasonic /= 10;


        //printf("Raw: %d\tVoltage: %d mV\n",adc_reading2, voltage);
        printf("Ultra1: %f\n", dist_ultrasonic);
        /*
        if(dist_ultrasonic <= 20){
        //payload = "Cat is napping!"
        //call udp client
      }*/
        vTaskDelay(pdMS_TO_TICKS(3000));

  }
}

static void udp_client_task(void *pvParameters)
{
    char rx_buffer[128];
    char host_ip[] = HOST_IP_ADDR;
    int addr_family = 0;
    int ip_protocol = 0;

    while (1) {

#if defined(CONFIG_EXAMPLE_IPV4)
        struct sockaddr_in dest_addr;
        dest_addr.sin_addr.s_addr = inet_addr(HOST_IP_ADDR);
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(PORT);
        addr_family = AF_INET;
        ip_protocol = IPPROTO_IP;
#elif defined(CONFIG_EXAMPLE_IPV6)
        struct sockaddr_in6 dest_addr = { 0 };
        inet6_aton(HOST_IP_ADDR, &dest_addr.sin6_addr);
        dest_addr.sin6_family = AF_INET6;
        dest_addr.sin6_port = htons(PORT);
        dest_addr.sin6_scope_id = esp_netif_get_netif_impl_index(EXAMPLE_INTERFACE);
        addr_family = AF_INET6;
        ip_protocol = IPPROTO_IPV6;
#elif defined(CONFIG_EXAMPLE_SOCKET_IP_INPUT_STDIN)
        struct sockaddr_in6 dest_addr = { 0 };
        ESP_ERROR_CHECK(get_addr_from_stdin(PORT, SOCK_DGRAM, &ip_protocol, &addr_family, &dest_addr));
#endif

        int sock = socket(addr_family, SOCK_DGRAM, ip_protocol);
        if (sock < 0) {
            ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "Socket created, sending to %s:%d", HOST_IP_ADDR, PORT);

        while (1) {
            sprintf(payload,"%.2f", dist_ultrasonic);
            int err = sendto(sock, payload, strlen(payload), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
            if (err < 0) {
                ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
                break;
            }
            ESP_LOGI(TAG, "Message sent");

            struct sockaddr_in source_addr; // Large enough for both IPv4 or IPv6
            socklen_t socklen = sizeof(source_addr);
            // int len = recvfrom(sock, rx_buffer, sizeof(rx_buffer) - 1, 0, (struct sockaddr *)&source_addr, &socklen);
            //
            // // Error occurred during receiving
            // if (len < 0) {
            //     ESP_LOGE(TAG, "recvfrom failed: errno %d", errno);
            //     break;
            // }
            // // Data received
            // else {
            //     rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string
            //     ESP_LOGI(TAG, "Received %d bytes from %s:", len, host_ip);
            //     ESP_LOGI(TAG, "%s", rx_buffer);
            //     if (strncmp(rx_buffer, "OK: ", 4) == 0) {
            //         ESP_LOGI(TAG, "Received expected message, reconnecting");
            //         break;
            //     }
            // }

            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }

        if (sock != -1) {
            ESP_LOGE(TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        }
    }
    vTaskDelete(NULL);
}



void app_main(void)
{
    // ESP_ERROR_CHECK(nvs_flash_init());
    // ESP_ERROR_CHECK(esp_netif_init());
    // ESP_ERROR_CHECK(esp_event_loop_create_default());
    //
    // /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
    //  * Read "Establishing Wi-Fi or Ethernet Connection" section in
    //  * examples/protocols/README.md for more information about this function.
    //  */
    // ESP_ERROR_CHECK(example_connect());


    //xTaskCreate(udp_client_task, "udp_client", 4096, NULL, 5, NULL);
    xTaskCreate(ultra_sensor, "ultrasonic", 2048, NULL, 5, NULL);

}

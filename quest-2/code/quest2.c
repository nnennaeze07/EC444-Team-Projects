/* Authors: Nnenna Eze, Lesbeth Roque
Quest 2: Tactile Internet
Team 15
10-07-20 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
//#include "i2c_display.c"
#if CONFIG_IDF_TARGET_ESP32
#include "esp_adc_cal.h"
#endif

#define DEFAULT_VREF    1100        //Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES   64          //Multisampling

#if CONFIG_IDF_TARGET_ESP32
static esp_adc_cal_characteristics_t *adc_chars;
static const adc_channel_t channel = ADC_CHANNEL_6;     //ADC1: GPIO34 (A2), GPIO39 (A3), GPIO36 (A4)
//A2 - this is an analog input A2 and also GPI #34. Note it is not an output-capable pin! It uses ADC #1
// A3 - this is an analog input A3 and also GPI #39. Note it is not an output-capable pin! It uses ADC #1
// A4 - this is an analog input A4 and also GPI #36. Note it is not an output-capable pin! It uses ADC #1
#elif CONFIG_IDF_TARGET_ESP32S2BETA
static const adc_channel_t channel = ADC_CHANNEL_6;     // GPIO7 if ADC1, GPIO17 if ADC2
#endif
static const adc_atten_t atten = ADC_ATTEN_DB_0;
static const adc_unit_t unit = ADC_UNIT_1;

#if CONFIG_IDF_TARGET_ESP32

// Global Variables
float temp, dist_ultrasonic, dist_IR;
static void check_efuse(void)
{
    //Check TP is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK) {
        printf("eFuse Two Point: Supported\n");
    } else {
        printf("eFuse Two Point: NOT supported\n");
    }

    //Check Vref is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF) == ESP_OK) {
        printf("eFuse Vref: Supported\n");
    } else {
        printf("eFuse Vref: NOT supported\n");
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
#endif

void app_main(void)
{
     //i2c_example_master_init();
     //i2c_scanner();
#if CONFIG_IDF_TARGET_ESP32
    //Check if Two Point or Vref are burned into eFuse
    check_efuse();
#endif

    //Configure ADC
    if (unit == ADC_UNIT_1) {
        adc1_config_width(ADC_WIDTH_BIT_12);
        adc1_config_channel_atten(channel, atten);
    } else {
        adc2_config_channel_atten((adc2_channel_t)channel, atten);
    }

#if CONFIG_IDF_TARGET_ESP32
    //Characterize ADC
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);
    print_char_val_type(val_type);
#endif

    while (1) {
        uint32_t adc_reading = 0;
        //Multisampling
        for (int i = 0; i < NO_OF_SAMPLES; i++) {
            if (unit == ADC_UNIT_1) {
                adc_reading += adc1_get_raw((adc1_channel_t)channel);
            } else {
                int raw;
                adc2_get_raw((adc2_channel_t)channel, ADC_WIDTH_BIT_12, &raw);
                adc_reading += raw;
            }
        }
        //R0=10K, B=3435, T0=25 celsius->298.15
        //adc_reading/1000;
        adc_reading /= NO_OF_SAMPLES;
#if CONFIG_IDF_TARGET_ESP32
        //Convert adc_reading to voltage in mV
        float Beta = 1.0/3435;
        float Tinit = 1.0/(298.15);
        temp = (float)adc_reading/10000; //thermistor resistor
        temp = log10(temp);
        temp = temp*Beta; //B coefficient given
        temp = temp + Tinit; //273.15K = 25C
        temp = 1.0/temp;
        temp -= 273.15;
        //code derived from reference below
        //https://learn.adafruit.com/thermistor/using-a-thermistor

        dist_ultrasonic = (adc_reading) * 5;
        dist_ultrasonic/=1000;
        // Equation taken from data sheet for the analog input
        // https://www.maxbotix.com/documents/HRLV-MaxSonar-EZ_Datasheet.pdf
        uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);
        dist_IR = ((16569/((float)voltage + 25)) - 11)/1000;
        //for skill 15, change atten value to 2.5
        //code derived from reference below (skill 15)
        //http://home.roboticlab.eu/en/examples/sensor/ir_distance
        //https://www.limulo.net/website/coding/physical-computing/sharp-linearization.html

        //sprintf(str,"%hu",voltage);
        //Temperature
        printf("%f,%f,%f\n",temp, dist_ultrasonic, dist_IR);
        //printf("Raw: %d\tVoltage: %dmV\n", adc_reading, voltage);
#elif CONFIG_IDF_TARGET_ESP32S2BETA
        //printf("ADC%d CH%d Raw: %d\t\n", unit, channel, adc_reading);
#endif
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

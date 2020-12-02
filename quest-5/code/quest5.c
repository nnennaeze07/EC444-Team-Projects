/* Authors: Nnenna Eze, Lesbeth Roque
  Date: 12/01/2020
  Quest 5, Team 15
*/
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "driver/i2c.h"
#include <sys/param.h>
#include "esp_types.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_adc_cal.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "freertos/portmacro.h"
#include "driver/periph_ctrl.h"
#include "driver/ledc.h"
#include "driver/pcnt.h"
#include "driver/rmt.h"
#include "esp_attr.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "driver/timer.h"

#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"
#include "soc/timer_group_struct.h"
#include "soc/rmt_reg.h"
#include "soc/uart_reg.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include <lwip/netdb.h>
//#include "addr_from_stdin.h" //remember to add to this folder

static const uint16_t alphafonttable[] = {

    0b0000000000000001, 0b0000000000000010, 0b0000000000000100,
    0b0000000000001000, 0b0000000000010000, 0b0000000000100000,
    0b0000000001000000, 0b0000000010000000, 0b0000000100000000,
    0b0000001000000000, 0b0000010000000000, 0b0000100000000000,
    0b0001000000000000, 0b0010000000000000, 0b0100000000000000,
    0b1000000000000000, 0b0000000000000000, 0b0000000000000000,
    0b0000000000000000, 0b0000000000000000, 0b0000000000000000,
    0b0000000000000000, 0b0000000000000000, 0b0000000000000000,
    0b0001001011001001, 0b0001010111000000, 0b0001001011111001,
    0b0000000011100011, 0b0000010100110000, 0b0001001011001000,
    0b0011101000000000, 0b0001011100000000,
    0b0000000000000000, //
    0b0000000000000110, // !
    0b0000001000100000, // "
    0b0001001011001110, // #
    0b0001001011101101, // $
    0b0000110000100100, // %
    0b0010001101011101, // &
    0b0000010000000000, // '
    0b0010010000000000, // (
    0b0000100100000000, // )
    0b0011111111000000, // *
    0b0001001011000000, // +
    0b0000100000000000, // ,
    0b0000000011000000, // -
    0b0000000000000000, // .
    0b0000110000000000, // /
    0b0000110000111111, // 0
    0b0000000000000110, // 1
    0b0000000011011011, // 2
    0b0000000010001111, // 3
    0b0000000011100110, // 4
    0b0010000001101001, // 5
    0b0000000011111101, // 6
    0b0000000000000111, // 7
    0b0000000011111111, // 8
    0b0000000011101111, // 9
    0b0001001000000000, // :
    0b0000101000000000, // ;
    0b0010010000000000, // <
    0b0000000011001000, // =
    0b0000100100000000, // >
    0b0001000010000011, // ?
    0b0000001010111011, // @
    0b0000000011110111, // A
    0b0001001010001111, // B
    0b0000000000111001, // C
    0b0001001000001111, // D
    0b0000000011111001, // E
    0b0000000001110001, // F
    0b0000000010111101, // G
    0b0000000011110110, // H
    0b0001001000000000, // I
    0b0000000000011110, // J
    0b0010010001110000, // K
    0b0000000000111000, // L
    0b0000010100110110, // M
    0b0010000100110110, // N
    0b0000000000111111, // O
    0b0000000011110011, // P
    0b0010000000111111, // Q
    0b0010000011110011, // R
    0b0000000011101101, // S
    0b0001001000000001, // T
    0b0000000000111110, // U
    0b0000110000110000, // V
    0b0010100000110110, // W
    0b0010110100000000, // X
    0b0001010100000000, // Y
    0b0000110000001001, // Z
    0b0000000000111001, // [
    0b0010000100000000, //
    0b0000000000001111, // ]
    0b0000110000000011, // ^
    0b0000000000001000, // _
    0b0000000100000000, // `
    0b0001000001011000, // a
    0b0010000001111000, // b
    0b0000000011011000, // c
    0b0000100010001110, // d
    0b0000100001011000, // e
    0b0000000001110001, // f
    0b0000010010001110, // g
    0b0001000001110000, // h
    0b0001000000000000, // i
    0b0000000000001110, // j
    0b0011011000000000, // k
    0b0000000000110000, // l
    0b0001000011010100, // m
    0b0001000001010000, // n
    0b0000000011011100, // o
    0b0000000101110000, // p
    0b0000010010000110, // q
    0b0000000001010000, // r
    0b0010000010001000, // s
    0b0000000001111000, // t
    0b0000000000011100, // u
    0b0010000000000100, // v
    0b0010100000010100, // w
    0b0010100011000000, // x
    0b0010000000001100, // y
    0b0000100001001000, // z
    0b0000100101001001, // {
    0b0001001000000000, // |
    0b0010010010001001, // }
    0b0000010100100000, // ~
    0b0011111111111111,

};

#define PCNT_TEST_UNIT      PCNT_UNIT_0
#define PCNT_H_LIM_VAL      90000
#define PCNT_L_LIM_VAL     -90000
#define PCNT_THRESH1_VAL    6
#define PCNT_THRESH0_VAL   -6
#define PCNT_INPUT_SIG_IO   34//4  // Pulse Input GPIO
#define PCNT_INPUT_CTRL_IO  5  // Control GPIO HIGH=count up, LOW=count down


#define TIMER_DIVIDER         16    //  Hardware timer clock divider
#define TIMER_SCALE           (TIMER_BASE_CLK / TIMER_DIVIDER)  // to seconds
#define TIMER_INTERVAL_SEC   (1)    // Sample test interval for the first timer
#define TEST_WITH_RELOAD      1     // Testing will be done with auto reload


//You can get these value from the datasheet of servo you use, in general pulse width varies between 1000 to 2000 mocrosecond
#define SERVO_MIN_PULSEWIDTH 1000 //Minimum pulse width in microsecond
#define SERVO_MAX_PULSEWIDTH 2000 //Maximum pulse width in microsecond
#define SERVO_MAX_DEGREE 30 //Maximum angle in degree upto which servo can rotate

//defined LED pins
#define RLED 14
#define GLED 32
#define BLED 15

// Master I2C
#define I2C_EXAMPLE_MASTER_SCL_IO          22   // gpio number for i2c clk
#define I2C_EXAMPLE_MASTER_SDA_IO          23   // gpio number for i2c data
#define I2C_EXAMPLE_MASTER_NUM             I2C_NUM_0  // i2c port
#define I2C_EXAMPLE_MASTER_TX_BUF_DISABLE  0    // i2c master no buffer needed
#define I2C_EXAMPLE_MASTER_RX_BUF_DISABLE  0    // i2c master no buffer needed
#define I2C_EXAMPLE_MASTER_FREQ_HZ         100000     // i2c master clock freq, maybe change back to 100000
#define WRITE_BIT                          I2C_MASTER_WRITE // i2c master write
#define READ_BIT                           I2C_MASTER_READ  // i2c master read
#define ACK_CHECK_EN                       true // i2c master will check ack
#define ACK_CHECK_DIS                      false// i2c master will not check ack
#define ACK_VAL                            0x00 // i2c ack value
#define NACK_VAL                           0xFF // i2c nack value

// ADXL343
#define SLAVE_ADDR                         0x62 //for LIDAR
#define SLAVE_ADDR2                        0x70 //for alphanumeric display
#define OSC                                0x21 // oscillator cmd
#define HT16K33_BLINK_DISPLAYON            0x01 // Display on cmd
#define HT16K33_BLINK_OFF                  0    // Blink off cmd
#define HT16K33_BLINK_CMD                  0x80 // Blink cmd
#define HT16K33_CMD_BRIGHTNESS             0xE0 // Brightness cmd
#define REG1                               0x00
#define REGVAL                             0x04
#define REGHL                              0x8f

#define DEFAULT_VREF                        1100
#define NO_OF_SAMPLES                       64

#define PORT CONFIG_EXAMPLE_PORT

static const char *TAG = "Quest 5";



/*the esp on the buggy will act as a server;
it waits for a signal from the client (the laptop)
and then depending on that signal, the esp will make
the car start/stop when appropriate*/

int16_t count = 0;
int16_t cflag = 0;
int16_t prev = 0;
float speed = 0;
float prev_err = 0;
int16_t front_dist = 0;
int dt_complete = 0;
char str[16];
int car_switch = 1; //to decide whether or not to stop the car, 1 for go and 0 for stop


// A simple structure to pass "events" to main task
typedef struct {
    int flag;     // flag for enabling stuff in main code
    uint64_t timer_group;
    uint64_t timer_counter_val;
    int pulsec;
} timer_event_t;

// Initialize queue handler for timer-based events
//xQueueHandle timer_queue;
//xQueueHandle pcnt_evt_queue;   // A queue to handle pulse counter events
//pcnt_isr_handle_t user_isr_handle = NULL; //user's ISR service handle

typedef struct {
    int unit;  // the PCNT unit that originated an interrupt
    uint32_t status; // information on the event type that caused the interrupt
} pcnt_evt_t;

// ISR handler
// static void IRAM_ATTR pcnt_example_intr_handler(void *arg)
// {
//     int pcnt_unit = (int)arg;
//     pcnt_evt_t evt;
//     evt.unit = pcnt_unit;
//     /* Save the PCNT event type that caused an interrupt
//        to pass it to the main program */
//     pcnt_get_event_status(pcnt_unit, &evt.status);
//     xQueueSendFromISR(pcnt_evt_queue, &evt, NULL);
// }

// void IRAM_ATTR timer_group0_isr(void *para) {
//
//   uint64_t timer_counter_val =
//       ((uint64_t) TIMERG0.hw_timer[TIMER_0].cnt_high) << 32
//       | TIMERG0.hw_timer[TIMER_0].cnt_low;
//     // Prepare basic event data, aka set flag
//     timer_event_t evt;
//     evt.timer_group = 0;
//     // evt.timer_idx = timer_idx;
//     evt.timer_counter_val = timer_counter_val;
//     evt.pulsec = count - prev;
//
//     prev = count;
//
//     dt_complete = 1;
//
//     // Clear the interrupt, Timer 0 in group 0
//     TIMERG0.int_clr_timers.t0 = 1;
//
//     // After the alarm triggers, we need to re-enable it to trigger it next time
//     TIMERG0.hw_timer[TIMER_0].config.alarm_en = TIMER_ALARM_EN;
//
//     // Send the event data back to the main program task
//     xQueueSendFromISR(timer_queue, &evt, NULL);
// }

// static void pcnt_example_init(void)
// {
//     /* Prepare configuration for the PCNT unit */
//     pcnt_config_t pcnt_config = {
//         // Set PCNT input signal and control GPIOs
//         .pulse_gpio_num = PCNT_INPUT_SIG_IO,
//         .ctrl_gpio_num = PCNT_INPUT_CTRL_IO,
//         .channel = PCNT_CHANNEL_0,
//         .unit = PCNT_TEST_UNIT,
//         // What to do on the positive / negative edge of pulse input?
//         .pos_mode = PCNT_COUNT_INC,   // Count up on the positive edge
//         .neg_mode = PCNT_COUNT_DIS,   // Keep the counter value on the negative edge
//         // What to do when control input is low or high?
//         .lctrl_mode = PCNT_MODE_REVERSE, // Reverse counting direction if low
//         .hctrl_mode = PCNT_MODE_KEEP,    // Keep the primary counter mode if high
//         // Set the maximum and minimum limit values to watch
//         .counter_h_lim = PCNT_H_LIM_VAL,
//         .counter_l_lim = PCNT_L_LIM_VAL,
//     };
//     /* Initialize PCNT unit */
//     pcnt_unit_config(&pcnt_config);
//
//     /* Configure and enable the input filter */
//     pcnt_set_filter_value(PCNT_TEST_UNIT, 30);
//     pcnt_filter_enable(PCNT_TEST_UNIT);
//
//     /* Set threshold 0 and 1 values and enable events to watch */
//     // pcnt_set_event_value(unit, PCNT_EVT_THRES_1, PCNT_THRESH1_VAL);
//     // pcnt_event_enable(unit, PCNT_EVT_THRES_1);
//     // pcnt_set_event_value(unit, PCNT_EVT_THRES_0, PCNT_THRESH0_VAL);
//     // pcnt_event_enable(unit, PCNT_EVT_THRES_0);
//     // /* Enable events on zero, maximum and minimum limit values */
//     // pcnt_event_enable(unit, PCNT_EVT_ZERO);
//     // pcnt_event_enable(unit, PCNT_EVT_H_LIM);
//     // pcnt_event_enable(unit, PCNT_EVT_L_LIM);
//
//     /* Initialize PCNT's counter */
//     pcnt_counter_pause(PCNT_TEST_UNIT);
//     pcnt_counter_clear(PCNT_TEST_UNIT);
//
//     /* Install interrupt service and add isr callback handler */
//     // pcnt_isr_service_install(0);
//     // pcnt_isr_handler_add(unit, pcnt_example_intr_handler, (void *)unit);
//
//     /* Everything is set up, now go to counting */
//     pcnt_counter_resume(PCNT_TEST_UNIT);
// }

// static void alarm_init() {
//     /* Select and initialize basic parameters of the timer */
//     timer_config_t config;
//     config.divider = TIMER_DIVIDER;
//     config.counter_dir = TIMER_COUNT_UP;
//     config.counter_en = TIMER_PAUSE;
//     config.alarm_en = TIMER_ALARM_EN;
//     config.intr_type = TIMER_INTR_LEVEL;
//     config.auto_reload = TEST_WITH_RELOAD; //auto_reload;
//     timer_init(TIMER_GROUP_0, TIMER_0, &config);
//
//     // Timer's counter will initially start from value below
//     timer_set_counter_value(TIMER_GROUP_0, TIMER_0, 0x00000000ULL);
//
//     // Configure the alarm value and the interrupt on alarm
//     timer_set_alarm_value(TIMER_GROUP_0, TIMER_0, TIMER_INTERVAL_SEC * TIMER_SCALE);
//     timer_enable_intr(TIMER_GROUP_0, TIMER_0);
//     timer_isr_register(TIMER_GROUP_0, TIMER_0, timer_group0_isr,
//         (void *) TIMER_0, ESP_INTR_FLAG_IRAM, NULL);
//
//     // Start timer
//     timer_start(TIMER_GROUP_0, TIMER_0);
// }

static void i2c_master_init(){
  // Debug
  printf("\n>> i2c Config\n");
  int err;

  // Port configuration
  int i2c_master_port = I2C_EXAMPLE_MASTER_NUM;

  /// Define I2C configurations
  i2c_config_t conf;
  conf.mode = I2C_MODE_MASTER;                              // Master mode
  conf.sda_io_num = I2C_EXAMPLE_MASTER_SDA_IO;              // Default SDA pin
  conf.sda_pullup_en = GPIO_PULLUP_ENABLE;                  // Internal pullup
  conf.scl_io_num = I2C_EXAMPLE_MASTER_SCL_IO;              // Default SCL pin
  conf.scl_pullup_en = GPIO_PULLUP_ENABLE;                  // Internal pullup
  conf.master.clk_speed = I2C_EXAMPLE_MASTER_FREQ_HZ;       // CLK frequency
  err = i2c_param_config(i2c_master_port, &conf);           // Configure
  if (err == ESP_OK) {printf("- parameters: ok\n");}

  // Install I2C driver
  err = i2c_driver_install(i2c_master_port, conf.mode,
                     I2C_EXAMPLE_MASTER_RX_BUF_DISABLE,
                     I2C_EXAMPLE_MASTER_TX_BUF_DISABLE, 0);
  if (err == ESP_OK) {printf("- initialized: yes\n");}

  // Data in MSB mode
  i2c_set_data_mode(i2c_master_port, I2C_DATA_MODE_MSB_FIRST, I2C_DATA_MODE_MSB_FIRST);
}

// Utility function to test for I2C device address -- not used in deploy
int testConnection(uint8_t devAddr, int32_t timeout) {
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, (devAddr << 1) | I2C_MASTER_WRITE, ACK_CHECK_EN);
  i2c_master_stop(cmd);
  int err = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
  i2c_cmd_link_delete(cmd);
  return err;
}

// Utility function to scan for i2c device
static void i2c_scanner() {
  int32_t scanTimeout = 1000;
  printf("\n>> I2C scanning ..."  "\n");
  uint8_t count = 0;
  for (uint8_t i = 1; i < 127; i++) {
    // printf("0x%X%s",i,"\n");
    if (testConnection(i, scanTimeout) == ESP_OK) {
      printf( "- Device found at address: 0x%X%s", i, "\n");
      count++;
    }
  }
  if (count == 0) {printf("- No I2C devices found!" "\n");}
}


static void mcpwm_example_gpio_initialize(void)
{
    printf("initializing mcpwm servo control gpio......\n");
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0B, 18);    //Set GPIO 18(esc) or 5(steering) as PWM0B, to which servo is connected
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, 5);
}

// Turn on oscillator for alpha display
int alpha_oscillator() {
  int ret;
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, ( SLAVE_ADDR2 << 1 ) | WRITE_BIT, ACK_CHECK_EN);
  i2c_master_write_byte(cmd, OSC, ACK_CHECK_EN);
  i2c_master_stop(cmd);
  ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
  i2c_cmd_link_delete(cmd);
  vTaskDelay(200 / portTICK_RATE_MS);
  return ret;
}

// Set blink rate to off
int no_blink() {
  int ret;
  i2c_cmd_handle_t cmd2 = i2c_cmd_link_create();
  i2c_master_start(cmd2);
  i2c_master_write_byte(cmd2, ( SLAVE_ADDR2 << 1 ) | WRITE_BIT, ACK_CHECK_EN);
  i2c_master_write_byte(cmd2, HT16K33_BLINK_CMD | HT16K33_BLINK_DISPLAYON | (HT16K33_BLINK_OFF << 1), ACK_CHECK_EN);
  i2c_master_stop(cmd2);
  ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd2, 1000 / portTICK_RATE_MS);
  i2c_cmd_link_delete(cmd2);
  vTaskDelay(200 / portTICK_RATE_MS);
  return ret;
}

// Set Brightness
int set_brightness_max(uint8_t val) {
  int ret;
  i2c_cmd_handle_t cmd3 = i2c_cmd_link_create();
  i2c_master_start(cmd3);
  i2c_master_write_byte(cmd3, ( SLAVE_ADDR2 << 1 ) | WRITE_BIT, ACK_CHECK_EN);
  i2c_master_write_byte(cmd3, HT16K33_CMD_BRIGHTNESS | val, ACK_CHECK_EN);
  i2c_master_stop(cmd3);
  ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd3, 1000 / portTICK_RATE_MS);
  i2c_cmd_link_delete(cmd3);
  vTaskDelay(200 / portTICK_RATE_MS);
  return ret;
}


static void test_alpha_display() {
    // Debug
    int ret;
    printf(">> Test Alphanumeric Display: \n");
    //i2c_scanner();
    // Set up routines
    // Turn on alpha oscillator
    ret = alpha_oscillator();
    if(ret == ESP_OK) {printf("- oscillator: ok \n");}
    // Set display blink off
    ret = no_blink();
    if(ret == ESP_OK) {printf("- blink: off \n");}
    ret = set_brightness_max(0xF);
    if(ret == ESP_OK) {printf("- brightness: max \n");}

    // Write to characters to buffer
    //printf("Enter a \n", );
    uint16_t displaybuffer[8];

    // for(int i = 0;i < 8;i++){
    //   displaybuffer[i] = alphafonttable[(uint16_t)str[i]];
    // }

    // Continually writes the same command
    while (1) {

      for(int i = 0;i < 8;i++){
        displaybuffer[i] = alphafonttable[(uint16_t)str[i]];
      }

      // Send commands characters to display over I2C
      i2c_cmd_handle_t cmd4 = i2c_cmd_link_create();
      i2c_master_start(cmd4);
      i2c_master_write_byte(cmd4, ( SLAVE_ADDR2 << 1 ) | WRITE_BIT, ACK_CHECK_EN);
      i2c_master_write_byte(cmd4, (uint8_t)0x00, ACK_CHECK_EN);
      for (uint8_t i=0; i<8; i++) {
        i2c_master_write_byte(cmd4, displaybuffer[i] & 0xFF, ACK_CHECK_EN);
        i2c_master_write_byte(cmd4, displaybuffer[i] >> 8, ACK_CHECK_EN);
      }
      i2c_master_stop(cmd4);
      ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd4, 1000 / portTICK_RATE_MS);
      i2c_cmd_link_delete(cmd4);


    }
}



static uint32_t servo_per_degree_init(uint32_t degree_of_rotation)
{
    uint32_t cal_pulsewidth = 0;
    cal_pulsewidth = (SERVO_MIN_PULSEWIDTH + (((SERVO_MAX_PULSEWIDTH - SERVO_MIN_PULSEWIDTH) * (degree_of_rotation)) / (SERVO_MAX_DEGREE)));
    return cal_pulsewidth;
}



void calibrateESC(){
  vTaskDelay(5000 / portTICK_PERIOD_MS);
  printf("Calibrating...\n");
  printf("Neutral\n");
  mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B, 1400); // NEUTRAL signal in microseconds
  vTaskDelay(3000 / portTICK_PERIOD_MS);
  //mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B, 1400); // reset the ESC to neutral (non-moving) value
  printf("Calibrated\n");
}

// Write one byte to register
int writeRegister(uint8_t reg, uint8_t data) {
  /*  1. Master write Start
      2. Master write Slave address + Write bit
      3. Master waits for ACK from Slave
      4. Master write register address
      5. Master waits for ACK from Slave
      6. Master writes data
      7. Master waits for ACK from Slave
      8. Master write Stop
      */
      int ret;
      i2c_cmd_handle_t cmd = i2c_cmd_link_create();
      i2c_master_start(cmd); //Start
      i2c_master_write_byte(cmd, (SLAVE_ADDR << 1) | WRITE_BIT, ACK_CHECK_EN);
      i2c_master_write_byte(cmd, reg, ACK_CHECK_EN);
      i2c_master_write_byte(cmd, data, ACK_CHECK_EN);
      i2c_master_stop(cmd);

      ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
      i2c_cmd_link_delete(cmd);
      return ret;
}

// Read register
uint8_t readRegister(uint8_t reg) {
  // YOUR CODE HERE
  // Assuming single-byte read
  /*  1. Master write Start
      2. Master write Slave address + Write bit
      3. Master waits for ACK from Slave
      4. Master write register address
      5. Master waits for ACK from Slave
      6. Master write Start again
      7. Master write Slave address + Read bit
      8. Master waits for ACK from Slave
      9. Master waits for Data from Slave <--------------but how
      10. Master writes NACK
      11. Master writes Stop
      */
      uint8_t data;
      i2c_cmd_handle_t cmd = i2c_cmd_link_create();
      i2c_master_start(cmd);
      i2c_master_write_byte(cmd, (SLAVE_ADDR << 1) | WRITE_BIT, ACK_CHECK_EN);
      i2c_master_write_byte(cmd, reg, ACK_CHECK_EN);
      i2c_master_start(cmd);
      i2c_master_write_byte(cmd, (SLAVE_ADDR << 1) | READ_BIT, ACK_CHECK_EN);
      i2c_master_read_byte(cmd, &data, ACK_CHECK_DIS);
      i2c_master_stop(cmd);
      i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
      i2c_cmd_link_delete(cmd);
      return data;
}

// read 16 bits (2 bytes)
int16_t read16(uint8_t reg) {
    uint8_t data1, data2;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_cmd_handle_t cmd2 = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (SLAVE_ADDR << 1) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, reg, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 100 / portTICK_RATE_MS);

    i2c_master_start(cmd2);
    i2c_master_write_byte(cmd2, (SLAVE_ADDR << 1) | READ_BIT, ACK_CHECK_EN);
    i2c_master_read_byte(cmd2, &data1, ACK_VAL);
    i2c_master_read_byte(cmd2, &data2, ACK_VAL);
    i2c_master_stop(cmd2);
    i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd2, 100 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    i2c_cmd_link_delete(cmd2);

    int16_t temp = data1 << 8;
    int16_t data = temp | data2;
    return (data - 5);//data - 30
}

void mcpwm_example_servo_control(void *arg)
{
    uint32_t angle, servo_count;
    //1. mcpwm gpio initialization
    mcpwm_example_gpio_initialize();
    printf("Configuring Initial Parameters of mcpwm......\n");
    mcpwm_config_t pwm_config;
    pwm_config.frequency = 50;    //frequency = 50Hz, i.e. for every servo motor time period should be 20ms
    pwm_config.cmpr_a = 0;    //duty cycle of PWMxA = 0
    pwm_config.cmpr_b = 0;    //duty cycle of PWMxb = 0
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);    //Configure PWM0A & PWM0B with above settings
    printf("Finished configurations\n");

    //2. initial mcpwm configuration
    calibrateESC();
    while (1) {

      //for(speed = 1400; speed < 2100; speed+=5){
        printf("Running\n");
        if(!car_switch){
          mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B, 1400); //car is stopped OR we can make car go backwards and turn
          printf("Car stopped\n");
        }else{
          mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B, 1500);

          //for steering servo
            // for (count = 0; count < SERVO_MAX_DEGREE; count++) { //moves left
            //     //printf("Angle of rotation: %d\n", count);
            //     angle = servo_per_degree_init(count);
            //     //printf("pulse width: %dus\n", angle);
            //     mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, angle);
            //     vTaskDelay(10);     //Add delay, since it takes time for servo to rotate, generally 100ms/60degree rotation at 5V
            // }
            //
            // for (count = SERVO_MAX_DEGREE - 1; count > 0; count--) { //moves back to center
            //     //printf("Angle of rotation: %d\n", count);
            //     angle = servo_per_degree_init(count);
            //     //printf("pulse width: %dus\n", angle);
            //     mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, angle);
            //     vTaskDelay(10);     //Add delay, since it takes time for servo to rotate, generally 100ms/60degree rotation at 5V
            // }
            printf("Done\n");
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS);
      //}
    }
}

//task to read lidar data
void lidar_task(){
  printf("\n>> Polling Lidar\n");
  while(1){
    writeRegister(REG1, REGVAL);
    //uint8_t lsb;
    front_dist = read16(REGHL);
    printf("data reading: %u cm\n",front_dist);
    if(front_dist <= 20) //stop within 20cm of wall
      car_switch = 0;
    //add else statement to make car start again if dist is greater than 20cm
    sprintf(str,"%d",front_dist);
    //printf("%s\n",str);
    vTaskDelay(500 / portTICK_RATE_MS);
  }
}




int error = 0;
int setpoint = 50; //50cm
int integral = 0;
double output = 0;
double dt = 100; //100ms
double derivative = 0;
int previous_error = 0;
double kp = 1; //1
double ki = 0;//0;
double kd = 0;//0;


// void PID_task(){
//   if(car_switch){
//      error = setpoint - measured_value;
//      integral = integral + error * dt;
//      derivative = (error - previous_error) / dt;
//      output = kp * error + ki * integral + kd * derivative;
//      previous_error = error;
//   }
// }

static void udp_server_task(void *pvParameters){
  char rx_buffer[8]; //this will be start/stop signal
  char addr_str[128];
  int addr_family = (int)pvParameters;
  int ip_protocol = 0;
  struct sockaddr_in6 dest_addr;

  while (1) {

        if (addr_family == AF_INET) {
            struct sockaddr_in *dest_addr_ip4 = (struct sockaddr_in *)&dest_addr;
            dest_addr_ip4->sin_addr.s_addr = htonl(INADDR_ANY);
            dest_addr_ip4->sin_family = AF_INET;
            dest_addr_ip4->sin_port = htons(PORT);
            ip_protocol = IPPROTO_IP;
        } else if (addr_family == AF_INET6) {
            bzero(&dest_addr.sin6_addr.un, sizeof(dest_addr.sin6_addr.un));
            dest_addr.sin6_family = AF_INET6;
            dest_addr.sin6_port = htons(PORT);
            ip_protocol = IPPROTO_IPV6;
        }

        int sock = socket(addr_family, SOCK_DGRAM, ip_protocol);
        if (sock < 0) {
            ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "Socket created");

        int err = bind(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
        if (err < 0) {
            ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
        }
        ESP_LOGI(TAG, "Socket bound, port %d", PORT);

        while (1) {

            ESP_LOGI(TAG, "Waiting for data");
            struct sockaddr_in6 source_addr; // Large enough for both IPv4 or IPv6
            socklen_t socklen = sizeof(source_addr);
            int len = recvfrom(sock, rx_buffer, sizeof(rx_buffer) - 1, 0, (struct sockaddr *)&source_addr, &socklen);

            // Error occurred during receiving
            if (len < 0) {
                ESP_LOGE(TAG, "recvfrom failed: errno %d", errno);
                break;
            }
            // Data received
            else {
                // Get the sender's ip address as string
                if (source_addr.sin6_family == PF_INET) {
                    inet_ntoa_r(((struct sockaddr_in *)&source_addr)->sin_addr.s_addr, addr_str, sizeof(addr_str) - 1);
                } else if (source_addr.sin6_family == PF_INET6) {
                    inet6_ntoa_r(source_addr.sin6_addr, addr_str, sizeof(addr_str) - 1);
                }

                rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string...
                ESP_LOGI(TAG, "Received %d bytes from %s:", len, addr_str);
                ESP_LOGI(TAG, "%s", rx_buffer);
                if(strcmp(rx_buffer, "go")){
                  car_switch  = 1;
                }else if(strcmp(rx_buffer, "stop")){
                  car_switch = 0;
                }

                int err = sendto(sock, rx_buffer, len, 0, (struct sockaddr *)&source_addr, sizeof(source_addr));
                if (err < 0) {
                    ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
                    break;
                }
                //maybe send back message received? OR status of car
            }
        }

        if (sock != -1) {
            ESP_LOGE(TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        }
    }
    vTaskDelete(NULL);
}


// The main task of this example program
// static void timer_evt_task(void *arg) {
//     while (1) {
//         // Create dummy structure to store structure from queue
//         timer_event_t evt;
//
//         // Transfer from queue
//         xQueueReceive(timer_queue, &evt, portMAX_DELAY);
//
//         // Do something if triggered!
//         if (dt_complete == 1) {
//             printf("Action!\n");
//             //PID_task();
//             dt_complete = 0;
//
//             TIMERG0.hw_timer[TIMER_0].config.alarm_en = 1;
//         }
//     }
// }

// static void optical_encoder(void) {
//   //int16_t flag = 0;
//   //int16_t co;
//   pcnt_evt_t evt;
//   portBASE_TYPE res;
//
//   while(1){
//     res = xQueueReceive(pcnt_evt_queue, &evt, 50 / portTICK_PERIOD_MS);
//     pcnt_get_counter_value(PCNT_TEST_UNIT, &count);
//     printf("Current counter value :%d\n", count);
//   }
//
//   // if(user_isr_handle){ //may not need this
//   //   esp_intr_free(user_isr_handle);
//   //   user_isr_handle = NULL;
//   // }
// }


void app_main(void)
{
  // Routine
  i2c_master_init();
  i2c_scanner();

  ESP_ERROR_CHECK(nvs_flash_init());
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());
//  ESP_ERROR_CHECK(example_connect());
  // Create a FIFO queue for timer-based
  // pcnt_example_init();
  // pcnt_evt_queue = xQueueCreate(10, sizeof(pcnt_evt_t));
  // timer_queue = xQueueCreate(10, sizeof(timer_event_t));

  //printf("Testing servo motor.......\n");
  xTaskCreate(mcpwm_example_servo_control, "mcpwm_example_servo_control", 4096, NULL, 5, NULL);
  xTaskCreate(lidar_task, "lidar_task", 4096, NULL, 5, NULL);
  xTaskCreate(test_alpha_display, "test_alpha_display", 4096, NULL, 5, NULL);
  xTaskCreate(udp_server_task, "udp_server", 4095, (void*)AF_INET, 5, NULL);
  // Create task to handle timer-based events
  //xTaskCreate(timer_evt_task, "timer_evt_task", 2048, NULL, 5, NULL);
  //xTaskCreate(optical_encoder, "optical_encoder", 2048, NULL, 5, NULL);
  // Initiate alarm using timer API
  //alarm_init();
}

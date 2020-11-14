//Code for Quest 4
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"
#include "driver/rmt.h"
#include "soc/rmt_reg.h"
#include "soc/uart_reg.h"
#include "driver/uart.h"
#include "driver/periph_ctrl.h"
#include "driver/timer.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>
#include "addr_from_stdin.h"

//set up address of fob the data is being sent to
#if defined(CONFIG_EXAMPLE_IPV4)
#define HOST_IP_ADDR "192.168.1.74"// change to ip address of poll leader fob
#elif defined(CONFIG_EXAMPLE_IPV6)
#define HOST_IP_ADDR "192.168.1.74" //CONFIG_EXAMPLE_IPV6_ADDR
#else
#define HOST_IP_ADDR "192.168.1.74" //or "192.168.1.124", maybe "192.168.1.132"
#endif

//#define DEV_IP_ADDR "192.168.1.124"//"192.168.1.124" //DEV_IP_ADDR is ip address of this device
#define ID 8 //id of this fob, based on ip address, change for each fob

#define PORT CONFIG_EXAMPLE_PORT //port 3333

// RMT definitions
#define RMT_TX_CHANNEL    1     // RMT channel for transmitter
#define RMT_TX_GPIO_NUM   25    // GPIO number for transmitter signal -- A1
#define RMT_CLK_DIV       100   // RMT counter clock divider
#define RMT_TICK_10_US    (80000000/RMT_CLK_DIV/100000)   // RMT counter value for 10 us.(Source clock is APB clock)
#define rmt_item32_tIMEOUT_US   9500     // RMT receiver timeout value(us)

// UART definitions
#define UART_TX_GPIO_NUM 26 // A0
#define UART_RX_GPIO_NUM 36 // A2
#define BUF_SIZE (1024)

// Hardware interrupt definitions
#define GPIO_INPUT_IO_1       4
#define ESP_INTR_FLAG_DEFAULT 0
#define GPIO_INPUT_PIN_SEL    1ULL<<GPIO_INPUT_IO_1

// LED Output pins definitions
#define BLUEPIN   14
#define GREENPIN  32
#define REDPIN    15
#define ONBOARD   13
//#define PUSHBUTTON 39

#define TIMER_DIVIDER         16    //  Hardware timer clock divider
#define TIMER_SCALE           (TIMER_BASE_CLK / TIMER_DIVIDER)  // to seconds
#define TIMER_INTERVAL_2_SEC  (2)
#define TIMER_INTERVAL_10_SEC (10)
#define TEST_WITH_RELOAD      1     // Testing will be done with auto reload


#define COLOR 'R'
#define POLL_LEAD 0 //not necessary?


// Variables for my ID, minVal and status plus string fragments
char start = 0x1B;
char myID = (char) ID;
char myColor = (char) COLOR;
int len_out = 4;
char vote;
char buffer[4];
int flagg = 0;
//int wrt = 0; //to determine whether txt file should be written into

//initialize setup for writing to file
// FILE *fp;


// Mutex (for resources), and Queues (for button)
SemaphoreHandle_t mux = NULL;
static xQueueHandle gpio_evt_queue = NULL;
static xQueueHandle timer_queue;

// A simple structure to pass "events" to main task
typedef struct {
    int flag;     // flag for enabling stuff in timer task
} timer_event_t;

// System tags
static const char *TAG_SYSTEM = "system";       // For debug logs
static const char *TAG = "example";
static const char *payload = "connected";

// Button interrupt handler -- add to queue
static void IRAM_ATTR gpio_isr_handler(void* arg){
  uint32_t gpio_num = (uint32_t) arg;
  xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

// ISR handler
void IRAM_ATTR timer_group0_isr(void *para) {

    // Prepare basic event data, aka set flag
    timer_event_t evt;
    evt.flag = 1;

    // Yellow is shorter
    if (myColor == 'G') {
      timer_set_alarm_value(TIMER_GROUP_0, TIMER_0, TIMER_INTERVAL_2_SEC * TIMER_SCALE);
    }
    else {
      timer_set_alarm_value(TIMER_GROUP_0, TIMER_0, TIMER_INTERVAL_10_SEC * TIMER_SCALE);
    }

    // Clear the interrupt, Timer 0 in group 0
    TIMERG0.int_clr_timers.t0 = 1;

    // After the alarm triggers, we need to re-enable it to trigger it next time
    TIMERG0.hw_timer[TIMER_0].config.alarm_en = TIMER_ALARM_EN;

    // Send the event data back to the main program task
    xQueueSendFromISR(timer_queue, &evt, NULL);
}

// Utilities ///////////////////////////////////////////////////////////////////

// Checksum
char genCheckSum(char *p, int len) {
  char temp = 0;
  for (int i = 0; i < len; i++){
    temp = temp^p[i];
  }
  // printf("%X\n",temp);

  return temp;
}
bool checkCheckSum(uint8_t *p, int len) {
  char temp = (char) 0;
  bool isValid;
  for (int i = 0; i < len-1; i++){
    temp = temp^p[i];
  }
  // printf("Check: %02X ", temp);
  if (temp == p[len-1]) {
    isValid = true; }
  else {
    isValid = false; }
  return isValid;
}

// Init Functions //////////////////////////////////////////////////////////////
// RMT tx init
static void rmt_tx_init() {
    rmt_config_t rmt_tx;
    rmt_tx.channel = RMT_TX_CHANNEL;
    rmt_tx.gpio_num = RMT_TX_GPIO_NUM;
    rmt_tx.mem_block_num = 1;
    rmt_tx.clk_div = RMT_CLK_DIV;
    rmt_tx.tx_config.loop_en = false;
    rmt_tx.tx_config.carrier_duty_percent = 50;
    // Carrier Frequency of the IR receiver
    rmt_tx.tx_config.carrier_freq_hz = 38000;
    rmt_tx.tx_config.carrier_level = 1;
    rmt_tx.tx_config.carrier_en = 1;
    // Never idle -> aka ontinuous TX of 38kHz pulses
    rmt_tx.tx_config.idle_level = 1;
    rmt_tx.tx_config.idle_output_en = true;
    rmt_tx.rmt_mode = 0;
    rmt_config(&rmt_tx);
    rmt_driver_install(rmt_tx.channel, 0, 0);
}

// Configure UART
static void uart_init() {
  // Basic configs
  uart_config_t uart_config = {
      .baud_rate = 1200, // Slow BAUD rate
      .data_bits = UART_DATA_8_BITS,
      .parity    = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
  };
  uart_param_config(UART_NUM_1, &uart_config);

  // Set UART pins using UART0 default pins
  uart_set_pin(UART_NUM_1, UART_TX_GPIO_NUM, UART_RX_GPIO_NUM, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

  // Reverse receive logic line
  uart_set_line_inverse(UART_NUM_1,UART_SIGNAL_RXD_INV);//UART_INVERSE_RXD);

  // Install UART driver
  uart_driver_install(UART_NUM_1, BUF_SIZE * 2, 0, 0, NULL, 0);
}

// GPIO init for LEDs
static void led_init() {
    gpio_pad_select_gpio(BLUEPIN);
    gpio_pad_select_gpio(GREENPIN);
    gpio_pad_select_gpio(REDPIN);
    gpio_pad_select_gpio(ONBOARD);
    //gpio_pad_select_gpio(PUSHBUTTON);
    gpio_set_direction(BLUEPIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(GREENPIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(REDPIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(ONBOARD, GPIO_MODE_OUTPUT);
    //gpio_set_direction(PUSHBUTTON,GPIO_MODE_INPUT);
}


// Button interrupt init
static void button_init() {
    gpio_config_t io_conf;
    //interrupt of rising edge
    io_conf.intr_type = GPIO_PIN_INTR_POSEDGE;
    //bit mask of the pins, use GPIO4 here
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    //set as input mode
    io_conf.mode = GPIO_MODE_INPUT;
    //enable pull-up mode
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);
    gpio_intr_enable(GPIO_INPUT_IO_1 );
    //install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_LEVEL3);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_INPUT_IO_1, gpio_isr_handler, (void*) GPIO_INPUT_IO_1);
    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    //start gpio task
}

////////////////////////////////////////////////////////////////////////////////

// Tasks ///////////////////////////////////////////////////////////////////////
// Send task -- sends payload | Start | myID | Start | myID
void button_send_task(){ //this will be when second button is implemented, sends data
  //while(1) {
    //printf("Sending...\n");
    char *data_out = (char *) malloc(len_out);
    xSemaphoreTake(mux, portMAX_DELAY);
    data_out[0] = start;
    data_out[1] = (char) myColor;
    data_out[2] = (char) myID;
    data_out[3] = genCheckSum(data_out,len_out-1);
     //ESP_LOG_BUFFER_HEXDUMP(TAG_SYSTEM, data_out, len_out, ESP_LOG_INFO);

    uart_write_bytes(UART_NUM_1, data_out, len_out);
    xSemaphoreGive(mux);

    vTaskDelay(5 / portTICK_PERIOD_MS);
//  }
}


// Button task -- rotate through myIDs
void button_task(){
  uint32_t io_num;
  while(1) {

    if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
      //printf("Action!\n");
      if (myColor == 'R') {
        myColor = 'G';
      }
      else if (myColor == 'G') {
        myColor = 'Y';
      }
      else if (myColor == 'Y') {
        myColor = 'R';
      }
      vote = myColor; //vote will be the same as color chosen
      char *data_out = (char *) malloc(len_out); //this code will be replaced with "button_send_task" when it is implemented
      xSemaphoreTake(mux, portMAX_DELAY);
      data_out[0] = start;
      data_out[1] = (char) myColor;
      data_out[2] = (char) myID;
      data_out[3] = genCheckSum(data_out,len_out-1);
       //ESP_LOG_BUFFER_HEXDUMP(TAG_SYSTEM, data_out, len_out, ESP_LOG_INFO);

      uart_write_bytes(UART_NUM_1, data_out, len_out);
      xSemaphoreGive(mux);

      //button_send_task();
      //wrt = 1;
      //printf("Button pressed.\n");
      //printf("ID: %d, Vote: %c\n",myID,vote);

    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}


// LED task to light LED based on traffic state
void led_task(){
  while(1) {
    switch((int)myColor){
      case 'R' : // Red
        gpio_set_level(GREENPIN, 0);
        gpio_set_level(REDPIN, 1);
        gpio_set_level(BLUEPIN, 0);
        break;
      case 'Y' : // Yellow
        gpio_set_level(GREENPIN, 0);
        gpio_set_level(REDPIN, 0);
        gpio_set_level(BLUEPIN, 1);
        break;
      case 'G' : // Green
        gpio_set_level(GREENPIN, 1);
        gpio_set_level(REDPIN, 0);
        gpio_set_level(BLUEPIN, 0);
        break;
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

// LED task to blink onboard LED based on ID
void id_task(){
  while(1) {
    for (int i = 0; i < (int) myID; i++) {
      gpio_set_level(ONBOARD,1);
      vTaskDelay(200 / portTICK_PERIOD_MS);
      gpio_set_level(ONBOARD,0);
      vTaskDelay(200 / portTICK_PERIOD_MS);
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
static void udp_client_task(void *pvParameters)
{
    //char buffer[4];
    char host_ip[] = HOST_IP_ADDR;
    int addr_family = 0;
    int ip_protocol = 0;

    while (1) {

#if defined(CONFIG_EXAMPLE_IPV4)
        struct sockaddr_in dest_addr;
        dest_addr.sin_addr.s_addr = inet_addr(HOST_IP_ADDR); //how fob figures out where to send data
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
        //if(flagg == 1){ //if 1 data available to send from client to server
          while (1) {

              if(flagg == 1){
                buffer[0] = myID + '0'; //fob id, may change to myID
                buffer[1] = ','; //color voted for
                buffer[2] = vote;
              }
              // printf("Buffer is: \n");
              // printf(buffer);
              int err = sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr)); //sending data to another server
              if (err < 0) {
                  ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
                  break;
              }
              ESP_LOGI(TAG, "Message sent: %s",buffer);

              vTaskDelay(2000 / portTICK_PERIOD_MS);
          }
        //}


        if (sock != -1) {
            //ESP_LOGE(TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        }
    }
    vTaskDelete(NULL);
}

static void udp_server_task(void *pvParameters)
{
  //char buffer[4];
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
          //ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
          break;
      }
      //ESP_LOGI(TAG, "Socket created");

#if defined(CONFIG_EXAMPLE_IPV4) && defined(CONFIG_EXAMPLE_IPV6)
      if (addr_family == AF_INET6) {
          // Note that by default IPV6 binds to both protocols, it is must be disabled
          // if both protocols used at the same time (used in CI)
          int opt = 1;
          setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
          setsockopt(sock, IPPROTO_IPV6, IPV6_V6ONLY, &opt, sizeof(opt));
      }
#endif

      int err = bind(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
      if (err < 0) {
          //ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
      }
      ESP_LOGI(TAG, "Socket bound, port %d", PORT);

      while (1) {

          //ESP_LOGI(TAG, "Waiting for data"); //waits for data from a client
          struct sockaddr_in6 source_addr; // Large enough for both IPv4 or IPv6
          socklen_t socklen = sizeof(source_addr);
          int len = recvfrom(sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&source_addr, &socklen);

          // Error occurred during receiving
          if (len < 0) {
              //ESP_LOGE(TAG, "recvfrom failed: errno %d", errno);
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

              buffer[len] = 0; // Null-terminate whatever we received and treat like a string...
              //ESP_LOGI(TAG, "Received %d bytes from %s:", len, addr_str);
              ESP_LOGI(TAG, "%s", buffer);
              printf("%s",buffer);
          }
      }

      if (sock != -1) {
          //ESP_LOGE(TAG, "Shutting down socket and restarting...");
          shutdown(sock, 0);
          close(sock);
      }
  }
  vTaskDelete(NULL);
}

// Receives task -- looks for Start byte then stores received values
void recv_task(){
  // Buffer for input data
  uint8_t *data_in = (uint8_t *) malloc(BUF_SIZE);
  while (1) {
    int len_in = uart_read_bytes(UART_NUM_1, data_in, BUF_SIZE, 20 / portTICK_RATE_MS);
    if (len_in >0) {
      if (data_in[0] == start) {
        //printf("Data received\n");
        if (checkCheckSum(data_in,len_out)) {
          //ESP_LOG_BUFFER_HEXDUMP(TAG_SYSTEM, data_in, len_out, ESP_LOG_INFO);
          myColor = data_in[1];
          vote = myColor;
          flagg = 1;
        }
      }
    }
    else{
      //printf("Nada :(\n");
       //printf("%X\n",data_in[1]);
    }
    vTaskDelay(5 / portTICK_PERIOD_MS);
  }
  free(data_in);
}

void app_main() {
  //char data[3];
  //initializations for UDP
  ESP_ERROR_CHECK(nvs_flash_init());
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  ESP_ERROR_CHECK(example_connect());
    // Mutex for current values when sending
    mux = xSemaphoreCreateMutex();

    // Create a FIFO queue for timer-based events
    timer_queue = xQueueCreate(10, sizeof(timer_event_t));

    // Create task to handle timer-based events
    //xTaskCreate(timer_evt_task, "timer_evt_task", 2048, NULL, 5, NULL);

    // Initialize all the things
    rmt_tx_init();
    uart_init();
    led_init();
    //alarm_init();
    button_init();




    //change to id of poll leader
    if(ID == 8){
      //printf("Starting task to create server...\n");
      xTaskCreate(udp_server_task, "udp_server", 4096, (void*)AF_INET, 5, NULL);
    }
    if(ID != 1){//change to id of initial fob
      //printf("Starting task to send to server...\n");
      xTaskCreate(udp_client_task, "udp_client", 4096, NULL, 5, NULL);
    }

    // Create tasks for receive, send, set gpio, and button
    xTaskCreate(recv_task, "uart_rx_task", 1024*4, NULL, configMAX_PRIORITIES, NULL);
    //xTaskCreate(send_task, "uart_tx_task", 1024*2, NULL, configMAX_PRIORITIES, NULL);
    xTaskCreate(led_task, "set_traffic_task", 1024*2, NULL, configMAX_PRIORITIES, NULL);
    xTaskCreate(id_task, "set_id_task", 1024*2, NULL, configMAX_PRIORITIES, NULL);
    //xTaskCreate(button_send_task, "button_send_task", 1024*2, NULL, configMAX_PRIORITIES, NULL);
    xTaskCreate(button_task, "button_task", 1024*2, NULL, configMAX_PRIORITIES, NULL);

}

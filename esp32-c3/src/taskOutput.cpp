#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <driver/ledc.h>
#include "./common.h"
#include "./params.h"

void updateColors();

#define LEDC_FREQ 1000

void TaskOutput(void* pvParameters) {
  ledc_timer_config_t ledc_timer = {.speed_mode = LEDC_LOW_SPEED_MODE,
                                    .duty_resolution = LEDC_TIMER_10_BIT,
                                    .timer_num = LEDC_TIMER_1,
                                    .freq_hz = LEDC_FREQ,
                                    .clk_cfg = LEDC_USE_RTC8M_CLK};

  ledc_timer_config(&ledc_timer);

  ledcSetup(0, 1000, 8);
  ledcSetup(1, 1000, 8);
  ledcSetup(2, 1000, 8);
  ledcSetup(3, 1000, 8);
  ledcSetup(4, 1000, 8);
  ledcSetup(5, 1000, 8);
  ledcSetup(6, 1000, 8);
  ledcSetup(7, 1000, 8);
  ledcSetup(8, 1000, 8);

  ledcAttachPin(21, 0);
  ledcAttachPin(7, 1);
  ledcAttachPin(6, 2);

  ledcAttachPin(5, 3);
  ledcAttachPin(4, 4);
  ledcAttachPin(3, 5);

  ledcAttachPin(2, 6);
  ledcAttachPin(20, 7);
  ledcAttachPin(8, 8);

  while (true) {
    updateColors();

    ledcWrite(0, getParameter(PARAM_RED1));    // OUT1
    ledcWrite(1, getParameter(PARAM_GREEN1));  // OUT2
    ledcWrite(2, getParameter(PARAM_BLUE1));   // OUT3

    ledcWrite(3, getParameter(PARAM_RED2));    // OUT4
    ledcWrite(4, getParameter(PARAM_GREEN2));  // OUT5
    ledcWrite(5, getParameter(PARAM_BLUE2));   // OUT6

    ledcWrite(6, getParameter(PARAM_RED3));    // OUT7
    ledcWrite(7, getParameter(PARAM_GREEN3));  // OUT8
    ledcWrite(8, getParameter(PARAM_BLUE3));   // OUT9

    vTaskDelay(40);  // 25 times per seconds
  }
}

void taskOutput() {
  // Now set up two tasks to run independently.
  xTaskCreatePinnedToCore(TaskOutput, "TaskOutput",
                          4096,  // This stack size can be checked & adjusted
                                 // by reading the Stack Highwater
                          NULL,
                          2,  // Priority, with 3 (configMAX_PRIORITIES - 1)
                              // being the highest, and 0 being the lowest.
                          NULL, 1);
}

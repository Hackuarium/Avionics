#include "./common.h"

void TaskBlink(void* pvParameters) {
  (void)pvParameters;

#ifdef LED_ON_BOARD
  pinMode(LED_ON_BOARD, OUTPUT);
#endif

  while (true) {
#ifdef LED_ON_BOARD
    digitalWrite(LED_ON_BOARD, HIGH);
#endif
    vTaskDelay(2);
#ifdef LED_ON_BOARD
    digitalWrite(LED_ON_BOARD, LOW);
#endif
    vTaskDelay(100);
  }
}

void taskBlink() {
  // Now set up two tasks to run independently.
  xTaskCreatePinnedToCore(TaskBlink, "TaskBlink",
                          1024,  // Crash if less than 1024 !!!!
                                 // This stack size can be checked & adjusted by
                                 // reading the Stack Highwater
                          NULL,
                          0,  // Priority, with 3 (configMAX_PRIORITIES - 1)
                              // being the highest, and 0 being the lowest.
                          NULL, 1);
}

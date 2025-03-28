#include "./common.h"
#include "./params.h"

SemaphoreHandle_t xSemaphoreWire = xSemaphoreCreateBinary();

void taskBlink();
void taskSerial();
void taskOutput();
void taskInput();
void taskTimer();
void taskBluetooth();
void taskOneWire();
void taskForecast();
void taskUptime();
void taskWifi();
void taskWifiAP();
void taskOTA();
void taskMDNS();
void taskInternalTemperature();
void taskWebserver();
void taskWire();

void setup() {
  xSemaphoreGive(xSemaphoreWire);
  Serial.begin(115200);  // only for debug purpose

  setParameter(PARAM_STATUS, 0);

  setupParameters();
#ifdef THR_PIXELS
  // taskPixels();
#endif
  // taskRocket();
  //  taskBMP280();
  // taskGY521();  // = MPU-6050

  taskSerial();
  //  taskServo();
  //   taskSi7021();

  taskOTA();
  // taskMDNS();  // incompatible with taskOTA
  if (false) {
    taskWifi();
  } else {
    taskWifiAP();
  }
  taskWebserver();
#ifdef THR_FORECAST
  taskForecast();
#endif

  taskOutput();
  taskInput();
  taskTimer();

  // taskMQTT();
  // taskOneWire();
  // taskNTPD();
  // taskInternalTemperature();
  // taskBatteryLevel();
  // taskUptime();
  // taskWire();
  // taskSPIFSLogger();
  // taskBluetooth();
#ifdef LED_ON_BOARD
  taskBlink();
#endif

  vTaskDelay(30 * 1000);  // waiting 30s before normal operation
}

void loop() {
  vTaskDelay(100);
}

void restart() {
  ESP.restart();
}

void deepSleep(int seconds) {
  if (seconds < 10) {
    return;
  }
  esp_sleep_enable_timer_wakeup(seconds * 1e6);
  esp_deep_sleep_start();
  setParameter(PARAM_STATUS, 0);
}

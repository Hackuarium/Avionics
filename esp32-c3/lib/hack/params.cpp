#include "./params.h"
#include <ArduinoNvs.h>
#include "./SerialUtilities.h"
#include "./common.h"
#include "./taskNTPD.h"
#include "./toHex.h"

#define INT_MAX_VALUE 32767
#define INT_MIN_VALUE -32768
#define LONG_MAX_VALUE 2147483647

// value that should not be taken into account
// in case of error the parameter is set to this value
#define ERROR_VALUE -32768

RTC_DATA_ATTR int16_t parameters[MAX_PARAM];

// todo uint16_t getQualifier();
boolean setParameterBit(byte number, byte bitToSet);
boolean clearParameterBit(byte number, byte bitToClear);
String numberToLabel(byte number);
void checkParameters();

boolean getParameterBit(byte number, byte bitToRead) {
  return (parameters[number] >> bitToRead) & 1;
}

boolean setParameterBit(byte number, byte bitToSet) {
  if (getParameterBit(number, bitToSet))
    return false;
  parameters[number] |= 1 << bitToSet;
  return true;
}

boolean clearParameterBit(byte number, byte bitToClear) {
  if (!getParameterBit(number, bitToClear))
    return false;
  parameters[number] &= ~(1 << bitToClear);
  return true;
}

void toggleParameterBit(byte number, byte bitToToggle) {
  parameters[number] ^= 1 << bitToToggle;
}

void setupParameters() {
  NVS.begin();
  for (byte i = 0; i < MAX_PARAM; i++) {
    parameters[i] = NVS.getInt(numberToLabel(i));
  }
  checkParameters();
}

void setParameter(char* key, char* value) {
  NVS.setString(key, value);
}

void getParameter(char* key, char* value) {
  strcpy(value, NVS.getString(key).c_str());
}

int16_t getParameter(byte number) {
  return parameters[number];
}

void setParameter(byte number, int16_t value) {
  parameters[number] = value;
}

void incrementParameter(byte number) {
  parameters[number]++;
}

void saveParameters() {
  for (byte i = 0; i < MAX_PARAM; i++) {
    NVS.setInt(numberToLabel(i), parameters[i]);
  }
}

/*
  This will take time, around 4 ms
  This will also use the EEPROM that is limited to 100000 writes
*/
void setAndSaveParameter(byte number, int16_t value) {
  parameters[number] = value;
  NVS.setInt(numberToLabel(number), value);

#ifdef EVENT_LOGGING
  writeLog(EVENT_PARAMETER_SET + number, value);
#endif
}

// this method will check if there was a change in the error status and log it
// in this case
boolean saveAndLogError(boolean isError, byte errorFlag) {
  if (isError) {
    if (setParameterBit(PARAM_ERROR, errorFlag)) {  // the status has changed
#ifdef EVENT_LOGGING
      writeLog(EVENT_ERROR_FAILED, errorFlag);
#endif
      return true;
    }
  } else {
    if (clearParameterBit(PARAM_ERROR, errorFlag)) {  // the status has changed
#ifdef EVENT_LOGGING
      writeLog(EVENT_ERROR_RECOVER, errorFlag);
#endif
      return true;
    }
  }
  return false;
}

String numberToLabel(byte number) {
  String result = "";
  if (number > 25) {
    result += (char)(floor(number / 26) + 64);
  }
  result += (char)(number - floor(number / 26) * 26 + 65);
  return result;
}

void printParameter(Print* output, byte number) {
  output->print(number);
  output->print("-");
  if (number < 26) {
    output->print(" ");
  }
  output->print(numberToLabel(number));
  output->print(": ");
  output->println(parameters[number]);
}

void printParameters(Print* output) {
  for (int16_t i = 0; i < MAX_PARAM; i++) {
    printParameter(output, i);
    vTaskDelay(1);
  }
}

void printCompactParameters(Print* output, byte number) {
  if (number > MAX_PARAM) {
    number = MAX_PARAM;
  }
  byte checkDigit = 0;

  // we first add epoch
  // checkDigit ^= toHex(output, (long)now());
  checkDigit ^= toHex(output, getEpoch());
  for (int16_t i = 0; i < number; i++) {
    int16_t value = getParameter(i);
    checkDigit ^= toHex(output, value);
  }
  checkDigit ^= toHex(output, (int16_t)getQualifier());
  toHex(output, checkDigit);
  output->println("");
}

void printCompactParameters(Print* output) {
  printCompactParameters(output, MAX_PARAM);
}

void resetParameters() {
  for (byte i = 0; i < MAX_PARAM; i++) {
    setAndSaveParameter(i, ERROR_VALUE);
  }

  setAndSaveParameter(PARAM_OUT1_COLOR1, 0x0FF);
  setAndSaveParameter(PARAM_OUT1_COLOR2, 0x0FF);
  setAndSaveParameter(PARAM_OUT1_COLOR3, 0x0FF);
  setAndSaveParameter(PARAM_OUT1_COLOR4, 0xFF0);
  setAndSaveParameter(PARAM_OUT1_COLOR5, 0xFF0);
  setAndSaveParameter(PARAM_OUT1_COLOR6, 0xFF0);
  setAndSaveParameter(PARAM_OUT1_COLOR7, 0xF0F);
  setAndSaveParameter(PARAM_OUT1_COLOR8, 0xF0F);

  setAndSaveParameter(PARAM_OUT2_COLOR1, 0x00F);
  setAndSaveParameter(PARAM_OUT2_COLOR2, 0x00F);
  setAndSaveParameter(PARAM_OUT2_COLOR3, 0x00F);
  setAndSaveParameter(PARAM_OUT2_COLOR4, 0x0F0);
  setAndSaveParameter(PARAM_OUT2_COLOR5, 0x0F0);
  setAndSaveParameter(PARAM_OUT2_COLOR6, 0x0F0);
  setAndSaveParameter(PARAM_OUT2_COLOR7, 0xF00);
  setAndSaveParameter(PARAM_OUT2_COLOR8, 0xF00);

  setAndSaveParameter(PARAM_OUT3_COLOR1, 0x0F0);
  setAndSaveParameter(PARAM_OUT3_COLOR2, 0x0F0);
  setAndSaveParameter(PARAM_OUT3_COLOR3, 0x0F0);
  setAndSaveParameter(PARAM_OUT3_COLOR4, 0xF00);
  setAndSaveParameter(PARAM_OUT3_COLOR5, 0xF00);
  setAndSaveParameter(PARAM_OUT3_COLOR6, 0xF00);
  setAndSaveParameter(PARAM_OUT3_COLOR7, 0x00F);
  setAndSaveParameter(PARAM_OUT3_COLOR8, 0x00F);

  setAndSaveParameter(PARAM_SPEED, 6);
  setAndSaveParameter(PARAM_BRIGHTNESS, 100);
  setAndSaveParameter(PARAM_TIMER, 120);

  setQualifier(16961);
}

void checkParameters() {
  if (getParameter(0) < ERROR_VALUE) {
    resetParameters();
  }
}

void setQualifier(int16_t value) {
  NVS.setInt("q", value);
}

int getQualifier() {
  return NVS.getInt("q");
}
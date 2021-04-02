/* JSONVoltronic library by Benoit Galmiche
*/

#include <Arduino.h>
#include <ArduinoJson.h>
#include "WatchPower.h"
#include "JsonVoltronic.h"

JsonVoltronic::JsonVoltronic(){}

JsonVoltronic::~JsonVoltronic(){}

int JsonVoltronic::buildQPGIS1(WatchPower *watchPower, char *messBuffer){

    StaticJsonDocument<256> buffer;

    buffer["gridVoltage"] = watchPower->gridVoltage.flt;
    buffer["gridFreq"] = watchPower->gridFreq.flt;
    buffer["outputVoltage"] = watchPower->outputVoltage.flt;
    buffer["outputFreq"] = watchPower->outputFreq.flt;
    buffer["outputPowerApparent"] = watchPower->outputPowerApparent.flt;
    buffer["outputPowerActive"] = watchPower->outputPowerActive.flt;
    buffer["loadPercent"] = watchPower->loadPercent.flt;
    buffer["batteryVoltage"] = watchPower->batteryVoltage.flt;
    buffer["batteryCurrent"] = watchPower->batteryCurrent.flt;
    buffer["batteryCapacity"] = watchPower->batteryCapacity.flt;

    char output[256];
    size_t n = serializeJson(buffer, output);

    strncpy(messBuffer, output, 256);

    return n;
}

int JsonVoltronic::buildQPGIS2(WatchPower *watchPower, char *messBuffer){

    StaticJsonDocument<256> buffer;

    buffer["busVoltage"] = watchPower->busVoltage.flt;
    buffer["temperature"] = watchPower->temperature.flt;
    buffer["solarCurrent"] = watchPower->solarCurrent.flt;
    buffer["solarVoltage"] = watchPower->solarVoltage.flt;
    buffer["batteryVoltageSCC"] = watchPower->batteryVoltageSCC.flt;
    buffer["batteryDischargeCurrent"] = watchPower->batteryDischargeCurrent.flt;    
    buffer["solarInputPower"] = watchPower->solarInputPower.flt;

    char output[256];
    size_t n = serializeJson(buffer, output);

    strncpy(messBuffer, output, 256);

    return n;
}

int JsonVoltronic::buildQPGIS3(WatchPower *watchPower, char *messBuffer){

    StaticJsonDocument<256> buffer;

    buffer["status"] = watchPower->status.str;
    buffer["isCharging"] = watchPower->isCharging();
    buffer["isSolarCharging"] = watchPower->isSolarCharging();
    buffer["isGridCharging"] = watchPower->isGridCharging();
    buffer["isOnBattery"] = watchPower->isOnBattery();
    buffer["isOnGrid"] = watchPower->isOnGrid();
    buffer["isGridAvailable"] = watchPower->isGridAvailable();
    buffer["isSolarAvailable"] = watchPower->isSolarAvailable();
    
    buffer["RSV1"] = watchPower->RSV1.flt;
    buffer["RSV2"] = watchPower->RSV2.flt;
    buffer["status2"] = watchPower->status2.str;

    char output[256];
    size_t n = serializeJson(buffer, output);

    strncpy(messBuffer, output, 256);

    return n;
}


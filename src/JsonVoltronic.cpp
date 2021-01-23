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

    buffer["gridVoltage"] = watchPower->gridVoltage.str;
    buffer["gridFreq"] = watchPower->gridFreq.str;
    buffer["outputVoltage"] = watchPower->outputVoltage.str;
    buffer["outputFreq"] = watchPower->outputFreq.str;
    buffer["outputPowerApparent"] = watchPower->outputPowerApparent.str;
    buffer["outputPowerActive"] = watchPower->outputPowerActive.str;
    buffer["loadPercent"] = watchPower->loadPercent.str;
    buffer["batteryVoltage"] = watchPower->batteryVoltage.str;
    buffer["batteryCurrent"] = watchPower->batteryCurrent.str;
    buffer["batteryCapacity"] = watchPower->batteryCapacity.str;

    char output[256];
    size_t n = serializeJson(buffer, output);

    strncpy(messBuffer, output, 256);

    return n;
}

int JsonVoltronic::buildQPGIS2(WatchPower *watchPower, char *messBuffer){

    StaticJsonDocument<256> buffer;

    buffer["busVoltage"] = watchPower->busVoltage.str;
    buffer["temperature"] = watchPower->temperature.str;
    buffer["solarCurrent"] = watchPower->solarCurrent.str;
    buffer["solarVoltage"] = watchPower->solarVoltage.str;
    buffer["batteryVoltageSCC"] = watchPower->batteryVoltageSCC.str;
    buffer["batteryDischargeCurrent"] = watchPower->batteryDischargeCurrent.str;
    buffer["status"] = watchPower->status.str;
    buffer["RSV1"] = watchPower->RSV1.str;
    buffer["RSV2"] = watchPower->RSV2.str;
    buffer["solarInputPower"] = watchPower->solarInputPower.str;
    buffer["status2"] = watchPower->status2.str; 

    char output[256];
    size_t n = serializeJson(buffer, output);

    strncpy(messBuffer, output, 256);

    return n;
}


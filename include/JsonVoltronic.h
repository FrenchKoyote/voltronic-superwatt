#pragma once

/* WatchPower library by Benoit Galmiche (FrenchKoyote)
*/

#include <Arduino.h>
#include <ArduinoJson.h>
#include "WatchPower.h"
#include <JsonVoltronic.h>

class JsonVoltronic{
public:

    /* Constructor and Destructor */
    JsonVoltronic();
    ~JsonVoltronic();

    /* Get parameters */
    int buildQPGIS1(WatchPower *watchPower, char *messBuffer);
    int buildQPGIS2(WatchPower *watchPower, char *messBuffer);
};
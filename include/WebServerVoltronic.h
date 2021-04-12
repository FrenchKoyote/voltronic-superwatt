#pragma once

/* WebServer Voltronic library by Benoit Galmiche (FrenchKoyote)
*/

#include <WebServerVoltronic.h>
#include <WebServer.h> 
#include <WatchPower.h>

class WebServerVoltronic{
public:

    /* Constructor and Destructor */
    WebServerVoltronic(WatchPower &_refWat);

    ~WebServerVoltronic();

    /* Get parameters */
    void setup();
    void handleClient();
    static void buildHomePage();
    static void buildNotFoundPage();

private:

    static WatchPower* refWat; 

    static void setChargePriorityUtility();
    static void setChargePrioritySolar();
    static void setOutputPrioritySolar();
    static void setOutputPriorityUtility();
    static void getOutputPriority();
    static void getChargePriority();
};


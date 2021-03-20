#pragma once

/* WebServer Voltronic library by Benoit Galmiche (FrenchKoyote)
*/

#include <WebServerVoltronic.h>
#include <WebServer.h> 

class WebServerVoltronic{
public:

   

    /* Constructor and Destructor */
    WebServerVoltronic();

    ~WebServerVoltronic();

    /* Get parameters */
    void setup();
    void handleClient();
    static void buildHomePage();
    static void buildNotFoundPage();
};
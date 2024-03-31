#include "WebInterface.h"
#include <iostream>
#include <WebInterface.h>

int main(int argc, char** argv)
{
    WebInterface app;
    return app.run(argc, argv);
}

// MQTT: {   "url": "localhost",   "port": 1884,   "client_name": "client1",   "topic": "test/topic",   "timeout": 100000 }
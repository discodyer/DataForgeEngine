#include "WebInterface.h"
#include <iostream>
#include <WebInterface.h>

int main(int argc, char** argv)
{
    HTTPServerApp app;
    return app.run(argc, argv);
}
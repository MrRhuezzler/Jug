#include <iostream>
#include "jug/application.h"

int main()
{
    auto app = Jug::Application::createInstance("Hello", 900, 900);
    app->run();
    delete app;
    return 0;
}
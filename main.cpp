#include "app.h"

int main(int, char**){

    App app;

    while(app.isRunning()){
        app.Run();
    }

    return 0;
}


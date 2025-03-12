#include "app.h"

int main(int, char**){

    Serializer::loadData();

    App app;

    while(app.isRunning()){
        app.Run();
    }

    Serializer::storeData();

    return 0;
}


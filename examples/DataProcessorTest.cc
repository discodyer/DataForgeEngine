#include "DataProcessor.h"

int main() {
   
    StepManager engine;
    engine.buildStepTree("example_steps.json");

    engine.execute();
    
    return 0;
}

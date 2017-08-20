#include <Arduino.h>
#include <SPI.h>
#include "U8g2lib.h"

void displayContinuousMode(bool enabled) {
  
}

void displayLastMeasurement(double measurementValue) {
  
}

void displayAverageMeasurement(double averageValue) {
  
}

void displayMeasurement(std::vector<double>* input) {
    // display average
    double avg = accumulate(input.begin(), input.end(), 0.0) / input.size();
    displayAverageMeasurement(avg);

    // display the last measurement; it is at the end
    displayLastMeasurement(input->back());
}

void init() {
  
}


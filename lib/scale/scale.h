#include "Arduino.h"
#include "HX711_ADC.h"

#ifndef scale_h
#define scale_h

class scale_C
{
    public:
        scale_C(unsigned dout, unsigned sck); //
        void begin();
        unsigned update();
        void calibrate();
        void tare();
        void changeSavedCalFactor();
        boolean isTare;

    private:
        HX711_ADC LoadCell;
};
#endif
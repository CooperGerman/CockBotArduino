#include <scale.h>
#include <scale_config.h>
#if defined(ESP8266) || defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif
scale_C::scale_C( unsigned dout, unsigned sck )
{
    // HX711 constructor:
    LoadCell.setPins(dout, sck) ;
}
void scale_C::begin()
{
    LoadCell.begin();
    LoadCell.start(stabilizingtime, _tare);
    if (LoadCell.getTareTimeoutFlag())
    {
        Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
        while (1)
            ;
    }
    else
    {
        LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
        Serial.println("Startup is complete");
    }
}

unsigned scale_C::update()
{

    boolean newDataReady = false;
    float i ;

    while (newDataReady==false)
    {
        // check for new data/start next conversion:
        if (LoadCell.update())
            newDataReady = true;

        i = LoadCell.getData();
    }
    return i;
}

void scale_C::tare()
{
    LoadCell.tareNoDelay();
}

void scale_C::calibrate()
{
    Serial.println("***");
    Serial.println("Start calibration:");
    Serial.println("Place the load cell an a level stable surface.");
    Serial.println("Remove any load applied to the load cell.");
    Serial.println("Send 't' from serial monitor to set the tare offset.");

    boolean _resume = false;
    while (_resume == false)
    {
        LoadCell.update();
        if (Serial.available() > 0)
        {
            if (Serial.available() > 0)
            {
                char inByte = Serial.read();
                if (inByte == 't')
                    LoadCell.tareNoDelay();
            }
        }
        if (LoadCell.getTareStatus() == true)
        {
            Serial.println("Tare complete");
            _resume = true;
        }
    }

    Serial.println("Now, place your known mass on the loadcell.");
    Serial.println("Then send the weight of this mass (i.e. 100.0) from serial monitor.");

    float known_mass = 0;
    _resume = false;
    while (_resume == false)
    {
        LoadCell.update();
        if (Serial.available() > 0)
        {
            known_mass = Serial.parseFloat();
            if (known_mass != 0)
            {
                Serial.print("Known mass is: ");
                Serial.println(known_mass);
                _resume = true;
            }
        }
    }

    LoadCell.refreshDataSet();                                          // refresh the dataset to be sure that the known mass is measured correct
    float newCalibrationValue = LoadCell.getNewCalibration(known_mass); // get the new calibration value

    Serial.print("New calibration value has been set to: ");
    Serial.print(newCalibrationValue);
    Serial.println(", use this as calibration value (calFactor) in your project sketch.");
    Serial.print("Save this value to EEPROM adress ");
    Serial.print(calVal_eepromAdress);
    Serial.println("? y/n");

    _resume = false;
    while (_resume == false)
    {
        if (Serial.available() > 0)
        {
            char inByte = Serial.read();
            if (inByte == 'y')
            {
#if defined(ESP8266) || defined(ESP32)
                EEPROM.begin(512);
                EEPROM.put(calVal_eepromAdress, newCalibrationValue);
                EEPROM.commit();
                EEPROM.get(calVal_eepromAdress, newCalibrationValue);
#endif
                Serial.print("Value ");
                Serial.print(newCalibrationValue);
                Serial.print(" saved to EEPROM address: ");
                Serial.println(calVal_eepromAdress);
                _resume = true;
            }
            else if (inByte == 'n')
            {
                Serial.println("Value not saved to EEPROM");
                _resume = true;
            }
        }
    }

    Serial.println("End calibration");
    Serial.println("***");
    Serial.println("To re-calibrate, send 'r' from serial monitor.");
    Serial.println("For manual edit of the calibration value, send 'c' from serial monitor.");
    Serial.println("***");
}

void scale_C::changeSavedCalFactor()
{
    float oldCalibrationValue = LoadCell.getCalFactor();
    boolean _resume = false;
    Serial.println("***");
    Serial.print("Current value is: ");
    Serial.println(oldCalibrationValue);
    Serial.println("Now, send the new value from serial monitor, i.e. 696.0");
    float newCalibrationValue;
    while (_resume == false)
    {
        if (Serial.available() > 0)
        {
            newCalibrationValue = Serial.parseFloat();
            if (newCalibrationValue != 0)
            {
                Serial.print("New calibration value is: ");
                Serial.println(newCalibrationValue);
                LoadCell.setCalFactor(newCalibrationValue);
                _resume = true;
            }
        }
    }
    _resume = false;
    Serial.print("Save this value to EEPROM adress ");
    Serial.print(calVal_eepromAdress);
    Serial.println("? y/n");
    while (_resume == false)
    {
        if (Serial.available() > 0)
        {
            char inByte = Serial.read();
            if (inByte == 'y')
            {
#if defined(ESP8266) || defined(ESP32)
                EEPROM.begin(512);
                EEPROM.put(calVal_eepromAdress, newCalibrationValue);
                EEPROM.commit();
                EEPROM.get(calVal_eepromAdress, newCalibrationValue);
#endif
                Serial.print("Value ");
                Serial.print(newCalibrationValue);
                Serial.print(" saved to EEPROM address: ");
                Serial.println(calVal_eepromAdress);
                _resume = true;
            }
            else if (inByte == 'n')
            {
                Serial.println("Value not saved to EEPROM");
                _resume = true;
            }
        }
    }
    Serial.println("End change calibration value");
    Serial.println("***");
}

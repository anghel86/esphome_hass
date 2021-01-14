#include "esphome.h"
// #include "config/config.dist.h"
#include <driver/adc.h>

#include "EmonLib.h" 

#define ADC_INPUT 36
#define ADC_VOLTAGE_INPUT 35
#define vCalibration 544
#define currCalibration 30.6
#define ADC_BITS    12

static const char *TAG = "CustEnergyMonitor";

namespace esphome {
class CustEnergyMonitor : public PollingComponent {
    public: 
        EnergyMonitor emon1;

        Sensor *volts_sensor = new Sensor();
        Sensor *amps_sensor = new Sensor();
        Sensor *watts_sensor = new Sensor();
        
        CustEnergyMonitor() : PollingComponent(5000) {}

        void setup() override {
            ESP_LOGD(TAG, "setup");
            adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_MAX);
            analogReadResolution(ADC_BITS);
            pinMode(ADC_INPUT, INPUT);
            emon1.current(ADC_INPUT, currCalibration);
            emon1.voltage(ADC_VOLTAGE_INPUT, vCalibration, 1.7);
        }

        void update() override {
            // ESP_LOGD(TAG, "update");
            
            // // int read_count = 2;
            
            // for(int read_count = 1; read_count < 10; read_count++){
            //     ESP_LOGD(TAG, "read_count %d", read_count);
            //     int min = 4096;
            //     int max = 0;
            //     for (int i = 0; i < 10000; i++) {
            //         int sum_read = 0;
            //         for(int j = 0; j< read_count; j++){
            //             sum_read += analogRead(ADC_INPUT);
            //         }

            //         int sampleI = sum_read / read_count;
                    
            //         if(min > sampleI)
            //             min = sampleI;
            //         if(max < sampleI)
            //             max = sampleI;
                    
            //     }
                
            //     ESP_LOGD(TAG, "diff %d", max-min);
            // }
            
            emon1.calcVI(250, 500);
            volts_sensor->publish_state(emon1.Vrms);
            amps_sensor->publish_state(emon1.Irms);
            watts_sensor->publish_state(emon1.apparentPower);
        }
};
}

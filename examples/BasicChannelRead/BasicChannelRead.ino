/*
  BasicChannelRead

  Configures the ADC and manually reads a channel.
*/

#include <ADS7138.h>

#define HW_ADC_SDA      33  // SDA pin
#define HW_ADC_SCL      25  // SCL pin

ADS7138 adc;

void setup() {
  // init Serial
  Serial.begin(115200);

  // configure ADC
  // set I2C pins
  adc.begin(HW_ADC_SDA, HW_ADC_SCL);
  // configure manual operation mode
  adc.configureOpMode(OSC_SEL_LOW_POWER, CONV_MODE_MANUAL, CONV_ON_ERR_CONTINUE);
  // disable auto sequence mode
  adc.configureSequenceMode(SEQ_MODE_MANUAL, SEQ_START_END);
  // configure oversampling to 128 samples
  adc.configureOsr(OSR_128);
  // set reference voltage in millivolts
  adc.setReferenceVoltage(3300);
  // select channel
  adc.selectChannel(MANUAL_CHID_AIN0);
}

void loop() {
  uint32_t voltage;
  // read channel 0
  voltage = adc.readChannelVoltage(MANUAL_CHID_AIN0);
  // print
  Serial.print("Voltage (mV): ");
  Serial.println(voltage);
  // wait
  delay(1000);
}

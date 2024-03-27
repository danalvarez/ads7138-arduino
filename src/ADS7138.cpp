/**************************************************************************/
/*!
  @file     ADS7138.cpp

  @mainpage Texas Instruments ADS7138 SAR ADC

  @section intro Introduction

  This is a library for the Texas Instruments ADS7138 SAR ADC.

  @section author Author

  Dan Alvarez
*/
/**************************************************************************/
#include "ADS7138.h"

/**
 * @brief Instantiates a new ADS7138 class.
 * 
 * This constructor initializes a new instance of the ADS7138 class.
 */
ADS7138::ADS7138() {}

/**
 * @brief Sets up the I2C connection.
 * 
 * This function initializes the I2C communication with the ADS7138 device using Wire.
 *
 * @param sda The pin for SDA.
 * @param scl The pin for SCL.
 * @param deviceAddress The 7-bit I2C address of the ADS7138.
 */
void ADS7138::begin(int sda, int scl, int deviceAddress) {
    // configure I2C
    _deviceAddress = (deviceAddress != -1) ? deviceAddress : ADS7138_I2CADDR_DEFAULT;
    if(sda != -1 && scl != -1) {
        Wire.setPins(sda, scl);
    }
    Wire.begin();
    // assume oversampling ratio is 1
    _currentOsr = OSR_1;
    // ADC reference voltage (assume 3.3 V)
    _referenceVoltage = 3300;
    // default channel
    _currentChannel = MANUAL_CHID_AIN0;
}

/**
 * @brief Sets the ADC voltage reference in mV.
 *
 * @param reference The ADC reference voltage in mV.
 */
void ADS7138::setReferenceVoltage(uint32_t reference) {
    _referenceVoltage = reference;
}

/**
 * @brief Writes an 8-bit value to a specified register address in the ADC.
 *
 * Initiates an I2C transmission to the ADS7138 device, writing the register address
 * and the provided 8-bit value. The transmission is then terminated.
 *
 * @param registerAddress The address of the register in the ADS7138 device to write to.
 * @param value The 8-bit value to be written to the specified register.
 */
void ADS7138::writeRegister8(uint8_t registerAddress, uint8_t value) {
    Wire.beginTransmission(_deviceAddress);
    Wire.write(SINGLE_REGISTER_WRITE);
    Wire.write(registerAddress);
    Wire.write(value);
    Wire.endTransmission();
}

/**
 * @brief Configures the operation mode of the ADC.
 *
 * Configures the OPMODE_CFG register of the ADC. Always sets CLK_DIV field to zero.
 *
 * @param oscSel Selects the oscillator for internal timing generation.
 * @param convMode These bits set the mode of conversion of the ADC.
 * @param convOnErr Control continuation of autonomous and turbo comparator modes if CRC error is detected on communication interface.
 */
void ADS7138::configureOpMode(ADS7138__OSC_SEL oscSel, ADS7138__CONV_MODE convMode, ADS7138__CONV_ON_ERR convOnErr) {
    uint8_t send = (0x00 & CLK_DIV_MASK) | (oscSel & OSC_SEL_MASK) | (convMode & CONV_MODE_MASK) | (convOnErr & CONV_ON_ERR_MASK);
    writeRegister8(OPMODE_CFG_ADDRESS, send);
}

/**
 * @brief Configures the ADC channel sequencing.
 *
 * Configures the SEQUENCE_CFG register of the ADC.
 *
 * @param seqMode Selects the mode of scanning of analog input channels.
 * @param seqStart Control for start of channel sequence when using auto sequence mode.
 */
void ADS7138::configureSequenceMode(ADS7138__SEQ_MODE seqMode, ADS7138__SEQ_START seqStart) {
    uint8_t send = (seqMode & SEQ_MODE_MASK) | (seqStart & SEQ_START_MASK);
    writeRegister8(SEQUENCE_CFG_ADDRESS, send);
}

/**
 * @brief Configures the oversampling ratio.
 *
 * Configures the oversampling ratio of the programmable averaging filter in the ADC.
 *
 * @param osr The oversampling ratio value for the average filter.
 */
void ADS7138::configureOsr(ADS7138__OSR osr){
    _currentOsr = osr;                                                      // update osr
    writeRegister8(OSR_CFG_ADDRESS, osr & OSR_MASK);   
}

/**
 * @brief Manually selects the analog channel that will be read.
 *
 * When in manual mode, this command selects the channel that the user wishes to read.
 *
 * @param channel The channel to select.
 */
void ADS7138::selectChannel(ADS7138__MANUAL_CHID channel) {
    _currentChannel = channel;
    writeRegister8(MANUAL_CH_SEL_ADDRESS, channel & MANUAL_CHID_MASK);
}

/**
 * @brief Reads the ADC.
 *
 * Initiates an I2C request to the ADS7138 device, reading a value from the
 * specified address. The value is constructed from the most and least significant bytes.
 *
 * @return uint16_t The value read from the ADS7138 device.
 */
uint16_t ADS7138::read() {
    uint16_t value = 0;
    uint8_t requestedBytes = 2;
    Wire.requestFrom(_deviceAddress, requestedBytes);
    if(Wire.available()) {
        // frame A
        if(_currentOsr != OSR_1) {
            value = Wire.read() << 4;   // reads most significant
            value += Wire.read() >> 4;  // adds least significant
        }
        // frame B
        else {
            value = Wire.read() << 8;   // reads most significant
            value += Wire.read();       // adds least significant
        }
    }
    return value;
}

/**
 * @brief Manually selects and reads a channel.
 *
 * @return uint16_t The value read from the ADS7138 device.
 */
uint16_t ADS7138::readChannel(ADS7138__MANUAL_CHID channel) {
    if(channel != _currentChannel) selectChannel(channel);
    return read();
}

/**
 * @brief Reads a value from the ADC and converts it to a voltage in mV.
 *
 * Initiates an I2C request to the ADS7138 device, reading a value from the
 * specified address. The value is constructed from the most and least significant bytes.
 * This is then converted to a voltage in mV.
 *
 * @return uint32_t The voltage in mV.
 */
uint32_t ADS7138::readVoltage() {
    uint16_t value = read();
    uint32_t voltage = value * _referenceVoltage / 4095;
    return voltage;
}

/**
 * @brief Manually selects and reads a channel's voltage.
 *
 * @return uint32_t The voltage in mV.
 */
uint32_t ADS7138::readChannelVoltage(ADS7138__MANUAL_CHID channel) {
    if(channel != _currentChannel) selectChannel(channel);
    return readVoltage();
}
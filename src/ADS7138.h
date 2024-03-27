//**********************************************************************************
/**
  @file     ADS7138.h

  Author: Dan Alvarez

  This is a library for the TI ADS7138 SAR ADC.
  
  Register definitions taken from TI-provided header file.
  See https://www.ti.com/tool/download/SBAC286
  Copyright (C) 2021 Texas Instruments Incorporated - http://www.ti.com/

*/
//**********************************************************************************

#ifndef ads7138_h
#define ads7138_h

#include <Arduino.h>
#include <Wire.h>

/** Default ADS7138 I2C address. */
#define ADS7138_I2CADDR_DEFAULT 0x10

//**********************************************************************************
//
// Device commands
//
//**********************************************************************************
    # define SINGLE_REGISTER_WRITE                                          ((uint8_t) 0b00001000)

//**********************************************************************************
//
// Register definitions
//
//**********************************************************************************

/* Register 0x03 (OSR_CFG) definition
 * |-----------------------------------------------------------------------------------------------------------------------|
 * |     Bit 7    |     Bit 6    |     Bit 5    |     Bit 4    |     Bit 3    |     Bit 2    |     Bit 1    |     Bit 0    |
 * |-----------------------------------------------------------------------------------------------------------------------|
 * |                               RESERVED[4:0]                              |                  OSR[2:0]                  |
 * |-----------------------------------------------------------------------------------------------------------------------|
 */

    /* OSR_CFG register */
    #define OSR_CFG_ADDRESS													((uint8_t) 0x03)
    #define OSR_CFG_DEFAULT													((uint8_t) 0x00)

    #define OSR_MASK														((uint8_t) 0x07)
    typedef enum _OSR {
        OSR_1 =															    ((uint8_t) 0x00),   // DEFAULT
        OSR_2 =															    ((uint8_t) 0x01),
        OSR_4 =															    ((uint8_t) 0x02),
        OSR_8 =															    ((uint8_t) 0x03),
        OSR_16 =															((uint8_t) 0x04),
        OSR_32 =															((uint8_t) 0x05),
        OSR_64 =															((uint8_t) 0x06),
        OSR_128 =															((uint8_t) 0x07)
    } ADS7138__OSR;

/* Register 0x04 (OPMODE_CFG) definition
 * |-----------------------------------------------------------------------------------------------------------------------|
 * |     Bit 7    |     Bit 6    |     Bit 5    |     Bit 4    |     Bit 3    |     Bit 2    |     Bit 1    |     Bit 0    |
 * |-----------------------------------------------------------------------------------------------------------------------|
 * |  CONV_ON_ERR |        CONV_MODE[1:0]       |    OSC_SEL   |                        CLK_DIV[3:0]                       |
 * |-----------------------------------------------------------------------------------------------------------------------|
 */

    /* OPMODE_CFG register */
    #define OPMODE_CFG_ADDRESS												((uint8_t) 0x04)
    #define OPMODE_CFG_DEFAULT												((uint8_t) 0x00)

    /* CONV_ON_ERR field */
    #define CONV_ON_ERR_MASK												((uint8_t) 0x80)
    typedef enum _CONV_ON_ERR {
        CONV_ON_ERR_CONTINUE =											    ((uint8_t) 0x00),   // DEFAULT
        CONV_ON_ERR_PAUSE =												    ((uint8_t) 0x80)
    } ADS7138__CONV_ON_ERR;

    /* CONV_MODE field */
    #define CONV_MODE_MASK													((uint8_t) 0x60)
    typedef enum _CONV_MODE {
        CONV_MODE_MANUAL =												    ((uint8_t) 0x00),   // DEFAULT
        CONV_MODE_AUTO =												    ((uint8_t) 0x20)
    } ADS7138__CONV_MODE;

    /* OSC_SEL field */
    #define OSC_SEL_MASK													((uint8_t) 0x10)
    typedef enum _OSC_SEL {
        OSC_SEL_HIGH_SPEED =									            ((uint8_t) 0x00),   // DEFAULT
        OSC_SEL_LOW_POWER =											        ((uint8_t) 0x10)
    } ADS7138__OSC_SEL;

    /* CLK_DIV field */
    #define CLK_DIV_MASK                                                    ((uint8_t) 0x0F)

/* Register 0x10 (SEQUENCE_CFG) definition
 * |-----------------------------------------------------------------------------------------------------------------------|
 * |     Bit 7    |     Bit 6    |     Bit 5    |     Bit 4    |     Bit 3    |     Bit 2    |     Bit 1    |     Bit 0    |
 * |-----------------------------------------------------------------------------------------------------------------------|
 * |                RESERVED[2:0]               |   SEQ_START  |        RESERVED[1:0]        |        SEQ_MODE[1:0]        |
 * |-----------------------------------------------------------------------------------------------------------------------|
 */

    /* SEQUENCE_CFG register */
    #define SEQUENCE_CFG_ADDRESS											((uint8_t) 0x10)
    #define SEQUENCE_CFG_DEFAULT											((uint8_t) 0x00)

    /* SEQ_START field */
    #define SEQ_START_MASK													((uint8_t) 0x10)
    typedef enum _SEQ_START {
        SEQ_START_END =													    ((uint8_t) 0x00),   // DEFAULT
        SEQ_START_ASSEND =												    ((uint8_t) 0x10)
    } ADS7138__SEQ_START;

    /* SEQ_MODE field */
    #define SEQ_MODE_MASK													((uint8_t) 0x03)
    typedef enum _SEQ_MODE {
        SEQ_MODE_MANUAL =												    ((uint8_t) 0x00),   // DEFAULT
        SEQ_MODE_AUTO =													    ((uint8_t) 0x01)
    } ADS7138__SEQ_MODE;

/* Register 0x11 (MANUAL_CH_SEL) definition
 * |-----------------------------------------------------------------------------------------------------------------------|
 * |     Bit 7    |     Bit 6    |     Bit 5    |     Bit 4    |     Bit 3    |     Bit 2    |     Bit 1    |     Bit 0    |
 * |-----------------------------------------------------------------------------------------------------------------------|
 * |                       RESERVED[3:0]                       |                      MANUAL_CHID[3:0]                     |
 * |-----------------------------------------------------------------------------------------------------------------------|
 */

    /* MANUAL_CH_SEL register */
    #define MANUAL_CH_SEL_ADDRESS											((uint8_t) 0x11)
    #define MANUAL_CH_SEL_DEFAULT											((uint8_t) 0x00)

    /* MANUAL_CHID field */
    #define MANUAL_CHID_MASK												((uint8_t) 0x0F)
    typedef enum _MANUAL_CHID {
        MANUAL_CHID_AIN0 =												    ((uint8_t) 0x00),   // DEFAULT
        MANUAL_CHID_AIN1 =												    ((uint8_t) 0x01),
        MANUAL_CHID_AIN2 =												    ((uint8_t) 0x02),
        MANUAL_CHID_AIN3 =												    ((uint8_t) 0x03),
        MANUAL_CHID_AIN4 =												    ((uint8_t) 0x04),
        MANUAL_CHID_AIN5 =												    ((uint8_t) 0x05),
        MANUAL_CHID_AIN6 =												    ((uint8_t) 0x06),
        MANUAL_CHID_AIN7 =												    ((uint8_t) 0x07)
    } ADS7138__MANUAL_CHID;

class ADS7138 {
    public:
        ADS7138();
        void begin(int sda = -1, int scl = -1, int deviceAddress = -1);
        void setReferenceVoltage(uint32_t reference);
        void writeRegister8(uint8_t registerAddress, uint8_t value);
        void configureOpMode(ADS7138__OSC_SEL oscSel, ADS7138__CONV_MODE convMode, ADS7138__CONV_ON_ERR convOnErr);
        void configureSequenceMode(ADS7138__SEQ_MODE seqMode, ADS7138__SEQ_START seqStart);
        void configureOsr(ADS7138__OSR osr);
        void selectChannel(ADS7138__MANUAL_CHID channel);
        uint16_t read();
        uint16_t readChannel(ADS7138__MANUAL_CHID channel);
        uint32_t readVoltage();
        uint32_t readChannelVoltage(ADS7138__MANUAL_CHID channel);
    private:
        uint8_t _deviceAddress;
        uint32_t _referenceVoltage;
        ADS7138__OSR _currentOsr;
        ADS7138__MANUAL_CHID _currentChannel;
};

#endif
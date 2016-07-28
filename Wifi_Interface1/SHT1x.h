/*
 * mbed library to use a SENSIRION SHT1x/SHT7x sensor
 * Copyright (c) 2011 Hiroshi Suga
 * Released under the MIT License: http://mbed.org/license/mit
 */
 
/** @file
 * @brief mbed library to use a SENSIRION SHT1x/SHT7x sensor
 * humidity/temperature sensor SHT11, SHT15, SHT71, SHT75 (SENSIRION)
 * interface: I2C digital
 */
 
#ifndef SHT1x_H
#define SHT1x_H

#include "mbed.h"

/**
 * @brief resolution setting
 */
enum SHT_acc {
    SHT_low = 0, ///< RH 8bit, temp 12bit
    SHT_high = 1, ///< RH 10bit, temp 14bit
};

enum SHT_vdd {
    SHT_3V3 = 0,
    SHT_5V = 1,
};

/**
 * @brief SHT1x class
 */
class SHT1x : public Base {
public:
    /**
     * @brief Initializes interface (private I2C)
     * @param p_sda port of I2C SDA
     * @param p_scl port of I2C SCL
     * @param p_acc resolution setting
     * @param p_vdd power
     */
    SHT1x(PinName p_sda, PinName p_scl, SHT_acc p_acc = SHT_high, SHT_vdd p_vdd = SHT_3V3);

    /**
     * @brief Initializes interface (public I2C)
     * @param p_i2c instance of I2C class
     * @param p_acc resolution setting
     * @param p_vdd power
     */
    SHT1x(I2C& p_i2c, SHT_acc p_acc = SHT_high, SHT_vdd p_vdd = SHT_3V3);

    /**
     * @brief Get temperature
     * @return temperature (`C)
     */
    float get_temperature();

    /**
     * @brief Get humidity
     * @return humidity (%)
     */
    float get_humidity();

    /**
     * @brief Update results
     */
    void update();

protected:
    void init(SHT_acc p_acc, SHT_vdd p_vdd);
    unsigned short twi_readshort (int addr, int wait);
    void twi_writechar (int, int);

    I2C i2c;
    float humidity, temperature;

private:
    float d1, d2, c1, c2, c3, t1, t2;
    enum SHT_acc accuracy;
};

#endif
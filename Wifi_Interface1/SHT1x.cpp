/*
 * mbed library to use a SENSIRION SHT1x/SHT7x sensor
 * Copyright (c) 2011 Hiroshi Suga
 * Released under the MIT License: http://mbed.org/license/mit
 */
 
/** @file
 * @brief mbed library to use a SENSIRION SHT1x/SHT7x sensor
 * humidity/temperature sensor SHT11, SHT15, SHT71, SHT75 (SENSIRION) Ver.4
 * interface: I2C digital
 */

#include "mbed.h"
#include "SHT1x.h"

enum SHT_cmd {
    SHT_cmd_temperature = 0x03,
    SHT_cmd_humidity = 0x05,
    SHT_cmd_read_status = 0x07,
    SHT_cmd_write_status = 0x06,
    SHT_cmd_reset = 0x1e
};

SHT1x::SHT1x (PinName p_sda, PinName p_scl, SHT_acc p_acc, SHT_vdd p_vdd) : i2c(p_sda, p_scl) {
    init(p_acc, p_vdd);
}

SHT1x::SHT1x (I2C& p_i2c, SHT_acc p_acc, SHT_vdd p_vdd) : i2c(p_i2c) {
    init(p_acc, p_vdd);
}

float SHT1x::get_temperature() {
    return temperature;
}

float SHT1x::get_humidity() {
    return humidity;
}

void SHT1x::update () {
    float t, rhl, rht;
    unsigned short sot, sorh;

    sot = twi_readshort(SHT_cmd_temperature, (accuracy == SHT_low ? 80 : 320));
    sorh = twi_readshort(SHT_cmd_humidity, (accuracy == SHT_low ? 20 : 80));
    if (sot == 0xffff || sorh == 0xffff) return;

    if (accuracy == SHT_low) {
        sot &= 0x0fff; // 12bit
        sorh &= 0x00ff; // 8bit
    } else {
        sot &= 0x3fff; // 14bit
        sorh &= 0x0fff; // 12bit
    }    

    t = d1 + d2 * (float)sot;
    temperature = t;

    rhl = c1 + c2 * (float)sorh + c3 * (float)(sorh * sorh);
    rht = (t - 25.0) * (t1 + t2 * (float)sorh) + rhl;
    if (rht > 99.0) rht = 100.0;
    humidity = rht;
}

void SHT1x::init (SHT_acc p_acc, SHT_vdd p_vdd) {

    humidity = 0;
    temperature = 0;

    if (p_vdd == SHT_3V3) {
        d1 = -39.66; // 3.3V
    } else {
        d1 = -40.1; // 5V
    }

    accuracy = p_acc;
    if (accuracy == SHT_low) {
        d2 = 0.04;
        c1 = -2.0468; c2 = 0.5872; c3 = -4.0845/10000.0;
        t1 = 0.01; t2 = 0.00128;
        twi_writechar(SHT_cmd_write_status, 1);
    } else {
        d2 = 0.01;
        c1 = -2.0468; c2 = 0.0367; c3 = -1.5955/1000000.0;
        t1 = 0.01; t2 = 0.00008;
        twi_writechar(SHT_cmd_write_status, 0);
    }    
}

unsigned short SHT1x::twi_readshort (int addr, int wait) {
    unsigned short i;

    i2c.start();
    i2c.stop();

    i2c.start();
    i2c.write(addr);
    wait_ms(wait);

    i = i2c.read(1) << 8;
    i |= i2c.read(1);
    i2c.read(0);
    i2c.stop();

    return i;
}

void SHT1x::twi_writechar (int addr, int dat) {

    i2c.start();
    i2c.stop();

    i2c.start();
    i2c.write(addr);
    i2c.write(dat);
    i2c.stop();
}

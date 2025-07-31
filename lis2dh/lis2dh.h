#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace lis2dh {

// LIS2DH I2C addresses
static const uint8_t LIS2DH_ADDRESS_LOW = 0x18;   // SDO connected to GND
static const uint8_t LIS2DH_ADDRESS_HIGH = 0x19;  // SDO connected to VCC

// LIS2DH registers
static const uint8_t LIS2DH_REG_WHO_AM_I = 0x0F;
static const uint8_t LIS2DH_REG_CTRL_REG1 = 0x20;
static const uint8_t LIS2DH_REG_CTRL_REG4 = 0x23;
static const uint8_t LIS2DH_REG_OUT_X_L = 0x28;

// WHO_AM_I response
static const uint8_t LIS2DH_WHO_AM_I_RESPONSE = 0x33;

// Scale factors for different ranges (±2g default)
static const float LIS2DH_SCALE_2G = 0.000976f;   // 2g / 2^11 for 12-bit left-justified
static const float LIS2DH_SCALE_4G = 0.001952f;   // 4g / 2^11
static const float LIS2DH_SCALE_8G = 0.003904f;   // 8g / 2^11
static const float LIS2DH_SCALE_16G = 0.007808f;  // 16g / 2^11

enum LIS2DHScale {
  LIS2DH_SCALE_2G_VAL = 0b00,
  LIS2DH_SCALE_4G_VAL = 0b01,
  LIS2DH_SCALE_8G_VAL = 0b10,
  LIS2DH_SCALE_16G_VAL = 0b11,
};

enum LIS2DHDataRate {
  LIS2DH_DATARATE_1HZ = 0b0001,
  LIS2DH_DATARATE_10HZ = 0b0010,
  LIS2DH_DATARATE_25HZ = 0b0011,
  LIS2DH_DATARATE_50HZ = 0b0100,
  LIS2DH_DATARATE_100HZ = 0b0101,
  LIS2DH_DATARATE_200HZ = 0b0110,
  LIS2DH_DATARATE_400HZ = 0b0111,
};

class LIS2DH : public PollingComponent, public i2c::I2CDevice {
 public:
  void setup() override;
  void update() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  void set_accel_x_sensor(sensor::Sensor *accel_x_sensor) { accel_x_sensor_ = accel_x_sensor; }
  void set_accel_y_sensor(sensor::Sensor *accel_y_sensor) { accel_y_sensor_ = accel_y_sensor; }
  void set_accel_z_sensor(sensor::Sensor *accel_z_sensor) { accel_z_sensor_ = accel_z_sensor; }

  void set_scale(LIS2DHScale scale) { scale_ = scale; }
  void set_data_rate(LIS2DHDataRate data_rate) { data_rate_ = data_rate; }

 protected:
  sensor::Sensor *accel_x_sensor_{nullptr};
  sensor::Sensor *accel_y_sensor_{nullptr};
  sensor::Sensor *accel_z_sensor_{nullptr};

  LIS2DHScale scale_{LIS2DH_SCALE_2G_VAL};
  LIS2DHDataRate data_rate_{LIS2DH_DATARATE_100HZ};

  bool read_acceleration_(float *x, float *y, float *z);
  float get_scale_factor_();
};

}  // namespace lis2dh
}  // namespace esphome

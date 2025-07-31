#include "lis2dh.h"
#include "esphome/core/log.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace lis2dh {

static const char *const TAG = "lis2dh";

void LIS2DH::setup() {
  ESP_LOGCONFIG(TAG, "Setting up LIS2DH...");

  // Check WHO_AM_I register
  uint8_t who_am_i;
  if (!this->read_byte(LIS2DH_REG_WHO_AM_I, &who_am_i)) {
    ESP_LOGE(TAG, "Failed to read WHO_AM_I register");
    this->mark_failed();
    return;
  }

  if (who_am_i != LIS2DH_WHO_AM_I_RESPONSE) {
    ESP_LOGE(TAG, "Invalid WHO_AM_I response: 0x%02X", who_am_i);
    this->mark_failed();
    return;
  }

  ESP_LOGD(TAG, "LIS2DH detected successfully");

  // Configure CTRL_REG1: Normal mode, data rate, XYZ enabled
  uint8_t ctrl_reg1 = (this->data_rate_ << 4) | 0x07;  // XYZ enabled
  if (!this->write_byte(LIS2DH_REG_CTRL_REG1, ctrl_reg1)) {
    ESP_LOGE(TAG, "Failed to configure CTRL_REG1");
    this->mark_failed();
    return;
  }

  // Configure CTRL_REG4: Scale and high resolution mode
  uint8_t ctrl_reg4 = (this->scale_ << 4) | 0x08;  // High resolution mode
  if (!this->write_byte(LIS2DH_REG_CTRL_REG4, ctrl_reg4)) {
    ESP_LOGE(TAG, "Failed to configure CTRL_REG4");
    this->mark_failed();
    return;
  }

  ESP_LOGCONFIG(TAG, "LIS2DH setup complete");
}

void LIS2DH::update() {
  float x, y, z;
  if (this->read_acceleration_(&x, &y, &z)) {
    if (this->accel_x_sensor_ != nullptr)
      this->accel_x_sensor_->publish_state(x);
    if (this->accel_y_sensor_ != nullptr)
      this->accel_y_sensor_->publish_state(y);
    if (this->accel_z_sensor_ != nullptr)
      this->accel_z_sensor_->publish_state(z);

    ESP_LOGD(TAG, "X: %.3f g, Y: %.3f g, Z: %.3f g", x, y, z);
  }
}

void LIS2DH::dump_config() {
  ESP_LOGCONFIG(TAG, "LIS2DH:");
  LOG_I2C_DEVICE(this);
  if (this->is_failed()) {
    ESP_LOGE(TAG, "Communication with LIS2DH failed!");
  }
  LOG_UPDATE_INTERVAL(this);
  LOG_SENSOR("  ", "Acceleration X", this->accel_x_sensor_);
  LOG_SENSOR("  ", "Acceleration Y", this->accel_y_sensor_);
  LOG_SENSOR("  ", "Acceleration Z", this->accel_z_sensor_);
}

bool LIS2DH::read_acceleration_(float *x, float *y, float *z) {
  uint8_t data[6];

  // Read all 6 bytes starting from OUT_X_L with auto-increment
  if (!this->read_bytes(LIS2DH_REG_OUT_X_L | 0x80, data, 6)) {
    ESP_LOGE(TAG, "Failed to read acceleration data");
    return false;
  }

  // Convert raw data to acceleration values
  int16_t raw_x = (int16_t)((data[1] << 8) | data[0]);
  int16_t raw_y = (int16_t)((data[3] << 8) | data[2]);
  int16_t raw_z = (int16_t)((data[5] << 8) | data[4]);

  // Convert to g (12-bit resolution, left-justified)
  float scale_factor = this->get_scale_factor_();
  *x = (raw_x >> 4) * scale_factor;
  *y = (raw_y >> 4) * scale_factor;
  *z = (raw_z >> 4) * scale_factor;

  return true;
}

float LIS2DH::get_scale_factor_() {
  switch (this->scale_) {
    case LIS2DH_SCALE_2G_VAL:
      return LIS2DH_SCALE_2G;
    case LIS2DH_SCALE_4G_VAL:
      return LIS2DH_SCALE_4G;
    case LIS2DH_SCALE_8G_VAL:
      return LIS2DH_SCALE_8G;
    case LIS2DH_SCALE_16G_VAL:
      return LIS2DH_SCALE_16G;
    default:
      return LIS2DH_SCALE_2G;
  }
}

}  // namespace lis2dh
}  // namespace esphome

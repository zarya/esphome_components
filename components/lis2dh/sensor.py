import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_ID,
    STATE_CLASS_MEASUREMENT,
    UNIT_METER_PER_SECOND_SQUARED,
)
from . import LIS2DH, CONF_LIS2DH_ID

DEPENDENCIES = ["lis2dh"]

# Use "g" as unit instead of m/s² for acceleration
UNIT_G = "g"

CONF_ACCELERATION_X = "acceleration_x"
CONF_ACCELERATION_Y = "acceleration_y"
CONF_ACCELERATION_Z = "acceleration_z"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(CONF_LIS2DH_ID): cv.use_id(LIS2DH),
    cv.Optional(CONF_ACCELERATION_X): sensor.sensor_schema(
        unit_of_measurement=UNIT_G,
        accuracy_decimals=3,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
    cv.Optional(CONF_ACCELERATION_Y): sensor.sensor_schema(
        unit_of_measurement=UNIT_G,
        accuracy_decimals=3,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
    cv.Optional(CONF_ACCELERATION_Z): sensor.sensor_schema(
        unit_of_measurement=UNIT_G,
        accuracy_decimals=3,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
})

async def to_code(config):
    parent = await cg.get_variable(config[CONF_LIS2DH_ID])

    if CONF_ACCELERATION_X in config:
        sens = await sensor.new_sensor(config[CONF_ACCELERATION_X])
        cg.add(parent.set_accel_x_sensor(sens))

    if CONF_ACCELERATION_Y in config:
        sens = await sensor.new_sensor(config[CONF_ACCELERATION_Y])
        cg.add(parent.set_accel_y_sensor(sens))

    if CONF_ACCELERATION_Z in config:
        sens = await sensor.new_sensor(config[CONF_ACCELERATION_Z])
        cg.add(parent.set_accel_z_sensor(sens))

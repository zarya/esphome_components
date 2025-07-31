import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c
from esphome.const import CONF_ID

DEPENDENCIES = ["i2c"]
MULTI_CONF = True

lis2dh_ns = cg.esphome_ns.namespace("lis2dh")
LIS2DH = lis2dh_ns.class_("LIS2DH", cg.PollingComponent, i2c.I2CDevice)

CONF_LIS2DH_ID = "lis2dh_id"

CONFIG_SCHEMA = (
    cv.Schema({
        cv.GenerateID(): cv.declare_id(LIS2DH),
        cv.Optional("address", default=0x19): cv.i2c_address,
    })
    .extend(cv.polling_component_schema("1s"))
    .extend(i2c.i2c_device_schema(0x19))
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)


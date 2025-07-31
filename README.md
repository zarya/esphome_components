# My ESPhome custom components

## LIS2DN
```yaml
# External component definition
external_components:
  - source: github://zarya/esphome_components
    components: [lis2dh]

i2c:
  sda: 6
  scl: 7
  scan: true

lis2dh:
  - id: lis2dh_component
    address: 0x19
    update_interval: 1s

sensor:
  - platform: lis2dh
    acceleration_x:
      name: "LIS2DH X Acceleration"
    acceleration_y:
      name: "LIS2DH Y Acceleration"
    acceleration_z:
      name: "LIS2DH Z Acceleration"
```

configuration: {}
triggers:
  - id: "1"
    configuration:
      itemName: ESP32__BMP280_Temperature
    type: core.ItemStateUpdateTrigger
conditions:
  - inputs: {}
    id: "2"
    configuration:
      itemName: ESP32__BMP280_Temperature
      operator: <
      state: "29.5"
    type: core.ItemStateCondition
actions:
  - inputs: {}
    id: "3"
    configuration:
      command: OFF
      itemName: Temp30_AlertSent
    type: core.ItemCommandAction

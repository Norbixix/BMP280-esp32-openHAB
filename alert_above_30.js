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
      operator: ">"
      state: "30"
    type: core.ItemStateCondition
  - inputs: {}
    id: "3"
    configuration:
      itemName: Temp30_AlertSent
      operator: =
      state: OFF
    type: core.ItemStateCondition
actions:
  - inputs: {}
    id: "4"
    configuration:
      type: application/javascript
      script: >
        var t = items.getItem("ESP32__BMP280_Temperature").state.toString();
        actions.get("telegram","telegram:telegramBot:83bd9d266f")
          .sendTelegram("🔥⚠️ Uwaga! Temperatura przekroczyła 30°C. Obecnie wynosi: " + t + " ⚠️🔥");
    type: script.ScriptAction
  - inputs: {}
    id: "5"
    configuration:
      command: ON
      itemName: Temp30_AlertSent
    type: core.ItemCommandAction

configuration: {}
triggers:
  - id: "1"
    configuration:
      cronExpression: 0 */30 * * * ?
    type: timer.GenericCronTrigger
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
      state: ON
    type: core.ItemStateCondition
actions:
  - inputs: {}
    id: "4"
    configuration:
      type: application/javascript
      script: >
        var t = items.getItem("ESP32__BMP280_Temperature").state.toString();
        actions.get("telegram","telegram:telegramBot:83bd9d266f")
          .sendTelegram("⏰🔥 Reminder: It's still above 30°C! Now: " + t + " 🔥⏰");
    type: script.ScriptAction

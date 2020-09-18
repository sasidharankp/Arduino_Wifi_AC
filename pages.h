static const char CONFIG_PAGE[] PROGMEM = R"(
{
  "uri": "/mqtt",
  "title": "MQTT",
  "menu": true,
  "element": [
    {
      "name": "text",
      "type": "ACText",
      "value": "MQTT Configuration",
      "style": "font-family:Arial;font-size:18px;font-weight:400;color:#191970"
    },
    {
      "name": "mqttusername",
      "type": "ACInput",
      "label": "MQTT USERNAME",
      "placeholder":"Adafruit UserName",
      "pattern": "^(([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\\-]*[a-zA-Z0-9])\\.)*([A-Za-z0-9]|[A-Za-z0-9][A-Za-z0-9\\-]*[A-Za-z0-9])$"
    },
    {
      "name": "mqttpassword",
      "type": "ACInput",
      "label": "MQTT PASSWORD",
      "placeholder":"Adafruit AIO KEY",
      "pattern": "^(([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\\-]*[a-zA-Z0-9])\\.)*([A-Za-z0-9]|[A-Za-z0-9][A-Za-z0-9\\-]*[A-Za-z0-9])$"
    },
    {
      "name": "adafruitname",
      "type": "ACInput",
      "label": "Adafruit User Name",
      "placeholder":"Adafruit Username for feed configuration",
      "pattern": "^(([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\\-]*[a-zA-Z0-9])\\.)*([A-Za-z0-9]|[A-Za-z0-9][A-Za-z0-9\\-]*[A-Za-z0-9])$"
    },
    {
      "name": "Warning",
      "type": "ACText",
      "value": "Do Not Change the below Fields Unless you know What you are doing",
      "style": "font-family:Arial;font-size:18px;font-weight:400;color:#ba2d25"
    },
    {
      "name": "mqttserver",
      "type": "ACInput",
      "label": "MQTT Server",
      "placeholder":"MQTT Server",
      "value":"io.adafruit.com"
    },
    {
      "name": "mqttport",
      "type": "ACInput",
      "label": "MQTT Port",
      "placeholder":"MQTT Port",
      "value":"1883"
    },
    {
      "name": "irch",
      "type": "ACInput",
      "label": "IR Channel",
      "placeholder":"Channel To Receive IR Commands",
      "value":"/feeds/ircommands"
    },
    {
      "name": "commandch",
      "type": "ACInput",
      "label": "Device Control Channel",
      "placeholder":"Channel to Receive device Commands",
      "value":"/feeds/devicecommands"
    },
    {
      "name": "telemetrych",
      "type": "ACInput",
      "label": "Device Telemetry Channel",
      "placeholder":"Channel to Send Telenetry info",
      "value":"/feeds/devicetelemetry"
    },
    {
      "name": "statech",
      "type": "ACInput",
      "label": "Device State Channel",
      "placeholder":"Channel to Send Device State",
      "value":"/feeds/devicestate"
    },
    {
      "name": "load",
      "type": "ACSubmit",
      "value": "Load",
      "uri": "/mqtt"
    },
    {
      "name": "save",
      "type": "ACSubmit",
      "value": "Save",
      "uri": "/save"
    },
    {
      "name": "adjust_width",
      "type": "ACElement",
      "value": "<script type=\"text/javascript\">window.onload=function(){var t=document.querySelectorAll(\"input[type='text']\");for(i=0;i<t.length;i++){var e=t[i].getAttribute(\"placeholder\");e&&t[i].setAttribute(\"size\",40)}};</script>"
    }
  ]
}
)";

static const char SAVE_PAGE[] PROGMEM = R"(
{
  "uri": "/save",
  "title": "MQTT",
  "menu": false,
  "element": [
    {
      "name": "caption",
      "type": "ACText",
      "format": "Elements have been saved to %s",
      "style": "font-family:Arial;font-size:18px;font-weight:400;color:#191970"
    },
    {
      "name": "validated",
      "type": "ACText",
      "style": "color:red"
    },
    {
      "name": "echo",
      "type": "ACText",
      "style": "font-family:monospace;font-size:small;white-space:pre;"
    },
    {
      "name": "ok",
      "type": "ACSubmit",
      "value": "OK",
      "uri": "/mqtt"
    }
  ]
}
)";

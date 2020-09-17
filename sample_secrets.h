#define MQTT_SERVER ""        //MQTT SERVER
#define MQTT_PORT             //MQTT SERVER PORT 
#define MQTT_USERNAME ""      //MQTT USERNAME
#define MQTT_PASSWORD ""      //AIO KEY


      //FEEL FREE TO MODIFY THE TOPIC PATTERNS OR ADD ADDITIONAL TOPICS HERE //
#define LISTENING_TOPIC MQTT_USERNAME"/feeds/mushroom.threshold";
#define OPERATING_MODE_TOPIC MQTT_USERNAME"/feeds/mushroom.operating-mode";
#define SPRINKLER_STATE_TOPIC MQTT_USERNAME"/feeds/mushroom.sprinkler-state";
#define SPRINKLER_OVERRIDE_TOPIC MQTT_USERNAME"/feeds/mushroom.sprinkler-override";
#define SPRINKLER_DURATION_TOPIC MQTT_USERNAME"/feeds/mushroom.sprinkler-duration";
#define TEMPERATURE_TOPIC MQTT_USERNAME"/feeds/mushroom.temperature";
#define HUMIDITY_TOPIC MQTT_USERNAME"/feeds/mushroom.humidity";

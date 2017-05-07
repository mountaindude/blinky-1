void setup();
void reconnect() ;
void setup_wifi();
int MapLedIdToPin(int ledId);

void callback(char* topic, byte* payload, unsigned int length);



// Max MQTT topic length (# of characters)
#define MAX_MQTT_MSG_LENGTH 100

// Blinking modes
#define LED_BREATH_SLOW 1
#define LED_BREATH_MEDIUM 2
#define LED_BREATH_FAST 3
#define LED_BLINK_SLOW 4
#define LED_BLINK_MEDIUM 5
#define LED_BLINK_FAST 6

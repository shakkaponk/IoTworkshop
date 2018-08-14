#include <WioLTEforArduino.h>
#include <WioLTEClient.h>
#include <PubSubClient.h>		// https://github.com/SeeedJP/pubsubclient
#include <stdio.h>
#include <ArduinoJson.h>

// 定数
#define capabilityAlternateId  "9ae29168b5ae2233"
#define sensorAlternateId      "2a58771a9b9efe26"

#define APN               "soracom.io"
#define USERNAME          "sora"
#define PASSWORD          "sora"

#define MQTT_SERVER_HOST  "beam.soracom.io"
#define MQTT_SERVER_PORT  (1883)

#define ID                "ab69d565671f1846"
#define OUT_TOPIC         "measures/ab69d565671f1846"
#define IN_TOPIC          "commands/ab69d565671f1846"

#define INTERVAL          (20000)
#define SENSOR_PIN   (WIOLTE_D38)


WioLTE Wio;
WioLTEClient WioClient(&Wio);
PubSubClient MqttClient;

void callback(char* topic, byte* payload, unsigned int length) {
  char c[length + 1];
  for (int i = 0; i < length; i++) c[i] =  (char) payload[i];
  c[length] = 0;
  SerialUSB.print("Subscribe:");
  SerialUSB.println(c);
  char *pos = strchr(c, '#');
  if (pos != NULL) {
    unsigned long r = 0;
    unsigned long g = 0;
    unsigned long b = 0;
    char rx[3] = {pos[1], pos[2], 0};
    r = strtoul(rx, NULL, 16);
    char gx[3] = {pos[3], pos[4], 0};
    g = strtoul(gx, NULL, 16);
    char bx[3] = {pos[5], pos[6], 0};
    b = strtoul(bx, NULL, 16);
    Wio.LedSetRGB(r, g, b);
  }
}


void setup() {
  delay(200);
  
  SerialUSB.println("");
  SerialUSB.println("--- START ---------------------------------------------------");
  
  SerialUSB.println("### I/O Initialize.");
  Wio.Init();
  
  SerialUSB.println("### Power supply ON.");
  Wio.PowerSupplyLTE(true);
  delay(500);

  SerialUSB.println("### Turn on or reset.");
  if (!Wio.TurnOnOrReset()) {
    SerialUSB.println("### Turn on or reset ERROR! ###");
    return;
  }

  SerialUSB.println("### Connecting to \""APN"\".");
  if (!Wio.Activate(APN, USERNAME, PASSWORD)) {
    SerialUSB.println("### APN ERROR! ###");
    return;
  }

  SerialUSB.println("### Connecting to MQTT server \""MQTT_SERVER_HOST"\"");
  MqttClient.setServer(MQTT_SERVER_HOST, MQTT_SERVER_PORT);
  MqttClient.setCallback(callback);
  MqttClient.setClient(WioClient);
  if (!MqttClient.connect(ID)) {
    SerialUSB.println("### MQTT ERROR! ###");
    return;
  }
  MqttClient.subscribe(IN_TOPIC);

  SerialUSB.println("### MQTT Setup completed.");

  TemperatureAndHumidityBegin(SENSOR_PIN);
  
}

void loop() {
  char data[200];
  char temdata[200];
  
  float temp;
  float humi;


StaticJsonBuffer<200> jsonBuffer;
  char buffer[256];
  JsonObject& root = jsonBuffer.createObject();
  root["capabilityAlternateId"] = capabilityAlternateId;
  root["sensorAlternateId"] = sensorAlternateId;
  JsonArray& measures = root.createNestedArray("measures");


  JsonObject& temphumi = jsonBuffer.createObject();
  

  if (!TemperatureAndHumidityRead(&temp, &humi)) {
    SerialUSB.println("ERROR!");
    goto err;
  }
  
  // 取得した気温、湿度をセットする。
  temphumi["temperature"] = """" + String(temp) + """";
  temphumi["humidity"]    = """" + String(humi) + """";
  measures.add(temphumi);
  
  
  root.printTo(buffer, sizeof(buffer));
  //Serial.println(buffer);
  SerialUSB.println(buffer);

  root.printTo(data, sizeof(data));
  
  //sprintf(data, "{\"uptime\":%lu}", millis() / 1000);
  SerialUSB.print("TOPIC:");
  SerialUSB.print(OUT_TOPIC);
  SerialUSB.println("");
  SerialUSB.print("Publish:");
  SerialUSB.print(data);
  SerialUSB.println("");
  MqttClient.publish(OUT_TOPIC, data,256);
 
   delay(40);
  
err:
  unsigned long next = millis();
  while (millis() < next + INTERVAL)
  {
    MqttClient.loop();
  }
}
////////////////////////////////////////////////////////////////////////////////////////
//

int TemperatureAndHumidityPin;

void TemperatureAndHumidityBegin(int pin)
{
  TemperatureAndHumidityPin = pin;
  DHT11Init(TemperatureAndHumidityPin);
}

bool TemperatureAndHumidityRead(float* temperature, float* humidity)
{
  byte data[5];
  
  DHT11Start(TemperatureAndHumidityPin);
  for (int i = 0; i < 5; i++) data[i] = DHT11ReadByte(TemperatureAndHumidityPin);
  DHT11Finish(TemperatureAndHumidityPin);
  
  if(!DHT11Check(data, sizeof (data))) return false;
  if (data[1] >= 10) return false;
  if (data[3] >= 10) return false;

  *humidity = (float)data[0] + (float)data[1] / 10.0f;
  *temperature = (float)data[2] + (float)data[3] / 10.0f;

  return true;
}

////////////////////////////////////////////////////////////////////////////////////////
//

void DHT11Init(int pin)
{
  digitalWrite(pin, HIGH);
  pinMode(pin, OUTPUT);
}

void DHT11Start(int pin)
{
  // Host the start of signal
  digitalWrite(pin, LOW);
  delay(18);
  
  // Pulled up to wait for
  pinMode(pin, INPUT);
  while (!digitalRead(pin)) ;
  
  // Response signal
  while (digitalRead(pin)) ;
  
  // Pulled ready to output
  while (!digitalRead(pin)) ;
}

byte DHT11ReadByte(int pin)
{
  byte data = 0;
  
  for (int i = 0; i < 8; i++) {
    while (digitalRead(pin)) ;

    while (!digitalRead(pin)) ;
    unsigned long start = micros();

    while (digitalRead(pin)) ;
    unsigned long finish = micros();

    if ((unsigned long)(finish - start) > 50) data |= 1 << (7 - i);
  }
  
  return data;
}

void DHT11Finish(int pin)
{
  // Releases the bus
  while (!digitalRead(pin)) ;
  digitalWrite(pin, HIGH);
  pinMode(pin, OUTPUT);
}

bool DHT11Check(const byte* data, int dataSize)
{
  if (dataSize != 5) return false;

  byte sum = 0;
  for (int i = 0; i < dataSize - 1; i++) {
    sum += data[i];
  }

  return data[dataSize - 1] == sum;
}

////////////////////////////////////////////////////////////////////////////////////////


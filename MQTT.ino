#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

//  Add values suitable for your network.
byte mac[]    = {  χ, χ, χ, χ, χ, χ };
IPAddress ip(χ, χ, χ, χ);
IPAddress server(χ, χ,χ, χ);

char data[80];
StaticJsonDocument<256> doc; // 6 version JSon


void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
  char inData[80];
  Serial.print("payload: ");
  for (int i = 26; i < length; i++) {
    inData[(i - 26)] = (char)payload[i];
  }
  Serial.println("value");
  deserializeJson(doc, inData);
  // Pull the value from key "number" from the JSON message {"number": 1 }
  int val = doc["number"];
  // Show number value 
  Serial.println(val);

}



EthernetClient ethClient;
PubSubClient mqttClient(ethClient);

void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {

    String value = "\"number\": " + String(2) ;
    String payload = "{ \"devices\": \"*\",\"payload\": {" + value + "}}";
	
	//send json message
    payload.toCharArray(data, (payload.length() + 1));
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect("arduinoClient")) {
      Serial.println("connected");
      // Once connected, publish in topic	  
      mqttClient.publish("topic", data);
	  
      // subscribe in topic
      mqttClient.subscribe("topic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(57600);

  mqttClient.setServer(server, 1883);
  mqttClient.setCallback(callback);

  Ethernet.begin(mac, ip);
  // Allow the hardware to sort itself out
  delay(1500);
}

void loop()
{
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();
}

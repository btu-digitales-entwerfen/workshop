#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi()
{
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  WiFi.begin("vrhalla-guest", "guesthalla");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void reconnect(String clientId)
{
  while (!client.connected())
  {
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");
      client.subscribe("test-blockhuzzah-topic");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");
}
//----------------------------------------
void setup()
{
  Serial.begin(9600);
  setup_wifi();
  client.setServer("test.mosquitto.org", 1883);
  client.setCallback(callback);
}

String clientId = "ESP8266Client-" + String(random(0xffff), HEX);

void loop()
{
  if (!client.connected())
  {
    reconnect(clientId);
  }
  for (auto i = 0; i < 10; i++)
  {
    client.publish("test-blockhuzzah-topic", (String(i) + "   from: " + clientId).c_str());
  }
  client.loop();
  delay(1000);
}
// Inspiration: https://www.reddit.com/r/arduino/comments/12uy1r2/never_gunna_give_you_up_wifi_beacon/

//"Never Gonna Give You Up" WiFi Beacon
//code written by NoMoreImFull for the good of humanity
//Lyrics by God Emperor Rick Astley

//Each line of the lyrics in the array
//is 32 characters (including spaces) or less
//to comply with SSID naming limitations.

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

String never_gonna[] = {
  "Were no strangers to love",
  "You know the rules and so do I",
  "A full commitment is",
  "what Im thinking of",
  "You wouldnt get this from",
  "any other guy",
  "I just wanna tell you",
  "how Im feeling",
  "Gotta make you understand",
  "Never gonna give you up",
  "Never gonna let you down",
  "Never gonna run around",
  "and desert you",
  "Never gonna make you cry",
  "Never gonna say goodbye",
  "Never gonna tell a lie",
  "and hurt you",
  "Weve known each other for so long",
  "Your hearts been aching, but",
  "youre too shy to say it",
  "Inside we both know",
  "whats been going on",
  "We know the game ",
  "and were gonna play it",
  "And if you ask me how Im feeling",
  "Dont tell me youre too blind to see",
  "Never gonna give you up",
  "Never gonna let you down",
  "Never gonna run around",
  "and desert you",
  "Never gonna make you cry",
  "Never gonna say goodbye",
  "Never gonna tell a lie",
  "and hurt you",
  "Never gonna give you up",
  "Never gonna let you down",
  "Never gonna run around",
  "and desert you",
  "Never gonna make you cry",
  "Never gonna say goodbye",
  "Never gonna tell a lie",
  "and hurt you",
  "Weve known each other for so long",
  "Your hearts been aching, but",
  "youre too shy to say it",
  "Inside we both know",
  "whats been going on",
  "We know the game ",
  "and were gonna play it",
  "I just wanna tell you",
  "how Im feeling",
  "Gotta make you understand",
  "Never gonna give you up",
  "Never gonna let you down",
  "Never gonna run around",
  "and desert you",
  "Never gonna make you cry",
  "Never gonna say goodbye",
  "Never gonna tell a lie",
  "and hurt you",
  "Never gonna give you up",
  "Never gonna let you down",
  "Never gonna run around",
  "and desert you",
  "Never gonna make you cry",
  "Never gonna say goodbye",
  "Never gonna tell a lie",
  "and hurt you",
  "Never gonna give you up",
  "Never gonna let you down",
  "Never gonna run around",
  "and desert you",
  "Never gonna make you cry",
  "Never gonna say goodbye",
  "Never gonna tell a lie",
  "and hurt you"
};

const int update_interval = 30000; // SSID update interval in ms

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);

String responseHTML = ""
                      "<!DOCTYPE html><html lang='en'><head>"
                      "<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>"
                      "<meta name='viewport' content='width=device-width'>"
                      "<title>Rickrolling the WiFi</title></head><body>"
                      "⡀⠛⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⣿⣿⣿⣿⣿⣿⣿⣿⣶⣤⣄⣀⠀⠀⠀⠀⠀⠀⠀⠉⠁⠀⣀⣀⠀⣀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣄⡀⠀⠀⠀<br>"
                      "⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣾⣿⣿⣿⣿⣿⣛⣿⣿⣷⣿⣿⣯⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⣏⠉⣿⠉⢹⠟⢉⠉⢻⣿⠉⢻⠋⠙⡏⣿⠋⢻⡏⠉⣿⠉⣉⣻⠀<br>"
                      "⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣻⡀⠈⢀⣿⠀⢸⠀⠀⣿⠀⢸⠀⠰⣿⣿⠀⢸⠁⢀⡟⠀⢹⣿⠀<br>"
                      "⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⣿⣿⣿⡿⠿⠿⢿⣿⣿⣿⣿⣿⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣧⠀⣼⣿⠀⢸⡀⠀⣏⠀⢸⠀⠀⣿⣿⡄⠘⠀⢸⡇⠀⢰⣾⠀<br>"
                      "⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⡿⠋⠀⠀⠀⠀⠀⠈⠉⠉⠁⠀⠀⠈⢻⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣀⣿⣿⣆⡈⢁⣰⣿⣄⠘⢀⣼⣿⣿⣇⣀⣀⣼⣧⣀⣈⣹⡇<br>"
                      "⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⣿⣿⣿⣿⣿⣿⠟⠿⢿⣿⠿⠛⠛⠻⠿⠿⠻⠛⠉⠉⠉⠀<br>"
                      "⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⠀⠀⢀⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠹⣧⣄⠀⠀⠀⠀⠀⠀⣴⠶⡶⠿⣿⣿⠿⠿⢿⡿⠿⠿⣿⠿⢿⡿⢿⡿⠀⠀⠀⠀⠀⠀⠀<br>"
                      "⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠰⣿⣿⠀⠀⢨⣭⣽⣿⡇⠀⢠⣾⣿⣿⣷⣆⠀⠀⢸⣿⠀⠀⠀⠀⠀⠀⣿⠀⢱⡆⠈⣿⠀⢴⣾⡇⠀⣶⣿⠀⠘⡇⠀⡇⠀⠀⠀⠀⠀⠀⠀<br>"
                      "⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⣿⠉⠛⠀⠀⠀⠀⠉⠁⠀⠀⠘⡏⠉⠉⠛⠋⠀⣠⣼⣿⠀⠀⠀⠀⠀⠀⣿⠀⢨⡁⠺⣿⠀⣈⣹⡇⠀⣉⣿⠀⡀⠁⠀⡇⠀⠀⠀⠀⠀⠀⠀<br>"
                      "⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⡀⠀⠀⠀⠀⠀⣀⠀⠀⠀⠀⢹⡄⠀⠀⠀⠀⣿⣿⡿⠀⠀⠀⠀⠀⠀⣿⠀⠸⠇⠀⣿⠀⠹⢿⡇⠀⠿⢿⠀⢸⡀⠀⡇⠀⠀⠀⠀⠀⠀⠀<br>"
                      "⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⢷⣄⡀⠀⢠⡾⠋⠀⠛⢶⣶⣾⡇⠀⣠⠄⢰⣿⠟⠀⠀⠀⠀⠀⠀⠀⠻⢶⣶⡶⠚⠓⠶⠶⠾⠷⠶⠶⠾⠶⠾⠳⠾⠟⠀⠀⠀⠀⠀⠀⠀<br>"
                      "⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣹⡷⣠⠏⠙⢷⣶⠲⠶⠶⣷⣶⡿⠋⢀⣾⠃⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣹⣧⡀⢀⠀⠀⣀⣀⣀⡀⢀⣀⣀⣀⣀⣀⠀⠀⠀⠀⠀⠀⠀<br>"
                      "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⠟⣫⣽⠃⠀⠀⠀⠉⠉⠙⠛⠋⠀⠀⢀⣾⡃⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠉⢉⡉⠻⡏⠉⣿⠟⢉⡉⠙⣿⠉⢹⡏⢉⡿⠀⠀⠀⠀⠀⠀⠀<br>"
                      "⠀⠀⠀⠀⠀⠀⠀⠀⢀⡴⠛⠁⠀⣼⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣼⡏⢳⡄⠀⠀⠀⠀⠀⠀⠀⠀⣿⠀⠸⠇⣰⡇⠀⣿⠀⢸⣧⣀⣿⠀⠈⠀⣼⠁⠀⠀⠀⠀⠀⠀⠀<br>"
                      "⠀⠀⠀⠀⠀⠀⢀⣴⠏⠀⠀⠀⢸⣿⣿⡀⠀⠀⠰⣦⣄⡀⣀⣤⡾⣿⣿⣧⠀⠻⢦⣄⡀⠀⠀⠀⠀⠀⣿⠀⢸⠀⠈⡇⠀⣿⠀⢸⡟⠛⣿⠀⢠⠀⢹⣆⠀⠀⠀⠀⠀⠀⠀<br>"
                      "⠀⠀⠀⠀⠀⠀⠘⠁⠀⠀⠀⠀⣾⣿⣿⣷⣄⡀⠀⠙⠿⣿⣏⣽⣿⣿⣿⣿⠄⢸⣧⠈⠙⠶⣤⣀⠀⠀⣿⣀⣸⣄⣠⣷⣀⣿⣦⣀⣁⣠⣿⣀⣸⣧⣀⣿⠀⠀⠀⠀⠀⠀⠀<br>"
                      "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣿⣿⣿⣿⣿⣿⣶⣶⣶⣿⣿⣿⣿⣿⣿⣿⠀⠀⠹⣆⠀⠀⠀⠉⠳⣦⡀⠉⠉⠙⠻⣿⠉⠁⠀⠉⠉⠀⠀⠈⠉⠀⠉⠹⠇⠀⠀⠀⠀⠀⠀<br>"
                      "⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠛⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠀⠀⠀⢿⡆⠀⠀⠀⠀⠻⣿⠓⠒⠲⢦⣹⠷⠒⠲⣶⡖⠒⣶⣶⠒⢶⣾⠗⠒⠲⡶⠒⡖⠶⣄<br>"
                      "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⠀⡞⣿⠀⠀⠀⠀⠀⢹⠀⢹⡀⢈⡏⠀⣿⠀⢸⡇⠀⣿⡟⠀⢸⣿⠀⢸⣶⡇⠀⢳⠀⢸<br>"
                      "⠀⠀⠀⠀⠀⠀⠀⢀⠀⠀⠀⠀⠀⣻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠁⠀⢀⣰⠃⢹⡆⠀⠀⠀⠀⢸⠀⢠⠀⠛⡇⠀⣿⠀⢸⡇⠀⣿⡇⠀⢸⣿⠀⢠⣬⡇⠀⢸⠀⢸<br>"
                      "⠀⠀⠀⠀⠀⠀⠀⠙⠛⠛⠋⠉⠀⠺⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠃⠶⠞⠋⠀⠀⠀⢿⠀⠀⠀⠀⣸⠀⢸⠀⢰⣧⠀⠛⠀⣸⡇⠀⠛⣧⠀⠘⢻⠀⠘⠛⡇⠀⠚⠀⢸<br>"
                      "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠃⠀⠀⠀⠀⠀⠀⠀⠀⠆⠀⠀⠀⠿⠓⠛⠓⠛⠉⠙⠒⠚⠉⠛⣛⡚⠛⠛⠛⠛⠛⠓⡚⠛⠛⠓⠛⠉<br>"
                      "<p><a href='https://youtu.be/dQw4w9WgXcQ'>Have fun</a></p></body></html>";

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("RickRoller started...");

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  
  Serial.println("Setting SSID to '" + never_gonna[0] + "'.");
  WiFi.softAP(never_gonna[0].c_str());

  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", apIP);

  // reply to all requests with same HTML
  webServer.onNotFound([]() {
    webServer.send(200, "text/html; charset=utf-8", responseHTML);
  });
  webServer.begin();
}

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
  
  static unsigned long last_update_time = 0;
  if (millis() - last_update_time > update_interval) {
    static int current_line = 0;
    current_line = (current_line + 1) % (sizeof(never_gonna) / sizeof(String));
    Serial.println("Setting SSID to '" + never_gonna[current_line] + "'.");
    WiFi.softAP(never_gonna[current_line].c_str());
    last_update_time = millis();
  }
  delay(1000);
}

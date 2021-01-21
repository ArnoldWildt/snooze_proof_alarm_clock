#include <Arduino.h>
#include <WiFiManager.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include "LittleFS.h" // Include the SPIFFS library "LittleFS" https://arduino-esp8266.readthedocs.io/en/latest/filesystem.html#spiffs-and-littlefs

#define Project_name "Mobi-Clone"

ESP8266WebServer server(80);

String get_content_type(String filename); // convert the file extension to the MIME type
bool handle_file_read(String path);       // send the right file to the client (if it exists)
void handle_not_found();
void list_dir(const char *dirname);

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP

  WiFiManager wm;
  wm.disconnect();
  //wm.resetSettings();

  bool res;
  res = wm.autoConnect(Project_name, "1234512345");

  if (!res)
  {
    Serial.println("Failed to connect");
    // ESP.restart();
  }
  else
  {
    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");

    if (!MDNS.begin("esp8266"))
    { // Start the mDNS responder for esp8266.local
      Serial.println("Error setting up MDNS responder!");
    }
    Serial.println("mDNS responder started");

    LittleFS.begin(); // Start the SPI Flash Files System
    Serial.println("Files:");
    list_dir("/");

    server.onNotFound(handle_not_found); // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

    server.begin(); // Actually start the server
    Serial.println("HTTP server started");
  }
}

void loop()
{
  server.handleClient();
  MDNS.update();
}

String get_content_type(String filename)
{ // convert the file extension to the MIME type
  if (filename.endsWith(".html"))
    return "text/html";
  else if (filename.endsWith(".css"))
    return "text/css";
  else if (filename.endsWith(".js"))
    return "application/javascript";
  else if (filename.endsWith(".ico"))
    return "image/x-icon";
  else if (filename.endsWith(".gz"))
    return "application/x-gzip";
  return "text/plain";
}

// TODO: Clean up
bool handle_file_read(String path)
{ // send the right file to the client (if it exists)
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/"))
    path += "index.html";                      // If a folder is requested, send the index file
  String contentType = get_content_type(path); // Get the MIME type
  String pathWithGz = path + ".gz";
  if (LittleFS.exists(pathWithGz) || LittleFS.exists(path))
  {                                                     // If the file exists, either as a compressed archive, or normal
    if (LittleFS.exists(pathWithGz))                    // If there's a compressed version available
      path += ".gz";                                    // Use the compressed version
    File file = LittleFS.open(path, "r");               // Open the file
    size_t sent = server.streamFile(file, contentType); // Send it to the client
    file.close();                                       // Close the file again
    Serial.println(String("\tSent file: ") + path);
    return true;
  }
  Serial.println(String("\tFile Not Found: ") + path);
  return false; // If the file doesn't exist, return false
}

void handle_not_found()
{
  if (!handle_file_read(server.uri()))
  {
    server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
  }
}

void list_dir(const char *dirname)
{
  Serial.printf("Listing directory: %s\n", dirname);

  Dir root = LittleFS.openDir(dirname);

  while (root.next())
  {
    File file = root.openFile("r");
    Serial.print("  FILE: ");
    Serial.print(root.fileName());
    Serial.print("  SIZE: ");
    Serial.print(file.size());
    time_t cr = file.getCreationTime();
    time_t lw = file.getLastWrite();
    file.close();
    struct tm *tmstruct = localtime(&cr);
    Serial.printf("    CREATION: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
    tmstruct = localtime(&lw);
    Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
  }
}
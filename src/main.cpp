#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <sqlite3.h>

#define WIFI_IMEROUT_MS 20000

const char* ssid = "#ToCoToCo";
const char* password = "1900636936";

AsyncWebServer server(80);

void handleLogin(AsyncWebServerRequest *request){
	File fileHTML = SPIFFS.open("/login.html", "r");
	if (!fileHTML) {
		Serial.println("Failed to open file");
		request->send(404, "text/plain", "File not found");
		return;
	}
	String htmlContent = fileHTML.readString();
	request->send(200, "text/html", htmlContent);
	fileHTML.close();
}

void handleSignup(AsyncWebServerRequest *request){
	File fileHTML = SPIFFS.open("/signup.html", "r");
	if(!fileHTML){
		Serial.println("Failed to open file");
		request->send(404, "text/plain", "File not found");
		return;
	}
	String htmlContent = fileHTML.readString();
	request->send(200, "text/html", htmlContent);
	fileHTML.close();
}

void connectToWifi(){
	Serial.print("Connecting to WiFi");
	WiFi.begin(ssid, password);
	unsigned long startAttemptTime = millis();
	while(WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < WIFI_IMEROUT_MS){
		Serial.print(".");
		delay(500);
	}
	if(WiFi.status() != WL_CONNECTED){
		Serial.println("Failed!");
	}else{
		
		Serial.println("Connected SUCCES ^.^");
		Serial.print("IP: ");
		Serial.println(WiFi.localIP());
	}
}

void connectDatabase(const char *filename ,sqlite3 *db){
	int rc = sqlite3_open(filename, &db);
	if(rc == SQLITE_OK){
		Serial.println("Database connection SUCCESS\n");
		sqlite3_close(db);
	}else{
		Serial.println("Failed to open database\n");
	}
}

void setup() {
  	Serial.begin(115200);
  	if (!SPIFFS.begin(true)) {
		Serial.println("An error occurred while mounting SPIFFS");
		return;
	}

	/*begin code connect wifi*/
  	connectToWifi();
  	/*end code connect wifi*/

  	/*begin code server*/
  	server.on("/", HTTP_GET, handleLogin);
  	server.on("/signup", HTTP_GET, handleSignup);
  	/*end code server*/

  	/*begin code connect database*/
  	sqlite3 *db;
  	connectDatabase("/database/database.db", db);
  	/*end code connect database*/

  	/*begin code static image*/
  	server.on("/img/background.jpg", HTTP_GET, [](AsyncWebServerRequest *request){
    	request->send(SPIFFS, "/img/background.jpg", "image/jpeg");
	});
	server.on("/img/docker.svg", HTTP_GET, [](AsyncWebServerRequest *request){
	    request->send(SPIFFS, "/img/docker.svg", "image/svg+xml");
	});
	server.on("/img/facebook.svg", HTTP_GET, [](AsyncWebServerRequest *request){
	    request->send(SPIFFS, "/img/facebook.svg", "image/svg+xml");
	});
	server.on("/img/github.svg", HTTP_GET, [](AsyncWebServerRequest *request){
	    request->send(SPIFFS, "/img/github.svg", "image/svg+xml");
	});
	server.on("/img/icon.svg", HTTP_GET, [](AsyncWebServerRequest *request){
	    request->send(SPIFFS, "/img/icon.svg", "image/svg+xml");
	});
	server.on("/img/lock.svg", HTTP_GET, [](AsyncWebServerRequest *request){
	    request->send(SPIFFS, "/img/lock.svg", "image/svg+xml");
	});
	server.on("/img/user.svg", HTTP_GET, [](AsyncWebServerRequest *request){
	    request->send(SPIFFS, "/img/user.svg", "image/svg+xml");
	});
	server.on("/img/youtube.svg", HTTP_GET, [](AsyncWebServerRequest *request){
	    request->send(SPIFFS, "/img/youtube.svg", "image/svg+xml");
	});
  	/*end code static image*/

  	/*begin code static css*/
  	server.on("/css/styles.css", HTTP_GET, [](AsyncWebServerRequest *request){
	    request->send(SPIFFS, "/css/styles.css", "text/css");
	});
  	/*end code static css*/

  	/*begin code static js*/
  	server.on("/js/main.js", HTTP_GET, [](AsyncWebServerRequest *request){
  		request->send(SPIFFS, "/js/main.js", "application/javascript");
  	});
  	/*end code static js*/
  	server.begin();
  	Serial.println("HTTP server started");
}

void loop() {

}

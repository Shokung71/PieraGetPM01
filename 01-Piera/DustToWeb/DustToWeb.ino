#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

#define RXPIN 26
#define TXPIN 33

const char* ssid = "IICE-WiFI";
const char* password = "admin@123";

AsyncWebServer server(80);
AsyncEventSource events("/sse");
String serial2Out;

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial2.begin(115200, SERIAL_8N1, RXPIN, TXPIN);

  Serial.println();
  Serial.println("**** 7100 ESP32 Example ****");
  Serial.println("**** Piera Systems ****");
  Serial.println("Warming up 7100...");

  // ไม่ตั้งค่า Static IP เพื่อหลีกเลี่ยงปัญหา IP ซ้ำหรือ subnet ไม่ตรง
  // IPAddress local_IP(192, 168, 0, 2);
  // IPAddress gateway(192, 168, 0, 1);
  // IPAddress subnet(255, 255, 255, 0);
  // if (!WiFi.config(local_IP, gateway, subnet)) {
  //   Serial.println("IP configuration failed");
  // }

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("ESP32 IP address: ");
  Serial.println(WiFi.localIP());

  // ตั้งค่าคำสั่งสำหรับ Serial2
  delay(2000);
  Serial2.write("$Wclnp=43200\r\n");
  Serial2.write("$Wazure=0\r\n");
  delay(100);

  // HTML หน้าเว็บ
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html",
      "<html lang='en'>"
      "<head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'>"
      "<title>ESP32 Sensor Data</title>"
      "<link href='https://fonts.googleapis.com/css2?family=Poppins:wght@300;400;600&display=swap' rel='stylesheet'>"
      "<link rel='stylesheet' href='https://use.fontawesome.com/releases/v5.7.2/css/all.css'>"
      "<style>"
      "body { font-family: 'Poppins', sans-serif; background-color: #f5ffff; color: #333; margin: 0; padding: 0; display: flex; flex-direction: column; align-items: center; justify-content: center; height: 100vh; }"
      "h1 { color: #3b9194; font-size: 3rem; margin-bottom: 20px; }"
      ".container { background-color: #ffffff; padding: 30px; border-radius: 15px; box-shadow: 0 4px 10px rgba(0, 0, 0, 0.1); width: 80%; max-width: 600px; text-align: center; }"
      ".data-container { display: flex; flex-wrap: wrap; justify-content: center; gap: 20px; }"
      ".data-item { background-color: #d5f6f7; color: black; padding: 15px 25px; border-radius: 10px; width: 180px; text-align: center; font-size: 1.2rem; }"
      ".data-item i { font-size: 3rem; margin-bottom: 10px; }"
      ".data-item .value { font-size: 2rem; font-weight: 400; margin-top: 10px; }"
      ".footer { margin-top: 30px; font-size: 0.9rem; color: #777; }"
      "</style></head><body>"
      "<h1>Piera IPS7100 with ESP32 Monitoring Ultrafine Particle Data</h1>"
      "<div class='container'><div class='data-container'>"
      "<div class='data-item' id='itemPm01'><i class='fas fa-wind'></i><div class='label'>PM<sub>0.1</sub></div><div id='pm01m3' class='value'>--</div></div>"
      "<div class='data-item' id='itemPm01'><i class='fas fa-wind'></i><div class='label'>PM<sub>0.1</sub></div><div id='pm01cm3' class='value'>--</div></div>"
      // "<div class='data-item' id='itemPm25'><i class='fas fa-cloud-showers-heavy'></i><div class='label'>PM2.5</div><div id='pm25' class='value'>--</div></div>"
      // "<div class='data-item' id='itemPm100'><i class='fas fa-wind'></i><div class='label'>PM10</div><div id='pm100' class='value'>--</div></div>"

      "<div class='container'><div class='data-container'>"
      "<div class='data-item' id='itemPc01'><i class='fas fa-wind'></i><div class='label'>PC<sub>0.1</sub></div><div id='pc01' class='value'>--</div></div>"

      "</div></div><div class='footer'></div>"
      "<script>"
      "function updateColor(pmValue, itemId) {"
      "  const item = document.getElementById(itemId);"
      "  let iconColor;"
      "  if ((pm01Valuem3 *1000) <= 50) iconColor = 'green';"
      "  else if ((pm01Valuem3 *1000) <= 100) iconColor = 'yellow';"
      "  else if ((pm01Valuem3 *1000) <= 150) iconColor = 'orange';"
      "  else if ((pm01Valuem3 *1000) <= 200) iconColor = 'red';"
      "  else if ((pm01Valuem3 *1000) <= 300) iconColor = 'purple';"
      "  else iconColor = 'maroon';"
      "  item.querySelector('i').style.color = iconColor;"
      "} "
      "if (!!window.EventSource) {"
      "  var source = new EventSource('/sse');"
      "  source.onmessage = function(event) {"
      "    var data = event.data.split(',');"
      "    const pm01Valuem3 = parseFloat(data[15].trim()).toFixed(2);"

      "    const pc01Value = parseFloat(data[1].trim()).toFixed(2);"

      "    const pm01displaym3 = `${pm01Valuem3} µg/m³`;"
      "    const pm01Valuecm3 = (parseFloat(data[15].trim()) * 1000 ).toFixed(2) ;"
      "    const pm01displaycm3 = `${pm01Valuecm3} µg/cm³`;"

      "    const pm25Value = parseFloat(data[23].trim()).toFixed(2);"
      "    const pm100Value = parseFloat(data[27].trim()).toFixed(2);"

      "    document.getElementById('pc01').innerHTML = pc01Value;" 

      "    document.getElementById('pm01m3').innerHTML = pm01displaym3;" 
      "    document.getElementById('pm01cm3').innerHTML = pm01displaycm3;"
      "    document.getElementById('pm25').innerHTML = pm25Value;"
      "    document.getElementById('pm100').innerHTML = pm100Value;"
      "    updateColor(pm01Valuem3, 'itemPm01m3');"
      "    updateColor(pm01Valuecm3, 'itemPm01cm3');"
      "    updateColor(pm25Value, 'itemPm25');"
      "    updateColor(pm100Value, 'itemPm100');"
      "  };"
      "} else { alert('Your browser does not support EventSource.'); }"
      "</script></body></html>");
  });

  server.addHandler(&events);

  Serial.println("Starting web server...");
  server.begin();
  Serial.println("Web server started!");
}

void loop() {
  if (Serial2.available()) {
    serial2Out = Serial2.readStringUntil('\n');
    Serial.println(serial2Out);
    events.send(serial2Out.c_str(), "message", millis());
  }
}

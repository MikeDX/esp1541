#include "web_disk.h"
#include "ram_disk.h"
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

#define WIFI_AP_SSID     "ESP1541"
#define WIFI_AP_PASSWORD "15411541"

static AsyncWebServer server(80);

static const char* upload_html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP1541 - Disk Loader</title>
  <style>
    * { box-sizing: border-box; }
    body { font-family: system-ui; max-width: 480px; margin: 2em auto; padding: 1em; }
    h1 { color: #333; }
    input[type=file] { margin: 1em 0; }
    button { background: #0066cc; color: white; border: none; padding: 0.6em 1.2em; font-size: 1em; cursor: pointer; border-radius: 4px; }
    button:hover { background: #0052a3; }
    .slot { background: #f5f5f5; padding: 0.8em; margin: 0.5em 0; border-radius: 4px; }
    .slot.loaded { background: #e8f5e9; }
    .status { color: #666; font-size: 0.9em; margin-top: 1em; }
  </style>
</head>
<body>
  <h1>ESP1541 Disk Loader</h1>
  <p>Upload D64 or G64 disk images. They load into RAM.</p>
  <form id="uploadForm">
    <input type="file" id="fileInput" accept=".d64,.g64,.D64,.G64">
    <button type="submit">Load into drive</button>
  </form>
  <div id="slots"></div>
  <div class="status" id="status"></div>
  <script>
    async function loadSlots() {
      try {
        const r = await fetch('/api/slots');
        const slots = await r.json();
        document.getElementById('slots').innerHTML = slots.map((s, i) => 
          '<div class="slot ' + (s.loaded ? 'loaded' : '') + '">' +
          'Slot ' + (i+1) + ': ' + (s.loaded ? s.filename + ' (' + s.size + ' bytes)' : 'empty') + '</div>'
        ).join('');
      } catch(e) { document.getElementById('status').textContent = 'Error: ' + e; }
    }
    document.getElementById('uploadForm').onsubmit = async (e) => {
      e.preventDefault();
      const file = document.getElementById('fileInput').files[0];
      if (!file) { document.getElementById('status').textContent = 'Select a file first'; return; }
      document.getElementById('status').textContent = 'Uploading ' + file.name + '...';
      try {
        const r = await fetch('/upload?name=' + encodeURIComponent(file.name), { method: 'POST', body: file });
        const j = await r.json();
        document.getElementById('status').textContent = j.ok ? 'Loaded: ' + file.name : 'Error: ' + (j.msg || 'failed');
      } catch(err) { document.getElementById('status').textContent = 'Error: ' + err; }
      loadSlots();
    };
    loadSlots();
    setInterval(loadSlots, 3000);
  </script>
</body>
</html>
)rawliteral";

void web_disk_setup(void) {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PASSWORD);
  Serial.printf("AP: %s\n", WiFi.softAPIP().toString().c_str());
  Serial.println("Open http://192.168.4.1");

  server.on("/", HTTP_GET, [](AsyncWebServerRequest* req) {
    req->send(200, "text/html", upload_html);
  });

  server.on("/api/slots", HTTP_GET, [](AsyncWebServerRequest* req) {
    String json = "[";
    for (int i = 0; i < RAM_DISK_MAX_SLOTS; i++) {
      ram_disk_slot_t* s = ram_disk_get_slot(i);
      if (i) json += ",";
      json += "{\"loaded\":" + String(s ? "true" : "false");
      if (s) json += ",\"filename\":\"" + String(s->filename) + "\",\"size\":" + String(s->size) + ",\"type\":" + String((int)s->type);
      json += "}";
    }
    json += "]";
    req->send(200, "application/json", json);
  });

  server.on("/upload", HTTP_POST, [](AsyncWebServerRequest* req) {},
    nullptr,
    [](AsyncWebServerRequest* req, uint8_t* data, size_t len, size_t index, size_t total) {
      static uint8_t* upload_buf = nullptr;
      static size_t upload_size = 0;
      if (index == 0) {
        if (upload_buf) free(upload_buf);
        upload_buf = (uint8_t*)malloc(total > RAM_DISK_MAX_SIZE ? RAM_DISK_MAX_SIZE : total);
        upload_size = 0;
      }
      if (upload_buf && len > 0) {
        size_t copy = (upload_size + len <= RAM_DISK_MAX_SIZE) ? len : RAM_DISK_MAX_SIZE - upload_size;
        memcpy(upload_buf + upload_size, data, copy);
        upload_size += copy;
      }
      if (index + len >= total && upload_buf) {
        int slot = ram_disk_find_free_slot();
        const char* name = req->hasParam("name") ? req->getParam("name")->value().c_str() : "upload.d64";
        bool ok = (slot >= 0) && ram_disk_load(slot, upload_buf, upload_size, name);
        free(upload_buf);
        upload_buf = nullptr;
        req->send(200, "application/json", ok ? "{\"ok\":true}" : "{\"ok\":false,\"msg\":\"no slot\"}");
      }
    }
  );

  server.begin();
}

void web_disk_loop(void) {
  delay(10);
}

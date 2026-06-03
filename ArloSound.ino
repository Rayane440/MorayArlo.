#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "AudioTools.h"
#include "BluetoothA2DPSink.h"

// =====================================================
// OLED
// =====================================================
#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(128, 64, &Wire, -1);

// =====================================================
// AUDIO (I2S + MAX98357A)
// =====================================================
I2SStream i2s;
BluetoothA2DPSink a2dp_sink(i2s);

// =====================================================
// DATA
// =====================================================
String title = "No Music Playing";
String artist = "Arlo Sound";

bool connected = false;

// =====================================================
// SCROLL
// =====================================================
int titleScroll = 0;
int artistScroll = 0;

unsigned long lastFrame = 0;

// =====================================================
// MANUAL CLOCK
// =====================================================
int hours = 20;
int minutes = 8;
unsigned long lastMinute = 0;

// =====================================================
// TIME
// =====================================================
String getTime() {
  char buf[6];
  sprintf(buf, "%02d:%02d", hours, minutes);
  return String(buf);
}

// =====================================================
// BLUETOOTH CALLBACKS
// =====================================================
void bt_state(esp_a2d_connection_state_t state, void*) {
  connected = (state == ESP_A2D_CONNECTION_STATE_CONNECTED);

  if (!connected) {
    title = "No Music Playing";
    artist = "Arlo Sound";
  }
}

void metadata(uint8_t id, const uint8_t *text) {
  String v = String((char*)text);

  if (id == ESP_AVRC_MD_ATTR_TITLE) {
    title = v;
    titleScroll = 0;
  }

  if (id == ESP_AVRC_MD_ATTR_ARTIST) {
    artist = v;
    artistScroll = 0;
  }
}

// =====================================================
// HEADER (APPLE STYLE)
// =====================================================
void drawHeader() {

  display.fillRect(0, 0, 128, 12, BLACK);

  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(2, 2);
  display.print("ARLO MUSIC");

  display.setCursor(92, 2);
  display.print(getTime());

  // FIXED LINE (corrigé)
  display.drawFastHLine(0, 13, 128, WHITE);
}

// =====================================================
// SCROLL SAFE
// =====================================================
void scrollText(String txt, int y, int size, int &scrollVar) {

  display.setTextSize(size);
  display.setTextColor(WHITE);

  int charW = (size == 2) ? 12 : 6;
  int textW = txt.length() * charW;

  int x = 128 - scrollVar;

  int h = (size == 2) ? 18 : 10;

  display.fillRect(0, y, 128, h, BLACK);

  display.setCursor(x, y);
  display.print(txt);

  scrollVar++;

  if (scrollVar > textW + 128) {
    scrollVar = 0;
  }
}

// =====================================================
// PLAYER UI
// =====================================================
void drawPlayer() {

  display.clearDisplay();

  drawHeader();

  scrollText(title, 18, 2, titleScroll);

  scrollText(artist, 48, 1, artistScroll);

  display.display();
}

// =====================================================
// WAIT UI
// =====================================================
void drawWaiting() {

  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);

  display.setCursor(18, 10);
  display.print("ARLO");

  display.setCursor(10, 32);
  display.print("SOUND");

  display.setTextSize(1);
  display.setCursor(12, 56);
  display.print("Bluetooth Ready");

  display.display();
}

// =====================================================
// SETUP
// =====================================================
void setup() {

  Serial.begin(115200);

  Wire.begin(OLED_SDA, OLED_SCL);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    while (1);
  }

  display.clearDisplay();
  display.display();

  // AUDIO CONFIG
  auto cfg = i2s.defaultConfig();

  cfg.pin_bck = 26;
  cfg.pin_ws = 25;
  cfg.pin_data = 27;

  cfg.sample_rate = 44100;
  cfg.bits_per_sample = 16;
  cfg.channels = 2;
  cfg.is_master = true;

  i2s.begin(cfg);

  // BLUETOOTH
  a2dp_sink.set_avrc_metadata_callback(metadata);
  a2dp_sink.set_on_connection_state_changed(bt_state);

  a2dp_sink.start("Arlo Sound");

  drawWaiting();
}

// =====================================================
// LOOP
// =====================================================
void loop() {

  // CLOCK UPDATE
  if (millis() - lastMinute > 60000) {

    lastMinute = millis();

    minutes++;

    if (minutes >= 60) {
      minutes = 0;
      hours++;
    }

    if (hours >= 24) {
      hours = 0;
    }
  }

  // SCREEN FPS
  if (millis() - lastFrame > 35) {

    lastFrame = millis();

    if (connected) drawPlayer();
    else drawWaiting();
  }
}

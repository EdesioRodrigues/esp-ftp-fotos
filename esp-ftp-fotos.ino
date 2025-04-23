#include <WiFi.h>
#include <ESP32_FTPClient.h>
#include "esp_camera.h"
#include <Preferences.h>  // Biblioteca para armazenamento persistente
#include <time.h>

// Configurações do Wi-Fi
const char* ssid = "******";
const char* password = "*****";

// Configurações do FTP
char ftp_server[] = "*****";   // Endereço do servidor FTP
char ftp_user[] = "*****";         // Usuário FTP
char ftp_password[] = "****"; // Senha FTP
char ftp_directory[] = "/******/"; // Diretório FTP

// Cliente FTP
ESP32_FTPClient ftp(ftp_server, ftp_user, ftp_password);

// Configuração da câmera
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// Configurações de NTP
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -10800; // Fuso horário do Brasil (-3 horas)
const int daylightOffset_sec = 0;  // Sem horário de verão

// Horários configurados para capturas (formato HH:MM)
String horarios[] = {"12:02", "12:05", "12:10", "12:30"};
int num_horarios = sizeof(horarios) / sizeof(horarios[0]);

// Variáveis de controle
Preferences preferences;  // Objeto para armazenamento persistente
int foto_count = 0;       // Contador de fotos
unsigned long lastCaptureTime = 0;  // Armazena o último tempo de captura
unsigned long lastStatusTime = 0;   // Armazena o último tempo de envio do status

// Função para inicializar a câmera
void startCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_UXGA;
  config.jpeg_quality = 10;
  config.fb_count = 1;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Erro ao inicializar a câmera: 0x%x\n", err);
    while (1);
  }
  Serial.println("Câmera inicializada com sucesso.");
}

// Função para capturar e enviar a foto via FTP
void captureAndSendPhoto() {
  foto_count++;
  camera_fb_t* fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Erro ao capturar a foto.");
    return;
  }

  ftp.OpenConnection();
  ftp.ChangeWorkDir(ftp_directory);

  String filename = "foto_" + String(foto_count) + ".jpg";
  ftp.InitFile("Type I");
  ftp.NewFile(filename.c_str());
  ftp.WriteData(fb->buf, fb->len);
  ftp.CloseFile();
  ftp.CloseConnection();

  preferences.putInt("foto_count", foto_count);  // Salva o contador na memória persistente
  Serial.println("Foto enviada com sucesso para o FTP: " + filename);

  esp_camera_fb_return(fb);
}

// Função para verificar se está na hora de capturar a foto
void checkAndCapture() {
  time_t now = time(nullptr);
  struct tm* timeinfo = localtime(&now);

  char current_time[6];
  strftime(current_time, sizeof(current_time), "%H:%M", timeinfo);

  for (int i = 0; i < num_horarios; i++) {
    if (String(current_time) == horarios[i] && millis() - lastCaptureTime > 60000) {
      Serial.println("Capturando foto às: " + String(current_time));
      lastCaptureTime = millis();
      captureAndSendPhoto();
    }
  }
}

// Adiciona a função de envio do arquivo de status
void sendStatusFile() {
  ftp.OpenConnection();
  ftp.ChangeWorkDir(ftp_directory);

  String filename = "status.txt";
  String status_content = "ESP32 está online";

  ftp.InitFile("Type A");
  ftp.NewFile(filename.c_str());
  ftp.WriteData((unsigned char*)status_content.c_str(), status_content.length());
  ftp.CloseFile();
  ftp.CloseConnection();

  Serial.println("Arquivo de status enviado: " + filename);
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao Wi-Fi...");
  }
  Serial.println("Wi-Fi conectado!");

  // Envio imediato do status.txt após a conexão ao Wi-Fi
  sendStatusFile();
  lastStatusTime = millis();  // Atualiza o tempo do último envio

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("Sincronizando horário com NTP...");
  time_t now = time(nullptr);
  while (now < 8 * 3600) {
    delay(1000);
    now = time(nullptr);
  }
  Serial.println("Horário sincronizado com sucesso.");

  // Inicializa a câmera
  startCamera();

  // Inicializa o armazenamento persistente e recupera o contador
  preferences.begin("esp32-cam", false);
  foto_count = preferences.getInt("foto_count", 0);
  Serial.println("Contador inicializado em: " + String(foto_count));
}

const unsigned long captureInterval = 1000;
const unsigned long statusInterval = 60000;

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - lastCaptureTime >= captureInterval) {
    checkAndCapture();
    lastCaptureTime = currentTime;
  }

  if (currentTime - lastStatusTime >= statusInterval) {
    sendStatusFile();
    lastStatusTime = currentTime;
  }
}

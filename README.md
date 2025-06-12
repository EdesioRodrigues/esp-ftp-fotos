## 📸 ESP32-CAM - Captura de Fotos Programadas

Este projeto permite que um módulo **ESP32-CAM** capture fotos automaticamente em horários programados. Ideal para aplicações como monitoramento ambiental, segurança básica ou timelapses. Todo o código está escrito em **C++ para Arduino**, utilizando a IDE do Arduino ou o **PlatformIO**.

---

### 🔧 Funcionalidades

* Captura de imagens automaticamente em horários definidos.
* Armazenamento local no cartão SD.
* Ajuste fácil de horários via código.
* Código limpo e comentado, ideal para aprendizado e personalização.

---

### 📦 Requisitos

* ESP32-CAM (com OV2640).
* Módulo de cartão microSD (já incluso em alguns modelos ESP32-CAM).
* Fonte 5V com no mínimo 1A.
* Arduino IDE ou PlatformIO.
* Biblioteca `esp_camera` já incluída na ESP32 board.

### 🧠 Como Funciona

* O relógio do sistema é obtido via RTC interno ou NTP (dependendo da versão).
* Um loop verifica a hora atual.
* Se a hora corresponder à programação, a foto é tirada e salva no SD.
* A imagem é nomeada com base na data e hora (`foto_YYYYMMDD_HHMMSS.jpg`).

### ❗️Atenção

* Se o ESP32 reiniciar inesperadamente, verifique se a fonte tem corrente suficiente.
* Para projetos mais avançados, considere usar RTC externo (ex: DS3231) para maior precisão.

---

### 📄 Licença

Este projeto é de **uso livre** (MIT). Fique à vontade para usar, modificar e contribuir.

---

### 🤝 Contribuindo

Pull requests são bem-vindos! Se você encontrar algum bug ou tiver sugestões.

## 📬 Contato 
- LinkedIn: [Edésio Rodrigues](https://www.linkedin.com/in/devedesio-rodrigues/)

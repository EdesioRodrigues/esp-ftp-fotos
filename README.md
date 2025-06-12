## 📸 ESP32-CAM - Captura de Fotos Programadas

Este projeto permite que um módulo **ESP32-CAM** capture fotos automaticamente em horários programados. Ideal para aplicações como monitoramento ambiental, segurança básica ou timelapses. Todo o código está escrito em **C++ para Arduino**, utilizando a IDE do Arduino ou o **PlatformIO**.

---

### 🔧 Funcionalidades

* Captura de imagens automaticamente em horários definidos.
* Envio via FTP, podendo ser alterado para endpoints ou emails.
* Ajuste fácil de horários via código.
* Código limpo e comentado, ideal para aprendizado e personalização.

---

### 📦 Requisitos

* ESP32-CAM (com OV2640).
* Módulo de cartão microSD (já incluso em alguns modelos ESP32-CAM).
* Fonte 5V com no mínimo 1A.
* Arduino IDE ou PlatformIO.

### 🧠 Como Funciona

* O relógio do sistema é obtido via NTP.
* Um loop verifica a hora atual.
* Se a hora corresponder à programação, a foto é tirada e enviada.
* A imagem é nomeada com base na data e hora (`foto_YYYYMMDD_HHMMSS.jpg`).

---

### 📄 Licença

Este projeto é de **uso livre** (MIT). Fique à vontade para usar, modificar e contribuir.

---

### 🤝 Contribuindo

Pull requests são bem-vindos! Se você encontrar algum bug ou tiver sugestões.

## 📬 Contato 
- LinkedIn: [Edésio Rodrigues](https://www.linkedin.com/in/devedesio-rodrigues/)

# Default Password Owner para cardputer

Este projeto utiliza um ESP32 (Cardputer) para escanear redes Wi-Fi próximas e verificar se há falhas de segurança em certos modelos de roteadores (Claro e NET).

<img src="img/card3.jpg" alt="Credentials" width="800"/>

<img src="img/card1.jpg" alt="Intro" width="400"/>
<img src="img/card2.jpg" alt="Scan" width="400"/>

## Introdução

Este projeto foi desenvolvido para identificar a senha padrão de certos modelos de roteadores.

## Como funciona

1. Primeiramente é identificado o SSID da rede e o endereço MAC.

2. Logo são concatenadas as Strings de SSID e MAC para o formato da senha padrão.

<img src="img/password.png" alt="Passwords" width="500"/>

3. O ESP32 tenta conectar-se com a rede usando a senha descoberta, e caso obtenha sucesso, guarda o SSID e Senha no cartão microSD.

## Instalação

### Pré-requisitos

- [VSCode](https://code.visualstudio.com/)
- [PlatformIO](https://platformio.org/)
- [Python 3](https://www.python.org/)
- [Esptool](https://docs.espressif.com/projects/esptool/en/latest/esp32/)

Você pode compilar o projeto localmente com o arquivo build.bat ou baixar o binário mais recente das releases e gravar localmente usando esptool.py

```bat
esptool --port COM1 write_flash 0x00000 dpwo.bin
```

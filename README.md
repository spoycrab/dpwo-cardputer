# Default Password Owner para cardputer

![Imagem](url_da_imagem)

Este projeto utiliza um ESP32-S3 (Cardputer) para escanear redes Wi-Fi próximas e verificar falhas de segurança em certos modelos de roteadores (Claro e NET).

## Introdução

Este projeto foi desenvolvido para identificar a senha padrão de certos modelos de roteadores.

Primeiramente é identificado o SSID da rede e o endereço CM MAC, com essas informações é possível concatena-las para descobrir a senha padrão do Wi-Fi do roteador.

## Instalação

### Pré-requisitos

- [PlatformIO](https://platformio.org/)
- [Espressif 32 Platform](https://platformio.org/platforms/espressif32)
- [Python 3](https://www.python.org/)

Para compilar o código voce deverá executar o build.bat

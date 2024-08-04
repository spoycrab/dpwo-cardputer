# Default Password Owner para cardputer

![Imagem](img/card1.jpg)

Este projeto utiliza um ESP32 (Cardputer) para escanear redes Wi-Fi próximas e verificar falhas de segurança em certos modelos de roteadores (Claro e NET).

## Introdução

Este projeto foi desenvolvido para identificar a senha padrão de certos modelos de roteadores.

## Como funciona

1. Primeiramente é identificado o SSID da rede e o endereço MAC.

2. Logo são concatenadas as Strings de SSID e MAC para o formato da senha padrão.

3. O esp tenta se conectar com a rede usando a senha descoberta, e caso obtenha sucesso, guarda o SSID e Senha no cartão microSD.

## Instalação

### Pré-requisitos

- [VSCode](https://code.visualstudio.com/)
- [PlatformIO](https://platformio.org/)
- [Espressif 32 Platform](https://platformio.org/platforms/espressif32)
- [Python 3](https://www.python.org/)

Para compilar o código voce poderá executar o arquivo build.bat

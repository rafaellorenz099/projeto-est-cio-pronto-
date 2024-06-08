Neste repositório se encontra o código para o funcionamento do projeto com os componentes:
ESP32
Sensor de gás MQ-9
Sensor de temperatura e umidade DHT22
Display LCD 16x2 com módulo I2C
Jumpers e cabos de conexão
Protoboard

Conexões
O sensor MQ-9 possui 4 pinos:
VCC: Alimentação (5V)
GND: Terra
AO: Saída analógica
DO: Saída digital
Vamos usar a saída analógica (AO) para leitura. Conecte os pinos do MQ-9 ao ESP32 da seguinte forma:
 *VCC* do MQ-9 ao 5V do ESP32
 *GND* do MQ-9 ao GND do ESP32
*AO* do MQ-9 ao pino 34 do ESP32

O DHT22 possui 4 pinos:
VCC: Alimentação (3.3V)
Data: Pino de dados
NC: Não conectado
GND: Terra
Conecte os pinos do DHT22 ao ESP32 da seguinte forma:
*VCC* do DHT22 ao 5V do ESP32
*GND* do DHT22 ao GND do ESP32
*Data* do DHT22 ao pino 26 do ESP32
Conexão do Display LCD 16x2 com Módulo I2C
O display LCD com módulo I2C geralmente possui 4 pinos:
VCC: Alimentação (5V)
GND: Terra
SDA: Linha de dados
SCL: Linha de clock



Conecte os pinos do display LCD ao ESP32 da seguinte forma:
VCC do módulo I2C ao 5V do ESP32
GND do módulo I2C ao GND do ESP32
SDA do módulo I2C ao pino 21 do ESP32
SCL do módulo I2C ao pino 22 do ESP32

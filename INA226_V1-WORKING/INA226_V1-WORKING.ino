/*
    INA226 Bi-directional Current/Power Monitor. Simple Example.
    Read more: http://www.jarzebski.pl/arduino/czujniki-i-sensory/cyfrowy-czujnik-pradu-mocy-ina226.html
    GIT: https://github.com/jarzebski/Arduino-INA226
    Web: http://www.jarzebski.pl
    (c) 2014 by Korneliusz Jarzebski
*/

#include <Wire.h>
#include <INA226.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266Influxdb.h>

const char *INFLUXDB_HOST = "192.168.0.200";
const uint16_t INFLUXDB_PORT = 8086;
const char *DATABASE = "ina226";
const char *DB_USER = "*******";
const char *DB_PASSWORD = "*******";

ESP8266WiFiMulti WiFiMulti;
Influxdb influxdb(INFLUXDB_HOST, INFLUXDB_PORT);

INA226 ina1;
INA226 ina2;
void checkConfig()
{
  Serial.print("Mode: ");
  switch (ina1.getMode())
  {
    case INA226_MODE_POWER_DOWN:      Serial.println("INA1 Power-Down"); break;
    case INA226_MODE_SHUNT_TRIG:      Serial.println("INA1 Shunt Voltage, Triggered"); break;
    case INA226_MODE_BUS_TRIG:        Serial.println("INA1 Bus Voltage, Triggered"); break;
    case INA226_MODE_SHUNT_BUS_TRIG:  Serial.println("INA1 Shunt and Bus, Triggered"); break;
    case INA226_MODE_ADC_OFF:         Serial.println("INA1 ADC Off"); break;
    case INA226_MODE_SHUNT_CONT:      Serial.println("INA1 Shunt Voltage, Continuous"); break;
    case INA226_MODE_BUS_CONT:        Serial.println("INA1 Bus Voltage, Continuous"); break;
    case INA226_MODE_SHUNT_BUS_CONT:  Serial.println("INA1 Shunt and Bus, Continuous"); break;
    default: Serial.println("unknown");
  }
  Serial.print("Mode: INA2 ");
  switch (ina2.getMode())
  {
    case INA226_MODE_POWER_DOWN:      Serial.println("Power-Down"); break;
    case INA226_MODE_SHUNT_TRIG:      Serial.println("Shunt Voltage, Triggered"); break;
    case INA226_MODE_BUS_TRIG:        Serial.println("Bus Voltage, Triggered"); break;
    case INA226_MODE_SHUNT_BUS_TRIG:  Serial.println("Shunt and Bus, Triggered"); break;
    case INA226_MODE_ADC_OFF:         Serial.println("ADC Off"); break;
    case INA226_MODE_SHUNT_CONT:      Serial.println("Shunt Voltage, Continuous"); break;
    case INA226_MODE_BUS_CONT:        Serial.println("Bus Voltage, Continuous"); break;
    case INA226_MODE_SHUNT_BUS_CONT:  Serial.println("Shunt and Bus, Continuous"); break;
    default: Serial.println("unknown");
  }
  Serial.print("Samples average       INA1: ");
  switch (ina1.getAverages())
  {
    case INA226_AVERAGES_1:           Serial.println("1 sample"); break;
    case INA226_AVERAGES_4:           Serial.println("4 samples"); break;
    case INA226_AVERAGES_16:          Serial.println("16 samples"); break;
    case INA226_AVERAGES_64:          Serial.println("64 samples"); break;
    case INA226_AVERAGES_128:         Serial.println("128 samples"); break;
    case INA226_AVERAGES_256:         Serial.println("256 samples"); break;
    case INA226_AVERAGES_512:         Serial.println("512 samples"); break;
    case INA226_AVERAGES_1024:        Serial.println("1024 samples"); break;
    default: Serial.println("unknown");
  }
  Serial.print("Samples average       INA2: ");  
  switch (ina2.getAverages())
  {
    case INA226_AVERAGES_1:           Serial.println("1 sample"); break;
    case INA226_AVERAGES_4:           Serial.println("4 samples"); break;
    case INA226_AVERAGES_16:          Serial.println("16 samples"); break;
    case INA226_AVERAGES_64:          Serial.println("64 samples"); break;
    case INA226_AVERAGES_128:         Serial.println("128 samples"); break;
    case INA226_AVERAGES_256:         Serial.println("256 samples"); break;
    case INA226_AVERAGES_512:         Serial.println("512 samples"); break;
    case INA226_AVERAGES_1024:        Serial.println("1024 samples"); break;
    default: Serial.println("unknown");
  }
  Serial.print("Bus conversion time   INA1:   ");
  switch (ina1.getBusConversionTime())
  {
    case INA226_BUS_CONV_TIME_140US:  Serial.println("140uS"); break;
    case INA226_BUS_CONV_TIME_204US:  Serial.println("204uS"); break;
    case INA226_BUS_CONV_TIME_332US:  Serial.println("332uS"); break;
    case INA226_BUS_CONV_TIME_588US:  Serial.println("558uS"); break;
    case INA226_BUS_CONV_TIME_1100US: Serial.println("1.100ms"); break;
    case INA226_BUS_CONV_TIME_2116US: Serial.println("2.116ms"); break;
    case INA226_BUS_CONV_TIME_4156US: Serial.println("4.156ms"); break;
    case INA226_BUS_CONV_TIME_8244US: Serial.println("8.244ms"); break;
    default: Serial.println("unknown");
  }
  Serial.print("Bus conversion time   INA2:   ");
  switch (ina2.getBusConversionTime())
  {
    case INA226_BUS_CONV_TIME_140US:  Serial.println("140uS"); break;
    case INA226_BUS_CONV_TIME_204US:  Serial.println("204uS"); break;
    case INA226_BUS_CONV_TIME_332US:  Serial.println("332uS"); break;
    case INA226_BUS_CONV_TIME_588US:  Serial.println("558uS"); break;
    case INA226_BUS_CONV_TIME_1100US: Serial.println("1.100ms"); break;
    case INA226_BUS_CONV_TIME_2116US: Serial.println("2.116ms"); break;
    case INA226_BUS_CONV_TIME_4156US: Serial.println("4.156ms"); break;
    case INA226_BUS_CONV_TIME_8244US: Serial.println("8.244ms"); break;
    default: Serial.println("unknown");
  }
  Serial.print("Shunt conversion time INA1: ");
  switch (ina1.getShuntConversionTime())
  {
    case INA226_SHUNT_CONV_TIME_140US:  Serial.println("140uS"); break;
    case INA226_SHUNT_CONV_TIME_204US:  Serial.println("204uS"); break;
    case INA226_SHUNT_CONV_TIME_332US:  Serial.println("332uS"); break;
    case INA226_SHUNT_CONV_TIME_588US:  Serial.println("558uS"); break;
    case INA226_SHUNT_CONV_TIME_1100US: Serial.println("1.100ms"); break;
    case INA226_SHUNT_CONV_TIME_2116US: Serial.println("2.116ms"); break;
    case INA226_SHUNT_CONV_TIME_4156US: Serial.println("4.156ms"); break;
    case INA226_SHUNT_CONV_TIME_8244US: Serial.println("8.244ms"); break;
    default: Serial.println("unknown");
  }
  Serial.print("Shunt conversion time INA2: ");
  switch (ina2.getShuntConversionTime())
  {
    case INA226_SHUNT_CONV_TIME_140US:  Serial.println("140uS"); break;
    case INA226_SHUNT_CONV_TIME_204US:  Serial.println("204uS"); break;
    case INA226_SHUNT_CONV_TIME_332US:  Serial.println("332uS"); break;
    case INA226_SHUNT_CONV_TIME_588US:  Serial.println("558uS"); break;
    case INA226_SHUNT_CONV_TIME_1100US: Serial.println("1.100ms"); break;
    case INA226_SHUNT_CONV_TIME_2116US: Serial.println("2.116ms"); break;
    case INA226_SHUNT_CONV_TIME_4156US: Serial.println("4.156ms"); break;
    case INA226_SHUNT_CONV_TIME_8244US: Serial.println("8.244ms"); break;
    default: Serial.println("unknown");
  }  
  Serial.print("Max possible current  INA1: ");
  Serial.print(ina1.getMaxPossibleCurrent());
  Serial.println(" A");

  Serial.print("Max current           INA1: ");
  Serial.print(ina1.getMaxCurrent());
  Serial.println(" A");

  Serial.print("Max shunt voltage     INA1: ");
  Serial.print(ina1.getMaxShuntVoltage());
  Serial.println(" V");

  Serial.print("Max power             INA1: ");
  Serial.print(ina1.getMaxPower());
  Serial.println(" W");
 
  Serial.print("Max possible current  INA2: ");
  Serial.print(ina2.getMaxPossibleCurrent());
  Serial.println(" A");

  Serial.print("Max current           INA2  ");
  Serial.print(ina2.getMaxCurrent());
  Serial.println(" A");

  Serial.print("Max shunt voltage     INA2: ");
  Serial.print(ina2.getMaxShuntVoltage());
  Serial.println(" V");

  Serial.print("Max power             INA2: ");
  Serial.print(ina2.getMaxPower());
  Serial.println(" W");  
}

void setup() 
{
  Serial.begin(115200);

    WiFiMulti.addAP("******", "*******");
  while (WiFiMulti.run() != WL_CONNECTED) {
      Serial.print(". ");
          delay(100);
  }
  influxdb.opendb(DATABASE, DB_USER, DB_PASSWORD);  
  Serial.println("Initialize INA226");
  Serial.println("-----------------------------------------------");

  // Default INA226 address is 0x40
  ina1.begin(0x40); 
  ina2.begin(0x41); 
  
  // Configure INA226
  ina1.configure(INA226_AVERAGES_512, INA226_BUS_CONV_TIME_332US, INA226_SHUNT_CONV_TIME_332US, INA226_MODE_SHUNT_BUS_CONT);
  ina2.configure(INA226_AVERAGES_512, INA226_BUS_CONV_TIME_332US, INA226_SHUNT_CONV_TIME_332US, INA226_MODE_SHUNT_BUS_CONT);
  
  // Calibrate INA226. Rshunt = 0.01 ohm, Max excepted current = 4A
  ina1.calibrate(0.01, 20);
  ina2.calibrate(0.01, 20);
  
  // Display configuration
  checkConfig();

  Serial.println("-----------------------------------------------");
}

void loop()
{

  float shuntvoltage_P1 = 0;
  float busvoltage_P1 = 0;
  float current_A_P1 = 0;
  float power_W_P1 = 0;
  shuntvoltage_P1 = (ina1.readShuntVoltage());
  busvoltage_P1 = (ina1.readBusVoltage());
  current_A_P1 = (ina1.readShuntCurrent());
  power_W_P1 = (ina1.readBusPower());

  Serial.print("Bus voltage   PACK01 :"); Serial.print(ina1.readBusVoltage(), 5)  ; Serial.println(" V");
  Serial.print("Bus power     PACK01: "); Serial.print(ina1.readBusPower(), 5)    ; Serial.println(" W");
  Serial.print("Shunt voltage PACK01: "); Serial.print(ina1.readShuntVoltage(), 5); Serial.println(" V");
  Serial.print("Shunt current PACK01: "); Serial.print(ina1.readShuntCurrent(), 5); Serial.println(" A");
  
  FIELD dataObj("packmon"); // Create field object with measurment name=analog_read 
  dataObj.addTag  ("packnumber", "pack01");
  dataObj.addField("busvoltage", (busvoltage_P1));
  dataObj.addField("shuntvoltage", (shuntvoltage_P1*1000));
  dataObj.addField("current_A", (current_A_P1));
  dataObj.addField("power_W", (power_W_P1));
  Serial.println(influxdb.write(dataObj) == DB_SUCCESS ? "WRITING PACK01 DATA" : "************* WRITING FAILED PACK01 ***********");
  dataObj.empty();
  Serial.println("");
  delay(5000);

  float shuntvoltage_P2 = 0;
  float busvoltage_P2 = 0;
  float current_A_P2 = 0;
  float power_W_P2 = 0;
  shuntvoltage_P2 = (ina2.readShuntVoltage());
  busvoltage_P2 = (ina2.readBusVoltage());
  current_A_P2 = (ina2.readShuntCurrent());
  power_W_P2 = (ina2.readBusPower());

  
  Serial.print("Bus voltage   PACK02 :"); Serial.print(ina2.readBusVoltage(), 5)  ; Serial.println(" V");
  Serial.print("Bus power     PACK02: "); Serial.print(ina2.readBusPower(), 5)    ; Serial.println(" W");
  Serial.print("Shunt voltage PACK02: "); Serial.print(ina2.readShuntVoltage(), 5); Serial.println(" V");
  Serial.print("Shunt current PACK02: "); Serial.print(ina2.readShuntCurrent(), 5); Serial.println(" A"); 
  
  dataObj.addTag  ("packnumber", "pack02");
  dataObj.addField("busvoltage", (busvoltage_P2));
  dataObj.addField("shuntvoltage", (shuntvoltage_P2*1000));
  dataObj.addField("current_A", (current_A_P2));
  dataObj.addField("power_W", (power_W_P2));
  Serial.println(influxdb.write(dataObj) == DB_SUCCESS ? "WRITING PACK02 DATA" : "************* WRITING FAILED PACK02 ***********");
  dataObj.empty();
  Serial.println("");
  delay(5000);

   
}

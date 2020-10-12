#include "max6675.h"
#define SensorPin0 0     //pH meter1 Analog output to arduino analog input 0
#define SensorPin1 1     //pH meter2 Analog output to arduino analog input 1
#define SensorPin2 2     //Voltmeter Analog output to arduino analog input 2
#define SensorPin3 3     //Ammeter Analog output to arduino analog input 3

unsigned long time;

//varying resistances
double resistances[] = {0, 1, 2.2, 4.7, 5.6, 7.5, 8.2, 10, 15, 22, 27, 33, 39, 47, 56, 68, 75, 82, 100, 120, 150, 180, 220, 270, 330, 390, 470, 510, 680, 820, 1000, 1500, 2200, 3000, 3900, 4700, 5600, 6800, 7500, 8200, 10000, 15000, 22000, 33000, 39000, 47000, 56000, 68000, 75000, 82000, 100000, 150000, 180000, 220000, 330000, 470000, 560000, 680000, 1000000, 1500000, 2000000, 3300000, 4700000, 5600000, 10000000, 220};
double currentResistance;
int numberOfResistors = 66;
int currentResistanceIndex;
int interruptPin = 2;
volatile bool buttonFlag = false;

//Thermo1
int thermo1SO = 8;
int thermo1CS = 9;
int thermo1CLK = 10;

MAX6675 thermo1(thermo1CLK, thermo1CS, thermo1SO);

//Thermo2
int thermo2SO = 11;
int thermo2CS = 12;
int thermo2CLK = 13;

MAX6675 thermo2(thermo2CLK, thermo2CS, thermo2SO);

void setup() {
  Serial.begin(9600);
  currentResistanceIndex = 0;
  currentResistance = resistances[currentResistanceIndex];
  Serial.println("Time (s), Voltage (V), current(mA), pH Anode, pH Cathode, Temperature Anode (C), Temperature Cathode (C), External Resistance (ohms)");
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), ISRButton, RISING);
  // give the MAX a little time to settle
  delay(500);
}

void loop() {

  //1st pH meter
  float phValue1 = 3.5 * readpHVoltage(SensorPin0);


  //2nd pH meter
  float phValue2 = 3.5 * readpHVoltage(SensorPin1);


  // Voltmeter

  float correctedVoltage = readVoltage(SensorPin2);

  // Ammeter


  float CorCVolts = readVoltage(SensorPin3);
  float current = CorCVolts / 220;                    //Using 220 ohm resistor (I checked with multimeter.)

  //updating resistance value

  //currentResistance = updateResistance(currentResistance, resistances, numberOfResistors);
  if (buttonFlag) {
    updateResistance(currentResistance, resistances, currentResistanceIndex, numberOfResistors);
    buttonFlag = false;
    delay(100);
  }

  // basic readout test
  time = millis();
  Serial.print((float) (time / 1000.0000), 3); //time in sec (s)
  Serial.print(",");
  Serial.print(correctedVoltage, 4);                //battery voltage (V)
  Serial.print(",");
  Serial.print(current * 1000, 8);                  //ammeter Current (mAmp)
  Serial.print(",");
  Serial.print(phValue1, 4);                        //pH 1, anode
  Serial.print(",");
  Serial.print(phValue2, 4);                        //pH 2, cathode
  Serial.print(",");
  Serial.print(thermo1.readCelsius());              //temp 1, anode
  Serial.print(",");
  Serial.print(thermo2.readCelsius());            //temp 2, cathode
  Serial.print(",");
  Serial.println(currentResistance);

  //delay(50);                            //Change for interval inbetween readings
}
















//function for finding VCC
long readVcc() {
  long result;
  // Read 1.1V reference against AVcc
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
  ADMUX = _BV(MUX5) | _BV(MUX0);
#elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
  ADMUX = _BV(MUX3) | _BV(MUX2);
#else
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#endif
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA, ADSC));
  result = ADCL;
  result |= ADCH << 8;
  result = 1126400L / result; // Calculate Vcc (in mV); 1126400 = 1.1*1024*1000
  return result;
}

//reads voltage and returns as a float
float readVoltage(int pinNumber) {
  int buf[10];
  int temp;
  int avgValue = 0;
  int volts = 0;

  for (int i = 0; i < 10; i++) {
    buf[i] = analogRead(pinNumber);
    delay(5);
  }

  //Sorts 10 points to accending order to take middle points average
  for (int i = 0; i < 9; i++) {
    for (int j = i + 1; j < 10; j++) {
      if (buf[i] > buf[j]) {
        temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }

  for (int i = 2; i < 8; i++) {
    avgValue += buf[i];           //middle 6 sum
  }

  volts = avgValue / 6;
  float voltage = (float) (volts / 1024.0000) * 5;

  float supply = readVcc() / 1000.0;
  float correctedVoltage = supply / 5.00 * voltage;
  return correctedVoltage;
}


//reading voltage for pH meters, without using vcc
float readpHVoltage(int pinNumber) {
  int buf[10];
  int temp;
  int avgValue = 0;
  int volts = 0;

  for (int i = 0; i < 10; i++) {
    buf[i] = analogRead(pinNumber);
    delay(5);
  }

  //Sorts 10 points to accending order to take middle points average
  for (int i = 0; i < 9; i++) {
    for (int j = i + 1; j < 10; j++) {
      if (buf[i] > buf[j]) {
        temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }
  for (int i = 2; i < 8; i++) {
    avgValue += buf[i];           //middle 6 sum
  }

  volts = avgValue * 5.0 / 1024 / 6;
  return volts;

}
/*
  double updateResistance(double currentResistance, double resistances[], int numberOfResistors) {
  for(int i = 0; i < numberOfResistors; i++) {
    if(abs(resistances[i] - currentResistance) < 0.1) {

      if(i >= numberOfResistors - 1) {
        return resistances[numberOfResistors - 1];
      }
      else {
        return resistances[i + 1];
      }
    }
  }
  }
*/

void updateResistance(double& currentResistance, double resistances[], int& currentResistanceIndex, int numberOfResistors) {
  if (currentResistanceIndex < numberOfResistors - 1) {
    currentResistanceIndex++;
  }
  currentResistance = resistances[currentResistanceIndex];
}

void ISRButton() {
  buttonFlag = true;
}

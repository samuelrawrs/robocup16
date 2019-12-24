#include <Wire.h>
#include <NewPing.h>
#include <math.h>

// Compass Address
#define CMPADDR 0x60
#define compTolerance 6
int compoffset=0, prevcompoffset=0, bearing=0;
float northing=0;

// Slave i2c port
#define MicroADDR 121

// IR Addresses
#define IRADDR_F 0x02
#define IRADDR_B 0x01
int IRdir=0, IRstr=0, IRFB=0;

// Ultrasound
#define SONAR_NUM 3
//int rangeUS[SONAR_NUM];

// LDR Pin
#define LDR A1
int ldrthres=60;

// Solenoid Pins
#define SLN_1 6
#define SLN_2 7
void fire_SLN() {
  analogWrite(SLN_1,255);
  analogWrite(SLN_2,255);
  delay(100);
  
  analogWrite(SLN_1,0);
  analogWrite(SLN_2,0);
}

// Switches
#define switch_spd A2
#define switch_mode A3

// Movement Pins
//Robot 1
#define PIN_FL 22
#define PIN_FR 26
#define PIN_BL 32
#define PIN_BR 38

#define PWM_FL 2
#define PWM_FR 3
#define PWM_BL 4
#define PWM_BR 5

#define FLBRfast 1.05
int SmotorFL=0, SmotorFR=0, SmotorBL=0, SmotorBR=0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  pinMode(PIN_FL,OUTPUT);pinMode(PIN_FR,OUTPUT);pinMode(PIN_BL,OUTPUT);pinMode(PIN_BR,OUTPUT);
  pinMode(PWM_FL,OUTPUT);pinMode(PWM_FR,OUTPUT);pinMode(PWM_BL,OUTPUT);pinMode(PWM_BR,OUTPUT);
  analogWrite(PWM_FL,0);analogWrite(PWM_FR,0);analogWrite(PWM_BL,0);analogWrite(PWM_BR,0);
  
  /*
  pinMode(SLN_1,OUTPUT);pinMode(SLN_2,OUTPUT);
  fire_SLN();
    
  // Initialise LDR
  ldrthres=analogRead(LDR)-ldrthres;
  */
  
  //delay(100);
  Serial.println("Initialising sensors");
  Serial.println("1.Compass"); initComp();
  Serial.println("2. Infraredlol"); initIR();
  //Serial.println("3. Ultrasonic"); initUS();
  Serial.println("Initialisation Complete!");
}

void setMove(int spd, float dir, int spin=0) {
  /*
    LOW - Anticlockwise (-ve)
    HIGH - Clockwise    (+ve)
  */
  
  if (dir>=360) dir-=360;
  /*
  if (bearing<=180 && bearing>compTolerance) northing = pow((bearing/20),2)+12;
  else if (bearing>180 && bearing<360-compTolerance) northing = -(pow(((bearing-360)/20),2)+12);
  else northing=0;
  */
  if (spd==0) { // not moving
    SmotorFL = northing;
    SmotorFR = northing;
    SmotorBL = northing;
    SmotorBR = northing;
    //Serial.println(northing);
  }
  else { // moving
    if (dir==0) {
      SmotorFL = -spd;
      SmotorFR = spd;
      SmotorBL = -spd;
      SmotorBR = spd;
    }
    else if (dir<=90) {
      SmotorFL = -spd;
      SmotorFR = spd*((35-dir)/55);
      SmotorBL = -spd*((35-dir)/55);
      SmotorBR = spd;
    }
    else if (dir<=180) {
      SmotorFL = -spd*((125-dir)/55);
      SmotorFR = -spd;
      SmotorBL = spd;
      SmotorBR = spd*((125-dir)/55);
    }
    else if (dir<=270) {
      SmotorFL = spd;
      SmotorFR = -spd*((215-dir)/55);
      SmotorBL = spd*((215-dir)/55);
      SmotorBR = -spd;
    }
    else { // (dir<360)
      SmotorFL = spd*((305-dir)/55);
      SmotorFR = spd;
      SmotorBL = -spd;
      SmotorBR = -spd*((305-dir)/55);
    }
  }
  
  // Calculate North Compensation
  northing *= .1*spd/5;
  SmotorFL += northing;
  SmotorFR += northing;
  SmotorBL += northing;
  SmotorBR += northing;
  
  //Compensate FL and BR motors
  SmotorFL *= FLBRfast;
  SmotorBR *= FLBRfast;
  
  //Serial.println(String(SmotorBR)+"\t"+String(SmotorBL)+"\t"+String(SmotorFR)+"\t"+String(SmotorFL));
  
  // GO!
  analogWrite(PWM_FL,abs(SmotorFL));digitalWrite(PIN_FL,SmotorFL>0?HIGH:LOW);
  analogWrite(PWM_FR,abs(SmotorFR));digitalWrite(PIN_FR,SmotorFR>0?HIGH:LOW);
  analogWrite(PWM_BL,abs(SmotorBL));digitalWrite(PIN_BL,SmotorBL>0?HIGH:LOW);
  analogWrite(PWM_BR,abs(SmotorBR));digitalWrite(PIN_BR,SmotorBR>0?HIGH:LOW);
}

void loop() {
  bearing = readComp();
  
  
  readIR();

  readUS();
  
  trackball();
  
  Serial.print(IRFB);Serial.print("\t");Serial.print(IRstr);Serial.print("\t");Serial.print(IRdir);Serial.print("\t");/*Serial.print();Serial.print("\t");*/Serial.println(bearing);

  //Serial.print(getRangeL());Serial.print("\t");Serial.print(getRangeB());Serial.print("\t");Serial.println(getRangeR());
}

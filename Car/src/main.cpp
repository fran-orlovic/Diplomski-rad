#include <Arduino.h>

#define POTENCIOMETAR 32
#define PWM_PIN 16
#define PWM_CHANNEL 0
#define FREQUENCY 50
#define RESOLUTION 12
#define ZMIGAVAC_L 22
#define ZMIGAVAC_D 21
#define SVJETLA 23
#define SKLOPKA 33

char slovo;
bool stanjeSvjetla;
int valueADC = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(ZMIGAVAC_L, OUTPUT);
  pinMode(ZMIGAVAC_D, OUTPUT);
  pinMode(SVJETLA, OUTPUT);
  pinMode(SKLOPKA, INPUT);

  digitalWrite(ZMIGAVAC_L, LOW);
  digitalWrite(ZMIGAVAC_D, LOW);
  digitalWrite(SVJETLA, LOW);
  stanjeSvjetla = false;
  ledcSetup(PWM_CHANNEL, FREQUENCY, RESOLUTION);
  ledcAttachPin(PWM_PIN, PWM_CHANNEL);
  ledcWrite(PWM_CHANNEL, 300);
}

void loop()
{
  valueADC = analogRead(POTENCIOMETAR);
  valueADC = map(valueADC, 0, 4095, 240, 360);
  ledcWrite(PWM_CHANNEL, valueADC);

  if(digitalRead(SKLOPKA) != stanjeSvjetla)
  {
    stanjeSvjetla = digitalRead(SKLOPKA);
    digitalWrite(SVJETLA, stanjeSvjetla);
    Serial.println("Stanje svjetla se promijenilo");
  }

  if (Serial.available() > 0)
  {
    slovo = Serial.read();
    Serial.println(slovo);

    if (slovo == 'l')
    {
      for(int i=0; i<5; i++)
      {
        digitalWrite(ZMIGAVAC_L, HIGH);
        delay(500);
        digitalWrite(ZMIGAVAC_L, LOW);
        delay(500);
      }
    }
    else if (slovo == 'd')
    {
      for(int i=0; i<5; i++)
      {
        digitalWrite(ZMIGAVAC_D, HIGH);
        delay(500);
        digitalWrite(ZMIGAVAC_D, LOW);
        delay(500);
      }
    }
  }
}
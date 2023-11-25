#include <Arduino.h>

// declaracion de varianles globales
#define ADC A0
#define PWM 10

uint32_t ValorADC = 0;
double CE[2] = {0.366786691562949	,-0.363118824647320};
double CS[2] = {1, 1};
double Ent[3] = {0};
double Sal[3] = {0};
double error = 0;
double referencia;
hw_timer_t *timer = NULL;
//******************************************************

void IRAM_ATTR Control()
{
  ValorADC = analogReadMilliVolts(ADC);
  double omega = ValorADC * 0.070756;
  error = referencia - omega;
  Ent[1] = Ent[0];
  Ent[0] = error;
  Sal[1] = Sal[0];
  Sal[0] = Ent[0] * CE[0] + Ent[1] * CE[1] + Sal[1] * CS[1] ;
  float duty = Sal[0];
  if (duty < 0)
  {
    duty = 0;
  }
   if (duty > 1023)
  {
    duty = 1023;
  }
  analogWrite(PWM,duty);
}

void setup()
{
  Serial.begin(115200);

  // Configurar el timer
  timer = timerBegin(0, 80, true);             // Iniciar el timer con prescaler de 80 y conteo ascendente
  timerAttachInterrupt(timer, &Control, true); // Adjuntar la función de interrupción
  timerAlarmWrite(timer, 1000, true);           // Configurar el tiempo de alarma y repetir
  timerAlarmEnable(timer);                     // Habilitar la alarma del timer
  // configuracion del pwm
  analogWriteFrequency(5000);
  analogWriteResolution(10);
}


void loop()
{
  if (Serial.available() > 0) {
    // Leer el número desde el puerto serial
    referencia = Serial.parseInt();
  }  
Serial.print(referencia);                                      //primera variable a graficar
Serial.print(",");                                              //separador para variables
Serial.println((ValorADC * 0.070756));                                         //escritura serial
delay(250);    

}

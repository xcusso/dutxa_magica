/*
  Projecte Dutxa Magica
  La idea del projecte es utilitzar els leds RGB de la dutxa
  per generar un codi de colors en funcio de la temperatura del aigua.
  
  Per fer-ho economic ho basarem en un attiny85 configurat amb dues entrades
  analogiques que llegira la temperatura de dos TMP36 i convertira la lectura 
  mes alta en un codi de colors canviants.
  
  Definirem fins a 6 punts de colors per indicar amb transicions suaus entre ells.
  TMIN = Temperatura minima de funcionament. Per sota d'aquest valor no hi ha cap
  LED ences.
  TBLAU = Temperatura de BLAU 100%. Correspondra al punt mig entre la temperatura
  optima TVERD i la TMIN.
  TCEL = Temperatura entre Blau i Verd.
  TVERD = Temperatura optima per la dutxa 37C. El color sera VERD 100%
  TGROC = Punt mig entre TVERD i TVERMELL. Encen VERMELL 100% i VERD 100%
  TVERMELL = Temperatura de VERMELL 100%. En teoria la maxima suportable.
  TMAX = Aigua cremant. RGB al 100%
  Per sobre de TBLANC farem un llum intermitent per indicar perill.
  
  TMIN -> TBLUE --> Rampa del 0 al 255 de color BLAU
  TBLUE -> TCYAN --> BLUE a 255 i GREEN rampa de 0 a 255
  TCYAN -> TGREEN --> GREEN a 255 i BLUE rampa de 255 a 0
  TGREEN -> TYELLOW --> GREEN a 255 i RED rampa de 0 a 255
  TYELLOW -> TRED --> RED a 255 i GREEN rampa de 255 a 0
  TRED -> TMAX --> RED a 255 i BLUE i GREEN rampa de 0 a 255
  > TMAx BLINK RGB 255/0 1/2 Segon
  
  */
  
  float TEMP;
  float TMIN = 0;
  float TBLUE = 10;
  float TGREEN = 20;
  float TCYAN = (((TGREEN - TBLUE) /2) + TBLUE);
  float TRED = 30;
  float TYELLOW = (((TRED-TGREEN) /2) + TGREEN);
  float TMAX = 50;
  byte RED = 0;
  byte GREEN = 0;
  byte BLUE = 0;
  boolean BLINK = 0;
  const int redPIN = 9;
  const int greenPIN = 10;
  const int bluePIN = 11;
  const int sensor = A0;
  
  void setup()
  {
    Serial.begin(9600); //Per veure valors
    pinMode(redPIN, OUTPUT);
    pinMode(greenPIN, OUTPUT);
    pinMode(bluePIN, OUTPUT);
    pinMode(sensor, INPUT);
  }
  
  void loop() 
  {
    TEMP = ((analogRead(sensor))/20); //Caldria fer la conversio a T Celsius
    
    if (TEMP <= TMIN)
    {  
      RED = 0;
      GREEN = 0;
      BLUE = 0;
    }
    else if (TEMP > TMIN && TEMP <= TBLUE)
    {
      RED = 0;
      GREEN = 0;
      BLUE = RAMPA(TBLUE,TMIN,TEMP);
    }
    else if (TEMP > TBLUE && TEMP <= TCYAN)
    {
      RED = 0;
      GREEN = RAMPA(TCYAN,TBLUE,TEMP);
      BLUE = 255;
    }
    else if (TEMP > TCYAN && TEMP <= TGREEN)
    {
      RED = 0;
      GREEN = 255;
      BLUE = (255 - (RAMPA(TGREEN,TCYAN,TEMP)));
    }
    else if (TEMP > TGREEN && TEMP <= TYELLOW)
    { 
      RED = RAMPA(TYELLOW,TGREEN,TEMP);
      GREEN = 255;
      BLUE = 0;
    }
    else if (TEMP > TYELLOW && TEMP <= TRED)
    {
      RED = 255;
      GREEN = (255 - (RAMPA(TRED,TYELLOW,TEMP)));
      BLUE = 0;
    }
    else if (TEMP > TRED && TEMP <= TMAX)
    {
      RED = 255;
      GREEN = RAMPA(TMAX,TRED,TEMP);
      BLUE = GREEN;
    }
    else if (TEMP > TMAX)
    {
      BLINK = !BLINK;
      if (BLINK == 0)
      {
        RED = 255;
        GREEN = 255;
        BLUE = 255;
        delay (500);
      } else if (BLINK == 1)
        {
        RED = 0;
        GREEN = 0;
        BLUE = 0;
        delay (500);
        }
    }
    
    analogWrite(redPIN,RED);
    analogWrite(greenPIN,GREEN);
    analogWrite(bluePIN,BLUE);
    
    //Per monitoritzar
    Serial.print("Temperatura: ");
    Serial.print(TEMP);
    Serial.print("RED: ");
    Serial.print(RED);
    Serial.print("GREEN: ");
    Serial.print(GREEN);
    Serial.print("BLUE: ");
    Serial.print(BLUE);    

  }
  
 byte RAMPA(float TU, float TD, float TA) {
   byte result;
   result = 255 * ((TA - TD) / (TU - TD));
   return result;
 }
 
  
 
  

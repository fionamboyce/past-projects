#define input0 A0
#define input1 A1
#define furtherprocessing 1

float readincurrent0=0;
float readinprevious0=0;
float readincurrent1=0;
float readinprevious1=0;
float derivative0 = 0;
float derivative1 = 0;

float gap = 10;

bool state=false;
long to;
long t;
int period;
int i;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
//pinMode(LED,OUTPUT);
//while(!Serial.available());
//while(Serial.available()){Serial.read();}
pinMode(LED_BUILTIN, OUTPUT);
digitalWrite(LED_BUILTIN, LOW);
pinMode(input0, INPUT);
pinMode(input1, INPUT);

readincurrent0=analogRead(input0);
readinprevious0=readincurrent0;
readincurrent1=analogRead(input1);
readinprevious1=readincurrent1;

}



void loop() {
  // put your main code here, to run repeatedly:
    delay(gap);
    readincurrent0=analogRead(input0);
    derivative0 = (readincurrent0-readinprevious0)/gap;
    
    readincurrent1=analogRead(input1);
    derivative1 = (readincurrent1-readinprevious1)/gap;

    // further processing section
    if(furtherprocessing)
    {
      if(derivative0>=0)
      {
        derivative0=0;
      }
      if(derivative1>=0)
      {
        derivative1=0;
      }
      derivative0=-derivative0;
      derivative1=-derivative1;
    }
    
    Serial.print(derivative0);
    Serial.print(',');
    Serial.println(derivative1);
    //Serial.print("P");
    
    readinprevious0=readincurrent0;
    readinprevious1=readincurrent1;
    
   
  }

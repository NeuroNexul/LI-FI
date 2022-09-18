#define LED_PIN A1
#define LDR_PIN A2

#define THRESHOLD 100
#define PERIOD 15

// receive
bool previous_state;
bool current_state;

// transmit
int string_length;

void setup () {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
}

void loop () {

  // Transmit
  if (Serial.available()) {
    String string = Serial.readString();
    int string_length = string.length();

    long int t1 = millis();
    for(int i = 0; i < string_length; i ++)
    {
      send_byte(string[i]);
    }
    long int t2 = millis();
    Serial.print("\n[SENT] => " + string);
    Serial.print("[TIME TAKEN BY LAST OPERATION] => "); Serial.print(t2-t1); Serial.print(" ms\n\n");
    
    delay(1000);
  }
  
  // Receive
  current_state = get_ldr();
  if(!current_state && previous_state)
  {
    print_byte(get_byte());
  }
  previous_state = current_state;
}

// Receive
bool get_ldr()
{
  int voltage = analogRead(LDR_PIN);
  return voltage > THRESHOLD ? true : false;
}
char get_byte()
{
  char ret = 0;
  delay(PERIOD*1.5);
  for(int i = 0; i < 8; i++)
  {
   ret = ret | get_ldr() << i; 
   delay(PERIOD);
  }
  return ret;
}
void print_byte(char my_byte)
{
  char buff[2];
  sprintf(buff, "%c", my_byte);
  Serial.print(buff);
}

// Transmit
void send_byte(char my_byte)
{
  digitalWrite(LED_PIN, LOW);
  delay(PERIOD);

  //transmission of bits
  for(int i = 0; i < 8; i++)
  {
    digitalWrite(LED_PIN, (my_byte&(0x01 << i))!=0 );
    delay(PERIOD);
  }

  digitalWrite(LED_PIN, HIGH);
  delay(PERIOD);
}

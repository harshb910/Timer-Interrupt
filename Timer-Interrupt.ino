// C++ code
//
int luminosity = 0;
bool toggle = LOW;
bool inBuiltLED = LOW;
bool isOkay = LOW;
int f=0;
unsigned long time=millis();
unsigned long newTime;
void setup()
{
  	pinMode(8, OUTPUT);
  	pinMode(A5, INPUT);
  	pinMode(LED_BUILTIN, OUTPUT);
  
  	checkLuminosity();
  
  	cli();	//stop interrupts
	//set timer1 interrupt at 0.5Hz that is time period of 2 sec.
  	TCCR1A = 0;	// set entire TCCR1A register to 0
  	TCCR1B = 0;	// same for TCCR1B
  	TCNT1  = 0;	//initialize counter value to 0
  	// set compare match register for 0.5hz increments
  	OCR1A = 31249;	// = (16*10^6) / (0.5*1024) - 1 (must be <65536) as timer1 uses 16 bit
  	// turn on CTC mode
  	TCCR1B |= (1 << WGM12);
  	// Set CS10 and CS12 bits for 1024 prescaler
  	TCCR1B |= (1 << CS12) | (1 << CS10);  
  	// enable timer compare interrupt
  	TIMSK1 |= (1 << OCIE1A);
	sei();	//allow interrupts
  	Serial.begin(9600);

}

ISR(TIMER1_COMPA_vect){
  if(luminosity>722){
    	if(f==0){isOkay=HIGH; f+=2;}
    	else if(f==58 && isOkay==HIGH){
    		newTime = millis();
        		Serial.print("ON Time = ");
        		Serial.print(newTime-time);
        		Serial.print("\n");
        		digitalWrite(8,HIGH);
        		time = newTime;
        		f=0;
        		isOkay=LOW;
    	}else{
    		f+=2;
    	}
    
  }else if(isOkay==HIGH && f<58){
  	f+=2;
  }else{
    	newTime = millis();
    	Serial.print("OFF Time = ");
    	Serial.print(newTime-time);
    	Serial.print("\n");
    	digitalWrite(8,LOW);
    	time = newTime;
    	isOkay=LOW;
  	f=0;
  }
}

void checkLuminosity(){
	luminosity = analogRead(A5);
  	if(luminosity>722){
    	digitalWrite(LED_BUILTIN,HIGH);
  	}else{
   		inBuiltLED = !inBuiltLED;
    		if(inBuiltLED){
    			digitalWrite(LED_BUILTIN,HIGH);
    		}else{
    			digitalWrite(LED_BUILTIN,LOW);
    		}
  	}
}
void loop()
{
  
  checkLuminosity();
  Serial.print("F = ");
  Serial.print(f);
  Serial.print("\t");
  Serial.println(luminosity);
  delay(2000); // Delay a little bit to make cycle of 1minute(i.e. 58+2 second)
}

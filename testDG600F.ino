/*
  Example for DG600F (S).
  The sample configures 2 inputs and 3 outputs.
  In the sample, only one input is used to reset the current balance: PIN 2
  In the sample, PIN 6 and 7 are connected to LEDs.
  PIN6 makes the LED blinks when a coin is detected.
  PIN7 blinks when enough is paid.
  PIN9 is illuminated when an error occurs.
*/

int input[3];
int index = 0;
int sum = 0;
const int ACTION_PRICE = 100; // 100 cents
 
//boolean stringComplete = false;  // whether the string is complete

void setup()
{
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(9, OUTPUT);
  Serial.begin(9600);
  Serial.println("Accepteur de pièces prêt!");
}

void newCoin(int val){
   //Serial.println("Pièce = ");
   digitalWrite(6, HIGH);   
   delay(5);                      
   digitalWrite(6, LOW);                      

   sum += val;
   Serial.println("COIN: " + String(val, 10) + " cents");
   Serial.println("TOTL: "+ String(sum, 10) + " cents");
}

bool checkData(int arr[]) {
  //Serial.println("Checking data...");  

  //Serial.println(String(arr[0], 16));  

  if (arr[0] != 0xaa) {
      Serial.println("ERR: Header failure");  
      return false;
  } else {
    Serial.println("CHK: Header OK");  
  }
    
  //Serial.println(String(arr[0], 2) + "^" + String(arr[1], 2) + " = " + String(arr[0] ^ arr[1], 2) + "  ?= " + String(arr[2], 2));  
  //Serial.println("Comparing " + String(arr[0] ^ arr[1], 2) + " with " + String(arr[2], 2) );  

  if ((arr[0] ^ arr[1]) != arr[2]) {          
    Serial.println("ERR: Checksum failure");  
    return false; 
  } else {
      Serial.println("CHK: Checksum OK");  
  }

  return true;
}

void serialEvent() {
  //Serial.println("Piece tombée");
 
  while (Serial.available()) {
    int inChar = (int)Serial.read();
    input[index++] = inChar;
    
    //Serial.println(String(inChar, 2) + " = " + String(inChar, 16) + " = " + (char)inChar);
  }
   
  if (index >= 3 && input[0] == 0xaa) { 
     //Serial.println(String(input[0], 16) + " " + String(input[1], 16) + " " + String(input[2], 16));  
     if (checkData(input)){
        newCoin(input[1]); 
     } else {
      Serial.println("BAD DATA: " + String(input[0], 16) + " " + String(input[1], 16) + " " + String(input[2], 16));  
      digitalWrite(9, HIGH);   
      delay(100);                      
      digitalWrite(9, LOW);
     }
     
     index = 0;  
  }
}

void runAction() {
  Serial.println("Fiesta!!! Débit de " + String(ACTION_PRICE, 10) + " cents");  
  
  for (int i=0; i< 3; i++){
    digitalWrite(7, HIGH);   
    delay(5);                      
    digitalWrite(7, LOW);
    delay(2);
  }  
  
  Serial.println("New balance: " + String(sum, 10) + " cents"); 
}

void loop()
{ 
  int button2 = digitalRead(2);
  int button3 = digitalRead(3);
  
  if (!button2) { 
    sum = 0;
    Serial.println("New balance: " + String(sum, 10) + " cents"); 
  }

  if (sum >= ACTION_PRICE) {
    sum -= ACTION_PRICE;
    runAction(); 
  }
  
  if (!button3) {
  }

  delay(10);
}                                                                                                                                                                                                                       


//inspiration code for:
//button https://docs.arduino.cc/built-in-examples/digital/Button
//force sensor https://arduinogetstarted.com/tutorials/arduino-force-sensor
//time loop https://forum.arduino.cc/t/getting-a-loop-to-execute-in-a-defined-amount-of-time/919975/3

int buttonPushes, buttonState;
unsigned long int startTime;
int force, newforce;
int TorL[3] = {1, 1, 1}; //default as truth

void setup()
{
  pinMode(2, INPUT); //initialize digital pin 2 as input (button)
  pinMode(3, OUTPUT); //initialize digital pin 3 as output (truth)
  pinMode(5, OUTPUT); //intialize digital pin 5 as output (lie)
  pinMode(7,OUTPUT); //intialize digital pin 7 as output (on light)

  Serial.begin(9600);
}

void loop()
{
  //turn light on
  analogWrite(7,150);

  //constantly check state of the button
  buttonState = digitalRead(2);
  //uncomment to error check
  /*Serial.print("Button pushed? ");
  Serial.println(buttonState);*/

  if (buttonState == 1) //button is pushed
  {
    buttonPushes++;
    //uncomment to error check
    /*Serial.print("Number of pushes: ");
    Serial.println(buttonPushes);*/
    if (buttonPushes < 4) //only measure 3 times
    {
      //lights on to indicate reading
      analogWrite(3,75);
      analogWrite(5,75);

      //get initial force on sensor
      delay(1000); //wait 1 sec to get steady reading
      force = analogRead(A0);
      //uncomment to error check
      /*Serial.print("Initial force: ");
      Serial.println(force);*/

      //read a new force for 4 sec to check for any change
      startTime = millis();
      while (millis()-startTime < 4000)
      {
      newforce = analogRead(A0);
      //uncomment to error check
      /*Serial.print("Reading force: ");
      Serial.println(newforce);*/
        if ((newforce < force-200) || (newforce > force+200)) //if the analog reading of the force changes by + or - 200
        {
          TorL[buttonPushes-1] = 0; //index one less than number of current button pushes
          startTime = 0; //jump out of loop
        }
        else
        {
          TorL[buttonPushes-1] = 1;
        }
          delay(500); //wait 0.5 sec before next reading
      }

      //if no change of force was detected, TorL of that index remains as true
      //uncomment to error check
      /*if (TorL[buttonPushes-1] == 1)
        Serial.println("Truth");
      else if (TorL[buttonPushes-1] == 0)
        Serial.println("Lie");*/

      //lights off to indicate done reading
      analogWrite(3,0);
      analogWrite(5,0);
    }
  }

  //display repective light according to TorL values
  if (buttonPushes > 4) //only after 3 questions are asked
  {
    for (int i = 0; i < 3; i++)
    {
      if (TorL[i] == 1) //flash green for 1 sec if truth
      {
        analogWrite(3,75);
        delay(1000);
        analogWrite(3, 0);

      }
      else if (TorL[i] == 0) //flash red for 1 sec if lie
      {
        analogWrite(5,75);
        delay(1000);
        analogWrite(5, 0);
      }
      delay(1000);
    }
    buttonPushes = 0; //reset pushes in order to go again
  }

} //end of program

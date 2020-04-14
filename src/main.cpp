#include <Arduino.h>
#include <MicroView.h>
int booting = 0;    //bootup screen
int bootscreen = 0; //the Nuke OS logo
int code = 0;       //has the user started entering the code yet?
int lock = 0;       //is the system locked still?
int confirmState;   //is the confirm button pressed or not
int dipState[8];    //dipSwitches state
int dipSwitchDraw = 5;
int dipSwitchBitDraw = 0;

int confirmButton = 0;                            //confirm button pin assignment
int dipSwitches[8] = {1, 2, 3, 5, 6, A0, A1, A2}; //dip switch assignment

uint8_t nuke[] = { //Nuke OS logo in bitmap
    0x00, 0xf8, 0xf8, 0x38, 0xe0, 0x80, 0x00, 0xf8, 0xf8, 0x00, 0x00, 0xe0, 0xe0, 0x00, 0x00, 0x00,
    0xe0, 0xe0, 0x00, 0x00, 0xfc, 0xfc, 0x80, 0xc0, 0x60, 0x20, 0x80, 0xc0, 0x60, 0x60, 0x60, 0xc0,
    0x80, 0x00, 0x00, 0x00, 0xc0, 0xf0, 0x18, 0x08, 0x08, 0x08, 0x08, 0x38, 0xe0, 0x00, 0x00, 0x70,
    0xc8, 0x88, 0x88, 0x08, 0x00, 0x00, 0xe0, 0x10, 0xc8, 0x68, 0x28, 0x28, 0x08, 0x10, 0xe0, 0x00,
    0x00, 0x1f, 0x1f, 0x00, 0x00, 0x03, 0x0e, 0x1f, 0x1f, 0x00, 0x00, 0x0f, 0x1f, 0x18, 0x10, 0x18,
    0x1f, 0x1f, 0x00, 0x00, 0x1f, 0x1f, 0x07, 0x0c, 0x18, 0x90, 0x8f, 0x9f, 0x92, 0x92, 0x92, 0x9b,
    0x99, 0x80, 0x80, 0x80, 0x03, 0x0f, 0x18, 0x10, 0x10, 0x10, 0x18, 0x0c, 0x07, 0x00, 0x00, 0x10,
    0x10, 0x10, 0x11, 0x1f, 0x0e, 0x00, 0x07, 0x08, 0x13, 0x16, 0x14, 0x14, 0x10, 0x08, 0x07, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xc0, 0x20, 0x90, 0xe8, 0xf4, 0xf2, 0xfa, 0xf1, 0xe1, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x80, 0xe1, 0xf1, 0xfa, 0xf2, 0xf4, 0xe8, 0x90, 0x20, 0xc0, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x03,
    0x7c, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7e, 0x78, 0x30, 0x80, 0xc0, 0x80,
    0x30, 0x78, 0x7e, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7c, 0x03, 0xfc, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x60,
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xe0, 0xf8, 0xfc, 0xfd, 0xfc,
    0xf8, 0xe0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x60, 0x1f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x20, 0x40, 0x5c, 0x9f, 0xbf, 0xbf, 0xbf, 0xbf, 0xbf, 0xbf,
    0xbf, 0xbf, 0xbf, 0x9f, 0x5c, 0x40, 0x20, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

uint8_t switches[] = { //DipSwitch image in bitmap
    0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
    0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
    0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
    0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0x01, 0x01, 0x01, 0x01, 0xff, 0xff, 0x01, 0x01, 0x01, 0x01,
    0x01, 0xff, 0xff, 0x01, 0x01, 0x01, 0x01, 0x01, 0xff, 0xff, 0x01, 0x01, 0x01, 0x01, 0x01, 0xff,
    0xff, 0x01, 0x01, 0x01, 0x01, 0x01, 0xff, 0xff, 0x01, 0x01, 0x01, 0x01, 0x01, 0xff, 0xff, 0x01,
    0x01, 0x01, 0x01, 0x01, 0xff, 0xff, 0x01, 0x01, 0x01, 0x01, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
    0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
    0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1f, 0x1f, 0x1e, 0x1e, 0x1e, 0x1e,
    0x1e, 0x1f, 0x1f, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1f, 0x1f, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1f,
    0x1f, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1f, 0x1f, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1f, 0x1f, 0x1e,
    0x1e, 0x1e, 0x1e, 0x1e, 0x1f, 0x1f, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void setup() // put your setup code here, to run once:
{

  pinMode(confirmButton, INPUT_PULLUP); //set  the confirm button as an input

  for (int x = 0; x < 8; x++) //set a dip switch as an input and then advance by one and repeate 8 times
  {
    pinMode(dipSwitches[x], INPUT_PULLUP);
  }

  if (booting == 0) //booting screen
  {
    uView.begin();
    uView.clear(PAGE);
    uView.setFontType(1);

    for (int x = 0; x < 3; x++)
    {
      uView.clear(PAGE);
      uView.setCursor(0, 0);
      uView.print("Booting");
      uView.display();
      delay(500);

      uView.clear(PAGE);
      uView.setCursor(0, 0);
      uView.print("Booting.");
      uView.display();
      delay(500);

      uView.clear(PAGE);
      uView.setCursor(0, 0);
      uView.print("Booting..");
      uView.display();
      delay(500);

      uView.clear(PAGE);
      uView.setCursor(0, 0);
      uView.print("Booting...");
      uView.display();
      delay(500);
    }
    uView.clear(PAGE);
    uView.setCursor(0, 0);
    uView.drawBitmap(nuke);
    uView.display();
    delay(5000);
  }
}

void loop()
{
  // put your main code here, to run repeatedly:
  while (bootscreen == 0) //check if this stage has aleady been run
  {
    confirmState = digitalRead(confirmButton); //read the confirm button's state

    uView.clear(PAGE);
    uView.setCursor(0, 0);
    uView.print("Please");
    uView.setCursor(0, 14);
    uView.print("Enter");
    uView.setCursor(0, 28);
    uView.print("Code");
    uView.display();
    if (confirmState == LOW) //if the button is pressed set bootscreen to 1 and end the while loop
    {
      bootscreen = 1;
      delay(1000);
    }
  }

  while (lock == 0) //check the dip switches and update the screen if they are on or off, only switch 1 is currently here, this seems to be where the issue is but I can't find it.
  {
    if (dipSwitchBitDraw == 0) //draw the dipSwitch bitmap, currently disabled for debugging
    {
      uView.clear(PAGE);
      uView.setCursor(0, 0);
      uView.drawBitmap(switches);
      uView.display();
    }
    for (int x = 0; x < 8; x++)
    {
      dipState[x] = digitalRead(dipSwitches[x]);
    }
    confirmState = digitalRead(confirmButton);
    uView.setCursor(0, 0);
    dipSwitchDraw = 5;
    dipSwitchBitDraw = 1;
    if (dipState[0] == HIGH)
    {
      uView.setColor(BLACK);
      uView.rectFill(dipSwitchDraw, 9, 5, 5);
      uView.setColor(WHITE);
      uView.rectFill(dipSwitchDraw, 28, 5, 5);
    }
    else if (dipState[0] == LOW)
    {
      uView.setColor(WHITE);
      uView.rectFill(dipSwitchDraw, 9, 5, 5);
      uView.setColor(BLACK);
      uView.rectFill(dipSwitchDraw, 28, 5, 5);
    }
    dipSwitchDraw = dipSwitchDraw + 7;
    if (dipState[1] == HIGH)
    {
      uView.setColor(BLACK);
      uView.rectFill(dipSwitchDraw, 9, 5, 5);
      uView.setColor(WHITE);
      uView.rectFill(dipSwitchDraw, 28, 5, 5);
    }
    else if (dipState[1] == LOW)
    {
      uView.setColor(WHITE);
      uView.rectFill(dipSwitchDraw, 9, 5, 5);
      uView.setColor(BLACK);
      uView.rectFill(dipSwitchDraw, 28, 5, 5);
    }
    dipSwitchDraw = dipSwitchDraw + 7;
    if (dipState[2] == HIGH)
    {
      uView.setColor(BLACK);
      uView.rectFill(dipSwitchDraw, 9, 5, 5);
      uView.setColor(WHITE);
      uView.rectFill(dipSwitchDraw, 28, 5, 5);
    }
    else if (dipState[2] == LOW)
    {
      uView.setColor(WHITE);
      uView.rectFill(dipSwitchDraw, 9, 5, 5);
      uView.setColor(BLACK);
      uView.rectFill(dipSwitchDraw, 28, 5, 5);
    }
    dipSwitchDraw = dipSwitchDraw + 7;
    if (dipState[3] == HIGH)
    {
      uView.setColor(BLACK);
      uView.rectFill(dipSwitchDraw, 9, 5, 5);
      uView.setColor(WHITE);
      uView.rectFill(dipSwitchDraw, 28, 5, 5);
    }
    else if (dipState[3] == LOW)
    {
      uView.setColor(WHITE);
      uView.rectFill(dipSwitchDraw, 9, 5, 5);
      uView.setColor(BLACK);
      uView.rectFill(dipSwitchDraw, 28, 5, 5);
    }
    dipSwitchDraw = dipSwitchDraw + 7;
    if (dipState[4] == HIGH)
    {
      uView.setColor(BLACK);
      uView.rectFill(dipSwitchDraw, 9, 5, 5);
      uView.setColor(WHITE);
      uView.rectFill(dipSwitchDraw, 28, 5, 5);
    }
    else if (dipState[4] == LOW)
    {
      uView.setColor(WHITE);
      uView.rectFill(dipSwitchDraw, 9, 5, 5);
      uView.setColor(BLACK);
      uView.rectFill(dipSwitchDraw, 28, 5, 5);
    }
    dipSwitchDraw = dipSwitchDraw + 7;
    if (dipState[5] == HIGH)
    {
      uView.setColor(BLACK);
      uView.rectFill(dipSwitchDraw, 9, 5, 5);
      uView.setColor(WHITE);
      uView.rectFill(dipSwitchDraw, 28, 5, 5);
    }
    else if (dipState[5] == LOW)
    {
      uView.setColor(WHITE);
      uView.rectFill(dipSwitchDraw, 9, 5, 5);
      uView.setColor(BLACK);
      uView.rectFill(dipSwitchDraw, 28, 5, 5);
    }
    dipSwitchDraw = dipSwitchDraw + 7;
    if (dipState[6] == HIGH)
    {
      uView.setColor(BLACK);
      uView.rectFill(dipSwitchDraw, 9, 5, 5);
      uView.setColor(WHITE);
      uView.rectFill(dipSwitchDraw, 28, 5, 5);
    }
    else if (dipState[6] == LOW)
    {
      uView.setColor(WHITE);
      uView.rectFill(dipSwitchDraw, 9, 5, 5);
      uView.setColor(BLACK);
      uView.rectFill(dipSwitchDraw, 28, 5, 5);
    }
    dipSwitchDraw = dipSwitchDraw + 7;
    if (dipState[7] == HIGH)
    {
      uView.setColor(BLACK);
      uView.rectFill(dipSwitchDraw, 9, 5, 5);
      uView.setColor(WHITE);
      uView.rectFill(dipSwitchDraw, 28, 5, 5);
    }
    else if (dipState[7] == LOW)
    {
      uView.setColor(WHITE);
      uView.rectFill(dipSwitchDraw, 9, 5, 5);
      uView.setColor(BLACK);
      uView.rectFill(dipSwitchDraw, 28, 5, 5);
    }
    uView.display();
    if (confirmState == LOW)
    {
      dipSwitchBitDraw = 0;
      uView.clear(PAGE);
      uView.setCursor(0, 0);
      uView.setColor(WHITE);
      uView.print("Autherizing");
      uView.display();
      delay(2000);
      if (dipState[0] == LOW && dipState[1] == HIGH && dipState[2] == LOW)
      {
        uView.clear(PAGE);
        uView.setCursor(0, 0);
        uView.print("yay");
        uView.display();
        delay(10000);
      }
      else
      {
        uView.clear(PAGE);
        uView.setCursor(0, 0);
        uView.print("error");
        uView.display();
        delay(10000);
      }
    }
  }
}
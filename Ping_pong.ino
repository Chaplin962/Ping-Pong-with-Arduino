#include <LiquidCrystal.h>
#include <Adafruit_NeoPixel.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define LEDs 1

Adafruit_NeoPixel strip1=Adafruit_NeoPixel(LEDs,0,NEO_GRB+NEO_KHZ800);
Adafruit_NeoPixel strip2=Adafruit_NeoPixel(LEDs,1,NEO_GRB+NEO_KHZ800);

#define start 13
#define P1U 6
#define P1D 7
#define P2U 8
#define P2D 9
#define piezo 10

byte paddle1[16]={0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0};
byte player11[8]={0,0,0,0,0,0,1,1};
byte player12[8]={1,1,0,0,0,0,0,0};

byte paddle2[16]={0,0,0,0,0,0,16,16,16,16,0,0,0,0,0,0};
byte player21[8]={0,0,0,0,0,0,16,16};
byte player22[8]={16,16,0,0,0,0,0,0};

byte ball[16]={0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0};
byte ball1[8]={0,0,0,0,0,0,0,1};
byte ball2[8]={0,0,0,0,0,0,0,0};

int x=0,bounce=0,v1=0,v2=0,v3=0,a=0,score1=0,score2=0;
char direction='L';
boolean game=0;

void setup()
{strip1.begin();
strip1.show();
strip2.begin();
strip2.show();

pinMode(P1U,INPUT);
pinMode(P1D,INPUT);
pinMode(P2U,INPUT);
pinMode(P2D,INPUT);
pinMode(start,INPUT);
pinMode(piezo,OUTPUT);

piezoSound(50);
lcd.begin(16,2);
lcd.clear();
lcd.setCursor(2,0);
lcd.print("Arduino UNO");
lcd.setCursor(1,1);
lcd.print("Ping Pong Game");
delay(1500);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("The Lower Button");
lcd.setCursor(0,1);
lcd.print("is Start");
delay(3000);}


void loop()
{while(game==0)
{for(a;a<1;a++)
{for(int i=0;i<16;i++)
{if(6<=i&&i<=9)
{paddle1[i]=1;
paddle2[i]=16;}
else
{paddle1[i]=0;
paddle2[i]=0;}
if(i==7)ball[i]=1;
else ball[i]=0;}
x=8;
lcd.clear();
lcd.setCursor(3,0);
lcd.print("Press start");
piezoSound(100);
delay(500);
lcd.setCursor(3,5);
lcd.print("Now");
piezoSound(200);}

if(digitalRead(start)==HIGH)
{lcd.clear();
lcd.setCursor(5,0);
lcd.print("Player");
lcd.setCursor(0,1);
lcd.print("1");
lcd.setCursor(15,1);
lcd.print("2");
delay(2000);

for(int i=3;i>0;i--)
{lcd.clear();
lcd.setCursor(4,0);
lcd.print("Get Set!");
lcd.setCursor(8,1);
lcd.print(i);
if(i==3)piezoSound(100);
if(i==2)piezoSound(100);
if(i==1)piezoSound(500);
delay(1000);}
a=0;
game=1;}  // set game to 1, exits the while loop and starts the game
delay(100);}

// game starts
if(digitalRead(P1U)==HIGH)
{paddle1Up();
delay(25);}
if(digitalRead(P1D)==HIGH)
{paddle1Down();
delay(25);}
if(digitalRead(P2U)==HIGH)
{paddle2Up();
delay(25);}
if(digitalRead(P2D)==HIGH)
{paddle2Down();
delay(25);}

//split arrays into 2 arrays
for(int i=0;i<=7;i++)
{player11[i]=paddle1[i];
player12[i]=paddle1[i+8];
player21[i]=paddle2[i];
player22[i]=paddle2[i+8];
ball1[i]=ball[i];
ball2[i]=ball[i+8];}

//create custom shapes for paddle1, paddle2 and ball
lcd.createChar(0, player11);
lcd.createChar(1, player12);
lcd.createChar(2, player21);
lcd.createChar(3, player22);
lcd.createChar(4, ball1);
lcd.createChar(5, ball2);

//print paddle1, paddle2 and ball
lcd.clear();
lcd.setCursor(0, 0);  // position of score1
lcd.print(score1);
lcd.setCursor(0, 1);  // position of score2
lcd.print(score2);
lcd.setCursor(3, 0);  // position of player11
lcd.write(byte(0));
lcd.setCursor(3, 1);  // position of player12
lcd.write(byte(1));
lcd.setCursor(15, 0); // position of player21
lcd.write(byte(2));
lcd.setCursor(15, 1); // position of player22
lcd.write(byte(3));
lcd.setCursor(x, 0);  // position of ball1, x is changed to move the ball on the display
lcd.write(byte(4));
lcd.setCursor(x, 1);  // position of ball2, x is changed to move the ball on the display
lcd.write(byte(5));

// checks whether the ball hits the paddle1
if(x==3)
{for(int b=0;b<16;b++)
{if(ball[b]!=0)
{v1=ball[b];
v2=paddle1[b];}}

// if ball hits paddle1, change direction of ball to R (right)
if(v1==v2)
{direction='R';
score1+=1;
piezoSound(10);}
// else display the winner
else
{lcd.clear();
lcd.setCursor(4,1);
piezoSound(500);
lcd.print("P2 won!");
delay(3000);
lcd.clear();
lcd.setCursor(3,0);
lcd.print("Game over");
piezoSound(1000);
// set scores to 0 and restart the game
score1=score2=0;
delay(1000);
game=0;}}

// checks whether the ball hits the paddle2
if(x==15)
{for(int b=0;b<16;b++)
{if(ball[b]!=0)
{v1=ball[b];
v3=paddle2[b];}}

// if ball hits paddle2, change direction of ball to L (left)
if(v1==v3)
{direction='L';
score2+=1;
piezoSound(10);}
// else display the winner
else
{lcd.clear();
lcd.setCursor(4,1);
piezoSound(500);
lcd.print("P1 won!");
delay(3000);
lcd.clear();
lcd.setCursor(3,0);
lcd.print("Game over");
piezoSound(1000);
// set scores to 0 and restart the game
score1=score2=0;
delay(1000);
game=0;}}

// if direction is L (left), move ball to left
if(direction=='L')
{for(int b=0;b<16;b++)
if(ball[b]==16)x-=1;
if(bounce==0)
ballLeftDown();
else
ballLeftUp();}

// if direction is R (right), move ball to right
if(direction=='R')
{for(int b=0;b<16;b++)
if(ball[b]==1)x+=1;
if(bounce==0)
ballRightDown();
else
  ballRightUp();}}

//User defined functions
void paddle1Up()
{if(paddle1[0]!=1)
{int temp=paddle1[0],i;
for(i=0;i<16;i++)
paddle1[i]=paddle1[i+1];
paddle1[i]=temp;}}

void paddle1Down()
{if(paddle1[15]!=1)
{int temp=paddle1[15],i;
for(i=15;i>0;i--)
paddle1[i]=paddle1[i-1];
paddle1[i]=temp;}}

void paddle2Up()
{if(paddle2[0]!=16)
{int temp=paddle2[0],i;
for(i=0;i<16;i++)
paddle2[i]=paddle2[i+1];
paddle2[i]=temp;}}

void paddle2Down()
{if(paddle2[15]!=16)
{int temp=paddle2[15],i;
for(i=15;i>0;i--)
paddle2[i]=paddle2[i-1];
paddle2[i]=temp;}}

void ballLeftDown()
{if(ball[15]==0)
{int temp=ball[15],i;
for(i=15;i>0;i--)
ball[i]=ball[i-1];
ball[i]=temp;
ballLeft();}
else
{bounce=1;
piezoSound(10);}}

void ballLeftUp()
{if(ball[0]==0)
{int temp=ball[0],i;
for(i=0;i<15;i++)
ball[i]=ball[i+1];
ball[i]=temp;
ballLeft();}
else
{bounce=0;
piezoSound(10);}}

void ballRightDown()
{if(ball[15]==0)
{int temp=ball[15],i;
for(i=15;i>0;i--)
ball[i]=ball[i-1];
ball[i]=temp;
ballRight();}
else
{bounce=1;
piezoSound(10);}}

void ballRightUp()
{if(ball[0]==0)
{int temp=ball[0],i;
for(i=0;i<15;i++)
ball[i]=ball[i+1];
ball[i]=temp;
ballRight();}
else
{bounce=0;
piezoSound(10);}}

void ballRight()
{for(int b=0;b<16;b++)
{if(ball[b]!=0)
{if(ball[b]==16)ball[b]=8;
else if(ball[b]==8)ball[b]=4;
else if(ball[b]==4)ball[b]=2;
else if(ball[b]==2)ball[b]=1;
else if(ball[b]==1)ball[b]=16;}}}

void ballLeft()
{for(int b=0;b<16;b++)
{if(ball[b]!=0)
{if(ball[b]==1)ball[b]=2;
else if(ball[b]==2)ball[b]=4;
else if(ball[b]==4)ball[b]=8;
else if(ball[b]==8)ball[b]=16;
else if(ball[b]==16)ball[b]=1;}}}

void piezoSound(int d)
{analogWrite(piezo, 20);
delay(d);
analogWrite(piezo, 0);}

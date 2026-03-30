/*  Motion Control Ball 
  This project works on the basis of the MPU sensor and OLED display 
  Where the MPU6050 sensor has the 3 axis (x,y,z)
  Using the MPU sensor the ball moves  by accelerometer which is intgrated with the sensor 
  This project works, how the ball moves it will move at that side 
  For Example: If a user tilt the board the ball moves like that to the whole screen where the user wants it to 
*/

#include <LF_Alpha.h> 
#include <math.h>

LF_Alpha_OLED OLED;
LF_Alpha_MPU6050 mpu;

/* Screen size */
const int W = 128;
const int H = 64;

/* Ball */
float ballX = W/2; //center of screen horizontally
float ballY = H/2; //center of screen   vertically 
int radius = 7;

/* smoothing */
float smooth = 0.35;

/* filter */
float axf = 0;
float ayf = 0;
float azf = 0;
float alpha = 0.25;

/* tilt → screen scale (bigger = reach edges easier) */
float scale = 90.0; //sensitivity factor

void splash()
{
  OLED.clearDisplay();
  OLED.setTextSize(2);
  OLED.setTextColor(SSD1306_WHITE);
  OLED.setCursor(19,14);//(x,y)
  OLED.println("Interial");
  OLED.setCursor(27,34);
  OLED.setTextSize(2);
  OLED.println("Motion");
  OLED.display();

  delay(1200);
}

void drawBall()
{
  float ax = mpu.readAccX(); // accleration in X direction
  float ay = mpu.readAccY(); // accleration in Y direction
  float az = mpu.readAccZ();// accleration in Z direction

  /* filter noise(LOW pass filter) */ 
  //filtered_value = previous_value + α × (new_value − previous_value)//
  axf += alpha * (ax - axf);
  ayf += alpha * (ay - ayf);
  azf += alpha * (az - azf);

  /* tilt angles */
  //roll = atan2(Y, Z)// 
  float roll  = atan2(ayf, azf);
  float pitch = atan2(axf, sqrt(ayf*ayf + azf*azf));  // FIXED DIRECTION

  /* convert to screen position */
  float targetX = W/2 + roll  * scale;
  float targetY = H/2 + pitch * scale;

  /* smooth movement */ 
  //new_position = current + (target - current) × smoothing//
  ballX += (targetX - ballX) * smooth;
  ballY += (targetY - ballY) * smooth;

  /* keep inside screen */ 
  if(ballX < radius) ballX = radius;
  if(ballX > W-radius) ballX = W-radius;

  if(ballY < radius) ballY = radius;
  if(ballY > H-radius) ballY = H-radius;

  OLED.clearDisplay();
  OLED.fillCircle((int)ballX,(int)ballY,radius,SSD1306_WHITE);
  OLED.display();
}

void setup()
{
  Serial.begin(115200);

  OLED.begin();
  splash();

  if(mpu.begin())
  {
    mpu.calibrateSensor();
  }
}

void loop()
{
  drawBall();
  delay(10);
}
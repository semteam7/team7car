/*
* Author Axel Slättman
*/

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<time.h>

void kalman(double measurement);

void input();

void clear_stdin();

//Kalman components
double x=0; //output value
double kg=0; //kalman gain
double p=1023; //estimation error
//Fiddle with these
double q=0.250; // process noise
double r=15; //sensor noise

double measurement = 0;//input from sensor

int main(){

  int go = 1;

  while(go == 1){
    input();
  }
}

void clear_stdin()
{
  while( getchar() != '\n' ){
    ;
  }
}

void kalman(double measurement){

  //prediction part
  p = p + q;

  //measurement part
  kg = p / (p+r);
  x = x + kg * (measurement - x);
  p = (1 - kg) * p;

  printf("x = %f, p = %f, kg = %f, measurement = %f\n", x, p, kg, measurement);

}

void input(){

  printf("enter measurement\n");

  scanf("%lf\n", &measurement);
  kalman(measurement);


}

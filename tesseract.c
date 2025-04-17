#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <string.h>


float cubeX, cubeY, cubeZ;
float cubeSize = 20;
int width = 160, height = 50;
float zBuf[160 * 50];
char buf[160 * 50];

int backChar = '-';

int camDist = 100;
float offsetX;

float A, B, C;
float oz;
float xplane, yplane;
float k = 40;
int idx;

float calcXcoord(int x, int y, int z){
  return y * sin(A) * sin (B) * cos(C) - z * cos(A) * sin(B) * cos(C) + y * cos(A) * sin(C) + z * sin(A) * sin(C) + x * cos(B) * cos(C);
}

float calcYcoord(int x, int y, int z){
  return y * cos(A) * cos(C) + z * sin(A) * cos(C) - y * sin(A) * sin(B) * sin(C) + z * cos(A) * sin(B) * sin(C) - x * cos(B) * sin(C);
}

float calcZcoord(int x, int y, int z){
  return z * cos(A) * cos(B) - y * sin(A) * cos(B) + x * sin(B);
}

void calcSurface(float cubeX, float cubeY, float cubeZ, int ch){
  float x, y, z;
  x = calcXcoord(cubeX, cubeY, cubeZ);
  y = calcYcoord(cubeX, cubeY, cubeZ);
  z = calcZcoord(cubeX, cubeY, cubeZ) + camDist; // stupidest person on EARTH i swear

  oz = 1 / z;
  
  xplane = (int)(width / 2 + offsetX + k * oz * x * 2);
  yplane = (int)(height / 2 + k * oz * y);
  idx = xplane + yplane * width;
  if (idx >= 0 && idx < width * height){
    if (oz > zBuf[idx]){
      zBuf[idx] = oz;
      buf[idx] = ch;
    }
  }
}


int main() {
  //printf("\033[2J");
  printf("\x1b[2J");
  // WHAT IN GODS GREEN EARTH
  while(1){
    memset(buf, backChar, sizeof(buf));
    memset(zBuf, 0, sizeof(zBuf)); // w * h * 4
    offsetX = -2 * cubeSize;
    float speed = 0.5;
    for(cubeX = -cubeSize; cubeX < cubeSize; cubeX += speed){
      for(cubeY = -cubeSize; cubeY < cubeSize; cubeY += speed){
        calcSurface(cubeX, cubeY, -cubeSize, '$');
        calcSurface(cubeSize, cubeY, cubeX, '@');
        calcSurface(-cubeSize, cubeY, -cubeX, '#');
        calcSurface(-cubeX, cubeY, cubeSize, '~');
        calcSurface(cubeX, -cubeSize, -cubeY, ';');
        calcSurface(cubeX, cubeSize, cubeY, '*');
      }
    }
    //printf("\033[H");
    printf("\x1b[H");
    for(int i = 0; i < width * height; i++){
      putchar(i % width ? buf[i] : 10);
    }
    A += 0.05;
    B += 0.05;
    C += 0.01;
    usleep(10000);
  }
  return 0;
}

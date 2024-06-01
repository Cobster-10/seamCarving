#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include "functions.h"

using namespace std;

Pixel** createImage(int width, int height) {
  cout << "Start createImage... " << endl;
  
  // Create a one dimensional array on the heap of pointers to Pixels 
  //    that has width elements (i.e. the number of columns)
  Pixel** image = new Pixel*[width];
  
  bool fail = false;
  
  for (int i=0; i < width; ++i) { // loop through each column
    // assign that column to a one dimensional array on the heap of Pixels
    //  that has height elements (i.e. the number of rows)
    image[i] = new Pixel[height];
    
    if (image[i] == nullptr) { // failed to allocate
      fail = true;
    }
  }
  
  if (fail) { // if any allocation fails, clean up and avoid memory leak
    // deallocate any arrays created in for loop
    for (int i=0; i < width; ++i) {
      delete [] image[i]; // deleting nullptr is not a problem
    }
    delete [] image; // delete array of pointers
    return nullptr;
  }
  
  // initialize cells
  //cout << "Initializing cells..." << endl;
  for (int row=0; row<height; ++row) {
    for (int col=0; col<width; ++col) {
      //cout << "(" << col << ", " << row << ")" << endl;
      image[col][row] = { 0, 0, 0 };
    }
  }
  cout << "End createImage... " << endl;
  return image;
}

void deleteImage(Pixel** image, int width) {
  cout << "Start deleteImage..." << endl;
  // avoid memory leak by deleting the array
  for (int i=0; i<width; ++i) {
    delete [] image[i]; // delete each individual array placed on the heap
  }
  delete [] image;
  image = nullptr;
}

// implement for part 1

int* createSeam(int length) {
  int *arrayPointer = new int[length]();
  for (int i = 0; i < length; ++i){
    *(arrayPointer + i) = 0;
  }
  
  return arrayPointer;
}

void deleteSeam(int* seam) {
  delete [] seam; 
}

bool loadImage(string filename, Pixel** image, int width, int height) {

  ifstream inputFile;

  // check if file opens
  inputFile.open(filename);
  if (!inputFile){
    cout << "Error: failed to open input file - " << filename << endl;
    return false;
  } 

  // check if type is p3
  string firstLine; 
  inputFile >> firstLine; 
  if (firstLine != "P3" && firstLine != "p3"){
    cout << "Error: type is " << firstLine << " instead of P3" << endl;
    return false; 
  }

  // check the width and height and max value(255)
  int checkWidth;
  int checkHeight;
  int maxValue;
  inputFile >> checkWidth; 
  if (inputFile.fail()){
    cout << "Error: read non-integer value" << endl;
    return false;
  }
  if (checkWidth != width){
    cout << "Error: input width ("<< width << ") does not match value in file (" << checkWidth << ")" << endl;
    return false;
  }
  
  inputFile >> checkHeight; 
  if (inputFile.fail()){
    cout << "Error: read non-integer value" << endl;
    return false;
  }
  if (checkHeight != height){
    cout << "Error: input height ("<< height << ") does not match value in file (" << checkHeight << ")" << endl;
    return false;
  }
  
  inputFile >> maxValue;
  if (inputFile.fail()){
    cout << "Error: read non-integer value" << endl;
    return false;
  }
  if (maxValue != 255){
    cout << "Error: invalid color value " << maxValue << endl;
    return false;
  }
  
  
  // check if there are any non integers
  
  
  
  
  int red;
  int green;
  int blue;

  for (int row=0; row<height; ++row) {
    for (int col=0; col<width; ++col) {
      inputFile >> red;
      if (inputFile.eof() && inputFile.fail()){
        cout << "Error: not enough color values" << endl;
        return false;
      }
      if (inputFile.fail()){
      cout << "Error: read non-integer value" << endl;
      return false;
      }

      if (red < 0 || red > 255){
        cout << "Error: invalid color value " << red;
        return false;
      }

      inputFile >> green;
      if (inputFile.eof() && inputFile.fail()){
        cout << "Error: not enough color values";
        return false;
      }
      if (inputFile.fail()){
        cout << "Error: read non-integer value";
        return false;
        }

      if (green < 0 || green > 255){
        cout << "Error: invalid color value " << green;
        return false;
      }

      inputFile >> blue;
      if (inputFile.eof() && inputFile.fail()){
        cout << "Error: not enough color values";
        return false;
      }
      if (inputFile.fail()){
        cout << "Error: read non-integer value";
        return false;
        }

      if (blue < 0 || blue > 255){
        cout << "Error: invalid color value " << blue;
        return false;
      }

      image[col][row].r = red;
      image[col][row].g = green;
      image[col][row].b = blue;
    }
  }
  int testInt;
  if (inputFile >> testInt){
    cout << "Error: too many color values";
    return false;
  }

  
  
  return true;
}

bool outputImage(string filename, const Pixel*const* image, int width, int height) {
  ofstream outputFile; 
   
  outputFile.open(filename);
  if (!outputFile){
    cout << "Error: failed to open output file - " << filename;
    return false;
  }

  outputFile << "P3" << "\n";
  outputFile << width << " " << height << "\n";
  outputFile << "255\n";
  for (int y =0; y < height; ++y){
    
    for (int x = 0; x < width; ++x){
      outputFile << image[x][y].r << " " << image[x][y].g  << " " << image[x][y].b;
    }
    outputFile << "\n";
  }
  

  return true;
}

int energy(const Pixel*const* image, int x, int y, int width, int height) {

  int left = 0;
  int right = width -1;
  int top = 0;
  int bottom = height -1;
  int xGradient;
  int yGradient;

  bool isLeft = false;
  bool isRight = false;
  bool isTop = false;
  bool isBottom = false;
  bool xisGreaterThan2 = true;
  bool yisGreaterThan2 = true;
  if(width <= 2){
    xGradient = 0;
    xisGreaterThan2 = false;
  }
  else if (x < left){
    x = width -1;
  }
  else if (x > right){
    x = 0;
  }
  else if (x == left ){
    isLeft = true;
  }
  else if (x == right){
    isRight = true;
  }

  // y calculations
  if (height <= 2) {
    yGradient =0;
    yisGreaterThan2 = false;
  }
  else if (y < top){
    y = height -1;
  }
  else if (y > bottom){
    y = 0;
  }
  else if (y == top){
    isTop = true;
  }
  else if(y == bottom){
    isBottom = true;
  }

  // calculate x gradient 
  int Rx;
  int Gx;
  int Bx;
  
  if (isLeft) {
    Rx = image[width - 1][y].r - image[x+1][y].r; 
    Gx = image[width - 1][y].g - image[x+1][y].g; 
    Bx = image[width - 1][y].b - image[x+1][y].b;
    xGradient = pow(Rx, 2) + pow(Gx, 2) + pow(Bx, 2);
    // square the result and add
    // make it equal to the x gradient
  }
  else if (isRight){
    Rx = image[x-1][y].r - image[0][y].r;
    Gx = image[x-1][y].g - image[0][y].g;
    Bx = image[x-1][y].b - image[0][y].b;
    xGradient = pow(Rx, 2) + pow(Gx, 2) + pow(Bx, 2);
    //square the results and add
    //make it equal to the x gradient
  }
  else if (xisGreaterThan2){
    Rx = image[x+1][y].r - image[x-1][y].r;
    Gx = image[x+1][y].g - image[x-1][y].g;
    Bx = image[x+1][y].b - image[x-1][y].b;
    xGradient = pow(Rx, 2) + pow(Gx, 2) + pow(Bx, 2);
    //square the results and add
    //make it equal to the x gradient
  }

  // calculate y gradient
  int Ry;
  int By;
  int Gy;
  
  if (isTop) {
    Ry = image[x][height -1].r - image[x][y+1].r; 
    By = image[x][height -1].b - image[x][y+1].b;
    Gy = image[x][height -1].g - image[x][y+1].g;
    yGradient = pow(Ry, 2) + pow(Gy, 2) + pow(By, 2);
    // square the result and add
    // make it equal to the y gradient
  }
  else if (isBottom){
    Ry = image[x][y-1].r - image[x][0].r;
    Gy = image[x][y-1].g - image[x][0].g;
    By = image[x][y-1].b - image[x][0].b;
    yGradient = pow(Ry, 2) + pow(Gy, 2) + pow(By, 2);
    //square the results and add
    //make it equal to the y gradient
  }
  else if (yisGreaterThan2){
    Ry = image[x][y+1].r - image[x][y-1].r;
    Gy = image[x][y+1].g - image[x][y-1].g;
    By = image[x][y+1].b - image[x][y-1].b;
    yGradient = pow(Ry, 2) + pow(Gy, 2) + pow(By, 2);
    //square the results and add
    //make it equal to the y gradient
  }

  // add up x gradient and y gradient
  return xGradient + yGradient;
}

// implement for part 2

// uncomment for part 2


int loadVerticalSeam(Pixel** image, int start_col, int width, int height, int* seam) {
  
  if (start_col > width -1){
    start_col = width -1;
  }
  else if (start_col < 0){
    start_col = 0;
  }
  bool alwaysStraightDown = false;
  if (width == 1){
    alwaysStraightDown = true;
  }

  seam[0] = start_col;
  int currentCol = start_col; 
  int left = width - 1;
  int right = 0;
  
  
  int leftDownE;
  int straightDownE;
  int rightDownE;

  int totalEnergy = energy(image, currentCol, 0, width, height);
  
  for(int i= 1; i < height; ++i){
    bool isLeft = false;
    bool isRight = false;

    bool goleftDown = false;
    bool gostraightDown = false;
    bool gorightDown = false;
    if (currentCol == left && !alwaysStraightDown){
      isLeft = true;
    }
    else if (currentCol == right && !alwaysStraightDown){
      isRight = true;
    }

    if(isLeft){
      straightDownE = energy(image, currentCol, i, width, height);
      rightDownE = energy(image, currentCol - 1, i, width, height);
      if(straightDownE <= rightDownE){
        gostraightDown = true;
      }
      else if (rightDownE < straightDownE){
        gorightDown = true;
      }
    }
    else if (isRight){
      leftDownE = energy(image, currentCol +1, i, width, height);
      straightDownE = energy(image, currentCol, i, width, height);
      if(straightDownE <= leftDownE){
        gostraightDown = true;
      }
      else if (leftDownE < straightDownE){
        goleftDown = true;
      }
    }  
    else if (!alwaysStraightDown) {
      leftDownE = energy(image, currentCol + 1, i, width, height);
      straightDownE = energy(image, currentCol, i, width, height);
      rightDownE = energy(image, currentCol - 1, i, width, height);
      if (straightDownE <= leftDownE && straightDownE <= rightDownE){
        gostraightDown = true;
      }
      else if (leftDownE < straightDownE && leftDownE <= rightDownE){
        goleftDown = true;
      }
      else if (rightDownE < straightDownE && rightDownE < leftDownE){
        gorightDown = true;
      }
    }
    else if (alwaysStraightDown){
      straightDownE = energy(image, currentCol, i, width, height);
      gostraightDown = true;
    }

    if (goleftDown){
      totalEnergy += leftDownE;
      currentCol = currentCol +1;
      seam[i] = currentCol;
    }
    else if (gostraightDown){
      totalEnergy += straightDownE;
      currentCol = currentCol;
      seam[i] = currentCol;
    }
    else if (gorightDown){
      totalEnergy += rightDownE;
      currentCol = currentCol - 1;
      seam[i] = currentCol;
    }
  }
  
  return totalEnergy;
}

// int loadHorizontalSeam(Pixel** image, int start_row, int width, int height, int* seam) {
//   return 0;
// }

int* findMinVerticalSeam(Pixel** image, int width, int height) {
  
  int *smallestSeam = new int[height];
  int *currentSeam = createSeam(height);
  
  int smallestEnergy = loadVerticalSeam(image, 0, width, height, smallestSeam);
  int currentEnergy;
  for (int i = 0; i < width; ++i){
    currentEnergy = loadVerticalSeam(image, i, width, height, currentSeam);
    if (currentEnergy < smallestEnergy){
      smallestEnergy = currentEnergy;
      delete[] smallestSeam;
      loadVerticalSeam(image, i, width, height, smallestSeam);
    }
  }
  
  
  
  return smallestSeam;
}

// int* findMinHorizontalSeam(Pixel** image, int width, int height) {
//   return nullptr;
// }

void removeVerticalSeam(Pixel** image, int width, int height, int* verticalSeam) {
  for (int row = 0; row < height; ++row){
    for (int column = verticalSeam[row] +1; column < width; ++column){
      image[column - 1][row] = image[column][row]; 
    }
    
  }
    
}


// void removeHorizontalSeam(Pixel** image, int width, int height, int* horizontalSeam) {
// }

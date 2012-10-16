// File to read map in and set gameplay as such
// modified from http://www.cplusplus.com/doc/tutorial/files/

// reading a text file
#include "mapReader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>// for atoi
using namespace std;

Map* MapReader::readMap(const string& s) {
	// To read file
  string file = s;
  string row;
  // To keep track of pos in file
  int iterator = 0;
  bool readingMap = false; // not until w/h/d are found
  bool widthFound = false;
  bool heightFound= false;
  bool depthFound = false;
  bool goalHeightFound = false;
  bool goalDepthFound = false;
  bool haveRed   = false;
  bool haveGreen = false;
  bool haveBlue  = false;
  float red, green, blue;
  int padding = 1;
  int w = padding;
  int h = padding;
  int d = padding;
  // To return map
  Map* map = new Map;
	
  // Stream in the file
  ifstream newMap;
  newMap.open(file.c_str());
	
  // Now find the ToC details
  if (newMap.is_open()) {
    while (newMap.good()) {
      getline(newMap, row);
      // Find initial properties
      if (!widthFound && !row.substr(0,6).compare("width:")) {
        widthFound = true;
        map->setWidth(2*padding+atoi((row.substr(6,row.length()-6)).c_str()));
      }
      if (!heightFound && !row.substr(0,7).compare("height:")){
        heightFound = true;
        map->setHeight(2*padding+atoi((row.substr(7,row.length()-7)).c_str()));
      }
      if (!depthFound && !row.substr(0,6).compare("depth:")) {
        depthFound = true;
        map->setDepth(2*padding+atoi((row.substr(6,row.length()-6)).c_str()));
      }
      if (!goalHeightFound && !row.substr(0,11).compare("goalHeight:")) {
        goalHeightFound = true;
        map->setGoalHeight(atoi((row.substr(11,row.length()-11)).c_str()));
      }
      if (!goalDepthFound && !row.substr(0,10).compare("goalDepth:")) {
        goalDepthFound = true;
        map->setGoalDepth(atoi((row.substr(10,row.length()-10)).c_str()));
      }
      // Find background color
      if (!haveRed && !row.substr(0,4).compare("red:")) {
        haveRed = true;
        red = atof((row.substr(4,row.length()-4)).c_str());
      }
      if (!haveGreen && !row.substr(0,6).compare("green:")) {
        haveGreen = true;
        green = atof((row.substr(6,row.length()-6)).c_str());
      }
      if (!haveBlue && !row.substr(0,5).compare("blue:")) {
        haveBlue = true;
        blue = atof((row.substr(5,row.length()-5)).c_str());
      }
      // Start the spot reading!
      // (read in the cubes from their individual spots)
      if (row.length()==0 && !readingMap && widthFound && heightFound && depthFound) {
        readingMap = true;
        // But also check to see if we have a full set of new colors
        if (haveRed && haveGreen && haveBlue) {
          map->setCustomColors(red, green, blue);
        }
      }
      if (readingMap) {
        if (row.length()==0) {
          d=padding;
          h++;
        } else {
          w = padding;
          while (w-padding<row.length()&&w<map->getWidth()-padding) {
            // Convert map numbers to material numbers
            std::string mapColorString = row.substr(w-padding,1);
            int mapColor = atoi(mapColorString.c_str());
            // Add any tile
            if (mapColor!=0) {
              CubeObj* newCube = new CubeObj();
              newCube->setMaterial(mapColor);
              newCube->tick();
              map->addCube(newCube,w,h,d);
              map->setCubeCount(map->getCubeCount()+1);
            }
            w++;
          }
          d++;
        }
      }
      iterator++;
    }

    if (newMap.good()) {
      cout << "It failed!" << endl;
    }
    newMap.close();
  }

  else cout << "Unable to open " << file << endl; 
	
  // Finally, just before returning it, add invisible walls
  // Z wallz first
  for (int w=0; w<map->getWidth(); w++) {
    for (int h=0; h<map->getHeight(); h++) {
      // Rear Wall
      for (int d=0; d<padding; d++) {
        CubeObj* newCube = new CubeObj();
        newCube->setInvisible(true);
        newCube->setMaterial(8);
        newCube->tick();
        map->addCube(newCube,w,h,map->getDepth()-padding+d); // THIS IS CORRECT ON LEVEL 0
        map->setCubeCount(map->getCubeCount()+1);
      }
      // Front Wall
      for (int d=0; d<padding; d++) {
        CubeObj* newCube = new CubeObj();
        newCube->setInvisible(true);
        newCube->setMaterial(8);
        newCube->tick();
        map->addCube(newCube,w,h,d); // THIS IS CORRECT ON LEVEL 0
        map->setCubeCount(map->getCubeCount()+1);
      }
    }
  }
  // Then X walls
  for (int d=padding; d<map->getDepth()-padding; d++) {
    for (int h=0; h<map->getHeight(); h++) {
      // Right Wall
      for (int w=0; w<padding; w++) {
        CubeObj* newCube = new CubeObj();
        newCube->setInvisible(true);
        newCube->setMaterial(8);
        newCube->tick();
        map->addCube(newCube,map->getWidth()-padding+w,h,d);
        map->setCubeCount(map->getCubeCount()+1);
      }
      // Left Wall
      for (int w=0; w<padding; w++) {
        CubeObj* newCube = new CubeObj();
        newCube->setInvisible(true);
        newCube->setMaterial(8);
        newCube->tick();
        map->addCube(newCube,w,h,d);
        map->setCubeCount(map->getCubeCount()+1);
      }
    }
  }
  return map;
}

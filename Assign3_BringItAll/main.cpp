//
//  main.cpp
//  Assign3_BringItAll
//
//  Created by Kapil Somani on 11/21/14.
//  Copyright (c) 2014 NCSU_Graphics. All rights reserved.
//

#include <iostream>
#include <GLUT/glut.h>

#include "objectLoader.h"
#include "GameScreen.h"

int main(int argc, char ** argv) {
    // insert code here...
    std::cout << "May the Force be with you!\n";
    
    glutInit(&argc, argv);
    
    GameScreen g;
    
    g.loadGame(1);
    g.loadScreen();
    
    return 0;
}

//
//  GameScreen.h
//  Assign3_BringItAll
//
//  Created by Kapil Somani on 11/21/14.
//  Copyright (c) 2014 NCSU_Graphics. All rights reserved.
//

#ifndef __Assign3_BringItAll__GameScreen__
#define __Assign3_BringItAll__GameScreen__


#include <iostream>
#include "objectLoader.h"

#include <GLUT/glut.h>

#define MAX_MODELS 5000

#define xWidth 1200
#define yWidth 750
#define zDepth 1000
#define xPix 2400
#define yPix 1200


class GameScreen
{
    private:
    public:
        void loadGameObjects();
        void loadGame(int levelNo);
        void loadScreen();
};

#endif /* defined(__Assign3_BringItAll__GameScreen__) */

//
//  objectLoader.h
//  Assign3_BringItAll
//
//  Created by Kapil Somani on 11/21/14.
//  Copyright (c) 2014 NCSU_Graphics. All rights reserved.
//

#ifndef __Assign3_BringItAll__objectLoader__
#define __Assign3_BringItAll__objectLoader__

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#define MAX_VERTICES 10000
#define MAX_FACES 10000
#define MAX_GROUPS 10000

using namespace std;

typedef enum ObjectType {OBJ_PLAYER, OBJ_ENEMY, OBJ_FENCE, OBJ_BULLET, OBJ_UFO, OBJ_INVALID, OBJ_BLAST, OBJ_ENEMY_BULLET, OBJ_DBL_BULLET} ObjectType;

class ObjectLoader
{
    private:
        vector<float> vertex[MAX_VERTICES];
        int vertexCount;
        vector<int> face[MAX_FACES];
        int faceCount;
        string groups[MAX_GROUPS];
        int groupCount;
        vector<float> coeff[MAX_GROUPS];
        bool visible;
        bool moving;
        ObjectType spaceShipType;
        float initX,initY;
        float scalX, scalY;
        int level;
    public:
        ObjectLoader();
        void loadModel(string fileName, float* translate, float* scale, ObjectType objType);
        void objectDetails();
        int getFaceCount();
        vector<float> getFaceVertices(int faceID);
        void moveBullet(float deltaY);
        void moveEnemy(float deltaX, float deltaY);
        void movePlayer(float deltaX);
        bool isVisible();
        bool isEnemy();
        bool isPlayer();
        bool isInvalid();
        bool isUFO();
        bool isBarricade();
        bool isFence();
        bool isBullet();
        bool isDoubleBullet();
        bool isEnemyBullet();
        bool isBlast();
        float objInitX();
        float objInitY();
        void setObjType(ObjectType newObjType);
        void killObj();
        void blastPhase();
    
};

#endif /* defined(__Assign3_BringItAll__objectLoader__) */

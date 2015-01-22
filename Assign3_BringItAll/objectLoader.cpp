//
//  objectLoader.cpp
//  Assign3_BringItAll
//
//  Created by Kapil Somani on 11/21/14.
//  Copyright (c) 2014 NCSU_Graphics. All rights reserved.
//

#include "objectLoader.h"

ObjectLoader::ObjectLoader()
{
    vertexCount = 0;
    faceCount = 0;
    visible = true;
}

void ObjectLoader::loadModel(string fileName, float* translate, float* scale, ObjectType objType)
{
    spaceShipType = objType;
    initX = translate[0];
    initY = translate[1];
    scalX = scale[0];
    scalY = scale[1];
    ifstream objFile (fileName);
    if(objFile.is_open())
    {
        string line;
        char delimiter = ' ';
        string token;
        groupCount = 0;
        int groupID = 0;
        
        while(getline(objFile, line))
        {
            if(line[0] == '#')
            {
                continue;
            }
            
            stringstream ss(line);
            getline(ss, token, delimiter);
            
            if(token == "v")
            {
                getline(ss, token, delimiter);
                vertex[vertexCount].push_back(scale[0]*(atof(token.c_str()) + translate[0]));
                getline(ss, token, delimiter);
                vertex[vertexCount].push_back(scale[1]*(atof(token.c_str()) + translate[1]));
                getline(ss, token, delimiter);
                vertex[vertexCount].push_back(1*(atof(token.c_str()) + translate[1]));
                
                vertexCount++;
            }
            
            if(token == "g")
            {
                getline(ss, token, delimiter);
                groups[groupCount] = token;
                
                groupCount++;
            }
            
            if(token == "f")
            {
                while(getline(ss, token, delimiter))
                {
                    face[faceCount].push_back(atoi(token.c_str()));
                }
                face[faceCount].push_back(groupCount);
                
                faceCount++;
            }
            
            if(token == "newmtl")
            {
                getline(ss, token, delimiter);
                for(int i=0; i<groupCount; i++)
                {
                    if(groups[i] == token)
                    {
                        groupID = i;
                        break;
                    }
                }
            }
            
            if(token == "Ka" || token == "Kd" || token == "Ks")
            {
                while(getline(ss, token, delimiter))
                {
                    coeff[groupID].push_back(atof(token.c_str()));
                }
            }
            
        }
    }
    else
    {
        cout<<"Error in loading file "<<fileName<<endl;
        perror("reason");
        exit(0);
    }
}


void ObjectLoader::objectDetails()
{
    cout << "Object Loader Func!!" << endl;
    cout << "vertexCount:" << vertexCount << endl;
    for(int i=0; i<vertexCount; i++)
    {
        cout << "V" << i+1 << ": " << vertex[i][0] << "," << vertex[i][1] << "," << vertex[i][2] << endl;
    }
    cout << "faceCount:" << faceCount << endl;
    for(int i=0; i<faceCount; i++)
    {
        cout << "F" << i+1 << ": " << face[i][0] << "," << face[i][1] << "," << face[i][2] << endl;
        int coeffId = face[i][3] - 1;
        cout << "\t Ka: " << coeff[coeffId][1] << endl;
    }
    cout << "groupCount:" << groupCount << endl;
    for(int i=0; i<groupCount; i++)
    {
        cout << "G" << i+1 << ": " << groups[i] << endl;
    }
}

int ObjectLoader::getFaceCount()
{
    return faceCount;
}

vector<float> ObjectLoader::getFaceVertices(int faceID)
{
    vector<float> ret;
    
    ret.push_back(vertex[face[faceID][0] - 1][0]);
    ret.push_back(vertex[face[faceID][0] - 1][1]);
    ret.push_back(vertex[face[faceID][0] - 1][2]);
    ret.push_back(vertex[face[faceID][1] - 1][0]);
    ret.push_back(vertex[face[faceID][1] - 1][1]);
    ret.push_back(vertex[face[faceID][1] - 1][2]);
    ret.push_back(vertex[face[faceID][2] - 1][0]);
    ret.push_back(vertex[face[faceID][2] - 1][1]);
    ret.push_back(vertex[face[faceID][2] - 1][2]);
    
    int coeffId = face[faceID][3] - 1;
    
    ret.push_back(coeff[coeffId][0]);
    ret.push_back(coeff[coeffId][1]);
    ret.push_back(coeff[coeffId][2]);
    ret.push_back(coeff[coeffId][3]);
    ret.push_back(coeff[coeffId][4]);
    ret.push_back(coeff[coeffId][5]);
    ret.push_back(coeff[coeffId][6]);
    ret.push_back(coeff[coeffId][7]);
    ret.push_back(coeff[coeffId][8]);
    
    
    return ret;
}

void ObjectLoader::killObj()
{
    for(int i=0;i<faceCount;i++)
    {
        int coeffId = face[i][3] - 1;
        
        coeff[coeffId][0] = 0.3;
        coeff[coeffId][1] = 0.3;
        coeff[coeffId][2] = 0.3;
        coeff[coeffId][3] = 0.3;
        coeff[coeffId][4] = 0.3;
        coeff[coeffId][5] = 0.3;
        coeff[coeffId][6] = 0.3;
        coeff[coeffId][7] = 0.3;
        coeff[coeffId][8] = 0.3;

    }
}

void ObjectLoader::setObjType(ObjectType newObjType)
{
    spaceShipType = newObjType;
//    if(newObjType == OBJ_INVALID)
}

void ObjectLoader::moveBullet(float deltaY)
{
    int i;
    initY = initY + deltaY;
    for(i = 0; i < vertexCount;i++)
    {
        vertex[i][1] = vertex[i][1] + deltaY;
        if(vertex[i][1] > 1600 || vertex[i][1] < -100)  //if bullet is out of bounds, invalidate that object
        {
           spaceShipType = OBJ_INVALID;
           // exit(246);
        }
    }
}

void ObjectLoader::movePlayer(float deltaX)
{
    initX = initX + deltaX;
    int i;
    for(i = 0; i < vertexCount;i++)
    {
        vertex[i][0] = vertex[i][0] + deltaX;
    }
}

void ObjectLoader::moveEnemy(float deltaX, float deltaY)
{
    int i;
    initX = initX + deltaX;
    initY = initY + deltaY;
    for(i = 0; i < vertexCount;i++)
    {
        vertex[i][0] = vertex[i][0] + deltaX;
        vertex[i][1] = vertex[i][1] + deltaY;
    }
}

bool ObjectLoader::isVisible()
{
    return visible;
}

void ObjectLoader::blastPhase()
{
    for(int i=0;i<faceCount;i++)
    {
        int coeffId = face[i][3] - 1;
        
        coeff[coeffId][0] = coeff[coeffId][0] + 0.0003;
        coeff[coeffId][1] = coeff[coeffId][1] + 0.0003;
        coeff[coeffId][2] = coeff[coeffId][2] + 0.0003;
        
        if(coeff[coeffId][0] >= 0.9)
        {
            spaceShipType = OBJ_INVALID;
        }
        
    }
}

bool ObjectLoader::isPlayer()
{
    if(spaceShipType == OBJ_PLAYER)
    {
        return true;
    }
    return false;
}

bool ObjectLoader::isEnemy()
{
    if(spaceShipType == OBJ_ENEMY)
    {
        return true;
    }
    return false;
}

bool ObjectLoader::isUFO()
{
    if(spaceShipType == OBJ_UFO)
    {
        return true;
    }
    return false;
}

bool ObjectLoader::isBarricade()
{
    if(spaceShipType == OBJ_FENCE)
    {
        return true;
    }
    return false;
}

bool ObjectLoader::isBullet()
{
    if(spaceShipType == OBJ_BULLET)
    {
        return true;
    }
    return false;
}

bool ObjectLoader::isEnemyBullet()
{
    
    if(spaceShipType == OBJ_ENEMY_BULLET)
    {
        return true;
    }
    return false;
}

bool ObjectLoader::isDoubleBullet()
{
    if(spaceShipType == OBJ_DBL_BULLET)
    {
        return true;
    }
    return false;
}

bool ObjectLoader::isBlast()
{
    if(spaceShipType == OBJ_BLAST)
    {
        return true;
    }
    return false;
}

bool ObjectLoader::isFence()
{
    if(spaceShipType == OBJ_FENCE)
    {
        return true;
    }
    return false;
}

bool ObjectLoader::isInvalid()
{
    if(spaceShipType == OBJ_INVALID)
    {
        return true;
    }
    return false;
}

float ObjectLoader::objInitX()
{
    return initX;
}

float ObjectLoader::objInitY()
{
    return initY;
}
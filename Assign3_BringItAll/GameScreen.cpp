//
//  GameScreen.cpp
//  Assign3_BringItAll
//
//  Created by Kapil Somani on 11/21/14.
//  Copyright (c) 2014 NCSU_Graphics. All rights reserved.
//

#include "GameScreen.h"
#include <stdio.h>
#include <stdlib.h>

//level
int currLevel;
//game objects
ObjectLoader gameObject[MAX_MODELS];
int gameObjectCount = 0;
//collision
float deltaCollide = 30;
//player
float playerTrans = 0;
float deltaPlayerTrans = 50;
float playerTransVal = 100;
float playerLeftTransLimit = 0;
float playerRightTransLimit = xPix;
int score = 0;
int lives = 3;
//enemy
float deltaXTransition = 20;
float deltaYTransition = 0;
float transitionX = 350;
float transitionY = -20;
float enemyLeftTransLimit = 20;
float enemyRightTransLimit = xPix/2 - 200;
float enemyBulletChance = 2;
float enemyBreachY = 350;
//bullet
float bulletXCord = 0;
float deltaBulletTransition = 10;
string bulletType = "images/bullet.obj";
int doubleBullet = 0;
int doubleBulletDuration = 20;
//texture
unsigned int width, height;
unsigned char* data;
//powerUp
float doubleBulletChance = 0.25;
//screens
int startScreenDuration = 100;
int levelTransitionDuration = 50;
int levelTransitionLeft = 0;
int killTransitionDuration = 50;
int killTransitionLeft = 0;
int gameOverTransitionDuration = 100;
int gameOverTransitionLeft = 0;
int breachTransitionDuration = 50;
int breachTransitionLeft = 0;

//functions
void loadLevel(int);
void clearField();
void gameOverScreen();
void shoot(int);
void checkCollision();
void breachScreen();

void checkCollision()
{
    for (int i=0; i<gameObjectCount; i++)
    {
        if(gameObject[i].isBullet())
        {
            float bullI = gameObject[i].objInitX();
            float bullJ = gameObject[i].objInitY();
            
            for (int j=0; j<gameObjectCount; j++)
            {
                if(gameObject[j].isEnemy())
                {
                    float enemI = gameObject[j].objInitX();
                    float enemJ = gameObject[j].objInitY();
                 
                    float diffX = bullI - enemI;
                    float diffY = bullJ - enemJ;
                    
                    if(diffX < deltaCollide && diffX > -deltaCollide)
                    {
                        if(diffY < 2*deltaCollide && diffY > -2*deltaCollide)
                        {
                            gameObject[j].setObjType(OBJ_BLAST);
                            gameObject[j].killObj();
                            gameObject[i].setObjType(OBJ_INVALID);
                            score = score + 5;
                          //  exit(0);
                        }
                    }
                }
                if(gameObject[j].isFence())
                {
                    float playI = gameObject[j].objInitX();
                    float playJ = gameObject[j].objInitY();
                    
                    float diffX = bullI - playI;
                    float diffY = bullJ - playJ;
                    
                    if(diffX < deltaCollide && diffX > -deltaCollide)
                    {
                        if(diffY < 10 && diffY > -10)
                        {
                            cout<<"FenceBroken by player Bullet!!"<<endl;
                            gameObject[i].setObjType(OBJ_INVALID);
                            gameObject[j].setObjType(OBJ_INVALID);
                        }
                    }
                }
            }
            
        }
    }
    for (int i=0; i<gameObjectCount; i++)
    {
        if(gameObject[i].isEnemyBullet())
        {
            float eBullI = gameObject[i].objInitX();
            float eBullJ = gameObject[i].objInitY();
            for(int j=0; j<gameObjectCount; j++)
            {
                if(gameObject[j].isPlayer())
                {
                    float playI = gameObject[j].objInitX();
                    float playJ = gameObject[j].objInitY();
                    
                    float diffX = eBullI - playI;
                    float diffY = eBullJ - playJ;
                    
                    if(diffX < 2*deltaCollide && diffX > -2*deltaCollide)
                    {
                        if(diffY < 2*deltaCollide && diffY > -2*deltaCollide)
                        {
                            cout<<"PlayerKilled!!"<<endl;
                            killTransitionLeft = killTransitionDuration;
                            lives = lives - 1;
                            if(lives == 0)
                            {
                                gameOverTransitionLeft = gameOverTransitionDuration;
                            }
                            clearField();
                            loadLevel(currLevel);
                        }
                    }
                }
                if(gameObject[j].isFence())
                {
                    float playI = gameObject[j].objInitX();
                    float playJ = gameObject[j].objInitY();
                    
                    float diffX = eBullI - playI;
                    float diffY = eBullJ - playJ;
                    
                    if(diffX < deltaCollide && diffX > -deltaCollide)
                    {
                        if(diffY < 10 && diffY > -10)
                        {
                            cout<<"FenceBroken by Enemy Bullet!!"<<endl;
                            gameObject[i].setObjType(OBJ_INVALID);
                            gameObject[j].setObjType(OBJ_INVALID);
                        }
                    }
                }
            }
        }
    }
    for (int i=0; i<gameObjectCount; i++)
    {
        if(gameObject[i].isDoubleBullet())
        {
            float powUpI = gameObject[i].objInitX();
            float powUpJ = gameObject[i].objInitY();
            for(int j=0; j<gameObjectCount; j++)
            {
                if(gameObject[j].isPlayer())
                {
                    float playI = gameObject[j].objInitX();
                    float playJ = gameObject[j].objInitY();
                    
                    float diffX = powUpI - playI;
                    float diffY = powUpJ - playJ;
                    
                    if(diffX < deltaCollide && diffX > -deltaCollide)
                    {
                        if(diffY < 2*deltaCollide && diffY > -2*deltaCollide)
                        {
                            cout<<"PowerUp Acquired!"<<endl;
                            doubleBullet = doubleBulletDuration;
                            gameObject[i].setObjType(OBJ_INVALID);
                            break;
                        }
                    }
                }
            }
        }
    }
}

void clearField()
{
    cout<<"Clearing field"<<endl;
    for(int i=0; i<gameObjectCount; i++)
    {
        if(gameObject[i].isBullet() || gameObject[i].isEnemyBullet() || gameObject[i].isEnemy()  || gameObject[i].isFence())
        {
            gameObject[i].setObjType(OBJ_INVALID);
        }
    }
}

void printText()
{
    glColor3f (1, 0, 0);
    
    glRasterPos3f( 100, yPix - 100, 0 );
    glutBitmapCharacter( GLUT_BITMAP_9_BY_15, 'S' );
    glutBitmapCharacter( GLUT_BITMAP_9_BY_15, 'c' );
    glutBitmapCharacter( GLUT_BITMAP_9_BY_15, 'o' );
    glutBitmapCharacter( GLUT_BITMAP_9_BY_15, 'r' );
    glutBitmapCharacter( GLUT_BITMAP_9_BY_15, 'e' );
    glutBitmapCharacter( GLUT_BITMAP_9_BY_15, ' ' );
    glutBitmapCharacter( GLUT_BITMAP_9_BY_15, ':' );
    glutBitmapCharacter( GLUT_BITMAP_9_BY_15, ' ' );
    char scoreStr[5];
    sprintf(scoreStr,"%d",score);
    glutBitmapCharacter( GLUT_BITMAP_9_BY_15, scoreStr[0] );
    glutBitmapCharacter( GLUT_BITMAP_9_BY_15, scoreStr[1] );
    glutBitmapCharacter( GLUT_BITMAP_9_BY_15, scoreStr[2] );
    glutBitmapCharacter( GLUT_BITMAP_9_BY_15, scoreStr[3] );
    
    glRasterPos3f( xPix/2 - 100, yPix - 100, 0 );
    glutBitmapCharacter( GLUT_BITMAP_9_BY_15, 'L' );
    glutBitmapCharacter( GLUT_BITMAP_9_BY_15, 'e' );
    glutBitmapCharacter( GLUT_BITMAP_9_BY_15, 'v' );
    glutBitmapCharacter( GLUT_BITMAP_9_BY_15, 'e' );
    glutBitmapCharacter( GLUT_BITMAP_9_BY_15, 'l' );
    glutBitmapCharacter( GLUT_BITMAP_9_BY_15, ' ' );
    glutBitmapCharacter( GLUT_BITMAP_9_BY_15, ':' );
    glutBitmapCharacter( GLUT_BITMAP_9_BY_15, ' ' );
    char levelStr[3];
    sprintf(levelStr,"%d",currLevel);
    glutBitmapCharacter( GLUT_BITMAP_9_BY_15, levelStr[0] );
    glutBitmapCharacter( GLUT_BITMAP_9_BY_15, levelStr[1] );
    
    glRasterPos3f( xPix - 350, yPix - 100, 0 );
    glutBitmapCharacter( GLUT_BITMAP_9_BY_15, 'L' );
    glutBitmapCharacter( GLUT_BITMAP_9_BY_15, 'i' );
    glutBitmapCharacter( GLUT_BITMAP_9_BY_15, 'v' );
    glutBitmapCharacter( GLUT_BITMAP_9_BY_15, 'e' );
    glutBitmapCharacter( GLUT_BITMAP_9_BY_15, 's' );
    glutBitmapCharacter( GLUT_BITMAP_9_BY_15, ' ' );
    glutBitmapCharacter( GLUT_BITMAP_9_BY_15, ':' );
    glutBitmapCharacter( GLUT_BITMAP_9_BY_15, ' ' );
    for(int i=0; i<lives; i++)
    {
        glutBitmapCharacter( GLUT_BITMAP_9_BY_15, ' ' );
        glutBitmapCharacter( GLUT_BITMAP_9_BY_15, 1 );
    }
    
    if(doubleBullet > 0 && (doubleBullet % 2) == 0)
    {
        glRasterPos3f( xPix - 250, yPix - 125, 0 );
        glutBitmapCharacter( GLUT_BITMAP_9_BY_15, 'P' );
        glutBitmapCharacter( GLUT_BITMAP_9_BY_15, 'o' );
        glutBitmapCharacter( GLUT_BITMAP_9_BY_15, 'w' );
        glutBitmapCharacter( GLUT_BITMAP_9_BY_15, 'e' );
        glutBitmapCharacter( GLUT_BITMAP_9_BY_15, 'r' );
        glutBitmapCharacter( GLUT_BITMAP_9_BY_15, ' ' );
        glutBitmapCharacter( GLUT_BITMAP_9_BY_15, 'U' );
        glutBitmapCharacter( GLUT_BITMAP_9_BY_15, 'p' );
    }
}

void printLevel()
{
    glRasterPos3f( xPix/2 - 200, yPix/2, startScreenDuration );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'L' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'e' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'v' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'e' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'l' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, ' ' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, ' ' );
    char levelStr[3];
    sprintf(levelStr,"%d",currLevel);
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, levelStr[0] );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, levelStr[1] );
}

void killScreen()
{
    glRasterPos3f( xPix/2 - 200, yPix/2, startScreenDuration );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'P' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'l' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'a' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'y' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'e' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'r' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, ' ' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'K' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'i' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'l' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'l' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'e' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'd' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, ' ' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, '!' );
    glFlush ();
    glutSwapBuffers();
}


void breachScreen()
{
    glRasterPos3f( xPix/2 - 200, yPix/2, startScreenDuration );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'E' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'n' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'e' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'm' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'y' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, ' ' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'B' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'r' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'e' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'a' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'c' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'h' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, ' ' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, '!' );
    glFlush ();
    glutSwapBuffers();
}

void gameOverScreen()
{
    glRasterPos3f( xPix/2 - 200, yPix/2, startScreenDuration );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'G' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, ' ' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'a' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, ' ' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'm' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, ' ' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'e' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, ' ' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, ' ' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'O' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, ' ' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'v' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, ' ' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'e' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, ' ' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'r' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, ' ' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, ' ' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, '!' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, ' ' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, '!' );
    glFlush ();
    glutSwapBuffers();
    if(gameOverTransitionLeft == 1)
    {
        exit(0);
    }
}

void startScreen()
{
    glClearColor (0.0, 0.0, 0.0, 1.0);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f (1, 0, 0);
    
    glRasterPos3f( xPix/2 - 400, yPix/2 + 200, startScreenDuration );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'S' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, ' ' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'P' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, ' ' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'A' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, ' ' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'C' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, ' ' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'E' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, ' ' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, ' ' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, ' ' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'I' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, ' ' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'N' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, ' ' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'V' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, ' ' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'A' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, ' ' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'D' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, ' ' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'E' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, ' ' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'R' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, ' ' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, 'S' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, ' ' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, ' ' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, '!' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, ' ' );
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, '!' );
    
    printLevel();
    
    glFlush ();
    glutSwapBuffers();
    
}

void display()
{
    glClearColor (0.0, 0.0, 0.0, 1.0);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
    while(startScreenDuration > 0)
    {
        startScreen();
        startScreenDuration = startScreenDuration - 1;
       // cout<<startScreenDuration;
        return;
    }
    
    while(levelTransitionLeft > 0)
    {
        startScreen();
        levelTransitionLeft = levelTransitionLeft - 1;
     //   cout<<levelTransitionLeft;
        return;
    }
    
    while(killTransitionLeft > 0)
    {
        killScreen();
        killTransitionLeft = killTransitionLeft - 1;
       // cout<<killTransitionLeft;
        return;
    }
    
    
    while(gameOverTransitionLeft > 0)
    {
        gameOverScreen();
        gameOverTransitionLeft = gameOverTransitionLeft - 1;
        // cout<<killTransitionLeft;
        return;
    }
    
    while(breachTransitionLeft > 0)
    {
        breachScreen();
        breachTransitionLeft = breachTransitionLeft - 1;
        // cout<<killTransitionLeft;
        return;
    }
    
    int flag = 0;
    float trans[3];
    float scale[3];
    
    trans[0] = 0;
    trans[1] = 0;
    trans[2] = 0;
    
    scale[0] = 1.0;
    scale[1] = 1.0;
    scale[2] = 1.0;
    
    deltaYTransition = 0;
    
    if (transitionX > enemyRightTransLimit || transitionX < enemyLeftTransLimit)
    {
        deltaXTransition = -deltaXTransition;
        deltaYTransition = transitionY;
    }
    
    glColor3f (1, 0, 0);
    glBegin(GL_TRIANGLES);
        glVertex3d(0, enemyBreachY-2, 0);
        glVertex3d(0, enemyBreachY+2, 0);
        glVertex3d(50, enemyBreachY, 0);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex3d(xPix, enemyBreachY-2, 0);
    glVertex3d(xPix, enemyBreachY+2, 0);
    glVertex3d(xPix - 50, enemyBreachY, 0);
    glEnd();
    
    printText();
    
    if(rand()%100 < doubleBulletChance)
    {
        trans[0] = rand()%xPix + 100;
        trans[1] = yPix - 10;
        gameObject[gameObjectCount].loadModel("images/doubleBulletPowerUp.obj", trans, scale, OBJ_DBL_BULLET);
        gameObjectCount++;
     //   cout<<"Genearting PowerUp"<<trans[0]<<endl;
    }
    
    for (int i=0; i<gameObjectCount; i++)
    {
        if(gameObject[i].isInvalid())
        {
            continue;
        }
        
        trans[0] = 0;
        trans[1] = 0;
        trans[2] = 0;
        
        scale[0] = 1.0;
        scale[1] = 1.0;
        scale[2] = 1.0;
        
        
        if(gameObject[i].isBullet())
        {
            gameObject[i].moveBullet(deltaBulletTransition);
        }
        
        if(gameObject[i].isEnemyBullet())
        {
            gameObject[i].moveBullet(-deltaBulletTransition);
        }
        
        if(gameObject[i].isDoubleBullet())
        {
          //  cout<<"Moving PowerUp"<<endl;
            gameObject[i].moveBullet(-2 * deltaBulletTransition);
        }
        
        else if(gameObject[i].isEnemy())
        {
            flag = 1;
            gameObject[i].moveEnemy(deltaXTransition, deltaYTransition);
            if(rand()%1000 < enemyBulletChance)
            {
                trans[0] = gameObject[i].objInitX();
                trans[1] = gameObject[i].objInitY();
                trans[2] = 0;
                
                scale[0] = 1.0;
                scale[1] = 1.0;
                scale[2] = 1.0;
                
                gameObject[gameObjectCount].loadModel("images/enemyBullet.obj", trans, scale, OBJ_ENEMY_BULLET);
                gameObjectCount++;
            }
            if(gameObject[i].objInitY() <= enemyBreachY)
            {
                cout<<"enemyWins"<<endl;
                breachTransitionLeft = breachTransitionDuration;
                clearField();
                lives = lives - 1;
                if(lives == 0)
                {
                    gameOverTransitionLeft = gameOverTransitionDuration;
                }
                loadLevel(currLevel);
            }
        }
        
        else if(gameObject[i].isPlayer())
        {
            gameObject[i].movePlayer(playerTransVal);
            playerTransVal = 0;
        }
        
        else if(gameObject[i].isBlast())
        {
            gameObject[i].blastPhase();
        }
        
        int fc = gameObject[i].getFaceCount();
        
        if(gameObject[i].isVisible())
        {
            glBegin(GL_TRIANGLES);
            for(int j=0; j<fc; j++)
            {
                vector<float> v;
                v=gameObject[i].getFaceVertices(j);
                glColor3f (v[9], v[10], v[11]);
                if(gameObject[i].isEnemy())
                {
                    
                    glVertex3f(v[0], v[1], v[2]);
                    glVertex3f(v[3], v[4], v[5]);
                    glVertex3f(v[6], v[7], v[8]);
                }
                else if(gameObject[i].isPlayer())
                {
                    glVertex3f(v[0], v[1], v[2]);
                    glVertex3f(v[3], v[4], v[5]);
                    glVertex3f(v[6], v[7], v[8]);
                }
                else if(gameObject[i].isBarricade())
                {
                    glVertex3f(v[0], v[1], v[2]);
                    glVertex3f(v[3], v[4], v[5]);
                    glVertex3f(v[6], v[7], v[8]);
                }
                else if(gameObject[i].isBullet())
                {
                    glVertex3f(v[0], v[1], v[2]);
                    glVertex3f(v[3], v[4], v[5]);
                    glVertex3f(v[6], v[7], v[8]);
                }
                else if(gameObject[i].isEnemyBullet())
                {
                    glVertex3f(v[0], v[1], v[2]);
                    glVertex3f(v[3], v[4], v[5]);
                    glVertex3f(v[6], v[7], v[8]);
                }
                else if(gameObject[i].isDoubleBullet())
                {
                  //  cout<<"Printing PowerUp";
                    glColor3f (1,0,0);
                    glVertex3f(v[0], v[1], 0);
                    glVertex3f(v[3], v[4], 0);
                    glVertex3f(v[6], v[7], 0);
                  //  cout<<"V"<<v[0]<<","<<v[1]<<","<<v[2]<<endl;
                  //  cout<<"V"<<v[3]<<","<<v[4]<<","<<v[5]<<endl;
                  //  cout<<"V"<<v[6]<<","<<v[7]<<","<<v[8]<<endl;
                }
                else if(gameObject[i].isBlast())
                {
                    glVertex3f(v[0], v[1], v[2]);
                    glVertex3f(v[3], v[4], v[5]);
                    glVertex3f(v[6], v[7], v[8]);
                }
            }
            glEnd();
        }
    }
    
    transitionX = transitionX + deltaXTransition;
    
    if(flag == 0)
    {
        cout<<"All Aliens killed!"<<endl;
        clearField();
        levelTransitionLeft = levelTransitionDuration;
        loadLevel(currLevel + 1);
    }
    
    checkCollision();
    
    glFlush ();
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27:        //ESC key is pressed
            exit(0);
        case 32:
            shoot(1);
            break;
        default:
            break;
    }
}

void specialKeys(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_LEFT: //along -X Left
            if(playerTrans < playerLeftTransLimit)
            {
                break;
            }
            playerTrans = playerTrans - deltaPlayerTrans;
            bulletXCord = bulletXCord - deltaPlayerTrans;
            playerTransVal = -deltaPlayerTrans;
            break;
            
        case GLUT_KEY_RIGHT:    //along +X Right
            if(playerTrans > playerRightTransLimit)
            {
                break;
            }
            playerTrans = playerTrans + deltaPlayerTrans;
            bulletXCord = bulletXCord + deltaPlayerTrans;
            playerTransVal = deltaPlayerTrans;
            break;
            
        default:
            break;
    }
}


void loadLevel(int levelNo)
{
    float trans[3];
    float scale[3];
    string objFile;
    
    currLevel = levelNo;
    
    
    //float playerTrans = 0;
    
    transitionX = 350;
    
    string fileName;
    fileName = "Levels/level" + std::to_string(levelNo) + ".txt";
    cout<<"Opening file "<<fileName<<endl;
    ifstream levelFile (fileName);
    if(levelFile.is_open())
    {
        string line;
        char delimiter = ' ';
        string token;
        while(getline(levelFile, line))
        {
            if(line[0] == '#')
            {
                continue;
            }
            
            stringstream ss(line);
            getline(ss, token, delimiter);
            trans[0] = trans[1] = trans[2] = 0;
            scale[0] = scale[1] = scale[2] = 1;
            
            if(token == "playerShipINVALID")
            {
                getline(ss, token, delimiter);
                objFile = token;
                getline(ss, token, delimiter);
                trans[0] = atoi(token.c_str());
                getline(ss, token, delimiter);
                trans[1] = atoi(token.c_str());
                getline(ss, token, delimiter);
                scale[0] = atof(token.c_str());
                getline(ss, token, delimiter);
                scale[1] = atof(token.c_str());
                
                bulletXCord = trans[0];
                
               // cout<<"PlayerShip is "<<objFile<<" with translations "<<trans[0]<<","<<trans[1]<<endl;
                
                gameObject[gameObjectCount].loadModel(objFile, trans, scale, OBJ_PLAYER);
                gameObjectCount++;
                
            }
            
            if(token == "fence")
            {
                getline(ss, token, delimiter);
                objFile = token;
                getline(ss, token, delimiter);
                trans[0] = atoi(token.c_str());
                getline(ss, token, delimiter);
                trans[1] = atoi(token.c_str());
                
                //cout<<"Fence is "<<objFile<<" with translations "<<trans[0]<<","<<trans[1]<<endl;
                
                gameObject[gameObjectCount].loadModel(objFile, trans, scale, OBJ_FENCE);
                gameObjectCount++;
            }
            
            if(token == "enemy")
            {
                getline(ss, token, delimiter);
                objFile = token;
                getline(ss, token, delimiter);
                trans[0] = atoi(token.c_str());
                getline(ss, token, delimiter);
                trans[1] = atoi(token.c_str());
                getline(ss, token, delimiter);
                scale[0] = atof(token.c_str());
                getline(ss, token, delimiter);
                scale[1] = atof(token.c_str());
                
                //cout<<"Enemy is "<<objFile<<" with translations "<<trans[0]<<","<<trans[1]<<endl;
                
                gameObject[gameObjectCount].loadModel(objFile, trans, scale, OBJ_ENEMY);
                gameObjectCount++;
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

void shoot(int value)
{
    float trans[3];
    float scale[3];
    
    trans[0] = playerTrans + 140;
    trans[1] = 20;
    trans[2] = 0;
    
    scale[0] = 1.0;
    scale[1] = 1.0;
    scale[2] = 1.0;
    
    if(doubleBullet > 0)
    {
        trans[0] = playerTrans - 15 + 140;
        gameObject[gameObjectCount].loadModel(bulletType, trans, scale, OBJ_BULLET);
        gameObjectCount++;
        trans[0] = playerTrans + 15 + 140;
        gameObject[gameObjectCount].loadModel(bulletType, trans, scale, OBJ_BULLET);
        gameObjectCount++;
        doubleBullet = doubleBullet - 1;
     //   cout<<doubleBullet;
    }
    else
    {
        gameObject[gameObjectCount].loadModel(bulletType, trans, scale, OBJ_BULLET);
        gameObjectCount++;
    }
    
   // glutTimerFunc(1000,shoot,0);
}

void moveAliens(int value)
{
    glutPostRedisplay();
    glutTimerFunc(60 - currLevel*10,moveAliens,0);
}

void reshape(int w, int h)
{

}

void GameScreen::loadGame(int levelNo)
{
    float trans[3];
    float scale[3];
    
    trans[0] = 0;
    trans[1] = 10;
    trans[2] = 0;
    
    scale[0] = 1.0;
    scale[1] = 1.0;
    scale[2] = 1.0;
    
    string objFile = "images/cannon.obj";
    gameObject[gameObjectCount].loadModel(objFile, trans, scale, OBJ_PLAYER);
    gameObjectCount++;
    loadLevel(levelNo);
}

void GameScreen::loadScreen()
{
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (xWidth, yWidth);
    glutInitWindowPosition (20, 20);
    glutCreateWindow ("Intro2CG-Assignment III [ S P A C E   I N V A D E R S ]");
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, xPix, 0, yPix, -zDepth, zDepth);

    
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    
    glutDisplayFunc(display);
    glutTimerFunc(100,moveAliens,0);
    //glutTimerFunc(1000,shoot,0);
    glutReshapeFunc(reshape);

    glutMainLoop();
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
#ifndef GAME_H
#define GAME_H
//---------------------------------------------------------------------
#include <cmath>
#include <stdio.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <gl/glu.h>
#include "Entity.h"
#include "Player.h"
#include "Body.h"
#include <string>
#include <iostream>
#include "MatrixMath.h"
#include "CollisionManager.h"
//---------------------------------------------------------------------
#define For(i,N) for (int (i) = 0; (i) < (N); (i)++)
//---------------------------------------------------------------------
class Game {

    CollisionManager collisionManager;

    enum class CameraMode
    {
        ORTHOGRAPHIC,
        PERSPECTIVE
    };
public:
    //-------------------------------------------------------
    Game(void);
    ~Game(void);
    //-------------------------------------------------------
    void  InitGFX();
    void  ChangeSize(int w, int h);
    void  ChangeCameraMode(CameraMode mode);
    //-------------------------------------------------------
    bool  Update();
    //-------------------------------------------------------
    void  Draw();
    void  DrawGrid();
    void  DrawHUD();
    //-------------------------------------------------------
    void  NormalKeys(unsigned char key, int state);
    void  SpecialKeys(int key, int state);
    void  KeyUp(int key);
    void  KeyHeld();
    void  Mouse(int button, int state, int x, int y);
    void  MouseMotion(int x, int y);
    //-------------------------------------------------------
	void  InitMtxFont();
	void  Draw_MtxText(float x, float y, const char *fmt,...);
	void  Draw_MtxFont(float x, float y, Uint8 c);
    //-------------------------------------------------------
    float mW, mH;
    int   mCounter;
    int   mMouseX, mMouseY, mMouseMotionX, mMouseMotionY;
    int   mMouseButton, mMouseState;

    std::vector<float> cameraScale = { 100, 100, 100 };
    std::vector<float> lightPosition = { 0,1, 1 };
    float cameraRotation = 0;
    float maxDistanceFromOrigin = 1000;

    float timeLastFrame;

    bool debug = false;

    std::vector<int> heldButtons;
    //-------------------------------------------------------
    GLUquadricObj* mQuadratic;
    //-------------------------------------------------------
    char  (*mMtxFont)[7][5];
    //-------------------------------------------------------
    Player playerShip;

    std::vector<Entity*> entities;
};
//---------------------------------------------------------------------
//---------------------------------------------------------------------
#endif
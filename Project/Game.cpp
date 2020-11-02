//---------------------------------------------------------------------
//---------------------------------------------------------------------
//  (c) Mikael Fridenfalk
//  All rights reserved
//  Template for use in the course:
//  Linear Algebra, Trigonometry and Geometry, 7.5 c
//  Uppsala University, Sweden
//---------------------------------------------------------------------
//---------------------------------------------------------------------
#include "Game.h"
//---------------------------------------------------------------------
//---------------------------------------------------------------------
Game::Game(): playerShip()
{
	mMtxFont = new char[128][7][5];
	InitMtxFont();
	mCounter = 0;
 	mW = 1280, mH = 720.f;
	mMouseX = mMouseY = 0;
    mMouseButton = mMouseState = 0;
    mQuadratic = gluNewQuadric();
    gluQuadricNormals(mQuadratic, GLU_SMOOTH);
    timeLastFrame = 0;

    entities.push_back(&playerShip);
    for (int i = 0; i < playerShip.projectiles.size(); i++)
    {
        entities.push_back(&playerShip.projectiles[i]);
    }
    for (int i = 0; i < 20; i++)
    {
        entities.push_back(new Body(Entity::EntityType::ImperfectSphere, 10, 10));
    }
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
Game::~Game() {
    gluDeleteQuadric(mQuadratic);
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void Game::InitGFX() {

    glClearColor(.0f, .0f, .0f, 1.f);

    //---------------------------------------
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_LIGHTING);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glShadeModel(GL_SMOOTH);
    glDepthFunc(GL_LEQUAL);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    //---------------------------------------

    GLfloat ambient[] = { -0.2, -0.2, -0.2, 1 };
    GLfloat diffuse[] = { 0.4, 0.4, 0.4, 1 };
    GLfloat position[] = { lightPosition[0], lightPosition[1], lightPosition[2], 0 };

    GLfloat front_mat_shininess[] = { 60.0 };
    GLfloat front_mat_specular[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat front_mat_diffuse[] = { 0.5, 0.5, 0.28, 1.0 };
    GLfloat back_mat_shininess[] = { 60.0 };
    GLfloat back_mat_specular[] = { 0.5, 0.5, 0.2, 1.0 };
    GLfloat back_mat_diffuse[] = { 1.0, 0.9, 0.2, 1.0 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    glMaterialfv(GL_FRONT, GL_SHININESS, front_mat_shininess);
    glMaterialfv(GL_FRONT, GL_SPECULAR, front_mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, front_mat_diffuse);
    glMaterialfv(GL_BACK, GL_SHININESS, back_mat_shininess);
    glMaterialfv(GL_BACK, GL_SPECULAR, back_mat_specular);
    glMaterialfv(GL_BACK, GL_DIFFUSE, back_mat_diffuse);

    const GLfloat lmodel_twoside[] = { GL_TRUE };
    glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);
    //---------------------------------------

    glEnable(GL_LIGHT0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //---------------------------------------

    glEnable(GL_MULTISAMPLE_ARB);//glEnable(GL_MULTISAMPLE);
    //---------------------------------------

}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void Game::ChangeSize(int w, int h){
    mW = w, mH = h;
	glViewport(0,0,mW,mH);                         // Reset Viewport
}
void Game::ChangeCameraMode(CameraMode mode)
{
    switch (mode)
    {
    case CameraMode::ORTHOGRAPHIC: 
        glDisable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION); glLoadIdentity();
        glOrtho(0.0f, mW, mH, 0.0f, -1.0f, 0.0f);
        glMatrixMode(GL_MODELVIEW); glLoadIdentity();
        break;
    case CameraMode::PERSPECTIVE: 
        glEnable(GL_DEPTH_TEST);
        double fov = 30, dnear = .1, dfar = 2500.;
        double ratio = double(mW) / double(mH);
        glMatrixMode(GL_PROJECTION); glLoadIdentity(); // Reset The Projection Matrix
        gluPerspective(fov, ratio, dnear, dfar);
        glMatrixMode(GL_MODELVIEW); glLoadIdentity();  // Reset The Modelview Matrix

        break;
    }
}
//---------------------------------------------------------------------
bool Game::Update()
{
    double time = double(SDL_GetTicks()) / 1000.;
    double dt = time - timeLastFrame;
    bool play = true;

    for (int i = 0; i < entities.size(); i++)
    {
        if (!entities[i]->Update(dt)) { return false; }
        entities[i]->CheckWorldBounds(maxDistanceFromOrigin);
    }
    for (int i = 0; i < entities.size(); i++)
    {
        for (int j = i +1; j < entities.size(); j++)
        {
            if (!entities[i]->active || !entities[j]->active 
                || entities[i]->col == nullptr || entities[j]->col == nullptr) {
                continue;
            }

            if (collisionManager.CheckCollision(entities[i], entities[j]))
            {
                entities[i]->OnCollide(*entities[j]);
                entities[j]->OnCollide(*entities[i]);
            }
        }
    }

    KeyHeld();
    timeLastFrame = time;
    return true;
}
//---------------------------------------------------------------------
void Game::Draw(void) {

    //--------------------------------------------Clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    ChangeCameraMode(CameraMode::PERSPECTIVE);
    double time = double(SDL_GetTicks()) / 1000.;

    std::vector<float> temp = { 0, 0, -1 };
    //Rotate so you view it from above
    temp = MMath::MatrixMultiplication(temp, MMath::GetRotationMatrix(0, 10), 1, 3);
    //Rotate to look where ship is looking
    temp = MMath::MatrixMultiplication(temp, MMath::GetRotationMatrix(1, cameraRotation), 1, 3);
    //Scale
    temp = MMath::MatrixMultiplication(temp, MMath::GetScalingMatrix(cameraScale[0], cameraScale[1], cameraScale[2]), 1, 3);

    gluLookAt(temp[0] + playerShip.position[0], temp[1] + playerShip.position[1] + 15, temp[2] + playerShip.position[2],
        playerShip.position[0], playerShip.position[1] + 15, playerShip.position[2],
        0., 1., 0.);
    
    //--------------------------------------------Geometry
    glLineWidth(1);
    glPointSize(10);

    if (debug) {DrawGrid();}
    for (int i = 0; i < entities.size(); i++)
    {
        entities[i]->Draw();
    }
    DrawHUD();
	//--------------------------------------------	
    mCounter++;
    //--------------------------------------------	
}
void Game::DrawGrid()
{
    glColor3ub(255, 255, 255);
    float s = 10; //scale
    int n = maxDistanceFromOrigin / s; //half of a length
    For(i, 2 * n + 1) {
        glBegin(GL_LINES);
        glVertex3f(-n * s, 0, (i - n)*s);
        glVertex3f(n * s, 0, (i - n)*s);
        glVertex3f((i - n)*s, 0, -n * s);
        glVertex3f((i - n)*s, 0, n * s);
        glEnd();
    }

    //-----------------------Origin
    glColor3ub(32, 32, 32);
    gluSphere(mQuadratic, 5, 32, 32); 
    //-----------------------x-axis
    glColor3ub(255, 0, 0);
    glPushMatrix();
    glTranslatef(1, 0, 0);
    glRotatef(90, 0, 1, 0);
    gluCylinder(mQuadratic, .06, 0., 0., 32, 32);
    gluCylinder(mQuadratic, .06, 0., .3, 32, 32);
    glTranslatef(0, 0, -1);
    gluCylinder(mQuadratic, .01, .01, 1., 32, 32);
    glPopMatrix();
    //-----------------------y-axis
    glColor3ub(0, 255, 0);
    glPushMatrix();
    glTranslatef(0, 1, 0);
    glRotatef(-90, 1, 0, 0);
    gluCylinder(mQuadratic, .06, 0., 0., 32, 32);
    gluCylinder(mQuadratic, .06, 0., .3, 32, 32);
    glTranslatef(0, 0, -1);
    gluCylinder(mQuadratic, .01, .01, 1., 32, 32);
    glPopMatrix();
    //-----------------------z-axis
    glColor3ub(0, 0, 255);
    glPushMatrix();
    glTranslatef(0, 0, 1);
    gluCylinder(mQuadratic, .06, 0., 0., 32, 32);
    gluCylinder(mQuadratic, .06, 0., .3, 32, 32);
    glTranslatef(0, 0, -1);
    gluCylinder(mQuadratic, .01, .01, 1., 32, 32);
    glPopMatrix();
    //-----------------------
}
void Game::DrawHUD()
{
    ChangeCameraMode(CameraMode::ORTHOGRAPHIC);

    //--------------------------------------------Text
    if (!debug)
    {
        int a = mW - 200;
        glColor3ub(255, 0, 0);
        Draw_MtxText(a, mH - 3 * 24, "LIVES: %2d",
            (playerShip.life));
        glColor3ub(255, 255 / 2, 0);
        Draw_MtxText(a, mH - 2 * 24, "ACCEL: %2d",
            (int)playerShip.acc);
    }
    else
    {
        int a = mW - 600;
        glColor3ub(255, 255, 0);
    Draw_MtxText(a, mH - 8 * 24, "CAM ROT X = %4d",
        (int)cameraRotation);
    Draw_MtxText(a, mH - 7 * 24, "CAM SCALE X = %4d Y = %4d Z = %4d",
        (int)cameraScale[0], (int)cameraScale[1], (int)cameraScale[2] );
    glColor3ub(255, 255 / 2, 0);
    Draw_MtxText(a, mH - 6 * 24, "ACCEL = %4d",
        (int)playerShip.acc);
    glColor3ub(0, 255, 0);
    Draw_MtxText(a, mH - 5 * 24, "X = %4d  Y = %4d  Z = %4d",
        (int)playerShip.position[0], (int)playerShip.position[1], (int)playerShip.position[2]);
    glColor3ub(255, 0, 0);
    Draw_MtxText(a, mH - 4 * 24, "X = %4d  Y = %4d",
        mMouseX, mMouseY);
    glColor3ub(100, 100, 220);
    Draw_MtxText(a, mH - 3 * 24, "X = %4d  Y = %4d",
        mMouseMotionX, mMouseMotionY);
    Draw_MtxText(a, mH - 2 * 24, "TIME = %7u",
        SDL_GetTicks());
    //printf("[ret = %u\n",ret););
    }
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//                              Font
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void Game::InitMtxFont(){
	
	For (i,128) For (j,7) For (k,5) mMtxFont[i][j][k] = 1;
	
    const char F0[] =
    "00000" "00000" "00000" "11111" "00000" "00000" "00000"//-
    "00000" "00000" "00000" "00000" "00000" "01100" "01100"//.
    "11111" "11111" "11111" "11111" "11111" "11111" "11111"//
    "01110" "10001" "10011" "10101" "11001" "10001" "01110"//0
    "00100" "01100" "00100" "00100" "00100" "00100" "01110"//1
    "01110" "10001" "00001" "00010" "00100" "01000" "11111"//2
    "01110" "10001" "00001" "00110" "00001" "10001" "01110"//3
    "00010" "00110" "01010" "10010" "11111" "00010" "00111"//4
    "11111" "10000" "11110" "00001" "00001" "10001" "01110"//5
    "01110" "10001" "10000" "11110" "10001" "10001" "01110"//6
    "11111" "10001" "00010" "00010" "00100" "00100" "00100"//7
    "01110" "10001" "10001" "01110" "10001" "10001" "01110"//8
    "01110" "10001" "10001" "01111" "00001" "00001" "01110"//9
    "00000" "01100" "01100" "00000" "01100" "01100" "00000"//:
    "11111" "11111" "11111" "11111" "11111" "11111" "11111"//
    "11111" "11111" "11111" "11111" "11111" "11111" "11111"//
    "00000" "00000" "11111" "00000" "11111" "00000" "00000"//=
    "11111" "11111" "11111" "11111" "11111" "11111" "11111"//
    "01110" "10001" "10001" "00010" "00100" "00000" "00100"//?
    "11111" "11111" "11111" "11111" "11111" "11111" "11111"//
    "01110" "10001" "10001" "11111" "10001" "10001" "10001"//A
    "11110" "10001" "10001" "11110" "10001" "10001" "11110"//B
    "01110" "10001" "10000" "10000" "10000" "10001" "01110"//C
    "11110" "10001" "10001" "10001" "10001" "10001" "11110"//D
    "11111" "10000" "10000" "11110" "10000" "10000" "11111"//E
    "11111" "10000" "10000" "11110" "10000" "10000" "10000"//F
    "01110" "10001" "10000" "10111" "10001" "10001" "01110"//G
    "10001" "10001" "10001" "11111" "10001" "10001" "10001"//H
    "01110" "00100" "00100" "00100" "00100" "00100" "01110"//I
    "00001" "00001" "00001" "00001" "10001" "10001" "01110"//J
    "10001" "10010" "10100" "11000" "10100" "10010" "10001"//K
    "10000" "10000" "10000" "10000" "10000" "10000" "11111"//L
    "10001" "11011" "10101" "10101" "10001" "10001" "10001"//M
    "10001" "10001" "11001" "10101" "10011" "10001" "10001"//N
    "01110" "10001" "10001" "10001" "10001" "10001" "01110"//O
    "11110" "10001" "10001" "11110" "10000" "10000" "10000"//P
    "01110" "10001" "10001" "10001" "10101" "10010" "01101"//Q
    "11110" "10001" "10001" "11110" "10100" "10010" "10001"//R
    "01111" "10000" "10000" "01110" "00001" "00001" "11110"//S
    "11111" "00100" "00100" "00100" "00100" "00100" "00100"//T
    "10001" "10001" "10001" "10001" "10001" "10001" "01110"//U
    "10001" "10001" "10001" "10001" "10001" "01010" "00100"//V
    "10001" "10001" "10001" "10101" "10101" "10101" "01010"//W
    "10001" "10001" "01010" "00100" "01010" "10001" "10001"//X
    "10001" "10001" "10001" "01010" "00100" "00100" "00100"//Y
    "11111" "00001" "00010" "00100" "01000" "10000" "11111";//Z
	
	For (i,46) For (j,7) For (k,5) mMtxFont[i+45][j][k] = F0[35*i+5*j+k];
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void Game::Draw_MtxText(float x, float y, const char *fmt,...){
	//--------------------
	glPushMatrix();
	glLoadIdentity();
    //--------------------
	char text[256]; va_list	ap;
	if (fmt == NULL) return;
    va_start(ap, fmt); 
#ifdef _WIN32 //32-bit and 64-bit Windows
    vsprintf_s(text, fmt, ap);
#else
    vsprintf(text, fmt, ap);
#endif
    va_end(ap);
	//--------------------
    int n = 0;
	glPointSize(2.f);
    glBegin(GL_POINTS);
	while (text[n] != 0){
		Draw_MtxFont(x+14.f*float(n), y, text[n]);
		n++;
	}
    glEnd();
    //--------------------
	glPopMatrix();
    //--------------------
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void inline Game::Draw_MtxFont(float x, float y, Uint8 c){
	if (c == ' ') return;	
    For (m,7) For (n,5){
        if (mMtxFont[c][m][n] == '0') continue;
		glVertex2f(x+2*n,y+2*m);
	}
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------

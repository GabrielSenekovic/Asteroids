#include "Game.h"

void Game::NormalKeys(unsigned char key, int state) 
{
    for (int i = 0; i < heldButtons.size(); i++)
    {if (key == heldButtons[i]) {return;}}
    heldButtons.push_back(key);
    switch (key)
    {
    case SDLK_3: debug = !debug; break;
    case SDLK_q: //dash left
        break;
    case SDLK_e: //dash right
        break;
    }
}
void Game::SpecialKeys(int key, int state) 
{
    for (int i = 0; i < heldButtons.size(); i++)
    {if (key == heldButtons[i]) { return; }}
    heldButtons.push_back(key);
}
void Game::KeyUp(int key)
{
    for (int i = 0; i < heldButtons.size(); i++)
    {if (key == heldButtons[i]) { heldButtons.erase( heldButtons.begin() + i); }}
}
void Game::KeyHeld()
{
    for (int i = 0; i < heldButtons.size(); i++)
    {
        switch (heldButtons[i])
        {
            case SDLK_1: 
                for (int i = 0; i < 3; i++)
                {
                    cameraScale[i]++;
                }break;
            case SDLK_2: 
                for (int i = 0; i < 3; i++)
                {
                    cameraScale[i]--;
                }break;

            case SDLK_w: playerShip.Move(1); break;
            case SDLK_s: playerShip.Move(-1); break;
            case SDLK_a: playerShip.Rotate(1, 2); cameraRotation += playerShip.turnSpeed;
                break;
            case SDLK_d: playerShip.Rotate(-1, 2);  cameraRotation -= playerShip.turnSpeed;
                break;

            case SDLK_UP: playerShip.Move(1); break;
            case SDLK_DOWN: playerShip.Move(-1); break;
        }
    }
}
void Game::Mouse(int button, int state, int x, int y) {
    mMouseButton = button;//SDL_BUTTON_LEFT/SDL_BUTTON_MIDDLE/SDL_BUTTON_RIGHT
    mMouseState = state;//SDL_PRESSED/SDL_RELEASED
    mMouseX = x; mMouseY = y;
    std::cout << "X:" << x << " Y: " << y << std::endl;
}
void Game::MouseMotion(int x, int y) {
    mMouseMotionX = x; mMouseMotionY = y;
};
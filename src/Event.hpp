
#ifndef EVENT_HPP
#define EVENT_HPP
#include <SDL2/SDL.h>
#include <time.h>

class Events {
    private:
        SDL_Event e;
        int mouseX;
        int mouseY;
    public:
        Events() {}
        bool poll() {
            return SDL_PollEvent(&this->e);
        }
        // Check if the user wants to quit
        bool quit() {
            return this->e.type == SDL_QUIT;
        }
        // Check if the user pressed a key
        bool keyDown() {
           return this->e.type == SDL_KEYDOWN;
        }
        // Check if the user released a key
        bool keyUp() {
            return this->e.type == SDL_KEYUP;
        }

        bool keyPressed(SDL_Keycode key) {
            return this->e.key.keysym.sym == key;
        }
        // Check if the user pressed a mouse button
        bool mouseButtonDown() {
            return this->e.type == SDL_MOUSEBUTTONDOWN;
        }
        // Check if the user released a mouse button
        bool mouseButtonUp() {
            return this->e.type == SDL_MOUSEBUTTONUP;
        }
        // Check if the user pressed a mouse button
        bool mouseButtonPressed(Uint8 button) {
            return this->e.button.button == button;
        }
        // Check if the user released a mouse button
        bool mouseButtonReleased(Uint8 button) {
            return this->e.button.button == button;
        }
        // update mouse position
        void updateMousePosition() {
            SDL_GetMouseState(&this->mouseX, &this->mouseY);
        }
        // Get mouse x position
        int getMouseX() {
            return this->mouseX;
        }
        // Get mouse y position
        int getMouseY() {
            return this->mouseY;
        }
        int getType() {
            return this->e.type;
        }
};
#endif
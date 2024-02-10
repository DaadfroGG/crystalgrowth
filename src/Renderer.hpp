#ifndef RENDERER_HPP
#define RENDERER_HPP
#include <limits.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>
#include <set>
#define WIDTH 800
#define HEIGHT 800
#define POINT_RADIUS 5
#define MAP_HEIGHT 100
#define MAP_WIDTH 100
class Point {
public:
    int x, y;
    Point(int x, int y) : x(x), y(y) {}
};

class Edge {
public:
    Point* start;
    Point* end;
    Edge(Point* start, Point* end) : start(start), end(end) {}
};
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
        // Check if the user pressed a key
        bool keyPressed(SDL_Keycode key) {
            return this->e.key.keysym.sym == key;
        }
        // Check if the user released a key
        bool keyReleased(SDL_Keycode key) {
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
};


class Renderer {
    /* 
        // Example usage:
        // Draw a point
        renderer.drawPoint(100, 100, 0xFF0000);
        // Draw a line
        renderer.drawLine(200, 200, 300, 300, 0x00FF00);
        // Draw a circle
        renderer.drawCircle(400, 400, 50, 0x0000FF);
        // Draw a triangle
        renderer.fillTriangle(500, 500, 600, 500, 550, 400, 0xFFFF00);
        // Draw a polygon
        std::vector<Point> vertices = {Point(100, 100), Point(200, 200), Point(300, 100), Point(200, 50)};
        renderer.fillPolygon(vertices, 0x00FFFF);
    */
    private:
        SDL_Renderer* renderer;
        SDL_Window* window;
    public:
        Renderer(SDL_Renderer* renderer, SDL_Window* window);
        ~Renderer();
        void drawPoint(int x, int y, int color);
        void drawLine(int x1, int y1, int x2, int y2, int color);
        void drawCircle(int x, int y, int radius, int color);
        void fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int color);
        void fillPolygon(std::vector<Point> vertices, int color);
        void update();
        void clear();
};
#endif

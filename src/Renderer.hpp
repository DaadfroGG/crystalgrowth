#ifndef RENDERER_HPP
#define RENDERER_HPP
#include <limits.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <SDL2/SDL.h>
#include <functional>
#include <stdbool.h>
#include <math.h>
#include <set>
#define WIDTH 1800
#define HEIGHT 1000
#define POINT_RADIUS 5
#define MAP_HEIGHT 1000
#define MAP_WIDTH 1000
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

class Tex {
    private:
        Uint32* pixels;
        SDL_Surface* surface;
        SDL_Texture* texture;
        int width;
        int height;
    public:
        Tex(SDL_Renderer* renderer, SDL_Window* window, int width, int height) {
            this->pixels = new Uint32[width * height];
            this->surface = SDL_CreateRGBSurfaceFrom(
                this->pixels,  // The pixel data array
                width,         // The width of the texture
                height,        // The height of the texture
                32,            // The depth of the pixel data   
                width *  4,    // The size of each row in bytes
                SDL_GetWindowSurface(window)->format->Rmask,    // The red mask
                SDL_GetWindowSurface(window)->format->Gmask,    // The green mask
                SDL_GetWindowSurface(window)->format->Bmask,    // The blue mask
                SDL_GetWindowSurface(window)->format->Amask);   // The alpha mask
            this->texture = SDL_CreateTextureFromSurface(renderer, surface);
            this->width = width;
            this->height = height;
        }
        ~Tex() {
            SDL_FreeSurface(this->surface);
            delete[] this->pixels;
            SDL_DestroyTexture(this->texture);
        }
        SDL_Texture* getTexture() {
            return this->texture;
        }
        Uint32* getPixels() {
            return this->pixels;
        }
        int getWidth() {
            return this->width;
        }
        int getHeight() {
            return this->height;
        }
        void setPixel(int x, int y, int color , SDL_Renderer* renderer) {
            if (x < 0 || x >= this->width || y < 0 || y >= this->height) {
                return;
            } else {
                this->pixels[y * this->width + x] = color;
            }
            (void)renderer;
        }
        void update(SDL_Renderer* renderer, SDL_Window* window) {
            this->surface = SDL_CreateRGBSurfaceFrom(
                this->pixels,         // The pixel data array
                this->width,  // The width of the texture
                this->height, // The height of the texture
                32,            // The depth of the pixel data   
                this->width *  4, // The size of each row in bytes
                SDL_GetWindowSurface(window)->format->Rmask,    // The red mask
                SDL_GetWindowSurface(window)->format->Gmask,    // The green mask
                SDL_GetWindowSurface(window)->format->Bmask,    // The blue mask
                SDL_GetWindowSurface(window)->format->Amask);   // The alpha mask
            SDL_DestroyTexture(this->texture);
            this->texture = SDL_CreateTextureFromSurface(renderer, surface);
        }
};
typedef void (*drawFunction)(int, int, int, SDL_Renderer*);
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
        SDL_Renderer* getRenderer();
        SDL_Window* getWindow();
        // those are the functions that you can use to draw directly to the screen
        void drawPoint(std::function<void(int, int, int, SDL_Renderer*)> drawFunction, int x, int y, int color);
        void drawLine(std::function<void(int, int, int, SDL_Renderer*)> drawFunction, int x1, int y1, int x2, int y2, int color);
        void drawCircle(std::function<void(int, int, int, SDL_Renderer*)> drawFunction, int x, int y, int radius, int color);

        void fillTriangle(std::function<void(int, int, int, SDL_Renderer*)> drawFunction, int x1, int y1, int x2, int y2, int x3, int y3, int color);
        void fillPolygon(std::function<void(int, int, int, SDL_Renderer*)> drawFunction, std::vector<Point> vertices, int color);
        void drawTex(Tex* tex, int x, int y);

        void setPixel(int x, int y, int color , SDL_Renderer* renderer) ;
        // those are the functions that update the screen
        void update();
        void clear();
};

#endif

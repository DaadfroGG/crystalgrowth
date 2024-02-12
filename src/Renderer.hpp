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
#define WIDTH 1920
#define HEIGHT 1080
#define POINT_RADIUS 5
#define MAP_HEIGHT 1080
#define MAP_WIDTH 1920
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
enum flag{
    NO_OVERLAP = (1 << 0),
    NOISE = (1 << 1),
};
class Tex {
    private:
        Uint32* pixels;
        SDL_Surface* surface;
        SDL_Texture* texture;
        int width;
        int height;
        double noise;
    public:
        Tex(SDL_Renderer* renderer, SDL_Window* window, int width, int height, int noise = 1) {
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

        void setPixel(int x, int y, int color , SDL_Renderer* renderer, int flag) {
            if (x < 0 || x >= this->width || y < 0 || y >= this->height ){
                return;
            } else if (flag & NO_OVERLAP && this->pixels[y * this->width + x] != 0) {
                return;
            } else {
                if (flag & NOISE) {
                    int r = rand() % 255 / this->noise;
                    int g = rand() % 255 / this->noise;
                    int b = rand() % 255 / this->noise;
                    
                    r += (color >> 16) & 0xFF;
                    g += (color >> 8) & 0xFF;
                    b += color & 0xFF;
                    color = r << 16 | g << 8 | b;

                }
                this->pixels[y * this->width + x] = color;
            }
            (void)renderer;
            (void )flag;
        }
        void setNoise(double noise) {
            this->noise = noise;
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
        void clear() {
            memset(this->pixels, 0, this->width * this->height * sizeof(Uint32));
        }
};
typedef void (*drawFunction)(int, int, int, SDL_Renderer*, int);
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
        void toggleFullscreen();
        SDL_Renderer* getRenderer();
        SDL_Window* getWindow();
        // those are the functions that you can use to draw directly to the screen
        void drawPoint(std::function<void(int, int, int, SDL_Renderer*, int)> drawFunction, int x, int y, int color, int flag);
        void drawLine(std::function<void(int, int, int, SDL_Renderer*, int)> drawFunction, int x1, int y1, int x2, int y2, int color, int flag);
        void drawCircle(std::function<void(int, int, int, SDL_Renderer*, int)> drawFunction, int x, int y, int radius, int color, int flag);

        void fillTriangle(std::function<void(int, int, int, SDL_Renderer*, int)> drawFunction, int x1, int y1, int x2, int y2, int x3, int y3, int color, int flag);
        void fillPolygon(std::function<void(int, int, int, SDL_Renderer*, int)> drawFunction, std::vector<Point> vertices, int color, int flag);
        void outlinePolygon(std::function<void(int, int, int, SDL_Renderer*, int)> drawFunction, std::vector<Point> vertices, int color, int flag);
        void drawTex(Tex* tex, int x, int y);

        void setPixel(int x, int y, int color , SDL_Renderer* renderer, int flag) ;
        // those are the functions that update the screen
        void update();
        void clear();
};

#endif

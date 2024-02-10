#include "Renderer.hpp"

Renderer::Renderer(SDL_Renderer* renderer, SDL_Window* window) {
    this->window = window;
    this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
}

Renderer::~Renderer() {
    SDL_DestroyRenderer(this->renderer);
}


// void drawPoint(int x, int y, int color) {
//     SDL_SetRenderDrawColor(renderer, color >> 16, (color >> 8) & 0xFF, color & 0xFF, 255);
//     SDL_RenderDrawPoint(renderer, x, y);
//}



void Renderer::drawPoint(int x, int y, int color) {
    SDL_SetRenderDrawColor(this->renderer, color >> 16, (color >> 8) & 0xFF, color & 0xFF, 255);
    SDL_RenderDrawPoint(this->renderer, x, y);
}

void Renderer::drawLine(int x1, int y1, int x2, int y2, int color) {
    SDL_SetRenderDrawColor(this->renderer, color >> 16, (color >> 8) & 0xFF, color & 0xFF, 255);
    SDL_RenderDrawLine(this->renderer, x1, y1, x2, y2);
}
void Renderer::drawCircle(int x, int y, int radius, int color) {
    SDL_SetRenderDrawColor(this->renderer, color >> 16, (color >> 8) & 0xFF, color & 0xFF, 255);
    int x1 = 0;
    int y1 = radius;
    int d = 3 - 2 * radius;
    while (y1 >= x1) {
        drawPoint(x + x1, y - y1, color);
        drawPoint(x + y1, y - x1, color);
        drawPoint(x + y1, y + x1, color);
        drawPoint(x + x1, y + y1, color);
        drawPoint(x - x1, y + y1, color);
        drawPoint(x - y1, y + x1, color);
        drawPoint(x - y1, y - x1, color);
        drawPoint(x - x1, y - y1, color);
        if (d < 0) {
            d = d + 4 * x1 + 6;
        } else {
            d = d + 4 * (x1 - y1) + 10;
            y1--;
        }
        x1++;
    }
}
void Renderer::fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int color) {
    SDL_SetRenderDrawColor(this->renderer, color >> 16, (color >> 8) & 0xFF, color & 0xFF, 255);
    int minX = std::min(x1, std::min(x2, x3));
    int minY = std::min(y1, std::min(y2, y3));
    int maxX = std::max(x1, std::max(x2, x3));
    int maxY = std::max(y1, std::max(y2, y3));
    for (int x = minX; x <= maxX; x++) {
        for (int y = minY; y <= maxY; y++) {
            int w0 = (x2 - x1) * (y - y1) - (y2 - y1) * (x - x1);
            int w1 = (x3 - x2) * (y - y2) - (y3 - y2) * (x - x2);
            int w2 = (x1 - x3) * (y - y3) - (y1 - y3) * (x - x3);
            if ((w0 >= 0 && w1 >= 0 && w2 >= 0) || (w0 <= 0 && w1 <= 0 && w2 <= 0)) {
                SDL_RenderDrawPoint(this->renderer, x, y);
            }
        }
    }
}
void Renderer::fillPolygon(std::vector<Point> vertices, int color) {
    int i = 0;
    while (i < (int)vertices.size() - 2) {
        fillTriangle(vertices[0].x, vertices[0].y, vertices[i + 1].x, vertices[i + 1].y, vertices[i + 2].x, vertices[i + 2].y, color);
        i++;
    }
    fillTriangle(vertices[0].x, vertices[0].y, vertices[i + 1].x, vertices[i + 1].y, vertices[1].x, vertices[1].y, color);
}

void Renderer::update() {
    SDL_RenderPresent(this->renderer);
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
}

void Renderer::clear() {
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
    SDL_RenderClear(this->renderer);
}
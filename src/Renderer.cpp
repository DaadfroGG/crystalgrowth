#include "Renderer.hpp"
// Constructor
Renderer::Renderer(SDL_Renderer* renderer, SDL_Window* window) {
    this->window = window;
    this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
}
// Destructor
Renderer::~Renderer() {
    SDL_DestroyRenderer(this->renderer);
}

// Those are the functions that you can use to draw on the screen
void Renderer::setPixel(int x, int y, int color , SDL_Renderer* renderer, int flag) {
    SDL_SetRenderDrawColor(renderer, color >> 16, (color >> 8) & 0xFF, color & 0xFF, 255);
    SDL_RenderDrawPoint(renderer, x, y);
    (void)flag;
}

SDL_Renderer* Renderer::getRenderer() {
    return this->renderer;
}

SDL_Window* Renderer::getWindow() {
    return this->window;
}

void Renderer::drawPoint(std::function<void(int, int, int, SDL_Renderer*, int)> drawFunction, int x, int y, int color, int flag) {
    drawFunction(x, y, color, this->renderer, flag);
}

void Renderer::drawLine(std::function<void(int, int, int, SDL_Renderer*, int)> drawFunction, int x1, int y1, int x2, int y2, int color, int flag) {
    SDL_SetRenderDrawColor(this->renderer, color >> 16, (color >> 8) & 0xFF, color & 0xFF, 255);
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2;
    int e2;
    for (;;) {
        drawPoint(drawFunction, x1, y1, color, flag);
        if (x1 == x2 && y1 == y2) {
            break;
        }
        e2 = err;
        if (e2 > -dx) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dy) {
            err += dx;
            y1 += sy;
        }
    }
}
void Renderer::drawCircle(std::function<void(int, int, int, SDL_Renderer*, int)> drawFunction, int x, int y, int radius, int color, int flag) {
    SDL_SetRenderDrawColor(this->renderer, color >> 16, (color >> 8) & 0xFF, color & 0xFF, 255);
    int x1 = 0;
    int y1 = radius;
    int d = 3 - 2 * radius;
    while (y1 >= x1) {
        drawPoint(drawFunction, x + x1, y - y1, color, flag);
        drawPoint(drawFunction, x + y1, y - x1, color, flag);
        drawPoint(drawFunction, x + y1, y + x1, color, flag);
        drawPoint(drawFunction, x + x1, y + y1, color, flag);
        drawPoint(drawFunction, x - x1, y + y1, color, flag);
        drawPoint(drawFunction, x - y1, y + x1, color, flag);
        drawPoint(drawFunction, x - y1, y - x1, color, flag);
        drawPoint(drawFunction, x - x1, y - y1, color, flag);
        if (d < 0) {
            d = d + 4 * x1 + 6;
        } else {
            d = d + 4 * (x1 - y1) + 10;
            y1--;
        }
        x1++;
    }
}
void Renderer::fillTriangle(std::function<void(int, int, int, SDL_Renderer*, int)> drawFunction, int x1, int y1, int x2, int y2, int x3, int y3, int color, int flag) {
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
                drawPoint(drawFunction, x, y, color, flag);
            }
        }
    }
}
void Renderer::fillPolygon(std::function<void(int, int, int, SDL_Renderer*, int)> drawFunction, std::vector<Point> vertices, int color, int flag) {
    int i = 0;
    while (i < (int)vertices.size() - 2) {
        fillTriangle(drawFunction, vertices[0].x, vertices[0].y, vertices[i + 1].x, vertices[i + 1].y, vertices[i + 2].x, vertices[i + 2].y, color, flag);
        i++;
    }
    fillTriangle(drawFunction, vertices[0].x, vertices[0].y, vertices[i + 1].x, vertices[i + 1].y, vertices[1].x, vertices[1].y, color, flag);
}

void Renderer::drawTex(Tex* tex, int x, int y){
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = tex->getWidth();
    dst.h = tex->getHeight();
    SDL_RenderCopy(this->renderer, tex->getTexture(), NULL, &dst);

}
// Those are the functions that you can use to draw on a texture


// those are the functions that you can use to update the screen
void Renderer::update() {
    SDL_RenderPresent(this->renderer);
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
}

void Renderer::clear() {
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
    SDL_RenderClear(this->renderer);
}


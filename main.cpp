#include "src/Renderer.hpp"
#include "src/Event.hpp"
#include <cmath>

class Crystal {
private:
public:
    Crystal() {}
};

int main() {
    Renderer renderer(nullptr, SDL_CreateWindow("SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0));
    Events events;

    Crystal crystal;
    Tex tex(renderer.getRenderer(), renderer.getWindow(), MAP_WIDTH, MAP_HEIGHT);

    int color = 0x000000;
    const int targetFPS = 60;
    const int frameDelay = 1000 / targetFPS;
    Uint32 frameStart;
    int frameTime;

    // Main loop
    while (!events.quit()) {
        frameStart = SDL_GetTicks();

        // Clear the screen
        renderer.clear();

        // Handle events
        while (events.poll()) {
            if (events.quit()) {
                break;
            }
            // if (events.getType() == 1025) {
            //     color = 0xFF0000;
            // }
            // if (events.getType() == 1026) {
            //     color = 0x00FF00;
            // }
        }

        // Update mouse position
        events.updateMousePosition();

        // Calculate crystal growth (currently not implemented)

        // Draw a growing polygon (starting from a point x y and growing with time)
        int x = MAP_HEIGHT / 2;
        int y = MAP_WIDTH / 2;

        double radius = 50.0;
        double speed = 0.2;
        double size = frameStart * speed;

        // Adjust the size based on the desired growth rate
        double growthRate = 0.2;
        size *= growthRate;

        std::vector<Point> vertices;

        // Calculate the vertices of the polygon based on the current size
        for (double angle = 0; angle < 2 * M_PI; angle += (2 * M_PI) / 3) {
            double newX = x + size * cos(angle);
            double newY = y + size * sin(angle);
            vertices.push_back(Point(newX, newY));
        }
        // Alter the color randomly based on the current time
        color = 0x000000 + (int)(sin(frameStart / 1000.0) * 0xFFFFFF);

        renderer.fillPolygon([&tex, &renderer](int x, int y, int color, SDL_Renderer* r, int flag) {
            tex.setPixel(x, y, color, r, NO_OVERLAP);
        }, vertices, color, NO_OVERLAP);

        // Draw a polygon on texture 
        // Draw the texture
        renderer.drawTex(&tex, WIDTH / 2 - MAP_WIDTH / 2, HEIGHT / 2 - MAP_HEIGHT / 2);

        // Present the screen
        tex.update(renderer.getRenderer(), renderer.getWindow());
        renderer.update();

        // Calculate frame time and delay if needed
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
    return 0;
}

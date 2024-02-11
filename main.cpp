#include "src/Renderer.hpp"
#include "src/Event.hpp"
#include <random>

class CrystalGenerator {
    public:
        CrystalGenerator() {
        }
       void generate(int seed_x, int seed_y) {

       }
};

int main() {
    Renderer renderer(nullptr, SDL_CreateWindow("SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0));
    Events events;

    CrystalGenerator crystalGenerator;
    Tex tex(renderer.getRenderer(), renderer.getWindow(), MAP_WIDTH, MAP_HEIGHT);

    int color = 0x000000;
    double length = 10.0;
// Main loop
    while (!events.quit()) {
        // Clear the screen
        renderer.clear();


        // Handle events
        while (events.poll()) {
            if (events.quit()) {
                break;
            }
            if (events.getType() == 1025) {
                color = 0xFF0000;
            }
            if (events.getType() == 1026) {
                color = 0x00FF00;
            }

        }

        // Update mouse position
        events.updateMousePosition();

        // draw circle on texture
        renderer.drawCircle([&tex, &renderer](int x, int y, int color, SDL_Renderer* r) {tex.setPixel(x, y, color, r);}, events.getMouseX() - WIDTH / 2 + MAP_WIDTH / 2, events.getMouseY() - HEIGHT / 2 + MAP_HEIGHT / 2, 5, color);
        // Draw the texture
        renderer.drawTex(&tex, WIDTH / 2 - MAP_WIDTH / 2, HEIGHT / 2 - MAP_HEIGHT / 2);

        // draw a bigger circle on direct screen
        renderer.drawCircle([&renderer](int x, int y, int color, SDL_Renderer* r) {renderer.setPixel(x, y, color, r);}, events.getMouseX(), events.getMouseY(), 10, color);
        // Present the screen
        tex.update(renderer.getRenderer(), renderer.getWindow());
        renderer.update();

    }


    return 0;
}

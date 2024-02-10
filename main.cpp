#include "src/Renderer.hpp"
#include <random>

class LindenmayerSystem {
private:
    Renderer& renderer;

public:
    LindenmayerSystem(Renderer& renderer) : renderer(renderer) {}

    // Recursive function to draw branches
    void drawBranch(int x, int y, double length, double angle, int level) {
        srand(level + x + y + 1
        );
        if (level < 0) {
            return; // Base case: stop recursion
        }

        // Calculate the end point of the branch
        int xEnd = x + static_cast<int>(length * cos(angle));
        int yEnd = y + static_cast<int>(length * sin(angle));

        // Draw the branch
        renderer.drawLine(x, y, xEnd, yEnd, 0x00FF00);

        // Flip a coin to decide if we should draw a sub-branch
        if (rand() % 2 == 0) {
            drawBranch(xEnd, yEnd, length * 1, angle - 0.3, level - 1);
        }
        if (rand() % 2 == 0) {
            drawBranch(xEnd, yEnd, length * 1, angle + 0.3, level - 1);
        }
                if (rand() % 2 == 0) {
            drawBranch(xEnd, yEnd, length * 1, angle - 0.8, level - 1);
        }
        if (rand() % 2 == 0) {
            drawBranch(xEnd, yEnd, length * 1, angle + 0.8, level - 1);
        }

    }
};

int main() {
    Renderer renderer(nullptr, SDL_CreateWindow("SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0));
    Events events;

    LindenmayerSystem lSystem(renderer);

    // Main loop
    while (!events.quit()) {
        events.updateMousePosition();
        events.poll();

        // Clear the screen
        renderer.clear();

        // Draw branches with the Lindenmayer system
        lSystem.drawBranch(WIDTH / 2, HEIGHT - 50, 100.0, -M_PI / 2, 10);

        // Present the screen
        renderer.update();
    }

    return 0;
}

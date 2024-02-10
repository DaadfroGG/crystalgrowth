#include "src/Renderer.hpp"
#include <random>

class LindenmayerSystem {
private:
    Renderer& renderer;
    double deviation;
    double reduction;
public:
    LindenmayerSystem(Renderer& renderer) : renderer(renderer) {
        this->deviation = 0.5;
        this->reduction = 0.5;
    }

    // Recursive function to draw branches
    void drawBranch(int x, int y, double length, double angle, int level) {
        srand(time(NULL));

        if (level < 0) {
            return; // Base case: stop recursion
        }

        // Calculate the end point of the branch
        int xEnd = x + static_cast<int>(length * cos(angle));
        int yEnd = y + static_cast<int>(length * sin(angle));

        // Draw the branch
        renderer.drawLine(x, y, xEnd, yEnd, 0x00FF00);
        // Draw the continuation
        drawBranch(xEnd, yEnd, length, angle, level - 1);

        // Flip a coin to decide if we should draw a sub-branch
        if (level % 2 == 0) {
            drawBranch(xEnd, yEnd, length*this->reduction, angle - this->deviation, level - 1);
        }
        else {
            drawBranch(xEnd, yEnd, length*this->reduction, angle + this->deviation, level - 2);
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
        lSystem.drawBranch(WIDTH / 2, HEIGHT - 10, 50.0, -M_PI / 2, 10);

        // Present the screen
        renderer.update();
    }

    return 0;
}

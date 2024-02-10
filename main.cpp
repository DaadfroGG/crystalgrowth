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
        // Draw the line center of the rectangle, direction dictating
        renderer.drawLine(x, y, xEnd, yEnd, 0x00FF00);
        drawRectangle(x, y, xEnd, yEnd, length);
        // Draw the branch body as a rectangle (proportional to len)
        // Draw the branch body as a rectangle (proportional to len)

        // Draw the continuation
        drawBranch(xEnd, yEnd, length, angle, level - 1);

        // Flip a coin to decide if we should draw a sub-branch
        if (level % 2 == 0) {
            drawBranch(xEnd, yEnd, length * this->reduction, angle - this->deviation, level - 1);
        } else {
            drawBranch(xEnd, yEnd, length * this->reduction, angle + this->deviation, level - 2);
        }
    }

void drawRectangle(int x1, int y1, int x2, int y2, double length) {
    // Calculate the perpendicular vectors to the branch
    double angle = atan2(y2 - y1, x2 - x1);
    double dx = 0.1 * length * sin(angle);
    double dy = -0.1 * length * cos(angle);

    // Calculate the vertices of the rectangle
    std::vector<Point> vertices = {
        Point(x1 + static_cast<int>(dx), y1 + static_cast<int>(dy)),
        Point(x2 + static_cast<int>(dx), y2 + static_cast<int>(dy)),
        Point(x2 - static_cast<int>(dx), y2 - static_cast<int>(dy)),
        Point(x1 - static_cast<int>(dx), y1 - static_cast<int>(dy))
    };

    renderer.fillPolygon(vertices, 0xFFFF00); // Use your desired color
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
        lSystem.drawBranch(WIDTH / 2, HEIGHT - 10, 40.0, -M_PI / 2, 15);

        // Present the screen
        renderer.update();
    }

    return 0;
}

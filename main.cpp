#include "src/Renderer.hpp"
#include "src/Event.hpp"
#include <cmath>

class Crystal {
private:
    std::vector<Point> nucleationSites;
    std::vector<double> growthRates;
    std::vector<int> colors;
    std::vector<double> angles;
    std::vector<double> sizes;
    int maxNumberOfNucleationSites = 25;
    double impurity; // The impurity of the crystal tells how much variation in the growth rate there is
    double total_material; //total material in the medium (counter for the max size of the crystal)
public:
    Crystal() {
        this->impurity = 0;
        this->total_material = 1000000;
    }
    void addNucleationSite(Point p, double rate, int color, int size, double impurity ) {
        if (this->nucleationSites.size() >= this->maxNumberOfNucleationSites) {
            return;
        }
        this->nucleationSites.push_back(p);
        this->growthRates.push_back(rate);
        this->colors.push_back(color);
        this->sizes.push_back(size);
        this->angles.push_back(rand() % 360);
        this->impurity = impurity;
        this->total_material = total_material;
    }
    void grow() {
        for (int i = 0; i < (int)this->nucleationSites.size(); i++) {
            if (this->sizes[i] < this->total_material)
            {
                this->sizes[i] += this->growthRates[i];
                this->total_material -= this->growthRates[i];
                //recude growth rate based on material amount

                //Change color gradually based on time
                int r = (int)(sin(SDL_GetTicks() * 0.03) * 127 + 128) + 5;
                int g = (int)(sin(SDL_GetTicks() * 0.03) * 127 + 128) + 5;
                int b = (int)(sin(SDL_GetTicks() * 0.03) * 127 + 128) + 5;
                this->colors[i] = r << 16 | g << 8 | b;
            }
        
        }

    }
    void getNucleationSite(int index, int *x, int *y, int *rate, int *color, double *size) {
        *x = this->nucleationSites[index].x;
        *y = this->nucleationSites[index].y;
        *rate = this->growthRates[index];
        *color = this->colors[index];
        *size = this->sizes[index];
    }
    double getInpurity() {
        return this->impurity;
    }
    double getAngle(int index) {
        return this->angles[index];
    }
    int getNucleationSiteNumber() {
        return this->nucleationSites.size();
    }



};

int main() {
    Renderer renderer(nullptr, SDL_CreateWindow("SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0));
    Events events;

    Crystal crystal;
    Tex tex(renderer.getRenderer(), renderer.getWindow(), MAP_WIDTH, MAP_HEIGHT);
    crystal.addNucleationSite(Point(MAP_WIDTH/2, MAP_HEIGHT/2), 1.51, 0xFFFFFF, 1, 225);

    int color = 0x00FFF0;
    const int targetFPS = 60;
    const int frameDelay = 1000 / targetFPS;
    Uint32 frameStart;
    int frameTime;
    // Main loop
    while (!events.quit()) {
srand(time(NULL));
        double frameStart = SDL_GetTicks();

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
 
        // Draw a growing polygon (starting from a point x y and growing with time)
        int x;
        int y;
        int rate;
        double size;
        crystal.getNucleationSite(0, &x, &y, &rate, &color, &size);
        // Adjust the size based on the desired growth rate
        crystal.grow();

        for (int i = 0; i < crystal.getNucleationSiteNumber(); i++) {
            crystal.getNucleationSite(i, &x, &y, &rate, &color, &size);
            std::vector<Point> vertices;

            // Calculate the angles based on the stored angles vector
            for (int j = 0; j < 3; j++) {
                double angle = crystal.getAngle(i) + j * (2 * M_PI) / 3;
                double newX = x + size * cos(angle);
                double newY = y + size * sin(angle);
                vertices.push_back(Point(newX, newY));
            }

            tex.setNoise(crystal.getInpurity());
            renderer.fillPolygon([&tex](int x, int y, int color, SDL_Renderer* r, int __attribute__((unused))flag) {
                tex.setPixel(x, y, color, r, NO_OVERLAP | NOISE);
            }, vertices, color, NO_OVERLAP);
            
            //Add random nucleation sites along the edges of the polygon, probability based on number of nucleation sites qnd impurity
            if (rand() % 200 < crystal.getNucleationSiteNumber() * crystal.getInpurity() / 100) {
                //choose two random vertices
                Point p1 = vertices[rand() % 3];
                Point p2 = vertices[rand() % 3];
                if (p1.x == p2.x && p1.y == p2.y) {
                    p2 = vertices[(rand() + 1) % 3];
                }
                //choose a random point between the two vertices
                Point p3 = Point((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);

                //add a new nucleation site at 
                crystal.addNucleationSite(p3, 1.51, 0xFFFFFF, 1, 225);
            }

        }

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

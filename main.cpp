#include "src/Renderer.hpp"
#include "src/Event.hpp"
#include <cmath>
#include <math.h>

double clamp(double x, double a, double b) {
    return fmax(a, fmin(b, x));
}

class Crystal {
private:
    std::vector<Point> nucleationSites;
    std::vector<double> growthRates;
    std::vector<int> colors;
    std::vector<double> angles;
    std::vector<double> sizes;
    int maxNumberOfNucleationSites = 1000;
    int growthIterations = 0;
    double assymeetricity; // The assymetricity of the crystal tells how much variation in the growth rate there is in xy described in a 0-1 scale representing the percentage of the variation
    double impurity; // The impurity of the crystal tells how much variation in the growth rate there is
    double total_material = 100000;
    int seed;
public:
    Crystal() {
        this->impurity = 0;
        this->assymeetricity = 0.5;
    }
    void addNucleationSite(Point p, double rate, int color, int size, double impurity, int added_material = 0, int seed = 0) {
        if (this->nucleationSites.size() >= this->maxNumberOfNucleationSites) {
            if (rand() % 1000 > 10) {
                this->removeNucleationSite(rand() % this->nucleationSites.size());
            }
            return;
        }
        this->nucleationSites.push_back(p);
        this->growthRates.push_back(rate);
        this->colors.push_back(color);
        this->sizes.push_back(size);
        this->angles.push_back(rand() % 360);
        this->impurity = impurity;
        this->total_material += added_material;
        this->seed = seed;
        //sometime remove a random nucleation site

    }
    void removeNucleationSite(int index) {
        if (index < 0 || index >= (int)this->nucleationSites.size()) {
            return;
        }
        this->nucleationSites.erase(this->nucleationSites.begin() + index);
        this->growthRates.erase(this->growthRates.begin() + index);
        this->colors.erase(this->colors.begin() + index);
        this->sizes.erase(this->sizes.begin() + index);
        this->angles.erase(this->angles.begin() + index);
    }
    void grow() {
        this->growthIterations++;
        for (int i = 0; i < (int)this->nucleationSites.size(); i++) {
            if (this->sizes[i] < this->total_material)
            {
                this->sizes[i] += this->growthRates[i];
                this->total_material -= this->growthRates[i];
                if (this->total_material < 0) {
                    this->total_material = 0;
                    this->removeNucleationSite(i);
                }
                //recude growth rate based on material amount
                this->growthRates[i] *= this->total_material / 100000;
                //Change color gradually based on time
                int r = (int)(sin(SDL_GetTicks() * 0.001 + i) * 127 + 128);
                int g = (int)(sin(SDL_GetTicks() * 0.001 + i + 2) * 127 + 128);
                int b = (int)(sin(SDL_GetTicks() * 0.001 + i + 4) * 127 + 128);

                //sort the r g b values so its all the same color 

                if (r > g) {
                    std::swap(r, g);
                }
                if (g > b) {
                    std::swap(g, b);
                }
                if (r > g) {
                    std::swap(r, g);
                }
                // lower the saturation
/* new_R=int((1−abs(2×l−1))×s×clamp(1−abs(rgb_to_hls(255R​,255G​,255B​)[0]mod2−1),0,1)×255)
new_G=int((1−abs(2×l−1))×s×clamp(1−abs(rgb_to_hls(R255,G255,B255)[0]mod  2−1),0,1)×255)
new_G=int((1−abs(2×l−1))×s×clamp(1−abs(rgb_to_hls(255R​,255G​,255B​)[0]mod2−1),0,1)×255)
new_B=int((1−abs(2×l−1))×s×clamp(1−abs(rgb_to_hls(R255,G255,B255)[0]mod  2−1),0,1)×255)
new_B=int((1−abs(2×l−1))×s×clamp(1−abs(rgb_to_hls(255R​,255G​,255B​)[0]mod2−1),0,1)×255) */
               
                //(change if its r, g or b depending on the crystal seed)
                if (this->seed % 5 == 0) {
                    this->colors[i] = (r << 16) | (g << 8) | b;
                } else if (this->seed % 5 == 1) {
                    this->colors[i] = (g << 16) | (b << 8) | r;
                } else if (this->seed % 5 == 2) {
                    this->colors[i] = (b << 16) | (r << 8) | g;
                } else if (this->seed % 5 == 3) {
                    this->colors[i] = (r << 16) | (b << 8) | g;
                } else if (this->seed % 5 == 4) {
                    this->colors[i] = (g << 16) | (r << 8) | b;
            }
        
        }

    }
    }

    void getNucleationSite(int index, int *x, int *y, int *rate, int *color, double *size) {
        if (index < 0 || index >= (int)this->nucleationSites.size() || this->nucleationSites.size() == 0){
            return;
        }
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
    int getGrowthIterations() {
        return this->growthIterations;
    }
    void drawCrystal(Tex *tex, Renderer *renderer){
        int x;
        int y;
        int rate;
        double size;
        int color;
        for (int i = 0; i < this->nucleationSites.size(); i++) {
            this->getNucleationSite(i, &x, &y, &rate, &color, &size);
            std::vector<Point> vertices;

            // Calculate the angles based on the stored angles vector and asymetricity
            for (int j = 0; j < 3; j++) {
                double angle = this->getAngle(i) + j * (2 * M_PI) / 3;
                double newX = x + size * cos(angle);
                double newY = y + size * sin(angle);
                vertices.push_back(Point(newX, newY));
            }

            tex->setNoise(this->getInpurity());
            renderer->outlinePolygon([&tex](int x, int y, int color, SDL_Renderer* r, int __attribute__((unused))flagn) {
                tex->setPixel(x, y, color, r, NO_OVERLAP | NOISE);
            }, vertices, color, 0);

            if (rand() % 1000 > this->getNucleationSiteNumber() * 10) {
                //choose two random vertices
                int index1 = rand() % vertices.size();
                int index2 = rand() % vertices.size();
                while (index1 == index2) {
                    index2 = rand() % vertices.size();
                }
                Point p1 = vertices[index1];
                Point p2 = vertices[index2];
                //choose a random point between the two vertices (not the midpoint you dingus)
                Point p3 = Point((p1.x + p2.x) / 2 + (rand() % 100 - 50), (p1.y + p2.y) / 2 + (rand() % 100 - 50));
                //add a new nucleation site at 
                this->addNucleationSite(p3, 1.51, 0xFFFFFF, 1, 225, 0, this->seed);
            }
        }
    }


};

class Time {
    private:
        Uint32 startTicks;
        Uint32 pausedTicks;
        int frameDelay = 1000 / 120;
        bool paused;
        bool started;
    public:
        Time() {
            this->startTicks = 0;
            this->pausedTicks = 0;
            this->paused = false;
            this->started = false;
        }
        void start() {
            this->started = true;
            this->paused = false;
            this->startTicks = SDL_GetTicks();
            this->pausedTicks = 0;
        }
        void stop() {
            this->started = false;
            this->paused = false;
            this->startTicks = 0;
            this->pausedTicks = 0;
        }
        void pause() {
            if (this->started && !this->paused) {
                this->paused = true;
                this->pausedTicks = SDL_GetTicks() - this->startTicks;
                this->startTicks = 0;
            }
        }
        void unpause() {
            if (this->started && this->paused) {
                this->paused = false;
                this->startTicks = SDL_GetTicks() - this->pausedTicks;
                this->pausedTicks = 0;
            }
        }
        Uint32 getTicks() {
            Uint32 time = 0;
            if (this->started) {
                if (this->paused) {
                    time = this->pausedTicks;
                } else {
                    time = SDL_GetTicks() - this->startTicks;
                }
            }
            return time;
        }
        bool isStarted() {
            return this->started;
        }
        bool isPaused() {
            return this->paused;
        }
        void limit() {
            if (this->getTicks() < this->frameDelay) {
                SDL_Delay(this->frameDelay - this->getTicks());
            }
        }
        void setFrameDelay(int delay) {
            this->frameDelay = delay;
        }
};
     
int main() {
    Renderer renderer(nullptr, SDL_CreateWindow("SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0));
    Events events;
    Crystal crystal;

    Tex tex(renderer.getRenderer(), renderer.getWindow(), MAP_WIDTH, MAP_HEIGHT);
    // crystal.addNucleationSite(Point(MAP_WIDTH/2, MAP_HEIGHT/2), 1.51, 0xFFFFFF, 1, 225);


    // Frame rate
    Time frameTime;
    events.updateMousePosition();
    int targetMouseX = 0;
    int targetMouseY = 0;
    int general_tick = 0;
    int autoplace = 0;
    // Main loop
    while (!events.quit()) {
        general_tick++;
        // Start the frame time
        frameTime.start();
        // Clear the screen
        renderer.clear();
        // Update mouse position
        events.updateMousePosition();
        // Handle events
        while (events.poll()) {
            if (events.quit()) {
                break;
            }
            if (events.keyDown())
            {
                crystal = Crystal();
                tex.clear();
                renderer.clear();
            }
            if (events.getType() == SDL_MOUSEBUTTONDOWN) {
            if (!autoplace && events.mouseButtonPressed(SDL_BUTTON_LEFT)) {
                //remove all previous nucleation sites
                crystal = Crystal();
                //add a new nucleation site
                crystal.addNucleationSite(Point(events.getMouseX() - WIDTH / 2 + MAP_WIDTH / 2, events.getMouseY() - HEIGHT / 2 + MAP_HEIGHT / 2), 1.51, 0xFF00FF, 1, 225, 1000, rand() % 1000);
            }
            if (events.mouseButtonPressed(SDL_BUTTON_RIGHT)) {
                if (autoplace)
                    autoplace = 0;
                else
                    autoplace = 1;
            }
            }


        }
        // Generate random crystal every 20000 ticks
        if (autoplace && general_tick > 80 + rand() % 40 - 20)
        {
            printf("test %d", general_tick);
            crystal = Crystal();
            crystal.addNucleationSite(Point(rand()%WIDTH,rand()%HEIGHT), 1.51, 0xFF00FF, 1, 225, 1000, rand() % 1000);
            general_tick = 0;
        }

        // Adjust the size based on the desired growth rate
        crystal.grow();

        // Draw the crystal
        crystal.drawCrystal(&tex, &renderer);

        // Draw the texture
        renderer.drawTex(&tex, WIDTH / 2 - MAP_WIDTH / 2, HEIGHT / 2 - MAP_HEIGHT / 2);

        std::vector<Point> vertices;

        for (int lagrange = 0; lagrange < 10; ++lagrange) {
            targetMouseX += (events.getMouseX() - targetMouseX) * lagrange * 0.01;
            targetMouseY += (events.getMouseY() - targetMouseY) * lagrange * 0.01;
            double lagrangeFactor = lagrange * 0.1; // Adjust the factor as needed
            double lagrangeTime = SDL_GetTicks() * 0.001 + lagrangeFactor;

            int r = (int)(sin(lagrangeTime) * 127 + 128);
            int g = (int)(sin(lagrangeTime + 2) * 127 + 128);
            int b = (int)(sin(lagrangeTime + 4) * 127 + 128);
            if (autoplace) // convert to grayscale
            {
                int grayscale = (int)(0.299 * r + 0.587 * g + 0.114 * b);
                r = b = g = grayscale;
            }

            for (int i = 0; i < 3; i++) {
                double angle = (i * (2 * M_PI) / 3) + lagrangeTime;
                double newX = targetMouseX + 20 * cos(angle);
                double newY = targetMouseY + 20 * sin(angle);
                vertices.push_back(Point(newX, newY));
            }

            renderer.outlinePolygon([&renderer](int x, int y, int color, SDL_Renderer* r, int __attribute__((unused))flagn) {
                renderer.setPixel(x, y, color, r, NO_OVERLAP);
            }, vertices, (r << 16) | (g << 8) | b, 0);

            vertices.clear();
        }

        // Present the screen
        tex.update(renderer.getRenderer(), renderer.getWindow());
        renderer.update();

        // Limit the frame rate
        frameTime.limit();
    }
    return 0;
}

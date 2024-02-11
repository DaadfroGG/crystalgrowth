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
    int maxNumberOfNucleationSites = 1000;
    int growthIterations = 0;
    double assymeetricity; // The assymetricity of the crystal tells how much variation in the growth rate there is in xy described in a 0-1 scale representing the percentage of the variation
    double impurity; // The impurity of the crystal tells how much variation in the growth rate there is
    double total_material = 10000;
public:
    Crystal() {
        this->impurity = 0;
        this->assymeetricity = 0.5;
    }
    void addNucleationSite(Point p, double rate, int color, int size, double impurity, int added_material = 0) {
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

                //Change color gradually based on time
                int r = (int)(sin(SDL_GetTicks() * 0.001 + i) * 127 + 128);
                int g = (int)(sin(SDL_GetTicks() * 0.001 + i + 2) * 127 + 128);
                int b = (int)(sin(SDL_GetTicks() * 0.001 + i + 4) * 127 + 128);
                this->colors[i] = r << 16 | g << 8 | b;
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
                this->addNucleationSite(p3, 1.51, 0xFFFFFF, 1, 225);
            }
        }
    }


};

class Time {
    private:
        Uint32 startTicks;
        Uint32 pausedTicks;
        int frameDelay = 1000 / 60;
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
    // Main loop
    while (!events.quit()) {
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
            if (events.getType() == 1025) {
                //remove all previous nucleation sites
                crystal = Crystal();
                //add a new nucleation site
                crystal.addNucleationSite(Point(events.getMouseX() - WIDTH / 2 + MAP_WIDTH / 2, events.getMouseY() - HEIGHT / 2 + MAP_HEIGHT / 2), 1.51, 0xFF00FF, 1, 225, 1000);
            }
        }

        // Adjust the size based on the desired growth rate
        crystal.grow();

        // Draw the crystal
        crystal.drawCrystal(&tex, &renderer);

        // Draw the texture
        renderer.drawTex(&tex, WIDTH / 2 - MAP_WIDTH / 2, HEIGHT / 2 - MAP_HEIGHT / 2);

        // Draw the cursor as a triangle
        std::vector<Point> vertices;
        vertices.push_back(Point(events.getMouseX(), events.getMouseY() - 10));
        vertices.push_back(Point(events.getMouseX() + 10, events.getMouseY() + 10));
        vertices.push_back(Point(events.getMouseX() - 10, events.getMouseY() + 10));
        renderer.fillPolygon([&renderer](int x, int y, int color, SDL_Renderer* r, int __attribute__((unused))flag) {
            renderer.setPixel(x, y, color, r, NO_OVERLAP);
        }, vertices, 0x00FFFF, 0);

        // Present the screen
        tex.update(renderer.getRenderer(), renderer.getWindow());
        renderer.update();

        // Limit the frame rate
        frameTime.limit();
    }
    return 0;
}

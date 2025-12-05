#include <SDL2/SDL.h>
#include <vector>
#include <cstdlib>
#include <ctime>

const int GRID_WIDTH  = 200;
const int GRID_HEIGHT = 150;
const int CELL_SIZE   = 4;  

const int WINDOW_WIDTH  = GRID_WIDTH * CELL_SIZE;
const int WINDOW_HEIGHT = GRID_HEIGHT * CELL_SIZE;

enum CellType {
    EMPTY = 0,
    SAND  = 1
};

int main(int argc, char* argv[]) {
    (void)argc; (void)argv;

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL_Init Error: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Sand Falling Simulator",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        SDL_Log("SDL_CreateWindow Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!renderer) {
        SDL_Log("SDL_CreateRenderer Error: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    std::vector<std::vector<CellType>> grid(
        GRID_HEIGHT, std::vector<CellType>(GRID_WIDTH, EMPTY)
    );
    std::vector<std::vector<CellType>> newGrid = grid;

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        int mouseX, mouseY;
        Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
        if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
            int gx = mouseX / CELL_SIZE;
            int gy = mouseY / CELL_SIZE;
            if (gx >= 0 && gx < GRID_WIDTH && gy >= 0 && gy < GRID_HEIGHT) {
                grid[gy][gx] = SAND;
            }
        }

        newGrid = grid;

        for (int y = GRID_HEIGHT - 2; y >= 0; --y) { 
            for (int x = 0; x < GRID_WIDTH; ++x) {
                if (grid[y][x] == SAND) {
                    int belowY = y + 1;

                    if (belowY < GRID_HEIGHT && grid[belowY][x] == EMPTY && newGrid[belowY][x] == EMPTY) {
                        newGrid[belowY][x] = SAND;
                        newGrid[y][x] = EMPTY;
                    } else {
                        bool moved = false;

                        int dir = (std::rand() % 2 == 0) ? -1 : 1;
                        int nx = x + dir;
                        int ny = y + 1;

                        if (nx >= 0 && nx < GRID_WIDTH && ny < GRID_HEIGHT &&
                            grid[ny][nx] == EMPTY && newGrid[ny][nx] == EMPTY) {
                            newGrid[ny][nx] = SAND;
                            newGrid[y][x] = EMPTY;
                            moved = true;
                        } else {
                            dir = -dir;
                            nx = x + dir;
                            ny = y + 1;
                            if (nx >= 0 && nx < GRID_WIDTH && ny < GRID_HEIGHT &&
                                grid[ny][nx] == EMPTY && newGrid[ny][nx] == EMPTY) {
                                newGrid[ny][nx] = SAND;
                                newGrid[y][x] = EMPTY;
                                moved = true;
                            }
                        }

                        (void)moved; 
                    }
                }
            }
        }

        grid.swap(newGrid);

        SDL_SetRenderDrawColor(renderer, 10, 10, 20, 255);
        SDL_RenderClear(renderer);

        for (int y = 0; y < GRID_HEIGHT; ++y) {
            for (int x = 0; x < GRID_WIDTH; ++x) {
                if (grid[y][x] == SAND) {
                    SDL_SetRenderDrawColor(renderer, 230, 200, 80, 255);
                    SDL_Rect r;
                    r.x = x * CELL_SIZE;
                    r.y = y * CELL_SIZE;
                    r.w = CELL_SIZE;
                    r.h = CELL_SIZE;
                    SDL_RenderFillRect(renderer, &r);
                }
            }
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

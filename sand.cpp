#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <cstdlib>
#include <ctime>

const int GRID_WIDTH  = 200;
const int GRID_HEIGHT = 150;
const int CELL_SIZE   = 4;

const int UI_HEIGHT = 40;

const int WINDOW_WIDTH  = GRID_WIDTH * CELL_SIZE;
const int WINDOW_HEIGHT = UI_HEIGHT + GRID_HEIGHT * CELL_SIZE;

enum CellType {
    EMPTY = 0,
    SAND  = 1
};

enum Tool {
    TOOL_SAND = 0
};

const int BUTTON_X      = 10;
const int BUTTON_Y      = 5;
const int BUTTON_W      = 100;
const int BUTTON_H      = 30;

const int SAND_BUTTON_X = BUTTON_X + BUTTON_W + 10;
const int SAND_BUTTON_Y = 5;
const int SAND_BUTTON_W = 100;
const int SAND_BUTTON_H = 30;

const int EXIT_BUTTON_W = 100;
const int EXIT_BUTTON_H = 30;
const int EXIT_BUTTON_X = WINDOW_WIDTH - EXIT_BUTTON_W - 10;
const int EXIT_BUTTON_Y = 5;

bool isInsideButton(int mx, int my, int bx, int by, int bw, int bh) {
    return mx >= bx && mx <= bx + bw &&
           my >= by && my <= by + bh;
}

int main(int argc, char* argv[]) {
    (void)argc; (void)argv;

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL_Init Error: %s", SDL_GetError());
        return 1;
    }

    if (TTF_Init() != 0) {
        SDL_Log("TTF_Init Error: %s", TTF_GetError());
        SDL_Quit();
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
        TTF_Quit();
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
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    TTF_Font* font = TTF_OpenFont("OpenSans-Regular.ttf", 18);
    if (!font) {
        SDL_Log("TTF_OpenFont Error: %s", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    std::vector<std::vector<CellType>> grid(
        GRID_HEIGHT, std::vector<CellType>(GRID_WIDTH, EMPTY)
    );
    std::vector<std::vector<CellType>> newGrid = grid;

    bool running = true;
    SDL_Event event;

    Tool currentTool = TOOL_SAND; 

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        int mouseX, mouseY;
        Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);

        if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
            if (mouseY < UI_HEIGHT) {
                if (isInsideButton(mouseX, mouseY, BUTTON_X, BUTTON_Y, BUTTON_W, BUTTON_H)) {
                    for (int y = 0; y < GRID_HEIGHT; ++y)
                        for (int x = 0; x < GRID_WIDTH; ++x)
                            grid[y][x] = EMPTY;
                }

                if (isInsideButton(mouseX, mouseY,
                                   SAND_BUTTON_X, SAND_BUTTON_Y, SAND_BUTTON_W, SAND_BUTTON_H)) {
                    currentTool = TOOL_SAND;
                }

                if (isInsideButton(mouseX, mouseY,
                                   EXIT_BUTTON_X, EXIT_BUTTON_Y, EXIT_BUTTON_W, EXIT_BUTTON_H)) {
                    running = false;
                }

            } else {
                int gx = mouseX / CELL_SIZE;
                int gy = (mouseY - UI_HEIGHT) / CELL_SIZE;
                if (gx >= 0 && gx < GRID_WIDTH && gy >= 0 && gy < GRID_HEIGHT) {
                    if (currentTool == TOOL_SAND) {
                        grid[gy][gx] = SAND;
                    }
                }
            }
        }

        newGrid = grid;

        for (int y = GRID_HEIGHT - 2; y >= 0; --y) {
            for (int x = 0; x < GRID_WIDTH; ++x) {
                if (grid[y][x] == SAND) {
                    int belowY = y + 1;

                    if (belowY < GRID_HEIGHT &&
                        grid[belowY][x] == EMPTY &&
                        newGrid[belowY][x] == EMPTY) {
                        newGrid[belowY][x] = SAND;
                        newGrid[y][x] = EMPTY;
                    } else {
                        int dir = (std::rand() % 2 ? -1 : 1);
                        int nx = x + dir, ny = y + 1;

                        if (nx >= 0 && nx < GRID_WIDTH && ny < GRID_HEIGHT &&
                            grid[ny][nx] == EMPTY &&
                            newGrid[ny][nx] == EMPTY) {
                            newGrid[ny][nx] = SAND;
                            newGrid[y][x] = EMPTY;
                        } else {
                            dir = -dir;
                            nx = x + dir;
                            if (nx >= 0 && nx < GRID_WIDTH &&
                                grid[ny][nx] == EMPTY &&
                                newGrid[ny][nx] == EMPTY) {
                                newGrid[ny][nx] = SAND;
                                newGrid[y][x] = EMPTY;
                            }
                        }
                    }
                }
            }
        }

        grid.swap(newGrid);

        SDL_SetRenderDrawColor(renderer, 10, 10, 20, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 30, 30, 60, 255);
        SDL_Rect uiRect = {0, 0, WINDOW_WIDTH, UI_HEIGHT};
        SDL_RenderFillRect(renderer, &uiRect);

        SDL_Color white = {255, 255, 255, 255};

        SDL_SetRenderDrawColor(renderer, 200, 50, 50, 255);
        SDL_Rect restartRect = { BUTTON_X, BUTTON_Y, BUTTON_W, BUTTON_H };
        SDL_RenderFillRect(renderer, &restartRect);

        SDL_Surface* txtRestart = TTF_RenderText_Blended(font, "Restart", white);
        if (txtRestart) {
            SDL_Texture* texRestart = SDL_CreateTextureFromSurface(renderer, txtRestart);
            if (texRestart) {
                int textW = txtRestart->w;
                int textH = txtRestart->h;
                SDL_Rect restartTextRect = {
                    BUTTON_X + (BUTTON_W - textW) / 2,
                    BUTTON_Y + (BUTTON_H - textH) / 2,
                    textW, textH
                };
                SDL_RenderCopy(renderer, texRestart, NULL, &restartTextRect);
                SDL_DestroyTexture(texRestart);
            }
            SDL_FreeSurface(txtRestart);
        }

        SDL_SetRenderDrawColor(renderer, 200, 180, 60, 255);
        SDL_Rect sandRect = { SAND_BUTTON_X, SAND_BUTTON_Y, SAND_BUTTON_W, SAND_BUTTON_H };
        SDL_RenderFillRect(renderer, &sandRect);

        SDL_Surface* txtSand = TTF_RenderText_Blended(font, "Sand", white);
        if (txtSand) {
            SDL_Texture* texSand = SDL_CreateTextureFromSurface(renderer, txtSand);
            if (texSand) {
                int textW = txtSand->w;
                int textH = txtSand->h;
                SDL_Rect sandTextRect = {
                    SAND_BUTTON_X + (SAND_BUTTON_W - textW) / 2,
                    SAND_BUTTON_Y + (SAND_BUTTON_H - textH) / 2,
                    textW, textH
                };
                SDL_RenderCopy(renderer, texSand, NULL, &sandTextRect);
                SDL_DestroyTexture(texSand);
            }
            SDL_FreeSurface(txtSand);
        }

        SDL_SetRenderDrawColor(renderer, 80, 80, 200, 255);
        SDL_Rect exitRect = { EXIT_BUTTON_X, EXIT_BUTTON_Y, EXIT_BUTTON_W, EXIT_BUTTON_H };
        SDL_RenderFillRect(renderer, &exitRect);

        SDL_Surface* txtExit = TTF_RenderText_Blended(font, "Exit", white);
        if (txtExit) {
            SDL_Texture* texExit = SDL_CreateTextureFromSurface(renderer, txtExit);
            if (texExit) {
                int textW = txtExit->w;
                int textH = txtExit->h;
                SDL_Rect exitTextRect = {
                    EXIT_BUTTON_X + (EXIT_BUTTON_W - textW) / 2,
                    EXIT_BUTTON_Y + (EXIT_BUTTON_H - textH) / 2,
                    textW, textH
                };
                SDL_RenderCopy(renderer, texExit, NULL, &exitTextRect);
                SDL_DestroyTexture(texExit);
            }
            SDL_FreeSurface(txtExit);
        }

        for (int y = 0; y < GRID_HEIGHT; ++y) {
            for (int x = 0; x < GRID_WIDTH; ++x) {
                if (grid[y][x] == SAND) {
                    SDL_SetRenderDrawColor(renderer, 230, 200, 80, 255);
                    SDL_Rect r = { x * CELL_SIZE, UI_HEIGHT + y * CELL_SIZE,
                                   CELL_SIZE, CELL_SIZE };
                    SDL_RenderFillRect(renderer, &r);
                }
            }
        }

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}

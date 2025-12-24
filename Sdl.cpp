/**
 * @file Sdl.cpp
 * @brief Implémentation de la classe Sdl
 */

#include "Sdl.hpp"
#include <limits>
#include <cstring>

/**
 * @brief Constructeur initialisant SDL2, la fenêtre et les tampons
 */
Sdl::Sdl(const std::string& title, int w, int h)
    : window(nullptr), renderer(nullptr), texture(nullptr),
      pixelBuffer(nullptr), depthBuffer(nullptr), width(w), height(h) {
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return;
    }

    window = SDL_CreateWindow(title.c_str(),
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              width, height,
                              SDL_WINDOW_SHOWN);
    if (!window) {
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        return;
    }

    texture = SDL_CreateTexture(renderer,
                                SDL_PIXELFORMAT_ARGB8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                width, height);
    if (!texture) {
        return;
    }

    pixelBuffer = new uint32_t[width * height];
    depthBuffer = new float[width * height];
}

/**
 * @brief Destructeur libérant les ressources SDL
 */
Sdl::~Sdl() {
    delete[] pixelBuffer;
    delete[] depthBuffer;
    if (texture) SDL_DestroyTexture(texture);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

/**
 * @brief Efface les tampons avec la couleur spécifiée
 */
void Sdl::clear(uint8_t r, uint8_t g, uint8_t b) {
    uint32_t color = (255 << 24) | (r << 16) | (g << 8) | b;
    for (int i = 0; i < width * height; ++i) {
        pixelBuffer[i] = color;
        depthBuffer[i] = std::numeric_limits<float>::max();
    }
}

/**
 * @brief Dessine un pixel avec profondeur
 */
void Sdl::setPixel(int x, int y, float depth, uint8_t r, uint8_t g, uint8_t b) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return;
    }
    
    int index = y * width + x;
    if (depth < depthBuffer[index]) {
        depthBuffer[index] = depth;
        pixelBuffer[index] = (255 << 24) | (r << 16) | (g << 8) | b;
    }
}

/**
 * @brief Affiche le contenu du tampon à l'écran
 */
void Sdl::present() {
    SDL_UpdateTexture(texture, nullptr, pixelBuffer, width * sizeof(uint32_t));
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

int Sdl::getWidth() const {
    return width;
}

int Sdl::getHeight() const {
    return height;
}

bool Sdl::isValid() const {
    return window && renderer && texture && pixelBuffer && depthBuffer;
}

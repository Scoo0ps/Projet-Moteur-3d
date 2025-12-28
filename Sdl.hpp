/**
 * @file Sdl.hpp
 * @brief Encapsulation de la bibliothèque SDL2
 */

#ifndef SDL_HPP
#define SDL_HPP

#include <SDL.h>
#include <string>
#include <cstdint>

/**
 * @class Sdl
 * @brief Classe contenant les fonctionnalités SDL pour le rendu graphique
 */
class Sdl {
private:
    SDL_Window* window; ///< Pointeur vers la fenêtre SDL
    SDL_Renderer* renderer; ///< Pointeur vers le renderer SDL
    SDL_Texture* texture; ///< Texture pour le rendu pixel par pixel
    uint32_t* pixelBuffer; ///< Tampon de pixels
    float* depthBuffer; ///< Tampon de profondeur 
    int width; ///< Largeur de la fenêtre
    int height; ///< Hauteur de la fenêtre

public:
    /**
     * @brief Constructeur de la classe Sdl
     * @param title Titre de la fenêtre
     * @param w Largeur de la fenêtre
     * @param h Hauteur de la fenêtre
     */
    Sdl(const std::string& title, int w, int h);

    /**
     * @brief Destructeur de la classe Sdl
     */
    ~Sdl();

    /**
     * @brief Efface le tampon de pixels avec une couleur
     * @param r Composante rouge (0-255)
     * @param g Composante verte (0-255)
     * @param b Composante bleue (0-255)
     */
    void clear(uint8_t r, uint8_t g, uint8_t b);

    /**
     * @brief Dessine un pixel dans le tampon
     * @param x Coordonnée X
     * @param y Coordonnée Y
     * @param depth Profondeur du pixel
     * @param r Composante rouge
     * @param g Composante verte
     * @param b Composante bleue
     */
    void setPixel(int x, int y, float depth, uint8_t r, uint8_t g, uint8_t b);

    /**
     * @brief Présente le contenu du tampon à l'écran
     */
    void present();

    /**
     * @brief Retourne la largeur de la fenêtre
     * @return Largeur en pixels
     */
    int getWidth() const;

    /**
     * @brief Retourne la hauteur de la fenêtre
     * @return Hauteur en pixels
     */
    int getHeight() const;

    /**
     * @brief Vérifie si SDL a été initialisé correctement
     * @return true si l'initialisation est réussie
     */
    bool isValid() const;
};

#endif 

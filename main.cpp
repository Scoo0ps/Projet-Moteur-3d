/**
 * @file main.cpp
 * @brief Boucle principale et gestion des événements clavier
 */

#include "Sdl.hpp"
#include "Geometry.hpp"
#include "Scene.hpp"
#include <cmath>
#include <iostream>

using namespace Engine3D;

const int WINDOW_WIDTH = 1000; ///< Largeur de la fenêtre en pixels

const int WINDOW_HEIGHT = 650; ///< Hauteur de la fenêtre en pixels

const float CAMERA_SPEED = 0.15f; ///< Vitesse de déplacement de la caméra

const float ROTATION_SPEED = 0.02f; ///< Vitesse de rotation du cube

/**
 * @brief Gère les événements clavier
 * @param camera instance actuelle de Camera
 * @param keys État des touches (géré par SDL)
 */
void handleKeyboard(Camera& camera, const Uint8* keys) {
    if (keys[SDL_SCANCODE_UP]) {
        camera.moveForward(CAMERA_SPEED);
    }
    if (keys[SDL_SCANCODE_DOWN]) {
        camera.moveForward(-CAMERA_SPEED);
    }
    if (keys[SDL_SCANCODE_LEFT]) {
        camera.moveLeft(-CAMERA_SPEED);
    }
    if (keys[SDL_SCANCODE_RIGHT]) {
        camera.moveRight(-CAMERA_SPEED);
    }
    if (keys[SDL_SCANCODE_SPACE]) {
        camera.moveUp(CAMERA_SPEED);
    }
    if (keys[SDL_SCANCODE_LCTRL]) {
        camera.moveUp(-CAMERA_SPEED);
    }
}

/**
 * @brief Crée et ajoute le cube à la scène
 * @param scene 
 * @param angle Angle de rotation actuel
 */
void addCube(Scene& scene, float angle) {
    Pave3d cube(Point3d(-2.5f, -1.0f, 2.0f), 1.5f, 1.5f, 1.5f);
    cube.setRotation(angle * 0.7f, angle, angle * 0.3f);
    scene.addTriangles(cube.getTriangles(), 0, 0, 255);
}

/**
 * @brief Crée et ajoute la sphère à la scène
 * @param scene instance actuelle de Scene
 */
void addSphere(Scene& scene) {
    Sphere3d sphere(Point3d(2.5f, 1.0f, 6.0f), 1.0f, 16, 12);
    scene.addTriangles(sphere.getTriangles(), 255, 0, 0);
}

/**
 * @brief Boucle principale du programme
 * @param sdl instance actuelle de Sdl
 * @param scene instance actuelle de Scene
 * @return le code de sortie (0 si succès)
 */
int mainLoop(Sdl& sdl, Scene& scene) {
    bool running = true;
    SDL_Event event;
    float rotationAngle = 0.0f;
    
    while (running) {
        // Gestion des événements
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
            }
        }
        
        // Gestion du clavier en continu
        const Uint8* keys = SDL_GetKeyboardState(nullptr);
        handleKeyboard(scene.getCamera(), keys);
        
        // Mise à jour de la rotation
        rotationAngle += ROTATION_SPEED;
        
        // Construction de la scène
        scene.clearTriangles();
        addCube(scene, rotationAngle);
        addSphere(scene);
        
        // Rendu
        scene.render();
        
        // Limiter à environ 60 images par seconde
        SDL_Delay(16);
    }
    
    return 0;
}

/**
 * @brief main
 * @param argc 
 * @param argv 
 * @return le code de sortie (0 si succès)
 */
int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    
    // Initialisation SDL
    Sdl sdl("Moteur 3D - PRAP", WINDOW_WIDTH, WINDOW_HEIGHT);
    
    if (!sdl.isValid()) {
        std::cerr << "Erreur d'initialisation SDL" << std::endl;
        return 1;
    }

    std::cout << "Démarrage du Moteur 3D" << std::endl;
    std::cout << "Flèches : Deplacer la camera" << std::endl;
    std::cout << "Espace : Monter" << std::endl;
    std::cout << "Controle Gauche : Descendre" << std::endl;
    std::cout << "Echap : Quitter" << std::endl;
    
    // Création de la scène
    Scene scene(sdl);
    
    // Configuration initiale de la caméra
    Camera& camera = scene.getCamera();
    camera.position = Point3d(0.0f, 0.0f, -2.0f);
    camera.direction = Point3d(0.0f, 0.0f, 1.0f);
    
    return mainLoop(sdl, scene);
}

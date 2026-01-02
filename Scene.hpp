/**
 * @file Scene.hpp
 * @brief Gestion de la scène, caméra et rasterisation
 */

#ifndef SCENE_HPP
#define SCENE_HPP

#include "Geometry.hpp"
#include "Sdl.hpp"
#include <vector>

namespace Engine3D {

/**
 * @class Camera
 * @brief Représente la caméra (œil) dans la scène
 */
class Camera {
public:
    Point3d position; ///< Position de l'oeil
    Point3d direction; ///< Direction de l'oeil
    Point3d up; ///< Vecteur haut
    float fov; ///< Champ de vision (field of view)
    float nearPlane; ///< Plan de projection proche

    /**
     * @brief Constructeur par défaut
     */
    Camera();

    /**
     * @brief Déplace la caméra vers l'avant/arrière
     */
    void moveForward(float delta);

    /**
     * @brief Déplace la caméra vers la droite
     */
    void moveRight(float delta);

    /**
     * @brief Déplace la caméra vers la gauche
     */
    void moveLeft(float delta);

    /**
     * @brief Déplace la caméra vers le haut/bas
     */
    void moveUp(float delta);
};

/**
 * @struct ColoredTriangle
 * @brief Triangle avec sa couleur associée
 */
struct ColoredTriangle {
    Triangle3d triangle; ///< Le triangle 3D
    uint8_t r; ///< Composante rouge
    uint8_t g; ///< Composante verte
    uint8_t b; ///< Composante bleue
};

/**
 * @class Scene
 * @ingroup Engine3DClasses
 * @brief Gère le rendu de la scène 3D
 */
class Scene {
private:
    Sdl& sdl; ///< Référence vers SDL
    Camera camera; ///< Caméra de la scène
    std::vector<ColoredTriangle> triangles; ///< Triangles colorés à render

public:
    /**
     * @brief Constructeur
     * @param sdl Référence vers l'objet Sdl
     */
    Scene(Sdl& sdl);

    /**
     * @brief Ajoute des triangles à la scène
     */
    void addTriangles(const std::vector<Triangle3d>& tris,
                      uint8_t r, uint8_t g, uint8_t b);

    /**
     * @brief Efface les triangles de la scène
     */
    void clearTriangles();

    /**
     * @brief Effectue le rendu de la scène
     */
    void render();

    /**
     * @brief Retourne une référence vers la caméra
     */
    Camera& getCamera();

private:
    /**
     * @brief Projette un point 3D en point 2D
     */
    Point2d project(const Point3d& p) const;

    /**
     * @brief Vérifie si un triangle est visible
     */
    bool isTriangleVisible(const Triangle3d& triangle) const;

    /**
     * @brief Rasterise un triangle 2D
     */
    void rasterizeTriangle(const Triangle2d& triangle, float avgDepth,
                           uint8_t r, uint8_t g, uint8_t b);

    /**
     * @brief Dessine une ligne horizontale du triangle via technique scanline
     */
    void drawScanline(int y, int x1, int x2, float depth,
                      uint8_t r, uint8_t g, uint8_t b);

    /**
     * @brief Rasterise la partie plate du triangle (haut ou bas)
     */
    void fillFlatBottom(const Point2d& p1, const Point2d& p2, 
                        const Point2d& p3, float depth,
                        uint8_t r, uint8_t g, uint8_t b);

    /**
     * @brief Rasterise la partie plate supérieure du triangle
     */
    void fillFlatTop(const Point2d& p1, const Point2d& p2, 
                     const Point2d& p3, float depth,
                     uint8_t r, uint8_t g, uint8_t b);
};

} 

#endif 

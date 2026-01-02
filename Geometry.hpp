/**
 * @file Geometry.hpp
 * @brief Définition des classes géométriques
 */

#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <vector>
#include <cmath>

/**
 * @namespace Engine3D
 * @brief Espace de noms contenant toutes les classes du moteur 3D
 */
namespace Engine3D {

/**
 * @class Point3d
 * @ingroup Engine3DClasses
 * @brief Représente un point dans l'espace 3D
 */
class Point3d {
public:
    float x; ///< Coordonnée X
    float y; ///< Coordonnée Y
    float z; ///< Coordonnée Z

    /**
     * @brief Constructeur par défaut
     */
    Point3d();

    /**
     * @brief Constructeur avec coordonnées
     * @param x 
     * @param y 
     * @param z 
     */
    Point3d(float x, float y, float z);

    /**
     * @brief Soustraction de deux points (retourne un vecteur)
     */
    Point3d operator-(const Point3d& other) const;

    /**
     * @brief Addition de deux points
     */
    Point3d operator+(const Point3d& other) const;

    /**
     * @brief Multiplication par un scalaire
     */
    Point3d operator*(float scalar) const;

    /**
     * @brief Produit vectoriel
     */
    Point3d cross(const Point3d& other) const;

    /**
     * @brief Produit scalaire
     */
    float dot(const Point3d& other) const;

    /**
     * @brief Normalise le vecteur
     */
    Point3d normalize() const;
};

/**
 * @class Point2d
 * @ingroup Engine3DClasses
 * @brief Représente un point sur l'écran 2D
 */
class Point2d {
public:
    int x; ///< Coordonnée X mais sur l'écran (projetée)
    int y; ///< Coordonnée Y mais sur l'écran (projetée)
    float z; ///< Profondeur

    /**
     * @brief Constructeur par défaut
     */
    Point2d();

    /**
     * @brief Constructeur avec coordonnées
     * @param x 
     * @param y 
     * @param z 
     */
    Point2d(int x, int y, float z);
};

/**
 * @class Triangle3d
 * @ingroup Engine3DClasses
 * @brief Triangle dans l'espace 3D
 */
class Triangle3d {
public:
    Point3d p1; ///< Premier sommet
    Point3d p2; ///< Deuxième sommet
    Point3d p3; ///< Troisième sommet

    /**
     * @brief Constructeur par défaut
     */
    Triangle3d();

    /**
     * @brief Constructeur avec trois points (sommets)
     * @param p1 
     * @param p2 
     * @param p3 
     */
    Triangle3d(const Point3d& p1, const Point3d& p2, const Point3d& p3);

    /**
     * @brief Calcule la normale du triangle
     */
    Point3d normal() const;

    /**
     * @brief Calcule le centre du triangle
     */
    Point3d center() const;
};

/**
 * @class Triangle2d
 * @ingroup Engine3DClasses
 * @brief Triangle projeté sur l'écran 2D
 */
class Triangle2d {
public:
    Point2d p1; ///< Premier sommet
    Point2d p2; ///< Deuxième sommet
    Point2d p3; ///< Troisième sommet

    /**
     * @brief Constructeur par défaut
     */
    Triangle2d();

    /**
     * @brief Constructeur avec trois points (sommets)
     * @param p1
     * @param p2
     * @param p3
     */
    Triangle2d(const Point2d& p1, const Point2d& p2, const Point2d& p3);
};

/**
 * @class Quad3d
 * @ingroup Engine3DClasses
 * @brief Quadrilatère composé de deux triangles
 */
class Quad3d {
public:
    Triangle3d t1; ///< Premier triangle
    Triangle3d t2; ///< Deuxième triangle

    /**
     * @brief Constructeur par défaut
     */
    Quad3d();

    /**
     * @brief Constructeur avec quatre points (dans le sens trigonométrique)
     * @param p1 
     * @param p2 
     * @param p3 
     * @param p4 
     */
    Quad3d(const Point3d& p1, const Point3d& p2, 
           const Point3d& p3, const Point3d& p4);

    /**
     * @brief Retourne les triangles du quad
     */
    std::vector<Triangle3d> getTriangles() const;
};

/**
 * @class Pave3d
 * @ingroup Engine3DClasses
 * @brief Pavé composé de 6 faces (Quad3d)
 */
class Pave3d {
private:
    Point3d center; ///< Centre du pavé
    float sizeX; ///< Taille en X (largeur)
    float sizeY; ///< Taille en Y (hauteur)
    float sizeZ; ///< Taille en Z (profondeur)
    float angleX; ///< Angle de rotation X
    float angleY; ///< Angle de rotation Y
    float angleZ; ///< Angle de rotation Z

public:
    /**
     * @brief Constructeur avec paramètres
     * @param center 
     * @param sizeX 
     * @param sizeY 
     * @param sizeZ 
     */
    Pave3d(const Point3d& center, float sizeX, float sizeY, float sizeZ);

    /**
     * @brief Définit les angles de rotation
     */
    void setRotation(float angleX, float angleY, float angleZ);

    /**
     * @brief Retourne tous les triangles du pavé
     */
    std::vector<Triangle3d> getTriangles() const;

private:
    /**
     * @brief Applique la rotation à un point
     */
    Point3d rotatePoint(const Point3d& p) const;
};

/**
 * @class Sphere3d
 * @ingroup Engine3DClasses
 * @brief Sphère composée d'un maillage de Quad3d
 */
class Sphere3d {
private:
    Point3d center; ///< Centre de la sphère
    float radius; ///< Rayon de la sphère
    int meridians; ///< Nombre de méridiens
    int parallels; ///< Nombre de parallèles

public:
    /**
     * @brief Constructeur
     * @param center
     * @param radius
     * @param meridians
     * @param parallels
     */
    Sphere3d(const Point3d& center, float radius, int meridians, int parallels);

    /**
     * @brief Retourne tous les triangles de la sphère
     */
    std::vector<Triangle3d> getTriangles() const;

private:
    /**
     * @brief Calcule un point sur la sphère
     */
    Point3d spherePoint(float theta, float phi) const;
};

} 

#endif 

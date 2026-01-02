/**
 * @file Geometry.cpp
 * @brief Implémentation des classes géométriques
 */

#include "Geometry.hpp"

namespace Engine3D {

Point3d::Point3d() : x(0), y(0), z(0) {}

Point3d::Point3d(float x, float y, float z) : x(x), y(y), z(z) {}

Point3d Point3d::operator-(const Point3d& other) const {
    return Point3d(x - other.x, y - other.y, z - other.z);
}

Point3d Point3d::operator+(const Point3d& other) const {
    return Point3d(x + other.x, y + other.y, z + other.z);
}

Point3d Point3d::operator*(float scalar) const {
    return Point3d(x * scalar, y * scalar, z * scalar);
}

Point3d Point3d::cross(const Point3d& other) const {
    return Point3d(
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    );
}

float Point3d::dot(const Point3d& other) const {
    return x * other.x + y * other.y + z * other.z;
}

Point3d Point3d::normalize() const {
    float len = std::sqrt(x * x + y * y + z * z);
    if (len > 0.0001f) {
        return Point3d(x / len, y / len, z / len);
    }
    return *this;
}


Point2d::Point2d() : x(0), y(0), z(0) {}

Point2d::Point2d(int x, int y, float z) : x(x), y(y), z(z) {}


Triangle3d::Triangle3d() {}

Triangle3d::Triangle3d(const Point3d& p1, const Point3d& p2, const Point3d& p3)
    : p1(p1), p2(p2), p3(p3) {}

Point3d Triangle3d::normal() const {
    Point3d v1 = p2 - p1;
    Point3d v2 = p3 - p1;
    return v1.cross(v2).normalize();
}

Point3d Triangle3d::center() const {
    return Point3d(
        (p1.x + p2.x + p3.x) / 3.0f,
        (p1.y + p2.y + p3.y) / 3.0f,
        (p1.z + p2.z + p3.z) / 3.0f
    );
}


Triangle2d::Triangle2d() {}

Triangle2d::Triangle2d(const Point2d& p1, const Point2d& p2, const Point2d& p3)
    : p1(p1), p2(p2), p3(p3) {}



Quad3d::Quad3d() {}

Quad3d::Quad3d(const Point3d& p1, const Point3d& p2, 
               const Point3d& p3, const Point3d& p4) {
    // Deux triangles en sens trigonométrique
    t1 = Triangle3d(p1, p2, p3);
    t2 = Triangle3d(p1, p3, p4);
}

std::vector<Triangle3d> Quad3d::getTriangles() const {
    std::vector<Triangle3d> triangles;
    triangles.push_back(t1);
    triangles.push_back(t2);
    return triangles;
}

Pave3d::Pave3d(const Point3d& center, float sx, float sy, float sz)
    : center(center), sizeX(sx), sizeY(sy), sizeZ(sz),
      angleX(0), angleY(0), angleZ(0) {}

void Pave3d::setRotation(float ax, float ay, float az) {
    angleX = ax;
    angleY = ay;
    angleZ = az;
}

Point3d Pave3d::rotatePoint(const Point3d& p) const {
    // Rotation X
    float cosX = std::cos(angleX);
    float sinX = std::sin(angleX);
    float y1 = p.y * cosX - p.z * sinX;
    float z1 = p.y * sinX + p.z * cosX;
    
    // Rotation Y
    float cosY = std::cos(angleY);
    float sinY = std::sin(angleY);
    float x2 = p.x * cosY + z1 * sinY;
    float z2 = -p.x * sinY + z1 * cosY;
    
    // Rotation Z
    float cosZ = std::cos(angleZ);
    float sinZ = std::sin(angleZ);
    float x3 = x2 * cosZ - y1 * sinZ;
    float y3 = x2 * sinZ + y1 * cosZ;
    
    return Point3d(x3 + center.x, y3 + center.y, z2 + center.z);
}

std::vector<Triangle3d> Pave3d::getTriangles() const {
    std::vector<Triangle3d> triangles;
    float hx = sizeX / 2.0f;
    float hy = sizeY / 2.0f;
    float hz = sizeZ / 2.0f;

    // 8 sommets du cube (avant rotation)
    Point3d v[8] = {
        Point3d(-hx, -hy, -hz), Point3d(hx, -hy, -hz),
        Point3d(hx, hy, -hz),   Point3d(-hx, hy, -hz),
        Point3d(-hx, -hy, hz),  Point3d(hx, -hy, hz),
        Point3d(hx, hy, hz),    Point3d(-hx, hy, hz)
    };

    // Appliquer la rotation à tous les sommets
    for (int i = 0; i < 8; ++i) {
        v[i] = rotatePoint(v[i]);
    }

    // 6 faces (sens trigonométrique vu de l'extérieur)
    Quad3d faces[6] = {
        Quad3d(v[0], v[1], v[2], v[3]),  // Face arrière
        Quad3d(v[5], v[4], v[7], v[6]),  // Face avant
        Quad3d(v[4], v[0], v[3], v[7]),  // Face gauche
        Quad3d(v[1], v[5], v[6], v[2]),  // Face droite
        Quad3d(v[4], v[5], v[1], v[0]),  // Face bas
        Quad3d(v[3], v[2], v[6], v[7])   // Face haut
    };

    for (int i = 0; i < 6; ++i) {
    std::vector<Triangle3d> tris = faces[i].getTriangles();
    triangles.insert(triangles.end(), tris.begin(), tris.end());
    }

    return triangles;
}

Sphere3d::Sphere3d(const Point3d& center, float radius, int meridians, int parallels)
    : center(center), radius(radius), meridians(meridians), parallels(parallels) {}

Point3d Sphere3d::spherePoint(float theta, float phi) const {
    float x = radius * std::sin(phi) * std::cos(theta);
    float y = radius * std::cos(phi);
    float z = radius * std::sin(phi) * std::sin(theta);
    return Point3d(center.x + x, center.y + y, center.z + z);
}

std::vector<Triangle3d> Sphere3d::getTriangles() const {
    std::vector<Triangle3d> triangles;
    const float PI = 3.14159265359f;

    for (int i = 0; i < meridians; ++i) {
        float theta1 = 2.0f * PI * i / meridians;
        float theta2 = 2.0f * PI * (i + 1) / meridians;

        for (int j = 0; j < parallels; ++j) {
            float phi1 = PI * j / parallels;
            float phi2 = PI * (j + 1) / parallels;

            Point3d p1 = spherePoint(theta1, phi1);
            Point3d p2 = spherePoint(theta2, phi1);
            Point3d p3 = spherePoint(theta2, phi2);
            Point3d p4 = spherePoint(theta1, phi2);

            // Créer un quad et récupérer ses triangles
            Quad3d quad(p1, p2, p3, p4);
            std::vector<Triangle3d> tris = quad.getTriangles();
            triangles.insert(triangles.end(), tris.begin(), tris.end());
        }
    }

    return triangles;
}

} // namespace Engine3D

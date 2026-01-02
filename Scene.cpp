/**
 * @file Scene.cpp
 * @brief Implémentation de la gestion de scène et rasterisation
 */

#include "Scene.hpp"
#include <algorithm>
#include <cmath>

namespace Engine3D {

Camera::Camera()
    : position(0, 0, -5), direction(0, 0, 1), up(0, 1, 0),
      fov(60.0f), nearPlane(0.1f) {}

void Camera::moveForward(float delta) {
    position.x += direction.x * delta;
    position.y += direction.y * delta;
    position.z += direction.z * delta;
}

void Camera::moveRight(float delta) {
    Point3d right = direction.cross(up).normalize();
    position.x += right.x * delta;
    position.y += right.y * delta;
    position.z += right.z * delta;
}

void Camera::moveLeft(float delta) {
    Point3d right = direction.cross(up).normalize();
    position.x -= right.x * delta;
    position.y -= right.y * delta;
    position.z -= right.z * delta;
}

void Camera::moveUp(float delta) {
    position.y += delta;
}


Scene::Scene(Sdl& sdl) 
    : sdl(sdl) {}

void Scene::addTriangles(const std::vector<Triangle3d>& tris,
                         uint8_t r, uint8_t g, uint8_t b) {
    for (const auto& tri : tris) {
        ColoredTriangle ct;
        ct.triangle = tri;
        ct.r = r;
        ct.g = g;
        ct.b = b;
        triangles.push_back(ct);
    }
}

void Scene::clearTriangles() {
    triangles.clear();
}

Camera& Scene::getCamera() {
    return camera;
}


Point2d Scene::project(const Point3d& p) const {
    // Transformation vers l'espace caméra
    Point3d rel = p - camera.position;
    
    // Projection perspective simple
    float fovRad = camera.fov * 3.14159f / 180.0f;
    float scale = 1.0f / std::tan(fovRad / 2.0f);
    
    float aspect = (float)sdl.getWidth() / sdl.getHeight();
    
    // Éviter la division par zéro
    float z = rel.z;
    if (z < camera.nearPlane) {
        z = camera.nearPlane;
    }
    
    float projX = (rel.x * scale / z) / aspect;
    float projY = (rel.y * scale / z);
    
    // Conversion en coordonnées écran
    int screenX = (int)((projX + 1.0f) * 0.5f * sdl.getWidth());
    int screenY = (int)((1.0f - projY) * 0.5f * sdl.getHeight());
    
    return Point2d(screenX, screenY, z);
}

bool Scene::isTriangleVisible(const Triangle3d& tri) const {
    Point3d normal = tri.normal();
    Point3d toCamera = camera.position - tri.center();
    
    // Le triangle est visible si sa normale pointe vers la caméra
    return normal.dot(toCamera) > 0;
}

void Scene::drawScanline(int y, int x1, int x2, float depth,
                         uint8_t r, uint8_t g, uint8_t b) {
    if (x1 > x2) std::swap(x1, x2);
    
    for (int x = x1; x <= x2; ++x) {
        sdl.setPixel(x, y, depth, r, g, b);
    }
}

void Scene::fillFlatBottom(const Point2d& p1, const Point2d& p2, 
                           const Point2d& p3, float depth,
                           uint8_t r, uint8_t g, uint8_t b) {
    // p1 est le sommet supérieur, p2 et p3 sont sur la ligne du bas
    float invSlope1 = (float)(p2.x - p1.x) / (p2.y - p1.y);
    float invSlope2 = (float)(p3.x - p1.x) / (p3.y - p1.y);
    
    float curX1 = (float)p1.x;
    float curX2 = (float)p1.x;
    
    for (int scanY = p1.y; scanY <= p2.y; ++scanY) {
        drawScanline(scanY, (int)curX1, (int)curX2, depth, r, g, b);
        curX1 += invSlope1;
        curX2 += invSlope2;
    }
}

void Scene::fillFlatTop(const Point2d& p1, const Point2d& p2, 
                        const Point2d& p3, float depth,
                        uint8_t r, uint8_t g, uint8_t b) {
    // p1 et p2 sont sur la ligne du haut, p3 est le sommet inférieur
    float invSlope1 = (float)(p3.x - p1.x) / (p3.y - p1.y);
    float invSlope2 = (float)(p3.x - p2.x) / (p3.y - p2.y);
    
    float curX1 = (float)p3.x;
    float curX2 = (float)p3.x;
    
    for (int scanY = p3.y; scanY >= p1.y; --scanY) {
        drawScanline(scanY, (int)curX1, (int)curX2, depth, r, g, b);
        curX1 -= invSlope1;
        curX2 -= invSlope2;
    }
}

void Scene::rasterizeTriangle(const Triangle2d& tri, float avgDepth,
                             uint8_t r, uint8_t g, uint8_t b) {
    // Trier les sommets par Y
    Point2d pts[3] = {tri.p1, tri.p2, tri.p3};
    
    if (pts[0].y > pts[1].y) std::swap(pts[0], pts[1]);
    if (pts[0].y > pts[2].y) std::swap(pts[0], pts[2]);
    if (pts[1].y > pts[2].y) std::swap(pts[1], pts[2]);
    
    // Cas particulier : triangle plat
    if (pts[0].y == pts[2].y) {
        return;
    }
    
    // Cas particulier : triangle avec base plate en bas
    if (pts[1].y == pts[2].y) {
        fillFlatBottom(pts[0], pts[1], pts[2], avgDepth, r, g, b);
        return;
    }
    
    // Cas particulier : triangle avec base plate en haut
    if (pts[0].y == pts[1].y) {
        fillFlatTop(pts[0], pts[1], pts[2], avgDepth, r, g, b);
        return;
    }
    
    // Cas général : diviser le triangle en deux
    int midY = pts[1].y;
    float t = (float)(midY - pts[0].y) / (pts[2].y - pts[0].y);
    int midX = pts[0].x + (int)(t * (pts[2].x - pts[0].x));
    Point2d mid(midX, midY, avgDepth);
    
    fillFlatBottom(pts[0], pts[1], mid, avgDepth, r, g, b);
    fillFlatTop(pts[1], mid, pts[2], avgDepth, r, g, b);
}

void Scene::render() {
    sdl.clear(0, 0, 0);  // Fond noir
    
    // Structure pour stocker triangles avec couleur et profondeur
    struct RenderTriangle {
        Triangle3d tri;
        float depth;
        uint8_t r, g, b;
    };
    
    std::vector<RenderTriangle> visibleTris;
    
    // Filtrer et collecter les triangles visibles
    for (const auto& ct : triangles) {
        if (isTriangleVisible(ct.triangle)) {
            RenderTriangle rt;
            rt.tri = ct.triangle;
            rt.depth = ct.triangle.center().z;
            rt.r = ct.r;
            rt.g = ct.g;
            rt.b = ct.b;
            visibleTris.push_back(rt);
        }
    }
    
    // Trier du plus loin au plus proche
    std::sort(visibleTris.begin(), visibleTris.end(),
              [](const RenderTriangle& a, const RenderTriangle& b) {
                  return a.depth > b.depth;
              });
    
    // Rasteriser chaque triangle
    for (const auto& rt : visibleTris) {
        Point2d p1 = project(rt.tri.p1);
        Point2d p2 = project(rt.tri.p2);
        Point2d p3 = project(rt.tri.p3);
        
        Triangle2d tri2d(p1, p2, p3);
        rasterizeTriangle(tri2d, rt.depth, rt.r, rt.g, rt.b);
    }
    
    sdl.present();
}

} // namespace Engine3D

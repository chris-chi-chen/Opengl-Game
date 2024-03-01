#ifndef Narrow_H
#define Narrow_H
class Sphere {
public:
    glm::vec3 center;
    float radius;

    Sphere(const glm::vec3& center, float radius) : center(center), radius(radius) {}
};

struct Triangle {
    glm::vec3 v0, v1, v2;

    Triangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2) : v0(v0), v1(v1), v2(v2) {}
};
glm::vec3 projectPointOntoPlane(const glm::vec3& P, const glm::vec3& N, float D) {
    float t = -(glm::dot(N, P) + D) / glm::length2(N);
    glm::vec3 projection = P + t * N;
    return projection;
}
float SqrDistancePointToSegment(glm::vec3 p, glm::vec3 a, glm::vec3 b) {
    glm::vec3 ab = b - a;
    glm::vec3 ap = p - a;
    float t = glm::dot(ap, ab) / glm::dot(ab, ab);
    t = std::max(t, 0.0f);
    t = std::min(t, 1.0f);
    glm::vec3 closest = a + ab * t;
    return glm::length2(p - closest);
}

bool SphereTriangleIntersect(const Sphere& sphere, const Triangle& triangle) {
    // Compute vectors along the 3 edges of the triangle.
    glm::vec3 edge0 = triangle.v1 - triangle.v0;
    glm::vec3 edge1 = triangle.v2 - triangle.v1;
    glm::vec3 edge2 = triangle.v0 - triangle.v2;

    // Compute the normal of the triangle's plane.
    glm::vec3 normal = glm::normalize(glm::cross(edge0, edge1));
    // Compute the distance of the sphere's center to the triangle's plane.
    float distance = glm::dot(sphere.center - triangle.v0, normal);
    if (std::abs(distance) > sphere.radius) {
        // The sphere's center is too far from the triangle's plane.
        return false;
    }

    // Project the sphere's center onto the plane of the triangle.
    glm::vec3 projectedCenter = sphere.center - normal * distance;
    /*glm::vec3 projectedCenter = projectPointOntoPlane(sphere.center, normal, -(normal.x * triangle.v0.x + normal.y * triangle.v0.y + normal.z * triangle.v0.z));*/
    // Check if the projected center is inside the triangle.
    glm::vec3 cross0 = glm::cross(edge0, projectedCenter - triangle.v0);
    glm::vec3 cross1 = glm::cross(edge1, projectedCenter - triangle.v1);
    glm::vec3 cross2 = glm::cross(edge2, projectedCenter - triangle.v2);
    if (glm::dot(cross0, normal) > 0 && glm::dot(cross1, normal) > 0 && glm::dot(cross2, normal) > 0) {
        // The projected center is inside the triangle.
        return true;
    }

    // Check for the closest point on the edges if the projected center is outside the triangle.
    float sqrDist0 = SqrDistancePointToSegment(sphere.center, triangle.v0, triangle.v1);
    float sqrDist1 = SqrDistancePointToSegment(sphere.center, triangle.v1, triangle.v2);
    float sqrDist2 = SqrDistancePointToSegment(sphere.center, triangle.v2, triangle.v0);
    float minSqrDist = std::min({ sqrDist0, sqrDist1, sqrDist2 });

    // If the sphere's radius is larger than the distance to any edge, there is an intersection.
    /*return minSqrDist <= (sphere.radius * sphere.radius);*/
    return false;
}
#endif 
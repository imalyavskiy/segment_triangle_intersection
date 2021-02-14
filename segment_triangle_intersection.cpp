#include <iostream>
#include <cassert>

// some stuff to make the intersect function work
namespace my {
    // basic vector 3 operations
    struct vec3 {
        vec3(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) : x(_x), y(_y), z(_z) {}

        float x;
        float y;
        float z;

        vec3 operator+(const vec3& arg) const {
            return vec3{ x + arg.x, y + arg.y, z + arg.z };
        }

        vec3 operator-(const vec3& arg) const {
            return vec3{ x - arg.x, y - arg.y, z - arg.z };
        }

        vec3 operator*(const float arg) const {
            return vec3{ x * arg, y * arg, z * arg };
        }

        bool operator==(const vec3& arg) const {
            return x == arg.x && y == arg.y && z == arg.z;
        }
    };

    // cross product
    vec3 cross(const vec3& a1, const vec3& a2)
    {
        return vec3{ a1.y * a2.z - a1.z * a2.y, a1.z * a2.x - a1.x * a2.z, a1.x * a2.y - a1.y * a2.x };
    }

    // dot product
    float dot(const vec3& a1, const vec3& a2)
    {
        return a1.x * a2.x + a1.y * a2.y + a1.z * a2.z;
    }
}

using namespace my;

bool intersect(const vec3 origin, const vec3 direction, const vec3 v0, const vec3 v1, const vec3 v2, /*out*/ vec3& point)
{
    vec3 u, v, n;
    vec3 w0, w;
    float r, a, b;

    // * plane of a triangle and its normal vector
    u = (v1 - v0);   // vector of one side of a triangle
    v = (v2 - v0);   // vector of the another side of a triangle
    n = cross(u, v); // the normal vector to the plane of the triangle

    // finding out whether the point reaches the plane or not
    w0 = origin - v0;       // vector from the main angle of the triangle to the point of the previous position of the particle, the origin
    a = -dot(n, w0);        // "distance" from a point to the plane of the triangle, along the normal
    b = dot(n, direction);  // "distance" from point to end point of the path, normal

    r = a / b; // the relation of "distance" (a) to "distance" (b), i.e. on what % of the path is the intersection point
    if (r < 0.0 || 1.0 < r)
        // r >= 1 - point "did not reach" the plane
        return false;

    // point shift in the "direction" is proportional to the ratio r
    point = origin + direction * r; // the result is the coordinates of a point on the plane of the triangle (not the fact that in a triangle)

    // Finding out whether the point of intersection of the segment (origin shifted to direction) with the plane is in the triangle
    float uu, uv, vv, wu, wv, D;

    // The rest of the function is not clear to me. The code below finds out does the coordinates written in point variable belong to triangle 
    uu = dot(u, u);
    vv = dot(v, v);
    uv = dot(u, v);

    w = point - v0;

    wu = dot(w, u);
    wv = dot(w, v);

    D = uv * uv - uu * vv; // what is this? Where it came from?

    float s, t;

    s = (uv * wv - vv * wu) / D;
    if (s < 0.0 || s > 1.0)
        return false;
    t = (uv * wu - uu * wv) / D;
    if (t < 0.0 || (s + t) > 1.0)
        return false;

    return true;
}

int main()
{
    // prev point pos
    vec3 origin{2.0f, 1.0f, 1.0f};
    
    // the direction where the pointy must to move
    vec3 direction{ 1.0f, -2.0f, 0.0 };

    // triangle
    const vec3 p0{0.0f, 0.0f, 0.0f};
    const vec3 p1(2.5f, 0.0f, 4.3301f);
    const vec3 p2(5.0f, 0.0f, 0.0f);

    vec3 pr;

    // the call
    bool result = intersect(origin, direction, p0, p1, p2, pr);
    if (result)
        std::cout << "SUCCESS: The point belong to the triangle.\n";
    else
        std::cout << "FAILURE: The point is outside the triangle.\n";


    return result ? 0 : -1;
}

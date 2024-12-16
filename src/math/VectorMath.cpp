#include "VectorMath.hpp"
#include <iostream>

namespace Math
{
    Vector2::Vector2(float x, float y) : x(x), y(y) {}
    /* conversion from Vector2Int */
    Vector2::Vector2(const Vector2Int& other) : x((int)other.x), y((int)other.y) {} 

    /* find vector magnitude */
    float Vector2::length() const {
        return sqrt((x*x) + (y*y));
    }

    /* normalise the vector */
    void Vector2::normalise() {
        float len = length();
        if (len == 0.0f) {
            x = y = 0.0f;
        } else {
            x /= len; y /= len;
        }
    }
    /* return the vector as a unit vector */
    Vector2 Vector2::normalised() const {
        float len = length();
        if (len == 0.0f) return {0.0f, 0.0f};
        return {x/len, y/len};
    }

    /* prints the vector components to console, tab separated, ending with newline. */
    void Vector2::print() const { 
        std::cout << x <<'\t'<< y <<'\n';
    }

    /* operator overlaods */
    // vector arithmetic
    // dot product
    float Vector2::operator*(const Vector2& other) const {
        return (x*other.x) + (y*other.y);
    }
    // hadamrd product
    Vector2 Vector2::operator&(const Vector2& other) const {
        return {x*other.x, y*other.y};
    }
    void Vector2::operator&=(const Vector2& other) {
        x *= other.x; y *= other.y;
    }
    // cross product
    float Vector2::operator^(const Vector2& other) const {
        return (x*other.y)-(y*other.x);
    }
    // scalar product
    Vector2 Vector2::operator*(const float& k) const {
        return {x * k, y * k};
    }
    void Vector2::operator*=(const float& k) {
        x *= k; y*= k;
    }
    Vector2 Vector2::operator/(const float& k) const {
        return {x/k, y/k};
    }
    void Vector2::operator/=(const float& k) {
        x /= k; y /= k;
    }
    // vector addition
    Vector2 Vector2::operator+(const Vector2& other) const {
        return {x+other.x, y+other.y};
    }
    void Vector2::operator+=(const Vector2& other) {
        x += other.x; y += other.y;
    }
    Vector2 Vector2::operator-(const Vector2& other) const {
        return {x-other.x, y-other.y};
    }
    void Vector2::operator-=(const Vector2& other) {
        x -= other.x; y -= other.y;
    }
    // boolean comparisons
    bool Vector2::operator==(const Vector2& other) const {
        return (x==other.x) && (y==other.y);
    }
    bool Vector2::operator!=(const Vector2& other) const {
        return (x != other.x) || (y != other.y);
    }


    const Vector2 Vector2_Zero  = { 0.0f, 0.0f};
    const Vector2 Vector2_One   = { 1.0f, 1.0f};
    const Vector2 Vector2_Up    = { 0.0f, 1.0f};
    const Vector2 Vector2_Down  = { 0.0f,-1.0f};
    const Vector2 Vector2_Left  = {-1.0f, 0.0f};
    const Vector2 Vector2_Right = { 1.0f, 0.0f};



    Vector2Int::Vector2Int(int x, int y) : x(x), y(y) {}
    /* conversion from Vector2 */
    Vector2Int::Vector2Int(const Vector2& other) : x((float)other.x), y((float)other.y) {}

    /* prints the vector components to console, tab separated, ending with newline. */
    void Vector2Int::print() const { 
        std::cout << x <<'\t'<< y <<'\n';
    }

    /* operator overlaods */
    // vector arithmetic
    // dot product
    int Vector2Int::operator*(const Vector2Int& other) const {
        return (x*other.x) + (y*other.y);
    }
    // hadamard product
    Vector2Int Vector2Int::operator&(const Vector2Int& other) const {
        return {x*other.x, y*other.y};
    }
    // hadamard product
    void Vector2Int::operator&=(const Vector2Int& other) {
        x *= other.x; y *= other.y;
    }
    // cross product
    int Vector2Int::operator^(const Vector2Int& other) const {
        return (x*other.y)-(y*other.x);
    }
    // scalar product
    Vector2Int Vector2Int::operator*(const float& k) const {
        return {int(x * k), int(y * k)};
    }
    // scalar product
    void Vector2Int::operator*=(const float& k) {
        x *= k; y*= k;
    }
    Vector2Int Vector2Int::operator/(const float& k) const {
        return {int(x/k), int(y/k)};
    }
    void Vector2Int::operator/=(const float& k) {
        x /= k; y /= k;
    }
    // vector addition
    Vector2Int Vector2Int::operator+(const Vector2Int& other) const {
        return {x+other.x, y+other.y};
    }
    void Vector2Int::operator+=(const Vector2Int& other) {
        x += other.x; y += other.y;
    }
    Vector2Int Vector2Int::operator-(const Vector2Int& other) const {
        return {x-other.x, y-other.y};
    }
    void Vector2Int::operator-=(const Vector2Int& other) {
        x -= other.x; y -= other.y;
    }
    // boolean comparisons
    bool Vector2Int::operator==(const Vector2Int& other) const {
        return (x==other.x) && (y==other.y);
    }
    bool Vector2Int::operator!=(const Vector2Int& other) const {
        return (x != other.x) || (y != other.y);
    }


    const Vector2Int Vector2Int_Zero  = { 0, 0};
    const Vector2Int Vector2Int_One   = { 1, 1};
    const Vector2Int Vector2Int_Up    = { 0, 1};
    const Vector2Int Vector2Int_Down  = { 0,-1};
    const Vector2Int Vector2Int_Left  = {-1, 0};
    const Vector2Int Vector2Int_Right = { 1, 0};


    /* returns a unit vector pointing from p0 to p1 */
    Vector2 getUnitVector(const Vector2& p0, const Vector2& p1) {
        return (p1-p0).normalised();
    }

    /*
     * Returns true if the point is within the bounds of the rect.
     * 
     * \param p The point being queried.
     * \param rect The region to check.
     * \param inclusive Whether to return true if the point lies on the rect's edge.  
     */
    bool isInRect(Vector2 p, RectF rect, bool inclusive)
    {
        if (inclusive) {
            return (p.x>=rect.x && p.x<=(rect.x+rect.w))
                && (p.y>=rect.y && p.y<=(rect.y+rect.h));
        }
        return (p.x>rect.x && p.x<(rect.x+rect.w))
            && (p.y>rect.y && p.y<(rect.y+rect.h));
    }
}   
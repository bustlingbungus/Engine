#ifndef MATH_HPP
#define MATH_HPP

#include <cmath>

/* ==========  NUMERICAL CONSTANTS  ========== */

// Ratio of a circle's circumference to it's diametre
#define PI              3.141592654f
// The square root of 2
#define ROOT2           1.414213562f
// Sin/cos of 45 degrees, or PI/4 radians, or sqrt(2)/2 
#define SIN45           0.7071067812f
// Natural rate of change, e
#define EULERS_NUMBER   2.7182818285f

namespace Math
{
    struct RectF {
        /* the coordinates of the rect's origin (top left corner) */
        float x = 0.0f, y = 0.0f;
        /* the width and height of the rectangle */
        float w = 0.0f, h = 0.0f;
        /* width * height */
        float area() const { return w * h; }
    };

    template <typename T>
    T min(const T& a, const T& b) { return (a<b)? a : b; }
    template <typename T>
    T max(const T& a, const T& b) { return (a>b)? a : b; }

    /* Returns x clamped between min and max */
    template <typename T>
    T clamp(const T& min, const T& max, const T& x) {
        if (min > x) return min;
        return (max < x)? max : x;
    }
    /* Clamps *x between min and max */
    template <typename T>
    void clamp(const T& min, const T& max, T *x) {
        if (min > *x) *x = min;
        else if (max < *x) *x = max;
    }

    /* brings x to the nth power */
    template <typename T>
    T pow(T x, int n) {
        T res = 1.0;
        for (int p=abs(n);p>0;p--) res *= x;
        if (n<0) res = (T)1.0 / res;
        return res;
    }

    /* Returns a positive number with the same magnitude as a */
    template <typename T>
    T abs(const T& a) { return (a<(T)0)? (T)-a : (T)a; }

    /* Returns n rounded up to the nearest integer */
    template <typename T> 
    T ceil(const T& n) {
        int i = (int)n;
        T d = n - (T)i;
        return (d==(T)0.0)? (T)i : T(i+1);
    }

    /* Returns n rounded down to the nearest integer */
    template <typename T> 
    T floor(const T& n) { return T(int(n)); }

    /* Returns n rounded up to the nearest integer */
    template <typename T> 
    int ceilToInt(const T& n) {
        int i = (int)n;
        T d = n - (T)i;
        return (d==(T)0.0)? i : i+1;
    }

    /* Returns n rounded down to the nearest integer */
    template <typename T>
    int floorToInt(const T& n) { return (int)n; }

    /* Returns -1 if x<0, 1 if x>0, and 0 if x is 0 */
    template <typename T>
    int sign(const T& x) {
        if (x == (T)0.0) return 0;
        return (x < (T)0.0)? -1 : 1;
    }

    /* Returns n! n must be positive */
    inline int fact(int n) {
        int res = 1;
        while (n > 0) res *= n--;
        return res;
    }

    /* Returns the number of high bits in a variable */
    template <typename T>
    unsigned int numBits(T var) {
        unsigned int res = 0;
        for (;var;var>>=1) if (var&1) res++;
        return res;
    }

    /* 
     * Returns the linear interpolation value from `a` to `b`, using `t` as an interpolator.
     * Starts at `a`, ends at `b`. i.e., if `t = 0` `a` is returned, and when `t = 1` `b` is returned.
     * 
     * Does this by returning `b*t + a*(1-t)`.
     * 
     * \note the formula is simplified to `a + (b-a)*t`.
     * 
     * \param a the starting point
     * \param b the ending point
     * \param t the interpolator
     */
    template <typename T>
    T lerp(T a, T b, float t) {
        return a + (b-a)*t;
    }
}

#endif
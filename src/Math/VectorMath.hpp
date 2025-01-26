#ifndef VECTORMATH_HPP
#define VECTORMATH_HPP

#include "Math.hpp"

namespace Math
{
    struct Vector2Int; // forward declaration

    /* two dimensional vector */
    struct Vector2
    {
        /* x and y components */
        float x, y;

        Vector2(float x = 0.0f, float y = 0.0f);
        Vector2(const Vector2Int& other);

        void print() const;

        float length() const;
        void normalise();
        Vector2 normalised() const;

        float operator*(const Vector2& other) const;
        Vector2 operator&(const Vector2& other) const;
        void operator&=(const Vector2& other);
        Vector2 operator|(const Vector2& other) const;
        void operator|=(const Vector2& other);
        float operator^(const Vector2& other) const;
        Vector2 operator*(const float& k) const;
        void operator*=(const float& k);
        Vector2 operator/(const float& k) const;
        void operator/=(const float& k);
        Vector2 operator+(const Vector2& other) const;
        void operator+=(const Vector2& other);
        Vector2 operator-(const Vector2& other) const;
        void operator-=(const Vector2& other);
        bool operator==(const Vector2& other) const;
        bool operator!=(const Vector2& other) const;
    };

    const extern Vector2 Vector2_Zero;
    const extern Vector2 Vector2_One;
    const extern Vector2 Vector2_Up;
    const extern Vector2 Vector2_Down;
    const extern Vector2 Vector2_Left;
    const extern Vector2 Vector2_Right;



    /* two dimensional vector (integer components) */
    struct Vector2Int
    {
        /* x and y components */
        int x, y;

        Vector2Int(int x = 0, int y = 0);
        Vector2Int(const Vector2& other);

        void print() const;

        int operator*(const Vector2Int& other) const;
        Vector2Int operator&(const Vector2Int& other) const;
        void operator&=(const Vector2Int& other);
        Vector2Int operator|(const Vector2Int& other) const;
        void operator|=(const Vector2Int& other);
        int operator^(const Vector2Int& other) const;
        Vector2Int operator*(const float& k) const;
        void operator*=(const float& k);
        Vector2Int operator/(const float& k) const;
        void operator/=(const float& k);
        Vector2Int operator+(const Vector2Int& other) const;
        void operator+=(const Vector2Int& other);
        Vector2Int operator-(const Vector2Int& other) const;
        void operator-=(const Vector2Int& other);
        bool operator==(const Vector2Int& other) const;
        bool operator!=(const Vector2Int& other) const;
    };

    const extern Vector2Int Vector2Int_Zero;
    const extern Vector2Int Vector2Int_One;
    const extern Vector2Int Vector2Int_Up;
    const extern Vector2Int Vector2Int_Down;
    const extern Vector2Int Vector2Int_Left;
    const extern Vector2Int Vector2Int_Right;


    Vector2 getUnitVector(const Vector2& p0, const Vector2& p1);

    bool isInRect(Vector2 p, RectF rect, bool inclusive = true);
}

#endif
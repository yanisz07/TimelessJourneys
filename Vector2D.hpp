#ifndef VECTOR2D_H
#define VECTOR2D_H

#include "iostream"
#include <cmath>

class Vector2D
{
public:
    double x;
    double y;

    Vector2D();
    Vector2D(double x, double y);
    double magnitude();

    Vector2D& Add(const Vector2D& vec);
    Vector2D& Subtract(const Vector2D& vec);
    Vector2D& Multiply(const Vector2D& vec);
    Vector2D& Divide(const Vector2D& vec);
    float distance(const Vector2D& vec);
    Vector2D& Normalize();
    Vector2D Rotation(double theta, bool clockwise); // in Rad!!
    double angle(const Vector2D& vec);
    double norm();

    friend Vector2D operator+(const Vector2D& v1, const Vector2D& v2);
    friend Vector2D operator-(const Vector2D& v1, const Vector2D& v2);
    friend Vector2D& operator*(Vector2D& v1, const Vector2D& v2);
    friend Vector2D& operator/(Vector2D& v1, const Vector2D& v2);

    Vector2D& operator+=(const Vector2D& vec);
    Vector2D& operator-=(const Vector2D& vec);
    Vector2D& operator*=(const Vector2D& vec);
    Vector2D& operator/=(const Vector2D& vec);

    Vector2D& operator*(const int& i);
    Vector2D& Zero();

    friend std::ostream& operator<<(std::ostream& stream, const Vector2D& vec);
};

#endif // VECTOR2D_H

#include "Vector2D.hpp"
#include <cmath>

const double pi = M_PI;

Vector2D::Vector2D()
{
    x = 0.0f;
    y = 0.0f;
}

Vector2D::Vector2D(double x, double y)
{
    this->x = x;
    this->y = y;
}

double Vector2D::magnitude(){
    return sqrt(x * x + y * y);
}

Vector2D& Vector2D::Add(const Vector2D &vec)
{
    this->x += vec.x;
    this->y += vec.y;

    return *this;
}

Vector2D& Vector2D::Subtract(const Vector2D &vec)
{
    this->x -= vec.x;
    this->y -= vec.y;

    return *this;
}

Vector2D& Vector2D::Multiply(const Vector2D &vec)
{
    this->x *= vec.x;
    this->y *= vec.y;

    return *this;
}

Vector2D& Vector2D::Divide(const Vector2D &vec)
{
    this->x /= vec.x;
    this->y /= vec.y;

    return *this;
}

float Vector2D::distance(const Vector2D &vec){
    return sqrt(pow(x-vec.x,2)+pow(y-vec.y,2));

}

Vector2D& Vector2D::Normalize() {
    double length = magnitude();

    if (length > 0) { // To avoid division by zero
        x /= length;
        y /= length;
    }

    return *this;
}

Vector2D Vector2D::Rotation(double theta, bool clockwise){ //in Rad!!
    Vector2D out;
    if (clockwise==true){
        out.x = x*cos((180/pi)*theta)+y*sin((180/pi)*theta);
        out.y = -x*sin((180/pi)*theta)+y*cos((180/pi)*theta);
    }else{
        out.x = x*cos((180/pi)*theta)-y*sin((180/pi)*theta);
        out.y = x*sin((180/pi)*theta)+y*cos((180/pi)*theta);
    }

    return out;
}

double Vector2D::angle(const Vector2D &vec){
    return acos((x*vec.x + y*vec.y)/((sqrt(x*x+y*y))*(sqrt(vec.x*vec.x+vec.y*vec.y))));
}

Vector2D operator+(const Vector2D& v1, const Vector2D& v2)
{
    return Vector2D(v1.x+v2.x,v1.y+v2.y);
}

Vector2D operator-(const Vector2D& v1, const Vector2D& v2)
{
    return Vector2D(v1.x-v2.x,v1.y-v2.y);
}

Vector2D& operator*(Vector2D& v1, const Vector2D& v2)
{
    return v1.Multiply(v2);
}

Vector2D& operator/(Vector2D& v1, const Vector2D& v2)
{
    return v1.Divide(v2);
}

Vector2D& Vector2D::operator+=(const Vector2D& vec)
{
    return this->Add(vec);
}

Vector2D& Vector2D::operator-=(const Vector2D& vec)
{
    return this->Subtract(vec);
}

Vector2D& Vector2D::operator*=(const Vector2D& vec)
{
    return this->Multiply(vec);
}

Vector2D& Vector2D::operator/=(const Vector2D& vec)
{
    return this->Divide(vec);
}

Vector2D& Vector2D::operator*(const int& i)
{
    this->x *= i;
    this->y *= i;

    return *this;
}

Vector2D& Vector2D::Zero()
{
    this->x = 0;
    this->y = 0;

    return *this;
}

std::ostream& operator<<(std::ostream& stream, const Vector2D& vec)
{
    stream << "(" << vec.x << "," << vec.y << ")";
    return stream;
}

double Vector2D::norm()
{
    return sqrt(x*x+y*y);
}

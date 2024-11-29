#include <math.h>
#include "Vec2.h"

Vec2::Vec2() : x(0), y(0) {}

Vec2::Vec2(float xin, float yin) : x(xin), y(yin) {}

bool Vec2::operator == (const Vec2 & rhs) const{
    return (x==rhs.x && y==rhs.y);
}

bool Vec2::operator != (const Vec2 & rhs) const{
    return (x!=rhs.x && y!=rhs.y);
}

Vec2 Vec2::operator + (const Vec2 & rhs) const{
    return Vec2(x+rhs.x, y+rhs.y);
}

Vec2 Vec2::operator - (const Vec2 & rhs) const{
    return Vec2(x-rhs.x, y-rhs.y);
}

Vec2 Vec2::operator / (const float val) const{
    return Vec2(x/val, y/val);
}

Vec2 Vec2::operator * (const float val) const{
    return Vec2(x*val, y*val);
}

void Vec2::operator += (const Vec2 & rhs){
    x += rhs.x;
    y += rhs.y;
}

void Vec2::operator -= (const Vec2 & rhs){
    x -= rhs.x;
    y -= rhs.y;
}

void Vec2::operator /= (const float val){
    x /= val;
    y /= val;
}

void Vec2::operator *= (const float val){
    x *= val;
    x *= val;
}

float Vec2::dist(const Vec2 & rhs) const{
    float dx = x - rhs.x;
    float dy = y - rhs.y;
    return sqrt(dx * dx + dy * dy);
}

float Vec2::length() const{
    return sqrt(x * x + y * y);
}
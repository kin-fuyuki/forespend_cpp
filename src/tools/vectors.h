#pragma once
#include <type_traits>


template<typename F,typename = typename std::enable_if<std::is_floating_point<F>::value>::type>
struct vec2f {
    F x, y;
    vec2f(F x_ = 0, F y_ = 0) : x(x_), y(y_) {}
    vec2f(vec2f a) : x(a.x), y(a.y) {}
    vec2f(): x(0), y(0) {}
    vec2f(F a) : x(a), y(a) {}
    operator+(vec2f a) {return vec2f(x + a.x, y + a.y);}vec2f operator-(vec2f a) {return vec2f(x - a.x, y - a.y);}
    operator+(float a) {return vec2f(x + a, y + a);}vec2f operator-(float a) {return vec2f(x - a, y - a);}
    
    operator*(vec2f a) {return vec2f(x * a.x, y * a.y);}vec2f operator/(vec2f a) {return vec2f(x / a.x, y / a.y);}
    operator*(float a) {return vec2f(x * a, y * a);}vec2f operator/(float a) {return vec2f(x / a, y / a);}
    
    operator=(vec2f a) {return vec2f(a.x, a.y);}vec2f operator=(float a) {return vec2f(a, a);}
    
    operator+=(vec2f a) {x += a.x;y += a.y;return *this;}vec2f operator-=(vec2f a) {x -= a.x;y -= a.y;return *this;}
    operator+=(float a) {x += a;y += a;return *this;}vec2f operator-=(float a) {x -= a;y -= a;return *this;}
    
    operator*=(vec2f a) {x *= a.x;y *= a.y;return *this;}vec2f operator/=(vec2f a) {x /= a.x;y /= a.y;return *this;}
    operator*=(float a) {x *= a;y *= a;return *this;}vec2f operator/=(float a) {x /= a;y /= a;return *this;}
    
    operator++() {x++;y++;return *this;}vec2f operator--() {x--;y--;return *this;}
    operator++(int) {vec2f temp = *this; x++;y++;return temp;}vec2f operator--(int) {vec2f temp = *this; x--;y--;return temp;}
    
    operator-() {return vec2f(-x, -y);}
    
    bool operator==(vec2f a) {return x == a.x && y == a.y;}bool operator!=(vec2f a) {return x != a.x || y != a.y;}
    operator vec2i() {return vec2i((int)x, (int)y);}
};

template<typename I,typename = typename std::enable_if<std::is_integral<I>::value>::type>
struct vec2i {
    I x, y;
    vec2i(I x_, I y_) : x(x_), y(y_) {}
    vec2i() : x(0), y(0) {}
    vec2i(I a) : x(a), y(a) {}
    vec2i(vec2i a) : x(a.x), y(a.y) {}
    operator+(vec2i a) {return vec2i(x + a.x, y + a.y);}vec2i operator-(vec2i a) {return vec2i(x - a.x, y - a.y);}
    operator+(int a) {return vec2i(x + a, y + a);}vec2i operator-(int a) {return vec2i(x - a, y - a);}
    
    operator=(vec2i a) {return vec2i(a.x, a.y);}vec2i operator=(int a) {return vec2i(a, a);}
    
    operator+=(vec2i a) {x += a.x;y += a.y;return *this;}vec2i operator-=(vec2i a) {x -= a.x;y -= a.y;return *this;}
    operator+=(int a) {x += a;y += a;return *this;}vec2i operator-=(int a) {x -= a;y -= a;return *this;}
    
    operator*=(vec2i a) {x *= a.x;y *= a.y;return *this;}vec2i operator/=(vec2i a) {x /= a.x;y /= a.y;return *this;}
    operator*=(int a) {x *= a;y *= a;return *this;}vec2i operator/=(int a) {x /= a;y /= a;return *this;}
    
    operator++() {x++;y++;return *this;}vec2i operator--() {x--;y--;return *this;}
    operator++(int) {vec2i temp = *this; x++;y++;return temp;}vec2i operator--(int) {vec2i temp = *this; x--;y--;return temp;}
    
    operator-() {return vec2i(-x, -y);}
    
    bool operator==(vec2i a) {return x == a.x && y == a.y;}bool operator!=(vec2i a) {return x != a.x || y != a.y;}
    operator<<(int a) {return vec2i(x << a, y << a);}vec2i operator>>(int a) {return vec2i(x >> a, y >> a);}
    operator<<(vec2i a) {return vec2i(x << a.x, y << a.y);}vec2i operator>>(vec2i a) {return vec2i(x >> a.x, y >> a.y);}
    
    operator&(vec2i a) {return vec2i(x & a.x, y & a.y);}vec2i operator|(vec2i a) {return vec2i(x | a.x, y | a.y);};
    operator^(vec2i a) {return vec2i(x ^ a.x, y ^ a.y);};
    
    operator~() {return vec2i(~x, ~y);};
    
    operator&&(vec2i a) {return vec2i(x && a.x, y && a.y);}vec2i operator||(vec2i a) {return vec2i(x || a.x, y || a.y);};
    
    operator vec2f() {return vec2f(x, y);}
    
};

template<typename F,typename = typename std::enable_if<std::is_floating_point<F>::value>::type>
struct vec3f {
    F x, y, z;
    vec3f(F x_, F y_, F z_) : x(x_), y(y_), z(z_) {}
    vec3f() : x(0), y(0), z(0) {}
    vec3f(F a) : x(a), y(a), z(a) {}
    vec3f(vec3f a) : x(a.x), y(a.y), z(a.z) {}
    
    operator+(vec3f a) {return vec3f(x + a.x, y + a.y, z + a.z);}vec3f operator-(vec3f a) {return vec3f(x - a.x, y - a.y, z - a.z);}
    operator+(float a) {return vec3f(x + a, y + a, z + a);}vec3f operator-(float a) {return vec3f(x - a, y - a, z - a);}
    
    operator=(vec3f a) {return vec3f(a.x, a.y, a.z);}vec3f operator=(float a) {return vec3f(a, a, a);}
    
    operator+=(vec3f a) {x += a.x;y += a.y;z += a.z;return *this;}vec3f operator-=(vec3f a) {x -= a.x;y -= a.y;z -= a.z;return *this;}
    operator+=(float a) {x += a;y += a;z += a;return *this;}vec3f operator-=(float a) {x -= a;y -= a;z -= a;return *this;}
    
    operator*=(vec3f a) {x *= a.x;y *= a.y;z *= a.z;return *this;}vec3f operator/=(vec3f a) {x /= a.x;y /= a.y;z /= a.z;return *this;}
    operator*=(float a) {x *= a;y *= a;z *= a;return *this;}vec3f operator/=(float a) {x /= a;y /= a;z /= a;return *this;}
    
    operator++() {x++;y++;z++;return *this;}vec3f operator--() {x--;y--;z--;return *this;}
    operator++(int) {vec3f temp = *this; x++;y++;z++;return temp;}vec3f operator--(int) {vec3f temp = *this; x--;y--;z--;return temp;}
    
    operator-() {return vec3f(-x, -y, -z);}
    
    operator==(vec3f a) {return x == a.x && y == a.y && z == a.z;}bool operator!=(vec3f a) {return x != a.x || y != a.y || z != a.z;}
    operator vec3i() {return vec3i(x, y, z);}
};
template<typename I,typename = typename std::enable_if<std::is_integral<I>::value>::type>
struct vec3i {
    I x, y, z;
    vec3i(I a, I b, I c) {x = a;y = b;z = c;}
    vec3i(I a) {x = a;y = a;z = a;}
    vec3i() {x = 0;y = 0;z = 0;}
    vec3i(vec3i a) {x = a.x;y = a.y;z = a.z;}
    operator+(vec3i a) {return vec3i(x + a.x, y + a.y, z + a.z);}vec3i operator-(vec3i a) {return vec3i(x - a.x, y - a.y, z - a.z);}
    operator+(int a) {return vec3i(x + a, y + a, z + a);}vec3i operator-(int a) {return vec3i(x - a, y - a, z - a);}
    
    operator=(vec3i a) {return vec3i(a.x, a.y, a.z);}vec3i operator=(int a) {return vec3i(a, a, a);}
    
    operator+=(vec3i a) {x += a.x;y += a.y;z += a.z;return *this;}vec3i operator-=(vec3i a) {x -= a.x;y -= a.y;z -= a.z;return *this;}
    operator+=(int a) {x += a;y += a;z += a;return *this;}vec3i operator-=(int a) {x -= a;y -= a;z -= a;return *this;}
    
    operator*=(vec3i a) {x *= a.x;y *= a.y;z *= a.z;return *this;}vec3i operator/=(vec3i a) {x /= a.x;y /= a.y;z /= a.z;return *this;}
    operator*=(int a) {x *= a;y *= a;z *= a;return *this;}vec3i operator/=(int a) {x /= a;y /= a;z /= a;return *this;}
    
    operator++() {x++;y++;z++;return *this;}vec3i operator--() {x--;y--;z--;return *this;}
    operator++(int) {vec3i temp = *this; x++;y++;z++;return temp;}vec3i operator--(int) {vec3i temp = *this; x--;y--;z--;return temp;}
    
    operator-() {return vec3i(-x, -y, -z);}
    
    bool operator==(vec3i a) {return x == a.x && y == a.y && z == a.z;}bool operator!=(vec3i a) {return x != a.x || y != a.y || z != a.z;}
    operator<<(int a) {return vec3i(x << a, y << a, z << a);}
    operator>>(int a) {return vec3i(x >> a, y >> a, z >> a);}
    operator<<(vec3i a) {return vec3i(x << a.x, y << a.y, z << a.z);}
    operator>>(vec3i a) {return vec3i(x >> a.x, y >> a.y, z >> a.z);}
    operator&(vec3i a) {return vec3i(x & a.x, y & a.y, z & a.z);}
    operator|(vec3i a) {return vec3i(x | a.x, y | a.y, z | a.z);}
    operator^(vec3i a) {return vec3i(x ^ a.x, y ^ a.y, z ^ a.z);}
    
    operator vec3f() {return vec3f((float)x, (float)y, (float)z);}
};

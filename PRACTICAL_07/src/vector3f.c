#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <math.h>

#include "./include/vector3f.h"

// Initialize a zero vector
void initVector3fZero(Vector3f_ *v) {
    v->x = 0.0f;
    v->y = 0.0f;
    v->z = 0.0f;
}

// Initialize a unit vector
void initUnitVector3f(Vector3f_ *v) {
    v->x = 1.0f;
    v->y = 1.0f;
    v->z = 1.0f;
}

// Initialize a vector with given values
void initVector3f(Vector3f_ *v, float x, float y, float z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

// Check if two vectors are equal
int equals(const Vector3f_ *lhs, const Vector3f_ *rhs) {
    return (lhs->x == rhs->x && lhs->y == rhs->y && lhs->z == rhs->z);
}

// Calculate the length of the vector
float length(const Vector3f_ *v) {
    return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
}

// Calculate the squared length of the vector
float lengthSquared(const Vector3f_ *v) {
    return (v->x * v->x + v->y * v->y + v->z * v->z);
}

// Normalize the vector
void normalize(Vector3f_ *v) {
    float magnitude = length(v);
    if (magnitude > 0) {
        v->x /= magnitude;
        v->y /= magnitude;
        v->z /= magnitude;
    }
}

// Input the vector
void inputVector3f(Vector3f_ *v) {
    printf("Enter x: ");
    scanf("%f", &v->x);
    printf("Enter y: ");
    scanf("%f", &v->y);
    printf("Enter z: ");
    scanf("%f", &v->z);
}


#ifdef __cplusplus
}
#endif
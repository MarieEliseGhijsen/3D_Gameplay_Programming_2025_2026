#ifndef VECTOR3F_H
#define VECTOR3F_H

#include <stdio.h>
#include <math.h>

#include "./include/debug.h"

typedef struct {
    float x, y, z;
} Vector3f_;

// Initialize a zero vector
void initVector3fZero(Vector3f_ *v);

// Initialize a unit vector
void initUnitVector3f(Vector3f_ *v);

// Initialize a vector with given values
void initVector3f(Vector3f_ *v, float x, float y, float z);

// Check if two vectors are equal
int equals(const Vector3f_ *lhs, const Vector3f_ *rhs);

// Calculate the length of the vector
float length(const Vector3f_ *v);

// Calculate the squared length of the vector
float lengthSquared(const Vector3f_ *v);

// Normalize the vector
void normalize(Vector3f_ *v);

// Print the vector
void printVector3f(const Vector3f_ *v);

// Input the vector
void inputVector3f(Vector3f_ *v);

#endif // VECTOR3F_H
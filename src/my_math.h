#pragma once
#include <stdbool.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

#define MY_PI 3.14159265358979323846
#define MY_PI_2 1.57079632679489661923
#define MY_PI_4 0.78539816339744830962

typedef struct Vector2D
{
	float x;
	float y;
}Vector2D;

typedef struct Line2D	// Cartesian equation (ax + by + c = 0)
{
	float a;
	float b;
	float c;
}Line2D;

typedef struct Circle2D
{
	Vector2D centre;
	float radius;
}Circle2D;

typedef struct Triangle2D
{
	Vector2D vertices[3];
}Triangle2D;

typedef struct Rectangle2D
{
	Vector2D vertices[4];
}Rectangle2D;

typedef struct Segment2D
{
	Vector2D vertices[2];
}Segment2D;

Vector2D calculatePointDependentTime2D(Vector2D orginePoint, Vector2D directionVector, float time);

float vectorMagnitude2D(Vector2D A, Vector2D B);

// Colliding
bool collisionSegmentCircle2D(Circle2D circle, Vector2D A, Vector2D B);
bool collisionLinePoint2D(Vector2D point, Line2D line);
bool collisionPointSegment(Vector2D pointToCheck, Vector2D segmentA, Vector2D segmentB);
bool collisionCircleCircle2D(Circle2D circle1, Circle2D circle2);
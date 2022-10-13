#pragma once
#include "my_math.h"

typedef enum EnemySize
{
    ES_SMALL,
    ES_MEDIUM,
    ES_BIG,
}EnemySize;

typedef enum EnemyType
{
    ET_FLOATING_MINE,
    ET_FIREBALL_MINE,
    ET_MAGNETIC_MINE,
    ET_MAGNETIC_FIREBALL_MINE,
    ET_MINE_LAYER,
}EnemyType;

typedef struct ConvexShape
{
    int vertexAmount;
    Vector2D vertices[6];
} ConvexShape;

typedef struct Core
{
    float rotation;
    float speed;
    Vector2D barycentre;
    float lastThrustRotation;
    int lifePoint;
    int numberVector;
	Vector2D hitbox[10];
    int convexShapeAmount;
    ConvexShape convexShapes[4];
}Core;


typedef struct Enemy
{
    Core core;
    EnemySize size;
    EnemyType type;
    bool hasSpawned;
    int minionSpawned;
}Enemy;

typedef struct FireBall
{
    bool isActive;
    int time;
	Circle2D hitbox;
    Vector2D moveVector;
}FireBall;

typedef struct Character
{
    float invisibility;
    float distance;
    Core core;
}Character;

typedef struct Vec2mM
{
    Vector2D min;
    Vector2D max;
}Vec2mM;
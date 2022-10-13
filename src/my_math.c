#include "my_math.h"

Vector2D calculatePointDependentTime2D(Vector2D orginePoint, Vector2D directionVector, float time)
{
    Vector2D newPoint;
    newPoint.x = orginePoint.x + time * directionVector.x;
    newPoint.y = orginePoint.y + time * directionVector.y;
    return newPoint;
}

float vectorMagnitude2D(Vector2D A, Vector2D B)
{
    return sqrtf( powf(A.x - B.x, 2) + powf(A.y - B.y, 2));
}

// Colliding
bool CollisionPointCercle2D(Circle2D circle, Vector2D A)
{
    float distX = A.x - circle.centre.x;
    float distY = A.y - circle.centre.y;

    float distance = sqrtf((distX * distX) + (distY * distY));

    if(distance <= circle.radius)
        return true;
    return false;
}

bool collisionLineCircle2D(Circle2D circle, Vector2D A, Vector2D B)
{
    Vector2D u;
    u.x = B.x - A.x;
    u.y = B.y - A.y;
    Vector2D AC;
    AC.x = circle.centre.x - A.x;
    AC.y = circle.centre.y - A.y;
    
    float numerateur = u.x*AC.y - u.y*AC.x;
    numerateur = fabs(numerateur);

    float denominateur = sqrtf(u.x * u.x + u.y * u.y);
    float CI = numerateur / denominateur;
    if (CI < circle.radius)
        return true;
    else
        return false;
}

bool collisionSegmentCircle2D(Circle2D circle, Vector2D A, Vector2D B)
{
    if (collisionLineCircle2D(circle, A, B) == false)
        return false;
    
    Vector2D AB,AC,BC;
    AB.x = B.x - A.x;
    AB.y = B.y - A.y;
    AC.x = circle.centre.x - A.x;
    AC.y = circle.centre.y - A.y;
    BC.x = circle.centre.x - B.x;
    BC.y = circle.centre.y - B.y;

    float pscal1 = AB.x*AC.x + AB.y*AC.y;
    float pscal2 = (-AB.x)*BC.x + (-AB.y)*BC.y;

    if (pscal1>=0 && pscal2>=0)
        return true;
        
    if (CollisionPointCercle2D(circle, A))
        return true;
    if (CollisionPointCercle2D(circle, B))
        return true;
    return false;
}

Line2D getLineFromPoints(Vector2D a, Vector2D b)
{
	return (Line2D){a.y - b.y, b.x - a.x, (a.x - b.x) * a.y + (b.y - a.y) * a.x};
}

bool collisionLinePoint2D(Vector2D point, Line2D line)
{
    float res = ( line.a * point.x + line.c ) / line.b * -1.f;
    return (res + 10 > point.y) && (res - 10 < point.y);
}

bool collisionPointSegment(Vector2D pointToCheck, Vector2D segmentPointA, Vector2D segmentPointB)
{
    if (pointToCheck.x < fminf(segmentPointA.x, segmentPointB.x) || pointToCheck.x > fmaxf(segmentPointA.x, segmentPointB.x) ||
        pointToCheck.y < fminf(segmentPointA.y, segmentPointB.y) || pointToCheck.y > fmaxf(segmentPointA.y, segmentPointB.y))
        return false;
    return collisionLinePoint2D(pointToCheck, getLineFromPoints(segmentPointA, segmentPointB));
}

bool collisionCircleCircle2D(Circle2D circle1, Circle2D circle2)
{
    return (sqrtf(powf(circle1.centre.x - circle2.centre.x, 2.f) + powf(circle1.centre.y - circle2.centre.y, 2.f))) < circle1.radius + circle2.radius;
}
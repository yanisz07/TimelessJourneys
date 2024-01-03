#include "Collision.hpp"
#include "ECS/ColliderComponent.hpp"
#include <iostream>
#include <cmath>

bool Collision::AABB(const SDL_Rect &recA, const SDL_Rect &recB)
{
    if (
        recA.x + recA.w >= recB.x &&
        recB.x + recB.w >= recA.x &&
        recA.y + recA.h >= recB.y &&
        recB.y + recB.h >= recA.y
        )
    {
        return true;
    }
    return false;
}

bool Collision::AABB(const ColliderComponent& colA, const ColliderComponent& colB)
{
    if (AABB(colA.collider, colB.collider))
    {
        //std::cout << colA.tag << " hit: " << colB.tag << std::endl;
        return true;
    }
    else
    {
        return false;
    }
}

double toRadians(double angle) {
    return angle * (M_PI / 180.0);
}

Vector2D UpperLeft(const SDL_Rect &rec, double theta)
{
    double thetaInRadians = toRadians(theta);
    double costheta = std::cos(thetaInRadians);
    double sinetheta = std::sin(thetaInRadians);
    //find center
    Vector2D center = Vector2D(rec.x+rec.w/2,rec.y+rec.h/2);
    Vector2D UL = Vector2D(rec.x,rec.y);
    Vector2D u = Vector2D(UL.x-center.x,UL.y-center.y);
    //rotate
    double x = u.x;
    double y = u.y;
    return Vector2D(center.x + costheta*x-sinetheta*y,center.y + sinetheta*x+costheta*y);
}

Vector2D UpperRight(const SDL_Rect &rec, double theta)
{
    double thetaInRadians = toRadians(theta);
    double costheta = std::cos(thetaInRadians);
    double sinetheta = std::sin(thetaInRadians);
    //find center
    Vector2D center = Vector2D(rec.x+rec.w/2,rec.y+rec.h/2);
    Vector2D UR = Vector2D(rec.x+rec.w,rec.y);
    Vector2D u = Vector2D(UR.x-center.x,UR.y-center.y);
    //rotate
    double x = u.x;
    double y = u.y;
    return Vector2D(center.x + costheta*x-sinetheta*y,center.y + sinetheta*x+costheta*y);
}

Vector2D LowerLeft(const SDL_Rect &rec, double theta)
{
    double thetaInRadians = toRadians(theta);
    double costheta = std::cos(thetaInRadians);
    double sinetheta = std::sin(thetaInRadians);
    //find center
    Vector2D center = Vector2D(rec.x+rec.w/2,rec.y+rec.h/2);
    Vector2D LL = Vector2D(rec.x,rec.y+rec.h);
    Vector2D u = Vector2D(LL.x-center.x,LL.y-center.y);
    //rotate
    double x = u.x;
    double y = u.y;
    return Vector2D(center.x + costheta*x-sinetheta*y,center.y + sinetheta*x+costheta*y);
}

Vector2D LowerRight(const SDL_Rect &rec, double theta)
{
    double thetaInRadians = toRadians(theta);
    double costheta = std::cos(thetaInRadians);
    double sinetheta = std::sin(thetaInRadians);
    //find center
    Vector2D center = Vector2D(rec.x+rec.w/2,rec.y+rec.h/2);
    Vector2D LR = Vector2D(rec.x+rec.w,rec.y+rec.h);
    Vector2D u = Vector2D(LR.x-center.x,LR.y-center.y);
    //rotate
    double x = u.x;
    double y = u.y;
    return Vector2D(center.x + costheta*x-sinetheta*y,center.y + sinetheta*x+costheta*y);
}

double dotProduct(Vector2D e1, Vector2D e2)
{
    return e1.x*e2.x + e1.y*e2.y;
}

double norm(Vector2D v)
{
    return std::sqrt(std::pow(v.x,2)+std::pow(v.y,2));
}

Vector2D Projection(Vector2D v, Vector2D u)
{
    double value = dotProduct(v,u)/dotProduct(u,u);
    return Vector2D(value*u.x,value*u.y);
}

Vector2D MinPoint(Vector2D p1, Vector2D p2)
{
    if (p1.x < p2.x)
    {
        return p1;
    }
    return p2;
}

Vector2D MinPoint(Vector2D p1, Vector2D p2, Vector2D p3, Vector2D p4)
{
    Vector2D tmp;
    tmp = p1;
    if (p2.x < tmp.x)
    {
        tmp = p2;
    }
    if (p3.x < tmp.x)
    {
        tmp = p3;
    }
    if (p4.x < tmp.x)
    {
        tmp = p4;
    }
    return tmp;
}

Vector2D MaxPoint(Vector2D p1, Vector2D p2)
{
    if (p1.x < p2.x)
    {
        return p2;
    }
    return p1;
}

Vector2D MaxPoint(Vector2D p1, Vector2D p2, Vector2D p3, Vector2D p4)
{
    Vector2D tmp;
    tmp = p1;
    if (p2.x > tmp.x)
    {
        tmp = p2;
    }
    if (p3.x > tmp.x)
    {
        tmp = p3;
    }
    if (p4.x > tmp.x)
    {
        tmp = p4;
    }
    return tmp;
}

bool Collision::SAT(const SDL_Rect& recA, double angleA, const SDL_Rect& recB, double angleB)
{
    int t = 0;

    //corners of recA
    Vector2D UR_A = UpperRight(recA,angleA);
    Vector2D UL_A = UpperLeft(recA,angleA);
    Vector2D LR_A = LowerRight(recA,angleA);
    Vector2D LL_A = LowerLeft(recA,angleA);

    //corners of recB
    Vector2D UR_B = UpperRight(recB,angleB);
    Vector2D UL_B = UpperLeft(recB,angleB);
    Vector2D LR_B = LowerRight(recB,angleB);
    Vector2D LL_B = LowerLeft(recB,angleB);

    //Axis1
    std::cout<< "Axis1" << std::endl;

    //compute director vector
    Vector2D u1 = Vector2D(UR_A.x-UL_A.x,UR_A.y-UL_A.y);
    //compute projections
    Vector2D UR_B1 = Projection(UR_B,u1);
    Vector2D UL_B1 = Projection(UL_B,u1);
    Vector2D LL_B1 = Projection(LL_B,u1);
    Vector2D LR_B1 = Projection(LR_B,u1);

    Vector2D UR_A1 = Projection(UR_A,u1);
    Vector2D UL_A1 = Projection(UL_A,u1);

    Vector2D Max_A1 = MaxPoint(UR_A1,UL_A1);
    Vector2D Min_A1 = MinPoint(UR_A1,UL_A1);

    Vector2D Max_B1 = MaxPoint(UR_B1,UL_B1,LL_B1,LR_B1);
    Vector2D Min_B1 = MinPoint(UR_B1,UL_B1,LL_B1,LR_B1);

    std::cout << Min_B1.x << std::endl;
    std::cout << Min_A1.x << std::endl;
    std::cout << Max_B1.x << std::endl;
    std::cout << Max_A1.x << std::endl;

    if (Min_B1.x <= Max_A1.x)
    {
        if (Max_B1.x >= Min_A1.x)
        {
            t+=1;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    //Axis2

    std::cout<< "Axis2" << std::endl;

    //compute director vector
    Vector2D u2 = Vector2D(UR_A.x-LR_A.x,UR_A.y-LR_A.y);
    //compute projections
    Vector2D UR_B2 = Projection(UR_B,u2);
    Vector2D UL_B2 = Projection(UL_B,u2);
    Vector2D LL_B2 = Projection(LL_B,u2);
    Vector2D LR_B2 = Projection(LR_B,u2);

    Vector2D UR_A2 = Projection(UR_A,u2);
    Vector2D LR_A2 = Projection(LR_A,u2);

    Vector2D Max_A2 = MaxPoint(UR_A2,LR_A2);
    Vector2D Min_A2 = MinPoint(UR_A2,LR_A2);

    Vector2D Max_B2 = MaxPoint(UR_B2,UL_B2,LL_B2,LR_B2);
    Vector2D Min_B2 = MinPoint(UR_B2,UL_B2,LL_B2,LR_B2);

    if (Min_B2.x <= Max_A2.x)
    {
        if (Max_B2.x >= Min_A2.x)
        {
            t+=1;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    //Axis3

    std::cout<< "Axis3" << std::endl;

    //compute director vector
    Vector2D u3 = Vector2D(UL_B.x-LL_B.x,UL_B.y-LL_B.y);
    //compute projections
    Vector2D UR_A3 = Projection(UR_A,u3);
    Vector2D UL_A3 = Projection(UL_A,u3);
    Vector2D LL_A3 = Projection(LL_A,u3);
    Vector2D LR_A3 = Projection(LR_A,u3);

    Vector2D UL_B3 = Projection(UL_B,u3);
    Vector2D LL_B3 = Projection(LL_B,u3);

    Vector2D Max_B3 = MaxPoint(UL_B3,LL_B3);
    Vector2D Min_B3 = MinPoint(UL_B3,LL_B3);

    Vector2D Max_A3 = MaxPoint(UR_A3,UL_A3,LL_A3,LR_A3);
    Vector2D Min_A3 = MinPoint(UR_A3,UL_A3,LL_A3,LR_A3);

    if (Min_B3.x <= Max_A3.x)
    {
        if (Max_B3.x >= Min_A3.x)
        {
            t+=1;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    //Axis4

    std::cout<< "Axis4" << std::endl;

    //compute director vector
    Vector2D u4 = Vector2D(UL_B.x-UR_B.x,UL_B.y-UR_B.y);
    //compute projections
    Vector2D UR_A4 = Projection(UR_A,u4);
    Vector2D UL_A4 = Projection(UL_A,u4);
    Vector2D LL_A4 = Projection(LL_A,u4);
    Vector2D LR_A4 = Projection(LR_A,u4);

    Vector2D UR_B4 = Projection(UR_B,u4);
    Vector2D UL_B4 = Projection(UL_B,u4);

    Vector2D Max_B4 = MaxPoint(UL_B4,UR_B4);
    Vector2D Min_B4 = MinPoint(UL_B4,UR_B4);

    Vector2D Max_A4 = MaxPoint(UR_A4,UL_A4,LL_A4,LR_A4);
    Vector2D Min_A4 = MinPoint(UR_A4,UL_A4,LL_A4,LR_A4);

    if (Min_B4.x <= Max_A4.x)
    {
        if (Max_B4.x >= Min_A4.x)
        {
            t+=1;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    return true;
}

bool Collision::SAT(const ColliderComponent& colA, const ColliderComponent& colB)
{
    return SAT(colA.collider, colA.angle, colB.collider, colB.angle);
}

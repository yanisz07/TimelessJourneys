#include "Collision.hpp"
#include "ECS/ColliderComponent.hpp"
#include "ECS/collidercomponentcircle.h"
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

Vector2D ClosestVertex(Vector2D p1, Vector2D p2, Vector2D p3, Vector2D p4,const ColliderComponentCircle& cA)
{
    Vector2D tmp = p1;
    double distance = std::sqrt(std::pow(cA.center.x - p1.x, 2) + std::pow(cA.center.y - p1.y, 2));
    if (std::sqrt(std::pow(cA.center.x - p2.x, 2) + std::pow(cA.center.y - p2.y, 2)<distance))
    {
        tmp = p2;
    }
    if (std::sqrt(std::pow(cA.center.x - p3.x, 2) + std::pow(cA.center.y - p3.y, 2)<distance))
    {
        tmp = p3;
    }
    if (std::sqrt(std::pow(cA.center.x - p4.x, 2) + std::pow(cA.center.y - p4.y, 2)<distance))
    {
        tmp = p4;
    }
    return tmp;
}


Vector2D RotationTheta(Vector2D u, double theta)
{
    double thetaInRadians = toRadians(theta);
    double costheta = std::cos(thetaInRadians);
    double sinetheta = std::sin(thetaInRadians);
    return Vector2D(u.x*costheta-u.y*sinetheta,u.x*sinetheta+costheta*u.y);
}

Vector2D UpperLeft(const SDL_Rect &rec, double theta)
{
    Vector2D center = Vector2D(rec.x+rec.w/2,rec.y+rec.h/2);
    Vector2D UL = Vector2D(-rec.w/2,-rec.h/2);
    Vector2D UL_Rotated = RotationTheta(UL,theta);
    return Vector2D(center.x + UL_Rotated.x,center.y + UL_Rotated.y);
}

Vector2D UpperRight(const SDL_Rect &rec, double theta)
{
    Vector2D center = Vector2D(rec.x+rec.w/2,rec.y+rec.h/2);
    Vector2D UR = Vector2D(rec.w/2,-rec.h/2);
    Vector2D UR_Rotated = RotationTheta(UR,theta);
    return Vector2D(center.x+UR_Rotated.x,center.y+UR_Rotated.y);
}

Vector2D LowerLeft(const SDL_Rect &rec, double theta)
{
    Vector2D center = Vector2D(rec.x+rec.w/2,rec.y+rec.h/2);
    Vector2D LL = Vector2D(-rec.w/2,rec.h/2);
    Vector2D LL_Rotated = RotationTheta(LL,theta);
    return Vector2D(center.x + LL_Rotated.x,center.y + LL_Rotated.y);
}

Vector2D LowerRight(const SDL_Rect &rec, double theta)
{
    Vector2D center = Vector2D(rec.x+rec.w/2,rec.y+rec.h/2);
    Vector2D LR = Vector2D(rec.w/2,rec.h/2);
    Vector2D LR_Rotated = RotationTheta(LR,theta);
    return Vector2D(center.x + LR_Rotated.x,center.y + LR_Rotated.y);
}

double dotProduct(Vector2D e1, Vector2D e2)
{
    return e1.x*e2.x + e1.y*e2.y;
}

double norm(Vector2D v)
{
    return std::sqrt(std::pow(v.x,2)+std::pow(v.y,2));
}

void normalize(Vector2D& v)
{
    v = Vector2D(v.x/norm(v),v.y/norm(v));
}

Vector2D Projection(Vector2D v, Vector2D u)
{
    double value = dotProduct(v,u)/dotProduct(u,u);
    return Vector2D(value*u.x,value*u.y);
}

double MinPoint(Vector2D p1, Vector2D p2)
{
    if (p1.x ==0 && p2.x == 0)
    {
        if(p1.y < p2.y)
        {
            return p1.y;
        }
        else
        {
            return p2.y;
        }
    }
    else
    {
        if (p1.x < p2.x)
        {
            return p1.x;
        }
        return p2.x;
    }
}

double MinPoint(Vector2D p1, Vector2D p2, Vector2D p3, Vector2D p4)
{
    Vector2D tmp;
    tmp = p1;
    if (p1.x == 0 && p2.x ==0 && p3.x ==0 && p4.x ==0)
    {
        if (p2.y < tmp.y)
        {
            tmp = p2;
        }
        if (p3.y < tmp.y)
        {
            tmp = p3;
        }
        if (p4.y < tmp.y)
        {
            tmp = p4;
        }
        return tmp.y;
    }
    else
    {
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
        return tmp.x;
    }
}

double MaxPoint(Vector2D p1, Vector2D p2)
{
    if (p1.x ==0 && p2.x == 0)
    {
        if(p1.y > p2.y)
        {
            return p1.y;
        }
        else
        {
            return p2.y;
        }
    }
    else
    {
        if (p1.x > p2.x)
        {
            return p1.x;
        }
        return p2.x;
    }
}

double MaxPoint(Vector2D p1, Vector2D p2, Vector2D p3, Vector2D p4)
{
    Vector2D tmp;
    tmp = p1;
    if (p1.x == 0 && p2.x == 0 && p3.x == 0 && p4.x ==0)
    {
        if (p2.y > tmp.y)
        {
            tmp = p2;
        }
        if (p3.y > tmp.y)
        {
            tmp = p3;
        }
        if (p4.y > tmp.y)
        {
            tmp = p4;
        }
        return tmp.y;
    }
    else
    {
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
        return tmp.x;
    }
}

bool Collision::SAT(const SDL_Rect& recA, double angleA, const SDL_Rect& recB, double angleB)
{
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

    /*
    std::cout << "arrow" << std::endl;
    std::cout << recA.w << " " << recA.h << std::endl;
    std::cout << UR_A.x << " " << UR_A.y << std::endl;
    std::cout << UL_A.x << " " << UL_A.y << std::endl;
    std::cout << LR_A.x << " " << LR_A.y << std::endl;
    std::cout << LL_A.x << " " << LL_A.y << std::endl;

    std::cout << "bloc" << std::endl;
    std::cout << UR_B.x << " " << UR_B.y << std::endl;
    std::cout << UL_B.x << " " << UL_B.y << std::endl;
    std::cout << LR_B.x << " " << LR_B.y << std::endl;
    std::cout << LL_B.x << " " << LL_B.y << std::endl;
    */

    //Axis1
    //std::cout<< "Axis1" << std::endl;

    //compute director vector
    Vector2D u1 = Vector2D(UR_A.x-UL_A.x,UR_A.y-UL_A.y);

    //compute projections
    Vector2D UR_B1 = Projection(UR_B,u1);
    Vector2D UL_B1 = Projection(UL_B,u1);
    Vector2D LL_B1 = Projection(LL_B,u1);
    Vector2D LR_B1 = Projection(LR_B,u1);

    Vector2D UR_A1 = Projection(UR_A,u1);
    Vector2D UL_A1 = Projection(UL_A,u1);

    double Max_A1 = MaxPoint(UR_A1,UL_A1);
    double Min_A1 = MinPoint(UR_A1,UL_A1);

    double Max_B1 = MaxPoint(UR_B1,UL_B1,LL_B1,LR_B1);
    double Min_B1 = MinPoint(UR_B1,UL_B1,LL_B1,LR_B1);

    if (!(Min_B1 <= Max_A1))
    {
        return false;
    }
    else
    {
        if (!(Max_B1 >= Min_A1))
        {
            return false;
        }
    }

    //Axis2

    //std::cout<< "Axis2" << std::endl;

    //compute director vector
    Vector2D u2 = Vector2D(UR_A.x-LR_A.x,UR_A.y-LR_A.y);

    //compute projections
    Vector2D UR_B2 = Projection(UR_B,u2);
    Vector2D UL_B2 = Projection(UL_B,u2);
    Vector2D LL_B2 = Projection(LL_B,u2);
    Vector2D LR_B2 = Projection(LR_B,u2);

    Vector2D UR_A2 = Projection(UR_A,u2);
    Vector2D LR_A2 = Projection(LR_A,u2);

    double Max_A2 = MaxPoint(UR_A2,LR_A2);
    double Min_A2 = MinPoint(UR_A2,LR_A2);

    double Max_B2 = MaxPoint(UR_B2,UL_B2,LL_B2,LR_B2);
    double Min_B2 = MinPoint(UR_B2,UL_B2,LL_B2,LR_B2);

    if (!(Min_B2 <= Max_A2))
    {
        return false;
    }
    else
    {
        if (!(Max_B2 >= Min_A2))
        {
            return false;
        }
    }


    //Axis3

    //std::cout<< "Axis3" << std::endl;

    //compute director vector
    Vector2D u3 = Vector2D(UR_B.x-LR_B.x,UR_B.y-LR_B.y);
    //compute projections
    Vector2D UR_A3 = Projection(UR_A,u3);
    Vector2D UL_A3 = Projection(UL_A,u3);
    Vector2D LL_A3 = Projection(LL_A,u3);
    Vector2D LR_A3 = Projection(LR_A,u3);

    Vector2D UR_B3 = Projection(UR_B,u3);
    Vector2D LR_B3 = Projection(LR_B,u3);

    double Max_B3 = MaxPoint(UR_B3,LR_B3);
    double Min_B3 = MinPoint(UR_B3,LR_B3);

    double Max_A3 = MaxPoint(UR_A3,UL_A3,LL_A3,LR_A3);
    double Min_A3 = MinPoint(UR_A3,UL_A3,LL_A3,LR_A3);

    if (!(Min_B3 <= Max_A3))
    {
        return false;
    }
    else
    {
        if (!(Max_B3 >= Min_A3))
        {
            return false;
        }
    }

    //Axis4

    //std::cout<< "Axis4" << std::endl;

    //compute director vector
    Vector2D u4 = Vector2D(UR_B.x-UL_B.x,UR_B.y-UL_B.y);
    //compute projections
    Vector2D UR_A4 = Projection(UR_A,u4);
    Vector2D UL_A4 = Projection(UL_A,u4);
    Vector2D LL_A4 = Projection(LL_A,u4);
    Vector2D LR_A4 = Projection(LR_A,u4);

    Vector2D UR_B4 = Projection(UR_B,u4);
    Vector2D UL_B4 = Projection(UL_B,u4);

    double Max_B4 = MaxPoint(UL_B4,UR_B4);
    double Min_B4 = MinPoint(UL_B4,UR_B4);

    double Max_A4 = MaxPoint(UR_A4,UL_A4,LL_A4,LR_A4);
    double Min_A4 = MinPoint(UR_A4,UL_A4,LL_A4,LR_A4);

    if (!(Min_B4 <= Max_A4))
    {
        return false;
    }
    else
    {
        if (!(Max_B4 >= Min_A4))
        {
            return false;
        }
    }

    return true;
}

/*bool Collision::CollisionCircles(const ColliderComponentCircle& cA, const ColliderComponentCircle& cB)
{
    if (distance(cA.center,cB.center)<=cA.radius+cB.radius)
    {
        return true;
    }
    return false;
}*/

Vector2D Max(Vector2D p1, Vector2D p2)
{
    if (p1.x == 0 && p2.x == 0)
    {
        if(p1.y > p2.y)
        {
            return p1;
        }
        return p2;
    }
    if (p1.x > p2.x)
    {
        return p1;
    }
    return p2;
}

Vector2D Min(Vector2D p1, Vector2D p2)
{
    if (p1.x == 0 && p2.x == 0)
    {
        if(p1.y < p2.y)
        {
            return p1;
        }
        return p2;
    }
    if (p1.x < p2.x)
    {
        return p1;
    }
    return p2;
}

Vector2D Min(Vector2D p1, Vector2D p2, Vector2D p3, Vector2D p4)
{
    Vector2D tmp = p1;
    if (p1.x==0 && p2.x == 0 && p3.x == 0 && p4.x == 0)
    {
        if (p2.y < tmp.y)
        {
            tmp = p2;
        }
        if (p3.y < tmp.y)
        {
            tmp = p3;
        }
        if (p4.y < tmp.y)
        {
            tmp = p4;
        }
        return tmp;
    }
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

Vector2D Max(Vector2D p1, Vector2D p2, Vector2D p3, Vector2D p4)
{
    Vector2D tmp = p1;
    if (p1.x==0 && p2.x == 0 && p3.x == 0 && p4.x == 0)
    {
        if (p2.y > tmp.y)
        {
            tmp = p2;
        }
        if (p3.y > tmp.y)
        {
            tmp = p3;
        }
        if (p4.y > tmp.y)
        {
            tmp = p4;
        }
        return tmp;
    }
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

bool Collision::CollisionRectCircle(const ColliderComponent &colA, const ColliderComponentCircle &cA)
{
    SDL_Rect recA = colA.collider;
    double angleA = colA.angle;
    //corners of recA
    Vector2D UR_A = UpperRight(recA,angleA);
    Vector2D UL_A = UpperLeft(recA,angleA);
    Vector2D LR_A = LowerRight(recA,angleA);
    Vector2D LL_A = LowerLeft(recA,angleA);

    double r = cA.radius;
    Vector2D center = cA.center;
    ///*
    std::cout << "this is r" << std::endl;
    std::cout << r << std::endl;
    std::cout << center.x << " " << center.y << std::endl;

    std::cout << "this is rect" << std::endl;
    std::cout << UR_A.x << " " << UR_A.y << std::endl;
    std::cout << UL_A.x << " " << UL_A.y << std::endl;
    std::cout << LR_A.x << " " << LR_A.y << std::endl;
    std::cout << LL_A.x << " " << LL_A.y << std::endl;
    //*/
    //*/
    //Axis 1
    Vector2D u1 = Vector2D(UR_A.x-UL_A.x,UR_A.y-UL_A.y);

    Vector2D center1 = Projection(center,u1);

    Vector2D UR_A1 = Projection(UR_A,u1);
    Vector2D UL_A1 = Projection(UL_A,u1);

    Vector2D Max_A1 = Max(UR_A1,UL_A1);
    Vector2D Min_A1 = Min(UR_A1,UL_A1);

    double dmin1 = std::sqrt(std::pow(Min_A1.x - center1.x, 2) + std::pow(Min_A1.y - center1.y, 2));
    double dmax1 = std::sqrt(std::pow(Max_A1.x - center1.x, 2) + std::pow(Max_A1.y - center1.y, 2));
    double d1;

    if(dmin1 < dmax1)
    {
        d1 = dmin1;
    }
    else
    {
        d1 = dmax1;
    }

    std::cout << Max_A1.x << " " << Max_A1.y << std::endl;
    std::cout << Min_A1.x << " " << Min_A1.y << std::endl;
    std::cout << center1.x << " " << center1.y << std::endl;
    std::cout << "YAAAAAAAAA" << std::endl;
    std::cout << d1 << std::endl;
    std::cout << r << std::endl;
    std::cout << "YAAAAAA" << std::endl;

    if (center1.x==0)
    {
        if (center1.y < Min_A1.y)
        {
            if (d1>=r)
            {
                return false;
            }
        }
        else if(center1.y > Max_A1.y)
        {
            if(d1>=r)
            {
                return false;
            }
        }
    }
    else
    {
        if (center1.x < Min_A1.x)
        {
            if (d1>=r)
            {
                return false;
            }
        }
        else if(center1.x > Max_A1.x)
        {
            if(d1>=r)
            {
                return false;
            }
        }
    }

    //Axis 2
    Vector2D u2 = Vector2D(UR_A.x-LR_A.x,UR_A.y-LR_A.y);

    Vector2D center2 = Projection(center,u2);

    Vector2D UR_A2 = Projection(UR_A,u2);
    Vector2D LR_A2 = Projection(LR_A,u2);

    Vector2D Max_A2 = Max(UR_A2,LR_A2);
    Vector2D Min_A2 = Min(UR_A2,LR_A2);

    double dmin2 = std::sqrt(std::pow(Min_A2.x - center2.x, 2) + std::pow(Min_A2.y - center2.y, 2));
    double dmax2 = std::sqrt(std::pow(Max_A2.x - center2.x, 2) + std::pow(Max_A2.y - center2.y, 2));
    double d2;

    if(dmin2 < dmax2)
    {
        d2 = dmin2;
    }
    else
    {
        d2 = dmax2;
    }

    std::cout << Max_A2.x << " " << Max_A2.y << std::endl;
    std::cout << "YAAAAAAAAA" << std::endl;
    std::cout << Min_A2.x << " " << Min_A2.y << std::endl;
    std::cout << center2.x << " " << center2.y << std::endl;
    std::cout << d2 << std::endl;

    if (center2.x==0)
    {
        if (center2.y < Min_A2.y)
        {
            if (d2>=r)
            {
                return false;
            }
        }
        else if(center2.y > Max_A2.y)
        {
            if(d2>=r)
            {
                return false;
            }
        }
    }
    else
    {
        if (center2.x < Min_A2.x)
        {
            if (d2>=r)
            {
                return false;
            }
        }
        else if(center2.x > Max_A2.x)
        {
            if(d2>=r)
            {
                return false;
            }
        }
    }

    //Axis3
    Vector2D closestvertex = ClosestVertex(UR_A,UL_A,LR_A,LL_A,cA);
    ///*
    std::cout << closestvertex.x << " " << closestvertex.y << std::endl;
    //*/
    Vector2D u3 = Vector2D(closestvertex.x-center.x,closestvertex.y-center.y);

    Vector2D UR_A3 = Projection(UR_A,u3);
    Vector2D UL_A3 = Projection(UL_A,u3);
    Vector2D LR_A3 = Projection(LR_A,u3);
    Vector2D LL_A3 = Projection(LL_A,u3);

    Vector2D center3 = Projection(center,u3);

    Vector2D Min_A3 = Min(UR_A3,UL_A3,LR_A3,LL_A3);
    Vector2D Max_A3 = Max(UR_A3,UL_A3,LR_A3,LL_A3);

    double dmin3 = std::sqrt(std::pow(Min_A3.x - center3.x, 2) + std::pow(Min_A3.y - center3.y, 2));
    double dmax3 = std::sqrt(std::pow(Max_A3.x - center3.x, 2) + std::pow(Max_A3.y - center3.y, 2));
    double d3;

    if(dmin3 < dmax3)
    {
        d3 = dmin3;
    }
    else
    {
        d3 = dmax3;
    }

    if (center3.x==0)
    {
        if (center3.y < Min_A3.y)
        {
            if (d3>=r)
            {
                return false;
            }
        }
        else if(center3.y > Max_A3.y)
        {
            if(d3>=r)
            {
                return false;
            }
        }
    }
    else
    {
        if (center3.x < Min_A3.x)
        {
            if (d3>=r)
            {
                return false;
            }
        }
        else if(center3.x > Max_A3.x)
        {
            if(d3>=r)
            {
                return false;
            }
        }
    }

    return true;
}

bool Collision::SAT(const ColliderComponent& colA, const ColliderComponent& colB)
{
    return SAT(colA.collider, colA.angle, colB.collider, colB.angle);
}

bool Collision::CheckCollision(const ColliderComponent& colA, const ColliderComponent& colB)
{
    if (colA.angle==0 && colB.angle==0)
    {
        return AABB(colA,colB);
    }
    else
    {
        return SAT(colA,colB);
    }
}


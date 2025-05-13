#include "Student.h"

#include <osg/Plane>
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

osg::Matrix inverse(const osg::Matrix& m)
{
    return osg::Matrix::inverse(m);
}

double degreesToRadians(double angleInDegrees)
{
    return osg::DegreesToRadians(angleInDegrees);
}

osg::Matrix getScaleMatrix(const osg::Vec3d& scale)
{
    //Task 1 - fill in the scale matrix
    osg::Matrix m(  scale.x(), 0, 0, 0,
                    0, scale.y(), 0, 0,
                    0, 0, scale.z(), 0,
                    0, 0, 0, 1);
    return m;
}

osg::Matrix getTranslationMatrix(const osg::Vec3d& translation)
{
    //Task 2 - fill in the translation matrix
    osg::Matrix m(  1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    translation.x(), translation.y(), translation.z(), 1);
    return m;
}

osg::Matrix rotateAroundX(double angleInRadians)
{
    double sina = std::sin(angleInRadians);
    double cosa = std::cos(angleInRadians);

    //Task 3 - fill in the rotation matrix around X axis
    osg::Matrix m(  1, 0, 0, 0,
                    0, cosa, sina, 0,
                    0, -sina, cosa, 0,
                    0, 0, 0, 1);
    return m;
}

osg::Matrix rotateAroundY(double angleInRadians)
{
    double sina = std::sin(angleInRadians);
    double cosa = std::cos(angleInRadians);

    //Task 4 - fill in the rotation matrix around Y axis
    osg::Matrix m(  cosa, 0, -sina, 0,
                    0, 1, 0, 0,
                    sina, 0, cosa, 0,
                    0, 0, 0, 1);
    return m;
}

osg::Matrix rotateAroundZ(double angleInRadians)
{
    double sina = std::sin(angleInRadians);
    double cosa = std::cos(angleInRadians);

    //Task 5 - fill in the rotation matrix around Z axis
    osg::Matrix m(  cosa, sina, 0, 0,
                    -sina, cosa, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1);
    return m;
}

osg::Matrix getForwardTransformation()
{
    //Task 6 - fill in forward transformation matrix consisting of 3 individual transformation matrices
    osg::Matrix m(  1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1);

    //transl 2 
    osg::Vec3d vT(2, 2, 2);
    m *= getTranslationMatrix(vT);
    //scale 2 
    m *= getScaleMatrix(vT);
    //rot y 45
    m *= rotateAroundY(degreesToRadians(-45));

    return m;
}
osg::Matrix getBackwardTransformation()
{
    //Task 7 - fill in backward transformation matrix consisting of 3 individual transformation matrices (you can use inverse if it helps you).
    osg::Matrix m(  1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1);

    //translate 1 z 1 y
    osg::Vec3d vT(0, -2, -2);
    m *= getTranslationMatrix(vT);
    //scale 2
    osg::Vec3d vS(2, 2, 2);
    m *= getScaleMatrix(vS);
    //rotate x 45
    m *= rotateAroundX(degreesToRadians(-45));

    //You HAVE TO return inverse of m
    return inverse(m);
}

#include <gtest/gtest.h>


#include <Student.h>
//#include <Student.cpp> //Extremely dirty trick, DON'T DO THIS EVER!! I was just lazy to create a library out of Student.cpp/h.

#include <tests/CustomListener.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <osg/Geode>
#include <osg/Node>
#include <osg/Geometry>
#include <osgDB/ReadFile>

osg::Vec3d toEuler(const osg::Quat& q)
{
    double sqx = q.x() * q.x();
    double sqy = q.y() * q.y();
    double sqz = q.z() * q.z();
    double sqw = q.w() * q.w();

    double unit = sqx + sqy + sqz + sqw;
    double test = q.x() * q.y() + q.z() * q.w();
    double x, y, z;

    if (test > 0.499)
    {
        y = 2 * std::atan2(q.x(), q.w());
        z = M_PI * 0.5;
        x = 0.0;
    }
    else if (test < -0.499)
    {
        y = -2 * std::atan2(q.x(), q.w());
        z = -M_PI * 0.5;
        x = 0.0;
    }
    else
    {
        y = atan2(2 * q.y() * q.w() - 2 * q.x() * q.z(), sqx - sqy - sqz + sqw);
        z = asin(2 * test / unit);
        x = atan2(2 * q.x() * q.w() - 2 * q.y() * q.z(), -sqx + sqy - sqz + sqw);
    }

    return osg::Vec3d(x, y, z);
}


void compareMatrices(const osg::Matrix& m1, const osg::Matrix& m2, const std::string& function)
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            EXPECT_NEAR(m1(i, j), m2(i, j), 1e-5) << "#" << "Failure in: " << function << ": at m[" << i << "," << j << "]"
                                                  << ", your value: " << m1(i, j) << " expected: " << m2(i, j);
        }

    }
}

TEST(MatricesTest, getTranslationMatrix)
{
    osg::Vec3d translation(1, 0, 0);

    compareMatrices(getTranslationMatrix(translation), osg::Matrix::translate(translation), "getTranslationMatrix");

    translation = { 0, 1, 0 };
    compareMatrices(getTranslationMatrix(translation), osg::Matrix::translate(translation), "getTranslationMatrix");

    translation = { 0, 0, 1 };
    compareMatrices(getTranslationMatrix(translation), osg::Matrix::translate(translation), "getTranslationMatrix");

    translation = { 0, 10, 1 };
    compareMatrices(getTranslationMatrix(translation), osg::Matrix::translate(translation), "getTranslationMatrix");

    translation = { 15, 0, 1 };
    compareMatrices(getTranslationMatrix(translation), osg::Matrix::translate(translation), "getTranslationMatrix");
}

TEST(MatricesTest, getScaleMatrix)
{
    osg::Vec3d scale(1, 0, 0);

    compareMatrices(getScaleMatrix(scale), osg::Matrix::scale(scale), "getScaleMatrix");

    scale = { 0, 1, 0 };
    compareMatrices(getScaleMatrix(scale), osg::Matrix::scale(scale), "getScaleMatrix");

    scale = { 0, 0, 1 };
    compareMatrices(getScaleMatrix(scale), osg::Matrix::scale(scale), "getScaleMatrix");

    scale = { 0, 2, 1 };
    compareMatrices(getScaleMatrix(scale), osg::Matrix::scale(scale), "getScaleMatrix");

    scale = { 10, 0, 1 };
    compareMatrices(getScaleMatrix(scale), osg::Matrix::scale(scale), "getScaleMatrix");
}

TEST(MatricesTest, RotationMatrixTest)
{
    double angle = osg::DegreesToRadians(45.0);

    const osg::Vec3d xAxis(1, 0, 0);
    const osg::Vec3d yAxis(0, 1, 0);
    const osg::Vec3d zAxis(0, 0, 1);

    auto mine = rotateAroundX(angle);
    auto osg = osg::Matrix::rotate(angle, xAxis);
    
    compareMatrices(rotateAroundX(angle), osg::Matrix::rotate(angle, xAxis), "rotateAroundX");
    compareMatrices(rotateAroundY(angle), osg::Matrix::rotate(angle, yAxis), "rotateAroundY");
    compareMatrices(rotateAroundZ(angle), osg::Matrix::rotate(angle, zAxis), "rotateAroundZ");


    angle = osg::DegreesToRadians(0.0);
    compareMatrices(rotateAroundX(angle), osg::Matrix::rotate(angle, xAxis), "rotateAroundX");
    compareMatrices(rotateAroundY(angle), osg::Matrix::rotate(angle, yAxis), "rotateAroundY");
    compareMatrices(rotateAroundZ(angle), osg::Matrix::rotate(angle, zAxis), "rotateAroundZ");


    angle = osg::DegreesToRadians(-15.0);
    compareMatrices(rotateAroundX(angle), osg::Matrix::rotate(angle, xAxis), "rotateAroundX");
    compareMatrices(rotateAroundY(angle), osg::Matrix::rotate(angle, yAxis), "rotateAroundY");
    compareMatrices(rotateAroundZ(angle), osg::Matrix::rotate(angle, zAxis), "rotateAroundZ");
}
//TODO: move it to some utils.h
void collectVertices(osg::Node* node, osg::Vec3Array* vertices)
{
    osg::Geode* geode = dynamic_cast<osg::Geode*>(node);
    if (geode)
    {
        for (unsigned int i = 0; i < geode->getNumDrawables(); ++i)
        {
            osg::Geometry* geometry = dynamic_cast<osg::Geometry*>(geode->getDrawable(i));
            if (geometry)
            {
                osg::Vec3Array* geomVertices = dynamic_cast<osg::Vec3Array*>(geometry->getVertexArray());
                if (geomVertices)
                {
                    vertices->insert(vertices->end(), geomVertices->begin(), geomVertices->end());
                }
            }
        }
    }
    else
    {
        osg::Group* group = dynamic_cast<osg::Group*>(node);
        if (group)
        {
            for (unsigned int i = 0; i < group->getNumChildren(); ++i)
            {
                collectVertices(group->getChild(i), vertices);
            }
        }
    }
}

bool areModelsIdentical(osg::ref_ptr<osg::Node>  modelRef, osg::ref_ptr<osg::Node>  model2, osg::Matrix transformMatrix)
{
    const double epsilon = 1e-5;

    // Ensure both models are valid
    if (!modelRef || !model2)
        return false;

    // Collect vertices from model1
    osg::ref_ptr<osg::Vec3Array> vertices1 = new osg::Vec3Array;
    collectVertices(modelRef, vertices1);


    // Collect vertices from loadedModel2
    osg::ref_ptr<osg::Vec3Array> vertices2 = new osg::Vec3Array;
    collectVertices(model2, vertices2);

    // Check if the number of vertices is the same
    if (vertices1->size() != vertices2->size())
        return false;

    // Compare vertex positions
    for (unsigned int i = 0; i < vertices1->size(); ++i)
    {
        osg::Vec3 transformedVertex = (*vertices2)[i] * transformMatrix;
        double distance = ((*vertices1)[i] - transformedVertex).length();
        if (std::isnan(distance) || distance > epsilon)
        {
            return false; // Vertices are not identical (within epsilon)
        }
    }

    // Models are identical
    return true;
}


TEST(TransformationTest, Forward)
{
    osg::setNotifyLevel(osg::FATAL); // Suppress debug output

    auto m = getForwardTransformation();

    osg::ref_ptr<osg::Node> ref = osgDB::readNodeFile("transformed_bunny.obj");
    osg::ref_ptr<osg::Node> in = osgDB::readNodeFile("bunny.obj");

    bool res = areModelsIdentical(ref, in, m);

    EXPECT_EQ(res, true) << "#" << "Failure in: " << "forward transformation.";
}

TEST(TransformationTest, BackWard)
{
    osg::setNotifyLevel(osg::FATAL); // Suppress debug output

    auto m = getBackwardTransformation();

    osg::ref_ptr<osg::Node> ref = osgDB::readNodeFile("monkey.obj");
    osg::ref_ptr<osg::Node> in = osgDB::readNodeFile("transformed_monkey.obj");
    
    bool res = areModelsIdentical(ref, in, m);

    EXPECT_EQ(res, true) << "#" << "Failure in: " << "backward transformation.";
}
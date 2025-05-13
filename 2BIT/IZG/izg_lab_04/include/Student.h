#pragma once

#include <osg/Matrix>
#include <osg/Vec3>
 
/*------------------------------HELPER FUNCTIONS-------------------------*/
//! \brief                  Returns inversed matrix.
//! \param  m               Matrix to be inversed.
osg::Matrix inverse(const osg::Matrix& m);

//! \brief                      Converts degrees to radians.
//! \param  angleInDegrees      Angle in degrees.
double degreesToRadians(double v);

/*------------------------------STUDENT TASKS-------------------------*/

//! \brief                  Task 1: Return scale matrix.
//! \param  scale           Vector of scale in each direcion.
osg::Matrix getScaleMatrix(const osg::Vec3d& scale);

//! \brief                  Task 2: Return translation matrix.
//! \param  translation     Vector of translation.
osg::Matrix getTranslationMatrix(const osg::Vec3d& translation);

//! \brief                  Task 3: Return rotation matrix around X axis.
//! \param  angle           Angle of rotation in RADIANS.
osg::Matrix rotateAroundX(double angleInRadians);

//! \brief                  Task 4: Return rotation matrix around Y axis.
//! \param  angle           Angle of rotation in RADIANS.
osg::Matrix rotateAroundY(double angleInRadians);

//! \brief                  Task 5: Return rotation matrix around Z axis.
//! \param  angle           Angle of rotation in RADIANS.
osg::Matrix rotateAroundZ(double angleInRadians);

osg::Matrix getForwardTransformation();
osg::Matrix getBackwardTransformation();

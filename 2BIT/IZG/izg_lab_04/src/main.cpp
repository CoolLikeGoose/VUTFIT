#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osg/CoordinateSystemNode>
#include <osgText/Text>
#include <osg/StateAttribute>
#include <osg/GL>
#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Node>
#include <osg/MatrixTransform>
#include <osg/Geometry>
#include <osg/Material>
#include <osg/io_utils>
#include <osg/PolygonMode>
#include <osg/LineWidth>

#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

#include "Student.h"

void makeWindowed(osgViewer::Viewer& viewer)
{
    osg::GraphicsContext::WindowingSystemInterface* wsi = osg::GraphicsContext::getWindowingSystemInterface();
    if (!wsi)
    {
        osg::notify(osg::NOTICE) << "Error, no WindowSystemInterface available, cannot create windows." << std::endl;
        return;
    }

    unsigned int width, height;
    osg::GraphicsContext::ScreenIdentifier main_screen_id;

    main_screen_id.readDISPLAY();
    main_screen_id.setUndefinedScreenDetailsToDefaultScreen();
    wsi->getScreenResolution(main_screen_id, width, height);

    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
    traits->x = 0;
    traits->y = 30;
    traits->width = width;
    traits->height = height - traits->y;
    traits->sharedContext = 0;
    traits->readDISPLAY();
    traits->setUndefinedScreenDetailsToDefaultScreen();
    traits->windowName = "IZG 04";
    traits->windowDecoration = true;
    traits->doubleBuffer = true;
    traits->samples = 4;

    osg::DisplaySettings::instance()->setNumMultiSamples(6);

    osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
    if (gc.valid())
    {
        osg::notify(osg::INFO) << "  GraphicsWindow has been created successfully." << std::endl;

        // need to ensure that the window is cleared make sure that the complete window is set the correct colour
        // rather than just the parts of the window that are under the camera's viewports
        gc->setClearColor(osg::Vec4f(0.2f, 0.2f, 0.6f, 1.0f));
        gc->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    else
    {
        osg::notify(osg::NOTICE) << "  GraphicsWindow has not been created successfully." << std::endl;
    }

    viewer.setUpViewInWindow(traits->x, traits->y, traits->width, traits->height);
}

osg::ref_ptr<osg::Geode> createAxes()
{
    float axesLength = 10.0f;

    // Create a geode to hold our axes
    osg::ref_ptr<osg::Geode> axesGeode = new osg::Geode;

    // Create geometry for each axis
    osg::ref_ptr<osg::Geometry> xAxisGeom = new osg::Geometry;
    osg::ref_ptr<osg::Geometry> yAxisGeom = new osg::Geometry;
    osg::ref_ptr<osg::Geometry> zAxisGeom = new osg::Geometry;

    // Create vertices for each axis
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    vertices->push_back(osg::Vec3(0.0f, 0.0f, 0.0f));  // Start at origin
    vertices->push_back(osg::Vec3(axesLength, 0.0f, 0.0f));  // X-axis endpoint
    vertices->push_back(osg::Vec3(0.0f, 0.0f, 0.0f));  // Start at origin
    vertices->push_back(osg::Vec3(0.0f, axesLength, 0.0f));  // Y-axis endpoint
    vertices->push_back(osg::Vec3(0.0f, 0.0f, 0.0f));  // Start at origin
    vertices->push_back(osg::Vec3(0.0f, 0.0f, axesLength));  // Z-axis endpoint

    // Set the vertices
    xAxisGeom->setVertexArray(vertices);
    yAxisGeom->setVertexArray(vertices);
    zAxisGeom->setVertexArray(vertices);

    osg::ref_ptr<osg::Vec4Array> colorsX = new osg::Vec4Array;
    colorsX->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f)); // Red
    xAxisGeom->setColorArray(colorsX.get());
    xAxisGeom->setColorBinding(osg::Geometry::BIND_OVERALL);

    osg::ref_ptr<osg::Vec4Array> colorsY = new osg::Vec4Array;
    colorsY->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f)); // Green
    yAxisGeom->setColorArray(colorsY.get());
    yAxisGeom->setColorBinding(osg::Geometry::BIND_OVERALL);

    osg::ref_ptr<osg::Vec4Array> colorsZ = new osg::Vec4Array;
    colorsZ->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f)); // Blue
    zAxisGeom->setColorArray(colorsZ.get());
    zAxisGeom->setColorBinding(osg::Geometry::BIND_OVERALL);

    // Create a drawable for each axis
    xAxisGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 2));
    yAxisGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 2, 2));
    zAxisGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 4, 2));

    // Create a geode to hold the axes and add the geometry to it
    axesGeode->addDrawable(xAxisGeom);
    axesGeode->addDrawable(yAxisGeom);
    axesGeode->addDrawable(zAxisGeom);

    // Set line width for better visualization
    osg::ref_ptr<osg::LineWidth> lineWidth = new osg::LineWidth;
    lineWidth->setWidth(2.0f);
    axesGeode->getOrCreateStateSet()->setAttribute(lineWidth);

    //Add cones at the end
    float coneHeight = 0.2f;

    {
        osg::ref_ptr<osg::Geode> coneXGeode = new osg::Geode;
        osg::ref_ptr<osg::Cone> coneX = new osg::Cone(osg::Vec3(0.0f, 0.0f, 0.0f), coneHeight * 0.25f, coneHeight);
        osg::ref_ptr <osg::ShapeDrawable> coneXDrawable = new osg::ShapeDrawable(coneX.get());
        // coneXGeode->setColor(osg::Vec4(0.0f,0.0f,1.0f,1.0f));

        osg::ref_ptr<osg::MatrixTransform> transformConeX = new osg::MatrixTransform;
        osg::Quat quat;
        quat.makeRotate(M_PI/2.0, osg::Vec3(0.0f, 1.0f, 0.0f));
        transformConeX->setMatrix(osg::Matrix(quat) * osg::Matrix::translate(axesLength, 0, 0)); //first * second

        coneXGeode->addDrawable(coneXDrawable);
        transformConeX->addChild(coneXGeode);

        axesGeode->addChild(transformConeX);
    }

    {
        osg::ref_ptr<osg::Geode> coneYGeode = new osg::Geode;
        osg::ref_ptr<osg::Cone> coneY = new osg::Cone(osg::Vec3(0.0f, 0.0f, 0.0f), coneHeight * 0.25f, coneHeight);
        osg::ref_ptr <osg::ShapeDrawable> coneYDrawable = new osg::ShapeDrawable(coneY.get());
        // coneYDrawable->setColor(osg::Vec4(0.0f,0.0f,1.0f,1.0f));

        osg::ref_ptr<osg::MatrixTransform> transformConeY = new osg::MatrixTransform;
        osg::Quat quat;
        quat.makeRotate(M_PI / 2.0, osg::Vec3(-1.0f, 0.0f, 0.0f));
        transformConeY->setMatrix(osg::Matrix(quat) * osg::Matrix::translate(0, axesLength, 0)); //first * second

        coneYGeode->addDrawable(coneYDrawable);
        transformConeY->addChild(coneYGeode);

        axesGeode->addChild(transformConeY);
    }

    {
        osg::ref_ptr<osg::Cone> coneZ = new osg::Cone(osg::Vec3(0.0f, 0.0f, axesLength), coneHeight * 0.25f, coneHeight);
        osg::ref_ptr <osg::ShapeDrawable> coneZDrawable = new osg::ShapeDrawable(coneZ.get());
        // coneDrawable->setColor(osg::Vec4(0.0f,0.0f,1.0f,1.0f));
        axesGeode->addDrawable(coneZDrawable);
    }

    // Disable lighting for the axes geode to prevent shading
    osg::ref_ptr<osg::StateSet> stateSet = axesGeode->getOrCreateStateSet();
    stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

    // Set a constant color for the axes
    osg::ref_ptr<osg::Material> material = new osg::Material;
    material->setColorMode(osg::Material::ColorMode::OFF);
    stateSet->setAttributeAndModes(material, osg::StateAttribute::ON);

    return axesGeode;
}

osg::ref_ptr<osg::MatrixTransform> loadModelAsWireframeOnSolid(std::string fn, osg::Matrix additionalTransform = osg::Matrix::identity())
{
    osg::ref_ptr<osg::MatrixTransform> transform = new osg::MatrixTransform;
    transform->setMatrix(additionalTransform); 

    osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFile(fn);


    // Clone the loaded model to render it as solid
    osg::ref_ptr<osg::Node> wireframeModel = dynamic_cast<osg::Node*>(loadedModel->clone(osg::CopyOp::DEEP_COPY_ALL));
    osg::ref_ptr<osg::PolygonMode> pm = new osg::PolygonMode;
    pm->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
    wireframeModel->getOrCreateStateSet()->setAttributeAndModes(pm, osg::StateAttribute::ON);

    // Set the wireframe color
    osg::ref_ptr<osg::Material> wireframeMaterial = new osg::Material;
    wireframeMaterial->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
    wireframeMaterial->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(0.0, 0.0, 0.0, 1.0)); 
    wireframeModel->getOrCreateStateSet()->setAttributeAndModes(wireframeMaterial, osg::StateAttribute::ON);

    transform->addChild(loadedModel);
    transform->addChild(wireframeModel);

    return transform;
}

osg::ref_ptr<osg::MatrixTransform> loadModelAsWireframe(std::string fn, float lineThickness = 1.0f,  osg::Vec4 color = osg::Vec4(0.0, 0.0, 0.0, 1.0))
{
    osg::ref_ptr<osg::MatrixTransform> transform = new osg::MatrixTransform;

    osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFile(fn);


     //Set the rendering mode to wireframe for the loaded model
    osg::ref_ptr<osg::PolygonMode> pm = new osg::PolygonMode;
    pm->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
    loadedModel->getOrCreateStateSet()->setAttributeAndModes(pm, osg::StateAttribute::ON);

    // Set the wireframe color
    osg::ref_ptr<osg::Material> wireframeMaterial = new osg::Material;
    wireframeMaterial->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
    wireframeMaterial->setDiffuse(osg::Material::FRONT_AND_BACK, color);

    // Set line width
    osg::LineWidth* lineWidthAttr = new osg::LineWidth(lineThickness);
    loadedModel->getOrCreateStateSet()->setAttribute(lineWidthAttr);

    loadedModel->getOrCreateStateSet()->setAttributeAndModes(wireframeMaterial, osg::StateAttribute::ON);

    transform->addChild(loadedModel);

    return transform;
}

osg::ref_ptr<osg::Node> loadModelAsSolid(std::string fn)
{
    osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFile(fn);

    osg::ref_ptr<osg::Material> matirial = new osg::Material;
    matirial->setColorMode(osg::Material::DIFFUSE);
    matirial->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(0, 0, 0, 1));
    matirial->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(0.9, 0.3, 0.4, 1.0));
    matirial->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(1, 1, 1, 1));
    matirial->setShininess(osg::Material::FRONT_AND_BACK, 64.0f);
    loadedModel->getOrCreateStateSet()->setAttributeAndModes(matirial.get(), osg::StateAttribute::ON);

    return loadedModel;
}

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

bool areModelsIdentical(osg::Node* model1, osg::Node* model2) 
{
    const double epsilon = 1e-5;

    // Ensure both models are valid
    if (!model1 || !model2)
        return false;

    // Convert model2 to MatrixTransform if necessary
    osg::MatrixTransform* transform1 = dynamic_cast<osg::MatrixTransform*>(model1);
    // Ensure model1 is a MatrixTransform
    if (!transform1)
        return false;

    osg::Matrix transformMatrix1 = transform1->getMatrix();
    // Get the child node (loadedModel1)
    osg::Node* loadedModel1 = transform1->getChild(0);

    // Collect vertices from model1
    osg::ref_ptr<osg::Vec3Array> vertices1 = new osg::Vec3Array;
    collectVertices(loadedModel1, vertices1);

    // Convert model2 to MatrixTransform if necessary
    osg::MatrixTransform* transform2 = dynamic_cast<osg::MatrixTransform*>(model2);
    // Ensure model2 is a MatrixTransform
    if (!transform2)
        return false;

    osg::Matrix transformMatrix2 = transform2->getMatrix();


    // Get the child node (loadedModel2)
    osg::Node* loadedModel2 = transform2->getChild(0);

    // Collect vertices from loadedModel2
    osg::ref_ptr<osg::Vec3Array> vertices2 = new osg::Vec3Array;
    collectVertices(loadedModel2, vertices2);

    // Check if the number of vertices is the same
    if (vertices1->size() != vertices2->size())
        return false;

     // Compare vertex positions
    for (unsigned int i = 0; i < vertices1->size(); ++i)
    {
        osg::Vec3 transformedVertex1 = (*vertices1)[i] * transformMatrix1;
        osg::Vec3 transformedVertex2 = (*vertices2)[i] * transformMatrix2;
        double distance = (transformedVertex1 - transformedVertex2).length();
        if (std::isnan(distance) || distance > epsilon)
        {
            return false; // Vertices are not identical (within epsilon)
        }
    }

    // Models are identical
    return true;
}

class MyKeyboardEventHandler : public osgGA::GUIEventHandler 
{
public: //types
    enum eTask
    {
        FIRST_TASK = 0,
        SECOND_TASK
    };
public:

    MyKeyboardEventHandler(osg::ref_ptr<osg::MatrixTransform> tf, 
                           osg::ref_ptr<osg::MatrixTransform> forwardReference, 
                           osg::ref_ptr<osg::MatrixTransform> forwardStep1, osg::ref_ptr<osg::MatrixTransform> forwardStep2,
                           osg::ref_ptr<osg::MatrixTransform> tb, 
                           osg::ref_ptr<osg::MatrixTransform> backwardStep1, osg::ref_ptr<osg::MatrixTransform> backwardStep2, 
                           osg::ref_ptr<osg::MatrixTransform> backwardReference) :
        m_currentTask(FIRST_TASK),
        m_forwardTransformationNode(tf),
        m_backwardTransformationNode(tb),
        m_forwardStep1(forwardStep1),
        m_forwardStep2(forwardStep2),
        m_forwardTransformationReference(forwardReference),
        m_backwardTransformationReference(backwardReference),
        m_backwardStep1(backwardStep1),
        m_backwardStep2(backwardStep2),
        m_modelsVisible(true)
    {
    }

    virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter&)
    {
        if (ea.getEventType() == osgGA::GUIEventAdapter::KEYDOWN) 
        {
            //if (ea.getKey() == osgGA::GUIEventAdapter::KEY_H)
            if (ea.getKey() == osgGA::GUIEventAdapter::KEY_B)
            {
                m_modelsVisible = !m_modelsVisible;
                showMainModels();
                return true; // Consume the event
            }

            if (ea.getKey() == osgGA::GUIEventAdapter::KEY_1)
            {
                m_currentTask = FIRST_TASK;
                switchTasks();
                return true; // Consume the event
            }
            if (ea.getKey() == osgGA::GUIEventAdapter::KEY_2)
            {
                m_currentTask = SECOND_TASK;
                switchTasks();
                return true; // Consume the event
            }

            if (ea.getKey() == osgGA::GUIEventAdapter::KEY_A) 
            {
                applyTransform();
                return true; // Consume the event
            }

            if (ea.getKey() == osgGA::GUIEventAdapter::KEY_R)
            {
                resetTransforms();
                return true; // Consume the event
            }

            if (ea.getKey() == osgGA::GUIEventAdapter::KEY_X)
            {
                applyInverseTransformOnTarget();
                return true; // Consume the event
            }
            
        }
        return false; // Continue handling
    }

    void changeColor(osg::ref_ptr<osg::Node> node, bool correct)
    {
        // Convert model2 to MatrixTransform if necessary
        osg::MatrixTransform* t = dynamic_cast<osg::MatrixTransform*>(node.get());
        // Ensure model2 is a MatrixTransform
        if (!t)
            return;

        osg::Node* loadedModel = t->getChild(0);

        if (!loadedModel)
            return;

        // Get the state set associated with the node
        osg::StateSet* stateSet = loadedModel->getOrCreateStateSet();

        // Check if the state set has a material
        osg::Material* material = dynamic_cast<osg::Material*>(stateSet->getAttribute(osg::StateAttribute::MATERIAL));

        if (material) 
        {
            if (correct)
            {
                material->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
            }
            else
            {
                material->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
            }
        }
    }

    void toggleNodeVisibility(osg::Node* node, bool visible) 
    {
        if (node) 
        {
            if (visible) 
            {
                node->setNodeMask(~0); // Show the node by setting the node mask to all 1s
            }
            else 
            {
                node->setNodeMask(0); // Hide the node by setting the node mask to 0
            }
        }
    }

    void switchTasks()
    {
        m_modelsVisible = true;
        switch (m_currentTask)
        {
        case MyKeyboardEventHandler::FIRST_TASK:
            toggleNodeVisibility(m_forwardTransformationNode, true);
            toggleNodeVisibility(m_forwardTransformationReference, true);
            toggleNodeVisibility(m_forwardStep1, true);
            toggleNodeVisibility(m_forwardStep2, true);

            toggleNodeVisibility(m_backwardTransformationReference, false);
            toggleNodeVisibility(m_backwardTransformationNode, false);
            toggleNodeVisibility(m_backwardStep1, false);
            toggleNodeVisibility(m_backwardStep2, false);
            break;
        case MyKeyboardEventHandler::SECOND_TASK:
            toggleNodeVisibility(m_forwardTransformationNode, false);
            toggleNodeVisibility(m_forwardTransformationReference, false);
            toggleNodeVisibility(m_forwardStep1, false);
            toggleNodeVisibility(m_forwardStep2, false);

            toggleNodeVisibility(m_backwardTransformationReference, true);
            toggleNodeVisibility(m_backwardTransformationNode, true);
            toggleNodeVisibility(m_backwardStep1, true);
            toggleNodeVisibility(m_backwardStep2, true);
            break;
        default:
            break;
        }

    }

    void resetTransforms()
    {
        m_forwardTransformationNode->setMatrix(osg::Matrix::identity());
        m_backwardTransformationNode->setMatrix(osg::Matrix::identity());
        m_forwardTransformationReference->setMatrix(osg::Matrix::identity());
        m_backwardTransformationReference->setMatrix(osg::Matrix::identity());

        changeColor(m_forwardTransformationReference, areModelsIdentical(m_forwardTransformationReference, m_forwardTransformationNode));
        changeColor(m_backwardTransformationReference, areModelsIdentical(m_backwardTransformationReference, m_backwardTransformationNode));
    }

    void applyTransform()
    {
        switch (m_currentTask)
        {
        case MyKeyboardEventHandler::FIRST_TASK:
        {
            auto m = getForwardTransformation();
            m_forwardTransformationNode->setMatrix(m_forwardTransformationNode->getMatrix() * m);

            changeColor(m_forwardTransformationReference, areModelsIdentical(m_forwardTransformationReference, m_forwardTransformationNode));
        }
            break;
        case MyKeyboardEventHandler::SECOND_TASK:
        {
            auto m = getBackwardTransformation();
            m_backwardTransformationNode->setMatrix(m_backwardTransformationNode->getMatrix() * m);

            changeColor(m_backwardTransformationReference, areModelsIdentical(m_backwardTransformationReference, m_backwardTransformationNode));
        }
            break;
        default:
            break;
        }

    }

    void applyInverseTransformOnTarget()
    {
        switch (m_currentTask)
        {
        case MyKeyboardEventHandler::FIRST_TASK:
        {
            auto m = getForwardTransformation();
            m_forwardTransformationReference->setMatrix(m_forwardTransformationReference->getMatrix() * osg::Matrix::inverse(m));
            changeColor(m_forwardTransformationReference, areModelsIdentical(m_forwardTransformationReference, m_forwardTransformationNode));

        }
        break;
        case MyKeyboardEventHandler::SECOND_TASK:
        {
            auto m = getBackwardTransformation();
            m_backwardTransformationReference->setMatrix(m_forwardTransformationReference->getMatrix() * osg::Matrix::inverse(m));
            changeColor(m_backwardTransformationReference, areModelsIdentical(m_backwardTransformationReference, m_backwardTransformationNode));

        }
        break;
        default:
            break;
        }
    }

    void showMainModels()
    {
        switch (m_currentTask)
        {
        case MyKeyboardEventHandler::FIRST_TASK:
            toggleNodeVisibility(m_forwardTransformationNode, m_modelsVisible);
            break;
        case MyKeyboardEventHandler::SECOND_TASK:
            toggleNodeVisibility(m_backwardTransformationNode, m_modelsVisible);
            break;
        default:
            break;
        }
    }

private: //properties
    eTask m_currentTask;
    bool m_modelsVisible;

    osg::ref_ptr<osg::MatrixTransform> m_forwardTransformationNode;
    osg::ref_ptr<osg::MatrixTransform> m_forwardStep1;
    osg::ref_ptr<osg::MatrixTransform> m_forwardStep2;
    osg::ref_ptr<osg::MatrixTransform> m_forwardTransformationReference;

    osg::ref_ptr<osg::MatrixTransform> m_backwardTransformationReference;
    osg::ref_ptr<osg::MatrixTransform> m_backwardTransformationNode;
    osg::ref_ptr<osg::MatrixTransform> m_backwardStep1;
    osg::ref_ptr<osg::MatrixTransform> m_backwardStep2;
};


int main(int argc, char** argv)
{
    osg::setNotifyLevel(osg::FATAL); // Suppress debug output

    osgViewer::Viewer viewer;

    osg::ref_ptr<osg::Group>root = new osg::Group;
       
    root->addChild(createAxes());

    auto forwardTransformNode = loadModelAsWireframeOnSolid("bunny.obj");
    auto forwardTransformReference = loadModelAsWireframe("transformed_bunny.obj", 2.0f, { 1.0, 0, 0, 1 });
    auto forwardStep1 = loadModelAsWireframe("step_1_bunny.obj", 1.0f, { 1, 1, 0, 1 });
    auto forwardStep2 = loadModelAsWireframe("step_2_bunny.obj", 1.0f, { 1.0, 0.45, 0.0, 1 });

    root->addChild(forwardStep1);
    root->addChild(forwardStep2);
    root->addChild(forwardTransformReference);
    root->addChild(forwardTransformNode);

    //TODO: make all nodes be osg::MatrixTransform
    auto backwardTransformNode = loadModelAsWireframeOnSolid("transformed_monkey.obj");
    auto backwardTransformReference = loadModelAsWireframe("monkey.obj", 2.0f, { 1.0, 0, 0, 1 });
    auto backwardStep1 = loadModelAsWireframe("step_2_monkey.obj", 1.0f, { 1, 1, 0, 1 });
    auto backwardStep2 = loadModelAsWireframe("step_1_monkey.obj", 1.0f, { 1.0, 0.45, 0.0, 1 });
    
    root->addChild(backwardStep1);
    root->addChild(backwardStep2);
    root->addChild(backwardTransformReference);
    root->addChild(backwardTransformNode);

    viewer.addEventHandler(new osgViewer::StatsHandler);
    viewer.addEventHandler(new osgViewer::WindowSizeHandler);

    makeWindowed(viewer);

    viewer.setSceneData(root);
    viewer.realize();

    // Disable the built-in key event handler
    auto handlers = viewer.getEventHandlers();
    for (auto h : handlers)
    {
        if (h)
        {
            viewer.removeEventHandler(h);
        }
    }

    osg::ref_ptr< MyKeyboardEventHandler > eventHandler = new 
                                                          MyKeyboardEventHandler(forwardTransformNode, forwardTransformReference, forwardStep1, forwardStep2,
                                                                                 backwardTransformNode, backwardStep1, backwardStep2, backwardTransformReference);
    eventHandler->switchTasks();
    // Add a custom keyboard event handler to the viewer
    viewer.addEventHandler(eventHandler);


    return viewer.run();

}
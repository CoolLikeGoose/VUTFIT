#pragma once

#include <framework/method.hpp>
#include <framework/model.hpp>

namespace modelMethod{

/**
 * @brief This class represents model visualizer
 */
class Method: public ::Method{
  public:
    Method(GPUMemory&m,MethodConstructionData const*cd = nullptr);
    /**
     * @brief Descturctor
     */
    virtual ~Method(){};
    virtual void onDraw(SceneParam const&sceneParam) override;
    ModelData     modelData;
    Model         model;
    CommandBuffer modelCB;
    CommandBuffer drawCB;
};

}

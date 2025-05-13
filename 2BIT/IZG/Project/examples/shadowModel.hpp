#pragma once

#include <framework/method.hpp>
#include <framework/model.hpp>

namespace shadowModelMethod{
class Method: public ::Method{
  public:
    Method(GPUMemory&m,MethodConstructionData const*cd = nullptr);
    virtual ~Method(){}
    virtual void onDraw(SceneParam const&sceneParam) override;
    virtual void onUpdate(float dt) override;
    float time = 0;

    ModelData     modelData;
    Model         model;

    CommandBuffer modelCB;
    CommandBuffer drawCB;
    TextureData   shadowMap;

    glm::mat4     lightProj;
    glm::mat4     lightBias;

    int shadowMapId = 999;
};

}

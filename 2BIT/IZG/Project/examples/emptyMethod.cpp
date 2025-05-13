#include <framework/programContext.hpp>

namespace emptyMethod{

/**
 * @brief Empty rendering method
 */
class Method: public ::Method{
  public:
    Method(GPUMemory&m,MethodConstructionData const*);
    virtual ~Method();
    virtual void onDraw(SceneParam const&sceneParam) override;
    CommandBuffer commandBuffer;
};


Method::Method(GPUMemory&m,MethodConstructionData const*): ::Method(m){
  pushClearCommand(commandBuffer,glm::vec4(.5,.2,0,1));
}

Method::~Method(){
}

void Method::onDraw(SceneParam const&){
  izg_enqueue(mem,commandBuffer);
}

EntryPoint main = [](){registerMethod<Method>("empty window");};

}

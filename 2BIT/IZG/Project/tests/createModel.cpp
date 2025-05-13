#include <functional>

#include<tests/createModel.hpp>

namespace tests::model{

NodeI::NodeI(
    int32_t                 m ,  
    std::vector<NodeI>const&c ,   
    glm::mat4         const&mm):mesh(m),children(c),model(mm){}

Model createModel(std::vector<MeshI>const&meshes,std::vector<NodeI>const&roots){
  Model model;
  for(auto const&m:meshes){
    Mesh mesh;
    mesh.nofIndices     = m.nofIndices    ;
    mesh.diffuseColor   = m.diffuseColor  ;
    mesh.diffuseTexture = m.diffuseTexture;
    mesh.doubleSided    = m.doubleSided   ;

    model.meshes.push_back(mesh);
  }

  std::function<void(Node&,NodeI const&)>rec;

  auto insert = [&](std::vector<Node>&o,std::vector<NodeI> const&i){
    for(auto const&n:i){
      o.push_back({});
      rec(o.back(),n);
    }
  };

  rec = [&](Node&o,NodeI const&i){
    o.mesh        = i.mesh;
    o.modelMatrix = i.model;
    insert(o.children,i.children);
  };
  insert(model.roots,roots);
  return model;
}
}

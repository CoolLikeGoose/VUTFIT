#include <tests/getExpectedPreparedModel.hpp>

#include<student/prepareModel.hpp>

namespace tests::model{

uint32_t drawCallUniforms(uint32_t drawId){
  return drawCallUniformOffset + drawId*drawCallNofUniforms;
}

uint32_t uniformIdToDrawCallUniform(uint32_t uId){
  return (uId - drawCallUniformOffset) % drawCallNofUniforms;
}


void nodes(
    GPUMemory    &mem,
    CommandBuffer&commandBuffer,
    Node      const&node,
    Model     const&model,
    glm::mat4 const&modelMatrix,
    uint32_t &cmdID            ){

  if(node.mesh>=0){
    auto const&mesh = model.meshes.at(node.mesh);

    auto&vao = mem.vertexArrays[cmdID];
    vao.indexBufferID = mesh.indexBufferID;
    vao.indexOffset   = mesh.indexOffset  ;
    vao.indexType     = mesh.indexType    ;
    vao.vertexAttrib[0] = mesh.position;
    vao.vertexAttrib[1] = mesh.normal  ;
    vao.vertexAttrib[2] = mesh.texCoord;
    bool doubleSided    = !mesh.doubleSided;

    pushBindVertexArrayCommand(commandBuffer,cmdID);
    pushDrawCommand(commandBuffer,mesh.nofIndices,doubleSided);

    auto&modelMatrixUniform        = mem.uniforms[drawCallUniforms(cmdID)+MODEL       ].m4;
    auto&inverseModelMatrixUniform = mem.uniforms[drawCallUniforms(cmdID)+INV_MODEL   ].m4;
    auto&diffuseColorUniform       = mem.uniforms[drawCallUniforms(cmdID)+DIFF_COLOR  ].v4;
    auto&diffuseTextureIDUniform   = mem.uniforms[drawCallUniforms(cmdID)+TEXTURE_ID  ].u1;
    auto&doubleSidedUniform        = mem.uniforms[drawCallUniforms(cmdID)+DOUBLE_SIDED].v1;

    diffuseColorUniform       = mesh.diffuseColor;
    doubleSidedUniform        = (float)mesh.doubleSided;
    diffuseTextureIDUniform   = mesh.diffuseTexture;
    //isTextureUniform          = mesh.diffuseTexture<0?0.f:1.f;
    modelMatrixUniform        = modelMatrix*node.modelMatrix;
    inverseModelMatrixUniform = glm::transpose(glm::inverse(modelMatrixUniform));

    cmdID++;
  }
  for(auto const&n:node.children)
    nodes(mem,commandBuffer,n,model,modelMatrix*node.modelMatrix,cmdID);
}

PreparedModel getExpectedPreparedModel(Model const&model){
  MEMCB(10,10);

  for(size_t i=0;i<model.buffers.size();++i)
    mem.buffers[i] = model.buffers[i];

  for(size_t i=0;i<model.textures.size();++i)
    mem.textures[i] = model.textures[i];


  //mem.programs[0].vertexShader   = drawModel_vertexShader;
  //mem.programs[0].fragmentShader = drawModel_fragmentShader;
  //mem.programs[0].vs2fs[0] = AttributeType::VEC3;
  //mem.programs[0].vs2fs[1] = AttributeType::VEC3;
  //mem.programs[0].vs2fs[2] = AttributeType::VEC2;
  //pushClearCommand(cb,glm::vec4(0.1,0.15,0.1,1.),10e10f);
  //pushBindProgramCommand(cb,0);

  uint32_t cmdID = 0;
  for(auto const&root:model.roots)
    nodes(mem,cb,root,model,glm::mat4(1.f),cmdID);

  return memCb;
}




}

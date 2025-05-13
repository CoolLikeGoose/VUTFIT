/*!
 * @file
 * @brief This file contains rendering method interface
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#pragma once

#include <iostream>

#include <glm/glm.hpp>

#include <student/gpu.hpp>
#include <framework/textureData.hpp>

class MethodConstructionData{
  public:
    virtual ~MethodConstructionData(){}
};

struct SceneParam{
  glm::mat4 proj  ;
  glm::mat4 view  ;
  glm::vec3 light ;
  glm::vec3 camera;
};

/**
 * @brief This class represents rendering method.
 */
class Method{
  public:
    /**
     * @brief Constructor of rendering method
     */
    Method(GPUMemory&m):mem(m){}
    /**
     * @brief Destructor of rendering method
     */
    virtual ~Method(){}
    /**
     * @brief This functino is called every frame.
     *
     * @param proj projection matrix
     * @param view view matrix
     * @param light light position
     * @param camera camera position
     */
    virtual void onDraw(SceneParam const&sceneParam) = 0;
    /**
     * @brief This function is called on update
     *
     * @param dt delta time - time between frames
     */
    virtual void onUpdate(float dt){(void)dt;}
    /**
     * @brief  This function is called when the windows is resized
     *
     * @param w new width
     * @param h new height
     */
    virtual void onResize(uint32_t w,uint32_t h){(void)w;(void)h;}
    /**
     * @brief GPU memory
     */
    GPUMemory&mem;
};


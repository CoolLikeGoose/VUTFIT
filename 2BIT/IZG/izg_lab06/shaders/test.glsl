#ifdef VERTEX_SHADER
//#define MAKE_STUDENT_RELEASE
#line 3
precision highp float;
uniform mat4 view;
uniform mat4 proj;

out vec3 vTexCoord;
flat out int materialId;

void drawFloor(int vId, mat4 vp) {
    materialId = 0;
    vec3 vertices[] = vec3[] (
        vec3(-1, 0, -1)*4,
        vec3(1, 0, -1)*4,
        vec3(-1, 0, 1)*4,
        vec3(1, 0, 1)*4
    );

    uint indicies[] = uint[] (
        0u, 1u, 2u, 1u, 2u, 3u
    );

    if (gl_VertexID >= indicies.length()) return;

    uint index = indicies[gl_VertexID];
    gl_Position = vp * vec4(vertices[index], 1);
    vTexCoord = vertices[index]*0.5f+0.5f;
}

void drawCube(int vId, mat4 vp) {
    materialId = 1;

    gl_Position = vec4(0.f,0.f,0.f,1.f);
  
   uint indices[] = uint[](
     0u,1u,2u,2u,1u,3u,
     4u,5u,6u,6u,5u,7u,
     0u,4u,2u,2u,4u,6u,
     1u,5u,3u,3u,5u,7u,
     0u,1u,4u,4u,1u,5u,
     2u,3u,6u,6u,3u,7u
   );
  
   vec3 pos;
   for(uint i=0u;i<3u;++i)
     pos[i] = float((indices[vId]>>i)&1u);
  
   gl_Position = proj*view*vec4(pos*2.0f-1.f,1.f);
}

void main(){
    gl_Position = vec4(0, 0, 0, 1);
    mat4 vp = proj * view;

    if (gl_VertexID <= 6) {
        drawFloor(gl_VertexID, vp);
    } else if (gl_VertexID <= 36) {
        drawCube(gl_VertexID, vp);
    } else {
        return;
    }
}
#endif



#ifdef FRAGMENT_SHADER
precision highp float;
out highp vec4 fColor;

flat in int materialId;
in vec3 vTexCoord;

highp vec4 floorTexture(highp vec3 uv) {
    float repeatCount = 4;
    vec3 repeatedUV = fract(vTexCoord*repeatCount);
    vec3 center = vec3(0.5);

    vec3 toCenter = center - repeatedUV;
    float dist = length(toCenter);
    // return vec4(dist, 0, 0, 1);
    if (dist <= 0.71f) {
        return vec4(0.149, 1.0, 0.0157, 1.0);
    } else {
        return vec4(0.0902, 0.3176, 0.0784, 1.0);
    }

    return vec4(0);
}

highp vec4 cubeTexture(highp vec3 uv) {
    return vec4(1);
}

void main(){
    if (materialId == 0) fColor = floorTexture(vTexCoord);
    if (materialId == 1) fColor = cubeTexture(vTexCoord);
}
#endif

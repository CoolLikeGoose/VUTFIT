#ifdef VERTEX_SHADER
precision highp float;

uniform float iTime;
uniform mat4 view;
uniform mat4 proj;

mat4 Rz(float a){
  float ra = radians(a);
  mat4 R = mat4(1);
  R[0][0] =  cos(ra);
  R[1][1] =  cos(ra);
  R[0][1] =  sin(ra);
  R[1][0] = -sin(ra);
  return R;
}

mat4 Ry(float a){
  float ra = radians(a);
  mat4 R = mat4(1);
  R[0][0] =  cos(ra);
  R[2][2] =  cos(ra);
  R[0][2] =  sin(ra);
  R[2][0] = -sin(ra);
  return R;
}

mat4 T(float x,float y,float z){
  mat4 R = mat4(1);
  R[3] = vec4(x,y,z,1);
  return R;          
}

mat4 S(float x,float y,float z){
  mat4 R = mat4(1);
  R[0][0] = x;
  R[1][1] = y;
  R[2][2] = z;
  return R;          
}

void quad(int offset,mat4 model){
  mat4 pv = proj * view;

 
  mat4 pvm = proj * view * model;
 
  vec3 verts[] = vec3[](
    vec3(0,0,0),
    vec3(1,0,0),
    vec3(0,1,0),
    vec3(0,1,0),
    vec3(1,0,0),
    vec3(1,1,0)
  );
 
  int vID = gl_VertexID - offset;
 
  if(vID < 0 || vID >= verts.length())return;
 
  gl_Position = pvm * vec4(verts[vID].xy*2-1,0,1);
}

flat out int qID;
 
void drawCube(int vId, mat4 pv, vec2 position, vec2 size) {

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

  gl_Position = pv*T(position.x, position.y, 0)*S(size.x, size.y, 1)*vec4(pos*2.0f-1.f,1.f);
}

void drawCube(int vId, mat4 pv, vec2 position, vec2 size, float rot) {

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

  gl_Position = pv*Rz(rot)*T(position.x, position.y, 0)*S(size.x, size.y, 1)*vec4(pos*2.0f-1.f,1.f);
}

void main(){
    gl_Position = vec4(0,0,0,1);
    mat4 pv = proj * view;
    
    //First
    if (gl_VertexID < 30) drawCube(gl_VertexID, pv, vec2(15, 0), vec2(1, 10)); 
    else if (gl_VertexID < 60) drawCube(gl_VertexID%30, pv, vec2(15, 5), vec2(9, 1));    
    else if (gl_VertexID < 90) drawCube(gl_VertexID%30, pv, vec2(15, -1), vec2(9, 1));    
    else if (gl_VertexID < 120) drawCube(gl_VertexID%30, pv, vec2(23, 0), vec2(1, 4)); 
    else if (gl_VertexID < 150) drawCube(gl_VertexID%30, pv, vec2(7, 0), vec2(1, 4)); 
    //second
    else if (gl_VertexID < 180) drawCube(gl_VertexID%30, pv, vec2(-15, 9), vec2(9, 1)); 
    else if (gl_VertexID < 210) drawCube(gl_VertexID%30, pv, vec2(-15+9, 0), vec2(1, 10)); 
    else if (gl_VertexID < 240) drawCube(gl_VertexID%30, pv, vec2(-15-9, 0), vec2(1, 10)); 
    else if (gl_VertexID < 270) drawCube(gl_VertexID%30, pv, vec2(-15, -7), vec2(9, 1)); 
    else if (gl_VertexID < 300) drawCube(gl_VertexID%30, pv, vec2(-15, -3), vec2(7, 1)); 
    else if (gl_VertexID < 330) drawCube(gl_VertexID%30, pv, vec2(-15, 6), vec2(6, 1)); 
    else if (gl_VertexID < 360) drawCube(gl_VertexID%30, pv, vec2(-15, 2), vec2(6, 1)); 
    else if (gl_VertexID < 390) drawCube(gl_VertexID%30, pv, vec2(-15, 1), vec2(1, 5));  

    else if (gl_VertexID < 420) drawCube(gl_VertexID%30, pv, vec2(-15, 1), vec2(1, 12), 45);  
}
              
#endif

#ifdef FRAGMENT_SHADER
precision highp float;

uniform float iTime;

// flat in int qID;
out vec4 fColor;

float rand(int x){
  x = x*(x*(x*(x*(x*(x+23231)+39495)+34993)+2329393)+23232)+2323219;
  return float(x) / float(0x7fffffff);
}

void main(){
 
//   fColor = vec4(vec3(sin(iTime+3),sin(iTime+2),sin(iTime+1))*.5+.5,1);
//   fColor = vec4(rand(qID),rand(qID+200),rand(qID+10000),1);
    fColor = vec4(1);
}
#endif
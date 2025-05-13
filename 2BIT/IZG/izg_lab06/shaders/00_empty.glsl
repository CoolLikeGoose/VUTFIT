#ifdef VERTEX_SHADER
precision highp float;

uniform mat4 view;
uniform mat4 proj;

void main(){
  if(gl_VertexID==0)gl_Position = proj*view*vec4(0.f,0.f,0.f,1.f);
  if(gl_VertexID==1)gl_Position = proj*view*vec4(1.f,0.f,0.f,1.f);
  if(gl_VertexID==2)gl_Position = proj*view*vec4(0.f,1.f,0.f,1.f);

}
#endif



#ifdef FRAGMENT_SHADER
precision highp float;

out vec4 fColor;

void main(){
  fColor = vec4(1);
}

#endif

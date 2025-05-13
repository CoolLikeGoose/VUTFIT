#ifdef VERTEX_SHADER
precision highp float;

uniform mat4 view;
uniform mat4 proj;
uniform float iTime;

mat4 T(float x,float y,float z){
  mat4 R = mat4(1);
  R[3] = vec4(x,y,z,1);
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

out vec3 vPosition;
flat out int qID;

void quad(int offset,mat4 model,int m){
  mat4 pv = proj * view;

  vec3 verts[] = vec3[](
    vec3(0,0,0),
    vec3(1,0,0),
    vec3(0,1,0),
    vec3(0,1,0),
    vec3(1,0,0),
    vec3(1,1,0)
  );
 
  int vID = gl_VertexID - offset;
 

  if(vID < 0 || vID>=verts.length())return;
  gl_Position = pv * model * vec4(verts[vID].xy*2-1,0,1);
  vPosition = verts[vID];
  qID = m;

}



void main(){
  gl_Position = vec4(0,0,0,1);
 
  qID = gl_VertexID/6;
 
  float a = radians(360*qID / 5000.f);
 
  int x = qID % 70;
  int y = qID / 70;
 
 
  quad(qID*6,T(x*3,y*3,0)*Ry(iTime*90),qID);
}


#endif

#ifdef FRAGMENT_SHADER
precision highp float;

out vec4 fColor;
in vec3 vPosition;
uniform float iTime;

flat in int qID;
highp vec4 southKorea(in highp vec2 uv )
{    
  highp vec2 size = vec2(3.f,2.f);
  highp vec3 red  = vec3(0xCD,0x2E,0x3A)/vec3(255);
  highp vec3 blu  = vec3(0x00,0x47,0xA0)/vec3(255);

  uv *= size;
 
  highp vec2 center = size/2.f;
 
  highp vec3 diag[4];
  diag[0] = vec3(-center.y,-center.x,+center.x*center.y+center.y*center.x)/length(center);
  diag[1] = vec3(-center.y,+center.x,+center.x*center.y-center.y*center.x)/length(center);
  diag[2] = vec3(+center.x,-center.y,-center.x*center.x+center.y*center.y)/length(center);
  diag[3] = vec3(+center.x,+center.y,-center.x*center.x-center.y*center.y)/length(center);
 
  highp float distDiag[4];
 
  for(int i=0;i<4;++i)
    distDiag[i] = dot(diag[i],vec3(uv,1))*48.f;
 
 
  highp float topRight = float(distDiag[0] <0.f);
 
  highp float centerCircle = float(length((uv-center)*48.f                ) < 24.f);
  highp float smallCircle  = float(length((uv-center)*48.f-12.f*diag[2].xy) < 12.f);
  highp float smallCircle2 = float(length((uv-center)*48.f+12.f*diag[2].xy) < 12.f);
  highp float redRegion = clamp(topRight-smallCircle+smallCircle2,0.f,1.f);
  highp float bluRegion = (1.f-redRegion);
 
  highp vec3 col;
 
  col=(bluRegion*blu+redRegion*red)*centerCircle;
  col+=vec3(1.f-centerCircle);
 
  highp vec2 strip;
  highp vec2 str;
 
  for(int i=0;i<2;++i){
    strip[i] = float(
      abs(abs(distDiag[2+i])-44.f) <  8.f &&
          abs(distDiag[i  ])       < 12.f  
    );
    str[i]=float((uint(floor(abs(distDiag[2+i])/2.f))+1u)%3u>0u);
  }
 
  col*=vec3(1.f-clamp(dot(strip,str),0.f,1.f));
 
  highp float pieceRightBotton = float(
    abs(distDiag[0] ) >  1.f ||
        distDiag[2]   < 36.f
  );
 
  col+=vec3(1.f-pieceRightBotton);
 
  highp float pieceLeftBotton = float(
    abs(distDiag[1]     ) > 1.f ||
    abs(distDiag[3]+44.f) > 2.f
  );
 
  col+=vec3(1.f-pieceLeftBotton);
 
  highp float pieceRightTop = float(
        abs(distDiag[1]     )      >  1.f ||
    abs(abs(distDiag[3]-44.f)-6.f) >  2.f
  );
 
  col+=vec3(1.f-pieceRightTop);

  return vec4(clamp(col,vec3(0.f),vec3(1.f)),1.0f);
}
void main(){
  fColor = vec4(1,0,0,1);
  float t = 1-fract(iTime);
  fColor = vec4(vPosition,1);
  if(qID == 0)fColor = vec4(0,0,0,1);
  if(qID >= 1)fColor = vec4(0,1,0,1);
  if(qID >= 2)fColor = vec4(vPosition.xy,0,1);
  if(qID >= 3)fColor = vec4(vPosition.xy*4,0,1);
  if(qID >= 4)fColor = vec4(fract(vPosition.xy*4),0,1);
  if(qID >= 5)fColor = vec4(fract(vPosition.x*4)>0.5==fract(vPosition.y*4)>0.5,0,0,1);
  if(qID >= 6)fColor = vec4(vec3(length(vPosition.xy)),1);
  if(qID >= 7)fColor = vec4(vec3(length(vPosition.xy-.5)),1);
  if(qID >= 8)fColor = vec4(vec3(length(vPosition.xy-.5)<.5),1);
  if(qID >= 9)fColor = vec4(vec3(1),length(vPosition.xy-.5)<.5);
  if(qID >= 10)fColor = southKorea(vPosition.xy);
  if(qID >= 11){
    float d = length(vPosition.xy-.5);
    if(d>0.5)discard;
    fColor = vec4(vec3(1),1);
  }
}
#endif
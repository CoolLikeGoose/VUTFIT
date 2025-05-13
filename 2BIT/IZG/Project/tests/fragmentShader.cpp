#include<iostream>
#include<functional>

#include<catch2/catch_test_macros.hpp>

#include<glm/glm.hpp>

#define __FILENAME__ "fragmentShader"
#include<tests/testCommon.hpp>
#include<tests/modelTestUtils.hpp>

#include<framework/textureData.hpp>

#include<student/fwd.hpp>
#include<student/gpu.hpp>

using namespace tests;
using namespace tests::model;

struct FragInput{
  glm::vec3           pos         = glm::vec3(0.f,0.f,0.f)    ;
  glm::vec3           nor         = glm::vec3(0.f,0.f,1.f)    ;
  glm::vec2           coord       = glm::vec2(0.5f,0.5f)      ;
  uint32_t            drawID      =  0                        ;
  int32_t             textureID   = -1                        ;
  glm::vec3           lPos        = glm::vec3(0.f,0.f,1.f)    ;
  glm::vec4           diffC       = glm::vec4(0.f)            ;
  glm::vec3           aLC         = glm::vec4(0.f)            ;
  glm::vec3           dLC         = glm::vec3(0.f)            ;
  std::vector<Texture>texs                                    ;
  float               doubleSided = 0.f                       ;
  glm::vec3           cam         = glm::vec3(0.f)            ;
  int32_t             shadowId    = -1                        ; 
  glm::vec4           shadowPos   = glm::vec4(.5f,.5f,.5f,1.f);
  OutFragment computeExpectedOutFragment()const;
  OutFragment computeStudentOutFragment ()const;

  void testLambert(std::function<void()>const&msg = [](){
    std::cerr << R".(
    Žádné specifické informace
    ).";
  })const;
  void printGeneralInfo()const;
  void printInputs()const;
  void printColorInfo(OutFragment const&expectedColor,OutFragment const&studentColor,std::function<void()>const&msg)const;
  void printDiscardInfo(OutFragment const& expected,OutFragment const& student)const;

};

auto clearTex(TextureData&d,glm::vec4 const&color){
  auto tt=(Texture*)&d.texture;
  auto&t = *tt;
  for(uint32_t y=0;y<t.height;++y)
    for(uint32_t x=0;x<t.width;++x)
      setColorF(t,x,y,color);
}

auto clearTex(TextureData&d,float depth){
  auto tt=(Texture*)&d.texture;
  auto&t = *tt;
  for(uint32_t y=0;y<t.height;++y)
    for(uint32_t x=0;x<t.width;++x)
      setDepth(t,x,y,depth);
}

OutFragment FragInput::computeExpectedOutFragment()const{
  OutFragment of;

  auto nor2 = nor;
  if(doubleSided > 0.f)
    nor2 *= glm::sign(glm::dot(cam-pos,nor2));

  glm::vec4 dC;
  if(textureID<0)
    dC = diffC;
  else
    dC = read_texture(texs[textureID],coord);
  if(dC.a < 0.5)of.discard = true;
  
  auto L = glm::normalize(lPos-pos);
  float dF = glm::max(glm::dot(L,glm::normalize(nor2)),0.f);

  glm::vec3 aL = glm::vec3(dC)*aLC;
  glm::vec3 dL = glm::vec3(dC)*dLC*dF;

  float il = 1.f;
  if(shadowId >= 0){
    auto sPos = shadowPos;
    sPos/=sPos.w;
    if(sPos.x>=0.f && sPos.x < 1.0 && sPos.y>=0.f && sPos.y < 1.f){
      auto sm = read_textureClamp(texs[shadowId],glm::vec2(sPos)).r;
      il = (float)(sPos.z < sm);
    }
  }

  of.gl_FragColor = glm::vec4(aL+dL,dC.a);
  return of;
}

OutFragment FragInput::computeStudentOutFragment()const{

  InFragment inF;
  OutFragment outF;

  struct UT{
    Uniform uniforms[10000];
    Texture textures[10000];
  };

  auto ut = std::make_shared<UT>();
  auto&uniforms = ut->uniforms;
  auto&textures = ut->textures;

  ShaderInterface si;
  si.textures = textures;
  si.uniforms = uniforms;
  si.gl_DrawID = drawID;

  inF.attributes[0].v3 = pos;
  inF.attributes[1].v3 = nor;
  inF.attributes[2].v2 = coord;
  
  uniforms[1].v3 = lPos ;
  uniforms[2].v3 = cam  ;
  uniforms[6].i1 = -1   ;
  uniforms[7].v3 = aLC  ;
  uniforms[8].v3 = dLC  ;
  uniforms[drawCallUniforms(drawID)+DIFF_COLOR  ].v4 = diffC      ;
  uniforms[drawCallUniforms(drawID)+TEXTURE_ID  ].i1 = textureID  ;
  uniforms[drawCallUniforms(drawID)+DOUBLE_SIDED].v1 = doubleSided;

  for(size_t k=0;k<texs.size();++k)
    textures[k] = texs[k];

  drawModel_fragmentShader(outF,inF,si);

  return outF;
}

void FragInput::printInputs()const{
  std::cerr << R".(
  ===================================
  ======== Vstupní fragment =========
  ===================================

  Vstupní fragment je tvořen čtyřmi atributy:
   - pozice fragmentu ve world-space,
   - normála ve world-space,
   - texturovací souřadnice fragmentu,
   - pozice fragmentu v clip-space stínové mapy.

  inFragment.attributes[0].v3 = /* pozice                                                  */ )."<<str(pos      )<<R".(;
  inFragment.attributes[1].v3 = /* normala                                                 */ )."<<str(nor      )<<R".(;
  inFragment.attributes[2].v2 = /* texturovaci souřadnice                                  */ )."<<str(coord    )<<R".(;
  inFragment.attributes[3].v4 = /* pozice zobrazovaného fragmentu v clip space shadow mapy */ )."<<str(shadowPos)<<R".(;

  Shader ma přístup k:
   - uniformním proměnným,
   - texturám
   - číslu vykresovacího příkazu.

  si.uniforms[1].v3                = /* pozice světla                                                               */ )."<< str(lPos)<<R".(; 
  si.uniforms[2].v3                = /* pozice kamery                                                               */ )."<< str(cam )<<R".(; 
  si.uniforms[6].i1                = /* číslo textury, která obsahuje stínovou mapu, nebo -1 pokud stíny nejsou     */ )."<< shadowId <<R".(; 
  si.uniforms[7].v3                = /* barva ambientního světla                                                    */ )."<< str(aLC )<<R".(; 
  si.uniforms[8].v3                = /* barva světla                                                                */ )."<< str(dLC )<<R".(; 
  si.uniforms[)."<<drawCallUniformOffset<<R".(+gl_DrawID*)."<<drawCallNofUniforms<<R".(+)."<<DrawCallUniform::DIFF_COLOR  <<R".(].v4 = /* difuzní barva materialu, - pokud není textura                               */ )."<<str(diffC      )<<R".(; 
  si.uniforms[)."<<drawCallUniformOffset<<R".(+gl_DrawID*)."<<drawCallNofUniforms<<R".(+)."<<DrawCallUniform::TEXTURE_ID  <<R".(].i1 = /* číslo textury s barvou nebo -1 pokud textura není.                          */ )."<<str(textureID  )<<R".(; 
  si.uniforms[)."<<drawCallUniformOffset<<R".(+gl_DrawID*)."<<drawCallNofUniforms<<R".(+)."<<DrawCallUniform::DOUBLE_SIDED<<R".(].v1 = /* 0.f znamená, že je to jednostraný povrch, 1.f znamená, že je to doubleSided */ )."<<str(doubleSided)<<R".(; 
  ss.textures[] = {...}; // textury
  si.gl_DrawID  = )."<<drawID<<R".(    ; // číslo vykresovacího příkazu - obvykle pro výběr matic a materiálů
  )." << std::endl;
}

void FragInput::printGeneralInfo()const{
  printInputs();

  std::cerr<<R".(

  ===================================
  ========== Obecné info ============
  ===================================

  Tento test zkouší, zda je funkce drawModel_fragmentShader správně naimplementována.
  Tato funkce reprezentuje fragment shader pro texturování, výpočet osvětlení a vrhání stínů z modelů.
  Fragment shader by měl spočítat barvu pomocí lambertova osvětlovacího modelu:
  https://en.wikipedia.org/wiki/Lambertian_reflectance
  A stín pomocí shadow mappingu:
  https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
  Většina shadow-mappingu už je udělána, je jen potřeba stínovou mapu využít.

  Vstupem FS je fragment a výstupem je fragment s vypočtenou barvou.
  ).";

  std::cerr << R".(
  Výstupní barva by měla být zapsána do proměnné outFragment.gl_FragColor.
  Pokud je neprůhlednost fragmentu příliž nízka, shader by měl nastavit příznak discard.
  V takovém případě na barvě nezáleží, protože se fragment stejně zahodí.

  Lambertův osvětlovcí model počítá barvu ze dvou složek: ambientní a diffusní.

  Ambietní složka je vypočítána jako součin barvy materiálu a ambientní barvy světla:
  aL = dC * ambientLightColor; // součin po komponentách

  Difuzní složka je vypočítána jako součin barvy materiálu s barvou světla
  vynásobenou difuzním faktorem dF
  dL = dC * lightColor * dF;

  výsledná barva je:
  vec4(aL+dL,dC.a);

  dC je difuzní barva materiálu, je to buď hodnota z textury nebo z uniformní proměnné, když není textura k dispozici.
  Pro čtení z textur použijte funkci read_texture(Texture const&tex,glm::vec2 const&coord);

  ambientLightColor je barva ambientního světla.
  dF je difuzní faktor.
  Difuzní faktor je spočítán jako skalární součin vektoru
  z fragmentu do světla a normály fragmentu ořezaný do nezáporných hodnot.
  dF = glm::clamp(glm::dot(L,N),0.f,1.f)

  Pozor oba vektory musí být normalizované!
  pro normalizaci můžete využít funkci: glm::normalize()

  Pokud je zapnutý příznak doubleSided, je potřeba otočit normálu, pokud směruje od kamery.
  Tzn. vektor od kamery k povrchu má stejný směr jako normála.

  dC.a je alpha - neprůhlednost

  Pokud je neprůhlednost < 0.5, měl by se discard příznak nastavit na true,
  tím se fragment zahodí.

  Poslední věcí, co by měl shader řešit je výpočet stínu.
  Ten je zajištěn tak, že pokud je fragment ve stínu, výsledná barva je pouze dána ambientním osvětlením (aL).

  Pro zjištění, zda je fragment ve snímu je potřeba stínové mapy.

  Stínová mapa je textura hloubky, kde je v každém texelu vzdálenost k nejbližšímu povrchu od světla.
  Stínová mapa (shadow mapa) může a nemusí být využívána.
  Pokud je číslo stínové mapy -1, pak není shadow-mapping využit.
  Výpočet, jestli je stín nebo ne je zajištěn porovnáním vzdálenost fragmentu ke světlu
  a nejbližší vzdálenosti povrchu od světla.
  Pro zjištění, který texel stínové, je potřeba pro porovnání použít, je potřeba
  z shadow mapu správně adresovat.
  K tomu je potřeba využít souřadnici fragmentu v clip-space shadow mapy (4. atribut).
  shadowPosition.xyzw
  Ten je potřeba převést do kartézkých souřadnic podělením w.
  shadowPosition /= shadowPosition.w
  Tím vzniknou souřadnice do stínové mapy v komponentách x,y.

  Poznámka: ty nejsou v rozsahu [-1,+1] jak jste zvyklí, ale v rozsahu [0,1].
  Je to způsobeno tzn. bias maticí.

  V případě, že osvětlovaný fragment padne mimo stínovou mapu < 0 || > 1,
  pak neleží ve stínu.
  Stínová mapa obecně nedokáže pokrýt celý svět, ale jen jeho podčást.

  Když se xy souřadnice použijí pro vyčtení hodnoty ze stínové mapy pomocí
  funkce read_texture, červený kanál obsahuje hloubku
  read_Texture(shadowMap, shadowPosition.xy).r

  Tu je potřeba porovnat se vzdáleností fragmentu od světla, což je
  3. (z) komponenta shadowPosition.
  if( shadowPosition.z < read_texture(...,...).r )
    stín;

  Něco se pokazilo...)."<<std::endl;
}

void FragInput::printDiscardInfo(OutFragment const&expected,OutFragment const&student)const{
  std::cerr << R".(
  Příznak discard měl být: outFrag.discard = )."<<str(expected.discard)<< ";" << R".(
  Ale byl                : outFrag.discard = )."<<str(student .discard)<< ";" << std::endl;
  printGeneralInfo();
}

void FragInput::printColorInfo(OutFragment const&expected,OutFragment const&student,std::function<void()>const&msg)const{
  std::cerr << R".(
  Barva měla být: outFrag.gl_FragColor = vec4)."<<str(expected.gl_FragColor)<< ";" << R".(
  Ale byla      : outFrag.gl_FragColor = vec4)."<<str(student .gl_FragColor)<< ";" << std::endl;

  std::cerr << R".(
  ===================================
  ========= Specifické info =========
  ===================================
  ).";
  msg();
  printGeneralInfo();
}

void FragInput::testLambert(std::function<void()>const&msg)const{
  auto expected = computeExpectedOutFragment();
  auto student  = computeStudentOutFragment ();



  if(breakTest() || expected.discard != student.discard){
    printDiscardInfo(expected,student);
    REQUIRE(false);
  }

  //do not check colors when fragment is being discarded
  if(expected.discard)return;

  if(breakTest() || !equalVec4(expected.gl_FragColor,student.gl_FragColor)){
    printColorInfo(expected,student,msg);
    REQUIRE(false);
  }
}



















void diffuseColorMsg(){
  std::cerr << R".(
  Test zkouší, zda funguje výpočet obarvení objektů difuzní barvou.

  Difuzní barva, albedo, barva materiálu, je barva objektů, bez osvětlení.

  Difuzní barva je uložena v uniformní proměnné.

  Pokud je v modelu přítomná textura (číslo příslušné uniformní promněnné je >= 0),
  pak by se měla vyčíst barva z textury pomocí texturovacích souřadnic.
  Texturovací souřadnice přichází do fragment shaderu jako atribut.
  Pro čtení z textur by se měla využít funkce read_texture.
  ).";
}

SCENARIO(TEST_NAME){
  std::cerr << testCounter() << " - drawModel_fragmentShader - diffuse color" << std::endl;

  FragInput i;
  i.pos         = glm::vec3(0.f)        ;
  i.nor         = glm::vec3(0,1,0)      ;
  i.coord       = glm::vec2(0.5,0.5)    ;
  i.drawID      = 0                     ;
  i.textureID   = -1                    ;
  i.lPos        = glm::vec3(0,1,0)      ;
  i.diffC       = glm::vec4(0,1,0,1)    ;
  i.aLC         = glm::vec3(1,1,1)      ;
  i.dLC         = glm::vec3(0,0,0)      ;
  i.texs        = {}                    ;
  i.doubleSided = false                 ;
  i.cam         = glm::vec3(0.f,1.f,0.f);

  i.testLambert(diffuseColorMsg);


  auto tex0 = TextureData(10,10,3);
  clearTex(tex0,glm::vec4(1,0,0,1));
  i.pos         = glm::vec3(0.f)        ;
  i.nor         = glm::vec3(0,1,0)      ;
  i.coord       = glm::vec2(0.5,0.5)    ;
  i.drawID      = 0                     ;
  i.textureID   = 0                     ;
  i.lPos        = glm::vec3(0,1,0)      ;
  i.diffC       = glm::vec4(0,1,0,1)    ;
  i.aLC         = glm::vec3(1,1,1)      ;
  i.dLC         = glm::vec3(0,0,0)      ;
  i.texs        = {tex0.getTexture()}   ;
  i.doubleSided = false                 ;
  i.cam         = glm::vec3(0.f,1.f,0.f);
  i.testLambert(diffuseColorMsg);
}

auto ambientMsg(){
  std::cerr << R".(
  Test zkouší, zda funguje výpočet ambientního osvětlení.

  Ambientní osvětlení je všesměrové, všudypřítomné osvětlení.
  Je to aproximace globálního osvětlení, které je složité na výpočet.
  Je součástí Lambertova i Phongova osvětlovacího modelu.
  V zásadě jde o to, aby objekty, které jsou ve tmě nebyly úplně černé.
  Ambientní osvětlení není ovlivněno stínem.

  Výpočet je dán vynásobením barvy materiálu s barvou ambientního světla:
  aL = dC * aLC
  kde:
   - aL je ambientní osvětlení
   - dC je barva materiálu
   - aLC je barva ambientního osvětlení
  ).";
}

SCENARIO(TEST_NAME){
  std::cerr << testCounter() << " - drawModel_fragmentShader - ambient light" << std::endl;

  FragInput i;
  i.pos         = glm::vec3(0.f)        ;
  i.nor         = glm::vec3(0,1,0)      ;
  i.coord       = glm::vec2(0.5,0.5)    ;
  i.drawID      = 0                     ;
  i.textureID   = -1                    ;
  i.lPos        = glm::vec3(0,1,0)      ;
  i.diffC       = glm::vec4(1.)         ;
  i.aLC         = glm::vec3(0,0,1)      ;
  i.dLC         = glm::vec3(0,0,0)      ;
  i.texs        = {}                    ;
  i.doubleSided = false                 ;
  i.cam         = glm::vec3(0.f,1.f,0.f);

  i.testLambert(ambientMsg);
}

void diffuseMsg(){
  std::cerr << R".(
  Test zkouší, zda funguje výpočet difuzího osvětlení.

  Difuzní osvětlení je přímé osvětlení povrchů, které má tu vlastnost,
  že nezáleží na úhlu pozorovatele a vypadá vždy stejně.
  Je to aproximace odraženého světla z povrchů, které jsou izolanty.
  U takových materiálů dochází k tomu, že se fotony zanoří částečně pod
  povrch a pak se opět vynoří ven v náhodném směru velmi blízko místa
  vstupu do materiálu.

  Je součástí Lambertova i Phongova osvětlovacího modelu.
  V zásadě jde o to, aby míra osvětlení závisela na úhlu dopadajícího světla
  a normálového vektoru povrchu.
  Difuzní osvětlení je ovlivněno stínem.

  Výpočet:
  dL = dC * dLC * dF
  kde:
   - dL je difuzní osvětlení
   - dC je barva materiálu (vektor)
   - dLC je barva světla (vektor)
   - dF difuzní faktor

  Difuzní faktor je dán skalárním součinem vektorů ke světlu a normálového
  vektoru. Výsledek musí být oříznutý do intervalu [0,1].

  dF = glm::clamp(glm::dot(L,N),0.f,1.f);
  kde:
   - L je normalizovaný vektor z fragmentu ke světlu
   - N je normálový vektor povrchu
  ).";
}

SCENARIO(TEST_NAME){
  std::cerr << testCounter() << " - drawModel_fragmentShader - diffuse light" << std::endl;

  FragInput i;
  i.pos         = glm::vec3(0.f)            ;
  i.nor         = glm::vec3(0,1,0)          ;
  i.coord       = glm::vec2(0.5,0.5)        ;
  i.drawID      = 0                         ;
  i.textureID   = -1                        ;
  i.lPos        = glm::vec3(1,1,0)          ;
  i.diffC       = glm::vec4(.2f,.3f,.4f,1.f);
  i.aLC         = glm::vec3(0,0,0)          ;
  i.dLC         = glm::vec3(0.4f,0.3f,0.1f) ;
  i.texs        = {}                        ;
  i.doubleSided = false                     ;
  i.cam         = glm::vec3(0.f,1.f,0.f)    ;

  i.testLambert(diffuseMsg);
}


SCENARIO(TEST_NAME){
  std::cerr << testCounter() << " - drawModel_fragmentShader - general tests" << std::endl;

  auto tex0 = TextureData(100,100,3);
  auto tex1 = TextureData(100,100,3);

  auto genTex0 = [&](TextureData&d){
    auto tt=(Texture*)&d.texture;
    auto&t = *tt;
    for(uint32_t y=0;y<t.height;++y)
      for(uint32_t x=0;x<t.width;++x){
        glm::vec4 color = glm::vec4(0.f);

        for(uint32_t c=0;c<t.img.channels;++c){
          auto uv = glm::vec2(x,y)/glm::vec2(t.width,t.height);
          color[c] = glm::clamp(glm::sin(uv.x+uv.y*c),0.f,1.f);
        }
        setColorF(t,x,y,color);
      }
  };

  genTex0(tex1);

  std::vector<Texture>texs;
  texs.push_back(tex0.getTexture());
  texs.push_back(tex1.getTexture());

  std::vector<uint32_t>drawIds = {0,1,10,23,24};

  glm::vec3 aLC = glm::vec3(.2f);
  for(uint32_t f=0;f<2;++f){//flip normal
    float flip = f==0?1.f:-1.f;
    for(uint32_t d=0;d<2;++d){
      float doubleSided = (float)d;
      for(int32_t textureID=-1;textureID<2;++textureID){//isTexture
        for(auto drawID:drawIds){
          FragInput i;

          i.pos         = glm::vec3(0.f)        ;
          i.nor         = flip*glm::vec3(0,1,0) ;
          i.coord       = glm::vec2(0.3f,0.6f)  ;
          i.drawID      = drawID                ;
          i.textureID   = textureID             ;
          i.lPos        = glm::vec3(0,1,0)      ;
          i.diffC       = glm::vec4(.7f)        ;
          i.aLC         = aLC                   ;
          i.texs        = texs                  ;
          i.doubleSided = doubleSided           ;
          i.cam         = glm::vec3(0.f,1.f,0.f);
          i.testLambert();


          i.pos         = glm::vec3(0.f)          ;
          i.nor         = flip*glm::vec3(0,1000,0);
          i.coord       = glm::vec2(0.2f,0.8f)    ;
          i.drawID      = drawID                  ;
          i.textureID   = textureID               ;
          i.lPos        = glm::vec3(0,1,0)        ;
          i.diffC       = glm::vec4(.7f)          ;
          i.aLC         = aLC                     ;
          i.texs        = texs                    ;
          i.doubleSided = doubleSided             ;
          i.cam         = glm::vec3(0.f,1.f,0.f)  ;
          i.testLambert();

          i.pos         = glm::vec3(0.f)        ;
          i.nor         = flip*glm::vec3(0,1,0) ;
          i.coord       = glm::vec2(0.5f,0.5f)  ;
          i.drawID      = drawID                ;
          i.textureID   = textureID             ;
          i.lPos        = glm::vec3(0,1000,0)   ;
          i.diffC       = glm::vec4(.7f)        ;
          i.aLC         = aLC                   ;
          i.texs        = texs                  ;
          i.doubleSided = doubleSided           ;
          i.cam         = glm::vec3(0.f,1.f,0.f);
          i.testLambert();

          i.pos         = glm::vec3(0.f)          ;
          i.nor         = flip*glm::vec3(0,1000,0);
          i.coord       = glm::vec2(0.2f,0.6f)    ;
          i.drawID      = drawID                  ;
          i.textureID   = textureID               ;
          i.lPos        = glm::vec3(0,1000,0)     ;
          i.diffC       = glm::vec4(.7f)          ;
          i.aLC         = aLC                     ;
          i.texs        = texs                    ;
          i.doubleSided = doubleSided             ;
          i.cam         = glm::vec3(0.f,1.f,0.f)  ;
          i.testLambert();

          i.pos         = glm::vec3(0,-1.f,0)   ;
          i.nor         = flip*glm::vec3(0,1,0) ;
          i.coord       = glm::vec2(0.9f,0.1f)  ;
          i.drawID      = drawID                ;
          i.textureID   = textureID             ;
          i.lPos        = glm::vec3(0,0,0)      ;
          i.diffC       = glm::vec4(.7f)        ;
          i.aLC         = aLC                   ;
          i.texs        = texs                  ;
          i.doubleSided = doubleSided           ;
          i.cam         = glm::vec3(0.f,1.f,0.f);
          i.testLambert();

          i.pos         = glm::vec3(0,0.f,0)    ;
          i.nor         = flip*glm::vec3(1,0,0) ;
          i.coord       = glm::vec2(0.55f,0.45f);
          i.drawID      = drawID                ;
          i.textureID   = textureID             ;
          i.lPos        = glm::vec3(0,1,0)      ;
          i.diffC       = glm::vec4(.7f)        ;
          i.aLC         = aLC                   ;
          i.texs        = texs                  ;
          i.doubleSided = doubleSided           ;
          i.cam         = glm::vec3(1.f,0.f,0.f);
          i.testLambert();

          i.pos         = glm::vec3(0,0.f,0)     ;
          i.nor         = flip*glm::vec3(0,-1,0) ;
          i.coord       = glm::vec2(0.45f,0.55f) ;
          i.drawID      = drawID                 ;
          i.textureID   = textureID              ;
          i.lPos        = glm::vec3(0,1,0)       ;
          i.diffC       = glm::vec4(.7f)         ;
          i.aLC         = aLC                    ;
          i.texs        = texs                   ;
          i.doubleSided = doubleSided            ;
          i.cam         = glm::vec3(0.f,-1.f,0.f);
          i.testLambert();

          i.pos         = glm::vec3(0,0.f,0)     ;
          i.nor         = flip*glm::vec3(0,-1,0) ;
          i.coord       = glm::vec2(0.45f,0.55f) ;
          i.drawID      = drawID                 ;
          i.textureID   = textureID              ;
          i.lPos        = glm::vec3(0,1,0)       ;
          i.diffC       = glm::vec4(.4f)         ;
          i.aLC         = aLC                    ;
          i.texs        = texs                   ;
          i.doubleSided = doubleSided            ;
          i.cam         = glm::vec3(0.f,-1.f,0.f);
          i.testLambert();

          i.pos         = glm::vec3(0,0.f,0)     ;
          i.nor         = flip*glm::vec3(0,-1,0) ;
          i.coord       = glm::vec2(0.45f,0.55f) ;
          i.drawID      = drawID                 ;
          i.textureID   = textureID              ;
          i.lPos        = glm::vec3(0,1,0)       ;
          i.diffC       = glm::vec4(.4f)         ;
          i.aLC         = aLC                    ;
          i.texs        = texs                   ;
          i.doubleSided = doubleSided            ;
          i.cam         = glm::vec3(0.f,-1.f,0.f);
          i.testLambert();
        }
      }
    }
  }
}

void shadowMsg(){
  std::cerr << R".(
  Test zkouší, zda funguje výpočet stínu pomocí shadow-mappingu.

  Shadow-mapping je metoda výpočtu stínů.
  Využívá při tom shadow mapu - stínovou mapu.
  Je to textura, obsahující nejbližší hloubku scény při pohledu od světla.

  Algoritmus je složený ze dvou částí:
   1. vykreslení scény do stínové mapy tím, že se kamera nastaví do světla.
   2. vykreslení scény z pohledu pozorovatele a všechny body povrchů se
      porovnají s hloubkou v stínové mapě.

  V této části už je stínová mapa hotová, stačí akorat porovnat hloubky.

  Stínová textura je uložena v textuře označené číslem v příslušné uniformní proměnné.
  Pokud je id < 0, pak není shadow mapping využíván.

  Čtení ze stínové mapy je stejné jako čtení z jakékoliv jiné textury
  pomocí read_texture.

  K adresaci stínové mapy je třeba využít světelné clip-space souřadnice fragmentu.
  Ty jsou uložené v příslušném atributu fragmentu shadowPosition.

  Je potřeba provést perspektivní dělení
  shadowPosition /= shadowPosition.w;
  tím se vypočtou 3 souřadnice x,y,z.
  x a y lze použít přímo pro adresaci stínové mapy.
  a z je vzdálenost zkoumaného fragmentu od světla.

  Pokud jsou x,y mimo rozsah [0,1] shadow mapping se nepoužije.

  Pokud se zjistí, že je stín, difuzní složka lambertova osvětlovacího modelu
  se nepoužije a zůstane pouze ambientní.

  Pseudo algoritmus:
  if (shadowId >=0 ){
    shadowPosition /= shadowPosition.w;
    if (shadowPosition.x >= 0 && ...){
      closestToLight = read_texture(textures[shadowID],shadowPosition.xy).r
      if(shadowPosition.z > closestToLight){
        //hura mame stín!
      }else{
        //není stín
      }
    }else{
      // není stín
    }
  }else{
    //není stín
  }

  Můžete se také podívát na příklad:
  examples/shadowMapping.cpp
  ).";
}

SCENARIO(TEST_NAME){
  std::cerr << testCounter() << " - drawModel_fragmentShader - shadow mapping" << std::endl;

  auto sm = TextureData(10,10,1,Image::FLOAT32);
  clearTex(sm,10.f);

  FragInput i;
  i.pos         = glm::vec3(0.f)               ;
  i.nor         = glm::vec3(0,1,0)             ;
  i.coord       = glm::vec2(0.5,0.5)           ;
  i.drawID      = 0                            ;
  i.textureID   = -1                           ;
  i.lPos        = glm::vec3(1,1,0)             ;
  i.diffC       = glm::vec4(.2f,.3f,.4f,1.f)   ;
  i.aLC         = glm::vec3(0.1f)              ;
  i.dLC         = glm::vec3(0.4f,0.3f,0.1f)    ;
  i.texs        = {sm.getTexture()}            ;
  i.doubleSided = false                        ;
  i.shadowId    = 0                            ;
  i.shadowPos   = glm::vec4(0.5f,0.5f,15.f,1.f);
  i.cam         = glm::vec3(0.f,1.f,0.f)       ;

  i.testLambert(shadowMsg);

  i.shadowPos   = glm::vec4(0.5f,0.5f,5.f,1.f);

  i.testLambert(shadowMsg);

  i.shadowPos   = glm::vec4(1.5f,0.5f,15.f,1.f);

  i.testLambert(shadowMsg);
}








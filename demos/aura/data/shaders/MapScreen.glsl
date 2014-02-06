// -----------------------------------------------------------------------------
// CreativeCommons BY-SA 3.0 2013 <Thibault Coppex>
//
//
// -----------------------------------------------------------------------------
//   references : 
//    http://altdevblogaday.com/2011/08/08/interesting-vertex-shader-trick/
//------------------------------------------------------------------------------


-- VS


out VDataBlock {
  vec2 texCoord;
} OUT;

void main() {
  OUT.texCoord.s = (gl_VertexID << 1) & 2;
  OUT.texCoord.t = gl_VertexID & 2;

  gl_Position = vec4(2.0f * OUT.texCoord - 1.0f, 0.0f, 1.0f);
}


--

//------------------------------------------------------------------------------


-- FS

uniform sampler2D uAOTex;
uniform sampler2D uSceneTex;

in VDataBlock {
  vec2 texCoord;
} IN;


layout(location = 0) out vec4 fragColor;

void main() {
  float ao = texture(uAOTex, IN.texCoord).r;
  //ao = smoothstep( 0.0f, 1.0f, ao*ao);
  vec4 color = texture(uSceneTex, IN.texCoord);

  fragColor = ao * color;
}

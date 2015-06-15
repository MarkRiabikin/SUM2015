/* Filename: TEST.FRAG: */

#version 330


uniform float Time;
uniform float PartNo;
uniform float TotalParts;


out vec4 OutColor;


in vec4 DrawColor;
in vec3 DrawPosition;
in vec3 DrawNormal;
in vec2 DrawTexCoord;


uniform sampler2D DrawTexture;

void main( void )
{
  vec4 tc = texture2D(DrawTexture, DrawTexCoord);

  if (distance(DrawPosition, vec3(0, 0.5, 0)) < 0.88 - 0.30 * sin(Time))
    ;//discard;
  OutColor = tc * DrawColor * dot(normalize(DrawNormal), normalize(vec3(1, 1, 1)));
}
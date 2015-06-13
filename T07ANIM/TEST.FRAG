/* Filename: TEST.FRAG: */

#version 330


uniform float Time;


out vec4 OutColor;


in vec4 DrawColor;
in vec3 DrawPosition;

void main( void )
{
  if (DrawPosition.y - 8 + 5 * sin(3 * Time) > 1.5)
    discard;
  OutColor = DrawColor + vec4(1, 1, 1, 1) * 0.03;
}
/* Filename: TEST.VERT */


// версия языка шейдера
#version 330

// кто куда приходит
layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec2 InTexCoord;
layout(location = 2) in vec3 InNormal;
layout(location = 3) in vec4 InColor;

// глобальные переменные
uniform mat4 MatrWorld;
uniform mat4 MatrView;
uniform mat4 MatrProj;
uniform mat4 MatrWVP;
uniform mat4 MatrWV;
uniform mat4 MatrWVInverse;
uniform mat4 MatrWInverse;
uniform float Time;
uniform float PartNo;
uniform float Rand;

// выходные параметры
out vec4 DrawColor;
out vec3 DrawNormal;
out vec3 DrawPosition;
out vec2 DrawTexCoord;

void main( void )
{

  if(PartNo != 10 && PartNo != 11)
  {
    float dx = Rand * 1 * abs(sin(Time*40)) * PartNo;
    float dy = Rand * 1 * abs(sin(Time*40)) * PartNo;
    float dz = Rand * 1 * abs(sin(Time*40)) * PartNo;
    mat4 M = transpose(mat4(vec4(1 ,0  , 0 , 0),
  			  vec4(0 ,1  , 0 , 0),
    			  vec4(0 ,0  , 1 , 0),
    			  vec4(dx, dy, dz, 1))); 
    
    gl_Position = MatrWVP * MatrWorld * M * vec4(InPosition * 10, 1);
  }
  else
    gl_Position = MatrWVP * MatrWorld * vec4(InPosition * 10, 1);
  DrawNormal = mat3(MatrWInverse) * InNormal;
  DrawPosition = vec3(MatrWorld * vec4(InPosition, 1));
  DrawColor = InColor * 3.3;
  DrawTexCoord = InTexCoord;
}

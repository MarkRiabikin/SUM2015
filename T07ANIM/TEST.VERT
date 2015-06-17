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
  float X;
  float dx;
  float dy; 
  float dz;

  X = radians(Time * 60.0);
  float co = cos(X);
  float si = sin(X);

  mat4 RotY = mat4(vec4(co, 0, -si, 0),
  	           vec4(0 , 1,  0 , 0),
    	           vec4(si, 0,  co, 0),
    		   vec4(0 , 0,  0 , 1));

 if(PartNo != 10 && PartNo != 11 && PartNo != 26)
  {
    dx = (Rand) * 100 * abs(sin(Time/2)) * mod(PartNo, 8);
    dy = (Rand) * 100 * abs(sin(Time/2)) * mod(PartNo, 9);
    dz = (Rand) * 100 * abs(sin(Time/2)) * mod(PartNo, 7);
    mat4 M = mat4(vec4(1 ,  0, 0 , 0),
  		  vec4(0 ,  1, 0 , 0),
    		  vec4(0 ,  0, 1 , 0),
    	          vec4(dx, dy, dz, 1)); 
    
    gl_Position = MatrWVP * MatrWorld * M * RotY * vec4(InPosition * 12, 1);
  }
  else
    gl_Position = MatrWVP * MatrWorld * RotY * vec4(InPosition * 12, 1);
  DrawNormal = mat3(MatrWInverse) * InNormal;
  DrawPosition = vec3(MatrWorld * vec4(InPosition, 1));
  DrawColor = InColor * 3.3;
  DrawTexCoord = InTexCoord;
}

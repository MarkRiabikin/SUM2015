/* Filename: TEST.FRAG: */

#version 330


uniform float Time;
uniform float PartNo;
uniform float TotalParts;

uniform mat4 MatrWorld;
uniform mat4 MatrView;
uniform mat4 MatrProj;
uniform mat4 MatrWVP;
uniform mat4 MatrWV;
uniform mat4 MatrWVInverse;
uniform mat4 MatrWInverse;
uniform int Id;

// 
out vec4 OutColor;

// 
in vec4 DrawColor;
in vec3 DrawPosition;
in vec3 DrawNormal;
in vec2 DrawTexCoord;

/***  ***/
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Kp;
uniform float Kt;
//uniform 

/*  */
uniform float IsTextureUse;
uniform sampler2D DrawTexture;



vec4 Shade( vec3 P, vec3 N )
{
  float a = 1;
  vec3 color = vec3(0, 0, 0);
  vec3 L = normalize(vec3(1, 1, 1));
  vec3 V;

  V = -normalize(P - vec3(MatrWVInverse * vec4(0, 0, 0, 1)));

  
  N = faceforward(N, V, N);
  N = normalize(N);
  
  float nl = dot(N, L); 
  if (nl > 0)
  {
    if (IsTextureUse > 0)
    {
      vec4 tc = texture2D(DrawTexture, DrawTexCoord);
      a = tc.a;
      color += Kd * tc.rgb * nl;
    }
    else
      color += Kd * nl;
  }

   
  vec3 R = reflect(V, N);
  float rl = dot(R, L); 
  if (rl > 0)
    color += Ks * pow(rl, Kp);

  return vec4(1 * color, (Kt * a));
}

void main( void )
{
  
  vec4 pos =  MatrView * vec4(DrawPosition, 1);
  vec4 fog = vec4(0.6, 0.6, 0.6, 1.0);
  float d = -pos.z, start = 26, end = 35, f;
  if (d < start)
    f = 1;
  else if(d > end)
    f = 0;
  else
    f = (d - end) / (start - end);
  OutColor = DrawColor * Shade(DrawPosition.xyz, DrawNormal);
  OutColor = OutColor * f + fog * (1 - f);
  
}

                   
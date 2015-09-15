#version 330
uniform sampler2D myTexture;
uniform vec4 lumpos;
uniform float balle;

in vec2 vsoTexCoord;
in vec3 vsoNormal;
in vec4 vsoModPosition;

out vec4 fragColor;

void main(void) {
//if( ! (int)balle){
  vec3 color;
  vec3 lum = normalize(vsoModPosition.xyz - lumpos.xyz);
  color = vec3(0.5) * dot(normalize(vsoNormal), -lum);
  fragColor = mix(vec4(texture(myTexture, vsoTexCoord).rgb, 1.0), vec4(color, 1.0), 0.8);
 // }
}

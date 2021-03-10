#version 120

uniform sampler2D texture;
uniform float multiply;

void main(void){
    gl_FragColor = texture2D(texture, gl_TexCoord[0].xy) * multiply;
}

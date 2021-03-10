#version 120

uniform sampler2D texture;
uniform sampler2D add_texture;
uniform float add_weight;

void main(void){
    vec4 tex_color = texture2D(texture, gl_TexCoord[0].xy);
    vec4 add_color = texture2D(add_texture, gl_TexCoord[0].xy) * add_weight;
    gl_FragColor = tex_color + add_color;
}

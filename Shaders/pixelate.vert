uniform sampler2D tex;
uniform int resolution;

void main()
{
 float dx = 15.*(1./resolution);
 float dy = 10.*(1./resolution);
 vec2 coord = vec2(dx*floor(gl_TexCoord[0].x/dx),
                   dy*floor(gl_TexCoord[0].y/dy));
 gl_FragColor = texture2D(tex, coord);
}

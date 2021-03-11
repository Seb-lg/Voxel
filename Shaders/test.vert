#ifdef GL_ES

precision mediump float;

#endif



#extension GL_OES_standard_derivatives : enable



uniform float time;

uniform vec2 mouse;

uniform vec2 resolution;

uniform vec2 surfaceSize;

varying vec2 surfacePosition;



vec2 Lissajous(float A, float B, float t, float w, float o)

{

	return vec2( A * sin( w * t + o ), B * sin(t) );

}



float f(vec2 p)

{

	p *= time;



	float A = 1.0;

	float B = 2.0;

	float w = 1.0;

	float o = 0.0;



	vec2 a = Lissajous(A,B,p.x,w,o);

	vec2 b = Lissajous(A,B,p.y,w,o);



	return fract( dot(a,b) );

}



void main( void ) {



	float v = f( surfacePosition );

	gl_FragColor = vec4( vec3( v ), 1.0 );



}

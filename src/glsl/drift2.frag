
//----- code from https://www.shadertoy.com/view/Xds3zN -------    //
// The MIT License
// Copyright © 2013 Inigo Quilez
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 

float length2( vec2 p )
{
	return sqrt( p.x*p.x + p.y*p.y );
}

float length8( vec2 p )
{
	p = p*p; p = p*p; p = p*p;
	return pow( p.x + p.y, 1.0/8.0 );
}

float sdTorus82( vec3 p, vec2 t )
{
    vec2 q = vec2(length2(p.xz)-t.x,p.y);
    return length8(q)-t.y;
}

// ------ end code from https://www.shadertoy.com/view/Xds3zN ----  // 

vec3 getGroundTube() {
  vec3 p = getLocation();
  p *= 3.0;
  float r = snoise_2(vec4(p, time / 5.0));
  float b = 0.8 + 0.2 * r;
  b *= smoothstep(0.0, 5.0, time);
  vec3 color = hsv2rgb(vec3((9.0 / 12.0) + r * 0.15, 1.0, b));
  return color;
}

vec3 tube(vec3 p, vec3 r, float off) {
  p *= rotation3dY(r.y);
  p *= rotation3dX(r.x);
  p *= rotation3dZ(r.z);

  p /= time > 1.0 ? 1.0 : bounceOut(time);

  p.y += 0.25 * sin(time / 2.0);

  float d = sdTorus82(p, vec2(0.4, 0.01));
  float b = smoothstep(0.2, 0.0, d);
  return hsv2rgb(vec3(
    fract(off + 0.12 * sin(time / 2.0) + 0.85 - 1.2 * length(d)),
    1.0, 
     b
  ));
}

vec3 getChandelier() {
  vec3 p = getLocation();

  float t = time / 2.0;

  vec3 color = tube(
    vec3(p.x, p.y, p.z),
    vec3(
      time / 2.0,
      time, 
      time / 3.0
    ),
    0.0
    );
  
  return color;
}

void main() {
  vec3 p = getLocation();
  vec3 off = getOffset();

  vec3 ground = getGroundTube();
  vec3 chandelier = getChandelier();

  gl_FragColor = vec4(
    mix(ground, chandelier, step(0.0, 1.0 - length(off))),
    1.0
  );
}

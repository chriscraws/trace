precision mediump float;
uniform float time;
uniform float tmix;
uniform sampler2D spos;
uniform sampler2D soff;
uniform sampler2D sceneA;
uniform sampler2D sceneB;

#pragma glslify: box = require('glsl-sdf-box')

#pragma glslify: hsv2rgb = require('glsl-y-hsv')
#pragma glslify: rgb2hsv = require('glsl-y-hsv/rgb2hsv')

#pragma glslify: snoise2 = require(glsl-noise/simplex/2d)
#pragma glslify: snoise3 = require(glsl-noise/simplex/3d)
#pragma glslify: snoise4 = require(glsl-noise/simplex/4d)
#pragma glslify: cnoise2 = require(glsl-noise/classic/2d)
#pragma glslify: cnoise3 = require(glsl-noise/classic/3d)
#pragma glslify: cnoise4 = require(glsl-noise/classic/4d)
#pragma glslify: pnoise2 = require(glsl-noise/periodic/2d)
#pragma glslify: pnoise3 = require(glsl-noise/periodic/3d)
#pragma glslify: pnoise4 = require(glsl-noise/periodic/4d)

#pragma glslify: rotate = require(glsl-rotate)
#pragma glslify: rotateX = require(glsl-rotate/rotateX)
#pragma glslify: rotateY = require(glsl-rotate/rotateY)
#pragma glslify: rotateZ = require(glsl-rotate/rotateZ)

#pragma glslify: eBackInOut = require(glsl-easings/back-in-out)
#pragma glslify: eBackIn = require(glsl-easings/back-in)
#pragma glslify: eBackOut = require(glsl-easings/back-out)
#pragma glslify: eBounceInOut = require(glsl-easings/bounce-in-out)
#pragma glslify: eBounceIn = require(glsl-easings/bounce-in)
#pragma glslify: eBounceOut = require(glsl-easings/bounce-out)
#pragma glslify: eCircularInOut = require(glsl-easings/circular-in-out)
#pragma glslify: eCircularIn = require(glsl-easings/circular-in)
#pragma glslify: eCircularOut = require(glsl-easings/circular-out)
#pragma glslify: eCubicInOut = require(glsl-easings/cubic-in-out)
#pragma glslify: eCubibIn = require(glsl-easings/cubic-in)
#pragma glslify: eCubicOut = require(glsl-easings/cubic-out)
#pragma glslify: eCubicInOut = require(glsl-easings/elastic-in-out)
#pragma glslify: eElasticIn = require(glsl-easings/elastic-in)
#pragma glslify: eElasticOut = require(glsl-easings/elastic-out)
#pragma glslify: eExpInOut = require(glsl-easings/exponential-in-out)
#pragma glslify: eExpIn = require(glsl-easings/exponential-in)
#pragma glslify: eExpOut = require(glsl-easings/exponential-out)
#pragma glslify: eLinear = require(glsl-easings/linear)
#pragma glslify: eQuadInOut = require(glsl-easings/quadratic-in-out)
#pragma glslify: eQuadIn = require(glsl-easings/quadratic-in)
#pragma glslify: eQuadOut = require(glsl-easings/quadratic-out)
#pragma glslify: eQuartInOut = require(glsl-easings/quartic-in-out)
#pragma glslify: eQuartIn = require(glsl-easings/quartic-in)
#pragma glslify: eQuartOut = require(glsl-easings/quartic-out)
#pragma glslify: eQuintInOut = require(glsl-easings/quintic-in-out)
#pragma glslify: eQuintIn = require(glsl-easings/quintic-in)
#pragma glslify: eQuintOut = require(glsl-easings/quintic-out)
#pragma glslify: eSineInOut = require(glsl-easings/sine-in-out)
#pragma glslify: eSineIn = require(glsl-easings/sine-in)
#pragma glslify: eSineOut = require(glsl-easings/sine-out)

#pragma glslify: random = require(glsl-random)

const vec2 BUF_D = vec2(64.0, 41.0);
const float COUNT = BUF_D.x * BUF_D.y;
const float MAX_OFF = 4.267 / (33.0  * 0.035);

vec3 getOffset() {
  vec2 uv = (gl_FragCoord.xy + 0.5) / BUF_D; 
  return (texture2D(soff, uv).xyz - 0.5) * 2.0 * MAX_OFF;
}

vec3 getLocation() {
  vec2 uv = (gl_FragCoord.xy + 0.5) / BUF_D; 
  return (texture2D(spos, uv).xyz - 0.5) * 2.0 + getOffset();
}

float getIndex() {
  return (gl_FragCoord.y * BUF_D.x) + gl_FragCoord.x;
}

vec3 getA() {
  vec2 uv = (gl_FragCoord.xy + 0.5) / BUF_D; 
  return texture2D(sceneA, uv).rgb;
}

vec3 getB() {
  vec2 uv = (gl_FragCoord.xy + 0.5) / BUF_D; 
  return texture2D(sceneB, uv).rgb;
}

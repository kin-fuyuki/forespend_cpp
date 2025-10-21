#version 330
in vec2 fragTexCoord;
in vec4 fragColor;
out vec4 finalColor;
uniform sampler2D texture0;
uniform vec4 colDiffuse;
const float BITS=8.0;
uniform bool DITHER=false;
uniform bool DEPTH=true;
uniform bool BLOOM=false;
uniform bool BLUR=false;
uniform float redbits=4.;
uniform float greenbits=4.;
uniform float bluebits=3.;
uniform int bloomquality=3;
const mat4 ditherTable=mat4(-4.0,0.0,-3.0,1.0,2.0,-2.0,3.0,-1.0,-3.0,1.0,-4.0,0.0,3.0,-1.0,2.0,-2.0);
vec3 depth( vec3 col){
 col.r+=0.01;
 col.g-=0.02;
 col.b+=0.01;
float r=exp2(redbits)-1.0;
float g=exp2(greenbits)-1.0;
float b=exp2(bluebits)-1.0;
 col.r=mix(col.r+0.5/r,col.r*(1.0+0.5/r),0.0);
 col.g=mix(col.g+0.5/g,col.g*(1.0+0.5/g),0.0);
 col.b=mix(col.b+0.5/b,col.b*(1.0+0.5/b),0.0);
 col=min(col,1.0);
 col.r=floor(col.r*r)/r;
 col.g=floor(col.g*g)/g;
 col.b=floor(col.b*b)/b;
 col.r-=0.01;
 col.g+=0.02;
 col.b-=0.01;
return col;
}
vec3 dither( vec3 col , vec2 coor){
 col+=ditherTable[int(coor.x)%4][int(coor.y)%4]*0.005;
return col;
}
vec3 blur(vec3 col){
vec3 result=vec3(0.0);
float kernel[9]=float[](1.0,2.0,1.0,2.0,4.0,2.0,1.0,2.0,1.0);
int index=0;
vec2 texelSize=1.0/vec2(textureSize(texture0,0));
for(int y=-1;y<=1;y++){
for(int x=-1;x<=1;x++){
vec2 offset=vec2(float(x),float(y))*texelSize;
 result+=texture(texture0,fragTexCoord+offset).rgb*(kernel[index]/2.);
 index++;
 }
 }
return result/8.0;
}
vec3 bloom(vec3 col){
vec3 result=vec3(0.0);
vec2 texelSize=1.0/vec2(textureSize(texture0,0));
float threshold=0.8;
float kernel[9]=float[](1.0,2.0,1.0,2.0,4.0,2.0,1.0,2.0,1.0);
for(int y=-bloomquality;y<=bloomquality;y++){
for(int x=-bloomquality;x<=bloomquality;x++){

vec2 offset=vec2(float(x),float(y))*texelSize*2.0;
vec3 sample=texture(texture0,fragTexCoord+offset).rgb;
float brightness=dot(sample,vec3(0.299,0.587,0.114));
if(brightness>threshold){
float dist=length(vec2(x,y));
result+=sample*exp(-dist*0.5);
}
}
}

return col+result*0.05;
}
void main(){
vec2 coor=gl_FragCoord.xy;
vec3 c=texture(texture0,fragTexCoord).rgb;
if (DITHER) c=dither(c,coor);
if (DEPTH) c=depth(c);
if (BLUR) c=blur(c);
if (BLOOM) c=bloom(c);
 finalColor.rgb=c;
 finalColor.a=1.0;
}
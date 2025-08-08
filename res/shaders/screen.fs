#version 330 core

uniform sampler2D tex;
uniform float opacity;
in vec2 texcoord;
float bayer4x4(int x,int y){
        int m[16]=int[16](
0       ,8      ,2      ,10,
12      ,4,     14      ,6,
3       ,11     ,1      ,9,
15      ,7      ,13     ,5
        );
        return float(m[y*4+x])/16.0;
}
float bayer2x2(int x,int y){
        int m[4]=int[4](
0,2,
3,1
        );
        return float(m[y*2+x])/4.0;
}
int ditherb=1;
void main(){
        vec2 tex_size=textureSize(tex,0);
        vec4 c=texture2D(tex,texcoord/tex_size);
        ivec2 pixel=ivec2(mod(texcoord.xy,4.0));
        float dither=bayer2x2(pixel.x,pixel.y);
        float levelsr=15.0;
        float levelsg=15.0;
        float levelsb=15.0;

        if(ditherb==1){
                if(fract(c.r*levelsr)>dither)c.r=ceil(c.r*levelsr)/levelsr;
                else c.r=floor(c.r*levelsr)/levelsr;

                if(fract(c.g*levelsg)>dither)c.g=ceil(c.g*levelsg)/levelsg;
                else c.g=floor(c.g*levelsg)/levelsg;

                if(fract(c.b*levelsb)>dither)c.b=ceil(c.b*levelsb)/levelsb;
                else c.b=floor(c.b*levelsb)/levelsb;
        }else{
                c.r=floor(c.r*levelsr+0.5)/levelsr;
                c.g=floor(c.g*levelsg+0.5)/levelsg;
                c.b=floor(c.b*levelsb+0.5)/levelsb;
        }
        return vec4(c.r,c.g,c.b,c.a);
}
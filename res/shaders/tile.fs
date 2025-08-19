#version 330 core
in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 normal;
uniform sampler2D tilemap;
uniform sampler2D tilesheet;
uniform vec2 size;
uniform int cols;
out vec4 FragColor;
vec3 n=normalize(normal);

vec4 directional(vec4 c){
	float mult;
	if(abs(n.y)>abs(n.x)&&abs(n.y)>abs(n.z)){
		mult=(n.y>0.0)?1.0:0.1;
	}else if(abs(n.x)>abs(n.y)&&abs(n.x)>abs(n.z)){
		mult=(n.x>0.0)?0.8:0.5;
	}else{
		mult=(n.z>0.0)?0.65:0.3;
	}
	vec3 c2=c.rgb;
	return vec4(c2*mult,c.a);
}

vec4 white(){
	return vec4(1.0,1.0,1.0,1.0);
}

vec4 textured(){
	return directional(white());
}
vec4 triplanar(){
	return directional(white());
}
int rendermode=2;
void main(){
	if(rendermode==3){
		FragColor=textured();
	}else if(rendermode==2){
		FragColor=triplanar();
	}else if(rendermode==1){
		FragColor=directional(fragColor);
	}else{
		FragColor=white();
	}
}
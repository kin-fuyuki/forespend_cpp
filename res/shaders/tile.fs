#version 330 core
in vec2 fragTexCoord;
uniform vec4 fragColor;
in vec3 normal;
in vec3 pos;
uniform vec3 campos;
uniform sampler2D tilemap;
uniform sampler2D tilesheet;
uniform vec2 size;
uniform int cols;
out vec4 FragColor;
float dist;
vec3 n=normalize(normal);
bool lava=false;
vec4 directional(vec4 c){
	float mult;
	if ((pos.y>=0.98))return c;
	float fac=pos.y/2+0.5;
	if(abs(n.y)>abs(n.x)&&abs(n.y)>abs(n.z)){
		mult=(n.y>0.0)?1.0:pos.y;
	}else if(abs(n.x)>abs(n.y)&&abs(n.x)>abs(n.z)){
		mult=(n.x>0.0)?fac+.2:fac+0.3;
	}else{
		mult=(n.z>0.0)?fac+.45:fac+0.1;
	}
	vec3 c2=c.rgb;
	return vec4(c2*mult,c.a);
}

vec4 white(){
	return vec4(1.0,1.0,1.0,1.0);
}

vec4 textured(){
	float minus=1./1024;
	float tile = texture(tilemap, fragTexCoord).r;
    int idx = int(tile * 255.0);
	bool terrain=idx<=250;
	if (pos.y>1.0){ //top
		if (!terrain){
			discard;
			return vec4(0.0,0.0,0.0,0.0);
		}
		
    }
	else if (pos.y>0.0){ //walls
		
		float previousx=texture(tilemap, vec2(fragTexCoord.x-minus,fragTexCoord.y)).r;
		float previousz=texture(tilemap, vec2(fragTexCoord.x,fragTexCoord.y-minus)).r;
		int previousxidx = int(previousx * 255.0);
		int previouszidx = int(previousz * 255.0);
		bool prevxidx=previousxidx<245;
		bool prevzidx=previouszidx<245;
		bool isterrain=1!=1;
		bool negx=abs(n.x) > abs(n.y) && abs(n.x) > abs(n.z);
		bool negz=abs(n.z) > abs(n.x) && abs(n.z) > abs(n.y);
		if (prevxidx&&negx&&!(idx<245)) { // forces outer on -x
			isterrain=prevxidx;
			idx=previousxidx;
		}else if (prevzidx&&negz&&!(idx<245)) { // forces outer on -z
			isterrain=prevzidx;
			idx=previouszidx;
		}
		if (!isterrain){
			if (
				(negx&&!prevxidx)
			||	(negz&&!prevzidx)
				
				)
			isterrain=(idx<245);
			
			if (!isterrain){
			discard;}
		}
		
		
	}
	if (pos.y<=0.0){
		
		if (idx<=250&&idx>245){
			
			idx+=5;
		}else{
			if (idx<=250){discard;}
		}
	}
    int x = idx % cols;
    int y = idx / cols;
    
    float tsize = 1.0 / float(cols);
    vec2 tuv = fract(fragTexCoord * size) / float(cols);
	
    return pos.y>0.0
			?directional(texture(tilesheet, vec2(float(x) * tsize, float(y) * tsize) + tuv))
			:texture(tilesheet, vec2(float(x) * tsize, float(y) * tsize) + tuv);
} 
vec4 triplanar(){
    vec3 weight = abs(n);
    weight = weight / (weight.x + weight.y + weight.z);
    
    vec4 x = texture(tilemap, pos.yz * 0.1);
    vec4 y = texture(tilemap, pos.xz * 0.1);  
    vec4 z = texture(tilemap, pos.xy * 0.1);
	
    vec4 triplanarColor = x * weight.x + y * weight.y + z * weight.z;
    
    return directional(triplanarColor);
}
int rendermode=3;
void main(){
vec3 cpos=vec3(campos.x,0,campos.z);
vec3 wpos=pos*4;
float dist = length(cpos - wpos);
	if (fragColor.r==1. && fragColor.b==1.){
		FragColor=white();
		return;
	}
	if(rendermode==3){
		FragColor=textured()*fragColor*1.4;
	}else if(rendermode==2){
		FragColor=triplanar();
	}else if(rendermode==1){
		FragColor=directional(fragColor);
	}else{
		FragColor=white();
	}
//	float fogFactor = 1.0 / exp( (dist * 1) * (dist * 1));
	FragColor.a=1/exp((dist*0.0008)*(dist*0.0008));
}
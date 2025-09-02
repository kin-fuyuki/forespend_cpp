#version 330 core
attribute vec3 vertexPosition;
attribute vec2 vertexTexCoord;
attribute vec4 vertexColor;
attribute vec3 vertexNormal;

out vec2 fragTexCoord;
out vec3 normal;
out vec3 pos;
out int level;
uniform mat4 mvp;
uniform mat4 modelmat;
void main() {
	
    fragTexCoord = vertexTexCoord;
	vec3 vpos=vertexPosition;
	if (vpos.y>1.0)	vpos.y=0.99;
	else if (vpos.y<0.0) vpos.y=0.01;
	vpos.x*=4.0;vpos.z*=4.0;
    gl_Position = mvp * vec4(vpos, 1.0);
    normal = mat3(modelmat) * vertexNormal;
	pos = (modelmat * vec4(vertexPosition, 1.0)).xyz;
	if (pos.y>=-0.01&&pos.y<=1.01){
		vec3 n = normalize(normal);
		if(abs(n.x)>abs(n.y)&&abs(n.x)>abs(n.z))
		fragTexCoord.y=-fragTexCoord.y;
		else{
			if (n.z > 0.5)
                fragTexCoord.y -= 0.1;  // Positive Z
            
			fragTexCoord.y-=0.00002;
			
		}
	}
}
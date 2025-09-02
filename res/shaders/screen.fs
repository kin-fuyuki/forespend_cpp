#version 330
in vec2 fragTexCoord;
in vec4 fragColor;

out vec4 finalColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

void main()
{
    vec4 c = texture(texture0, fragTexCoord);
	c.r=float(int( c.r*16.0 )) /16.0;
	c.g=float(int( c.g*16.0 )) /16.0;
	c.b=float(int( c.b*16.0 )) /16.0;
    finalColor = c * colDiffuse * fragColor;
}
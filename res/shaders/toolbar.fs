#version 330
in vec2 fragTexCoord;
in vec4 fragColor;

out vec4 finalColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform float time;
uniform float health;
uniform float stamina;
uniform float mana;
uniform float energy;
uniform float fatigue;
uniform float radioactivity;
uniform float xp;
#define NUM_BUBBLES 7.0
const float n = 5.1;
const float s = 7.3;


float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
}

float Hack( vec2 p, in float s)
{
    vec3 p2 = vec3(p.xy,17.3 * abs(sin(s)));
    return fract(sin(dot(p2,vec3(17.3,61.7, 12.4)))*173713.1);
}
float noise(vec2 vn) {
    const vec2 d = vec2(0.0, 1.0);
    vec2 b = floor(vn), f = smoothstep(vec2(0.0), vec2(1.0), fract(vn));
    return mix(mix(random(b), random(b + d.yx), f.x), mix(random(b + d.xy), random(b + d.yy), f.x), f.y);
}


float fbm(vec2 n) {
    float total = 0.0, amplitude = 1.0;
    for (int i = 0; i <5; i++) {
        total += noise(n) * amplitude;
        n += n*1.7;
        amplitude *= 0.47;
    }
    return total;
}

float HackNoise(in vec2 p, in float s)
{
    vec2 i = floor(p);
    vec2 f = fract(p);
    f *= f * (3.0-2.0*f);
    
    
    return mix(mix(Hack(i + vec2(0.,0.), s), Hack(i + vec2(1.,0.), s),f.x),
               mix(Hack(i + vec2(0.,1.), s), Hack(i + vec2(1.,1.), s),f.x),
               f.y) * s;
}

float Thunder(vec2 p, vec3 spec)
{
    vec2 verticalP = vec2(p.y, p.x);

    float v = - HackNoise(verticalP * 2.0, 0.25);
    v += HackNoise(verticalP * 1.1, 0.5) - HackNoise(verticalP * 1.1, 0.25);
    v += HackNoise(verticalP * 2.1, 0.25) - HackNoise(verticalP * 2.1, 0.125);
    v += HackNoise(verticalP * 4.1, 0.125)*spec.x*1.0 - HackNoise(verticalP * 8.1, 0.0625);
    v += HackNoise(verticalP * 8.1, 0.0625) - HackNoise(verticalP * 16.0, 0.03125)*spec.y*1.0;
    v += HackNoise(verticalP * 16.1, 0.03125);

    return v * 1.2;
}

// Generate bubbles in a 0..1 UV space for health bar
float bubble(vec2 uv, float t){
    uv.y = 1.0 - uv.y; // optional, depending on your UV orientation
    float col = 0.0;

    for(float i = 0.0; i < NUM_BUBBLES; i++){
        float randX = random(vec2(i, 1.24));
        float randY = random(vec2(i, 1.678));
        float speed = random(vec2(i, 3.456)) * 1.12;
        float size = mix(0.05, 0.25, random(vec2(i, 2.012)));

        // bubble position with horizontal oscillation
        float x = randX + sin(t + randX * 8.28) * 0.1;
        float y = randY + t * speed;

        vec2 pos = vec2(x, mod(y, 1.0)); // wrap vertical

        float d = length(uv - pos);

        // --- adjustable parameters ---
        float wallThickness = 0.2;  // thickness of bubble border (0..1)
        float borderSoftness = 0.02; // softness of outer edge
        float centerSmoothness = 0.55; // how soft the center glow is

        // outer edge (border)
        float edge = smoothstep(size + borderSoftness, size - wallThickness, d);

        // center glow
        float inner = smoothstep(size*centerSmoothness, 0.0, d);

        // subtle rim for more volume
        float rim = smoothstep(size + wallThickness*0.5, size - wallThickness*0.25, d);

        col += edge * 0.3 + rim * 0.2 + inner * 0.4; // adjust contributions for appearance
    }

    return clamp(col, 0.0, 1.0);
}
float gaussian(float x, float sigma){
    return exp(-0.5 * (x*x) / (sigma*sigma));
}

float heartbeat(float t, float bpm){
    float T = 60.0 / bpm;
    float phase = mod(t, T); // 0..T
    // params
    float A1 = 1.0;
    float A2 = 0.6;
    float sigma1 = 0.04;
    float sigma2 = 0.06;
    float delta = 0.34; // seconds between lub and dub

    float v1 = A1 * gaussian(phase, sigma1);
    float v2 = A2 * gaussian(phase - delta, sigma2);
    return v1 + v2; // 0..~1.6 depending on A
}
void main()
{
	/*
	xp bar is at x62 y 40
	to x197 y45
	texture size is x240 y55
	62/240=0.18
	40/55=0.72722727
	197/240=0.82083333
	45/55=0.81818181
	*/
	
    vec4 c = texture(texture0, fragTexCoord);
	if (c.a==0.){
		discard;
	}
	if (c.a<1.0 && c.b > c.r)
	{//xp
		
		if (fragTexCoord.x>0.173 && fragTexCoord.x<(0.173+(0.687*xp))
		){
		float wave=sin((-time*2.0)+(fragTexCoord.x*10))*0.5+0.8;
		vec4 bg=vec4(0.0,wave,wave,1.0);
		// cool idea to animate when gain xp
		//c.b*=100.0;
		//c.g*=100.0;
    	c = c.a * c + (1.0 - c.a) * bg;
		}
	}
	if (c.r>c.g && c.r>c.b)
	{//health
		if (fragTexCoord.y>0.18+(0.74*(1-health)) && fragTexCoord.y<0.92
		
		)
		{
			
		c.r=(c.r*1.8)-bubble(vec2(fragTexCoord.x*2,fragTexCoord.y), time)/2.;
		}
		
	}else if (c.g>c.r && c.g>c.b
	&& c.r>c.b
	){//stamina
		if (fragTexCoord.y>0.18+(0.74*(1-stamina)) && fragTexCoord.y<0.92){
			float fac=heartbeat(time, 60.0*stamina)/2;
		c.g*=1.6+fac;
		c.r*=1.6+fac;
		}
	}else if (c.b>c.r && c.g>c.b && c.g>c.r)
	{//radioactivity
		if (fragTexCoord.y>0.51+(0.44*(1-radioactivity)) && fragTexCoord.y<0.94)
		{
			float fac=2*random(fragTexCoord*vec2(100.0,100.0)+time*10.0);
			c.b*=fac+0.2;
			c.g*=fac+0.2;
		}
	}else if (c.g > c.r && c.g > c.b)
	{//fatigue
		if (fragTexCoord.y>0.51+(0.44*(1-fatigue)) && fragTexCoord.y<0.94){
		float fact= sin(-time*3.0+fragTexCoord.y*20)*0.5+1.0;
		fact*=cos(-time*2.0+fragTexCoord.x*20)*0.5+1.0;
		c.g*=1.0+fact;
		}
	}else if (c.b > c.r && c.b > c.g && c.r < 0.1 && c.a>=0.8)
	{//energy
		if (fragTexCoord.y>0.18+(0.74*(1-energy)) && fragTexCoord.y<0.92){
		
    		float WhoMadeWho = (time * s) + 1000.0;
			vec2 uv = fragTexCoord * vec2(2.0,1.0);
			uv.y-=0.8;
			float wave=sin((-time*2.0)+(uv.x*10))*0.5+0.8;
			vec3 spec = vec3(wave/1.0);
    for( float i = 1.0; i < 2; i++ )
    {
        float t = abs(4.0 / ((uv.y + Thunder( vec2(uv.x,uv.y+0.8)*5 + WhoMadeWho / i , spec)) * (i * 100.0)));
        c.rgb +=  t * vec3( i * 0.1*spec.x*2., 0.2, 1.30 );
		}
			float fac=2*random(fragTexCoord*vec2(2.0,1.0)+time*10.0)*2;}
		
	}else if (c.r > c.g && c.b > c.g && c.g < 0.1)
	{//mana
		if (fragTexCoord.y>0.18+(0.74*(1-mana)) && fragTexCoord.y<0.92){
		float fac=noise(fragTexCoord*vec2(50.0,30.0+time*10.0))*2;
		c.r*=1.0+fac;
		c.b*=1.0+fac;}
	}
    finalColor = c * colDiffuse * fragColor;
}
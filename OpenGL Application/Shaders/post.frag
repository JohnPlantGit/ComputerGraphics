// Vert shader
#version 440

in vec2 vTexCoord;

uniform sampler2D colourTarget;

out vec4 FragColour;

vec4 Default(vec2 texCoord);
vec4 BoxBlur(vec2 texCoord);
vec4 Distort(vec2 texCoord);
vec4 Sobel(vec2 texCoord);

void main() 
{
	// calculate texel size
	vec2 texSize = textureSize(colourTarget, 0);
	vec2 texelSize = 1.0f / texSize;

	// adjust texture coordinate
	vec2 scale = (texSize - texelSize) / texSize;
	vec2 texCoord = vTexCoord / scale + texelSize;

	FragColour = Default(texCoord);	
}

vec4 Default(vec2 texCoord)
{	
	return texture(colourTarget, texCoord);
}

vec4 BoxBlur(vec2 texCoord)
{
	vec2 texel = 1.0f / textureSize(colourTarget, 0);

	vec4 colour = texture(colourTarget, texCoord);
	colour += texture(colourTarget, texCoord + texel * vec2(-1,1));
	colour += texture(colourTarget, texCoord + texel * vec2(-1,0));
	colour += texture(colourTarget, texCoord + texel * vec2(-1,-1));
	colour += texture(colourTarget, texCoord + texel * vec2(0,1));
	colour += texture(colourTarget, texCoord + texel * vec2(0,-1));
	colour += texture(colourTarget, texCoord + texel * vec2(1,1));
	colour += texture(colourTarget, texCoord + texel * vec2(1,0));
	colour += texture(colourTarget, texCoord + texel * vec2(1,-1));	return colour / 9;
}

vec4 Distort(vec2 texCoord)
{	
	vec2 mid = vec2(0.5f);

	float distanceFromCentre = distance(texCoord, mid);
	vec2 normalizedCoord = normalize(texCoord - mid);
	float bias = distanceFromCentre + sin(distanceFromCentre * 15) * 0.05f;

	vec2 newCoord = mid + bias * normalizedCoord;
	return texture(colourTarget, newCoord);
}

vec4 Sobel(vec2 texCoord)
{
	mat3 sx = mat3(1.0, 2.0, 1.0,0.0, 0.0, 0.0,-1.0, -2.0, -1.0);
	mat3 sy = mat3(1.0, 0.0, -1.0,2.0, 0.0, -2.0,1.0, 0.0, -1.0);

	vec3 diffuse = texture(colourTarget, texCoord.st).rgb;
    mat3 I;
    for (int i=0; i<3; i++)
	{
        for (int j=0; j<3; j++)
		{
            vec3 temp  = texelFetch(colourTarget, ivec2(gl_FragCoord) + ivec2(i-1,j-1), 0 ).rgb;
            I[i][j] = length(temp); 
		}
	}

	float gx = dot(sx[0], I[0]) + dot(sx[1], I[1]) + dot(sx[2], I[2]); 
	float gy = dot(sy[0], I[0]) + dot(sy[1], I[1]) + dot(sy[2], I[2]);

	float g = sqrt(pow(gx, 2.0)+pow(gy, 2.0));

	//g = smoothstep(0.4, 0.8, g);

	return vec4(diffuse - vec3(g).rgb, 1.0);
}
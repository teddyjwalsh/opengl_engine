
#version 330 core
out vec4 FragColor;

in vec2 tex_coord_switch;
in vec3 normal_camspace;
in vec3 light_camspace;
in vec2 glpos;
in vec4 ShadowCoord;

in vec3 light_color;

in float cos;
in vec3 light_val;

uniform sampler2D texture0;
layout(binding=20) uniform sampler2D shadow_tex;


void main()
{
	vec3 n = normalize(normal_camspace);
	vec3 l = normalize(light_camspace);
	float bias = 0.01*tan(acos(clamp(dot(n,l),0,1))); // cosTheta is dot( n,l ), clamped between 0 and 1
	bias = clamp(bias, 0,0.005);

	float visibility = 1.0;
	
	//float shadow_dist = 2*abs(texture( shadow_tex, ShadowCoord.xy ).r  -  ShadowCoord.z);

	if ( texture( shadow_tex, ShadowCoord.xy ).r  <  ShadowCoord.z - 0.005){
		visibility = 0.0;
	}

	vec2 poissonDisk[4] = vec2[](
	  vec2( -0.94201624, -0.39906216 ),
	  vec2( 0.94558609, -0.76890725 ),
	  vec2( -0.094184101, -0.92938870 ),
	  vec2( 0.34495938, 0.29387760 )
	);

	//for (int i=0;i<4;i++){
	 //if ( texture( shadow_tex, ShadowCoord.xy + poissonDisk[i]/700.0 ).r  <  ShadowCoord.z-bias ){
		//visibility -= 0.2*shadow_dist;
	  //}
	//}

	if ( dot(l,n) <= 0.0)
	{
		visibility = 0.0;
	}

	if (ShadowCoord.x >= 1.0 || ShadowCoord.x <= 0 || ShadowCoord.y <= 0 || ShadowCoord.y >= 1.0)
	{
		visibility = 1.0;
	}

	
	//*max(visibility*dot(l,n),0.2);
	//(light_val*1/255)*
    FragColor = texture(texture0, tex_coord_switch);

	// CPU lighting test
	//FragColor.xyz = clamp(FragColor.xyz*(dot(-light_val,n)),FragColor.xyz*0.2, FragColor.xyz);

	FragColor.xyz = clamp(visibility*FragColor.xyz*(dot(l,n)),0.2*FragColor.xyz, FragColor.xyz);

	//FragColor.xyz = clamp(light_color*FragColor.xyz*max(visibility*dot(l,n),0.2) + FragColor.xyz*0.5*(dot(-light_val,n)),FragColor.xyz*0, FragColor.xyz);
	//FragColor = texture( shadow_tex, ShadowCoord.xy );
	if (FragColor.a <= 0.0)
	{
		discard;
	}

	if ((FragColor.a < 0.8) && (mod(int(gl_FragCoord.x),4) <= int(FragColor.a*4) && mod(int(gl_FragCoord.y),4) >= 4-int(FragColor.a*4)))// || (mod(gl_FragCoord.x+1,2) == 1 && mod(gl_FragCoord.y,2) == 1)))
	{
		discard;
	}
}
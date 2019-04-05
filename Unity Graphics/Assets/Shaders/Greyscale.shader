Shader "Hidden/Greyscale"
{
	Properties
	{
		_MainTex ("Texture", 2D) = "white" {}
		_bwBlend ("Black & White blend", Range(0, 1)) = 0
	}
	SubShader
	{
		// No culling or depth
		Cull Off ZWrite Off ZTest Always

		Pass
		{
			CGPROGRAM
			#pragma vertex vert_img
			#pragma fragment frag
			
			#include "UnityCG.cginc"
			
			sampler2D _MainTex;
			float _bwBlend;

			fixed4 frag(v2f_img i) : COLOR
			{
				float4 c = tex2D(_MainTex, i.uv);

				float lum = c.r * 0.3 + c.g * 0.59 + c.b * 0.11;
				float3 bw = float3(lum, lum, lum);

				float4 result = c;
				result.rgb = lerp(c.rgb, bw, _bwBlend);

				return result;
			}
			ENDCG
		}
	}
}

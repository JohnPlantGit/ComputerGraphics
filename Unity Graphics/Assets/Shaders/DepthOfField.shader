Shader "Hidden/DepthOfField"
{
	Properties
	{
		_MainTex ("Texture", 2D) = "white" {}
	}

	CGINCLUDE
		#include "UnityCG.cginc"

		sampler2D _MainTex;
		sampler2D _CameraDepthTexture;
		sampler2D _CoCTex;
		sampler2D _DoFTex;
		float4 _MainTex_TexelSize;
		float _FocusDistance;
		float _FocusRange;
		float _BokehRadius;

		struct appdata
		{
			float4 vertex : POSITION;
			float2 uv : TEXCOORD0;
		};

		struct v2f
		{
			float2 uv : TEXCOORD0;
			float4 vertex : SV_POSITION;
		};

		v2f vert(appdata v)
		{
			v2f o;
			o.vertex = UnityObjectToClipPos(v.vertex);
			o.uv = v.uv;
			return o;
		}

	ENDCG

	SubShader
	{
		// No culling or depth
		Cull Off ZWrite Off ZTest Always

		Pass // Circle of Confusion
		{
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag
										
			fixed4 frag (v2f i) : SV_Target
			{	
				float depth = tex2D(_CameraDepthTexture, i.uv).r;
				depth = LinearEyeDepth(depth);

				float coc = (depth - _FocusDistance) / _FocusRange;
				coc = clamp(-1, 1, coc) * _BokehRadius;
				return coc;
			}
			ENDCG
		}

		Pass // pre filter
		{
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag

			fixed Weigh(fixed3 c)
			{
				return 1 / (1 + max(max(c.r, c.g), c.b));
			}

			fixed4 frag(v2f i) : SV_Target
			{
				float4 offset = _MainTex_TexelSize.xyxy * float2(-0.5, 0.5).xxyy;

				fixed3 s0 = tex2D(_MainTex, i.uv + offset.xy).rgb;
				fixed3 s1 = tex2D(_MainTex, i.uv + offset.zy).rgb;
				fixed3 s2 = tex2D(_MainTex, i.uv + offset.xw).rgb;
				fixed3 s3 = tex2D(_MainTex, i.uv + offset.zw).rgb;

				fixed w0 = Weigh(s0);
				fixed w1 = Weigh(s1);
				fixed w2 = Weigh(s2);
				fixed w3 = Weigh(s3);

				fixed3 color = s0 * w0 + s1 * w1 + s2 * w2 + s3 * w3;
				color = color / max(w0 + w1 + w2 + w3, 0.00001);

				fixed coc0 = tex2D(_CoCTex, i.uv + offset.xy).r;
				fixed coc1 = tex2D(_CoCTex, i.uv + offset.zy).r;
				fixed coc2 = tex2D(_CoCTex, i.uv + offset.xw).r;
				fixed coc3 = tex2D(_CoCTex, i.uv + offset.zw).r;

				fixed cocMin = min(min(min(coc0, coc1), coc2), coc3);
				fixed cocMax = max(max(max(coc0, coc1), coc2), coc3);

				fixed coc = cocMax >= -cocMin ? cocMax : cocMin;

				return fixed4(color, coc);
			}
			ENDCG
		}

		Pass // Bokeh
		{
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag

			static const int kernelSampleCount = 16;
			static const float2 kernel[kernelSampleCount] =
			{
				float2(0, 0),
				float2(0.54545456, 0),
				float2(0.16855472, 0.5187581),
				float2(-0.44128203, 0.3206101),
				float2(-0.44128197, -0.3206102),
				float2(0.1685548, -0.5187581),
				float2(1, 0),
				float2(0.809017, 0.58778524),
				float2(0.30901697, 0.95105654),
				float2(-0.30901703, 0.9510565),
				float2(-0.80901706, 0.5877852),
				float2(-1, 0),
				float2(-0.80901694, -0.58778536),
				float2(-0.30901664, -0.9510566),
				float2(0.30901712, -0.9510565),
				float2(0.80901694, -0.5877853),
			};

			fixed Weigh(fixed coc, fixed radius)
			{
				return saturate((coc - radius + 2) / 2);
			}

			fixed4 frag(v2f i) : SV_Target
			{
				fixed3 color = 0;

				fixed coc = tex2D(_MainTex, i.uv).a;

				fixed3 bgColor = 0;
				fixed bgWeight = 0;
				fixed3 fgColor = 0;
				fixed fgWeight = 0;

				for (int k = 0; k < kernelSampleCount; k++)
				{
					float2 offset = kernel[k] * _BokehRadius;
					fixed radius = length(offset);
					offset = offset * _MainTex_TexelSize.xy;
					
					fixed4 s = tex2D(_MainTex, i.uv + offset);

					fixed bgw = Weigh(max(0, min(s.a, coc)), radius);
					bgColor += s.rgb * bgw;
					bgWeight += bgw;

					fixed fgw = Weigh(-s.a, radius);
					fgColor += s.rgb * fgw;
					fgWeight += fgw;
				}

				bgColor *= 1.0 / (bgWeight + (bgWeight == 0));
				fgColor *= 1.0 / (fgWeight + (fgWeight == 0));

				fixed bgfg = min(1, fgWeight * 3.14159265359 / kernelSampleCount);

				color = lerp(bgColor, fgColor, bgfg);

				return fixed4(color, bgfg);
			}
			ENDCG
		}

		Pass // Post Filter
		{
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag

			fixed4 frag(v2f i) : SV_Target
			{
				float4 offset = _MainTex_TexelSize.xyxy * float2(-0.5, 0.5).xxyy;
				fixed4 s = 
					tex2D(_MainTex, i.uv + offset.xy) +
					tex2D(_MainTex, i.uv + offset.zy) +
					tex2D(_MainTex, i.uv + offset.xw) +
					tex2D(_MainTex, i.uv + offset.zw);

				return s * 0.25;
			}
			ENDCG
		}

		Pass // Combine
		{
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag

			fixed4 frag(v2f i) : SV_Target
			{
				fixed4 source = tex2D(_MainTex, i.uv);
				fixed coc = tex2D(_CoCTex, i.uv).r;
				fixed4 dof = tex2D(_DoFTex, i.uv);
				
				fixed dofStrength = smoothstep(0.1, 1, abs(coc));
				fixed3 color = lerp(source.rgb, dof.rgb, dofStrength + dof.a - dofStrength * dof.a);
				return fixed4(color, source.a);
			}
			ENDCG
		}
	}
}

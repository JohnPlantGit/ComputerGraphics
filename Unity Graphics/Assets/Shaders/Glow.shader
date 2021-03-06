﻿Shader "Custom/NewSurfaceShader" 
{
	Properties 
	{
		_Color ("Color", Color) = (1,1,1,1)
		_MainTex ("Albedo (RGB)", 2D) = "white" {}
		_Glossiness("Smoothness", Range(0,1)) = 0.5
		_Metallic("Metallic", Range(0,1)) = 0.0
		_Amount("Amount", Range(-1, 1)) = 0.5
		_Outline("Outline color", Color) = (1, 0, 0, 1)
	}
	SubShader
	{
		Tags { "RenderType" = "Opaque" }
		LOD 200

		CGPROGRAM
		// Physically based Standard lighting model, and enable shadows on all light types
		#pragma surface surf Standard fullforwardshadows vertex:vert finalcolor:mycolor

		// Use shader model 3.0 target, to get nicer looking lighting
		#pragma target 3.0

		struct Input
		{
			float2 uv_MainTex;
			float3 worldPos;
			float3 worldNormal;
		};

		float _Amount;
		void vert(inout appdata_full v)
		{
			v.vertex.xyz += v.normal * _Amount;
		}		

		sampler2D _MainTex;

		half _Glossiness;
		half _Metallic;
		fixed4 _Color;

		// Add instancing support for this shader. You need to check 'Enable Instancing' on materials that use the shader.
		// See https://docs.unity3d.com/Manual/GPUInstancing.html for more information about instancing.
		// #pragma instancing_options assumeuniformscaling
		UNITY_INSTANCING_BUFFER_START(Props)
			// put more per-instance properties here
		UNITY_INSTANCING_BUFFER_END(Props)

		void surf(Input IN, inout SurfaceOutputStandard o)
		{
			// Albedo comes from a texture tinted by color
			fixed4 c = tex2D(_MainTex, IN.uv_MainTex) * _Color;
			o.Albedo = c.rgb;
			// Metallic and smoothness come from slider variables
			o.Metallic = _Metallic;
			o.Smoothness = _Glossiness;
			o.Alpha = c.a;
		
		}

		fixed4 _Outline;
		void mycolor(Input IN, SurfaceOutputStandard o, inout fixed4 color)
		{
			float facingPercentage = dot(normalize(_WorldSpaceCameraPos - IN.worldPos), IN.worldNormal);
			color = (color * facingPercentage) + _Outline * (1 - facingPercentage);
		}
		ENDCG
	}
	FallBack "Diffuse"
}

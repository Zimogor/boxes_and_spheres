// ------------------------------------------------------------------------------
// ������� ���������
// ------------------------------------------------------------------------------

cbuffer perAnimModel : register(b0) // ��� ������������ �������
{
	float4x4 gWorldAnimModel; // ������� ������� (JP ����� ������)
	float4x4  gBones[32]; // ������� ������
};
cbuffer perReset : register(b1) // ��������� ������
{
	float4x4 gOrthoMatrix; // ������� ��������������� �������������
};
cbuffer perObject : register(b2) // ��� ��������
{
	float4x4 gWorld; // ������� �������
}
cbuffer perFrame1 : register(b3) // ���������
{
	float4 lightVector; // ������ ����� (!!!���������� ������)
}
cbuffer perFont : register(b4) // ��� �������
{
	float4 fontColor; // ���� ������
}
cbuffer perFrame2 : register(b5) // ������
{
	float4 camPos; // ���������� ������
	float4 camFocus; // ����� ������
	float4x4 gViewProjMatrix; // ������� ������
}
cbuffer perFrame3 : register(b6) // ���������
{
	float4x4 gViewLightMatrix; // ������� ��������� ����� (JP ����� ������)
	float4x4 gViewLightMatrixAbs; // ������� ��������� ����� (JP ����� �����)
	float4x4 gProjLightMatrix; // �������� ��������� �����
}
cbuffer perOnce : register(b7) // ������ ���������
{
	float4 shadowMapDimentions; // ����������� ����� ����� (!!!���������� ������)
}

Texture2D textureIn : register(t0); // ����������� ��������
Texture2D shadowIn: register(t1); // ����� �����
Texture2D normalMapIn: register(t2); // ����� ��������

// ------------------------------------------------------------------------------
// ��������
// ------------------------------------------------------------------------------

SamplerState defaultSampler : register(s0);
SamplerComparisonState shadowSampler : register(s1);

// ------------------------------------------------------------------------------
// ������� �������
// ------------------------------------------------------------------------------

struct VInPos3Tex2 // ��� �������� c ���������
{
	float3 PosIn : POSITION;
	float2 TexIn : TEXCOORD0;
};
struct VInSpriteInstance // ��� ������ ����
{
	float3 PosIn : POSITION;
	float2 TexIn : TEXCOORD0;
	float3 IPosIn : INSTANCEPOS; // �������� ����������
	float2 ITexIn : INSTANCECOLOR; // �������� ��������
};
struct VInPos3Nor3 // ��� ������� ��� ��������
{
	float3 PosIn : POSITION;
	float3 NorIn : NORMAL0;
};
struct VInPos3Nor3Tex2 // ��� ������� � ���������
{
	float3 PosIn : POSITION;
	float3 NorIn : NORMAL0;
	float2 TexIn : TEXCOORD0;
};
struct VInTerrain // ��� ����������
{
	float3 PosIn : POSITION;
	float3 NorIn : NORMAL0;
	float3 TangentIn : TANGENT0; // ������� ��� ��������
	float2 TexIn1 : TEXCOORD0;
	float2 TexIn2 : TEXCOORD1;
	float2 MaskIn : TEXCOORD2;
};
struct VInPos3Nor3Tex2W1Bone2 // ��� ������������ �������
{
	float3 PosIn : POSITION;
	float3 NorIn : NORMAL0;
	float2 TexIn : TEXCOORD0;
	float WeightsIn : WEIGHTS;
	uint2 BoneIndIn : BONEINDICES;
};
struct VInInstance // ��� ������� �������
{
	float3 PosIn : POSITION;
	float3 NorIn : NORMAL0;
	float2 TexIn : TEXCOORD0;
	float3 IPosIn : INSTANCEPOS;
	float IRotIn : INSTANCECOLOR0; // �������
	float2 IScaleIn : INSTANCECOLOR1; // �������
};
struct VInPosCol // ��� �����
{
	float3 PosIn : POSITION0;
	float3 ColIn : COLOR0;
};

// ------------------------------------------------------------------------------
// �������� �������
// ------------------------------------------------------------------------------

struct VOutPos // ��� �������� ��� ��������
{
	float4 PosOut : SV_POSITION;
};
struct VOutPosTex // ��� �������� � ���������
{
	float4 PosOut : SV_POSITION;
	float2 TexOut : TEXTURE;
};
struct VOutPosNor // ��� ������� ��� ��������
{
	float4 PosOut : SV_POSITION;
	float4 NorOut : NORMAL;
};
struct VOutPosNorTex // ��� ������� � ���������
{
	float4 PosOut : SV_POSITION;
	float4 NorOut : NORMAL;
	float2 TexOut : TEXTURE;
};
struct VOutTerrain // ��� ����������
{
	float4 PosOut : SV_POSITION;
	float3 NorOut : NORMAL;
	float3 TangentOut : TANGENT; // ������� ��� ��������
	float2 TexOut1 : TEXTURE0;
	float2 TexOut2 : TEXTURE1;
	float2 MaskOut : TEXTURE2; // ����� ����������
	float4 PosOutLight : TEXTURE3; // ������� �����
};
struct VOutPosCol // ��� �����
{
	float4 PosOut : SV_POSITION;
	float3 ColOut : COLOR0;
};

// ------------------------------------------------------------------------------
// ��������� �������
// ------------------------------------------------------------------------------

VOutPosTex VSPos(VInPos3Tex2 vin) // ��� �������� ��� ��������
{
	VOutPosTex vout;
	vout.PosOut = mul(float4(vin.PosIn, 1.0f), mul(gWorld, gOrthoMatrix));
	vout.TexOut = vin.TexIn;
	return vout;
}
VOutPosTex VSPosTex(VInPos3Tex2 vin) // ��� �������� � ���������
{
	VOutPosTex vout;
	vout.PosOut = mul(float4(vin.PosIn, 1.0f), mul(gWorld, gOrthoMatrix));
	vout.TexOut = vin.TexIn;
	return vout;
}
VOutPosTex VSSpriteInstance(VInSpriteInstance vin) // ��� ������ ����
{
	VOutPosTex vout;
	vin.PosIn.x += vin.IPosIn.x;
	vin.PosIn.y += vin.IPosIn.y;
	vin.PosIn.z += vin.IPosIn.z;
	vout.PosOut = mul(float4(vin.PosIn, 1.0f), gOrthoMatrix);
	vout.TexOut = vin.TexIn + vin.ITexIn;
	return vout;
}
VOutPosNor VSPosNor(VInPos3Nor3 vin) // ��� ������� ��� ��������
{
	VOutPosNor vout;
	vout.PosOut = mul(float4(vin.PosIn, 1.0f), gWorld);
	vout.PosOut = mul(vout.PosOut, gViewProjMatrix);
	vout.NorOut = mul(float4(vin.NorIn, 0.0f), gWorld);
	return vout;
}
VOutPosNorTex VSPosNorTex(VInPos3Nor3Tex2 vin) // ��� ������� � ���������
{
	VOutPosNorTex vout;
	vout.PosOut = mul(float4(vin.PosIn, 1.0f), gWorld);
	vout.PosOut = mul(vout.PosOut, gViewProjMatrix);
	vout.NorOut = mul(float4(vin.NorIn, 0.0f), gWorld);
	vout.TexOut = vin.TexIn;
	return vout;
}
VOutTerrain VSTerrain(VInTerrain vin) // ��� ����������
{
	VOutTerrain vout;
	vout.TangentOut = normalize(vout.TangentOut);
	vout.PosOut = mul(float4(vin.PosIn, 1.0f), gWorld);
	vout.PosOutLight = mul(vout.PosOut, mul(gViewLightMatrix, gProjLightMatrix));
	vout.PosOut = mul(vout.PosOut, gViewProjMatrix);
	vout.NorOut = (float3)mul(float4(vin.NorIn, 0.0f), gWorld);
	vout.TangentOut = (float3)mul(float4(vin.TangentIn, 0.0f), gWorld);
	vout.TexOut1 = vin.TexIn1;
	vout.TexOut2 = vin.TexIn2;
	vout.MaskOut = vin.MaskIn;
	return vout;
}
VOutPosNorTex VSPosNorTexWBone(VInPos3Nor3Tex2W1Bone2 vin) // ��� ������������ �������
{
	VOutPosNorTex vout;
	float4x4 mBoneTransf = vin.WeightsIn * gBones[vin.BoneIndIn[0]] + (1 - vin.WeightsIn) * gBones[vin.BoneIndIn[1]];
	vout.PosOut = mul(float4(vin.PosIn, 1.0f), mBoneTransf);
	vout.PosOut = mul(vout.PosOut, gWorldAnimModel);
	vout.PosOut = mul(vout.PosOut, gViewProjMatrix);
	vout.NorOut = mul(float4(vin.NorIn, 0.0f), mBoneTransf);
	vout.NorOut = mul(vout.NorOut, gWorldAnimModel);
	vout.TexOut = vin.TexIn;
	return vout;
}
VOutPos VSShadow(VInPos3Nor3Tex2W1Bone2 vin) // ���� ��� ������������ �������
{
	VOutPos vout;
	float4x4 mBoneTransf = vin.WeightsIn * gBones[vin.BoneIndIn[0]] + (1 - vin.WeightsIn) * gBones[vin.BoneIndIn[1]];
	vout.PosOut = mul(float4(vin.PosIn, 1.0f), mBoneTransf);
	vout.PosOut = mul(vout.PosOut, gWorldAnimModel);
	vout.PosOut = mul(vout.PosOut, mul(gViewLightMatrix, gProjLightMatrix));
	return vout;
}
VOutPosNorTex VSInstance(VInInstance vin) // ��� ������� �������
{
	VOutPosNorTex vout;

	// ������ ���������
	float dX = vin.PosIn.x * cos(vin.IRotIn) + vin.PosIn.z * sin(vin.IRotIn);
	float dZ = vin.PosIn.z * cos(vin.IRotIn) - vin.PosIn.x * sin(vin.IRotIn);
	vin.PosIn.x = dX; vin.PosIn.z = dZ; // �������
	vin.PosIn = float3(dX * vin.IScaleIn.x, vin.PosIn.y * vin.IScaleIn.y, dZ * vin.IScaleIn.x); // �������
	vout.PosOut = float4(vin.PosIn + vin.IPosIn, 1.0f) - camPos;
	vout.PosOut.a = 1.0f;
	vout.PosOut = mul(vout.PosOut, gViewProjMatrix);

	// ������ �������� (�� ����������� �������)
	float norX = vin.NorIn.x * cos(vin.IRotIn) + vin.NorIn.z * sin(vin.IRotIn);
	float norZ = vin.NorIn.z * cos(vin.IRotIn) - vin.NorIn.x * sin(vin.IRotIn);
	vin.NorIn.x = norX; vin.NorIn.z = norZ;
	vout.NorOut = float4(vin.NorIn, 0.0f);

	vout.TexOut = vin.TexIn;
	return vout;
}
VOutPos VSShadowInstance(VInInstance vin) // ���� ��� ������� �������
{
	VOutPos vout;

	// ������ ���������
	float dX = vin.PosIn.x * cos(vin.IRotIn) + vin.PosIn.z * sin(vin.IRotIn);
	float dZ = vin.PosIn.z * cos(vin.IRotIn) - vin.PosIn.x * sin(vin.IRotIn);
	vin.PosIn.x = dX; vin.PosIn.z = dZ; // �������
	vin.PosIn = float3(dX * vin.IScaleIn.x, vin.PosIn.y * vin.IScaleIn.y, dZ * vin.IScaleIn.x); // �������
	vout.PosOut = float4(vin.PosIn + vin.IPosIn, 1.0f) - camFocus;
	vout.PosOut = mul(vout.PosOut, gViewLightMatrixAbs);
	vout.PosOut = mul(vout.PosOut, gProjLightMatrix);
	vout.PosOut.a = 1.0f;

	return vout;
}
VOutPosCol VSPosCol(VInPosCol vin) // ��� �����
{
	VOutPosCol vout;
	vout.PosOut = mul(float4(vin.PosIn, 1.0f), mul(gWorld, gOrthoMatrix));
	vout.ColOut = vin.ColIn;
	return vout;
}

// ------------------------------------------------------------------------------
// ���������� �������
// ------------------------------------------------------------------------------

float2 texOffset(int u, int v) // �������� �������� ����� ��� �� �����������
{
	return float2(u * 1.0f / shadowMapDimentions.x, v * 1.0f / shadowMapDimentions.y);
}
float4 PSPos(VOutPosTex pin) : SV_Target // ��� �������� ��� ��������
{
	return float4(0.5f, 0.5f, 0.5f, 0.5f);
	// ������� ����� �����
	float4 texColor = shadowIn.Sample(defaultSampler, pin.TexOut);
	return texColor;
}
float4 PSPosTex(VOutPosTex pin) : SV_Target // ��� �������� � ���������
{
	float4 texColor = textureIn.Sample(defaultSampler, pin.TexOut);
	clip(texColor.a - 0.1f);
	return texColor;
}
float4 PSFont(VOutPosTex pin) : SV_Target // ��� �������
{
	float4 texColor = textureIn.Sample(defaultSampler, pin.TexOut);
	clip(texColor.a - 0.1f);
	float4 result = fontColor;
	result.a = texColor.r;
	return result;
}
float4 PSSolid(VOutPosNor pin) : SV_Target // ��� �������������� �������
{
	return float4 (0.0f, 1.0f, 1.0f, 1.0f);
}
float4 PSPosNor(VOutPosNor pin) : SV_Target // ��� ������� ��� ��������
{
	float diffuse = max(dot(pin.NorOut, lightVector), 0);
	float ambient = 0.2f;
	float4 result = (diffuse + ambient) * float4(1.0f, 1.0f, 1.0f, 1.0f);
	result.a = 1.0f;
	return result;
}
float4 PSPosNorTex(VOutPosNorTex pin) : SV_Target // ��� ������� � ���������
{
	float4 texColor = textureIn.Sample(defaultSampler, pin.TexOut);
	float diffuse = max(dot(pin.NorOut, lightVector), 0);
	float ambient = 0.2f;
	float4 resColor = (diffuse + ambient) * texColor;
	return float4(resColor.r, resColor.g, resColor.b, texColor.a);
}
float4 PSTerrain(VOutTerrain pin) : SV_Target // ��� ���������� � ������
{

	// �������� ����
	float4 texColor1 = textureIn.Sample(defaultSampler, pin.TexOut1);
	float4 texColor2 = textureIn.Sample(defaultSampler, pin.TexOut2);
	float4 texMask = textureIn.Sample(defaultSampler, pin.MaskOut);
	float4 texColor = texMask.a * texColor1 + (1.0f - texMask.a) * texColor2;

	// ����(������) �������
	float4 norColor1 = normalMapIn.Sample(defaultSampler, pin.TexOut1);
	float4 norColor2 = normalMapIn.Sample(defaultSampler, pin.TexOut2);
	float3 norColor = (float3)(texMask.a * norColor1 + (1.0f - texMask.a) * norColor2);
	norColor = (2.0f * norColor) - 1.0f;
	
	// ������������ ������������ Tangent Space
	float3 biTangent = cross(pin.NorOut, pin.TangentOut);
	float3x3 texSpace = float3x3(pin.TangentOut, biTangent, pin.NorOut);
	pin.NorOut = normalize(mul(norColor, texSpace));

	// ���������� �����
	float diffuse = max(dot(pin.NorOut, (float3)lightVector), 0.0f);
	float ambient = 0.2f;
	float4 resColor = (diffuse + ambient) * texColor;
	float4 result = float4(resColor.r, resColor.g, resColor.b, 1.0f);

	// �� ��������� ��������� ����
	pin.PosOutLight.xyz /= pin.PosOutLight.w;
	if (pin.PosOutLight.x < -1.0f || pin.PosOutLight.x > 1.0f ||
		pin.PosOutLight.y < -1.0f || pin.PosOutLight.y > 1.0f ||
		pin.PosOutLight.z < 0.0f || pin.PosOutLight.z > 1.0f) return result;
	// ������� ����
	pin.PosOutLight.x = pin.PosOutLight.x / 2 + 0.5;
	pin.PosOutLight.y = pin.PosOutLight.y / -2 + 0.5;

	// ������������������� �����
	float sum = 0;
	float x, y;
	for (y = -1.5; y <= 1.5; y += 1.0)
	{
		for (x = -1.5; x <= 1.5; x += 1.0)
		{
			float2 offset = float2(x, y);
				sum += shadowIn.SampleCmpLevelZero(shadowSampler, pin.PosOutLight.xy + texOffset(x, y), pin.PosOutLight.z);
		}
	}

	float shadowFactor = sum / 16.0;
	shadowFactor = shadowFactor * 0.5f + 0.5f;
	return result * shadowFactor; // *shadowFactor;
}
float4 PSTerrainNoShadow(VOutTerrain pin) : SV_Target // ��� ���������� ��� �����
{
	// �������� ����
	float4 texColor1 = textureIn.Sample(defaultSampler, pin.TexOut1);
	float4 texColor2 = textureIn.Sample(defaultSampler, pin.TexOut2);
	float4 texMask = textureIn.Sample(defaultSampler, pin.MaskOut);
	float4 texColor = texMask.a * texColor1 + (1.0f - texMask.a) * texColor2;
	float diffuse = max(dot(pin.NorOut, (float3)lightVector), 0);
	float ambient = 0.2f;
	float4 resColor = (diffuse + ambient) * texColor;
	float4 result = float4(resColor.r, resColor.g, resColor.b, 1.0f);
	return result;
}
float4 PSClipping(VOutPosNorTex pin) : SV_Target // ��� ������� � ��������� � � alpha-clipping
{
	float4 texColor = textureIn.Sample(defaultSampler, pin.TexOut);
	clip(texColor.a - 0.1f);
	float diffuse = max(dot(pin.NorOut, lightVector), 0);
	float ambient = 0.2f;
	float4 resColor = (diffuse + ambient) * texColor;
		return float4(resColor.r, resColor.g, resColor.b, texColor.a);
}
float4 PSTest(VOutPosNorTex pin) : SV_Target // ��� �������� �����������
{
	return float4 (1.0f, 1.0f, 1.0f, 1.0f);
}
float4 PSPosCol(VOutPosCol pin) : SV_Target // ��� �����
{
	return float4 (pin.ColOut, 1.0f);
}
void PSEmpty(VOutPos pin) {} // ������ �������

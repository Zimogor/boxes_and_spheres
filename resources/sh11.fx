// ------------------------------------------------------------------------------
// входные параметры
// ------------------------------------------------------------------------------

cbuffer perAnimModel : register(b0) // для анимационных моделей
{
	float4x4 gWorldAnimModel; // мировая матрица (JP через камеру)
	float4x4  gBones[32]; // матрицы костей
};
cbuffer perReset : register(b1) // параметры камеры
{
	float4x4 gOrthoMatrix; // матрица ортонормального проецирования
};
cbuffer perObject : register(b2) // для объектов
{
	float4x4 gWorld; // мировая матрица
}
cbuffer perFrame1 : register(b3) // освещение
{
	float4 lightVector; // вектор света (!!!пиксельный шейдер)
}
cbuffer perFont : register(b4) // для шрифтов
{
	float4 fontColor; // цвет шрифта
}
cbuffer perFrame2 : register(b5) // камера
{
	float4 camPos; // координаты камеры
	float4 camFocus; // фокус камеры
	float4x4 gViewProjMatrix; // матрицы камеры
}
cbuffer perFrame3 : register(b6) // освещение
{
	float4x4 gViewLightMatrix; // матрица источника света (JP через камеру)
	float4x4 gViewLightMatrixAbs; // матрица источника света (JP через героя)
	float4x4 gProjLightMatrix; // проекция источника света
}
cbuffer perOnce : register(b7) // вечные константы
{
	float4 shadowMapDimentions; // размерность карты теней (!!!пиксельный шейдер)
}

Texture2D textureIn : register(t0); // загруженная текстура
Texture2D shadowIn: register(t1); // карта теней
Texture2D normalMapIn: register(t2); // карта нормалей

// ------------------------------------------------------------------------------
// сэмплеры
// ------------------------------------------------------------------------------

SamplerState defaultSampler : register(s0);
SamplerComparisonState shadowSampler : register(s1);

// ------------------------------------------------------------------------------
// входные вершины
// ------------------------------------------------------------------------------

struct VInPos3Tex2 // для спрайтов c текстурой
{
	float3 PosIn : POSITION;
	float2 TexIn : TEXCOORD0;
};
struct VInSpriteInstance // для иконок меню
{
	float3 PosIn : POSITION;
	float2 TexIn : TEXCOORD0;
	float3 IPosIn : INSTANCEPOS; // смещение координаты
	float2 ITexIn : INSTANCECOLOR; // смещение текстуры
};
struct VInPos3Nor3 // для моделей без текстуры
{
	float3 PosIn : POSITION;
	float3 NorIn : NORMAL0;
};
struct VInPos3Nor3Tex2 // для моделей с текстурой
{
	float3 PosIn : POSITION;
	float3 NorIn : NORMAL0;
	float2 TexIn : TEXCOORD0;
};
struct VInTerrain // для территории
{
	float3 PosIn : POSITION;
	float3 NorIn : NORMAL0;
	float3 TangentIn : TANGENT0; // тангент для нормалей
	float2 TexIn1 : TEXCOORD0;
	float2 TexIn2 : TEXCOORD1;
	float2 MaskIn : TEXCOORD2;
};
struct VInPos3Nor3Tex2W1Bone2 // для анимационных моделей
{
	float3 PosIn : POSITION;
	float3 NorIn : NORMAL0;
	float2 TexIn : TEXCOORD0;
	float WeightsIn : WEIGHTS;
	uint2 BoneIndIn : BONEINDICES;
};
struct VInInstance // для вектора моделей
{
	float3 PosIn : POSITION;
	float3 NorIn : NORMAL0;
	float2 TexIn : TEXCOORD0;
	float3 IPosIn : INSTANCEPOS;
	float IRotIn : INSTANCECOLOR0; // поворот
	float2 IScaleIn : INSTANCECOLOR1; // масштаб
};
struct VInPosCol // для карты
{
	float3 PosIn : POSITION0;
	float3 ColIn : COLOR0;
};

// ------------------------------------------------------------------------------
// выходные вершины
// ------------------------------------------------------------------------------

struct VOutPos // для спрайтов без текстуры
{
	float4 PosOut : SV_POSITION;
};
struct VOutPosTex // для спрайтов с текстурой
{
	float4 PosOut : SV_POSITION;
	float2 TexOut : TEXTURE;
};
struct VOutPosNor // для моделей без текстуры
{
	float4 PosOut : SV_POSITION;
	float4 NorOut : NORMAL;
};
struct VOutPosNorTex // для моделей с текстурой
{
	float4 PosOut : SV_POSITION;
	float4 NorOut : NORMAL;
	float2 TexOut : TEXTURE;
};
struct VOutTerrain // для территории
{
	float4 PosOut : SV_POSITION;
	float3 NorOut : NORMAL;
	float3 TangentOut : TANGENT; // тангент для нормалей
	float2 TexOut1 : TEXTURE0;
	float2 TexOut2 : TEXTURE1;
	float2 MaskOut : TEXTURE2; // маска смешивания
	float4 PosOutLight : TEXTURE3; // матрица теней
};
struct VOutPosCol // для карты
{
	float4 PosOut : SV_POSITION;
	float3 ColOut : COLOR0;
};

// ------------------------------------------------------------------------------
// вершинные шейдеры
// ------------------------------------------------------------------------------

VOutPosTex VSPos(VInPos3Tex2 vin) // для спрайтов без текстуры
{
	VOutPosTex vout;
	vout.PosOut = mul(float4(vin.PosIn, 1.0f), mul(gWorld, gOrthoMatrix));
	vout.TexOut = vin.TexIn;
	return vout;
}
VOutPosTex VSPosTex(VInPos3Tex2 vin) // для спрайтов с текстурой
{
	VOutPosTex vout;
	vout.PosOut = mul(float4(vin.PosIn, 1.0f), mul(gWorld, gOrthoMatrix));
	vout.TexOut = vin.TexIn;
	return vout;
}
VOutPosTex VSSpriteInstance(VInSpriteInstance vin) // для иконок меню
{
	VOutPosTex vout;
	vin.PosIn.x += vin.IPosIn.x;
	vin.PosIn.y += vin.IPosIn.y;
	vin.PosIn.z += vin.IPosIn.z;
	vout.PosOut = mul(float4(vin.PosIn, 1.0f), gOrthoMatrix);
	vout.TexOut = vin.TexIn + vin.ITexIn;
	return vout;
}
VOutPosNor VSPosNor(VInPos3Nor3 vin) // для моделей без текстуры
{
	VOutPosNor vout;
	vout.PosOut = mul(float4(vin.PosIn, 1.0f), gWorld);
	vout.PosOut = mul(vout.PosOut, gViewProjMatrix);
	vout.NorOut = mul(float4(vin.NorIn, 0.0f), gWorld);
	return vout;
}
VOutPosNorTex VSPosNorTex(VInPos3Nor3Tex2 vin) // для моделей с текстурой
{
	VOutPosNorTex vout;
	vout.PosOut = mul(float4(vin.PosIn, 1.0f), gWorld);
	vout.PosOut = mul(vout.PosOut, gViewProjMatrix);
	vout.NorOut = mul(float4(vin.NorIn, 0.0f), gWorld);
	vout.TexOut = vin.TexIn;
	return vout;
}
VOutTerrain VSTerrain(VInTerrain vin) // для территории
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
VOutPosNorTex VSPosNorTexWBone(VInPos3Nor3Tex2W1Bone2 vin) // для анимационных моделей
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
VOutPos VSShadow(VInPos3Nor3Tex2W1Bone2 vin) // тень для анимационных моделей
{
	VOutPos vout;
	float4x4 mBoneTransf = vin.WeightsIn * gBones[vin.BoneIndIn[0]] + (1 - vin.WeightsIn) * gBones[vin.BoneIndIn[1]];
	vout.PosOut = mul(float4(vin.PosIn, 1.0f), mBoneTransf);
	vout.PosOut = mul(vout.PosOut, gWorldAnimModel);
	vout.PosOut = mul(vout.PosOut, mul(gViewLightMatrix, gProjLightMatrix));
	return vout;
}
VOutPosNorTex VSInstance(VInInstance vin) // для вектора моделей
{
	VOutPosNorTex vout;

	// расчёт координат
	float dX = vin.PosIn.x * cos(vin.IRotIn) + vin.PosIn.z * sin(vin.IRotIn);
	float dZ = vin.PosIn.z * cos(vin.IRotIn) - vin.PosIn.x * sin(vin.IRotIn);
	vin.PosIn.x = dX; vin.PosIn.z = dZ; // поворот
	vin.PosIn = float3(dX * vin.IScaleIn.x, vin.PosIn.y * vin.IScaleIn.y, dZ * vin.IScaleIn.x); // масштаб
	vout.PosOut = float4(vin.PosIn + vin.IPosIn, 1.0f) - camPos;
	vout.PosOut.a = 1.0f;
	vout.PosOut = mul(vout.PosOut, gViewProjMatrix);

	// расчёт нормалей (не учитывается масштаб)
	float norX = vin.NorIn.x * cos(vin.IRotIn) + vin.NorIn.z * sin(vin.IRotIn);
	float norZ = vin.NorIn.z * cos(vin.IRotIn) - vin.NorIn.x * sin(vin.IRotIn);
	vin.NorIn.x = norX; vin.NorIn.z = norZ;
	vout.NorOut = float4(vin.NorIn, 0.0f);

	vout.TexOut = vin.TexIn;
	return vout;
}
VOutPos VSShadowInstance(VInInstance vin) // тень для вектора моделей
{
	VOutPos vout;

	// расчёт координат
	float dX = vin.PosIn.x * cos(vin.IRotIn) + vin.PosIn.z * sin(vin.IRotIn);
	float dZ = vin.PosIn.z * cos(vin.IRotIn) - vin.PosIn.x * sin(vin.IRotIn);
	vin.PosIn.x = dX; vin.PosIn.z = dZ; // поворот
	vin.PosIn = float3(dX * vin.IScaleIn.x, vin.PosIn.y * vin.IScaleIn.y, dZ * vin.IScaleIn.x); // масштаб
	vout.PosOut = float4(vin.PosIn + vin.IPosIn, 1.0f) - camFocus;
	vout.PosOut = mul(vout.PosOut, gViewLightMatrixAbs);
	vout.PosOut = mul(vout.PosOut, gProjLightMatrix);
	vout.PosOut.a = 1.0f;

	return vout;
}
VOutPosCol VSPosCol(VInPosCol vin) // для карты
{
	VOutPosCol vout;
	vout.PosOut = mul(float4(vin.PosIn, 1.0f), mul(gWorld, gOrthoMatrix));
	vout.ColOut = vin.ColIn;
	return vout;
}

// ------------------------------------------------------------------------------
// пиксельные шейдеры
// ------------------------------------------------------------------------------

float2 texOffset(int u, int v) // смещение текстуры теней для их сглаживания
{
	return float2(u * 1.0f / shadowMapDimentions.x, v * 1.0f / shadowMapDimentions.y);
}
float4 PSPos(VOutPosTex pin) : SV_Target // для спрайтов без текстуры
{
	return float4(0.5f, 0.5f, 0.5f, 0.5f);
	// примесь карты теней
	float4 texColor = shadowIn.Sample(defaultSampler, pin.TexOut);
	return texColor;
}
float4 PSPosTex(VOutPosTex pin) : SV_Target // для спрайтов с текстурой
{
	float4 texColor = textureIn.Sample(defaultSampler, pin.TexOut);
	clip(texColor.a - 0.1f);
	return texColor;
}
float4 PSFont(VOutPosTex pin) : SV_Target // для шрифтов
{
	float4 texColor = textureIn.Sample(defaultSampler, pin.TexOut);
	clip(texColor.a - 0.1f);
	float4 result = fontColor;
	result.a = texColor.r;
	return result;
}
float4 PSSolid(VOutPosNor pin) : SV_Target // для сплошноцветных моделей
{
	return float4 (0.0f, 1.0f, 1.0f, 1.0f);
}
float4 PSPosNor(VOutPosNor pin) : SV_Target // для моделей без текстуры
{
	float diffuse = max(dot(pin.NorOut, lightVector), 0);
	float ambient = 0.2f;
	float4 result = (diffuse + ambient) * float4(1.0f, 1.0f, 1.0f, 1.0f);
	result.a = 1.0f;
	return result;
}
float4 PSPosNorTex(VOutPosNorTex pin) : SV_Target // для моделей с текстурой
{
	float4 texColor = textureIn.Sample(defaultSampler, pin.TexOut);
	float diffuse = max(dot(pin.NorOut, lightVector), 0);
	float ambient = 0.2f;
	float4 resColor = (diffuse + ambient) * texColor;
	return float4(resColor.r, resColor.g, resColor.b, texColor.a);
}
float4 PSTerrain(VOutTerrain pin) : SV_Target // для территории с тенями
{

	// основной цвет
	float4 texColor1 = textureIn.Sample(defaultSampler, pin.TexOut1);
	float4 texColor2 = textureIn.Sample(defaultSampler, pin.TexOut2);
	float4 texMask = textureIn.Sample(defaultSampler, pin.MaskOut);
	float4 texColor = texMask.a * texColor1 + (1.0f - texMask.a) * texColor2;

	// цвет(вектор) нормали
	float4 norColor1 = normalMapIn.Sample(defaultSampler, pin.TexOut1);
	float4 norColor2 = normalMapIn.Sample(defaultSampler, pin.TexOut2);
	float3 norColor = (float3)(texMask.a * norColor1 + (1.0f - texMask.a) * norColor2);
	norColor = (2.0f * norColor) - 1.0f;
	
	// координатное пространство Tangent Space
	float3 biTangent = cross(pin.NorOut, pin.TangentOut);
	float3x3 texSpace = float3x3(pin.TangentOut, biTangent, pin.NorOut);
	pin.NorOut = normalize(mul(norColor, texSpace));

	// вычисление цвета
	float diffuse = max(dot(pin.NorOut, (float3)lightVector), 0.0f);
	float ambient = 0.2f;
	float4 resColor = (diffuse + ambient) * texColor;
	float4 result = float4(resColor.r, resColor.g, resColor.b, 1.0f);

	// за пределами светового луча
	pin.PosOutLight.xyz /= pin.PosOutLight.w;
	if (pin.PosOutLight.x < -1.0f || pin.PosOutLight.x > 1.0f ||
		pin.PosOutLight.y < -1.0f || pin.PosOutLight.y > 1.0f ||
		pin.PosOutLight.z < 0.0f || pin.PosOutLight.z > 1.0f) return result;
	// рассчёт тени
	pin.PosOutLight.x = pin.PosOutLight.x / 2 + 0.5;
	pin.PosOutLight.y = pin.PosOutLight.y / -2 + 0.5;

	// мультисэмплирование теней
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
float4 PSTerrainNoShadow(VOutTerrain pin) : SV_Target // для территории без теней
{
	// основной цвет
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
float4 PSClipping(VOutPosNorTex pin) : SV_Target // для моделей с текстурой и с alpha-clipping
{
	float4 texColor = textureIn.Sample(defaultSampler, pin.TexOut);
	clip(texColor.a - 0.1f);
	float diffuse = max(dot(pin.NorOut, lightVector), 0);
	float ambient = 0.2f;
	float4 resColor = (diffuse + ambient) * texColor;
		return float4(resColor.r, resColor.g, resColor.b, texColor.a);
}
float4 PSTest(VOutPosNorTex pin) : SV_Target // для тестовых манипуляций
{
	return float4 (1.0f, 1.0f, 1.0f, 1.0f);
}
float4 PSPosCol(VOutPosCol pin) : SV_Target // для карты
{
	return float4 (pin.ColOut, 1.0f);
}
void PSEmpty(VOutPos pin) {} // пустой теневой

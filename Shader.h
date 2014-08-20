#ifndef SHADER_H
#define SHADER_H

#include "Utilities.h"
#include "Nums.h"
#include "Constants.h"

class Shader {

public:

	// ������
	Shader(bool* result); // �����������
	~Shader(); // ����������

	// ������ �������
	bool Update(); // ��������

	// �������
	bool SetAlphaBlending(bool enable); // ����������/����� alpha blending
	bool SetObjectBuffer(XMFLOAT4X4* worldMatrix); // ���������� ������� �������
	bool SetObjectBuffer(CXMMATRIX worldMatrix); // ���������� ������� �������
	bool SetFontBuffer(XMFLOAT4* fontColor); // ���������� ���� ������
	bool SetResetBuffer(CXMMATRIX orthoMatrix); // ���������� ��������� ������
	bool SetCamBuffer(CXMMATRIX viewProjMatrix, XMFLOAT4 camPos, XMFLOAT4 camFocus); // ���������� ��������� ������
	bool SetWireframe(bool wireframe); // ����������/����� wireframe

	// ���������� ����������
	XMMATRIX GetViewLightMatrix(); // ������� ��������� �����
	XMMATRIX GetProjLightMatrix(); // ������� �������� �����
	ID3D11DepthStencilView* GetShadowMapView() { return pShadowMapDepthView; }; // ����� �����
	ID3D11ShaderResourceView* GetShadowResourceView() { return pShadowResourceView; }; // ������ ��� ����� �����

private:

	// ����������
	ID3D11BlendState* pBlendState = NULL; // blendState
	ID3D10Blob* VSBlob = NULL; // ��� �������� ��������� ��������
	ID3D10Blob* PSBlob = NULL; // ��� �������� ���������� ��������
	ID3D11RasterizerState* pRastStandard = NULL; // �������� ����������� ������������
	ID3D11RasterizerState* pRastWireframe = NULL; // ������������ ������
	ID3D11DepthStencilView* pShadowMapDepthView = NULL; // ����� �����
	ID3D11ShaderResourceView* pShadowResourceView = NULL; // ������ ��� ����� �����

	// �������
	bool CreateShadowMap(); // �������� ����� �����
	bool CreateVertexShadersAndLayouts(); // ������� ��������� ������� � �������� ������
	bool CreatePixelShaders(); // ������� ���������� �������
	bool CreateConstBuffers(); // ������� ����������� ������
	bool CreateStates(); // ������� blend state, sampler state � render state

	// ��������� ������
	ID3D11InputLayout* pInputLayoutPos3Tex2 = NULL; // ��� �������� � ���������
	ID3D11InputLayout* pInputLayoutSpriteInstance = NULL; // ��� ������ ����
	ID3D11InputLayout* pInputLayoutPos3Nor3 = NULL; // ��� ������� ��� ��������
	ID3D11InputLayout* pInputLayoutPos3Nor3Tex2 = NULL; // ��� ������� c ���������
	ID3D11InputLayout* pInputLayoutTerrain = NULL; // ��� ����������
	ID3D11InputLayout* pInputLayoutPos3Nor3Tex2W1Bone2 = NULL; // ��� ������������ �������
	ID3D11InputLayout* pInputLayoutInstance = NULL; // ��� ������� ������� � ���������
	ID3D11InputLayout* pInputLayoutPosCol = NULL; // ��� �����

	// ��������� �������
	ID3D11VertexShader* pVSPos = NULL; // ��� �������� ��� ��������
	ID3D11VertexShader* pVSPosTex = NULL; // ��� �������� � ���������
	ID3D11VertexShader* pVSSpriteInstance = NULL; // ��� ������ ����
	ID3D11VertexShader* pVSPosNor = NULL; // ��� ������� ��� ��������
	ID3D11VertexShader* pVSPosNorTex = NULL; // ��� ������� � ���������
	ID3D11VertexShader* pVSTerrain = NULL; // ��� �������� ����������
	ID3D11VertexShader* pVSPosNorTexWBone = NULL; // ��� ������������ ������� � �������
	ID3D11VertexShader* pVSShadow = NULL; // ���� ��� ������������ �������
	ID3D11VertexShader* pVSInstance = NULL; // ��� �������� �������
	ID3D11VertexShader* pVSShadowInstance = NULL; // ��� ����� �������� �������
	ID3D11VertexShader* pVSPosCol = NULL; // ��� �����

	// ���������� �������
	ID3D11PixelShader* pPSPos = NULL; // ��� �������� ��� ��������
	ID3D11PixelShader* pPSPosTex = NULL; // ��� �������� � ���������
	ID3D11PixelShader* pPSFont = NULL; // ��� �������
	ID3D11PixelShader* pPSSolidModel = NULL; // ��� �������� �������
	ID3D11PixelShader* pPSPosNor = NULL; // ��� ������� ��� ��������
	ID3D11PixelShader* pPSPosNorTex = NULL; // ��� ������� � ���������
	ID3D11PixelShader* pPSClipping = NULL; // ��� ������� � ��������� � ����������
	ID3D11PixelShader* pPSTerrain = NULL; // ��� �������� ����������
	ID3D11PixelShader* pPSTerrainNoShadow = NULL; // ��� �������� ���������� (��� ����)
	ID3D11PixelShader* pPSEmpty = NULL; // ������� ������
	ID3D11PixelShader* pPSPosCol = NULL; // ��� �����

	// ��������� ����������� ������
	ID3D11Buffer* cbufLightVector = NULL; // ������������ �������������
	ID3D11Buffer* cbufAnimModel = NULL; // ��� ������������ �������
	ID3D11Buffer* cbufReset = NULL; // ��� ������
	ID3D11Buffer* cbufObject = NULL; // ��� ��������
	ID3D11Buffer* cbufFont = NULL; // ��� �������
	ID3D11Buffer* cbufFrame = NULL; // ��� ������� �����
	ID3D11Buffer* cbufLight = NULL; // ��� ����� �����
	ID3D11Buffer* cbufOnce = NULL; // ������ ���������

};

#endif // SHADER_H
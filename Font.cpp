#include "Font.h"
#include "Mediator.h"

// ����������� ������
Font::Font(XMFLOAT3 color, FONT_TYPE fontType, float zDepth, bool* result) {

	// ������������� ����������
	{
		// ��������� ������
		fontConstants.color = XMFLOAT4(color.x, color.y, color.z, 1.0f);
		fontConstants.height = 22;
		fontConstants.symbolsAmount = 163;
		fontConstants.zDepth = zDepth;
	}
	switch (fontType) {
	case FT_CONSOLE: {
		HRR(D3DX11CreateShaderResourceViewFromFile(Mediator::pDev, L"resources//stuff//consoleFont.png", NULL, NULL, &pSRtexture, NULL));
		BRR(CreateConsoleDescriptor());
		break;
	}
	case FT_SLAVIC: {
		HRR(D3DX11CreateShaderResourceViewFromFile(Mediator::pDev, L"resources//stuff//slavicFont.png", NULL, NULL, &pSRtexture, NULL));
		BRR(CreateSlavicDescriptor());
		break;
	}
	}

	*result = true;

}

// �������� ����� ������ (�� ����������� �������� ������ ���� ������ � �����������)
bool Font::TextOutXY(const wchar_t* text, XMFLOAT2 position, SPRITE_ALIGN fontAlign, bool center, bool specialColorB, XMFLOAT3 specialColorF) {

	bool result;
	Sentence* newSentence;

	BRRC1(newSentence = new Sentence(text, position, descriptor, fontConstants, fontAlign, center, specialColorB, specialColorF, &result));
	sentences.push_back(newSentence);

	return true;


}

// ��������� ������
bool Font::Draw() { // ������� ���������

	// ��� ������
	if (!sentences.size()) return true;

	// ���������� ���������
	Mediator::pDevContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Mediator::pDevContext->IASetInputLayout(Mediator::pInputLayoutPos3Tex2);
	Mediator::pDevContext->PSSetShaderResources(TEXTURE_BUFFER, 1, &pSRtexture);
	BR(Mediator::shader->SetFontBuffer(&fontConstants.color));
	Mediator::pDevContext->VSSetShader(Mediator::pVSPosTex, NULL, 0);
	Mediator::pDevContext->PSSetShader(Mediator::pPSFont, NULL, 0);

	// ��������� ����������� �����������
	for (UINT i(0); i < sentences.size(); i++)
		if (!sentences[i]->specialColorB) { BR(sentences[i]->Draw()); }
	// ��������� ����������� ������� �����
	for (UINT i(0); i < sentences.size(); i++)
		if (sentences[i]->specialColorB) { BR(sentences[i]->Draw()); }

	return true;

}
bool Font::Draw(ButtonsDesc* buttonsDesc) { // ��������� ������ �� �������

	XMFLOAT4 dimcolor = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);

	// ���������� ���������
	Mediator::pDevContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Mediator::pDevContext->IASetInputLayout(Mediator::pInputLayoutPos3Tex2);
	Mediator::pDevContext->PSSetShaderResources(TEXTURE_BUFFER, 1, &pSRtexture);
	Mediator::pDevContext->VSSetShader(Mediator::pVSPosTex, NULL, 0);
	Mediator::pDevContext->PSSetShader(Mediator::pPSFont, NULL, 0);

	// ��������� �����������
	for (UINT i(0); i < sentences.size(); i++) {		

		// ���������� ���� ������
		if (buttonsDesc[i].enable) { BR(Mediator::shader->SetFontBuffer(&fontConstants.color)); }
		else { BR(Mediator::shader->SetFontBuffer(&dimcolor)); }

		// ��������
		BR(sentences[i]->Draw());
	}

	return true;

}

// ����� �����������
bool Font::Reset() {

	for (auto iter = sentences.begin(); iter != sentences.end(); iter++) {
		BR((*iter)->Reset());
	}

	return true;

}

// ������� ��� �����������
bool Font::DeleteAll() {

	for (UINT i(0); i < sentences.size(); i++) {
		ReleaseNULL(sentences[i]);
	}
	sentences.clear();

	return true;

}

// �������� �����������
bool Font::ReplaceSentence(UINT number, const wchar_t* newText) {

	bool result;
	XMFLOAT2 oldPosition = sentences[number]->getPosition();
	SPRITE_ALIGN oldFontAlign = sentences[number]->getAlignment();
	ReleaseNULL(sentences[number]);
	BRRC1(sentences[number] = new Sentence(newText, oldPosition, descriptor, fontConstants, oldFontAlign, false, false, XMFLOAT3(), &result));

	return true;

}

// ������� �����������
bool Font::Delete(UINT number) {

	// ������� ������ �����������
	delete sentences[number];
	sentences.erase(sentences.begin() + number);

	return true;

}

// �������� ��� ����������� ������� �� start (������ �����)
bool Font::MoveSentences(UINT start, XMFLOAT2 translation, bool delStart) {

	if (delStart) {

		//// ������� ������ �����������
		BR(Delete(start));

	}

	// �������� ��� �����������
	for (UINT i(start); i < sentences.size(); i ++) {

		sentences[i]->Move(translation);

	}

	return true;

}

// �������� ���������� ������ �����������
bool Font::SetPosition(UINT sentenceNumber, XMFLOAT3 newPosition) {

	BR(sentences[sentenceNumber]->SetPosition(newPosition));

	return true;

}
bool Font::SetPosition(UINT sentenceNumber, XMFLOAT2 newPosition) {

	BR(sentences[sentenceNumber]->SetPosition(newPosition));

	return true;

}

// ���������� ������
Font::~Font() {

	for (UINT i(0); i < sentences.size(); i ++) {
		ReleaseNULL(sentences[i]);
	}

	ReleaseNULLS(descriptor);
	ReleaseCOM(pSRtexture);

}

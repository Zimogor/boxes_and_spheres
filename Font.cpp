#include "Font.h"
#include "Mediator.h"

// конструктор шрифта
Font::Font(XMFLOAT3 color, FONT_TYPE fontType, float zDepth, bool* result) {

	// инициализаци€ переменных
	{
		// параметры шрифта
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

// создание новой строки (по координатам верхнего левого угла экрана и предложени€)
bool Font::TextOutXY(const wchar_t* text, XMFLOAT2 position, SPRITE_ALIGN fontAlign, bool center, bool specialColorB, XMFLOAT3 specialColorF) {

	bool result;
	Sentence* newSentence;

	BRRC1(newSentence = new Sentence(text, position, descriptor, fontConstants, fontAlign, center, specialColorB, specialColorF, &result));
	sentences.push_back(newSentence);

	return true;


}

// рисование шрифта
bool Font::Draw() { // обычное рисование

	// нет текста
	if (!sentences.size()) return true;

	// установить параметры
	Mediator::pDevContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Mediator::pDevContext->IASetInputLayout(Mediator::pInputLayoutPos3Tex2);
	Mediator::pDevContext->PSSetShaderResources(TEXTURE_BUFFER, 1, &pSRtexture);
	BR(Mediator::shader->SetFontBuffer(&fontConstants.color));
	Mediator::pDevContext->VSSetShader(Mediator::pVSPosTex, NULL, 0);
	Mediator::pDevContext->PSSetShader(Mediator::pPSFont, NULL, 0);

	// рисование стандартных предложений
	for (UINT i(0); i < sentences.size(); i++)
		if (!sentences[i]->specialColorB) { BR(sentences[i]->Draw()); }
	// рисование предложений другого цвета
	for (UINT i(0); i < sentences.size(); i++)
		if (sentences[i]->specialColorB) { BR(sentences[i]->Draw()); }

	return true;

}
bool Font::Draw(ButtonsDesc* buttonsDesc) { // рисование шрифта на кнопках

	XMFLOAT4 dimcolor = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);

	// установить параметры
	Mediator::pDevContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Mediator::pDevContext->IASetInputLayout(Mediator::pInputLayoutPos3Tex2);
	Mediator::pDevContext->PSSetShaderResources(TEXTURE_BUFFER, 1, &pSRtexture);
	Mediator::pDevContext->VSSetShader(Mediator::pVSPosTex, NULL, 0);
	Mediator::pDevContext->PSSetShader(Mediator::pPSFont, NULL, 0);

	// рисование предложений
	for (UINT i(0); i < sentences.size(); i++) {		

		// определить цвет шрифта
		if (buttonsDesc[i].enable) { BR(Mediator::shader->SetFontBuffer(&fontConstants.color)); }
		else { BR(Mediator::shader->SetFontBuffer(&dimcolor)); }

		// рисовать
		BR(sentences[i]->Draw());
	}

	return true;

}

// сброс предложени€
bool Font::Reset() {

	for (auto iter = sentences.begin(); iter != sentences.end(); iter++) {
		BR((*iter)->Reset());
	}

	return true;

}

// удалить все предложени€
bool Font::DeleteAll() {

	for (UINT i(0); i < sentences.size(); i++) {
		ReleaseNULL(sentences[i]);
	}
	sentences.clear();

	return true;

}

// заменить предложение
bool Font::ReplaceSentence(UINT number, const wchar_t* newText) {

	bool result;
	XMFLOAT2 oldPosition = sentences[number]->getPosition();
	SPRITE_ALIGN oldFontAlign = sentences[number]->getAlignment();
	ReleaseNULL(sentences[number]);
	BRRC1(sentences[number] = new Sentence(newText, oldPosition, descriptor, fontConstants, oldFontAlign, false, false, XMFLOAT3(), &result));

	return true;

}

// удал€ет предложение
bool Font::Delete(UINT number) {

	// удалить первое предложение
	delete sentences[number];
	sentences.erase(sentences.begin() + number);

	return true;

}

// сдвинуть все предложени€ начина€ со start (удал€€ старт)
bool Font::MoveSentences(UINT start, XMFLOAT2 translation, bool delStart) {

	if (delStart) {

		//// удалить первое предложение
		BR(Delete(start));

	}

	// сдвинуть все предложени€
	for (UINT i(start); i < sentences.size(); i ++) {

		sentences[i]->Move(translation);

	}

	return true;

}

// изменить координаты одного предложени€
bool Font::SetPosition(UINT sentenceNumber, XMFLOAT3 newPosition) {

	BR(sentences[sentenceNumber]->SetPosition(newPosition));

	return true;

}
bool Font::SetPosition(UINT sentenceNumber, XMFLOAT2 newPosition) {

	BR(sentences[sentenceNumber]->SetPosition(newPosition));

	return true;

}

// деструктор шрифта
Font::~Font() {

	for (UINT i(0); i < sentences.size(); i ++) {
		ReleaseNULL(sentences[i]);
	}

	ReleaseNULLS(descriptor);
	ReleaseCOM(pSRtexture);

}

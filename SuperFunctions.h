#ifndef SUPER_FUNCTIONS
#define SUPER_FUNCTIONS

#include <anl.h>
#include "Nums.h"

class SuperFunct {

public:

	// суперфункция
	SuperFunct(bool* result); // конструктор

	// карта биомов
	float HeightFunction(float x, float y) { return (float)(scaleDomainBioms.get(x, y)); };
	float HeightFunction(NumXY<float> input) { return (float)(scaleDomainBioms.get(input.x, input.y)); };

	// карта деревьев
	float TreeFunction(float x, float y) { return (float)scaleDomainTree.get(x, y); };
	float TreeFunction(NumXY<float> input)  { return (float)scaleDomainTree.get(input.x, input.y); };

	// полная случайность от -1.0f до 1.0f
	float FullRandom0(float x, float y) { return (float)basisFullRandom0.get(x, y); };
	float FullRandom0(NumXY<float> input) { return (float)basisFullRandom0.get(input.x, input.y); };
	float FullRandom1(float x, float y) { return (float)basisFullRandom1.get(x, y); };
	float FullRandom1(NumXY<float> input) { return (float)basisFullRandom1.get(input.x, input.y); };
	float FullRandom2(float x, float y) { return (float)basisFullRandom2.get(x, y); };
	float FullRandom2(NumXY<float> input) { return (float)basisFullRandom2.get(input.x, input.y); };
	float FullRandom3(float x, float y) { return (float)basisFullRandom3.get(x, y); };
	float FullRandom3(NumXY<float> input) { return (float)basisFullRandom3.get(input.x, input.y); };
	float FullRandom4(float x, float y) { return (float)basisFullRandom4.get(x, y); };
	float FullRandom4(NumXY<float> input) { return (float)basisFullRandom4.get(input.x, input.y); };

private:

	// карта биомов
	anl::CImplicitBasisFunction basisBioms;
	anl::CImplicitScaleOffset scaleOffsetBioms = anl::CImplicitScaleOffset(5.0f, 0.0);
	anl::CImplicitScaleDomain scaleDomainBioms;

	// карта высот (не используется)
	anl::CImplicitBasisFunction basisHeight;
	anl::CImplicitScaleOffset scaleOffsetHeight = anl::CImplicitScaleOffset(5.0, 0.0);
	anl::CImplicitScaleDomain scaleDomainHeight;

	// карта деревьев
	anl::CImplicitBasisFunction basisTree;
	anl::CImplicitScaleDomain scaleDomainTree;

	// полная случайность
	anl::CImplicitBasisFunction basisFullRandom0;
	anl::CImplicitBasisFunction basisFullRandom1;
	anl::CImplicitBasisFunction basisFullRandom2;
	anl::CImplicitBasisFunction basisFullRandom3;
	anl::CImplicitBasisFunction basisFullRandom4;

};

#endif // SUPER_FUNCTIONS
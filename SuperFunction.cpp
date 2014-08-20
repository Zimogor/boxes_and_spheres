#include "SuperFunctions.h"
#include "Mediator.h"

// конструктор суперфункция
SuperFunct::SuperFunct(bool* result) {

	// полная случайность
	basisFullRandom0.setInterp(anl::NONE);
	basisFullRandom1.setInterp(anl::NONE); basisFullRandom1.setSeed(1);
	basisFullRandom2.setInterp(anl::NONE); basisFullRandom2.setSeed(2);
	basisFullRandom3.setInterp(anl::NONE); basisFullRandom3.setSeed(3);
	basisFullRandom4.setInterp(anl::NONE); basisFullRandom4.setSeed(4);

	// карта высот (не используется)
	basisHeight.setInterp(anl::QUINTIC);
	scaleOffsetHeight.setSource(&basisHeight);
	scaleDomainHeight.setSource(&scaleOffsetHeight);
	scaleDomainHeight.setXScale(0.05);
	scaleDomainHeight.setYScale(0.05);

	// карта биомов
	basisBioms.setInterp(anl::QUINTIC);
	scaleOffsetBioms.setSource(&basisBioms);
	scaleDomainBioms.setSource(&scaleOffsetBioms);
	scaleDomainBioms.setXScale(0.05);
	scaleDomainBioms.setYScale(0.05);

	// карта деревьев
	basisTree.setInterp(anl::QUINTIC);
	scaleDomainTree.setSource(&basisTree);
	scaleDomainTree.setXScale(0.1);
	scaleDomainTree.setYScale(0.1);

	Mediator::superFunct = this;
	*result = true;

}

// карта деревьев
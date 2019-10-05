#include "RandomNumberGeneration.h"

#ifdef MULTIPRECISION
RandomNumberGenerator::RandomNumberGenerator()
{
	gmp_randinit_mt(this->RNGState);
}

void RandomNumberGenerator::generateUniformFloat(calcFloat& result)
{
	mpfr_urandomb(result.mpfr_ptr(), this->RNGState);
}

void RandomNumberGenerator::generateUniformFloat(const calcFloat& min, const calcFloat& max, calcFloat& result)
{
	this->generateUniformFloat(result);
	result *= (max - min);
	result += min;
}

void RandomNumberGenerator::generateUniformInteger(const calcSignedInt& min, const calcSignedInt& max, calcSignedInt& result)
{
	calcSignedInt randRange = (max - min + 1);
	mpz_urandomm(result.backend().data(), this->RNGState, randRange.backend().data());
	result += min;
}
#else
RandomNumberGenerator::RandomNumberGenerator()
{
	long long tickCount = chrono::high_resolution_clock::now().time_since_epoch().count();
	unsigned seedValue = static_cast<unsigned>(tickCount % (static_cast<long long>(numeric_limits<unsigned>::max()) + 1));

	randEngine.seed(seedValue);
}

void RandomNumberGenerator::generateUniformFloat(calcFloat& result)
{
	this->generateUniformFloat(0.0, 1.0, result);
}

void RandomNumberGenerator::generateUniformFloat(const calcFloat& min, const calcFloat& max, calcFloat& result)
{
	std::uniform_real_distribution<calcFloat> distributionGenerator(min, max);
	result = distributionGenerator(randEngine);
}

void RandomNumberGenerator::generateUniformInteger(const calcSignedInt& min, const calcSignedInt& max, calcSignedInt& result)
{
	std::uniform_int_distribution<calcSignedInt> distributionGenerator(min, max);
	result = distributionGenerator(randEngine);
}
#endif
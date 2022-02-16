/**
 * @file rng.cpp
 * Random Number Generation
 *
 * @author Jacob Sanchez Perez (G20812080) <jsanchez-perez@uclan.ac.uk>
 * Games Concepts (CO1301), University of Central Lancashire
 */


#include "rng.h"

using namespace desert;

RNG::RNG(int min, int max) : mGenerator(mRandomDevice()), mDistribution(std::uniform_int_distribution<>(min, max))
{
}

int RNG::getNumber()
{
	return mDistribution(mGenerator);
}

float RNG::getDecimalPoint()
{
	return mDistribution(mGenerator) / kHundred;
}
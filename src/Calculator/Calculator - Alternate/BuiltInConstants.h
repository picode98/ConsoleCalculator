#pragma once

#include "CalculatorNumericTypes.h"
#include "GlobalBuildVariables.h"

#ifdef MULTIPRECISION
inline calcFloat get_pi_value()
{
	return mpfr::const_pi();
}

inline calcFloat get_e_value()
{
	return mpfr::exp(calcFloat(1.0));
}
#else
inline calcFloat get_pi_value()
{
	return M_PI;
}

inline calcFloat get_e_value()
{
	return M_E;
}
#endif
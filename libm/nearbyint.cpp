/*
 * Copyright 2016, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include <fenv.h>
#include <math.h>

#include "fpmath.h"

double
nearbyint(double x)
{
#ifdef __aarch64__
	return __builtin_nearbyint(x);
#else
	fenv_t fenv;
	double res;

	feholdexcept(&fenv);
	res = rint(x);
	fesetenv(&fenv);

	return (res);
#endif
}

float
nearbyintf(float x)
{
#ifdef __aarch64__
	return __builtin_nearbyintf(x);
#else
	fenv_t fenv;
	float res;

	feholdexcept(&fenv);
	res = rintf(x);
	fesetenv(&fenv);

	return (res);
#endif
}

long double
nearbyintl(long double x)
{
	fenv_t fenv;
	long double res;

	feholdexcept(&fenv);
	res = rintl(x);
	fesetenv(&fenv);

	return (res);
}

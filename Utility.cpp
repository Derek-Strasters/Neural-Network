/*
 * NeuronUtility.cpp
 *
 *  Created on: Apr 13, 2015
 *      Author: paracite
 */

#include "Utility.h"

/******************************************  Global Normalized Random Number *****************************************/

RandTools* RandTools::myPointerInstance = NULL; // Instantiate

RandTools* RandTools::Instance() { //All this work is so the random number only seeds once.
	if (!myPointerInstance) {
		std::srand(std::time(NULL));
		DEBUG("RandTools Singlet Created\n");
	}

	return (myPointerInstance ? myPointerInstance : (myPointerInstance = new RandTools));
}

void RandExitHandler() { // Callback to clean the heap on exit.
	if (RandTools::myPointerInstance)
		delete RandTools::myPointerInstance;

	DEBUG("RandTools Singlet Deleted\n");

}

double RandTools::GetGaussianDouble(double sigma, double mu) {
	generate = !generate;

	if (!generate)
		return (z1 * sigma + mu);

	double u1, u2;

	do {
		u1 = GetRandomDouble();
		u2 = GetRandomDouble();
	}while (u1 <= epsilon);

	z0 = sqrt(-2.0 * log(u1)) * cos(two_pi * u2);
	z1 = sqrt(-2.0 * log(u1)) * sin(two_pi * u2);
	return (z0 * sigma + mu);
} // Generates two gaussian distributed random numbers and stores the second one untill called again, then repeats.

double RandTools::GetRandomDouble() {
	if (index >= POOL_SIZE) {
		for (--index; index >= 0; index--) {
			pool[index] = rand() * (1.0 / RAND_MAX);
		} // TODO 5: implement custom random number generator class.

		index = 1;
		return (pool[0]);
	}
	return (pool[index++]);
} //Returns a random number from the pool, or it refills the pool.

double GetRaD() {
	return (RandTools::Instance()->GetRandomDouble());
} //Returns random double in [0 .. 1]

double GetRaD(double mult) {
	return (mult * GetRaD());
} //Returns random double in  [0 .. 1] scaled by 'mult'

double GetRaD(double mult, double add) {
	return (mult * (GetRaD() - 0.5 + add));
} //Returns random double in unit width centered on 'add' scaled by 'mult' E.G. GetRaD(2,5) gives a random in [3 .. 7]

bool GetRaB() {
	return ( (GetRaD() > 0.5) ? true : false);
} //Returns random bool

double GetRaG(double sigma, double mu) {
	return (RandTools::Instance()->GetGaussianDouble(sigma, mu));
} //Returns random number in Gaussian distribution about mu with standard deviation sigma.

double GetRaG(double sigma) {
	return (RandTools::Instance()->GetGaussianDouble(sigma, 0));
} //Returns random number in Gaussian distribution about 0 with standard deviation sigma.

double GetRaG() {
	return (RandTools::Instance()->GetGaussianDouble(1, 0));
} //Returns random number in the standard Gaussian distribution.

double GetRaGCS(double sigma, double scale) {
	double temp = GetRaG(sigma, 0.5) * scale;
	return ( (temp < 0 || temp > 1) ? ( (temp < 0) ? 0 : 1) : temp); //Statcked ternary operaters, whoa!
} //Returns random in gaussian disribution with mu = .5 clipped (from 0 to 1) and scaled.


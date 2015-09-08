/*
 * Sigmoid.h
 *
 *
 *  Created on: Apr 2, 2015
 *      Author: paracite
 */

#ifndef NEURONTYPES_H_
#define NEURONTYPES_H_

#include <math.h>
#include <cfloat>

#include "Neuron.h"

/******************************************************| Sigmoid |*****************************************************/

template<int In_Size>
class Sigmoid : public Neuron<Sigmoid, In_Size> {
//This is an implementation of the Neuron Interface
	protected:
		//Friends
		friend class Neuron< ::Sigmoid, In_Size> ; // Passing full access back to base class.

		//Overridden Methods
		static double ActivationFct(double z) {
			return (1.0 / (1.0 + exp(-z)));
		}
		static double ActivationFctPrime(double z) {
			return (exp(z) / ( (1.0 + exp(z)) * (1 + exp(z))) );
		}

	public:
		//Constructor
		Sigmoid() : Neuron<Sigmoid, In_Size>() {
		}

		//Destructor
		~Sigmoid() {
		}
};

/*****************************************************| Perceptron |***************************************************/

template<int In_Size>
class Perceptron : public Neuron<Perceptron, In_Size> {
//This is an implementation of the Neuron Interface
	protected:
		//Friends
		friend class Neuron< ::Perceptron, In_Size> ; // Passing full access back to base class.

		//Overridden Methods
		static double ActivationFct(double z) {
			return ( (z > 0) ? true : false);
		}
		static double ActivationFctPrime(double z) {
			return (exp(z) / ((1.0 + exp(z)) * (1 + exp(z))));
		}

	public:
		//Constructor
		Perceptron() : Neuron<Perceptron, In_Size>() {
		}

		//Destructor
		~Perceptron() {
		}
};

#endif /* NEURONTYPES_H_ */

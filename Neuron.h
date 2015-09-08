// Neuron.h
//
//  Created on: Apr 2, 2015
//      Author: Derek Strasters

#ifndef NEURON_H_ // <0>
#define NEURON_H_

#include <stdexcept>
#include <cstdlib>
#include <iostream>

#include "Utility.h"

/****************************************************|Neuron Class|***************************************************/

template<template<int> class Neuro_Type, int In_Size> // <1> (see footer)
class Neuron : public DownCast<Neuro_Type<In_Size> > {

	private:

		template<template<int> class Neuro_Type_i, int In_Size_i, int Out_Size_i> // Access to protected members,
		friend class Layer; // avoids the use of setters and getters but still maintaining class encapsulation.

		using DownCast<Neuro_Type<In_Size> >::Self; //All kind of magic happens. See utility file for more info.

	protected:

		/* Mrs. Fields															(...cookies are delicious) */
		CommAry<In_Size> ignored_ary_; // TODO 6: When a bias is below a threshold, stop computing the input there.
		CommAry<In_Size> weight_ary_; // Array of weights for computing output. Unique to each neuron.
		double bias_; // Similar to a weight, but applied to the final output.
		double bias_fuzz_; // SPECIAL give random noise to output for each run.

		/* Constructor */
		Neuron();

		/* Destructor */
		~Neuron();

	public:

		/* Field */
		static const int IN_SIZE = In_Size;

		/* Methods */
		static bool IsANeuron(); // For "duck" type checking.
		FuncSig GetActivationFct(); // Computes the activation onto member 'output_'.
		FuncSig GetActivationFctPrime(); // Returns the derivative of 'Think()' at the value 'output_'
};

/****************************************************|Is A Neuron|****************************************************/

template<template<int Size> class Neuro_Type, int In_Size>
bool Neuron<Neuro_Type, In_Size>::IsANeuron() {
	return (true);
} // Returns true if object is of type Neuron or inherits type Neuron.

/**************************************************|De/Constructors|**************************************************/

template<template<int Size> class Neuro_Type, int In_Size> //<2>
Neuron<Neuro_Type, In_Size>::Neuron() :
				bias_(0.5),
				bias_fuzz_(1) {

	if (Neuro_Type<In_Size>::IN_SIZE != In_Size)
		throw std::invalid_argument("Nuron template types are of different size. You shoudn't see this error.");
	if (!Neuro_Type<In_Size>::IsANeuron())
		throw std::invalid_argument("Type is not a derived from 'Neuron'.");
}

template<template<int Size> class Neuro_Type, int In_Size>
Neuron<Neuro_Type, In_Size>::~Neuron() {
}

/*******************************************************|Think|*******************************************************/

template<template<int Size> class Neuro_Type, int In_Size>
FuncSig Neuron<Neuro_Type, In_Size>::GetActivationFct() {
	return Self()->ActivationFct;
} // The weighted and biased input is passed to the polymorphic instance for it's unique activation function.

/****************************************************|Think Prime|****************************************************/

template<template<int Size> class Neuro_Type, int In_Size>
FuncSig Neuron<Neuro_Type, In_Size>::GetActivationFctPrime() {
	return Self()->ActivationFctPrime;
	//TODO 4: possibly the source or errors when double values get extremely small/large

} // More polymorphism.

#endif /* NEURON_H_ */
// <0> Hey look, it's a guarded definition so it doesn't get called twice and break the compile.
// I give myself a smug pat on the back (I'm joking here of course :P ).
// Further down things are going to get quite a bit more intense than some silly definition guards!

// <1> Templates are used to pass in the array size at compile time, there is an underling CTRP base class.
// It's also used for Polymorphism (Cool!) so that each child class can call a simple override, without v-tables!
// Its an abstract class for other neurons to inherit, it has pseudo-virtual methods (how fancy :P)
// It has a pointer to an array wrapper struct (that was a pain to implement) because the inputs of each
// layer of neurons is shared between neurons and this was the best option performance-wise. The templating
// uses some meta style templating to have a method that instantiate a generic type, intended for the derived
// classes. 'weight' is instantiated to size and is meant to be set by the gradient decent operation.
// 'Ignored' is also of size and is set true by gradient decent when it is determined a check is not needed for
// a particular index, again to help speed up the processing.
// All arrays in a layer have the same size 'input_size' in easy fashion, because they all have something to do with.
// the layer that inputs to the one "this" particular object is on.

// <2> One awesome benefit of our class being templated is that our array can be declared with the size and
// type it will ultimately be of.  There is no need for resizing upon initialization.  This is done with the help
// of our wrapper class "NuralAry" as well since array's can't be templated.

// <3> This setup has the same effect as using the virtual keyword to make abstract, however it has the added
// performance bonus of not having to make expensive v-table lookups.


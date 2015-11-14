/*
 * LayerLord.h
 *
 *  Created on: Apr 4, 2015
 *      Author: paracite
 */

#ifndef LAYER_H_
#define LAYER_H_

#include <limits>
#include "Neuron.h"
#include "POSI.h"

/*************************************************|Synapse  Structure|************************************************/

template<int Size>
struct Synapse {
		const CoAr<Size>& output_ary_;
		const CoAr<Size>& z_ary_;
		CoAr<Size>& err_ary_;
		CoAr<Size>& error_sum_;
		Synapse(
				const CoAr<Size>& out,
				const CoAr<Size>& z,
				CoAr<Size>& err,
				CoAr<Size>& error_sum);
};

template<int Size>
inline Synapse<Size>::Synapse(
		const CoAr<Size>& out,
		const CoAr<Size>& z,
		CoAr<Size>& err,
		CoAr<Size>& error_sum) :
				output_ary_(out),
				z_ary_(z),
				err_ary_(err),
				error_sum_(error_sum) {
}

/************************************************| Layer Lord Classes |***********************************************/

/*------------------------------------------------| Layer Lord Base |------------------------------------------------*/

//TODO 3: This would be fun, figure out how to put a callback on the last piece of data
// that needs to be processed so when it's finished it starts the next layer automatically.
// then, have it know when it's reached the end and start back propagating :D
template<template<int> class N_T, int INP, int OUT>
class Layer {
// <0>
	private:

		//Some static fields to aid in for loop unrolling
		static const int num_of_rolls_out = (int) (OUT / 8);
		static const int rolled_total_out = num_of_rolls_out * 8;
		static const int num_of_rolls_in = (int) (INP / 8);
		static const int rolled_total_in = num_of_rolls_in * 8;

		struct LayerWrap {
				N_T<INP> neurons_[OUT];
		}; // Wrapper for array of Neuron object pointers2.

		//CommAry Tools
		CommZero<OUT> ZeroOut;
		CommSub<OUT> SubOut;
		CommAdd<OUT> AddOut;

		CommAdd<INP> AddIn;
		CommMulSum<INP> MulSumIn;

		//CommAry objects
		CoAr<OUT> output_ary_; //<1> See foot notes
		CoAr<OUT> z_ary_; //Neurons hold a references here.
		CoAr<OUT> err_ary_; //Points to the 'in_err_ary_' in the next class.
		CoAr<OUT> err_sum_; //Mini-Batch error sum.

		//CommAry Tail specific, will refer to self otherwise.
		const CoAr<OUT>& training_ary_ref_; //For Tail layer use only.

		//CommAry references
		const CoAr<INP>& input_ary_; //Reference to previous layer.
		const CoAr<INP>& in_z_ary_; //Reference to Z values of previous layer.
		CoAr<INP>& in_err_ary_; //Reference to Error values of the previous layer.
		CoAr<INP>& in_err_sum_; //Mini-Batch error sum.

		POSI::Position position_; // Stores what part of the layer chain this instance is.
		LayerWrap layer_;

		//The functions used during activation of the neurons
		FuncSig Fct;
		FuncSig FctPrime;

		void Init(); // Initializer to reduce constructor size.

	public:

		//---Constructors---
		Layer(const CoAr<INP>& comm_in); //Makes Head
		Layer(const Synapse<INP>& comm_prev); //Makes body
		Layer(const Synapse<INP>& comm_prev, const CoAr<OUT>& comm_trainer); //Makes Tail
		//TODO 1: Copies layer with different number of Neurons, removes duplicates.

		//Destructors
		~Layer();

		//---Methods---
		//Initializations
		void SetWeight(int layer_depth, int input_depth, double weight);
		void SetBias(int layer_depth, double bias);
		void SetAllWeight(double (*f)());
		void SetAllBiases(double (*f)()); // Overload
		void SetAllWeight(double (*f)(double first), double first);
		void SetAllBiases(double (*f)(double first), double first); // More overload
		void SetAllWeight(double (*f)(double first, double second), double first, double second);
		void SetAllBiases(double (*f)(double first, double second), double first, double second);

		//Getters and Setters
		const CoAr<OUT>& GetOutAry();
		const CoAr<OUT>& GetErrAry();
		//For saving and loading
		CoAr<OUT> GetBiases(); //TODO 1: Returns the bias values of this layer's neurons
		//TODO 1: Returns the weights of this layer's neurons
		void SetBiases(const CoAr<OUT> biases); //TODO 1: Sets the bias values for each neuron
		//TODO 1: Sets the weight values for each neuron

		//Creates a link between layers
		Synapse<OUT> Synaptogenisis();

		//Functions
		void Z_Cascade(); //Generate all z values in this 'layer_'
		void Activation(); //Generate all activations in 'layer_'
		void OutputErrorC(); //Generate 'layer_' error values in 'err_ary_' for output layer
		void ErrorCascade(); //Generate 'layer_' error values in 'err_ary_' for output layer
		void DescendGradient(double learn_rate, double batch_size);

		void ReInitialize(); //Resets the bias and weight values

		//Tail Specific
		//Returns the average error
		double AverageError();
};

/**************************************************|De/Constructor|***************************************************/

template<template<int> class N_T, int INP, int OUT>
Layer<N_T, INP, OUT>::Layer(const CoAr<INP>& comm_in) :
				training_ary_ref_(output_ary_),
				input_ary_(comm_in), //TODO: Find a damn sure way to block access to these!
				in_z_ary_(comm_in), //A (poopy) way to deal with uninitialized ref's b/c Head has no use.
				in_err_ary_(const_cast<CoAr<INP>&>(comm_in)), //Ugh, const_cast....
				in_err_sum_(const_cast<CoAr<INP>&>(comm_in)),
				position_(POSI::Head) { //TODO: Make the head
	Init();
}

template<template<int> class N_T, int INP, int OUT>
Layer<N_T, INP, OUT>::Layer(const Synapse<INP>& comm_prev) :
				training_ary_ref_(output_ary_),
				input_ary_(comm_prev.output_ary_),
				in_z_ary_(comm_prev.z_ary_),
				in_err_ary_(comm_prev.err_ary_),
				in_err_sum_(comm_prev.error_sum_),
				position_(POSI::Body) {
	Init();
}

template<template<int> class N_T, int INP, int OUT>
Layer<N_T, INP, OUT>::Layer(const Synapse<INP>& comm_prev, const CoAr<OUT>& comm_trainer) :
				training_ary_ref_(comm_trainer), //The only time this reference is assigned non trivially.
				input_ary_(comm_prev.output_ary_),
				in_z_ary_(comm_prev.z_ary_),
				in_err_ary_(comm_prev.err_ary_),
				in_err_sum_(comm_prev.error_sum_),
				position_(POSI::Tail) {
		Init();
}

template<template<int> class N_T, int INP, int OUT>
inline void Layer<N_T, INP, OUT>::Init() {
	FctPrime = (layer_.neurons_[0].GetActivationFctPrime());
	Fct = (layer_.neurons_[0].GetActivationFct());
	if (OUT < 1)
		throw std::invalid_argument("Layer must containt at least 1 neruon.");
}

template<template<int> class N_T, int INP, int OUT>
inline Synapse<OUT> Layer<N_T, INP, OUT>::Synaptogenisis() {
	return Synapse<OUT>(output_ary_, z_ary_, err_ary_, err_sum_);
}

template<template<int> class N_T, int INP, int OUT>
Layer<N_T, INP, OUT>::~Layer() {
}

/*****************************************************|SetWeight|*****************************************************/

template<template<int> class N_T, int INP, int OUT>
void Layer<N_T, INP, OUT>::SetWeight(int layer_depth, int input_depth, double weight) {
	layer_.neurons_[layer_depth].wt_ar_[input_depth] = weight;
}

/******************************************************|SetBias|******************************************************/

template<template<int> class N_T, int INP, int OUT>
void Layer<N_T, INP, OUT>::SetBias(int layer_depth, double bias) {
	layer_.neurons_[layer_depth].bias_ = bias;
}

/******************************************************|Set All|******************************************************/

template<template<int> class N_T, int INP, int OUT>
void Layer<N_T, INP, OUT>::SetAllWeight(double (*f)()) {
	for (int i = 0; i < OUT; i++) {
		for (int j = 0; j < INP; j++) {
			layer_.neurons_[i].wt_ar_.ary_[j] = f();
		}
	}
}

template<template<int> class N_T, int INP, int OUT>
void Layer<N_T, INP, OUT>::SetAllBiases(double (*f)()) {
	for (int i = 0; i < OUT; i++) {
		layer_.neurons_[i].bias_ = f();
	}
}

template<template<int> class N_T, int INP, int OUT>
void Layer<N_T, INP, OUT>::SetAllWeight(double (*f)(double first), double first) {
	for (int i = 0; i < OUT; i++) {
		for (int j = 0; j < INP; j++) {
			layer_.neurons_[i].wt_ar_.ary_[j] = f(first);
		}
	}
}

template<template<int> class N_T, int INP, int OUT>
void Layer<N_T, INP, OUT>::SetAllBiases(double (*f)(double first), double first) {
	for (int i = 0; i < OUT; i++) {
		layer_.neurons_[i].bias_ = f(first);
	}
}

template<template<int> class N_T, int INP, int OUT>
void Layer<N_T, INP, OUT>::SetAllWeight(
		double (*f)(double first, double second),
		double first,
		double second) {

	for (int i = 0; i < OUT; i++) {
		for (int j = 0; j < INP; j++) {
			layer_.neurons_[i].wt_ar_.ary_[j] = f(first, second);
		}
	}
}

template<template<int> class N_T, int INP, int OUT>
void Layer<N_T, INP, OUT>::SetAllBiases(
		double (*f)(double mult, double add),
		double mult,
		double add) {

	for (int i = 0; i < OUT; i++) {
		layer_.neurons_[i].bias_ = f(mult, add);
	}
} // Any more of this crap and we'll switch to variadic arguments.

/****************************************************|Get Output|*****************************************************/

template<template<int> class N_T, int INP, int OUT>
const CoAr<OUT>& Layer<N_T, INP, OUT>::GetOutAry() {
	return (output_ary_);
}

/*****************************************************|Get Error|*****************************************************/

template<template<int> class N_T, int INP, int OUT>
const CoAr<OUT>& Layer<N_T, INP, OUT>::GetErrAry() {
	return (err_ary_);
}

/*****************************************************|Z Cascade|*****************************************************/

template<template<int> class N_T, int INP, int OUT> //TODO 2: Unroll this loop
void Layer<N_T, INP, OUT>::Z_Cascade() {
	for (int i = 0; i < OUT; i++) {
		MulSumIn(z_ary_.ary_[i], input_ary_, layer_.neurons_[i].wt_ar_);
		z_ary_.ary_[i] += layer_.neurons_[i].bias_;
	}
}

/****************************************************| Activation |***************************************************/

template<template<int> class N_T, int INP, int OUT>
void Layer<N_T, INP, OUT>::Activation() {

	//TODO: make these members for reuse, initialize in constructor.

	for (int i = 0; i < rolled_total_out; i += 8) {
		output_ary_.ary_[i + 0] = Fct(z_ary_.ary_[i + 0]);
		output_ary_.ary_[i + 1] = Fct(z_ary_.ary_[i + 1]);
		output_ary_.ary_[i + 2] = Fct(z_ary_.ary_[i + 2]);
		output_ary_.ary_[i + 3] = Fct(z_ary_.ary_[i + 3]);
		output_ary_.ary_[i + 4] = Fct(z_ary_.ary_[i + 4]);
		output_ary_.ary_[i + 5] = Fct(z_ary_.ary_[i + 5]);
		output_ary_.ary_[i + 6] = Fct(z_ary_.ary_[i + 6]);
		output_ary_.ary_[i + 7] = Fct(z_ary_.ary_[i + 7]);
	}

	for (int i = rolled_total_out; i < OUT; i++) {
		output_ary_.ary_[i] = Fct(z_ary_.ary_[i]);
	}
} // USEAGE WARNING: Depends on 'z_' values being created first.

/********************************************|Output Layer  Error Cascade|********************************************/

template<template<int> class N_T, int INP, int OUT>
void Layer<N_T, INP, OUT>::OutputErrorC() {
	if (position_ != POSI::Tail)
		throw std::invalid_argument("Only Tail layer may call 'OutError(...)");

	/* Error array is reinitialized to the difference of output */
	SubOut(err_ary_, training_ary_ref_, output_ary_);

	for (int i = 0; i < rolled_total_out; i += 8) {
		err_ary_.ary_[i + 0] *= FctPrime(z_ary_.ary_[i + 0]);
		err_ary_.ary_[i + 1] *= FctPrime(z_ary_.ary_[i + 1]);
		err_ary_.ary_[i + 2] *= FctPrime(z_ary_.ary_[i + 2]);
		err_ary_.ary_[i + 3] *= FctPrime(z_ary_.ary_[i + 3]);
		err_ary_.ary_[i + 4] *= FctPrime(z_ary_.ary_[i + 4]);
		err_ary_.ary_[i + 5] *= FctPrime(z_ary_.ary_[i + 5]);
		err_ary_.ary_[i + 6] *= FctPrime(z_ary_.ary_[i + 6]);
		err_ary_.ary_[i + 7] *= FctPrime(z_ary_.ary_[i + 7]);
	}

	for (int i = rolled_total_out; i < OUT; i++) {
		err_ary_.ary_[i] *= FctPrime(z_ary_.ary_[i]);
	}

	/* Add result to the Error Sum array of the layer */
	AddOut(err_sum_, err_ary_);
} // USEAGE WARNING: Depends on activations ('output_lyr_') being created first.
//TODO: Make this the only public method, and then usage will be safe.

/***************************************************|Error Cascade|***************************************************/

template<template<int> class N_T, int INP, int OUT>
void Layer<N_T, INP, OUT>::ErrorCascade() {
	if (position_ == POSI::Head)
		throw std::invalid_argument("Head layer not allowed to call 'ErrorCascade()'.");

	for (int i = 0; i < INP; i++) {
		double e_00 = 0, e_01 = 0, e_02 = 0, e_03 = 0, e_04 = 0, e_05 = 0, e_06 = 0, e_07 = 0;

		for (int j = 0; j < rolled_total_out; j += 8) {
			e_00 += layer_.neurons_[j + 0].wt_ar_.ary_[i] * err_ary_.ary_[j + 0];
			e_01 += layer_.neurons_[j + 1].wt_ar_.ary_[i] * err_ary_.ary_[j + 1];
			e_02 += layer_.neurons_[j + 2].wt_ar_.ary_[i] * err_ary_.ary_[j + 2];
			e_03 += layer_.neurons_[j + 3].wt_ar_.ary_[i] * err_ary_.ary_[j + 3];
			e_04 += layer_.neurons_[j + 4].wt_ar_.ary_[i] * err_ary_.ary_[j + 4];
			e_05 += layer_.neurons_[j + 5].wt_ar_.ary_[i] * err_ary_.ary_[j + 5];
			e_06 += layer_.neurons_[j + 6].wt_ar_.ary_[i] * err_ary_.ary_[j + 6];
			e_07 += layer_.neurons_[j + 7].wt_ar_.ary_[i] * err_ary_.ary_[j + 7];
		}

		/* Error Array of previous layer is reinitialized to 0 or the sum of the loop unrolling */
		in_err_ary_.ary_[i] = (e_00 + e_01 + e_02 + e_03 + e_04 + e_05 + e_06 + e_07);

		/* Error Array of previous layer continues remaining computation */
		for (int j = rolled_total_out; j < OUT; j++) {
			in_err_ary_.ary_[i] += layer_.neurons_[j].wt_ar_.ary_[i] * err_ary_.ary_[j];
		}

		in_err_ary_.ary_[i] *= FctPrime(in_z_ary_.ary_[i]);

	} // Output Error (evaluates to) the Gradient (Hadamard Product) Derivative of Activation Func @ z_.

	/* Add result to Error Sum array of the layer */
	AddIn(in_err_sum_, in_err_ary_);
}

/*************************************************|Descend Gradient|**************************************************/

template<template<int> class N_T, int INP, int OUT>
void Layer<N_T, INP, OUT>::DescendGradient(double learn_rate, double batch_size) {
	double coef = learn_rate / batch_size;

	for (int i = 0; i < OUT; i++) {

		/* Calculate biases and weights using the average error of the mini batch (Error Sum/Batch Size) */
		layer_.neurons_[i].bias_ -= coef * err_sum_.ary_[i];

		for (int j = 0; j < rolled_total_in; j += 8) {
			layer_.neurons_[i].wt_ar_.ary_[j + 0] -= coef * err_sum_.ary_[i] * input_ary_.ary_[j + 0];
			layer_.neurons_[i].wt_ar_.ary_[j + 1] -= coef * err_sum_.ary_[i] * input_ary_.ary_[j + 1];
			layer_.neurons_[i].wt_ar_.ary_[j + 2] -= coef * err_sum_.ary_[i] * input_ary_.ary_[j + 2];
			layer_.neurons_[i].wt_ar_.ary_[j + 3] -= coef * err_sum_.ary_[i] * input_ary_.ary_[j + 3];
			layer_.neurons_[i].wt_ar_.ary_[j + 4] -= coef * err_sum_.ary_[i] * input_ary_.ary_[j + 4];
			layer_.neurons_[i].wt_ar_.ary_[j + 5] -= coef * err_sum_.ary_[i] * input_ary_.ary_[j + 5];
			layer_.neurons_[i].wt_ar_.ary_[j + 6] -= coef * err_sum_.ary_[i] * input_ary_.ary_[j + 6];
			layer_.neurons_[i].wt_ar_.ary_[j + 7] -= coef * err_sum_.ary_[i] * input_ary_.ary_[j + 7];
		}

		for (int j = rolled_total_in; j < INP; j++) {
			layer_.neurons_[i].wt_ar_.ary_[j] -= coef * err_sum_.ary_[i] * input_ary_.ary_[j];
		}
	}

	/* Clear Error Sum Array */
	ZeroOut(err_sum_);
}

#endif /* LAYER_H_ */

// <0> LayerLord holds an array of objects that inherit 'Neuron' class for a 'layer' of neruons.
// Since in c++98 the type can't be restricted to inheriting objects of Neuron class, there is a
// duck type check using the 'IsANeuron' member func of the Neuron, and throws an appropriate error.

// <1> The lords hold the acutal data arrays. Everything else is a reference.


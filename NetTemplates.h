/*
 * LayerBoss.h
 *
 *  Created on: May 11, 2015
 *      Author: paracite
 */

#ifndef NETTEMPLATES_H_
#define NETTEMPLATES_H_

#include "DLog.h"
#include "Neuron.h"
#include "NeuronTypes.h"
#include "POSI.h"
#include "Color.h"
#include "Layer.h"
#include "Utility.h"

// Avoiding the use of c++11, I simply have to put up with a large number of overloads since there is no access to
// variadic templates pre c++11. What a waste of effort! Vectors are slow, I refuse!

// TODO: Inheritance can be used here to make this better I'm sure of it...

/****************************************************| 02 Layers |****************************************************/

template<
		template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int OUT>
class LayerBoss02 {

	protected:

		//Layers
		Layer<N_T, INP, OUT> LLTL_;

	public:

		LayerBoss02(const CommAry<INP>& retina, const CommAry<OUT>& trainer, double nu);

		void FeedForward();
		void FeedForward(CommAry<OUT>& output);
		void FeedBackward();
		void Update(double nu, int batch_size);
};

/*--------------------------------------------------| Constructor |---------------------------------------------------*/

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int OUT>
inline LayerBoss02<
		N_T,
		INP,
		OUT
>::LayerBoss02(const CommAry<INP>& retina, const CommAry<OUT>& trainer, double nu) :
				LLTL_(Layer<N_T, INP, OUT>(LLTL_.Synaptogenisis(), trainer)) {

	LLTL_.SetAllWeight(GetRaGCS, 0.02, (0.5 / INP));

	LLTL_.SetAllBiases(GetRaG, 0.01, 0);
}

/*----------------------------------------------------| Methods |-----------------------------------------------------*/

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int OUT>
inline void LayerBoss02<
		N_T,
		INP,
		OUT>::FeedForward() {

	LLTL_.Z_Cascade();
	LLTL_.Activation();
}

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int OUT>
inline void LayerBoss02<
		N_T,
		INP,
		OUT>::FeedForward(CommAry<OUT>& output) {
	FeedForward();
	output = LLTL_.GetOutAry();
}

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int OUT>
inline void LayerBoss02<
		N_T,
		INP,
		OUT>::FeedBackward() {

	LLTL_.OutputErrorC();
}

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int OUT>
inline void LayerBoss02<
		N_T,
		INP,
		OUT>::Update(double nu, int batch_size) {
	LLTL_.DescendGradient(nu, batch_size);
}

/****************************************************| 03 Layers |****************************************************/

template<
		template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00,
		int OUT>
class LayerBoss03 {

	protected:

		//Layers
		Layer<N_T, INP, L00> LLHD_;
		Layer<N_T, L00, OUT> LLTL_;

	public:

		LayerBoss03(const CommAry<INP>& retina, const CommAry<OUT>& trainer, double nu);

		void FeedForward();
		void FeedForward(CommAry<OUT>& output);
		void FeedBackward();
		void Update(double nu, int batch_size);
};

/*--------------------------------------------------| Constructor |---------------------------------------------------*/

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00,
		int OUT>
inline LayerBoss03<
		N_T,
		INP,
		L00,
		OUT
>::LayerBoss03(const CommAry<INP>& retina, const CommAry<OUT>& trainer, double nu) :
				LLHD_(Layer<N_T, INP, L00>(retina)),
				LLTL_(Layer<N_T, L00, OUT>(LLHD_.Synaptogenisis(), trainer)) {

	LLHD_.SetAllWeight(GetRaGCS, 0.02, (0.5 / INP));
	LLTL_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L00));

	LLHD_.SetAllBiases(GetRaG, 0.01, 0);
	LLTL_.SetAllBiases(GetRaG, 0.01, 0);
}

/*----------------------------------------------------| Methods |-----------------------------------------------------*/

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int OUT>
inline void LayerBoss03<
		N_T,
		INP,
		L00,
		OUT>::FeedForward() {

	LLHD_.Z_Cascade();
	LLHD_.Activation();
	LLTL_.Z_Cascade();
	LLTL_.Activation();
}

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int OUT>
inline void LayerBoss03<
		N_T,
		INP,
		L00,
		OUT>::FeedForward(CommAry<OUT>& output) {
	FeedForward();
	output = LLTL_.GetOutAry();
}

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int OUT>
inline void LayerBoss03<
		N_T,
		INP,
		L00,
		OUT>::FeedBackward() {

	LLTL_.OutputErrorC();
	LLTL_.ErrorCascade();
}

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int OUT>
inline void LayerBoss03<
		N_T,
		INP,
		L00,
		OUT>::Update(double nu, int batch_size) {
	LLHD_.DescendGradient(nu, batch_size);
	LLTL_.DescendGradient(nu, batch_size);
}

/****************************************************| 04 Layers |****************************************************/

template<
		template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int OUT>
class LayerBoss04 {

		//protected: TODO: TEST
	public:

		//Layers
		Layer<N_T, INP, L00> LLHD_;
		Layer<N_T, L00, L01> LL01_;
		Layer<N_T, L01, OUT> LLTL_;

		//public: TODO: TEST

		LayerBoss04(const CommAry<INP>& retina, const CommAry<OUT>& trainer, double nu);

		void FeedForward();
		void FeedForward(CommAry<OUT>& output);
		void FeedBackward();
		void Update(double nu, int batch_size);
};

/*--------------------------------------------------| Constructor |---------------------------------------------------*/

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00,
		int L01,
		int OUT>
inline LayerBoss04<
		N_T,
		INP,
		L00,
		L01,
		OUT
>::LayerBoss04(const CommAry<INP>& retina, const CommAry<OUT>& trainer, double nu) :
				LLHD_(Layer<N_T, INP, L00>(retina)),
				LL01_(Layer<N_T, L00, L01>(LLHD_.Synaptogenisis())),
				LLTL_(Layer<N_T, L01, OUT>(LL01_.Synaptogenisis(), trainer)) {

	LLHD_.SetAllWeight(GetRaGCS, 0.02, (0.5 / INP));
	LL01_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L00));
	LLTL_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L01));

	LLHD_.SetAllBiases(GetRaG, 0.01, 0);
	LL01_.SetAllBiases(GetRaG, 0.01, 0);
	LLTL_.SetAllBiases(GetRaG, 0.01, 0);
}

/*----------------------------------------------------| Methods |-----------------------------------------------------*/

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int OUT>
inline void LayerBoss04<
		N_T,
		INP,
		L00,
		L01,
		OUT>::FeedForward() {

	LLHD_.Z_Cascade();
	LLHD_.Activation();
	LL01_.Z_Cascade();
	LL01_.Activation();
	LLTL_.Z_Cascade();
	LLTL_.Activation();
}

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int OUT>
inline void LayerBoss04<
		N_T,
		INP,
		L00,
		L01,
		OUT>::FeedForward(CommAry<OUT>& output) {
	FeedForward();
	output = LLTL_.GetOutAry();
}

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int OUT>
inline void LayerBoss04<
		N_T,
		INP,
		L00,
		L01,
		OUT>::FeedBackward() {

	LLTL_.OutputErrorC();
	LLTL_.ErrorCascade();
	LL01_.ErrorCascade();
}

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int OUT>
inline void LayerBoss04<
		N_T,
		INP,
		L00,
		L01,
		OUT>::Update(double nu, int batch_size) {
	LLHD_.DescendGradient(nu, batch_size);
	LL01_.DescendGradient(nu, batch_size);
	LLTL_.DescendGradient(nu, batch_size);
}

/****************************************************| 05 Layers |****************************************************/

template<
		template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int L02, //
		int OUT>
class LayerBoss05 {

		//protected: TODO: TEST
	public:

		//Layers
		Layer<N_T, INP, L00> LLHD_;
		Layer<N_T, L00, L01> LL01_;
		Layer<N_T, L01, L02> LL02_;
		Layer<N_T, L02, OUT> LLTL_;

		//public: TODO: TEST

		LayerBoss05(const CommAry<INP>& retina, const CommAry<OUT>& trainer, double nu);

		void FeedForward();
		const CommAry<OUT>& FeedForward(bool do_return); //TODO 1: add these to all cases!S
		void FeedBackward();
		void Update(double nu, int batch_size);
};

/*--------------------------------------------------| Constructor |---------------------------------------------------*/

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00,
		int L01,
		int L02,
		int OUT>
inline LayerBoss05<
		N_T,
		INP,
		L00,
		L01,
		L02,
		OUT
>::LayerBoss05(const CommAry<INP>& retina, const CommAry<OUT>& trainer, double nu) :
				LLHD_(Layer<N_T, INP, L00>(retina)),
				LL01_(Layer<N_T, L00, L01>(LLHD_.Synaptogenisis())),
				LL02_(Layer<N_T, L01, L02>(LL01_.Synaptogenisis())),
				LLTL_(Layer<N_T, L02, OUT>(LL02_.Synaptogenisis(), trainer)) {

	LLHD_.SetAllWeight(GetRaGCS, 0.02, (0.5 / INP));
	LL01_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L00));
	LL02_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L01));
	LLTL_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L02));

	LLHD_.SetAllBiases(GetRaG, 0.01, 0);
	LL01_.SetAllBiases(GetRaG, 0.01, 0);
	LL02_.SetAllBiases(GetRaG, 0.01, 0);
	LLTL_.SetAllBiases(GetRaG, 0.01, 0);
}

/*----------------------------------------------------| Methods |-----------------------------------------------------*/

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int L02, //
		int OUT>
inline void LayerBoss05<
		N_T,
		INP,
		L00,
		L01,
		L02,
		OUT>::FeedForward() {

	LLHD_.Z_Cascade();
	LLHD_.Activation();
	LL01_.Z_Cascade();
	LL01_.Activation();
	LL02_.Z_Cascade();
	LL02_.Activation();
	LLTL_.Z_Cascade();
	LLTL_.Activation();
}

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int L02, //
		int OUT>
inline const CommAry<OUT>& LayerBoss05<
		N_T,
		INP,
		L00,
		L01,
		L02,
		OUT>::FeedForward(bool do_return) {
// It's going to return one whether you give it true or not :/
	FeedForward();
	return LLTL_.GetOutAry();
}

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int L02, //
		int OUT>
inline void LayerBoss05<
		N_T,
		INP,
		L00,
		L01,
		L02,
		OUT>::FeedBackward() {

	LLTL_.OutputErrorC();
	LLTL_.ErrorCascade();
	LL02_.ErrorCascade();
	LL01_.ErrorCascade();
}

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int L02, //
		int OUT>
inline void LayerBoss05<
		N_T,
		INP,
		L00,
		L01,
		L02,
		OUT>::Update(double nu, int batch_size) {
	LLHD_.DescendGradient(nu, batch_size);
	LL01_.DescendGradient(nu, batch_size);
	LL02_.DescendGradient(nu, batch_size);
	LLTL_.DescendGradient(nu, batch_size);
}

/****************************************************| 06 Layers |****************************************************/

template<
		template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int L02, //
		int L03, //
		int OUT>
class LayerBoss06 {

		//protected: TODO: TEST
	public:

		//Layers
		Layer<N_T, INP, L00> LLHD_;
		Layer<N_T, L00, L01> LL01_;
		Layer<N_T, L01, L02> LL02_;
		Layer<N_T, L02, L03> LL03_;
		Layer<N_T, L03, OUT> LLTL_;

		//public: TODO: TEST

		LayerBoss06(const CommAry<INP>& retina, const CommAry<OUT>& trainer, double nu);

		void FeedForward();
		void FeedForward(CommAry<OUT>& output);
		void FeedBackward();
		void Update(double nu, int batch_size);
};

/*--------------------------------------------------| Constructor |---------------------------------------------------*/

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00,
		int L01,
		int L02,
		int L03,
		int OUT>
inline LayerBoss06<
		N_T,
		INP,
		L00,
		L01,
		L02,
		L03,
		OUT
>::LayerBoss06(const CommAry<INP>& retina, const CommAry<OUT>& trainer, double nu) :
				LLHD_(Layer<N_T, INP, L00>(retina)),
				LL01_(Layer<N_T, L00, L01>(LLHD_.Synaptogenisis())),
				LL02_(Layer<N_T, L01, L02>(LL01_.Synaptogenisis())),
				LL03_(Layer<N_T, L02, L03>(LL02_.Synaptogenisis())),
				LLTL_(Layer<N_T, L02, OUT>(LL03_.Synaptogenisis(), trainer)) {

	LLHD_.SetAllWeight(GetRaGCS, 0.02, (0.5 / INP));
	LL01_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L00));
	LL02_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L01));
	LL03_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L02));
	LLTL_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L03));

	LLHD_.SetAllBiases(GetRaG, 0.01, 0);
	LL01_.SetAllBiases(GetRaG, 0.01, 0);
	LL02_.SetAllBiases(GetRaG, 0.01, 0);
	LL03_.SetAllBiases(GetRaG, 0.01, 0);
	LLTL_.SetAllBiases(GetRaG, 0.01, 0);
}

/*----------------------------------------------------| Methods |-----------------------------------------------------*/

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int L02, //
		int L03, //
		int OUT>
inline void LayerBoss06<
		N_T,
		INP,
		L00,
		L01,
		L02,
		L03,
		OUT>::FeedForward() {

	LLHD_.Z_Cascade();
	LLHD_.Activation();
	LL01_.Z_Cascade();
	LL01_.Activation();
	LL02_.Z_Cascade();
	LL02_.Activation();
	LL03_.Z_Cascade();
	LL03_.Activation();
	LLTL_.Z_Cascade();
	LLTL_.Activation();
}

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int L02, //
		int L03, //
		int OUT>
inline void LayerBoss06<
		N_T,
		INP,
		L00,
		L01,
		L02,
		L03,
		OUT>::FeedForward(CommAry<OUT>& output) {

	FeedForward();
	output = LLTL_.GetOutAry();
}

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int L02, //
		int L03, //
		int OUT>
inline void LayerBoss06<
		N_T,
		INP,
		L00,
		L01,
		L02,
		L03,
		OUT>::FeedBackward() {

	LLTL_.OutputErrorC();
	LLTL_.ErrorCascade();
	LL03_.ErrorCascade();
	LL02_.ErrorCascade();
	LL01_.ErrorCascade();
}

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int L02, //
		int L03, //
		int OUT>
inline void LayerBoss06<
		N_T,
		INP,
		L00,
		L01,
		L02,
		L03,
		OUT>::Update(double nu, int batch_size) {
	LLHD_.DescendGradient(nu, batch_size);
	LL01_.DescendGradient(nu, batch_size);
	LL02_.DescendGradient(nu, batch_size);
	LL03_.DescendGradient(nu, batch_size);
	LLTL_.DescendGradient(nu, batch_size);
}

/****************************************************| 07  Layers |****************************************************/

template<
		template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int L02, //
		int L03, //
		int L04, //
		int OUT>
class LayerBoss07 {

	protected:

		//Layers
		Layer<N_T, INP, L00> LLHD_;
		Layer<N_T, L00, L01> LL01_;
		Layer<N_T, L01, L02> LL02_;
		Layer<N_T, L02, L03> LL03_;
		Layer<N_T, L03, L04> LL04_;
		Layer<N_T, L04, OUT> LLTL_;

	public:

		LayerBoss07(const CommAry<INP>& retina, const CommAry<OUT>& trainer, double nu);

		void FeedForward();
		void FeedBackward();
		void Update(double nu, int batch_size);
};

/*--------------------------------------------------| Constructor |---------------------------------------------------*/

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00,
		int L01,
		int L02,
		int L03,
		int L04,
		int OUT>
inline LayerBoss07<
		N_T,
		INP,
		L00,
		L01,
		L02,
		L03,
		L04,
		OUT
>::LayerBoss07(const CommAry<INP>& retina, const CommAry<OUT>& trainer, double nu) :
				LLHD_(Layer<N_T, INP, L00>(retina)),
				LL01_(Layer<N_T, L00, L01>(LLHD_.Synaptogenisis())),
				LL02_(Layer<N_T, L01, L02>(LL01_.Synaptogenisis())),
				LL03_(Layer<N_T, L02, L03>(LL02_.Synaptogenisis())),
				LL04_(Layer<N_T, L03, L04>(LL03_.Synaptogenisis())),
				LLTL_(Layer<N_T, L04, OUT>(LLTL_.Synaptogenisis(), trainer)) {

	LLHD_.SetAllWeight(GetRaGCS, 0.02, (0.5 / INP));
	LL01_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L00));
	LL02_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L01));
	LL03_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L02));
	LL04_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L03));
	LLTL_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L04));

	LLHD_.SetAllBiases(GetRaG, 0.01, 0);
	LL01_.SetAllBiases(GetRaG, 0.01, 0);
	LL02_.SetAllBiases(GetRaG, 0.01, 0);
	LL03_.SetAllBiases(GetRaG, 0.01, 0);
	LL04_.SetAllBiases(GetRaG, 0.01, 0);
	LLTL_.SetAllBiases(GetRaG, 0.01, 0);
}

/*----------------------------------------------------| Methods |-----------------------------------------------------*/

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int L02, //
		int L03, //
		int L04, //
		int OUT>
inline void LayerBoss07<
		N_T,
		INP,
		L00,
		L01,
		L02,
		L03,
		L04,
		OUT>::FeedForward() {

	LLHD_.Z_Cascade();
	LLHD_.Activation();
	LL01_.Z_Cascade();
	LL01_.Activation();
	LL02_.Z_Cascade();
	LL02_.Activation();
	LL03_.Z_Cascade();
	LL03_.Activation();
	LL04_.Z_Cascade();
	LL04_.Activation();
	LLTL_.Z_Cascade();
	LLTL_.Activation();
}

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int L02, //
		int L03, //
		int L04, //
		int OUT>
inline void LayerBoss07<
		N_T,
		INP,
		L00,
		L01,
		L02,
		L03,
		L04,
		OUT>::FeedBackward() {

	LLTL_.OutputErrorC();
	LLTL_.ErrorCascade();
	LL04_.ErrorCascade();
	LL03_.ErrorCascade();
	LL02_.ErrorCascade();
	LL01_.ErrorCascade();
}

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int L02, //
		int L03, //
		int L04, //
		int OUT>
inline void LayerBoss07<
		N_T,
		INP,
		L00,
		L01,
		L02,
		L03,
		L04,
		OUT>::Update(double nu, int batch_size) {
	LLHD_.DescendGradient(nu, batch_size);
	LL01_.DescendGradient(nu, batch_size);
	LL02_.DescendGradient(nu, batch_size);
	LL03_.DescendGradient(nu, batch_size);
	LL04_.DescendGradient(nu, batch_size);
	LLTL_.DescendGradient(nu, batch_size);
}

/****************************************************| 08  Layers |****************************************************/

template<
		template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int L02, //
		int L03, //
		int L04, //
		int L05,
		int OUT>
class LayerBoss08 {

	protected:

		//Layers
		Layer<N_T, INP, L00> LLHD_;
		Layer<N_T, L00, L01> LL01_;
		Layer<N_T, L01, L02> LL02_;
		Layer<N_T, L02, L03> LL03_;
		Layer<N_T, L03, L04> LL04_;
		Layer<N_T, L04, L05> LL05_;
		Layer<N_T, L05, OUT> LLTL_;

	public:

		LayerBoss08(const CommAry<INP>& retina, const CommAry<OUT>& trainer, double nu);

		void FeedForward();
		void FeedBackward();
		void Update(double nu, int batch_size);
};

/*--------------------------------------------------| Constructor |---------------------------------------------------*/

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00,
		int L01,
		int L02,
		int L03,
		int L04,
		int L05,
		int OUT>
inline LayerBoss08<
		N_T,
		INP,
		L00,
		L01,
		L02,
		L03,
		L04,
		L05,
		OUT
>::LayerBoss08(const CommAry<INP>& retina, const CommAry<OUT>& trainer, double nu) :
				LLHD_(Layer<N_T, INP, L00>(retina)),
				LL01_(Layer<N_T, L00, L01>(LLHD_.Synaptogenisis())),
				LL02_(Layer<N_T, L01, L02>(LL01_.Synaptogenisis())),
				LL03_(Layer<N_T, L02, L03>(LL02_.Synaptogenisis())),
				LL04_(Layer<N_T, L03, L04>(LL03_.Synaptogenisis())),
				LL05_(Layer<N_T, L04, L05>(LL04_.Synaptogenisis())),
				LLTL_(Layer<N_T, L05, OUT>(LL05_.Synaptogenisis(), trainer)) {

	LLHD_.SetAllWeight(GetRaGCS, 0.02, (0.5 / INP));
	LL01_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L00));
	LL02_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L01));
	LL03_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L02));
	LL04_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L03));
	LL05_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L04));
	LLTL_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L05));

	LLHD_.SetAllBiases(GetRaG, 0.01, 0);
	LL01_.SetAllBiases(GetRaG, 0.01, 0);
	LL02_.SetAllBiases(GetRaG, 0.01, 0);
	LL03_.SetAllBiases(GetRaG, 0.01, 0);
	LL04_.SetAllBiases(GetRaG, 0.01, 0);
	LL05_.SetAllBiases(GetRaG, 0.01, 0);
	LLTL_.SetAllBiases(GetRaG, 0.01, 0);
}

/*----------------------------------------------------| Methods |-----------------------------------------------------*/

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int L02, //
		int L03, //
		int L04, //
		int L05, //
		int OUT>
inline void LayerBoss08<
		N_T,
		INP,
		L00,
		L01,
		L02,
		L03,
		L04,
		L05,
		OUT>::FeedForward() {

	LLHD_.Z_Cascade();
	LLHD_.Activation();
	LL01_.Z_Cascade();
	LL01_.Activation();
	LL02_.Z_Cascade();
	LL02_.Activation();
	LL03_.Z_Cascade();
	LL03_.Activation();
	LL04_.Z_Cascade();
	LL04_.Activation();
	LL05_.Z_Cascade();
	LL05_.Activation();
	LLTL_.Z_Cascade();
	LLTL_.Activation();
}

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int L02, //
		int L03, //
		int L04, //
		int L05, //
		int OUT>
inline void LayerBoss08<
		N_T,
		INP,
		L00,
		L01,
		L02,
		L03,
		L04,
		L05,
		OUT>::FeedBackward() {

	LLTL_.OutputErrorC();
	LLTL_.ErrorCascade();
	LL05_.ErrorCascade();
	LL04_.ErrorCascade();
	LL03_.ErrorCascade();
	LL02_.ErrorCascade();
	LL01_.ErrorCascade();
}

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int L02, //
		int L03, //
		int L04, //
		int L05, //
		int OUT>
inline void LayerBoss08<
		N_T,
		INP,
		L00,
		L01,
		L02,
		L03,
		L04,
		L05,
		OUT>::Update(double nu, int batch_size) {
	LLHD_.DescendGradient(nu, batch_size);
	LL01_.DescendGradient(nu, batch_size);
	LL02_.DescendGradient(nu, batch_size);
	LL03_.DescendGradient(nu, batch_size);
	LL04_.DescendGradient(nu, batch_size);
	LL05_.DescendGradient(nu, batch_size);
	LLTL_.DescendGradient(nu, batch_size);
}

/****************************************************| 09  Layers |****************************************************/

template<
		template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int L02, //
		int L03, //
		int L04, //
		int L05, //
		int L06, //
		int OUT>
class LayerBoss09 {

	protected:

		//Layers
		Layer<N_T, INP, L00> LLHD_;
		Layer<N_T, L00, L01> LL01_;
		Layer<N_T, L01, L02> LL02_;
		Layer<N_T, L02, L03> LL03_;
		Layer<N_T, L03, L04> LL04_;
		Layer<N_T, L04, L05> LL05_;
		Layer<N_T, L05, L06> LL06_;
		Layer<N_T, L06, OUT> LLTL_;

	public:

		LayerBoss09(const CommAry<INP>& retina, const CommAry<OUT>& trainer, double nu);

		void FeedForward();
		void FeedBackward();
		void Update(double nu, int batch_size);
};

/*--------------------------------------------------| Constructor |---------------------------------------------------*/

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00,
		int L01,
		int L02,
		int L03,
		int L04,
		int L05,
		int L06,
		int OUT>
inline LayerBoss09<
		N_T,
		INP,
		L00,
		L01,
		L02,
		L03,
		L04,
		L05,
		L06,
		OUT
>::LayerBoss09(const CommAry<INP>& retina, const CommAry<OUT>& trainer, double nu) :
				LLHD_(Layer<N_T, INP, L00>(retina)),
				LL01_(Layer<N_T, L00, L01>(LLHD_.Synaptogenisis())),
				LL02_(Layer<N_T, L01, L02>(LL01_.Synaptogenisis())),
				LL03_(Layer<N_T, L02, L03>(LL02_.Synaptogenisis())),
				LL04_(Layer<N_T, L03, L04>(LL03_.Synaptogenisis())),
				LL05_(Layer<N_T, L04, L05>(LL04_.Synaptogenisis())),
				LL06_(Layer<N_T, L05, L06>(LL05_.Synaptogenisis())),
				LLTL_(Layer<N_T, L06, OUT>(LL06_.Synaptogenisis(), trainer)) {

	LLHD_.SetAllWeight(GetRaGCS, 0.02, (0.5 / INP));
	LL01_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L00));
	LL02_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L01));
	LL03_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L02));
	LL04_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L03));
	LL05_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L04));
	LL06_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L05));
	LLTL_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L06));

	LLHD_.SetAllBiases(GetRaG, 0.01, 0);
	LL01_.SetAllBiases(GetRaG, 0.01, 0);
	LL02_.SetAllBiases(GetRaG, 0.01, 0);
	LL03_.SetAllBiases(GetRaG, 0.01, 0);
	LL04_.SetAllBiases(GetRaG, 0.01, 0);
	LL05_.SetAllBiases(GetRaG, 0.01, 0);
	LL06_.SetAllBiases(GetRaG, 0.01, 0);
	LLTL_.SetAllBiases(GetRaG, 0.01, 0);
}

/*----------------------------------------------------| Methods |-----------------------------------------------------*/

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int L02, //
		int L03, //
		int L04, //
		int L05, //
		int L06, //
		int OUT>
inline void LayerBoss09<
		N_T,
		INP,
		L00,
		L01,
		L02,
		L03,
		L04,
		L05,
		L06,
		OUT>::FeedForward() {

	LLHD_.Z_Cascade();
	LLHD_.Activation();
	LL01_.Z_Cascade();
	LL01_.Activation();
	LL02_.Z_Cascade();
	LL02_.Activation();
	LL03_.Z_Cascade();
	LL03_.Activation();
	LL04_.Z_Cascade();
	LL04_.Activation();
	LL05_.Z_Cascade();
	LL05_.Activation();
	LL06_.Z_Cascade();
	LL06_.Activation();
	LLTL_.Z_Cascade();
	LLTL_.Activation();
}

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int L02, //
		int L03, //
		int L04, //
		int L05, //
		int L06, //
		int OUT>
inline void LayerBoss09<
		N_T,
		INP,
		L00,
		L01,
		L02,
		L03,
		L04,
		L05,
		L06,
		OUT>::FeedBackward() {

	LLTL_.OutputErrorC();
	LLTL_.ErrorCascade();
	LL06_.ErrorCascade();
	LL05_.ErrorCascade();
	LL04_.ErrorCascade();
	LL03_.ErrorCascade();
	LL02_.ErrorCascade();
	LL01_.ErrorCascade();
}

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int L02, //
		int L03, //
		int L04, //
		int L05, //
		int L06, //
		int OUT>
inline void LayerBoss09<
		N_T,
		INP,
		L00,
		L01,
		L02,
		L03,
		L04,
		L05,
		L06,
		OUT>::Update(double nu, int batch_size) {
	LLHD_.DescendGradient(nu, batch_size);
	LL01_.DescendGradient(nu, batch_size);
	LL02_.DescendGradient(nu, batch_size);
	LL03_.DescendGradient(nu, batch_size);
	LL04_.DescendGradient(nu, batch_size);
	LL05_.DescendGradient(nu, batch_size);
	LL06_.DescendGradient(nu, batch_size);
	LLTL_.DescendGradient(nu, batch_size);
}

/****************************************************| 10  Layers |****************************************************/

template<
		template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int L02, //
		int L03, //
		int L04, //
		int L05, //
		int L06, //
		int L07, //
		int OUT>
class LayerBoss10 {

	protected:

		//Layers
		Layer<N_T, INP, L00> LLHD_;
		Layer<N_T, L00, L01> LL01_;
		Layer<N_T, L01, L02> LL02_;
		Layer<N_T, L02, L03> LL03_;
		Layer<N_T, L03, L04> LL04_;
		Layer<N_T, L04, L05> LL05_;
		Layer<N_T, L05, L06> LL06_;
		Layer<N_T, L06, L07> LL07_;
		Layer<N_T, L07, OUT> LLTL_;

	public:

		LayerBoss10(const CommAry<INP>& retina, const CommAry<OUT>& trainer, double nu);

		void FeedForward();
		void FeedBackward();
		void Update(double nu, int batch_size);
};

/*--------------------------------------------------| Constructor |---------------------------------------------------*/

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00,
		int L01,
		int L02,
		int L03,
		int L04,
		int L05,
		int L06,
		int L07,
		int OUT>
inline LayerBoss10<
		N_T,
		INP,
		L00,
		L01,
		L02,
		L03,
		L04,
		L05,
		L06,
		L07,
		OUT
>::LayerBoss10(const CommAry<INP>& retina, const CommAry<OUT>& trainer, double nu) :
				LLHD_(Layer<N_T, INP, L00>(retina)),
				LL01_(Layer<N_T, L00, L01>(LLHD_.Synaptogenisis())),
				LL02_(Layer<N_T, L01, L02>(LL01_.Synaptogenisis())),
				LL03_(Layer<N_T, L02, L03>(LL02_.Synaptogenisis())),
				LL04_(Layer<N_T, L03, L04>(LL03_.Synaptogenisis())),
				LL05_(Layer<N_T, L04, L05>(LL04_.Synaptogenisis())),
				LL06_(Layer<N_T, L05, L06>(LL05_.Synaptogenisis())),
				LL07_(Layer<N_T, L06, L07>(LL06_.Synaptogenisis())),
				LLTL_(Layer<N_T, L07, OUT>(LL07_.Synaptogenisis(), trainer)) {

	LLHD_.SetAllWeight(GetRaGCS, 0.02, (0.5 / INP));
	LL01_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L00));
	LL02_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L01));
	LL03_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L02));
	LL04_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L03));
	LL05_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L04));
	LL06_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L05));
	LL07_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L06));
	LLTL_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L07));

	LLHD_.SetAllBiases(GetRaG, 0.01, 0);
	LL01_.SetAllBiases(GetRaG, 0.01, 0);
	LL02_.SetAllBiases(GetRaG, 0.01, 0);
	LL03_.SetAllBiases(GetRaG, 0.01, 0);
	LL04_.SetAllBiases(GetRaG, 0.01, 0);
	LL05_.SetAllBiases(GetRaG, 0.01, 0);
	LL06_.SetAllBiases(GetRaG, 0.01, 0);
	LL07_.SetAllBiases(GetRaG, 0.01, 0);
	LLTL_.SetAllBiases(GetRaG, 0.01, 0);
}

/*----------------------------------------------------| Methods |-----------------------------------------------------*/

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int L02, //
		int L03, //
		int L04, //
		int L05, //
		int L06, //
		int L07, //
		int OUT>
inline void LayerBoss10<
		N_T,
		INP,
		L00,
		L01,
		L02,
		L03,
		L04,
		L05,
		L06,
		L07,
		OUT>::FeedForward() {

	LLHD_.Z_Cascade();
	LLHD_.Activation();
	LL01_.Z_Cascade();
	LL01_.Activation();
	LL02_.Z_Cascade();
	LL02_.Activation();
	LL03_.Z_Cascade();
	LL03_.Activation();
	LL04_.Z_Cascade();
	LL04_.Activation();
	LL05_.Z_Cascade();
	LL05_.Activation();
	LL06_.Z_Cascade();
	LL06_.Activation();
	LL07_.Z_Cascade();
	LL07_.Activation();
	LLTL_.Z_Cascade();
	LLTL_.Activation();
}

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int L02, //
		int L03, //
		int L04, //
		int L05, //
		int L06, //
		int L07, //
		int OUT>
inline void LayerBoss10<
		N_T,
		INP,
		L00,
		L01,
		L02,
		L03,
		L04,
		L05,
		L06,
		L07,
		OUT>::FeedBackward() {

	LLTL_.OutputErrorC();
	LLTL_.ErrorCascade();
	LL07_.ErrorCascade();
	LL06_.ErrorCascade();
	LL05_.ErrorCascade();
	LL04_.ErrorCascade();
	LL03_.ErrorCascade();
	LL02_.ErrorCascade();
	LL01_.ErrorCascade();
}

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int L02, //
		int L03, //
		int L04, //
		int L05, //
		int L06, //
		int L07, //
		int OUT>
inline void LayerBoss10<
		N_T,
		INP,
		L00,
		L01,
		L02,
		L03,
		L04,
		L05,
		L06,
		L07,
		OUT>::Update(double nu, int batch_size) {
	LLHD_.DescendGradient(nu, batch_size);
	LL01_.DescendGradient(nu, batch_size);
	LL02_.DescendGradient(nu, batch_size);
	LL03_.DescendGradient(nu, batch_size);
	LL04_.DescendGradient(nu, batch_size);
	LL05_.DescendGradient(nu, batch_size);
	LL06_.DescendGradient(nu, batch_size);
	LL07_.DescendGradient(nu, batch_size);
	LLTL_.DescendGradient(nu, batch_size);
}

/****************************************************| 11  Layers |****************************************************/

template<
		template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int L02, //
		int L03, //
		int L04, //
		int L05, //
		int L06, //
		int L07, //
		int L08, //
		int OUT>
class LayerBoss11 {

	protected:

		//Layers
		Layer<N_T, INP, L00> LLHD_;
		Layer<N_T, L00, L01> LL01_;
		Layer<N_T, L01, L02> LL02_;
		Layer<N_T, L02, L03> LL03_;
		Layer<N_T, L03, L04> LL04_;
		Layer<N_T, L04, L05> LL05_;
		Layer<N_T, L05, L06> LL06_;
		Layer<N_T, L06, L07> LL07_;
		Layer<N_T, L07, L08> LL08_;
		Layer<N_T, L08, OUT> LLTL_;

	public:

		LayerBoss11(const CommAry<INP>& retina, const CommAry<OUT>& trainer, double nu);

		void FeedForward();
		void FeedBackward();
		void Update(double nu, int batch_size);
};

/*--------------------------------------------------| Constructor |---------------------------------------------------*/

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00,
		int L01,
		int L02,
		int L03,
		int L04,
		int L05,
		int L06,
		int L07,
		int L08,
		int OUT>
inline LayerBoss11<
		N_T,
		INP,
		L00,
		L01,
		L02,
		L03,
		L04,
		L05,
		L06,
		L07,
		L08,
		OUT
>::LayerBoss11(const CommAry<INP>& retina, const CommAry<OUT>& trainer, double nu) :
				LLHD_(Layer<N_T, INP, L00>(retina)),
				LL01_(Layer<N_T, L00, L01>(LLHD_.Synaptogenisis())),
				LL02_(Layer<N_T, L01, L02>(LL01_.Synaptogenisis())),
				LL03_(Layer<N_T, L02, L03>(LL02_.Synaptogenisis())),
				LL04_(Layer<N_T, L03, L04>(LL03_.Synaptogenisis())),
				LL05_(Layer<N_T, L04, L05>(LL04_.Synaptogenisis())),
				LL06_(Layer<N_T, L05, L06>(LL05_.Synaptogenisis())),
				LL07_(Layer<N_T, L06, L07>(LL06_.Synaptogenisis())),
				LL08_(Layer<N_T, L07, L08>(LL07_.Synaptogenisis())),
				LLTL_(Layer<N_T, L08, OUT>(LL08_.Synaptogenisis(), trainer)) {

	LLHD_.SetAllWeight(GetRaGCS, 0.02, (0.5 / INP));
	LL01_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L00));
	LL02_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L01));
	LL03_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L02));
	LL04_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L03));
	LL05_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L04));
	LL06_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L05));
	LL07_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L06));
	LL08_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L07));
	LLTL_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L08));

	LLHD_.SetAllBiases(GetRaG, 0.01, 0);
	LL01_.SetAllBiases(GetRaG, 0.01, 0);
	LL02_.SetAllBiases(GetRaG, 0.01, 0);
	LL03_.SetAllBiases(GetRaG, 0.01, 0);
	LL04_.SetAllBiases(GetRaG, 0.01, 0);
	LL05_.SetAllBiases(GetRaG, 0.01, 0);
	LL06_.SetAllBiases(GetRaG, 0.01, 0);
	LL07_.SetAllBiases(GetRaG, 0.01, 0);
	LL08_.SetAllBiases(GetRaG, 0.01, 0);
	LLTL_.SetAllBiases(GetRaG, 0.01, 0);
}

/*----------------------------------------------------| Methods |-----------------------------------------------------*/

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int L02, //
		int L03, //
		int L04, //
		int L05, //
		int L06, //
		int L07, //
		int L08, //
		int OUT>
inline void LayerBoss11<
		N_T,
		INP,
		L00,
		L01,
		L02,
		L03,
		L04,
		L05,
		L06,
		L07,
		L08,
		OUT>::FeedForward() {

	LLHD_.Z_Cascade();
	LLHD_.Activation();
	LL01_.Z_Cascade();
	LL01_.Activation();
	LL02_.Z_Cascade();
	LL02_.Activation();
	LL03_.Z_Cascade();
	LL03_.Activation();
	LL04_.Z_Cascade();
	LL04_.Activation();
	LL05_.Z_Cascade();
	LL05_.Activation();
	LL06_.Z_Cascade();
	LL06_.Activation();
	LL07_.Z_Cascade();
	LL07_.Activation();
	LL08_.Z_Cascade();
	LL08_.Activation();
	LLTL_.Z_Cascade();
	LLTL_.Activation();
}

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int L02, //
		int L03, //
		int L04, //
		int L05, //
		int L06, //
		int L07, //
		int L08, //
		int OUT>
inline void LayerBoss11<
		N_T,
		INP,
		L00,
		L01,
		L02,
		L03,
		L04,
		L05,
		L06,
		L07,
		L08,
		OUT>::FeedBackward() {

	LLTL_.OutputErrorC();
	LLTL_.ErrorCascade();
	LL08_.ErrorCascade();
	LL07_.ErrorCascade();
	LL06_.ErrorCascade();
	LL05_.ErrorCascade();
	LL04_.ErrorCascade();
	LL03_.ErrorCascade();
	LL02_.ErrorCascade();
	LL01_.ErrorCascade();
}

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int L02, //
		int L03, //
		int L04, //
		int L05, //
		int L06, //
		int L07, //
		int L08, //
		int OUT>
inline void LayerBoss11<
		N_T,
		INP,
		L00,
		L01,
		L02,
		L03,
		L04,
		L05,
		L06,
		L07,
		L08,
		OUT>::Update(double nu, int batch_size) {
	LLHD_.DescendGradient(nu, batch_size);
	LL01_.DescendGradient(nu, batch_size);
	LL02_.DescendGradient(nu, batch_size);
	LL03_.DescendGradient(nu, batch_size);
	LL04_.DescendGradient(nu, batch_size);
	LL05_.DescendGradient(nu, batch_size);
	LL06_.DescendGradient(nu, batch_size);
	LL07_.DescendGradient(nu, batch_size);
	LL08_.DescendGradient(nu, batch_size);
	LLTL_.DescendGradient(nu, batch_size);
}

/****************************************************| 12  Layers |****************************************************/

template<
		template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int L02, //
		int L03, //
		int L04, //
		int L05, //
		int L06, //
		int L07, //
		int L08, //
		int L09, //
		int OUT>
class LayerBoss12 {

	protected:

		//Layers
		Layer<N_T, INP, L00> LLHD_;
		Layer<N_T, L00, L01> LL01_;
		Layer<N_T, L01, L02> LL02_;
		Layer<N_T, L02, L03> LL03_;
		Layer<N_T, L03, L04> LL04_;
		Layer<N_T, L04, L05> LL05_;
		Layer<N_T, L05, L06> LL06_;
		Layer<N_T, L06, L07> LL07_;
		Layer<N_T, L07, L08> LL08_;
		Layer<N_T, L08, L09> LL09_;
		Layer<N_T, L09, OUT> LLTL_;

	public:

		LayerBoss12(const CommAry<INP>& retina, const CommAry<OUT>& trainer, double nu);

		void FeedForward();
		void FeedBackward();
		void Update(double nu, int batch_size);
};

/*--------------------------------------------------| Constructor |---------------------------------------------------*/

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00,
		int L01,
		int L02,
		int L03,
		int L04,
		int L05,
		int L06,
		int L07,
		int L08,
		int L09,
		int OUT>
inline LayerBoss12<
		N_T,
		INP,
		L00,
		L01,
		L02,
		L03,
		L04,
		L05,
		L06,
		L07,
		L08,
		L09,
		OUT
>::LayerBoss12(const CommAry<INP>& retina, const CommAry<OUT>& trainer, double nu) :
				LLHD_(Layer<N_T, INP, L00>(retina)),
				LL01_(Layer<N_T, L00, L01>(LLHD_.Synaptogenisis())),
				LL02_(Layer<N_T, L01, L02>(LL01_.Synaptogenisis())),
				LL03_(Layer<N_T, L02, L03>(LL02_.Synaptogenisis())),
				LL04_(Layer<N_T, L03, L04>(LL03_.Synaptogenisis())),
				LL05_(Layer<N_T, L04, L05>(LL04_.Synaptogenisis())),
				LL06_(Layer<N_T, L05, L06>(LL05_.Synaptogenisis())),
				LL07_(Layer<N_T, L06, L07>(LL06_.Synaptogenisis())),
				LL08_(Layer<N_T, L07, L08>(LL07_.Synaptogenisis())),
				LL09_(Layer<N_T, L08, L09>(LL08_.Synaptogenisis())),
				LLTL_(Layer<N_T, L09, OUT>(LL09_.Synaptogenisis(), trainer)) {

	LLHD_.SetAllWeight(GetRaGCS, 0.02, (0.5 / INP));
	LL01_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L00));
	LL02_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L01));
	LL03_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L02));
	LL04_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L03));
	LL05_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L04));
	LL06_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L05));
	LL07_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L06));
	LL08_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L07));
	LL09_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L08));
	LLTL_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L09));

	LLHD_.SetAllBiases(GetRaG, 0.01, 0);
	LL01_.SetAllBiases(GetRaG, 0.01, 0);
	LL02_.SetAllBiases(GetRaG, 0.01, 0);
	LL03_.SetAllBiases(GetRaG, 0.01, 0);
	LL04_.SetAllBiases(GetRaG, 0.01, 0);
	LL05_.SetAllBiases(GetRaG, 0.01, 0);
	LL06_.SetAllBiases(GetRaG, 0.01, 0);
	LL07_.SetAllBiases(GetRaG, 0.01, 0);
	LL08_.SetAllBiases(GetRaG, 0.01, 0);
	LL09_.SetAllBiases(GetRaG, 0.01, 0);
	LLTL_.SetAllBiases(GetRaG, 0.01, 0);
}

/*----------------------------------------------------| Methods |-----------------------------------------------------*/

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int L02, //
		int L03, //
		int L04, //
		int L05, //
		int L06, //
		int L07, //
		int L08, //
		int L09, //
		int OUT>
inline void LayerBoss12<
		N_T,
		INP,
		L00,
		L01,
		L02,
		L03,
		L04,
		L05,
		L06,
		L07,
		L08,
		L09,
		OUT>::FeedForward() {

	LLHD_.Z_Cascade();
	LLHD_.Activation();
	LL01_.Z_Cascade();
	LL01_.Activation();
	LL02_.Z_Cascade();
	LL02_.Activation();
	LL03_.Z_Cascade();
	LL03_.Activation();
	LL04_.Z_Cascade();
	LL04_.Activation();
	LL05_.Z_Cascade();
	LL05_.Activation();
	LL06_.Z_Cascade();
	LL06_.Activation();
	LL07_.Z_Cascade();
	LL07_.Activation();
	LL08_.Z_Cascade();
	LL08_.Activation();
	LL09_.Z_Cascade();
	LL09_.Activation();
	LLTL_.Z_Cascade();
	LLTL_.Activation();
}

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int L02, //
		int L03, //
		int L04, //
		int L05, //
		int L06, //
		int L07, //
		int L08, //
		int L09, //
		int OUT>
inline void LayerBoss12<
		N_T,
		INP,
		L00,
		L01,
		L02,
		L03,
		L04,
		L05,
		L06,
		L07,
		L08,
		L09,
		OUT>::FeedBackward() {

	LLTL_.OutputErrorC();
	LLTL_.ErrorCascade();
	LL09_.ErrorCascade();
	LL08_.ErrorCascade();
	LL07_.ErrorCascade();
	LL06_.ErrorCascade();
	LL05_.ErrorCascade();
	LL04_.ErrorCascade();
	LL03_.ErrorCascade();
	LL02_.ErrorCascade();
	LL01_.ErrorCascade();
}

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int L02, //
		int L03, //
		int L04, //
		int L05, //
		int L06, //
		int L07, //
		int L08, //
		int L09, //
		int OUT>
inline void LayerBoss12<
		N_T,
		INP,
		L00,
		L01,
		L02,
		L03,
		L04,
		L05,
		L06,
		L07,
		L08,
		L09,
		OUT>::Update(double nu, int batch_size) {
	LLHD_.DescendGradient(nu, batch_size);
	LL01_.DescendGradient(nu, batch_size);
	LL02_.DescendGradient(nu, batch_size);
	LL03_.DescendGradient(nu, batch_size);
	LL04_.DescendGradient(nu, batch_size);
	LL05_.DescendGradient(nu, batch_size);
	LL06_.DescendGradient(nu, batch_size);
	LL07_.DescendGradient(nu, batch_size);
	LL08_.DescendGradient(nu, batch_size);
	LL09_.DescendGradient(nu, batch_size);
	LLTL_.DescendGradient(nu, batch_size);
}

/****************************************************| 13  Layers |****************************************************/

template<
		template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int L02, //
		int L03, //
		int L04, //
		int L05, //
		int L06, //
		int L07, //
		int L08, //
		int L09, //
		int L10, //
		int OUT>
class LayerBoss13 {

	protected:

		//Layers
		Layer<N_T, INP, L00> LLHD_;
		Layer<N_T, L00, L01> LL01_;
		Layer<N_T, L01, L02> LL02_;
		Layer<N_T, L02, L03> LL03_;
		Layer<N_T, L03, L04> LL04_;
		Layer<N_T, L04, L05> LL05_;
		Layer<N_T, L05, L06> LL06_;
		Layer<N_T, L06, L07> LL07_;
		Layer<N_T, L07, L08> LL08_;
		Layer<N_T, L08, L09> LL09_;
		Layer<N_T, L09, L10> LL10_;
		Layer<N_T, L10, OUT> LLTL_;

	public:

		LayerBoss13(const CommAry<INP>& retina, const CommAry<OUT>& trainer, double nu);

		void FeedForward();
		void FeedBackward();
		void Update(double nu, int batch_size);
};

/*--------------------------------------------------| Constructor |---------------------------------------------------*/

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00,
		int L01,
		int L02,
		int L03,
		int L04,
		int L05,
		int L06,
		int L07,
		int L08,
		int L09,
		int L10,
		int OUT>
inline LayerBoss13<
		N_T,
		INP,
		L00,
		L01,
		L02,
		L03,
		L04,
		L05,
		L06,
		L07,
		L08,
		L09,
		L10,
		OUT
>::LayerBoss13(const CommAry<INP>& retina, const CommAry<OUT>& trainer, double nu) :
				LLHD_(Layer<N_T, INP, L00>(retina)),
				LL01_(Layer<N_T, L00, L01>(LLHD_.Synaptogenisis())),
				LL02_(Layer<N_T, L01, L02>(LL01_.Synaptogenisis())),
				LL03_(Layer<N_T, L02, L03>(LL02_.Synaptogenisis())),
				LL04_(Layer<N_T, L03, L04>(LL03_.Synaptogenisis())),
				LL05_(Layer<N_T, L04, L05>(LL04_.Synaptogenisis())),
				LL06_(Layer<N_T, L05, L06>(LL05_.Synaptogenisis())),
				LL07_(Layer<N_T, L06, L07>(LL06_.Synaptogenisis())),
				LL08_(Layer<N_T, L07, L08>(LL07_.Synaptogenisis())),
				LL09_(Layer<N_T, L08, L09>(LL08_.Synaptogenisis())),
				LL10_(Layer<N_T, L09, L10>(LL09_.Synaptogenisis())),
				LLTL_(Layer<N_T, L10, OUT>(LL10_.Synaptogenisis(), trainer)) {

	LLHD_.SetAllWeight(GetRaGCS, 0.02, (0.5 / INP));
	LL01_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L00));
	LL02_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L01));
	LL03_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L02));
	LL04_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L03));
	LL05_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L04));
	LL06_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L05));
	LL07_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L06));
	LL08_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L07));
	LL09_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L08));
	LL10_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L09));
	LLTL_.SetAllWeight(GetRaGCS, 0.02, (0.5 / L10));

	LLHD_.SetAllBiases(GetRaG, 0.01, 0);
	LL01_.SetAllBiases(GetRaG, 0.01, 0);
	LL02_.SetAllBiases(GetRaG, 0.01, 0);
	LL03_.SetAllBiases(GetRaG, 0.01, 0);
	LL04_.SetAllBiases(GetRaG, 0.01, 0);
	LL05_.SetAllBiases(GetRaG, 0.01, 0);
	LL06_.SetAllBiases(GetRaG, 0.01, 0);
	LL07_.SetAllBiases(GetRaG, 0.01, 0);
	LL08_.SetAllBiases(GetRaG, 0.01, 0);
	LL09_.SetAllBiases(GetRaG, 0.01, 0);
	LL10_.SetAllBiases(GetRaG, 0.01, 0);
	LLTL_.SetAllBiases(GetRaG, 0.01, 0);
}

/*----------------------------------------------------| Methods |-----------------------------------------------------*/

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int L02, //
		int L03, //
		int L04, //
		int L05, //
		int L06, //
		int L07, //
		int L08, //
		int L09, //
		int L10, //
		int OUT>
inline void LayerBoss13<
		N_T,
		INP,
		L00,
		L01,
		L02,
		L03,
		L04,
		L05,
		L06,
		L07,
		L08,
		L09,
		L10,
		OUT>::FeedForward() {

	LLHD_.Z_Cascade();
	LLHD_.Activation();
	LL01_.Z_Cascade();
	LL01_.Activation();
	LL02_.Z_Cascade();
	LL02_.Activation();
	LL03_.Z_Cascade();
	LL03_.Activation();
	LL04_.Z_Cascade();
	LL04_.Activation();
	LL05_.Z_Cascade();
	LL05_.Activation();
	LL06_.Z_Cascade();
	LL06_.Activation();
	LL07_.Z_Cascade();
	LL07_.Activation();
	LL08_.Z_Cascade();
	LL08_.Activation();
	LL09_.Z_Cascade();
	LL09_.Activation();
	LL10_.Z_Cascade();
	LL10_.Activation();
	LLTL_.Z_Cascade();
	LLTL_.Activation();
}

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int L02, //
		int L03, //
		int L04, //
		int L05, //
		int L06, //
		int L07, //
		int L08, //
		int L09, //
		int L10, //
		int OUT>
inline void LayerBoss13<
		N_T,
		INP,
		L00,
		L01,
		L02,
		L03,
		L04,
		L05,
		L06,
		L07,
		L08,
		L09,
		L10,
		OUT>::FeedBackward() {

	LLTL_.OutputErrorC();
	LLTL_.ErrorCascade();
	LL10_.ErrorCascade();
	LL09_.ErrorCascade();
	LL08_.ErrorCascade();
	LL07_.ErrorCascade();
	LL06_.ErrorCascade();
	LL05_.ErrorCascade();
	LL04_.ErrorCascade();
	LL03_.ErrorCascade();
	LL02_.ErrorCascade();
	LL01_.ErrorCascade();
}

template<template<int> class N_T, //Type of Neuron
		int INP, //Layer sizes for each layer
		int L00, //
		int L01, //
		int L02, //
		int L03, //
		int L04, //
		int L05, //
		int L06, //
		int L07, //
		int L08, //
		int L09, //
		int L10, //
		int OUT>
inline void LayerBoss13<
		N_T,
		INP,
		L00,
		L01,
		L02,
		L03,
		L04,
		L05,
		L06,
		L07,
		L08,
		L09,
		L10,
		OUT>::Update(double nu, int batch_size) {
	LLHD_.DescendGradient(nu, batch_size);
	LL01_.DescendGradient(nu, batch_size);
	LL02_.DescendGradient(nu, batch_size);
	LL03_.DescendGradient(nu, batch_size);
	LL04_.DescendGradient(nu, batch_size);
	LL05_.DescendGradient(nu, batch_size);
	LL06_.DescendGradient(nu, batch_size);
	LL07_.DescendGradient(nu, batch_size);
	LL08_.DescendGradient(nu, batch_size);
	LL09_.DescendGradient(nu, batch_size);
	LL10_.DescendGradient(nu, batch_size);
	LLTL_.DescendGradient(nu, batch_size);
}

#endif /* NETTEMPLATES_H_ */


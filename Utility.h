/*
 * DownCaster.cpp
 *
 *  Created on: Apr 11, 2015
 *      Author: paracite
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#include <stdexcept>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <iomanip>
#include <cstring>

#include <math.h>
#include "DLog.h"
#include "Color.h"

/*************************************************|Function Signature|************************************************/

typedef double (*FuncSig)(double z); // Defines a simple function signature.

/************************************************|Communication Array|************************************************/

template<int Ar_Sz>
struct CoAr { //
		double ary_[Ar_Sz]; // The only actual data is this array

		void SetFromAr(const double (&copy_me)[Ar_Sz]) {
			memcpy(ary_, copy_me, sizeof(ary_));
		}

		int Size() {
			return Ar_Sz;
		}
};

/*-----------------------------------------|Communication Array Print Tools|-----------------------------------------*/

template<int Ar_Sz>
void PrintAr(const CoAr<Ar_Sz>& in, const char color[6]) {
	if (Ar_Sz > 50) {
		throw std::invalid_argument("Array too large to print.");
	}

	std::cout << color << std::fixed << std::setprecision(3);
	std::cout << std::right << std::setw(7) << in.ary_[0];
	for (int i = 1; i < Ar_Sz; ++i) {
		std::cout << std::right << std::setw(7) << in.ary_[i];
	}

	std::cout << RESET;
}

template<int Ar_Sz>
void PrintArMatch(
		const CoAr<Ar_Sz>& in,
		const CoAr<Ar_Sz>& compare,
		const char color_n[6],
		const char color_y[6],
		double factr) {
	if (Ar_Sz > 50) {
		throw std::invalid_argument("Array too large to print.");
	}

	bool is_match[Ar_Sz];

	for (int i = 0; i < Ar_Sz; i++) {
		is_match[i] = ( (in.ary_[i] - compare.ary_[i]) < factr && (in.ary_[i] - compare.ary_[i]) > -(factr)) ? true : false;
	}

	if (is_match[0])
		std::cout << BOLD << color_y;
	else
		std::cout << color_n;

	std::cout << std::fixed << std::setprecision(3) << std::right << std::setw(7) << in.ary_[0] << RESET;
	for (int i = 1; i < Ar_Sz; ++i) {

		if (is_match[i])

			std::cout << BOLD << color_y;
		else
			std::cout << color_n;

		std::cout << std::right << std::setw(7) << in.ary_[i] << RESET;
	}

	std::cout << RESET;
}

template<int Ar_Sz>
void PrintArySci(
		const CoAr<Ar_Sz>& in,
		const char color[6]) {
	if (Ar_Sz > 50) {
		throw std::invalid_argument("Array too large to print.");
	}

	std::cout << color << std::scientific << std::setprecision(0);
	std::cout << std::right << std::setw(7) << (in.ary_[0]);
	for (int i = 1; i < Ar_Sz; ++i) {
		std::cout << std::right << std::setw(7) << (in.ary_[i]);
	}

	std::cout << RESET;
}

template<int Ar_Sz>
void PrintAry(
		const CoAr<Ar_Sz>& in) {
	if (Ar_Sz > 100) {
		throw std::invalid_argument("Array too large to print.");
	}

	std::cout << std::fixed << std::setprecision(3);
	std::cout << ( (signbit(in.ary_[0]) != 0) ? ("") : (" ")) << std::setw(5) << in.ary_[0];
	for (int i = 1; i < Ar_Sz; ++i) {
		std::cout << " " << std::setw(5) << in.ary_[i];
	}
}

/*-------------------------------------------------|CoAr ToolBox|-------------------------------------------------*/

template<int Ar_Sz>
struct CommZero { // Element-wise adds b to a, where parameter is a return parameter also.
		static const int num_of_rolls = Ar_Sz / 8;
		static const int rolled_total = num_of_rolls * 8;

		void operator()(CoAr<Ar_Sz>& a) {

			for (int i = 0; i < rolled_total; i += 8) {
				a.ary_[i + 0] = 0;
				a.ary_[i + 1] = 0;
				a.ary_[i + 2] = 0;
				a.ary_[i + 3] = 0;
				a.ary_[i + 4] = 0;
				a.ary_[i + 5] = 0;
				a.ary_[i + 6] = 0;
				a.ary_[i + 7] = 0;
			}

			for (int i = rolled_total; i < Ar_Sz; i++) {
				a.ary_[i] = 0;
			}
		}
};

template<int Ar_Sz>
struct CommAdd { // Element-wise adds b to a, where parameter is a return parameter also.
		static const int num_of_rolls = Ar_Sz / 8;
		static const int rolled_total = num_of_rolls * 8;

		void operator()(CoAr<Ar_Sz>& a, const CoAr<Ar_Sz>& b) {

			for (int i = 0; i < rolled_total; i += 8) {
				a.ary_[i + 0] += b.ary_[i + 0];
				a.ary_[i + 1] += b.ary_[i + 1];
				a.ary_[i + 2] += b.ary_[i + 2];
				a.ary_[i + 3] += b.ary_[i + 3];
				a.ary_[i + 4] += b.ary_[i + 4];
				a.ary_[i + 5] += b.ary_[i + 5];
				a.ary_[i + 6] += b.ary_[i + 6];
				a.ary_[i + 7] += b.ary_[i + 7];
			}

			for (int i = rolled_total; i < Ar_Sz; i++) {
				a.ary_[i] += b.ary_[i];
			}
		}
};

template<int Ar_Sz>
struct CommSub {
		static const int num_of_rolls = Ar_Sz / 8;
		static const int rolled_total = num_of_rolls * 8;

		// Element-wise subtracts b from a, where parameter is a return parameter also.
		void operator()(CoAr<Ar_Sz>& a, const CoAr<Ar_Sz>& b) {

			for (int i = 0; i < rolled_total; i += 8) {
				a.ary_[i + 0] -= b.ary_[i + 0];
				a.ary_[i + 1] -= b.ary_[i + 1];
				a.ary_[i + 2] -= b.ary_[i + 2];
				a.ary_[i + 3] -= b.ary_[i + 3];
				a.ary_[i + 4] -= b.ary_[i + 4];
				a.ary_[i + 5] -= b.ary_[i + 5];
				a.ary_[i + 6] -= b.ary_[i + 6];
				a.ary_[i + 7] -= b.ary_[i + 7];
			}

			for (int i = rolled_total; i < Ar_Sz; i++) {
				a.ary_[i] -= b.ary_[i];
			}
		}

		// Element-wise subtracts b from c, a is the return parameters
		void operator()(CoAr<Ar_Sz>& a, const CoAr<Ar_Sz>& b, const CoAr<Ar_Sz>& c) {

			for (int i = 0; i < rolled_total; i += 8) { //Possible DELETEME
				a.ary_[i + 0] = 0;
				a.ary_[i + 1] = 0;
				a.ary_[i + 2] = 0;
				a.ary_[i + 3] = 0;
				a.ary_[i + 4] = 0;
				a.ary_[i + 5] = 0;
				a.ary_[i + 6] = 0;
				a.ary_[i + 7] = 0;
			}

			for (int i = rolled_total; i < Ar_Sz; i++) {
				a.ary_[i] = 0;
			}

			for (int i = 0; i < rolled_total; i += 8) {
				a.ary_[i + 0] = c.ary_[i + 0] - b.ary_[i + 0];
				a.ary_[i + 1] = c.ary_[i + 1] - b.ary_[i + 1];
				a.ary_[i + 2] = c.ary_[i + 2] - b.ary_[i + 2];
				a.ary_[i + 3] = c.ary_[i + 3] - b.ary_[i + 3];
				a.ary_[i + 4] = c.ary_[i + 4] - b.ary_[i + 4];
				a.ary_[i + 5] = c.ary_[i + 5] - b.ary_[i + 5];
				a.ary_[i + 6] = c.ary_[i + 6] - b.ary_[i + 6];
				a.ary_[i + 7] = c.ary_[i + 7] - b.ary_[i + 7];
			}

			for (int i = rolled_total; i < Ar_Sz; i++) {
				a.ary_[i] = c.ary_[i] - b.ary_[i];
			}
		}
};

template<int Ar_Sz>
struct CommMulCns { // Sum of the element-wise products of b and c, c is a constant, a is the return parameter.
		static const int num_of_rolls = Ar_Sz / 8;
		static const int rolled_total = num_of_rolls * 8;

		void operator()(const CoAr<Ar_Sz>& a, const CoAr<Ar_Sz>& b, const double& c) {
			a = 0;
			double a_00 = 0, a_01 = 0, a_02 = 0, a_03 = 0, a_04 = 0, a_05 = 0, a_06 = 0, a_07 = 0;

			for (int i = 0; i < rolled_total; i += 8) {
				a_00 += b.ary_[i + 0] * c;
				a_01 += b.ary_[i + 1] * c;
				a_02 += b.ary_[i + 2] * c;
				a_03 += b.ary_[i + 3] * c;
				a_04 += b.ary_[i + 4] * c;
				a_05 += b.ary_[i + 5] * c;
				a_06 += b.ary_[i + 6] * c;
				a_07 += b.ary_[i + 7] * c;
			}

			for (int i = rolled_total; i < Ar_Sz; i++) {
				a += b.ary_[i] * c;
			}
			a += a_00 + a_01 + a_02 + a_03 + a_04 + a_05 + a_06 + a_07;
		}
};

template<int Ar_Sz>
struct CommMulSum { // Sum of the element-wise products of b and c, a is the return parameter.
		static const int num_of_rolls = Ar_Sz / 8;
		static const int rolled_total = num_of_rolls * 8;

		void operator()(double& a, const CoAr<Ar_Sz>& b, const CoAr<Ar_Sz>& c) {
			a = 0;
			double a_00 = 0, a_01 = 0, a_02 = 0, a_03 = 0, a_04 = 0, a_05 = 0, a_06 = 0, a_07 = 0;

			for (int i = 0; i < rolled_total; i += 8) {
				a_00 += b.ary_[i + 0] * c.ary_[i + 0];
				a_01 += b.ary_[i + 1] * c.ary_[i + 1];
				a_02 += b.ary_[i + 2] * c.ary_[i + 2];
				a_03 += b.ary_[i + 3] * c.ary_[i + 3];
				a_04 += b.ary_[i + 4] * c.ary_[i + 4];
				a_05 += b.ary_[i + 5] * c.ary_[i + 5];
				a_06 += b.ary_[i + 6] * c.ary_[i + 6];
				a_07 += b.ary_[i + 7] * c.ary_[i + 7];
			}

			for (int i = rolled_total; i < Ar_Sz; i++) {
				a += b.ary_[i] * c.ary_[i];
			}
			a += a_00 + a_01 + a_02 + a_03 + a_04 + a_05 + a_06 + a_07;
		}
};

// TODO 9: Mull around the idea of making the array type ambiguous with another template parm. Lets do it!
// This array's elements get pointed to by all 'Neuron' objects in one layer.
// Array Wrapper. The template provides a convenient parameter for initializing the array to the correct size and
// type, because everything will be defined on compile time. It has the added utility of carrying around
// array sizing information that can be used for instantiation of others (see member function 'MakeNeuron(...)').

/********************************************| Layer Weights And Bias Data |******************************************/
template<int INP, int OUT>
struct Anamnesis {

		std::string type_of_neuron_;


};

/*******************************************|Global Normalized Random Number|*****************************************/

class RandTools {
		friend void RandExitHandler();

	private:

		// Hidden Constructors
		RandTools() :
						epsilon(std::numeric_limits<double>::min()),
						two_pi(2.0 * 3.14159265358979323846),
						z0(0),
						z1(0),
						generate(false),
						index(POOL_SIZE) {
		}

		RandTools(RandTools const&) :
						epsilon(0),
						two_pi(0),
						z0(0),
						z1(0),
						generate(false),
						index(POOL_SIZE) {
		}

		~RandTools() {
		}

		// Fields
		static const int POOL_SIZE = 2048;
		static RandTools* myPointerInstance;
		const double epsilon;
		const double two_pi;
		double z0, z1;
		bool generate;
		double pool[POOL_SIZE];
		int index;

	public:

		// Methods
		static RandTools* Instance();
		double GetRandomDouble();
		double GetGaussianDouble(double mu, double sigma);
};

extern void RandExitHandler(); // Called on exit to delete heap instance.

// ATTENTION: Users of this class MUST register the exit handler with 'atexit(...)'
// I'm using a singleton pattern here to have a single random number generator. It may be unneccisary though.
// I know C++ random numbers aren't the best, if I have time I'll make my own class for it. It works for this purpose
// though.

double GetRaD();
double GetRaD(double mult);
double GetRaD(double mult, double add);
bool GetRaB();
double GetRaG(double sigma, double mu);
double GetRaG(double sigma);
double GetRaG();
double GetRaGCS(double sigma, double scale);

/***********************************************|DownCast CRTP Wrapper|***********************************************/

template<typename Derived>
class DownCast {

	private:

		typedef DownCast BASE;
		// Producing typedef for casting the return type.  I'm not 100% sure this is necissary.

	public:

		Derived const* Self() const {
			return static_cast<Derived const*>(this);
		}

		Derived* Self() {
			return const_cast<Derived*>(static_cast<BASE const*>(this)->Self());
		}
		// This overloaded method

	protected:
		~DownCast() {
		}
};

// This clever little class and functions provide static polymorphism without the need for time intensice vtable
// lookups that are required for 'virtual' functions. The mechanism is: 'DownCast' is inherited by another class.
// The inheriting class passes itself back to 'DownCast' in the template parameter, setting up a CRTP.  This means
// that 'DownCast' is able to make calls to children.  The two member functions (actually one member function with
// 'const' overloading... sort of overloading, the one calls the other if neccissary) will return a 'static const'
// pointer to a 'DownCast' object. That object can be called by derived classes and BAM we have abstraction :)

/*************************************************|Heap Object Counter|***********************************************/

template<class Neuro_Type>
class Counter {

	private:

		static int objects_created_;
		static int objects_alive_;

	public:

		Counter();
		virtual ~Counter();

		int ObjectsCreated();
		int ObjectsAlive();
};
//FIXME This doesn't quite work right...

template<class Neuro_Type>
Counter<Neuro_Type>::Counter() {
	++objects_created_;
	++objects_alive_;
}

template<class Neuro_Type>
Counter<Neuro_Type>::~Counter() {
	--objects_alive_;
}

template<class Neuro_Type>
int Counter<Neuro_Type>::ObjectsCreated() {
	return (objects_created_);
}

template<class Neuro_Type>
int Counter<Neuro_Type>::ObjectsAlive() {
	return (objects_alive_);
}

template<class Neuro_Type> int Counter<Neuro_Type>::objects_created_(0);
template<class Neuro_Type> int Counter<Neuro_Type>::objects_alive_(0);

/*********************************************************************************************************************/

#endif /* UTILITY_H_ */

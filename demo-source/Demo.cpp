/*
 * alan.cpp
 *
 *  Created on: Apr 15, 2015
 *      Author: paracite
 */
#include <ctime>
#include <iostream>
#include <cstdlib>
#include <string.h>
#include <iomanip>
#include <unistd.h>

#include "../DLog.h" // NOTICE: Must only be defined in main() .
#include "../Neuron.h"
#include "../NeuronTypes.h"
#include "../POSI.h"
#include "../Color.h"
#include "../Layer.h"
#include "../Utility.h"
#include "../TestData.h"

using std::cout;
using std::cin;
using std::endl;
using std::setprecision;
using std::left;
using std::right;
using std::setw;

/********************************************************************************************************************/

void BackPropTest();

/********************************************************************************************************************/

int main(void) {

	/* Preamble */
	CLRSCR;
	DEBUG("Preamble Start\n");
	const int ext_rslt_01 = std::atexit(RandExitHandler); // Handle static heap object cleanup with callback.
	if (ext_rslt_01 != 0) {
		std::cerr << "Exit Handler Registration failed\n";
	}else {
		DEBUG("RandTools Exit Handler registerd.");
	}
	DEBUG("Preamble Finish\n");

	/* Body */
	DEBUG("Body Start\n");

	BackPropTest();
	cout << endl;

	//CreateLayersTest();
	DEBUG("Body Finish\n");

	/* Postamble */
	DEBUG("Postamble Start\n");
	DEBUG("Postamble Finish\n");

	std::cin.ignore();

	return (42);
}

/********************************************************************************************************************/

void BackPropTest() {
	DEBUG("Entering Backpropagation Test\n");

	/* Define Layer sizes and I/O sizes */
	const int INP = 8; // Input
	const int L00 = 9;
	const int L01 = 7;
	const int L02 = 6;
	const int OUT = 5;

	/* Make new Communication objects We'll construct them with the fist training pair */
	CoAr<INP> retina;
	CoAr<OUT> expect;

	/* Set first Training Set */
	retina.SetFromAr(ti[0]);
	expect.SetFromAr(ex[0]);

	/* Make and chain together LayerLord objects with proper input Communication */
	Layer<Sigmoid, INP, L00>* LLHD = new Layer<Sigmoid, INP, L00>(retina);
	Layer<Sigmoid, L00, L01>* LL01 = new Layer<Sigmoid, L00, L01>(LLHD->Synaptogenisis());
	Layer<Sigmoid, L01, L02>* LL02 = new Layer<Sigmoid, L01, L02>(LL01->Synaptogenisis());
	Layer<Sigmoid, L02, OUT>* LLTL = new Layer<Sigmoid, L02, OUT>(LL02->Synaptogenisis(), expect);

	/* Tie in to Output and Errors of Layers with Communication objects */
	const CoAr<L00>& errHD = LLHD->GetErrAry();
	const CoAr<L01>& err01 = LL01->GetErrAry();
	const CoAr<L02>& err02 = LL02->GetErrAry();
	const CoAr<OUT>& errTL = LLTL->GetErrAry();

	const CoAr<L00>& outHD = LLHD->GetOutAry();
	const CoAr<L01>& out01 = LL01->GetOutAry();
	const CoAr<L02>& out02 = LL02->GetOutAry();
	const CoAr<OUT>& outTL = LLTL->GetOutAry();

	/* Initialize with random numbers */
	LLHD->SetAllWeight(GetRaGCS, 0.02, (0.5 / INP));
	LL01->SetAllWeight(GetRaGCS, 0.02, (0.5 / L00));
	LL02->SetAllWeight(GetRaGCS, 0.02, (0.5 / L01));
	LLTL->SetAllWeight(GetRaGCS, 0.02, (0.5 / L02));

	LLHD->SetAllBiases(GetRaG, 0.01, 0);
	LL01->SetAllBiases(GetRaG, 0.01, 0);
	LL02->SetAllBiases(GetRaG, 0.01, 0);
	LLTL->SetAllBiases(GetRaG, 0.01, 0);

	/* Setup for User Input and UI */
	unsigned long int iterations;
	unsigned long int iter_total = 0;
	unsigned long int correct = 0;
	unsigned long int epoch = 0;
	double accuracy = 0.03;
	int perc = ((int) (accuracy * 100 + 0.5));
	int skips;
	int selection;
	bool was_selected[NT];
	double mu = 90;
	unsigned int time_step;
	int divisions = 16;

	while (true) {

		if (iter_total == 0) {

			for (int j = 0; j < NT; j++)
				was_selected[j] = false;
		}

		correct = 0;

		cout << "Enter number of times to run gradient descent on training inputs (0 to exit):  \n";
		while (! (cin >> iterations)) {
			CLRSCR;
			cin.clear(); // reset failbit
			cin.ignore(999, '\n');//skip bad input
			cout << "\rThat wasn't a very awesome number... Please try again :  ";
		}

		if (iterations == 0)
			break;

		if (iterations <= 50) {
			time_step = 500000u;
			skips = 1;
		}else if (iterations <= 500) {
			time_step = 80000u;
			skips = 1;
		}else if (iterations <= 1000) {
			time_step = 80000u;
			skips = 10;
		}else if (iterations <= 5000) {
			time_step = 80000u;
			skips = 10;
		}else if (iterations > 5000) {
			time_step = 80000u;
			skips = 1000;
		}else if (iterations > 20000) {
			time_step = 80000u;
			skips = 10000;
		}else if (iterations > 500000) {
			time_step = 80000u;
			skips = 50000;
		}

		for (unsigned int i = 0; i < iterations; i++) {

			iter_total++;

			if ( ( (iter_total) % divisions) == 0) {

				epoch++;

				for (int j = 0; j < NT; j++)
					was_selected[j] = false;
			}

			/* Run a Mini-Batch sort of Random values picked from array */
			for (int j = 0; j < (NT / divisions); j++) {

				selection = (int) (GetRaD(NT) + 1);

				while ( (was_selected[selection])) {

					selection++;

					if (selection >= NT)
						selection = 0;
				}

				was_selected[selection] = true;

				retina.SetFromAr(ti[selection]);
				expect.SetFromAr(ex[selection]);

				/* ORDERED - Apply Activation Function */
				LLHD->Z_Cascade();
				LLHD->Activation();
				LL01->Z_Cascade();
				LL01->Activation();
				LL02->Z_Cascade();
				LL02->Activation();
				LLTL->Z_Cascade();
				LLTL->Activation();

				/* ORDERED - Calculate Errors, each iteration adds it's error to a sum */
				LLTL->OutputErrorC();
				LLTL->ErrorCascade();
				LL02->ErrorCascade();
				LL01->ErrorCascade();
			}

			/* Apply Gradient Descent, the average error is begat from the sum of errors */
			LLHD->DescendGradient(mu, NT);
			LL01->DescendGradient(mu, NT);
			LL02->DescendGradient(mu, NT);
			LLTL->DescendGradient(mu, NT);

			for (int k = 0; k < OUT; k++) {
				correct =
							( (outTL.ary_[k] - expect.ary_[k]) < (accuracy) && (outTL.ary_[k] - expect.ary_[k])
																				> - (accuracy)) ?
																									(correct + 1) :
																									correct;
			}

			if (i % skips == 0 || i == (iterations - 1)) {

				/* Print Outputs */
				CLRSCR;

				cout << "╔══════════════════════════════════════════════════════════════════════════════╗\n";
				cout << "║    Retina: ";
				PrintAr(retina,MAGENTA);
				cout << setw(69 - INP * 7) << right << "║" << "\n║  Error 00: ";
				PrintArySci(errHD, CYAN);
				cout << setw(69 - L00 * 7) << right << "║" << "\n║  Layer 00: ";
				PrintAr(outHD, RED);
				cout << setw(69 - L00 * 7) << right << "║" << "\n║  Error 01: ";
				PrintArySci(err01, CYAN);
				cout << setw(69 - L01 * 7) << right << "║" << "\n║  Layer 01: ";
				PrintAr(out01, RED);
				cout << setw(69 - L01 * 7) << right << "║" << "\n║  Error 02: ";
				PrintArySci(err02, CYAN);
				cout << setw(69 - L02 * 7) << right << "║" << "\n║  Layer 02: ";
				PrintAr(out02, RED);
				cout << setw(69 - L02 * 7) << right << "║" << "\n║ Error OUT: ";
				PrintArySci(errTL, CYAN);
				cout << setw(69 - OUT * 7) << right << "║" << "\n║ Layer OUT: ";
				PrintArMatch(outTL, expect, YELLOW, GREEN, accuracy);
				cout << setw(69 - OUT * 7) << right << "║" << "\n║   Trainer: ";
				PrintAr(expect, MAGENTA);
				cout << setw(69 - OUT * 7) << right << "║" << "\n";

				cout << "╠══════════════════════════════════════╤═══════════════════════════════════════╣\n";
				cout << "║     Epoch (entire set): " << std::setw(12) << (epoch) <<" │";
				cout << "       Outputs Within " << perc << "%: " << std::setw(12) << (correct) << " ║\n";

				cout << "║  Batches Over All Time: " << std::setw(12) << (iter_total) << " │";
				cout << "    Percentage Within " << perc << "%: " << std::setw(12);
				cout << (correct * 100 / ( ((double)(i + 1)) * (double)OUT)) << " ║\n";

				cout << "║ Cascades Over All Time: " << std::setw(12);
				cout << ((unsigned long int)( ((double)iter_total + 1.0) * (double)NT / (double)divisions + 0.5));
				cout << " │";
				cout << "   Batches Done This Run: " << std::setw(12) << (i + 1) << " ║\n";

				cout << "║ Bat.Size/Bat. in Epcoh: "<< RED << std::setw(4) << (NT / divisions) << "  / ";
				cout << std::setw(4) << (divisions) << RESET << " │";
				cout << "  Batches Total This Run: " << RED << std::setw(12) << (iterations) << RESET " ║\n";
				cout << "╚══════════════════════════════════════╧═══════════════════════════════════════╝";
				cout << endl << endl;

				if (i != (iterations - 1)) {
					cout << endl;
					usleep(time_step);
				}
			}
		}
	}

	cout << "\nPress Enter to continue...";
	std::cin.ignore();

	delete LLHD;
	delete LL01;
	delete LLTL;

	DEBUG("Exiting Backpropagation Test\n");
}

/********************************************************************************************************************/

// Items marked with SPECIAL tag will refer to variables that will be varied randomly for the study of the effect
// on the outcome; or possibly as part of the gradient descention.
// TODO 8: Make Nuron type Gaussian
// TODO 8: Make Nuron type Multiquadratic
// TODO 8: Make Nuron type hyperbolic tangent
// TODO 6: Implement multithreading :) Get some use out of those 26 processors!!!
// TODO 4: Have another 'main()' compile this file at runtime using compile defines as arguments for template parms.
// TODO 3: Finite state machine to run main cycle. Sates: Learn, process, respond, yadda yadda...

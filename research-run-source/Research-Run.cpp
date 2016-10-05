/*
 * alan.cpp
 *
 *  Created on: Apr 15, 2015
 *      Author: paracite
 */
#include <ctime>
#include <iostream>
#include <cstdlib>
#include <string>
#include <iomanip>
#include <unistd.h>

#include "../DLog.h" // NOTICE: Must only be defined in main() .
#include "../Neuron.h"
#include "../NeuronTypes.h"
#include "../POSI.h"
#include "../Color.h"
#include "../Layer.h"
#include "../Utility.h"
#include "../NetTemplates.h"
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

	const int INP = 8; // Input
	const int L00 = 9;
	const int L01 = 7;
	const int L02 = 6;
	const int OUT = 5;

	/* Make new Communication objects We'll construct them with the fist training pair */
	CoAr<8> retina;
	CoAr<5> expect;

	/* Set first Training Set */
	retina.SetFromAr(ti[0]);
	expect.SetFromAr(ex[0]);

	/* Create a Neural Network From A Template */
	LayerBoss05<
			Sigmoid,
			INP,
			L00,
			L01,
			L02,
			OUT
	>* testNet = new LayerBoss05<
			Sigmoid,
			INP,
			L00,
			L01,
			L02,
			OUT
					>(retina, expect, 90.0);

	/* Tie in to Output of Layers with a Communication objects */
	const CoAr<OUT>& output = testNet->FeedForward(true);

	/* Setup for User Input and UI */
	double nu; // Learning factor.
	int batch_size; // How many items from a set of training data to train on per gradient descent.
	unsigned long int iterations; // Number of gradient descents on a run.
	unsigned long int iter_total = 0; // Total number of descents in all time.
	unsigned long int cascades = 0; // Cascades (Forward/Backward propagations) within the set of training data.
	unsigned long int cascades_total = 0; // Total number of cascades in all time.
	unsigned long int correct = 0; // Number of output digits within "accuracy" of expected value.
	unsigned long int epoch = 0; // Number of cycles through entire set of training data.
	unsigned long int skips; // Number of events to skip from outputting to console.
	double accuracy = 0.03; // Factor that outputs must be within of expected value to count as correct.
	int perc = ((int) (accuracy * 100 + 0.5)); // Accuracy factor as a percentage.
	int selection; // The selected index of the training data.
	bool was_selected[NT] = {false}; // An array that tracks if a selection has been made within the epoch.
	unsigned int time_step; // Time in microseconds to wait between screen prints.

	/* Start the Main Loop */
	while (true) {

		/* Collect user input */
		cout << "Enter number of times to run gradient descent on training inputs (0 to exit):  \n";
		while (! (cin >> iterations)) {
			cout << CLRSCR;
			cin.clear(); // reset failbit
			cin.ignore(999, '\n');//skip bad input
			cout << "\rThat wasn't a very awesome number... Please try again :  ";
		}
		/* Exits if 0 is entered */
		if (iterations == 0)
			break;
		cout << "Enter nu:  \n";
		while (! (cin >> nu)) {
			cout << CLRSCR;
			cin.clear(); // reset failbit
			cin.ignore(999, '\n');//skip bad input
			cout << "\rThat wasn't a very awesome number1	... Please try again :  ";
		}
		cout << "Enter mini batch size:  \n";
		while (! (cin >> batch_size) || batch_size > NT) {
			cout << CLRSCR;
			cin.clear(); // reset failbit
			cin.ignore(999, '\n');//skip bad input
			cout << "\rThat wasn't a very awesome number... Please try again :  ";
		}
		// Done Collecting Input.

		/* Set Refresh Time and Number of Iterations to Skip Between Displays */
		if (iterations <= 49) {
			time_step = 500000u;
			skips = 1u;
		}else if (iterations <= 200u) {
			time_step = 100000u;
			skips = 2u;
		}else if (iterations <= 2000u) {
			time_step = 100000u;
			skips = 20u;
		}else if (iterations <= 5000u) {
			time_step = 100000u;
			skips = 50u;
		}else if (iterations <= 20000u) {
			time_step = 100000u;
			skips = 200u;
		}else if (iterations <= 50000u) {
			time_step = 100000u;
			skips = 500u;
		}else if (iterations <= 200000u) {
			time_step = 100000u;
			skips = 2000u;
		}else /* if (iterations <= 500000u)*/ {
			time_step = 100000u;
			skips = 50000u;
		}
		// Done setting timing values.

		//Reset number of correct counts before beginning run.
		correct = 0;

		/* Begin Loop of User Defined Number of Descents */
		for (unsigned int i = 0; i < iterations; i++) {

			iter_total++;

			/* Run a Mini-Batch of Random values picked from array */
			for (int j = 0; j < batch_size; j++) {

				// Tracks number of per epoch, resets selected status when epoch is complete.
				cascades++;
				cascades_total++;
				if (cascades >= (unsigned) NT) {
					cascades = 0;
					epoch++;
					for (int j = 0; j < NT; j++)
						was_selected[j] = false;
					break;
				}

				// Random number used for making selection.
				selection = (int) (GetRaD(NT) + 1);

				// Choose next highest available number if selection has already been made.
				while ( (was_selected[selection])) {
					selection++;
					if (selection >= NT)
						selection = 0;
				}

				// Mark selection as used.
				was_selected[selection] = true;

				/* Set Input Data and Feed Forward */
				retina.SetFromAr(ti[selection]);
				testNet->FeedForward();

				/* Calculate Errors, each iteration adds it's error to a sum */
				expect.SetFromAr(ex[selection]);
				testNet->FeedBackward();
			} //Each FeedBackward event adds to a sum of errors for this mini-batch.

			/* Apply Gradient Descent, the average error is begot from the sum of errors */
			testNet->Update(nu, batch_size); // Mini-batch sum is cleared here.

			// Count the number of correct (within defined accuracy) responses.
			for (int k = 0; k < OUT; k++) {
				correct = ( (output.ary_[k] - expect.ary_[k]) < (accuracy)
							&& (output.ary_[k] - expect.ary_[k]) > (-accuracy)) ?
																					(correct + 1) :
																					correct;
			}

			// Print output only within defined frequency
			if (i % skips == 0u || i == (iterations - 1)) {

				/* Print Outputs */
				cout << CLRSCR;

				cout << "╔══════════════════════════════════════════════════════════════════════════════╗\n";
				cout << "║    Retina: ";
				PrintAr(retina,MAGENTA);
				cout << setw(69 - INP * 7) << right << "║" << "\n║  Error 00: ";
				PrintArySci(testNet->LLHD_.GetErrAry(), CYAN);
				cout << setw(69 - L00 * 7) << right << "║" << "\n║  Layer 00: ";
				PrintAr(testNet->LLHD_.GetOutAry(), RED);
				cout << setw(69 - L00 * 7) << right << "║" << "\n║  Error 01: ";
				PrintArySci(testNet->LL01_.GetErrAry(), CYAN);
				cout << setw(69 - L01 * 7) << right << "║" << "\n║  Layer 01: ";
				PrintAr(testNet->LL01_.GetOutAry(), RED);
				cout << setw(69 - L01 * 7) << right << "║" << "\n║  Error 02: ";
				PrintArySci(testNet->LL02_.GetErrAry(), CYAN);
				cout << setw(69 - L02 * 7) << right << "║" << "\n║  Layer 02: ";
				PrintAr(testNet->LL02_.GetOutAry(), RED);
				cout << setw(69 - L02 * 7) << right << "║" << "\n║ Error OUT: ";
				PrintArySci(testNet->LLTL_.GetErrAry(), CYAN);
				cout << setw(69 - OUT * 7) << right << "║" << "\n║ Layer OUT: ";
				PrintArMatch(output, expect, YELLOW, GREEN, accuracy);
				cout << setw(69 - OUT * 7) << right << "║" << "\n║   Trainer: ";
				PrintAr(expect, MAGENTA);
				cout << setw(69 - OUT * 7) << right << "║" << "\n";

				cout << "╠══════════════════════════════════════╤═══════════════════════════════════════╣\n";
				cout << "║     Epoch (entire set): " << std::setw(12) << (epoch) <<" │";
				cout << "      Within " << perc << "% This Run: " << std::setw(12) << (correct) << " ║\n";

				cout << "║  Batches Over All Time: " << std::setw(12) << (iter_total) << " │";
				cout << "    Percentage Within " << perc << "%: " << std::setw(12);
				cout << (correct * 100 / ( ((double)(i + 1)) * (double)OUT)) << " ║\n";

				cout << "║ Cascades Over All Time: " << std::setw(12);
				cout << (cascades_total);
				cout << " │";
				cout << "   Batches Done This Run: " << std::setw(12) << (i + 1) << " ║\n";

				cout << "║ Bat.Size/Bat. in Epcoh: "<< RED << std::setw(4) << (batch_size) << "  / ";
				cout << std::setw(4) << ((int)(NT / batch_size)) << RESET << " │";
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

	cout << CLRSCR<< "\nPress Enter to continue...";
	std::cin.ignore();

	delete testNet;

	DEBUG("Exiting Backpropagation Test\n");

}

/*********************************************************************************************************************/

// Items marked with SPECIAL tag will refer to variables that will be varied randomly for the study of the effect
// on the outcome; or possibly as part of the gradient descention.
// TODO 8: Make Nuro-type Gaussian
// TODO 8: Make Nuro-type Multiquadratic
// TODO 8: Make Nuro-type hyperbolic tangent
// TODO 6: Implement multithreading :) Get some use out of those 26 processors!!!
// TODO 4: Have another 'main()' compile this file at runtime using compile defines as arguments for template parms.
// TODO 3: Finite state machine to run main cycle. Sates: Learn, process, respond, yadda yadda...






































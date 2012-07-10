/*
 * Main.h
 *
 *  Created on: 9 Jul 2012
 *      Author: little-john
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


#include "Network.cpp"

class Main {
public:
	Main();
	virtual ~Main();
	double randInt();
	double rantMax(double max_tests);
	double calculate_mse(int test);
	double init_network();
	double set_networt_inputs(int test,double noise);
	double sqr(double input);
};

#define INPUT_NEURONS 	4
#define OUTPUT_NEURONS 	1
#define HIDDEN_NEURONS 	10
#define MAX_TESTS 1


#endif /* MAIN_H_ */

/*
 * Network.h
 *
 *  Created on: 5 Jul 2012
 *      Author: little-john
 */

#ifndef NETWORK_H_
#define NETWORK_H_

#include <iostream>
#include <fstream>
#include <string>

class Network {
public:
	Network();
	virtual ~Network();
	int DefineVars();
	double sigmoid(double number);
	double sigmoid_d(double number);
	void feed_forward(void);
	void backpropagate_error(int test);
#define INPUT_NEURONS 	35
#define OUTPUT_NEURONS 	10
#define HIDDEN_NEURONS 	10
};

#endif /* NETWORK_H_ */

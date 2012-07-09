/*
 * Network.h
 *
 *  Created on: 5 Jul 2012
 *      Author: little-john
 */

#ifndef NETWORK_H_
#define NETWORK_H_


#include <fstream>
#include <string>

class Network {
public:
	network();
	virtual ~network();
	int defineVars();
	double sigmoid(double number);
	double sigmoid_d(double number);
	void feed_forward(void);
	void backpropagate_error(int test);
};

#endif /* NETWORK_H_ */

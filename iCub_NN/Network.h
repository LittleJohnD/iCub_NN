/*
 * Network.h
 *
 *  Created on: 9 Jul 2012
 *      Author: little-john
 */

#ifndef NETWORK_H_
#define NETWORK_H_

#include <fstream>
namespace network {

class Network {
public:
	Network();
	virtual ~Network();
	int defineVars();
	int sigmoid(int number);
	int sigmoid_d(int number);
	void feed_forward(void);
	void backpropagate_error(int test);
};
#define INPUT_NEURONS 	4
#define OUTPUT_NEURONS 	1
#define HIDDEN_NEURONS 	10
#define MAX_TESTS 1
#define RHO 0.1
} /* namespace network */
#endif /* NETWORK_H_ */

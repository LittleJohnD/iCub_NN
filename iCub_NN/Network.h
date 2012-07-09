/*
 * Network.h
 *
 *  Created on: 9 Jul 2012
 *      Author: little-john
 */

#ifndef NETWORK_H_
#define NETWORK_H_

namespace network {

class Network {
public:
	Network();
	virtual ~Network();
	int defineVars();
	double sigmoid(double number);
	double sigmoid_d(double number);
	void feed_forward(void);
	void backpropagate_error(int test);
};

} /* namespace network */
#endif /* NETWORK_H_ */

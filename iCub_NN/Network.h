/*
 * Network.h
 *
 *  Created on: 9 Jul 2012
 *      Author: little-john
 */

#ifndef NETWORK_H_
#define NETWORK_H_

#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

class Network {
public:
	Network(int num_input, int num_hidden, int num_output);
	virtual ~Network();
	void init();
	void update(double *input_vector);
	void backpropagate_error(double teaching_input)/*Due to only one output(double *teaching_input)*/;
	inline void set_rho( double r){RHO = r;}
	inline void reset_meanSqrErr( void ){ meanSqrErr = 0.0;}
	inline double get_meanSqrErr( void ){ return meanSqrErr;}
	inline double* get_output( void ){return outputs;}

private:
	int INPUT_NEURONS;
	int OUTPUT_NEURONS;
	int HIDDEN_NEURONS;
	int MAX_TESTS;
	double RHO;
	double meanSqrErr;

	double *inputs;
	double *hidden;
	double *outputs;
	double *err_hid;
	double *err_out;

	double **w_h_i;
	double **w_o_h;
	double *w_h_b;
	double *w_o_b;

	//int defineVars();
	double sigmoid(double number);
	double sigmoid_d(double number);
};
#endif /* NETWORK_H_ */

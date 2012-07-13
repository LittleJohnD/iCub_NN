/*
 * Network.cpp
 *
 *  Created on: 9 Jul 2012
 *      Author: little-john
 */

#include "Network.h"


Network::Network(int num_input, int num_hidden, int num_output) {
	// TODO Auto-generated constructor stub
	INPUT_NEURONS = num_input;
	HIDDEN_NEURONS = num_hidden;
	OUTPUT_NEURONS = num_output;
	MAX_TESTS = 1;
	RHO = 0.1;
	inputs = new double [INPUT_NEURONS];
	hidden = new double [HIDDEN_NEURONS];
	outputs = new double [OUTPUT_NEURONS];
	w_h_i = new double *[HIDDEN_NEURONS];
	for (int i = 0; i < HIDDEN_NEURONS; i++)
		w_h_i[i] = new double [INPUT_NEURONS];
	w_o_h = new double *[OUTPUT_NEURONS];
	for (int i = 0; i <OUTPUT_NEURONS; i++)
	       w_o_h[i] = new double [HIDDEN_NEURONS];
	w_h_b = new double[HIDDEN_NEURONS];
	w_o_b = new double[OUTPUT_NEURONS];

	err_out = new double [OUTPUT_NEURONS];
	err_hid = new double [HIDDEN_NEURONS];
}

Network::~Network() {
	// TODO Auto-generated destructor stub
	delete[] inputs;
	delete[] hidden ;
	delete[] outputs;
	for (int i = 0; i < HIDDEN_NEURONS+1; i++)
		delete[] w_h_i[i];
	delete[] w_h_i;
	for (int i = 0; i <OUTPUT_NEURONS; i++)
	       delete[] w_o_h[i];
	delete[] w_o_h;
	delete[] w_h_b;
	delete[] w_o_b;

	delete[] err_out;
	delete[] err_hid;
}

void Network::init() {
	for (int h = 0; h < HIDDEN_NEURONS; h++){
		for (int i = 0; i < INPUT_NEURONS+1; i++){
			w_h_i[h][i] = rand()%1 - 0.5;
		}
		w_h_b[h] = rand()%1 - 0.5;
	}

	for(int out = 0; out < OUTPUT_NEURONS; out++){
		for(int h = 0; h < HIDDEN_NEURONS; h++){
			w_o_h[out][h] = rand() %1 - 0.5;
		}
		w_o_b[out] = rand()%1 - 0.5;
	}
}

void Network::update( double *input_vector)
{
	//calculate outputs for the hidden layer
	for(int h=0;h<HIDDEN_NEURONS;h++)
	{
		hidden[h]= w_h_b[h];
		for(int i=0;i<INPUT_NEURONS;i++)
		{
			hidden[h]+=(w_h_i[h][i]*input_vector[i]);
			printf("Hidden %d: %f\n",h,hidden[h]);
		}
		hidden[h]=sigmoid(hidden[h]);
	}
	//calculate outputs for the output layer
	for(int out=0;out<OUTPUT_NEURONS;out++)
	{
		outputs[out]=w_o_b[out];
		for(int h=0;h<HIDDEN_NEURONS;h++)
		{
			outputs[out]+=w_o_h[out][h]*hidden[h];
			printf("Output %d: %f\n",out,outputs[out]);
		}
		outputs[out]=sigmoid(outputs[out]);
	}
}

double Network::sigmoid(double number)
{
	//TODO define as 1/1+e^-t where t =function
	return 1.0/ 1 + exp(-(number));
}

double Network::sigmoid_d(double number)
{
	//TODO define as (1- y(x))y(x);
	return (1.0 - number) * number;
}

void Network::backpropagate_error(double teaching_input)/*Due to there only being one output (double *teaching_input)*/
{
	int out,hid,inp;

	//Compute the error for the output nodes
	for(out=0;out<OUTPUT_NEURONS;out++)
	{
		double tmp_err = (teaching_input-outputs[out]);/*Due to there only being one output (teaching_input[out]-outputs[out]);*/
		err_out[out] = tmp_err*sigmoid_d(outputs[out]);
		meanSqrErr += tmp_err *tmp_err;
	}
	//Compute the error for the hidden nodes
	for(hid=0;hid<HIDDEN_NEURONS;hid++)
	{
		err_hid[hid]=0.0;
		//Include error contribution for all output nodes
		for(out=0;out<OUTPUT_NEURONS;out++)
		{
			err_hid[hid]+=err_out[out]*w_o_h[out][hid];
		}
		err_hid[hid]*=sigmoid_d(hidden[hid]);
	}


	//Adjust the weights from the hidden to output layer
	for(out=0;out<OUTPUT_NEURONS;out++)
	{
		for(hid=0;hid<HIDDEN_NEURONS;hid++)
		{
			w_o_h[out][hid]+=RHO * err_out[out]*hidden[hid];
		}
	}
	//Adjust the weights from the input to the hidden layer
	for(hid=0;hid<HIDDEN_NEURONS;hid++)
	{
		for(inp=0;inp<INPUT_NEURONS;inp++)
		{
			w_h_i[hid][inp]+=RHO * err_hid[hid]*inputs[inp];
		}
	}
}

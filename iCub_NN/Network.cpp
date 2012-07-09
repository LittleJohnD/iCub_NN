/*
 * Network.cpp
 *
 *  Created on: 5 Jul 2012
 *      Author: little-john
 */

#include "Network.h"
using namespace std;

double inputs[INPUT_NEURONS +1];
double hidden[HIDDEN_NEURONS+1];
double outputs[OUTPUT_NEURONS+1];
double w_h_i[HIDDEN_NEURONS][INPUT_NEURONS+1];
double w_o_h[OUTPUT_NEURONS][INPUT_NEURONS+1];

Network::Network() {
	// TODO Auto-generated constructor stub

}

Network::~Network() {
	// TODO Auto-generated destructor stub
}

int Network::DefineVars()
{
	fstream sessionVars;
	sessionVars.open("/home/little-john/git/iCub_NN/iCub_NN/sessionVars.conf");
	if(sessionVars.is_open())
		cout<<"File loaded"<<endl;
	sessionVars.close();

	return 0;
}
double Network::sigmoid(double number)
{
	return number;
}
double Network::sigmoid_d(double number)
{
	return number;
}

void Network::feed_forward(void)
{
	int i,j;
	//calculate outputs for the hidden layer
	for(i=0;i<HIDDEN_NEURONS;i++)
	{
		hidden[i]=0.0;
		for(j=0;j<INPUT_NEURONS+1;j++)
		{
			hidden[i]+=(w_h_i[i][j]*inputs[j]);
		}
		hidden[i]=Network::sigmoid(hidden[i]);
	}
	//calculate ouptuts for the output layer
	for(i=0;i<OUTPUT_NEURONS;i++)
	{
		outputs[i]=0.0;
		for(j=0;j<HIDDEN_NEURONS+1;j++)
		{
			outputs[i]+=(w_o_h[i][j]*hidden[j]);
		}
		outputs[i]=Network::sigmoid(outputs[i]);
	}
}

void Network::backpropagate_error(int test)
{
	int out,hid,inp;
	double err_out[OUTPUT_NEURONS];
	double err_hid[HIDDEN_NEURONS];
	//Compute the error for the output nodes
	for(out=0;out<OUTPUT_NEURONS;out++)
	{
		err_out[out]=((double)tests[test].outputs[out]-outputs[out])*Network::sigmoid_d(outputs[out]);
	}
	//Compute the error for the hidden nodes
	for(hid=0;hid<HIDDEN_NEURONS;hid++)
	{
		err_hid=0.0;
		//Include error contribution for all output nodes
		for(out=0;out<OUTPUT_NEURONS;out++)
		{
			err_hid[hid]+=err_out[out]*w_o_h[out][hid];
		}
		err_hid[hid]*=Network::sigmoid_d(hidden[hid]);
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
	return;
}
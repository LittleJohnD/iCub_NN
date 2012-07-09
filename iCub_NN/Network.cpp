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

int DefineVars()
{
	fstream sessionVars;
	sessionVars.open("/home/little-john/git/iCub_NN/iCub_NN/sessionVars.conf");
	if(sessionVars.is_open())
		cout<<"File loaded"<<endl;
	sessionVars.close();

	return 0;
}
double sigmoid(double number)
{
	return number;
}

void feed_forward(void)
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
		hidden[i]=sigmoid(hidden[i]);
	}
	//calculate ouptuts for the output layer
	for(i=0;i<OUTPUT_NEURONS;i++)
	{
		outputs[i]=0.0;
		for(j=0;j<HIDDEN_NEURONS+1;j++)
		{
			outputs[i]+=(w_o_h[i][j]*hidden[j]);
		}
		outputs[i]=sigmoid(outputs[i]);
	}
}

void backpropagate_error(int test)
{

}


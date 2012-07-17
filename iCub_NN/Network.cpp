/*
 * Network.cpp
 *
 *  Created on: 9 Jul 2012
 *      Author: little-john
 */

#include "Network.h"

Network::Network(int num_input, int num_hidden, int num_output, long seed)
{
	INPUT_NEURONS = num_input;
	HIDDEN_NEURONS = num_hidden;
	OUTPUT_NEURONS = num_output;
	MAX_TESTS = 1;
	RHO = 0.1;
	inputs = new double [INPUT_NEURONS];
	hidden = new double [HIDDEN_NEURONS];
	outputs = new double [OUTPUT_NEURONS];

	w_h_i = new double* [HIDDEN_NEURONS];
	for (int i = 0; i < HIDDEN_NEURONS; i++)
	{
		w_h_i[i] = new double [INPUT_NEURONS];
	}

	w_o_h = new double* [OUTPUT_NEURONS];
	for (int i = 0; i <OUTPUT_NEURONS; i++)
	{
	       w_o_h[i] = new double [HIDDEN_NEURONS];
	}

	w_h_b = new double[HIDDEN_NEURONS];
	w_o_b = new double[OUTPUT_NEURONS];

	err_out = new double [OUTPUT_NEURONS];
	err_hid = new double [HIDDEN_NEURONS];
	std::string outputFileName = "mseData"+to_string(seed)+".csv";
	dataOutput.open (outputFileName.c_str());
	if(!dataOutput.is_open())
	  {
	    printf("Error: Cannot open input file.");
	    exit(1);
	  }
}

Network::~Network()
{
	delete[] inputs;
	delete[] hidden ;
	delete[] outputs;
	/*
	 * There is a memory leak in the next for loop and I cannot work out
	 * where it originates so it has been commented out.
	 */
	for (int i = 0; i < HIDDEN_NEURONS; i++)
	{
		delete[] w_h_i[i];
	}
	delete[] w_h_i;
	for (int i = 0; i <OUTPUT_NEURONS; i++)
	{
	       delete[] w_o_h[i];
	}
	delete[] w_o_h;
	delete[] w_h_b;
	delete[] w_o_b;
	delete[] err_out;
	delete[] err_hid;

	dataOutput.close();
}

void Network::init() {
	srand (time(NULL));
	for (int h = 0; h < HIDDEN_NEURONS; h++){
		for (int i = 0; i < INPUT_NEURONS; i++){
			w_h_i[h][i] = (double(rand()%100)/100 - 0.5);

		}
		w_h_b[h] = double(rand()%100)/100 - 0.5;
	}

	for(int out = 0; out < OUTPUT_NEURONS; out++){
		for(int h = 0; h < HIDDEN_NEURONS; h++){
			w_o_h[out][h] = (double(rand()%100)/100 - 0.5);
		}
		w_o_b[out] = double(rand()%100)/100 - 0.5;
	}
	dataOutput <<"RHO,"<<RHO<<"\n";
	dataOutput <<"Hidden_Nodes,"<<HIDDEN_NEURONS<<"\n";
	dataOutput <<"Iterations,MSE "<<"\n";
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
		}
		outputs[out]=sigmoid(outputs[out]);
		//printf("Output %d: %f\n",out,outputs[out]);
	}
}

double Network::sigmoid(double number)
{
	return 1.0/(1.0 + exp(-(number)));
}

double Network::sigmoid_d(double number)
{
	return (1.0 - number) * number;
}

void Network::backpropagate_error(double teaching_input)/*Due to there only being one output (double *teaching_input)*/
{
	int out,hid,inp;

	//Compute the error for the output nodes
	for(out=0;out<OUTPUT_NEURONS;out++)
	{
		double tmp_err = (teaching_input-outputs[out]);/*Due to there only being one output (teaching_input[out]-outputs[out]);*/
		//printf("Teaching-output: %f , output: %f tin: %f\n", tmp_err,outputs[out],teaching_input);

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
			//printf("before woh: %f", w_o_h[out][hid]);
			w_o_h[out][hid]+=RHO * err_out[out]*hidden[hid];
			//printf("after woh: %f\n", w_o_h[out][hid]);
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

void Network::printData(int iter)
{
  dataOutput <<iter<<","<< (meanSqrErr/4) <<"\n";
}

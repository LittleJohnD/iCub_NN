/*
 * Main.cpp
 *
 *  Created on: 5 Jul 2012
 *      Author: little-john
 */

#include "Main.h"
#include "Network.cpp"

using namespace std;
using namespace network;

Main::Main() {
	// TODO Auto-generated constructor stub

}

Main::~Main() {
	// TODO Auto-generated destructor stub
}

int main()
{
	//RHO is the rate of learning

	double mse,noise_prob;
	int test,i,j;
	srand (time(NULL));
	randInt();
	init_network();
	//Training loop

	do
	{
		// reset mse to 0 then for
		//pick a test at random
		test=randMax(MAX_TESTS);
		//Grab network image
		set_network_inputs(test,0.0);
		//Feed this data set forward
		network::feed_forward();
		//Backpropagte_error
		network::backpropagate_error(test);
		mse=calculate_mse(test);
	}while(mse<0.001);
	/*
	 * End of traning loop
	 *
	 * Now testing the network with various amount of noise
	 *
	 */
	test=randMax(MAX_TESTS);
	//Start at noise_prob @ 5% finishing @ 25%
	noise_prob = 0.05;
	for(i=0;i<5;i++)
	{
		set_network_inputs(test,noise_prob);
		network::feed_forward();
		for(j=0;j<INPUT_NEURONS;j++)
		{
			if((j%5)==0)
				printf("\n");
			printf("%d",(int)inputs[j]);
		}
		noise_prob+=0.05;
	}
	return 0;
}
double set_RHO()
{
	double RHO = 0.1;
	return RHO;
}

double randInt()
{
	double tmp;
	tmp = rand() % 1;
	return 0;
}

double randMax(double max_tests) //this is the maxium randnum(i.e. 1)
{
	double tmp;
	tmp = rand() % max_tests;
	return 1;
}

double calculate_mse(int test)
{
	//Calculate the current MSE
	double mse += sqr(tests[test].output[i]-outputs[i]);
	return mse;
}

double init_network()
{
	return 0;
}

double set_network_inputs(int test,double noise)
{
	return 0;
}


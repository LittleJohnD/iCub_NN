/*
 * Main.cpp
 *
 *  Created on: 5 Jul 2012
 *      Author: little-john
 */

#include "Main.h"
using namespace Network;
Main::Main() {
	// TODO Auto-generated constructor stub

}

Main::~Main() {
	// TODO Auto-generated destructor stub
}

int main(void)
{
	double mse,noise_prob;
	int test,i,j;
	// TODO RANDIT() init_network(), RANDMAX(), MAX_TESTS, set_network_inputs, calculate_mse(), classifier()
	RANDINT();
	init_network();
	//Training loop
	do
	{
		//pick a test at random
		test=RANDMAX(MAX_TESTS);
		//Grab network image
		set_network_inputs(test,0.0);
		//Feed this data set forward
		Network::feed_forward();
		//Backpropagte_error
		Network::backpropagate_error(test);
		//Calculate the current MSE
		mse=calculate_mse(test);
	}while(mse<0.001);
	/*
	 * End of traning loop
	 *
	 * Now testing the network with various amount of noise
	 *
	 */
	test=RANDMAX(MAX_TESTS);
	//Start at noise_prob @ 5% finishing @ 25%
	noise_prob= 0.05;
	for(i=0;i<5;i++)
	{
		set_network_inputs(test,noise_prob);
		Network::feed_forward();
		for(j=0;j<Network::INPUT_NEURONS;j++)
		{
			if((j%5)==0)
				printf("\n");
			printf("%d",(int)inputs[j]);
		}
		printf("\nclassified as %d\n\n",classifier());
		noise_prob+=0.05;
	}
	return 0;
}


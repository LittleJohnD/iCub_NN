/*
 * Main.cpp
 *
 *  Created on: 9 Jul 2012
 *      Author: little-john
 */

#include "Main.h"
using namespace std;
using namespace network;

namespace main
{

	Main::Main()
	{
		// TODO Auto-generated constructor stub
	}

	Main::~Main()
	{
		// TODO Auto-generated destructor stub
	}

	int main()
	{
		//RHO is the rate of learning
		double meanSqrErr,noise_prob;
		int test,i,j;
		srand (time(NULL));
		randInt();
		init_network();
		//Training loop

		do
		{
			// reset mse to 0 then for
			//pick a test at random
			test = randMax(MAX_TESTS);
			//Grab network image
			set_network_inputs(test,noise_prob);
			//Feed this data set forward
			feed_forward();
			//Backpropagte_error
			backpropagate_error(test);
			//the second value is put in to make it work TODO put in complete value
			meanSqrErr = calculate_mse(test);
		}while(meanSqrErr<0.001);
		/*
		 * End of traning loop
		 *
		 * Now testing the network with various amount of noise
		 *
		 */
		test = randMax(MAX_TESTS);
		//Start at noise_prob @ 5% finishing @ 25%
		noise_prob = 0.05;
		for(i=0;i<5;i++)
		{
			set_network_inputs(test,noise_prob);
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

	double randInt()
	{
		double tmp;
		tmp = rand() % 1;
		return 0;
	}

	double randMax(double max_tests) //this is the maximum randNum(i.e. 1)
	{
		int tmp;
		tmp = rand() % int(max_tests);
		return double(tmp);
	}

	double sqr(double input)
	{
		return (input * input);
	}

	double calculate_mse(int test, int output)
	{
		//Calculate the current MSE
		double meanSqrErr;
		meanSqrErr += sqr(1-outputs[output]);
		//tests[test].output[i]-outputs[i]);
		return meanSqrErr;
	}

	double init_network()
	{
		return 0;
	}

	double set_network_inputs(int test,double noise)
	{
		return 0;
	}

} /* namespace main */

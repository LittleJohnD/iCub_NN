/*
 * Main.cpp
 *
 *  Created on: 9 July 2012
 *      Author: little-john
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


#include "Network.h"

double input[4][2] ={
			{0.0, 0.0},
			{0.0, 1.0},
			{1.0, 0.0},
			{1.0, 1.0}};
double output[4] = {0.0, 0.0, 0.0, 1.0};
int sequence[4] = {0, 1, 2, 3};

int main(int argc, char** argv)
{
	srand (time(NULL));
	Network *net;
	net = new Network(2,3,1);

	net->set_rho(0.1);
	net->init();
	//load input output pairs;


	//RHO is the rate of learning
	//double meanSqrErr,noise_prob;
	//int test,i,j;
	//
	//randInt();
	//init_network();
	//Training loop

	do{
		//shuffle(sequence);
		net->reset_meanSqrErr();
		for(int i = 0; i < 4; i++){
			// reset mse to 0 then for
			//pick a test at random

			//test = int(randMax(MAX_TESTS));
			//Grab network image
			//set_network_inputs(test,noise_prob);
			//Feed this data set forward
			//n.feed_forward();
			//net->update(input[sequence[i]]);


			//Backpropagte_error
			//net->backpropagate_error(output[sequence[i]]);
			//the second value is put in to make it work TODO put in complete value
			//meanSqrErr += calculate_mse(test,1);
		}
	}while(net->get_meanSqrErr()/(4.0)<=0.001);
	delete net;
	/*
	 * End of traning loop
	 *
	 * Now testing the network with various amount of noise
	 *
	 */
//	test = randMax(MAX_TESTS);
//	//Start at noise_prob @ 5% finishing @ 25%
//	noise_prob = 0.05;
//	for(i=0;i<5;i++)
//	{
//		set_network_inputs(test,noise_prob);
//		for(j=0;j<INPUT_NEURONS;j++)
//		{
//			if((j%5)==0)
//				printf("\n");
//			printf("%d",(int)inputs[j]);
//		}
//		noise_prob+=0.05;
//	}
	return 0;
}

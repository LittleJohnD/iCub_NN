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
//void shuffle(int* array,int size);

void shuffle(int array[],int size)
{
	srand (time(NULL));
	int tmpAry[size];
	int tmpNum;
	int usedNum[size];
	for(int i=0;i<size;i++)
		tmpAry[i] = array[i];

	for(int j=0;j<size;j++)
	{
		randloop:
		tmpNum = rand() % size;
		for(int x=0;x<j;x++)
		{
			if(tmpNum==usedNum[x])
				goto randloop;

		}
		usedNum[j] = tmpNum;
	}
	delete[]tmpAry;
	delete[]usedNum;

}

int main(int argc, char** argv)
{
	/*
	 * To seed this I just need to change the value in srand to a value and that will create a seed,
	 * This can be done with a random number or given value.
	 */
	srand (time(NULL));
	Network *net;
	net = new Network(2,3,1);

	net->set_rho(0.1);
	net->init();

	//Training loop
	do
	{
		//Randomise input and reset MSE
		shuffle(sequence,4);
		net->reset_meanSqrErr();
		for(int i = 0; i < 4; i++)
		{
			printf("Sequence: %d\n",sequence[i]);
			//Feed this data set forward;
			net->update(input[sequence[i]]);
			//Backpropagte_error
			net->backpropagate_error(output[sequence[i]]);
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

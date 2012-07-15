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
double desiredOutput[4] = {0.0, 0.0, 0.0, 1.0};
int * sequence;

void shuffle(int* array,int size)
{
	/*
	 * To seed this I just need to change the value in srand() to a value and that will create a seed,
	 * This can be done with a random number or given value.
	 */
	srand (time(NULL));
	int* tmpAry;
	int tmpNum;
	int usedNum[size];
	tmpAry = new int[size];
	for(int i=0;i<size;i++)
	{
		randloop:
		tmpNum = rand() % size;
		for(int j=0;j<i;j++)
		{
			if(tmpNum==usedNum[j])
				goto randloop;
		}
		usedNum[i] = tmpNum;
	}
	for(int x=0;x<size;x++)
	{
		sequence[x]=usedNum[x];
		//printf("Sequence: %d\n",sequence[x]);
	}
	delete[]tmpAry;
	//delete[]usedNum;
}

int main(int argc, char** argv)
{

	Network *net;
	net = new Network(2,3,1);
	sequence = new int[4];
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
			//Feed this data set forward;
			net->update(input[sequence[i]]);
			//Backpropagte_error
			net->backpropagate_error(desiredOutput[sequence[i]]);
		}
		printf("MSE: %f\n",net->get_meanSqrErr());
	}while((net->get_meanSqrErr()/4.0)>=0.001);
	delete net;
	delete[] sequence;
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

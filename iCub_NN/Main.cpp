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
	net->set_rho(0.75);
	net->set_mSEBound(0.01);
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
		//printf("MSE: %f\n",net->get_meanSqrErr()/4);
	}while((net->get_meanSqrErr()/4.0)>=net->get_mSEBound());

	/*
	 * End of traning loop
	 *
	 * Now testing the network
	 *
	 */
	shuffle(sequence,4);
	for(int t=0;t<4;t++)
	{
		net->update(input[sequence[t]]);
		printf("Desired output: %f ", desiredOutput[sequence[t]]);
		printf("Output: %f\n",net->get_output()[0]);
	}
	delete[] sequence;
	delete net;
	return 0;
}

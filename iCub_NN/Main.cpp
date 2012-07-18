/*
 * Main.cpp
 *
 *  Created on: 9 July 2012
 *      Author: little-john
 */

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string>
#include <vector>

#include "Network.h"
#include "ReadInput.h"
#include "Normalise.h"

using namespace std;

vector< vector<double> > input;
vector< vector<double> > output;
double eyeAngles[5];
double motorAngles[4];
double desiredOutput[4] = {0.0, 0.0, 0.0, 1.0};
int * sequence;
int iterations;
long seed = 42949;
int vectSize=0;
int trainingVectSize=0;

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

	printf("Initiating training\n");
	Network *net;
	ReadInput *readIn;
	readIn = new ReadInput();
	readIn->readInInputFromFile("joint_space.xml",input,output);
	net = new Network(3,10,9,seed);

	sequence = new int[input.size()];
	net->set_rho(0.5);
	net->set_mSEBound(0.001);
	net->init();
	iterations=0;
	//Training loop

	vectSize= input.size();
	trainingVectSize = vectSize * 0.75;
	vectSize -= trainingVectSize;
	do
	{
          //Randomise input and reset MSE
          shuffle(sequence,vectSize);
          net->reset_meanSqrErr();
          for(int i = 0; i < vectSize; i++)
          {
                  //Feed this data set forward;
                  net->update(input,sequence[i]);
                  //Backpropagte_error
                  net->backpropagate_error(output,sequence[i]);
                  //printf("Print instead of back_prop\n");
          }
          iterations++;
          if(iterations%5000==0)
            net->printData(iterations,vectSize);
          if(iterations%500000==0)
            printf("\n %f \n",net->get_meanSqrErr());
          else if(iterations%5000==0)
            printf(".");

	}while((net->get_meanSqrErr()/double(vectSize))>=net->get_mSEBound());
	net->printData(iterations,vectSize);
	delete[] sequence;
	printf("\nTraining complete\n");
	/*
	 * End of traning loop
	 *
	 * Now testing the network
	 *
	 */
	printf("Initiating testing...\n");
	for(int t=0;t<trainingVectSize;t++)
	{
		net->update(input,(vectSize+t));
		printf("Desired output: ");
		for(int d= 0;d<(output[vectSize+t].size());d++)
		  {
		    printf(" %f ", output[vectSize+t][d]);
		    printf("Output: %f\n",net->get_output()[d]);
		  }
		printf("\n\n\n");
	}
	printf("Testing complete\n");
	delete net;
	return 0;
}

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

#include "Network.h"

double input[4][3] ={
			{0.0, 0.0, 0.0},
			{0.0, 1.0, 0.0},
			{1.0, 0.0, 0.0},
			{1.0, 1.0, 0.0}};
double eyeAngles[5];
double motorAngles[4];
double desiredOutput[4] = {0.0, 0.0, 0.0, 1.0};
int * sequence;
int iterations;
long seed = 42949;
std::ifstream inputFile;
char linkOpen[7],inputNodeOpen[27],rightEyeTilt[58],rightEyePan[58];
char leftEyeTilt[58],leftEyePan[58],vergence[58],inputNodeClose[20];
char outputNodeOpen[28],motorAngle0[59],motorAngle1[59],motorAngle2[59];
char motorAngle3[59],outputNodeClose[21],linkAtOpen[26],rotMatrix1[61];
char rotMatrix2[61],rotMatrix3[61],linkAtClose[19],linkClose[8],emptyLn[1];

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
void readInInputFromFile(std::string inputFileName)
{
  inputFile.open(inputFileName.c_str());
  if(inputFile.is_open())
    {
      inputFile.getline(linkOpen,7);

      inputFile.getline(inputNodeOpen,27);
      inputFile.getline(rightEyeTilt,58);
      inputFile.getline(rightEyePan,58);
      inputFile.getline(leftEyeTilt,58);
      inputFile.getline(leftEyePan,58);
      inputFile.getline(vergence,58);
      inputFile.getline(inputNodeClose,20);

      inputFile.getline(outputNodeOpen,28);
      inputFile.getline(motorAngle0,59);
      inputFile.getline(motorAngle1,59);
      inputFile.getline(motorAngle2,59);
      inputFile.getline(motorAngle3,59);
      inputFile.getline(outputNodeClose,21);

      inputFile.getline(linkAtOpen,26);
      inputFile.getline(rotMatrix1,61);
      inputFile.getline(rotMatrix2,61);
      inputFile.getline(rotMatrix3,61);
      inputFile.getline(linkAtClose,19);

      inputFile.getline(linkClose,8);
      inputFile.getline(emptyLn,1);
    }
  else
    {
      printf("Error: Cannot open input file.");
      exit(1);
    }
  //TODO parse XML
  for(int i=0;i<=58;i++)
    {
      switch(rightEyeTilt[i])
      {
        case '\"':
          double tmpNumber;
          if(rightEyeTilt[i+1]=='-')
            {
              eyeAngles[0]-=0.000001;
            }
          for(int a=0;a<8;a++)
            {
              tmpNumber+= (rightEyeTilt[i+a+2]/(10*a));
            }
          eyeAngles[0]+= tmpNumber;
          eyeAngles[0]+=0.000001;
          printf("Right eye Tilt: %f", eyeAngles[0]);
          break;
      }
    }


}

int main(int argc, char** argv)
{

	printf("Initiating training\n");
	Network *net;
	net = new Network(2,3,1,seed);
	sequence = new int[4];
	net->set_rho(0.5);
	net->set_mSEBound(0.001);
	net->init();
	iterations=0;
	//Training loop
	readInInputFromFile("~/Documents/iCub/Programs/elio_sim/my_icub/build/joint_space.xml");
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
		iterations++;
		if(iterations%5000==0)
		  net->printData(iterations);
		if(iterations%500000==0)
		  printf("\n");
		else if(iterations%5000==0)
		  printf(".");
	}while((net->get_meanSqrErr()/4.0)>=net->get_mSEBound());
	net->printData(iterations);
	delete[] sequence;
	printf("\nTraining complete\n");
	/*
	 * End of traning loop
	 *
	 * Now testing the network
	 *
	 */
	printf("Initiating testing...\n");
	int tmpAry[] = {0,1,2,3};
	for(int t=0;t<4;t++)
	{
		net->update(input[tmpAry[t]]);
		printf("Desired output: %.0f ", desiredOutput[tmpAry[t]]);
		printf("Output: %f\n",net->get_output()[0]);
	}
	printf("Testing complete\n");
	delete net;
	return 0;
}

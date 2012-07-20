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

#include "tinyxml.h"

#include "Network.h"

using namespace std;

vector< vector<double> > input;
vector< vector<double> > output;
vector< vector<double> > minMaxInput(10,vector<double>(2,0));
vector< vector<double> > minMaxOutput(10,vector<double>(2,0));
double eyeAngles[5];
double motorAngles[4];
double desiredOutput[4] = {0.0, 0.0, 0.0, 1.0};
int * sequence;
int iterations;
long seed = 42949;
int vectSize=0;
int trainingVectSize=0;

void readInInputFromFile( void )
{
  TiXmlDocument jSpace("joint_space.xml");
  string elemName;
  TiXmlElement* elem;
  const char* attr;
  int count =0;
  vector<double>tmpVector;

  printf("Read in\n");
  if(jSpace.LoadFile())
    {
      for(TiXmlNode* link = jSpace.FirstChildElement();link!=NULL;link = link->NextSibling ())
        {
          if(link == NULL)
            {
                printf("Failed to load file: No root element.\n");
                jSpace.Clear();
            }
            for(elem = link->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
            {
              elemName = elem->Value();
              if(elemName == "InputNode")
                {
                  tmpVector.clear();
                  for(TiXmlElement* e = elem->FirstChildElement("Component"); e != NULL; e = e->NextSiblingElement("Component"))
                    {
                      attr = e->Attribute("Value");
                      if(attr != NULL)
                        {
                          tmpVector.push_back(atof(attr));
                          //printf("Component : %s\n",attr); // Do stuff with it
                        }
                    }
                  for(TiXmlElement* e = elem->FirstChildElement("Component"); e != NULL; e = e->NextSiblingElement("Component"))
                    {
                      attr = e->Attribute("Value");
                      if(attr != NULL)
                        {
                          tmpVector.push_back(atof(attr));
                         // Do stuff with it
                        }
                    }

                  output.push_back(tmpVector);
                }
              else if(elemName == "LinkAttr")
                {
                  count++;
                  tmpVector.clear();
                  for(TiXmlElement* e = elem->FirstChildElement("LinkAttrValue"); e != NULL; e = e->NextSiblingElement("LinkAttrValue"))
                  {
                      attr = e->Attribute("LinkValue");
                      if(attr != NULL)
                        {

                          tmpVector.push_back(double(atof(attr)));
                          //printf("Component : %s\n",attr); // Do stuff with it
                        }
                  }
                  input.push_back(tmpVector);
                }
              else if(!(elemName == "OutputNode"))
                printf("Error reading in File\n");
            }
        }

    }
  else
    printf("Failed to load file\n");
  jSpace.Clear();
  printf("Count: %d\n",count);
  printf("Input Vector size: %d\n", input.size());
  //printf("Input Vector first number %f  last number: %f\n", input_vector.front(),input_vector.back());
  printf("Output Vector size: %d\n", output.size());
}


void findMinMax(string io)
{
  printf("MinMax %s start.. \n",io.c_str());
  if(io=="input")
    {
      for(unsigned int i=0;i<input.size();i++)
        {
          printf("loop 1 start.. \n");
          for(unsigned int j=0;j<(input[i].size());j++)
            {
              printf("loop 2 start.. \n");
              if(input[i][j]<minMaxInput[j][0])
                {
                  printf("Less than.. \n");
                  minMaxInput[j][0]=input[i][j];
                }
              else if(input[i][j]>minMaxInput[j][1])
                {
                  printf("More than.. \n");
                  minMaxInput[j][1]=input[i][j];
                }
            }
        }
    }
  else if(io == "output")
    {
      for(unsigned int i=0;i<output.size();i++)
        {
          printf("loop 1 start.. \n");
          for(unsigned int j=0;j<(output[i].size());j++)
            {
              printf("loop 2 start.. \n");
              if(output[i][j]<minMaxOutput[j][0])
                {
                  printf("Less than.. \n");
                  minMaxOutput[j][0]=output[i][j];
                }
              else if(output[i][j]>minMaxOutput[j][1])
                {
                  printf("More than.. \n");
                  minMaxOutput[j][1]=output[i][j];
                }
            }
        }
    }
  else
    {
      cerr<<"Error finding minMax"<<endl;
    }

}
void normaliseData(string io)
{
  if(io == "input")
    {
      for(unsigned int i= 0;i<input.size();i++)
        {
          for(unsigned int j = 0;j<input[i].size();j++)
            {
              input[i][j]=((input[i][j]-minMaxInput[j][0])/(minMaxInput[j][1]-minMaxInput[j][0]));
            }
        }
    }
  else if(io == "output")
    {
      for(unsigned int i= 0;i<output.size();i++)
        {
          for(unsigned int j = 0;j<output[i].size();j++)
            {
              output[i][j]=((minMaxOutput[i][j]-minMaxOutput[j][0])/(minMaxOutput[j][1]-minMaxOutput[j][0]));
            }
        }
    }
  else
    {
      cerr<<"Error Normalising Data"<<endl;
    }

}
void unNormaliseData(string io)
{
  if(io=="input")
    {
      for(unsigned int i= 0;i<input.size();i++)
        {
          for(unsigned int j = 0;j<input[i].size();j++)
            {
              input[i][j]=((input[i][j]+minMaxInput[j][0])*(minMaxInput[j][1]-minMaxInput[j][0]));
            }
        }
    }
  else if(io == "output")
    {
      for(unsigned int i= 0;i<output.size();i++)
        {
          for(unsigned int j = 0;j<output[i].size();j++)
            {
              output[i][j]=((output[i][j]+minMaxOutput[j][0])*(minMaxOutput[j][1]-minMaxOutput[j][0]));
            }
        }
    }
  else
    {
      cerr<<"Error Unnormalising Data"<<endl;
    }


}

void processToNormaliseData()
  {
    printf("Normal start.. \n");
    findMinMax("input");
    printf("..min max input.. \n");
    normaliseData("input");
    printf("..normalised input.. \n");
    findMinMax("output");
    printf("..min max output.. \n");
    normaliseData("output");
    printf("..normalised output.. \n");
    printf("..normalise complete\n");
  }
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
	readInInputFromFile();
	cout<<"Data read in"<< endl;
	//normaliseData(input,output);
	cout<< "Data Normalised"<<endl;
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

/*
 * Main.cpp
 *
 *  Created on: 9 July 2012
 *      Author: little-john
 */

#include <iostream>
#include <algorithm>
#include <functional>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "tinyxml.h"

#include "Network.h"

using namespace std;

vector< vector<double> > input;
vector< vector<double> > output;
vector< vector<double> > evaluationSet;
vector< vector<double> > minMaxInput(3,vector<double>(2));
vector< vector<double> > minMaxOutput(8,vector<double>(2));
double eyeAngles[5];
double motorAngles[4];
int * sequence;
int iterations;
long seed = 42949;
int vectSize;
int evaluationVectSize;

void readInInputFromFile( void )
{
  TiXmlDocument jSpace("joint_space.xml");
  string elemName;
  TiXmlElement* elem;
  const char* attr;
  int count =0;
  vector<double>tmpInputVector;
  vector<double>tmpOutputVector;

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
                  tmpOutputVector.clear();
                  for(TiXmlElement* e = elem->FirstChildElement("Component"); e != NULL; e = e->NextSiblingElement("Component"))
                    {
                      attr = e->Attribute("Value");
                      if(attr != NULL)
                        {
                          tmpOutputVector.push_back(atof(attr));
                          //printf("Component : %s\n",attr); // Do stuff with it
                        }
                    }

                }
              else if(elemName == "OutputNode")
                {
                  for(TiXmlElement* e = elem->FirstChildElement("Component"); e != NULL; e = e->NextSiblingElement("Component"))
                    {
                      attr = e->Attribute("Value");
                      if(attr != NULL)
                        {
                          tmpOutputVector.push_back(atof(attr));
                         // Do stuff with it
                        }
                    }

                  output.push_back(tmpOutputVector);
                }
              else if(elemName == "LinkAttr")
                {
                  count++;
                  tmpInputVector.clear();
                  for(TiXmlElement* e = elem->FirstChildElement("LinkAttrValue"); e != NULL; e = e->NextSiblingElement("LinkAttrValue"))
                  {
                      attr = e->Attribute("LinkValue");
                      if(attr != NULL)
                        {

                          tmpInputVector.push_back(double(atof(attr)));
                          //printf("Component : %s\n",attr); // Do stuff with it
                        }
                  }
                  input.push_back(tmpInputVector);
                }
              else
                printf("Error reading in File\n");
            }
        }

    }
  else
    printf("Failed to load file\n");
  jSpace.Clear();
//  printf("Count: %d\n",count);
//  printf("Input Vector size: %d\n", input.size());
//  cout<<"Number of inputs: "<<input[0].size()<<" Number of outputs: "<<output[0].size()<<endl;
//  printf("Output Vector size: %d\n", output.size());
//  for(unsigned int n =0;n<input.size();n++)
//    {
//      printf("Input Vector[%d]:\n",n);
//      for(unsigned int i=0;i<input[n].size();i++)
//        printf("\t %d: %f\t",i, input[n][i]);
//      printf("\n");
//      printf("Output Vector[%d]:\n",n);
//      for(unsigned int o=0;o<output[n].size();o++)
//        printf("\t %d: %f\t",o,output[n][o]);
//      printf("\n");
//    }
}

void findMinMax(string io)
{
  if(io=="input")
    {
      for(unsigned int i=0;i<input.size();i++)
        {
          for(unsigned int j=0;j<(input[i].size());j++)
            {

              if(input[i][j]>minMaxInput[j][1])
                minMaxInput[j][1]=input[i][j];
              else if(input[i][j]<minMaxInput[j][0])
                minMaxInput[j][0]=input[i][j];
            }
          if(i==0)
            {
              for(unsigned int k=0;k<(input[i].size());k++)
                minMaxInput[k][0]=minMaxInput[k][1];
            }
        }
    }
  else if(io == "output")
    {
      for(unsigned int i=0;i<output.size();i++)
        {
          for(unsigned int j=0;j<(output[i].size());j++)
            {
              if(output[i][j]>minMaxOutput[j][1])
                minMaxOutput[j][1]=output[i][j];
              else if(output[i][j]<minMaxOutput[j][0])
                minMaxOutput[j][0]=output[i][j];
            }
          if(i==0)
            {
              for(unsigned int k=0;k<(output[i].size());k++)
                minMaxOutput[k][0]=minMaxOutput[k][1];
            }
        }
    }
  else
    cerr<<"Error finding minMax"<<endl;

}
void normaliseData(string io)
{
  if(io == "input")
    {
      for(unsigned int i= 0;i<input.size();i++)
        {
          for(unsigned int j = 0;j<input[i].size();j++)
            {
              if(minMaxInput[j][0]!=minMaxInput[j][1])
                input[i][j]=((input[i][j] - minMaxInput[j][0]) / (minMaxInput[j][1] - minMaxInput[j][0]));
              else
                input[i][j]=0;
            }
        }
    }
  else if(io == "output")
    {
      for(unsigned int i= 0;i<output.size();i++)
        {
          for(unsigned int j = 0;j<output[i].size();j++)
            {
              if(minMaxOutput[j][0]!=minMaxOutput[j][1])
                output[i][j]=((output[i][j] - minMaxOutput[j][0]) / (minMaxOutput[j][1] - minMaxOutput[j][0]));
              else
                output[i][j]=0;
            }
        }
    }
  else
    cerr<<"Error Normalising Data"<<endl;

}
void unNormaliseData(string io)
{
  if(io=="input")
    {
      for(unsigned int i= 0;i<input.size();i++)
        {
          for(unsigned int j = 0;j<input[i].size();j++)
            {
              if(minMaxInput[j][0]!=minMaxInput[j][1])
                input[i][j]=((input[i][j] * (minMaxInput[j][1] - minMaxInput[j][0])) + minMaxInput[j][0]);
              else
                input[i][j]=minMaxInput[j][1];
            }
        }
    }
  else if(io == "output")
    {
      for(unsigned int i= 0;i<output.size();i++)
        {
          for(unsigned int j = 0;j<output[i].size();j++)
            {
              if(minMaxOutput[j][0]!=minMaxOutput[j][1])
                {
                  //cout<<"Norm:\t"<<output[i][j];
                  output[i][j]=((output[i][j] * (minMaxOutput[j][1] - minMaxOutput[j][0])) + minMaxOutput[j][0]);
                  //cout<<"\t Unnorm:\t"<<output[i][j]<<endl;
                }
              else
                output[i][j]=minMaxOutput[j][1];
            }
        }
    }
  else
    cerr<<"Error Unnormalising Data"<<endl;
}

void processToNormaliseData()
  {
    findMinMax("input");
    normaliseData("input");
    findMinMax("output");
    normaliseData("output");
  }
void extractTrainningData(void)
{
  random_shuffle(sequence,(sequence+vectSize));
  for(int d=(vectSize-evaluationVectSize);d<vectSize;d++)
    {
      vector<double> tmpVect;
      for(unsigned int i = 0; i< (input[sequence[d]].size()) ;i++)
        {
          tmpVect.push_back(input[sequence[d]][i]);
        }
      for(unsigned int j = 0;j<output[sequence[d]].size();j++)
        {
          tmpVect.push_back(output[sequence[d]][j]);
        }
      evaluationSet.push_back(tmpVect);
    }
}

void shuffle(int* array,int size,long seedNum)
{
        /*
         * To seed this I just need to change the value in srand() to a value and that will create a seed,
         * This can be done with a random number or given value.
         */
        int* tmpAry;
        int tmpNum;
        int usedNum[size];
        tmpAry = new int[size];
        for(int i=0;i<size;i++)
        {
                tmpNum = rand() % size;
                for(int j=0;j<i;j++)
                {
                        if(tmpNum==usedNum[j])
                          break;
                }
                usedNum[i] = tmpNum;
        }
        for(int x=0;x<size;x++)
        {
//            sequence[x]=x;
            sequence[x]=usedNum[x];
                //printf("Sequence: %d\n",sequence[x]);
        }
        delete[]tmpAry;
        //delete[]usedNum;
}

int main(int argc, char** argv)
{
  srand(time(NULL));
  printf("Initiating training\n");
	Network *net;
	readInInputFromFile();
	processToNormaliseData();
	net = new Network(3,8,8,seed);
	sequence = new int[input.size()];
	for(unsigned int x=0;x<input.size();x++)
	  sequence[x]=x;
	net->set_rho(0.1);
	net->set_mSEBound(0.05);
	net->set_evaluationSize(0.5);
	net->init();
	iterations=0;
	//Training loop
	vectSize= input.size();
	evaluationVectSize = vectSize * net->get_evaluationSize();
	extractTrainningData();
	vectSize -= evaluationVectSize;
	net->set_MSE(0.05);
	//output to data file and then reload in.
	do
	{
	  //Randomise input and reset MSE
	    net->reset_meanSqrErr();
	    random_shuffle(sequence,(sequence + vectSize));
//	    for(unsigned int x=0;x<vectSize;x++)
//	      cout<<sequence[x]<<" ";
//	    cout<<endl;
	    //cout<<"tmp_err: ";
          for(int i = 0; i < vectSize; i++)
          {
              //Feed this data set forward;
              net->update(input[sequence[i]]);
              //Backpropagte_error
              net->backpropagate_error(output[sequence[i]]);
              //tmpCount++;
          }
         // cout<<endl;
          //iterations++;
//          if(iterations%5000==0)
//            net->printData(iterations,vectSize);
//          if(iterations%500000==0)
            printf("MSE:\t%f\n",(net->get_meanSqrErr() / double(vectSize)));
            //net->update_meanSqrErr(net->get_meanSqrErr());
//          else if(iterations%5000==0)
//            printf(".");

	}while((net->get_meanSqrErr() / double(vectSize)) > net->get_mSEBound());//while(net->update_meanSqrErr(net->get_meanSqrErr())); //
	net->printData(iterations,vectSize);
	delete[] sequence;
	printf("\nTraining complete\n");
	/*
	 * End of training loop
	 *
	 * Now testing the network
	 *
	 */
	printf("Initiating testing...\n");
	for(int t=0;t<evaluationVectSize;t++)
	{
		net->update(input[(vectSize+t)]);
	}
	unNormaliseData("output");
	//restoreData("output");
	for(int c=0;c<evaluationVectSize;c++)
	    {
	      printf("Desired output[%d]:\n",c);
	      for(int d= 0;d<(output[vectSize+c].size());d++)
                {
                  printf("\t %f ", output[vectSize+c][d]);
                  printf("\t\t Output: %f\n",((net->get_output()[d] *  (minMaxOutput[d][1] - minMaxOutput[d][0])) + minMaxOutput[d][0]));
                }
              printf("\n\n\n");
	    }
	printf("Testing complete\n");
	delete net;
	return 0;
}

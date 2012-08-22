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
vector< vector<double> > evaluationInputSet;
vector< vector<double> > evaluationOutputSet;
vector< vector<double> > minMaxInput(3 , vector < double >(2));
vector< vector<double> > minMaxOutput(8 , vector < double >(2));
int * sequence;
int iterations;
double seed = 42949;
int vectSize;
int evaluationVectSize;

void readInInputFromFile( void )
{
  TiXmlDocument jSpace("joint_space.xml");
  string elemName;
  TiXmlElement* elem;
  const char* attr;
  int count =0;
  vector< double >tmpInputVector;
  vector< double >tmpOutputVector;
  ofstream cartData;
  cartData.open ("cartData.dat");
  cartData<<"#This is the data file showing the cartesian coordinates"<<endl<<"#X \t Y \t Z"<<endl;
  if(jSpace.LoadFile() && cartData.is_open())
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
                          cartData << double(atof(attr))<<"\t";
                        }
                  }
                  input.push_back(tmpInputVector);
                  cartData<<endl;
                }
              else
                printf("Error reading in File\n");
            }
        }
    }
  else
    printf("Failed to load file\n");
  jSpace.Clear();
  cartData.close();
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
                {
                  double tmpInt=0.0;
                  tmpInt=minMaxInput[k][0];
                  minMaxInput[k][0]=minMaxInput[k][1];
                  minMaxInput[k][1]=tmpInt;
                }
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
                {
                  double tmpInt =0.0;
                  tmpInt=minMaxOutput[k][0];
                  minMaxOutput[k][0]=minMaxOutput[k][1];
                  minMaxOutput[k][1]=tmpInt;
                }
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
                  output[i][j]=((output[i][j] * (minMaxOutput[j][1] - minMaxOutput[j][0])) + minMaxOutput[j][0]);
                }
              else
                output[i][j]=minMaxOutput[j][1];
            }
        }
      for(unsigned int i= 0;i<evaluationOutputSet.size();i++)
        {
          for(unsigned int j = 0;j<evaluationOutputSet[i].size();j++)
            {
              if(minMaxOutput[j][0]!=minMaxOutput[j][1])
                {
                  evaluationOutputSet[i][j]=((evaluationOutputSet[i][j] * (minMaxOutput[j][1] - minMaxOutput[j][0])) + minMaxOutput[j][0]);
                }
              else
                evaluationOutputSet[i][j]=minMaxOutput[j][1];
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
      vector< double > tmpEvalVect;
      for(unsigned int i = 0; i< (input[sequence[d]].size()) ;i++)
        {
          tmpEvalVect.push_back(input[sequence[d]][i]);
        }
      evaluationInputSet.push_back(tmpEvalVect);
      tmpEvalVect.clear();
      for(unsigned int j = 0;j<output[sequence[d]].size();j++)
        {
          tmpEvalVect.push_back(output[sequence[d]][j]);
        }
      evaluationOutputSet.push_back(tmpEvalVect);
      tmpEvalVect.clear();
    }
}

int main(int argc, char** argv)
{
  srand(time(NULL));
  printf("Initiating training\n");
  /*
   * Pre-process the data by reading it in and normalising it.
   * Then initialising the networks and sequence array.
   */
  seed = double(rand() % 100000);
  int hiddenNodes = 8;
  double learningRate = 0.1;
  double evalSize = 0.5;
  Network *motorNet;
  Network *eyeNet;
  readInInputFromFile();
  processToNormaliseData();
  sequence = new int[input.size()];
  for(unsigned int x=0;x<input.size();x++)
    sequence[x]=x;
  cout<<"Starting eye network"<<endl;
  do
  {
    eyeNet = new Network(3,hiddenNodes,4,seed,"eye");
    eyeNet->set_rho(learningRate);
    eyeNet->set_mSEBound(0.01);
    eyeNet->set_evaluationSize(evalSize);
    eyeNet->set_iterBound(150000);
    eyeNet->init();
    vectSize= input.size();
    evaluationVectSize = vectSize * eyeNet->get_evaluationSize();
    extractTrainningData();
    vectSize -= evaluationVectSize;
    iterations = 0;
    do
    {
      /*
       * This loop is identical to the one above except it feeds a different instance of a Network object
       */
      eyeNet->reset_meanSqrErr();
      random_shuffle(sequence,(sequence + vectSize));
      for(int i = 0; i < vectSize; i++)
      {
        eyeNet->update(input[sequence[i]]);
        eyeNet->backpropagate_error(output[sequence[i]]);
      }
      iterations++;
      if(iterations%1000==0)
        eyeNet->printData(iterations,vectSize);
//      if(iterations%50000==0)
//        printf("MSE:\t%f\t Iterations: %d\n",(eyeNet->get_meanSqrErr() / double(vectSize)),iterations);
    }while((eyeNet->get_meanSqrErr() / double(vectSize)) > eyeNet->get_mSEBound()&&iterations < eyeNet->get_iterBound());
    if(iterations==eyeNet->get_iterBound())
      cout<<"Too many iterations, re-initialling"<<endl;
  }while((eyeNet->get_meanSqrErr() / ((double)vectSize)) > eyeNet->get_mSEBound());
    eyeNet->printData(iterations,vectSize);
    eyeNet->closeFile();
    cout<<"Eye network finished"<<endl;
    printf("\nTraining complete\nAfter %d iterations \n\n",iterations);

  cout<<"Starting motor network"<<endl;
  do
    {
      motorNet = new Network(3,hiddenNodes,4,seed,"motor");
      motorNet->set_rho(learningRate);
      motorNet->set_mSEBound(0.01);
      motorNet->set_evaluationSize(evalSize);
      motorNet->init();
      motorNet->set_iterBound(150000);
      vectSize= input.size();
      evaluationVectSize = vectSize * motorNet->get_evaluationSize();
      extractTrainningData();
      vectSize -= evaluationVectSize;
      iterations=0;
      //Training loop
      do
      {
        /*
         * Randomise input by putting it through random shuffle and reset MSE to 0.0
         */
        motorNet->reset_meanSqrErr();
        random_shuffle(sequence,(sequence + vectSize));
        for(int i = 0; i < vectSize; i++)
        {
            /*
             * Feed this data set forward into the motorNet
             */
            motorNet->update(input[sequence[i]]);
            /*
             * Backpropagte error
             */
            motorNet->backpropagate_error(output[sequence[i]]);
        }
        iterations++;
        if(iterations%1000==0)
          motorNet->printData(iterations,vectSize);
//        if(iterations%50000==0)
//          printf("MSE:\t%f\t Iterations: %d\n",(motorNet->get_meanSqrErr() / double(vectSize)),iterations);
      }while((motorNet->get_meanSqrErr() / ((double)vectSize)) > motorNet->get_mSEBound()&& iterations < motorNet->get_iterBound());
      if(iterations==motorNet->get_iterBound())
        cout<<"Too many iterations, re-initialling"<<endl;
    }while((motorNet->get_meanSqrErr() / ((double)vectSize)) > motorNet->get_mSEBound());
    motorNet->printData(iterations,vectSize);
    motorNet->closeFile();
    cout<<"Motor network finished"<<endl;
    printf("\nTraining complete\nAfter %d iterations \n",iterations);

  delete[] sequence;
  /*
   * End of training loop
   *
   * Now testing the network
   *
   */
  printf("Initiating testing...\n");
  vector< vector< double> > motorOutputVect(evaluationVectSize,vector<double>(4));
  vector< vector< double> > eyeOutputVect(evaluationVectSize,vector<double>(4));
  for(int t=0;t<evaluationVectSize;t++)
  {
          motorNet->update(evaluationInputSet[t]);
          for(int i=0;i<4;i++)
            {
              motorOutputVect[t].push_back(motorNet->get_output()[i]);
            }
          eyeNet->update(evaluationInputSet[t]);
          for(int i=0;i<4;i++)
            {
              eyeOutputVect[t].push_back(eyeNet->get_output()[i]);
            }
  }
//  cout<<"Eye output"<<endl;
//  for(unsigned int i=0;i<eyeOutputVect.size();i++)
//    {
//      for(unsigned int j=0;j<eyeOutputVect[i].size();j++)
//        {
//          cout<<eyeOutputVect[i][j]<<" ";
//        }
//      cout<<endl;
//    }
//  cout<<endl;
//  cout<<"motor output"<<endl;
//  for(unsigned int i=0;i<motorOutputVect.size();i++)
//    {
//      for(unsigned int j=0;j<motorOutputVect[i].size();j++)
//        {
//          cout<<motorOutputVect[i][j]<<" ";
//        }
//      cout<<endl;
//    }
//  cout<<endl;
  unNormaliseData("output");
  //restoreData("output");
  for(int c=0;c<evaluationVectSize;c++)
      {
        printf("Desired output[%d]:\n",c);

        for(int d= 0;d<(evaluationOutputSet[c].size());d++)
          {
                    printf("\t %f ", evaluationOutputSet[c][d]);
                    if(d<4)
                      printf("\t\t Output: %f\n",((motorOutputVect[c][d+4] * (minMaxOutput[d][1] - minMaxOutput[d][0])) + minMaxOutput[d][0]));
                    else
                      printf("\t\t Output: %f\n",((eyeOutputVect[c][d+4] * (minMaxOutput[d][1] - minMaxOutput[d][0])) + minMaxOutput[d][0]));
          }
        printf("\n");
      }
  printf("Testing complete\n");
  delete motorNet;
  delete eyeNet;
  printf("Program complete\n");
  return (0);
}

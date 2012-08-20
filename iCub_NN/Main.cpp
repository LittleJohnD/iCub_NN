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
                          //printf("Component : %s\n",attr); // Do stuff with it
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
//      printf("Data Vector[%d]:\n",n);
//      for(unsigned int o=0;o<data[n].size();o++)
//        printf("\t %d: %f\t",o,data[n][o]);
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
//      for(unsigned int n =0;n<minMaxInput.size();n++)
//        {
//          printf("minMaxInput\n");
//          for(unsigned int i=0;i<minMaxInput[n].size();i++)
//            cout<<minMaxInput[n][i]<<"\t";
//          printf("\n");
//        }
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
//      for(unsigned int n =0;n<minMaxOutput.size();n++)
//        {
//          printf("minMaxOutput\n");
//          for(unsigned int i=0;i<minMaxOutput[n].size();i++)
//            cout<<minMaxOutput[n][i]<<"\t";
//          printf("\n");
//        }
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
//      for(unsigned int n =0;n<input.size();n++)
//        {
//          printf("Normalised Input Data\n");
//          for(unsigned int i=0;i<input[n].size();i++)
//            cout<<input[n][i]<<"\t";
//          printf("\n");
//        }
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
//      for(unsigned int n =0;n<input.size();n++)
//        {
//          printf("Normalised Output Data\n");
//          for(unsigned int i=0;i<output[n].size();i++)
//            cout<<output[n][i]<<"\t";
//          printf("\n");
//        }
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
      for(unsigned int i= 0;i<evaluationOutputSet.size();i++)
        {
          for(unsigned int j = 0;j<evaluationOutputSet[i].size();j++)
            {
              if(minMaxOutput[j][0]!=minMaxOutput[j][1])
                {
                  //cout<<"Norm:\t"<<output[i][j];
                  evaluationOutputSet[i][j]=((evaluationOutputSet[i][j] * (minMaxOutput[j][1] - minMaxOutput[j][0])) + minMaxOutput[j][0]);
                  //cout<<"\t Unnorm:\t"<<output[i][j]<<endl;
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
      vector< double > tmpVect;
      for(unsigned int i = 0; i< (input[sequence[d]].size()) ;i++)
        {
          tmpVect.push_back(input[sequence[d]][i]);
        }
      evaluationInputSet.push_back(tmpVect);
      tmpVect.clear();
      for(unsigned int j = 0;j<output[sequence[d]].size();j++)
        {
          tmpVect.push_back(output[sequence[d]][j]);
        }
      evaluationOutputSet.push_back(tmpVect);
      tmpVect.clear();
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
  do
    {
      net = new Network(3,8,8,seed);
      sequence = new int[input.size()];
      for(unsigned int x=0;x<input.size();x++)
        sequence[x]=x;
      net->set_rho(0.5);
      net->set_mSEBound(0.001);
      net->set_evaluationSize(0.5);
      net->set_iterBound(1000000);
      net->init();
      iterations=0;
      //Training loop
      vectSize= input.size();
      evaluationVectSize = vectSize * net->get_evaluationSize();
      extractTrainningData();
      vectSize -= evaluationVectSize;

      do
      {
          //Randomise input and reset MSE
          net->reset_meanSqrErr();
          random_shuffle(sequence,(sequence + vectSize));
        for(int i = 0; i < vectSize; i++)
        {
            //Feed this data set forward;
            net->update(input[sequence[i]]);
            //Backpropagte_error
            net->backpropagate_error(output[sequence[i]]);
        }
       // cout<<endl;
        iterations++;
        if(iterations%50000==0)
          net->printData(iterations,vectSize);
        if(iterations%50000==0)
          printf("MSE:\t%f \t Iternations: \t %d \n",(net->get_meanSqrErr() / ((double) vectSize)),iterations);
          //net->update_meanSqrErr(net->get_meanSqrErr());
      //          else if(iterations%5000==0)
      //            printf(".");

      }while((net->get_meanSqrErr() / ((double)vectSize)) > net->get_mSEBound()&&iterations<net->get_iterBound());//while(net->update_meanSqrErr(net->get_meanSqrErr())); //
      if(iterations==net->get_iterBound())
        cout<<"Too many iterterations, reintailisng"<<endl;
    }while((net->get_meanSqrErr() / ((double)vectSize)) > net->get_mSEBound());
  net->printData(iterations,vectSize);
  delete[] sequence;
  printf("\nTraining complete\nAfter %d iterations \n",iterations);
  /*
   * End of training loop
   *
   * Now testing the network
   *
   */
  printf("Initiating testing...\n");
  for(int t=0;t<evaluationVectSize;t++)
  {
          net->update(evaluationInputSet[t]);
  }
  unNormaliseData("output");
  //restoreData("output");
  for(int c=0;c<evaluationVectSize;c++)
      {
        printf("Desired output[%d]:\n",c);

        for(int d= 0;d<(evaluationOutputSet[c].size());d++)
          {
                    printf("\t %f ", evaluationOutputSet[c][d]);
                    printf("\t\t Output: %f\n",evaluationOutputSet[c][d]);// *  (minMaxOutput[d][1] - minMaxOutput[d][0])) + minMaxOutput[d][0]));
          }
        printf("\n\n\n");
      }
  printf("Testing complete\n");
  delete net;
  return (0);
}

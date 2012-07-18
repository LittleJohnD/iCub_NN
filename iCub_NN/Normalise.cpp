/*
 * Normalise.cpp
 *
 *  Created on: 18 Jul 2012
 *      Author: little-john
 */

#include "Normalise.h"

using namespace std;

Normalise::Normalise()
{

}

Normalise::~Normalise()
{
  // TODO Auto-generated destructor stub
}

void Normalise::normaliseData( std::vector< std::vector<double> > &input_vector, std::vector< std::vector<double> > &output_vector)
  {
    findMinMax(input_vector,minMaxInput);
    normData(input_vector,minMaxInput);
    findMinMax(output_vector,minMaxOutput);
    normData(input_vector,minMaxOutput);
  }

void Normalise::findMinMax( std::vector< std::vector<double> > &minMaxVector, std::vector< std::vector<double> > minMax)
{
  for(unsigned int i=0;i<minMaxVector.size();i++)
    {
      for(int j=0;j<(minMaxVector[i].size());j++)
        {
          if(minMax[i].empty())
            {
              vector <double> tmpVector(2,0.0);
              minMax.push_back(tmpVector);
            }

          if(minMaxVector[i][j]<minMax[j][0])
            {
              minMax[j][0]=minMaxVector[i][j];
            }
          else if(minMaxVector[i][j]>minMax[j][1])
            {
              minMax[j][1]=minMaxVector[i][j];
            }
        }
    }
}
void Normalise::normData( std::vector< std::vector<double> > &dataVector,std::vector< std::vector<double> > &minMaxVector)
{
  for(unsigned int i= 0;i<dataVector.size();i++)
    {
      for(unsigned int j = 0;j<dataVector[i].size();j++)
        {
          dataVector[i][j]=((dataVector[i][j]-minMaxVector[j][0])/(minMaxVector[j][1]-minMaxVector[j][0]));
        }
    }
}
void Normalise::unNormData( std::vector< std::vector<double> > &dataVector,std::vector< std::vector<double> > &minMaxVector)
{
  for(unsigned int i= 0;i<dataVector.size();i++)
    {
      for(unsigned int j = 0;j<dataVector[i].size();j++)
        {
          dataVector[i][j]=((dataVector[i][j]+minMaxVector[j][0])*(minMaxVector[j][1]-minMaxVector[j][0]));
        }
    }
}

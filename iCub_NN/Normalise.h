/*
 * Normalise.h
 *
 *  Created on: 18 Jul 2012
 *      Author: little-john
 */

#ifndef NORMALISE_H_
#define NORMALISE_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>

class Normalise
{
public:
  Normalise();
  void normaliseData( std::vector< std::vector<double> > &input_vector, std::vector< std::vector<double> > &output_vector);
  void findMinMax( std::vector< std::vector<double> > &vector, std::vector< std::vector<double> > minMax);
  void normData( std::vector< std::vector<double> > &dataVector,std::vector< std::vector<double> > &minMaxVector);
  void unNormData( std::vector< std::vector<double> > &dataVector,std::vector< std::vector<double> > &minMaxVector);
  inline std::vector< std::vector<double> > get_minMaxInput( void ){return minMaxInput;}
  inline std::vector< std::vector<double> > get_minMaxOutput( void ){return minMaxOutput;}

  std::vector< std::vector<double> > minMaxInput;
  std::vector< std::vector<double> > minMaxOutput;


  virtual
  ~Normalise();
};

#endif /* NORMALISE_H_ */

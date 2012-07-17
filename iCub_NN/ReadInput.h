/*
 * ReadInput.h
 *
 *  Created on: 17 Jul 2012
 *      Author: little-john
 */

#ifndef READINPUT_H_
#define READINPUT_H_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "tinyxml.h"


class ReadInput
{
public:
  ReadInput();
  void readInInputFromFile(std::string inputFileName);

  virtual
  ~ReadInput();
};

#endif /* READINPUT_H_ */

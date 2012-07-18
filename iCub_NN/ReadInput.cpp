/*
 * ReadInput.cpp
 *
 *  Created on: 17 Jul 2012
 *      Author: little-john
 */

#include "ReadInput.h"

using namespace std;


ReadInput::ReadInput()
{
  // TODO Auto-generated contructor stub
}

ReadInput::~ReadInput()
{
  // TODO Auto-generated destructor stub
}

void ReadInput::readInInputFromFile(string inputFileName,std::vector< std::vector<double> > &input_vector,std::vector< std::vector<double> > &output_vector)
{
  TiXmlDocument jSpace(inputFileName.c_str());
  string elemName;
  TiXmlElement* elem;
  const char* attr;
  int count =0;
  vector<double>tmpVector;

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

                  output_vector.push_back(tmpVector);
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
                  input_vector.push_back(tmpVector);
                }
              else if(!(elemName == "OutputNode"))
                printf("Error reading in File\n");
            }
        }

    }
  else
    printf("Failed to load file \"%s\"\n", inputFileName.c_str());
  jSpace.Clear();
//  printf("Count: %d\n",count);
//  printf("Input Vector size: %d\n", input_vector.size());
//  printf("Output Vector size: %d\n", output_vector.size());
}

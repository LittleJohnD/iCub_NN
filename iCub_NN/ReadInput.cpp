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

void ReadInput::readInInputFromFile(string inputFileName,std::vector< std::vector<double> > &input_vector,double place)
{
  TiXmlDocument jSpace(inputFileName.c_str());
  string elemName;
  TiXmlElement* elem;
  const char* attr;
  int count =0;

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
              if(elemName == "LinkAttr")
                {
                  count++;
                  vector<double>tmpVector;
                  for(TiXmlElement* e = elem->FirstChildElement("LinkAttrValue"); e != NULL; e = e->NextSiblingElement("LinkAttrValue"))
                  {
                      attr = e->Attribute("LinkValue");
                      if(attr != NULL)
                        {

                          tmpVector.push_back(double(atoi(attr)));
                          //printf("Component : %s\n",attr); // Do stuff with it
                        }
                  }
                  input_vector.push_back(tmpVector);
                }
            }
        }

    }
  else
    printf("Failed to load file \"%s\"\n", inputFileName.c_str());
  jSpace.Clear();
  printf("Count: %d\n",count);
  printf("Vector size: %d\n", input_vector.size());
}

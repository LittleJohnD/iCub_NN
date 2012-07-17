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

void ReadInput::readInInputFromFile(string inputFileName)
{
  TiXmlDocument jSpace(inputFileName.c_str());
  string elemName;
  TiXmlElement* elem;
  if(jSpace.LoadFile())
    {
      TiXmlElement* link = jSpace.FirstChildElement();
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
            printf("LinkAttr\n");
            for(TiXmlNode* e = elem->FirstChild(); e != NULL; e = e->NextSibling())
            {
                TiXmlText* text = e->ToText();
                if(text == NULL)
                    continue;
                printf("Text: %s\n", text->Value());
                // Do stuff
            }
          }
      }
    }
  else
    printf("Failed to load file \"%s\"\n", inputFileName.c_str());
}

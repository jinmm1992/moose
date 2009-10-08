#ifndef GENERICBCBLOCK_H
#define GENERICBCBLOCK_H

#include "ParserBlock.h"

class GenericBCBlock: public ParserBlock
{
public:
  GenericBCBlock(const std::string & reg_id, const std::string & real_id, const GetPot & input_file);

  virtual void execute();

private:
  std::string _type;
};

  

#endif //GENERICBCBLOCK_H

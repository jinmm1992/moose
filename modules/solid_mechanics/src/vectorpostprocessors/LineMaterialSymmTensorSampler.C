/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/


#include "LineMaterialSymmTensorSampler.h"

template<>
InputParameters validParams<LineMaterialSymmTensorSampler>()
{
  InputParameters params = validParams<LineMaterialSamplerBase<Real> >();
  params += validParams<MaterialTensorCalculator>();
  return params;
}

LineMaterialSymmTensorSampler::LineMaterialSymmTensorSampler(const std::string & name, InputParameters parameters) :
    LineMaterialSamplerBase<SymmTensor>(name, parameters),
    MaterialTensorCalculator(name, parameters)
{
}

Real
LineMaterialSymmTensorSampler::getScalarFromProperty(SymmTensor &property, const Point * curr_point)
{
  RealVectorValue direction;
  return getTensorQuantity(property, curr_point, direction);
}

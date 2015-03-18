/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef COMPUTESTRAINBASE_H
#define COMPUTESTRAINBASE_H

#include "Material.h"
#include "RankTwoTensor.h"
#include "ElasticityTensorR4.h"
#include "RotationTensor.h"
#include "DerivativeMaterialInterface.h"

/**
 * ComputeStrainBase is the base class for strain tensors
 */
class ComputeStrainBase : public DerivativeMaterialInterface<Material>
{
public:
  ComputeStrainBase(const std:: string & name, InputParameters parameters);

protected:
  virtual void initQpStatefulProperties();
  virtual void computeProperties() = 0;

  VariableGradient & _grad_disp_x;
  VariableGradient & _grad_disp_y;
  VariableGradient & _grad_disp_z;

  VariableGradient & _grad_disp_x_old;
  VariableGradient & _grad_disp_y_old;
  VariableGradient & _grad_disp_z_old;

  VariableValue & _T;
  const Real _T0;
  const Real _thermal_expansion_coeff;

  std::string _base_name;

  MaterialProperty<RankTwoTensor> & _total_strain;
};

#endif //COMPUTESTRAINBASE_H

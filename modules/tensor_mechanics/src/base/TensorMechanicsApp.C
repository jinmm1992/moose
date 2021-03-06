/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "TensorMechanicsApp.h"
#include "Moose.h"
#include "AppFactory.h"

#include "TensorMechanicsAction.h"
#include "PressureActionTM.h"

#include "StressDivergenceTensors.h"
#include "CosseratStressDivergenceTensors.h"
#include "MomentBalancing.h"
#include "GravityTM.h"

#include "LinearElasticMaterial.h"
#include "FiniteStrainElasticMaterial.h"
#include "FiniteStrainPlasticMaterial.h"
#include "FiniteStrainRatePlasticMaterial.h"
#include "FiniteStrainMohrCoulomb.h"
#include "FiniteStrainCrystalPlasticity.h"
#include "FiniteStrainMultiPlasticity.h"
#include "CosseratLinearElasticMaterial.h"
#include "ElementPropertyReadFileTest.h"
#include "TwoPhaseStressMaterial.h"
#include "SimpleEigenStrainMaterial.h"
#include "ComputeElasticityTensor.h"
#include "ComputeIsotropicElasticityTensor.h"
#include "ComputeSmallStrain.h"
#include "ComputeFiniteStrain.h"
#include "ComputeLinearElasticStress.h"
#include "ComputeFiniteStrainElasticStress.h"
#include "Compute1PhaseEigenStrain.h"

#include "TensorMechanicsPlasticSimpleTester.h"
#include "TensorMechanicsPlasticTensile.h"
#include "TensorMechanicsPlasticTensileMulti.h"
#include "TensorMechanicsPlasticMohrCoulomb.h"
#include "TensorMechanicsPlasticMohrCoulombMulti.h"
#include "TensorMechanicsPlasticWeakPlaneTensile.h"
#include "TensorMechanicsPlasticWeakPlaneTensileN.h"
#include "TensorMechanicsPlasticWeakPlaneShear.h"
#include "TensorMechanicsPlasticJ2.h"
#include "TensorMechanicsHardeningConstant.h"
#include "TensorMechanicsHardeningGaussian.h"
#include "TensorMechanicsHardeningExponential.h"
#include "TensorMechanicsHardeningCutExponential.h"
#include "TensorMechanicsHardeningCubic.h"
#include "ElementPropertyReadFile.h"

#include "RankTwoAux.h"
#include "RealTensorValueAux.h"
#include "RankFourAux.h"
#include "TensorElasticEnergyAux.h"
#include "FiniteStrainPlasticAux.h"
#include "CrystalPlasticitySlipSysAux.h"
#include "CrystalPlasticityRotationOutAux.h"
#include "RankTwoScalarAux.h"
#include "StressDivergencePFFracTensors.h"

#include "PressureTM.h"


template<>
InputParameters validParams<TensorMechanicsApp>()
{
  InputParameters params = validParams<MooseApp>();
  params.set<bool>("use_legacy_uo_initialization") = true;
  params.set<bool>("use_legacy_uo_aux_computation") = false;

  return params;
}

TensorMechanicsApp::TensorMechanicsApp(const std::string & name, InputParameters parameters) :
    MooseApp(name, parameters)
{
  srand(processor_id());

  Moose::registerObjects(_factory);
  TensorMechanicsApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  TensorMechanicsApp::associateSyntax(_syntax, _action_factory);
}

TensorMechanicsApp::~TensorMechanicsApp()
{
}

extern "C" void TensorMechanicsApp_registerApps() { TensorMechanicsApp::registerApps(); }
void
TensorMechanicsApp::registerApps()
{
  registerApp(TensorMechanicsApp);
}

void
TensorMechanicsApp::registerObjects(Factory & factory)
{
  registerKernel(StressDivergenceTensors);
  registerKernel(CosseratStressDivergenceTensors);
  registerKernel(MomentBalancing);
  registerKernel(StressDivergencePFFracTensors);
  registerKernel(GravityTM);

  registerMaterial(LinearElasticMaterial);
  registerMaterial(FiniteStrainElasticMaterial);
  registerMaterial(FiniteStrainPlasticMaterial);
  registerMaterial(FiniteStrainMohrCoulomb);
  registerMaterial(FiniteStrainRatePlasticMaterial);
  registerMaterial(FiniteStrainCrystalPlasticity);
  registerMaterial(FiniteStrainMultiPlasticity);
  registerMaterial(CosseratLinearElasticMaterial);
  registerMaterial(ElementPropertyReadFileTest);
  registerMaterial(TwoPhaseStressMaterial);
  registerMaterial(SimpleEigenStrainMaterial);
  registerMaterial(ComputeElasticityTensor);
  registerMaterial(ComputeIsotropicElasticityTensor);
  registerMaterial(ComputeSmallStrain);
  registerMaterial(ComputeFiniteStrain);
  registerMaterial(ComputeLinearElasticStress);
  registerMaterial(ComputeFiniteStrainElasticStress);
  registerMaterial(Compute1PhaseEigenStrain);

  registerUserObject(TensorMechanicsPlasticSimpleTester);
  registerUserObject(TensorMechanicsPlasticTensile);
  registerUserObject(TensorMechanicsPlasticTensileMulti);
  registerUserObject(TensorMechanicsPlasticMohrCoulomb);
  registerUserObject(TensorMechanicsPlasticMohrCoulombMulti);
  registerUserObject(TensorMechanicsPlasticWeakPlaneTensile);
  registerUserObject(TensorMechanicsPlasticWeakPlaneTensileN);
  registerUserObject(TensorMechanicsPlasticWeakPlaneShear);
  registerUserObject(TensorMechanicsPlasticJ2);
  registerUserObject(TensorMechanicsHardeningConstant);
  registerUserObject(TensorMechanicsHardeningGaussian);
  registerUserObject(TensorMechanicsHardeningExponential);
  registerUserObject(TensorMechanicsHardeningCutExponential);
  registerUserObject(TensorMechanicsHardeningCubic);
  registerUserObject(ElementPropertyReadFile);

  registerAux(RankTwoAux);
  registerAux(RealTensorValueAux);
  registerAux(RankFourAux);
  registerAux(TensorElasticEnergyAux);
  registerAux(FiniteStrainPlasticAux);
  registerAux(CrystalPlasticitySlipSysAux);
  registerAux(CrystalPlasticityRotationOutAux);
  registerAux(RankTwoScalarAux);

  registerBoundaryCondition(PressureTM);
}

void
TensorMechanicsApp::associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
  syntax.registerActionSyntax("TensorMechanicsAction", "Kernels/TensorMechanics");

  syntax.registerActionSyntax("EmptyAction", "BCs/PressureTM");
  syntax.registerActionSyntax("PressureActionTM", "BCs/PressureTM/*");

  registerAction(TensorMechanicsAction, "add_kernel");
  registerAction(PressureActionTM, "add_bc");
}

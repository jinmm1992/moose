/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/
#ifndef DARCY_THERMO_MECHAPP_H
#define DARCY_THERMO_MECHAPP_H

#include "MooseApp.h"

class DarcyThermoMechApp;

template<>
InputParameters validParams<DarcyThermoMechApp>();

class DarcyThermoMechApp : public MooseApp
{
public:
  DarcyThermoMechApp(const std::string & name, InputParameters parameters);
  virtual ~DarcyThermoMechApp();

  static void registerApps();
  static void registerObjects(Factory & factory);
  static void associateSyntax(Syntax & syntax, ActionFactory & action_factory);
};

#endif /* DARCY_THERMO_MECHAPP_H */

/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/


//  Rogers-Stallybrass-Clements version of effective saturation of oil (gas) phase
//  valid for residual saturations = 0, and viscosityOil = 2*viscosityWater.  (the "2" is important here!).
// C Rogers, MP Stallybrass and DL Clements "On two phase filtration under gravity and with boundary infiltration: application of a Backlund transformation" Nonlinear Analysis Theory Methods and Applications 7 (1983) 785--799.
//
#include "RichardsSeff2gasRSC.h"

template<>
InputParameters validParams<RichardsSeff2gasRSC>()
{
  InputParameters params = validParams<RichardsSeff>();
  params.addParam<Real>("oil_viscosity", "Viscosity of oil (gas) phase.  It is assumed this is double the water-phase viscosity");
  params.addParam<Real>("scale_ratio", "This is porosity/permeability/beta^2, where beta may be chosen by the user (RSC define beta<0, but MOOSE only uses beta^2, so its sign is irrelevant).  It has dimensions [time]");
  params.addParam<Real>("shift", "effective saturation is a function of (Pc - shift)");
  params.addClassDescription("Rogers-Stallybrass-Clements version of effective saturation for the oil (gas) phase, valid for residual saturations = 0, and viscosityOil = 2*viscosityWater.  seff_gas = 1 - 1/Sqrt(1 + Exp((Pc - shift)/scale)), where scale = 0.25*scale_ratio*oil_viscosity");
  return params;
}

RichardsSeff2gasRSC::RichardsSeff2gasRSC(const std::string & name, InputParameters parameters) :
    RichardsSeff(name, parameters),
    _oil_viscosity(getParam<Real>("oil_viscosity")),
    _scale_ratio(getParam<Real>("scale_ratio")),
    _shift(getParam<Real>("shift")),
    _scale(0.25*_scale_ratio*_oil_viscosity)
{}



Real
RichardsSeff2gasRSC::seff(std::vector<VariableValue *> p, unsigned int qp) const
{
  Real pc = (*p[1])[qp] - (*p[0])[qp];
  return 1 - RichardsSeffRSC::seff(pc, _shift, _scale);
}

void
RichardsSeff2gasRSC::dseff(std::vector<VariableValue *> p, unsigned int qp, std::vector<Real> &result) const
{
  Real pc = (*p[1])[qp] - (*p[0])[qp];
  result[1] = -RichardsSeffRSC::dseff(pc, _shift, _scale);
  result[0] = -result[1];
}

void
RichardsSeff2gasRSC::d2seff(std::vector<VariableValue *> p, unsigned int qp, std::vector<std::vector<Real> > &result) const
{
  Real pc = (*p[1])[qp] - (*p[0])[qp];
  result[1][1] = -RichardsSeffRSC::d2seff(pc, _shift, _scale);
  result[0][1] = -result[1][1];
  result[1][0] = -result[1][1];
  result[0][0] = result[1][1];
}


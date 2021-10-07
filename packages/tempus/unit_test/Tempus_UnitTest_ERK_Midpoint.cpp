// @HEADER
// ****************************************************************************
//                Tempus: Copyright (2017) Sandia Corporation
//
// Distributed under BSD 3-clause license (See accompanying file Copyright.txt)
// ****************************************************************************
// @HEADER

#include "Tempus_UnitTest_RK_Utils.hpp"


namespace Tempus_Unit_Test {

using Teuchos::RCP;
using Teuchos::rcp;
using Teuchos::rcp_const_cast;
using Teuchos::rcp_dynamic_cast;


// ************************************************************
// ************************************************************
TEUCHOS_UNIT_TEST(ERK_Midpoint, Default_Construction)
{
  auto stepper = rcp(new Tempus::StepperERK_Midpoint<double>());
  testExplicitRKAccessorsFullConstruction(stepper);

  // Test stepper properties.
  TEUCHOS_ASSERT(stepper->getOrder() == 2);
}


// ************************************************************
// ************************************************************
TEUCHOS_UNIT_TEST(ERK_Midpoint, StepperFactory_Construction)
{
  auto model = rcp(new Tempus_Test::SinCosModel<double>());
  testFactoryConstruction("RK Explicit Midpoint", model);
}


// ************************************************************
// ************************************************************
TEUCHOS_UNIT_TEST(ERK_Midpoint, AppAction)
{
  auto stepper = rcp(new Tempus::StepperERK_Midpoint<double>());
  auto model = rcp(new Tempus_Test::SinCosModel<double>());
  testRKAppAction(stepper, model, out, success);
}


} // namespace Tempus_Test

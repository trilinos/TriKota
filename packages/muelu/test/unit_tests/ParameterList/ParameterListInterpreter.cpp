// @HEADER
//
// ***********************************************************************
//
//        MueLu: A package for multigrid based preconditioning
//                  Copyright 2012 Sandia Corporation
//
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact
//                    Jonathan Hu       (jhu@sandia.gov)
//                    Andrey Prokopenko (aprokop@sandia.gov)
//                    Ray Tuminaro      (rstumin@sandia.gov)
//
// ***********************************************************************
//
// @HEADER
#include <Teuchos_UnitTestHarness.hpp>
#include <Teuchos_ParameterList.hpp>
#include <Teuchos_XMLParameterListHelpers.hpp>

#include <MueLu_TestHelpers.hpp>

#include <MueLu_ParameterListInterpreter.hpp>
#include <MueLu_Exceptions.hpp>

#include <Xpetra_IO.hpp>

namespace MueLuTests {

  TEUCHOS_UNIT_TEST_TEMPLATE_4_DECL(ParameterListInterpreter, SetParameterList, Scalar, LocalOrdinal, GlobalOrdinal, Node)
  {
#   include <MueLu_UseShortNames.hpp>
    MUELU_TESTING_SET_OSTREAM;
    MUELU_TESTING_LIMIT_SCOPE(Scalar,GlobalOrdinal,Node);
#if defined(HAVE_MUELU_TPETRA) && defined(HAVE_MUELU_EPETRA) && defined(HAVE_MUELU_IFPACK) && defined(HAVE_MUELU_IFPACK2) && defined(HAVE_MUELU_AMESOS) && defined(HAVE_MUELU_AMESOS2)

    RCP<Matrix> A = TestHelpers::TestFactory<SC, LO, GO, NO>::Build1DPoisson(99);
    RCP<const Teuchos::Comm<int> > comm = TestHelpers::Parameters::getDefaultComm();

    ArrayRCP<std::string> fileList = TestHelpers::GetFileList(std::string("ParameterList/ParameterListInterpreter/"), std::string(".xml"));

    for(int i=0; i< fileList.size(); i++) {
      // Ignore files with "BlockCrs" in their name
      auto found = fileList[i].find("BlockCrs");
      if(found != std::string::npos) continue;

      out << "Processing file: " << fileList[i] << std::endl;
      ParameterListInterpreter mueluFactory("ParameterList/ParameterListInterpreter/" + fileList[i],*comm);

      RCP<Hierarchy> H = mueluFactory.CreateHierarchy();
      H->GetLevel(0)->Set("A", A);

      mueluFactory.SetupHierarchy(*H);

      //TODO: check no unused parameters
      //TODO: check results of Iterate()
    }

#   else
    out << "Skipping test because some required packages are not enabled (Tpetra, Epetra, EpetraExt, Ifpack, Ifpack2, Amesos, Amesos2)." << std::endl;
#   endif
  }


TEUCHOS_UNIT_TEST_TEMPLATE_4_DECL(ParameterListInterpreter, BlockCrs, Scalar, LocalOrdinal, GlobalOrdinal, Node)
  {
#   include <MueLu_UseShortNames.hpp>
    MUELU_TESTING_SET_OSTREAM;
    MUELU_TESTING_LIMIT_SCOPE(Scalar,GlobalOrdinal,Node);
#if defined(HAVE_MUELU_TPETRA)
    MUELU_TEST_ONLY_FOR(Xpetra::UseTpetra) {
      Teuchos::ParameterList matrixParams;
      matrixParams.set("matrixType","Laplace1D");
      matrixParams.set("nx",(GlobalOrdinal)300);// needs to be even

      RCP<Matrix> A = TestHelpers::TpetraTestFactory<SC, LO, GO, NO>::BuildBlockMatrix(matrixParams,Xpetra::UseTpetra);  
      out<<"Matrix Size (block) = "<<A->getGlobalNumRows()<<" (point) "<<A->getRangeMap()->getGlobalNumElements()<<std::endl;
      RCP<const Teuchos::Comm<int> > comm = TestHelpers::Parameters::getDefaultComm();
      
      ArrayRCP<std::string> fileList = TestHelpers::GetFileList(std::string("ParameterList/ParameterListInterpreter/"), std::string(".xml"));
      
      for(int i=0; i< fileList.size(); i++) {
        // Only run files with "BlockCrs" in their name
        auto found = fileList[i].find("BlockCrs");
        if(found == std::string::npos) continue;

        out << "Processing file: " << fileList[i] << std::endl;
        ParameterListInterpreter mueluFactory("ParameterList/ParameterListInterpreter/" + fileList[i],*comm);
        
        RCP<Hierarchy> H = mueluFactory.CreateHierarchy();
        H->GetLevel(0)->Set("A", A);
        
        mueluFactory.SetupHierarchy(*H);
        
        //TODO: check no unused parameters
        //TODO: check results of Iterate()
      }
    }
#   endif
    TEST_EQUALITY(1,1);
  }

#define MUELU_ETI_GROUP(Scalar, LocalOrdinal, GlobalOrdinal, Node) \
  TEUCHOS_UNIT_TEST_TEMPLATE_4_INSTANT(ParameterListInterpreter, SetParameterList, Scalar, LocalOrdinal, GlobalOrdinal, Node) \
  TEUCHOS_UNIT_TEST_TEMPLATE_4_INSTANT(ParameterListInterpreter, BlockCrs, Scalar, LocalOrdinal, GlobalOrdinal, Node)

#include <MueLu_ETI_4arg.hpp>

} // namespace MueLuTests

// TODO: some tests of the parameter list parser can be done without building the Hierarchy.

// @HEADER
//
// ***********************************************************************
//
//             Xpetra: A linear algebra interface package
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
#ifndef XPETRA_VECTORFACTORY_DEF_HPP
#define XPETRA_VECTORFACTORY_DEF_HPP

#include "Xpetra_VectorFactory_decl.hpp"



namespace Xpetra {



template<class Scalar, class LocalOrdinal, class GlobalOrdinal, class Node>
VectorFactory<Scalar, LocalOrdinal, GlobalOrdinal, Node>::
VectorFactory()
{
}



template<class Scalar, class LocalOrdinal, class GlobalOrdinal, class Node>
Teuchos::RCP< Vector<Scalar,LocalOrdinal,GlobalOrdinal,Node> >
VectorFactory<Scalar, LocalOrdinal, GlobalOrdinal, Node>::
Build(const Teuchos::RCP<const Map>& map, 
      bool zeroOut)
{
    XPETRA_MONITOR("VectorFactory::Build");

    RCP<const Xpetra::BlockedMap<LocalOrdinal, GlobalOrdinal, Node>> 
      bmap = Teuchos::rcp_dynamic_cast<const Xpetra::BlockedMap<LocalOrdinal, GlobalOrdinal, Node>>(map);

    if(!bmap.is_null())
    {
        return rcp(new Xpetra::BlockedVector<Scalar, LocalOrdinal, GlobalOrdinal, Node>(bmap, zeroOut));
    }

#ifdef HAVE_XPETRA_TPETRA
    if(map->lib() == UseTpetra)
    {
        return rcp(new TpetraVector(map, zeroOut));
    }
#endif

    XPETRA_FACTORY_ERROR_IF_EPETRA(map->lib());
    XPETRA_FACTORY_END;
}



}      // namespace Xpetra


#endif      // XPETRA_VECTORFACTORY_DEF_HPP



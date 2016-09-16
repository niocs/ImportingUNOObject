/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*************************************************************************
 *
 *  The Contents of this file are made available subject to the terms of
 *  the BSD license.
 *
 *  Copyright 2000, 2010 Oracle and/or its affiliates.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of Sun Microsystems, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 *  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 *  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 *  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *************************************************************************/

#include <stdio.h>
#include <sal/main.h>
#include <cppuhelper/bootstrap.hxx>
#include <com/sun/star/bridge/XUnoUrlResolver.hpp>
#include <com/sun/star/lang/XMultiServiceFactory.hpp>
#include <com/sun/star/lang/XMultiComponentFactory.hpp>

using namespace com::sun::star::uno;
using namespace com::sun::star::lang;
using namespace com::sun::star::bridge;

using ::rtl::OString;
using ::rtl::OUString;
using ::rtl::OUStringToOString;

SAL_IMPLEMENT_MAIN()
{
    // create the initial component context
    Reference< XComponentContext > xLocalComponentContext = cppu::defaultBootstrap_InitialComponentContext();

    // Initial service manager
    Reference< XMultiComponentFactory > xServiceManager = xLocalComponentContext->getServiceManager();

    // Create a URL resolver
    Reference< XInterface > xInstance =
        xServiceManager->createInstanceWithContext(
            "com.sun.star.bridge.UnoUrlResolver",
            xLocalComponentContext );

    // Query for the XUnoUrlResolver interface
    Reference< XUnoUrlResolver > xResolver( xInstance, UNO_QUERY );

    if( ! xResolver.is() )
    {
        printf( "Error: Couldn't instantiate com.sun.star.bridge.UnoUrlResolver service\n" );
        fflush(stdout);
        return 1;
    }
    try
    {
        // Import the object
        xInstance = xResolver->resolve( OUString(
            "uno:socket,host=localhost,port=2002;urp;StarOffice.ServiceManager" ) );

        if( ! xInstance.is() )
        {
            printf( "StarOffice.ServiceManager is not exported from remote counterpart\n" );
            fflush(stdout);
            return 1;
        }

        // query for the simpler XMultiServiceFactory interface, sufficient for scripting
        Reference< XMultiServiceFactory > xOfficeServiceManager (xInstance, UNO_QUERY);

        if( ! xOfficeServiceManager.is() )
        {
            printf( "XMultiServiceFactory interface is not exported for StarOffice.ServiceManager\n" );
            fflush(stdout);
            return 1;
        }

        printf( "Connected successfully to the office\n" );
        fflush(stdout);
    }
    catch( Exception &e )
    {
        OString o = OUStringToOString( e.Message, RTL_TEXTENCODING_ASCII_US );
        printf( "Caught exception : %s\n", o.pData->buffer );
        fflush(stdout);
        return 1;
    }
    return 0;
}

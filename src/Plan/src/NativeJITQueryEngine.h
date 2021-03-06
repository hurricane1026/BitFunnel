// The MIT License (MIT)

// Copyright (c) 2018, Microsoft

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#pragma once

#include <memory>                                   // std::unique_ptr embedded.

#include "BitFunnel/Configuration/IStreamConfiguration.h"
#include "BitFunnel/Index/ISimpleIndex.h"
#include "BitFunnel/IDiagnosticStream.h"
#include "BitFunnel/Plan/IQueryEngine.h"
#include "NativeJIT/CodeGen/ExecutionBuffer.h"  // Template parameter.
#include "NativeJIT/CodeGen/FunctionBuffer.h"   // Template parameter.
#include "Temporary/Allocator.h"                // Template parameter.


namespace BitFunnel
{
    //*************************************************************************
    //
    // ByteCodeQueryEngine
    //
    // The class used to run parsed queries using the ByteCodeInterpreter.
    //
    //*************************************************************************
    class NativeJITQueryEngine : public IQueryEngine
    {
    public:
        NativeJITQueryEngine(ISimpleIndex const & index,
                             IStreamConfiguration const & config,
                             size_t treeAllocatorBytes,
                             size_t codeAllocatorBytes);

        // Parse a query
        virtual TermMatchNode const *Parse(const char *query) override;

        // Runs a parsed query
        virtual void Run(TermMatchNode const * tree,
                         QueryInstrumentation & instrumentation,
                         ResultsBuffer & resultsBuffer) override;

        // Adds the diagnostic keyword prefix to the list of prefixes that
        // enable diagnostics.
        virtual void EnableDiagnostic(char const * prefix) override;

        // Removes the diagnostic keyword prefix from the list of prefixes
        // that enable diagnostics.
        virtual void DisableDiagnostic(char const * prefix) override;

    private:
        ISimpleIndex const & m_index;
        IStreamConfiguration const & m_config;
        std::unique_ptr<IDiagnosticStream> m_diagnostic;
        std::unique_ptr<IAllocator> m_matchTreeAllocator;
        std::unique_ptr<NativeJIT::Allocator> m_expressionTreeAllocator;
        std::unique_ptr<NativeJIT::ExecutionBuffer> m_codeAllocator;
        std::unique_ptr<NativeJIT::FunctionBuffer> m_code;

        // First available row pointer register is R8.
        // TODO: is this valid on all platforms or only on Windows?
        static const unsigned c_registerBase = 8;

        // Row pointers stored in the eight registers R8..R15.
        // TODO: is this valid on all platforms or only on Windows?
        static const unsigned c_registerCount = 8;
    };
}

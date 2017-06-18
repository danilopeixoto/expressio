// Copyright (c) 2017, Danilo Peixoto. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef EXPRESSIO_AST_H
#define EXPRESSIO_AST_H

#include "global.h"
#include "types.h"
#include "tree.h"
#include <string>

EXPRESSIO_NAMESPACE_BEGIN

class Symbol {
public:
    enum Type {
        Variable = 0,
        Number,
        Assignment,
        Addition,
        Subtraction,
        Multiplication,
        Division,
        Exponentiation,
        Modulo,
        LParenthesis,
        RParenthesis,
        EndOfFile
    };

    Type type;
    UInt position;

    Symbol(Type, UInt);
    virtual ~Symbol();
};

typedef Symbol * SymbolPointer;
typedef Tree<SymbolPointer> AbstractSyntaxTree;

class VariableSymbol : public Symbol {
public:
    std::string name;
    Float value;
    Bool isOutput;

    VariableSymbol();
    VariableSymbol(std::string, Float = 0, UInt = 0);
    ~VariableSymbol();
};

class NumberSymbol : public Symbol {
public:
    Float value;

    NumberSymbol();
    NumberSymbol(Float, UInt = 0);
    ~NumberSymbol();
};

class OperatorSymbol : public Symbol {
public:
    OperatorSymbol(Type, UInt);
    virtual ~OperatorSymbol();

    virtual SymbolPointer compute(const SymbolPointer &,
        const SymbolPointer &) const = 0;
};

class AssignmentSymbol : public OperatorSymbol {
public:
    AssignmentSymbol(UInt = 0);
    ~AssignmentSymbol();

    SymbolPointer compute(const SymbolPointer &,
        const SymbolPointer &) const override;
};

class AdditionSymbol : public OperatorSymbol {
public:
    AdditionSymbol(UInt = 0);
    ~AdditionSymbol();

    SymbolPointer compute(const SymbolPointer &,
        const SymbolPointer &) const override;
};

class SubtractionSymbol : public OperatorSymbol {
public:
    SubtractionSymbol(UInt = 0);
    ~SubtractionSymbol();

    SymbolPointer compute(const SymbolPointer &,
        const SymbolPointer &) const override;
};

class MultiplicationSymbol : public OperatorSymbol {
public:
    MultiplicationSymbol(UInt = 0);
    ~MultiplicationSymbol();

    SymbolPointer compute(const SymbolPointer &,
        const SymbolPointer &) const override;
};

class DivisionSymbol : public OperatorSymbol {
public:
    DivisionSymbol(UInt = 0);
    ~DivisionSymbol();

    SymbolPointer compute(const SymbolPointer &,
        const SymbolPointer &) const override;
};

class ExponentiationSymbol : public OperatorSymbol {
public:
    ExponentiationSymbol(UInt = 0);
    ~ExponentiationSymbol();

    SymbolPointer compute(const SymbolPointer &,
        const SymbolPointer &) const override;
};

class ModuloSymbol : public OperatorSymbol {
public:
    ModuloSymbol(UInt = 0);
    ~ModuloSymbol();

    SymbolPointer compute(const SymbolPointer &,
        const SymbolPointer &) const override;
};

class LParenthesisSymbol : public Symbol {
public:
    LParenthesisSymbol(UInt = 0);
    ~LParenthesisSymbol();
};

class RParenthesisSymbol : public Symbol {
public:
    RParenthesisSymbol(UInt = 0);
    ~RParenthesisSymbol();
};

class EOFSymbol : public Symbol {
public:
    EOFSymbol(UInt = 0);
    ~EOFSymbol();
};

EXPRESSIO_NAMESPACE_END

#endif
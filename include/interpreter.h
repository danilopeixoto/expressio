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

#ifndef EXPRESSIO_INTERPRETER_H
#define EXPRESSIO_INTERPRETER_H

#include "global.h"
#include "types.h"
#include "queue.h"
#include "ast.h"
#include "translator.h"
#include <string>

EXPRESSIO_NAMESPACE_BEGIN

typedef Queue<SymbolPointer> TokenStream;
typedef Queue<VariableSymbol> VariableTable;
typedef VariableSymbol Result;

struct ErrorContent {
    enum Type {
        UnknownSymbol = 0,
        InvalidExpression,
        UndefinedVariable,
        DivisionByZero,
        None
    };

    Type type;
    UInt position;

    ErrorContent();
    ErrorContent(Type, UInt = 0);
    ~ErrorContent();
};

struct Expression {
public:
    Result output;
    ErrorContent error;

    Expression();
    Expression(Result, ErrorContent = ErrorContent::None);
    ~Expression();
};

class Interpreter {
public:
    Interpreter();
    ~Interpreter();

    Expression run(const std::string &);
    Interpreter & setTranslator(Translator *);
    VariableTable getVariableTable() const;
    Interpreter & clear();

private:
    Translator * translator;
    VariableTable variableTable;

    ErrorContent tokenize(const std::string &, TokenStream &) const;
    ErrorContent parse(const TokenStream &, AbstractSyntaxTree &) const;
    ErrorContent evaluate(AbstractSyntaxTree::NodePointer, SymbolPointer &);

    void deleteTokens(TokenStream &) const;
    Bool isVariable(const std::string &, UInt &) const;
    Bool isNumber(const std::string &, UInt &) const;

    AbstractSyntaxTree::NodePointer literal(TokenStream::ConstIterator &,
        ErrorContent &) const;
    AbstractSyntaxTree::NodePointer factor(TokenStream::ConstIterator &,
        ErrorContent &) const;
    AbstractSyntaxTree::NodePointer term(TokenStream::ConstIterator &,
        ErrorContent &) const;
    AbstractSyntaxTree::NodePointer expression(TokenStream::ConstIterator &,
        ErrorContent &) const;
    AbstractSyntaxTree::NodePointer definition(TokenStream::ConstIterator &,
        ErrorContent &) const;
};

EXPRESSIO_NAMESPACE_END

#endif
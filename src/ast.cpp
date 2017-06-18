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

#include "ast.h"
#include <cmath>

EXPRESSIO_NAMESPACE_BEGIN

Symbol::Symbol(Type type, UInt position) : type(type), position(position) {};
Symbol::~Symbol() {};

VariableSymbol::VariableSymbol()
    : Symbol(Variable, 0), value(0), isOutput(false) {};
VariableSymbol::VariableSymbol(std::string name, Float value, UInt position)
    : Symbol(Variable, position), name(name), value(value), isOutput(false) {}
VariableSymbol::~VariableSymbol() {}

NumberSymbol::NumberSymbol() : Symbol(Number, position), value(0) {}
NumberSymbol::NumberSymbol(Float value, UInt position)
    : Symbol(Number, position), value(value) {}
NumberSymbol::~NumberSymbol() {}

OperatorSymbol::OperatorSymbol(Type type, UInt position)
    : Symbol(type, position) {}
OperatorSymbol::~OperatorSymbol() {}

AssignmentSymbol::AssignmentSymbol(UInt position)
    : OperatorSymbol(Assignment, position) {}
AssignmentSymbol::~AssignmentSymbol() {}

SymbolPointer AssignmentSymbol::compute(const SymbolPointer & lhs,
    const SymbolPointer & rhs) const {
    return new VariableSymbol(((VariableSymbol *)lhs)->name,
        ((NumberSymbol *)rhs)->value, position);
}

AdditionSymbol::AdditionSymbol(UInt position)
    : OperatorSymbol(Addition, position) {}
AdditionSymbol::~AdditionSymbol() {}

SymbolPointer AdditionSymbol::compute(const SymbolPointer & lhs,
    const SymbolPointer & rhs) const {
    return new NumberSymbol(((NumberSymbol *)lhs)->value
        + ((NumberSymbol *)rhs)->value, position);
}

SubtractionSymbol::SubtractionSymbol(UInt position)
    : OperatorSymbol(Subtraction, position) {}
SubtractionSymbol::~SubtractionSymbol() {}

SymbolPointer SubtractionSymbol::compute(const SymbolPointer & lhs,
    const SymbolPointer & rhs) const {
    return new NumberSymbol(((NumberSymbol *)lhs)->value
        - ((NumberSymbol *)rhs)->value, position);
}

MultiplicationSymbol::MultiplicationSymbol(UInt position)
    : OperatorSymbol(Multiplication, position) {}
MultiplicationSymbol::~MultiplicationSymbol() {}

SymbolPointer MultiplicationSymbol::compute(const SymbolPointer & lhs,
    const SymbolPointer & rhs) const {
    return new NumberSymbol(((NumberSymbol *)lhs)->value
        * ((NumberSymbol *)rhs)->value, position);
}

DivisionSymbol::DivisionSymbol(UInt position) : OperatorSymbol(Division, position) {}
DivisionSymbol::~DivisionSymbol() {}

SymbolPointer DivisionSymbol::compute(const SymbolPointer & lhs,
    const SymbolPointer & rhs) const {
    return new NumberSymbol(((NumberSymbol *)lhs)->value
        / ((NumberSymbol *)rhs)->value, position);
}

ExponentiationSymbol::ExponentiationSymbol(UInt position)
    : OperatorSymbol(Exponentiation, position) {}
ExponentiationSymbol::~ExponentiationSymbol() {}

SymbolPointer ExponentiationSymbol::compute(const SymbolPointer & lhs,
    const SymbolPointer & rhs) const {
    return new NumberSymbol(std::pow(((NumberSymbol *)lhs)->value,
        ((NumberSymbol *)rhs)->value), position);
}

ModuloSymbol::ModuloSymbol(UInt position) : OperatorSymbol(Modulo, position) {}
ModuloSymbol::~ModuloSymbol() {}

SymbolPointer ModuloSymbol::compute(const SymbolPointer & lhs,
    const SymbolPointer & rhs) const {
    return new NumberSymbol(std::fmod(((NumberSymbol *)lhs)->value,
        ((NumberSymbol *)rhs)->value), position);
}

LParenthesisSymbol::LParenthesisSymbol(UInt position)
    : Symbol(LParenthesis, position) {}
LParenthesisSymbol::~LParenthesisSymbol() {}

RParenthesisSymbol::RParenthesisSymbol(UInt position)
    : Symbol(RParenthesis, position) {}
RParenthesisSymbol::~RParenthesisSymbol() {}

EOFSymbol::EOFSymbol(UInt position)
    : Symbol(EndOfFile, position) {}
EOFSymbol::~EOFSymbol() {}

EXPRESSIO_NAMESPACE_END
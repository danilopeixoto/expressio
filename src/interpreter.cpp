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

#include "interpreter.h"
#include <cctype>

EXPRESSIO_NAMESPACE_BEGIN

ErrorContent::ErrorContent() : type(None), position(0) {}
ErrorContent::ErrorContent(Type type, UInt position)
    : type(type), position(position) {}
ErrorContent::~ErrorContent() {}

Expression::Expression() : error(ErrorContent::None) {}
Expression::Expression(Result output, ErrorContent error)
    : output(output), error(error) {}
Expression::~Expression() {}

Interpreter::Interpreter() {}
Interpreter::~Interpreter() {}

Expression Interpreter::run(const std::string & source) {
    Expression expression;

    TokenStream tokens;
    expression.error = tokenize(source, tokens);

    if (expression.error.type != ErrorContent::None) {
        deleteTokens(tokens);

        return expression;
    }

    AbstractSyntaxTree ast;
    expression.error = parse(tokens, ast);

    if (expression.error.type != ErrorContent::None) {
        deleteTokens(tokens);

        return expression;
    }

    SymbolPointer result = EXPRESSIO_NULL;
    expression.error = evaluate(ast.getRoot(), result);

    if (expression.error.type != ErrorContent::None) {
        deleteTokens(tokens);

        if (result != EXPRESSIO_NULL)
            delete result;

        return expression;
    }

    if (result->type == Symbol::Variable) {
        VariableSymbol * variableSymbol = (VariableSymbol *)result;

        VariableTable::Iterator it(variableTable.getBegin());
        Bool find = false;

        while (it != variableTable.getEnd()) {
            if (it->name == variableSymbol->name) {
                find = true;
                it->value = variableSymbol->value;

                break;
            }

            it++;
        }

        if (!find)
            variableTable.insert(*variableSymbol);

        variableSymbol->isOutput = true;
        expression.output = *variableSymbol;
    }
    else
        expression.output = Result("", ((NumberSymbol *)result)->value);

    delete result;

    deleteTokens(tokens);

    return expression;
}
Interpreter & Interpreter::setTranslator(Translator * translator) {
    this->translator = translator;

    return *this;
}
VariableTable Interpreter::getVariableTable() const {
    return variableTable;
}
Interpreter & Interpreter::clear() {
    variableTable.clear();

    return *this;
}

ErrorContent Interpreter::tokenize(const std::string & source,
    TokenStream & tokens) const {
    UInt i;

    for (i = 0; i < source.length(); i++) {
        Character c = source[i];

        if (c < -1 || c > 255)
            return ErrorContent(ErrorContent::UnknownSymbol, i);

        if (std::isspace(c))
            continue;

        UInt s;

        if (isVariable(source.substr(i), s)) {
            tokens.insert(new VariableSymbol(source.substr(i, s), 0, i));
            i += s - 1;
        }
        else if (isNumber(source.substr(i), s)) {
            tokens.insert(new NumberSymbol(std::stod(source.substr(i, s)), i));
            i += s - 1;
        }
        else {
            switch (c) {
            case '+':
                tokens.insert(new AdditionSymbol(i));
                break;
            case '-':
                tokens.insert(new SubtractionSymbol(i));
                break;
            case '*':
                tokens.insert(new MultiplicationSymbol(i));
                break;
            case '/':
                tokens.insert(new DivisionSymbol(i));
                break;
            case '^':
                tokens.insert(new ExponentiationSymbol(i));
                break;
            case '%':
                tokens.insert(new ModuloSymbol(i));
                break;
            case '=':
                tokens.insert(new AssignmentSymbol(i));
                break;
            case '(':
                tokens.insert(new LParenthesisSymbol(i));
                break;
            case ')':
                tokens.insert(new RParenthesisSymbol(i));
                break;
            default:
                return ErrorContent(ErrorContent::UnknownSymbol, i);
            }
        }
    }

    tokens.insert(new EOFSymbol(i));

    return ErrorContent(ErrorContent::None);
}
ErrorContent Interpreter::parse(const TokenStream & tokens,
    AbstractSyntaxTree & ast) const {
    TokenStream::ConstIterator token(tokens.getBegin());
    ErrorContent error;

    AbstractSyntaxTree::NodePointer node = definition(token, error);
    ast.setRoot(node);

    if (error.type != ErrorContent::None) {
        ast.clear();

        token = tokens.getBegin();
        error = ErrorContent(ErrorContent::None);

        node = expression(token, error);

        if ((*token)->type != Symbol::EndOfFile)
            error = ErrorContent(ErrorContent::InvalidExpression, (*token)->position);

        ast.setRoot(node);
    }

    return error;
}
ErrorContent Interpreter::evaluate(AbstractSyntaxTree::NodePointer node,
    SymbolPointer & result) {
    if (node == EXPRESSIO_NULL)
        return ErrorContent(ErrorContent::InvalidExpression);

    if (node->left == EXPRESSIO_NULL && node->right == EXPRESSIO_NULL) {
        SymbolPointer symbol = node->data;

        if (symbol->type == Symbol::Variable) {
            VariableSymbol * variableSymbol = (VariableSymbol *)symbol;

            VariableTable::Iterator it(variableTable.getBegin());
            Bool find = false;

            while (it != variableTable.getEnd()) {
                if (it->name == variableSymbol->name) {
                    find = true;
                    result = new NumberSymbol(it->value, variableSymbol->position);

                    break;
                }

                it++;
            }

            if (!find)
                return ErrorContent(ErrorContent::UndefinedVariable, variableSymbol->position);
        }
        else if (symbol->type == Symbol::Number) {
            NumberSymbol * numberSymbol = (NumberSymbol *)symbol;
            result = new NumberSymbol(numberSymbol->value, numberSymbol->position);
        }
        else
            return ErrorContent(ErrorContent::InvalidExpression);
    }
    else {
        OperatorSymbol * operatorSymbol = (OperatorSymbol *)node->data;

        ErrorContent error;
        SymbolPointer lhs, rhs;

        if (operatorSymbol->type == Symbol::Assignment) {
            VariableSymbol * variableSymbol = (VariableSymbol *)node->left->data;
            lhs = new VariableSymbol(variableSymbol->name, 0, variableSymbol->position);
        }
        else {
            error = evaluate(node->left, lhs);

            if (error.type != ErrorContent::None)
                return error;
        }

        error = evaluate(node->right, rhs);

        if (error.type != ErrorContent::None) {
            delete lhs;

            return error;
        }

        if (operatorSymbol->type == Symbol::Division && rhs->type == Symbol::Number) {
            if (((NumberSymbol *)rhs)->value == 0) {
                delete lhs, rhs;

                return ErrorContent(ErrorContent::DivisionByZero, operatorSymbol->position + 1);
            }
        }

        result = operatorSymbol->compute(lhs, rhs);

        delete lhs, rhs;
    }

    return ErrorContent(ErrorContent::None);
}

void Interpreter::deleteTokens(TokenStream & tokens) const {
    TokenStream::Iterator it(tokens.getBegin());

    while (it != tokens.getEnd()) {
        if (*it != EXPRESSIO_NULL)
            delete *it;

        it++;
    }

    tokens.clear();
}
Bool Interpreter::isVariable(const std::string & string, UInt & size) const {
    std::string::const_iterator c(string.begin());

    if (!std::isalpha(*c))
        return false;

    while (c != string.end() && std::isalpha(*c))
        c++;

    size = c - string.begin();

    return true;
}
Bool Interpreter::isNumber(const std::string & string, UInt & size) const {
    std::string::const_iterator c(string.begin());

    if (!std::isdigit(*c))
        return false;

    while (c != string.end() && std::isdigit(*c))
        c++;

    if (c != string.end() && *c == translator->DECIMAL_SEPARATOR) {
        if (++c == string.end())
            return false;

        if (!std::isdigit(*c))
            return false;

        while (c != string.end() && std::isdigit(*c))
            c++;
    }

    if (c != string.end() && *c == 'e') {
        if (++c == string.end())
            return false;

        if (*c == '+' || *c == '-') {
            if (++c == string.end())
                return false;
        }

        if (!std::isdigit(*c))
            return false;

        while (c != string.end() && std::isdigit(*c))
            c++;
    }

    size = c - string.begin();

    return true;
}

AbstractSyntaxTree::NodePointer Interpreter::literal(TokenStream::ConstIterator & token,
    ErrorContent & error) const {
    if ((*token)->type == Symbol::Variable || (*token)->type == Symbol::Number)
        return new AbstractSyntaxTree::Node(*token++);
    else if ((*token)->type == Symbol::LParenthesis) {
        AbstractSyntaxTree::NodePointer expr = expression(++token, error);

        if (error.type != ErrorContent::None)
            return expr;

        if ((*token)->type == Symbol::RParenthesis) {
            token++;

            return expr;
        }
    }

    error = ErrorContent(ErrorContent::InvalidExpression, (*token)->position);

    return EXPRESSIO_NULL;
}
AbstractSyntaxTree::NodePointer Interpreter::factor(TokenStream::ConstIterator & token,
    ErrorContent & error) const {
    AbstractSyntaxTree::NodePointer node = literal(token, error);

    if (error.type != ErrorContent::None)
        return node;

    while ((*token)->type == Symbol::Exponentiation) {
        node = new AbstractSyntaxTree::Node(*token++, node);
        node->right = literal(token, error);

        if (error.type != ErrorContent::None)
            return node;
    }

    return node;
}
AbstractSyntaxTree::NodePointer Interpreter::term(TokenStream::ConstIterator & token,
    ErrorContent & error) const {
    AbstractSyntaxTree::NodePointer node = factor(token, error);

    if (error.type != ErrorContent::None)
        return node;

    while ((*token)->type == Symbol::Multiplication || (*token)->type == Symbol::Division
        || (*token)->type == Symbol::Modulo) {
        node = new AbstractSyntaxTree::Node(*token++, node);
        node->right = factor(token, error);

        if (error.type != ErrorContent::None)
            return node;
    }

    return node;
}
AbstractSyntaxTree::NodePointer Interpreter::expression(TokenStream::ConstIterator & token,
    ErrorContent & error) const {
    AbstractSyntaxTree::NodePointer node = term(token, error);

    if (error.type != ErrorContent::None)
        return node;

    while ((*token)->type == Symbol::Addition || (*token)->type == Symbol::Subtraction) {
        node = new AbstractSyntaxTree::Node(*token++, node);
        node->right = term(token, error);

        if (error.type != ErrorContent::None)
            return node;
    }

    return node;
}
AbstractSyntaxTree::NodePointer Interpreter::definition(TokenStream::ConstIterator & token,
    ErrorContent & error) const {
    if ((*token)->type != Symbol::Variable) {
        error = ErrorContent(ErrorContent::InvalidExpression, (*token)->position);

        return EXPRESSIO_NULL;
    }

    AbstractSyntaxTree::NodePointer node = new AbstractSyntaxTree::Node;
    node->left = new AbstractSyntaxTree::Node(*token++);

    if ((*token)->type != Symbol::Assignment) {
        error = ErrorContent(ErrorContent::InvalidExpression, (*token)->position);

        return node;
    }

    node->data = *token++;

    AbstractSyntaxTree::NodePointer expr = expression(token, error);

    if (error.type != ErrorContent::None)
        return node;

    node->right = expr;

    return node;
}

EXPRESSIO_NAMESPACE_END
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

#include "translator.h"
#include <locale>

EXPRESSIO_NAMESPACE_BEGIN

Translator::Translator() {
    setEnglishLanguage();
}
Translator::~Translator() {}

Translator & Translator::setLanguage(const Language & language) {
    switch (language) {
    case Language::Portuguese:
        setPortugueseLanguage();
        break;
    default:
        setEnglishLanguage();
    }

    return *this;
}
Translator::Language Translator::getLanguage() const {
    return language;
}

Translator & Translator::setEnglishLanguage() {
    language = English;
    setlocale(LC_ALL, "english");

    EDITOR = "Editor";
    CLEAR = "Clear";
    EXPORT = "Export";
    PREFERENCES = "Preferences";
    ABOUT = "About";
    EXIT = "Exit";

    EDITOR_MESSAGE = "Enter a mathematical expression:";
    CLEAR_MESSAGE = "Are you sure you want to clear the history list?";
    EXPORT_MESSAGE = "Enter a filename to export the history list:";
    LANGUAGE_MESSAGE = "Select a language for application:";
    THEME_MESSAGE = "Select a theme for application:";
    ABOUT_MESSAGE = "A simple mathematical expression solver.";
    COPYRIGHT_MESSAGE = "Copyright (c) 2017, Danilo Peixoto. All rights reserved.";
    LICENSE_MESSAGE = "Software developed under the BSD-3-Clause license.";
    EXIT_MESSAGE = "Are you sure you want to close the application?";

    EMPTY_HISTORY_LIST_WARNING = "History list is empty.";

    UNKNOWN_SYMBOL_ERROR = "Error: unknown symbol.";
    INVALID_EXPRESSION_ERROR = "Error: invalid expression.";
    UNDEFINED_VARIABLE_ERROR = "Error: undefined variable.";
    DIVISION_BY_ZERO_ERROR = "Error: division by zero.";

    ENGLISH = "English";
    PORTUGUESE = "Portuguese";
    LIGHT = "Light";
    DARK = "Dark";
    DEFAULT = "Default";
    SAVE = "Save";

    YES = "Yes";
    NO = "No";
    OK = "OK";
    CANCEL = "Cancel";
    CONTINUE = "Continue";

    DECIMAL_SEPARATOR = '.';

    return *this;
}
Translator & Translator::setPortugueseLanguage() {
    language = Portuguese;
    setlocale(LC_ALL, "portuguese");

    EDITOR = "Editor";
    CLEAR = "Limpar";
    EXPORT = "Exportar";
    PREFERENCES = "Preferências";
    ABOUT = "Sobre";
    EXIT = "Sair";

    EDITOR_MESSAGE = "Insira uma expressão matemática:";
    CLEAR_MESSAGE = "Você tem certeza que deseja limpar a lista de histórico?";
    EXPORT_MESSAGE = "Insira um nome de arquivo para exportar a lista de histórico:";
    LANGUAGE_MESSAGE = "Selecione um idioma para aplicação:";
    THEME_MESSAGE = "Selecione um tema para aplicação:";
    ABOUT_MESSAGE = "Um simples solucionador de expressão matemática.";
    COPYRIGHT_MESSAGE = "Copyright (c) 2017, Danilo Peixoto. Todos os direitos reservados.";
    LICENSE_MESSAGE = "Programa desenvolvido sob a licença BSD-3-Clause.";
    EXIT_MESSAGE = "Você tem certeza que deseja fechar a aplicação?";

    EMPTY_HISTORY_LIST_WARNING = "A lista de histórico está vazia.";

    UNKNOWN_SYMBOL_ERROR = "Erro: símbolo desconhecido.";
    INVALID_EXPRESSION_ERROR = "Erro: expressão inválida.";
    UNDEFINED_VARIABLE_ERROR = "Erro: variável indefinida.";
    DIVISION_BY_ZERO_ERROR = "Erro: divisão por zero.";

    ENGLISH = "Inglês";
    PORTUGUESE = "Português";
    LIGHT = "Claro";
    DARK = "Escuro";
    DEFAULT = "Padrão";
    SAVE = "Salvar";

    YES = "Sim";
    NO = "Não";
    OK = "OK";
    CANCEL = "Cancelar";
    CONTINUE = "Continuar";

    DECIMAL_SEPARATOR = ',';

    return *this;
}

EXPRESSIO_NAMESPACE_END
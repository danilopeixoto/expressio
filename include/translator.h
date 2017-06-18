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

#ifndef EXPRESSIO_TRANSLATOR_H
#define EXPRESSIO_TRANSLATOR_H

#include "global.h"
#include "types.h"

EXPRESSIO_NAMESPACE_BEGIN

class Translator {
public:
    enum Language {
        English = 0,
        Portuguese,
        Default
    };

    Character * EDITOR;
    Character * CLEAR;
    Character * EXPORT;
    Character * PREFERENCES;
    Character * ABOUT;
    Character * EXIT;

    Character * EDITOR_MESSAGE;
    Character * CLEAR_MESSAGE;
    Character * EXPORT_MESSAGE;
    Character * LANGUAGE_MESSAGE;
    Character * THEME_MESSAGE;
    Character * ABOUT_MESSAGE;
    Character * COPYRIGHT_MESSAGE;
    Character * LICENSE_MESSAGE;
    Character * EXIT_MESSAGE;

    Character * EMPTY_HISTORY_LIST_WARNING;

    Character * UNKNOWN_SYMBOL_ERROR;
    Character * INVALID_EXPRESSION_ERROR;
    Character * UNDEFINED_VARIABLE_ERROR;
    Character * DIVISION_BY_ZERO_ERROR;

    Character * ENGLISH;
    Character * PORTUGUESE;
    Character * LIGHT;
    Character * DARK;
    Character * DEFAULT;
    Character * SAVE;

    Character * YES;
    Character * NO;
    Character * OK;
    Character * CANCEL;
    Character * CONTINUE;

    Character DECIMAL_SEPARATOR;

    Translator();
    ~Translator();

    Translator & setLanguage(const Language &);
    Language getLanguage() const;

private:
    Language language;

    Translator & setEnglishLanguage();
    Translator & setPortugueseLanguage();
};

EXPRESSIO_NAMESPACE_END

#endif
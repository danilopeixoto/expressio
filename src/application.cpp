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

#include "application.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include <fstream>

EXPRESSIO_NAMESPACE_BEGIN

Application::Preferences::Preferences(const Translator::Language & language,
    const Theme & theme) : language(language), theme(theme) {}
Application::Preferences::~Preferences() {}

Application::Application() {
    preferenceFile = fopen("preferences", "rb+");

    if (preferenceFile != EXPRESSIO_NULL) {
        fread(&preferences, sizeof(Preferences), 1, preferenceFile);
    }
    else {
        preferenceFile = fopen("preferences", "wb+");

        if (preferenceFile != EXPRESSIO_NULL)
            fwrite(&preferences, sizeof(Preferences), 1, preferenceFile);
    }

    fclose(preferenceFile);

    translator.setLanguage(preferences.language);

#if _WIN64
    setTheme(preferences.theme);
#endif

    interpreter.setTranslator(&translator);

    clear();
}
Application::~Application() {
    if (preferenceFile != EXPRESSIO_NULL)
        fclose(preferenceFile);
}

UInt Application::execute() {
    clear();

    createTitle();
    separator();

    if (!message.empty()) {
        print(message);
        separator();

        message.clear();
    }

    Queue<std::string> menu;
    menu.insert(translator.EDITOR);
    menu.insert(translator.CLEAR);
    menu.insert(translator.EXPORT);
    menu.insert(translator.PREFERENCES);
    menu.insert(translator.ABOUT);
    menu.insert(translator.EXIT);

    createMenu(menu);
    separator();

    requestOption();

    switch (option) {
    case 1:
        editorAction();
        break;
    case 2:
        if (!historyList.isEmpty())
            clearAction();
        else {
            message = translator.EMPTY_HISTORY_LIST_WARNING;
            execute();
        }

        break;
    case 3:
        if (!historyList.isEmpty())
            exportAction();
        else {
            message = translator.EMPTY_HISTORY_LIST_WARNING;
            execute();
        }

        break;
    case 4:
        preferencesAction();
        break;
    case 5:
        aboutAction();
        break;
    case 6:
        exitAction();
        break;
    default:
        execute();
    }

    return 0;
}

void Application::setTheme(const Theme & theme) {
    switch (theme) {
    case Theme::Dark:
        system("color 0F");
        break;
    default:
        system("color F0");
    }
}
void Application::requestText(std::string & string) {
    std::getline(std::cin, string);
}
void Application::requestOption() {
    std::string stringOption;

    requestText(stringOption);
    stringOption = stringOption.substr(0, EXPRESSIO_MAX_OPTION_LENGTH);

    option = isUInt(stringOption) ? std::stoi(stringOption) : 0;
}

void Application::createTitle() {
    print("* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *");
    print("*                                  EXPRESSIO                                  *");
    print("* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *");
}
void Application::createMenu(const Queue<std::string> & menu) {
    Queue<std::string>::ConstIterator it = menu.getBegin();

    for (UInt i = 0; i < menu.getSize(); i++)
        std::cout << i + 1 << ". " << *it++ << std::endl;
}
void Application::print(const std::string & string) {
    std::cout << string << std::endl;
}
void Application::separator() {
    std::cout << std::endl;
}
void Application::clear() {
#ifdef _WIN64
    system("cls");
#else
    system("clear");
#endif
}

Bool Application::isUInt(const std::string & string) const {
    UInt type;
    std::stringstream sstream(string);

    sstream >> std::noskipws >> type;

    return sstream && sstream.eof();
}

void Application::editorAction() {
    clear();

    createTitle();
    separator();

    print(translator.EDITOR);
    separator();

    print(translator.EDITOR_MESSAGE);
    separator();

    if (!historyList.isEmpty()) {
        Queue<std::string>::Iterator it(historyList.getBegin());

        while (it != historyList.getEnd())
            print(*it++);

        separator();
    }

    std::string source, result;

    do {
        requestText(source);
        historyList.insert(source);

        Expression expression = interpreter.run(source);

        if (expression.error.type == ErrorContent::None) {
            Result variable = expression.output;

            result = ">> ";

            if (variable.isOutput)
                result += variable.name + " = ";

            std::stringstream sstream;
            sstream << std::fixed << std::setprecision(5) << variable.value;

            result += sstream.str();
        }
        else {
            result += std::string(expression.error.position, ' ');
            result += "^\n>> ";

            switch (expression.error.type) {
            case ErrorContent::UnknownSymbol:
                result += translator.UNKNOWN_SYMBOL_ERROR;
                break;
            case ErrorContent::InvalidExpression:
                result += translator.INVALID_EXPRESSION_ERROR;
                break;
            case ErrorContent::UndefinedVariable:
                result += translator.UNDEFINED_VARIABLE_ERROR;
                break;
            case ErrorContent::DivisionByZero:
                result += translator.DIVISION_BY_ZERO_ERROR;
                break;
            }
        }

        historyList.insert(result);

        print(result);
        separator();

        result.clear();

        Queue<std::string> menu;
        menu.insert(translator.CONTINUE);
        menu.insert(translator.CANCEL);
        menu.insert(translator.EXIT);

        createMenu(menu);
        separator();

        requestOption();
        separator();
    } while (option == 1);

    switch (option) {
    case 2:
        execute();
        break;
    case 3:
        exitAction();
        break;
    default:
        editorAction();
    }
}
void Application::clearAction() {
    clear();

    createTitle();
    separator();

    print(translator.CLEAR);
    separator();

    print(translator.CLEAR_MESSAGE);
    separator();

    Queue<std::string> menu;
    menu.insert(translator.YES);
    menu.insert(translator.NO);
    menu.insert(translator.EXIT);

    createMenu(menu);
    separator();

    requestOption();
    separator();

    switch (option) {
    case 1:
        historyList.clear();
        interpreter.clear();

        execute();

        break;
    case 2:
        execute();
        break;
    case 3:
        exitAction();
        break;
    default:
        clearAction();
    }
}
void Application::exportAction() {
    clear();

    createTitle();
    separator();

    print(translator.EXPORT);
    separator();

    print(translator.EXPORT_MESSAGE);

    std::string filename;
    std::fstream file;

    requestText(filename);
    separator();

    Queue<std::string> menu;
    menu.insert(translator.EXPORT);
    menu.insert(translator.CANCEL);
    menu.insert(translator.EXIT);

    createMenu(menu);
    separator();

    requestOption();
    separator();

    switch (option) {
    case 1:
        file.open(filename, std::fstream::out);

        if (file.is_open()) {
            Queue<std::string>::Iterator it(historyList.getBegin());

            while (it != historyList.getEnd())
                file << *it++ << std::endl;

            file.close();

            execute();
        }
        else
            exportAction();

        break;
    case 2:
        execute();
        break;
    case 3:
        exitAction();
        break;
    default:
        exportAction();
    }
}
void Application::preferencesAction() {
    clear();

    createTitle();
    separator();

    print(translator.PREFERENCES);
    separator();

    print(translator.LANGUAGE_MESSAGE);
    separator();

    Queue<std::string> menu;
    menu.insert(translator.ENGLISH);
    menu.insert(translator.PORTUGUESE);
    menu.insert(translator.DEFAULT);

    createMenu(menu);
    separator();

    requestOption();
    separator();

    if (option < 1 || option > menu.getSize())
        preferencesAction();

    preferences.language = (Translator::Language)(option - 1);

#ifdef _WIN64
    print(translator.THEME_MESSAGE);
    separator();

    menu.clear();
    menu.insert(translator.LIGHT);
    menu.insert(translator.DARK);
    menu.insert(translator.DEFAULT);

    createMenu(menu);
    separator();

    requestOption();
    separator();

    if (option < 1 || option > menu.getSize())
        preferencesAction();

    preferences.theme = (Theme)(option - 1);
#endif

    menu.clear();
    menu.insert(translator.SAVE);
    menu.insert(translator.CANCEL);
    menu.insert(translator.EXIT);

    createMenu(menu);
    separator();

    requestOption();

    switch (option) {
    case 1:
        preferenceFile = fopen("preferences", "wb+");

        if (preferenceFile != EXPRESSIO_NULL)
            fwrite(&preferences, sizeof(Preferences), 1, preferenceFile);

        fclose(preferenceFile);

        if (preferences.language != translator.getLanguage()) {
            historyList.clear();
            interpreter.clear();
        }

        translator.setLanguage(preferences.language);

#ifdef _WIN64
        setTheme(preferences.theme);
#endif

        execute();

        break;
    case 2:
        execute();
        break;
    case 3:
        exitAction();
        break;
    default:
        preferencesAction();
    }
}
void Application::aboutAction() {
    clear();

    createTitle();
    separator();

    print(translator.ABOUT);
    separator();

    print(translator.ABOUT_MESSAGE);
    separator();

    print(translator.COPYRIGHT_MESSAGE);
    print(translator.LICENSE_MESSAGE);
    separator();

    Queue<std::string> menu;
    menu.insert(translator.OK);
    menu.insert(translator.EXIT);

    createMenu(menu);
    separator();

    requestOption();
    separator();

    switch (option) {
    case 1:
        execute();
        break;
    case 2:
        exitAction();
        break;
    default:
        aboutAction();
    }
}
void Application::exitAction() {
    clear();

    createTitle();
    separator();

    print(translator.EXIT);
    separator();

    print(translator.EXIT_MESSAGE);
    separator();

    Queue<std::string> menu;
    menu.insert(translator.YES);
    menu.insert(translator.NO);

    createMenu(menu);
    separator();

    requestOption();
    separator();

    switch (option) {
    case 1:
        return;
        break;
    case 2:
        execute();
        break;
    default:
        exitAction();
    }
}

EXPRESSIO_NAMESPACE_END
/*
 * Copyright (C) 2010 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"

#include "TestShell.h"
#include "webkit/support/webkit_support.h"
#include <wtf/Vector.h>

using namespace std;

static const char optionComplexText[] = "--complex-text";
static const char optionDumpAllPixels[] = "--dump-all-pixels";
static const char optionNotree[] = "--notree";
static const char optionPixelTests[] = "--pixel-tests";
static const char optionThreaded[] = "--threaded";
static const char optionTree[] = "--tree";

static void runTest(TestShell& shell, TestParams& params, const string& testName)
{
    string pathOrURL = testName;
    string::size_type separatorPosition = pathOrURL.find("'");
    if (separatorPosition != string::npos) {
        params.pixelHash = pathOrURL.substr(separatorPosition + 1);
        pathOrURL.erase(separatorPosition);
    }
    params.testUrl = webkit_support::CreateURLForPathOrURL(pathOrURL);
    shell.resetTestController();
    shell.runFileTest(params);
}

int main(int argc, char* argv[])
{
    webkit_support::SetUpTestEnvironment();

    TestParams params;
    Vector<string> tests;
    bool serverMode = false;
    for (int i = 1; i < argc; ++i) {
        string argument(argv[i]);
        if (argument == "-")
            serverMode = true;
        else if (argument == optionNotree)
            params.dumpTree = false;
        else if (argument == optionPixelTests)
            params.dumpPixels = true;
        else if (argument.size() && argument[0] == '-')
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
        else
            tests.append(argument);
    }

    { // Explicit scope for the TestShell instance.
        TestShell shell;
        if (serverMode && !tests.size()) {
            params.printSeparators = true;
            char testString[2048]; // 2048 is the same as the sizes of other platforms.
            while (fgets(testString, sizeof(testString), stdin)) {
                char* newLinePosition = strchr(testString, '\n');
                if (newLinePosition)
                    *newLinePosition = '\0';
                if (testString[0] == '\0')
                    continue;
                runTest(shell, params, testString);
            }
        } else if (!tests.size())
            printf("#EOF\n");
        else {
            params.printSeparators = tests.size() > 1;
            for (unsigned i = 0; i < tests.size(); i++)
                runTest(shell, params, tests[i]);
        }

        shell.callJSGC();
        shell.callJSGC();

        // When we finish the last test, cleanup the LayoutTestController.
        // It may have references to not-yet-cleaned up windows.  By
        // cleaning up here we help purify reports.
        shell.resetTestController();
    }

    webkit_support::TearDownTestEnvironment();
    return EXIT_SUCCESS;
}

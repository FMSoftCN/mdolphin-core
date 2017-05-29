/*
 * THIS FILE WAS AUTOMATICALLY GENERATED, DO NOT EDIT.
 *
 * This file was generated by the dom/make_names.pl script.
 *
 * Copyright (C) 2005, 2006, 2007, 2008, 2009 Apple Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"

#if ENABLE(WML)

#include "WMLElementFactory.h"
#include "WMLNames.h"
#include "WMLAElement.h"
#include "WMLAccessElement.h"
#include "WMLAnchorElement.h"
#include "WMLBRElement.h"
#include "WMLCardElement.h"
#include "WMLDoElement.h"
#include "WMLFieldSetElement.h"
#include "WMLGoElement.h"
#include "WMLElement.h"
#include "WMLImageElement.h"
#include "WMLInputElement.h"
#include "WMLInsertedLegendElement.h"
#include "WMLMetaElement.h"
#include "WMLNoopElement.h"
#include "WMLOnEventElement.h"
#include "WMLOptGroupElement.h"
#include "WMLOptionElement.h"
#include "WMLPElement.h"
#include "WMLPostfieldElement.h"
#include "WMLPrevElement.h"
#include "WMLRefreshElement.h"
#include "WMLSelectElement.h"
#include "WMLSetvarElement.h"
#include "WMLTableElement.h"
#include "WMLTemplateElement.h"
#include "WMLTimerElement.h"
#include <wtf/HashMap.h>

#if ENABLE(DASHBOARD_SUPPORT) || ENABLE(VIDEO)
#include "Document.h"
#include "Settings.h"
#endif

namespace WebCore {

using namespace WMLNames;

typedef PassRefPtr<WMLElement> (*ConstructorFunction)(const QualifiedName&, Document*, bool createdByParser);
typedef HashMap<AtomicStringImpl*, ConstructorFunction> FunctionMap;

static FunctionMap* gFunctionMap = 0;

static PassRefPtr<WMLElement> aConstructor(const QualifiedName& tagName, Document* document, bool)
{
    return WMLAElement::create(tagName, document);
}

static PassRefPtr<WMLElement> accessConstructor(const QualifiedName& tagName, Document* document, bool)
{
    return WMLAccessElement::create(tagName, document);
}

static PassRefPtr<WMLElement> anchorConstructor(const QualifiedName& tagName, Document* document, bool)
{
    return WMLAnchorElement::create(tagName, document);
}

static PassRefPtr<WMLElement> brConstructor(const QualifiedName& tagName, Document* document, bool)
{
    return WMLBRElement::create(tagName, document);
}

static PassRefPtr<WMLElement> cardConstructor(const QualifiedName& tagName, Document* document, bool)
{
    return WMLCardElement::create(tagName, document);
}

static PassRefPtr<WMLElement> doConstructor(const QualifiedName& tagName, Document* document, bool)
{
    return WMLDoElement::create(tagName, document);
}

static PassRefPtr<WMLElement> fieldsetConstructor(const QualifiedName& tagName, Document* document, bool)
{
    return WMLFieldSetElement::create(tagName, document);
}

static PassRefPtr<WMLElement> goConstructor(const QualifiedName& tagName, Document* document, bool)
{
    return WMLGoElement::create(tagName, document);
}

static PassRefPtr<WMLElement> imageConstructor(const QualifiedName& tagName, Document* document, bool)
{
    return WMLImageElement::create(tagName, document);
}

static PassRefPtr<WMLElement> inputConstructor(const QualifiedName& tagName, Document* document, bool)
{
    return WMLInputElement::create(tagName, document);
}

static PassRefPtr<WMLElement> insertedlegendConstructor(const QualifiedName& tagName, Document* document, bool)
{
    return WMLInsertedLegendElement::create(tagName, document);
}

static PassRefPtr<WMLElement> metaConstructor(const QualifiedName& tagName, Document* document, bool)
{
    return WMLMetaElement::create(tagName, document);
}

static PassRefPtr<WMLElement> noopConstructor(const QualifiedName& tagName, Document* document, bool)
{
    return WMLNoopElement::create(tagName, document);
}

static PassRefPtr<WMLElement> oneventConstructor(const QualifiedName& tagName, Document* document, bool)
{
    return WMLOnEventElement::create(tagName, document);
}

static PassRefPtr<WMLElement> optgroupConstructor(const QualifiedName& tagName, Document* document, bool)
{
    return WMLOptGroupElement::create(tagName, document);
}

static PassRefPtr<WMLElement> optionConstructor(const QualifiedName& tagName, Document* document, bool)
{
    return WMLOptionElement::create(tagName, document);
}

static PassRefPtr<WMLElement> pConstructor(const QualifiedName& tagName, Document* document, bool)
{
    return WMLPElement::create(tagName, document);
}

static PassRefPtr<WMLElement> postfieldConstructor(const QualifiedName& tagName, Document* document, bool)
{
    return WMLPostfieldElement::create(tagName, document);
}

static PassRefPtr<WMLElement> prevConstructor(const QualifiedName& tagName, Document* document, bool)
{
    return WMLPrevElement::create(tagName, document);
}

static PassRefPtr<WMLElement> refreshConstructor(const QualifiedName& tagName, Document* document, bool)
{
    return WMLRefreshElement::create(tagName, document);
}

static PassRefPtr<WMLElement> selectConstructor(const QualifiedName& tagName, Document* document, bool)
{
    return WMLSelectElement::create(tagName, document);
}

static PassRefPtr<WMLElement> setvarConstructor(const QualifiedName& tagName, Document* document, bool)
{
    return WMLSetvarElement::create(tagName, document);
}

static PassRefPtr<WMLElement> tableConstructor(const QualifiedName& tagName, Document* document, bool)
{
    return WMLTableElement::create(tagName, document);
}

static PassRefPtr<WMLElement> templateConstructor(const QualifiedName& tagName, Document* document, bool)
{
    return WMLTemplateElement::create(tagName, document);
}

static PassRefPtr<WMLElement> timerConstructor(const QualifiedName& tagName, Document* document, bool)
{
    return WMLTimerElement::create(tagName, document);
}

static void addTag(const QualifiedName& tag, ConstructorFunction func)
{
    gFunctionMap->set(tag.localName().impl(), func);
}

static void createFunctionMap()
{
    ASSERT(!gFunctionMap);

    // Create the table.
    gFunctionMap = new FunctionMap;
    
    // Populate it with constructor functions.
    addTag(aTag, aConstructor);
    addTag(accessTag, accessConstructor);
    addTag(anchorTag, anchorConstructor);
    addTag(brTag, brConstructor);
    addTag(cardTag, cardConstructor);
    addTag(doTag, doConstructor);
    addTag(fieldsetTag, fieldsetConstructor);
    addTag(goTag, goConstructor);
    addTag(imgTag, imageConstructor);
    addTag(inputTag, inputConstructor);
    addTag(insertedLegendTag, insertedlegendConstructor);
    addTag(metaTag, metaConstructor);
    addTag(noopTag, noopConstructor);
    addTag(oneventTag, oneventConstructor);
    addTag(optgroupTag, optgroupConstructor);
    addTag(optionTag, optionConstructor);
    addTag(pTag, pConstructor);
    addTag(postfieldTag, postfieldConstructor);
    addTag(prevTag, prevConstructor);
    addTag(refreshTag, refreshConstructor);
    addTag(selectTag, selectConstructor);
    addTag(setvarTag, setvarConstructor);
    addTag(tableTag, tableConstructor);
    addTag(templateTag, templateConstructor);
    addTag(timerTag, timerConstructor);
}

PassRefPtr<WMLElement> WMLElementFactory::createWMLElement(const QualifiedName& qName, Document* document, bool createdByParser)
{
    if (!document)
        return 0;

#if ENABLE(DASHBOARD_SUPPORT)
    Settings* settings = document->settings();
    if (settings && settings->usesDashboardBackwardCompatibilityMode())
        return 0;
#endif
    if (!gFunctionMap)
        createFunctionMap();
    if (ConstructorFunction function = gFunctionMap->get(qName.localName().impl()))
        return function(qName, document, createdByParser);
    return WMLElement::create(qName, document);
}

} // namespace WebCore

#endif
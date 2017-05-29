/*
 * Copyright (C) 2009 Alex Milowski (alex@milowski.com). All rights reserved.
 * Copyright (C) 2010 François Sausset (sausset@gmail.com). All rights reserved.
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

#if ENABLE(MATHML)

#include "RenderMathMLFraction.h"

#include "GraphicsContext.h"
#include "MathMLNames.h"
#include "RenderText.h"

namespace WebCore {
    
using namespace MathMLNames;

static const double gHorizontalPad = 0.2;
static const int gLineThin = 1;
static const int gLineMedium = 3;
static const int gLineThick = 5;
static const double gFractionAlignment = 0.25;
static const double gFractionBarWidth = 0.05;
static const double gDenominatorPad = 0.1;

RenderMathMLFraction::RenderMathMLFraction(Element* fraction) 
    : RenderMathMLBlock(fraction)
    , m_lineThickness(gLineThin)
{
    setChildrenInline(false);
}

void RenderMathMLFraction::updateFromElement()
{
    // FIXME: mfrac where bevelled=true will need to reorganize the descendants
    if (isEmpty()) 
        return;
    
    Element* fraction = static_cast<Element*>(node());
    
    RenderObject* numerator = firstChild();
    String nalign = fraction->getAttribute(MathMLNames::numalignAttr);
    if (equalIgnoringCase(nalign, "left"))
        numerator->style()->setTextAlign(LEFT);
    else if (equalIgnoringCase(nalign, "right"))
        numerator->style()->setTextAlign(RIGHT);
    else
        numerator->style()->setTextAlign(CENTER);
    
    RenderObject* denominator = numerator->nextSibling();
    if (!denominator)
        return;
    
    String dalign = fraction->getAttribute(MathMLNames::denomalignAttr);
    if (equalIgnoringCase(dalign, "left"))
        denominator->style()->setTextAlign(LEFT);
    else if (equalIgnoringCase(dalign, "right"))
        denominator->style()->setTextAlign(RIGHT);
    else
        denominator->style()->setTextAlign(CENTER);
    
    // FIXME: parse units
    String thickness = fraction->getAttribute(MathMLNames::linethicknessAttr);
    m_lineThickness = gLineThin;
    if (equalIgnoringCase(thickness, "thin"))
        m_lineThickness = gLineThin;
    else if (equalIgnoringCase(thickness, "medium"))
        m_lineThickness = gLineMedium;
    else if (equalIgnoringCase(thickness, "thick"))
        m_lineThickness = gLineThick;
    else if (equalIgnoringCase(thickness, "0"))
        m_lineThickness = 0;

    // Update the style for the padding of the denominator for the line thickness
    lastChild()->style()->setPaddingTop(Length(static_cast<int>(m_lineThickness + style()->fontSize() * gDenominatorPad), Fixed));
}

void RenderMathMLFraction::addChild(RenderObject* child, RenderObject* beforeChild)
{
    RenderBlock* row = new (renderArena()) RenderMathMLBlock(node());
    RefPtr<RenderStyle> rowStyle = makeBlockStyle();
    
    rowStyle->setTextAlign(CENTER);
    Length pad(static_cast<int>(rowStyle->fontSize() * gHorizontalPad), Fixed);
    rowStyle->setPaddingLeft(pad);
    rowStyle->setPaddingRight(pad);
    
    // Only add padding for rows as denominators
    bool isNumerator = isEmpty();
    if (!isNumerator) 
        rowStyle->setPaddingTop(Length(2, Fixed));
    
    row->setStyle(rowStyle.release());
    RenderBlock::addChild(row, beforeChild);
    row->addChild(child);
    updateFromElement();
}

void RenderMathMLFraction::layout()
{
    updateFromElement();
    
    // Adjust the fraction line thickness for the zoom
    if (lastChild() && lastChild()->isRenderBlock())
        m_lineThickness = m_lineThickness * ceil(gFractionBarWidth * style()->fontSize());
    
    if (previousSibling() && previousSibling()->isRenderBlock()) {
        RenderBlock* sibling = toRenderBlock(previousSibling());
        verticalAlignCompute(sibling);
    } else if (nextSibling() && nextSibling()->isRenderBlock()) {
        RenderBlock* sibling = toRenderBlock(nextSibling());
        verticalAlignCompute(sibling);
    }

    RenderBlock::layout();
    
    // The row layout can affect the numerator/denominator width.
    // FIXME: This is probably only needed if one of the children
    // contains an mrow.
    setNeedsLayoutAndPrefWidthsRecalc();
    markContainingBlocksForLayout();
    
    RenderBlock::layout();
    
}
    
void RenderMathMLFraction::paint(PaintInfo& info, int tx, int ty)
{
    RenderMathMLBlock::paint(info, tx, ty);
    if (info.context->paintingDisabled() || info.phase != PaintPhaseForeground)
        return;
    
    if (!firstChild() ||!m_lineThickness)
        return;

    int verticalOffset = 0;
    // The children are always RenderMathMLBlock instances
    if (firstChild()->isRenderMathMLBlock()) {
        int adjustForThickness = m_lineThickness > 1 ? m_lineThickness / 2 : 1;
        if (m_lineThickness % 2 == 1)
            adjustForThickness++;
        RenderMathMLBlock* numerator = toRenderMathMLBlock(firstChild());
        if (numerator->isRenderMathMLRow())
            verticalOffset = numerator->offsetHeight() + adjustForThickness;
        else 
            verticalOffset = numerator->offsetHeight();        
    }
    
    tx += x();
    ty += y() + verticalOffset;
    
    info.context->save();
    
    info.context->setStrokeThickness(static_cast<float>(m_lineThickness));
    info.context->setStrokeStyle(SolidStroke);
    info.context->setStrokeColor(Color(0, 0, 0), sRGBColorSpace);
    
    info.context->drawLine(IntPoint(tx, ty), IntPoint(tx + offsetWidth(), ty));
    
    info.context->restore();
}

void RenderMathMLFraction::verticalAlignCompute(RenderBlock* sibling)
{
    if (sibling->isRenderMathMLBlock()) {
        RenderMathMLBlock* op = toRenderMathMLBlock(sibling);
        style()->setVerticalAlign(LENGTH);
        int verticalShift = static_cast<int>(ceil(gFractionAlignment * op->offsetHeight() + 0.5 * lastChild()->style()->borderTopWidth()));
        style()->setVerticalAlignLength(Length(verticalShift, Fixed));
    }
}

int RenderMathMLFraction::baselinePosition(bool firstLine, bool isRootLineBox) const
{
    if (firstChild()->isRenderMathMLBlock()) {
        RenderMathMLBlock* numerator = toRenderMathMLBlock(firstChild());
        return numerator->offsetHeight();
    }
    return RenderBlock::baselinePosition(firstLine, isRootLineBox);
}

}


#endif // ENABLE(MATHML)

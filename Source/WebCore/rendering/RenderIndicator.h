/*
 * Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#ifndef RenderIndicator_h
#define RenderIndicator_h

#if ENABLE(PROGRESS_TAG) || ENABLE(METER_TAG)
#include "RenderBlock.h"

namespace WebCore {

class RenderIndicatorPart : public RenderBlock {
public:
    RenderIndicatorPart(Node*);
    virtual ~RenderIndicatorPart();

protected:
    EVisibility originalVisibility() const { return m_originalVisibility; }
    virtual IntRect preferredFrameRect() = 0;
    virtual bool shouldBeHidden() = 0;
private:
    virtual void layout();
    virtual bool requiresForcedStyleRecalcPropagation() const { return true; }
    virtual bool canHaveChildren() const { return false; }
    virtual void styleDidChange(StyleDifference, const RenderStyle*);

    EVisibility m_originalVisibility;
};

class RenderIndicator : public RenderBlock {
public:
    RenderIndicator(Node*);
    virtual ~RenderIndicator();

protected:
    virtual void layout();
    virtual void updateFromElement();
    virtual bool requiresForcedStyleRecalcPropagation() const { return true; }
    virtual bool canHaveChildren() const { return false; }

    virtual void layoutParts() = 0;
};

} // namespace WebCore

#endif

#endif // RenderIndicator_h


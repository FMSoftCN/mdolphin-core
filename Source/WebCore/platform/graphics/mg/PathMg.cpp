/*
** $Id$
**
** PathMg.cpp: vector graphics path implements file.
**
** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd.
**
** All rights reserved by Feynman Software.
**
** Current maintainer: lvlei
** 
** Create date: 2010-06-02
*/

#include "minigui.h"

#include "config.h"
#include "Path.h"
#include "AffineTransform.h"
#include "FloatRect.h"
#include "GraphicsContext.h"
#include "PlatformString.h"
#include "MathExtras.h"

#include "NotImplemented.h"

#include <math.h>

namespace WebCore {
//path platform implements
class PlatformPath
{
public:
    PlatformPath():require_moveTo(false)
    {
        struct PathElement e;
        e.type = PathElementMoveToPoint;
        e.points = new FloatPoint[1];
        e.npoints = 1;
        elements.append (e);
    }

    ~PlatformPath()
    {
        clear();
    }

    PlatformPath(const PlatformPath & other);
    PlatformPath& operator=(const PlatformPath & other);

    void clear();
    
    bool require_moveTo;
    Vector<struct PathElement> elements;
};

void PlatformPath::clear()
{
    for (int i=0; i < (int)elements.size(); i++) {
        struct PathElement & e = elements[i];
        delete [] e.points;
    }
    elements.clear();
    require_moveTo = true;
}

PlatformPath::PlatformPath(const PlatformPath & other)
{
    elements = other.elements;
    for (int i=0; i < (int)elements.size(); i++) {
        elements[i].type = other.elements[i].type;
        elements[i].points = new FloatPoint[other.elements[i].npoints];
            for (int j=0; j < elements[i].npoints; j++)
                elements[i].points[j] = other.elements[i].points[j];
        elements[i].npoints = other.elements[i].npoints;
        require_moveTo = other.require_moveTo; 
    }
}

PlatformPath& PlatformPath::operator=(const PlatformPath & other)
{
    if (&other != this) {

        elements = other.elements;
        for (int i=0; i < (int)elements.size(); i++) {
            elements[i].type = other.elements[i].type;
            elements[i].points = new FloatPoint[other.elements[i].npoints];
            for (int j=0; j < elements[i].npoints; j++)
                elements[i].points[j] = other.elements[i].points[j];
            elements[i].npoints = other.elements[i].npoints;
            require_moveTo = other.require_moveTo; 
        }
    }
    return *this;
}

//path implements    

Path::Path()
    :m_path(new PlatformPath)
{
}

Path::~Path()
{
    delete m_path;
}

Path::Path(const Path& other) 
    :m_path(new PlatformPath(*(other.platformPath())))
{
}

Path& Path::operator=(const Path& other)
{
    if (&other != this) {
        delete m_path;
        m_path = new PlatformPath(*(other.platformPath()));
        //m_rule = other.windingRule();
    }
    return*this;
}

static void isect_line (const FloatPoint & p1, const FloatPoint & p2, const FloatPoint & pos, int *winding)
{
    float x1 = p1.x();
    float y1 = p1.y();
    float x2 = p2.x();
    float y2 = p2.y();
    float y = pos.y();

    int dir = 1;

    if (((y1 - y2) < 0.00001) && ((y1 - y2) > -0.00001))
        // ignore horizontal lines according to scan conversion rule
        return;
    else if (y2 < y1){
        float x_tmp = x2; x2 = x1; x1 = x_tmp;
        float y_tmp = y2; y2 = y1; y1 = y_tmp;
        dir = -1;
    }

    if (y >= y1 && y < y2) {
        float x = x1 + ((x2 - x1) / (y2 - y1)) * (y - y1);

        // count up the winding number if we're
        if (x<=pos.x()) {
            (*winding) += dir;
        }
    }
}

static FloatRect curve_bounds (const FloatPoint & p1, const FloatPoint & p2, const FloatPoint & p3, const FloatPoint & p4)
{
    float xmin = p1.x();
    float xmax = p1.x();
    if (p2.x() < xmin)
        xmin = p2.x();
    else if (p2.x() > xmax)
        xmax = p2.x();
    if (p3.x() < xmin)
        xmin = p3.x();
    else if (p3.x() > xmax)
        xmax = p3.x();
    if (p4.x() < xmin)
        xmin = p4.x();
    else if (p4.x() > xmax)
        xmax = p4.x();

    float ymin = p1.y();
    float ymax = p1.y();
    if (p2.y() < ymin)
        ymin = p2.y();
    else if (p2.y() > ymax)
        ymax = p2.y();
    if (p3.y() < ymin)
        ymin = p3.y();
    else if (p3.y() > ymax)
        ymax = p3.y();
    if (p4.y() < ymin)
        ymin = p4.y();
    else if (p4.y() > ymax)
        ymax = p4.y();
    return FloatRect(xmin, ymin, xmax-xmin, ymax-ymin);
}

static void curve_split(const FloatPoint & p1, const FloatPoint & p2, const FloatPoint & p3, const FloatPoint & p4, 
                              FloatPoint & fp1, FloatPoint & fp2, FloatPoint & fp3, FloatPoint & fp4,
                              FloatPoint & sp1, FloatPoint & sp2, FloatPoint & sp3, FloatPoint & sp4)
{
    float c = (p2.x() + p3.x())/2;
    fp2.setX((p1.x() + p2.x())/2);
    sp3.setX((p3.x() + p4.x())/2);
    fp1.setX(p1.x());
    sp4.setX(p4.x());
    fp3.setX((fp2.x() + c)/2);
    sp2.setX((sp3.x() + c)/2);
    fp4.setX((fp3.x() + sp2.x())/2);
    sp1.setX((fp3.x() + sp2.x())/2); 

    c = (p2.y() + p3.y())/2;
    fp2.setY((p1.y() + p2.y())/2);
    sp3.setY((p3.y() + p4.y())/2);
    fp1.setY(p1.y());
    sp4.setY(p4.y());
    fp3.setY((fp2.y() + c)/2);
    sp2.setY((sp3.y() + c)/2);
    fp4.setY((fp3.y() + sp2.y())/2);
    sp1.setY((fp3.y() + sp2.y())/2); 
}

static void isect_curve(const FloatPoint & p1, const FloatPoint & p2, 
                        const FloatPoint & p3, const FloatPoint & p4,
                        const FloatPoint & pos, int *winding)
{
    float y = pos.y();
    float x = pos.x();
    FloatRect bounds = curve_bounds(p1, p2, p3, p4);

    if (y >= bounds.y() && y < bounds.y() + bounds.height()) {
        const float lower_bound = 0.01f;
        if (bounds.width() < lower_bound && bounds.height() < lower_bound) {
            if (p1.x() <= x) {
                (*winding) += (p4.y() > p1.y() ? 1 : -1);
            }
            return;
        }

        // split curve and try again...
        FloatPoint first_half[4];
        FloatPoint second_half[4];

        curve_split(p1,p2,p3,p4, first_half[0],first_half[1],first_half[2],first_half[3],
                                second_half[0], second_half[1],second_half[2],second_half[3]);

        isect_curve(first_half[0], first_half[1], first_half[2], first_half[3], pos, winding);
        isect_curve(second_half[0], second_half[1], second_half[2], second_half[3], pos, winding);
    }
}

bool Path::contains(const FloatPoint& pt, WindRule rule) const 
{
    if (isEmpty())
         return false;

    int winding_number = 0;

    FloatPoint last_pt;
    FloatPoint last_start;

    for (int i = 0; i < (int)m_path->elements.size(); ++i) {
        const struct PathElement & e = m_path->elements[i];

        switch (e.type) {
            case PathElementMoveToPoint:
                if (i > 0)
                    isect_line (last_pt, last_start, pt, &winding_number);
                last_start = last_pt = e.points[0];
                break;
            case PathElementAddLineToPoint:
                isect_line (last_pt, e.points[0], pt, &winding_number);
                last_pt = e.points[0];
                break;
            case PathElementAddQuadCurveToPoint:
            case PathElementAddCurveToPoint:
                isect_curve(last_pt, e.points[0], e.points[1], e.points[2], pt, &winding_number);
                last_pt = e.points[2];
                break;
            case PathElementCloseSubpath:
                isect_line(last_pt, e.points[0], pt, &winding_number);
                last_pt = e.points[0];
                break;
        }
    }
    
    if (last_pt != last_start)
        isect_line(last_pt, last_start, pt, &winding_number);

    return (rule == RULE_NONZERO)?(winding_number != 0):((winding_number%2) != 0);
}

#define BEZIER_A(coord) 3 * (-p1.coord() \
                                        + 3*p2.coord() \
                                        - 3*p3.coord() \
                                        +p4.coord())

#define BEZIER_B(coord) 6 * (p1.coord() \
                                        - 2*p2.coord() \
                                        + p3.coord())

#define BEZIER_C(coord) 3 * (-p1.coord() \
                                        + p2.coord())

#define BEZIER_CHECK_T(t) \
    if (t >= 0 && t <= 1) { \
        FloatPoint p = bezier_point_at(p1,p2,p3,p4,t); \
        if (p.x() < minx) minx = p.x(); \
        else if (p.x() > maxx) maxx = p.x(); \
        if (p.y() < miny) miny = p.y(); \
        else if (p.y() > maxy) maxy = p.y(); \
    }

static inline FloatPoint bezier_point_at (const FloatPoint & p1, const FloatPoint & p2, 
                    const FloatPoint & p3, const FloatPoint & p4, float t)
{
    float a, b, c, d;
    float m_t = 1 - t;
    b = m_t * m_t;
    c = t * t;
    d = c * t;
    a = b * m_t;
    b *= 3 * t;
    c *= 3 * m_t;

    return FloatPoint(a*p1.x() + b*p2.x() + c*p3.x() + d*p4.x(), a*p1.y() + b*p2.y() + c*p3.y() + d*p4.y());
}

static FloatRect bezier_extrema(const FloatPoint & p1, const FloatPoint & p2, const FloatPoint & p3, const FloatPoint & p4)
{
    float minx, miny, maxx, maxy;
    // initialize with end points
    if (p1.x() < p4.x()) {
        minx = p1.x();
        maxx = p4.x();
    } else {
        minx = p4.x();
        maxx = p1.x();
    }
    if (p1.y() < p4.y()) {
        miny = p1.y();
        maxy = p4.y();
    } else {
        miny = p4.y();
        maxy = p1.y();
    }

    // Update for the X extrema
        float ax = BEZIER_A(x);
        float bx = BEZIER_B(x);
        float cx = BEZIER_C(x);
        // specialcase quadratic curves to avoid div by zero
        if (((ax - 0) < 0.00001) && ((ax - 0) > -0.00001)) {

            // linear curves are covered by initialization.
            if (!(((bx - 0) < 0.00001) && ((bx - 0) > -0.00001))) {
                float t = -cx / bx;
                BEZIER_CHECK_T(t);
            }

        } else {
            float t1 = (-bx + sqrt(bx * bx - 4 * ax * cx)) / (2 * ax);
            BEZIER_CHECK_T(t1);

            float t2 = (-bx - sqrt(bx * bx - 4 * ax * cx)) / (2 * ax);
            BEZIER_CHECK_T(t2);
        }

    // Update for the Y extrema
        float ay = BEZIER_A(y);
        float by = BEZIER_B(y);
        float cy = BEZIER_C(y);

        // specialcase quadratic curves to avoid div by zero
        if (((ay - 0) < 0.00001) && ((ay - 0) > -0.00001)) {

            // linear curves are covered by initialization.
            if (!(((by - 0) < 0.00001) && ((by - 0) > -0.00001))) {
                float t = -cy / by;
                BEZIER_CHECK_T(t);
            }

        } else {
            float t1 = (-by + sqrt(by * by - 4 * ay * cy)) / (2 * ay);
            BEZIER_CHECK_T(t1);

            float t2 = (-by - sqrt(by * by - 4 * ay * cy)) / (2 * ay);
            BEZIER_CHECK_T(t2);
        }

    return FloatRect(minx, miny, maxx - minx, maxy - miny);
}

FloatRect Path::boundingRect() const 
{ 
    if (isEmpty())
        return FloatRect();

    float minx, maxx, miny, maxy;
    FloatPoint last_pt;

    minx = maxx = m_path->elements[0].points[0].x();
    miny = maxy = m_path->elements[0].points[0].y();
    for (int i = 0; i < (int)m_path->elements.size(); ++i) {
        const struct PathElement & e = m_path->elements[i];

        switch (e.type) {
            case PathElementMoveToPoint:
            case PathElementAddLineToPoint:
                if (e.points[0].x() > maxx) maxx = e.points[0].x();
                else if (e.points[0].x() < minx) minx = e.points[0].x();

                if (e.points[0].y() > maxy) maxy = e.points[0].y();
                else if (e.points[0].y() < miny) miny = e.points[0].y();
                last_pt = e.points[0];
                break;
            case PathElementAddQuadCurveToPoint:
            case PathElementAddCurveToPoint:
                {
                    FloatRect r = bezier_extrema(last_pt, e.points[0], e.points[1], e.points[2]);

                    float right = r.maxX();
                    float bottom = r.maxY();
                    if (r.x() < minx) minx = r.x();
                    if (right > maxx) maxx = right;
                    if (r.y() < miny) miny = r.y();
                    if (bottom > maxy) maxy = bottom;
                }
                last_pt = e.points[2];
                break;
            case PathElementCloseSubpath:
                break;
        }
    }

    return FloatRect(minx, miny, maxx - minx, maxy - miny);
}

void Path::clear()
{
    m_path->clear();
}

bool Path::isEmpty() const 
{
    return (m_path->elements.size() == 0)||
            ((m_path->elements.size() == 1)&&(m_path->elements[0].type == PathElementMoveToPoint)); 
}

void Path::translate(const FloatSize& size) 
{
    AffineTransform matrix;
    matrix.translate (size.width(), size.height());
    transform (matrix);
}

void Path::moveTo(const FloatPoint& p) 
{
    if ((m_path->elements.size() > 0) && (m_path->elements.last().type == PathElementMoveToPoint)) {
        m_path->elements.last().points[0].setX(p.x());
        m_path->elements.last().points[0].setY(p.y());
    } else {
        struct PathElement e;
        e.type = PathElementMoveToPoint;
        e.points = new FloatPoint[1];
        e.npoints = 1;
        e.points[0].setX(p.x());
        e.points[0].setY(p.y());
        m_path->elements.append (e);
    }
    m_path->require_moveTo = false;
}

void Path::addLineTo(const FloatPoint& p) 
{
    if (m_path->require_moveTo) {
        struct PathElement e;
        e.type = PathElementMoveToPoint;
        e.points = new FloatPoint[1];
        e.npoints = 1;
        e.points[0] = m_path->elements.last().points[m_path->elements.last().npoints-1];
        m_path->elements.append (e);
        m_path->require_moveTo = false;
    }


    if (p == m_path->elements.last().points[0])
        return;

    struct PathElement e2;
    e2.type = PathElementAddLineToPoint;
    e2.points = new FloatPoint[1];
    e2.npoints = 1;
    e2.points[0].setX(p.x());
    e2.points[0].setY(p.y());
    m_path->elements.append (e2);
    
}

void Path::addQuadCurveTo(const FloatPoint& c, const FloatPoint& e) 
{
    FloatPoint prev = m_path->elements.last().points[m_path->elements.last().npoints-1];

    if (prev == c && c == e)
        return;

    FloatPoint c1((prev.x() + 2*c.x()) / 3, (prev.y() + 2*c.y()) / 3);
    FloatPoint c2((e.x() + 2*c.x()) / 3, (e.y() + 2*c.y()) / 3);
    addBezierCurveTo(c1, c2, e);

    m_path->elements.last().type = PathElementAddQuadCurveToPoint;
}

void Path::addBezierCurveTo(const FloatPoint& c1, const FloatPoint& c2, const FloatPoint& et) 
{
    if (m_path->elements.last().points[m_path->elements.last().npoints-1] == c1 && c1 == c2 && c2 == et)
        return;

    if (m_path->require_moveTo) {
        struct PathElement e;
        e.type = PathElementMoveToPoint;
        e.points = new FloatPoint[1];
        e.npoints = 1;
        e.points[0] = m_path->elements.last().points[m_path->elements.last().npoints-1];
        m_path->elements.append (e);
        m_path->require_moveTo = false;
    }

    struct PathElement e2;
    e2.type = PathElementAddCurveToPoint;
    e2.points = new FloatPoint[3];
    e2.npoints = 3;
    e2.points[0].setX(c1.x());
    e2.points[0].setY(c1.y());
    e2.points[1].setX(c2.x());
    e2.points[1].setY(c2.y());
    e2.points[2].setX(et.x());
    e2.points[2].setY(et.y());
    m_path->elements.append (e2);
}

void Path::closeSubpath() 
{
    if (isEmpty())
        return;
    FloatPoint first = m_path->elements.first().points[0];
    FloatPoint last = m_path->elements.last().points[m_path->elements.first().npoints-1];
    if (first.x() != last.x() || first.y() != last.y()) {
        struct PathElement e;
        e.type = PathElementCloseSubpath;
        e.points = new FloatPoint[1];
        e.npoints = 1;
        e.points[0].setX(first.x());
        e.points[0].setY(first.y());
        m_path->elements.append (e);
    }
    m_path->require_moveTo = true;
}


void Path::addRect(const FloatRect& r) 
{
    moveTo (FloatPoint(r.x(), r.y()));
    addLineTo(FloatPoint(r.x()+r.width(), r.y()));
    addLineTo(FloatPoint(r.x()+r.width(), r.y()+r.height()));
    addLineTo(FloatPoint(r.x(), r.y()+r.height()));
    addLineTo(FloatPoint(r.x(), r.y()));
    m_path->require_moveTo = true;
}

static inline float line_length (const FloatPoint & pt1, const FloatPoint & pt2)
{
     float x = pt2.x() - pt1.x();
     float y = pt2.y() - pt1.y();
     return sqrt(x*x + y*y);
}

static inline FloatPoint line_new_length (const FloatPoint & pt1, const FloatPoint & pt2, float len)
{
   float x = pt2.x() - pt1.x();
   float y = pt2.y() - pt1.y();

   float slen = sqrt(x*x + y*y);
   FloatPoint tp(pt1.x() + x/slen, pt1.y() + y/slen);
   
   return FloatPoint(pt1.x() + (tp.x()-pt1.x()) * len, pt1.y() + (tp.y()-pt1.y()) * len); 
}

#define PATH_KAPPA 0.5522847498
static FloatPoint curve_for_arc (const FloatRect &rect, float startAngle, float sweepLength, 
                                                        FloatPoint * curves, int * point_count)
{
    *point_count = 0;

    if (rect.isEmpty()) {
        return FloatPoint();
    }

    if (sweepLength > 360) sweepLength = 360;
    else if (sweepLength < -360) sweepLength = -360;

    // Special case fast path
    if (startAngle == 0.0 && sweepLength == 360.0) {
        float x = rect.x();
        float y = rect.y();

        float w = rect.width();
        float w2 = rect.width() / 2;
        float w2k = w2 * PATH_KAPPA;

        float h = rect.height();
        float h2 = rect.height() / 2;
        float h2k = h2 * PATH_KAPPA;

        // 0 -> 270 degrees
        curves[(*point_count)++] = FloatPoint(x + w, y + h2 + h2k);
        curves[(*point_count)++] = FloatPoint(x + w2 + w2k, y + h);
        curves[(*point_count)++] = FloatPoint(x + w2, y + h);

        // 270 -> 180 degrees
        curves[(*point_count)++] = FloatPoint(x + w2 - w2k, y + h);
        curves[(*point_count)++] = FloatPoint(x, y + h2 + h2k);
        curves[(*point_count)++] = FloatPoint(x, y + h2);

        // 180 -> 90 degrees
        curves[(*point_count)++] = FloatPoint(x, y + h2 - h2k);
        curves[(*point_count)++] = FloatPoint(x + w2 - w2k, y);
        curves[(*point_count)++] = FloatPoint(x + w2, y);

        // 90 -> 0 degrees
        curves[(*point_count)++] = FloatPoint(x + w2 + w2k, y);
        curves[(*point_count)++] = FloatPoint(x + w, y + h2 - h2k);
        curves[(*point_count)++] = FloatPoint(x + w, y + h2);

        return FloatPoint(x + w, y + h2);
    }

#define ANGLE(t) ((t) * 2 * M_PI / 360.0)
#define SIGN(t) (t > 0 ? 1 : -1)
    float a = rect.width() / 2.0;
    float b = rect.height() / 2.0;

    float absSweepLength = (sweepLength < 0 ? -sweepLength : sweepLength);
    int iterations = (int)ceil((absSweepLength) / 90.0);

    FloatPoint first_point;

    if (iterations == 0) {
        first_point = FloatPoint(rect.x()+ rect.width()/2, 
                                 rect.y()+rect.height()/2) + FloatPoint(a * cos(ANGLE(startAngle)),
                                 -b * sin(ANGLE(startAngle)));
    } else {
        float clength = sweepLength / iterations;
        float cosangle1, sinangle1, cosangle2, sinangle2;

        for (int i=0; i<iterations; ++i) {
            float cangle = startAngle + i * clength;

            cosangle1 = cos(ANGLE(cangle));
            sinangle1 = sin(ANGLE(cangle));
            cosangle2 = cos(ANGLE(cangle + clength));
            sinangle2 = sin(ANGLE(cangle + clength));

            // Find the start and end point of the curve.
            FloatPoint startPoint = FloatPoint(rect.x()+ rect.width()/2, rect.y()+rect.height()/2) 
                                                        + FloatPoint(a * cosangle1, -b * sinangle1);
            FloatPoint endPoint = FloatPoint(rect.x()+ rect.width()/2, rect.y()+rect.height()/2) 
                                                        + FloatPoint(a * cosangle2, -b * sinangle2);

            // The derived at the start and end point.
            float sdx = -a * sinangle1;
            float sdy = -b * cosangle1;
            float edx = -a * sinangle2;
            float edy = -b * cosangle2;

            // Creating the tangent lines. We need to reverse their direction if the
            // sweep is negative (clockwise)
            FloatPoint ap1 = startPoint;
            FloatPoint ap2 = startPoint + FloatPoint(round(sdx*SIGN(sweepLength)), round(sdy*SIGN(sweepLength)));
            FloatPoint bp1 = endPoint;
            FloatSize s = endPoint - FloatPoint(round(edx*SIGN(sweepLength)), round(edy*SIGN(sweepLength))); 
            FloatPoint bp2 = FloatPoint(s.width(), s.height());
            // We need to scale down the control lines to match that of the current sweeplength.
            // qAbs because we only want to scale, not change direction.
            float kappa = PATH_KAPPA * fabs(clength) / 90.0;
            // Adjust their length to fit the magic KAPPA length.
            ap2 = line_new_length(ap1, ap2, line_length(ap1, ap2) * kappa);
            bp2 = line_new_length(bp1, bp2, line_length(bp1, bp2) * kappa);

            curves[(*point_count)++] = ap2;
            curves[(*point_count)++] = bp2;
            curves[(*point_count)++] = endPoint;

            if (i == 0)
                first_point = startPoint;
        }
    }

    return first_point;
}

void Path::addEllipse(const FloatRect& boundingRect) 
{
    if (boundingRect.isEmpty())
        return;

    FloatPoint pts[12];
    int point_count;
    FloatPoint start = curve_for_arc(boundingRect, 0, 360, pts, &point_count);
    
    moveTo (start);
    addBezierCurveTo(pts[0], pts[1], pts[2]);           // 0 -> 270
    addBezierCurveTo(pts[3], pts[4], pts[5]);           // 270 -> 180
    addBezierCurveTo(pts[6], pts[7], pts[8]);           // 180 -> 90
    addBezierCurveTo(pts[9], pts[10], pts[11]);         // 90 - >0
    m_path->require_moveTo = true;
}

#define DEGREES(t) ((t) * 180.0 / M_PI)
#define FLOATCOMPARE(a,b) (((a-b) < 0.00001)&&((a-b) > -0.00001))
void Path::addArc(const FloatPoint& p, float r, float sar, float ear, bool anticlockwise)
{
    float xc = p.x();
    float yc = p.y();
    float radius = r;

    sar = -sar;
    ear = -ear;
    anticlockwise = !anticlockwise;

    float sa = DEGREES(sar);
    float ea = DEGREES(ear);

    double span = 0;

    double xs = xc - radius;
    double ys = yc - radius;
    double width  = radius*2;
    double height = radius*2;

    if (!anticlockwise && (ea < sa))
        span += 360;
    else if (anticlockwise && (sa < ea))
        span -= 360;

    // this is also due to switched coordinate system
    // we would end up with a 0 span instead of 360
    if (!(FLOATCOMPARE(span + (ea - sa), 0.0) &&
          FLOATCOMPARE(fabs(span), 360.0))) {
        span += ea - sa;
    }

    moveTo(FloatPoint(xc + radius  * cos(sar), yc - radius  * sin(sar)));

    FloatRect rect(xs, ys, width, height);
    int point_count;
    FloatPoint pts[12];
    FloatPoint curve_start = curve_for_arc(rect, sa, span, pts, &point_count);

    addLineTo(curve_start);
    for (int i=0; i<point_count; i+=3) 
        addBezierCurveTo(pts[i], pts[i+1], pts[i+2]);
}

void Path::transform(const AffineTransform& matrix) 
{
    for (int i=0; i < (int)m_path->elements.size(); i++) {
        const struct PathElement & e = m_path->elements[i];
        for (int j=0; j < e.npoints; j++){
            double x = 0, y = 0;
            matrix.map (e.points[j].x(), e.points[j].y(), x, y);
            e.points[j].setX(x);
            e.points[j].setY(y);
        }
    }
}


void Path::apply(void* info, PathApplierFunction function) const
{
    for (int i = 0; i < (int)m_path->elements.size(); ++i) {
        const struct PathElement & e = m_path->elements[i];
        function (info, &e);
    }
}

//not implements.
void Path::addArcTo(const FloatPoint& p1, const FloatPoint& p2, float radius) 
{
    notImplemented();
}

bool Path::hasCurrentPoint() const
{
    return !isEmpty();
}

FloatRect Path::strokeBoundingRect(StrokeStyleApplier* applier) const
{
    notImplemented();
    return FloatRect();
}

FloatPoint Path::currentPoint() const
{
    notImplemented();
    return FloatPoint();
}

}

// vim: ts=4 sw=4 et

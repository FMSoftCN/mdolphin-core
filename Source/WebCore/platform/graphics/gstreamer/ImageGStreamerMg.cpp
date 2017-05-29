/*
 * Copyright (C) 2010 Igalia S.L
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
 */

#include "config.h"
#if ENABLE(VIDEO)
#include "GOwnPtr.h"
#include "ImageGStreamer.h"
#include "minigui.h"

#include "ImageBufferData.h"

using namespace std;

using namespace WebCore;

PassRefPtr<ImageGStreamer> ImageGStreamer::createImage(GstBuffer* buffer)
{
    int width = 0, height = 0;
    GstCaps* caps = gst_buffer_get_caps(buffer);
    GstVideoFormat format;
    if (!gst_video_format_parse_caps(caps, &format, &width, &height)) {
        gst_caps_unref(caps);
        return 0;
    }

    gst_caps_unref(caps);
    return adoptRef(new ImageGStreamer(buffer, IntSize(width, height)));
}

ImageGStreamer::ImageGStreamer(GstBuffer*& buffer, IntSize size)
    : m_image(0)
{
#if 0
    m_mybitmap = new MYBITMAP;
	m_mybitmap->flags = MYBMP_TYPE_RGB|MYBMP_FLOW_DOWN|MYBMP_RGBSIZE_4;
    m_mybitmap->frames = 1;
    m_mybitmap->depth = 32;
    m_mybitmap->w = size.width();
    m_mybitmap->h = size.height();
    m_mybitmap->pitch = m_mybitmap->w * 4;
    m_mybitmap->size = m_mybitmap->pitch * m_mybitmap->h; 
    m_mybitmap->bits = reinterpret_cast<unsigned char*>(GST_BUFFER_DATA(buffer));
;
    m_image = BitmapImage::create((NativeImagePtr)m_mybitmap);
#endif
    unsigned char* bits =reinterpret_cast<unsigned char*>(GST_BUFFER_DATA(buffer)); 
    RefPtr<MDBitmap> imagePtr = MDBitmap::create(size.width(), size.height(), false, bits);
    m_image = BitmapImage::create(imagePtr);
}

ImageGStreamer::~ImageGStreamer()
{
    if (m_image)
        m_image.clear();
	m_image = 0;
	//delete m_mybitmap?
	//	if(m_mybitmap)
}
#endif

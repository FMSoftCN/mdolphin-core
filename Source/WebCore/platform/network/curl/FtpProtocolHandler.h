/*
** $Id: FtpProtocolHandler.h 546 2007-10-30 09:26:33Z whzeng $
**
** FtpProtocolHandler.h: FTP protocol header file.
**
** Copyright(C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date: 2007-11-16
*/

#ifndef FtpProtocolHandler_h
#define FtpProtocolHandler_h

#if ENABLE(FTPDIR)

namespace WebCore {

class ResourceHandle;

void initFtpJob(ResourceHandle *job);
} /* namespace WebCore */

#endif

#endif /* FtpProtocolHandler_h */

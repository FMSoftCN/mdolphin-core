/*
** $Id: MDolphinEncoding.cpp 590 2007-10-31 09:12:47Z jpzhang $
**
** MDolphinEncoding.cpp: Character encoding table file.
**
** Copyright (C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date:2007-7-5
*/

#include "minigui.h"

#include "config.h"
#include "MDolphinEncoding.h"

namespace Control {

const char* EncodingTable[] ={
  "ISO8859-1",
  "ISO8859-2",
  "ISO8859-3",
  "ISO8859-4",
  "ISO8859-5",
  "ISO8859-6",
  "ISO8859-7",
  "ISO8859-8",
  "ISO8859-9",
  "ISO8859-10",
  "ISO8859-11",
  "ISO8859-12",
  "ISO8859-13",
  "ISO8859-14",
  "ISO8859-15",
  "ISO8859-16",
  "EUC-CN",
  "GB1988-0",      
  "GB2312-0",      
  "GBK",
  "GB18030-0",     
  "BIG5",
  "KSC5636-0",     
  "KSC5601-0",     
  "EUC-JP",
  "JISX0201-0",    
  "JISX0208-0",    
  "SHIFT-JIS",
  "JISX0201-1",    
  "JISX0208-1",    
  "ISO-10646-1",   
  "UTF-8",
};

int EncodingTableSize = TABLESIZE(EncodingTable);

}

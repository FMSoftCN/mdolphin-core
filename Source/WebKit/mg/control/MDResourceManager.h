#ifndef MDResourceManager_h
#define MDResourceManager_h
#include "minigui.h"
#include "config.h"

#include "SharedBuffer.h"
#include "wtf/HashMap.h"
#include <wtf/text/WTFString.h>

using namespace WebCore;

//define the file name suffix
#define SUFFIX ".png"
#define DEFAULTDIRECTORY "/usr/local/share/minigui/res/"    

class MDResourceManager{
	public:
		PassRefPtr<SharedBuffer> loadResource(const char* name);
		int addInnerResource(INNER_RES* inner_res, int count);
		void setFileDirectory(const char* path);
		static MDResourceManager* getSharedInstance();
	private:
		MDResourceManager();
		INNER_RES* getInnerResource(const char* name);
		PassRefPtr<SharedBuffer> loadResourceFromFile(const char* name);
		PassRefPtr<SharedBuffer> loadResourceFromInner(const char* name);
		HashMap<RES_KEY,INNER_RES*> m_InnerRES;
		static MDResourceManager* m_sharedInstance;
		String m_fileDirectory;
};

#endif

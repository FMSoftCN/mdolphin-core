#include "MDResourceManager.h"
#include "CString.h"

MDResourceManager::MDResourceManager()
{
    m_fileDirectory= DEFAULTDIRECTORY;
}

MDResourceManager* MDResourceManager::m_sharedInstance=NULL;
MDResourceManager* MDResourceManager::getSharedInstance()
{
    if(m_sharedInstance==NULL){
        m_sharedInstance=new MDResourceManager;
    }
    return m_sharedInstance;
}

int MDResourceManager::addInnerResource(INNER_RES* inner_res, int count)
{
    for(int i=0;i<count;++i){
        m_InnerRES.set(inner_res[i].key,&(inner_res[i]));
    }
    return 0;
}

PassRefPtr<SharedBuffer> MDResourceManager::loadResource(const char* name)
{
    //first we load resource from extern file
    RefPtr<SharedBuffer> sb = loadResourceFromFile(name);
    //if the extern file is not exsit,we load from inner
    if(sb==NULL)
        sb=loadResourceFromInner(name);
    return sb.release();
}

void MDResourceManager::setFileDirectory(const char* path)
{
    m_fileDirectory=path;
}

PassRefPtr<SharedBuffer> MDResourceManager::loadResourceFromFile(const char* name)
{
    String filename=m_fileDirectory+name+SUFFIX;
    return SharedBuffer::createWithContentsOfFile(filename);
}

INNER_RES* MDResourceManager::getInnerResource(const char* name)
{
    String filename(name);
    filename+=SUFFIX;
    RES_KEY key=Str2Key(filename.utf8().data());
    return m_InnerRES.get(key);
}

PassRefPtr<SharedBuffer> MDResourceManager::loadResourceFromInner(const char* name)
{
    INNER_RES* res= getInnerResource(name);
    if(res)
        return SharedBuffer::create(res->data, res->data_len);
    else
        return NULL;
}

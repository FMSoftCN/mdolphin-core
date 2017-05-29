#include "minigui.h"
#include "config.h"
#include "MDInnerResource.h"
#include "MDResourceManager.h"
#define InnerResource 1
#include "resource/image/InnerResource.c"
#include "resource/image/audio-volume-high.png.c"
#include "resource/image/media-playback-pause.png.c"
#include "resource/image/media-skip-backward.png.c"
#include "resource/image/audio-volume-low.png.c"  
#include "resource/image/media-playback-start.png.c"
#include "resource/image/media-skip-forward.png.c"
#include "resource/image/audio-volume-medium.png.c" 
#include "resource/image/media-playback-stop.png.c" 
#include "resource/image/mediaSliderThumb.png.c"
#include "resource/image/audio-volume-muted.png.c"
#include "resource/image/media-record.png.c"    
#include "resource/image/mediaVolumeSliderThumb.png.c"
#include "resource/image/media-seek-backward.png.c"
#include "resource/image/media-eject.png.c"
#include "resource/image/media-seek-forward.png.c"
#include "resource/image/textAreaResizeCorner.png.c"
#include "resource/image/urlIcon.png.c"
#include "resource/image/panIcon.png.c"
#include "resource/image/nullPlugin.png.c"
#include "resource/image/missingImage.png.c"
#include "resource/image/deleteButton.png.c"

void loadInnerResource()
{
	static int flag=1;
	if(flag){
		MDResourceManager* res=MDResourceManager::getSharedInstance();
		res->addInnerResource(_mgirInnerResource, sizeof(_mgirInnerResource)/sizeof(INNER_RES));
		flag=0;
	}
}


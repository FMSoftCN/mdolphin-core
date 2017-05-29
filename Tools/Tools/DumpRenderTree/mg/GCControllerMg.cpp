#include "minigui.h"

#include "config.h"
#include "GCController.h"

#include "mdolphin.h"
#include "IMDWebView.h"

void GCController::collect() const
{
    GCCollectJSObjects();
}

void GCController::collectOnAlternateThread(bool waitUntilDone) const
{
    GCCollectJSObjectsOnAlternateThread(waitUntilDone);
}

size_t GCController::getJSObjectCount() const
{
    return GCCollectJSObjectsCount();
}

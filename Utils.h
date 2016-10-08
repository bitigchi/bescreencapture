#ifndef __UTILS_H
#define __UTILS_H

#include <MediaDefs.h>
#include <StorageDefs.h>
#include <SupportDefs.h>

#include <ObjectList.h>

class BPath;
class BRect;

BString GetUniqueFileName(const BString name, const char *extension);
void FixRect(BRect &rect, const bool fixWidth = false, const bool fixHeight = false);
status_t UpdateMediaFormat(const int32 &width, const int32 &height,
	const color_space &colorSpace, const int32 &fieldRate,
	media_format &mediaFormat);

void GetWindowsFrameList(BObjectList<BRect> &framesList, int32 border = 0);
BRect GetWindowFrameForToken(int32 token, int32 border = 0);
int32 GetWindowTokenForFrame(BRect rect, int32 border = 0);


#endif


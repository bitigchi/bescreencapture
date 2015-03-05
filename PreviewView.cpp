#include "PreviewView.h"

#include <cstdio>

#include <Alignment.h>
#include <Bitmap.h>
#include <GroupLayout.h>
#include <GroupLayoutBuilder.h>
#include <LayoutBuilder.h>
#include <LayoutUtils.h>
#include <Screen.h>
#include <StringView.h>


class BitmapView : public BView {
public:
	BitmapView();
	virtual ~BitmapView();
	
	virtual void Draw(BRect update);
	virtual BSize MinSize();
};


PreviewView::PreviewView()
	:
	BView("Rect View", B_WILL_DRAW),
	fBitmapView(NULL),
	fLeftTop(NULL),
	fRightBottom(NULL)
{
	fCoordRect = BRect(10, 10, 20, 20);
	fChanged = true;

	SetLayout(new BGroupLayout(B_HORIZONTAL));
	SetExplicitAlignment(BAlignment(B_ALIGN_LEFT,
		B_ALIGN_VERTICAL_CENTER));
	AddChild(BGroupLayoutBuilder(B_VERTICAL)
		.AddGroup(B_HORIZONTAL)
			.AddGlue()
			.AddGlue()
		.End()
		.AddGroup(B_HORIZONTAL)
			.AddGlue()
			.Add(fLeftTop = new BStringView("lefttop", ""))
			.Add(fBitmapView = new BitmapView())
			.Add(fRightBottom = new BStringView("rightbottom", ""))
			.AddGlue()
		.End()
		.AddGroup(B_HORIZONTAL)
			.AddGlue()
			.AddGlue()
		.End()
	);
	
	fLeftTop->SetExplicitAlignment(BAlignment(B_ALIGN_LEFT,
		B_ALIGN_TOP));
	fRightBottom->SetExplicitAlignment(BAlignment(B_ALIGN_RIGHT,
		B_ALIGN_BOTTOM));
}


void
PreviewView::AttachedToWindow()
{	
	BView::AttachedToWindow();
	SetViewColor(Parent()->ViewColor());

	Update();
}


void
PreviewView::_SetRect(const BRect& rect)
{
	if (fCoordRect != rect) {
		fCoordRect = rect;
		
		char str[16];
		snprintf(str, sizeof(str), "%d, %d", (int)rect.left, (int)rect.top);
		fLeftTop->SetText(str);
		snprintf(str, sizeof(str), "%d, %d", (int)rect.right, (int)rect.bottom);
		fRightBottom->SetText(str);
	}
}


void
PreviewView::Update(const BRect* rect, BBitmap* bitmap)
{
	if (rect != NULL)
		_SetRect(*rect);

	if (bitmap == NULL && Window() != NULL) {
		BScreen screen(Window());
		screen.GetBitmap(&bitmap, false, &fCoordRect);
	}
	if (bitmap != NULL) {
		fBitmapView->SetViewBitmap(bitmap,
			bitmap->Bounds().OffsetToCopy(B_ORIGIN),
			fBitmapView->Bounds(),
			B_FOLLOW_TOP|B_FOLLOW_LEFT, 0);
		if (Window() != NULL)
			Invalidate();
	}
}


BSize
PreviewView::MinSize()
{
	return BLayoutUtils::ComposeSize(ExplicitMinSize(), BSize(160, 100));
}


// BitmapView
BitmapView::BitmapView()
	:
	BView("bitmap view", B_WILL_DRAW|B_FULL_UPDATE_ON_RESIZE)
{
}


BitmapView::~BitmapView()
{
}


void
BitmapView::Draw(BRect rect)
{
	BView::Draw(rect);
	SetHighColor(0, 0, 0);
	StrokeRect(Bounds());
}


BSize
BitmapView::MinSize()
{
	return BLayoutUtils::ComposeSize(ExplicitMinSize(), BSize(100, 60));
}


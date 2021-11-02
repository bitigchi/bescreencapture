/*
 * Copyright 2013-2021, Stefano Ceccherini <stefano.ceccherini@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef __SELECTIONWINDOW_H
#define __SELECTIONWINDOW_H

#include <Messenger.h>
#include <Window.h>

class SelectionView;
class SelectionWindow : public BWindow {
public:
	enum mode {
		REGION,
		WINDOW
	};
	
	SelectionWindow(int mode, BMessenger& target, uint32 command);
	
	virtual void Show();
	virtual void SetTarget(BMessenger& target);
	virtual void SetCommand(uint32 command);
	virtual bool QuitRequested();
	
private:	
	SelectionView *fView;	
	BMessenger fTarget;
	uint32 fCommand;
};

#endif

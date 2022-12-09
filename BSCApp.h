/*
 * Copyright 2013-2021, Stefano Ceccherini <stefano.ceccherini@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef __BSCAPP_H
#define __BSCAPP_H

#include <Application.h>

#define kAppSignature "application/x-vnd.BeScreenCapture"


class Arguments;
class BSCApp : public BApplication {
public:
	BSCApp();
	virtual ~BSCApp();
	
	virtual void ArgvReceived(int32 argc, char** argv);
	virtual void ReadyToRun();
	virtual bool QuitRequested();
	virtual void MessageReceived(BMessage *message);
	virtual status_t GetSupportedSuites(BMessage* message);
	virtual void AboutRequested();

	void InstallDeskbarReplicant();
	void RemoveDeskbarReplicant();

	bool WasLaunchedSilently() const;
	bool LaunchedFromCommandline() const;
	
private:
	void _UsageRequested();

	BWindow *fWindow;
	Arguments* fArgs;
	bool fShouldStartRecording;
};

#endif // __BSCAPP_H

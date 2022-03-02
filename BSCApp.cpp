/*
 * Copyright 2013-2021, Stefano Ceccherini <stefano.ceccherini@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "BSCApp.h"

#include "Arguments.h"
#include "BSCWindow.h"
#include "Constants.h"
#include "Controller.h"
#include "DeskbarControlView.h"
#include "PublicMessages.h"
#include "Settings.h"

#include <private/interface/AboutWindow.h>
#include <Deskbar.h>
#include <StringList.h>

#include <cstdio>
#include <string>


const char kChangeLog[] = {
#include "Changelog.h"
};

const char* kAuthors[] = {
	"Stefano Ceccherini (stefano.ceccherini@gmail.com)",
	NULL
};

int
main()
{
	BSCApp app;
	app.Run();
	
	return 0;
}


BSCApp::BSCApp()
	:
	BApplication(kAppSignature),
	fWindow(NULL),
	fArgs(NULL)
{
	fArgs = new Arguments(0, NULL);
	
	Settings::Initialize();

	fShouldStartRecording = false;
	gControllerLooper = new Controller();
}


BSCApp::~BSCApp()
{
	RemoveDeskbarReplicant();

	Settings::Current().Save();
	Settings::Destroy();

	gControllerLooper->Lock();
	gControllerLooper->Quit();
	
	delete fArgs;
}


void
BSCApp::ArgvReceived(int32 argc, char** argv)
{
	LaunchedFromCommandline();
	fArgs->Parse(argc, argv);
	if (fArgs->UsageRequested()) {
		_UsageRequested();
		PostMessage(B_QUIT_REQUESTED);
		return;
	}
	
	fShouldStartRecording = fArgs->RecordNow();
}


void
BSCApp::ReadyToRun()
{
	try {
		fWindow = new BSCWindow();
	} catch (...) {
		PostMessage(B_QUIT_REQUESTED);
		return;	
	}

	if (fShouldStartRecording) {
		fWindow->Run();
		BMessenger(gControllerLooper).SendMessage(kMsgGUIToggleCapture);
	} else {
		fWindow->Show();
		InstallDeskbarReplicant();
	}
}


bool
BSCApp::QuitRequested()
{
	return BApplication::QuitRequested();
}


void
BSCApp::MessageReceived(BMessage *message)
{
	switch (message->what) {
		case kMsgGUIToggleCapture:
		case kMsgGUITogglePause:
			if (gControllerLooper != NULL)
				BMessenger(gControllerLooper).SendMessage(message);
			else {
				// Start recording as soon as a window is created
				fShouldStartRecording = true;
			}
			break;
		default:
			BApplication::MessageReceived(message);
			break;
	}
}


BStringList
SplitChangeLog(const char* changeLog)
{
	BStringList list;
	char* stringStart = (char*)changeLog;
	int i = 0;
	char c;
	while ((c = stringStart[i]) != '\0') {
		if (c == '-'  && i > 2 && stringStart[i - 1] == '-' && stringStart[i - 2] == '-') {
			BString string;
			string.Append(stringStart, i - 2);
			string.RemoveAll("\t");
			//string.ReplaceFirst("-", "\n\t-");
			string.ReplaceAll("-", "\n\t-");
			list.Add(string);
			stringStart = stringStart + i + 1;
			i = 0;
		} else
			i++;
	}
	return list;		
}


void
BSCApp::AboutRequested()
{
	BAboutWindow* aboutWindow = new BAboutWindow("BeScreenCapture", kAppSignature);
	aboutWindow->AddDescription("BeScreenCapture is a screen recording application for Haiku");
	aboutWindow->AddAuthors(kAuthors);
	aboutWindow->AddCopyright(2013, "Stefano Ceccherini");
	BStringList list = SplitChangeLog(kChangeLog);
	int32 stringCount = list.CountStrings();
	char** charArray = new char* [stringCount + 1];
	for (int32 i = 0; i < stringCount; i++) {
		charArray[i] = (char*)list.StringAt(i).String();
	}
	charArray[stringCount] = NULL;
	
	aboutWindow->AddVersionHistory((const char**)charArray);
	delete[] charArray;
	
	aboutWindow->Show();
}


void
BSCApp::InstallDeskbarReplicant()
{
	BDeskbar deskbar;
	if (deskbar.IsRunning()) {
		if (!deskbar.HasItem(BSC_DESKBAR_VIEW)) {
			deskbar.AddItem(new DeskbarControlView(BRect(0, 0, 15, 15)));
		}
	}
}


void
BSCApp::RemoveDeskbarReplicant()
{
	BDeskbar deskbar;
	if (deskbar.IsRunning()) {
		while (deskbar.HasItem(BSC_DESKBAR_VIEW))
			deskbar.RemoveItem(BSC_DESKBAR_VIEW);
	}
}


bool
BSCApp::WasLaunchedSilently() const
{
	return fShouldStartRecording;
}


bool
BSCApp::LaunchedFromCommandline() const
{
	team_info teamInfo;
	get_team_info(getppid(), &teamInfo);

	// TODO: Not correct, since someone could use another shell
	std::string args = teamInfo.args;
	return args.find("/bin/bash") != std::string::npos;
}


void
BSCApp::_UsageRequested()
{
}

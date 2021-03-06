// ServiceHelper.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>

#include "TextHelper.h"
#include "ServiceController.h"

#include "spdlog\sinks\basic_file_sink.h"

using namespace std;

#define ShowUsage() cout << "\tUsage: ServiceHelper.exe " << START << "|" << STOP << " SettingFile.xml" << "\n\t   start|stop: Specifies whether to start or stop the service(s)\n\t   SettingFile.xml: Specifies the settings file to use." << endl << endl << "Press ENTER to quit";

/***************************************************** Main entry point *****************************************************************/
int main(int argc, char* argv[])
{
	// Strings used to identify start and stop actions.
	const string START = "start";
	const string STOP = "stop";
	ServiceController* serviceController;
	

	// Clear the console window.
	system("CLS");

	// Make sure the app is running with elevated privileges.
	if (Privileges::IsElevated() == FALSE)
	{
		TextHelper::Blue();
		cout << "This program must be run with elevated privileges" << endl << "Try using Run as administrator" << endl << endl;
		TextHelper::White();
		cout << "Press ENTER to quit";
		cin.get();
		return 1;
	}

	// Make sure the stop/start argument was passed.
	if (argc < 3)
	{
		cout << "Missing command line argument" << endl;
		TextHelper::Gray();
		ShowUsage();
		cin.get();
		return 1;
	}

	string serviceAction = string(argv[1]);
	if (((serviceAction.compare(START) != 0) && (serviceAction.compare(STOP) != 0)))
	{
		cout << "Invalid command line argument" << endl;
		TextHelper::Gray();
		ShowUsage();
		cin.get();
		return 1;
	}

	// Check that the file specified as the settings file exists.
	if(false == ((fstream)(argv[2])).good() )
	{
		TextHelper::YellowHighlight();
		cout << "Can not find settings file: " << argv[2] << endl;
		ShowUsage();
		cin.get();
		return 1;
	}
	TextHelper::White();
	
	// Create the service controller.
	serviceController = new ServiceController(string(argv[2]));

	
	// Check if we are stopping or starting the service.
	if (serviceAction.compare(STOP) == 0)
	{
		serviceController->StopServers();
	}
	else
	{
		if (serviceController->SerialStart() == 0)
		{
			serviceController->StartServers();
		}
		else
		{
			serviceController->StartServersSerial();
		}
	}
	cout << "\n" << endl;
	
	delete serviceController;
#if DEBUG
	TextHelper::White();
	cout << "Press ENTER to quit";
	cin.get();
#endif
	return 0;
}

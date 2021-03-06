#pragma once
#ifndef WORLDSIM_DRIVER_GLOBAL_SETTINGS_CPP
#define WORLDSIM_DRIVER_GLOBAL_SETTINGS_CPP

/* WorldSim: Driver_GlobalSettings.cpp
#include "Driver_GlobalSettings.cpp"

	My attempt at moving stuff from a global include into an object which can load ini data.
	Obviously everything will lose it's const but what can you do.
*/

#include <File/WTFManager.hpp> // loading raws

#include <algorithm> // replace
#include <string>

class GlobalSettings
{
	public:
	
	std::string SAVE_FOLDER_PATH;
	std::string SAVE_FOLDER_PATH_FALLBACK; // use this if the custom path is bad.
	
	GlobalSettings()
	{
		SAVE_FOLDER_PATH = "SAVES";
		SAVE_FOLDER_PATH_FALLBACK = "SAVES";
	}
	
	void load(std::string settingsData)
	{
		std::cout<<"\n   *** Loading ini data ***\n";
		
		WTFManager iniManager;
		iniManager.parse(settingsData);
		
		std::string strSavePath = iniManager.getValue("INI.SAVE_FOLDER_PATH");
		// the WTF spec is currently broken and doesn't allow colons, so I'll just use a semicolon and replace it here
		// for now
		std::replace( strSavePath.begin(), strSavePath.end(), ';', ':'); // replace all semicolons with colon
		
		if ( strSavePath != "" )
		{
			SAVE_FOLDER_PATH=strSavePath;
		}
		std::cout<<"SAVE_FOLDER_PATH set to "<<SAVE_FOLDER_PATH<<"\n";

		std::cout<<"   *** END Loading ini data ***\n\n";

	}
	
	
};

#endif

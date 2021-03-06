// This file is part of Notepad++ project
// Copyright (C)2017 Don HO <don.h@free.fr>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// Note that the GPL places important restrictions on "derived works", yet
// it does not provide a detailed definition of that term.  To avoid      
// misunderstandings, we consider an application to constitute a          
// "derivative work" for the purpose of this license if it does any of the
// following:                                                             
// 1. Integrates source code from Notepad++.
// 2. Integrates/includes/aggregates Notepad++ into a proprietary executable
//    installer, such as those produced by InstallShield.
// 3. Links to a library or executes a program that does any of the above.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.


#pragma once

#include "StaticDialog.h"
#include "pluginsAdminRes.h"
#include "TabBar.h"
#include "ListView.h"
#include "tinyxml.h"

class PluginsManager;

struct PluginUpdateInfo
{
	generic_string name;
	generic_string version;
	generic_string homepage;
	generic_string sourceUrl;
	generic_string description;
	generic_string author;
	generic_string md5;
	generic_string alias;
	generic_string download;

	generic_string describe();
};

struct Version
{
	unsigned long _major = 0;
	unsigned long _minor = 0;
	unsigned long _patch = 0;
	unsigned long _build = 0;
	void setVersionFrom(generic_string filePath);
	generic_string toString();
};

struct LoadedPluginInfo
{
	generic_string _fullFilePath;

	generic_string _id;
	generic_string _name; // found from id/hash (or product name - retrieved from binary) or file name 
	Version _version;

	LoadedPluginInfo(const generic_string & fullFilePath, const generic_string & filename);
};

class PluginsAdminDlg final : public StaticDialog
{
public :
	PluginsAdminDlg() {};
	~PluginsAdminDlg() {
		_availableListView.destroy();
		_updateListView.destroy();
		_installedListView.destroy();
		if (_pPluginsXmlDoc) delete _pPluginsXmlDoc;
	}
    void init(HINSTANCE hInst, HWND parent)	{
        Window::init(hInst, parent);
	};

	virtual void create(int dialogID, bool isRTL = false);

    void doDialog(bool isRTL = false) {
    	if (!isCreated())
		{
			create(IDD_PLUGINSADMIN_DLG, isRTL);
		}

		if (!::IsWindowVisible(_hSelf))
		{

		}
	    display();
    };

	void switchDialog(int indexToSwitch);

	bool getPluginList(); // call WinGup fo the 1st time
	bool loadFomList();
	void setPluginsManager(PluginsManager *pluginsManager) { _pPluginsManager = pluginsManager; };

	//long searchPlugin(generic_string str2search, bool isNextMode);

protected:
	virtual INT_PTR CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);

private :
	TabBar _tab;

	ListView _availableListView;
	ListView _updateListView;
	ListView _installedListView;
	std::vector<PluginUpdateInfo> _availablePluginList;
	std::vector<PluginUpdateInfo> _updatePluginList;
	std::vector<PluginUpdateInfo> _installedPluginList;

	TiXmlDocument *_pPluginsXmlDoc = nullptr;
	PluginsManager *_pPluginsManager = nullptr;

	std::vector<LoadedPluginInfo> _loadedPluginInfos;

	bool readFromXml();
	bool searchInPlugins(bool isNextMode) const;
	const bool inNames = true;
	const bool inDescs = false;
	long searchFromCurrentSel(generic_string str2search, bool inWhichPart, bool isNextMode) const;
	long searchInNamesFromCurrentSel(generic_string str2search, bool isNextMode) const {
		return searchFromCurrentSel(str2search, inNames, isNextMode);
	};
	long searchInDescsFromCurrentSel(generic_string str2search, bool isNextMode) const {
		return searchFromCurrentSel(str2search, inDescs, isNextMode);
	};
	bool getLoadedPluginInfos();
};


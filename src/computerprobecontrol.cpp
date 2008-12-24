/***************************************************************************
 *   Copyright (C) 2006 by Bjorn Hansen                                    *
 *   holomorph@users.sourceforge.net                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "computerprobecontrol.h"
#include "log.h"

using namespace Balder;

Balder::ComputerProbeControl::ComputerProbeControl(player_id pid, std::string module): id(pid)
{
    pModule = ComputerProbeControl::LoadPythonModule(module);
    if (pModule != NULL) {
        pFunc = ComputerProbeControl::FindAIFunction(pModule);
    }
    else { pFunc = 0; }
}

Balder::ComputerProbeControl::~ComputerProbeControl()
{
    Py_XDECREF(pFunc);
    Py_XDECREF(pModule);
}

/****************************************************************************/
/*						Python setup Funtions                               */
/****************************************************************************/
/** Load the give python module **/
PyObject* Balder::ComputerProbeControl::LoadPythonModule(std::string module)
{
	PyObject *pName;
	pName = PyString_FromString(module.c_str());
    /* Error checking of pName left out */
    Log::output("Importing module named: " + module);
    PyObject *pModule = PyImport_Import(pName);
	Py_DECREF(pName);
    if (pModule != NULL) {
        Log::output("Successfully imported AI module");
    }
    else Log::output("unable to load AI module");
	return pModule;
}

PyObject* Balder::ComputerProbeControl::FindAIFunction(PyObject *pModule) {
    PyObject *pFunc = PyObject_GetAttrString(pModule, "DoProbeControl");
    /* pFunc is a new reference */
    if (!pFunc || !PyCallable_Check(pFunc)) {
        Log::output("unable to load AI function");
    }
    else Log::output("found AI function");
    return pFunc;
}


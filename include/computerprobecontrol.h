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

#ifndef BALDERARCOMPUTERPROBECONTROL_H
#define BALDERARCOMPUTERPROBECONTROLLER_H

#include <Python.h>
#include <list>
#include <string>
#include "balder2dtypes.h"

namespace Balder {

struct ComputerProbeControl {
    player_id id;
    input_states inputs;
    PyObject *pModule; // the name of the python module containing the ai
    PyObject* pFunc; // the python ai function to use
    ComputerProbeControl(player_id pid, std::string module);
    ~ComputerProbeControl();
    static PyObject* LoadPythonModule(std::string module);
    static PyObject* FindAIFunction(PyObject *pModule);
};
}
#endif

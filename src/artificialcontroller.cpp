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

#include <Python.h>
#include <stdlib.h>
#include <sstream>
#include "artificialcontroller.h"
#include "computerprobecontrol.h"
#include "gamemanager.h"
#include "collisionmask.h"
#include "input.h"
#include "entity.h"
#include "probe.h"
#include "filepathmanager.h"
#include "log.h"

using namespace Balder;

typedef std::list<Balder::ComputerProbeControl*>::iterator ProbeIterator;

GameManager* ArtificialController::game_manager = 0;
CollisionMask* ArtificialController::geometry = 0;
std::list<ComputerProbeControl*> ArtificialController::controlled_probes;

static PyMethodDef BalderAIMethods[] = {
    {"IsWall",  Balder::ArtificialController::py_IsWall,
    METH_VARARGS, "determine if a given point is part of a wall."},
    {"IsLineOfSight",  Balder::ArtificialController::py_IsLineOfSight,
    METH_VARARGS, "determine if any wall is blocking a \"line of sight\" between the given points."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

void ArtificialController::Initialize()
{
    Py_InitializeEx(0);
    // set up the python environment to find the scrpits
    Log::output("Loading AI scripts. . .");
    PyRun_SimpleString("import sys\n");
    std::list<std::string> searchpath = FilePathManager::GetSearchPath();
    //Iterate through the list:
    for(std::list<std::string>::iterator iter = searchpath.begin(); iter != searchpath.end(); ++iter)
    {
        Log::output("Setting script search path: " + *iter + "/scripts");
        std::string command = "sys.path.append(r'" + *iter + "/scripts')\n";
        PyRun_SimpleString(command.c_str());
    }
    PyObject *balder_module = Py_InitModule("balder", BalderAIMethods);
    PyModule_AddIntConstant(balder_module, "INPUT_LEFT", LEFT);
    PyModule_AddIntConstant(balder_module, "INPUT_RIGHT", RIGHT);
    PyModule_AddIntConstant(balder_module, "INPUT_STICK", STICK);
    PyModule_AddIntConstant(balder_module, "INPUT_FIRE", FIRE);
}


void ArtificialController::ShutDown()
{
    ProbeIterator pit = controlled_probes.begin();
    while(pit != controlled_probes.end()) {
        delete (*pit);
        ++pit;
    }
    controlled_probes.clear();
    if (Py_IsInitialized()){
        Py_Finalize();
    }
}


/*!
   m \fn Balder::ArtificialController::ControlProbe(player_id)
 */
void ArtificialController::ControlProbe(player_id id, std::string script)
{
    ComputerProbeControl *p = new ComputerProbeControl(id, script);
    // if the function was not found, don't try to control this probe
    if (p->pFunc && PyCallable_Check(p->pFunc)) {
        controlled_probes.push_back(p);
    }
    else {delete p;}
}

/*!
   m \fn Balder::ArtificialController::StopControlling(player_id)
 */
void ArtificialController::StopControlling(player_id id)
{
    ProbeIterator pit = controlled_probes.begin();
    while(pit != controlled_probes.end()) {
        if ((*pit)->id == id) {
            controlled_probes.erase(pit);
            delete (*pit);
            return;
        }
        ++pit;
    }
}


/*!
    \fn Balder::ArtificialController::DoControl()
 */
void ArtificialController::DoControl(std::list<Entity*> &entities)
{
	// create lists to be passed to the python script
	PyObject* probes = PyDict_New();
	PyObject* projectiles = PyList_New(0);
	PyObject* powerups = PyList_New(0);
	PyObject* pArgs = PyTuple_New(4);
	EntityIterator ent = entities.begin();
	while (ent != entities.end()) {
        // first create a dictionary and add things common to all entities
        PyObject* pEntityData = PyDict_New();
        PyObject* x = PyInt_FromLong((*ent)->GetX());
        PyObject* y = PyInt_FromLong((*ent)->GetY());
        PyObject* x_velocity = PyFloat_FromDouble((*ent)->GetXVelocity());
        PyObject* y_velocity = PyFloat_FromDouble((*ent)->GetYVelocity());
        PyDict_SetItemString(pEntityData, "x", x); Py_DECREF(x);
        PyDict_SetItemString(pEntityData, "y", y); Py_DECREF(y);
        PyDict_SetItemString(pEntityData, "x_velocity", x_velocity); Py_DECREF(x_velocity);
        PyDict_SetItemString(pEntityData, "y_velocity", y_velocity); Py_DECREF(y_velocity);
		if ((*ent)->GetType() == "probe") {
            Probe* p = dynamic_cast<Probe*>(*ent);
            PyObject* charge = PyInt_FromLong(p->GetCharge());
            PyObject* life = PyInt_FromLong(p->GetLife());
            PyObject* deaths = PyInt_FromLong(p->GetDeaths());
            PyObject* stuck = PyBool_FromLong(p->GetStuckStatus());
            PyObject* rotation_angle = PyFloat_FromDouble(p->GetRotationAngle());
            PyDict_SetItemString(pEntityData, "charge", charge); Py_DECREF(charge);
            PyDict_SetItemString(pEntityData, "life", life); Py_DECREF(life);
            PyDict_SetItemString(pEntityData, "deaths", deaths); Py_DECREF(deaths);
            PyDict_SetItemString(pEntityData, "stuck", stuck); Py_DECREF(stuck);
            PyDict_SetItemString(pEntityData, "rotation_angle", rotation_angle); Py_DECREF(rotation_angle);
			// add to probe dictionary with the id as the key
			PyObject* owner = PyInt_FromLong(p->GetOwner());
            PyDict_SetItem(probes, owner , pEntityData);Py_DECREF(owner);
		}
		else if ((*ent)->GetType() == "projectile") {
			// add to projectile list
             PyList_Append( projectiles, pEntityData);
		}
		else if ((*ent)->GetType() == "powerup") {
			// add to powerup list
             PyList_Append( powerups, pEntityData);
		}
		++ent;
		Py_DECREF(pEntityData);
	}
	PyTuple_SetItem(pArgs, 1, probes);
	PyTuple_SetItem(pArgs, 2, projectiles);
	PyTuple_SetItem(pArgs, 3, powerups);

    ProbeIterator pit = controlled_probes.begin();
    while(pit != controlled_probes.end())
    {
        PyObject* probeID = PyInt_FromLong((*pit)->id);
        PyTuple_SetItem(pArgs, 0, probeID);
        input_states old = (*pit)->inputs;
        // do AI stuff here . .
        PyObject *inputs = PyObject_CallObject((*pit)->pFunc, pArgs);
        // check for errors!
        if (inputs == NULL){ // something went wrong with the call print the error
            std::stringstream ss;
            ss << "Error in the scripts controlling probe " << (*pit)->id;
            Log::output(ss.str());
            PyObject *excptn = PyErr_Occurred();
            if (excptn) { PyErr_Print(); }
            // lets remove this probe from the list to control
            // so this doesn't happen every frame
            controlled_probes.erase(pit);
            delete (*pit);
        }
        else {
            (*pit)->inputs = PyInt_AsLong(inputs);
        }
        // send the input states, if they've changed
        if ((*pit)->inputs != old ) {
            game_manager->SendInputState((*pit)->id,(*pit)->inputs);
        }
        //Py_DECREF(inputs);
        //Py_DECREF(probeID);
        ++pit;
    }
    // need to decref all the lists etc that we created at the beginning of this function
    // probably would be better to just create these once in initialization, and then clear them each time
    Py_DECREF(probes);
	Py_DECREF(projectiles);
	Py_DECREF(powerups);
	Py_DECREF(pArgs);

}

PyObject* ArtificialController::py_IsWall(PyObject *self, PyObject *args)
{
    int x, y;
    if (!PyArg_ParseTuple(args, "ii", &x, &y)) {
        PyErr_BadArgument();
        return NULL;
    }
    bool result = geometry->TestPixel(x,y);
    return PyBool_FromLong(result);
}

PyObject* ArtificialController::py_IsLineOfSight(PyObject *self, PyObject *args)
{
    int x1, y1, x2, y2;
    if (!PyArg_ParseTuple(args, "(ii)(ii)", &x1, &y1, &x2, &y2)) {
        PyErr_BadArgument();
        return NULL;
    }
    std::stringstream ss;
//    ss << "testing line from (" << x1 << ", " << y1 << ") to (" << x2 << ", " << y2 << ")";
    // test every pixel along the line connecting the two points
    if (x1 == x2 && y1 == y2) {
        return PyBool_FromLong(true);
    }
    int x, y, xdelta, ydelta;
    xdelta = x2-x1;
    ydelta = y2-y1;
    /* we want to proceed on the direction that will test the most
    points along the line, if we just tested along x the loop would
    have good resolution in the x direction,
    but could be very poor in the y direction, if the x values are
    very close but y values that are not close for example testing
    along the line from (5,1) to (6,20) would only test two points!
    */
    if (xdelta*xdelta > ydelta*ydelta){
        float slope = float(ydelta)/xdelta;
         // make sure the points have x1 < x2 points so I can just write the loop once
        if (x1 > x2) {
            int temp = x2;
            x2 = x1;
            x1 = temp;
            temp = y2;
            y2 = y1;
            y1 = temp;
        }
        for (x=x1; x<x2; ++x) {
            // determine the y value
            xdelta = x - x1;
            ydelta = int(xdelta*slope);
            y = y1 + ydelta;
            if (geometry->TestPixel(x,y)) {
                return PyBool_FromLong(false);
            }
        }
    }
    else {
        float slope = float(xdelta)/ydelta;
        // make sure the points have y1 < y2 points so I can just write the loop once
        if (y1 > y2) {
            int temp = x2;
            x2 = x1;
            x1 = temp;
            temp = y2;
            y2 = y1;
            y1 = temp;
        }
        for (y=y1; y<y2; ++y) {
            // determine the y value
            ydelta = y - y1;
            xdelta = int(ydelta*slope);
            x = x1 + xdelta;
            if (geometry->TestPixel(x,y)) {
                return PyBool_FromLong(false);
            }
        }
    }
    return PyBool_FromLong(true);
}

/*!
    \fn Balder::ArtificialController::SetGameManager(GameManager* gm)
 */
void ArtificialController::SetGameManager(GameManager* gm)
{
    game_manager = gm;
}

/*!
    \fn Balder::ArtificialController::SetGeometry(CollisionMask*)
 */
void ArtificialController::SetGeometry(CollisionMask* cm)
{
    geometry = cm;
}

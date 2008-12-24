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

#ifndef BALDERARTIFICIALCONTROLLER_H
#define BALDERARTIFICIALCONTROLLER_H

#include "balder2dtypes.h"
#include <list>
#include <string>

namespace Balder {

class GameManager;
struct CollisionMask;
struct ComputerProbeControl;
struct Entity;

/**
@author Bjorn Hansen
*/
class ArtificialController{
public:
    static void Initialize();
    static void ShutDown();
    static void ControlProbe(player_id, std::string scriptName);
    static void StopControlling(player_id);
    static void DoControl(std::list<Entity*> &entity_list);
    static void SetGeometry(CollisionMask*);
    static void SetGameManager(GameManager* );
    #ifdef Py_PYTHON_H  // so gamemanager doesn't see this python related stuff
    static PyObject* py_IsWall(PyObject *self, PyObject *args);
    static PyObject* py_IsLineOfSight(PyObject *self, PyObject *args);
    #endif

private:
    ArtificialController(); // so we can't instantiate one of these
    static GameManager* game_manager;
    static CollisionMask* geometry;
    static std::list<ComputerProbeControl*> controlled_probes;
};

}

#endif

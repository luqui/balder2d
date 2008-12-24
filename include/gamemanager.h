/***************************************************************************
 *   Copyright (C) 2004 by Bjorn Hansen                                    *
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

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#define TICK_INTERVAL    30  // this is the number of ms between frames
#define STATWIDTH 100 // width in pixels for the status display
#define STATHEIGHT 100

#include "balder2dtypes.h"
#include "configmanager.h"
#include <SDL/SDL.h>
#include <list>
#include <map>

struct SDL_Surface;

namespace Balder{

class ConfigManager;
class Renderer;
class SoundManager;
class CollisionMask;
class Entity;
class ProbeFactory;
class ProjectileFactory;
class PowerUpFactory;
class Probe;
class Player;
class Input;

// need a cirular queu of spawn locations
struct SpawnLocation{
	int number;
	int x;
	int y;
	double x_vel;
	double y_vel;
	bool stuck;
	SpawnLocation* next;
};

class GameManager {
  private:
 	frame_number frame;
 	frame_number start_frame;
 	Uint32 start_time;
 	Uint32 end_time;
    Uint32 pause_start_time;
    int mapwidth;
    int mapheight;
    Renderer *render;
    SoundManager *sound;
    Input* input;
    CollisionMask *collision_mask;
    std::list<Entity*> entity_list;
    std::map<player_id, Player*> player_list;
    bool still_playing; //quit the game when this is false
    bool paused;
    ProbeFactory *probe_factory;
    ProjectileFactory *projectile_factory;
    PowerUpFactory *powerup_factory;
    SpawnLocation* spawn;
    // calculate stats and display them in the HUD
    void DoStats();
    void EndGame(std::string message = "");
    SDL_Surface* statbox;
    int statbox_x, statbox_y;
    void Run();
    bool InitMap(const char* mapname);
    //determine the time left in the current frame
    Uint32 FrameTimeLeft();
    void DumpEntityState();  // this one in a debugging aid
    std::string map_music;

  public:
    GameManager(Renderer *, SoundManager *);
    ~GameManager();
    frame_number GetFrame() const {return frame;}
    int GetWidth() {return mapwidth;}
    int GetHeight() {return mapheight;}
    void Start();
    player_id CreateNewPlayer (  );
    bool CreateNewPlayer(player_id, int score = 0, bool respawn = true);
    void RemovePlayer(player_id, bool notifyOthers = false);
    void ScoreAdd(player_id, int toAdd);
    void CreateProjectile ( player_id, double x, double y, double x_vel, double y_velocity );
    void SetNextSpawn(int);
    void SetStartFrame(frame_number f);
    const int getMaxLives() const {return ConfigManager::GetIntValue("GameSetup", "LifeLimit");}
    void RespawnProbe(Probe*);
    void RegisterEntity ( Entity*  );
    void RemoveEntity (Entity*);
    void SendUpdate(std::string toAddress);
    void UpdateInputState ( player_id id, input_states s);
    void SendInputState ( player_id id, input_states s);
    void PlaySound(const char* sound_type);
    void Quit();
    void TogglePause();
    void UpdatePlayer(player_id id, int score);
};
typedef std::list<Entity*>::iterator EntityIterator;
typedef std::map<player_id, Player*>::iterator PlayerIterator;
}
#endif


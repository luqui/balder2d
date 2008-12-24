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

#include "log.h"
#include <SDL/SDL.h>
#include "gamemanager.h"
#include "configmanager.h"
#include "player.h"
#include "input.h"
#include "artificialcontroller.h"
#include "probe.h"
#include "projectile.h"
#include "powerup.h"
#include "renderer.h"
#include "soundmanager.h"
#include "collisionmask.h"
#include "mapparser.h"
#include "probefactory.h"
#include "projectilefactory.h"
#include "powerupfactory.h"

using namespace Balder;

GameManager::GameManager(Renderer *r, SoundManager* s): mapwidth(0), mapheight(0), frame(0),
	render(r), sound(s), spawn(0)
{
    input = new Input(this, ConfigManager::GetNumPlayers());
    ArtificialController::Initialize();
    ArtificialController::SetGameManager(this);
    if (!input) throw "error creating input handler";
	// create entity factories for creating probes and projectiles
    probe_factory = new ProbeFactory(render, sound);
    projectile_factory = new ProjectileFactory(render);
    powerup_factory = new PowerUpFactory(render);
    // create a temporary surface of the correct size for stat boxes
	SDL_Surface* temp = SDL_CreateRGBSurface(SDL_SWSURFACE, STATWIDTH, STATHEIGHT, 8,0,0,0,0);
    // now convert the temp surface to the display format and use that for the stat boxes
    statbox = SDL_DisplayFormatAlpha(temp);
}

GameManager::~GameManager()
{
    if (input){
        delete input;
        input = 0;
    }
    ArtificialController::ShutDown();
    delete probe_factory;
    delete projectile_factory;
    delete powerup_factory;
}

// this is just a utility function for timing the game loop below
Uint32 GameManager::FrameTimeLeft()
{
    Uint32 next_time = (frame - start_frame)*TICK_INTERVAL;
    Uint32 now = SDL_GetTicks() - start_time;
    if ( next_time <= now ) {
        return(0);
    }
    return(next_time-now);
}

void GameManager::Start()
{
    // load the map
    InitMap(ConfigManager::GetMap());
    // create local players
    player_id id;
    int numplayers = ConfigManager::GetNumPlayers();
    try
    {
    	for (int i=1; i < numplayers+1; ++i)
    	{
            // this will create a new player and associated probe
            id = CreateNewPlayer();
            // check and see if it's a human controlled player, or what AI script to use
            std::string controlType = ConfigManager::GetControlType(i);
            if ("Human" == controlType){
                input->SetPlayerInputController(id, i);
            }
            else {
                std::stringstream ss;
                ss << "player " << id << " is computer controlled ";
                Log::output(ss.str());
                ArtificialController::ControlProbe(id, controlType);
            }
    	}
    }
    catch(...)
    {
            throw "error creating player, or player probe";
    }
    Log::output("Starting game...");
    start_time = SDL_GetTicks();
    int time = ConfigManager::GetIntValue("GameSetup", "TimeLimit");
    if (time > 0) end_time = start_time + (time*60*1000);
    else end_time = 0;
    Run();
}

bool GameManager::InitMap(const char* mapname)
{
	Log::output("Loading Map: " + std::string(mapname));
	// set up the map related stuff
    MapParser mp(mapname);

    // initalize level music
    map_music = mp.GetMapMusic();
    sound->PlayMusic(map_music.c_str());

    mapwidth = statbox_x = mp.GetMapWidth();
    mapheight = statbox_y = mp.GetMapHeight();
    render->SetScreen(mp.GetMapWidth(), mp.GetMapHeight()+STATHEIGHT,
    		ConfigManager::GetIntValue("Video", "fullscreen"));
    // create the collision mask
	collision_mask = new CollisionMask(mapname);
    powerup_factory->SetCollisionMask(collision_mask);
    ArtificialController::SetGeometry(collision_mask);
	// load the background
    if (! render->LoadBackground(mapname) ){
    	throw "unable to load background for map";
    }
    // get the spawn points from the config manager
    SpawnLocation *first = mp.FirstSpawnLocation();
    spawn = first;
    int i = 1;
    while( (spawn->next = mp.NextSpawnLocation()) )
    {
    	spawn->number = i;
		spawn = spawn->next;
		++i;
    }
    spawn->next = first;
    return true;
}

void GameManager::Run()
{
    start_frame = frame;
    int powerup_spawn_interval = ConfigManager::GetIntValue("Powerup", "spawn_interval");
    int maxScore = ConfigManager::GetIntValue("GameSetup", "ScoreLimit");
    //start the game loop
    still_playing = true;
    paused = false;
    while(still_playing)
    {
        // check for input
        input->CheckInput();

        if (paused) {
            ++frame;
            SDL_Delay(FrameTimeLeft());
            continue;
        }
        // spawn goodies
        if (frame % powerup_spawn_interval == 50){
        	powerup_factory->CreatePowerUp(this, frame);
        }
        ArtificialController::DoControl(entity_list);
        // erase, move and redraw the game entities
        EntityIterator ent1 = entity_list.begin();
        while(ent1 != entity_list.end())
        {
            render->EraseEntity((*ent1));
            (*ent1)->Move();
            Entity* e = (*ent1);
            ++ent1;
            if( e->IsDead()) {
            	e->Die();
            }
        }
        // test for collisions between entities
        ent1 = entity_list.begin();
        EntityIterator ent2;
        while(ent1 != entity_list.end())
        {
            ent2 = ent1;
            ++ent2;
            int x1 = (*ent1)->GetX();
            int y1 = (*ent1)->GetY();
            int w1 = (*ent1)->GetWidth();
            int h1 = (*ent1)->GetHeight();
            while (ent2 != entity_list.end())
            {
                // first a crude test
                if ( abs(x1 - (*ent2)->GetX()) > w1 + (*ent2)->GetWidth() ||
                     abs(y1 - (*ent2)->GetY()) > h1 + (*ent2)->GetHeight() )
                {
                	++ent2;
                    continue;
                }
                // the entities are close enough they might be colliding
                if ( (*ent1)->Collide((*ent2)))
                {
                    (*ent2)->Collide((*ent1));
                }
                ++ent2;
            }
            render->DrawEntity((*ent1));
            ++ent1;
        }
        // draw the stats/HUD
        DoStats();
        // check if the game should end
        if (maxScore > 0) {
        	// loop through players and see if anyone has reached the maximum
        	PlayerIterator pl = player_list.begin();
			while(pl != player_list.end()){
				if ( (*pl).second->getScore() >= maxScore ) {
					EndGame("Maximum Score Reached");
					break;
				}
				++pl;
			}
        }
        if (end_time){
        	if (SDL_GetTicks() > end_time) EndGame("Time Up");
        }
        // finish the frame
        render->FinishFrame();
        ++frame;
        // wait until it is time for the next frame
        SDL_Delay(FrameTimeLeft());

        // repeat map music.
        if (!sound->IsMusicPlaying()) {
            sound->PlayMusic(map_music.c_str());
        }
    }

    sound->PlayMusic("menu");
}

player_id GameManager::CreateNewPlayer (  )
{
    int id = (player_list.size() + 1);
    Player* player = new Player(static_cast<player_id>(id), this);
    probe_color color = ConfigManager::GetPlayerColor(id, render->GetScreenPixelFormat()->alpha);
    Probe* probe = probe_factory->CreateProbe(this, 100, 100, 1, 1, false, color, id);
    RespawnProbe(probe);
    player->SetProbe(probe);
    player_list[static_cast<player_id>(id)] = player;
    return id;
}

bool GameManager::CreateNewPlayer (player_id id, int score, bool respawn )
{
	std::stringstream ss;
	ss << "creating player with ID: " << id << " at frame " << frame;
	Log::output(ss.str());
    Player* player = new Player(id, this);
    player->SetScore(score);
    probe_color color = ConfigManager::GetPlayerColor(id, render->GetScreenPixelFormat()->alpha);// (probe_color)id%4+1;  This was used when we were using four different sprite sets to color up to four different players.
    Probe* probe = probe_factory->CreateProbe(this, 0, 0, 0, 0, true, color, id);
    if (respawn) {RespawnProbe(probe);}
    player->SetProbe(probe);
    player_list[id] = player;
    DumpEntityState();
    return true;
}

/*!
    \fn Balder::GameManager::UpdatePlayer(player_id id, frame_number frame, int score)
 */
void Balder::GameManager::UpdatePlayer(player_id id, int score)
{
	Log::output("updating player");
    if (player_list.count(id) == 0) { // the player doesn't exist, create them
    	CreateNewPlayer(id, score, false);
	}
	else {
		player_list[id]->SetScore(score);
	}
}

void GameManager::RemovePlayer(player_id id, bool notifyOthers)
{
	if (player_list.count(id) == 0) {
		Log::output("attempting to remove non-existant player!");
		return;
	}
    ArtificialController::StopControlling(id);
    Player* p = player_list[id];
    if(p) {
    	std::stringstream ss;
		ss << "Removing player with ID: " << id;
		Log::output(ss.str());
		// get rid of the player's probe
		const_cast<Probe*>(p->GetProbe())->Remove();
	}
	// now remove the player from the player list
	player_list.erase(id);
	if (ConfigManager::GetIntValue("GameSetup", "LifeLimit") > 0
		&& player_list.size() == 1) {
            PlayerIterator p = player_list.begin();
            render->DrawEntity(const_cast<Probe*>((*p).second->GetProbe()));
            player_id lastplayerID = (*player_list.begin()).first;
            std::stringstream ss;
            ss << "Only Player " << lastplayerID << " Remains.";
			EndGame(ss.str());
		}
}

void GameManager::ScoreAdd(player_id addToID, int toAdd)
{
	// if the player no longer exists, forget it
	if (player_list.count(addToID) == 0) return;
	(player_list[addToID])->AddScore(toAdd);
}
void GameManager::CreateProjectile ( player_id pid, double x, double y,
	double x_vel, double y_vel )
{
	// test to make sure the projectile will not be inside a wall
	if (!collision_mask->TestPixel(static_cast<int>(x),static_cast<int>(y))){
		Projectile* p = projectile_factory->CreateProjectile(this, x, y, x_vel, y_vel, pid);
	}
}

void GameManager::SetNextSpawn(int s)
{
	while (s != spawn->number)
	{
		spawn = spawn->next;
	}
}

void GameManager::SetStartFrame(frame_number f){
	frame = f;
	start_time = SDL_GetTicks();
}

void GameManager::RespawnProbe(Probe* p)
{
	// first check if this probe is out of lives
	int maxlives = getMaxLives();
	if( maxlives > 0 &&  p->GetDeaths() >= maxlives) {
		// eliminate the player
		RemovePlayer(p->GetOwner());
		return;
	}
    srand(frame); // seed with the frame number for predictable randomness
	// now we can respawn the probe
	if (!spawn) {
		p->SetProbeState(200,200,1,-1, false);
		return;
	}
	// ok we have at least one spawn location defined
	bool collision = true;
	const SpawnLocation* start = spawn;
	spawn = spawn->next;
	while (collision)
	{
		collision = false;
        if (spawn != start)
            p->SetProbeState(spawn->x,spawn->y,spawn->x_vel,spawn->y_vel,spawn->stuck);
        else { // already tested all spawn spots and got collisions, spawn pseudo-randomly
            int x = rand()%collision_mask->GetWidth();
            int y = rand()%collision_mask->GetHeight();
            int radius = p->GetWidth()/2;
            // test and make sure this doesn't burry the probe too deeply in a wall
            if (collision_mask->TestPixel(x, y + radius)){collision=true;continue;}
            if (collision_mask->TestPixel(x, x - radius)){collision=true;continue;}
            if (collision_mask->TestPixel(x + radius, y)){collision=true;continue;}
            if (collision_mask->TestPixel(x - radius, y)){collision=true;continue;}
            p->SetProbeState(x,y, 1-rand()%20/10.0, 1-rand()%20/10.0, false);
        }
		// make sure there isn't another entity already there
		EntityIterator ent = entity_list.begin();
    	while(ent != entity_list.end())
	    {
	    	// don't test the probe against itself
	    	Probe *p2 = (Probe*)(*ent);
	    	if (p == p2){
	    		++ent;
	    		continue;
	    	}
    		// first a crude test
        	if ( abs(p->GetX() - (*ent)->GetX()) < p->GetWidth() + (*ent)->GetWidth() &&
				abs(p->GetY() - (*ent)->GetY()) < p->GetHeight() + (*ent)->GetHeight() )
	        {
	        	if (start != spawn) spawn = spawn->next;
    	    	collision = true;
        	    break;
	        }
    	    ++ent;
	    }
	}
}

void GameManager::RegisterEntity ( Entity* e)
{
    e->SetCollisionMask(collision_mask);
	entity_list.push_back(e);
}

void GameManager::RemoveEntity(Entity* e)
{
	entity_list.remove(e);
	delete e;
}

void GameManager::UpdateInputState (player_id id, input_states s)
{
	if (player_list.count(id) == 0) return;
    Player* p = player_list[id];
    if (player_list.empty()) throw "player list is empty!";
   	if(p) p->DoControl(s);
}

void GameManager::SendInputState (player_id id, input_states s)
{
    UpdateInputState(id, s);
}

void GameManager::PlaySound(const char* sound_type)
{
    sound->PlaySound(sound_type);
}

void GameManager::Quit()
{
    Log::output("Quiting game...");
    still_playing = false;
}

void GameManager::DoStats()
{
	if (end_time){
		static std::stringstream ss;
		int secondsLeft = 0;
        if (end_time > SDL_GetTicks()) {
            secondsLeft = (end_time - SDL_GetTicks())/1000;
        }
		int minutesLeft = secondsLeft/60;
		ss.str("");
		ss << minutesLeft << ":";
        if (secondsLeft%60 < 10) {ss << "0";}
        ss << secondsLeft%60;
		render->DisplayText(5,0,ss.str());
	}
	PlayerIterator pl = player_list.begin();
   	while(pl != player_list.end())
   	{
   	    // (*pl).first is the player id, (*pl).second is the player pointer
   		(*pl).second->DrawStats(statbox);
   		render->BlitSurface(((*pl).first - 1)*(STATWIDTH+10), statbox_y,
   			STATWIDTH, STATHEIGHT, statbox);
   		++pl;
   	}
}

void GameManager::EndGame(std::string message)
{
	// pause, display game over (*TODO*) and wait for escape to be pressed
	render->DisplayText(100,100, "Game Over");
	render->DisplayText(100,200, message);
	render->DisplayText(100,300, "Press Esc To return to Main Menu");
	render->FinishFrame();
	while (still_playing)
	{
		input->CheckInput();
	}
}

void GameManager::TogglePause()
{
    if(end_time != 0) {
        if (!paused) {
            pause_start_time = SDL_GetTicks();
            render->DisplayText(100, 100, "Paused");
            render->FinishFrame();
        } else {
            end_time += (SDL_GetTicks() - pause_start_time);
            render->EraseScreen();
        }
    }
    paused = !paused;
}

void GameManager::DumpEntityState(){
	std::stringstream ss;
	//ss.precision(54);
	ss << "\n*********** entity states for frame " << frame << "***********\n";
	EntityIterator ent1 = entity_list.begin();
    while(ent1 != entity_list.end())
    {
    	Entity *e = *ent1;
    	ss << "\nPlayer " << e->GetOwner() << " " << e->GetType() << ":\n"
    		<< "\tX: " << e->GetXPosition() << "\tY: " << e->GetYPosition() << "\n"
    		<< "\tXvel: " << e->GetXVelocity() << "\tYvel: " << e->GetYVelocity()
    		<< "\n";
    	++ent1;
		if (0 == e->GetType().compare("probe" )) {
			ss << "\trotation: " << static_cast<Probe*>(e)->GetRotationAngle() << "\n";
		}
    }
   ss << "\n***************************************************\n";
   Log::output(ss.str());
}


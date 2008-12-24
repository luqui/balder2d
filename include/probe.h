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

#ifndef PROBE_H
#define PROBE_H

#define RECHARGE_FRAME_INTERVAL_ON 50
#define RECHARGE_FRAME_INTERVAL_OFF 3000

#include "entity.h"
#include "balder2dtypes.h"
#include <math.h>

namespace Balder{
const int PROBEWIDTH = 32;
const int PROBEHEIGHT = 32;
const int AMMOWIDTH = 4;
const int AMMOHEIGHT = 4;
const int MAXFRAME = 47; // 48 frames, starting at frame 0
const int HITFRAMES = 11;
const int SPAWNFRAMES = 30;
const int NO_ROT = 0;
const int LEFT_ROT = 1;
const int RIGHT_ROT = 2;
// pushOffState:
// 0 = not ready to push off, still it stick mode
// 1 = ready to push off, ie, have stuck to a wall and released the stick key
// 2 = pushing off, just waiting for the push(stick) key to be released again
enum {PUSH_NOT_READY, PUSH_READY, PUSH_POWERING};

class Projectile;
class PowerUp;

class Probe: public Entity {
  private:
	  static const int MAXHITS;
	  static const double MAXCHARGE;
	  static const int RECHARGETIME;
	  static const double RECHARGERATE;
	  static const double MAXSPEED;
	  static const double POWERUPTIME; //in frames
	  static const double TURNINGSPEED;
  public:
    Probe(GameManager* , double x_position, double y_position,
    	double x_velocity, double y_velocity, bool stuck, probe_color, player_id);
    void SetProbeState(double x_pos, double y_pos, double x_vel, double y_vel, bool stuck);
    void Move ( );
    bool Collide (Entity*);
    bool Collide (Probe*);
    bool Collide (Projectile*);
    bool Collide(PowerUp*);
    bool AddImpulse(double x_vel, double y_vel);
    void Die();
    void RotateRight ( bool  );
    void RotateLeft ( bool  );
    void Stick ( bool  );
    void PushOff (  );
    void Fire (  );
    const std::string GetType() const { return "probe";}
    const player_id GetOwner() const {return owner;}
    virtual SDL_Rect* GetFrame (  );
    virtual void DrawOnto(SDL_Surface*);
    const double GetRotationAngle() const { return facing_angle; }
    // stat stuff
    const probe_color GetProbeColor() const { return color;}
    const int GetLife() const {return life_remaining;}
    const int GetCharge()const {return (int)charge_remaining;}
    const double GetPushOffSpeed()const {return pushOffSpeed;}
    const double GetMAXSPEED()const {return MAXSPEED;}
    const bool GetStuckStatus() const {return stuck;}
    const int GetDeaths(){return deaths;}
    void Remove();
  private:
    void DrawAmmo(SDL_Surface*);
    float charge_remaining;
    int life_remaining;
    float shield_power;
    double facing_angle;
    bool stick;
    bool stuck;
    short int pushOffState;
    double pushOffSpeed;
    int rotation_state;
    double x_vel_add;
    double y_vel_add;
    int death_frame;
    int hit_frame;
    int spawn_frame;
    double ammo_angle;
    int startRechargeMSec;
    int currentRechargeMSec;
    int rechargeFrameInterval;
    bool rechargeFrameOn;
    probe_color color;
	player_id owner;
	int powerups;
	int deaths;
	bool removeFromGame;
private:
    void Destroy();
    void Spawn();
};

}
#endif

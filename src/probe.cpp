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
#include "probe.h"
#include "projectile.h"
#include "powerup.h"
#include "gamemanager.h"
#include "configmanager.h"
#include <SDL/SDL_gfxPrimitives.h>

using namespace Balder;

const int Probe::MAXHITS = ConfigManager::GetIntValue("Probe", "maxhits");
const double Probe::MAXCHARGE = ConfigManager::GetDoubleValue("Probe", "maxcharge");
const int Probe::RECHARGETIME = ConfigManager::GetIntValue("Probe", "rechargetime");
const double Probe::RECHARGERATE = MAXCHARGE/RECHARGETIME;
const double Probe::MAXSPEED = ConfigManager::GetDoubleValue("Probe", "maxspeed");
const double Probe::POWERUPTIME = ConfigManager::GetDoubleValue("Probe", "poweruptime");
const double Probe::TURNINGSPEED = M_PI*ConfigManager::GetDoubleValue("Probe", "turningspeed");
const bool Projectile::Add_Probe_Velocity = ConfigManager::GetIntValue("Projectile", "AddProbeVelocity");
const double Projectile::SPEED = ConfigManager::GetDoubleValue("Projectile", "speed");

Probe::Probe(GameManager* gm, double x_pos, double y_pos,
		double x_vel, double y_vel, bool stuck, probe_color c, player_id p)
    :Entity(gm, x_pos, y_pos, x_vel, y_vel, PROBEWIDTH, PROBEHEIGHT),
    charge_remaining(MAXCHARGE), life_remaining(MAXHITS), shield_power(0),
    facing_angle(0),stick(false), stuck(stuck), pushOffState(0), pushOffSpeed(0),
    rotation_state(NO_ROT), x_vel_add(0), y_vel_add(0), death_frame(0), hit_frame(0),
    spawn_frame(0), ammo_angle(0), color(c), owner(p), powerups(2), deaths(0),
    removeFromGame(false)
{
    bounding_box.w = PROBEWIDTH;
    bounding_box.h = PROBEHEIGHT;
    bounding_box.y = 0;//PROBEHEIGHT*(color-1);
}

SDL_Rect* Probe::GetFrame (  )
{
	// play dying animation if this probe is dying
	if (death_frame){
		bounding_box.x = PROBEWIDTH*(death_frame-1);
		return &bounding_box;
	}
	else if (hit_frame){
		bounding_box.x = PROBEWIDTH*(hit_frame-1);
		return &bounding_box;
	}
    else if (spawn_frame){
        bounding_box.x = PROBEWIDTH*(spawn_frame-1);
        return &bounding_box;
    }
	// not dying, spawning, or just hit, just decide where this probe is facing
	// need to shift the angle forward by half a step so that a frame is
	// the frame angle + or - half a step to the next/prev frame
	double angle = facing_angle + M_PI/(MAXFRAME+1);
	int frame_number = (int)(angle/(2*M_PI) * (MAXFRAME+1));
	if (frame_number > MAXFRAME) {frame_number = 0;}
	bounding_box.x = (int)PROBEWIDTH*frame_number;
	return &bounding_box;
}

void Probe::DrawOnto(SDL_Surface* destination)
{
    // this will render EFFECT_RECHARGE_FAST onto the players probe
    probe_color rechargeColor = color;

    if (powerups & EFFECT_RECHARGE_FAST) {
        currentRechargeMSec = SDL_GetTicks();
        if (currentRechargeMSec >= startRechargeMSec + rechargeFrameInterval) {
                rechargeFrameOn = !rechargeFrameOn;
                if (rechargeFrameOn) {
                        rechargeFrameInterval = RECHARGE_FRAME_INTERVAL_ON;
                }
                else {
                        rechargeFrameInterval = RECHARGE_FRAME_INTERVAL_OFF;
                }
                startRechargeMSec = currentRechargeMSec;
        }
        if (rechargeFrameOn) {
                rechargeColor = rechargeColor >> 8;
                Uint8 cBlue = Uint8(color);
                rechargeColor = rechargeColor >> 8;
                Uint8 cGreen = Uint8(color);
                rechargeColor = rechargeColor >> 8;
                Uint8 cRed = Uint8(color);

                if (cRed <= 205) cRed += 50;
                else cRed -= 50;
                if (cGreen <= 205) cRed += 50;
                else cGreen -= 50;
                if (cBlue <= 205) cRed += 50;
                else cBlue -= 50;

                rechargeColor = cRed;
                rechargeColor = rechargeColor << 8;
                rechargeColor += cGreen;
                rechargeColor = rechargeColor << 8;
                rechargeColor += cBlue;
                rechargeColor = rechargeColor << 8;
                rechargeColor += 255;
        }
    }

    // unless the probe is exploding or spawning draw all kinds of indicators etc.
    if (spawn_frame){
        int radius = int(spawn_frame/2.0*width/SPAWNFRAMES -1);
        circleColor(destination, GetX(), GetY(), radius, rechargeColor);
        filledCircleColor(destination, GetX(), GetY(), radius/2, rechargeColor);
    }
    else if (!death_frame){
        // first draw a circle the color of the probe, indicating hitpoints:
		// lets let full life fill 2/3 of the probe radius
		int healthRadius = width/(3*MAXHITS) * GetLife();
		// better make sure it's not too big, in case the player has bonus life
		if (healthRadius < width/2-1) {
        	circleColor(destination, GetX(), GetY(), width/2 -1, rechargeColor);
        	filledCircleColor(destination, GetX(), GetY(), healthRadius, rechargeColor);
		}
		else { // only need to bother drawing the filled circle
			filledCircleColor(destination, GetX(), GetY(), width/2-1, rechargeColor);
		}
        // now draw ammo indicators:
        DrawAmmo(destination);

        // finally draw powerup indicators:
        // this will render EFFECT_SPREADFIRE onto the players probe
        if (powerups & EFFECT_SPREADFIRE) {
	        int lastFrameSet = bounding_box.y;
                bounding_box.y = PROBEHEIGHT*1;
                Entity::DrawOnto(destination);
	        bounding_box.y = lastFrameSet;
        }
        // draw the EFFECT_SHIELD indicator, if the probe has enough shield power.
        if (shield_power >= 1) {
            Uint32 shieldColor = 0x888888FF;
            circleColor(destination, GetX(), GetY(), width/2+4, shieldColor);
        }
        // Experimental aiming indicator
        Uint32 aimerColor = 0xFFD700FF;
        float x_direction = cos(facing_angle);
        float y_direction = sin(facing_angle);
        int x_pos = int(x_position + x_direction*PROBEWIDTH*3.0/5);
        int y_pos = int(y_position + y_direction*PROBEHEIGHT*3.0/5);
        circleColor(destination, x_pos, y_pos, 1, aimerColor);
        if (powerups & EFFECT_SPREADFIRE) { // draw extra guns
            x_direction = cos(facing_angle + M_PI/48);
            y_direction = sin(facing_angle + M_PI/48);
            x_pos = int(x_position + cos(facing_angle + M_PI/12)*PROBEWIDTH*3.0/5);
            y_pos = int(y_position + sin(facing_angle + M_PI/12)*PROBEHEIGHT*3.0/5);
            circleColor(destination, x_pos, y_pos, 1, aimerColor);
            x_direction = cos(facing_angle - M_PI/48);
            y_direction = sin(facing_angle - M_PI/48);
            x_pos = int(x_position + cos(facing_angle - M_PI/12)*PROBEWIDTH*3.0/5);
            y_pos = int(y_position + sin(facing_angle - M_PI/12)*PROBEHEIGHT*3.0/5);
            circleColor(destination, x_pos, y_pos, 1, aimerColor);
        }
    }
    // now blit the sprite on top of the circle
    Entity::DrawOnto(destination);


}

void Probe::DrawAmmo(SDL_Surface* destination)
{
    if (GetCharge() > 0) {
        SDL_Surface* s = Renderer::sprite_registry["probe"];
        if (!s) throw "no sprite found for entity!";
        SDL_Rect sourceRect = {0, PROBEHEIGHT*2, AMMOWIDTH, AMMOHEIGHT};

        ammo_angle += M_PI/48;
        if (ammo_angle > 2*M_PI) { ammo_angle -= 2*M_PI;}
        // initialize calculation variables.
        double ammoAngle = 0;
        SDL_Rect target_rect;

        for (int currentCharge = 0;currentCharge < GetCharge(); currentCharge++) {
            ammoAngle = ((2*M_PI)/GetCharge())*currentCharge;
            ammoAngle += ammo_angle;

            float x_direction = cos(ammoAngle);
            float y_direction = sin(ammoAngle);
            float x_pos = x_position + x_direction*PROBEWIDTH*3/8; // 3/4 probe radius
            float y_pos = y_position + y_direction*PROBEHEIGHT*3/8;

            target_rect.x = Sint16(x_pos - AMMOWIDTH/2);
            target_rect.y = Sint16(y_pos - AMMOHEIGHT/2);

            // draw ammo frames.
            SDL_BlitSurface( s, &sourceRect, destination, &target_rect);
        }
    }
}

void Probe::Move()
{
	// add any velocity imparted on this probe last frame
	if (x_vel_add){
		x_velocity += x_vel_add;
		x_vel_add = 0;
	}
	if (y_vel_add){
		y_velocity += y_vel_add;
		y_vel_add = 0;
	}
	// to see if we bounce off a walle
	double old_x_vel = x_velocity;
	double old_y_vel = y_velocity;
	// first do all the moving stuff that all entities should do
	Entity::Move();
	if(stick && (old_x_vel != x_velocity || old_y_vel != y_velocity)){
		stuck = true;
		x_velocity = y_velocity = 0;
		game_manager->PlaySound("probe stick");
	}
	// now rotate if appropriate
	if (RIGHT_ROT == rotation_state)
	{
		facing_angle += TURNINGSPEED;
		if (facing_angle > 2*M_PI) { facing_angle -= 2*M_PI;}
	}
	if (LEFT_ROT == rotation_state)
	{
		facing_angle -= TURNINGSPEED;
		if (facing_angle < 0) { facing_angle += 2*M_PI;}
	}
	// update the charge remaining
	if (charge_remaining < MAXCHARGE){
		int accelerator = ((powerups & EFFECT_RECHARGE_FAST) ? 3:1);
		charge_remaining += RECHARGERATE * (accelerator);
	}
    // update the shield power
    if ((powerups & EFFECT_SHIELD) && shield_power < 1) {
    shield_power += RECHARGERATE;
    if (shield_power >=1) {game_manager->PlaySound("shields up");}
    }
	// update the death animation frame if necesary
	if (death_frame){
		++death_frame;
		if (death_frame > MAXFRAME) {
	    	is_dead = true;
		}
	}
	if (hit_frame){
		++hit_frame;
		if (hit_frame > HITFRAMES) {
			hit_frame = 0;
			bounding_box.y = 0; //PROBEHEIGHT*(color-1);
		}
	}
    if (spawn_frame){
        ++spawn_frame;
        if (spawn_frame > SPAWNFRAMES) {
            spawn_frame = 0;
            bounding_box.y = 0;
        }
    }
	// add power to push off with if pushing off
	if ( (PUSH_POWERING == pushOffState) && (pushOffSpeed < MAXSPEED) ) {
		pushOffSpeed += MAXSPEED/POWERUPTIME;
	}

}

bool Probe::Collide(Entity* e)
{
	// determine what type of entity this is
	const std::string etype = e->GetType();
	if (0 == etype.compare("probe")){
		//it's another probe
		return Collide( (Probe*)e );
	}
	else if ( 0 == etype.compare("projectile")) {
		// it's a projectile
		return Collide( (Projectile*)e );
	}
	else if ( 0 == etype.compare("powerup")) {
		// it's a projectile
		return Collide( (PowerUp*)e );
	}
	// by this point we don't know what it is
	return false;
}

bool Probe::Collide(Probe* p)
{
    // don't collide probes that are exploding
    if (p->GetLife() <1 || death_frame) {return false;}
    double x = p->GetX() - GetX();
    double y = p->GetY() - GetY();
    if (x*x + y*y > PROBEWIDTH*PROBEHEIGHT) return false;
	// impart all the velocity in the collision direction to the other probe
	// first, normalize the direction
	double norm = sqrt(x*x+y*y);
	x = x/norm;
	y = y/norm;
	// now project our velocity along that direction
	double projection = x*x_velocity + y*y_velocity;
	// make sure the velocity was at least partly in the direction of the other
	// probe, otherwise we were moving away and don't want to impart anything
	if (projection < 0) {return true;}
	// calculate the final velocity vector
	x = projection * x;
	y = projection * y;
	bool bump = p->AddImpulse(x, y);
	// now subtract what we imparted on the other probe or bounce off
        if (bump){
	    x_velocity -= x;
	    y_velocity -= y;
        }
        else{ // they did not move so bounce off
            x_velocity -= 2*x;
            y_velocity -= 2*y;
        }
	game_manager->PlaySound("probe collide");
    return true;
}

bool Probe::Collide(Projectile* p)
{
	// don't collide with projectiles that are already exploding
	if (p->IsExploding()) {return false;}
	// it's still live, test for collisions
    int x = p->GetX() - GetX();
    int y = p->GetY() - GetY();
    if (x*x + y*y > (PROBEWIDTH/2 + p->GetWidth()/2)
    				*(PROBEHEIGHT/2 + p->GetHeight()/2)){
    	return false;
    }
    // We're hit!
    // if we have shields, or are just spawning absorb it
    if (shield_power >=1 || spawn_frame) {
    	shield_power = 0;
    	return true;
    }
    // so take damage and return true
    --life_remaining;
    if (0 == life_remaining) {
    	// time to die!
		Destroy();
    	// if this wasn't fired by us, give credit to whoever shot us
    	if (p->GetOwner() != owner) {
    		game_manager->ScoreAdd(p->GetOwner(), 3);
    	}
    	// lose a point for dying
    	game_manager->ScoreAdd(owner, -1);
    }
    else if (life_remaining > 0){
    	hit_frame = 1;
    	bounding_box.y = PROBEHEIGHT*5;
    	game_manager->PlaySound("probe hit");
    	// if this wasn't fired by us, give credit to whoever shot us
    	if (p->GetOwner() != owner) {
    		game_manager->ScoreAdd(p->GetOwner(), 1);
    	}
    	else { // lose a point for shooting yourself
    		game_manager->ScoreAdd(owner, -1);
    	}
    }
    return true;
}

/*!
    \fn Balder::Probe::Collide(PowerUp*)
 */
bool Balder::Probe::Collide(PowerUp* p)
{
    int x = p->GetX() - GetX();
    int y = p->GetY() - GetY();
    if (x*x + y*y > (PROBEWIDTH/2 + p->GetWidth()/2)
    				*(PROBEHEIGHT/2 + p->GetHeight()/2)){
    	return false;
    }
    // right we touched it, collect the powerup
    if (powerups & p->getEffect()) { ++life_remaining; }
    else {
	    powerups = powerups | p->getEffect();
        if (p->getEffect() == EFFECT_SHIELD) {
        shield_power =1;
        game_manager->PlaySound("shields up");
        }
        if (p->getEffect() == EFFECT_RECHARGE_FAST) {
            rechargeFrameOn = false;
            startRechargeMSec = SDL_GetTicks();
            rechargeFrameInterval = RECHARGE_FRAME_INTERVAL_OFF;
        }
    }
    return true;
}

/*!
    \fn Balder::Probe::Destroy()
 */
void Balder::Probe::Destroy()
{
    // stop playing the hit animation
    hit_frame = 0;
    // start playing the esplode animation
    death_frame = 1;
    bounding_box.y = PROBEHEIGHT*4;
    game_manager->PlaySound("probe destroyed");
    ++deaths;
}

void Balder::Probe::Spawn()
{
    // play the spawn animation and sound
    spawn_frame=1;
    bounding_box.y = PROBEHEIGHT*6;
    game_manager->PlaySound("spawn probe");
}

bool Probe::AddImpulse(double x_vel, double y_vel)
{
    if (stuck && x_vel*x_vel+y_vel*y_vel > 0.25){
        stuck = false;
		pushOffSpeed = 0;
        pushOffState = 0;

    }
    if (!stuck){
        x_vel_add += x_vel;
        y_vel_add += y_vel;
        return true;
    }
    return false;
}

void Probe::Die()
{
    	x_velocity = 0;
    	y_velocity = 0;
        stuck = true;
    	life_remaining = MAXHITS;
    	charge_remaining = MAXCHARGE;
        shield_power = 0;
    	is_dead = false;
    	powerups = 0;
    	death_frame = 0;
        pushOffSpeed = 0;
        pushOffState = 0;
    	bounding_box.y = 0; //PROBEHEIGHT*(color-1);
    	if (removeFromGame) game_manager->RemoveEntity(this);
        else {
            game_manager->RespawnProbe(this);
            Spawn();
        }
}

void Probe::SetProbeState(double x_pos, double y_pos, double x_vel, double y_vel, bool stk)
{
	x_position = x_pos;
	y_position = y_pos;
	x_velocity = x_vel;
	y_velocity = y_vel;
	stuck = stk;
}

void Probe::RotateRight ( bool r )
{
	if (r) {rotation_state = RIGHT_ROT;}
	else if (rotation_state & RIGHT_ROT) {rotation_state = NO_ROT;}
}

void Probe::RotateLeft ( bool r )
{
	if (r) {rotation_state = LEFT_ROT;}
	else if (rotation_state & LEFT_ROT){rotation_state = NO_ROT;}
}

void Probe::Stick ( bool s )
{
	stick = s;
	if (!stick && stuck && (PUSH_NOT_READY == pushOffState)) {
		pushOffState = PUSH_READY;
	}
	else if ( stick && (PUSH_READY == pushOffState)) {
		pushOffState = PUSH_POWERING;
	}
	else if ( !stick && (PUSH_POWERING == pushOffState)) {
		PushOff();
	}
}

void Probe::PushOff (  )
{
	if (stuck){
		x_velocity = pushOffSpeed*cos(facing_angle);
		y_velocity = pushOffSpeed*sin(facing_angle);
		pushOffSpeed = 0;
		game_manager->PlaySound("probe push");
	}
	stuck = false;
	pushOffState = 0;
}

void Probe::Fire (  )
{
    // make sure you can't fire when you're dead
    if (death_frame) { return;}
	if(charge_remaining > 1){
		charge_remaining -= 1;
		// fire the projectile
		float x_direction = cos(facing_angle);
		float y_direction = sin(facing_angle);
        float x_pos = x_position + x_direction*2*PROBEWIDTH/3;
        float y_pos = y_position + y_direction*2*PROBEHEIGHT/3;
        if (!Projectile::Add_Probe_Velocity) game_manager->CreateProjectile(owner, x_pos, y_pos , x_direction*Projectile::SPEED, y_direction*Projectile::SPEED);
        else game_manager->CreateProjectile(owner, x_pos, y_pos , x_direction*Projectile::SPEED+x_velocity, y_direction*Projectile::SPEED+y_velocity);
		if (powerups & EFFECT_SPREADFIRE) { // fire 2 bonus shots
			x_direction = cos(facing_angle + M_PI/48);
			y_direction = sin(facing_angle + M_PI/48);
            x_pos = x_position + cos(facing_angle + M_PI/12)*2*PROBEWIDTH/3;
            y_pos = y_position + sin(facing_angle + M_PI/12)*2*PROBEHEIGHT/3;
            if (!Projectile::Add_Probe_Velocity) game_manager->CreateProjectile(owner, x_pos, y_pos , x_direction*Projectile::SPEED, y_direction*Projectile::SPEED);
            else game_manager->CreateProjectile(owner, x_pos, y_pos , x_direction*4+x_velocity, y_direction*Projectile::SPEED+y_velocity);
			x_direction = cos(facing_angle - M_PI/48);
			y_direction = sin(facing_angle - M_PI/48);
            x_pos = x_position + cos(facing_angle - M_PI/12)*2*PROBEWIDTH/3;
            y_pos = y_position + sin(facing_angle - M_PI/12)*2*PROBEHEIGHT/3;
            if (!Projectile::Add_Probe_Velocity) game_manager->CreateProjectile(owner, x_pos, y_pos , x_direction*Projectile::SPEED, y_direction*Projectile::SPEED);
            else game_manager->CreateProjectile(owner, x_pos, y_pos , x_direction*Projectile::SPEED+x_velocity, y_direction*Projectile::SPEED+y_velocity);
		}
		// now the reaction from firing if not stuck to a wall
		if (!stuck){
			x_velocity -= x_direction/4;
			y_velocity -= y_direction/4;
		}
		game_manager->PlaySound("fire");
	}
}



/*!
    \fn Balder::Probe::Remove()
    Permamently remove the probe from the game
 */
void Balder::Probe::Remove()
{
	//Destroy();
	removeFromGame = true;
}

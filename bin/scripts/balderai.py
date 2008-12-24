import balder
from balderutil import entity, probe
import math, random
try:
  import psyco
except ImportError:
  # no psyco, so don't use it
  pass
else:
  psyco.full()

controlled_probes = {}

class probe_tracker:
    def __init__(self):
        self.lastfire = 0
        self.pushtimer = 0
        self.fireAtTarget = True
    def incrementFrame(self):
        self.lastfire += 1

def DestroyClosestProbe(probe_id, probe_dict):
    inputs = 0
    # first, lets find out where we are and where we are aiming
    xloc = probe_dict[probe_id]['x']
    yloc = probe_dict[probe_id]['y']
    aiming = probe_dict[probe_id]['rotation_angle']
    # aim for the closest probe
    target = entity.FindClosestProbe(probe_id, probe_dict)
    if not target in probe_dict:
        return balder.INPUT_STICK
    xtarget = probe_dict[target]['x']
    ytarget = probe_dict[target]['y']
    inputs = probe.AimProbeAt(probe_dict[probe_id], xtarget, ytarget)
    if inputs == 0 and balder.IsLineOfSight((xloc,yloc),(xtarget,ytarget)):
        inputs = balder.INPUT_FIRE
    return inputs

def DoProbeControl(probe_id, probe_dict, projectile_list, powerup_list):
    # don't try to do AI for non existant probes
    if not probe_id in probe_dict:
        return 0
    global controlled_probes
    if not probe_id in controlled_probes:
       controlled_probes[probe_id] = probe_tracker()
    closestprobe_id = entity.FindClosestProbe(probe_id, probe_dict)
    thisprobe = controlled_probes[probe_id]
    thisprobe.incrementFrame()
    inputs = 0
    # first, lets find out where we are and where we are aiming
    xloc = probe_dict[probe_id]['x']
    yloc = probe_dict[probe_id]['y']
    aiming = probe_dict[probe_id]['rotation_angle']
    # first look for powerups
    if len(powerup_list) and probe_dict[probe_id]['stuck']:
        thisprobe.fireAtTarget = False
        xtarget, ytarget = entity.FindClosestEntity(xloc, yloc, powerup_list)
        if not balder.IsLineOfSight((xloc,yloc),(xtarget,ytarget)):
            thisprobe.fireAtTarget = True
            inputs = DestroyClosestProbe(probe_id, probe_dict)
        else:
            inputs = probe.AimProbeAt(probe_dict[probe_id],xtarget, ytarget)
    else:
        # aim for the closest probe
        thisprobe.fireAtTarget = True
        inputs = DestroyClosestProbe(probe_id, probe_dict)
    if inputs == balder.INPUT_FIRE:
        if closestprobe_id in probe_dict:
            closestprobe = probe_dict[closestprobe_id]
            distance = entity.DistanceTo(xloc, yloc, closestprobe)
            if (distance/10 - thisprobe.lastfire) < 0 \
            and thisprobe.fireAtTarget:
                thisprobe.lastfire = 0
        else:
            inputs = balder.INPUT_STICK
    if probe_dict[probe_id]['stuck']:
        if not thisprobe.pushtimer == 0:
            inputs = inputs | balder.INPUT_STICK
        thisprobe.pushtimer += 1
        # push off after 20 frames
        if thisprobe.pushtimer == 20:
            thisprobe.pushtimer = 0
    else:
        inputs = inputs | balder.INPUT_STICK
    return inputs

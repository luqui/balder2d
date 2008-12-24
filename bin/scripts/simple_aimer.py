import balder
from balderutil import entity
from balderutil import probe
import math
try:
  import psyco
except ImportError:
  # no psyco, so don't use it
  pass
else:
  psyco.full()

def DoProbeControl(probe_id, probe_dict, projectile_list, powerup_list):
    # don't try to do AI for non existant probes
    if not probe_id in probe_dict:
        return 0
    inputs = 0
    # first, lets find out where we are and where we are aiming]
    # aim for the closest probe
    target = entity.FindClosestProbe(probe_id, probe_dict)
    if not target in probe_dict:
        return 0
    xtarget = probe_dict[target]['x']
    ytarget = probe_dict[target]['y']
    inputs = probe.AimProbeAt(probe_dict[probe_id], xtarget, ytarget)
    if inputs == 0:
        inputs = balder.INPUT_FIRE
    return inputs

import balder
import random
try:
  import psyco
except ImportError:
  # no psyco, so don't use it
  pass
else:
  psyco.full()

def DoProbeControl(probe_id, probe_dict, projectile_list, powerup_list):
    return random.randint(0,16)

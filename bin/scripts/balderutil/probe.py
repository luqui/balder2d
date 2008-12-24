import math
import balder
try:
  import psyco
except ImportError:
  # no psyco, so don't use it
  pass
else:
  psyco.full()

def AimProbeAt(probeToAim, xtarget, ytarget):
    """ figure out how to turn to aim at the given target coordinates
    """
    # need these to be floats so that later divisions don't do integer division
    xloc = probeToAim['x']
    yloc = probeToAim['y']
    aiming = probeToAim['rotation_angle']
    xaim = float(xtarget - xloc)
    yaim = float(ytarget - yloc)
    # if xaim is very small, we are going to get something very large if we try
    # to devide yaim by xaim.  So in this case we just want to aim up or down
    if  math.fabs(xaim) < 0.1:
        if yaim == math.fabs(yaim): # yaim is positive (y is positive down)
            want_to_aim = math.pi/2 # pi/2 is straight down
        else:
            want_to_aim = 3*math.pi/2
    else: # it should be safe to compute the angle directly
        want_to_aim = math.atan(yaim/xaim)
        # this only gives us the right angle if x and y are positive though
        if xaim < 0:
            want_to_aim = want_to_aim + math.pi
        elif yaim < 0: # y negative, but x positive gives a negative angle, add 2*pi  
            want_to_aim = want_to_aim + 2*math.pi 
    # first, lets check if we are already pointed approximately that way
    if math.fabs(want_to_aim - aiming) < 0.1:
        return 0
    else: # need to adjust our aim
        if want_to_aim > aiming:
            # make sure we turn the shortest way
            if want_to_aim - aiming < math.pi:
                inputs = balder.INPUT_RIGHT
            else:
                inputs = balder.INPUT_LEFT
        else:
            # again, make sure to turn the quickest way
            if aiming - want_to_aim > math.pi:
                inputs = balder.INPUT_RIGHT
            else:
                inputs = balder.INPUT_LEFT
    return inputs

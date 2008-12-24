import math
try:
  import psyco
except ImportError:
  # no psyco, so don't use it
  pass
else:
  psyco.full()

def FindClosestProbe(this_probe_id, probe_dict, max_distance = 1000):
    """ returns the ID of the closest probe
    """
    closest_distance = max_distance**2;
    closest_probe_id = -1
    xorigin = probe_dict[this_probe_id]['x']
    yorigin = probe_dict[this_probe_id]['y']
    for probe in probe_dict.items():
        id = probe[0]
        if id == this_probe_id:
            continue
        xtarget = math.fabs(xorigin - probe_dict[id]['x'])
        ytarget = math.fabs(yorigin - probe_dict[id]['y'])
        distance = xtarget**2 + ytarget**2
        if distance < closest_distance:
            closest_distance = distance
            closest_probe_id = id
    return closest_probe_id

def FindClosestEntity(x, y, entity_list, max_distance = 1000):
    """ returns the x and y coordinates of the closest entity in the given list
        to the given point, or 0,0 if no entity is close enough
    """
    xent, yent = 0, 0
    closest_distance = max_distance**2;
    for entity in entity_list:
        xtarget = math.fabs(x - entity['x'])
        ytarget = math.fabs(y - entity['y'])
        distance = xtarget**2 + ytarget**2
        if distance < closest_distance:
            closest_distance = distance
            xent, yent = entity['x'], entity['y']
    return xent, yent

def DistanceTo(x, y, entity):
    """ returns the distance from the entity to the given point
    """
    xent = entity['x']
    yent = entity['y']
    return int(math.sqrt((x - xent)**2 + (y - yent)**2))
 
import unittest
import sys
sys.path.append('..')
from balderutil import entity

INPUT_LEFT = 1;
INPUT_RIGHT = 2;
INPUT_STICK = 4;
INPUT_PUSH = 8;
INPUT_FIRE = 16;

# for reference; n*pi/4, n=0...8:
# 0.0
# 0.785398163397
# 1.57079632679
# 2.35619449019
# 3.14159265359
# 3.92699081699
# 4.71238898038
# 5.49778714378
# 6.28318530718


class DoProbeControlTests(unittest.TestCase):
    def setUp(self):
        #lets surround the target at 4 points, 2 probes each point
        self.probe_dict = {
        1:{'x':10,'y':10, 'rotation_angle':0.0}} # the target probe
        
    def testFire(self):
        self.probe_dict[2] = {'x':10,'y':20, 'rotation_angle':4.71} # ~3*pi/2should fire
        inputs = balderai.DoProbeControl(2, self.probe_dict,[],[])
        self.failUnlessEqual( inputs, balder.INPUT_FIRE)
    
    def testOnLeftTurnLeft(self):
        self.probe_dict[3] = {'x':0,'y':10, 'rotation_angle':1.57} # ~ pi/2, needs to turn left
        inputs = balderai.DoProbeControl(3, self.probe_dict,[],[])
        self.failUnlessEqual( inputs, balder.INPUT_LEFT)
    
    def testOnLeftTurnRight(self):
        self.probe_dict[4] = {'x':0,'y':10, 'rotation_angle':4.71} # ~3*pi/2, needs to turn right
        inputs = balderai.DoProbeControl(4, self.probe_dict,[],[])
        self.failUnlessEqual( inputs, balder.INPUT_RIGHT)
        
    def testAboveTurnLeft(self):
        self.probe_dict[5] = {'x':10,'y':0, 'rotation_angle':3.14} # needs to turn left
        inputs = balderai.DoProbeControl(5, self.probe_dict,[],[])
        self.failUnlessEqual( inputs, balder.INPUT_LEFT)
    
    def testAboveTurnRight(self):
        self.probe_dict[6] = {'x':10,'y':0, 'rotation_angle':0.0} # ~pi, needs to turn right
        inputs = balderai.DoProbeControl(6, self.probe_dict,[],[])
        self.failUnlessEqual( inputs, balder.INPUT_RIGHT)
        
    def testOnRightTurnLeft(self):
        self.probe_dict[7] = {'x':20,'y':10, 'rotation_angle':4.71} # ~3*pi/2, needs to turn left
        inputs = balderai.DoProbeControl(7, self.probe_dict,[],[])
        self.failUnlessEqual( inputs, balder.INPUT_LEFT)
    
    def testOnRightTurnRight(self):
        self.probe_dict[8] = {'x':20,'y':10, 'rotation_angle':1.57} # ~pi/2, needs to turn right
        inputs = balderai.DoProbeControl(8, self.probe_dict,[],[])
        self.failUnlessEqual( inputs, balder.INPUT_RIGHT)
        
    def testBelowTurnLeft(self):
        self.probe_dict[9] = {'x':10,'y':20, 'rotation_angle':0.0} # ~needs to turn left
        inputs = balderai.DoProbeControl(9, self.probe_dict,[],[])
        self.failUnlessEqual( inputs, balder.INPUT_LEFT)
    
    def testBelowTurnRight(self):
        self.probe_dict[10] = {'x':10,'y':20, 'rotation_angle':3.14} # ~pi, needs to turn right
        inputs = balderai.DoProbeControl(10, self.probe_dict,[],[])
        self.failUnlessEqual( inputs, balder.INPUT_RIGHT)
       

class FindClosestProbeTests(unittest.TestCase):
    def testClosestProbe(self):
        probe_dict = {
        1:{'x':10,'y':10}, # the target probe
        2:{'x':10,'y':20}, 
        3:{'x':20,'y':20}, 
        4:{'x':30,'y':10}} 
        self.failUnlessEqual(2, balder.FindClosestProbe(1,probe_dict))
 
 
controlSuite = unittest.makeSuite(DoProbeControlTests)
closestProbeSuite = unittest.makeSuite(FindClosestProbeTests)
testSuite = unittest.TestSuite((controlSuite, closestProbeSuite))
unittest.TextTestRunner().run(testSuite)

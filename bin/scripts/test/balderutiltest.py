import unittest
import sys
sys.path.append('..')
from balderutil import probe, entity

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


class ProbeAimAtTests(unittest.TestCase):
    def testNoTurn(self):
        inputs = probe.AimAt(10, 20, 4.71, 10, 10)
        self.failUnlessEqual( inputs, 0)
    
    def testOnLeftTurnLeft(self):
        inputs = probe.AimAt(0, 10, 1.57, 10, 10)
        self.failUnlessEqual( inputs, INPUT_LEFT)
    
    def testOnLeftTurnRight(self):
        inputs = probe.AimAt(0, 10, 4.71, 10, 10)
        self.failUnlessEqual( inputs, INPUT_RIGHT)
        
    def testAboveTurnLeft(self):
        inputs = probe.AimAt(10, 0, 3.14, 10, 10)
        self.failUnlessEqual( inputs, INPUT_LEFT)
    
    def testAboveTurnRight(self):
        inputs = probe.AimAt(10, 0, 0.0, 10, 10)
        self.failUnlessEqual( inputs, INPUT_RIGHT)
        
    def testOnRightTurnLeft(self):
        inputs = probe.AimAt(20, 10, 4.71, 10, 10)
        self.failUnlessEqual( inputs, INPUT_LEFT)
    
    def testOnRightTurnRight(self):
        inputs = probe.AimAt(20, 10, 1.57, 10, 10)
        self.failUnlessEqual( inputs, INPUT_RIGHT)
        
    def testBelowTurnLeft(self):
        inputs = probe.AimAt(10, 20, 0.0, 10, 10)
        self.failUnlessEqual( inputs, INPUT_LEFT)
    
    def testBelowTurnRight(self):
        inputs = probe.AimAt(10, 20, 3.14, 10, 10)
        self.failUnlessEqual( inputs, INPUT_RIGHT)

class EntityFindClosestProbeTests(unittest.TestCase):
    def testClosestProbe(self):
        probe_dict = {
        1:{'x':10,'y':10}, # the target probe
        2:{'x':10,'y':10}, 
        3:{'x':10,'y':10}, 
        4:{'x':30,'y':10}} 
        self.failUnlessEqual(2, entity.FindClosestProbe(1,probe_dict))

def run():
    controlSuite = unittest.makeSuite(ProbeAimAtTests)
    closestProbeSuite = unittest.makeSuite(EntityFindClosestProbeTests)
    testSuite = unittest.TestSuite((controlSuite, closestProbeSuite))
    unittest.TextTestRunner().run(testSuite)

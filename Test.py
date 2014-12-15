#!/usr/bin/env python

import sys, math
from libavg import player, testcase, testapp

player.loadPlugin('pyglm')

class GLMTestCase(testcase.AVGTestCase):
    def __init__(self, testFuncName):
        testcase.AVGTestCase.__init__(self, testFuncName)

    def testVec3(self):
        def testHash():
            ptMap = {}
            ptMap[pyglm.vec3(0,0,0)] = 0
            ptMap[pyglm.vec3(1,0,0)] = 1
            ptMap[pyglm.vec3(0,0,0)] = 2
            self.assertEqual(len(ptMap), 2)
            self.assertEqual(ptMap[pyglm.vec3(0,0,0)], 2)
        
        def testToTupleConversion():
            pt = pyglm.vec3(10, 20, 30)
            tpl = tuple(pt)
            self.assertEqual(pt.x, tpl[0])
            self.assertEqual(pt.y, tpl[1])
            self.assertEqual(pt.z, tpl[2])

        def testFromTupleConversion():
            tpl = (15, 20, 25)
            pt = pyglm.vec3(tpl)
            self.assertEqual(pt.x, tpl[0])
            self.assertEqual(pt.y, tpl[1])
            self.assertEqual(pt.z, tpl[2])

        pt = pyglm.vec3()
        self.assertEqual(pt, pyglm.vec3(0,0,0))
        pt = pyglm.vec3(10, 20, 30)
        self.assertEqual(pt[0], pt.x)
        self.assertEqual(pt[1], pt.y)
        self.assertEqual(pt[2], pt.z)
        pt = pyglm.vec3(10, 10, 10)
        self.assertEqual(pt, pyglm.vec3(10, 10, 10))
        self.assertEqual(pt, (10, 10, 10))
        self.assertEqual(pt, pyglm.vec3([10, 10, 10]))
        self.assertNotEqual(pt, pyglm.vec3(11, 10, 10))
        self.assertEqual(str(pt), "(10,10,10)")
        pt2 = eval(repr(pt))
        self.assertEqual(pt2, pt)
        testHash()
        testFromTupleConversion()
        testToTupleConversion()
        self.assertAlmostEqual(pyglm.vec3(10, 0, 0).getNormalized(), pyglm.vec3(1, 0, 0))
        self.assertEqual(-pt, (-10, -10, -10))
        self.assertEqual(pt-(10, 0, 0), (0, 10, 10))
        self.assertEqual(pt+(10, 0, 0), (20, 10, 10))
        self.assertEqual(pt*2, (20, 20, 20))
        self.assertEqual(2*pt, (20, 20, 20))
        pt.x = 21
        pt.y = 23
        pt.z = 25
        self.assertEqual(pt, pyglm.vec3(21, 23, 25))
        pt.x -= 11
        pt.y -= 13
        pt.z -= 15
        pt += pyglm.vec3(10, 10, 10)
        self.assertEqual(pt, pyglm.vec3(20, 20, 20))
        pt -= pyglm.vec3(6, 6, 6)
        self.assertEqual(pt, pyglm.vec3(14, 14, 14))
        self.assertNotEqual(pt, pyglm.vec3(13, 13, 13))
        pt = pt/2.
        self.assertEqual(pt, pyglm.vec3(7, 7, 7))
        pt = pyglm.vec3((10, 10, 10))
        self.assertEqual(pt, (10, 10, 10))
        self.assertEqual(len(pt), 3)
        self.assertEqual(pt[0], pt.x)
        self.assertEqual(pt[1], pt.y)
        self.assertEqual(pt[2], pt.z)
        self.assertRaises(IndexError, lambda: pt[3])
        self.assertRaises(RuntimeError, pyglm.vec3(0,0,0).getNormalized)
        # boost ArgumentError can't be caught explicitly
        self.assertRaises(Exception, lambda: pyglm.vec3(0,))
        self.assertRaises(Exception, lambda: pyglm.vec3(0,1,2,3))

    def testQuat(self):
        euler1 = pyglm.vec3(math.pi/2,0,0)
        q = pyglm.quat(euler1)
        euler2 = q.toEuler()
        self.assertEqual(euler1, euler2)
        v1 = pyglm.vec3(0,10,0)
        v2 = q*v1
        self.assertEqual(v2, (0,0,10))

        invQ = q.getInverse()
        v3 = invQ*v2
        self.assert_(testcase.almostEqual(v1, v3, 0.00001))

        q2 = pyglm.quat(0.5,1,-1,0)

        self.assertEqual(q2[0], q2.w)
        self.assertEqual(q2[1], q2.x)
        self.assertEqual(q2[2], q2.y)
        self.assertEqual(q2[3], q2.z)

        self.assertEqual(str(q2), "(0.5,1,-1,0)")
        q3 = eval(repr(q2))
        self.assertAlmostEqual(q3, q2)

        euler2 = pyglm.vec3(0,0,0)
        q2 = pyglm.quat(euler2)
        qMix = pyglm.quat.slerp(q, q2, 0.5)
        eulerMix = qMix.toEuler()
        self.assert_(testcase.almostEqual(eulerMix, (math.pi/4,0,0), 0.00001))


def pyglmTestSuite(tests):
    availableTests = (
        "testVec3",
        "testQuat",
        )
    return testcase.createAVGTestSuite(availableTests, GLMTestCase, tests)

app = testapp.TestApp()

app.registerSuiteFactory('pyglm', pyglmTestSuite)
app.run()
sys.exit(app.exitCode())


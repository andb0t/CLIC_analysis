import numpy as np

from src import physics
from src import utils

# maybe have to implement px, py, pz, m etc as functions to avoid memory usage


class lorentz:
    def __init__(self, a, b, c, d, coords='PtThetaPhiE'):
        if coords == 'PtThetaPhiE':
            self.pt = a
            self.theta = b
            self.phi = c
            self.e = d
            self.eta = physics.theta_to_eta(self.theta)
            self.px = np.multiply(self.pt, np.sin(self.phi))
            self.py = np.multiply(self.pt, np.cos(self.phi))
            self.pz = np.divide(self.pt, np.tan(self.theta))
            # print('self.pt', self.pt)
            # print('self.theta', self.theta)
            # print('self.phi', self.phi)
            # print('self.e', self.e)
            # print('self.eta', self.eta)
            # print('self.px', self.px)
            # print('self.py', self.py)
            # print('self.pz', self.pz)
        elif coords == 'PxPyPzE':
            self.px = a
            self.py = b
            self.pz = c
            self.e = d
            self.pt = np.sqrt(np.add(np.square(self.px), np.square(self.py)))
            self.theta = np.arctan(np.divide(self.pt, self.pz))
            self.eta = physics.theta_to_eta(self.theta)
            self.phi = np.arctan(np.divide(self.px, self.py))
        elif coords == 'PtEtaPhiE':
            self.pt = a
            self.eta = b
            self.phi = c
            self.e = d
            self.theta = physics.eta_to_theta(self.eta)
            self.px = np.multiply(self.pt, np.sin(self.phi))
            self.py = np.multiply(self.pt, np.cos(self.phi))
            self.pz = np.divide(self.pt, np.tan(self.theta))
        self.vec = (self.e, self.px, self.py, self.pz)
        dotProd = physics.dot(self.vec, self.vec)
        if utils.isAny(np.less(dotProd, 0)):
            self.m = -np.sqrt(-dotProd)
        else:
            self.m = np.sqrt(dotProd)

    def __add__(self, other):
        px = np.add(self.px, other.px)
        py = np.add(self.py, other.py)
        pz = np.add(self.pz, other.pz)
        e = np.add(self.e, other.e)
        return lorentz(px, py, pz, e, coords='PxPyPzE')


def test(n):
    import random
    testPassed = True
    nTests = 0
    for idx in range(0, n):
        pt = random.random() * 10
        theta = random.random() * np.pi / 2
        phi = (2 * random.random() - 1) * np.pi / 2
        e = (random.random() + 1) * pt

        # pt, theta, phi, e = 1, 0.001, 0, 1.1
        # pt, theta, phi, e = 5.364010343614192, 1.3122617467593344, -1.1107031952477, 9.778291364949327

        lor = []
        # 0
        lor.append(lorentz(pt, theta, phi, e))
        # 1
        lor.append(lorentz(lor[0].px, lor[0].py, lor[0].pz, lor[0].e, coords='PxPyPzE'))
        # 2
        lor.append(lorentz(lor[0].pt, lor[0].eta, lor[0].phi, lor[0].e, coords='PtEtaPhiE'))
        # 3
        lor.append(lorentz(lor[1].pt, lor[1].theta, lor[1].phi, lor[1].e))
        # 4
        lor.append(lorentz(lor[2].pt, lor[2].theta, lor[2].phi, lor[2].e))

        items = [lor[0].m, lor[1].m, lor[2].m, lor[3].m, lor[4].m]
        isSame = all((x - items[0]) / items[0] < 0.01 for x in items)
        isAllNan = all(x != x for x in items)
        nTests += 1
        if not isSame and not isAllNan:
            testPassed = False
            print('Warning: detected unequality in round', idx)
            print('{:.3f} {:.3f} {:.3f} {:.3f}'.format(pt, theta, phi, e))
            print(items)
            for idx, l in enumerate(lor):
                print('{}: {:.3f} {:.3f} {:.3f} {:.3f} {:.3f} {:.3f} {:.3f}'
                      .format(idx, l.pt, l.theta, l.phi, l.e, l.px, l.py, l.pz))
        elif isAllNan:
            print('Warning: all values NaN')
            print(items)
    if testPassed:
        print('Test passed after', nTests, 'tests')


if __name__ == '__main__':
    test(1000)

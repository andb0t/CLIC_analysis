#!/usr/bin/env python3
import numpy as np

from src.content import lorentz


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
        lor.append(lorentz.lorentz(pt, theta, phi, e))
        # 1
        lor.append(lorentz.lorentz(lor[0].px, lor[0].py, lor[0].pz, lor[0].e, coords='PxPyPzE'))
        # 2
        lor.append(lorentz.lorentz(lor[0].pt, lor[0].eta, lor[0].phi, lor[0].e, coords='PtEtaPhiE'))
        # 3
        lor.append(lorentz.lorentz(lor[1].pt, lor[1].theta, lor[1].phi, lor[1].e))
        # 4
        lor.append(lorentz.lorentz(lor[2].pt, lor[2].theta, lor[2].phi, lor[2].e))

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

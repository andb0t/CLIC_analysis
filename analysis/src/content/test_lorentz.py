import random

import numpy as np

from src.content import lorentz


def test_random_initializations():
    n = 1000
    testPassed = True
    nTests = 0
    nFailures = 0
    for idx in range(0, n):
        pt = random.random() * 10
        theta = random.random() * np.pi
        phi = (2 * random.random() - 1) * np.pi / 2
        e = (random.random() + 2) * pt

        # pt, theta, phi, e = 1, 0.001, 0, 1.1
        # pt, theta, phi, e = 5.364010343614192, 1.3122617467593344, -1.1107031952477, 9.778291364949327

        lor = []
        lor.append(lorentz.lorentz(pt, theta, phi, e, silent=True))  # 0
        lor.append(lorentz.lorentz(lor[0].px, lor[0].py, lor[0].pz, lor[0].e, coords='PxPyPzE', silent=True))  # 1
        lor.append(lorentz.lorentz(lor[1].pt, lor[1].theta, lor[1].phi, lor[1].e, silent=True))  # 2
        lor.append(lorentz.lorentz(lor[0].pt, lor[0].eta, lor[0].phi, lor[0].e, coords='PtEtaPhiE', silent=True))  # 3
        lor.append(lorentz.lorentz(lor[3].pt, lor[3].theta, lor[3].phi, lor[3].e, silent=True))  # 4
        lor.append(lorentz.lorentz(lor[0].px, lor[0].py, lor[0].pz, lor[0].m, coords='PxPyPzM', silent=True))  # 5

        items = list(map(lambda l: l.m, lor))
        isSame = all((x - items[0]) / items[0] < 0.01 for x in items)
        isAllNan = all(x != x for x in items)
        nTests += 1
        if not isSame and not isAllNan:
            testPassed = False
            nFailures += 1
            print('Warning: detected unequality in round', idx)
            print('Initialized with', lor[0])
            # print('Masses:', items)
            for idx, l in enumerate(lor):
                print(idx, ':             ', l)
        elif isAllNan:
            print('Warning: all values NaN')
            print(items)
    if testPassed:
        print('Test passed after', nTests, 'tests')
    else:
        print('Test failed with', nFailures, '/', nTests, 'failures')


def test_inititalizations():

    def compare_lorentz_inits(px, py, pz, e, pt, eta, theta, phi, m):
        a = lorentz.lorentz(pt, theta, phi, e, coords='PtThetaPhiE', silent=True)
        b = lorentz.lorentz(px, py, pz, e, coords='PxPyPzE', silent=True)
        c = lorentz.lorentz(px, py, pz, m, coords='PxPyPzM', silent=True)
        d = lorentz.lorentz(pt, eta, phi, e, coords='PtEtaPhiE', silent=True)
        loList = [a, b, c, d]

        if any(l.equals(loList[0]) is False for l in loList):
            print("Failed equality test:")
            true = """lorentz({:.2f}, {:.2f}, {:.2f}, {:.2f}, coords=\'PtThetaPhiE\'),
                      ({:.2f}, {:.2f}, {:.2f}, {:.2f}, coords=\'PxPyPzM\') eta {:.2f}""" \
                      .format(pt, theta, phi, e, px, py, pz, m, eta)
            print('True:       ', true)
            print('PtThetaPhiE:', a)
            print('PxPyPzE:    ', b)
            print('PxPyPzM:    ', c)
            print('PtEtaPhiE:  ', d)

    px = 1
    py = 2
    pz = 3
    e = 4
    pt = 2.23607
    eta = 1.10359
    phi = 1.10715
    m = 1.41421
    theta = 0.640522
    compare_lorentz_inits(px, py, pz, e, pt, eta, theta, phi, m)

    px = 1
    py = 0
    pz = 3
    e = 4
    pt = 1
    eta = 1.81845
    phi = 0
    m = 2.44949
    theta = 0.321751
    compare_lorentz_inits(px, py, pz, e, pt, eta, theta, phi, m)

    px = 1
    py = 2
    pz = 0
    e = 4
    pt = 2.23607
    eta = -0
    phi = 1.10715
    m = 3.31662
    theta = 1.5708
    compare_lorentz_inits(px, py, pz, e, pt, eta, theta, phi, m)

    px = 1
    py = 2
    pz = 3
    e = 0
    pt = 2.23607
    eta = 1.10359
    phi = 1.10715
    m = -3.74166
    theta = 0.640522
    compare_lorentz_inits(px, py, pz, e, pt, eta, theta, phi, m)

    px = 1
    py = 2
    pz = -3
    e = 4
    pt = 2.23607
    eta = -1.10359
    phi = 1.10715
    m = 1.41421
    theta = 2.50107
    compare_lorentz_inits(px, py, pz, e, pt, eta, theta, phi, m)

    px = 1
    py = 0
    pz = -3
    e = 4
    pt = 1
    eta = -1.81845
    phi = 0
    m = 2.44949
    theta = 2.81984
    compare_lorentz_inits(px, py, pz, e, pt, eta, theta, phi, m)


def test_boost():
    for _ in range(0, 1000):
        px = random.random() * 11 - 5
        py = random.random() * 11 - 5
        pz = random.random() * 11 - 5
        e =  np.sqrt(px ** 2 + py ** 2 + pz ** 2) + 0.1 + random.random() * 10

        bx = random.random() * 11 - 5
        by = random.random() * 11 - 5
        bz = random.random() * 11 - 5
        be = np.sqrt(bx ** 2 + by ** 2 + bz ** 2) + 0.1 + random.random() * 10

        # a = lorentz.lorentz(px, py, pz, e, coords = 'PxPyPzE', silent=True)
        # b = lorentz.lorentz(bx, by, bz, be, coords = 'PxPyPzE', silent=True)
        # if abs(a.m - a.boost(b).m) > 1e-4:
        #     print('Before boost to b frame:', a)
        #     print('After boost to b frame: ', a.boost(b))

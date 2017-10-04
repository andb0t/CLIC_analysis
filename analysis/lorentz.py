import numpy as np

import functools

import physics

# maybe have to implement px, py, pz, m etc as functions to avoid memory usage


class lorentz:
    def __init__(self, a, b, c, d, coords='PtThetaPhiE'):
        if coords == 'PtThetaPhiE':
            self.pt = a
            self.theta = b
            self.phi = c
            self.e = d
            self.px = functools.reduce(np.multiply, [self.pt, np.sin(self.theta), np.sin(self.phi)])
            self.py = functools.reduce(np.multiply, [self.pt, np.sin(self.theta), np.cos(self.phi)])
            self.pz = functools.reduce(np.multiply, [self.pt, np.cos(self.theta)])
            self.eta = physics.theta_to_eta(self.theta)
        elif coords == 'PxPyPzE':
            self.px = a
            self.py = b
            self.pz = c
            self.e = d
            self.pt = np.sqrt(self.px**2 + self.py**2)
            self.theta = np.arctan(self.pt / self.pz)
            self.eta = physics.theta_to_eta(self.theta)
            self.phi = np.arctan(self.px / self.py)
        elif coords == 'PtEtaPhiE':
            self.pt = a
            self.eta = b
            self.phi = c
            self.e = d
            self.theta = physics.eta_to_theta(self.eta)
            self.px = functools.reduce(np.multiply, [self.pt, np.sin(self.theta), np.sin(self.phi)])
            self.py = functools.reduce(np.multiply, [self.pt, np.sin(self.theta), np.cos(self.phi)])
            self.pz = functools.reduce(np.multiply, [self.pt, np.cos(self.theta)])
        self.vec = (self.e, self.px, self.py, self.pz)
        self.m = np.sqrt(physics.dot(self.vec, self.vec))

    def __add__(self, other):
        px = np.add(self.px, other.px)
        py = np.add(self.py, other.py)
        pz = np.add(self.pz, other.pz)
        e = np.add(self.e, other.e)
        return lorentz(px, py, pz, e, coords='PxPyPzE')


# l = lorentz(1,1,1,3)
# h = lorentz(l.px, l.py, l.pz, l.e, coords='PxPyPzE')
# j = lorentz(l.pt, l.eta, l.phi, l.e, coords='PtEtaPhiE')

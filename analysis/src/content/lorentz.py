import numpy as np

from src.content import physics
from src.utils import utils

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

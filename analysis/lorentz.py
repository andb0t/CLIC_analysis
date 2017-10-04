import numpy as np

import functools

import physics

# maybe have to implement px, py, pz, m etc as functions to avoid memory usage


class lorentz:
    def __init__(self, pt, theta, phi, e):
        self.pt = pt
        self.theta = theta
        self.phi = phi
        self.e = e
        self.eta = physics.theta_to_eta(self.theta)
        self.px = functools.reduce(np.multiply, [pt, np.sin(theta), np.sin(phi)])
        self.py = functools.reduce(np.multiply, [pt, np.sin(theta), np.cos(phi)])
        self.pz = functools.reduce(np.multiply, [pt, np.cos(theta)])
        self.vec = (self.e, self.px, self.py, self.pz)
        self.m = np.sqrt(physics.dot(self.vec, self.vec))

    def __add__(self, other):
        pt = np.add(self.pt, other.pt)
        theta = np.add(self.theta, other.theta)
        phi = np.add(self.phi, other.phi)
        e = np.add(self.e, other.e)
        return lorentz(pt, theta, phi, e)

    def __mul__(self, other):
        pt = np.multiply(self.pt, other.pt)
        theta = np.multiply(self.theta, other.theta)
        phi = np.multiply(self.phi, other.phi)
        e = np.multiply(self.e, other.e)
        return lorentz(pt, theta, phi, e)

import numpy as np

from src.content import physics


class lorentz:
    """ A class for Lorentz vectors

    Keyword arguments:
    coords -- coordinate system for initialization
    """

    def __init__(self, a, b, c, d, coords='PtThetaPhiE', silent=False):
        self.silent = silent

        np.seterr(divide='ignore', invalid='ignore')

        if coords == 'PtThetaPhiE':
            self.pt = a
            self.theta = b
            self.phi = c
            self.e = d
            self.px = np.multiply(self.pt, np.cos(self.phi))
            self.py = np.multiply(self.pt, np.sin(self.phi))

            self.eta = physics.theta_to_eta(self.theta)
            self.pz = np.divide(self.pt, np.tan(self.theta))

            self.vec = (self.e, self.px, self.py, self.pz)
            dotProd = physics.dot(self.vec, self.vec)
            self.m = np.multiply(np.sign(dotProd), np.sqrt(np.absolute(dotProd)))

        elif coords == 'PxPyPzE':
            self.px = a
            self.py = b
            self.pz = c
            self.e = d
            self.vec = (self.e, self.px, self.py, self.pz)
            dotProd = physics.dot(self.vec, self.vec)
            self.m = np.multiply(np.sign(dotProd), np.sqrt(np.absolute(dotProd)))
            self.pt = np.sqrt(np.add(np.square(self.px), np.square(self.py)))
            self.theta = np.vectorize(physics.pz_pt_to_theta)(self.pz, self.pt)
            self.eta = physics.theta_to_eta(self.theta)
            self.phi = np.arctan(np.divide(self.py, self.px))

        elif coords == 'PxPyPzM':
            self.px = a
            self.py = b
            self.pz = c
            self.m = d
            self.e = np.nan_to_num(np.sqrt(np.add(np.multiply(np.square(self.m), np.sign(self.m)), np.add(np.square(self.px), np.add(np.square(self.py), np.square(self.pz))))))
            self.pt = np.sqrt(np.add(np.square(self.px), np.square(self.py)))
            self.theta = np.vectorize(physics.pz_pt_to_theta)(self.pz, self.pt)
            self.eta = physics.theta_to_eta(self.theta)
            self.phi = np.arctan(np.divide(self.py, self.px))

        elif coords == 'PtEtaPhiE':
            self.pt = a
            self.eta = b
            self.phi = c
            self.e = d
            self.theta = physics.eta_to_theta(self.eta)
            self.px = np.multiply(self.pt, np.cos(self.phi))
            self.py = np.multiply(self.pt, np.sin(self.phi))
            self.pz = np.divide(self.pt, np.tan(self.theta))
            self.vec = (self.e, self.px, self.py, self.pz)
            dotProd = physics.dot(self.vec, self.vec)
            self.m = np.multiply(np.sign(dotProd), np.sqrt(np.absolute(dotProd)))

        else:
            raise NotImplementedError("coordinate system \'{}\' is not implemented".format(coords))

        try:
            if (self.m < 0).any() or (self.e < 0).any() or (self.e < self.m).any():
                if not self.silent:
                    print('Warning: unphysical entity')
        except AttributeError:
            if self.m < 0 or self.e < 0 or self.e < self.m:
                if not self.silent:
                    print('Warning: unphysical entity', self)

        np.seterr(divide='warn', invalid='warn')

    def __str__(self):
        useDegres = False
        return 'lorentz({:.2f}, {:.2f}, {:.2f}, {:.2f}, coords=\'PtThetaPhiE\'), ({:.2f}, {:.2f}, {:.2f}, {:.2f}, coords=\'PxPyPzM\') eta {:.2f}'.format(
            self.pt, physics.toDegrees(self.theta) if useDegres else self.theta, physics.toDegrees(self.phi) if useDegres else self.phi, self.e, self.px, self.py, self.pz, self.m, self.eta)

    def __neg__(self):
        return lorentz(-self.px, -self.py, -self.pz, self.e, coords='PxPyPzE', silent=self.silent)

    def __add__(self, other):
        px = np.add(self.px, other.px)
        py = np.add(self.py, other.py)
        pz = np.add(self.pz, other.pz)
        e = np.add(self.e, other.e)
        return lorentz(px, py, pz, e, coords='PxPyPzE', silent=self.silent)

    def __sub__(self, other):
        return self + -other

    def __eq__(self, other):
        return (self.pt == other.pt and
                self.eta == other.eta and
                self.phi == other.phi and
                self.e == other.e and
                self.theta == other.theta and
                self.px == other.px and
                self.py == other.py and
                self.pz == other.pz and
                self.m == other.m)

    def equals(self, other):
        precisionEnergy = 1e-2
        precisionAngle = 1e-5
        return (self.pt - other.pt < precisionEnergy and
                self.eta - other.eta < precisionAngle and
                self.phi - other.phi < precisionAngle and
                self.e - other.e < precisionEnergy and
                self.theta - other.theta < precisionAngle and
                self.px - other.px < precisionEnergy and
                self.py - other.py < precisionEnergy and
                self.pz - other.pz < precisionEnergy and
                self.m - other.m < precisionEnergy)

    def boost(self, other):
        """Boost the Lorentz vector to the frame of another four-vector
        other -- the four-vector of the desired reference frame (mass/time term is ignored)
        """

        bx = np.divide(other.px, other.m)
        by = np.divide(other.py, other.m)
        bz = np.divide(other.pz, other.m)
        gamma = np.divide(1, np.sqrt(1 - bx ** 2 - by ** 2 - bz ** 2))

        # if 1 - bx ** 2 - by ** 2 - bz ** 2 < 0:
        #     print(self)
        #     print(other)
        #     print('Boost vector', bx, by, bz , 'gamma', gamma)

        bp = bx * self.px + by * self.py + bz * self.pz
        bMag = np.sqrt(np.square(bx) + np.square(by) + np.square(bz))

        newE = np.multiply(gamma, np.add(self.e, - bp))

        newPx = self.px + (gamma - 1) * bp * bx / (bMag ** 2) - gamma * self.e * bx
        newPy = self.py + (gamma - 1) * bp * by / (bMag ** 2) - gamma * self.e * by
        newPz = self.pz + (gamma - 1) * bp * bz / (bMag ** 2) - gamma * self.e * bz

        return lorentz(newPx, newPy, newPz, newE, coords='PxPyPzE', silent=self.silent)

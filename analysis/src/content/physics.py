import numpy as np

from src.utils import utils


def dot(a, b, metric=(1, -1, -1, -1)):
    try:
        result = np.zeros(len(a[0]))
    except TypeError:
        result = 0
    for i, m in enumerate(metric):
        try:
            result += np.multiply(a[i], b[i]) * m
        except IndexError:
            print('Warning: specified metric larger than vectors')
            break
    return result

def eta_to_theta(eta):
    return 2 * np.arctan(np.exp(-eta))


def theta_to_eta(theta):
    logArg = np.tan(theta / 2)
    return -np.log(logArg)

def coerce_theta(theta):
    """ Make sure theta is 0 <= theta < Pi """
    return theta
    if theta >= 0 and theta < np.pi:
        return theta
    elif theta < 0:
        return coerce_theta(-theta)
    elif theta > 2 * np.pi:
        return coerce_theta(theta - 2 * np.pi)
    elif theta >= np.pi:
        return coerce_theta(np.pi - 2 * (theta - np.pi))
    else:
        print('this cannot be happen')

def pz_pt_to_theta(pz, pt):
    if pz >= 0:
        return np.arctan(np.divide(pt, pz))
    else:
        return np.pi + np.arctan(np.divide(pt, pz))

def toDegrees(rad):
    return rad / (2 * np.pi) * 360

def toRad(degree):
    return degree / 360 * (2 * np.pi)

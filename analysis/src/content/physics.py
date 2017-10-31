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

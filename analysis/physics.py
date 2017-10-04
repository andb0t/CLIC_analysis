import numpy as np

import utils

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
    if utils.isAny(np.less(logArg, 0)):
        print('Warning: log arg < 0. Return None!')
        # print('theta', theta, 'logArg', logArg, ' < 0. Return None!')
        return None
    return -np.log(logArg)

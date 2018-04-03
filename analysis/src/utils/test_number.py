import numpy as np
import pytest

from src.utils.number import PhysicsNumber


a = PhysicsNumber(2, 2)
b = PhysicsNumber(1, 1)
c = PhysicsNumber(3, 4)
d = PhysicsNumber(1, 1)
e = PhysicsNumber(2, unit='GeV')
f = PhysicsNumber(2, unit='TeV')


def test_equality():
    assert b == c


def test_addition():
    assert a + b == PhysicsNumber(3, np.sqrt(5))
    assert a + 3 == PhysicsNumber(5, 2)
    assert 3 + a == PhysicsNumber(5, 2)


def test_subtraction():
    assert a - b == PhysicsNumber(1, np.sqrt(5))
    assert a - 3 == PhysicsNumber(-1, 2)
    assert 3 - a == PhysicsNumber(1, 2)


def test_division():
    assert a / b == PhysicsNumber(2, np.sqrt(2**2 + 2**2))
    assert a / c == PhysicsNumber(2, np.sqrt((2/3)**2 + (4 * 2 / 3 ** 2) ** 2))
    assert 2 / b == PhysicsNumber(2, 2)
    assert a / 3 == PhysicsNumber(2, 2/3)


def test_multiplication():
    assert a * b == PhysicsNumber(2, np.sqrt(2 ** 2 + 2 ** 2))
    assert a * c == PhysicsNumber(6, np.sqrt((2 * 4) ** 2 + (2 * 3) ** 2))
    assert a * 3 == PhysicsNumber(6, 2 * 3)
    assert 3 * a == PhysicsNumber(6, 3 * 2)


def test_power():
    assert a ** 2 == PhysicsNumber(4, 8)
    assert a ** 0.5 == PhysicsNumber(np.sqrt(2), np.sqrt(2) / 2)


def test_addition_exception():
    with pytest.raises(TypeError):
        a + e
    with pytest.raises(TypeError):
        e + f

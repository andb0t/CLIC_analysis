from src.content import physics


def test_dot():
    assert physics.dot([0, 0, 0, 0],  [0, 0, 0, 0]) == 0
    assert physics.dot([1, 1, 1, 1],  [1, 1, 1, 1]) == -2
    assert physics.dot([4, 6, 2, 1],  [1, 2, 3, 4]) == -18

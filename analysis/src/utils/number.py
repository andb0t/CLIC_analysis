import math


class PhysicsNumber:
    def __init__(self, value, unc=0, sep='+-', unit=''):
        self.value = value
        if unc == 'stat':
            self.uncertainty = math.sqrt(self.value)
        else:
            self.uncertainty = unc
        self.sep = sep
        self.unit = unit

    def val(self):
        return self.value

    def unc(self):
        return self.uncertainty

    def __str__(self):
        return '{} {} {}{}{}'.format(self.value, self.sep, self.uncertainty, ' ' if self.unit else '', self.unit)

    def __format__(self, format_spec):
        return format(self.value, format_spec) + ' {} '.format(self.sep) + \
            format(self.uncertainty, format_spec) + (' ' if self.unit else '') + self.unit

    def __neg__(self):
        return PhysicsNumber(-self.value, self.uncertainty, sep=self.sep, unit=self.unit)

    def __add__(self, other):
        try:
            value = self.value + other.value
            unc = math.sqrt(self.uncertainty ** 2 + other.uncertainty ** 2)
        except AttributeError:
            value = self.value + other
            unc = self.uncertainty
        return PhysicsNumber(value, unc, sep=self.sep, unit=self.unit)

    def __mul__(self, other):
        try:
            value = self.value * other.value
            unc = math.sqrt((other.uncertainty * self.value) ** 2 +
                            (self.uncertainty * other.value) ** 2)
        except AttributeError:
            value = self.value * other
            unc = self.uncertainty * other
        return PhysicsNumber(value, unc, sep=self.sep, unit=self.unit)

    def __truediv__(self, other):
        try:
            value = self.value / other.value
            unc = math.sqrt((self.uncertainty / other.value) ** 2 +
                            (other.uncertainty * self.value / other.value ** 2) ** 2)
        except AttributeError:
            value = self.value / other
            unc = self.uncertainty / other
        return PhysicsNumber(value, unc, sep=self.sep, unit=self.unit)

    def __rtruediv__(self, other):
        try:
            value = other.value / self.value
            unc = math.sqrt((other.uncertainty / self.value) ** 2 +
                            (self.uncertainty * other.value / self.value ** 2) ** 2)
        except AttributeError:
            value = other / self.value
            unc = self.uncertainty * other / self.value ** 2
        return PhysicsNumber(value, unc, sep=self.sep, unit=self.unit)

    def __sub__(self, other):
        return self + -other

    def __radd__(self, other):
        return self + other

    def __rsub__(self, other):
        return other + -self

    def __rmul__(self, other):
        return self * other

    def __getitem__(self, i):
        if i == 0:
            return self.value
        elif i == 1:
            return self.uncertainty
        else:
            raise IndexError('indices > 1 not implemented!')

# a = PhysicsNumber(2, 2)
# b = PhysicsNumber(1, 1)
# c = PhysicsNumber(3, 3)
# d = PhysicsNumber(2, 'stat', sep='\pm', unit='GeV')
# one = PhysicsNumber(1, 0)
# two = PhysicsNumber(2, 0)
#
# e = d / 2
# print(e)

import math


class PhysicsNumber:
    """A value with an uncertainty

    Keyword arguments:
    value       -- the central value of the number
    unc         -- its uncertainty, use 'stat' for automatic assignment to sqrt(value)
    sep         -- the separator between value and uncertainty for printing
    unit        -- the unit of the number
    statEntries -- a modifier to unc = 'stat' to use scaled statistical uncertainties relative to sqrt(statEntries) / statEntries

    """
    def __init__(self, value, unc=0, sep='+-', unit='', statEntries=None):
        self.value = value
        self.uncertainty = unc
        self.sep = sep
        self.unit = unit

    def val(self):
        return self.value

    def unc(self):
        return self.uncertainty

    def __str__(self):
        if (self.uncertainty):
            return '{} {} {}{}{}'.format(self.value, self.sep, self.uncertainty, ' ' if self.unit else '', self.unit)
        else:
            return '{}{}{}'.format(self.value, ' ' if self.unit else '', self.unit)

    def __format__(self, format_spec):
        if (self.uncertainty):
            return format(self.value, format_spec) + ' {} '.format(self.sep) + \
                format(self.uncertainty, format_spec) + (' ' if self.unit else '') + self.unit
        else:
            return format(self.value, format_spec) + (' ' if self.unit else '') + self.unit

    def __eq__(self, other):
        return (self.value == other.value and \
                self.uncertainty == other.uncertainty and \
                self.sep == other.sep and \
                self.unit == other.unit
                )

    def __neg__(self):
        return PhysicsNumber(-self.value, self.uncertainty, sep=self.sep, unit=self.unit)

    def __add__(self, other):
        try:
            assert (self.unit == other.unit), 'unit mismatch'
            value = self.value + other.value
            unc = math.sqrt(self.uncertainty ** 2 + other.uncertainty ** 2)
        except AttributeError:
            assert (self.unit == ''), 'unit mismatch'
            value = self.value + other
            unc = self.uncertainty
        return PhysicsNumber(value, unc, sep=self.sep, unit=self.unit)

    def __sub__(self, other):
        return self + -other

    def __mul__(self, other):
        try:
            value = self.value * other.value
            unc = math.sqrt((other.uncertainty * self.value) ** 2 +
                            (self.uncertainty * other.value) ** 2)
            unit = (self.unit + '*' + other.unit) if self.unit or other.unit else ''
        except AttributeError:
            value = self.value * other
            unc = self.uncertainty * other
            unit = self.unit
        return PhysicsNumber(value, unc, sep=self.sep, unit=unit)

    def __pow__(self, other):
        return PhysicsNumber(math.pow(self.value, other), self.uncertainty * other * math.pow(self.value, other-1))

    def __truediv__(self, other):
        try:
            value = self.value / other.value
            unc = math.sqrt((self.uncertainty / other.value) ** 2 +
                            (other.uncertainty * self.value / other.value ** 2) ** 2)
            unit = (self.unit + '/' + other.unit) if self.unit or other.unit else ''
        except AttributeError:
            value = self.value / other
            unc = self.uncertainty / other
            unit = self.unit
        return PhysicsNumber(value, unc, sep=self.sep, unit=unit)

    def __rtruediv__(self, other):
        try:
            value = other.value / self.value
            unc = math.sqrt((other.uncertainty / self.value) ** 2 +
                            (self.uncertainty * other.value / self.value ** 2) ** 2)
            unit = (other.unit + '/' + self.unit) if self.unit or other.unit else ''
        except AttributeError:
            value = other / self.value
            unc = self.uncertainty * other / self.value ** 2
            unit = ('1/' + self.unit) if self.unit else ''
        return PhysicsNumber(value, unc, sep=self.sep, unit=unit)

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
# d = PhysicsNumber(1, 1)
# one = PhysicsNumber(1, 0)
# two = PhysicsNumber(2, 0)

# g = PhysicsNumber(2, unit='GeV')
# print(g)

# print(b == d)
import os.path

from src import settings
from src.utils.number import PhysicsNumber


def save_value_latex(name, newcommand, value, unit, digits=2):
    with open(os.path.join(settings.TEX_NCMD_DIR, name), 'w') as myfile:
        try:
            texVal = PhysicsNumber(value.val(), value.unc(), sep=r'\pm', unit=unit)
        except AttributeError:
            texVal = value
        print(r'\newcommand{\\'[:-1]+ newcommand + '}{' + '{0:.{1}f}'.format(texVal, digits) + '}', file=myfile)

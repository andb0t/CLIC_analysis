import os.path

from src import settings
from src.utils.number import PhysicsNumber


def save_value_latex(name, newcommand, value, unit):
    with open(os.path.join(settings.TEX_DIR, name), 'w') as myfile:
        texVal = PhysicsNumber(value.val(), value.unc(), sep=r'\pm', unit=unit)
        print(r'\newcommand{\\'[:-1]+ newcommand + '}{' + '{:.2f}'.format(texVal) + '}', file=myfile)

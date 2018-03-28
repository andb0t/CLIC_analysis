import os.path

from src import settings
from src.utils.number import PhysicsNumber


def save_value_latex(name, newcommand, value, unit='', digits=2):
    print('Writing LateX newcommand {} in {}'.format(newcommand, name))
    spaceUnit = '~' + unit if unit else unit
    try:
        texVal = PhysicsNumber(value.val(), value.unc(), sep=r'\pm', unit=spaceUnit)
    except AttributeError:
        texVal = PhysicsNumber(value, 0, unit=spaceUnit)
    with open(os.path.join(settings.TEX_NCMD_DIR, name), 'w') as myfile:
        print(r'\newcommand{\\'[:-1]+ newcommand + r'}{\ensuremath{' + '{0:.{1}f}'.format(texVal, digits) + r'}\xspace}', file=myfile)

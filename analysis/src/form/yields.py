import math
import re
import tabulate

from src import settings
from src.utils.number import PhysicsNumber


def tabulate_escape_latex():
    # remove backslash escape from tabulate-latex
    backslashKey = []
    for key, value in tabulate.LATEX_ESCAPE_RULES.items():
        if 'textbackslash' in value:
            backslashKey.append(key)
        if '\\$' in value:
            backslashKey.append(key)
    if backslashKey:
        for key in backslashKey:
            del tabulate.LATEX_ESCAPE_RULES[key]


def print_event_yields(dataCont, name='event', latex=False, silent=False, noUnc=False):
    headers = ['Sample', 'Entries', 'Events']

    if latex:
        tabulate_escape_latex()

    # add single contributions
    sep = {'sep': '$\\pm$'} if latex else {}
    totEntries = PhysicsNumber(0, 0, **sep)
    totEvents = PhysicsNumber(0, 0, **sep)
    bkgEntries = PhysicsNumber(0, 0, **sep)
    bkgEvents = PhysicsNumber(0, 0, **sep)
    table = []
    for cont in dataCont:
        if noUnc:
            entries = PhysicsNumber(cont.get_entries(), 0, **sep)
            events = PhysicsNumber(0, 0, **sep) if math.isnan(cont.get_events()) else PhysicsNumber(cont.get_events(), 0, **sep)
        else:
            entries = PhysicsNumber(cont.get_entries(), cont.get_entries_unc(), **sep)
            events = PhysicsNumber(0, 0, **sep) if math.isnan(cont.get_events()) else PhysicsNumber(cont.get_events(), cont.get_events_unc(), **sep)
        table.append([cont.name, '{:.0f}'.format(entries), '{:.1f}'.format(events)])
        totEntries += entries
        totEvents += events
        if 'signal' not in cont.name.lower():
            bkgEntries += entries
            bkgEvents += events

    # add summary values
    entryFrac = bkgEntries / totEntries * 100
    eventFrac = bkgEvents / totEvents * 100
    entryFrac.unit= '%'
    eventFrac.unit= '%'
    table.append(['Signal + background', '{:.0f}'.format(totEntries), '{:.1f}'.format(totEvents)])
    table.append(['Background fraction', '{:.1f}'.format(entryFrac), '{:.1f}'.format(eventFrac)])

    if not silent:
        print(tabulate.tabulate(table, headers=headers, tablefmt='grid'))
    if latex:
        fileName = settings.TEX_DIR + name.lower() + '_yield.tex'
        print('Writing', name, 'yield to', fileName)
        with open(fileName, 'w') as myfile:
            print(r'\begin{table}', file=myfile)
            print(tabulate.tabulate(table, headers=headers, tablefmt='latex'), file=myfile)
            print('\caption{Event yields for the', name.replace('_', '\_').lower(), 'sample}', file=myfile)
            print(r'\end{table}', file=myfile)


def print_samples(conts, latex=False, name=''):

    if latex:
        tabulate_escape_latex()

    def extract_int(str):
        return int(re.search(r'\d+', str).group())

    headers = ['Sample', 'ID', 'Xsec [fb]']
    table = [[cont.origName, extract_int(cont.fileName), cont.xSec] for cont in conts]
    print(tabulate.tabulate(table, headers=headers, tablefmt='grid'))
    if latex:
        if name:
            name = name + '_'
        fileName = settings.TEX_DIR + name + 'samples.tex'
        print('Writing to', fileName)
        with open(fileName, 'w') as myfile:
            print(r'\begin{table}', file=myfile)
            print(tabulate.tabulate(table, headers=headers, tablefmt='latex'), file=myfile)
            print('\caption{Samples used in the analysis}', file=myfile)
            print(r'\end{table}', file=myfile)
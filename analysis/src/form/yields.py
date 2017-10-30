import re
import tabulate

from src import settings


def print_event_yields(dataCont, name='event', latex=False, silent=False):
    headers = ['Sample', 'Entries', 'Events']

    # add single contributions
    totEntries = 0
    totEvents = 0
    bkgEntries = 0
    bkgEvents = 0
    table = []
    for cont in dataCont:
        entries = cont.df.shape[0]
        events = cont.df[settings.SF].sum()
        print(cont.df[settings.SF])
        table.append([cont.name, '{:d}'.format(entries), '{:.1f}'.format(events)])
        totEntries += entries
        totEvents += events
        if 'signal' not in cont.name:
            bkgEntries += entries
            bkgEvents += events
        print('After adding', cont.name, ':', totEntries, totEvents, bkgEntries, bkgEvents)

    print(totEntries, totEvents, bkgEntries, bkgEvents)

    # add summary values
    entryFrac = bkgEntries / totEntries
    eventFrac = bkgEvents / totEvents
    table.append(['Signal + background', '{:d}'.format(totEntries), '{:.1f}'.format(totEvents)])
    table.append(['Background fraction', '{:.1%}'.format(entryFrac), '{:.1%}'.format(eventFrac)])

    if not silent:
        print(tabulate.tabulate(table, headers=headers, tablefmt='grid'))
    if latex:
        fileName = settings.TEX_DIR + name.lower() + '_yield.tex'
        print('Writing', name, 'yield to', fileName)
        with open(fileName, 'w') as myfile:
            print(r'\begin{table}', file=myfile)
            print(tabulate.tabulate(table, headers=headers, tablefmt='latex'), file=myfile)
            print('\caption{Event yields for the', name.lower(), 'sample}', file=myfile)
            print(r'\end{table}', file=myfile)


def print_samples(conts, latex=False):

    def extact_int(str):
        return int(re.search(r'\d+', str).group())

    headers = ['Sample', 'ID', 'Xsec [fb]']
    table = [[cont.origName, extact_int(cont.fileName), cont.xSec] for cont in conts]
    print(tabulate.tabulate(table, headers=headers, tablefmt='grid'))
    if latex:
        fileName = settings.TEX_DIR + 'samples.tex'
        print('Writing to', fileName)
        with open(fileName, 'w') as myfile:
            print(r'\begin{table}', file=myfile)
            print(tabulate.tabulate(table, headers=headers, tablefmt='latex'), file=myfile)
            print('\caption{Samples used in the analysis}', file=myfile)
            print(r'\end{table}', file=myfile)
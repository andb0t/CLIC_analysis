import tabulate

from src import settings

def print_event_yields(dataCont, name='event', latex=False, silent=False):
	headers = ['Sample', 'Entries', 'Events']

	# add single contributions
	totEntries = 0
	totEvents = 0
	bkgEvents = 0
	bkgEntries = 0
	table = []
	for cont in dataCont:
		entries = cont.df.shape[0]
		events = cont.df[settings.SF].sum()
		table.append([cont.name, '{:d}'.format(entries), '{:.1f}'.format(events)])
		totEntries += entries
		totEvents += events
		if 'signal' not in cont.name:
			bkgEntries += entries
			bkgEvents += events

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
		with open(fileName, 'w')  as myfile:
			print(r'\begin{table}', file=myfile)
			print(tabulate.tabulate(table, headers=headers, tablefmt='latex'), file=myfile)
			print('\caption{Event yields for the', name.lower(), 'sample}', file=myfile)
			print(r'\end{table}', file=myfile)

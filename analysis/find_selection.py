import argparse
import sklearn

from src import settings


parser = argparse.ArgumentParser()
parser.add_argument("--full", action="store_true", default=False, help='Execute on full data files, not on small test samples')
parser.add_argument("--maxevt", nargs='?', type=int, help="Specify number maximum number of events", default=None)
args = parser.parse_args()

print(args.full)
print(args.maxevt)
# SETTINGS

# Directories
EXAMPLE_DATA_DIR = 'example_data/'
FULL_DATA_DIR = '../data/csv/'
EXAMPLE_PLOT_DIR = 'fig/fig_ex/'
FULL_PLOT_DIR = 'fig/fig_full/'
EXAMPLE_TEX_DIR = 'tex/tex_ex/'
FULL_TEX_DIR = 'tex/tex_full/'

# Luminosity in 1/fb
LUMI = 1500

# Samples with x-sec in fb
SIG_SAMPLE = {'csv': '3249.csv', 'xs': 4309.7}
QQLL_SAMPLE = {'csv': '3246.csv', 'xs': 2725.8}
QQQQLL_SAMPLE = {'csv': '5572.csv', 'xs': 62.1}
QQNN_SAMPLE = {'csv': '2094.csv', 'xs': 933.9}
QQ_SAMPLE = {'csv': '2091.csv', 'xs': 4009.5}
QQQQ_SAMPLE = {'csv': '4034.csv', 'xs': 1245.1}
QQQQNN_SAMPLE = {'csv': '5527.csv', 'xs': 23.2}
QQQQLN_SAMPLE = {'csv': '5594.csv', 'xs': 110.4}
NEWDET_SIG_SAMPLE = {'csv': '9402.csv', 'xs': 4309.7}

# Objects
JET = 'jet_vlc_R08_'
LEP = 'lep_'
MISS = 'miss_'
MC = 'mc_'
SF = 'scaleFactor'
EL = 'el_'
MU = 'mu_'


# FUNCTIONALITY

DATA_DIR = ''
PLOT_DIR = ''
TEX_DIR = ''


def set_data_dir(dataDir):
    global DATA_DIR
    DATA_DIR = dataDir

def set_plot_dir(plotDir):
    global PLOT_DIR
    PLOT_DIR = plotDir

def set_tex_dir(texDir):
    global TEX_DIR
    TEX_DIR = texDir

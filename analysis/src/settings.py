# SETTINGS

# Directories
EXAMPLE_DATA_DIR = 'example_data/'
FULL_DATA_DIR = '../data/csv/'
EXAMPLE_PLOT_DIR = 'fig/fig_ex/'
FULL_PLOT_DIR = 'fig/fig_full/'
EXAMPLE_TEX_DIR = 'tex/tex_ex/'
FULL_TEX_DIR = 'tex/tex_full/'
EXAMPLE_TEX_NCMD_DIR = 'tex/tex_ex/newcommands/'
FULL_TEX_NCMD_DIR = 'tex/tex_full/newcommands/'

# Luminosity in 1/fb
LUMI = 1500

# Energy in GeV
ENERGY = 1400

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
SF = 'scaleFactor'
EL = 'el_'
MU = 'mu_'

# Plots
plot_descriptor = 'CLIC ILD @ {} TeV, $\mathcal{{L}} = {}$ ab$^{{-1}}$'.format(ENERGY / 1000, LUMI / 1000)


# FUNCTIONALITY (ONLY TOUCH IF YOU KNOW WHAT YOU ARE DOING)

DATA_DIR = ''
PLOT_DIR = ''
TEX_DIR = ''
TEX_NCMD_DIR = ''


def set_data_dir(dataDir):
    global DATA_DIR
    DATA_DIR = dataDir

def set_plot_dir(plotDir):
    global PLOT_DIR
    PLOT_DIR = plotDir

def set_tex_dir(texDir):
    global TEX_DIR
    TEX_DIR = texDir

def set_tex_ncmd_dir(texNcmdDir):
    global TEX_NCMD_DIR
    TEX_NCMD_DIR = texNcmdDir

def init_globals(full=False):
    if full:
        set_data_dir(FULL_DATA_DIR)
        set_plot_dir(FULL_PLOT_DIR)
        set_tex_dir(FULL_TEX_DIR)
        set_tex_ncmd_dir(FULL_TEX_NCMD_DIR)
    else:
        set_data_dir(EXAMPLE_DATA_DIR)
        set_plot_dir(EXAMPLE_PLOT_DIR)
        set_tex_dir(EXAMPLE_TEX_DIR)
        set_tex_ncmd_dir(EXAMPLE_TEX_NCMD_DIR)
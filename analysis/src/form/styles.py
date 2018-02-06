from src import settings


def modified_title(axis):
    common_title = 'CLIC ILD @ {} TeV, $\mathcal{{L}} = {}$ ab$^{{-1}}$'.format(settings.ENERGY / 1000, settings.LUMI / 1000)
    curr_title = axis.get_title()
    if curr_title:
        return curr_title + '\n' + common_title
    return common_title


def style_hist(axis):
    axis.grid(True, which='both')
    axis.legend(loc="best")
    axis.margins(x=0)
    axis.set_title(modified_title(axis), fontsize=8)


def style_raw(axis):
    axis.grid(True, which='both')
    axis.legend(loc="best")
    axis.set_title(modified_title(axis), fontsize=8)


def style_scatter(axis):
    axis.grid(True, which='both')
    axis.set_title(modified_title(axis), fontsize=8)


def style_corr(axis):
    axis.xaxis.set_ticks_position('bottom')
    axis.set_title(modified_title(axis), fontsize=12)


def style_heat(axis):
    axis.set_title(modified_title(axis), fontsize=8)

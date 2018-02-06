from src import settings


def modified_title(axis):
    curr_title = axis.get_title()
    if curr_title:
        return curr_title + '\n' + settings.plot_descriptor
    return settings.plot_descriptor


def style_hist(axis):
    axis.grid(True, which='both')
    axis.margins(x=0)
    axis.set_title(modified_title(axis), fontsize=8)
    lg = axis.legend(loc="best", prop={'size': 6}, title='')
    lg.get_title().set_fontsize(7)


def style_raw(axis):
    axis.grid(True, which='both')
    axis.set_title(modified_title(axis), fontsize=8)
    lg = axis.legend(loc="best", prop={'size': 6}, title='')
    lg.get_title().set_fontsize(7)

def style_scatter(axis):
    axis.grid(True, which='both')
    axis.set_title(modified_title(axis), fontsize=8)
    lg = axis.legend(loc="best", prop={'size': 6}, title='')
    lg.get_title().set_fontsize(7)

def style_corr(axis):
    axis.xaxis.set_ticks_position('bottom')
    axis.set_title(modified_title(axis), fontsize=12)


def style_heat(axis):
    axis.set_title(modified_title(axis), fontsize=8)

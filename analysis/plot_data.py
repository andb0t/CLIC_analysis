import math

from matplotlib import pyplot as plt
import numpy as np
import pandas as pd


plt.rcParams['figure.figsize'] = (8.0, 8.0)

# tells matplotlib how to show you the plots, there are multiple differnt options in addition to 'inline'
get_ipython().magic('matplotlib inline')


# ### Importing data with pandas

# path to the cernbox with HexDAQ data in it
root = "C:\\Users\\Andreas Maier\\Dropbox\\Projects\\Python\\CLIC_analysis"

fname = "{}\\all_output_small.txt".format(root)
data = pd.read_csv(fname, sep="\t", comment="#", index_col=False, engine="python", header=0)
names = list(data.dtypes.index)[:-1]


def plot_input(suffix=''):
    fig, ax = plt.subplots(1, 1)

    for name in names:
        if name.find(suffix) != -1:
            ax.plot(getattr(data, name), label=name)
    # ax.plot(data.lep_n, label="n leptons")
    # ax.plot(data.jet_DH_n, label="n jets DH")
    # ax.plot(data.jet_KT_R05_n, label="n jets KT5")
    ax.grid(True, which='both')
    ax.set(ylabel='entries', xlabel='event')
    ax.legend(loc="best")

    ax.grid(True, which='both')
    ax.margins(x=0)

    fig.savefig("input.pdf")


plot_input('_n')


# ## Curve fitting

# In[4]:


#scipy has loads of useful scientific functions
from scipy.optimize import curve_fit

#define a model function that takes an array (the x axis) and any number of parameters
#in this case I call it f because it's a frequency
#i'm only fitting the magnitude of the complex impedance
def parrallel(f, R, C):
    Z = 1/(1/R + 1.0j * 2*pi*f*C)
    return np.abs(Z)

#popt holds the parameter estimates
#pcov is a covariance matrix for the estimates, the diagonals give the variances of the parameter estimates
popt, pcov = curve_fit(parrallel,
                       xdata = f, #independent data
                       ydata = np.abs(Z), #dependent data
                       p0=(22e6,0.1e-12), #initial guess
                       #sigma= ..., #give it errors
                       #absolute_sigma=True #if you tell it that the errors are absolute
                       #it will estimate the errors in the params
                      )


R,C = popt
dR = np.sqrt(pcov[0,0])
dC = np.sqrt(pcov[1,1])

Zmodel = 1/(1/R + 1.0j * 2*pi*f*C)
plot_model_vs_data(f, Z, Zmodel)

print("""
Parameter estimates:
    R = {0:.3g}MΩ +\- {1:.2g}MΩ
    C = {2:.4g}pF +\- {3:.1g}pF
""".format(R*1e-6, dR*1e-6, C*1e12, dC*1e12))


# ### A more complex example of manipulating labels, adding text, and filling under curves

# In[5]:


def plot_deps(axes, xn, xp, Nd, Na, Vb):
    def N(x):
        return np.piecewise(x,
                     [x < xn, (xn <= x)&(x < 0), (0 < x)&(x < xp), xp < x],
                     [0, Nd, -Na, 0])

    def dv(x):
        return np.piecewise(x,
                     [x < xn, (xn <= x)&(x < 0), (0 <= x)&(x < xp), xp <= x],
                     [0, lambda x : -Nd*(x - xn), lambda x : (Na*x + Nd*xn), 0])

    def v(x):
        Vj = Vb + (Na*xp**2/2 + Nd*xn*xp - Nd/2*xn**2)
        return np.piecewise(x,
                     [x < xn, (xn <= x)&(x < 0), (0 <= x)&(x < xp), xp <= x],
                     [Vj,
                      lambda x : Vj + Nd*(x-xn)**2/2,
                      lambda x : Vj - (Na/2*x**2 + Nd*xn*x - Nd/2*xn**2),
                      Vj -(Na*xp**2/2 + Nd*xn*xp - Nd/2*xn**2)])


    x = np.linspace(-3,3,300)

    for ax in axes:
        ax.get_xaxis().set_visible(False)
        ax.spines['top'].set_visible(False)
        ax.spines['right'].set_visible(False)
        ax.spines['bottom'].set_visible(False)

    axes[-1].get_xaxis().set_visible(True)
    axes[-1].spines['bottom'].set_visible(True)

    plt.setp(axes, xticks = [xn, 0, xp], xticklabels = ["$x_n$", 0, "$x_p$"])

    ax = axes[0]
    ax.plot(x, N(x))

    ax.fill_between(x, N(x), where = (x < 0), color = "red", alpha = 0.2, interpolate = True)
    ax.fill_between(x, N(x), where = (x > 0), color = "black", alpha = 0.2, interpolate = True)

    font = {'family': 'serif',
            'color':  'black',
            'weight': 'normal',
            'size': 12,
            }
    ax.text(-2.5, 2.5, r'n doped region', fontdict=font)
    ax.text(0.5, 2.5, r'p doped region', fontdict=font)

    ax.plot([-3,-3],[-Na,Nd],"--k")
    ax.plot([3,3],[-Na,Nd],"--k")

    ax.set(yticks = [Nd, 0, -Na], yticklabels = ["$N_d$", 0, "$-N_a$"],
           ylabel = r"$\rho(x)/e$",
          )

    ax = axes[1]
    ax.plot(x, dv(x), color = "red")
    ax.set(yticks = [Nd*xn, 0],
           yticklabels = ["$N_dx_n$", 0],
          ylabel = "E field E(x) $[Vm^{-1}]$"
          )

    ax = axes[2]
    ax.plot(x, v(x), color = "green")
    ax.set(yticks = [Vb,0],
           yticklabels = ["$V_b$", 0],
           ylabel = "Potential $\Phi$ [V]"
          )

Vb = 3; Nd = 2; Na = 1;
xp = min(3, np.sqrt(Vb*2/Na /(1+Na/Nd))); xn = -xp * Na/Nd
print("xp = {}".format(xp))

f, axes = plt.subplots(3,1, figsize = (5,10), sharex = "col")
plot_deps(axes, xn, xp, Nd, Na, Vb)
f.savefig("depletion.pdf", bbox_inches='tight')


# In[ ]:





# In[ ]:

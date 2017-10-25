from src import settings
from src.form import plots


def kinematic_figures(plotCont, savePrefix='', savePlots=True):
    stdPlots = plots.plots(savePrefix=savePrefix, noLegName=True, savePlots=savePlots)
    # stdPlots.plot_hist(plotCont, settings.SF, (0, 2000), 40, xlabel='Scale factor', weighted=False, mode='stacked', save='sf.pdf')
    # stdPlots.plot_hist(plotCont, 'mjj', (0, 200), 40, xlabel='m$_{dijet}$ [GeV]', mode='stacked', save='mjj.pdf')
    # stdPlots.plot_hist(plotCont, 'mln', (0, 1000), 40, xlabel=r'm$_{l\nu}$ [GeV]', mode='stacked', save='mln.pdf')
    # # stdPlots.plot_hist(plotCont, 'mtot', (0, 200), 40, xlabel='m$_{tot}$ [GeV]', mode='stacked', save='mtot.pdf')
    # # stdPlots.plot_hist(plotCont, 'etot', (0, 200), 40, xlabel='E$_{tot}$ [GeV]', mode='stacked', save='etot.pdf')
    # stdPlots.plot_hist(plotCont, 'beam_m', (0, 1400), 40, xlabel='m$_{beam}$ [GeV]', mode='stacked', save='beam_m.pdf')
    # stdPlots.plot_hist(plotCont, 'beam_e', (0, 1400), 40, xlabel='E$_{beam}$ [GeV]', mode='stacked', save='beam_e.pdf')
    # stdPlots.plot_hist(plotCont, settings.LEP + 'n', (0, 6), 6, xlabel='N$_{lep}$', mode='stacked', save='lep_n.pdf')
    # stdPlots.plot_hist(plotCont, settings.LEP + 'type', (-20, 20), 40, xlabel='Lepton type', mode='stacked', save='lep_type.pdf')
    # stdPlots.plot_hist(plotCont, settings.LEP + 'etot', (0, 1000), 40, xlabel='Lepton E$_{tot}$ [GeV]', mode='stacked', save='lep_etot.pdf')
    # stdPlots.plot_hist(plotCont, settings.LEP + 'pt', (0, 600), 40, xlabel='Lepton p$_{T}$ [GeV]', mode='stacked', save='lep_pt.pdf')
    # stdPlots.plot_hist(plotCont, settings.LEP + 'e', (0, 1000), 40, xlabel='Lepton E [GeV]', mode='stacked', save='lep_e.pdf')
    # stdPlots.plot_hist(plotCont, settings.LEP + 'theta', (0, 3.2), 40, xlabel=r'Lepton $\theta$', mode='stacked', save='lep_theta.pdf')
    # stdPlots.plot_hist(plotCont, settings.LEP + 'phi', (-3.2, 3.2), 40, xlabel='Lepton $\phi$', mode='stacked', save='lep_phi.pdf')
    # stdPlots.plot_hist(plotCont, settings.JET + 'n', (0, 6), 6, xlabel='N$_{jet}$', mode='stacked', save='jet_n.pdf')
    # stdPlots.plot_hist(plotCont, settings.JET + 'etot', (0, 1000), 40, xlabel='Jet E$_{tot}$ [GeV]', mode='stacked', save='jet_etot.pdf')
    # stdPlots.plot_hist(plotCont, settings.JET + 'pt_[\d]', (0, 400), 40, xlabel='Jet p$_{T}$ [GeV]', mode='stacked', save='jet_pt.pdf')
    # stdPlots.plot_hist(plotCont, settings.JET + 'theta_[\d]', (0, 3.2), 40, xlabel=r'Jet $\theta$', mode='stacked', save='jet_theta.pdf')
    # stdPlots.plot_hist(plotCont, settings.JET + 'phi_[\d]', (-3.2, 3.2), 40, xlabel='Jet $\phi$', mode='stacked', save='jet_phi.pdf')
    # stdPlots.plot_hist(plotCont, settings.JET + 'e_[\d]', (0, 400), 40, xlabel='Jet E [GeV]', mode='stacked', save='jet_e.pdf')
    # stdPlots.plot_hist(plotCont, settings.JET + 'pt_0', (0, 400), 40, xlabel='Leading jet p$_{T}$ [GeV]', mode='stacked', save='jet_pt_0.pdf')
    # stdPlots.plot_hist(plotCont, settings.JET + 'pt_1', (0, 400), 40, xlabel='Subleading jet p$_{T}$ [GeV]', mode='stacked', save='jet_pt_1.pdf')
    # stdPlots.plot_hist(plotCont, settings.MISS + 'pt', (0, 600), 40, xlabel='Missing p$_{T}$ [GeV]', mode='stacked', save='miss_pt.pdf')
    # stdPlots.plot_hist(plotCont, settings.MISS + 'e', (0, 1400), 40, xlabel='Missing E [GeV]', mode='stacked', save='miss_e.pdf')
    stdPlots.plot_hist(plotCont, settings.MISS + 'theta', (0, 3.2), 40, xlabel=r'Missing $\theta$', mode='stacked', save='miss_theta.pdf')
    # stdPlots.plot_hist(plotCont, settings.MISS + 'phi', (-3.2, 3.2), 40, xlabel='Missing $\phi$', mode='stacked', save='miss_phi.pdf')
    # stdPlots.plot_hist(plotCont, settings.MC + 'qq_m', (0, 200), 40, xlabel='True m$_{qq}$ [GeV]', mode='stacked', save='mc_qq_m.pdf')
    # stdPlots.plot_hist(plotCont, settings.MC + 'ln_m', (0, 1400), 40, xlabel=r'True m$_{l\nu}$ [GeV]', mode='stacked', save='mc_ln_m.pdf')
    # stdPlots.plot_heat(plotCont[0], settings.JET + 'pt_1', settings.JET + 'pt_0', (0, 400), 40, (0, 400), 40, xlabel='Subleading jet p$_{T}$ [GeV]', ylabel='Leading jet p$_{T}$ [GeV]', save='jet_pt_lead_vs_sublead.pdf')
    # stdPlots.plot_heat(plotCont[0], settings.MC + 'qq_m', settings.MC + 'ln_m', (0, 200), 40, (0, 1400), 40, xlabel='True m$_{qq}$ [GeV]', ylabel=r'True m$_{l\nu}$ [GeV]', save='mc_qq_m_vs_mc_qq_ln.pdf')


def correlation_figures(plotCont, savePrefix='', savePlots=True):
    stdPlots = plots.plots(savePrefix=savePrefix, noLegName=True, savePlots=savePlots)
    filterCont = plotCont.filter(items=[settings.JET + 'pt_0', settings.JET + 'pt_1'])
    stdPlots.plot_corr(filterCont, save='corr_filtered.pdf')
    stdPlots.plot_corr(plotCont.filter(regex='jet'), save='corr_filtered_regex.pdf')
    stdPlots.plot_corr(plotCont, save='corr.pdf')

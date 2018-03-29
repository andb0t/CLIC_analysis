from src import settings
from src.form import plots


def kinematic_figures(plotCont, savePrefix='', savePlots=True, mode='stacked', normed=True):
    stdPlots = plots.plots(savePrefix=savePrefix, noLegName=True, savePlots=savePlots)
    stdPlots.plot_hist(plotCont, 'mjj', (0, 200), 40, xlabel='m$_{dijet}$ [GeV]', mode=mode, normed=normed, save='mjj.pdf')
    stdPlots.plot_hist(plotCont, 'mln', (0, 2400), 40, xlabel=r'm$_{l\nu}$ [GeV]', mode=mode, normed=normed, save='mln.pdf')
    stdPlots.plot_hist(plotCont, '^beam_e', (0, 1400), 40, xlabel='Reco E$_{beam}$ [GeV]', mode=mode, normed=normed, save='beam_e.pdf')
    stdPlots.plot_hist(plotCont, '^qq_m', (0, 200), 40, xlabel='m$_{qq}$ [GeV]', mode=mode, normed=normed, save='qq_m.pdf')
    stdPlots.plot_hist(plotCont, '^ln_m', (0, 1400), 40, xlabel=r'm$_{l\nu}$ [GeV]s', mode=mode, normed=normed, save='ln_m.pdf')
    stdPlots.plot_hist(plotCont, settings.SF, (0, 2000), 40, xlabel='Scale factor', weighted=False, mode=mode, normed=normed, save='sf.pdf')
    stdPlots.plot_hist(plotCont, settings.LEP + 'n', (0, 6), 6, xlabel='N$_{lep}$', mode=mode, normed=normed, save='lep_n.pdf')
    stdPlots.plot_hist(plotCont, settings.LEP + 'type', (-20, 20), 40, xlabel='Lepton type', mode=mode, normed=normed, save='lep_type.pdf')
    stdPlots.plot_hist(plotCont, settings.LEP + 'etot', (0, 1000), 40, xlabel='Lepton E$_{tot}$ [GeV]', mode=mode, normed=normed, save='lep_etot.pdf')
    stdPlots.plot_hist(plotCont, settings.LEP + 'pt_0', (0, 600), 40, xlabel='Leading lepton p$_{T}$ [GeV]', mode=mode, normed=normed, save='lep_pt_0.pdf')
    stdPlots.plot_hist(plotCont, settings.LEP + 'pt_1', (0, 20), 40, xlabel='Subleading lepton p$_{T}$ [GeV]', mode=mode, normed=normed, save='lep_pt_1.pdf')
    stdPlots.plot_hist(plotCont, settings.LEP + 'pt', (0, 600), 40, xlabel='Lepton p$_{T}$ [GeV]', mode=mode, normed=normed, save='lep_pt.pdf')
    stdPlots.plot_hist(plotCont, settings.LEP + 'e_[\d]', (0, 1000), 40, xlabel='Lepton E [GeV]', mode=mode, normed=normed, save='lep_e.pdf')
    stdPlots.plot_hist(plotCont, settings.LEP + 'theta', (0, 3.2), 40, xlabel=r'Lepton $\theta$', mode=mode, normed=normed, save='lep_theta.pdf')
    stdPlots.plot_hist(plotCont, settings.LEP + 'phi', (-3.2, 3.2), 40, xlabel='Lepton $\phi$', mode=mode, normed=normed, save='lep_phi.pdf')
    stdPlots.plot_hist(plotCont, settings.LEP + 'charge_0', (-2.5, 2.5), 5, xlabel='Lepton charge', mode=mode, normed=normed, save='lep_charge.pdf')
    stdPlots.plot_hist(plotCont, settings.EL + 'pt', (0, 600), 40, xlabel='Electron p$_{T}$ [GeV]', mode=mode, normed=normed, save='el_pt.pdf')
    stdPlots.plot_hist(plotCont, settings.EL + 'e_[\d]', (0, 1000), 40, xlabel='Electron E [GeV]', mode=mode, normed=normed, save='el_e.pdf')
    stdPlots.plot_hist(plotCont, settings.EL + 'theta', (0, 3.2), 40, xlabel=r'Electron $\theta$', mode=mode, normed=normed, save='el_theta.pdf')
    stdPlots.plot_hist(plotCont, settings.EL + 'phi', (-3.2, 3.2), 40, xlabel='Electron $\phi$', mode=mode, normed=normed, save='el_phi.pdf')
    stdPlots.plot_hist(plotCont, settings.EL + 'charge_0', (-2.5, 2.5), 5, xlabel='Electron charge', mode=mode, normed=normed, save='el_charge.pdf')
    stdPlots.plot_hist(plotCont, settings.MU + 'pt', (0, 600), 40, xlabel='Muon p$_{T}$ [GeV]', mode=mode, normed=normed, save='mu_pt.pdf')
    stdPlots.plot_hist(plotCont, settings.MU + 'e_[\d]', (0, 1000), 40, xlabel='Muon E [GeV]', mode=mode, normed=normed, save='mu_e.pdf')
    stdPlots.plot_hist(plotCont, settings.MU + 'theta', (0, 3.2), 40, xlabel=r'Muon $\theta$', mode=mode, normed=normed, save='mu_theta.pdf')
    stdPlots.plot_hist(plotCont, settings.MU + 'phi', (-3.2, 3.2), 40, xlabel='Muon $\phi$', mode=mode, normed=normed, save='mu_phi.pdf')
    stdPlots.plot_hist(plotCont, settings.MU + 'charge_0', (-2.5, 2.5), 5, xlabel='Muon charge', mode=mode, normed=normed, save='mu_charge.pdf')
    stdPlots.plot_hist(plotCont, settings.JET + 'n', (0, 6), 6, xlabel='N$_{jet}$', mode=mode, normed=normed, save='jet_n.pdf')
    stdPlots.plot_hist(plotCont, settings.JET + 'etot', (0, 1000), 40, xlabel='Jet E$_{tot}$ [GeV]', mode=mode, normed=normed, save='jet_etot.pdf')
    stdPlots.plot_hist(plotCont, settings.JET + 'pt_[\d]', (0, 400), 40, xlabel='Jet p$_{T}$ [GeV]', mode=mode, normed=normed, save='jet_pt.pdf')
    stdPlots.plot_hist(plotCont, settings.JET + 'theta_[\d]', (0, 3.2), 40, xlabel=r'Jet $\theta$', mode=mode, normed=normed, save='jet_theta.pdf')
    stdPlots.plot_hist(plotCont, settings.JET + 'phi_[\d]', (-3.2, 3.2), 40, xlabel='Jet $\phi$', mode=mode, normed=normed, save='jet_phi.pdf')
    stdPlots.plot_hist(plotCont, settings.JET + 'e_[\d]', (0, 400), 40, xlabel='Jet E [GeV]', mode=mode, normed=normed, save='jet_e.pdf')
    stdPlots.plot_hist(plotCont, settings.JET + 'pt_0', (0, 400), 40, xlabel='Leading jet p$_{T}$ [GeV]', mode=mode, normed=normed, save='jet_pt_0.pdf')
    stdPlots.plot_hist(plotCont, settings.JET + 'pt_1', (0, 400), 40, xlabel='Subleading jet p$_{T}$ [GeV]', mode=mode, normed=normed, save='jet_pt_1.pdf')
    stdPlots.plot_hist(plotCont, settings.MISS + 'pt', (0, 400), 40, xlabel='Missing p$_{T}$ [GeV]', mode=mode, normed=normed, save='miss_pt.pdf')
    stdPlots.plot_hist(plotCont, settings.MISS + 'e', (0, 1400), 40, xlabel='Missing E [GeV]', mode=mode, normed=normed, save='miss_e.pdf')
    stdPlots.plot_hist(plotCont, settings.MISS + 'theta', (0, 3.2), 40, xlabel=r'Missing $\theta$', mode=mode, normed=normed, save='miss_theta.pdf')
    stdPlots.plot_hist(plotCont, settings.MISS + 'phi', (-3.2, 3.2), 40, xlabel='Missing $\phi$', mode=mode, normed=normed, save='miss_phi.pdf')
    stdPlots.plot_hist(plotCont, 'mc_beam_e', (0, 1400), 40, xlabel='True E$_{beam}$ [GeV]', mode=mode, normed=normed, save='mc_beam_e.pdf')
    stdPlots.plot_hist(plotCont, 'mc_qq_m', (0, 200), 40, xlabel='True m$_{qq}$ [GeV]', mode=mode, normed=normed, save='mc_qq_m.pdf')
    stdPlots.plot_hist(plotCont, 'mc_ln_m', (0, 1400), 40, xlabel=r'True m$_{l\nu}$ [GeV]', mode=mode, normed=normed, save='mc_ln_m.pdf')
    stdPlots.plot_heat(plotCont[0], settings.JET + 'pt_1', settings.JET + 'pt_0', (0, 400), 40, (0, 400), 40, xlabel='Subleading jet p$_{T}$ [GeV]', ylabel='Leading jet p$_{T}$ [GeV]', save='jet_pt_lead_vs_sublead.pdf')
    stdPlots.plot_heat(plotCont[0], 'mc_qq_m', 'mc_ln_m', (0, 200), 40, (0, 1400), 40, xlabel='True m$_{qq}$ [GeV]', ylabel=r'True m$_{l\nu}$ [GeV]', save='mc_qq_m_vs_mc_qq_ln.pdf')
    stdPlots.plot_hist(plotCont, 'theta_W', (0, 3.2), 40, xlabel=r'$\theta_{W}$', mode=mode, normed=normed, save='theta_W.pdf')
    stdPlots.plot_hist(plotCont, 'theta_L', (0, 3.2), 40, xlabel=r'$\theta_{L}$', mode=mode, normed=normed, save='theta_L.pdf')
    stdPlots.plot_hist(plotCont, 'theta_H', (0, 3.2), 40, xlabel=r'$\theta_{H}$', mode=mode, normed=normed, save='theta_H.pdf')
    stdPlots.plot_hist(plotCont, 'phi_L', (-3.2, 3.2), 40, xlabel=r'$\phi_{L}$', mode=mode, normed=normed, save='phi_L.pdf')
    stdPlots.plot_hist(plotCont, 'phi_H', (-3.2, 3.2), 40, xlabel=r'$\phi_{H}$', mode=mode, normed=normed, save='phi_H.pdf')


def correlation_figures(plotCont, savePrefix='', savePlots=True):
    stdPlots = plots.plots(savePrefix=savePrefix, noLegName=True, savePlots=savePlots)
    filterCont = plotCont.filter(items=[settings.JET + 'pt_0', settings.JET + 'pt_1'])
    stdPlots.plot_corr(filterCont, save='corr_filtered.pdf')
    stdPlots.plot_corr(plotCont.filter(regex='jet'), save='corr_filtered_regex.pdf')
    stdPlots.plot_corr(plotCont, save='corr.pdf')


def revent_by_event_figures(plotCont, savePrefix='', savePlots=True):
    stdPlots = plots.plots(savePrefix=savePrefix, noLegName=True, savePlots=savePlots)
    stdPlots.plot_raw(plotCont[0], 'mjj', ylabel='m$_{dijet}$ [GeV]', save='events_mjj.pdf')
    stdPlots.plot_raw(plotCont[0], 'mln', ylabel=r'm$_{l\nu}$ [GeV]', save='events_mln.pdf')
    stdPlots.plot_scatter(plotCont, 'mjj', 'mln', (0, 200), (0, 200), xlabel='m$_{dijet}$ [GeV]', ylabel=r'm$_{l\nu}$ [GeV]', save='scatter_mjj_mln.pdf')
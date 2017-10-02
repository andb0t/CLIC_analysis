#include "/afs/cern.ch/user/a/amaier/Utils/cpp/RootUtils.C"
#include "/afs/cern.ch/user/a/amaier/Utils/cpp/HistoComparator.cxx"

#include "TLorentzVector.h"
#include "TGraphErrors.h"
#include "TDatabasePDG.h"

const int MAX_NJET=20;
const int MAX_NFILE=20;
const int NJETS=2;
const double MeV=1000;

TDatabasePDG *pdg(0);


void JER_analysis(string savetag="",string plotformat=".pdf", int verbose=1){
	string savestring="fig/JER_analysis_"+savetag+"_";
	string pdg_table="/cvmfs/clicdp.cern.ch/software/ROOT/6.08.00/x86_64-slc6-gcc62-opt/etc/pdg_table.txt";

	std::vector<double> energies,energies_input;
	std::vector<string> inputfiles,inputfiles_input;
	inputfiles_input.push_back("/afs/cern.ch/work/a/amaier/CLIC/rootfiles/ana_reco_output_91.root"); energies_input.push_back(91);
	inputfiles_input.push_back("/afs/cern.ch/work/a/amaier/CLIC/rootfiles/ana_reco_output_100.root"); energies_input.push_back(100);
	inputfiles_input.push_back("/afs/cern.ch/work/a/amaier/CLIC/rootfiles/ana_reco_output_200.root"); energies_input.push_back(200);
	inputfiles_input.push_back("/afs/cern.ch/work/a/amaier/CLIC/rootfiles/ana_reco_output_380.root"); energies_input.push_back(380);
	inputfiles_input.push_back("/afs/cern.ch/work/a/amaier/CLIC/rootfiles/ana_reco_output_500.root"); energies_input.push_back(500);
	inputfiles_input.push_back("/afs/cern.ch/work/a/amaier/CLIC/rootfiles/ana_reco_output_750.root"); energies_input.push_back(750);
	inputfiles_input.push_back("/afs/cern.ch/work/a/amaier/CLIC/rootfiles/ana_reco_output_1000.root"); energies_input.push_back(1000);
	inputfiles_input.push_back("/afs/cern.ch/work/a/amaier/CLIC/rootfiles/ana_reco_output_1500.root"); energies_input.push_back(1500);
	inputfiles_input.push_back("/afs/cern.ch/work/a/amaier/CLIC/rootfiles/ana_reco_output_2000.root"); energies_input.push_back(2000);
	inputfiles_input.push_back("/afs/cern.ch/work/a/amaier/CLIC/rootfiles/ana_reco_output_3000.root"); energies_input.push_back(3000);

	//order them ascending by energy
	energies=energies_input;
	inputfiles=inputfiles_input;
	std::sort (energies.begin(), energies.end());
	for (unsigned int i = 0; i < energies.size(); ++i)
	{
		int idx=std::find (energies_input.begin (),energies_input.end (), energies.at(i)) - energies_input.begin();
		inputfiles.at(i)=inputfiles_input.at(idx);

	}

	TCanvas* c_JERJES = new TCanvas("c_JERJES","c_JERJES",0,0,1200,1000);
	TCanvas* c_dijet = new TCanvas("c_dijet","c_dijet",0,0,1200,500);
	TCanvas* c_etot = new TCanvas("c_etot","c_etot",0,0,1200,1000);
	TCanvas* c_etot_residual = new TCanvas("c_etot_residual","c_etot_residual",0,0,1200,1000);
	TCanvas* c_charge_frac = new TCanvas("c_charge_frac","c_charge_frac",0,0,1200,1000);

	TGraphErrors* gr_pfo_JER = new TGraphErrors(0);
	TGraphErrors* gr_jet_JER = new TGraphErrors(0);
	TGraphErrors* gr_pfo_JES = new TGraphErrors(0);
	TGraphErrors* gr_jet_JES = new TGraphErrors(0);
	TGraphErrors* gr_E_charge_frac = new TGraphErrors(0);
	TGraphErrors* gr_pion_frac = new TGraphErrors(0);

	TH1F* h_dijet_m[MAX_NFILE];
	TH1F* h_dijet_balance[MAX_NFILE];
	TH1F* h_mc_etot[MAX_NFILE];
	TH1F* h_mc_etot_residual[MAX_NFILE];
	TH1F* h_pfo_etot[MAX_NFILE];
	TH1F* h_pfo_etot_residual[MAX_NFILE];
	TH1F* h_jet_etot[MAX_NFILE];
	TH1F* h_jet_etot_residual[MAX_NFILE];
	TH1F* h_jet_charge_frac[MAX_NFILE];
	TH1F* h_jet_pion_frac[MAX_NFILE];
	TH2F* h_JES_charge_frac_corr=new TH2F("h_JES_charge_frac_corr","h_JES_charge_frac_corr;JES;jet charge fration;Entries",50,0,1,50,0,1);
	TH2F* h_JES_pion_frac_corr=new TH2F("h_JES_pion_frac_corr","h_JES_pion_frac_corr;JES;jet #pi^{#pm} fration;Entries",50,0,1,50,0,1);


	TFile* f_in[MAX_NFILE];
	TTree* t_in[MAX_NFILE];



	pdg = new TDatabasePDG();
	pdg->ReadPDGTable(pdg_table.c_str());


	for (unsigned int i_file = 0; i_file < inputfiles.size(); ++i_file)
	{
		f_in[i_file]=(TFile*) TFile::Open(inputfiles.at(i_file).c_str());
		t_in[i_file]=(TTree*) f_in[i_file]->Get("tracktree");

		printf("Running on sample for %.0f GeV with %d entries...\n",energies.at(i_file),int(t_in[i_file]->GetEntries()) );

		h_dijet_m[i_file]=new TH1F(Form("h_dijet_m[%d]",i_file),Form("E_{true}=%.0f GeV;m_{dijet} [GeV];Number of events",energies.at(i_file)),100,0,energies.back()*1.5);
		h_dijet_balance[i_file]=new TH1F(Form("h_dijet_balance[%d]",i_file),Form("E_{true}=%.0f GeV;|p_{dijet}^{res}| [GeV];Number of events",energies.at(i_file)),50,-200,200);
		h_mc_etot[i_file]=new TH1F(Form("h_mc_etot[%d]",i_file),Form("E_{true}=%.0f GeV;E_{tot}^{MC} [GeV];Number of events",energies.at(i_file)),500,0,energies.back()*1.1);
		h_mc_etot_residual[i_file]=new TH1F(Form("h_mc_etot_residual[%d]",i_file),Form("E_{true}=%.0f GeV;E_{tot}^{MC} - E_{true} [MeV];Number of events",energies.at(i_file)),50,-1,1);
		h_pfo_etot[i_file]=new TH1F(Form("h_pfo_etot[%d]",i_file),Form("E_{true}=%.0f GeV;E_{tot}^{PFO} [GeV];Number of events",energies.at(i_file)),500,0,energies.back()*1.5);
		h_pfo_etot_residual[i_file]=new TH1F(Form("h_pfo_etot_residual[%d]",i_file),Form("E_{true}=%.0f GeV;E_{tot}^{PFO} - E_{true} [GeV];Number of events",energies.at(i_file)),250,-2000,2000);
		h_jet_etot[i_file]=new TH1F(Form("h_jet_etot[%d]",i_file),Form("E_{true}=%.0f GeV;E_{tot}^{jet} [GeV];Number of events",energies.at(i_file)),500,0,energies.back()*1.5);
		h_jet_etot_residual[i_file]=new TH1F(Form("h_jet_etot_residual[%d]",i_file),Form("E_{true}=%.0f GeV;E_{tot}^{jet} - E_{true} [GeV];Number of events",energies.at(i_file)),250,-2000,2000);
		h_jet_charge_frac[i_file]=new TH1F(Form("h_jet_charge_frac[%d]",i_file),Form("E_{true}=%.0f GeV;Charge fraction;Number of events",energies.at(i_file)),50,0,1);
		h_jet_pion_frac[i_file]=new TH1F(Form("h_jet_pion_frac[%d]",i_file),Form("E_{true}=%.0f GeV;Pion fraction;Number of events",energies.at(i_file)),50,0,1);


		int jet_n;
		double jet_etot,pfo_etot,mc_etot;
		std::vector<double> *jet_pt(0);
		std::vector<double> *jet_theta(0);
		std::vector<double> *jet_phi(0);
		std::vector<double> *jet_e(0);
		std::vector<std::vector<double>> *jet_pfo_relation(0);
		std::vector<int> *pfo_pdg(0);
		t_in[i_file]->SetBranchAddress("jet_n", &jet_n);
		t_in[i_file]->SetBranchAddress("jet_etot", &jet_etot);
		t_in[i_file]->SetBranchAddress("jet_pt", &jet_pt);
		t_in[i_file]->SetBranchAddress("jet_theta", &jet_theta);
		t_in[i_file]->SetBranchAddress("jet_phi", &jet_phi);
		t_in[i_file]->SetBranchAddress("jet_e", &jet_e);
		t_in[i_file]->SetBranchAddress("jet_pfo_relation", &jet_pfo_relation);
		t_in[i_file]->SetBranchAddress("pfo_etot", &pfo_etot);
		t_in[i_file]->SetBranchAddress("pfo_pdg", &pfo_pdg);
		t_in[i_file]->SetBranchAddress("mc_etot", &mc_etot);

		TLorentzVector jet_vec[MAX_NJET];

		for (int ievt = 0; ievt < t_in[i_file]->GetEntries(); ++ievt)
		{
			t_in[i_file]->GetEntry(ievt);

			//jets
			double jet_charge_frac[MAX_NJET]={0};
			double jet_pion_frac[MAX_NJET]={0};
			// printf("Event %d with jet_etot %f\n",ievt,jet_etot );
			for (int ijet = 0; ijet < jet_n; ++ijet)
			{
				double eta=-TMath::Log(TMath::Tan(jet_theta->at(ijet)/2));
				jet_vec[ijet].SetPtEtaPhiE(jet_pt->at(ijet),eta,jet_phi->at(ijet),jet_e->at(ijet));
				for (unsigned int ijp = 0; ijp < jet_pfo_relation->at(ijet).size(); ++ijp)
				{
					int pfo_idx=jet_pfo_relation->at(ijet).at(ijp);
					jet_charge_frac[ijet]*=ijp;
					jet_pion_frac[ijet]*=ijp;
					int this_pdg=pfo_pdg->at(pfo_idx);
					if (!pdg->GetParticle(this_pdg)) {if (verbose>1) printf("Warning! PDG %d not contained in $ROOTSYS/etc/pdg_table.txt!\n",this_pdg);}
					else {
						if (pdg->GetParticle(this_pdg)->Charge()) ++jet_charge_frac[ijet];
						if (fabs(this_pdg) == 211) ++jet_pion_frac[ijet];
					}
					jet_charge_frac[ijet]/=ijp+1;
					jet_pion_frac[ijet]/=ijp+1;
				}
				// printf("Event %d jet %d charge fraction %.2f\n",ievt,ijet,jet_charge_frac[ijet] );
			}

			double dijet_m=(jet_vec[0]+jet_vec[1]).M();
			double dijet_balance=(jet_vec[0].P()-jet_vec[1].P());
			double average_charge_frac=(jet_charge_frac[0]+jet_charge_frac[1])/2;
			double average_pion_frac=(jet_pion_frac[0]+jet_pion_frac[1])/2;

			//frac
			h_jet_charge_frac[i_file]->Fill(average_charge_frac);
			h_jet_pion_frac[i_file]->Fill((jet_pion_frac[0]+jet_pion_frac[1])/2);

			//dijet
			h_dijet_m[i_file]->Fill(dijet_m);
			h_dijet_balance[i_file]->Fill(dijet_balance);

			//mc
			h_mc_etot[i_file]->Fill(mc_etot);
			h_mc_etot_residual[i_file]->Fill((mc_etot-energies.at(i_file))*MeV);

			//jets
			h_jet_etot[i_file]->Fill(jet_etot);
			h_jet_etot_residual[i_file]->Fill(jet_etot-energies.at(i_file));

			//pfos
			h_pfo_etot[i_file]->Fill(pfo_etot);
			h_pfo_etot_residual[i_file]->Fill(pfo_etot-energies.at(i_file));

			//corr
			h_JES_charge_frac_corr->Fill(average_charge_frac,jet_etot-energies.at(i_file));
			h_JES_pion_frac_corr->Fill(average_pion_frac,jet_etot-energies.at(i_file));
		}

		//analyze histos
		double frac=0.9; //quantity in smallest region containing frac of all events

		double rms_pfo_etot=GetMinHistoRegion(h_pfo_etot[i_file],frac,"RMS")[0];
		double mean_pfo_etot=GetMinHistoRegion(h_pfo_etot[i_file],frac,"Mean")[0];
		double JER_pfo_etot=1/sqrt(double(NJETS)) * rms_pfo_etot/mean_pfo_etot;
		double JES_pfo_etot=GetMinHistoRegion(h_pfo_etot_residual[i_file],frac,"Mean")[0]/energies.at(i_file);

		double rms_jet_etot=GetMinHistoRegion(h_jet_etot[i_file],frac,"RMS")[0];
		double mean_jet_etot=GetMinHistoRegion(h_jet_etot[i_file],frac,"Mean")[0];
		double JER_jet_etot=1/sqrt(double(NJETS)) * rms_jet_etot/mean_jet_etot;
		double JES_jet_etot=GetMinHistoRegion(h_jet_etot_residual[i_file],frac,"Mean")[0]/energies.at(i_file);

		//fill graphs
		gr_pfo_JER->SetPoint(i_file,energies.at(i_file),100.*JER_pfo_etot);
		gr_jet_JER->SetPoint(i_file,energies.at(i_file),100.*JER_jet_etot);
		gr_pfo_JES->SetPoint(i_file,energies.at(i_file),100.*JES_pfo_etot);
		gr_jet_JES->SetPoint(i_file,energies.at(i_file),100.*JES_jet_etot);
		gr_E_charge_frac->SetPoint(i_file,energies.at(i_file),h_jet_charge_frac[i_file]->GetMean());
		gr_pion_frac->SetPoint(i_file,energies.at(i_file),h_jet_pion_frac[i_file]->GetMean());

		printf("File %d E = %.0f:\n",i_file,energies.at(i_file));
		printf("The JER based on PFOs / jets is %f / %f\n",JER_pfo_etot,JER_jet_etot );
		printf("The JES based on PFOs / jets is %f / %f\n",JES_pfo_etot,JES_jet_etot );
	}


	HistoComparator hc;
	hc.setSeparatecanvas(false);
	hc.setStandardlegendtextsize();
	hc.setNormalize(false);
	// hc.setVerbose(2);


	c_JERJES->Divide(2,2);
	c_JERJES->cd(1);
	if (savetag.size()) hc.setSavename(savestring+"JER"+plotformat);
	hc.setXtitle("E_{truth} [GeV]");
	hc.setYtitle("JER [%]");
	hc.setLegendheader("Determined from");
	hc.addGraph(gr_pfo_JER,"PFOs");
	hc.addGraph(gr_jet_JER,"jets");
	hc.Draw();
	hc.clearHists();

	c_JERJES->cd(2);
	if (savetag.size()) hc.setSavename(savestring+"JES"+plotformat);
	hc.setXtitle("E_{truth} [GeV]");
	hc.setYtitle("JES [%]");
	hc.setLegendheader("Determined from");
	hc.addGraph(gr_pfo_JES,"PFOs");
	hc.addGraph(gr_jet_JES,"jets");
	hc.Draw();
	hc.clearHists();


	c_dijet->Divide(2,1);
	c_dijet->cd(1);
	if (savetag.size()) hc.setSavename(savestring+"dijet_m"+plotformat);
	for (unsigned int i_file = 0; i_file < inputfiles.size(); ++i_file) hc.addHist(h_dijet_m[i_file]);
	hc.Draw();
	hc.clearHists();
	c_dijet->cd(2);
	if (savetag.size()) hc.setSavename(savestring+"dijet_balance"+plotformat);
	for (unsigned int i_file = 0; i_file < inputfiles.size(); ++i_file) hc.addHist(h_dijet_balance[i_file]);
	hc.Draw();
	hc.clearHists();

	c_etot->Divide(2,2);
	c_etot->cd(1);
	if (savetag.size()) hc.setSavename(savestring+"mc_etot"+plotformat);
	for (unsigned int i_file = 0; i_file < inputfiles.size(); ++i_file) hc.addHist(h_mc_etot[i_file]);
	hc.Draw();
	hc.clearHists();
	c_etot->cd(2);
	if (savetag.size()) hc.setSavename(savestring+"pfo_etot"+plotformat);
	for (unsigned int i_file = 0; i_file < inputfiles.size(); ++i_file) hc.addHist(h_pfo_etot[i_file]);
	hc.Draw();
	hc.clearHists();
	c_etot->cd(3);
	if (savetag.size()) hc.setSavename(savestring+"jet_etot"+plotformat);
	for (unsigned int i_file = 0; i_file < inputfiles.size(); ++i_file) hc.addHist(h_jet_etot[i_file]);
	hc.Draw();
	hc.clearHists();

	c_etot_residual->Divide(2,2);
	c_etot_residual->cd(1);
	if (savetag.size()) hc.setSavename(savestring+"mc_etot_residual"+plotformat);
	for (unsigned int i_file = 0; i_file < inputfiles.size(); ++i_file) hc.addHist(h_mc_etot_residual[i_file]);
	hc.Draw();
	hc.clearHists();
	c_etot_residual->cd(2);
	if (savetag.size()) hc.setSavename(savestring+"pfo_etot_residual"+plotformat);
	for (unsigned int i_file = 0; i_file < inputfiles.size(); ++i_file) hc.addHist(h_pfo_etot_residual[i_file]);
	hc.Draw();
	hc.clearHists();
	c_etot_residual->cd(3);
	if (savetag.size()) hc.setSavename(savestring+"jet_etot_residual"+plotformat);
	for (unsigned int i_file = 0; i_file < inputfiles.size(); ++i_file) hc.addHist(h_jet_etot_residual[i_file]);
	hc.Draw();
	hc.clearHists();

	c_charge_frac->Divide(2,3);
	c_charge_frac->cd(1);
	if (savetag.size()) hc.setSavename(savestring+"jet_charge_frac"+plotformat);
	for (unsigned int i_file = 0; i_file < inputfiles.size(); ++i_file) hc.addHist(h_jet_charge_frac[i_file]);
	hc.Draw();
	hc.clearHists();
	c_charge_frac->cd(2);
	if (savetag.size()) hc.setSavename(savestring+"E_charge_frac"+plotformat);
	hc.setXtitle("E_{truth} [GeV]");
	hc.setYtitle("");
	hc.addGraph(gr_E_charge_frac,"Charge fraction");
	hc.addGraph(gr_pion_frac,"#pi^{#pm} fraction");
	hc.Draw();
	hc.clearHists();
	c_charge_frac->cd(3);
	DrawTH2F(h_JES_charge_frac_corr);
	c_charge_frac->cd(4);
	DrawTH2F(h_JES_pion_frac_corr);

}
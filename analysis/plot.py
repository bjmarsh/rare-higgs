# Handling ROOT files
import os, sys
import numpy as np
import uproot
import pandas as pd
# Plotting
import matplotlib
from matplottery import Hist1D,Hist2D, plot_stack
import matplotlib.pyplot as plt
# Custom
from data import GetData
from config import config

class Plots():
    def __init__(self, config, data, outDir="plots/", lumi=137.0, verbose=False):
        # Args
        self.signal = config["signal"] 
        self.analysis = config["analysis"]
        self.data = data
        # Kwargs
        self.outDir = outDir
        self.lumi = lumi
        self.verbose = verbose
        # Internal
        self.colors = {}
        # Init functions
        self._initColors()
        self._initOutput()

    def _initColors(self):
        """ Initialize available colors """
        rgbs=[[0.4, 0.4, 0.4], [1.0, 0.4, 1.0], [0.4, 0.0, 0.8],
              [0.4, 0.6, 1.0], [0.4, 0.8, 0.4], [0.9, 0.9, 0.9],
              [0.4, 0.4, 0.6], [0.0, 0.4, 0.0]
             ]
        for i, name in enumerate(self.data.keys()):
            color = [1.0,0.0,0.0] if name == self.signal else rgbs[i]
            self.colors[name] = color

        return

    def _initOutput(self):
        """ Initialize output directory """
        out = self.outDir
        if not os.path.isdir(out): os.mkdir(out)
        if out[-1] != "/":
            self.outDir += "/"

        return

    def Plot(self):
        """ Make validation plots """
        # Mass plots
        self.Stacked("recoPhi_mass", 200,0,2, xLabel=r"$m_{K^{+}K^{-}}$ (GeV)", logY=True)
        self.Stacked("recoRho_mass", 200,0,2, xLabel=r"$m_{\pi^{+}\pi^{-}}$ (GeV)", logY=True)
        self.Stacked("recoHiggs_mass", 200,0,200, xLabel=r"$m_{H}$ (GeV)", logY=True)
        # dR plots
        self.Stacked("recoKpKm_dR", 100,0,0.1, xLabel=r"$dR(K^{+}, K^{-})$", logY=True)
        self.Stacked("recoPipPim_dR", 100,0,0.1, xLabel=r"$dR(\pi^{+}, \pi^{-})$", logY=True)
        # iso plots
        self.Stacked("recoPhi_iso", 100,0,5, xLabel=r"$iso_{\phi}$", logY=True)
        self.Stacked("recoRho_iso", 100,0,5, xLabel=r"$iso_{\rho}$", logY=True)
        # pt, eta, phi plots
        self.Stacked("recoGamma_pt", 100,0,200, xLabel=r"$p_{T,\gamma}$", logY=True)
        self.Stacked("recoKp_pt", 100,0,200, xLabel=r"$p_{T,K^{+}}$", logY=True)
        self.Stacked("recoKm_pt", 100,0,200, xLabel=r"$p_{T,K{-}}$", logY=True)
        self.Stacked("recoPip_pt", 100,0,200, xLabel=r"$p_{T,\pi^{+}}$", logY=True)
        self.Stacked("recoPim_pt", 100,0,200, xLabel=r"$p_{T,\pi^{-}}$", logY=True)

        return

    def Stacked(self, col, nBins, xMin, xMax, xLabel="", logY=False, sigWeight=0.1):
        """ Make plot of stacked 1D histograms """
        bgsPlots = []
        sigPlots = []
        for name, df in self.data.iteritems():
            # Drop rho or phi events
            if self.analysis == "phi":
                df = df.drop(df.query("genHiggsMeson_id == 113").index)
            elif self.analysis == "rho":
                df = df.drop(df.query("genKp_pt > 0").index)
            # Get data
            x = df.loc[(df[col] != -999), col].to_numpy()
            # Get scale1fb
            weights = df.loc[(df.scale1fb != -999) & (df[col] != -999), "scale1fb"].to_numpy()
            # Manually set signal scalef1b
            if name == self.signal and np.array_equal(weights, np.ones_like(weights)):
                weights = 0.0*weights+sigWeight
            # Plot data
            thisPlot = Hist1D(x,label=name.split("_")[0], bins=np.linspace(xMin,xMax,nBins), weights=weights, color=self.colors[name])
            # Add plot to stack
            if name == self.signal: sigPlots.append(thisPlot)
            else: bgsPlots.append(thisPlot)
        # Plot stack
        plot_stack(bgs=bgsPlots, sigs=sigPlots, do_log=logY,
                   xlabel=xLabel, ylabel="Events",cms_type="Simulation",
                   lumi=str(self.lumi), filename=(self.outDir+col+".pdf"));
        
        if self.verbose: print("Created {} stack plot.".format(col))

        return

if __name__ == "__main__":
    print("Loading dataframes...")
    dataframes = GetData("outputs", verbose=True)
    plots = Plots(config, dataframes, verbose=True)
    plots.Plot()
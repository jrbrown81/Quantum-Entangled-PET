# Quantum-Entangled-PET
Photon quantum entanglement in the MeV regime and its application in PET imaging
Data and source code associated with the above publication.

Contents:
figs_2_and_10
   dPhiComparison.C                    - ROOT code to produce figures 2 and 10
   newData.root                        - ROOT file containing expereimental data
   histos_40mill_tangle_uncoll.root    - ROOT file containing entangled simulation data
   histos_37mill_pol_uncoll.root       - ROOT file containing polarised simulation data
   histos_33mill_unpol_uncoll.root     - ROOT file containing unpolarised simulation data
fig_4
   scatSimComp_38.C                    - ROOT code to produce figure 4
   nylonLab.root                       - ROOT file containing nylon scatter data
   nylonSim.root                       - ROOT file containing nylon scatter simulation
   b2bExpt.root                        - ROOT file containing back-to-back experiemtal data
   b2bSim.root                         - ROOT file containing back-to_back simulation data
figs_6_and_12
   scatExtract.C                       - ROOT code to produce figures 6 and 12
   bottom_scatter_noPhiWindow.csv      - csv file with a priori scatter profile for bottom capilaries
   bottom_true_80-100.csv              - csv file with a priori true profile for bottom capilaries
   bottom_trueScatter_0-20.csv         - csv file with true and scatter simulated profile for phi 0-20 deg
   bottom_trueScatter_80-100.csv       - csv file with true and scatter simulated profile for phi 80-100 deg
   middle_scatter_noPhiWindow.csv      - csv file with a priori scatter profile for bottom capilaries
   middle_true_80-100.csv              - csv file with a priori true profile for bottom capilaries
   middle_trueScatter_0-20.csv         - csv file with true and scatter simulated profile for phi 0-20 deg
   middle_trueScatter_80-100.csv       - csv file with true and scatter simulated profile for phi 80-100 deg
fig_7
   randExtract.C                                - ROOT code to produce figures 7
   bottom_random_onlyThetaWindow_3BillLORs.csv  - csv file with a priori random profile for bottom capilaries
   bottom_true_onlyThetaWindow_3BillLORs.csv    - csv file with a priori true profile for bottom capilaries
   bottom_trueRandom_0-20_3BillLORs.csv         - csv file with true and random simulated profile for phi 0-20 deg
   bottom_trueRandom_80-100_3BillLORs.csv       - csv file with true and random simulated profile for phi 80-100 deg
figs_8_and_9
   entanglePlot.C                     - ROOT code to plot figures 8 and 9 using
   GetEnhancement.C                   - ROOT fitting code used by enntanglePlot.C
   entangledCZT_hist.root             - QE-Geant4 simulation in 10x10x10m3 CZT cube
   unentangledCZT_hist.root           - standard Geant4 simulation in 10x10x10m3 CZT cube

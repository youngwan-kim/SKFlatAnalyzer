#!/bin/bash

python python/SKFlat.py -a SMPValidation -y 2017 -i DoubleMuon -n 20 &
python python/SKFlat.py -a SMPValidation -y 2017 -i DYJets -n 100 &
python python/SKFlat.py -a SMPValidation -y 2017 -i TTLL_powheg -n 20 &
python python/SKFlat.py -a SMPValidation -y 2017 -i WJets_MG -n 50 &
python python/SKFlat.py -a SMPValidation -y 2017 -i WW_pythia -n 10 &
python python/SKFlat.py -a SMPValidation -y 2017 -i WZ_pythia -n 10 &
python python/SKFlat.py -a SMPValidation -y 2017 -i ZZ_pythia -n 10 &

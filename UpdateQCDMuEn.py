import os

l_sample=["QCD_Pt-1000_MuEnrichedPt5_TuneCP5_13TeV-pythia8_TauHLT",
"QCD_Pt-120To170_MuEnrichedPt5_TuneCP5_13TeV-pythia8_TauHLT",
"QCD_Pt-15To20_MuEnrichedPt5_TuneCP5_13TeV-pythia8_TauHLT",
"QCD_Pt-170To300_MuEnrichedPt5_TuneCP5_13TeV-pythia8_TauHLT",
"QCD_Pt-20To30_MuEnrichedPt5_TuneCP5_13TeV-pythia8_TauHLT",
"QCD_Pt-20_MuEnrichedPt15_TuneCP5_13TeV-pythia8_TauHLT",
"QCD_Pt-20toInf_MuEnrichedPt5_TuneCP5_13TeV_pythia8_TauHLT",
"QCD_Pt-300To470_MuEnrichedPt5_TuneCP5_13TeV-pythia8_TauHLT",
"QCD_Pt-30To50_MuEnrichedPt5_TuneCP5_13TeV-pythia8_TauHLT",
"QCD_Pt-30_MuEnrichedPt4_TuneCP5_13TeV_pythia8_TauHLT",
"QCD_Pt-470To600_MuEnrichedPt5_TuneCP5_13TeV-pythia8_TauHLT",
"QCD_Pt-50To80_MuEnrichedPt5_TuneCP5_13TeV-pythia8_TauHLT",
"QCD_Pt-600To800_MuEnrichedPt5_TuneCP5_13TeV-pythia8_TauHLT",
"QCD_Pt-800To1000_MuEnrichedPt5_TuneCP5_13TeV-pythia8_TauHLT",
"QCD_Pt-80To120_MuEnrichedPt5_TuneCP5_13TeV-pythia8_TauHLT"]


def save_absolute_paths(directory, output_file):
    with open(output_file, 'w') as file:
        for root, dirs, files in os.walk(directory):
            for file_name in files:
                file_path = os.path.abspath(os.path.join(root, file_name))
                file.write(file_path + '\n')


if __name__ == "__main__":
    # Replace 'path/to/directory_A' with the actual path of your directory A
    
    for s in l_sample :
    
        directory_A = f"/gv0/Users/youngwan/WRTauBkgMC/2017/{s}"
        output_file_name = f'/data9/Users/youngwan/work/SKFlatAnalyzer_Sandbox/data/Run2UltraLegacy_v3/2017/Sample/ForSNU/{s}.txt'
        output_file_path = os.path.join(directory_A, output_file_name)

        save_absolute_paths(directory_A, output_file_path)

        print(f"Absolute paths saved to {output_file_path}")
    
    '''for s in l_sample : 
        name = s.split("_")[1]
        os.system(f"cp /data9/Users/youngwan/work/SKFlatAnalyzer_Sandbox/data/Run2UltraLegacy_v3/2017/Sample/CommonSampleInfo/QCD_{name}_MuEnriched.txt {s}.txt")
        os.system(f"sed -i 's/QCD_{name}_MuEnriched/QCD_{name}_MuEnriched_TauHLT/g' {s}.txt")
        os.system(f"sed -i 's/QCD_{name}_MuEnrichedPt5_TuneCP5_13TeV-pythia8/{s}/g' {s}.txt")
    '''
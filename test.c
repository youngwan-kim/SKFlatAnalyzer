#include <iostream>
#include <vector>
#include "TFile.h"
#include "TTree.h"

void PrintBranchEntries(const char* filename, const char* branchName) {
    // Open the ROOT file
    TFile file(filename);
    
    // Access the TTree object
    TTree *tree = dynamic_cast<TTree*>(file.Get("recoTree/SKFlat")); // Replace "tree_name" with the name of your TTree
    
    if (!tree) {
        std::cerr << "Error: Could not find the TTree in the file." << std::endl;
        return;
    }
    
    // Check if the branch exists
    if (!tree->GetBranch(branchName)) {
        std::cerr << "Error: Branch '" << branchName << "' does not exist." << std::endl;
        return;
    }
    
    // Set the branch address to read its content
    std::vector<std::string> var;
    TBranch *branch = tree->GetBranch(branchName);
    branch->SetAddress(&var);
    
    // Loop over all entries in the branch and print them
    for (Long64_t i = 0; i < branch->GetEntries(); ++i) {
        branch->GetEntry(i);
        // Print the string values in the vector
        std::cout << "Entry " << i << ": ";
        for (const auto& str : var) {
            std::cout << str << " ";
        }
        std::cout << std::endl;
    }
}

int test() {
    const char* filename = "SKFlatNtuple_2016preVFP_MC_1.root"; // Replace "your_file.root" with the name of your ROOT file
    const char* branchName = "HLT_TriggerName"; // Replace "branch" with the name of your TBranch
    PrintBranchEntries(filename, branchName);
    return 0;
}


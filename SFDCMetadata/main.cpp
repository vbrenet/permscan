//
//  main.cpp
//  SFDCMetadata
//
//  Created by Vincent Brenet on 07/01/2020.
//  Copyright © 2020 Vincent Brenet. All rights reserved.
//

#include <iostream>
#include "metadataSession.hpp"
#include "Orchestrator.hpp"
#include "expectedParameters.hpp"
#include "ActualParameters.hpp"
#include "globals.hpp"
#include "config.hpp"
#include "utils.hpp"
//
//
const std::string version = "0.1.0";
//
//
//
void exitWithVersion() {
    std::cout << "permscan version: " << version << std::endl;
    exit(0);
}
//
//
//
void exitWithHelp() {
    std::cout << "SYNTAX:" << std::endl;
    std::cout << "permscan [-v] workingDirectory" << std::endl;
    std::cout << "permscan -help" << std::endl;
    std::cout << "permscan -version" << std::endl << std::endl;
    std::cout << "OPTIONS:" << std::endl;
    std::cout << "-v : verbose mode" << std::endl;
    std::cout << "-vv : very verbose mode (trace)" << std::endl;
    std::cout << "workingDirectory (mandatory) : working directory which must contain config files" << std::endl;
    exit(0);
}
//
//
//
void exitWithSyntaxError() {
    std::cerr << "Syntax error - SYNTAX : permscan [-version] [-help] [-v] [-vv] workingDirectory" << std::endl;
    exit(-1);
}
//
//
//
int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "exec name : " << argv[0] << std::endl;
    
    // argument analysis
    expectedParameters ep {
        true,
        1,
        {
            {"-v",{false,false}},
            {"-vv",{false,false}},
            {"-help",{false,false}},
            {"-version",{false,false}}
        }
    };
    
    ActualParameters ap;
        
    ap.set(argc, argv, ep);
    
    const std::vector<NamedParameter> parameters = ap.getNamedParameters();
    
    for (auto curr : parameters) {
        if (curr.getName().compare("-version") == 0)
            exitWithVersion();

        if (curr.getName().compare("-help") == 0)
            exitWithHelp();
    }
    
    if (!ap.areValid(ep))
        exitWithSyntaxError();
    
    for (auto curr : parameters) {
        
        if (curr.getName().compare("-v") == 0) {
            globals::verbose = true;
        }
        
        if (curr.getName().compare("-vv") == 0) {
            globals::verbose = true;
            globals::veryverbose = true;
        }

     }  // end for parameters
    
    
    const std::vector<std::string> values = ap.getValues();
    if (values.size() != 1) {
        std::cerr << "Missing parameter : workingDirectory" << std::endl;
        exitWithSyntaxError();
    }
    
    globals::workingDirectory = values[0];
    
    // lecture fichier de config
    if (!config::getConfig(globals::workingDirectory + "/config")) {
        std::cerr << "config file not found or empty" << std::endl;
        exitWithSyntaxError();
    }

    // if verbose, print custom object authorizations
    if (globals::verbose) {
        std::cout << "default custom object number: " << config::getDefaultAuthorizedObjectNumber() << std::endl;
        auto licensemap = config::getpslicensemap();
        for (auto it = licensemap.begin(); it != licensemap.end(); ++it)
            std::cout << "permission set license: " << it->first << " -custom objet number: " << it->second << std::endl;
    }
    
    orchestrator theOrchestrator {};
    
    if (!theOrchestrator.run()) {
        std::cerr << "run failure" << std::endl;
    }
    else
        std::cout << "run successfull" << std::endl;
    
    return 0;
}

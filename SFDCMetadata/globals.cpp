//
//  globals.cpp
//  SFDCMetadata
//
//  Created by Vincent Brenet on 22/12/2021.
//  Copyright © 2021 Vincent Brenet. All rights reserved.
//

#include "globals.hpp"

std::string globals::workingDirectory;
bool globals::verbose {false};
bool globals::veryverbose {false};
datasetOperation globals::operation;
int globals::nbOrgCustomObjects {0};


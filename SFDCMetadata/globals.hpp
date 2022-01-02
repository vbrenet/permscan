//
//  globals.hpp
//  SFDCMetadata
//
//  Created by Vincent Brenet on 22/12/2021.
//  Copyright © 2021 Vincent Brenet. All rights reserved.
//

#ifndef globals_hpp
#define globals_hpp

#include <stdio.h>
#include <string>

enum class datasetOperation  {OVERWRITE, APPEND, UPSERT};

class globals {
public:
    static std::string workingDirectory;
    static bool verbose;
    static bool veryverbose;
    static bool nodatasets;
    static int nbOrgCustomObjects;
};

#endif /* globals_hpp */

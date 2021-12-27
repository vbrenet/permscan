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

class globals {
public:
    static std::string workingDirectory;
    static bool verbose;
    static bool veryverbose;
};

#endif /* globals_hpp */

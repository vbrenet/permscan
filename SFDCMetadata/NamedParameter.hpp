//
//  NamedParameter.hpp
//  SFDCMetadata
//
//  Created by Vincent Brenet on 22/12/2021.
//  Copyright © 2021 Vincent Brenet. All rights reserved.
//

#ifndef NamedParameter_hpp
#define NamedParameter_hpp

#include <stdio.h>
#include <string>

class NamedParameter {
private:
    std::string name;
    std::string value;
public:
    NamedParameter(const std::string n, const std::string v) : name {n}, value {v} {}
    const std::string getName() const {return name;}
    const std::string getValue() const {return value;}
};

#endif /* NamedParameter_hpp */

//
//  ActualParameters.hpp
//  SFDCMetadata
//
//  Created by Vincent Brenet on 22/12/2021.
//  Copyright © 2021 Vincent Brenet. All rights reserved.
//

#ifndef ActualParameters_hpp
#define ActualParameters_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include "NamedParameter.hpp"
#include "expectedParameters.hpp"

class ActualParameters {
private:
    std::string currExecName {};
    std::vector<NamedParameter> namedParameters {};
    std::vector<std::string> values {};
public:
    ActualParameters() {}
    const std::vector<NamedParameter>& getNamedParameters() const {return namedParameters;}
    const std::vector<std::string>& getValues() const {return values;}
    const std::string getExecName() const {return  currExecName;}
    void set(int argc, const char * argv[], const expectedParameters&);
    bool areValid(const expectedParameters&) const;
    void print() const;
};
#endif /* ActualParameters_hpp */

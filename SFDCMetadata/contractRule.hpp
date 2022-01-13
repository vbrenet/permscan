//
//  contractRule.hpp
//  SFDCMetadata
//
//  Created by Vincent Brenet on 07/01/2022.
//  Copyright © 2022 Vincent Brenet. All rights reserved.
//

#ifndef contractRule_hpp
#define contractRule_hpp

#include <stdio.h>
#include <string>
#include <vector>

class contractRule {
private:
    std::vector<std::string> forbiddenObjects;
    std::vector<std::string> forbiddenFeatures;
public:
    contractRule() {forbiddenObjects.clear(); forbiddenFeatures.clear();}
    //
    const std::vector<std::string>& getForbiddenObjects() const {return forbiddenObjects;}
    const std::vector<std::string>& getForbiddenFeatures() const {return forbiddenFeatures;}
    //
    void addObject(std::string o) {forbiddenObjects.push_back(o);}
    void addFeature(std::string f) {forbiddenFeatures.push_back(f);}
    //
    void printRule() const;
};
#endif /* contractRule_hpp */

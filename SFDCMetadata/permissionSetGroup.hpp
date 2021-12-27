//
//  permissionSetGroup.hpp
//  SFDCMetadata
//
//  Created by Vincent Brenet on 27/12/2021.
//  Copyright © 2021 Vincent Brenet. All rights reserved.
//

#ifndef permissionSetGroup_hpp
#define permissionSetGroup_hpp

#include <stdio.h>
#include <string>
#include <vector>

class permissionSetGroup {
private:
    std::string developername;
    std::string label;
    std::string id;
    std::vector<std::string> psids;
    
public:
    permissionSetGroup(const std::string i, const std::string l, const std::string n) : id {i}, label {l}, developername {n} {}
    const std::string getDeveloperName() const {return developername;}
    const std::string getLabel() const {return label;}
    const std::string getId() const {return id;}
    const std::vector<std::string>& getPsids() const {return psids;}
//
    void insertPsid(const std::string psid) {psids.push_back(psid);}
};
#endif /* permissionSetGroup_hpp */

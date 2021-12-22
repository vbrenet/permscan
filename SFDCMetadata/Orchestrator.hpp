//
//  Orchestrator.hpp
//  SFDCMetadata
//
//  Created by Vincent Brenet on 21/12/2021.
//  Copyright © 2021 Vincent Brenet. All rights reserved.
//

#ifndef Orchestrator_hpp
#define Orchestrator_hpp

#include <stdio.h>
#include <string>
#include <map>
#include "permissionSet.hpp"

class orchestrator {
private:
    std::map<std::string,permissionSet> permissionSetMap;
    void initializePermissionsSet(const std::string& xmlBuffer);
    void addObjectsToPermissionSet(std::string id, const std::string& xmlBuffer);
public:
    bool run();
};
#endif /* Orchestrator_hpp */

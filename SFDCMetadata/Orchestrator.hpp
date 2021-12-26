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
#include "salesforceUser.hpp"
#include "profile.hpp"
#include "license.hpp"
#include "permissionSetLicense.hpp"

class orchestrator {
private:
    std::map<std::string,permissionSet> permissionSetMap;
    std::map<std::string,profile> profileMap;
    std::map<std::string,salesforceUser> userMap;
    std::map<std::string,license> licenseMap;
    std::map<std::string,permissionSetLicense> permissionSetLicenseMap;

    void initializePermissionsSet(const std::string& xmlBuffer);
    void initializeProfiles(const std::string& xmlBuffer);
    void initializeLicenses(const std::string& xmlBuffer);
    void initializePermissionSetLicenses(const std::string& xmlBuffer);
    bool initializeUsers(const std::string& xmlBuffer, std::string& nextUrl);
    void addObjectsToPermissionSet(std::string id, const std::string& xmlBuffer);
    void addObjectsToProfile(std::string id, const std::string& xmlBuffer);
    bool updateUsersWithPermissionSetId (std::string id, const std::string& xmlBuffer, std::string& nextUrl);
    void addPermissionSetObjectsToUser(std::string psid, std::string assigneeid);
public:
    bool run();
};
#endif /* Orchestrator_hpp */

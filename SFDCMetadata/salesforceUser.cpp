//
//  salesforceUser.cpp
//  SFDCMetadata
//
//  Created by Vincent Brenet on 23/12/2021.
//  Copyright © 2021 Vincent Brenet. All rights reserved.
//
#include <iostream>
#include "salesforceUser.hpp"
#include "utils.hpp"
#include "config.hpp"
#include "globals.hpp"
//
//
//
bool salesforceUser::isCompliant() const {
    if (viewAllData || modifyAllData)
        return globals::nbOrgCustomObjects <= maxcustomobjects;
    else
        return nbCustomObjects() <= maxcustomobjects;
}
//
//
//
void salesforceUser::computeMaxCustomObjects() {
    
    maxcustomobjects = config::getDefaultAuthorizedObjectNumber();
    auto authmap = config::getpslicensemap();

    for (auto it=permissionSetLicenses.begin(); it!=permissionSetLicenses.end(); ++it) {
        auto authampentry = authmap.find(*it);
        if (authampentry != authmap.end()) {
            if (authampentry->second > maxcustomobjects) {
                maxcustomobjects = authampentry->second;
            }
        }
    }
    
    // license entry, if exist, supersedes the PS license map authorizations
    auto autlicensemap = config::getlicensemap();
    auto entry = autlicensemap.find(getLicenseName());
    if (entry != autlicensemap.end()) {
        maxcustomobjects = entry->second;
    }
}
//
//
//
void salesforceUser::distributeObjects() {
    for (auto it = allPermittedObjects.begin(); it != allPermittedObjects.end(); ++it) {
        if (endsWith(*it, "__c")) {
            std::string prefix = objectNamePrefix(*it);
            if (prefix.size() == 0) {
                customobjects.push_back(*it);
            }
            else {
                packagedobjects.push_back(*it);
            }
        }
        else {
            standardobjects.push_back(*it);
        }
    }
}
//
//
//
void salesforceUser::print(std::ofstream& output) {
    output << id << ": " << username << " -profile id: " << profileid << " -profile name: " << profilename << " -license: " << licensename << std::endl;
    output << "Total Permission Set Licenses : " << permissionSetLicenses.size() << std::endl;
    for (auto it = permissionSetLicenses.begin(); it != permissionSetLicenses.end(); ++it) output << *it << " "; output << std::endl;
    output << "Max authorized objects: " << getMaxCustomObjects() << std::endl;
    output << "Compliant ? " << std::boolalpha << isCompliant() << std::endl;
    output << "Total objects : " << allPermittedObjects.size() << std::endl;
    output << "Custom objects : " << customobjects.size() << std::endl;
    for (auto it = customobjects.begin(); it != customobjects.end(); ++it) output << *it << " "; output << std::endl;
    output << "Packaged objects : " << packagedobjects.size() << std::endl;
    for (auto it = packagedobjects.begin(); it != packagedobjects.end(); ++it) output << *it << " "; output << std::endl;
    output << "Standard objects : " << standardobjects.size() << std::endl;
    for (auto it = standardobjects.begin(); it != standardobjects.end(); ++it) output << *it << " "; output << std::endl;
}

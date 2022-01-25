//
//  salesforceUser.cpp
//  SFDCMetadata
//
//  Created by Vincent Brenet on 23/12/2021.
//  Copyright © 2021 Vincent Brenet. All rights reserved.
//
#include <iostream>
#include <sstream>
#include "salesforceUser.hpp"
#include "utils.hpp"
#include "config.hpp"
#include "globals.hpp"
//
//
//
const long salesforceUser::nbCustomObjects() const {
    if (viewAllData || modifyAllData)
        return globals::nbOrgCustomObjects;
    else
        return customobjects.size();
}
//
//
//
bool salesforceUser::isCompliant() const {
    return complianceStatus;
}
//
//
//
bool salesforceUser::computeComplianceStatus() {
    complianceStatus = nbCustomObjects() <= maxcustomobjects;
    if (!complianceStatus) {
        std::stringstream reason;
        reason << "Too Much Custom Objects: " << nbCustomObjects() << " vs " << maxcustomobjects << " permitted";
        nonComplianceReason = reason.str();
        nonComplianceCode = "TooMuchCustomObjects";
    }
    else {
        // 0- check if user has CPQ permission set license or Field Service Dispatcher license
        bool hasCPQ {false};
        bool hasFslPlus {false};
        for (auto it = permissionSetLicenses.begin(); it != permissionSetLicenses.end(); ++it) {
            if ((*it).compare("Salesforce CPQ License") == 0)   hasCPQ = true;
            if ((*it).compare("Field Service Dispatcher") == 0)   hasFslPlus = true;
        }
        // search user permissionSetLicenses matching contract rules
        bool atleastonematching {false};
        for (auto itpsrules = config::getpsLicenseContractRules().begin(); itpsrules != config::getpsLicenseContractRules().end(); ++itpsrules) {
            for (auto ituserpslicense = permissionSetLicenses.begin(); ituserpslicense != permissionSetLicenses.end(); ++ituserpslicense) {
                // test if current user ps license match the current rule
                if (itpsrules->first.compare(*ituserpslicense) == 0) {
                    atleastonematching = true;
                    auto psrules = itpsrules->second;
                    auto itobj = psrules.getForbiddenObjects();
                    // search forbidden objects
                    for (auto it = itobj.begin(); it != itobj.end(); ++it) {
                        std::string forbiddenobjecttosearch = *it;
                        // check if user has view or modify all
                        if (isViewAllData() || isModifyAllData()) {
                            std::stringstream reason;
                            reason << "Forbidden Object for " << *ituserpslicense << " license: " << forbiddenobjecttosearch << " and user has view or modify all";
                            nonComplianceReason = reason.str();
                            nonComplianceCode = "ForbiddenObjectAndViewOrModifyAll";
                            complianceStatus = false;
                            break;
                        }
                        // search in the user objects
                        if (allPermittedObjects.find(forbiddenobjecttosearch) != allPermittedObjects.end()) {
                            // user has access to the object
                            if ((forbiddenobjecttosearch.compare("SBQQ__Quote__c") == 0 && hasCPQ) ||
                                ((forbiddenobjecttosearch.compare("Contract") == 0) && hasCPQ && hasFslPlus)) {
                                // user has access to the object because he/she has CPQ or FSL +
                            }
                            else {
                                std::stringstream reason;
                                reason << "Forbidden Object for " << *ituserpslicense << " license: " << forbiddenobjecttosearch;
                                nonComplianceReason = reason.str();
                                nonComplianceCode = "ForbiddenObject";
                                complianceStatus = false;
                                break;
                            }
                        }   // forbidden object found
                    }   // end for forbidden objects
                    if (!complianceStatus)
                        break;  // we can leave because one broken rule is enough
                    auto itforbiddenFeatures = psrules.getForbiddenFeatures();
                    for (auto it = itforbiddenFeatures.begin(); it != itforbiddenFeatures.end(); ++it) {
                        if (allPermissions.find(*it) != allPermissions.end()) {
                            nonComplianceCode = "PermissionNotAllowed";
                            std::stringstream reason;
                            reason << "Permission " << *it << " not allowed for this license: " << *ituserpslicense;
                            nonComplianceReason = reason.str();
                            complianceStatus = false;
                            break;
                        }
                        if ((*it).compare("Forecast") == 0) {
                            if (forecastEnabled) {
                                nonComplianceReason = "Forecast enabled not permitted for this license";
                                nonComplianceCode = "ForecastNotPermitted";
                                complianceStatus = false;
                                break;
                            }
                        }
                    } // end for
                    break; // matching found, so we can break
                }   // end matching
            }   // end for browsing user ps license
            if (!complianceStatus)
                break;  // we can leave because one broke rule is enough
        }   // end for browsing all ps license contract rule

        // now analyze default rules
        if (complianceStatus && !atleastonematching) {
            // 1- search forbidden objects
            contractRule defaultRules = config::getContractRules();
            auto itforbiddenObjects = defaultRules.getForbiddenObjects();
            for (auto it = itforbiddenObjects.begin(); it != itforbiddenObjects.end(); ++it) {
                std::string forbiddenobjecttosearch = *it;
                if (isViewAllData() || isModifyAllData()) {
                    std::stringstream reason;
                    reason << "Forbidden Object for for Sales & Service: " << forbiddenobjecttosearch << " and user has view or modify all";
                    nonComplianceReason = reason.str();
                    nonComplianceCode = "ForbiddenObjectAndViewOrModifyAll";
                    complianceStatus = false;
                    break;
                }
                // search in the user objects
                if (allPermittedObjects.find(forbiddenobjecttosearch) != allPermittedObjects.end()) {
                    if (forbiddenobjecttosearch.compare("SBQQ__Quote__c") == 0 && hasCPQ) {
                        // user has access to the object because he/she has CPQ
                    }
                    else {
                        std::stringstream reason;
                        reason << "Forbidden Object for Sales & Service : " << forbiddenobjecttosearch;
                        nonComplianceReason = reason.str();
                        nonComplianceCode = "ForbiddenObject";
                        complianceStatus = false;
                        break;
                    }
                }   // forbidden object found
            }   // end for forbidden objects
            
            // 2- at this stage there are no forbidden objects - now we search for forbidden features
            auto itforbiddenFeatures = defaultRules.getForbiddenFeatures();
            for (auto it = itforbiddenFeatures.begin(); it != itforbiddenFeatures.end(); ++it) {
                if (allPermissions.find(*it) != allPermissions.end()) {
                    nonComplianceCode = "PermissionNotAllowed";
                    std::stringstream reason;
                    reason << "Permission " << *it << " not allowed in default rules";
                    nonComplianceReason = reason.str();
                    complianceStatus = false;
                    break;
                }
                if ((*it).compare("Forecast") == 0) {
                    if (forecastEnabled) {
                        nonComplianceReason = "Forecast enabled not permitted for this license";
                        nonComplianceCode = "ForecastNotPermitted";
                        complianceStatus = false;
                        break;
                    }
                }
            }   // end for forbidden features
        }   // end default rule analysis
    }
    return complianceStatus;
}
//
//
//
bool salesforceUser::computeComplianceStatusOld() {
    complianceStatus = nbCustomObjects() <= maxcustomobjects;
    if (!complianceStatus) {
        std::stringstream reason;
        reason << "Too Much Custom Objects: " << nbCustomObjects() << " vs " << maxcustomobjects << " permitted";
        nonComplianceReason = reason.str();
        nonComplianceCode = "TooMuchCustomObjects";
    }
    else {
        // 0- check if user has CPQ permission set license
        //bool hasCPQ = (std::find(permissionSetLicenses.begin(), permissionSetLicenses.end(), "Salesforce CPQ License") != permissionSetLicenses.end());
        bool hasCPQ {false};
        for (auto it = permissionSetLicenses.begin(); it != permissionSetLicenses.end(); ++it) {
            if ((*it).compare("Salesforce CPQ License") == 0) {
                hasCPQ = true;
                break;
            }
        }
        // search user permissionSetLicenses matching contract rules
        for (auto itpsrules = config::getpsLicenseContractRules().begin(); itpsrules != config::getpsLicenseContractRules().end(); ++itpsrules) {
            for (auto ituserpslicense = permissionSetLicenses.begin(); ituserpslicense != permissionSetLicenses.end(); ++ituserpslicense) {
                // test if current user ps license match the current rule
                if (itpsrules->first.compare(*ituserpslicense) == 0) {
                    auto psrules = itpsrules->second;
                    auto itobj = psrules.getForbiddenObjects();
                    // search forbidden objects
                    for (auto it = itobj.begin(); it != itobj.end(); ++it) {
                        std::string forbiddenobjecttosearch = *it;
                        // search in the user objects
                        if (allPermittedObjects.find(forbiddenobjecttosearch) != allPermittedObjects.end()) {
                            // user has access to the object
                            if (hasCPQ && (forbiddenobjecttosearch.compare("SBQQ__Quote__c") ==0 || forbiddenobjecttosearch.compare("Contract") ==0)) {
                                // user has access to the object because he/she has CPQ
                            }
                            else {
                                std::stringstream reason;
                                reason << "Forbidden Object for this license: " << forbiddenobjecttosearch;
                                nonComplianceReason = reason.str();
                                nonComplianceCode = "ForbiddenObject";
                                complianceStatus = false;
                                break;
                            }
                        }   // forbidden object found
                    }   // end for forbidden objects
                    if (!complianceStatus)
                        break;  // we can leave because one broke rule is enough
                    auto itforbiddenFeatures = psrules.getForbiddenFeatures();
                    for (auto it = itforbiddenFeatures.begin(); it != itforbiddenFeatures.end(); ++it) {
                        if (allPermissions.find(*it) != allPermissions.end()) {
                            nonComplianceCode = "PermissionNotAllowed";
                            std::stringstream reason;
                            reason << "Permission " << *it << " not allowed for this license: " << *ituserpslicense;
                            nonComplianceReason = reason.str();
                            complianceStatus = false;
                            break;
                        }
                        if ((*it).compare("Forecast") ==0) {
                            if (forecastEnabled) {
                                nonComplianceReason = "Forecast enabled not permitted for this license";
                                nonComplianceCode = "ForecastNotPermitted";
                                complianceStatus = false;
                                break;
                            }
                        }
                    } // end for
                    break; // matching found, so we can break
                }   // end matching
            }   // end for browsing user ps license
            if (!complianceStatus)
                break;  // we can leave because one broke rule is enough
        }   // end for browsing all ps license contract rule

        // now analyze default rules
        if (complianceStatus) {
            // 1- search forbidden objects
            contractRule defaultRules = config::getContractRules();
            auto itforbiddenObjects = defaultRules.getForbiddenObjects();
            for (auto it = itforbiddenObjects.begin(); it != itforbiddenObjects.end(); ++it) {
                std::string forbiddenobjecttosearch = *it;
                // search in the user objects
                if (allPermittedObjects.find(forbiddenobjecttosearch) != allPermittedObjects.end()) {
                    // user has access to the object
                    if (hasCPQ && (forbiddenobjecttosearch.compare("SBQQ__Quote__c") ==0 || forbiddenobjecttosearch.compare("Contract") ==0)) {
                        // user has access to the object because he/she has CPQ
                    }
                    else {
                        std::stringstream reason;
                        reason << "Forbidden Object for this license: " << forbiddenobjecttosearch;
                        nonComplianceReason = reason.str();
                        nonComplianceCode = "ForbiddenObject";
                        complianceStatus = false;
                        break;
                    }
                }   // forbidden object found
            }   // end for forbidden objects
            
            // 2- at this stage there are no forbidden objects - now we search for forbidden features
            auto itforbiddenFeatures = defaultRules.getForbiddenFeatures();
            for (auto it = itforbiddenFeatures.begin(); it != itforbiddenFeatures.end(); ++it) {
                if (allPermissions.find(*it) != allPermissions.end()) {
                    nonComplianceCode = "PermissionNotAllowed";
                    std::stringstream reason;
                    reason << "Permission " << *it << " not allowed in default rules";
                    nonComplianceReason = reason.str();
                    complianceStatus = false;
                    break;
                }
                if ((*it).compare("Forecast") ==0) {
                    if (forecastEnabled) {
                        nonComplianceReason = "Forecast enabled not permitted for this license";
                        nonComplianceCode = "ForecastNotPermitted";
                        complianceStatus = false;
                        break;
                    }
                }
            }   // end for forbidden features
        }   // end default rule analysis
    }
    return complianceStatus;
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
        if (endsWith(*it, "__c") || endsWith(*it, "__mdt")) {
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
    output << std::endl
    << "********************************************************************************************************************************************************************"
    << std::endl << getFirstName() << " " << getLastName() << ": Compliant ? " << std::boolalpha << isCompliant() << std::endl;
    if (!isCompliant()) {
        output << "Code reason : " << getNonComplianceCode() << " : " << getNonComplianceReason() << std::endl;
    }
    output << id << ": " << username << " -profile id: " << profileid << " -profile name: " << profilename << " -license: " << licensename << std::endl;
    output << "Max authorized objects: " << getMaxCustomObjects() << std::endl;
    output << std::boolalpha << "View all ? " << isViewAllData() << " Modify all ? " << isModifyAllData() << std::endl;
    output << "Total Permission Set Licenses : " << permissionSetLicenses.size() << std::endl;
    for (auto it = permissionSetLicenses.begin(); it != permissionSetLicenses.end(); ++it) output << *it << "; "; output << std::endl;
    output << "Total objects : " << nbCustomObjects() + packagedobjects.size() + standardobjects.size() << std::endl;
    output << "Custom objects : " << nbCustomObjects() << std::endl;
    for (auto it = customobjects.begin(); it != customobjects.end(); ++it) output << *it << "; "; output << std::endl;
    output << "Packaged objects : " << packagedobjects.size() << std::endl;
    for (auto it = packagedobjects.begin(); it != packagedobjects.end(); ++it) output << *it << "; "; output << std::endl;
    output << "Standard objects : " << standardobjects.size() << std::endl;
    for (auto it = standardobjects.begin(); it != standardobjects.end(); ++it) output << *it << "; "; output << std::endl;
}

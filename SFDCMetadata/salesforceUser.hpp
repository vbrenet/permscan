//
//  salesforceUser.hpp
//  SFDCMetadata
//
//  Created by Vincent Brenet on 23/12/2021.
//  Copyright © 2021 Vincent Brenet. All rights reserved.
//

#ifndef salesforceUser_hpp
#define salesforceUser_hpp

#include <stdio.h>
#include <fstream>
#include <string>
#include <set>
#include <vector>
#include "permissionSetLicense.hpp"

class salesforceUser {
private:
    std::string username;
    std::string id;
    std::string firstname;
    std::string lastname;
    std::string profileid;
    std::string profilename;
    std::string licensename;
    std::vector<std::string> permissionSetLicenses;
    std::set<std::string> allPermittedObjects;
    std::vector<std::string> customobjects;
    std::vector<std::string> packagedobjects;
    std::vector<std::string> standardobjects;
    int maxcustomobjects;
    bool viewAllData;
    bool modifyAllData;

public:
    salesforceUser(const std::string i, const std::string f, const std::string l, const std::string u, const std::string p) : username {u}, firstname {f}, lastname {l}, id {i}, profileid {p} {viewAllData=false;modifyAllData=false;}
    //  accessors
    const std::string getFullName() const {return firstname + " " + lastname;}
    const std::string getFirstName() const {return firstname;}
    const std::string getLastName() const {return lastname;}
    const std::string getId() const {return id;}
    const std::string getUsername() const {return username;}
    const std::string getProfile() const {return profileid;}
    const std::string getProfileName() const {return profilename;}
    const std::string getLicenseName() const {return licensename;}
    const std::set<std::string>& getPermittedObjects() const {return allPermittedObjects;}
    const long nbObjects() {return nbCustomObjects() + nbPackagedObjects() + nbStandardObjects();}
    const long nbCustomObjects() const;
    const long nbPackagedObjects() const {return packagedobjects.size();}
    const long nbStandardObjects() const {return standardobjects.size();}
    int getMaxCustomObjects() const {return maxcustomobjects;}
    bool isCompliant() const;
    const long getPermissionSetLicenceNumber() const {return permissionSetLicenses.size();}
    bool isViewAllData() const {return viewAllData;}
    bool isModifyAllData() const {return modifyAllData;}
   //
    void setProfileName(const std::string name) {profilename=name;}
    void setLicenseName(const std::string name) {licensename=name;}
    void insertPermittedObject(const std::string o) {allPermittedObjects.insert(o);}
    void insertPermissionSetLicense(const std::string pslicence) {permissionSetLicenses.push_back(pslicence);}
    void setViewAllData() {viewAllData=true;}
    void setModifyAllData() {modifyAllData=true;}

    //
    void computeMaxCustomObjects();
    void distributeObjects();
    void print(std::ofstream&);
};
#endif /* salesforceUser_hpp */

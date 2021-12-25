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
#include <string>
#include <set>
#include <vector>

class salesforceUser {
private:
    std::string username;
    std::string id;
    std::string firstname;
    std::string lastname;
    std::string theprofile;
    std::set<std::string> allPermittedObjects;
    std::vector<std::string> customobjects;
    std::vector<std::string> packagedobjects;
    std::vector<std::string> standardobjects;

public:
    salesforceUser(const std::string i, const std::string f, const std::string l, const std::string u, const std::string p) : username {u}, firstname {f}, lastname {l}, id {i}, theprofile {p} {}
    //  accessors
    const std::string getFullName() const {return firstname + " " + lastname;}
    const std::string getId() const {return id;}
    const std::string getUsername() const {return username;}
    const std::string getProfile() const {return theprofile;}
    const std::set<std::string>& getPermittedObjects() const {return allPermittedObjects;}
    const long nbCustomObjects() {return customobjects.size();}
    const long nbPackagedObjects() {return packagedobjects.size();}
    const long nbStandardObjects() {return standardobjects.size();}
    //
    void insertPermittedObject(const std::string o) {allPermittedObjects.insert(o);}
    //
    void distributeObjects();
    void print();
};
#endif /* salesforceUser_hpp */

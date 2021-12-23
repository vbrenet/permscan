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

class salesforceUser {
private:
    std::string username;
    std::string id;
    std::string firstname;
    std::string lastname;
    std::string theprofile;
    std::set<std::string> allPermittedObjects;
    
public:
    salesforceUser(const std::string i, const std::string f, const std::string l, const std::string u, const std::string p) : username {u}, firstname {f}, lastname {l}, id {i}, theprofile {p} {}
    //  accessors
    const std::string getFullName() const {return firstname + " " + lastname;}
    const std::string getId() const {return id;}
    const std::string getUsername() const {return username;}
    const std::string getProfile() const {return theprofile;}
    const std::set<std::string>& getPermittedObjects() const {return allPermittedObjects;}
    //
    void insertPermittedObject(const std::string o) {allPermittedObjects.insert(o);}
};
#endif /* salesforceUser_hpp */

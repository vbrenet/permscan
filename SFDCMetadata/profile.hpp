//
//  profile.hpp
//  SFDCMetadata
//
//  Created by Vincent Brenet on 23/12/2021.
//  Copyright © 2021 Vincent Brenet. All rights reserved.
//

#ifndef profile_hpp
#define profile_hpp

#include <stdio.h>
#include <string>
#include <set>

class profile {
private:
    std::string name;
    std::string id;
    std::string licenceId;
    std::set<std::string> objects;
    
public:
    profile(const std::string i, const std::string n, const std::string lid) : name {n}, id {i}, licenceId {lid} {}
    //
    const std::string getName() const {return name;}
    const std::string getId() const {return id;}
    const std::string getLicenceId() const {return licenceId;}
    const std::set<std::string>& getobjects() const {return objects;}
    //
    void insertObject(const std::string o) {objects.insert(o);}
};
#endif /* profile_hpp */

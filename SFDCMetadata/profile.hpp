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
#include <vector>

class profile {
private:
    std::string name;
    std::string id;
    std::string licenceId;
    std::set<std::string> objects;
    std::vector<std::string> customobjects;
    std::vector<std::string> packagedobjects;
    std::vector<std::string> standardobjects;

public:
    profile(const std::string i, const std::string n, const std::string lid) : name {n}, id {i}, licenceId {lid} {}
    //
    const std::string getName() const {return name;}
    const std::string getId() const {return id;}
    const std::string getLicenceId() const {return licenceId;}
    const std::set<std::string>& getobjects() const {return objects;}
    long getTotalNumberOfObjects() {return objects.size();}
    long getNumberOfCustomObjects() {return customobjects.size();}
    long getNumberOfPackagedObjects() {return packagedobjects.size();}
    long getNumberOfStandardObjects() {return standardobjects.size();}
    //
    void insertObject(const std::string o) {objects.insert(o);}
    //
    void distributeObjects();
};
#endif /* profile_hpp */

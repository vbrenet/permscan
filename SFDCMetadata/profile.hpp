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
    std::set<std::string> permissions;
    std::set<std::string> objects;
    std::vector<std::string> customobjects;
    std::vector<std::string> packagedobjects;
    std::vector<std::string> standardobjects;
    bool viewAllData;
    bool modifyAllData;
    bool lightningConsole;
public:
    profile(const std::string i, const std::string n, const std::string lid) : name {n}, id {i}, licenceId {lid} {viewAllData=false;modifyAllData=false;lightningConsole=false;}
    //
    const std::string getName() const {return name;}
    const std::string getId() const {return id;}
    const std::string getLicenceId() const {return licenceId;}
    const std::set<std::string>& getobjects() const {return objects;}
    const std::set<std::string>& getpermissions() const {return permissions;}
    long getTotalNumberOfObjects() const {return objects.size();}
    long getNumberOfCustomObjects() const {return customobjects.size();}
    long getNumberOfPackagedObjects() const {return packagedobjects.size();}
    long getNumberOfStandardObjects() const {return standardobjects.size();}
    bool isViewAllData() const {return viewAllData;}
    bool isModifyAllData() const {return modifyAllData;}
    bool hasLightningConsole() const {return lightningConsole;}
    //
    void insertObject(const std::string o) {objects.insert(o);}
    void insertPermission(const std::string p) {permissions.insert(p);}
    //
    void distributeObjects();
    //
    void setViewAllData() {viewAllData=true;}
    void setModifyAllData() {modifyAllData=true;}
    void setLightningConsole() {lightningConsole=true;}
    //
    bool hasObject(const std::string o) const {return (objects.find(o) != objects.end());}
    bool hasPermission(const std::string p) const {return (permissions.find(p) != permissions.end());}
};
#endif /* profile_hpp */

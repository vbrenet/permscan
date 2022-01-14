//
//  permissionSet.hpp
//  SFDCMetadata
//
//  Created by Vincent Brenet on 22/12/2021.
//  Copyright © 2021 Vincent Brenet. All rights reserved.
//

#ifndef permissionSet_hpp
#define permissionSet_hpp

#include <stdio.h>
#include <string>
#include <set>
#include <vector>

class permissionSet {
private:
    std::string name;
    std::string id;
    std::set<std::string> permissions;
    std::set<std::string> objects;
    std::vector<std::string> customobjects;
    std::vector<std::string> packagedobjects;
    std::vector<std::string> standardobjects;
    bool viewAllData;
    bool modifyAllData;
    bool lightningConsole;

public:
    permissionSet(const std::string i, const std::string n) : name {n}, id {i} {viewAllData=false;modifyAllData=false;}
    const std::string getName() const {return name;}
    const std::string getId() const {return id;}
    void insertPermission(const std::string p) {permissions.insert(p);}
    void insertObject(const std::string o) {objects.insert(o);}
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
    void setViewAllData() {viewAllData=true;}
    void setModifyAllData() {modifyAllData=true;}
    void setLightningConsole() {lightningConsole=true;}
    
    void distributeObjects();
    bool hasObject(const std::string o) const {return (objects.find(o) != objects.end());}
    bool hasPermission(const std::string p) const {return (permissions.find(p) != permissions.end());}

};
#endif /* permissionSet_hpp */

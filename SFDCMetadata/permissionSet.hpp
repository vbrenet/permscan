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
    std::set<std::string> objects;
    std::vector<std::string> customobjects;
    std::vector<std::string> packagedobjects;
    std::vector<std::string> standardobjects;

public:
    permissionSet(const std::string i, const std::string n) : name {n}, id {i} {}
    const std::string getName() const {return name;}
    const std::string getId() const {return id;}
    void insertObject(const std::string o) {objects.insert(o);}
    const std::set<std::string>& getobjects() const {return objects;}
    long getTotalNumberOfObjects() {return objects.size();}
    long getNumberOfCustomObjects() {return customobjects.size();}
    long getNumberOfPackagedObjects() {return packagedobjects.size();}
    long getNumberOfStandardObjects() {return standardobjects.size();}
    //
    void distributeObjects();
};
#endif /* permissionSet_hpp */

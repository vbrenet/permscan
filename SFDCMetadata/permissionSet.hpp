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

class permissionSet {
private:
    std::string name;
    std::string id;
    std::set<std::string> objects;
    
public:
    permissionSet(const std::string i, const std::string n) : name {n}, id {i} {}
    const std::string getName() const {return name;}
    const std::string getId() const {return id;}
    void insertObject(const std::string o) {objects.insert(o);}
    const std::set<std::string>& getobjects() const {return objects;}
};
#endif /* permissionSet_hpp */

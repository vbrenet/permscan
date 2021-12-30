//
//  permissionSet.cpp
//  SFDCMetadata
//
//  Created by Vincent Brenet on 22/12/2021.
//  Copyright © 2021 Vincent Brenet. All rights reserved.
//

#include "permissionSet.hpp"
#include "utils.hpp"
//
//
//
void permissionSet::distributeObjects() {
    for (auto it = objects.begin(); it != objects.end(); ++it) {
        if (endsWith(*it, "__c")) {
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

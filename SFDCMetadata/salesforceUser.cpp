//
//  salesforceUser.cpp
//  SFDCMetadata
//
//  Created by Vincent Brenet on 23/12/2021.
//  Copyright © 2021 Vincent Brenet. All rights reserved.
//
#include <iostream>
#include "salesforceUser.hpp"
#include "utils.hpp"
//
//
void salesforceUser::distributeObjects() {
    for (auto it = allPermittedObjects.begin(); it != allPermittedObjects.end(); ++it) {
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
//
//
void salesforceUser::print() {
    std::cout << id << ": " << username << " -profile id: " << profileid << " -profile name: " << profilename << " -license: " << licencename << std::endl;
    std::cout << "Total objects : " << allPermittedObjects.size() << std::endl;
    std::cout << "Custom objects : " << customobjects.size() << std::endl;
    for (auto it = customobjects.begin(); it != customobjects.end(); ++it) std::cout << *it << " "; std::cout << std::endl;
    std::cout << "Packaged objects : " << packagedobjects.size() << std::endl;
    for (auto it = packagedobjects.begin(); it != packagedobjects.end(); ++it) std::cout << *it << " "; std::cout << std::endl;
    std::cout << "Standard objects : " << standardobjects.size() << std::endl;
    for (auto it = standardobjects.begin(); it != standardobjects.end(); ++it) std::cout << *it << " "; std::cout << std::endl;
}


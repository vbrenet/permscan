//
//  contractRule.cpp
//  SFDCMetadata
//
//  Created by Vincent Brenet on 07/01/2022.
//  Copyright © 2022 Vincent Brenet. All rights reserved.
//
#include <iostream>
#include "contractRule.hpp"

void contractRule::printRule() const {
    std::cout << "forbiddenObjects: ";
    for (auto it = forbiddenObjects.begin(); it != forbiddenObjects.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl << "forbiddenFeatures: ";
    for (auto it = forbiddenFeatures.begin(); it != forbiddenFeatures.end(); ++it) {
        std::cout << *it << " ";
    }
}

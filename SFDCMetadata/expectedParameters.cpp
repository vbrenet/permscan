//
//  expectedParameters.cpp
//  SFDCMetadata
//
//  Created by Vincent Brenet on 22/12/2021.
//  Copyright © 2021 Vincent Brenet. All rights reserved.
//

#include "expectedParameters.hpp"

bool expectedParameters::isValued(const std::string parameterName) const {
    auto d = descriptors.find(parameterName);
    if (d != descriptors.end())
        return d->second.isValued;
    return false;
}

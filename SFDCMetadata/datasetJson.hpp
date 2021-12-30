//
//  datasetJson.hpp
//  SFDCMetadata
//
//  Created by Vincent Brenet on 30/12/2021.
//  Copyright © 2021 Vincent Brenet. All rights reserved.
//

#ifndef datasetJson_hpp
#define datasetJson_hpp

#include <stdio.h>
#include <string>
#include <sstream>


class datasetJson {
private:
    static void initFieldBookJson();
    static void initGlobalUsageJson();
    static void initUsageByRTJson();
    static void initPicklistUsageJson();
    static void initPicklistUsageByRTJson();

    static void addTextField(std::stringstream&, const std::string);
    
public:
    static std::string fieldBookJson;
    static std::string globalUsageJson;
    static std::string usageByRTJson;
    static std::string picklistUsageJson;
    static std::string picklistUsageByRTJson;

    
    static void initDatasetJsonDescriptors();
};
#endif /* datasetJson_hpp */

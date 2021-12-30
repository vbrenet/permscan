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
    static void initUsersJson();
    static void initPermissionSetsJson();
    static void initProfilesJson();

    static void addTextField(std::stringstream&, const std::string);
    
public:
    static std::string usersJson;
    static std::string permissionSetsJson;
    static std::string profilesJson;

    static void initDatasetJsonDescriptors();
};
#endif /* datasetJson_hpp */

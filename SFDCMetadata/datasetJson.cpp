//
//  datasetJson.cpp
//  SFDCMetadata
//
//  Created by Vincent Brenet on 30/12/2021.
//  Copyright © 2021 Vincent Brenet. All rights reserved.
//

#include "datasetJson.hpp"
#include <sstream>

std::string datasetJson::usersJson {};
std::string datasetJson::permissionSetsJson {};
std::string datasetJson::profilesJson {};

//
//
//
void datasetJson::addTextField(std::stringstream& s, const std::string fieldname) {
    
    s
    << "{"
    << "\"fullyQualifiedName\" : \"" << fieldname << "\","
    << "\"label\" : \"" << fieldname << "\","
    << "\"type\" : \"Text\","
    << "\"name\" : \"" << fieldname << "\""
    << "},"
    ;
}
//
//
//
void datasetJson::initDatasetJsonDescriptors() {
    initUsersJson();
    initPermissionSetsJson();
    initProfilesJson();
}
//
//
//
void datasetJson::initUsersJson() {
    std::stringstream json;
        
    json
    << "{"
    <<"\"fileFormat\": {"
    <<"\"charsetName\": \"UTF-8\","
    <<"\"fieldsDelimitedBy\": \",\","
    <<"\"fieldsEnclosedBy\": \"\\\"\", "
    <<"\"numberOfLinesToIgnore\": 1"
    <<"},"
    << "\"objects\" : ["
    << "{"
    << "\"connector\" : \"permscan\","
    << "\"fullyQualifiedName\" : \"permscan_users\","
    << "\"label\" : \"permscan users\","
    << "\"name\" : \"permscan_users\","
    << "\"fields\" : ["

    << "{"
    << "\"fullyQualifiedName\" : \"Date\","
    << "\"label\" : \"Date\","
    << "\"type\" : \"Date\","
    << "\"format\" : \"dd/MM/yyyy\","
    << "\"name\" : \"Date\""
    << "},"
    ;
    
    //Date,Id,Username,FirstName,LastName,Profile,License,TotalPermissionSetLicences,TotalObjectNumber,StandardObjectNumber,PackagedObjectNumber,CustomObjectNumber,MaxCustomObjects,IsCompliant

    addTextField(json, "Id");
    addTextField(json, "Username");
    addTextField(json, "FirstName");
    addTextField(json, "LastName");
    addTextField(json, "Profile");
    addTextField(json, "License");

    
    json
    << "{"
    << "\"fullyQualifiedName\" : \"TotalPermissionSetLicences\","
    << "\"label\" : \"TotalPermissionSetLicences\","
    << "\"name\" : \"TotalPermissionSetLicences\","
    << "\"type\" : \"Numeric\","
    << "\"precision\" : 18,"
    << "\"scale\" : 0,"
    << "\"defaultValue\" : \"0\""
    << "},"

    << "{"
    << "\"fullyQualifiedName\" : \"TotalObjectNumber\","
    << "\"label\" : \"TotalObjectNumber\","
    << "\"name\" : \"TotalObjectNumber\","
    << "\"type\" : \"Numeric\","
    << "\"precision\" : 18,"
    << "\"scale\" : 0,"
    << "\"defaultValue\" : \"0\""
    << "},"
        
    << "{"
    << "\"fullyQualifiedName\" : \"StandardObjectNumber\","
    << "\"label\" : \"StandardObjectNumber\","
    << "\"name\" : \"StandardObjectNumber\","
    << "\"type\" : \"Numeric\","
    << "\"precision\" : 18,"
    << "\"scale\" : 0,"
    << "\"defaultValue\" : \"0\""
    << "},"
        
    << "{"
    << "\"fullyQualifiedName\" : \"PackagedObjectNumber\","
    << "\"label\" : \"PackagedObjectNumber\","
    << "\"name\" : \"PackagedObjectNumber\","
    << "\"type\" : \"Numeric\","
    << "\"precision\" : 18,"
    << "\"scale\" : 0,"
    << "\"defaultValue\" : \"0\""
    << "},"
        
    << "{"
    << "\"fullyQualifiedName\" : \"CustomObjectNumber\","
    << "\"label\" : \"CustomObjectNumber\","
    << "\"name\" : \"CustomObjectNumber\","
    << "\"type\" : \"Numeric\","
    << "\"precision\" : 18,"
    << "\"scale\" : 0,"
    << "\"defaultValue\" : \"0\""
    << "},"
        
    << "{"
    << "\"fullyQualifiedName\" : \"MaxCustomObjects\","
    << "\"label\" : \"MaxCustomObjects\","
    << "\"name\" : \"MaxCustomObjects\","
    << "\"type\" : \"Numeric\","
    << "\"precision\" : 18,"
    << "\"scale\" : 0,"
    << "\"defaultValue\" : \"0\""
    << "},"
        
    << "{"
    << "\"fullyQualifiedName\" : \"IsCompliant\","
    << "\"label\" : \"IsCompliant\","
    << "\"type\" : \"Text\","
    << "\"name\" : \"IsCompliant\""
    << "}"
    
    << "]"
    << "} ]"
    << "}"
    ;
    
    usersJson = json.str();

}
//
//
//
void datasetJson::initPermissionSetsJson() {
    std::stringstream json;
        
    json
    << "{"
    <<"\"fileFormat\": {"
    <<"\"charsetName\": \"UTF-8\","
    <<"\"fieldsDelimitedBy\": \",\","
    <<"\"fieldsEnclosedBy\": \"\\\"\", "
    <<"\"numberOfLinesToIgnore\": 1"
    <<"},"
    << "\"objects\" : ["
    << "{"
    << "\"connector\" : \"permscan\","
    << "\"fullyQualifiedName\" : \"permscan_permissionsets\","
    << "\"label\" : \"permscan permissionsets\","
    << "\"name\" : \"permscan_permissionsets\","
    << "\"fields\" : ["

    << "{"
    << "\"fullyQualifiedName\" : \"Date\","
    << "\"label\" : \"Date\","
    << "\"type\" : \"Date\","
    << "\"format\" : \"dd/MM/yyyy\","
    << "\"name\" : \"Date\""
    << "},"
    ;
    
    //Date,Id,Name,TotalObjectNumber,StandardObjectNumber,PackagedObjectNumber,CustomObjectNumber
    
    addTextField(json, "Id");
    addTextField(json, "Name");
    
    json
    << "{"
    << "\"fullyQualifiedName\" : \"TotalObjectNumber\","
    << "\"label\" : \"TotalObjectNumber\","
    << "\"name\" : \"TotalObjectNumber\","
    << "\"type\" : \"Numeric\","
    << "\"precision\" : 18,"
    << "\"scale\" : 0,"
    << "\"defaultValue\" : \"0\""
    << "},"

    << "{"
    << "\"fullyQualifiedName\" : \"StandardObjectNumber\","
    << "\"label\" : \"StandardObjectNumber\","
    << "\"name\" : \"StandardObjectNumber\","
    << "\"type\" : \"Numeric\","
    << "\"precision\" : 18,"
    << "\"scale\" : 0,"
    << "\"defaultValue\" : \"0\""
    << "},"
        
    << "{"
    << "\"fullyQualifiedName\" : \"PackagedObjectNumber\","
    << "\"label\" : \"PackagedObjectNumber\","
    << "\"name\" : \"PackagedObjectNumber\","
    << "\"type\" : \"Numeric\","
    << "\"precision\" : 18,"
    << "\"scale\" : 0,"
    << "\"defaultValue\" : \"0\""
    << "},"
        
    << "{"
    << "\"fullyQualifiedName\" : \"CustomObjectNumber\","
    << "\"label\" : \"CustomObjectNumber\","
    << "\"name\" : \"CustomObjectNumber\","
    << "\"type\" : \"Numeric\","
    << "\"precision\" : 18,"
    << "\"scale\" : 0,"
    << "\"defaultValue\" : \"0\""
    << "}"
        
    
    << "]"
    << "} ]"
    << "}"
    ;
    
    permissionSetsJson = json.str();

}
//
//
//
void datasetJson::initProfilesJson() {
    std::stringstream json;
        
    json
    << "{"
    <<"\"fileFormat\": {"
    <<"\"charsetName\": \"UTF-8\","
    <<"\"fieldsDelimitedBy\": \",\","
    <<"\"fieldsEnclosedBy\": \"\\\"\", "
    <<"\"numberOfLinesToIgnore\": 1"
    <<"},"
    << "\"objects\" : ["
    << "{"
    << "\"connector\" : \"permscan\","
    << "\"fullyQualifiedName\" : \"permscan_profiles\","
    << "\"label\" : \"permscan profiles\","
    << "\"name\" : \"permscan_profiles\","
    << "\"fields\" : ["

    << "{"
    << "\"fullyQualifiedName\" : \"Date\","
    << "\"label\" : \"Date\","
    << "\"type\" : \"Date\","
    << "\"format\" : \"dd/MM/yyyy\","
    << "\"name\" : \"Date\""
    << "},"
    ;
    
    //Date,Id,Name,TotalObjectNumber,StandardObjectNumber,PackagedObjectNumber,CustomObjectNumber
    
    addTextField(json, "Id");
    addTextField(json, "Name");
    
    json
    << "{"
    << "\"fullyQualifiedName\" : \"TotalObjectNumber\","
    << "\"label\" : \"TotalObjectNumber\","
    << "\"name\" : \"TotalObjectNumber\","
    << "\"type\" : \"Numeric\","
    << "\"precision\" : 18,"
    << "\"scale\" : 0,"
    << "\"defaultValue\" : \"0\""
    << "},"

    << "{"
    << "\"fullyQualifiedName\" : \"StandardObjectNumber\","
    << "\"label\" : \"StandardObjectNumber\","
    << "\"name\" : \"StandardObjectNumber\","
    << "\"type\" : \"Numeric\","
    << "\"precision\" : 18,"
    << "\"scale\" : 0,"
    << "\"defaultValue\" : \"0\""
    << "},"
        
    << "{"
    << "\"fullyQualifiedName\" : \"PackagedObjectNumber\","
    << "\"label\" : \"PackagedObjectNumber\","
    << "\"name\" : \"PackagedObjectNumber\","
    << "\"type\" : \"Numeric\","
    << "\"precision\" : 18,"
    << "\"scale\" : 0,"
    << "\"defaultValue\" : \"0\""
    << "},"
        
    << "{"
    << "\"fullyQualifiedName\" : \"CustomObjectNumber\","
    << "\"label\" : \"CustomObjectNumber\","
    << "\"name\" : \"CustomObjectNumber\","
    << "\"type\" : \"Numeric\","
    << "\"precision\" : 18,"
    << "\"scale\" : 0,"
    << "\"defaultValue\" : \"0\""
    << "}"
        
    
    << "]"
    << "} ]"
    << "}"
    ;
    
    profilesJson = json.str();

}

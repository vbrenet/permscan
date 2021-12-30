//
//  datasetJson.cpp
//  SFDCMetadata
//
//  Created by Vincent Brenet on 30/12/2021.
//  Copyright © 2021 Vincent Brenet. All rights reserved.
//

#include "datasetJson.hpp"
#include <sstream>

std::string datasetJson::fieldBookJson {};
std::string datasetJson::globalUsageJson {};
std::string datasetJson::usageByRTJson {};
std::string datasetJson::picklistUsageJson {};
std::string datasetJson::picklistUsageByRTJson {};

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
    initFieldBookJson();
    initGlobalUsageJson();
    initUsageByRTJson();
    initPicklistUsageJson();
    initPicklistUsageByRTJson();
}
//
//
//
void datasetJson::initPicklistUsageJson() {
    std::stringstream json;
    
    // Date,sObject,PicklistName,FromPackage,DefaultValue,UsageBucket,PicklistLabel,PicklistValue,Usage,PercentUsage
    
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
    << "\"connector\" : \"curltry\","
    << "\"fullyQualifiedName\" : \"Picklist_Usage\","
    << "\"label\" : \"Picklist Usage\","
    << "\"name\" : \"Picklist_Usage\","
    << "\"fields\" : ["

    << "{"
    << "\"fullyQualifiedName\" : \"Date\","
    << "\"label\" : \"Date\","
    << "\"type\" : \"Date\","
    << "\"format\" : \"dd/MM/yyyy\","
    << "\"name\" : \"Date\""
    << "},"
    ;
    
    addTextField(json, "sObject");
    addTextField(json, "PicklistName");
    addTextField(json, "FromPackage");
    addTextField(json, "DefaultValue");
    addTextField(json, "UsageBucket");
    addTextField(json, "PicklistLabel");
    addTextField(json, "PicklistValue");

    
    json
    << "{"
    << "\"fullyQualifiedName\" : \"Usage\","
    << "\"label\" : \"Usage\","
    << "\"name\" : \"Usage\","
    << "\"type\" : \"Numeric\","
    << "\"precision\" : 18,"
    << "\"scale\" : 0,"
    << "\"defaultValue\" : \"0\""
    << "},"

    << "{"
    << "\"fullyQualifiedName\" : \"PercentUsage\","
    << "\"label\" : \"PercentUsage\","
    << "\"name\" : \"PercentUsage\","
    << "\"type\" : \"Numeric\","
    << "\"precision\" : 5,"
    << "\"scale\" : 2,"
    << "\"defaultValue\" : \"0.0\""
    << "}"
        
    << "]"
    << "} ]"
    << "}"
    ;
    
    picklistUsageJson = json.str();

}
//
//
//
void datasetJson::initPicklistUsageByRTJson() {
    std::stringstream json;
    
    // Date,sObject,RecordTypeId,RecordType,PicklistName,FromPackage,PercentUsage,UsageBucket,DefaultValue,PicklistLabel,PicklistValue,Usage
    
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
    << "\"connector\" : \"curltry\","
    << "\"fullyQualifiedName\" : \"Picklist_Matrix_Usage\","
    << "\"label\" : \"Picklist Matrix Usage\","
    << "\"name\" : \"Picklist_Matrix_Usage\","
    << "\"fields\" : ["

    << "{"
    << "\"fullyQualifiedName\" : \"Date\","
    << "\"label\" : \"Date\","
    << "\"type\" : \"Date\","
    << "\"format\" : \"dd/MM/yyyy\","
    << "\"name\" : \"Date\""
    << "},"
    ;
    
    addTextField(json, "sObject");
    addTextField(json, "RecordTypeId");
    addTextField(json, "RecordType");
    addTextField(json, "PicklistName");
    addTextField(json, "FromPackage");

    json
    << "{"
    << "\"fullyQualifiedName\" : \"PercentUsage\","
    << "\"label\" : \"PercentUsage\","
    << "\"name\" : \"PercentUsage\","
    << "\"type\" : \"Numeric\","
    << "\"precision\" : 5,"
    << "\"scale\" : 2,"
    << "\"defaultValue\" : \"0.0\""
    << "},"
    ;

    addTextField(json, "UsageBucket");
    addTextField(json, "DefaultValue");
    addTextField(json, "PicklistLabel");
    addTextField(json, "PicklistValue");

    json
    << "{"
    << "\"fullyQualifiedName\" : \"Usage\","
    << "\"label\" : \"Usage\","
    << "\"name\" : \"Usage\","
    << "\"type\" : \"Numeric\","
    << "\"precision\" : 18,"
    << "\"scale\" : 0,"
    << "\"defaultValue\" : \"0\""
    << "}"
        
    << "]"
    << "} ]"
    << "}"
    ;
    
    picklistUsageByRTJson = json.str();

}
//
//
//
void datasetJson::initUsageByRTJson() {
    std::stringstream json;
    
    //Date,sObject,RecordTypeId,RecordType,FieldName,FromPackage,FieldType,DefaultValue,FieldUsage,PercentRecordTypeUsage,UsageBucket
    
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
    << "\"connector\" : \"curltry\","
    << "\"fullyQualifiedName\" : \"Field_Usage_By_RT\","
    << "\"label\" : \"Field Usage By RT\","
    << "\"name\" : \"Field_Usage_By_RT\","
    << "\"fields\" : ["

    << "{"
    << "\"fullyQualifiedName\" : \"Date\","
    << "\"label\" : \"Date\","
    << "\"type\" : \"Date\","
    << "\"format\" : \"dd/MM/yyyy\","
    << "\"name\" : \"Date\""
    << "},"
    ;
    
    addTextField(json, "sObject");
    addTextField(json, "RecordTypeId");
    addTextField(json, "RecordType");
    addTextField(json, "FieldName");
    addTextField(json, "FromPackage");
    addTextField(json, "FieldType");
    addTextField(json, "DefaultValue");

    json
    << "{"
    << "\"fullyQualifiedName\" : \"FieldUsage\","
    << "\"label\" : \"FieldUsage\","
    << "\"name\" : \"FieldUsage\","
    << "\"type\" : \"Numeric\","
    << "\"precision\" : 18,"
    << "\"scale\" : 0,"
    << "\"defaultValue\" : \"0\""
    << "},"
    ;
    
    json
    << "{"
    << "\"fullyQualifiedName\" : \"PercentRecordTypeUsage\","
    << "\"label\" : \"PercentRecordTypeUsage\","
    << "\"name\" : \"PercentRecordTypeUsage\","
    << "\"type\" : \"Numeric\","
    << "\"precision\" : 5,"
    << "\"scale\" : 2,"
    << "\"defaultValue\" : \"0.0\""
    << "},"
    ;
    
    json
    << "{"
    << "\"fullyQualifiedName\" : \"UsageBucket\","
    << "\"label\" : \"UsageBucket\","
    << "\"type\" : \"Text\","
    << "\"name\" : \"UsageBucket\""
    << "}"
    
    << "]"
    << "} ]"
    << "}"
    ;
    
    
    usageByRTJson = json.str();

}
//
//
//
void datasetJson::initFieldBookJson() {
//Date,sObject,sObjectLabel,fieldAPIName,BusinessOwnerId,BusinessStatus,ComplianceGroup,DataType,ExtraTypeInfo,IsApiFilterable,IsApiGroupable,IsApiSortable,IsCalculated,IsCompactLayoutable,IsCompound,IsFieldHistoryTracked,IsHighScaleNumber,IsHtmlFormatted,IsIndexed,IsListFilterable,IsListSortable,IsListVisible,IsNameField,Label,LastModifiedById,LastModifiedDate,Length,MasterLabel,NamespacePrefix,Package,Precision,Scale,SecurityClassification

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
    << "\"connector\" : \"curltry\","
    << "\"fullyQualifiedName\" : \"fieldBook\","
    << "\"label\" : \"FieldBook\","
    << "\"name\" : \"fieldBook\","
    << "\"fields\" : ["

    << "{"
    << "\"fullyQualifiedName\" : \"Date\","
    << "\"label\" : \"Date\","
    << "\"type\" : \"Date\","
    << "\"format\" : \"dd/MM/yyyy\","
    << "\"name\" : \"Date\""
    << "},"
    ;
    
    addTextField(json, "sObject");
    addTextField(json, "sObjectLabel");
    addTextField(json, "fieldAPIName");
    addTextField(json, "BusinessOwnerId");
    addTextField(json, "BusinessStatus");
    addTextField(json, "ComplianceGroup");
    addTextField(json, "DataType");
    addTextField(json, "ExtraTypeInfo");
    addTextField(json, "IsApiFilterable");
    addTextField(json, "IsApiGroupable");
    addTextField(json, "IsApiSortable");
    addTextField(json, "IsCalculated");
    addTextField(json, "IsCompactLayoutable");
    addTextField(json, "IsCompound");
    addTextField(json, "IsFieldHistoryTracked");
    addTextField(json, "IsHighScaleNumber");
    addTextField(json, "IsHtmlFormatted");
    addTextField(json, "IsIndexed");
    addTextField(json, "IsListFilterable");
    addTextField(json, "IsListSortable");
    addTextField(json, "IsListVisible");
    addTextField(json, "IsNameField");
    addTextField(json, "Label");
    addTextField(json, "LastModifiedById");

    json
    << "{"
    << "\"fullyQualifiedName\" : \"LastModifiedDate\","
    << "\"label\" : \"LastModifiedDate\","
    << "\"type\" : \"Date\","
    << "\"format\" : \"yyyy-MM-dd\","
    << "\"name\" : \"LastModifiedDate\""
    << "},"
    
    << "{"
    << "\"fullyQualifiedName\" : \"Length\","
    << "\"label\" : \"Length\","
    << "\"name\" : \"Length\","
    << "\"type\" : \"Numeric\","
    << "\"precision\" : 18,"
    << "\"scale\" : 0,"
    << "\"defaultValue\" : \"0\""
    << "},"
    ;
    
    addTextField(json, "MasterLabel");
    addTextField(json, "NamespacePrefix");
    addTextField(json, "Package");

    json
    << "{"
    << "\"fullyQualifiedName\" : \"Precision\","
    << "\"label\" : \"Precision\","
    << "\"name\" : \"Precision\","
    << "\"type\" : \"Numeric\","
    << "\"precision\" : 2,"
    << "\"scale\" : 0,"
    << "\"defaultValue\" : \"0\""
    << "},"
    ;
    
    addTextField(json, "Scale");

    json
    << "{"
    << "\"fullyQualifiedName\" : \"SecurityClassification\","
    << "\"label\" : \"SecurityClassification\","
    << "\"type\" : \"Text\","
    << "\"name\" : \"SecurityClassification\""
    << "}"
    
    << "]"
    << "} ]"
    << "}"
    ;

    fieldBookJson = json.str();
}
//
//
//
void datasetJson::initGlobalUsageJson() {
    // Date,sObject,FieldName,FromPackage,FieldType,DefaultValue,FieldUsage,PercentUsage,UsageBucket
    
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
    << "\"connector\" : \"curltry\","
    << "\"fullyQualifiedName\" : \"Field_Global_Usage\","
    << "\"label\" : \"Field Global Usage\","
    << "\"name\" : \"Field_Global_Usage\","
    << "\"fields\" : ["

    << "{"
    << "\"fullyQualifiedName\" : \"Date\","
    << "\"label\" : \"Date\","
    << "\"type\" : \"Date\","
    << "\"format\" : \"dd/MM/yyyy\","
    << "\"name\" : \"Date\""
    << "},"
    
    << "{"
    << "\"fullyQualifiedName\" : \"sObject\","
    << "\"label\" : \"sObject\","
    << "\"type\" : \"Text\","
    << "\"name\" : \"sObject\""
    << "},"

    << "{"
    << "\"fullyQualifiedName\" : \"FieldName\","
    << "\"label\" : \"FieldName\","
    << "\"type\" : \"Text\","
    << "\"name\" : \"FieldName\""
    << "},"

    << "{"
    << "\"fullyQualifiedName\" : \"FromPackage\","
    << "\"label\" : \"FromPackage\","
    << "\"type\" : \"Text\","
    << "\"name\" : \"FromPackage\""
    << "},"

    << "{"
    << "\"fullyQualifiedName\" : \"FieldType\","
    << "\"label\" : \"FieldType\","
    << "\"type\" : \"Text\","
    << "\"name\" : \"FieldType\""
    << "},"
    
    << "{"
    << "\"fullyQualifiedName\" : \"DefaultValue\","
    << "\"label\" : \"DefaultValue\","
    << "\"type\" : \"Text\","
    << "\"name\" : \"DefaultValue\""
    << "},"
    
    << "{"
    << "\"fullyQualifiedName\" : \"FieldUsage\","
    << "\"label\" : \"FieldUsage\","
    << "\"name\" : \"FieldUsage\","
    << "\"type\" : \"Numeric\","
    << "\"precision\" : 18,"
    << "\"scale\" : 0,"
    << "\"defaultValue\" : \"0\""
    << "},"
    
    << "{"
    << "\"fullyQualifiedName\" : \"PercentUsage\","
    << "\"label\" : \"PercentUsage\","
    << "\"name\" : \"PercentUsage\","
    << "\"type\" : \"Numeric\","
    << "\"precision\" : 4,"
    << "\"scale\" : 1,"
    << "\"defaultValue\" : \"0.0\""
    << "},"
    
    << "{"
    << "\"fullyQualifiedName\" : \"UsageBucket\","
    << "\"label\" : \"UsageBucket\","
    << "\"type\" : \"Text\","
    << "\"name\" : \"UsageBucket\""
    << "}"

    << "]"
    << "} ]"
    << "}"
    ;
    
    globalUsageJson = json.str();
}

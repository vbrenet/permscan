//
//  Orchestrator.cpp
//  SFDCMetadata
//
//  Created by Vincent Brenet on 21/12/2021.
//  Copyright © 2021 Vincent Brenet. All rights reserved.
//

#include "Orchestrator.hpp"
#include <iostream>
#include "metadataSession.hpp"
#include "rapidxml/rapidxml.hpp"
#include "config.hpp"
#include "SalesforceSession.hpp"
#include "utils.hpp"
#include "globals.hpp"
//
//
//
void orchestrator::addObjectsToPermissionSet(std::string id, const std::string& xmlBuffer) {
    
    auto it = permissionSetMap.find(id);
    
    if (it == permissionSetMap.end()) {
        std::cerr << "addObjectsToPermissionSet id not found in Map " << id << std::endl;
        return;
    }
    
    using namespace rapidxml;
    xml_document<> document;
    document.parse<0>((char *)xmlBuffer.c_str());
    xml_node<> *node = document.first_node("soapenv:Envelope");
    node = node->first_node("soapenv:Body");
    node = node->first_node("readMetadataResponse");
    node = node->first_node("result");
    node = node->first_node("records");
    
    xml_node<> * servernode = node->first_node("objectPermissions");

   for (xml_node<> *child = servernode; child; child = child->next_sibling())
   {
       xml_node<> * objectnode = child->first_node("object");
       if (objectnode) {
           if (globals::verbose)
               std::cout <<  "object: " << objectnode->value() << std::endl;
           it->second.insertObject(objectnode->value());
       }
   }
}
//
//
//
void orchestrator::initializePermissionsSet(const std::string& xmlBuffer) {
    using namespace rapidxml;
    xml_document<> document;
    document.parse<0>((char *)xmlBuffer.c_str());
    xml_node<> *node = document.first_node("QueryResult");
    
    xml_node<> * servernode = node->first_node("records");

   for (xml_node<> *child = servernode; child; child = child->next_sibling())
   {
       std::string id {};
       std::string name {};
       xml_node<> * objectnode = child->first_node("Id");
       if (objectnode) {
           id = objectnode->value();
           //std::cout <<  "Id: " << objectnode->value() << std::endl;
       }
       xml_node<> * namenode = child->first_node("Name");
       if (namenode) {
           name = namenode->value();
           //std::cout <<  "Name: " << namenode->value() << std::endl;
       }
       if (namenode && objectnode) {
           permissionSetMap.insert ( std::pair<std::string,permissionSet>(id, {id,name}) );
       }
   }
}
//
//
//
bool orchestrator::run() {
    
    //  open REST session
    sessionCredentials credentials {
            config::isSandbox(),
            config::getDomain(),
            config::getClientId(),
            config::getClientSecret(),
            config::getUsername(),
            config::getPassword(),
            config::getSecurityToken()
        };

    if (!SalesforceSession::openSession(credentials))
        return false ;
    
    //  read permissions set
    std::cout << "Reading permissions set ...";
    
    std::string readBuffer;

    restQuery("?q=SELECT+ID+,+Name+FROM+PermissionSet", readBuffer);

    if (globals::verbose) {
        std::cout << "PermissionSet query: " << std::endl;
        std::cout << readBuffer << std::endl;
    }
    
    initializePermissionsSet(readBuffer);
    
    // open metadatasession
    //
    if (!metadataSession::openMetadataSession(config::isSandbox(), config::getUsername(), config::getPassword(), config::getApiVersion(), config::getSecurityToken())) {
        std::cerr << "openMetadataSession error" << std::endl;
        return false;
    }
        
    // iterate on permissionSet map
    std::cout << std::endl << "Analyzing permissions set metadata...";
    
    for (auto it : permissionSetMap) {
        std::string result;
        std::string thebody = "<soapenv:Body>";
        thebody +="<met:readMetadata>";
        thebody +="<met:type>PermissionSet</met:type>";
        thebody +="<met:fullName>";
        thebody += it.second.getName();
        thebody +="</met:fullName>";
        thebody +="</met:readMetadata>";
        thebody +="</soapenv:Body>";
        if (!metadataSession::call("readMetadata", thebody, result))
            std::cerr << "call error" << std::endl;
        
        addObjectsToPermissionSet(it.first, result);
    }

    std::cout << std::endl;
    
    // iterate on permissionSet map and print objects
    for (auto it : permissionSetMap) {
        std::cout << "Permission set name : " << it.second.getName() << std::endl;
        auto theset = it.second.getobjects();
        for (auto itset = theset.begin(); itset != theset.end(); ++itset)
            std::cout << *itset << " ";
        std::cout << std::endl;
        
    }
    
    return true;
}

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
void orchestrator::updateUsersWithPermissionSetId (std::string psid, const std::string& xmlBuffer) {
    using namespace rapidxml;
    xml_document<> document;
    document.parse<0>((char *)xmlBuffer.c_str());
    xml_node<> *node = document.first_node("QueryResult");
    
    xml_node<> * sizenode = node->first_node("totalSize");
    int totalsize = std::stoi( sizenode->value() );
    std::cout << "updateUsersWithPermissionSetId size = " << totalsize << std::endl;
    
    xml_node<> * servernode = node->first_node("records");

   for (xml_node<> *child = servernode; child; child = child->next_sibling())
   {
       std::string assigneeid {};
    
       xml_node<> * objectnode = child->first_node("AssigneeId");
       if (objectnode) {
           assigneeid = objectnode->value();
           addPermissionSetObjectsToUser(psid,assigneeid);
       }
   }
}
//
//
//
void orchestrator::addPermissionSetObjectsToUser(std::string psid, std::string assigneeid) {
    auto theUser = userMap.find(assigneeid);
    auto thePS = permissionSetMap.find(psid);
    
    if (theUser == userMap.end()) {
        if (globals::verbose)
            std::cout << "addPermissionSetObjectsToUser: user not found in user map, may be inactive, id =" << assigneeid << std::endl;
        return;
    }

    if (thePS == permissionSetMap.end()) {
        std::cerr << "addPermissionSetObjectsToUser: permission set not found in permission set map, id =" << psid << std::endl;
        return;
    }

    auto theset = thePS->second.getobjects();
    for (auto itset = theset.begin(); itset != theset.end(); ++itset)
        theUser->second.insertPermittedObject(*itset);
}
//
//
//
void orchestrator::addObjectsToPermissionSet(std::string id, const std::string& xmlBuffer) {
    
    auto it = permissionSetMap.find(id);
    
    if (it == permissionSetMap.end()) {
        std::cerr << "addObjectsToPermissionSet: id not found in Map " << id << std::endl;
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
bool orchestrator::initializeUsers(const std::string& xmlBuffer, std::string& nextUrl) {
    bool moretoread {false};
    using namespace rapidxml;
    xml_document<> document;
    document.parse<0>((char *)xmlBuffer.c_str());
    xml_node<> *node = document.first_node("QueryResult");
    
    xml_node<> * sizenode = node->first_node("totalSize");
    std::cout << "Total number of active users from query : " << sizenode->value() << std::endl;
    
    xml_node<> * nexturl = node->first_node("nextRecordsUrl");
    if (nexturl) {
        std::cout << "nextRecordsUrl : " << nexturl->value() << std::endl;
        std::string url = nexturl->value();
        
        // extract url
        size_t beginindex = url.find("query/");
            
        nextUrl = url.substr(beginindex+6);
        std::cout << "url " << nextUrl << std::endl;
        
        moretoread = true;
    }
    
    xml_node<> * servernode = node->first_node("records");

    int nbinsert {0};
    for (xml_node<> *child = servernode; child; child = child->next_sibling())
    {
        std::string id {};
        std::string username {};
        std::string firstname {};
        std::string lastname {};

        xml_node<> * objectnode = child->first_node("Id");
        if (objectnode) {
            id = objectnode->value();
        }
        
        xml_node<> * usernamenode = child->first_node("Username");
        if (usernamenode) {
            username = usernamenode->value();
        }

        xml_node<> * firstnamenode = child->first_node("FirstName");
        if (firstnamenode) {
           firstname = firstnamenode->value();
        }

        xml_node<> * lastnamenode = child->first_node("LastName");
        if (lastnamenode) {
           lastname = lastnamenode->value();
        }
        
        if (objectnode) {
           if (userMap.find(id) != userMap.end()) {
               std::cerr << "id already exist in user map " << child->value() << std::endl;
               break;
           }
           userMap.insert ( std::pair<std::string,salesforceUser>(id, {id, firstname, lastname, username}) );
           nbinsert++;
        }
        
    }
    std::cout << "number insertions : " << nbinsert << std::endl;
    
    return  moretoread;
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
    std::cout << std::endl << "Analyzing permissions set metadata..." << std::endl;

    int i {0};
    for (auto it : permissionSetMap) {
        i++;
        std::cout << "\r" << i << " of " << permissionSetMap.size() << std::flush;
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
    
    // read users
    std::cout << "Reading active users ..." << std::endl;
    restQuery("?q=SELECT+ID+,+Username+,+Firstname+,+Lastname+FROM+User+where+isactive+=+true", readBuffer);
    
    std::string nexturl {};
    
    bool nextRecordsToRead = initializeUsers(readBuffer, nexturl);
    
    while (nextRecordsToRead) {
        if (restQuery(nexturl, readBuffer))
            nextRecordsToRead = initializeUsers(readBuffer, nexturl);
        else
            break;
    }
    
    std::cout << "Nb active users : " << userMap.size() << std::endl;
    
    // read permission set assignments
    std::cout << "Reading permission set assignments ..." << std::endl;
    for (auto it : permissionSetMap) {
        std::cout << it.second.getName() << " ..." << std::endl;
        restQuery("?q=SELECT+AssigneeId+FROM+PermissionSetAssignment+where+PermissionSetId+=+'" + it.first + "'", readBuffer);
        updateUsersWithPermissionSetId (it.first, readBuffer);
    }
    
    // iterate on permissionSet map and print objects
    for (auto it : permissionSetMap) {
        std::cout << "Permission set name : " << it.second.getName() << std::endl;
        auto theset = it.second.getobjects();
        for (auto itset = theset.begin(); itset != theset.end(); ++itset)
            std::cout << *itset << " ";
        std::cout << std::endl;
    }
    
    // iterate on user map and print objects
    for (auto it : userMap) {
        std::cout << "User : " << it.second.getUsername() << std::endl;
        auto theset = it.second.getPermittedObjects();
        for (auto itset = theset.begin(); itset != theset.end(); ++itset)
            std::cout << *itset << " ";
        std::cout << std::endl;
    }

    return true;
}

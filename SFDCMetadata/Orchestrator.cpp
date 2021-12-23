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
bool orchestrator::updateUsersWithPermissionSetId (std::string psid, const std::string& xmlBuffer, std::string& nextUrl) {
    bool moretoread {false};
    using namespace rapidxml;
    xml_document<> document;
    document.parse<0>((char *)xmlBuffer.c_str());
    xml_node<> *node = document.first_node("QueryResult");
    
    xml_node<> * nexturl = node->first_node("nextRecordsUrl");
    if (nexturl) {
        std::cout << "updateUsersWithPermissionSetId : nextRecordsUrl : " << nexturl->value() << std::endl;
        std::string url = nexturl->value();
        
        // extract url
        size_t beginindex = url.find("query/");
            
        nextUrl = url.substr(beginindex+6);
        
        moretoread = true;
    }
    
    //xml_node<> * sizenode = node->first_node("totalSize");
    //int totalsize = std::stoi( sizenode->value() );
    //std::cout << "updateUsersWithPermissionSetId size = " << totalsize << std::endl;
    
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
    
return  moretoread;
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
void orchestrator::addObjectsToProfile(std::string id, const std::string& xmlBuffer) {
    
    auto it = profileMap.find(id);
    
    if (it == profileMap.end()) {
        std::cerr << "addObjectsToProfile: id not found in Map " << id << std::endl;
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
}//
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
void orchestrator::initializeProfiles(const std::string& xmlBuffer) {
    using namespace rapidxml;
    xml_document<> document;
    document.parse<0>((char *)xmlBuffer.c_str());
    xml_node<> *node = document.first_node("QueryResult");
    
    xml_node<> * servernode = node->first_node("records");

   for (xml_node<> *child = servernode; child; child = child->next_sibling())
   {
       std::string id {};
       std::string name {};
       std::string licenceid {};
       xml_node<> * objectnode = child->first_node("Id");
       if (objectnode) {
           id = objectnode->value();
       }
       xml_node<> * namenode = child->first_node("Name");
       if (namenode) {
           name = namenode->value();
       }
       xml_node<> * licencenode = child->first_node("UserLicenceId");
       if (licencenode) {
           name = licencenode->value();
       }
       if (namenode && objectnode) {
           profileMap.insert ( std::pair<std::string,profile>(id, {id, name, licenceid}) );
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
        std::string theprofile {};

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
        
        xml_node<> * profilenode = child->first_node("ProfileId");
        if (profilenode) {
            theprofile = profilenode->value();
        }
        
        if (objectnode) {
           if (userMap.find(id) != userMap.end()) {
               std::cerr << "id already exist in user map " << child->value() << std::endl;
               break;
           }
           userMap.insert ( std::pair<std::string,salesforceUser>(id, {id, firstname, lastname, username, theprofile}) );
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
    
    // read profiles
    std::cout << std::endl << "Reading profiles ...";
    
    restQuery("?q=SELECT+ID+,+Name+,+UserLicenseId+FROM+Profile", readBuffer);

    if (globals::verbose) {
        std::cout << "PermissionSet query: " << std::endl;
        std::cout << readBuffer << std::endl;
    }
    
    initializeProfiles(readBuffer);
    std::cout << std::endl << profileMap.size() << " profiles inserted";
    
    // open metadatasession
    //
    if (!metadataSession::openMetadataSession(config::isSandbox(), config::getUsername(), config::getPassword(), config::getApiVersion(), config::getSecurityToken())) {
        std::cerr << "openMetadataSession error" << std::endl;
        return false;
    }
    
    // iterate on profiles map
    std::cout << std::endl << "Analyzing profiles metadata..." << std::endl;

    int j {0};
    for (auto it = profileMap.begin(); it != profileMap.end(); ++it) {
        j++;
        std::cout << "\r" << j << " of " << profileMap.size() << std::flush;
        std::string result;
        std::string thebody = "<soapenv:Body>";
        thebody +="<met:readMetadata>";
        thebody +="<met:type>Profile</met:type>";
        thebody +="<met:fullName>";
        thebody += it->second.getName();
        thebody +="</met:fullName>";
        thebody +="</met:readMetadata>";
        thebody +="</soapenv:Body>";
        if (!metadataSession::call("readMetadata", thebody, result))
            std::cerr << "call error" << std::endl;

        addObjectsToProfile(it->first, result);
    }

    std::cout << std::endl;
 
    // iterate on permissionSet map
    std::cout << std::endl << "Analyzing permissions set metadata..." << std::endl;

    int i {0};
    for (auto it = permissionSetMap.begin(); it != permissionSetMap.end(); ++it) {
        i++;
        std::cout << "\r" << i << " of " << permissionSetMap.size() << std::flush;
        std::string result;
        std::string thebody = "<soapenv:Body>";
        thebody +="<met:readMetadata>";
        thebody +="<met:type>PermissionSet</met:type>";
        thebody +="<met:fullName>";
        thebody += it->second.getName();
        thebody +="</met:fullName>";
        thebody +="</met:readMetadata>";
        thebody +="</soapenv:Body>";
        if (!metadataSession::call("readMetadata", thebody, result))
            std::cerr << "call error" << std::endl;
        
        addObjectsToPermissionSet(it->first, result);
    }

    std::cout << std::endl;
 
    // read users
    std::cout << "Reading active users ..." << std::endl;
    restQuery("?q=SELECT+ID+,+Username+,+Firstname+,+Lastname+,+ProfileId+FROM+User+where+isactive+=+true", readBuffer);
    
    std::string nexturl {};
    
    bool nextRecordsToRead = initializeUsers(readBuffer, nexturl);
    
    while (nextRecordsToRead) {
        if (restQuery(nexturl, readBuffer))
            nextRecordsToRead = initializeUsers(readBuffer, nexturl);
        else {
            std::cerr << "User query error" << std::endl;
            return false;
        }
    }
    
    std::cout << "Nb active users : " << userMap.size() << std::endl;
    
    // add objects from profiles to users
    std::cout << "Assigning objects to users from profiles ..." << std::endl;
    int k {0};
    for (auto it = userMap.begin(); it != userMap.end(); ++it) {
        std::string theprofile = it->second.getProfile();
        auto prfit = profileMap.find(theprofile);
        if (prfit != profileMap.end()) {
            auto theset = prfit->second.getobjects();
            for (auto itset = theset.begin(); itset != theset.end(); ++itset) {
                //std::cout << "to insert " << *itset << std::endl;
                it->second.insertPermittedObject(*itset);
                k++;
            }
        }
        else {
            std::cerr << "Profile " << theprofile << " not found in the map for user " << it->second.getFullName() << " userid: " << it->second.getId() << std::endl;
        }
        //std::cout << "User : " << it->second.getFullName() << " set size " << it->second.getPermittedObjects().size() << std::endl;
    }
    //std::cout << "Total number of objects added to users : " << k << std::endl;
    
    // read permission set assignments
    std::cout << "Reading permission set assignments ..." << std::endl;
    
    for (auto it = permissionSetMap.begin(); it != permissionSetMap.end(); ++it) {
        std::cout << it->second.getName() << " ..." << std::endl;
        if (!restQuery("?q=SELECT+AssigneeId+FROM+PermissionSetAssignment+where+PermissionSetId+=+'" + it->first + "'", readBuffer)) {
            std::cerr << "PermissionSetAssignment query error" << std::endl;
            return false;
        }
        
        nextRecordsToRead = updateUsersWithPermissionSetId (it->first, readBuffer, nexturl);
        while (nextRecordsToRead) {
            if (restQuery(nexturl, readBuffer))
                nextRecordsToRead = updateUsersWithPermissionSetId (it->first, readBuffer, nexturl);
            else {
                std::cerr << "PermissionSetAssignment query error" << std::endl;
                return false;
            }
        }
    }
    
    // if verbose iterate on profiles and permissionSet map and print objects
    if (globals::verbose) {
        for (auto it : permissionSetMap) {
            std::cout << "Permission set name : " << it.second.getName() << std::endl;
            auto theset = it.second.getobjects();
            for (auto itset = theset.begin(); itset != theset.end(); ++itset)
                std::cout << *itset << " ";
            std::cout << std::endl;
        }
        for (auto it : profileMap) {
            std::cout << "Profile name : " << it.second.getName() << std::endl;
            auto theset = it.second.getobjects();
            for (auto itset = theset.begin(); itset != theset.end(); ++itset)
                std::cout << *itset << " ";
            std::cout << std::endl;
        }

    }
   
    // iterate on user map and print objects

    for (auto it = userMap.begin(); it != userMap.end(); ++it) {
        std::cout << "User : " << it->second.getUsername() << std::endl;
        std::string theprofile = it->second.getProfile();
        auto prfit = profileMap.find(theprofile);
        if (prfit != profileMap.end()) {
            std::cout << "Profile : " << prfit->second.getName() << std::endl;
        }
        auto theset = it->second.getPermittedObjects();
        std::cout << "Set size " << it->second.getPermittedObjects().size() << std::endl;
        for (auto itset = theset.begin(); itset != theset.end(); ++itset)
            std::cout << *itset << " ";
        std::cout << std::endl;
    }

    return true;
}

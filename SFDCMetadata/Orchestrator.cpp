//
//  Orchestrator.cpp
//  SFDCMetadata
//
//  Created by Vincent Brenet on 21/12/2021.
//  Copyright © 2021 Vincent Brenet. All rights reserved.
//

#include "Orchestrator.hpp"
#include <iostream>
#include <fstream>
#include "metadataSession.hpp"
#include "rapidxml/rapidxml.hpp"
#include "config.hpp"
#include "SalesforceSession.hpp"
#include "utils.hpp"
#include "globals.hpp"
//
//
//
void orchestrator::outputcsv() {
    std::ofstream ofs {globals::workingDirectory + "/users.csv"};
    
    // header
    ofs << "Date,";
    ofs << "Id,";
    ofs << "Username,";
    ofs << "FirstName,";
    ofs << "LastName,";
    ofs << "Profile,";
    ofs << "License,";
    ofs << "TotalObjectNumber,";
    ofs << "StandardObjectNumber,";
    ofs << "PackagedObjectNumber,";
    ofs << "CustomObjectNumber,";
    ofs << "MaxCustomObjects,";
    ofs << "IsCompliant" << std::endl;
    
    ofs << std::boolalpha;
    
    for (auto it = userMap.begin(); it != userMap.end(); ++it){
        ofs << getDateString() << ",";
        ofs << "\"" << it->second.getId() << "\"" << ",";
        ofs << "\"" << it->second.getUsername() << "\"" << ",";
        ofs << "\"" << it->second.getFirstName() << "\"" << ",";
        ofs << "\"" << it->second.getLastName() << "\"" << ",";
        ofs << "\"" << it->second.getProfileName() << "\"" << ",";
        ofs << "\"" << it->second.getLicenseName() << "\"" << ",";
        ofs << it->second.nbObjects() << ",";
        ofs << it->second.nbStandardObjects() << ",";
        ofs << it->second.nbPackagedObjects() << ",";
        ofs << it->second.nbCustomObjects() << ",";
        ofs << it->second.getMaxCustomObjects() << ",";
        ofs << it->second.isCompliant();
        ofs << std::endl;
    }
    
    ofs.close();
}
//
//
//
int orchestrator::countTotalObjectsInUserMap() {
    int count {0};
    for (auto it = userMap.begin(); it != userMap.end(); ++it) {
        count += it->second.getPermittedObjects().size();
    }
    return count;
}
//
//
//
bool orchestrator::setPSIdsOnPSG (const std::string& xmlBuffer, std::string& nextUrl) {
    bool moretoread {false};
    using namespace rapidxml;
    xml_document<> document;
    document.parse<0>((char *)xmlBuffer.c_str());
    xml_node<> *node = document.first_node("QueryResult");
    
    xml_node<> * nexturl = node->first_node("nextRecordsUrl");
    if (nexturl) {
        std::string url = nexturl->value();
        
        // extract url
        size_t beginindex = url.find("query/");
            
        nextUrl = url.substr(beginindex+6);
        
        moretoread = true;
    }
    
    xml_node<> * servernode = node->first_node("records");

   for (xml_node<> *child = servernode; child; child = child->next_sibling())
   {
       std::string PSGId {};
       std::string PSId {};

       xml_node<> * PSGIdNode = child->first_node("PermissionSetGroupId");
       if (PSGIdNode) {
           PSGId = PSGIdNode->value();
       }
       xml_node<> * PSIdNode = child->first_node("PermissionSetId");
       if (PSIdNode) {
           PSId = PSIdNode->value();
       }
       if (PSIdNode && PSGIdNode) {
           auto PSGit = permissionSetGroupMap.find(PSGId);
           if (PSGit != permissionSetGroupMap.end()) {
               auto PSit = permissionSetMap.find(PSId);
               if (PSit != permissionSetMap.end()) {
                   // the two ids are found
                   PSGit->second.insertPsid(PSit->second.getId());
               }
               else {
                   std::cerr << "setPSIdsOnPSG: Permission Set " << PSId << " not found in PS Map" << std::endl;
               }
           }
           else {
               std::cerr << "setPSIdsOnPSG: Permission Set Group " << PSGId << " not found in PSG Map" << std::endl;
           }
       }
}
    
return  moretoread;
}
//
//
//
bool orchestrator::updateUsersWithPermissionSetLicenseId (std::string pslid, const std::string& xmlBuffer, std::string& nextUrl) {
    bool moretoread {false};
    using namespace rapidxml;
    xml_document<> document;
    document.parse<0>((char *)xmlBuffer.c_str());
    xml_node<> *node = document.first_node("QueryResult");
    
    xml_node<> * nexturl = node->first_node("nextRecordsUrl");
    if (nexturl) {
        std::string url = nexturl->value();
        
        // extract url
        size_t beginindex = url.find("query/");
            
        nextUrl = url.substr(beginindex+6);
        
        moretoread = true;
    }
    
    xml_node<> * servernode = node->first_node("records");

   for (xml_node<> *child = servernode; child; child = child->next_sibling())
   {
       std::string assigneeid {};
    
       xml_node<> * objectnode = child->first_node("AssigneeId");
       if (objectnode) {
           assigneeid = objectnode->value();
           addPermissionSetLicenseObjectsToUser(pslid,assigneeid);
       }
   }
    
return  moretoread;
}
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
        std::string url = nexturl->value();
        
        // extract url
        size_t beginindex = url.find("query/");
            
        nextUrl = url.substr(beginindex+6);
        
        moretoread = true;
    }
    
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
bool orchestrator::updateUsersWithPermissionSetIdFromPSG (std::string psgid, const std::string& xmlBuffer, std::string& nextUrl) {
    bool moretoread {false};
    using namespace rapidxml;
    xml_document<> document;
    document.parse<0>((char *)xmlBuffer.c_str());
    xml_node<> *node = document.first_node("QueryResult");
    
    xml_node<> * nexturl = node->first_node("nextRecordsUrl");
    if (nexturl) {
        std::string url = nexturl->value();
        
        // extract url
        size_t beginindex = url.find("query/");
            
        nextUrl = url.substr(beginindex+6);
        
        moretoread = true;
    }
    
    xml_node<> * servernode = node->first_node("records");

   for (xml_node<> *child = servernode; child; child = child->next_sibling())
   {
       std::string assigneeid {};
    
       xml_node<> * objectnode = child->first_node("AssigneeId");
       if (objectnode) {
           assigneeid = objectnode->value();
           addPermissionSetObjectsToUserFromPSG(psgid, assigneeid);
       }
   }
    
return  moretoread;
}//
//
//
void orchestrator::addPermissionSetLicenseObjectsToUser(std::string pslid, std::string assigneeid) {
    auto theUser = userMap.find(assigneeid);
    auto thePS = permissionSetLicenseMap.find(pslid);
    
    if (theUser == userMap.end()) {
        if (globals::verbose)
            std::cout << "addPermissionSetLicenseObjectsToUser: user not found in user map, may be inactive, id =" << assigneeid << std::endl;
        return;
    }

    if (thePS == permissionSetLicenseMap.end()) {
        std::cerr << "addPermissionSetLicenseObjectsToUser: permission set not found in permission set map, id =" << pslid << std::endl;
        return;
    }

    theUser->second.insertPermissionSetLicense(thePS->second.getLabel());
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
void orchestrator::addPermissionSetObjectsToUserFromPSG(std::string psgid, std::string assigneeid) {
    auto userit = userMap.find(assigneeid);
    auto PSGit = permissionSetGroupMap.find(psgid);
    
    if (userit == userMap.end()) {
        if (globals::verbose)
            std::cout << "addPermissionSetObjectsToUserFromPSG: user not found in user map, may be inactive, id =" << assigneeid << std::endl;
        return;
    }

    if (PSGit == permissionSetGroupMap.end()) {
        std::cerr << "addPermissionSetObjectsToUserFromPSG: permission set group not found in permission set group map, id =" << psgid << std::endl;
        return;
    }

    // get vector of psid
    auto psids = PSGit->second.getPsids();
    for (auto itpsids = psids.begin(); itpsids != psids.end(); ++itpsids) {
        addPermissionSetObjectsToUser(*itpsids, assigneeid);
    }
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
       xml_node<> * allowCreate = child->first_node("allowCreate");
       xml_node<> * allowDelete = child->first_node("allowDelete");
       xml_node<> * allowEdit = child->first_node("allowEdit");
       xml_node<> * allowRead = child->first_node("allowRead");
       xml_node<> * modifyAllRecords = child->first_node("modifyAllRecords");
       xml_node<> * viewAllRecords = child->first_node("viewAllRecords");

       std::vector<bool> permissions {};
       if (allowCreate) permissions.push_back(getBooleanValue(allowCreate->value()));
       if (allowDelete) permissions.push_back(getBooleanValue(allowDelete->value()));
       if (allowEdit) permissions.push_back(getBooleanValue(allowEdit->value()));
       if (allowRead) permissions.push_back(getBooleanValue(allowRead->value()));
       if (modifyAllRecords) permissions.push_back(getBooleanValue(modifyAllRecords->value()));
       if (viewAllRecords) permissions.push_back(getBooleanValue(viewAllRecords->value()));

       xml_node<> * objectnode = child->first_node("object");
       if (objectnode) {
           bool atleastone {false};
           for (auto itbool = permissions.begin(); itbool != permissions.end(); ++itbool) {
               if (*itbool) {
                   atleastone = true;
                   it->second.insertObject(objectnode->value());
                   break;
               }
           }
           if (!atleastone) {
               if (globals::verbose)
                   std::cout << "Profile id: " << id << " all " << objectnode->value() << " permissions are false" << std::endl;
           }
       }
   }    // end for objectpermissions
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
       xml_node<> * allowCreate = child->first_node("allowCreate");
       xml_node<> * allowDelete = child->first_node("allowDelete");
       xml_node<> * allowEdit = child->first_node("allowEdit");
       xml_node<> * allowRead = child->first_node("allowRead");
       xml_node<> * modifyAllRecords = child->first_node("modifyAllRecords");
       xml_node<> * viewAllRecords = child->first_node("viewAllRecords");

       std::vector<bool> permissions {};
       if (allowCreate) permissions.push_back(getBooleanValue(allowCreate->value()));
       if (allowDelete) permissions.push_back(getBooleanValue(allowDelete->value()));
       if (allowEdit) permissions.push_back(getBooleanValue(allowEdit->value()));
       if (allowRead) permissions.push_back(getBooleanValue(allowRead->value()));
       if (modifyAllRecords) permissions.push_back(getBooleanValue(modifyAllRecords->value()));
       if (viewAllRecords) permissions.push_back(getBooleanValue(viewAllRecords->value()));

       xml_node<> * objectnode = child->first_node("object");

       if (objectnode) {
           bool atleastone {false};
           for (auto itbool = permissions.begin(); itbool != permissions.end(); ++itbool) {
               if (*itbool) {
                   atleastone = true;
                   it->second.insertObject(objectnode->value());
                   break;
               }
           }
           if (!atleastone) {
               if (globals::verbose)
                   std::cout << "Permission set id: " << id << " all " << objectnode->value() << " permissions are false" << std::endl;
           }
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
       xml_node<> * licencenode = child->first_node("UserLicenseId");
       if (licencenode) {
           licenceid = licencenode->value();
       }
       if (namenode && objectnode) {
           profileMap.insert ( std::pair<std::string,profile>(id, {id, name, licenceid}) );
       }
   }
}
//
//
//
void orchestrator::initializeLicenses(const std::string& xmlBuffer) {
    using namespace rapidxml;
    xml_document<> document;
    document.parse<0>((char *)xmlBuffer.c_str());
    xml_node<> *node = document.first_node("QueryResult");
    
    xml_node<> * servernode = node->first_node("records");

   for (xml_node<> *child = servernode; child; child = child->next_sibling())
   {
       std::string id {};
       std::string name {};
       std::string status {};
       std::string total {};
       std::string used {};

       xml_node<> * objectnode = child->first_node("Id");
       if (objectnode) {
           id = objectnode->value();
       }
       xml_node<> * namenode = child->first_node("Name");
       if (namenode) {
           name = namenode->value();
       }
       xml_node<> * statusnode = child->first_node("Status");
       if (statusnode) {
           status = statusnode->value();
       }
       xml_node<> * totalnode = child->first_node("TotalLicenses");
       if (statusnode) {
           total = totalnode->value();
       }
       xml_node<> * usednode = child->first_node("UsedLicenses");
       if (statusnode) {
           used = usednode->value();
       }

       if (namenode && objectnode) {
           licenseMap.insert ( std::pair<std::string,license>(id, {id, name, status, std::stoi(total), std::stoi(used)}) );
       }
   }
}
//
//
//
void orchestrator::initializePermissionSetLicenses(const std::string& xmlBuffer) {
    using namespace rapidxml;
    xml_document<> document;
    document.parse<0>((char *)xmlBuffer.c_str());
    xml_node<> *node = document.first_node("QueryResult");
    
    xml_node<> * servernode = node->first_node("records");

   for (xml_node<> *child = servernode; child; child = child->next_sibling())
   {
       std::string id {};
       std::string name {};
       std::string label {};
       std::string status {};
       std::string total {};
       std::string used {};

       xml_node<> * objectnode = child->first_node("Id");
       if (objectnode) {
           id = objectnode->value();
       }
       xml_node<> * namenode = child->first_node("DeveloperName");
       if (namenode) {
           name = namenode->value();
       }
       xml_node<> * labelnode = child->first_node("MasterLabel");
       if (labelnode) {
           label = labelnode->value();
       }
      xml_node<> * statusnode = child->first_node("Status");
       if (statusnode) {
           status = statusnode->value();
       }
       xml_node<> * totalnode = child->first_node("TotalLicenses");
       if (statusnode) {
           total = totalnode->value();
       }
       xml_node<> * usednode = child->first_node("UsedLicenses");
       if (statusnode) {
           used = usednode->value();
       }

       if (namenode && objectnode) {
           permissionSetLicenseMap.insert ( std::pair<std::string,permissionSetLicense>(id, {id, name, label, status, std::stoi(total), std::stoi(used)}) );
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
       }
       xml_node<> * namenode = child->first_node("Name");
       if (namenode) {
           name = namenode->value();
       }
       if (namenode && objectnode) {
           permissionSetMap.insert ( std::pair<std::string,permissionSet>(id, {id,name}) );
       }
   }
}
//
//
//
void orchestrator::initializePermissionsSetGroup(const std::string& xmlBuffer) {
    using namespace rapidxml;
    xml_document<> document;
    document.parse<0>((char *)xmlBuffer.c_str());
    xml_node<> *node = document.first_node("QueryResult");
    
    xml_node<> * servernode = node->first_node("records");

   for (xml_node<> *child = servernode; child; child = child->next_sibling())
   {
       std::string id {};
       std::string developername {};
       std::string label {};
       xml_node<> * objectnode = child->first_node("Id");
       if (objectnode) {
           id = objectnode->value();
       }
       xml_node<> * namenode = child->first_node("DeveloperName");
       if (namenode) {
           developername = namenode->value();
       }
       xml_node<> * labelnode = child->first_node("MasterLabel");
       if (labelnode) {
           label = labelnode->value();
       }
       if (namenode && objectnode) {
           permissionSetGroupMap.insert ( std::pair<std::string,permissionSetGroup>(id, {id, label, developername}) );
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
    if (globals::verbose)
        std::cout << "Total number of active users from query : " << sizenode->value() << std::endl;
    
    xml_node<> * nexturl = node->first_node("nextRecordsUrl");
    if (nexturl) {
        std::string url = nexturl->value();
        
        // extract url
        size_t beginindex = url.find("query/");
            
        nextUrl = url.substr(beginindex+6);
        
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
    if (globals::verbose)
        std::cout << "number user insertions : " << nbinsert << std::endl;
    
    return  moretoread;
}
//
//
//
bool orchestrator::run() {
    
    //
    //
    //  open REST session
    //
    //
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
    
    //
    //
    //  read permissions set
    //
    //
    std::cout << "Reading permissions set ...";
    
    std::string readBuffer;

    restQuery("?q=SELECT+ID+,+Name+FROM+PermissionSet", readBuffer);

    if (globals::veryverbose) {
        std::cout << "PermissionSet query: " << std::endl;
        std::cout << readBuffer << std::endl;
    }
    
    initializePermissionsSet(readBuffer);
    
    //
    //
    // read permission set groups
    //
    //
    std::cout << std::endl << "Reading permissions set groups...";
    
    restQuery("?q=SELECT+ID+,+MasterLabel+,+DeveloperName+FROM+PermissionSetGroup", readBuffer);

    if (globals::veryverbose) {
        std::cout << "PermissionSetGroup query: " << std::endl;
        std::cout << readBuffer << std::endl;
    }
    
    initializePermissionsSetGroup(readBuffer);
    
    if (globals::verbose) {
        std::cout << permissionSetGroupMap.size() << " permission set groups read" << std::endl;
    }
    
    //
    //
    // read Permission Set Group Components and enrich Permission Set Groups with Permission Set ids
    //
    //
    std::cout << std::endl << "Reading permissions set group components...";

    restQuery("?q=SELECT+ID+,+PermissionSetGroupId+,+PermissionSetId+FROM+PermissionSetGroupComponent", readBuffer);
    
    std::string nexturl {};
    
    bool nextRecordsToRead = setPSIdsOnPSG(readBuffer, nexturl);
    
    while (nextRecordsToRead) {
        if (restQuery(nexturl, readBuffer))
            nextRecordsToRead = setPSIdsOnPSG(readBuffer, nexturl);
        else {
            std::cerr << "PermissionSetGroupComponent query error" << std::endl;
            return false;
        }
    }
    
    if (globals::verbose) {
        for (auto it1 = permissionSetGroupMap.begin(); it1 != permissionSetGroupMap.end(); ++it1) {
            std::cout << "Permission set group: " << it1->second.getLabel() << std::endl;
            auto psids = it1->second.getPsids();
            for (auto it2 = psids.begin(); it2 != psids.end(); ++it2)
                std::cout << *it2 << " ";
            std::cout << std::endl;
        }
    }
    //
    //
    // read profiles
    //
    //
    std::cout << std::endl << "Reading profiles ...";
    
    restQuery("?q=SELECT+ID+,+Name+,+UserLicenseId+FROM+Profile", readBuffer);

    if (globals::veryverbose) {
        std::cout << "PermissionSet query: " << std::endl;
        std::cout << readBuffer << std::endl;
    }
    
    initializeProfiles(readBuffer);
    std::cout << std::endl << profileMap.size() << " profiles inserted";
    
    // read licenses
    std::cout << std::endl << "Reading licences ...";
    
    restQuery("?q=SELECT+ID+,+Name+,+Status+,+UsedLicenses+,+TotalLicenses+FROM+UserLicense", readBuffer);

    if (globals::veryverbose) {
        std::cout << "License query: " << std::endl;
        std::cout << readBuffer << std::endl;
    }
    
    initializeLicenses(readBuffer);
    std::cout << std::endl << licenseMap.size() << " licenses inserted";

    if (globals::verbose) {
        std::cout << std::endl;
        for (auto it=licenseMap.begin(); it != licenseMap.end(); ++it)
            std::cout << it->second.getId() << " " << it->second.getName() << " " << it->second.getStatus() << " total: " << it->second.getTotal() << " used: " << it->second.getUsed() << std::endl;
    }
    
    //
    //
    // read permission set license table
    //
    //
    std::cout << std::endl << "Reading permission set licences ...";
    
    restQuery("?q=SELECT+ID+,+DeveloperName+,+Status+,+UsedLicenses+,+TotalLicenses+FROM+PermissionSetLicense", readBuffer);

    if (globals::veryverbose) {
        std::cout << "permission set license query: " << std::endl;
        std::cout << readBuffer << std::endl;
    }
    
    initializePermissionSetLicenses(readBuffer);
    std::cout << std::endl << permissionSetLicenseMap.size() << " permission set licenses inserted";

    if (globals::verbose) {
        std::cout << std::endl;
        for (auto it=permissionSetLicenseMap.begin(); it != permissionSetLicenseMap.end(); ++it)
            std::cout << it->second.getId() << " " << it->second.getName() << " " << it->second.getStatus() << " total: " << it->second.getTotal() << " used: " << it->second.getUsed() << std::endl;
    }

    //
    //
    // open metadatasession
    //
    //
    if (!metadataSession::openMetadataSession(config::isSandbox(), config::getUsername(), config::getPassword(), config::getApiVersion(), config::getSecurityToken())) {
        std::cerr << "openMetadataSession error" << std::endl;
        return false;
    }
    
    //
    //
    // iterate on profiles map
    //
    //
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
 
    //
    //
    // iterate on permissionSet map
    //
    //
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
 
    //
    //
    // read users
    //
    //
    std::cout << "Reading active users ..." << std::endl;
    restQuery("?q=SELECT+ID+,+Username+,+Firstname+,+Lastname+,+ProfileId+FROM+User+where+isactive+=+true", readBuffer);
        
    nextRecordsToRead = initializeUsers(readBuffer, nexturl);
    
    while (nextRecordsToRead) {
        if (restQuery(nexturl, readBuffer))
            nextRecordsToRead = initializeUsers(readBuffer, nexturl);
        else {
            std::cerr << "User query error" << std::endl;
            return false;
        }
    }
    
    std::cout << "Nb active users : " << userMap.size() << std::endl;
    
    //
    //
    // add objects from profiles to users, and main license
    //
    //
    std::cout << "Assigning objects to users from profiles ..." << std::endl;

    for (auto it = userMap.begin(); it != userMap.end(); ++it) {
        // profile analysis
        std::string theprofile = it->second.getProfile();
        auto prfit = profileMap.find(theprofile);
        if (prfit != profileMap.end()) {
            it->second.setProfileName(prfit->second.getName());
            auto theset = prfit->second.getobjects();
            for (auto itset = theset.begin(); itset != theset.end(); ++itset) {
                it->second.insertPermittedObject(*itset);
            }
        // license
            std::string licenseid = prfit->second.getLicenceId();
            auto licenseit = licenseMap.find(licenseid);
            if (licenseit != licenseMap.end()) {
                it->second.setLicenseName(licenseit->second.getName());
            }
            else {
                std::cerr << "License " << licenseid << " not found in the map for user " << it->second.getFullName() << " userid: " << it->second.getId() << std::endl;
            }
        }
        else {
            std::cerr << "Profile " << theprofile << " not found in the map for user " << it->second.getFullName() << " userid: " << it->second.getId() << std::endl;
        }
    } // end for users
    
    //
    //
    // read permission set assignments
    //
    //
    std::cout << "Reading permission set assignments ..." << std::endl;
    
    for (auto it = permissionSetMap.begin(); it != permissionSetMap.end(); ++it) {
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
    
    //
    //
    // read permission set group assignments
    //
    //
    std::cout << "Reading permission set group assignments ..." << std::endl;
    
    if (globals::verbose) {
        std::cout << "Before PSG assignments, " << countTotalObjectsInUserMap() << std::endl;
    }

    for (auto it = permissionSetGroupMap.begin(); it != permissionSetGroupMap.end(); ++it) {
        if (!restQuery("?q=SELECT+AssigneeId+FROM+PermissionSetAssignment+where+PermissionSetGroupId+=+'" + it->first + "'", readBuffer)) {
            std::cerr << "PermissionSetGroupAssignment query error" << std::endl;
            return false;
        }
        
        nextRecordsToRead = updateUsersWithPermissionSetIdFromPSG (it->first, readBuffer, nexturl);
        while (nextRecordsToRead) {
            if (restQuery(nexturl, readBuffer))
                nextRecordsToRead = updateUsersWithPermissionSetIdFromPSG (it->first, readBuffer, nexturl);
            else {
                std::cerr << "PermissionSetGroupAssignment query error" << std::endl;
                return false;
            }
        }
    }
    
    if (globals::verbose) {
        std::cout << "After PSG assignments, " << countTotalObjectsInUserMap() << std::endl;
    }

    //
    //
    // read permission set license assignments
    //
    //
    std::cout << "Reading permission set license assignments ..." << std::endl;
    
    for (auto it = permissionSetLicenseMap.begin(); it != permissionSetLicenseMap.end(); ++it) {
        if (!restQuery("?q=SELECT+AssigneeId+FROM+PermissionSetLicenseAssign+where+PermissionSetLicenseId+=+'" + it->first + "'", readBuffer)) {
            std::cerr << "PermissionSetLicenseAssign query error" << std::endl;
            return false;
        }
        
        nextRecordsToRead = updateUsersWithPermissionSetLicenseId (it->first, readBuffer, nexturl);
        while (nextRecordsToRead) {
            if (restQuery(nexturl, readBuffer))
                nextRecordsToRead = updateUsersWithPermissionSetLicenseId (it->first, readBuffer, nexturl);
            else {
                std::cerr << "PermissionSetLicenseAssign query error" << std::endl;
                return false;
            }
        }
    }
    
    
    // if verbose iterate on profiles and permissionSet map and print objects
    if (globals::veryverbose) {
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
   
    //
    //
    // iterate on user map, distribute and print objects
    //
    //

    for (auto it = userMap.begin(); it != userMap.end(); ++it) {
        it->second.computeMaxCustomObjects();
        it->second.distributeObjects();
    }

    for (auto it = userMap.begin(); it != userMap.end(); ++it) {
        std::cout << std::endl;
        it->second.print();
    }

    std::cout << std::endl << "*** Users not compliant :" << std::endl;
    int u {0};
    for (auto it = userMap.begin(); it != userMap.end(); ++it) {
        if (!it->second.isCompliant()) {
            u++;
            std::cout << it->second.getFullName() << " " << it->second.nbCustomObjects() << " custom objects " << " vs. authorized: " << it->second.getMaxCustomObjects() << std::endl;
        }
    }
    std::cout << "*** total active not compliant users: " << u << std::endl;

    //
    //
    // output csv
    //
    //
    outputcsv();
    
    return true;
}

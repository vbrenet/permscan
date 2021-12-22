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

bool orchestrator::run() {
    
    //  ouverture session metadata
    //  list metadata permission set
    //  boucle sur toutes les permission set, retrieve - creation instances classes permission set avec vecteurs d'objets, vecteurs de permissions
    //  list metadata profils
    //  boucle sur toutes les permission set, retrieve - creation instances classes profils avec vecteurs d'objets, vecteurs de permissions
    //  lecture des tous les users actifs avec les infos principales - mise dans une map avec les ids
    //
    //
    
    if (!metadataSession::openMetadataSession(config::isSandbox(), config::getUsername(), config::getPassword(), config::getApiVersion(), config::getSecurityToken())) {
        std::cerr << "openMetadataSession error" << std::endl;
        return false;
    }
        
    else
        std::cout << "openMetadataSession successfull" << std::endl;

    /*
    std::string thebody = "<soapenv:Body>";
    thebody += "<met:listMetadata>";
    thebody += "<met:ListMetadataQuery>";
    thebody += "<met:type>ApexClass</met:type>";
    thebody += "</met:ListMetadataQuery>";
    thebody += "</met:listMetadata>";
    thebody += "</soapenv:Body>";

    if (!metadataSession::call("listMetadata", thebody))
        std::cerr << "call error" << std::endl;
    else
        std::cout << "call successfull" << std::endl;
     */
    
    std::string result;
    std::string thebody = "<soapenv:Body>";
    thebody +="<met:readMetadata>";
    thebody +="<met:type>PermissionSet</met:type>";
    thebody +="<met:fullName>FM_DataAccessCallCenter</met:fullName>";
    thebody +="</met:readMetadata>";
    thebody +="</soapenv:Body>";
    if (!metadataSession::call("readMetadata", thebody, result))
        std::cerr << "call error" << std::endl;
    else
        std::cout << "call successfull" << std::endl;
     
    
     using namespace rapidxml;
     xml_document<> document;
     document.parse<0>((char *)result.c_str());
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
            std::cout <<  "object: " << objectnode->value() << std::endl;
        }
    }
    
/*
    if (!metadataSession::describeMetadata())
        std::cerr << "describeMetadata error" << std::endl;
    else
        std::cout << "describeMetadata successfull" << std::endl;
*/
    
/*
 ensuite exemple d'une requete qui marche (POST, sur /m)
 
 <soapenv:Envelope xmlns:soapenv="http://schemas.xmlsoap.org/soap/envelope/" xmlns:met="http://soap.sforce.com/2006/04/metadata">
 <soapenv:Header>
       <met:SessionHeader>
          <met:sessionId>00D3X000001teE0!AR8AQN.wQK0vVVyeyuzMj2FBjbI7k7FE3pheGjS4J.JolFSpN0jrbmYkKtOZQRzFqjGwfxtP9CHlyLaLMoXdXefS6BkaVUoo</met:sessionId>
       </met:SessionHeader>
    </soapenv:Header>
    <soapenv:Body>
       <met:readMetadata>
          <met:type>CustomObject</met:type>
          <met:fullName>Lead</met:fullName>
       </met:readMetadata>
    </soapenv:Body>
 </soapenv:Envelope>
 
 Obtention listMetadata d'un type donné (ex: ApexClass)
 <soapenv:Body>
 <met:listMetadata>
 <met:ListMetadataQuery>
 <met:type>ApexClass</met:type>
 </met:ListMetadataQuery>
 </met:listMetadata>
 </soapenv:Body>
 </soapenv:Envelope>

 */
    return true;
}

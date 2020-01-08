//
//  main.cpp
//  SFDCMetadata
//
//  Created by Vincent Brenet on 07/01/2020.
//  Copyright © 2020 Vincent Brenet. All rights reserved.
//

#include <iostream>
#include "metadataSession.hpp"

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    
    if (!metadataSession::openMetadataSession(false, "vbrenet@playful-badger-9ctmwk.com", "Smyslov1", "47.0", "UleqXE8IQfeJgI3XnSx0GkuF"))
        std::cerr << "openMetadataSession error" << std::endl;
    else
        std::cout << "openMetadataSession successfull" << std::endl;


    

    
    
    if (!metadataSession::describeMetadata())
        std::cerr << "describeMetadata error" << std::endl;
    else
        std::cout << "describeMetadata successfull" << std::endl;
     
    
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
 */
    
    
    return 0;
}

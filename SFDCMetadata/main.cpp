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

    return 0;
}

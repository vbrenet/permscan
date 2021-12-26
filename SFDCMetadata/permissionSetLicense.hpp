//
//  permissionSetLicense.hpp
//  SFDCMetadata
//
//  Created by Vincent Brenet on 26/12/2021.
//  Copyright © 2021 Vincent Brenet. All rights reserved.
//

#ifndef permissionSetLicense_hpp
#define permissionSetLicense_hpp

#include <stdio.h>
#include <string>

class permissionSetLicense {
private:
    std::string id;
    std::string name;
    std::string label;
    std::string status;
    int total;
    int used;
public:
    permissionSetLicense(const std::string i, const std::string n, const std::string l,const std::string s, int t, int u) : id {i}, name {n}, label {l}, status {s}, total {t}, used {u} {}
    //
    const std::string getId() const {return id;}
    const std::string getName() const {return name;}
    const std::string getLabel() const {return label;}
    const std::string getStatus() const {return status;}
    const int getTotal() const {return total;}
    const int getUsed() const {return used;}

};
#endif /* permissionSetLicense_hpp */

//
//  license.hpp
//  SFDCMetadata
//
//  Created by Vincent Brenet on 26/12/2021.
//  Copyright © 2021 Vincent Brenet. All rights reserved.
//

#ifndef license_hpp
#define license_hpp

#include <stdio.h>
#include <string>

class license {
private:
    std::string id;
    std::string name;
    std::string status;
    int total;
    int used;
public:
    license(const std::string i, const std::string n, const std::string s, int t, int u) : name {n}, id {i}, status {s}, total {t}, used {u} {}
    //
    const std::string getId() const {return id;}
    const std::string getName() const {return name;}
    const std::string getStatus() const {return status;}
    const int getTotal() const {return total;}
    const int getUsed() const {return used;}

};
#endif /* license_hpp */

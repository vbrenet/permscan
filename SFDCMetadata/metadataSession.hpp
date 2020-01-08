//
//  metadataSession.hpp
//  SFDCMetadata
//
//  Created by Vincent Brenet on 07/01/2020.
//  Copyright © 2020 Vincent Brenet. All rights reserved.
//

#ifndef metadataSession_hpp
#define metadataSession_hpp

#include <stdio.h>
#include <string>
#include <curl/curl.h>
#include <cstring>

class metadataSession {
private:
    static std::string sessionId;   // returned at login
    static std::string serverUrl;   // calculated at login
    static std::string apiversion;
    static void processResponse(const std::string&);
public:
    static bool firstTime;
    //static std::string body;

    static bool openMetadataSession(bool isSandbox, const std::string username, const std::string password, const std::string apiversion, const std::string securitytoken);
    static bool describeMetadata();
    static bool readMetadata();

    //
    //  accessors
    static std::string &getSessionId() {return sessionId;}
    static std::string &getServerUrl() {return serverUrl;}

};
#endif /* metadataSession_hpp */

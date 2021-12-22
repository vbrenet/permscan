//
//  SalesforceSession.cpp
//  SFDCMetadata
//
//  Created by Vincent Brenet on 22/12/2021.
//  Copyright © 2021 Vincent Brenet. All rights reserved.
//

#include "SalesforceSession.hpp"
#include <iostream>
#include <string>
#include <curl/curl.h>
#include "config.hpp"
#include "globals.hpp"
#include "utils.hpp"

std::string SalesforceSession::connectedAppToken;
std::string SalesforceSession::domain;

//
//  extract token to be used to authenticate further calls
//
std::string SalesforceSession::extractToken(const std::string buffer) {
    size_t beginindex = buffer.find("access_token") + 15;
    size_t endindex = buffer.find('"', beginindex);
    
    return buffer.substr(beginindex,endindex-beginindex);
}
//
//  extract domain to be used to authenticate further calls
//
std::string SalesforceSession::extractDomain(const std::string buffer) {
    
    size_t beginindex = buffer.find("instance_url") + 15;
    beginindex = buffer.find("//",beginindex);
    size_t endindex = buffer.find('"', beginindex);
    
    return buffer.substr(beginindex+2,endindex-beginindex-2);
}
//
//  open connected app session
//
bool SalesforceSession::openSession(const std::string thedomain, const std::string client_id, const std::string client_secret, const std::string username, const std::string password, const std::string securitytoken) {
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    
    domain = thedomain;
    
    std::string urlParameters = "grant_type=password&client_id=" + client_id + "&client_secret=" + client_secret + "&username=" + username + "&password=" + password + securitytoken;
    
    if (globals::verbose) {
        std::cout << "OpenSession: " << urlParameters << std::endl;
        std::cout << "URL : " << "https://" + domain + "/services/oauth2/token" << std::endl;
    }
    
    curl = curl_easy_init();
    
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, ("https://" + domain + "/services/oauth2/token").c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS,urlParameters.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
//      curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        res = curl_easy_perform(curl);
    }
    else
    {
        std::cerr << "OpenSession: " << "curl_easy_init error" << std::endl;
        return false;
    }
    
    if (res != CURLE_OK) {
        std::cerr << "openSession : curl_easy_perform error: " << curl_easy_strerror(res) << std::endl;
        return false;
    }

    long http_code = 0;

    curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);

    if (http_code != 200) {
        std::cerr << "openSession : http error: " << http_code << std::endl;
        std::cerr << "openSession : response: " << readBuffer << std::endl;
        return false;
    }
    
    if (globals::verbose) {
        std::cout << "openSession response : " << readBuffer << std::endl;
    }

    connectedAppToken = SalesforceSession::extractToken(readBuffer);
    
    if (domain == "test.salesforce.com" || domain == "login.salesforce.com") {
        domain = extractDomain(readBuffer);
        if (globals::verbose)
            std::cout << "openSession, extracted domain : " << domain << std::endl;
    }
    
    curl_easy_cleanup(curl);

    return true;
}
//
//  open connected app session
//
bool SalesforceSession::openSession(const sessionCredentials &credentials) {
    return openSession(credentials.domain,
                       credentials.clientId,
                       credentials.clientSecret,
                       credentials.username,
                       credentials.password,
                       credentials.userSecurityToken
                       );
}

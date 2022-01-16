//
//  metadataSession.cpp
//  SFDCMetadata
//
//  Created by Vincent Brenet on 07/01/2020.
//  Copyright © 2020 Vincent Brenet. All rights reserved.
//

#include <iostream>
#include <sstream>
#include "metadataSession.hpp"
#include "rapidxml/rapidxml.hpp"
#include "utils.hpp"
#include "globals.hpp"

std::string metadataSession::sessionId;
std::string metadataSession::serverUrl;
std::string metadataSession::apiversion;

bool metadataSession::firstTime {true};
std::string body;


//
//
//
static size_t read_callback(void *dest, size_t size, size_t nmemb, void *userp)
{
    char *thedest = (char *)dest;
        for (auto i=0; i < body.size(); i++)
            thedest[i] = body[i];
        return body.size();
}
//
//  open a metadata session with Salesforce
//
bool metadataSession::openMetadataSession(bool isSandbox, const std::string username, const std::string password, const std::string theapiversion, const std::string securitytoken) {
    bool result {true};
    
    apiversion = theapiversion;
    
    std::stringstream ssurl;
    ssurl << "https://" << ((isSandbox) ? "test." : "login.") << "salesforce.com/services/Soap/c/" << apiversion;
    
    if (globals::verbose)
        std::cout << "metadata session open url: " << ssurl.str() << std::endl;
    
    std::stringstream ssbody;
    ssbody << "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n"
    << "<soapenv:Envelope xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\">\n"
    << "<soapenv:Body>\n"
    << "<login xmlns=\"urn:enterprise.soap.sforce.com\">\n"
    << "<username>"
    << username
    << "</username>\n"
    << "<password>"
    << password << securitytoken
    << "</password>\n"
    << "</login>\n"
    << "</soapenv:Body>\n"
    << "</soapenv:Envelope>\n";
    
    body = ssbody.str();
    
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    
    curl = curl_easy_init();
    
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, ssurl.str().c_str());
        
        if (globals::veryverbose) {
            std::cout << "URL: " << ssurl.str() << std::endl;
            std::cout << "BODY size: " << strlen(ssbody.str().c_str());
            std::cout << "\nBODY: \n" << ssbody.str() << std::endl;
        }

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        
        // set header
        struct curl_slist *list = NULL;
        list = curl_slist_append(list, "SOAPAction: login");
        list = curl_slist_append(list, "Content-Type: text/xml; charset=UTF-8");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        /* Now specify we want to POST data */
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

        /* we want to use our own read function */
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
        
        /* pointer to pass to our read function */
        curl_easy_setopt(curl, CURLOPT_READDATA, body.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(body.c_str()));
        
        if (globals::veryverbose) {
            std::cout << "openMetadataSession before curl_easy_perform" << std::endl;
        }
        res = curl_easy_perform(curl);
        if (globals::veryverbose) {
            std::cout << "openMetadataSession after curl_easy_perform" << std::endl;
        }

        curl_slist_free_all(list); /* free the list  */

    }
    else
        return false;
    
    if (res != CURLE_OK) {
        std::cerr << "openMetadataSession : curl_easy_perform error: " << curl_easy_strerror(res) << std::endl;
        return false;
    }
    
    long http_code = 0;
    curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
    if (http_code >= 400) {
        std::cerr << "openMetadataSession : http error: " << http_code << std::endl;
        std::cout << "Received buffer: " << readBuffer << std::endl;

        return false;
    }
    
    if (globals::veryverbose)
        std::cout << "Received buffer: " << readBuffer << std::endl;

    using namespace rapidxml;
    xml_document<> document;
    document.parse<0>((char *)readBuffer.c_str());
    xml_node<> *node = document.first_node("soapenv:Envelope");
    node = node->first_node("soapenv:Body");
    node = node->first_node("loginResponse");
    node = node->first_node("result");
    
    xml_node<> * servernode = node->first_node("metadataServerUrl");
    xml_node<> * sessionnode = node->first_node("sessionId");

    std::string theserver (servernode->value());
    serverUrl = theserver;
    std::string thesession (sessionnode->value());
    sessionId = thesession;

    if (globals::verbose) {
        std::cout <<  "sessionid: " << sessionId << std::endl;
        std::cout <<  "serverurl: " << serverUrl << std::endl;
    }

    curl_easy_cleanup(curl);

    return result;
}
//
//
//
bool metadataSession::call(const std::string theaction, const std::string thebody, std::string& readBuffer)
{
    bool result {true};
    
    
    std::stringstream ssurl;
    ssurl << serverUrl;
    
    body = "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n";
    body += "<soapenv:Envelope xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\"\n";
    body += "xmlns:met=\"http://soap.sforce.com/2006/04/metadata\">\n";
    
    body += "<soapenv:Header>\n";
    body += "<met:SessionHeader>\n";
    body += "<met:sessionId>";
    body += sessionId;
    body += "</met:sessionId>\n";
    body += "</met:SessionHeader>\n";
    body += "</soapenv:Header>\n";

    body += thebody;
    
    body += "</soapenv:Envelope>\n";

    
    CURL *curl;
    CURLcode res;
    
    curl = curl_easy_init();
    
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, ssurl.str().c_str());
        
        if (globals::veryverbose) {
            std::cout << "metadataSession::call" << std::endl;
            std::cout << "URL: " << ssurl.str() << std::endl;
            std::cout << "BODY size: " << strlen(body.c_str());
            std::cout << "\nBODY: \n" << body  << std::endl;
        }

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "gzip");

        // set header
        struct curl_slist *list = NULL;
        std::string action = "SOAPAction: " + theaction;
        list = curl_slist_append(list, action.c_str());
        list = curl_slist_append(list, "Content-Type: text/xml; charset=UTF-8");
        list = curl_slist_append(list, "Accept-Encoding: gzip");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        /* Now specify we want to POST data */
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

        /* we want to use our own read function */
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
        
        /* pointer to pass to our read function */
        curl_easy_setopt(curl, CURLOPT_READDATA, body.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(body.c_str()));
        
        if (globals::veryverbose) {
            std::cout << "metadataSession::call before curl_easy_perform" << std::endl;
        }

        res = curl_easy_perform(curl);
        curl_slist_free_all(list); /* free the list  */
        if (globals::veryverbose) {
            std::cout << "metadataSession::call after curl_easy_perform" << std::endl;
        }
    }
    else
        return false;
    
    if (res != CURLE_OK) {
        std::cerr << "readMetadata : curl_easy_perform error: " << curl_easy_strerror(res) << std::endl;
        return false;
    }
    
    long http_code = 0;
    curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
    if (http_code >= 400) {
        std::cerr << "readMetadata : http error: " << http_code << std::endl;
        std::cout << "Received buffer: " << readBuffer << std::endl;

        curl_easy_cleanup(curl);

        return false;
    }
    
    curl_easy_cleanup(curl);

    return result;
}

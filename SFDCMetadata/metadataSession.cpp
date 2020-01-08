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

std::string metadataSession::sessionId;
std::string metadataSession::serverUrl;
std::string metadataSession::apiversion;

bool metadataSession::firstTime {true};
std::string body;

//
std::string extractXmlToken(const std::string& inputbuffer, const std::string& token) {
    std::string endtoken = token;
    endtoken.insert(1,1,'/');
    
    size_t beginpos = inputbuffer.find(token);
    size_t endpos = inputbuffer.find(endtoken);
    
    return inputbuffer.substr(beginpos+token.size(),endpos-beginpos-token.size());
}

//
 size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
   ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}
//


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
//
void metadataSession::processResponse(const std::string& response) {
    std::string serverUrlToken = extractXmlToken(response, "<serverUrl>");
    // received : https://vbrlight-dev-ed.my.salesforce.com/services/Soap/u/39.0/00D58000000ZzE0
    // target : https://vbrlight-dev-ed.my.salesforce.com/services/async/39.0
    size_t lastslash = serverUrlToken.find_last_of('/');
    serverUrlToken.erase(lastslash);
    lastslash = serverUrlToken.find_last_of('/');
    std::string version = serverUrlToken.substr(lastslash+1);
    size_t soap = serverUrlToken.find("/Soap");
    serverUrl = serverUrlToken.substr(0,soap) + "/async/" + version;

    sessionId = extractXmlToken(response, "<sessionId>");
}

//
//  open a metadata session with Salesforce
//
bool metadataSession::openMetadataSession(bool isSandbox, const std::string username, const std::string password, const std::string theapiversion, const std::string securitytoken) {
    bool result {true};
    
    apiversion = theapiversion;
    
    std::stringstream ssurl;
    ssurl << "https://" << ((isSandbox) ? "test." : "login.") << "salesforce.com/services/Soap/c/" << apiversion;
    
    std::cout << "session open url: " << ssurl.str() << std::endl;
    
    std::stringstream ssbody;
    ssbody << "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n"
    << "<soapenv:Envelope xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\">\n"
    << "<soapenv:Body>\n"
    << "<login xmlns=\"urn:enterprise.soap.sforce.com\">\n"
    << "<username>"
    << username
    << "</username>\n"
    << "<password>"
    << password
    << "</password>\n"
    << "</login>\n"
    << "</soapenv:Body>\n"
    << "</soapenv:Envelope>\n";
    
    /*
     <soapenv:Envelope xmlns:soapenv="http://schemas.xmlsoap.org/soap/envelope/">
        <soapenv:Body>
            <login xmlns="urn:enterprise.soap.sforce.com">
                <username>vbrenet@playful-badger-9ctmwk.com</username>
                <password>Smyslov1UleqXE8IQfeJgI3XnSx0GkuF</password>
            </login>
        </soapenv:Body>
     </soapenv:Envelope>
     */
    body = ssbody.str();
    
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    
    curl = curl_easy_init();
    
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, ssurl.str().c_str());
        
        std::cout << "URL: " << ssurl.str() << std::endl;
        std::cout << "BODY size: " << strlen(ssbody.str().c_str());
        std::cout << "\nBODY: \n" << ssbody.str() << std::endl;

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
        
        res = curl_easy_perform(curl);
        curl_slist_free_all(list); /* free the list  */

        curl_easy_cleanup(curl);
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
    
    std::cout << "Received buffer: " << readBuffer << std::endl;

    processResponse(readBuffer);
    
        std::cout <<  "sessionid: " << sessionId << std::endl;
        std::cout <<  "serverurl: " << serverUrl << std::endl;

    return result;
}

//
//
//
bool metadataSession::describeMetadata() {
    bool result {true};
        
    std::stringstream ssurl;
    ssurl << "https://playful-badger-9ctmwk-dev-ed.my.salesforce.com/services/Soap/m/47";
    
    std::cout << "session open url: " << ssurl.str() << std::endl;
    
    std::stringstream ssbody;
    
    /*
     <soapenv:Envelope xmlns:soapenv="http://schemas.xmlsoap.org/soap/envelope/" xmlns:met="http://soap.sforce.com/2006/04/metadata">
     <soapenv:Header>
           <met:SessionHeader>
              <met:sessionId>00D3X000001teE0!AR8AQGNuqI270qAoa89yzppcG34QfNiOO7ffg27qQvSyMBaH45pM2.sM4f7w9Ak92cSHyS7V.sdyaesiQJmDWoRrfYE4BMB5</met:sessionId>
           </met:SessionHeader>
        </soapenv:Header>
        <soapenv:Body>
           <met:describeMetadata>

           </met:describeMetadata>
        </soapenv:Body>
     </soapenv:Envelope>

     */
    
    
    
    ssbody << "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n"
    << "<soapenv:Envelope xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\"\n"
    << "xmlns:met=\"http://soap.sforce.com/2006/04/metadata\">\n"
    
    << "<soapenv:Header>\n"
    << "<met:SessionHeader>\n"
    << "<met:sessionId>"
    << sessionId
    << "</met:sessionId>\n"
    << "</met:SessionHeader>\n"
    << "</soapenv:Header>\n"

    << "<soapenv:Body>\n"
    << "<met:describeMetadata>"
    << "</met:describeMetadata>"

    << "</soapenv:Body>\n"
    << "</soapenv:Envelope>\n";
    

    body = ssbody.str();
    
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    
    curl = curl_easy_init();
    
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, ssurl.str().c_str());
        
        std::cout << "URL: " << ssurl.str() << std::endl;
        std::cout << "BODY size: " << strlen(ssbody.str().c_str());
        std::cout << "\nBODY: \n" << ssbody.str() << std::endl;

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        
        // set header
        struct curl_slist *list = NULL;
        list = curl_slist_append(list, "SOAPAction: describeMetadata");
        list = curl_slist_append(list, "Content-Type: text/xml; charset=UTF-8");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        /* Now specify we want to POST data */
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

        /* we want to use our own read function */
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
        
        /* pointer to pass to our read function */
        curl_easy_setopt(curl, CURLOPT_READDATA, ssbody.str().c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(ssbody.str().c_str()));
        
        res = curl_easy_perform(curl);
        curl_slist_free_all(list); /* free the list  */

        curl_easy_cleanup(curl);
    }
    else
        return false;
    
    if (res != CURLE_OK) {
        std::cerr << "describeMetadata : curl_easy_perform error: " << curl_easy_strerror(res) << std::endl;
        return false;
    }
    
    long http_code = 0;
    curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
    if (http_code >= 400) {
        std::cerr << "describeMetadata : http error: " << http_code << std::endl;
        std::cout << "Received buffer: " << readBuffer << std::endl;

        return false;
    }
    
    std::cout << "Received buffer: " << readBuffer << std::endl;
    
   
    using namespace rapidxml;
   xml_document<> document;
    document.parse<0>((char *)readBuffer.c_str());
    xml_node<> *node = document.first_node("soapenv:Envelope");
    node = node->first_node("soapenv:Body");
    node = node->first_node("describeMetadataResponse");
    node = node->first_node("result");
    xml_node<> * meta_node = node->first_node("metadataObjects");
    while (meta_node){
        xml_node<> * xml_node = meta_node->first_node("xmlName");
        if (xml_node)
            std::cout << "Value   is: " << xml_node->value() << "\n";
        else
            break;
        meta_node = meta_node->next_sibling();
    }

    
    
    return result;
}
//
//
//
bool metadataSession::readMetadata(){
    bool result {true};
    
    
    std::stringstream ssurl;
    ssurl << "https://playful-badger-9ctmwk-dev-ed.my.salesforce.com/services/Soap/m/47";

    std::cout << "session  url: " << ssurl.str() << std::endl;
    
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
    body += "<soapenv:Body>\n";
    body += "<met:readMetadata>\n";
    body += "<met:type>CustomObject</met:type>\n";
    body += "<met:fullName>Lead</met:fullName>\n";
    body += "</met:readMetadata>\n";

    body += "</soapenv:Body>\n";

    
    
    body += "</soapenv:Envelope>\n";

    
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    
    curl = curl_easy_init();
    
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, ssurl.str().c_str());
        
        std::cout << "URL: " << ssurl.str() << std::endl;
        std::cout << "BODY size: " << strlen(body.c_str());
        std::cout << "\nBODY: \n" << body  << std::endl;

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        
        // set header
        struct curl_slist *list = NULL;
        list = curl_slist_append(list, "SOAPAction: readMetadata");
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
        
        res = curl_easy_perform(curl);
        curl_slist_free_all(list); /* free the list  */

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
    
    std::cout << "Received buffer: " << readBuffer << std::endl;

    curl_easy_cleanup(curl);

    return result;
}

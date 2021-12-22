//
//  utils.hpp
//  SFDCMetadata
//
//  Created by Vincent Brenet on 22/12/2021.
//  Copyright © 2021 Vincent Brenet. All rights reserved.
//

#ifndef utils_hpp
#define utils_hpp

#include <stdio.h>
#include <string>

std::string getDateString();
std::string removeCommas (const std::string& input);
void removeDoubleQuote (std::string& input);
std::string replaceDoubleQuotes (const std::string& input);
bool getBooleanValue (const std::string&);
bool isStringNumeric (const std::string input);
std::string extractXmlToken(const std::string& inputbuffer, size_t pos, const std::string& token);
std::string extractXmlToken(const std::string& inputbuffer, const std::string& token);
size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
bool restQuery(const std::string& query, std::string& result);
std::string getFrenchDate(const std::string input);
#endif /* utils_hpp */

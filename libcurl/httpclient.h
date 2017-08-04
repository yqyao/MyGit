#ifndef __HTTP_CURL_H__  
#define __HTTP_CURL_H__ 

#include <string>
using namespace std; 

class HttpClient
{
public:
    HttpClient();
    ~HttpClient();
public:

    /*
    @brief HTTP get request
    @param url, request url example: http://www.baidu.com
    @param response, response from the server
    @return success or not
     */
    int getUrl(const string url, string &response);

    /*
    @brief HTTP post request
    @param url, request url example: http://www.baidu.com
    @param postStr, request postStr, json format
    @param response, response from the server
    @return success or not
     */

    int postUrlJson(const string url, const string & postStr, string &response);

    /*
    @brief HTTP post request
    @param url, request url example: http://www.baidu.com
    @param postStr, request postStr, form format
    @param response, response from the server
    @return success or not
     */

    int postUrlForm(const string url, const string postStr, string &response);

    /*
    @brief HTTP put request
    @param url, request url example: http://www.baidu.com
    @param putstr request putStr, json format
    @param response, response from the server
    @return success or not
     */

    int putUrlJson(const string url, const string putStr, string &response);

    /*
    @brief HTTP put request
    @param url, request url example: http://www.baidu.com
    @param putstr request putStr, form format
    @param response, response from the server
    @return success or not
     */

    int putUrlForm(const string url, const string putStr, string &response);
};

#endif  
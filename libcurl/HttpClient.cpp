#include <stdio.h>  
#include <curl/curl.h>
#include <dist/json/json.h>
#include <dist/json/json-forwards.h>
#include <dist/jsoncpp.cpp>
#include <typeinfo>
#include <iostream>

#include "httpclient.h"
using namespace std; 

HttpClient::HttpClient()
{
    curl_global_init(CURL_GLOBAL_ALL); //Must initialize libcurl before any threads are started
}

HttpClient::~HttpClient()
{

}
Json::Value stringToJson(string str);

/*
WriteCallback function
 */

static size_t OnWriteData(void* buffer, size_t size, size_t nmemb, void* lpVoid)  
{  
    string* str = dynamic_cast<string*>((string *)lpVoid);  
    if( NULL == str || NULL == buffer )  
    {  
        return -1;  
    }  
  
    char* pData = (char*)buffer;  
    str->append(pData, size * nmemb);  
    return nmemb;  
} 

int HttpClient::getUrl(const string url, string &response)
{
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;  
    headers = curl_slist_append(headers, "Accept: */*");  
    curl = curl_easy_init();    // 初始化  
    if (curl)  
    {  
        //curl_easy_setopt(curl, CURLOPT_PROXY, "10.99.60.201:8080);// 代理  
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);// 改协议头  
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); 
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData); 
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&response); //将返回的http头输出到fp指向的文件  
        res = curl_easy_perform(curl);   // 执行  
        cout << "status: " << res << endl; 
        if (res != 0) {  
            curl_slist_free_all(headers);  
            curl_easy_cleanup(curl);
            return -1;  
        }
        cout << "get response:\t" << response << endl;  
        return 0;  
    }
    return -1;   
}


int HttpClient::postUrlJson(const string url, const string & postStr, string &response)
{
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;  
    headers = curl_slist_append(headers, "Accept: */*");
    headers = curl_slist_append(headers, "Host: localhost"); 
    headers = curl_slist_append(headers, "Content-Type: application/json");  //
    // headers = curl_slist_append(headers, "Host:") remove the headers of host 
    curl = curl_easy_init();    // 初始化 
    if (curl)  
    {  
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postStr.c_str());    // 指定post内容  
        //curl_easy_setopt(curl, CURLOPT_PROXY, "10.99.60.201:8080");  
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());   // 指定url  
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&response);
        // curl_easy_setopt(curl, CURLOPT_HEADERDATA, fp);  
        res = curl_easy_perform(curl);
        if (res != 0) {  
            curl_slist_free_all(headers);  
            curl_easy_cleanup(curl);
            return -1;  
        }
        // stringToJson(response);  
        cout << "post json response:\t" << response << endl;
        return 0; 
    }
    return -1;  
}

int HttpClient::postUrlForm(const string url, const string postStr, string &response)
{
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;  
    headers = curl_slist_append(headers, "Accept: */*");
    headers = curl_slist_append(headers, "Host: localhost"); 
    // headers = curl_slist_append(headers, "Content-Type: application/json");  
    curl = curl_easy_init();    // 初始化 
    if (curl)  
    {  
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postStr.c_str());    // 指定post内容  
        //curl_easy_setopt(curl, CURLOPT_PROXY, "10.99.60.201:8080");  
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());   // 指定url  
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&response);
        // curl_easy_setopt(curl, CURLOPT_HEADERDATA, fp);  
        res = curl_easy_perform(curl);
        if (res != 0) {  
            curl_slist_free_all(headers);  
            curl_easy_cleanup(curl);
            return -1;  
        }  
        cout << "post form response:\t" << response << endl;
        return 0; 
    }
    return -1;  
}


int HttpClient::putUrlJson(const string url, const string putStr, string &response)
{
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;  
    headers = curl_slist_append(headers, "Accept: */*");
    headers = curl_slist_append(headers, "Host: localhost"); 
    headers = curl_slist_append(headers, "Content-Type: application/json");  
    curl = curl_easy_init();    // 初始化 
    if (curl)  
    {  
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, putStr.c_str());    // 指定post内容  
        //curl_easy_setopt(curl, CURLOPT_PROXY, "10.99.60.201:8080");
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");  
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());   // 指定url  
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&response);
        // curl_easy_setopt(curl, CURLOPT_HEADERDATA, fp);  
        res = curl_easy_perform(curl);
        if (res != 0) {  
            curl_slist_free_all(headers);  
            curl_easy_cleanup(curl);
            return -1;  
        }  
        cout << "put json response:\t" << response << endl;
        return 0; 
    }
    return -1;  
}

int HttpClient::putUrlForm(const string url, const string putStr, string &response)
{
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;  
    headers = curl_slist_append(headers, "Accept: */*");
    headers = curl_slist_append(headers, "Host: localhost"); 
    // headers = curl_slist_append(headers, "Content-Type: application/json");  
    curl = curl_easy_init();    // 初始化 
    if (curl)  
    {  
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, putStr.c_str());    // 指定post内容  
        //curl_easy_setopt(curl, CURLOPT_PROXY, "10.99.60.201:8080");
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");  
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());   // 指定url  
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&response);
        // curl_easy_setopt(curl, CURLOPT_HEADERDATA, fp);  
        res = curl_easy_perform(curl);
        if (res != 0) {  
            curl_slist_free_all(headers);  
            curl_easy_cleanup(curl);
            return -1;  
        }  
        cout << "put form response:\t" << response << endl;
        return 0; 
    }
    return -1;  
}
Json::Value stringToJson(string str)
{
    Json::Reader reader;
    Json::Value root;
    if (reader.parse(str, root)) {
    if (!root["name"].isNull()) {
        string strValue= root["name"].asString(); 
        cout << strValue << endl;
        }
    }
}

string postJsonStr()
{
    Json::Value root;    
    root["post_json"] = "json";  
    root["post_id"] = 3;  
    return root.toStyledString();
}

string postFormStr()
{
    string formStr="&post_form=form&post_id=4";
    return formStr;
}

string putJsonStr()
{
    Json::Value root;
    root["put_json"] = "put";
    root["put_id"] = 5;
    return root.toStyledString();
}

string putFormStr()
{
    string formStr="&put_form=form&put_id=6";
    return formStr;
}

int main()
{
    // HttpClient *httpclient = new HttpClient()
    shared_ptr<HttpClient> httpclient = nullptr;
    httpclient = make_shared<HttpClient>();
    string get_url = "http://localhost:8089/get";
    string post_form_url = "http://localhost:8089/post_form";
    string post_json_url = "http://localhost:8089/post_json";
    string put_json_url = "http://localhost:8089/put_json";
    string put_form_url = "http://localhost:8089/put_form";
    string get_rep;
    string post_json_rep;
    string post_form_rep;
    string put_json_rep;
    string put_form_rep;
    httpclient->getUrl(get_url, get_rep);
    httpclient->postUrlJson(post_json_url, postJsonStr(), post_json_rep);
    // httpclient->postUrlForm(post_form_url, postFormStr(), post_form_rep);
    // httpclient->putUrlJson(put_json_url, putJsonStr(), put_json_rep);
    // httpclient->putUrlForm(put_form_url, putFormStr(), put_form_rep);
    return 0;
}
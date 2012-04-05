//
//  SimpleHttpRequest.h
//  drawstory
//
//  Created by 张 靖宇 on 12-3-30.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef drawstory_SimpleHttpRequest_h
#define drawstory_SimpleHttpRequest_h

#include "cocos2d.h"
#include "pthread.h"
#include "curl/curl.h"
#include "json/json.h"
#include <string>


class SimpleHttpRequest;
class SimpleHttpRequestDelegate
{
public:
    virtual void onResponse(bool result,const Json::Value& response,SimpleHttpRequest* request) = 0;
};


class SimpleHttpRequest : public cocos2d::CCObject
{
public:
    typedef enum {
        kHttpMethodGet,
        kHttpMethodPost,
        kHttpMethodPut,
        kHttpMethodDelete
    } HttpMethod;
    
    typedef enum {
        kPostDataTypeText,
        kPostDataTypeBinary,
    } PostDataType;
    
public:
    static SimpleHttpRequest* simpleHttpRequestWithURL(const std::string& url,
                                                       HttpMethod method,
                                                       SimpleHttpRequestDelegate* delegate,
                                                       int tag = 0);
public:
    SimpleHttpRequest(const std::string& url,HttpMethod method,SimpleHttpRequestDelegate* delegate,int tag);
    virtual ~SimpleHttpRequest();
    
    // Operator
    void cancel();
    bool cancelled() const { return cancelled_; }
    
    // start 
    bool start();
    
    
    void setURL(const std::string& url) { url_ = url; }
    const std::string& url() const { return url_; }
    
    void setHttpMethod(HttpMethod method) { method_ = method; }
    HttpMethod httpMethod() const { return method_; }
    
    void setPostText(const std::string& text) { post_ = text; }
    const std::string& post() const { return post_; }

    
    
    int tag() const { return tag_; }
    void setTag(int tag) { tag_ = tag; }
protected:    
    static size_t writeData(char *data, size_t size, size_t nmemb, SimpleHttpRequest *userdata);
    std::string& buffer() { return buffer_; }
    
    
    static void* doRequest(void* param);
protected:
    pthread_t thread_; 
    pthread_mutex_t mutex_;
    
    int tag_;
    
    // need lock mutex
    bool cancelled_;
    
    SimpleHttpRequestDelegate* delegate_;
    // URL
    std::string url_;
    //
    HttpMethod method_;
    
    std::string post_;
    // 
    std::string buffer_;

};


#endif

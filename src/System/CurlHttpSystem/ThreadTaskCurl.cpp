#	include "ThreadTaskCurl.h"

#	include "Interface/FileSystemInterface.h"
#	include "Interface/ConfigInterface.h"

#	include "Logger/Logger.h"

#   include <sstream>

namespace Menge
{

	//////////////////////////////////////////////////////////////////////////
    ThreadTaskCurl::ThreadTaskCurl()
        : m_id( 0 )
        , m_receiver( nullptr )
        , m_code( 0 )
        , m_status( CURLE_OK )
	{
	}
    //////////////////////////////////////////////////////////////////////////
    void ThreadTaskCurl::setRequestId( HttpRequestID _id )
    {
        m_id = _id;
    }
    //////////////////////////////////////////////////////////////////////////
    HttpRequestID ThreadTaskCurl::getRequestId() const
    {
        return m_id;
    }
    //////////////////////////////////////////////////////////////////////////
    void ThreadTaskCurl::setReceiver( HttpReceiver * _receiver )
    {
        m_receiver = _receiver;
    }
    //////////////////////////////////////////////////////////////////////////
    HttpReceiver * ThreadTaskCurl::getReceiver() const
    {
        return m_receiver;
    }
	//////////////////////////////////////////////////////////////////////////
	bool ThreadTaskCurl::_onRun()
	{
		return true;
	}
    //////////////////////////////////////////////////////////////////////////
    bool ThreadTaskCurl::_onMain()
    {
        /* init the curl session */
        CURL * curl = curl_easy_init();

        this->_onCURL( curl );

        CURLcode status = curl_easy_perform( curl );

        m_status = status;

        long http_code = 0;
        curl_easy_getinfo( curl, CURLINFO_RESPONSE_CODE, &http_code );

        m_code = (uint32_t)http_code;

        curl_easy_cleanup( curl );

        return true;
    }
    ////////////////////////////////////////////////////////////////////////
    static size_t s_writeRequestPerformerResponse( char *ptr, size_t size, size_t nmemb, void *userdata )
    {
        ThreadTaskCurl * perfomer = static_cast<ThreadTaskCurl *>(userdata);

        size_t total = size * nmemb;

        perfomer->writeResponse( ptr, total );

        return total;
    }
    //////////////////////////////////////////////////////////////////////////
    void ThreadTaskCurl::setupWriteResponse( CURL * _curl )
    {
        /* send all data to this function  */
        curl_easy_setopt( _curl, CURLOPT_WRITEDATA, (void *)this );
        curl_easy_setopt( _curl, CURLOPT_WRITEFUNCTION, &s_writeRequestPerformerResponse );
    }
	//////////////////////////////////////////////////////////////////////////
	void ThreadTaskCurl::writeResponse( char * _ptr, size_t _size )
	{
		m_response.append( _ptr, _size );
	}
	//////////////////////////////////////////////////////////////////////////
	void ThreadTaskCurl::_onComplete( bool _successful )
	{
		m_receiver->onHttpRequestComplete( m_id, (uint32_t)m_status, m_response, m_code, _successful );
	}
}
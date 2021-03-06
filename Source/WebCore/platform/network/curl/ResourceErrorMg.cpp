#include "mdolphin_errcode.h"
#include "ResourceError.h"

using namespace WebCore;
#define MDEC_NET_UNSUPPORTED  0

static int NetworkErrorCode[] = {
	MDEC_NET_UNSUPPORTED,           /* CURLE_OK 0 */
	MDEC_NET_UNSUPPORTED_PROTOCOL,  /* CURLE_UNSUPPORTED_PROTOCOL 1 */
	MDEC_NET_UNKNOWN_ERROR,         /* CURLE_FAILED_INIT 2 */
	MDEC_NET_URL_ERROR,             /* CURLE_URL_MALFORMAT 3 */
	MDEC_NET_UNSUPPORTED,           /* CURLE_URL_MALFORMAT_USER - NOT USED 4 */
	MDEC_NET_PROXY_ERROR,           /* CURLE_COULDNT_RESOLVE_PROXY 5 */
	MDEC_NET_DNS_ERROR,             /* CURLE_COULDNT_RESOLVE_HOST 6 */
	MDEC_NET_COULDNT_CONNECT,       /* CURLE_COULDNT_CONNECT 7 */
	MDEC_NET_FTP_UNKNOWN_ERROR,     /* CURLE_FTP_WEIRD_SERVER_REPLY 8 */
	MDEC_NET_FTP_UNKNOWN_ERROR,     /* CURLE_FTP_ACCESS_DENIED 9 */
	MDEC_NET_UNSUPPORTED,           /* CURLE_FTP_USER_PASSWORD_INCORRECT 10 - NOT USED */
	MDEC_NET_FTP_UNKNOWN_ERROR,     /* CURLE_FTP_WEIRD_PASS_REPLY 11 */
	MDEC_NET_FTP_UNKNOWN_ERROR,     /* CURLE_FTP_WEIRD_USER_REPLY 12 */
	MDEC_NET_FTP_UNKNOWN_ERROR,     /* CURLE_FTP_WEIRD_PASV_REPLY 13 */
	MDEC_NET_FTP_UNKNOWN_ERROR,     /* CURLE_FTP_WEIRD_227_FORMAT 14 */
	MDEC_NET_FTP_UNKNOWN_ERROR,     /* CURLE_FTP_CANT_GET_HOST 15 */
	MDEC_NET_FTP_UNKNOWN_ERROR,     /* CURLE_FTP_CANT_RECONNECT 16 */
	MDEC_NET_FTP_UNKNOWN_ERROR,     /* CURLE_FTP_COULDNT_SET_BINARY 17 */
	MDEC_NET_UNKNOWN_ERROR,         /* CURLE_PARTIAL_FILE 18 */
	MDEC_NET_FTP_UNKNOWN_ERROR,     /* CURLE_FTP_COULDNT_RETR_FILE 19 */
	MDEC_NET_FTP_UNKNOWN_ERROR,     /* CURLE_FTP_WRITE_ERROR 20 */
	MDEC_NET_FTP_UNKNOWN_ERROR,     /* CURLE_FTP_QUOTE_ERROR 21 */
	MDEC_NET_UNKNOWN_ERROR,         /* CURLE_HTTP_RETURNED_ERROR 22 CURLOPT_FAILONERROR unsupport*/
	MDEC_NET_UNKNOWN_ERROR,         /* CURLE_WRITE_ERROR 23 */
	MDEC_NET_UNSUPPORTED,           /* CURLE_MALFORMAT_USER 24 - NOT USED */
	MDEC_NET_FTP_UNKNOWN_ERROR,     /* CURLE_FTP_COULDNT_STOR_FILE 25 - failed FTP upload */
	MDEC_NET_UNKNOWN_ERROR,         /* CURLE_READ_ERROR 26 - could open/read from file */
	MDEC_NET_UNKNOWN_ERROR,         /* CURLE_OUT_OF_MEMORY 27 */
	MDEC_NET_UNKNOWN_ERROR,         /* CURLE_OPERATION_TIMEOUTED 28 */
	MDEC_NET_FTP_UNKNOWN_ERROR,     /* CURLE_FTP_COULDNT_SET_ASCII 29 - TYPE A failed */
	MDEC_NET_FTP_UNKNOWN_ERROR,     /* CURLE_FTP_PORT_FAILED 30 - FTP PORT operation failed */
	MDEC_NET_FTP_UNKNOWN_ERROR,     /* CURLE_FTP_COULDNT_USE_REST 31 - the REST command failed */
	MDEC_NET_FTP_UNKNOWN_ERROR,     /* CURLE_FTP_COULDNT_GET_SIZE 32 - the SIZE command failed */
	MDEC_NET_UNKNOWN_ERROR,         /* CURLE_HTTP_RANGE_ERROR 33 - RANGE "command" didn't work */
	MDEC_NET_UNKNOWN_ERROR,         /* CURLE_HTTP_POST_ERROR 34 */
	MDEC_NET_SSL_CONNECT_ERROR,     /* CURLE_SSL_CONNECT_ERROR 35 */
	MDEC_NET_UNKNOWN_ERROR,         /* CURLE_BAD_DOWNLOAD_RESUME 36 - couldn't resume download */
	MDEC_NET_FILE_READ_ERROR,       /* CURLE_FILE_COULDNT_READ_FILE 37 */
	MDEC_NET_UNSUPPORTED,           /* CURLE_LDAP_CANNOT_BIND 38 */
	MDEC_NET_UNSUPPORTED,           /* CURLE_LDAP_SEARCH_FAILED 39 */
	MDEC_NET_UNSUPPORTED,           /* CURLE_LIBRARY_NOT_FOUND 40 */
	MDEC_NET_UNSUPPORTED,           /* CURLE_FUNCTION_NOT_FOUND 41 */
	MDEC_NET_UNKNOWN_ERROR,         /* CURLE_ABORTED_BY_CALLBACK 42 */
	MDEC_NET_UNKNOWN_ERROR,         /* CURLE_BAD_FUNCTION_ARGUMENT 43 */
	MDEC_NET_UNSUPPORTED,           /* CURLE_BAD_CALLING_ORDER 44 - NOT USED */
	MDEC_NET_UNKNOWN_ERROR,         /* CURLE_INTERFACE_FAILED 45 - CURLOPT_INTERFACE failed */
	MDEC_NET_UNSUPPORTED,           /* CURLE_BAD_PASSWORD_ENTERED 46 - NOT USED */
	MDEC_NET_UNKNOWN_ERROR,         /* CURLE_TOO_MANY_REDIRECTS 47 */
	MDEC_NET_UNSUPPORTED,           /* CURLE_UNKNOWN_TELNET_OPTION 48 */
	MDEC_NET_UNSUPPORTED,           /* CURLE_TELNET_OPTION_SYNTAX  49 - Malformed telnet option */
	MDEC_NET_UNSUPPORTED,           /* CURLE_OBSOLETE 50 - NOT USED */
	MDEC_NET_SSL_PEER_CERTIFICATE,  /* CURLE_SSL_PEER_CERTIFICATE 51 */
	MDEC_NET_UNKNOWN_ERROR,         /* CURLE_GOT_NOTHING 52 - when this is a specific error */
	MDEC_NET_SSL_ENGINE_NOTFOUND,   /* CURLE_SSL_ENGINE_NOTFOUND 53 - SSL crypto engine not found */
	MDEC_NET_SSL_ENGINE_SETFAILED,  /* CURLE_SSL_ENGINE_SETFAILED 54 */
	MDEC_NET_UNKNOWN_ERROR,         /* CURLE_SEND_ERROR 55 - failed sending network data */
	MDEC_NET_UNKNOWN_ERROR,         /* CURLE_RECV_ERROR 56 - failure in receiving network data */
	MDEC_NET_UNKNOWN_ERROR,         /* CURLE_SHARE_IN_USE 57 - share is in use */
	MDEC_NET_SSL_CERTPROBLEM,       /* CURLE_SSL_CERTPROBLEM 58 */
	MDEC_NET_SSL_CIPHER,            /* CURLE_SSL_CIPHER 59 - couldn't use specified cipher */
	MDEC_NET_SSL_CACERT,            /* CURLE_SSL_CACERT 60 - problem with the CA cert (path?) */
	MDEC_NET_UNKNOWN_ERROR,         /* CURLE_BAD_CONTENT_ENCODING 61 */
	MDEC_NET_UNSUPPORTED,           /* CURLE_LDAP_INVALID_URL 62 - Invalid LDAP URL */
	MDEC_NET_UNKNOWN_ERROR,         /* CURLE_FILESIZE_EXCEEDED 63 - Maximum file size exceeded */
	MDEC_NET_SSL_FTP_ERROR,         /* CURLE_FTP_SSL_FAILED 64 - Requested FTP SSL level failed */
	MDEC_NET_UNKNOWN_ERROR,         /* CURLE_SEND_FAIL_REWIND 65 */
	MDEC_NET_SSL_ENGINE_INITFAILED, /* CURLE_SSL_ENGINE_INITFAILED 66 */
	MDEC_NET_UNKNOWN_ERROR,         /* CURLE_LOGIN_DENIED 67 */
	MDEC_NET_UNSUPPORTED,           /* CURLE_TFTP_NOTFOUND 68 - file not found on server */
	MDEC_NET_UNSUPPORTED,           /* CURLE_TFTP_PERM 69 - permission problem on server */
	MDEC_NET_UNSUPPORTED,           /* CURLE_TFTP_DISKFULL 70 - out of disk space on server */
	MDEC_NET_UNSUPPORTED,           /* CURLE_TFTP_ILLEGAL 71 - Illegal TFTP operation */
	MDEC_NET_UNSUPPORTED,           /* CURLE_TFTP_UNKNOWNID 72 - Unknown transfer ID */
	MDEC_NET_UNSUPPORTED,           /* CURLE_TFTP_EXISTS 73 - File already exists */
	MDEC_NET_UNSUPPORTED,           /* CURLE_TFTP_NOSUCHUSER 74 - No such user */
	MDEC_NET_UNKNOWN_ERROR,         /* CURLE_CONV_FAILED 75 - conversion failed */
	MDEC_NET_UNKNOWN_ERROR,         /* CURLE_CONV_REQD 76 - caller must register conversion */
	MDEC_NET_SSL_CACERT_BADFILE,    /* CURLE_SSL_CACERT_BADFILE 77 */
	MDEC_NET_UNSUPPORTED,           /* CURL_LAST never use! */
};


void ResourceError::platformLazyInit()
{
	if(m_dataIsUpToDate)
		return;
	//m_errorCode==0 means no error
	if(m_errorCode == 0)
		return;
	//printf("uri:%s\n",m_failingURL.utf8().data());
	if (m_type == HttpNetError)
		m_errorCode +=  MDEC_NET_HTTP_BASE;
	else if (m_type == FtpNetError)
		m_errorCode +=  MDEC_NET_FTP_BASE;
	else if (m_type == CurlNetError) {
		if(m_errorCode > 0 && (unsigned int)m_errorCode < sizeof(NetworkErrorCode)/sizeof(int))
			m_errorCode = NetworkErrorCode[m_errorCode];
	}
	m_dataIsUpToDate = true;
}


//
// Created by mojiajun on 2020/3/6.
//
#include "http_context.h"

using namespace tinyreactor;

// return false if any error
bool HttpContext::parseRequestFromBuffer(Buffer *buf) {
    bool ok = true;
    bool hasMore = true;
    while (hasMore) {
        if (http_req_parse_state_ == kExpectRequestLine) {
            const char *crlf = buf->findCRLF();
            if (crlf) {
                ok = processRequestLine(buf->peek(), crlf);
                if (ok) {
                    buf->retrieveUntil(crlf + 2);
                    http_req_parse_state_ = kExpectHeaders;
                } else {
                    hasMore = false;
                }
            } else {
                hasMore = false;
            }
        } else if (http_req_parse_state_ == kExpectHeaders) {
            const char *crlf = buf->findCRLF();
            if (crlf) {
                const char *colon = std::find(buf->peek(), crlf, ':');
                if (colon != crlf) {
                    request_.addHeader(buf->peek(), colon, crlf);
                } else {
                    // empty line, end of header
                    // FIXME:
                    http_req_parse_state_ = kGotAll;
                    hasMore = false;
                }
                buf->retrieveUntil(crlf + 2);
            } else {
                hasMore = false;
            }
        } else if (http_req_parse_state_ == kExpectBody) {
            // FIXME:
        }
    }
    return ok;
}

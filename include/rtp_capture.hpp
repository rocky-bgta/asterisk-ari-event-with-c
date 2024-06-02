// rtp_capture.hpp
#ifndef RTP_CAPTURE_HPP
#define RTP_CAPTURE_HPP

#include <string>

void startRtpCapture(const std::string& iface, const std::string& filter_exp);

#endif // RTP_CAPTURE_HPP
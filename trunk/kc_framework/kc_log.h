#pragma once

#include <string>
#include <boost/log/trivial.hpp>
using std::string;

#define KC_WRITE_LOG(level) BOOST_LOG_SEV((MyLog::s_slg),(level))

// ��ʹ��֮ǰ�����ȵ��� init
// ʹ�÷�ʽ  KC_WRITE_LOG(log::trivial::info) << "test string";

namespace KC
{
    class MyLog
    {
    public:
      MyLog() = delete;

      // ��ʹ��֮ǰ�����ȵ��ô˺���
      static void Init(const char*, boost::log::trivial::severity_level);
      static boost::log::sources::severity_logger<boost::log::trivial::severity_level> s_slg;
    };
}

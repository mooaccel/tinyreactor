//
// Created by mojiajun on 2020/3/6.
//

#include "glog/logging.h"
#include "src/channel.h"

using namespace tinyreactor;

int main(int argc, char *argv[]) {
    LOG(INFO) << "Channel::kNoneEvent = " << Channel::kNoneEvent;
    LOG(INFO) << "Channel::kReadEvent = " << Channel::kReadEvent;
    LOG(INFO) << "Channel::kWriteEvent = " << Channel::kWriteEvent;
}

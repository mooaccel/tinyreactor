//
// Created by Mo Jiajun on 2019/9/13.
//
#ifndef SINGLE_THREAD_REACTOR_SRC_BUFFER_H_
#define SINGLE_THREAD_REACTOR_SRC_BUFFER_H_

#include <algorithm>
#include <cassert>
#include <string>
#include <vector>

namespace tinyreactor {

/// Buffer的有些细节还需要探究
class Buffer {
 public:
  static const size_t kCheapPrepend = 8;
  static const size_t kInitialSize = 1024;

  explicit Buffer(size_t initialSize = kInitialSize)
      : buffer_(kCheapPrepend + initialSize),
        readerIndex_(kCheapPrepend),
        writerIndex_(kCheapPrepend) {
      assert(readableBytes() == 0);
      assert(writableBytes() == initialSize);
      assert(prependableBytes() == kCheapPrepend);
  }

  size_t prependableBytes() const {
      return readerIndex_;
  }
  size_t readableBytes() const {
      return writerIndex_ - readerIndex_;
  }
  size_t writableBytes() const {
      return buffer_.size() - writerIndex_;
  }

  /// 返回指向readable范围内的第一个字节的指针
  const char *peek() const { return begin() + readerIndex_; }

  /// 功能描述?
  /// TODO, 单元测试, 待写
  const char *findCRLF() const {
      // FIXME: replace with memmem()?
      // 在[peek(), beginWrite())范围内搜寻
      const char *crlf = std::search(peek(), beginWrite(), kCRLF, kCRLF + 2);
      return crlf == beginWrite() ? nullptr : crlf;
  }

  const char *findCRLF(const char *start) const {
      assert(peek() <= start);
      assert(start <= beginWrite());
      // FIXME: replace with memmem()?
      const char *crlf = std::search(start, beginWrite(), kCRLF, kCRLF + 2);
      return crlf == beginWrite() ? nullptr : crlf;
  }

  /// 获取len个字节从readableBytes中
  void retrieve(size_t len) {
      assert(len <= readableBytes());
      if (len < readableBytes()) {
          readerIndex_ += len;
      } else {
          retrieveAll();
      }
  }

  // end相当于是尾后指针?它指向的位置不需要获取吧?
  void retrieveUntil(const char* end)
  {
      assert(peek() <= end);
      assert(end <= beginWrite());
      retrieve(end - peek());
  }

  void retrieveAll() {
      readerIndex_ = kCheapPrepend;
      writerIndex_ = kCheapPrepend;
  }

  void append(const char *data, size_t len) {
      ensureWritableBytes(len);
      std::copy(data, data + len, beginWrite());
      hasWritten(len);
  }

  void append(const std::string &str) {
      append(str.data(), str.size());
  }

  void hasWritten(size_t len) {
      assert(len <= writableBytes());
      writerIndex_ += len;
  }

  void ensureWritableBytes(size_t len) {
      if (writableBytes() < len) {
          makeSpace(len);
      }
      assert(writableBytes() >= len);
  }

  std::string retrieveAllAsString() {
      return retrieveAsString(readableBytes());
  }

  std::string retrieveAsString(size_t len) {
      assert(len <= readableBytes());
      std::string result(peek(), len);
      retrieve(len);
      return result;
  }

  char *beginWrite() { return begin() + writerIndex_; }
  const char *beginWrite() const { return begin() + writerIndex_; }

  /// TODO
  ssize_t readFd(int fd, int *savedErrno);

 private:
  /// 返回指向第一个元素的指针
  char *begin() { return &*buffer_.begin(); }

  const char *begin() const { return &*buffer_.begin(); }

  void makeSpace(size_t len) {
      if (writableBytes() + prependableBytes() < len + kCheapPrepend) {  // 真的扩张vector<char>.
          // FIXME: move readable data
          buffer_.resize(writerIndex_ + len);
      } else {
          // move readable data to the front, make space inside buffer
          assert(kCheapPrepend < readerIndex_);
          size_t readable = readableBytes();
          std::copy(begin() + readerIndex_,
                    begin() + writerIndex_,
                    begin() + kCheapPrepend);
          readerIndex_ = kCheapPrepend;
          writerIndex_ = readerIndex_ + readable;
          assert(readable == readableBytes());
      }
  }

  std::vector<char> buffer_;
  size_t readerIndex_;
  size_t writerIndex_;

  /// TODO, 添加描述
  static const char kCRLF[];
};

}
#endif //SINGLE_THREAD_REACTOR_SRC_BUFFER_H_

#pragma once
#include <stdint.h>
#include <string>
#include <memory>
#include <map>
#include <functional>
#include <google/protobuf/message.h>

//TODO:
//  1. len and nameLen should be network byte-order.

// ProtobufCodec Format:
//    struct ProtobufTransportFormat __attribute__ ((__packed__))
//    {
//      uint32_t  len;
//      uint32_t  nameLen;
//      char     typeName[nameLen];
//      char     protobufData[len-nameLen-8];
//    }

class ProtobufCodec
{
 public:
  typedef std::shared_ptr<google::protobuf::Message> MessagePtr;
  typedef void (*Callback)(google::protobuf::Message *);

  ProtobufCodec()
  {}

  int pack(uint8_t *buf, uint32_t buf_len, const google::protobuf::Message &msg);
  int unpack(uint8_t *buf, uint32_t buf_len);
  void dump(uint8_t *buf, uint32_t buf_len);

  template<typename MSGTYPE>
  void register_handler(void(*callback)(MSGTYPE *))
  {
    const google::protobuf::Descriptor *desp = MSGTYPE::descriptor();
    if (desp)
    {
      DespMap_[desp->full_name()] = desp;
      CallbackMap_[desp->full_name()] = reinterpret_cast<Callback>(callback);
    }
  }
  
 private:
  google::protobuf::Message* createMessage(const std::string& typeName);
  std::map<std::string, const google::protobuf::Descriptor *> DespMap_;
  std::map<std::string, Callback> CallbackMap_;
};
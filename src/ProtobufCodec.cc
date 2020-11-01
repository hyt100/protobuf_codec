#include "ProtobufCodec.h"
#include <stdio.h>
#include <string.h>
#include <iostream>

void ProtobufCodec::dump(uint8_t *buf, uint32_t buf_len)
{
  uint32_t *p = reinterpret_cast<uint32_t *>(buf);
  uint32_t total_len = *p;
  uint32_t name_len = *(p + 1);
  uint32_t protobuf_len = total_len - name_len - 8;
  uint8_t *name_start = buf + 8;
  uint8_t *protobuf_start = buf + 8+ name_len;

  printf("total: %u \n", total_len);
  printf("name: %s \n", name_start);
  printf("protobuf: %u \n\t", protobuf_len);
  for (int i=0; i<protobuf_len; ++i)
    printf("%x ", protobuf_start[i]);
  printf("\n");
}

int ProtobufCodec::pack(uint8_t *buf, uint32_t buf_len, const google::protobuf::Message &msg)
{
  const std::string &name = msg.GetTypeName();
  uint32_t name_len = name.size() + 1; /* include '\0' */
  uint32_t byte_len = msg.ByteSizeLong();
  uint32_t total_len = sizeof(uint32_t) * 2 + name_len + byte_len;

  if (total_len > buf_len)
    return -1;

  uint8_t *start = buf;
  memcpy(start, &total_len, sizeof(uint32_t));
  start += sizeof(uint32_t);

  memcpy(start, &name_len, sizeof(uint32_t));
  start += sizeof(uint32_t);

  memcpy(start, name.c_str(), name_len);
  start += name_len;

  bool ret = msg.SerializeToArray(start, static_cast<int>(byte_len));
  if (ret == false)
    return -1;
  return total_len;
}

google::protobuf::Message* ProtobufCodec::createMessage(const std::string& typeName)
{
  google::protobuf::Message* message = nullptr;

  auto it = DespMap_.find(typeName);
  if (it == DespMap_.end()) {
    //std::cout<<"find failed"<<std::endl;
    return nullptr;
  }

  const google::protobuf::Descriptor* descriptor = it->second;
  if (descriptor)
  {
    const google::protobuf::Message* prototype =
      google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
    if (prototype)
    {
      message = prototype->New();
    }
  }
  return message;
}

int ProtobufCodec::unpack(uint8_t *buf, uint32_t buf_len)
{
  uint32_t *p = reinterpret_cast<uint32_t *>(buf);
  uint32_t total_len = *p;
  uint32_t name_len = *(p + 1);
  uint32_t protobuf_len = total_len - name_len - 8;
  uint8_t *name_start = buf + 8;
  uint8_t *protobuf_start = buf + 8+ name_len;

  std::string name(reinterpret_cast<const char *>(name_start), name_len-1); /* note: exclude '\0' */

  MessagePtr message;
  message.reset(createMessage(name));
  if (message)
  {
    bool ret = message->ParseFromArray(protobuf_start, static_cast<int>(protobuf_len));
    if (!ret)
      return -1;
    Callback cb = CallbackMap_[name];
    if (cb != nullptr)
      (*cb)(message.get());  // ???
    return 0;
  }

  return -1;
}
#include <stdio.h>
#include <iostream>
#include <vector>
#include "ProtobufCodec.h"
#include "addressbook.pb.h"

using std::cout;
using std::endl;

void mycalllback(tutorial::AddressBook *book)
{
  cout<<"callback ok!!! "<<endl<<endl;
  for (int i=0; i<book->people_size(); ++i)
  {
    const tutorial::Person &people = book->people(i);
    cout<<"name: "<<people.name()<<endl;
    cout<<"id: "<<people.id()<<endl;
    cout<<"email: "<<people.email()<<endl;

    for (int j=0; j<people.phones_size(); ++j)
    {
      const tutorial::Person_PhoneNumber &phone = people.phones(j);
      cout<<"phone number: "<<phone.number()<<endl;
      cout<<"phone type: "<<phone.type()<<endl;
    }

    cout<<endl;
  }
}

int main(int argc, char *argv[])
{
  ProtobufCodec codec;
  codec.register_handler(mycalllback);

  std::vector<uint8_t> bufvec(300);
  
  {
    tutorial::AddressBook book;
    
    tutorial::Person *people1 = book.add_people();
    people1->set_id(1);
    people1->set_name("erick");
    people1->set_email("ccc@126.com");
    tutorial::Person_PhoneNumber *phone1 = people1->add_phones();
    phone1->set_type(tutorial::Person_PhoneType_HOME);
    phone1->set_number("13490940398");

    tutorial::Person *people2 = book.add_people(); //email is defalut empter string, PhoneNumber is none.
    people2->set_id(2);
    people2->set_name("jack");

    /* tutorial::AddressBook::descriptor()->full_name() is the same of msg->GetTypeName() */
    // google::protobuf::Message *msg = &book;
    // cout<<book.GetTypeName()<<endl;
    // cout<<tutorial::AddressBook::descriptor()->full_name()<<endl;
    // cout<<msg->GetTypeName()<<endl;

    int len = codec.pack(bufvec.data(), bufvec.size(), book);
    if (len <= 0) {
      cout << "pack failed" << endl;
      return -1;
    }
    codec.unpack(bufvec.data(), len);
  }

  return 0;
}
/*
#include "../rapidjson/document.h"
#include "../rapidjson/writer.h"
#include <iostream>

using namespace rapidjson;

void sample_basic() {
    // 1. Parse a JSON string into DOM.
    const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";
    Document d;
    d.Parse(json);

    // 2. Modify it by DOM.
    Value& s = d["stars"];
    s.SetInt(s.GetInt() + 1);

    // 3. Stringify the DOM
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    // Output {"project":"rapidjson","stars":11}
    std::cout << buffer.GetString() << std::endl;
}

void sample_other() {
    // document is the root of a json message
    rapidjson::Document document;

    // define the document as an object rather than an array
    document.SetObject();

    // create a rapidjson array type with similar syntax to std::vector
    rapidjson::Value array(rapidjson::kArrayType);

    // must pass an allocator when the object may need to allocate memory
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

    // chain methods as rapidjson provides a fluent interface when modifying its objects
    array.PushBack("hello", allocator).PushBack("world", allocator);//"array":["hello","world"]

    document.AddMember("Name", "XYZ", allocator);
    document.AddMember("Rollnumer", 2, allocator);
    document.AddMember("array", array, allocator);

    // create a rapidjson object type
    rapidjson::Value object(rapidjson::kObjectType);
    object.AddMember("Math", "50", allocator);
    object.AddMember("Science", "70", allocator);
    object.AddMember("English", "50", allocator);
    object.AddMember("Social Science", "70", allocator);
    document.AddMember("Marks", object, allocator);
    //	fromScratch["object"]["hello"] = "Yourname";

    StringBuffer strbuf;
    Writer<StringBuffer> writer(strbuf);
    document.Accept(writer);

    std::cout << strbuf.GetString() << std::endl;
}

void sample_sendapi() {
    Document d;
    Document::AllocatorType& alloc = d.GetAllocator();

    std::string mytext = "this is my text";

    d.SetObject();

    d.AddMember("FromEmail", "sender@gmail.com", alloc);
    d.AddMember("FromName", "Sender's name", alloc);
    d.AddMember("Subject", "My subject", alloc);


    {
        Value recipients(kArrayType);

        {
            Value recipient(kObjectType);
            recipient.AddMember("Email", "recipient@gmail.com", alloc);
            recipients.PushBack(recipient, alloc);
        }

        d.AddMember("Recipients", recipients, alloc);
    }

    {
        Value textPart;
        textPart.SetString(mytext.c_str(), alloc);
        d.AddMember("Text-part", textPart, alloc);
    }

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    std::cout << buffer.GetString() << std::endl;
}

int main() {
    sample_basic();

    sample_other();

    sample_sendapi();

    return 0;
}*/

#include "../rapidjson/document.h"
#include "../rapidjson/writer.h"
#include "../rapidjson/filewritestream.h"
#include <fstream>
#include <iostream>

/*
using namespace rapidjson;


int main() {

*/
/*const char* json = "{\"hello\":\"world\",\"t\":true}";
    rapidjson::Document d;
    d.Parse(json);

    assert(d.HasMember("hello"));
    printf("hello = %s\n", d["hello"].GetString());*//*



    // Create the JSON document
    Document d;
    d.SetObject();

    // Add data to the JSON document
    d.AddMember("name", "Geek", d.GetAllocator());
    d.AddMember("age", 30, d.GetAllocator());

    // Open the output file
    //std::ofstream file("example.json");
    FILE *file = fopen("example.json", "wb"); // non-Windows use "w"

    // Write the JSON data to the file
    char buffer[65536];
    FileWriteStream os(file, buffer, sizeof(buffer));
    Writer<FileWriteStream> writer(os);
    d.Accept(writer);

    return 0;
}
*/

/*

#include "../rapidjson/filereadstream.h"

int main() {
    // Read JSON file
    FILE *fp = fopen("data.json", "rb"); // non-Windows use "r"
    char readBuffer[65536];
    FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    Document d;
    d.ParseStream(is);
    fclose(fp);

    // Modify the JSON data
    Value &s = d["name"];
    s.SetString("geek geeks", d.GetAllocator());

    // Write JSON file
    FILE *fp2 = fopen("data_modified.json",
                      "wb"); // non-Windows use "w"
    char writeBuffer[65536];
    FileWriteStream os(fp2, writeBuffer,
                       sizeof(writeBuffer));
    Writer<FileWriteStream> writer(os);
    d.Accept(writer);
    fclose(fp2);

    return 0;
}*/

#include "../rapidjson/document.h"
#include "../rapidjson/writer.h"
#include "../rapidjson/stringbuffer.h"
#include "PasswordManager.h"
#include <iostream>

/*
using namespace rapidjson;

int main() {
    // 1. Parse a JSON string into DOM.
    const char* json = R"({"project":"rapidjson","stars":10})";
    Document d;
    d.Parse(json);

    // 2. Modify it by DOM.
    Value& s = d["stars"];
    s.SetInt(s.GetInt() + 1);

    // 3. Stringify the DOM
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    // Output {"project":"rapidjson","stars":11}
    std::cout << buffer.GetString() << std::endl;
    return 0;
}*/

#include <string>

using namespace std;

int main() {
    PasswordManager manager;
    manager.startup();



/*cout << "Enter username: " << endl;
    string username;
    cin >> username;
    PasswordManager manager(username);
    manager.load();*/
}
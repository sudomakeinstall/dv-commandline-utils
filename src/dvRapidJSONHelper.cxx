#include <dvRapidJSONHelper.h>
#include <string>

namespace dv {

void
check_and_set_int(const rapidjson::Document& doc, int& a, const std::string& s)
{
  if (doc.HasMember(s.c_str()) && doc[s.c_str()].IsInt())
    a = doc[s.c_str()].GetInt();
};

void
check_and_set_uint(const rapidjson::Document& doc,
                   unsigned int& a,
                   const std::string& s)
{
  if (doc.HasMember(s.c_str()) && doc[s.c_str()].IsUint())
    a = doc[s.c_str()].GetUint();
};

void
check_and_set_bool(const rapidjson::Document& doc,
                   bool& a,
                   const std::string& s)
{
  if (doc.HasMember(s.c_str()) && doc[s.c_str()].IsBool())
    a = doc[s.c_str()].GetBool();
};

void
check_and_set_double(const rapidjson::Document& doc,
                     double& a,
                     const std::string& s)
{
  if (doc.HasMember(s.c_str()) && doc[s.c_str()].IsDouble())
    a = doc[s.c_str()].GetDouble();
};

void
check_and_set_double_array(const rapidjson::Document& doc,
                           double a[3],
                           std::string s)
{
  if (doc.HasMember(s.c_str()) && doc[s.c_str()].IsArray() &&
      (3 == doc[s.c_str()].Size())) {
    for (size_t i = 0; i < 3; ++i)
      a[i] = doc[s.c_str()][i].GetDouble();
  }
};

}

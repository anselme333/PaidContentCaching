//Content Controller (CC) implementation
#include <ndn-cxx/util/signal.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <ndn-cxx/interest.hpp>
#include <ndn-cxx/name.hpp>
#include <iostream>
#include <map>
#include <unordered_map>
#include <string>
#include "cs-internal.hpp"
#include "ns3/string.h"
#include "name-tree-entry.hpp"
using namespace std;
namespace nfd
{
class CC_D
{
public:
 std::string ContentName;
 Name m_contentname;
 shared_ptr<Face> m_face;

 CC_D(std::string contentname)
 {
ContentName = contentname;
 }

bool operator==(const CC_D &contentname) const
{
return ContentName==contentname.ContentName;
}
};

class CC_I
{

public:
std::string ContentName;
std::string faces;

CC_I(std::string contentname, std::string Faces)
{
	faces=Faces;
	ContentName = contentname;
	}

bool operator==(const CC_I &ManagedNode) const
{
return (faces== ManagedNode.faces && ContentName== ManagedNode.ContentName);
}
};

class MakeCC
{
public:
size_t operator()(const CC_I &ManagedNode) const
{
return std::hash<std::string>()(ManagedNode.faces) ^ std::hash<std::string>()(ManagedNode.ContentName);
}
};
}

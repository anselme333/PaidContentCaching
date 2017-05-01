#include "cc.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <ndn-cxx/util/signal.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <ndn-cxx/data.hpp>
#include <ndn-cxx/interest.hpp>
#include <ndn-cxx/name.hpp>
#include <iostream>
#include <map>
#include <unordered_map>
#include <string>
#include "cs-internal.hpp"
#include "ns3/string.h"
#include "name-tree-entry.hpp"
#include "pit-entry.hpp"
#include "pit-face-record.hpp"
#include "pit-in-record.hpp"
using boost::unordered::unordered_map;
using nfd::pit::Entry;
using nfd::pit::FaceRecord;
using std::multimap;
using namespace std;
/*
namespace nfd
{

int main()
{
Name m_contentname;
const Interest interest;
Entry name1(interest);
m_contentname=name1.getName();

shared_ptr<Face> m_face;
shared_ptr<Face> face;
FaceRecord faces(face);
m_face=faces.getFace();

boost::unordered_map<CC_I,CC_D,MakeCC> ContentController;

std::string ContentN=std::to_string(m_contentname);
std::string ManagedFace=std::to_string(m_face);

CC_I FACE0(ManagedFace,ContentN);
CC_D CONTENTN(ContentN);
ContentController.emplace(FACE0, CONTENTN);

auto search = ContentController.find(ContentN,faces,ContentN);
if(search !=ContentController.end())
{

}
  else
   {

   }

for(auto &itr : ContentController)
{
cout << itr.first.ContentName << " " << itr.first.faces<< ": " << itr.second.ContentName<< endl;
}
return 0;
}
} // namespace nfd

*/

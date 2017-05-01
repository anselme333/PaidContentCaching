/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2014-2015,  Regents of the University of California,
 *                           Arizona Board of Regents,
 *                           Colorado State University,
 *                           University Pierre & Marie Curie, Sorbonne University,
 *                           Washington University in St. Louis,
 *                           Beijing Institute of Technology,
 *                           The University of Memphis.
 *
 * This file is part of NFD (Named Data Networking Forwarding Daemon).
 * See AUTHORS.md for complete list of NFD authors and contributors.
 *
 * NFD is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * NFD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * NFD, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "controller-strategy.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <string>
//#include "ns3/ndnSIM/NFD/daemon/table/cc.hpp"
#include <unordered_map>
using boost::unordered::unordered_map;
using nfd::pit::Entry;
using nfd::pit::FaceRecord;
using std::multimap;
namespace nfd {
namespace fw {

const Name ControllerStrategy::STRATEGY_NAME("ndn:/localhost/nfd/strategy/controller/%FD%01");
NFD_REGISTER_STRATEGY(ControllerStrategy);

ControllerStrategy::ControllerStrategy(Forwarder& forwarder, const Name& name)
  : Strategy(forwarder, name)
{
}

void
ControllerStrategy::afterReceiveInterest(const Face& inFace,
                   const Interest& interest,
                   shared_ptr<fib::Entry> fibEntry,
                   shared_ptr<pit::Entry> pitEntry)
{
/****************************************************************************/

//Added by anselme
 Name  ContentName;
 ContentName =interest.getName();
 std::string ContentN=std::to_string(ContentName);
 boost::unordered_map<CC_I,CC_D,MakeCC> ContentController;
 shared_ptr<Face> m_face;
 shared_ptr<Face> face;
 FaceRecord faces(face);
 m_face=faces.getFace();
 std::string ManagedFace=std::to_string(m_face);
 CC_I FACE0(ManagedFace,ContentN);
 CC_D CONTENTN(ContentN);

 for(auto &itr : ContentController)
 {
	std::string cachedContent=std::to_string(itr.first.ContentName) ;
	std::string cachedFace= std::to_string(itr.first.faces);
	if(cachedContent==ContentN)
	{

    const fib::NextHopList& nexthops0 = fibEntry->getNextHops();
	for (fib::NextHopList::const_iterator it0 = nexthops0.begin(); it0 != nexthops0.end(); ++it0) {
	shared_ptr<Face> outFace0 = m_face;
	 if (pitEntry->canForwardTo(*outFace0)) {
	      this->sendInterest(pitEntry, m_face);
	    }

	  }

	}
    else
	{
		    	 break;
     }

	}
ContentController.emplace(FACE0, CONTENTN);
const fib::NextHopList& nexthops1 = fibEntry->getNextHops();
for (fib::NextHopList::const_iterator it1 = nexthops1.begin(); it1 != nexthops1.end(); ++it1) {
shared_ptr<Face> outFace1 = it1->getFace();
 if (pitEntry->canForwardTo(*outFace1)) {
      this->sendInterest(pitEntry, outFace1);
    }

  }

  if (!pitEntry->hasUnexpiredOutRecords()) {
    this->rejectPendingInterest(pitEntry);
  }
}

} // namespace fw
} // namespace nfd

/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2011-2015  Regents of the University of California.
 *
 * This file is part of ndnSIM. See AUTHORS for complete list of ndnSIM authors and
 * contributors.
 *
 * ndnSIM is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * ndnSIM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ndnSIM, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 **/

// Modified by Anselme

#ifndef PRICE_BASED_POLICY_H_
#define PRICE_BASED_POLICY_H_

/// @cond include_hidden
#include <boost/intrusive/options.hpp>
#include <boost/intrusive/list.hpp>
#include "ns3/ndnSIM/model/ndn-net-device-face.hpp"
#include "ns3/ndnSIM/model/ndn-l3-protocol.hpp"
#include "ns3/net-device.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/ndnSIM/utils/ndn-rtt-estimator.hpp"
#include <ctime>
#include <chrono>
#include <sys/time.h>

namespace ns3 {
namespace ndn {
namespace ndnSIM {

/**
 * @brief price based replacement policy
 */
struct price_based_traits {
  /// @brief Name that can be used to identify the policy (for NS-3 object model and logging)
  static std::string
  GetName()
  {
    return "Pbr";
  }

  struct policy_hook_type : public boost::intrusive::set_member_hook<> {
    Time timeWhenShouldExpire;
  };

  template<class Container>
  struct container_hook {
    typedef boost::intrusive::member_hook<Container, policy_hook_type, &Container::policy_hook_>
      type;
  };

  template<class Base, class Container, class Hook>
  struct policy {
    static Time&
    get_freshness(typename Container::iterator item)
    {
      return static_cast<typename policy_container::value_traits::hook_type*>(
               policy_container::value_traits::to_node_ptr(*item))->timeWhenShouldExpire;
    }

    static const Time&
    get_freshness(typename Container::const_iterator item)
    {
      return static_cast<const typename policy_container::value_traits::hook_type*>(
               policy_container::value_traits::to_node_ptr(*item))->timeWhenShouldExpire;
    }

    template<class Key>
    struct MemberHookLess {
      bool
      operator()(const Key& a, const Key& b) const
      {
        return get_freshness(&a) < get_freshness(&b);
      }
    };

    typedef boost::intrusive::multiset<Container,
                                       boost::intrusive::compare<MemberHookLess<Container>>,
                                       Hook> policy_container;

    class type : public policy_container {
    public:
      typedef policy policy_base; // to get access to get_freshness methods from outside
      typedef Container parent_trie;

      type(Base& base)
        : base_(base)
        , max_size_(100)
      {
      }
      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // added by anselme

            inline uint32_t  get_link_capacity(shared_ptr< Face > face)
                 {
          	  if(ns3::ndn::NetDeviceFace *netf = dynamic_cast<ns3::ndn::NetDeviceFace*>(&(*face)))
          	  {
          	    //-256 because 0-255 is reserved by ndn local/app faces;
          	    ns3::Ptr<ns3::PointToPointNetDevice> nd1 = netf->GetNetDevice()->GetNode ()->GetDevice(netf->getId () - 256)->GetObject<ns3::PointToPointNetDevice>();
          	    ns3::DataRateValue dv;
          	    nd1->GetAttribute("DataRate", dv);
          	    ns3::DataRate d = dv.Get();
          	    link_capacity = double(d.GetBitRate());
          	    transit_fee_1mbps = 0.63;  //average transit fee per Mbps
          	    linkcost = link_capacity * transit_fee_1mbps;
          	    return linkcost;
          	  }
          	  else
          	    {
          	      return 0;
          	    }
                 }
            inline uint32_t content_price(shared_ptr<const Data> data)
                       {
          	    content_size = data->getContent().value_size();
          	    m_interestName = data->getName();
          	    seq = std::numeric_limits<uint32_t>::max();
          	    popularity=seq;
          	    if(m_interestName=="/book")
          	    {
          	    	price=15;
          	    	popularity=popularity++;
          	    }
          	  else if (m_interestName=="/song")
          	  {
          		price=5;
          		popularity=popularity++;
          	  }
          	  else
          	  {
          		price= 7;
          		popularity=popularity++;
          	  }

                price_content = content_size * price ;
                return price_content;
                }
            inline uint32_t set_cache_replacement_prameter(size_t max_size)
                             {
            	            nfd::NetworkLayerCounters  n_request;// number of the requests
            	            int  demand_count = n_request.getNInInterests();// Counting Interest
            	            //ns3::ndn::RttEstimator* RTTmax;
            	            //replacement_parameter = double(((price_content * RTTmax->m_maxRto.ToDouble (Time::S)*demand_count ) + linkcost) * (1/RTTmax->m_maxRto.ToDouble (Time::S)));
            	        	replacement_parameter = double(((price_content *demand_count ) + linkcost));
            	            return replacement_parameter;
                             }

      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            inline void
               update(typename parent_trie::iterator item)
               {
                 // do nothing
               }
               inline bool
               insert(typename parent_trie::iterator item)
               {
            	 boost::chrono::duration<double> d(replacement_parameter); // added by Anselme
                 time::milliseconds freshness = boost::chrono::duration_cast<boost::chrono::milliseconds>(d); //Added by anselme
                 if (freshness > time::milliseconds::zero()) {
                   get_freshness(item) = Simulator::Now() + MilliSeconds(freshness.count());

                   // push item only if freshness is non zero. otherwise, this payload is not
                   // controlled by the policy.
                   // Note that .size() on this policy would return only the number of items with
                   // non-infinite freshness policy
                   policy_container::insert(*item);
                 }

                 return true;
               }

               inline void
               lookup(typename parent_trie::iterator item)
               {
                 // do nothing. it's random policy
               }

               inline void
               erase(typename parent_trie::iterator item)
               {
            	boost::chrono::duration<double> d(replacement_parameter);
            	time::milliseconds freshness = boost::chrono::duration_cast<boost::chrono::milliseconds>(d);
                 if (freshness > time::milliseconds::zero())
                 {
                // erase only if freshness is positive (otherwise an item is not in the policy)
                   policy_container::erase(policy_container::s_iterator_to(*item));
                 }
               }
               inline void
               clear()
               {
                 policy_container::clear();
               }

               inline void
               set_max_size(size_t max_size)
               {
                 max_size_ = max_size;
               }

               inline size_t
               get_max_size() const
               {
                 return max_size_;
               }
    public:
      double link_capacity;
      double transit_fee_1mbps;
      double linkcost;
      double content_size;
      double price;
      double price_content;
      double  replacement_parameter;
      uint32_t popularity;
      uint32_t seq;
      nfd::NetworkLayerCounters  interest;
    private:
      type()
        : base_(*((Base*)0)){};
      Name m_interestName;     ///< \brief NDN Name of the Interest (use Name)

    private:
      Base& base_;
      size_t max_size_;


    };
  };
};

} // ndnSIM
} // ndn
} // ns3


#endif //PRICE_BASED_POLICY_H_

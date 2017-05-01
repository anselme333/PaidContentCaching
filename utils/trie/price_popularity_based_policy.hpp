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

// Modified by Anselme for KIISE 2016

#ifndef PRICE_BASED_POLICY_H_
#define PRICE_BASED_POLICY_H_

/// @cond include_hidden
#include <boost/intrusive/options.hpp>
#include <boost/intrusive/list.hpp>
#include "ns3/ndnSIM/model/ndn-net-device-face.hpp"
#include "ns3/ndnSIM/model/ndn-l3-protocol.hpp"
#include "ns3/net-device.h"
#include "ns3/point-to-point-net-device.h"

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
    return "Ppbr";
  }

  struct policy_hook_type : public boost::intrusive::list_member_hook<> {
  };

  template<class Container>
  struct container_hook {
    typedef boost::intrusive::member_hook<Container, policy_hook_type, &Container::policy_hook_>
      type;
  };

  template<class Base, class Container, class Hook>
  struct policy {
    typedef typename boost::intrusive::list<Container, Hook> policy_container;

    // could be just typedef
    class type : public policy_container {
    public:
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
          	      return ULONG_MAX;
          	    }
                 }
            inline uint32_t content_price(shared_ptr<const Data> data)
                       {
          	  content_size = data->getContent().value_size();
                price = data->getPrice();
                price_content = content_size * price ;
                return price_content;
                        }
            inline uint32_t set_cache_replacement_prameter(size_t max_size)
                             {
            	replacement_prameter = price_content/linkcost;
            	return replacement_prameter;
                             }

      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


      inline void
      update(typename parent_trie::iterator item)
      {
        // do relocation
        policy_container::splice(policy_container::end(), *this,
                                 policy_container::s_iterator_to(*item));
      }

      inline bool
      insert(typename parent_trie::iterator item)
      {
        if (max_size_ != 0 && policy_container::size() >= max_size_) {
          base_.erase(&(*policy_container::begin()));
        }

        policy_container::push_back(*item);
        return true;
      }

      inline void
      lookup(typename parent_trie::iterator item)
      {
        // do relocation
        policy_container::splice(policy_container::end(), *this,
                                 policy_container::s_iterator_to(*item));
      }

      inline void
      erase(typename parent_trie::iterator item)
      {
        policy_container::erase(policy_container::s_iterator_to(*item));
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

      static double& get_time (typename Container::iterator item)
          {
            return static_cast<typename policy_container::value_traits::hook_type*>
              (policy_container::value_traits::to_node_ptr(*item))->LiveTime;
          }

      static const double& get_time (typename Container::const_iterator item)
             {
            return static_cast<const typename policy_container::value_traits::hook_type*>
              (policy_container::value_traits::to_node_ptr(*item))->LiveTime;
          }
    public:
      double link_capacity;
      double transit_fee_1mbps;
      double linkcost;
      double content_size;
      double price;
      double price_content;
      double replacement_prameter;

    private:
      type()
        : base_(*((Base*)0)){};

    private:
      Base& base_;
      size_t max_size_;


    };
  };
};

} // ndnSIM
} // ndn
} // ns3

/// @endcond

#endif

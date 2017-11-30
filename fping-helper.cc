/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include "fping-helper.h"
#include "ns3/v4fping.h"
#include "ns3/names.h"

namespace ns3 {

V4FPingHelper::V4FPingHelper (int n, Ipv4Address remote[])
{
  m_factory.SetTypeId ("ns3::V4FPing");
  m_factory.Set ("Remote", Ipv4AddressValue (remote));
    m_factory.Set ("Number", n);


}

void 
V4FPingHelper::SetAttribute (std::string name, const AttributeValue &value)
{
  m_factory.Set (name, value);
}

ApplicationContainer
V4FPingHelper::Install (Ptr<Node> node) const
{
  return ApplicationContainer (InstallPriv (node));
}

ApplicationContainer
V4FPingHelper::Install (std::string nodeName) const
{
  Ptr<Node> node = Names::Find<Node> (nodeName);
  return ApplicationContainer (InstallPriv (node));
}

ApplicationContainer
V4FPingHelper::Install (NodeContainer c,Ipv4InterfaceContainer addresses ) const
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      apps.Add (InstallPriv (*i , addresses));
    }

  return apps;
}

Ptr<Application>
V4FPingHelper::InstallPriv (Ptr<Node> node ,  Ipv4InterfaceContainer addresses) const
{
  Ptr<V4FPing> app = m_factory.Create<V4FPing> (addresses);
  node->AddApplication (app);

  return app;
}

} // namespace ns3

/*
 Copyright (C) 2012 Gabor Papp

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published
 by the Free Software Foundation; either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <memory>
#include <string>

#include "cinder/Cinder.h"

#include "lo/lo.h"

#include "OscCore.h"

namespace mndl { namespace osc {

typedef std::shared_ptr< class Client > ClientRef;

class Client
{
 public:
	static ClientRef create( const std::string &host, int port, Proto proto = PROTO_UDP )
	{ return ClientRef( new Client( host, port, proto ) ); }

	void send( const osc::Message &message );

 protected:
	Client( const std::string &host, int port, Proto proto = PROTO_UDP );

	lo_address mAddress;
};

} } // namespace mndl::osc

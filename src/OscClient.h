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

#include <string>

#include "cinder/Cinder.h"

#include "lo/lo.h"

#include "OscCore.h"

namespace mndl { namespace osc {

class Client
{
 public:
	Client() {}
	Client( std::string host, int port, Proto proto = PROTO_UDP );

	void send( const osc::Message &message );

 protected:
	struct Obj
	{
		Obj() {}
		Obj( std::string host, int port, Proto proto );

		lo_address mAddress;
	};

	std::shared_ptr< Client::Obj > mObj;
};

} } // namespace mndl::osc

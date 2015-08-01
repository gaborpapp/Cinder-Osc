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

#include "boost/lexical_cast.hpp"

#include "OscClient.h"

namespace mndl { namespace osc {

Client::Client( const std::string &host, int port, Proto proto /* = PROTO_UDP */ )
{
	std::string portStr = boost::lexical_cast< std::string >( port );
	mAddress = lo_address_new_with_proto( proto, host.c_str(), portStr.c_str() );
}

void Client::send( const osc::Message &message )
{
	lo_message msg = lo_message_new();

	for ( size_t i = 0; i < message.getNumArgs(); i++ )
	{
		switch ( message.getArgType( i ) )
		{
			case 'i':
				lo_message_add_int32( msg, message.getArg< int32_t >( i ) );
				break;

			case 'f':
				lo_message_add_float( msg, message.getArg< float >( i ) );
				break;

			case 's':
				lo_message_add_string( msg, message.getArg< std::string >( i ).c_str() );
				break;

			default:
				break;
		}
	}

	lo_send_message( mAddress, message.getAddressPattern().c_str(), msg );
	lo_message_free( msg );
}

} } // mndl::osc

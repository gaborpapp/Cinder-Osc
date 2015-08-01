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
#include <vector>
#include <map>
#include <memory>

#include "cinder/Cinder.h"
#include "cinder/Function.h"

#include "lo/lo.h"

#include "OscCore.h"

namespace mndl { namespace osc {

typedef std::shared_ptr< class Server > ServerRef;
typedef bool( Callback )( const osc::Message & );

class Server
{
 public:
	static ServerRef create( int port, Proto proto = PROTO_UDP )
	{ return ServerRef( new Server( port, proto ) ); }

	~Server();

	template < typename T >
	uint32_t registerOscReceived( bool( T::*fn )( const osc::Message & ), T *inst, const std::string &addressPattern = "", const std::string &typeTag = "" )
	{
		std::shared_ptr< std::function< osc::Callback > > callbackPtr( new std::function< osc::Callback >(
					std::bind( fn, inst, std::placeholders::_1 ) ) );
		mOscCallbacks.push_back( callbackPtr );

		const char *addressPatternPtr = addressPattern.empty() ? NULL : addressPattern.c_str();
		const char *typeTagPtr = typeTag.empty() ? NULL : typeTag.c_str();
		lo_server_thread_add_method( mThread, addressPatternPtr, typeTagPtr, implOscCallback, (void *)callbackPtr.get() );

		sCallbackId++;
		mCallbackSpecs[ sCallbackId ] = std::make_pair( addressPattern, typeTag );
		return sCallbackId;
	}

	void unregisterOscReceived( uint32_t callbackId );

	int getPort() const { return mPort; }

 protected:
	Server( int port, Proto proto = PROTO_UDP );

	int mPort;

	static void errorHandler( int num, const char *m, const char *path );

	lo_server_thread mThread;

	static int implOscCallback( const char *path, const char *types, lo_arg **argv, int argc, void *data, void *userData );

	std::vector< std::shared_ptr< std::function< osc::Callback > > > mOscCallbacks;
	static uint32_t sCallbackId;
	std::map< uint32_t, std::pair< std::string, std::string > > mCallbackSpecs;
};

} } // mndl::osc

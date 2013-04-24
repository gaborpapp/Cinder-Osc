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

#include "cinder/Cinder.h"
#include "cinder/Function.h"

#include "lo/lo.h"

#include "OscCore.h"

namespace mndl { namespace osc {

typedef bool( Callback )( const osc::Message & );

class Server
{
	public:
		Server() {}
		Server( int port, Proto proto = PROTO_UDP );

		template < typename T >
		uint32_t registerOscReceived( bool( T::*fn )( const osc::Message & ), T *inst, const std::string &addressPattern = "", const std::string &typeTag = "" )
		{
			std::shared_ptr< std::function< osc::Callback > > callbackPtr( new std::function< osc::Callback >(
						std::bind( fn, inst, std::_1 ) ) );
			mObj->mOscCallbacks.push_back( callbackPtr );

			const char *addressPatternPtr = addressPattern.empty() ? NULL : addressPattern.c_str();
			const char *typeTagPtr = typeTag.empty() ? NULL : typeTag.c_str();
			lo_server_thread_add_method( mObj->mThread, addressPatternPtr, typeTagPtr, mObj->implOscCallback, (void *)callbackPtr.get() );

			Obj::sCallbackId++;
			mObj->mCallbackSpecs[ Obj::sCallbackId ] = std::make_pair( addressPattern, typeTag );
			return Obj::sCallbackId;
		}

		void unregisterOscReceived( uint32_t callbackId );

		int getPort() const { return mObj->mPort; }

	protected:
		struct Obj
		{
			Obj() {}
			Obj( int port, Proto proto );

			int mPort;

			static void errorHandler( int num, const char *m, const char *path );

			lo_server_thread mThread;

			static int implOscCallback( const char *path, const char *types, lo_arg **argv, int argc, void *data, void *userData );

			std::vector< std::shared_ptr< std::function< osc::Callback > > > mOscCallbacks;
			static uint32_t sCallbackId;
			std::map< uint32_t, std::pair< std::string, std::string > > mCallbackSpecs;
		};

		std::shared_ptr< Server::Obj > mObj;
};

} } // mndl::osc


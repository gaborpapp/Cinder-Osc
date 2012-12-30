/*
 Copyright (C) 2012 Gabor Papp

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published
 by the Free Software Foundation; either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "cinder/app/App.h"

#include "boost/lexical_cast.hpp"

#include "OscServer.h"

using namespace std;

namespace mndl { namespace osc {

Server::Server( int port, Proto proto /* = PROTO_UDP */ ) :
	mObj( shared_ptr< Server::Obj >( new Obj( port, proto ) ) )
{
}

void Server::unregisterOscReceived( uint32_t callbackId )
{
	string path = mObj->mCallbackSpecs[ callbackId ].first;
	string typeSpec = mObj->mCallbackSpecs[ callbackId ].second;
	const char *pathPtr = path.empty() ? NULL : path.c_str();
	const char *typeSpecPtr = typeSpec.empty() ? NULL : typeSpec.c_str();

	lo_server_thread_del_method( mObj->mThread, pathPtr, typeSpecPtr );
	mObj->mCallbackSpecs.erase( callbackId );
}

Server::Obj::Obj( int port, Proto proto )
{
	string portStr = boost::lexical_cast< string >( port );
	mThread = lo_server_thread_new_with_proto( portStr.c_str(), proto, errorHandler );
	lo_server_thread_start( mThread );
}

void Server::Obj::errorHandler( int num, const char *msg, const char *path )
{
	ci::app::console() << "liblo server error " << num << " in path " << path << ": " << msg << endl;
}

int Server::Obj::implOscCallback( const char *path, const char *types, lo_arg **argv, int argc, void *data, void *userData )
{
	std::function< osc::Callback > *fn = reinterpret_cast< std::function< osc::Callback >* >( userData );

	Message m( path );
	for ( int i = 0; i < argc; i++ )
	{
		switch ( types[ i ] )
		{
			case 'i':
				m.addArg( argv[ i ]->i32 );
				break;

			case 'f':
				m.addArg( argv[ i ]->f );
				break;

			case 's':
				m.addArg( string( &argv[ i ]->s ) );
				break;

			default:
				break;
		}
	}

	return (*fn)( m ) ? 1 : 0;
}

uint32_t Server::Obj::sCallbackId = 0;

} } // mndl::osc


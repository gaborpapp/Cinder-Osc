/*
 Copyright (C) 2012-2015 Gabor Papp

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "OscServer.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class OscListenerApp : public App
{
 public:
	void setup();

	void update();
	void draw();

 private:
	bool oscReceived( const mndl::osc::Message &message );

	mndl::osc::ServerRef mListener;
};

void OscListenerApp::setup()
{
	mListener = mndl::osc::Server::create( 7770 );

	mListener->registerOscReceived< OscListenerApp >( &OscListenerApp::oscReceived, this );
}

bool OscListenerApp::oscReceived( const mndl::osc::Message &message )
{
	console() << getElapsedSeconds() << " message received " << message.getAddressPattern() << endl;
	for ( size_t i = 0; i < message.getNumArgs(); i++ )
	{
		console() <<  " argument: " << i;
		console() <<  " type: " << message.getArgType( i ) << " value: ";
		switch ( message.getArgType( i ) )
		{
			case 'i':
				console() << message.getArg< int32_t >( i );
				break;

			case 'f':
				console() << message.getArg< float >( i );
				break;

			case 's':
				console() << message.getArg< string >( i );
				break;

			default:
				break;
		}
		console() << endl;
	}
	return false;
}

void OscListenerApp::update()
{
}

void OscListenerApp::draw()
{
	gl::clear();
}

CINDER_APP( OscListenerApp, RendererGl )

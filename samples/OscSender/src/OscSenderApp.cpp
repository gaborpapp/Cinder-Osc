/*
 Copyright (C) 2012 Gabor Papp

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "cinder/Cinder.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/System.h"

#include "OscClient.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class OscSenderApp : public AppBasic
{
	public:
		void setup();

		void update();
		void draw();

		void mouseMove( MouseEvent event );
		void mouseDrag( MouseEvent event );

	private:
		mndl::osc::Client mServer;

		int mMouseX;
};

void OscSenderApp::setup()
{
	mMouseX = 0;
	/*
	string host = System::getIpAddress();
	if ( host.rfind( '.' ) != string::npos )
		host.replace( host.rfind( '.' ) + 1, 3, "255" );
	*/
	mServer = mndl::osc::Client( "127.0.0.1", 7770 );
}

void OscSenderApp::update()
{
	mndl::osc::Message msg( "/cinder/osc/1" );
	msg.addArg( mMouseX );
	mServer.send( msg );
}

void OscSenderApp::draw()
{
	gl::clear( Color::black() );
}

void OscSenderApp::mouseMove( MouseEvent event )
{
	mMouseX = event.getX();
}

void OscSenderApp::mouseDrag( MouseEvent event )
{
	mouseMove( event );
}

CINDER_APP_BASIC( OscSenderApp, RendererGl )

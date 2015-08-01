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

#include "OscClient.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class OscSenderApp : public App
{
 public:
	void setup();

	void update();
	void draw();

	void mouseMove( MouseEvent event );
	void mouseDrag( MouseEvent event );

 private:
	mndl::osc::ClientRef mSender;

	int mMouseX;
};

void OscSenderApp::setup()
{
	mMouseX = 0;
	mSender = mndl::osc::Client::create( "127.0.0.1", 7770 );
}

void OscSenderApp::update()
{
	mndl::osc::Message msg( "/cinder/osc/1" );
	msg.addArg( mMouseX );
	mSender->send( msg );
}

void OscSenderApp::draw()
{
	gl::clear();
}

void OscSenderApp::mouseMove( MouseEvent event )
{
	mMouseX = event.getX();
}

void OscSenderApp::mouseDrag( MouseEvent event )
{
	mouseMove( event );
}

CINDER_APP( OscSenderApp, RendererGl )

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

#include "boost/variant.hpp"

namespace mndl { namespace osc {

typedef enum
{
	PORT_ANY = 0
} Port;

typedef enum
{
	PROTO_UDP = LO_UDP,
	PROTO_TCP = LO_TCP,
	PROTO_UNIX = LO_UNIX
} Proto;

typedef enum
{
	TYPE_INT32 = 'i',
	TYPE_FLOAT = 'f',
	TYPE_STRING = 's'
} ArgType;

typedef boost::variant< int32_t, float, std::string > Argument;

class Message
{
	public:
		Message( const std::string &addressPattern ) : mAddressPattern( addressPattern ), mTypeTag( "" ) {}

		const std::string &getAddressPattern() const
		{
			return mAddressPattern;
		}

		const std::string &getTypeTag() const
		{
			return mTypeTag;
		}

		void addArg( int32_t arg )
		{
			mTypeTag += TYPE_INT32;
			mArguments.push_back( arg );
		}

		void addArg( float arg )
		{
			mTypeTag += TYPE_FLOAT;
			mArguments.push_back( arg );
		}

		void addArg( const std::string &arg )
		{
			mTypeTag += TYPE_STRING;
			mArguments.push_back( arg );
		}

		template< typename T>
		T getArg( int32_t idx ) const
		{
			return boost::get< T >( mArguments[ idx ] );
		}

		const char getArgType( int32_t idx ) const
		{
			return mTypeTag.at( idx );
		}

		const std::vector< Argument > &getArgs() const
		{
			return mArguments;
		}

		size_t getNumArgs() const
		{
			return mArguments.size();
		}

		friend std::ostream& operator<<( std::ostream &lhs, const Message &rhs )
		{
			lhs << rhs.mAddressPattern << " " << rhs.mTypeTag;
			for ( std::vector< Argument >::const_iterator it = rhs.mArguments.begin(); it != rhs.mArguments.end(); ++it )
				lhs << " " << *it;
			return lhs;
		}

	protected:
		std::string mAddressPattern;
		std::vector< Argument > mArguments;
		std::string mTypeTag;
};

} } // namespace mndl::osc


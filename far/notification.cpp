#pragma once

/*
notification.cpp

*/
/*
Copyright � 2013 Far Group
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. The name of the authors may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "headers.hpp"
#pragma hdrstop

#include "notification.hpp"

listener::listener(inotification& n):
	m_notification(n)
{
	m_notification.subscribe(this);
}

listener::~listener()
{
	m_notification.unsubscribe(this);
}


void inotification::dispatch()
{
	while(!m_events.empty())
	{
		auto p = m_events.front().get();
		std::for_each(RANGE(m_listeners, i)
		{
			i->callback(*p);
		});
		m_events.pop();
	}
}


void notifier::dispatch()
{
	std::for_each(RANGE(m_notifications, i)
	{
		i.second->dispatch();
	});
}

void notifier::add(inotification* i)
{
	m_notifications.insert(VALUE_TYPE(m_notifications)(i->name(), VALUE_TYPE(m_notifications)::second_type(i)));
}

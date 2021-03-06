// Copyright (c) 2011-2013, Cornell University
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright notice,
//       this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of WTF nor the names of its contributors may be
//       used to endorse or promote products derived from this software without
//       specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef wtf_client_message_hyperdex_del_h_
#define wtf_client_message_hyperdex_del_h_

// e
#include <e/intrusive_ptr.h>

//WTF  
#include "client/client.h"
#include "client/message.h"

namespace wtf __attribute__ ((visibility("hidden")))
{

class message_hyperdex_del : public message
{
    public:
        message_hyperdex_del(client* cl, const char* space,
            const char* key);
        virtual ~message_hyperdex_del() throw ();

    public:
        int64_t send();
        hyperdex_client_returncode status() { return m_status; }
        int64_t reqid() { return m_reqid; }

    // refcount
    protected:
        friend class e::intrusive_ptr<message_hyperdex_del>;

    // noncopyable
    private:
        message_hyperdex_del(const message_hyperdex_del& other);
        message_hyperdex_del& operator = (const message_hyperdex_del& rhs);

    // operation state
    private:
        std::string m_space;
        std::string m_key;
        hyperdex_client_returncode m_status;
};

}
#endif // wtf_client_message_hyperdex_del_h_

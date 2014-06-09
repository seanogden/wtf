// Copyright (c) 2013, Cornell University
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

#ifndef wtf_client_pending_aggregation_h_
#define wtf_client_pending_aggregation_h_

// STL
#include <utility>
#include <vector>

// E
#include <e/intrusive_ptr.h>

// WTF
#include "client/pending.h"
#include "client/message.h"

namespace wtf __attribute__ ((visibility("hidden")))
{

class pending_aggregation : public pending
{
    public:
        pending_aggregation(uint64_t client_visible_id,
                            wtf_client_returncode* status);
        virtual ~pending_aggregation() throw ();

    // handle aggregation across servers; must call handle_* messages from
    // subclasses for this to work
    public:
        bool aggregation_done();

    // events
    public:
        virtual void handle_sent_to_wtf(const server_id& si);
        virtual void handle_sent_to_hyperdex(e::intrusive_ptr<message>& msg);
        virtual void handle_wtf_failure(const server_id& si);
        virtual void handle_hyperdex_failure(int64_t reqid);

        // pass NULL for msg; we don't need it
        virtual bool handle_wtf_message(client*,
                                    const server_id& si,
                                    std::auto_ptr<e::buffer> msg,
                                    e::unpacker up,
                                    wtf_client_returncode* status,
                                    e::error* error);

        virtual bool handle_hyperdex_message(client* cl,
                                    int64_t reqid,
                                    hyperdex_client_returncode rc,
                                    wtf_client_returncode* status,
                                    e::error* error);
    // refcount
    protected:
        friend class e::intrusive_ptr<pending_aggregation>;

    // noncopyable
    private:
        pending_aggregation(const pending_aggregation& other);
        pending_aggregation& operator = (const pending_aggregation& rhs);

    private:
        virtual void remove_wtf_message(const server_id& si);
        virtual void remove_hyperdex_message(int64_t reqid);

    private:
        std::vector<server_id> m_outstanding_wtf;
        std::vector<e::intrusive_ptr<message> > m_outstanding_hyperdex;
};

}

#endif // wtf_client_pending_aggregation_h_

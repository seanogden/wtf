// Copyright (c) 2012-2013, Cornell University
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

//hyperdex
#include <hyperdex/client.hpp>

// WTF
#include "client/pending_chmod.h"
#include "common/response_returncode.h"
#include "client/message_hyperdex_put.h"

using wtf::pending_chmod;

pending_chmod :: pending_chmod(client* cl, int64_t client_visible_id, 
                           wtf_client_returncode* status, std::string path,
                           mode_t mode)
    : pending_aggregation(client_visible_id, status) 
    , m_cl(cl)
    , m_path(path)
    , m_mode(mode)
    , m_done(false)
{
    set_status(WTF_CLIENT_SUCCESS);
    set_error(e::error());
}

pending_chmod :: ~pending_chmod() throw ()
{
}

bool
pending_chmod :: can_yield()
{
    return this->aggregation_done() && !m_done;
}

bool
pending_chmod :: yield(wtf_client_returncode* status, e::error* err)
{
    *status = WTF_CLIENT_SUCCESS;
    *err = e::error();
    assert(this->can_yield());
    m_done = true;
    return true;
}

void
pending_chmod :: handle_hyperdex_failure(int64_t reqid)
{
    return pending_aggregation::handle_hyperdex_failure(reqid);
}

bool
pending_chmod :: handle_hyperdex_message(client* cl,
                                    int64_t reqid,
                                    hyperdex_client_returncode rc,
                                    wtf_client_returncode* status,
                                    e::error* err)
{
    bool handled = pending_aggregation::handle_hyperdex_message(cl, reqid, rc, status, err);
    assert(handled);

    if (rc != HYPERDEX_CLIENT_SUCCESS)
    {
        PENDING_ERROR(SERVERERROR) << "hyperdex returned " << rc;
    }

    return true;
}

bool
pending_chmod :: try_op()
{
    struct hyperdex_client_attribute attr;
    attr.attr = "mode";
    attr.value = (const char*)&m_mode;
    attr.value_sz = sizeof(mode_t);
    attr.datatype = HYPERDATATYPE_INT64;

    e::intrusive_ptr<message_hyperdex_put> msg = 
        new message_hyperdex_put(m_cl, "wtf", m_path.c_str(), &attr, 1);

    if (msg->send() < 0)
    {
        PENDING_ERROR(IO) << "Couldn't put to HyperDex: " << msg->status();
    }
    else
    {
        m_cl->add_hyperdex_op(msg->reqid(), this);
        e::intrusive_ptr<message> m = msg.get();
        pending_aggregation::handle_sent_to_hyperdex(m);
    }
    return true;
}
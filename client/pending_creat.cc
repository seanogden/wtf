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
#include <hyperdex/datastructures.h>
#include <hyperdex/client.hpp>

// WTF
#include "client/pending_creat.h"
#include "common/response_returncode.h"
#include "client/message_hyperdex_search.h"
#include "client/message_hyperdex_put.h"
#include "client/message_hyperdex_del.h"

using wtf::pending_creat;
using wtf::message_hyperdex_put;

pending_creat :: pending_creat(client* cl, uint64_t client_visible_id, 
                           wtf_client_returncode* status, e::intrusive_ptr<file> f)
    : pending_aggregation(client_visible_id, status) 
    , m_cl(cl)
    , m_file(f)
    , m_done(false)
{
    set_status(WTF_CLIENT_SUCCESS);
    set_error(e::error());
}

pending_creat :: ~pending_creat() throw ()
{
}

bool
pending_creat :: can_yield()
{
    return this->aggregation_done() && !m_done;
}

bool
pending_creat :: yield(wtf_client_returncode* status, e::error* err)
{
    *status = WTF_CLIENT_SUCCESS;
    *err = e::error();
    assert(this->can_yield());
    m_done = true;
    return true;
}

bool
pending_creat :: handle_hyperdex_message(client* cl,
                                    int64_t reqid,
                                    hyperdex_client_returncode rc,
                                    wtf_client_returncode* status,
                                    e::error* err)
{
    pending_aggregation::handle_hyperdex_message(cl, reqid, rc, status, err);
}

typedef struct hyperdex_ds_arena* arena_t;
typedef const struct hyperdex_client_attribute* attr_t;

bool
pending_creat :: send_put(std::string& path, const hyperdex_client_attribute* attrs, size_t attrs_sz)
{
    e::intrusive_ptr<message_hyperdex_put> msg = 
        new message_hyperdex_put(m_cl, "wtf", path.c_str(), attrs, attrs_sz);

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

bool
pending_creat :: try_op()
{
    int64_t ret = -1;
    int i = 0;

    hyperdex_client_returncode hstatus;

    typedef std::map<uint64_t, e::intrusive_ptr<wtf::block> > block_map;

    uint64_t mode = m_file->mode;
    uint64_t directory = m_file->is_directory;
    std::auto_ptr<e::buffer> blockmap_update = m_file->serialize_blockmap();
    struct hyperdex_client_attribute update_attr[3];

    update_attr[0].attr = "mode";
    update_attr[0].value = (const char*)&mode;
    update_attr[0].value_sz = sizeof(mode);
    update_attr[0].datatype = HYPERDATATYPE_INT64;

    update_attr[1].attr = "directory";
    update_attr[1].value = (const char*)&directory;
    update_attr[1].value_sz = sizeof(directory);
    update_attr[1].datatype = HYPERDATATYPE_INT64;

    update_attr[2].attr = "blockmap";
    update_attr[2].value = reinterpret_cast<const char*>(blockmap_update->data());
    update_attr[2].value_sz = blockmap_update->size();
    update_attr[2].datatype = HYPERDATATYPE_STRING;
    
    return send_put(std::string(m_file->path().get()), update_attr, 3);
}

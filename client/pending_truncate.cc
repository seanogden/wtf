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
#include "client/constants.h"
#include "common/macros.h"
#include "client/pending_truncate.h"
#include "common/response_returncode.h"
#include "client/message_hyperdex_put.h"

using wtf::pending_truncate;

pending_truncate :: pending_truncate(client* cl, int64_t client_visible_id, 
                           e::intrusive_ptr<file> f, off_t length,
                           wtf_client_returncode* status)
    : pending_aggregation(client_visible_id, status) 
    , m_cl(cl)
    , m_file(f)
    , m_length(length)
    , m_done(false)
{
    set_status(WTF_CLIENT_SUCCESS);
    set_error(e::error());
}

pending_truncate :: ~pending_truncate() throw ()
{
}

bool
pending_truncate :: can_yield()
{
    return this->aggregation_done() && !m_done;
}

bool
pending_truncate :: yield(wtf_client_returncode* status, e::error* err)
{
    *status = WTF_CLIENT_SUCCESS;
    *err = e::error();
    assert(this->can_yield());
    m_done = true;
    return true;
}

void
pending_truncate :: handle_sent_to_hyperdex(e::intrusive_ptr<message> msg)
{
    pending_aggregation::handle_sent_to_hyperdex(msg);
}

bool
pending_truncate :: handle_hyperdex_message(client* cl,
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

typedef struct hyperdex_ds_arena* arena_t;
typedef struct hyperdex_client_attribute* attr_t;

bool
pending_truncate :: try_op()
{
    //If there's some other op in front of this, put this in the list and wait
    //for that other op to run us
   
    do_op();
    return true;
}

void
pending_truncate :: do_op()
{
    std::vector<block_location> bl;
    uint32_t len;

    m_file->truncate(m_length, bl, len);

    if (bl[0] != block_location())
    {
        std::cout << "sending data" << std::endl;

        if (!send_data(bl, len))
        {
            std::cout << "CANT SEND DATA!!!" << std::endl;
            PENDING_ERROR(IO) << "Couldn't send data to blockservers.";
        }
    }
    else
    {
        //XXX send hyperdex op right away. (there are no half blocks to clip)
    }
}

bool
pending_truncate :: send_data(std::vector<block_location> bl, uint32_t len)
{
    uint32_t num_replicas = bl.size();

    size_t sz = WTF_CLIENT_HEADER_SIZE_REQ
        + sizeof(uint64_t) // m_token
        + sizeof(uint32_t) // number of block locations
        + num_replicas*block_location::pack_size()
        + sizeof(uint32_t); // len 
    std::auto_ptr<e::buffer> msg(e::buffer::create(sz));
    e::buffer::packer pa = msg->pack_at(WTF_CLIENT_HEADER_SIZE_REQ);
    pa = pa << m_cl->m_token << num_replicas;

    std::vector<server_id> servers;

    for (int i = 0; i < num_replicas; ++i)
    {
        pa = pa << bl[i];
        servers.push_back(server_id(bl[i].si));
    }

    pa = pa << len;


    //SEND
    wtf_client_returncode status;
    m_cl->perform_aggregation(servers, this, REQ_TRUNCATE, msg, &status);

    return true;
}



void
pending_truncate :: send_metadata_update()
{
    //XXX need to also write changes to block server first
    
    hyperdex_ds_returncode status;
    arena_t arena = hyperdex_ds_arena_create();
    attr_t attrs = hyperdex_ds_allocate_attribute(arena, 2);

    std::cout << *m_file << std::endl;
    std::auto_ptr<e::buffer> blockmap_update = m_file->serialize_blockmap();

    size_t sz;
    attrs[0].datatype = HYPERDATATYPE_INT64;
    hyperdex_ds_copy_string(arena, "time", 5,
                            &status, &attrs[0].attr, &sz);
    hyperdex_ds_copy_int(arena, time(NULL), 
                            &status, &attrs[0].value, &attrs[0].value_sz);

    attrs[1].datatype = HYPERDATATYPE_STRING;
    hyperdex_ds_copy_string(arena, "blockmap", 9,
                            &status, &attrs[1].attr, &sz);
    hyperdex_ds_copy_string(arena, 
                            reinterpret_cast<const char*>(blockmap_update->data()), 
                            blockmap_update->size(),
                            &status, &attrs[1].value, &attrs[1].value_sz);

    e::intrusive_ptr<message_hyperdex_put> msg = 
        new message_hyperdex_put(m_cl, "wtf", m_file->path().get(), arena, attrs, 2);

    if (msg->send() < 0)
    {
        std::cout << msg->status() << std::endl;
        PENDING_ERROR(IO) << "Couldn't put to HyperDex: " << msg->status();
    }
    else
    {
        m_cl->add_hyperdex_op(msg->reqid(), this);
        e::intrusive_ptr<message> m = msg.get();
        pending_aggregation::handle_sent_to_hyperdex(m);
    }
}

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
#include "common/macros.h"
#include "client/pending_getattr.h"
#include "common/response_returncode.h"
#include "client/message_hyperdex_get.h"

using wtf::pending_getattr;
using wtf::message_hyperdex_get;

pending_getattr :: pending_getattr(client* cl, uint64_t client_visible_id, 
        const char* path,
        wtf_client_returncode* status, struct wtf_file_attrs* fa)
    : pending_aggregation(client_visible_id, status) 
    , m_path(path)
    , m_cl(cl)
    , m_file_attrs(fa)
      , m_done(false)
{
    TRACE;
    set_status(WTF_CLIENT_SUCCESS);
    set_error(e::error());
}

pending_getattr :: ~pending_getattr() throw ()
{
    TRACE;
}

    bool
pending_getattr :: can_yield()
{
    TRACE;
    return this->aggregation_done() && !m_done;
}

    bool
pending_getattr :: yield(wtf_client_returncode* status, e::error* err)
{
    TRACE;
    *status = WTF_CLIENT_SUCCESS;
    *err = e::error();
    assert(this->can_yield());
    m_done = true;
    return true;
}

    bool
pending_getattr :: handle_hyperdex_message(client* cl,
        int64_t reqid,
        hyperdex_client_returncode rc,
        wtf_client_returncode* status,
        e::error* err)
{
    TRACE;
    e::intrusive_ptr<message_hyperdex_get> msg = 
        dynamic_cast<message_hyperdex_get*>(m_outstanding_hyperdex[0].get());

    if (msg->status() == HYPERDEX_CLIENT_NOTFOUND)
    {
        std::cout << "AAAAAAAAAAAAA Not found in hyperdex." << std::endl;
        PENDING_ERROR(NOTFOUND);
    }
    else{

        const hyperdex_client_attribute* attrs = msg->attrs();
        size_t attrs_sz = msg->attrs_sz();

        e::intrusive_ptr<file> f = new file(0,0,0);

        for (size_t i = 0; i < attrs_sz; ++i)
        {
            if (strcmp(attrs[i].attr, "blockmap") == 0)
            {
                TRACE;
                e::unpacker up(attrs[i].value, attrs[i].value_sz);

                if (attrs[i].value_sz == 0)
                {
                    continue;
                }

                up = up >> f;
            }
            else if (strcmp(attrs[i].attr, "directory") == 0)
            {
                TRACE;
                uint64_t is_dir;

                e::unpacker up(attrs[i].value, attrs[i].value_sz);
                up = up >> is_dir;
                e::unpack64be((uint8_t*)&is_dir, &is_dir);

                if (is_dir == 0)
                {
                    f->is_directory = false;
                }
                else
                {
                    f->is_directory = true;
                }
            }
            else if (strcmp(attrs[i].attr, "mode") == 0)
            {
                TRACE;
                uint64_t mode = 0;

                e::unpacker up(attrs[i].value, attrs[i].value_sz);
                up = up >> mode;
                std::cout << "BBBBBBBB mode is " << mode << std::endl;
                e::unpack64be((uint8_t*)&mode, &mode);
                std::cout << "CCCCCCCC mode is " << mode << std::endl;
                f->mode = mode;
            }
            else if (strcmp(attrs[i].attr, "owner") == 0)
            {
                TRACE;
                std::string owner(attrs[i].value, attrs[i].value_sz);
                f->owner = owner;
            }
            else if (strcmp(attrs[i].attr, "group") == 0)
            {
                TRACE;
                std::string group(attrs[i].value, attrs[i].value_sz);
                f->group = group;
            }
            else if (strcmp(attrs[i].attr, "time") == 0)
            {
                TRACE;
                uint64_t t = 0;

                e::unpacker up(attrs[i].value, attrs[i].value_sz);
                up = up >> t;
                std::cout << "BBBBBBBB time is " << t << std::endl;
                e::unpack64be((uint8_t*)&t, &t);
                std::cout << "CCCCCCCC time is " << t << std::endl;
                f->time = t;

            }
        }

        /*fill out file_attrs*/
        m_file_attrs->size = f->length();
        m_file_attrs->mode = f->mode;
        m_file_attrs->flags = f->flags;
        m_file_attrs->is_dir = f->is_directory;
        m_file_attrs->time = f->time;
        m_file_attrs->owner = new char[f->owner.length() + 1];
        m_file_attrs->group = new char[f->group.length() + 1];
        std::strcpy (m_file_attrs->owner, f->owner.c_str());
        std::strcpy (m_file_attrs->group, f->group.c_str());

    }

    pending_aggregation::handle_hyperdex_message(cl, reqid, rc, status, err);
    return true;
}

    bool
pending_getattr :: try_op()
{
    TRACE;
    /* Get the file metadata from HyperDex */
    e::intrusive_ptr<message_hyperdex_get> msg =
        new message_hyperdex_get(m_cl, "wtf", m_path.c_str()); 

    if (msg->send() < 0)
    {
        PENDING_ERROR(IO) << "Couldn't get from HyperDex: " << msg->status();
    }
    else
    {
        m_cl->add_hyperdex_op(msg->reqid(), this);
        e::intrusive_ptr<message> m = msg.get();
        handle_sent_to_hyperdex(m);
    }

    return true;
}

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

#ifndef wtf_client_pending_rename_h_
#define wtf_client_pending_rename_h_

// STL
#include <map>
#include <vector>

// WTF
#include "client/pending_aggregation.h"
#include "client/file.h"

namespace wtf __attribute__ ((visibility("hidden")))
{
class pending_rename : public pending_aggregation
{
    public:
        pending_rename(client* cl, uint64_t client_visible_id,
                          wtf_client_returncode* status,
                          const char* src, const char* dst); 
        virtual ~pending_rename() throw ();

    // return to client
    public:
        virtual bool can_yield();
        virtual bool yield(wtf_client_returncode* status, e::error* error);

    // events
    public:
        virtual void handle_sent_to_wtf(const server_id& si);
        virtual void handle_sent_to_hyperdex(e::intrusive_ptr<message> msg);
        virtual void handle_wtf_failure(const server_id& si);
        virtual void handle_hyperdex_failure(int64_t reqid);

        virtual bool handle_wtf_message(client*,
                                    const server_id& si,
                                    std::auto_ptr<e::buffer> msg,
                                    e::unpacker up,
                                    wtf_client_returncode* status,
                                    e::error* error);
        virtual bool handle_hyperdex_message(client*,
                                    int64_t reqid,
                                    hyperdex_client_returncode rc,
                                    wtf_client_returncode* status,
                                    e::error* error);
        virtual bool handle_search(client*,
                                    int64_t reqid,
                                    hyperdex_client_returncode rc,
                                    wtf_client_returncode* status,
                                    e::error* error);
        virtual bool handle_put_and_delete(client*,
                                    int64_t reqid,
                                    hyperdex_client_returncode rc,
                                    wtf_client_returncode* status,
                                    e::error* error);
        virtual bool try_op();

    friend class e::intrusive_ptr<pending_aggregation>;

    // noncopyable
    private:
        pending_rename(const pending_rename& other);
        pending_rename& operator = (const pending_rename& rhs);

    private:
        bool send_put(std::string& dst, struct hyperdex_ds_arena* arena,
            const struct hyperdex_client_attribute* attrs, size_t attrs_sz);
        bool send_del(std::string& src);
        typedef const struct hyperdex_client_attribute* attr_t;
        typedef struct hyperdex_ds_arena* arena_t;
        attr_t change_name(arena_t arena, attr_t attrs, 
                        size_t sz, std::string& dst, std::string& src);
    private:
        client* m_cl;
        std::string m_src;
        std::string m_dst;
        int64_t m_search_id;
        hyperdex_client_returncode m_search_status;
        hyperdex_client_attribute* m_search_attrs;
        bool m_done;
};

}

#endif // wtf_client_pending_rename_h_

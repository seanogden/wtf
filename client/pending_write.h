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

#ifndef wtf_client_pending_write_h_
#define wtf_client_pending_write_h_

// STL
#include <map>

// WTF
#include "client/pending_aggregation.h"
#include "client/buffer_descriptor.h"
#include "client/file.h"
#include "client/client.h"

namespace wtf __attribute__ ((visibility("hidden")))
{
class pending_write : public pending_aggregation
{
    public:
        pending_write(client* cl, uint64_t id, e::intrusive_ptr<file> f,
                               e::slice& data, std::vector<block_location>& bl,
                               uint64_t file_offset,
                               e::intrusive_ptr<buffer_descriptor> bd,
                               wtf_client_returncode* status);
        virtual ~pending_write() throw ();

    // return to client
    public:
        virtual bool can_yield();
        virtual bool yield(wtf_client_returncode* status, e::error* error);

    friend class file;
    friend class rereplicate;

    // events
    public:
        virtual void handle_wtf_failure(const server_id& si);
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
        virtual bool try_op();
        void do_op();

    // noncopyable
    private:
        pending_write(const pending_write& other);
        pending_write& operator = (const pending_write& rhs);
        typedef std::map<uint64_t, e::intrusive_ptr<block> > changeset_t;

    private:
        void send_metadata_update();
        void apply_metadata_update_locally();
        bool send_data();
        void prepare_write_op(e::intrusive_ptr<file> f, 
                              size_t& rem, 
                              std::vector<block_location>& bl,
                              size_t& buf_offset,
                              uint32_t& block_offset,
                              uint32_t& block_capacity,
                              uint64_t& file_offset,
                              size_t& slice_len);
        void get_new_metadata();
        bool handle_new_metadata(client*,
                                    int64_t reqid,
                                    hyperdex_client_returncode rc,
                                    wtf_client_returncode* status,
                                    e::error* error);

    private:
        client* m_cl;
        e::slice m_data;
        std::vector<block_location> m_block_locations;
        uint32_t m_block_offset;
        uint32_t m_block_capacity;
        uint64_t m_file_offset;
        e::intrusive_ptr<buffer_descriptor> m_buffer_descriptor;
        e::intrusive_ptr<file> m_file;
        std::string m_path;
        std::auto_ptr<e::buffer> m_old_blockmap;
        changeset_t m_changeset;
        bool m_done;
        int m_state;
        e::intrusive_ptr<pending_write> m_next;
        bool m_deferred;
        bool m_retry;
        bool m_retried;
};

}

#endif // wtf_client_pending_write_h_

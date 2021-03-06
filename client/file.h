// Copyright (c) 2013, Sean Ogden
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

#ifndef wtf_file_h_
#define wtf_file_h_

// e
#include <e/intrusive_ptr.h>

// STL
#include <vector>
#include <map>

//PO6
#include <po6/pathname.h>

//WTF
#include "client/client.h"
#include "common/block.h"
#include "common/block_location.h"
#include "common/interval_map.h"

namespace wtf __attribute__ ((visibility("hidden")))
{
class pending_write;    
class file
{

    public:
        file(const char* path, size_t replicas, size_t block_size);
        ~file() throw ();

    public:
        void set_fd(int64_t fd) { m_fd = fd; }
        int64_t fd() { return m_fd; }
        void path(const char* path) { m_path = po6::pathname(path); }
        po6::pathname path() { return m_path; }
        uint64_t replicas() { return m_replicas; }
        void set_replicas(size_t num_replicas) { m_replicas = num_replicas; }
        bool pending_ops_empty();
        int64_t pending_ops_pop_front();
        void add_pending_op(uint64_t client_id);
        void insert_block(uint64_t insert_address, wtf::slice& slc);
        void apply_changeset(std::map<uint64_t, e::intrusive_ptr<block> >& changeset);
        void set_last_op(uint32_t block_offset, e::intrusive_ptr<wtf::pending_write> op);
        bool has_last_op(uint32_t block_offset);
        e::intrusive_ptr<wtf::pending_write> last_op(uint32_t block_offset);

        void set_offset(uint64_t offset) { m_offset = offset;}
        uint64_t offset() { return m_offset; }
        uint64_t pack_size();
        uint64_t length() const;
        std::auto_ptr<e::buffer> serialize_blockmap();
        void truncate(size_t length);
        size_t block_size() { return m_block_size; }
        size_t bytes_left_in_file();

    private:
        friend class e::intrusive_ptr<file>;
        friend std::ostream& 
            operator << (std::ostream& lhs, const file& rhs);
        friend e::buffer::packer 
            operator << (e::buffer::packer pa, const file& rhs);
        friend e::unpacker 
            operator >> (e::unpacker up, e::intrusive_ptr<file>& rhs);

    private:
        file(const file&);

    private:
        void inc() { ++m_ref; }
        void dec() { assert(m_ref > 0); if (--m_ref == 0) delete this; }

    private:
        typedef std::map<uint64_t, e::intrusive_ptr<wtf::pending_write> > op_map_t;
        file& operator = (const file&);

    private:
        size_t m_ref;
        po6::pathname m_path;
        int64_t m_fd;
        std::list<int64_t> m_pending;
        interval_map m_block_map;
        op_map_t m_last_op;
        size_t m_offset;
        size_t m_replicas;
        size_t m_block_size;


    public:
        bool is_directory;
        int flags;
        uint64_t mode;
        uint64_t time;
        std::string owner;
        std::string group;
};

inline std::ostream& 
operator << (std::ostream& lhs, const file& rhs) 
{ 
    lhs << "\tpath: " << rhs.m_path << std::endl;
    lhs << "\treplicas: " << rhs.m_replicas << std::endl;
    lhs << "\tlength: " << rhs.length() << std::endl;
    lhs << "\tflags: " << rhs.flags << std::endl;
    lhs << "\tmode: " << rhs.mode << std::endl;
    lhs << "\ttime: " << rhs.time << std::endl;
    lhs << "\toffset: " << rhs.m_offset << std::endl;
    lhs << "\tis_directory: " << rhs.is_directory << std::endl;
    lhs << "\tblock_size: " << rhs.m_block_size << std::endl;
    lhs << "\tblocks: " << std::endl;
    lhs << rhs.m_block_map << std::endl;
    return lhs;
} 


inline e::buffer::packer 
operator << (e::buffer::packer pa, const file& rhs) 
{ 
    uint64_t block_size = rhs.m_block_size;
    uint64_t replicas = rhs.m_replicas;
    pa = pa << replicas << block_size << rhs.m_block_map;
    return pa;
} 

inline e::unpacker 
operator >> (e::unpacker up, e::intrusive_ptr<file>& rhs) 
{ 
    uint64_t block_size;
    uint64_t replicas;
    up = up >> replicas >> block_size >> rhs->m_block_map; 
    rhs->m_block_size = block_size;
    rhs->m_replicas = replicas;
    return up; 
}

}

#endif // wtf_file_h_

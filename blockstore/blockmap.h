
//sys
#include <sys/mman.h>

//glog
#include <glog/logging.h>
#include <glog/raw_logging.h>

// LevelDB
#include <hyperleveldb/db.h>
#include <hyperleveldb/write_batch.h>
#include <hyperleveldb/filter_policy.h>

// po6
#include <po6/pathname.h>
#include <po6/io/fd.h>

//e
#include <e/slice.h>
#include <e/unpacker.h>

//leveldb
#include <hyperleveldb/db.h>

#include <tr1/memory>

#include <sys/stat.h>
#include <unistd.h>

#include "blockstore/disk.h"

#include "blockstore/vblock.h"

#ifndef wtf_blockmap_h_
#define wtf_blockmap_h_

namespace wtf __attribute__ ((visibility("default")))
{
    class blockmap
    {
        public:
            blockmap();
            ~blockmap();
            bool setup(const po6::pathname& path,
                  const po6::pathname& backing_path);

            ssize_t write(const e::slice& data,
                        uint64_t& bid);
            ssize_t update(const e::slice& data,
                        uint64_t offset,
                        uint64_t& bid,
                        uint64_t& block_len);
            ssize_t read(uint64_t bid,
                        uint8_t* data, 
                        size_t data_offset,
                        size_t data_sz);
            ssize_t truncate(uint64_t& bid,
                             size_t len);
        private:
            ssize_t read_offset_map(uint64_t bid, vblock& vb);
            ssize_t write_offset_map(uint64_t bid, vblock& vb);
            ssize_t update_offset_map(uint64_t bid, vblock& vb, size_t offset, size_t len, size_t disk_offset);

        private:
            typedef std::tr1::shared_ptr<leveldb::DB> leveldb_db_ptr;
            leveldb_db_ptr m_db;
            uint64_t m_backing_size;
            disk* m_disk;
            po6::io::fd m_fd;
            uint64_t m_block_id;

    };
}
#endif //wtf_blockmap_h_

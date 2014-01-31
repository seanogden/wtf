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

// C++
#include <iostream>

// WTF
#include "common/macros.h"
#include "common/network_msgtype.h"

std::ostream&
wtf :: operator << (std::ostream& lhs, wtf_network_msgtype rhs)
{
    switch (rhs)
    {
        STRINGIFY(REQ_GET);
        STRINGIFY(RESP_GET);
        STRINGIFY(REQ_PUT);
        STRINGIFY(RESP_PUT);
        STRINGIFY(REQ_UPDATE);
        STRINGIFY(RESP_UPDATE);
        STRINGIFY(PACKET_NOP);
        STRINGIFY(CONFIGMISMATCH);
        default:
            lhs << "unknown msgtype";
    }

    return lhs;
}

e::buffer::packer
wtf :: operator << (e::buffer::packer lhs, const wtf_network_msgtype& rhs)
{
    uint8_t mt = static_cast<uint8_t>(rhs);
    return lhs << mt;
}

e::unpacker
wtf :: operator >> (e::unpacker lhs, wtf_network_msgtype& rhs)
{
    uint8_t mt;
    lhs = lhs >> mt;
    rhs = static_cast<wtf_network_msgtype>(mt);
    return lhs;
}

size_t
wtf :: pack_size(const wtf_network_msgtype&)
{
    return sizeof(uint8_t);
}

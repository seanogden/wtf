#!/bin/sh
exec python "${WTF_SRCDIR}"/test/runner.py --wtf-daemons=2 --hyperdex-daemons=1 -- \
     "${WTF_BUILDDIR}"/test/readwrite-sync-stress-test -h {WTF_HOST} -p {WTF_PORT} \
      -H {HYPERDEX_HOST} -P {HYPERDEX_PORT} --file-length=64 --file-charset='hex' \
      --value-length=1000 -n 100 -b 65536 --min-read-length=1 --max-read-length=67108864 \
      --min-write-length=1 --max-write-length=67108864

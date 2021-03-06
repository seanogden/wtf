# Copyright (c) 2013, Sean Ogden
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#     * Redistributions of source code must retain the above copyright notice,
#       this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of WTF nor the names of its contributors may be
#       used to endorse or promote products derived from this software without
#       specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}
AM_DISTCHECK_CONFIGURE_FLAGS = --enable-java-bindings
AM_CPPFLAGS  = -I${abs_top_srcdir}/include $(PO6_CFLAGS) $(E_CFLAGS) $(BUSYBEE_CFLAGS) $(HYPERLEVELDB_CFLAGS) $(REPLICANT_CFLAGS) $(SPARSEHASH_CFLAGS)
AM_CFLAGS = $(WANAL_CFLAGS)
AM_CXXFLAGS = $(WANAL_CXXFLAGS) $(PO6_CFLAGS) $(E_CFLAGS) $(BUSYBEE_CFLAGS) $(LRT_CFLAGS)
AM_LDFLAGS = $(PO6_LDFLAGS) $(E_LDFLAGS) $(BUSYBEE_LDFLAGS) $(REPLICANT_FLAGS)

TESTS_ENVIRONMENT = . $(abs_top_srcdir)/test/env.sh "${abs_top_srcdir}" "${abs_top_builddir}" "${VERSION}";
#"${abs_top_srcdir}" "${abs_top_builddir}" "${VERSION}";
wtfexecdir = $(exec_prefix)/libexec/$(PACKAGE)-$(VERSION)
wtfheaderdir = $(includedir)/wtf
pkgconfigdir = $(libdir)/pkgconfig

if ENABLE_DEBUG
    AM_CXXFLAGS += -ggdb -O0
endif

lib_LTLIBRARIES =
wtfexec_LTLIBRARIES =

wtfheader_HEADERS =
noinst_HEADERS = 

bin_PROGRAMS =
wtfexec_PROGRAMS =
noinst_PROGRAMS =
check_PROGRAMS =

pkgconfig_DATA =

jardir = $(datadir)/java
jar_DATA =

EXTRA_DIST =
CLEANFILES =
TESTS = 

noinst_HEADERS += daemon/settings.h
noinst_HEADERS += tools/common.h
noinst_HEADERS += client/constants.h
noinst_HEADERS += common/configuration_flags.h
noinst_HEADERS += client/client.h
noinst_HEADERS += common/block.h
noinst_HEADERS += common/coordinator_link.h
noinst_HEADERS += common/block_location.h
noinst_HEADERS += common/server.h
noinst_HEADERS += common/serialization.h
noinst_HEADERS += common/configuration.h
noinst_HEADERS += common/coordinator_returncode.h
noinst_HEADERS += common/coordinator_link.h
noinst_HEADERS += common/macros.h
noinst_HEADERS += common/mapper.h
noinst_HEADERS += common/network_msgtype.h
noinst_HEADERS += common/packing.h
noinst_HEADERS += common/response_returncode.h
noinst_HEADERS += common/special_objects.h
noinst_HEADERS += common/ids.h
noinst_HEADERS += common/interval_map.h
noinst_HEADERS += coordinator/coordinator.h
noinst_HEADERS += coordinator/server_state.h
noinst_HEADERS += daemon/block_storage_manager.h
noinst_HEADERS += daemon/connection.h
noinst_HEADERS += daemon/coordinator_link_wrapper.h
noinst_HEADERS += daemon/daemon.h
noinst_HEADERS += blockstore/blockmap.h
noinst_HEADERS += blockstore/disk.h
noinst_HEADERS += blockstore/vblock.h
noinst_HEADERS += visibility.h

noinst_PROGRAMS += wtf-stat
noinst_PROGRAMS += wtf-backup

bin_PROGRAMS += wtf-fuse
bin_PROGRAMS += wtf

wtfexec_PROGRAMS += wtf-coordinator
wtfexec_PROGRAMS += wtf-daemon 
wtfexec_PROGRAMS += wtf-mkfs
wtfexec_PROGRAMS += wtf-wtfio
wtfexec_PROGRAMS += wtf-wtfio-checker
wtfexec_PROGRAMS += wtf-server-register
wtfexec_PROGRAMS += wtf-server-online
wtfexec_PROGRAMS += wtf-server-offline
wtfexec_PROGRAMS += wtf-server-kill
wtfexec_PROGRAMS += wtf-server-forget
wtfexec_PROGRAMS += wtf-coordinator


################################################################################
################################## Blockstore ##################################
################################################################################

lib_LTLIBRARIES += libwtfblockstore.la

libwtfblockstore_la_SOURCES = 
libwtfblockstore_la_SOURCES += blockstore/vblock.cc
libwtfblockstore_la_SOURCES += blockstore/disk.cc
libwtfblockstore_la_SOURCES += blockstore/blockmap.cc

libwtfblockstore_la_LIBADD = $(E_LIBS) $(HYPERLEVELDB_LIBS) -lglog -ldl

################################################################################
################################## Daemon ######################################
################################################################################

wtf_daemon_SOURCES =
wtf_daemon_SOURCES += common/server.cc
wtf_daemon_SOURCES += common/ids.cc
wtf_daemon_SOURCES += common/block_location.cc
wtf_daemon_SOURCES += common/configuration.cc
wtf_daemon_SOURCES += common/coordinator_link.cc
wtf_daemon_SOURCES += common/coordinator_returncode.cc
wtf_daemon_SOURCES += common/mapper.cc
wtf_daemon_SOURCES += common/network_msgtype.cc
wtf_daemon_SOURCES += common/packing.cc
wtf_daemon_SOURCES += common/response_returncode.cc
wtf_daemon_SOURCES += daemon/block_storage_manager.cc
wtf_daemon_SOURCES += daemon/connection.cc
wtf_daemon_SOURCES += daemon/coordinator_link_wrapper.cc
wtf_daemon_SOURCES += daemon/daemon.cc
wtf_daemon_SOURCES += daemon/main.cc

wtf_daemon_CXXFLAGS = $(CXXFLAGS) $(AM_CXXFLAGS)
wtf_daemon_LDADD = $(E_LIBS) $(BUSYBEE_LIBS) $(REPLICANT_LIBS) \
                   $(HYPERLEVELDB_LIBS) libwtfblockstore.la -lglog -ldl -lpthread

################################################################################
################################## Coordinator #################################
################################################################################

wtfexec_LTLIBRARIES += libwtf-coordinator.la

noinst_HEADERS += coordinator/coordinator.h
noinst_HEADERS += coordinator/server_barrier.h
noinst_HEADERS += coordinator/transitions.h
noinst_HEADERS += coordinator/util.h

libwtf_coordinator_la_SOURCES =
libwtf_coordinator_la_SOURCES += common/ids.cc
libwtf_coordinator_la_SOURCES += common/serialization.cc
libwtf_coordinator_la_SOURCES += common/server.cc
libwtf_coordinator_la_SOURCES += coordinator/server_barrier.cc
libwtf_coordinator_la_SOURCES += coordinator/coordinator.cc
libwtf_coordinator_la_SOURCES += coordinator/symtable.c
libwtf_coordinator_la_SOURCES += coordinator/transitions.cc

wtf_coordinator_SOURCES = tools/coordinator.cc
wtf_coordinator_CPPFLAGS = -DWTF_EXEC_DIR=\""$(wtfexecdir)\"" $(AM_CPPFLAGS) $(CPPFLAGS)
wtf_coordinator_LDADD =
wtf_coordinator_LDADD += $(E_LIBS)
wtf_coordinator_LDADD += $(BUSYBEE_LIBS)
wtf_coordinator_LDADD += -lpopt

################################################################################
################################## Client ######################################
################################################################################


lib_LTLIBRARIES += libwtf-client.la

wtfheader_HEADERS += include/wtf/client.h
wtfheader_HEADERS += include/wtf/client.hpp

pkgconfig_DATA += wtf-client.pc

noinst_HEADERS += include/wtf/client.h
noinst_HEADERS += include/wtf/client.hpp
noinst_HEADERS += client/file.h
noinst_HEADERS += common/interval_map.h
noinst_HEADERS += client/buffer_descriptor.h
noinst_HEADERS += client/pending.h
noinst_HEADERS += client/pending_aggregation.h
noinst_HEADERS += client/pending_getattr.h
noinst_HEADERS += client/pending_truncate.h
noinst_HEADERS += client/pending_chdir.h
noinst_HEADERS += client/pending_read.h
noinst_HEADERS += client/pending_chmod.h
noinst_HEADERS += client/pending_write.h
noinst_HEADERS += client/pending_readdir.h
noinst_HEADERS += client/pending_rename.h
noinst_HEADERS += client/pending_mkdir.h
noinst_HEADERS += client/pending_del.h
noinst_HEADERS += client/pending_creat.h
noinst_HEADERS += client/pending_open.h
noinst_HEADERS += client/message.h
noinst_HEADERS += client/message_hyperdex_get.h
noinst_HEADERS += client/message_hyperdex_put.h
noinst_HEADERS += client/message_hyperdex_condput.h
noinst_HEADERS += client/message_hyperdex_del.h
noinst_HEADERS += client/message_hyperdex_search.h
noinst_HEADERS += client/rereplicate.h

libwtf_client_la_CXXFLAGS = $(CXXFLAGS) $(AM_CXXFLAGS)

libwtf_client_la_SOURCES =
libwtf_client_la_SOURCES += common/server.cc
libwtf_client_la_SOURCES += common/ids.cc
libwtf_client_la_SOURCES += common/block_location.cc
libwtf_client_la_SOURCES += common/configuration.cc
libwtf_client_la_SOURCES += common/mapper.cc
libwtf_client_la_SOURCES += common/network_msgtype.cc
libwtf_client_la_SOURCES += common/packing.cc
libwtf_client_la_SOURCES += common/response_returncode.cc
libwtf_client_la_SOURCES += common/block.cc
libwtf_client_la_SOURCES += client/c.cc
libwtf_client_la_SOURCES += client/pending_aggregation.cc
libwtf_client_la_SOURCES += client/pending_getattr.cc
libwtf_client_la_SOURCES += client/pending_truncate.cc
libwtf_client_la_SOURCES += client/pending_chdir.cc
libwtf_client_la_SOURCES += client/pending.cc
libwtf_client_la_SOURCES += client/pending_read.cc
libwtf_client_la_SOURCES += client/pending_chmod.cc
libwtf_client_la_SOURCES += client/pending_write.cc
libwtf_client_la_SOURCES += client/pending_readdir.cc
libwtf_client_la_SOURCES += client/pending_rename.cc
libwtf_client_la_SOURCES += client/pending_mkdir.cc
libwtf_client_la_SOURCES += client/pending_del.cc
libwtf_client_la_SOURCES += client/pending_creat.cc
libwtf_client_la_SOURCES += client/pending_open.cc
libwtf_client_la_SOURCES += client/message_hyperdex_get.cc
libwtf_client_la_SOURCES += client/message_hyperdex_put.cc
libwtf_client_la_SOURCES += client/message_hyperdex_condput.cc
libwtf_client_la_SOURCES += client/message_hyperdex_del.cc
libwtf_client_la_SOURCES += client/message_hyperdex_search.cc
libwtf_client_la_SOURCES += common/coordinator_link.cc
libwtf_client_la_SOURCES += client/file.cc
libwtf_client_la_SOURCES += common/interval_map.cc
libwtf_client_la_SOURCES += client/buffer_descriptor.cc
libwtf_client_la_SOURCES += client/client.cc
libwtf_client_la_SOURCES += client/rereplicate.cc

libwtf_client_la_LIBADD = 
libwtf_client_la_LIBADD += $(E_LIBS) 
libwtf_client_la_LIBADD += $(BUSYBEE_LIBS) 
libwtf_client_la_LIBADD += $(REPLICANT_LIBS) 
libwtf_client_la_LIBADD += $(HYPERCLIENT_LIBS) -lglog

################################################################################
##################################### Admin ####################################
################################################################################


lib_LTLIBRARIES += libwtf-admin.la

wtfheader_HEADERS += include/wtf/admin.h
wtfheader_HEADERS += include/wtf/admin.hpp

pkgconfig_DATA += wtf-admin.pc

noinst_HEADERS += include/wtf/admin.h
noinst_HEADERS += include/wtf/admin.hpp
noinst_HEADERS += admin/admin.h
noinst_HEADERS += admin/constants.h
noinst_HEADERS += admin/coord_rpc.h
noinst_HEADERS += admin/coord_rpc_generic.h
noinst_HEADERS += admin/multi_yieldable.h
noinst_HEADERS += admin/pending.h
noinst_HEADERS += admin/pending_string.h
noinst_HEADERS += admin/yieldable.h

libwtf_admin_la_CXXFLAGS = $(CXXFLAGS) $(AM_CXXFLAGS)

libwtf_admin_la_SOURCES =
libwtf_admin_la_SOURCES += common/block_location.cc
libwtf_admin_la_SOURCES += common/configuration.cc
libwtf_admin_la_SOURCES += common/coordinator_link.cc
libwtf_admin_la_SOURCES += common/ids.cc
libwtf_admin_la_SOURCES += common/mapper.cc
libwtf_admin_la_SOURCES += common/network_msgtype.cc
libwtf_admin_la_SOURCES += common/serialization.cc
libwtf_admin_la_SOURCES += common/server.cc
libwtf_admin_la_SOURCES += admin/admin.cc
libwtf_admin_la_SOURCES += admin/c.cc
libwtf_admin_la_SOURCES += admin/coord_rpc.cc
libwtf_admin_la_SOURCES += admin/coord_rpc_generic.cc
libwtf_admin_la_SOURCES += admin/multi_yieldable.cc
libwtf_admin_la_SOURCES += admin/pending.cc
libwtf_admin_la_SOURCES += admin/pending_string.cc
libwtf_admin_la_SOURCES += admin/yieldable.cc
libwtf_admin_la_LIBADD =
libwtf_admin_la_LIBADD += $(E_LIBS)
libwtf_admin_la_LIBADD += $(BUSYBEE_LIBS)
libwtf_admin_la_LIBADD += $(REPLICANT_LIBS)

################################################################################
################################## other   #####################################
################################################################################

wtf_SOURCES = wtf.cc
wtf_CPPFLAGS = -DWTF_EXEC_DIR="\"$(wtfexecdir)\""

################################################################################
################################## benchmarks ##################################
################################################################################

################################################################################
################################## tests   #####################################
################################################################################
shell_wrappers =
#shell_wrappers += test/sh/readdir_test.sh
#shell_wrappers += test/sh/chdir_test.sh
#shell_wrappers += test/sh/getattr_test.sh
#shell_wrappers += test/sh/readwrite_sync_stress_test.basic.sh
#shell_wrappers += test/sh/readwrite_sync_stress_test.2GB.sh
shell_wrappers += test/sh/closetest.sh
#shell_wrappers += test/sh/lseektest.sh
#shell_wrappers += test/sh/lseektest2.sh
#shell_wrappers += test/sh/appendtest.sh
#shell_wrappers += test/sh/truncatetest.sh

#wtf-test_readwrite
check_PROGRAMS += test/readwrite-sync-stress-test
test_readwrite_sync_stress_test_SOURCES = test/readwrite_sync_stress_test.cc 
test_readwrite_sync_stress_test_LDADD = libwtf-client.la $(E_LIBS) -lpopt -larmnod

check_PROGRAMS += test/chdir-test
test_chdir_test_SOURCES = test/chdir_test.cc 
test_chdir_test_LDADD = libwtf-client.la $(E_LIBS) -lpopt -larmnod

check_PROGRAMS += test/readdir-test
test_readdir_test_SOURCES = test/readdir_test.cc 
test_readdir_test_LDADD = libwtf-client.la $(E_LIBS) -lpopt -larmnod

check_PROGRAMS += test/closetest
test_closetest_SOURCES = test/closetest.cc 
test_closetest_LDADD = libwtf-client.la $(E_LIBS) -lpopt -larmnod

check_PROGRAMS += test/appendtest
test_appendtest_SOURCES = test/appendtest.cc 
test_appendtest_LDADD = libwtf-client.la $(E_LIBS) -lpopt -larmnod

check_PROGRAMS += test/truncatetest
noinst_PROGRAMS += test/truncatetest
test_truncatetest_SOURCES = test/truncatetest.cc 
test_truncatetest_LDADD = libwtf-client.la $(E_LIBS) -lpopt -larmnod

check_PROGRAMS += test/lseektest2
test_lseektest2_SOURCES = test/lseektest2.cc 
test_lseektest2_LDADD = libwtf-client.la $(E_LIBS) -lpopt -larmnod

check_PROGRAMS += test/lseektest
test_lseektest_SOURCES = test/lseektest.cc 
test_lseektest_LDADD = libwtf-client.la $(E_LIBS) -lpopt -larmnod

check_PROGRAMS += test/getattr-test
test_getattr_test_SOURCES = test/getattr_test.cc 
test_getattr_test_LDADD = libwtf-client.la $(E_LIBS) -lpopt -larmnod

#java tests
if ENABLE_JAVA_BINDINGS
java_wrappers =
#java_wrappers += test/sh/bindings.java.Basic.sh
shell_wrappers += $(java_wrappers)
endif
TESTS += $(shell_wrappers)

EXTRA_DIST += $(shell_wrappers)
EXTRA_DIST += test/env.sh
EXTRA_DIST += test/runner.py
EXTRA_DIST += tools/startlocal.sh
EXTRA_DIST += .hyperdex_daemon_hosts
EXTRA_DIST += .hyperdex_coordinator_host
EXTRA_DIST += .wtf_daemon_hosts
EXTRA_DIST += .wtf_coordinator_host
EXTRA_DIST += .wtf_client_hosts
EXTRA_DIST += tools/startcluster.sh 
EXTRA_DIST += tools/stopcluster.sh 


#if ENABLE_GTEST
#wtf_test_blockstore_SOURCES = test/test_blockstore.cc
#wtf_test_blockstore_LDADD = libwtfblockstore.la -lpopt -lglog -lgtest
#endif

################################################################################
################################## benchmarks ##################################
################################################################################

wtf_wtfio_SOURCES = benchmarks/wtfio.cc
wtf_wtfio_LDADD = libwtf-client.la $(E_LIBS) -lpopt

wtf_wtfio_checker_SOURCES = benchmarks/wtfio-checker.cc
wtf_wtfio_checker_LDADD = libwtf-client.la $(E_LIBS) -lpopt


################################################################################
################################## tools   #####################################
################################################################################

# wtf-mkfs
wtf_mkfs_SOURCES = tools/mkfs.cc
wtf_mkfs_CPPFLAGS = $(CPPFLAGS) 
wtf_mkfs_LDADD = $(HYPERCLIENT_LIBS) -lpopt

# wtf-server-register
wtf_server_register_SOURCES = tools/server-register.cc
wtf_server_register_LDADD = libwtf-admin.la -lpopt

# wtf-server-online
wtf_server_online_SOURCES = tools/server-online.cc
wtf_server_online_LDADD = libwtf-admin.la -lpopt

# wtf-server-offline
wtf_server_offline_SOURCES = tools/server-offline.cc
wtf_server_offline_LDADD = libwtf-admin.la -lpopt

# wtf-server-forget
wtf_server_forget_SOURCES = tools/server-forget.cc
wtf_server_forget_LDADD = libwtf-admin.la -lpopt

# wtf-server-kill
wtf_server_kill_SOURCES = tools/server-kill.cc
wtf_server_kill_LDADD = libwtf-admin.la -lpopt

# wtf-stat (metadata dump)
wtf_stat_SOURCES =
wtf_stat_SOURCES += common/block.cc
wtf_stat_SOURCES += common/block_location.cc
wtf_stat_SOURCES += client/file.cc
wtf_stat_SOURCES += common/interval_map.cc
wtf_stat_SOURCES += tools/stat.cc
wtf_stat_LDADD = libwtf-admin.la $(REPLICANT_LIBS) $(HYPERCLIENT_LIBS)

# wtf-backup (rereplicate file block)
wtf_backup_SOURCES =
wtf_backup_SOURCES += client/pending_aggregation.cc
wtf_backup_SOURCES += client/pending.cc
wtf_backup_SOURCES += client/pending_getattr.cc
wtf_backup_SOURCES += client/pending_truncate.cc
wtf_backup_SOURCES += client/pending_chdir.cc
wtf_backup_SOURCES += client/pending_read.cc
wtf_backup_SOURCES += client/pending_chmod.cc
wtf_backup_SOURCES += client/pending_write.cc
wtf_backup_SOURCES += client/pending_readdir.cc
wtf_backup_SOURCES += client/pending_rename.cc
wtf_backup_SOURCES += client/pending_mkdir.cc
wtf_backup_SOURCES += client/pending_del.cc
wtf_backup_SOURCES += client/pending_creat.cc
wtf_backup_SOURCES += client/pending_open.cc
wtf_backup_SOURCES += client/message_hyperdex_get.cc
wtf_backup_SOURCES += client/message_hyperdex_put.cc
wtf_backup_SOURCES += client/message_hyperdex_condput.cc
wtf_backup_SOURCES += client/message_hyperdex_del.cc
wtf_backup_SOURCES += client/message_hyperdex_search.cc
wtf_backup_SOURCES += common/server.cc
wtf_backup_SOURCES += common/ids.cc
wtf_backup_SOURCES += common/block_location.cc
wtf_backup_SOURCES += common/configuration.cc
wtf_backup_SOURCES += common/mapper.cc
wtf_backup_SOURCES += common/network_msgtype.cc
wtf_backup_SOURCES += common/packing.cc
wtf_backup_SOURCES += common/response_returncode.cc
wtf_backup_SOURCES += common/block.cc
wtf_backup_SOURCES += common/coordinator_link.cc
wtf_backup_SOURCES += client/file.cc
wtf_backup_SOURCES += common/interval_map.cc
wtf_backup_SOURCES += client/buffer_descriptor.cc
wtf_backup_SOURCES += client/client.cc
wtf_backup_SOURCES += client/rereplicate.cc
wtf_backup_SOURCES += tools/backup.cc
wtf_backup_LDADD = $(REPLICANT_LIBS) $(HYPERCLIENT_LIBS) libwtf-client.la
wtf_backup_CXXFLAGS = $(CXXFLAGS) $(AM_CXXFLAGS)

################################ FUSE ##########################################

wtf_fuse_SOURCES =
wtf_fuse_SOURCES += bindings/fuse/fusewtf.cc
wtf_fuse_SOURCES += client/pending_aggregation.cc
wtf_fuse_SOURCES += client/pending.cc
wtf_fuse_SOURCES += client/pending_getattr.cc
wtf_fuse_SOURCES += client/pending_truncate.cc
wtf_fuse_SOURCES += client/pending_chdir.cc
wtf_fuse_SOURCES += client/pending_read.cc
wtf_fuse_SOURCES += client/pending_chmod.cc
wtf_fuse_SOURCES += client/pending_write.cc
wtf_fuse_SOURCES += client/pending_readdir.cc
wtf_fuse_SOURCES += client/pending_rename.cc
wtf_fuse_SOURCES += client/pending_mkdir.cc
wtf_fuse_SOURCES += client/pending_del.cc
wtf_fuse_SOURCES += client/pending_creat.cc
wtf_fuse_SOURCES += client/pending_open.cc
wtf_fuse_SOURCES += client/message_hyperdex_get.cc
wtf_fuse_SOURCES += client/message_hyperdex_put.cc
wtf_fuse_SOURCES += client/message_hyperdex_condput.cc
wtf_fuse_SOURCES += client/message_hyperdex_del.cc
wtf_fuse_SOURCES += client/message_hyperdex_search.cc
wtf_fuse_SOURCES += common/server.cc
wtf_fuse_SOURCES += common/ids.cc
wtf_fuse_SOURCES += common/block_location.cc
wtf_fuse_SOURCES += common/configuration.cc
wtf_fuse_SOURCES += common/mapper.cc
wtf_fuse_SOURCES += common/network_msgtype.cc
wtf_fuse_SOURCES += common/packing.cc
wtf_fuse_SOURCES += common/response_returncode.cc
wtf_fuse_SOURCES += common/block.cc
wtf_fuse_SOURCES += common/coordinator_link.cc
wtf_fuse_SOURCES += client/file.cc
wtf_fuse_SOURCES += common/interval_map.cc
wtf_fuse_SOURCES += client/buffer_descriptor.cc
wtf_fuse_SOURCES += client/client.cc
wtf_fuse_CXXFLAGS = $(CXXFLAGS) $(AM_CXXFLAGS) -D_FILE_OFFSET_BITS=64 -lfuse
wtf_fuse_LDADD = $(REPLICANT_LIBS) $(HYPERCLIENT_LIBS) libwtf-client.la

################################# Java #########################################


wtf_client_jarfile = bindings/java/org.wtf.client-$(VERSION).jar

if ENABLE_JAVA_BINDINGS
lib_LTLIBRARIES += libwtf-client-java.la
jar_DATA += $(wtf_client_jarfile)
endif

CLEANFILES += $(wtf_client_jarfile)
CLEANFILES += bindings/java/org/wtf/client/*.class
CLEANFILES += test/java/*.class

EXTRA_DIST += bindings/java/org/wtf/client/ByteString.java
EXTRA_DIST += bindings/java/org_wtf_client_Client.definitions.c
EXTRA_DIST += bindings/java/org/wtf/client/Client.java
EXTRA_DIST += bindings/java/org/wtf/client/Deferred.java
EXTRA_DIST += bindings/java/org/wtf/client/WTFClientException.java
EXTRA_DIST += bindings/java/org/wtf/client/Iterator.java
EXTRA_DIST += bindings/java/org/wtf/client/Operation.java
EXTRA_DIST += bindings/java/org/wtf/client/WTFFileAttrs.java

# Client
noinst_HEADERS += bindings/java/org_wtf_client_Client.h
noinst_HEADERS += bindings/java/org_wtf_client_Deferred.h
noinst_HEADERS += bindings/java/org_wtf_client_Iterator.h

libwtf_client_java_la_SOURCES =
libwtf_client_java_la_SOURCES += bindings/java/org_wtf_client_Client.c
libwtf_client_java_la_CPPFLAGS = $(JNI_CPPFLAGS) $(AM_CPPFLAGS) $(CPPFLAGS)
libwtf_client_java_la_CFLAGS = $(AM_CFLAGS) $(CFLAGS) -Wmissing-prototypes
libwtf_client_java_la_LIBADD = libwtf-client.la

JAVA_SRC_BASE=${abs_top_srcdir}/bindings/java
JAVA_BUILD_BASE=${abs_top_builddir}/bindings/java
JAVA_SRC_PKG=${JAVA_SRC_BASE}/org/wtf
JAVA_BUILD_PKG=${JAVA_BUILD_BASE}/org/wtf

$(wtf_client_jarfile): $(wildcard ${JAVA_SRC_PKG}/client/*.java)
	javac -d ${JAVA_BUILD_BASE} ${JAVA_SRC_PKG}/client/*.java
	cd ${JAVA_BUILD_BASE} && $(JAR) cvf $(JARFLAGS) ${abs_top_builddir}/${wtf_client_jarfile} org/wtf/client/*.class

#################################### Fuse #########################################



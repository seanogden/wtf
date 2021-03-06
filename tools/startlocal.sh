#!/usr/bin/zsh 

WTF_DIR=${HOME}/src/wtf

export WTF_COORD_LIB=${WTF_DIR}/.libs/libwtf-coordinator.so
WTF_COORDINATOR=${WTF_DIR}/.wtf_coordinator_host
WTF_PORT=1981
HYPERDEX_COORDINATOR=${WTF_DIR}/.hyperdex_coordinator_host
HYPERDEX_PORT=1982
WTF_DAEMONS=${WTF_DIR}/.wtf_daemon_hosts
HYPERDEX_DAEMONS=${WTF_DIR}/.hyperdex_daemon_hosts
WTF_CLIENTS=${WTF_DIR}/.wtf_client_hosts
DATA_DIR=${WTF_DIR}/wtf-data
WTF_COORDINATOR_DATA_DIR=${DATA_DIR}/wtf-coordinator
WTF_DAEMON_DATA_DIR=${DATA_DIR}/wtf-daemon
HYPERDEX_DAEMON_DATA_DIR=${DATA_DIR}/hyperdex-daemon
HYPERDEX_COORDINATOR_DATA_DIR=${DATA_DIR}/hyperdex-coordinator
HYPERDEX=hyperdex
WTF=${WTF_DIR}/wtf

if test '!' -d ${DATA_DIR}; then echo creating data directory && mkdir -p ${DATA_DIR}; fi

ALL_HOSTS=`mktemp`
sort .wtf_client_hosts .wtf_daemon_hosts .hyperdex_daemon_hosts .wtf_coordinator_host .hyperdex_coordinator_host | sort -u > ${ALL_HOSTS}

HYPERDEX_HOSTS=`mktemp`
sort .hyperdex_daemon_hosts .hyperdex_coordinator_host | sort -u > ${HYPERDEX_HOSTS}

WTF_HOSTS=`mktemp`
sort .wtf_daemon_hosts .wtf_coordinator_host | sort -u > ${WTF_HOSTS}

CLUSTER_HOSTS=`mktemp`
sort .wtf_daemon_hosts .hyperdex_daemon_hosts .wtf_coordinator_host .hyperdex_coordinator_host | sort -u > ${CLUSTER_HOSTS}

HC=`cat ${HYPERDEX_COORDINATOR}`
WC=`cat ${WTF_COORDINATOR}`

kill_cluster()
{
    echo "KILLING CLUSTER...\n"
    pkill -9 hyper
    pkill -9 replicant
    pkill -9 wtf 
}

clean_cluster()
{
    echo "CLEANING CLUSTER...\n"
    rm -rf ${DATA_DIR}
    mkdir -p ${WTF_COORDINATOR_DATA_DIR} \
        && mkdir ${WTF_DAEMON_DATA_DIR}
    mkdir ${HYPERDEX_COORDINATOR_DATA_DIR}
    mkdir ${HYPERDEX_DAEMON_DATA_DIR}
    rm wtf-*.log
    mkdir ${WTF_DAEMON_DATA_DIR}/data1
    mkdir ${WTF_DAEMON_DATA_DIR}/data2
    mkdir ${WTF_DAEMON_DATA_DIR}/data3
    mkdir ${WTF_DAEMON_DATA_DIR}/1
    mkdir ${WTF_DAEMON_DATA_DIR}/2
    mkdir ${WTF_DAEMON_DATA_DIR}/3
}

reset_cluster()
{
    kill_cluster
    clean_cluster
    echo "STARTING HYPERDEX COORDINATOR...\n"
    ${HYPERDEX} coordinator -D ${HYPERDEX_COORDINATOR_DATA_DIR} -l ${HC} -p ${HYPERDEX_PORT}
    sleep 1
    echo "STARTING HYPERDEX DAEMONS...\n"
    ${HYPERDEX} daemon -D ${HYPERDEX_DAEMON_DATA_DIR} -c ${HC} -P ${HYPERDEX_PORT} -t 1
    sleep 1
    echo "ADDING WTF SPACE...\n"
    echo 'space wtf key path attributes string blockmap, int directory, int mode, string owner, string group, int time' | ${HYPERDEX} add-space -h ${HC} -p ${HYPERDEX_PORT}
    sleep 1
    ./wtf-mkfs -H ${HC} -P ${HYPERDEX_PORT}
    echo "STARTING WTF COORDINATOR...\n"
    ${WTF} coordinator -D ${WTF_COORDINATOR_DATA_DIR} -l ${WC} -p ${WTF_PORT} -d
    sleep 1
    echo "STARTING WTF DAEMON #1...\n"
    ${WTF} daemon -D ${WTF_DAEMON_DATA_DIR}/1 -M ${WTF_DAEMON_DATA_DIR}/data1/metadata -p 2013 -c ${WC} -P ${WTF_PORT} -t 1 -d
    sleep 1
    echo "STARTING WTF DAEMON #2...\n"
#    ${WTF} daemon -D ${WTF_DAEMON_DATA_DIR}/2 -M ${WTF_DAEMON_DATA_DIR}/data2/metadata -p 2014 -c ${WC} -P ${WTF_PORT} -t 1 -d
    echo "STARTING WTF DAEMON #3...\n"
#    ${WTF} daemon -D ${WTF_DAEMON_DATA_DIR}/3 -M ${WTF_DAEMON_DATA_DIR}/data3/metadata -p 2015 -c ${WC} -P ${WTF_PORT} -t 1 -d
    #libtool --mode=execute gdb --args ${WTF} daemon -D ${WTF_DAEMON_DATA_DIR}/2 -M ${WTF_DAEMON_DATA_DIR}/data2/metadata -p 2014 -c ${WC} -P ${WTF_PORT} -t 1 -f
    #libtool --mode=execute gdb --args ${WTF} daemon -D ${WTF_DAEMON_DATA_DIR}/2 -M ${WTF_DAEMON_DATA_DIR}/data2/metadata -p 2014 -c ${WC} -P ${WTF_PORT} -t 1 -f
}
#
#run_iteration()
#{
#    echo "RUNNING ITERATION...\n"
#    ${PSSH} -p 100 -t 6000 -h ${WTF_CLIENTS}  -i \
#        ${WTF_DIR}/$1 -h ${WC} -p ${WTF_PORT} -H ${HC} -P ${HYPERDEX_PORT} \
#        --file-length=$((64-PSSH_NODENUM)) --file-charset='hex' \
#        --value-length=$2 -n $3 --output=$1'$((PSSH_NODENUM))'.log
#}

reset_cluster
#run_iteration wtf-sync-benchmark 512 10
#kill_cluster

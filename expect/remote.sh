#!/bin/sh 

# [usage]
# ./remote.sh <chassis> <slot>
#
# [config file]
# file path : /storage/etc/dev_ip.map
# format:
#     <chassis> <slot> <ip> <port> <user>
# example:
# 1    5    192.168.1.1    22    root


if [[ $# != 2 ]]
then
    #echo $#
    echo "Usage: "
    echo "    $0 <chassis> <slot>"
    echo "    eg: $0 1 5"
    echo ""
    exit
fi

chassis=$1
slot=$2
remote_ip=
remote_port=22
remote_user="root"
config_file="/storage/etc/dev_ip.map"

get_remote_ip()
{
    if [ -f $config_file ]; then
        while read line
        do
            val_chassis=`echo $line | awk -F ' ' '{print $1}'`
            val_slot=`echo $line | awk -F ' ' '{print $2}'`
            val_ip=`echo $line | awk -F ' ' '{print $3}'`
            val_port=`echo $line | awk -F ' ' '{print $4}'`
            val_user=`echo $line | awk -F ' ' '{print $5}'`

            if [[ "$val_chassis" == "$1" ]] && [[ "$val_slot" == "$2" ]]
            then
                remote_ip=$val_ip
                remote_port=$val_port
                remote_user=$val_user
                break
            fi
        done < $config_file

        if [ ! "$remote_ip" ] || [ -z "$remote_ip" ]
        then
            echo "remote ip address not find."
            exit
        fi
    else
        ## calculate device ip according to chassis and slot
	remote_ip="192.168."$chassis"."$slot
    fi
}

get_remote_ip $chassis $slot

ssh -p ${remote_port} ${remote_user}@${remote_ip}


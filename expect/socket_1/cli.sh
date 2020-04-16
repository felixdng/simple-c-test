#!/usr/bin/expect

#usage:
#./cli.sh <srv_ip> <srv_port>

#param
set srv_ip      [lindex $argv 0]
set srv_port    [lindex $argv 1]

#set chan [socket 127.0.0.1 12004]
set chan [socket $srv_ip $srv_port]
fconfigure $chan -buffering none

puts "connect to service ok."

gets $chan recv_buf
puts "receive from service : $recv_buf"

puts $chan "commond execute success."

close $chan


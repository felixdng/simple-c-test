#!/bin/expect

#test.sh remote_user remote_passwd remote_ip ssh_port prompt socket_port

#param
set ssh_user    [lindex $argv 0]
set ssh_pwd     [lindex $argv 1]
set ssh_ip      [lindex $argv 2]
set ssh_port    [lindex $argv 3]
set cmd_prompt  [lindex $argv 4]
set sock_port   [lindex $argv 5]

#login remote
spawn ssh $ssh_user@$ssh_ip -p $ssh_port
expect {
    "yes/no" { send "yes\r"; exp_continue }
    "password:" { send "$ssh_pwd\r"; exp_continue }
    $cmd_prompt { send "\r" }
    default { 
    puts $output "ssh link fail\r\n"; 
    exit}
}

#buffer max and timeout
match_max 65535
set timeout 100

#socket
set chan [socket 127.0.0.1 $sock_port]
#socket buffer line
fconfigure $chan -buffering line

puts "connetc to server ok."

while 1 {
    if {[gets $chan chars] == -1} break
    if { $chars eq "exit" } {
        send "$chars\r"
        break
    }

    send "$chars\r"
    expect -re "\r\n(.*)\r\n(.*)$cmd_prompt"
    set outcome $expect_out(1,string)
    puts $chan $outcome
}

expect eof
exit

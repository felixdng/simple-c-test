#!/bin/expect -d

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
set timeout 10

#socket
socket -myaddr 127.0.0.1 -server callback $sock_port
puts "service start..."

proc callback {chan addr port} {
fconfigure $chan -blocking 0 -buffering line;
fileevent $chan readable [list readcallback $chan];
puts "in callback"
}

proc readcallback {socketid} {
        puts "in readcallback"
        if {[eof $socketid] || [catch {gets $socketid chars} err]} {
                close $socketid;
        } else {
		send "ifconfig\r";
		expect {
			-re "\r\n\$cmd_prompt\s{0,}$chars\s{0,}\r\n(.*)\$cmd_prompt" {
				set outcome $expect_out(2,string);
				puts $socketid "$outcome";
				flush $socketid;
				exp_continue
			}
			eof {
				close $socketid;
				exit
			}
			timeout {
				close $socketid;
				exit
			}
		}
        }
}

vwait forever
expect eof
exit

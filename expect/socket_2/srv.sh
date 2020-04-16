#!/usr/bin/expect

#usage:
#./srv.sh <srv_ip> <srv_port>

#param
set srv_ip	[lindex $argv 0]
set srv_port	[lindex $argv 1]

socket -myaddr $srv_ip -server callback $srv_port
puts "service start..."

proc callback {chan addr port} {
fconfigure $chan -blocking 0 -buffering line;
fileevent $chan readable [list readcallback $chan];
puts "callback"
}

proc readcallback {socketid} {
	puts "readcallback"
	if {[eof $socketid] || [catch {gets $socketid line} err]} {
		close $socketid;
	} else {
		#deal with line; suppose line is tcl scripts eval it and return the results
		#puts line=$line
		#if {[catch {eval $line} err]} {
		#	puts $socketid $err
		#} else {
		#	puts $socketid $err
		#}
		puts $line
		if {$line eq "exit"} {
			close $socketid;
			exit;
		}
		puts $socketid "the number is NO.9527"
	}
}

vwait forever

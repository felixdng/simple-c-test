#!/bin/bash

types=(
"md5" 
"sha256" 
"sha512"
)

if [ $# -eq 2 ]; then
  for (( i=0; i<${#types[@]}; i++  ))
  do
    if [ "$2" == "${types[i]}" ]; then
		echo -n "$1" | openssl ${types[i]}
		break
	fi
  done
else
  echo "$0 <string> <type>"
  echo "types:"
  for (( i=0; i<${#types[@]}; i++  ))
  do
    echo "  ${types[i]}"
  done
fi


#!/bin/bash

CUR_DIR=$(pwd)

function myrealpath() {
	if [ $# -eq 1 ] && [ -n "$1" ]; then
		dirn=$(cd $(dirname "$1"); pwd)
		basen=$(basename "$1")
		# 'basename .' output '.', ignore it.
		[ "$basen" == "." ] && basen=""

		if [ -n "$dirn" ] && [ -n "$basen" ]; then
			realp="${dirn}/${basen}"
		elif [ -n "$dirn" ] && [ ! -n "$basen" ]; then
			realp="${dirn}"
		fi

		if [ -d "$realp" ] || [ -f "$realp" ]; then
			echo "$realp" | sed "s/\([\/][\/]\+\)/\//g"
		fi
	fi
}

function pro_var_set() {
	if [ $# -eq 1 ] && [ -n "$1" ]; then
		PRO_FILE=`myrealpath $1`
	fi
}
function out_var_set() {
	if [ $# -eq 1 ] && [ -n "$1" ]; then
		OUT_DIR=`myrealpath $1`
		if [ -d "${OUT_DIR}" ]; then
			OBJ_FILE="${OUT_DIR}/.obj.files"
			IGN_FILE="${OUT_DIR}/.ignore.files"
			CS_FILE="${OUT_DIR}/csfile.lists"
			CS_OUT="${OUT_DIR}/cscope.out"
		fi
	fi
}

# default params
pro_var_set "${CUR_DIR}/project_files"
out_var_set "${CUR_DIR}"

function pr_help() {
	echo "Usage: $0 [OPTION]..."
	echo "Create cscope symbol files of project."
	echo "The current directory by default output directory,"
	echo "and the \"./project_files\" by default project files list."
	echo "options:"
	echo "  help                   Display this help and exit."
	echo "  pf=FILE                Project files list."
	echo "  out=OUT_DIR            Output directory."
}

# parse command params
if [ $# -ge 1 ]; then
	for i in $@; do
		if [ $# -eq 1 ] && [ "$i" == "help" ]; then
			pr_help
			exit 0
		fi

		arg=$(echo "$i" | awk -F '=' '{if ($1 == "pf") print $2;}')
		if [ -n "$arg" ]; then
			pro_var_set $arg
			continue
		fi

		arg=$(echo "$i" | awk -F '=' '{if ($1 == "out") print $2;}')
		if [ -n "$arg" ]; then
			out_var_set $arg
			continue
		fi

		echo "$0: invalid arguments." 1>&2
		echo "Try '$0 help' for more information." 1>&2
		exit 1
	done
fi

function del_temp_files() {
	rm -f ${OBJ_FILE} ${IGN_FILE}
}

del_temp_files

# parse PRO_FILE, output OBJ_FILE and IGN_FILE
if [ -d "${OBJ_FILE%/*}" ] && [ -f "${PRO_FILE}" ]; then
	awk -v objfile="$OBJ_FILE" -v ignfile="$IGN_FILE" '{\
		for (i = 1; i <= NF; i++) {\
			if ($i ~ /^!/) {sub(/!/,"",$i); print $i > ignfile;} \
			else {print $i > objfile;}}\
	}' $PRO_FILE
else
	[ ! -d "${OBJ_FILE%/*}" ] && echo "Error: invalid directory 'OUT_DIR:{${OBJ_FILE%/*}}'." 1>&2
	[ ! -f "${PRO_FILE}" ] && echo "Error: file 'PRO_FILE:{${PRO_FILE}}' is not exist." 1>&2
	exit 1
fi

# Not existed OBJ_FILE, nothing to do.
if [ ! -f "${OBJ_FILE}" ]; then
	exit 1
fi

# find the target files and output CS_FILE
rm -f `find "$(dirname ${CS_FILE})" -name "*$(basename ${CS_FILE})*"`
while read line
do
	[ ! -n "$line" ] && continue
	_path=`myrealpath $line`
	if [ -d "${_path}" ]; then
		find "${_path}" -name "*.h" -or -name "*.c" -or -name "*.cc" -or -name "*.cpp" >> "$CS_FILE"
	elif [ -f "${_path}" ]; then
		echo "${_path}" >> "$CS_FILE"
	fi
done < "${OBJ_FILE}"

# delete ignore files from CS_FILE
if [ -f "${IGN_FILE}" ]; then
while read line
do
	[ ! -n "$line" ] && continue
	_path=`myrealpath $line`
	if [ -n "${_path}" ]; then
		_ex=`echo ${_path} | awk '{gsub(/\//,"\/");print}'`
		sed -i "/^${_ex}/d" "$CS_FILE"
	fi
done < "${IGN_FILE}"
fi

# sort and delete duplicate files
if [ -f "${CS_FILE}" ]; then
	#cp -f "${CS_FILE}" "$(dirname "${CS_FILE}")"/".$(basename "${CS_FILE}").old"
	sort "${CS_FILE}" | uniq > "${CS_FILE}.uniq"
	mv "${CS_FILE}.uniq" "${CS_FILE}"
fi

# cscope
if [ -f "${CS_FILE}" ]; then
	cscope -bkq -f "${CS_OUT}" -i "${CS_FILE}"
fi

# end
del_temp_files

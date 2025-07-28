#!/bin/bash
function DoSource() { local ScriptName="$1" ; shift ; source "$ScriptName" ; }

#https://stackoverflow.com/questions/59895/how-do-i-get-the-directory-where-a-bash-script-is-located-from-within-the-script
SCRIPT_DIR=$(cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd)

config_filename="tvsapp"
DoSource $SCRIPT_DIR/utils.sh "$config_filename"

delta=${1:-1}

ports=($(get_ports))
IFS=$'\n'; arr=($(sort -n <<<"${ports[*]}")); unset IFS;
ports=("${arr[@]}"); unset arr;


if [[ ${#ports[@]} -eq 0 ]]; then
    echo "No ports found in the Nginx configuration."
else
    for (( count=${#ports[@]}, idx=(count-1); idx>=0 && idx>=(count-delta); idx--)); do
        port=${ports[$idx]}
        service="$config_filename@$port.service"
        unset 'ports[idx]'
        if systemctl is-active --quiet "$service" ; then
            echo "Stopping instance on port $port..."
            systemctl stop "$service"
        else
            echo "$service already stopped"   
        fi
    done

    for idx in "${!ports[@]}"; do
        arr+=("${ports[idx]}")
    done
    ports=("${arr[@]}"); unset arr;

    write_ports "${ports[@]}"
fi

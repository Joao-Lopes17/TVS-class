#!/bin/bash
function DoSource() { local ScriptName="$1" ; shift ; source "$ScriptName" ; }

#https://stackoverflow.com/questions/59895/how-do-i-get-the-directory-where-a-bash-script-is-located-from-within-the-script
SCRIPT_DIR=$(cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd)

config_file="tvsapp"
DoSource $SCRIPT_DIR/utils.sh "$config_file"

stop_services() {
    echo ":: Stopping TVS app services ::"
    ports=($(get_ports))

    if [[ ${#ports[@]} -eq 0 ]]; then
        echo "No ports found in the Nginx configuration."
    else
        for port in ${ports[@]}; do
            service="$config_filename@$port.service"
            if systemctl is-active --quiet $service ; then
                echo "Stopping $service"
                systemctl stop $service
            else
                echo "$service already stopped"  
            fi
        done
    fi
}

stop_fixed_services() {
    echo ":: Stopping fixed services ::"

    if systemctl is-active --quiet nginx ; then
        echo "Stopping nginx"
        systemctl stop nginx
    else 
        echo "nginx already stopped"  
    fi

    if [[ "$1" == "-db" ]]; then
        if systemctl is-active --quiet elasticsearch ; then
            echo "Stopping elasticsearch"
            systemctl stop elasticsearch
        else 
            echo "elasticsearch already stopped"
        fi
    fi
}

stop_services
stop_fixed_services "$1"

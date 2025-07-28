#!/bin/bash
function DoSource() { local ScriptName="$1" ; shift ; source "$ScriptName" ; }

#https://stackoverflow.com/questions/59895/how-do-i-get-the-directory-where-a-bash-script-is-located-from-within-the-script
SCRIPT_DIR=$(cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd)

config_file="tvsapp"
DoSource $SCRIPT_DIR/utils.sh "$config_file"

start_services() {
    echo ":: Starting TVS app services ::"
    ports=($(get_ports))

    if [[ ${#ports[@]} -eq 0 ]]; then
        echo "No ports found in the Nginx configuration."
    else
        for port in ${ports[@]}; do
            service="$config_filename@$port.service"
            if ! systemctl is-active --quiet $service; then
                echo "Starting $service"
                systemctl start $service 
            else
                echo "$service already started"
            fi
        done
    fi
}

start_fixed_services() {
    echo ":: Starting fixed services ::"

    if ! systemctl is-active --quiet elasticsearch; then
        echo "Starting elasticsearch..."
        systemctl start elasticsearch
    else
        echo "elasticsearch already started"    
    fi

    echo "Restarting Nginx..."
    systemctl restart nginx
}

start_services
start_fixed_services
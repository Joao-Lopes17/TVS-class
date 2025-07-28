#!/bin/bash
function DoSource() { local ScriptName="$1" ; shift ; source "$ScriptName" ; }

#https://stackoverflow.com/questions/59895/how-do-i-get-the-directory-where-a-bash-script-is-located-from-within-the-script
SCRIPT_DIR=$(cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd)

config_file="tvsapp"
DoSource $SCRIPT_DIR/utils.sh "$config_file"

check_services() {
    echo ":: Checking services for TVS app ports ::"
    ports=($(get_ports))

    if [[ ${#ports[@]} -eq 0 ]]; then
        echo "No ports found in the Nginx configuration."
    else
        for port in ${ports[@]}; do
            service="$config_filename@$port.service"
            systemctl status $service --no-pager
        done
    fi
}

check_fixed_services() {
    echo ":: Checking fixed services ::"
    systemctl status elasticsearch --no-pager
    systemctl status nginx --no-pager
}

check_services
check_fixed_services
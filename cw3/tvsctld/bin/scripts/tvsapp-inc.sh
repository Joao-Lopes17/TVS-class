#!/bin/bash
function DoSource() { local ScriptName="$1" ; shift ; source "$ScriptName" ; }

#https://stackoverflow.com/questions/59895/how-do-i-get-the-directory-where-a-bash-script-is-located-from-within-the-script
SCRIPT_DIR=$(cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd)

config_filename="tvsapp"
DoSource $SCRIPT_DIR/utils.sh "$config_filename"

DEFAULT_PORT=35000

delta=${1:-1}

ports=($(get_ports))
IFS=$'\n'; arr=($(sort -n <<<"${ports[*]}")); unset IFS;
ports=("${arr[@]}"); unset arr;

base=0
if [[ ${#ports[@]} -eq 0 ]]; then
    base=$DEFAULT_PORT
else
    base=(${ports[-1]})
fi

for ((i=1; i<($delta+1); i++)); do
    port=$(($base+$i))
    ports+=($port)   
    service="$config_filename@$port.service"
    if ! systemctl is-active --quiet "$service" ; then
        echo "Starting instance on port $port..."
        systemctl start "$service"
    else
        echo "$service already started"   
    fi
done

write_ports "${ports[@]}"
#!/bin/bash
function DoSource() { local ScriptName="$1" ; shift ; source "$ScriptName" ; }

#https://stackoverflow.com/questions/59895/how-do-i-get-the-directory-where-a-bash-script-is-located-from-within-the-script
SCRIPT_DIR=$(cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd)

config_filename="tvsapp"
DoSource $SCRIPT_DIR/utils.sh "$config_filename"

scale=${1:-1}
base=${2:-35000}

ports=($(get_ports))
IFS=$'\n'; arr=($(sort -n <<<"${ports[*]}")); unset IFS;
ports=("${arr[@]}"); unset arr;

for port in ${ports[@]}; do
    service="$config_filename@$port.service"
    if systemctl is-active --quiet $service ; then
        echo "Stopping $service"
        systemctl stop $service
    fi
done

for (( i=0; i<$scale; i++ )); do
    port=$(($base+$i))
    new_ports+=($port)
    service="$config_filename@$port.service"
    if systemctl is-active --quiet $service ; then
        echo "Stopping $service"
        systemctl stop $service
    fi
done

echo "Set $scale configurated instance(s) starting from port $base."

write_ports "${new_ports[@]}"
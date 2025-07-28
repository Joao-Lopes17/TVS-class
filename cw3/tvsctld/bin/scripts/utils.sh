#!/bin/bash
ADDRESS="127.0.0.1"
NGINX_SITES_AVAILABLE="/etc/nginx/sites-available"
config_filename="$1"
config_filepath="$NGINX_SITES_AVAILABLE/$config_filename"

get_ports()
{
    echo $(awk -v config_name="$config_filename" -v address="$ADDRESS" '/upstream/ && config_name,/}/ { if ($2 ~ address) print $2 }' "$config_filepath" | cut -d: -f2 | tr -d ';' | sort -n)
}

write_ports() 
{
    arr=("$@")
    IFS=$'\n'; sorted_ports=($(sort -n <<<"${arr[*]}")); unset IFS;
    awk -v set_ports="$(echo "${sorted_ports[@]}")" -v config_name="$config_filename" -v address="$ADDRESS" '
        BEGIN { 
            split(set_ports, ports); 
            for (i in ports) set[address ":" ports[i]] = 1;
        }
        /upstream/ && config_name { 
            print; 
            in_upstream = 1; 
            next;
        }
        in_upstream && /}/ {
            for (port in set) print "    server " port ";";
            print $0;
            in_upstream = 0;
            next;
        }
        !in_upstream { print }
    ' "$config_filepath" > "$config_filepath.tmp"

    if [[ -s "$config_filepath.tmp" ]]; then
        mv "$config_filepath.tmp" "$config_filepath"
    else
        echo "Error: Temporary file not created. Aborting."
        rm -f "$config_filepath.tmp"
        return 1
    fi

    if [[ ${#arr[@]} -ne 0 ]]; then
        $(nginx -s reload)
    fi
}


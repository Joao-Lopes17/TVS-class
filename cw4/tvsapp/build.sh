docker build -t tvsapp .

docker run -d -p 12001:8010 --name tvsapp1 tvsapp
docker run -d -p 12002:8010 --name tvsapp2 tvsapp
docker run -d -p 12003:8010 --name tvsapp3 tvsapp
docker run -d -p 12004:8010 --name tvsapp4 tvsapp

# 172.17.0.2:12001 docker -> endereço IP entre docker Host e a bridge network: protocolo tcp/udp
docker inspect tvsapp1 --format='{{.NetworkSettings.IPAddress}}:{{(index (index .NetworkSettings.Ports "8010/tcp") 0).HostPort}}'
docker inspect tvsapp2 --format='{{.NetworkSettings.IPAddress}}:{{(index (index .NetworkSettings.Ports "8010/tcp") 0).HostPort}}'
docker inspect tvsapp3 --format='{{.NetworkSettings.IPAddress}}:{{(index (index .NetworkSettings.Ports "8010/tcp") 0).HostPort}}'
docker inspect tvsapp4 --format='{{.NetworkSettings.IPAddress}}:{{(index (index .NetworkSettings.Ports "8010/tcp") 0).HostPort}}'

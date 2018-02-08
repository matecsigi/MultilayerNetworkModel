#!/bin/sh

echo "Hello World!"
counter=1
while :
do
    latestFolder=$(ls bin/generated/hebbianNetworks/ -t | head -2 | tail -1)
    echo $latestFolder

    mv bin/generated/hebbianNetworks bin/generated/hebbianNetworks-$counter
    mkdir bin/generated/hebbianNetworks
    mv bin/generated/parameters.txt bin/generated/parameters-$counter.txt
    mv bin/generated/log.txt bin/generated/log-$counter.txt

    mpirun -np 5 -hostfile ./clusterFile ./cluster bin/generated/hebbianNetworks-$counter/$latestFolder &
    wait
    ((counter++))
done

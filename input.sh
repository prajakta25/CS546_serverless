#!/bin/bash
declare -a bsize
declare -a freq
declare -a pattern=(0 0 0 0 0 0 0 0)

echo "Enter the number of messages to be transmitted : "
read n
echo "n=\"$n\"" > config
echo "Enter the buffer size for each message in KB "
for ((i=0; i<$n; i++))
do
echo "Buffer bsize for message $[$i+1]"
read bsize[i]
done
declare -p bsize >> config
echo ""
echo "Enter f"
read f
echo "f=\"$f\"" >> config
echo "Enter the frequency for each transmittion in ms : "
for ((i=0; i<$f; i++))
do
read freq[i]
done
declare -p freq >> config
flag=0
while [ $flag -lt 1 ]
do
    echo ""
    echo "Select"
    echo -e  "0. MPI Broadcast\n1. MPI Scatter\n2. MPI Gather\n3. MPI Scatter Gather\n4. MPI Send Receive\n5. MPI iSend iReceive\n6. MPI Alltoall Allreduce\n7.Alltoallv"
    read ch

    if [ ${pattern[$ch]} -eq 1 ]; then
        echo ""
        echo "Option is already selected"
    else
        pattern[$ch]=1
    fi

    echo ""
    echo "Do you want to select more option? y/n"
    read choice

    if [ $choice = "N" -o $choice = "n" ]; then
        flag=$(($flag+1))
    fi
done 

declare -p pattern >> config
echo ""
echo "Enter the no. of processors to be used"
read proc
echo "proc=\"$proc\"" >> config
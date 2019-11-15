#!/bin/bash
declare -a bsize
declare -a freq
declare -a pattern=(0 0 0 0 0 0 0)

echo "Enter the number of messages to be transmitted : "
read n
echo "Enter the buffer size for each message in KB "
for ((i=0; i<$n; i++))
do
echo "Buffer bsize for message $[$i+1]"
read bsize[i]
done

echo ""
echo "Enter f"
read f
echo "Enter the frequency for each transmittion in ms : "
for ((i=0; i<$f; i++))
do
read freq[i]
done

flag=0
while [ $flag -lt 1 ]
do
    echo ""
    echo "Select"
    echo -e  "0. MPI Broadcast\n1. MPI Scatter\n2. MPI Gather\n3. MPI Scatter Gather\n4. MPI Send Receive\n5. MPI iSend iReceive\n6. MPI Alltoall Allreduce"
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

echo ""
echo "Enter the no. of processors to be used"
read proc

for ((i=0; i< ${#pattern[*]}; i++ ))
do
    if [ ${pattern[$i]} -eq 1 ]; then
        for ((j=0;j<$n;j++))
        do 
            for ((k=0; k<$f; k++))
            do  
                if [ $i -eq 0 ]; then
                mpic++ bcast.cpp -o bcast -I '/usr/local/Cellar/mpich/3.3.1/include'
                g++ fork_ex.cpp -o fork_ex
                #time ./fork_ex "$proc" "${bsize[$j]}" "${freq[$k]}" bcast
                { time ls ;} 2> op.txt
                { time mpiexec -n "$proc" ./bcast "${bsize[$j]}" "${freq[$k]}" ;} 2> output.json
                echo ""
                fi

                if [ $i -eq 1 ]; then
                mpic++ scatter.cpp -o scatter -I '/usr/local/Cellar/mpich/3.3.1/include'
                { time mpiexec -n "$proc" ./scatter "${bsize[$j]}" "${freq[$k]}" ;} 2>> output.json
                echo ""
                fi

                if [ $i -eq 2 ]; then
                mpic++ gather.cpp -o gather -I '/usr/local/Cellar/mpich/3.3.1/include'
                time mpiexec -n "$proc" ./gather "${bsize[$j]}" "${freq[$k]}" >> output.json
                echo ""
                fi

                if [ $i -eq 3 ]; then
                mpic++ scatter_gather.cpp -o sg -I '/usr/local/Cellar/mpich/3.3.1/include'
                time mpiexec -n "$proc" ./sg "${bsize[$j]}" "${freq[$k]}" >> output.json
                echo ""
                fi

                if [ $i -eq 4 ]; then
                mpic++ send_receive.cpp -o sr -I '/usr/local/Cellar/mpich/3.3.1/include'
                time mpiexec -n "$proc" ./sr "${bsize[$j]}" "${freq[$k]}" >> output.json
                echo ""
                fi

                if [ $i -eq 5 ]; then
                mpic++ isend_recv.cpp -o isr -I '/usr/local/Cellar/mpich/3.3.1/include'
                time mpiexec -n "$proc" ./isr "${bsize[$j]}" "${freq[$k]}" >> output.json
                echo ""
                fi

                if [ $i -eq 6 ]; then
                mpic++ alltoall_allreduce.cpp -o aa_ar -I '/usr/local/Cellar/mpich/3.3.1/include'
                time mpiexec -n "$proc" ./aa_ar "${bsize[$j]}" "${freq[$k]}" >> output.json
                echo ""
                fi

            done
        done
    fi
done
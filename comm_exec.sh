#!/bin/bash
source config

echo "MPI_Collective Buffer_Size_KB Freqeuncy Time_Taken_ms" > output.txt
for ((i=0; i< ${#pattern[*]}; i++ ))
do
    if [ ${pattern[$i]} -eq 1 ]; then
        for ((j=0;j<$n;j++))
        do 
            for ((k=0; k<$f; k++))
            do  
                if [ $i -eq 0 ]; then
                mpic++ bcast.cpp -o bcast -I '/usr/local/Cellar/mpich/3.3.1/include'
                mpiexec -n "$proc" ./bcast "${bsize[$j]}" "${freq[$k]}" >> output.txt
                printf "."
                fi

                if [ $i -eq 1 ]; then
                mpic++ scatter.cpp -o scatter -I '/usr/local/Cellar/mpich/3.3.1/include'
                mpiexec -n "$proc" ./scatter "${bsize[$j]}" "${freq[$k]}" >> output.txt
                printf "."
                fi

                if [ $i -eq 2 ]; then
                mpic++ gather.cpp -o gather -I '/usr/local/Cellar/mpich/3.3.1/include'
                mpiexec -n "$proc" ./gather "${bsize[$j]}" "${freq[$k]}" >> output.txt
                printf "."
                fi

                if [ $i -eq 3 ]; then
                mpic++ scatter_gather.cpp -o sg -I '/usr/local/Cellar/mpich/3.3.1/include'
                mpiexec -n "$proc" ./sg "${bsize[$j]}" "${freq[$k]}" >> output.txt
                printf "."
                fi

                if [ $i -eq 4 ]; then
                mpic++ send_receive.cpp -o sr -I '/usr/local/Cellar/mpich/3.3.1/include'
                mpiexec -n "$proc" ./sr "${bsize[$j]}" "${freq[$k]}" >> output.txt
                printf "."
                fi

                if [ $i -eq 5 ]; then
                mpic++ isend_recv.cpp -o isr -I '/usr/local/Cellar/mpich/3.3.1/include'
                mpiexec -n "$proc" ./isr "${bsize[$j]}" "${freq[$k]}" >> output.txt
                printf "."
                fi

                if [ $i -eq 6 ]; then
                mpic++ alltoall.cpp -o atoa -I '/usr/local/Cellar/mpich/3.3.1/include'
                mpiexec -n "$proc" ./atoa "${bsize[$j]}" "${freq[$k]}" >> output.txt
                printf "."
                fi

                if [ $i -eq 7 ]; then
                mpic++ allreduce.cpp -o allreduce -I '/usr/local/Cellar/mpich/3.3.1/include'
                mpiexec -n "$proc" ./allreduce "${bsize[$j]}" "${freq[$k]}" >> output.txt
                printf "."
                fi

                if [ $i -eq 8 ]; then
                mpic++ alltoall_allreduce.cpp -o aa_ar -I '/usr/local/Cellar/mpich/3.3.1/include'
                mpiexec -n "$proc" ./aa_ar "${bsize[$j]}" "${freq[$k]}" >> output.txt
                printf "."
                fi

            done
        done
    fi
done

echo ""
python printTable.py
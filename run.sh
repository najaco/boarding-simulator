function usage {
    echo "$0 [-irs] <number_of_passengers>"
    echo "--------------------------"
    printf "    %-10s|%-20s\n" "FLAG" "USE"
    echo "--------------------------"
    printf "    %-10s|%-20s\n" "-i" "Inorder"
    printf "    %-10s|%-20s\n" "-r" "Reverse"
    printf "    %-10s|%-20s\n" "-s" "Shuffle"
    echo "--------------------------"
}
if [ $# -lt 2 ]; then
    usage
    exit 1
fi
number_of_passengers=$2
if [ "$1" == "-r" ]; then
    seq -s"\n" $number_of_passengers -1 1 | ./boarding-problem
elif [ $1 == "-s" ]; then
    seq 1000 | shuf | ./boarding-problem
else
    seq 1000 | ./boarding-problem
fi



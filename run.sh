function usage {
    echo "$0 [-h --help] [-irs] <number_of_passengers>"
    echo "--------------------------"
    printf "    %-10s|%-20s\n" "FLAG" "USE"
    echo "--------------------------"
    printf "    %-10s|%-20s\n" "-i" "Inorder"
    printf "    %-10s|%-20s\n" "-r" "Reverse"
    printf "    %-10s|%-20s\n" "-s" "Shuffle"
    echo "--------------------------"
}
if [ $# -lt 2 ] || [ "$1" == "-h" ] || [ "$1" == "--help" ]; then
    usage
    exit 1
fi
number_of_passengers=$2
make # compile if not done so
if [ "$1" == "-r" ]; then
    seq -s"\n" $number_of_passengers -1 1 | ./boarding-problem
elif [ $1 == "-s" ]; then
    seq $number_of_passengers | shuf | ./boarding-problem
else
    seq $number_of_passengers | ./boarding-problem
fi



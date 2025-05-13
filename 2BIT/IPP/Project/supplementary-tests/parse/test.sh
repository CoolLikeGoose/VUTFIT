#!/usr/bin/env bash
#!/bin/bash
#!/usr/bin/bash

PASSED=0
FAILED=0

isMerlin() {
    hostname | grep -q "merlin.fit.vutbr.cz"
    return $?
}

jexamxml_merlin="/pub/courses/ipp/jexamxml"

if isMerlin; then
    jexamxml="$jexamxml_merlin"
else
    # Change this variable if you have local installation of JExamXML 
    # with custom path different than merlin or current dir (".")
    jexamxml="./jexamxml"
fi

OK() {
	printf "  \e[32mOK\e[0m\n"
}

ERROR() {
	printf "  \e[31mError\e[0m: $1\n"

    if [[ -z "$NO_EXIT" ]]; then
        exit 1
    fi
}

if [[ -z "$BASH" ]]; then
	ERROR "This script must be run with bash"
fi

# Check if the interpreter is set
if [[ -z "$PARSE" ]]; then
    # Check if first argument is set
    if [[ -z "$1" ]]; then
        PARSE="parse.py"
    else
        PARSE="$1"
    fi
fi

if [[ ! -f "$PARSE" ]]; then
	ERROR "parse.py is missing; copy it here (or make a symlink, or specify \$PARSE/first argument) and try again"
    exit 1
fi

if [[ ! -f "$jexamxml"/jexamxml.jar ]]; then
	ERROR "jexamxml.jar is missing; are you running on merlin? \n
(alternatively, you can install Java and download the .jar and options file
from merlin on this path: $jexamxml_merlin/jexamxml.jar and put it in the ./jexamxml directory)"
    exit 1
fi

if [[ ! -f "$jexamxml"/options ]]; then
	ERROR "jexamxml options file is missing; are you running on merlin? \n
(alternatively, you can download the file from merlin on this path:
$jexamxml_merlin/options and put it in the ./jexamxml directory)"
    exit 1
fi

if ! command -v java > /dev/null; then
	ERROR "Java is not installed."
    exit 1
fi


PYTHON="python3.10"
if ! command -v "python3.10" > /dev/null; then
    PYTHON="python3"
    if ! command -v "python3" > /dev/null; then
        ERROR "Python 3 is not installed."
        exit 1
    fi
fi

for src in $(find "$PWD" -name '*.src');
do
	test="${src%.*}"
	echo "$(basename "$src" .src)"

	"$PYTHON" -O "$PARSE" < "$src" > "$test".your_out 2> /dev/null
	echo -n "$?" > "$test".your_rc

    PASS=1

    if [[ $(diff -bw "$test".rc "$test".your_rc) ]]; then
        ERROR "Exit code differs, expected $(cat "$test".rc), got $(cat "$test".your_rc)"
        PASS=0
    fi

    if [[ $PASS -ne 0 ]] && [[ $(cat "$test".your_rc) == "0" ]] && [[ -n "$(cat "$test.your_out")" ]] && ! java -jar "$jexamxml"/jexamxml.jar "$test".out "$test".your_out "$test"_diff.xml  /D "$jexamxml"/options > /dev/null; then
        ERROR "Output differs, see "$test"_diff.xml"
        PASS=0
    fi

    if [[ $PASS -eq 1 ]]; then
        OK
        PASSED=$((PASSED+1))
    else
        FAILED=$((FAILED+1))
    fi
done

echo ""
printf "\e[32mPASSED: $PASSED\e[0m\n"
printf "\e[31mFAILED: $FAILED\e[0m\n"
echo ""

if [[ ! -z "$CLEANUP" ]]; then
    echo "Cleaning up..."
    find "$PWD" \( -type f  -name '*.your_out' -o -name '*.your_rc' -o -name '*_diff.xml' -o -name "*.out.log" \) -delete 
fi

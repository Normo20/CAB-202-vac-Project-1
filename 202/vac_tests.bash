#!/bin/bash

test_num=0
REPORT_FILE=test_report.txt

echo "Remember to do these two commands before running this script:
export COLUMNS
export LINES
"
echo "Test run starting at $(date)" >>${REPORT_FILE}

#==============================================================================
#	Operations available in the ZDJ2 simulation.
#==============================================================================
function move_zombie () {
	printf "z${1},${2},${3},${4} " 
}

function move_hero () {
	printf "h${1},${2} " 
}

function set_timeout () {
	printf "t${1} "
}

function set_delay () {
	printf "m${1} "
}

function pause_resume () {
	printf "p "
}

function reset () {
	printf "r${1} "
}

function left () {
	printf "a "
}

function right () {
	printf "d "
}

function up () {
	printf "w "
}

function down () {
	printf "s "
}

function quit () {
	printf "q "
}

function loop () {
	if (( $# >= 1 )) && (( $1 > 0 )) 
	then
		for (( i = 0; i < $1; i++ ))
		do
			printf " "
		done
	else
		printf " "
	fi
}

function enter () {
	printf "${1} "
}

#========================================================================
#	Run a single test.
#========================================================================	

function run_test () {
	(( test_num ++ ))
	echo "Test    : ${test_num}"
	echo "Category: ${category}"
	echo "Details : ${details}"
	echo "Expect  : ${expect}"
	
	read -p "Press r to run the test or s to skip..." run_or_skip
	
	if [ "${run_or_skip}" == "r" ]
	then
		echo ${cmd} | ./zdj2
		read -p "Did the test pass or fail (p/f)? " result
		echo "Test ${test_num}: ${result}" >>${REPORT_FILE}
	fi
}

#==============================================================================
category="Setup hero"
details="Hero alone in center of small terminal window."
expect="Hero is displayed in centre; no zombie is visible."

cmd=$(
	enter 1
	pause_resume
	move_zombie 0 1000 1000 0
	set_timeout 5
)

run_test 
#==============================================================================
category="Setup zombie"
details="Hero and 1 zombies."
expect="Hero is displayed in centre; 1 zombie is visible; none overlap the hero or the border."

cmd=$(
	enter 1
	pause_resume
	set_timeout 5
)

run_test 
#==============================================================================
category="Setup zombie"
details="Hero and 3 zombies."
expect="Hero is displayed in centre; 3 zombies are visible; none overlap the hero or the border."

cmd=$(
	enter 3
	pause_resume
	set_timeout 5
)

run_test 
#==============================================================================
category="Setup zombie"
details="Hero and 5 zombies."
expect="Hero is displayed in centre; 5 zombies are visible; none overlap the hero or the border."

cmd=$(
	enter 5
	pause_resume
	set_timeout 5
)

run_test 
#==============================================================================
category="Setup zombie"
details="Hero and 10 zombies."
expect="Hero is displayed in centre; 10 zombies are visible; none overlap the hero or the border."

cmd=$(
	enter 10
	pause_resume
	set_timeout 5
)

run_test 

#==============================================================================
category="Do Hero Cheat"
details="Hero alone; placed in four corners of display, clockwise."
expect="Hero is displayed briefly in centre; no zombie is visible; hero teleports in turn to [5,5], [w-5,5], [w-5,h-5], [5,h-5]."

(( LEFT = 5, TOP = 5, RIGHT = COLUMNS - 5, BOTTOM = LINES - 5 ))

cmd=$(
	enter 1
	pause_resume
	move_zombie 0 1000 1000 0
	set_timeout 6
	set_delay 500
	move_hero ${LEFT} ${TOP}
	move_hero ${RIGHT} ${TOP}
	move_hero ${RIGHT} ${BOTTOM}
	move_hero ${LEFT} ${BOTTOM}
	quit
)

run_test

#==============================================================================
category="Update Hero"
details="Hero alone; runs left."
expect="Hero is displayed in centre; no zombie is visible; hero runs left until hitting the wall."

cmd=$(
	enter 1
	pause_resume
	move_zombie 0 1000 1000 0
	set_timeout 5
	for (( i = 0; i < COLUMNS; i++ )) ; do left ; done
	set_delay 1000
	quit
)

run_test

#==============================================================================
category="Update Hero"
details="Hero alone; runs right."
expect="Hero is displayed in centre; no zombie is visible; hero runs right until hitting the wall."

cmd=$(
	enter 1
	pause_resume
	move_zombie 0 1000 1000 0
	set_timeout 5
	for (( i = 0; i < COLUMNS; i++ )) ; do right ; done
	set_delay 1000
	quit
)

run_test

#==============================================================================
category="Update Hero"
details="Hero alone; runs north."
expect="Hero is displayed in centre; no zombie is visible; hero runs right until hitting the wall."

cmd=$(
	enter 1
	pause_resume
	move_zombie 0 1000 1000 0
	set_timeout 5
	for (( i = 0; i < COLUMNS; i++ )) ; do up ; done
	set_delay 1000
	quit
)

run_test

#==============================================================================
category="Update Hero"
details="Hero alone; runs south."
expect="Hero is displayed in centre; no zombie is visible; hero runs right until hitting the wall."

cmd=$(
	enter 1
	pause_resume
	move_zombie 0 1000 1000 0
	set_timeout 5
	for (( i = 0; i < COLUMNS; i++ )) ; do down ; done
	set_delay 1000
	quit
)

run_test

#==============================================================================
category="Update Hero"
details="Hero alone; runs left and right at top."
expect="Hero is displayed at top left corner; no zombie is visible; hero runs right and left until hitting the wall."

cmd=$(
	enter 1
	pause_resume
	set_delay 5
	move_zombie 0 1000 1000 0
	move_hero 5 5
	set_timeout 15
	for (( i = 0; i < COLUMNS * 11 / 10; i++ )) ; do right ; done
	for (( i = 0; i < COLUMNS * 11 / 10; i++ )) ; do left ; done
	for (( i = 0; i < COLUMNS * 11 / 10; i++ )) ; do right ; done
	for (( i = 0; i < COLUMNS * 11 / 10; i++ )) ; do left ; done
	set_delay 1000
	quit
)

run_test

#==============================================================================
category="Update Hero"
details="Hero alone; runs up and down at top."
expect="Hero is displayed at top left corner; no zombie is visible; hero runs up and down until hitting the wall."

cmd=$(
	enter 1
	pause_resume
	set_delay 5
	move_zombie 0 1000 1000 0
	move_hero 5 5
	set_timeout 15
	for (( i = 0; i < LINES * 2; i++ )) ; do down ; done
	for (( i = 0; i < LINES * 2; i++ )) ; do up ; done
	for (( i = 0; i < LINES * 2; i++ )) ; do down ; done
	for (( i = 0; i < LINES * 2; i++ )) ; do up ; done
	set_delay 1000
	quit
)

run_test

#==============================================================================
category="Do Zombie Cheat"
details="Zombie alone; placed in four corners of display, clockwise."
expect="Zombie is displayed briefly at random location; Zombie teleports in turn to [5,5], [w-5,5], [w-5,h-5], [5,h-5]."

(( LEFT = 5, TOP = 5, RIGHT = COLUMNS - 5, BOTTOM = LINES - 5 ))

cmd=$(
	enter 1
	pause_resume
	move_hero 0 1000 1000 0
	set_timeout 6
	set_delay 500
	move_zombie 0 ${LEFT} ${TOP} 0
	move_zombie 0 ${RIGHT} ${TOP} 0
	move_zombie 0 ${RIGHT} ${BOTTOM} 0
	move_zombie 0 ${LEFT} ${BOTTOM} 0
	set_delay 1000
	quit
)

run_test

#==============================================================================
category="Update Zombie"
details="Zombie alone; run right and bounce."
expect="Zombie is displayed in centre; Hero is missing; zombie moves right until hitting wall, and bounces back."

(( CX = COLUMNS / 2, CY = LINES / 2, DIR = 0 ))

cmd=$(
	enter 1
	pause_resume
	set_delay 5
	move_hero 1000 1000
	move_zombie 0 ${CX} ${CY} ${DIR}
	set_timeout 5
	pause_resume
)

run_test

#==============================================================================
category="Update Zombie"
details="Zombie alone; run left and bounce."
expect="Zombie is displayed in centre; Hero is missing; zombie moves left until hitting wall, and bounces back."

(( CX = COLUMNS / 2, CY = LINES / 2, DIR = 180 ))

cmd=$(
	enter 1
	pause_resume
	set_delay 5
	move_hero 1000 1000
	move_zombie 0 ${CX} ${CY} ${DIR}
	set_timeout 5
	pause_resume
)

run_test

#==============================================================================
category="Update Zombie"
details="Zombie alone; run north and bounce."
expect="Zombie is displayed in centre; Hero is missing; zombie moves north until hitting wall, and bounces back."

(( CX = COLUMNS / 2, CY = LINES / 2, DIR = 270 ))

cmd=$(
	enter 1
	pause_resume
	set_delay 5
	move_hero 1000 1000
	move_zombie 0 ${CX} ${CY} ${DIR}
	set_timeout 5
	pause_resume
)

run_test

#==============================================================================
category="Update Zombie"
details="Zombie alone; run south and bounce."
expect="Zombie is displayed in centre; Hero is missing; zombie moves south until hitting wall, and bounces back."

(( CX = COLUMNS / 2, CY = LINES / 2, DIR = 90 ))

cmd=$(
	enter 1
	pause_resume
	set_delay 5
	move_hero 1000 1000
	move_zombie 0 ${CX} ${CY} ${DIR}
	set_timeout 5
	pause_resume
)

run_test

#==============================================================================
category="Update Zombie"
details="Zombie alone; run south-east and bounce."
expect="Zombie is displayed in centre; Hero is missing; zombie moves south-east and reflects from wall."

(( CX = COLUMNS / 2, CY = LINES / 2, DIR = 45 ))

cmd=$(
	enter 1
	pause_resume
	set_delay 5
	move_hero 1000 1000
	move_zombie 0 ${CX} ${CY} ${DIR}
	set_timeout 5
	pause_resume
)

run_test

#==============================================================================
category="Update Zombie"
details="Zombie alone; run north-east and bounce."
expect="Zombie is displayed in centre; Hero is missing; zombie moves north-east and reflects from wall."

(( CX = COLUMNS / 2, CY = LINES / 2, DIR = 365 - 45 ))

cmd=$(
	enter 1
	pause_resume
	set_delay 5
	move_hero 1000 1000
	move_zombie 0 ${CX} ${CY} ${DIR}
	set_timeout 5
	pause_resume
)

run_test

#==============================================================================
category="Update Zombie"
details="Zombie alone; run south-west and bounce."
expect="Zombie is displayed in centre; Hero is missing; zombie moves south-west and reflects from wall."

(( CX = COLUMNS / 2, CY = LINES / 2, DIR = 180 - 45 ))

cmd=$(
	enter 1
	pause_resume
	set_delay 5
	move_hero 1000 1000
	move_zombie 0 ${CX} ${CY} ${DIR}
	set_timeout 5
	pause_resume
)

run_test

#==============================================================================
category="Update Zombie"
details="Zombie alone; run north-west and bounce."
expect="Zombie is displayed in centre; Hero is missing; zombie moves north-west and reflects from wall."

(( CX = COLUMNS / 2, CY = LINES / 2, DIR = 180 + 45 ))

cmd=$(
	enter 1
	pause_resume
	set_delay 5
	move_hero 1000 1000
	move_zombie 0 ${CX} ${CY} ${DIR}
	set_timeout 5
	pause_resume
)

run_test

#==============================================================================
category="Collision"
details="Zombie passes just north of hero (near miss)."
expect="Hero is displayed in centre; Zombie is above hero and to the left; Zombie runs past hero, just missing."

(( CX = COLUMNS / 2, CY = LINES / 2, ZX = CX - 12, ZY = CY - 4, DIR = 0 ))

cmd=$(
	enter 1
	pause_resume
	set_delay 5
	move_zombie 0 ${ZX} ${ZY} ${DIR}
	set_timeout 5
	pause_resume
)

run_test

#==============================================================================
category="Collision"
details="Zombie passes just south of hero (near miss)."
expect="Hero is displayed in centre; Zombie is below hero and to the left; Zombie runs past hero, just missing."

(( CX = COLUMNS / 2, CY = LINES / 2, ZX = CX - 12, ZY = CY + 4, DIR = 0 ))

cmd=$(
	enter 1
	pause_resume
	set_delay 5
	move_zombie 0 ${ZX} ${ZY} ${DIR}
	set_timeout 5
	pause_resume
)

run_test

#==============================================================================
category="Collision"
details="Zombie passes just west of hero (near miss)."
expect="Hero is displayed in centre; Zombie is below hero and to the left; Zombie runs north past hero, just missing."

(( CX = COLUMNS / 2, CY = LINES / 2, ZX = CX - 8, ZY = CY + 6, DIR = 270 ))

cmd=$(
	enter 1
	pause_resume
	set_delay 5
	move_zombie 0 ${ZX} ${ZY} ${DIR}
	set_timeout 5
	pause_resume
)

run_test

#==============================================================================
category="Collision"
details="Zombie passes just east of hero (near miss)."
expect="Hero is displayed in centre; Zombie is below hero and to the right; Zombie runs north past hero, just missing."

(( CX = COLUMNS / 2, CY = LINES / 2, ZX = CX + 8, ZY = CY + 6, DIR = 270 ))

cmd=$(
	enter 1
	pause_resume
	set_delay 5
	move_zombie 0 ${ZX} ${ZY} ${DIR}
	set_timeout 5
	pause_resume
)

run_test

#==============================================================================
category="Collision"
details="Zombie clips north edge of hero (hit)."
expect="Hero is displayed in centre; Zombie is above hero and to the left; Zombie runs east and collides with hero."

(( CX = COLUMNS / 2, CY = LINES / 2, ZX = CX - 12, ZY = CY - 3, DIR = 0 ))

cmd=$(
	enter 1
	pause_resume
	set_delay 5
	move_zombie 0 ${ZX} ${ZY} ${DIR}
	set_timeout 5
	pause_resume
)

run_test

#==============================================================================
category="Collision"
details="Zombie clips south edge of hero (hit)."
expect="Hero is displayed in centre; Zombie is below hero and to the left; Zombie runs east and collides with hero"

(( CX = COLUMNS / 2, CY = LINES / 2, ZX = CX - 12, ZY = CY + 3, DIR = 0 ))

cmd=$(
	enter 1
	pause_resume
	set_delay 5
	move_zombie 0 ${ZX} ${ZY} ${DIR}
	set_timeout 5
	pause_resume
)

run_test

#==============================================================================
category="Collision"
details="Zombie clips west edge of hero (hit)."
expect="Hero is displayed in centre; Zombie is below hero and to the left; Zombie runs north and collides with hero."

(( CX = COLUMNS / 2, CY = LINES / 2, ZX = CX - 7, ZY = CY + 6, DIR = 270 ))

cmd=$(
	enter 1
	pause_resume
	set_delay 5
	move_zombie 0 ${ZX} ${ZY} ${DIR}
	set_timeout 5
	pause_resume
)

run_test

#==============================================================================
category="Collision"
details="Zombie clips east edge of hero (hit)."
expect="Hero is displayed in centre; Zombie is below hero and to the right; Zombie runs north and collides with hero."

(( CX = COLUMNS / 2, CY = LINES / 2, ZX = CX + 7, ZY = CY + 6, DIR = 270 ))

cmd=$(
	enter 1
	pause_resume
	set_delay 5
	move_zombie 0 ${ZX} ${ZY} ${DIR}
	set_timeout 5
	pause_resume
)

run_test

#==============================================================================
category="Collision"
details="Zombie clips north edge of hero (hit)."
expect="Hero is displayed in centre; Zombie is above hero and to the right; Zombie runs west and collides with hero."

(( CX = COLUMNS / 2, CY = LINES / 2, ZX = CX + 12, ZY = CY - 3, DIR = 180 ))

cmd=$(
	enter 1
	pause_resume
	set_delay 5
	move_zombie 0 ${ZX} ${ZY} ${DIR}
	set_timeout 5
	pause_resume
)

run_test

#==============================================================================
category="Collision"
details="Zombie clips south edge of hero (hit)."
expect="Hero is displayed in centre; Zombie is below hero and to the right; Zombie runs west and collides with hero. "

(( CX = COLUMNS / 2, CY = LINES / 2, ZX = CX + 12, ZY = CY + 3, DIR = 180 ))

cmd=$(
	enter 1
	pause_resume
	set_delay 5
	move_zombie 0 ${ZX} ${ZY} ${DIR}
	set_timeout 5
	pause_resume
)

run_test

#==============================================================================
category="Collision"
details="Zombie clips west edge of hero (hit)."
expect="Hero is displayed in centre; Zombie is above hero and to the left; Zombie runs south and collides with hero."

(( CX = COLUMNS / 2, CY = LINES / 2, ZX = CX - 7, ZY = CY - 6, DIR = 90 ))

cmd=$(
	enter 1
	pause_resume
	set_delay 5
	move_zombie 0 ${ZX} ${ZY} ${DIR}
	set_timeout 5
	pause_resume
)

run_test

#==============================================================================
category="Collision"
details="Zombie clips east edge of hero (hit)."
expect="Hero is displayed in centre; Zombie is above hero and to the right; Zombie runs south and collides with hero."

(( CX = COLUMNS / 2, CY = LINES / 2, ZX = CX + 7, ZY = CY - 6, DIR = 90 ))

cmd=$(
	enter 1
	pause_resume
	set_delay 5
	move_zombie 0 ${ZX} ${ZY} ${DIR}
	set_timeout 5
	pause_resume
)

run_test
while :
do
	sleep 60
	if pidof -s test > /dev/null; then
    		echo 'It is already running!'
	else
    	echo 'process not found...'
    		~/Programs/breakpad_example/test
	fi

done



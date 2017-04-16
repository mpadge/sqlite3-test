#!/bin/sh
SESSION="junk"

tmux -2 new-session -d -s $SESSION

cd ./src/
tmux new-window -t $SESSION:1 -k -n src
tmux send-keys -t $SESSION:1 'vim junk.h' C-m
tmux send-keys -t $SESSION:1 ':' 'tabe junk.c++' C-m

tmux split-window -h
tmux send-keys -t $SESSION:1 'cd ..' C-m
tmux send-keys -t $SESSION:1 'vim README.md' C-m
tmux select-pane -t 0

cd ../
tmux new-window -t $SESSION:2 -n makefile
tmux select-window -t $SESSION:2
tmux send-keys -t $SESSION:2 'vim tmux-start.bash' C-m
tmux send-keys -t $SESSION:2 ':' 'tabe CMakeLists.txt' C-m
tmux send-keys -t $SESSION:2 ':' 'tabe build/mycmake.bash' C-m

tmux split-window -h
tmux send-keys -t $SESSION:2 'cd build/' C-m
tmux select-pane -t 0

tmux select-window -t $SESSION:1

tmux attach -t $SESSION

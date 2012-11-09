#!/system/bin/sh

event_player()
{
  for item in $@; do
    input_event_player a810 $item
    sleep 3
  done
}

event_player 2010-01-24-10-59-40
event_player 2010-01-24-10-57-34

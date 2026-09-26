#!/bin/sh
# Убираем стенд
NS=wsc
ip netns delete $NS
ip link delete veth-h
echo "Стенд убран."
